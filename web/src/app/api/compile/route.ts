import { NextRequest, NextResponse } from "next/server";
import { execFile } from "child_process";
import { mkdir, readFile, unlink, writeFile } from "fs/promises";
import { join } from "path";
import { randomUUID } from "crypto";
import { resolvePipelinePathCached } from "@/lib/compilerPath";

export const runtime = "nodejs";
export const dynamic = "force-dynamic";
export const maxDuration = 30;

type DiagnosticKind = "lexical" | "syntax" | "semantic" | "runtime" | "unknown";

type CompilerDiagnostic = {
  kind: DiagnosticKind;
  message: string;
  line?: number;
  raw: string;
  humanMessage?: string;
  fixHint?: string;
};

type PipelineResult = {
  stdout: string;
  stderr: string;
  exitCode: number;
};

function getLineText(code: string, line?: number): string {
  if (!line || line < 1) {
    return "";
  }

  const lines = code.split(/\r?\n/);
  return lines[line - 1] ?? "";
}

function looksLikeMissingTerminator(lineText: string): boolean {
  const trimmed = lineText.trim();
  if (!trimmed) {
    return false;
  }

  if (trimmed.endsWith(".") || trimmed.endsWith("{") || trimmed.endsWith("}")) {
    return false;
  }

  return /(count|flag|string|float|speak|alert|set|replay|if|otherwise|back|:=|->|\b(and|or|not)\b)/i.test(trimmed);
}

function enrichDiagnostics(diagnostics: CompilerDiagnostic[], code: string): CompilerDiagnostic[] {
  return diagnostics.map((diagnostic) => {
    const lineText = getLineText(code, diagnostic.line);

    if (diagnostic.kind === "syntax") {
      if (looksLikeMissingTerminator(lineText)) {
        return {
          ...diagnostic,
          humanMessage: `Syntax issue near line ${diagnostic.line ?? "?"}: this statement likely needs a semicolon terminator.`,
          fixHint: "Add a trailing ';' at the end of the statement.",
        };
      }

      return {
        ...diagnostic,
        humanMessage: `Syntax issue near line ${diagnostic.line ?? "?"}: the parser could not continue from this token sequence.`,
        fixHint: "Check bracket/parenthesis pairing and semicolon statement terminators (;).",
      };
    }

    if (diagnostic.kind === "semantic") {
      if (/variable\s+\w+\s+not declared/i.test(diagnostic.message)) {
        return {
          ...diagnostic,
          humanMessage: `Semantic issue on line ${diagnostic.line ?? "?"}: you are using a variable before declaration.`,
          fixHint: "Declare it first with a declaration statement, for example: count x;",
        };
      }

      if (/array\s+\w+\s+not declared/i.test(diagnostic.message)) {
        return {
          ...diagnostic,
          humanMessage: `Semantic issue on line ${diagnostic.line ?? "?"}: array usage found before declaration.`,
          fixHint: "Declare the array before use, for example: count arr[5];",
        };
      }

      if (/already declared/i.test(diagnostic.message)) {
        return {
          ...diagnostic,
          humanMessage: `Semantic issue on line ${diagnostic.line ?? "?"}: duplicate declaration detected.`,
          fixHint: "Rename one declaration or remove the duplicate definition.",
        };
      }

      return {
        ...diagnostic,
        humanMessage: `Semantic issue on line ${diagnostic.line ?? "?"}: ${diagnostic.message}.`,
        fixHint: "Review declarations and type/identifier usage in this scope.",
      };
    }

    if (diagnostic.kind === "lexical") {
      return {
        ...diagnostic,
        humanMessage: `Lexical issue on line ${diagnostic.line ?? "?"}: unknown or invalid character sequence encountered.`,
        fixHint: "Use valid EmotionScript tokens and remove unsupported characters.",
      };
    }

    return {
      ...diagnostic,
      humanMessage: diagnostic.message,
      fixHint: "Review the highlighted line and adjust the statement structure.",
    };
  });
}

function buildUserMessage(diagnostics: CompilerDiagnostic[]): string {
  if (diagnostics.length === 0) {
    return "Compilation failed. Please review the diagnostics below.";
  }

  const first = diagnostics[0];
  const lineText = first.line ? ` on line ${first.line}` : "";
  const primary = `${first.humanMessage ?? `Compilation issue${lineText}: ${first.message}`}`;
  const hint = first.fixHint ? ` Suggested fix: ${first.fixHint}` : "";
  return `${primary}${hint}`;
}

function parseCompilerDiagnosticLine(line: string): CompilerDiagnostic | null {
  const trimmed = line.trim();
  if (!trimmed) {
    return null;
  }

  const normalized = trimmed.replace(/^[✓✔✗✘xX]\s+/u, "");

  // Success banners from the compiler should never be treated as errors.
  if (/^✓\s+/u.test(trimmed) || /no\s+syntax\s+errors\s+found/i.test(normalized)) {
    return null;
  }

  const lexical = normalized.match(/^LEXICAL ERROR at line (\d+):\s*(.+)$/i);
  if (lexical) {
    return {
      kind: "lexical",
      line: Number.parseInt(lexical[1], 10),
      message: lexical[2],
      raw: trimmed,
    };
  }

  const syntax = normalized.match(/^SYNTAX ERROR at line (\d+):\s*(.+)$/i);
  if (syntax) {
    return {
      kind: "syntax",
      line: Number.parseInt(syntax[1], 10),
      message: syntax[2],
      raw: trimmed,
    };
  }

  const syntaxAlt = normalized.match(/^Syntax error(?: at line (\d+))?:\s*(.+)$/i);
  if (syntaxAlt) {
    return {
      kind: "syntax",
      line: syntaxAlt[1] ? Number.parseInt(syntaxAlt[1], 10) : undefined,
      message: syntaxAlt[2],
      raw: trimmed,
    };
  }

  const semantic = normalized.match(/^Semantic Error(?: at line (\d+))?:\s*(.*)$/i);
  if (semantic) {
    return {
      kind: "semantic",
      line: semantic[1] ? Number.parseInt(semantic[1], 10) : undefined,
      message: semantic[2]?.trim() || "Semantic validation failed.",
      raw: trimmed,
    };
  }

  const runtime = normalized.match(/^RUNTIME ERROR(?: at line (\d+))?:\s*(.+)$/i);
  if (runtime) {
    return {
      kind: "runtime",
      line: runtime[1] ? Number.parseInt(runtime[1], 10) : undefined,
      message: runtime[2],
      raw: trimmed,
    };
  }

  if (/^Cannot\b/i.test(normalized)) {
    return {
      kind: "semantic",
      message: normalized,
      raw: trimmed,
    };
  }

  return null;
}

function parseSpeakOutput(outputText: string): string {
  const lines = outputText.split(/\r?\n/);
  return lines
    .filter((line) => /^\[(SPEAK|ALERT)\]/.test(line.trim()))
    .join("\n")
    .trim();
}

function extractTokensFromSyntax(syntaxText: string): string {
  const lines = syntaxText.split(/\r?\n/);
  const tokenLines = lines.filter((line) => /token|lex|lexer|parser/i.test(line));
  return tokenLines.join("\n").trim();
}

async function readTextIfExists(filePath: string): Promise<string> {
  try {
    return await readFile(filePath, "utf-8");
  } catch {
    return "";
  }
}

async function runPipeline(inputPath: string, outputRoot: string, rebuild: boolean): Promise<PipelineResult> {
  const repoRoot = join(process.cwd(), "..");
  const scriptPath = await resolvePipelinePathCached();
  if (!scriptPath) {
    throw new Error("Pipeline script not found. Expected run-pipeline.ps1 at the repository root.");
  }

  const shell = process.platform === "win32" ? "powershell.exe" : "pwsh";
  const args = process.platform === "win32"
    ? ["-NoProfile", "-ExecutionPolicy", "Bypass", "-File", scriptPath, "-InputFile", inputPath, "-OutputDir", outputRoot]
    : ["-NoProfile", "-File", scriptPath, "-InputFile", inputPath, "-OutputDir", outputRoot];

  if (rebuild) {
    args.push("-Rebuild");
  }

  return await new Promise<PipelineResult>((resolve, reject) => {
    execFile(
      shell,
      args,
      { cwd: repoRoot, timeout: 20000, maxBuffer: 1024 * 1024 },
      (error, stdout, stderr) => {
        const result: PipelineResult = {
          stdout: stdout ?? "",
          stderr: stderr ?? "",
          exitCode: error && typeof (error as NodeJS.ErrnoException).code === "number"
            ? (error as NodeJS.ErrnoException).code as number
            : 0,
        };

        if (error && !stdout && !stderr) {
          reject(error);
          return;
        }

        resolve(result);
      }
    );
  });
}

function parseCompilerDiagnostics(outputText: string): CompilerDiagnostic[] {
  const diagnostics: CompilerDiagnostic[] = [];
  const seen = new Set<string>();

  for (const line of outputText.split("\n")) {
    const parsed = parseCompilerDiagnosticLine(line);
    if (!parsed) {
      continue;
    }

    const key = `${parsed.kind}|${parsed.line ?? 0}|${parsed.message}`;
    if (seen.has(key)) {
      continue;
    }

    seen.add(key);
    diagnostics.push(parsed);
  }

  return diagnostics;
}

export async function POST(request: NextRequest) {
  let tempFile = "";

  try {
    const { code, rebuild } = await request.json();

    if (!code || typeof code !== "string") {
      return NextResponse.json({ error: "No code provided" }, { status: 400 });
    }

    const pipelinePath = await resolvePipelinePathCached();
    if (!pipelinePath) {
      return NextResponse.json(
        {
          error:
            "Pipeline script is missing. Expected run-pipeline.ps1 at repository root.",
        },
        { status: 500 },
      );
    }

    const tempDir = join(process.cwd(), "..", "pipeline_input");
    await mkdir(tempDir, { recursive: true });

    const fileStem = randomUUID();
    tempFile = join(tempDir, `${fileStem}.ems`);
    await writeFile(tempFile, code, "utf-8");

    const outputRoot = join(process.cwd(), "..", "pipeline_output", "web_api");
    await mkdir(outputRoot, { recursive: true });

    const pipelineResult = await runPipeline(tempFile, outputRoot, Boolean(rebuild));

    const artifactFolder = join(outputRoot, "pipeline_input");
    const syntaxPath = join(artifactFolder, `${fileStem}.syntax.txt`);
    const tacPath = join(artifactFolder, `${fileStem}.tac`);
    const executionPath = join(artifactFolder, `${fileStem}.execution.txt`);
    const outputPath = join(artifactFolder, `${fileStem}.output.txt`);

    const [syntaxText, tacText, executionText, outputText] = await Promise.all([
      readTextIfExists(syntaxPath),
      readTextIfExists(tacPath),
      readTextIfExists(executionPath),
      readTextIfExists(outputPath),
    ]);

    const stdoutText = pipelineResult.stdout.trim();
    const stderrText = pipelineResult.stderr.trim();

    let output = parseSpeakOutput(outputText || executionText);
    const ir = tacText.trim();
    const tokens = extractTokensFromSyntax(syntaxText);
    let error = "";
    const combinedLogs = [stdoutText, stderrText, syntaxText].filter(Boolean).join("\n");

    const parsedDiagnostics = parseCompilerDiagnostics(combinedLogs);
    const diagnostics = enrichDiagnostics(parsedDiagnostics, code);
    const diagnosticText = diagnostics
      .map((entry) => (entry.line ? `line ${entry.line}: ${entry.message}` : entry.message))
      .join("\n")
      .trim();

    if (pipelineResult.exitCode !== 0 || diagnostics.length > 0 || stderrText) {
      error = diagnosticText || stderrText || "Compilation failed";
      const userMessage = buildUserMessage(diagnostics);
      return NextResponse.json({
        output,
        tokens,
        ir,
        stdout: [stdoutText, executionText].filter(Boolean).join("\n").trim(),
        stderr: stderrText,
        error,
        userMessage,
        diagnostics,
      }, { status: 422 });
    }

    if (!output && executionText) {
      output = parseSpeakOutput(executionText);
    }

    return NextResponse.json({
      output,
      tokens,
      ir,
      stdout: [stdoutText, executionText].filter(Boolean).join("\n").trim(),
      stderr: stderrText,
      diagnostics: [],
    });
  } catch (err) {
    const message = err instanceof Error ? err.message : "Compilation failed";
    return NextResponse.json({ error: message }, { status: 500 });
  } finally {
    if (tempFile) {
      unlink(tempFile).catch(() => {});
    }
  }
}

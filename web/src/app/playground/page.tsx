"use client";

import { type CSSProperties, useCallback, useEffect, useMemo, useRef, useState } from "react";
import { CodeEditorPanel } from "@/components/playground/CodeEditorPanel";
import { EditorDivider } from "@/components/playground/EditorDivider";
import Navbar from "@/components/Navbar";
import { OutputPanel } from "@/components/playground/OutputPanel";
import { PlaygroundSidebar } from "@/components/playground/PlaygroundSidebar";
import { PlaygroundToolbar } from "@/components/playground/PlaygroundToolbar";
import { StatusBar } from "@/components/playground/StatusBar";

type PlaygroundTab = "output" | "tokens" | "ir" | "errors";

type PlaygroundDiagnostic = {
  kind: "lexical" | "syntax" | "semantic" | "runtime" | "unknown";
  message: string;
  line?: number;
  raw: string;
  humanMessage?: string;
  fixHint?: string;
};

type CompileResponse = {
  output?: string;
  tokens?: string;
  ir?: string;
  error?: string;
  stdout?: string;
  stderr?: string;
  userMessage?: string;
  diagnostics?: PlaygroundDiagnostic[];
};

type HealthResponse = {
  compilerReady: boolean;
  message?: string;
  compilerPath?: string;
};

type ExampleListItem = {
  id: string;
  path: string;
  name: string;
  category: string;
};

type ExampleListResponse = {
  examples?: ExampleListItem[];
  error?: string;
};

type ExampleContentResponse = {
  path?: string;
  content?: string;
  error?: string;
};

const starterCode = `mind EmotionStarter
awake

speak("Welcome to EmotionScript")

sleep`;

export default function PlaygroundPage() {
  const [activeTab, setActiveTab] = useState<PlaygroundTab>("output");
  const [selectedExample, setSelectedExample] = useState("");
  const codeRef = useRef(starterCode);
  const [editorExternalCode, setEditorExternalCode] = useState(starterCode);
  const [exampleItems, setExampleItems] = useState<ExampleListItem[]>([]);
  const [examplesLoading, setExamplesLoading] = useState(true);
  const [exampleLoading, setExampleLoading] = useState(false);
  const [examplesError, setExamplesError] = useState("");
  const [split, setSplit] = useState(60);
  const [running, setRunning] = useState(false);
  const [output, setOutput] = useState("");
  const [tokens, setTokens] = useState("");
  const [ir, setIr] = useState("");
  const [error, setError] = useState("");
  const [stdout, setStdout] = useState("");
  const [stderr, setStderr] = useState("");
  const [userMessage, setUserMessage] = useState("");
  const [diagnostics, setDiagnostics] = useState<PlaygroundDiagnostic[]>([]);
  const [jumpToLine, setJumpToLine] = useState<number | null>(null);
  const [line, setLine] = useState(1);
  const [column, setColumn] = useState(1);
  const [lastRunMs, setLastRunMs] = useState<number | null>(null);
  const [compilerReady, setCompilerReady] = useState<boolean | null>(null);
  const [compilerStatusText, setCompilerStatusText] = useState("Checking compiler...");
  const [editorExternalVersion, setEditorExternalVersion] = useState(0);
  const splitContainerRef = useRef<HTMLDivElement>(null);
  const cursorRafRef = useRef<number | null>(null);
  const latestCursorRef = useRef<{ line: number; column: number }>({ line: 1, column: 1 });
  const activeExampleRequestIdRef = useRef(0);

  const exampleOptions = useMemo(
    () => exampleItems.map((item) => ({ key: item.path, label: item.name, group: item.category })),
    [exampleItems],
  );

  const loadExampleContent = useCallback(async (examplePath: string) => {
    const requestId = activeExampleRequestIdRef.current + 1;
    activeExampleRequestIdRef.current = requestId;
    setExampleLoading(true);
    setExamplesError("");

    try {
      const response = await fetch(`/api/examples?file=${encodeURIComponent(examplePath)}`, { cache: "no-store" });
      const data = (await response.json()) as ExampleContentResponse;

      if (!response.ok || typeof data.content !== "string") {
        throw new Error(data.error ?? "Failed to load selected example.");
      }

      if (activeExampleRequestIdRef.current !== requestId) {
        return;
      }

      setSelectedExample(examplePath);
      codeRef.current = data.content;
      setEditorExternalCode(data.content);
      setEditorExternalVersion((value) => value + 1);
      setActiveTab("output");
    } catch (loadError) {
      if (activeExampleRequestIdRef.current !== requestId) {
        return;
      }

      const message = loadError instanceof Error ? loadError.message : "Failed to load selected example.";
      setExamplesError(message);
    } finally {
      if (activeExampleRequestIdRef.current === requestId) {
        setExampleLoading(false);
      }
    }
  }, []);

  const refreshExamples = useCallback(async () => {
    setExamplesLoading(true);

    try {
      const response = await fetch("/api/examples", { cache: "no-store" });
      const data = (await response.json()) as ExampleListResponse;

      if (!response.ok) {
        throw new Error(data.error ?? "Unable to load examples.");
      }

      const items = (data.examples ?? []).slice();
      setExampleItems(items);

      if (items.length === 0) {
        setExamplesError("No .ems example files were found in examples/.");
        setSelectedExample("");
        return;
      }

      const preferred = items.find((item) => item.path.toLowerCase() === "loops/for_basic.ems") ?? items[0];
      await loadExampleContent(preferred.path);
    } catch (loadError) {
      const message = loadError instanceof Error ? loadError.message : "Unable to load examples.";
      setExamplesError(message);
      setExampleItems([]);
      setSelectedExample("");
    } finally {
      setExamplesLoading(false);
    }
  }, [loadExampleContent]);

  const refreshCompilerHealth = useCallback(async () => {
    try {
      const response = await fetch("/api/health", { cache: "no-store" });
      const data = (await response.json()) as HealthResponse;
      const ready = response.ok && data.compilerReady;

      setCompilerReady(ready);
      if (ready) {
        setCompilerStatusText(`Compiler online (${data.compilerPath?.split(/[\\/]/).pop() ?? "binary"})`);
      } else {
        setCompilerStatusText(data.message ?? "Compiler offline");
      }
    } catch {
      setCompilerReady(false);
      setCompilerStatusText("Health check failed");
    }
  }, []);

  useEffect(() => {
    refreshCompilerHealth();
  }, [refreshCompilerHealth]);

  useEffect(() => {
    void refreshExamples();
  }, [refreshExamples]);

  const handleRun = useCallback(async () => {
    if (compilerReady === false) {
      setError(`Compiler unavailable: ${compilerStatusText}`);
      setActiveTab("errors");
      return;
    }

    setRunning(true);
    setError("");
    setActiveTab("output");
    const startedAt = performance.now();

    try {
      const response = await fetch("/api/compile", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code: codeRef.current }),
      });

      const data = (await response.json()) as CompileResponse;

      setOutput(data.output ?? "");
      setTokens(data.tokens ?? "");
      setIr(data.ir ?? "");
      setStdout(data.stdout ?? "");
      setStderr(data.stderr ?? "");
      setUserMessage(data.userMessage ?? "");
      setDiagnostics(data.diagnostics ?? []);

      if (!response.ok || data.error) {
        setError(data.error ?? "Compilation failed");
        setActiveTab("errors");
      } else {
        setError("");
        setActiveTab("output");
      }
    } catch (compileError) {
      const message = compileError instanceof Error ? compileError.message : "Compilation failed";
      setError(message);
      setUserMessage("The compile request failed before diagnostics were produced. Please retry.");
      setDiagnostics([]);
      setActiveTab("errors");
    } finally {
      setRunning(false);
      setLastRunMs(Math.round(performance.now() - startedAt));
    }
  }, [compilerReady, compilerStatusText]);

  useEffect(() => {
    const onKeyDown = (event: KeyboardEvent) => {
      if ((event.ctrlKey || event.metaKey) && event.key === "Enter") {
        event.preventDefault();
        if (!running) {
          handleRun();
        }
      }
    };

    window.addEventListener("keydown", onKeyDown);
    return () => window.removeEventListener("keydown", onKeyDown);
  }, [handleRun, running]);

  const handleCodeChange = useCallback((nextCode: string) => {
    codeRef.current = nextCode;
  }, []);

  const handleClear = useCallback(() => {
    const cleared = "";
    codeRef.current = cleared;
    setEditorExternalCode(cleared);
    setEditorExternalVersion((value) => value + 1);
    setOutput("");
    setTokens("");
    setIr("");
    setError("");
    setStdout("");
    setStderr("");
    setUserMessage("");
    setDiagnostics([]);
    setJumpToLine(null);
    setLastRunMs(null);
    setActiveTab("output");
  }, []);

  const handleSelectExample = useCallback((key: string) => {
    if (!key || exampleLoading) {
      return;
    }

    void loadExampleContent(key);
  }, [exampleLoading, loadExampleContent]);

  const handleCursorChange = useCallback((nextLine: number, nextColumn: number) => {
    latestCursorRef.current = { line: nextLine, column: nextColumn };

    if (cursorRafRef.current !== null) {
      return;
    }

    cursorRafRef.current = requestAnimationFrame(() => {
      cursorRafRef.current = null;
      const { line: currentLine, column: currentColumn } = latestCursorRef.current;
      setLine((previous) => (previous === currentLine ? previous : currentLine));
      setColumn((previous) => (previous === currentColumn ? previous : currentColumn));
    });
  }, []);

  useEffect(() => {
    return () => {
      if (cursorRafRef.current !== null) {
        cancelAnimationFrame(cursorRafRef.current);
      }
    };
  }, []);

  const handleDividerMouseDown = useCallback((event: React.MouseEvent<HTMLButtonElement>) => {
    event.preventDefault();

    const onMouseMove = (moveEvent: MouseEvent) => {
      const host = splitContainerRef.current;
      if (!host) {
        return;
      }

      const rect = host.getBoundingClientRect();
      const nextSplit = ((moveEvent.clientX - rect.left) / rect.width) * 100;
      setSplit(Math.min(78, Math.max(32, nextSplit)));
    };

    const onMouseUp = () => {
      window.removeEventListener("mousemove", onMouseMove);
      window.removeEventListener("mouseup", onMouseUp);
    };

    window.addEventListener("mousemove", onMouseMove);
    window.addEventListener("mouseup", onMouseUp);
  }, []);

  const handleJumpHandled = useCallback(() => {
    setJumpToLine(null);
  }, []);

  const handleJumpToLine = useCallback((lineNumber: number) => {
    setJumpToLine(lineNumber);
  }, []);

  return (
    <div className="relative flex min-h-screen flex-col overflow-hidden bg-[#10131e] text-[#f6f6f8]">
      <div className="pointer-events-none absolute inset-0">
        <div className="absolute -left-24 top-16 h-72 w-72 rounded-full bg-[#5166ff]/20 blur-3xl" />
        <div className="absolute -right-24 -top-8 h-80 w-80 rounded-full bg-[#4dc5ff]/16 blur-3xl" />
        <div className="absolute bottom-12 left-1/3 h-56 w-56 rounded-full bg-[#1f2f68]/24 blur-3xl" />
      </div>

      <Navbar />
      <PlaygroundToolbar
        onRun={handleRun}
        onClear={handleClear}
        running={running}
        selectedExample={selectedExample}
        examples={exampleOptions}
        onSelectExample={handleSelectExample}
        examplesLoading={examplesLoading || exampleLoading}
        examplesError={examplesError}
        compilerReady={compilerReady}
        compilerStatusText={compilerStatusText}
        onRefreshCompilerHealth={refreshCompilerHealth}
      />

      <main className="relative z-10 flex flex-1 overflow-hidden" aria-label="EmotionScript playground workspace">
        <PlaygroundSidebar />

        <div
          ref={splitContainerRef}
          className="flex flex-1 flex-col overflow-hidden md:flex-row"
          style={{ "--editor-split": `${split}%`, "--output-split": `${100 - split}%` } as CSSProperties}
        >
          <div className="w-full md:h-full md:basis-(--editor-split)">
            <CodeEditorPanel
              externalCode={editorExternalCode}
              externalCodeVersion={editorExternalVersion}
              onChange={handleCodeChange}
              onCursorChange={handleCursorChange}
              diagnostics={diagnostics}
              jumpToLine={jumpToLine}
              onJumpHandled={handleJumpHandled}
            />
          </div>

          <EditorDivider onMouseDown={handleDividerMouseDown} />

          <div className="min-h-72 w-full flex-1 md:h-full md:min-h-0 md:basis-(--output-split)">
            <OutputPanel
              activeTab={activeTab}
              onChangeTab={setActiveTab}
              running={running}
              output={output}
              tokens={tokens}
              ir={ir}
              error={error}
              stdout={stdout}
              stderr={stderr}
              userMessage={userMessage}
              diagnostics={diagnostics}
              onJumpToLine={handleJumpToLine}
            />
          </div>
        </div>
      </main>

      <StatusBar running={running} line={line} column={column} lastRunMs={lastRunMs} compilerReady={compilerReady} />
    </div>
  );
}

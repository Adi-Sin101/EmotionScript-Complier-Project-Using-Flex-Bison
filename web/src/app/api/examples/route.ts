import { NextRequest, NextResponse } from "next/server";
import { readdir, readFile } from "fs/promises";
import { extname, join, relative, resolve, sep } from "path";

export const runtime = "nodejs";
export const dynamic = "force-dynamic";

type ExampleListItem = {
  id: string;
  path: string;
  name: string;
  category: string;
};

const repoRoot = resolve(process.cwd(), "..");
const examplesRoot = resolve(repoRoot, "examples");

function normalizeToPosix(input: string): string {
  return input.replaceAll("\\", "/");
}

function toTitleCase(input: string): string {
  return input
    .split(/[\s_-]+/)
    .filter(Boolean)
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1).toLowerCase())
    .join(" ");
}

async function collectExampleFiles(directory: string): Promise<string[]> {
  const entries = await readdir(directory, { withFileTypes: true });
  const files: string[] = [];

  for (const entry of entries) {
    const fullPath = join(directory, entry.name);
    if (entry.isDirectory()) {
      const nested = await collectExampleFiles(fullPath);
      files.push(...nested);
      continue;
    }

    if (!entry.isFile()) {
      continue;
    }

    if (extname(entry.name).toLowerCase() !== ".ems") {
      continue;
    }

    const rel = normalizeToPosix(relative(examplesRoot, fullPath));
    files.push(rel);
  }

  return files;
}

function ensureSafeExamplePath(requestedPath: string): string | null {
  const normalized = normalizeToPosix(requestedPath.trim());
  if (!normalized || normalized.includes("\0")) {
    return null;
  }

  if (normalized.startsWith("/") || /^[A-Za-z]:\//.test(normalized)) {
    return null;
  }

  const segments = normalized.split("/");
  if (segments.some((segment) => segment === "..")) {
    return null;
  }

  if (!normalized.toLowerCase().endsWith(".ems")) {
    return null;
  }

  const resolved = resolve(examplesRoot, normalized);
  const withinRoot = resolved === examplesRoot || resolved.startsWith(`${examplesRoot}${sep}`);
  if (!withinRoot) {
    return null;
  }

  return resolved;
}

function buildExampleItem(relativePath: string): ExampleListItem {
  const parts = relativePath.split("/");
  const filename = parts[parts.length - 1];
  const stem = filename.replace(/\.ems$/i, "");
  const folderParts = parts.slice(0, -1);

  return {
    id: relativePath,
    path: relativePath,
    name: toTitleCase(stem),
    category: folderParts.length > 0 ? folderParts.map((part) => toTitleCase(part)).join(" / ") : "General",
  };
}

export async function GET(request: NextRequest) {
  const requestedFile = request.nextUrl.searchParams.get("file");

  if (requestedFile) {
    const safePath = ensureSafeExamplePath(requestedFile);
    if (!safePath) {
      return NextResponse.json({ error: "Invalid example path." }, { status: 400 });
    }

    try {
      const content = await readFile(safePath, "utf-8");
      const relPath = normalizeToPosix(relative(examplesRoot, safePath));
      return NextResponse.json({ path: relPath, content });
    } catch {
      return NextResponse.json({ error: "Example file not found." }, { status: 404 });
    }
  }

  try {
    const files = await collectExampleFiles(examplesRoot);
    files.sort((a, b) => a.localeCompare(b));
    const examples = files.map((file) => buildExampleItem(file));
    return NextResponse.json({ examples });
  } catch {
    return NextResponse.json(
      { error: "Unable to load examples directory. Ensure examples/ exists at repository root." },
      { status: 500 },
    );
  }
}

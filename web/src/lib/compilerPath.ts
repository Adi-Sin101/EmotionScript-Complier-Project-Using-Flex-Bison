import { access } from "fs/promises";
import { join } from "path";

export const EMOTIONSCRIPT_COMPILER_CANDIDATES = [
  process.env.EMOTIONSCRIPT_COMPILER_PATH,
  join(process.cwd(), "..", "backend", "compiler", "emotionscript.exe"),
  join(process.cwd(), "..", "backend", "compiler", "emotionscript"),
].filter((candidate): candidate is string => Boolean(candidate));

export const PIPELINE_SCRIPT_CANDIDATES = [
  process.env.EMOTIONSCRIPT_PIPELINE_PATH,
  join(process.cwd(), "..", "run-pipeline.ps1"),
].filter((candidate): candidate is string => Boolean(candidate));

export const COMPILER_ERROR_PATTERN = /(LEXICAL ERROR|SYNTAX ERROR|Syntax error|Semantic Error|RUNTIME ERROR|^Error:)/im;

let cachedCompilerPath: string | null = null;
let cachedPipelinePath: string | null = null;
let cacheExpiryMs = 0;
const CACHE_TTL_MS = 60_000;

export async function resolveCompilerPath(): Promise<string | null> {
  for (const candidate of EMOTIONSCRIPT_COMPILER_CANDIDATES) {
    try {
      await access(candidate);
      return candidate;
    } catch {
      // Try the next compiler candidate.
    }
  }

  return null;
}

export async function resolvePipelinePath(): Promise<string | null> {
  for (const candidate of PIPELINE_SCRIPT_CANDIDATES) {
    try {
      await access(candidate);
      return candidate;
    } catch {
      // Try the next pipeline script candidate.
    }
  }

  return null;
}

export async function resolveCompilerPathCached(): Promise<string | null> {
  const now = Date.now();
  if (cachedCompilerPath && now < cacheExpiryMs) {
    return cachedCompilerPath;
  }

  const compilerPath = await resolveCompilerPath();
  cachedCompilerPath = compilerPath;
  cacheExpiryMs = now + CACHE_TTL_MS;
  return compilerPath;
}

export async function resolvePipelinePathCached(): Promise<string | null> {
  const now = Date.now();
  if (cachedPipelinePath && now < cacheExpiryMs) {
    return cachedPipelinePath;
  }

  const pipelinePath = await resolvePipelinePath();
  cachedPipelinePath = pipelinePath;
  cacheExpiryMs = now + CACHE_TTL_MS;
  return pipelinePath;
}
import { stat } from "fs/promises";
import { NextResponse } from "next/server";
import {
  EMOTIONSCRIPT_COMPILER_CANDIDATES,
  PIPELINE_SCRIPT_CANDIDATES,
  resolveCompilerPathCached,
  resolvePipelinePathCached,
} from "@/lib/compilerPath";

export const runtime = "nodejs";
export const dynamic = "force-dynamic";
export const maxDuration = 5;

export async function GET() {
  const checkedAt = new Date().toISOString();
  const [compilerPath, pipelinePath] = await Promise.all([
    resolveCompilerPathCached(),
    resolvePipelinePathCached(),
  ]);

  if (!compilerPath || !pipelinePath) {
    return NextResponse.json(
      {
        ok: false,
        compilerReady: Boolean(compilerPath),
        pipelineReady: Boolean(pipelinePath),
        checkedAt,
        message: "EmotionScript backend is not fully ready. Ensure backend/compiler/emotionscript(.exe) and run-pipeline.ps1 are present.",
        compilerCandidates: EMOTIONSCRIPT_COMPILER_CANDIDATES,
        pipelineCandidates: PIPELINE_SCRIPT_CANDIDATES,
      },
      { status: 503 },
    );
  }

  const [compilerDetails, pipelineDetails] = await Promise.all([
    stat(compilerPath),
    stat(pipelinePath),
  ]);

  return NextResponse.json({
    ok: true,
    compilerReady: true,
    pipelineReady: true,
    checkedAt,
    compilerPath,
    pipelinePath,
    compilerCandidates: EMOTIONSCRIPT_COMPILER_CANDIDATES,
    pipelineCandidates: PIPELINE_SCRIPT_CANDIDATES,
    compilerModifiedAt: compilerDetails.mtime.toISOString(),
    compilerSizeBytes: compilerDetails.size,
    pipelineModifiedAt: pipelineDetails.mtime.toISOString(),
    pipelineSizeBytes: pipelineDetails.size,
  });
}

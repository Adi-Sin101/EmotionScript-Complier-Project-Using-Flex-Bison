import { memo } from "react";

type PlaygroundTab = "output" | "tokens" | "ir" | "errors";

type OutputDiagnostic = {
  kind: "lexical" | "syntax" | "semantic" | "runtime" | "unknown";
  message: string;
  line?: number;
  humanMessage?: string;
  fixHint?: string;
};

type OutputPanelProps = {
  activeTab: PlaygroundTab;
  onChangeTab: (tab: PlaygroundTab) => void;
  running: boolean;
  output: string;
  tokens: string;
  ir: string;
  error: string;
  stdout: string;
  stderr: string;
  userMessage: string;
  diagnostics: OutputDiagnostic[];
  onJumpToLine: (line: number) => void;
};

const tabs: { key: PlaygroundTab; label: string }[] = [
  { key: "output", label: "Output" },
  { key: "tokens", label: "Tokens" },
  { key: "ir", label: "Intermediate Code" },
  { key: "errors", label: "Errors" },
];

function countNonEmptyLines(text: string): number {
  return text.split("\n").filter((line) => line.trim().length > 0).length;
}

function countTacInstructions(ir: string): number {
  return ir.split("\n").filter((line) => /^\s*\d+:/.test(line)).length;
}

function getTabText(props: OutputPanelProps): string {
  if (props.running) {
    return "Compiling mission...";
  }

  if (props.activeTab === "output") {
    if (props.output.trim()) {
      return props.output;
    }

    if (!props.error && !props.stderr) {
      if ((props.stdout || "").includes("<no execution trace found>")) {
        return "Program compiled successfully, but this example produced no runtime execution trace in the current VM.";
      }

      return "Program compiled successfully, but no [SPEAK]/[ALERT] output was produced.";
    }

    return "Run your EmotionScript program to view runtime output.";
  }

  if (props.activeTab === "tokens") {
    return (
      props.tokens ||
      "Token stream is not emitted by this compiler build. Check the raw compiler stream:\n\n" +
        (props.stdout || "No stdout available.")
    );
  }

  if (props.activeTab === "ir") {
    return props.ir || "No intermediate code was generated.";
  }

  return props.error || props.stderr || "No errors.";
}

function OutputPanelComponent(props: OutputPanelProps) {
  const bodyText = getTabText(props);
  const outputLines = countNonEmptyLines(props.output);
  const tokenLines = countNonEmptyLines(props.tokens);
  const instructionCount = countTacInstructions(props.ir);
  const hasErrors = Boolean(props.error || props.stderr);

  return (
    <section className="flex h-full min-h-80 flex-col border-t border-white/10 bg-[#0f1528]/90 md:border-t-0 md:border-l md:border-white/10" aria-label="Compiler output panel">
      <div className="flex h-10 items-center gap-1 border-b border-white/10 px-3 text-[11px] uppercase tracking-[0.08em] text-[#7f86ac] md:px-4">
        {tabs.map((tab) => (
          <button
            key={tab.key}
            type="button"
            onClick={() => props.onChangeTab(tab.key)}
            aria-label={`Open ${tab.label} tab`}
            className={`h-full border-b px-2 transition-colors md:px-3 ${
              props.activeTab === tab.key
                ? "border-[#2f3ce8] text-[#9aa5ff]"
                : "border-transparent hover:text-[#cad0ee]"
            }`}
          >
            {tab.label}
          </button>
        ))}
      </div>

      <div className="flex-1 overflow-auto px-3 py-3 font-mono text-sm leading-7 text-[#d0d5ef] sm:px-4">
        <div className="mb-4 grid gap-2 text-[11px] uppercase tracking-[0.08em] text-[#8e97bf] sm:grid-cols-4">
          <div className="rounded-md border border-white/10 bg-white/4 px-2 py-1.5">Output Lines: {outputLines}</div>
          <div className="rounded-md border border-white/10 bg-white/4 px-2 py-1.5">Token Lines: {tokenLines}</div>
          <div className="rounded-md border border-white/10 bg-white/4 px-2 py-1.5">IR Instructions: {instructionCount}</div>
          <div className={`rounded-md border px-2 py-1.5 ${hasErrors ? "border-rose-300/30 bg-rose-400/10 text-rose-200" : "border-emerald-300/30 bg-emerald-400/10 text-emerald-200"}`}>
            Diagnostics: {hasErrors ? "Present" : "None"}
          </div>
        </div>

        {props.activeTab === "errors" && props.diagnostics.length > 0 ? (
          <div className="space-y-2">
            <div className="rounded-md border border-amber-300/25 bg-amber-400/10 px-3 py-2 text-xs leading-6 text-amber-100">
              {props.userMessage || "Compilation failed. Review the detailed errors below."}
            </div>
            {props.diagnostics.map((item, index) => (
              <button
                key={`${item.kind}-${item.line ?? 0}-${item.message}-${index}`}
                type="button"
                onClick={() => item.line && props.onJumpToLine(item.line)}
                className="block w-full rounded-md border border-rose-300/20 bg-rose-400/10 px-3 py-2 text-left text-xs leading-6 text-rose-100 transition-colors hover:bg-rose-400/15"
              >
                <span className="mr-2 rounded bg-rose-200/15 px-1.5 py-0.5 text-[10px] uppercase tracking-[0.08em] text-rose-200">
                  {item.kind}
                </span>
                <span className="font-semibold">{item.line ? `Line ${item.line}: ` : ""}{item.humanMessage ?? item.message}</span>
                {item.fixHint ? <span className="block text-rose-100/80">Suggested fix: {item.fixHint}</span> : null}
              </button>
            ))}
            <p className="text-[11px] text-[#aeb5d7]">Tip: click an error to jump to its line in the editor.</p>
          </div>
        ) : (
          <pre className="whitespace-pre-wrap wrap-break-word rounded-md border border-white/6 bg-[#121a30] p-3">{bodyText}</pre>
        )}

        <div className="mt-5 rounded-md border border-[#28398b]/60 bg-[#131a3b]/70 px-3 py-3 text-xs leading-6 text-[#b8c0ef]">
          <p>
            <span className="mr-2 inline-flex h-4 w-4 items-center justify-center rounded-full bg-[#2435a9] text-[10px] font-semibold text-[#d7defe]">
              i
            </span>
            {props.running
              ? "Compiler process is running. Results will appear here as soon as execution completes."
              : "Use the tabs to inspect runtime output, token stream, intermediate code, and parser errors."}
          </p>
        </div>
      </div>
    </section>
  );
}

export const OutputPanel = memo(OutputPanelComponent);

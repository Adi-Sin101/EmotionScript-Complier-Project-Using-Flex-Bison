type StatusBarProps = {
  running: boolean;
  line: number;
  column: number;
  lastRunMs: number | null;
  compilerReady: boolean | null;
};

export function StatusBar({ running, line, column, lastRunMs, compilerReady }: StatusBarProps) {
  return (
    <footer className="relative z-10 flex min-h-8 flex-wrap items-center justify-between gap-2 border-t border-white/10 bg-[#0e1426] px-3 py-1 text-[11px] uppercase tracking-[0.06em] text-[#7e86a7] md:px-4">
      <div className="flex flex-wrap items-center gap-3 font-mono">
        <span className="inline-flex items-center gap-1">
          <span className={`h-2 w-2 rounded-full ${running ? "bg-[#f4c97b]" : "bg-[#2adf78]"}`} />
          {running ? "Running" : "Engine Ready"}
        </span>
        <span>{lastRunMs === null ? "No recent run" : `${lastRunMs} ms`}</span>
        <span>
          {compilerReady === null ? "Compiler: checking" : compilerReady ? "Compiler: online" : "Compiler: offline"}
        </span>
      </div>
      <div className="flex items-center gap-4 font-mono">
        <span>{`Ln ${line}, Col ${column}`}</span>
        <span className="hidden sm:inline">UTF-8</span>
      </div>
    </footer>
  );
}

type PlaygroundToolbarProps = {
  onRun: () => void;
  onClear: () => void;
  running: boolean;
  selectedExample: string;
  examples: Array<{ key: string; label: string; group: string }>;
  onSelectExample: (key: string) => void;
  examplesLoading: boolean;
  examplesError?: string;
  compilerReady: boolean | null;
  compilerStatusText: string;
  onRefreshCompilerHealth: () => void;
};

export function PlaygroundToolbar({
  onRun,
  onClear,
  running,
  selectedExample,
  examples,
  onSelectExample,
  examplesLoading,
  examplesError,
  compilerReady,
  compilerStatusText,
  onRefreshCompilerHealth,
}: PlaygroundToolbarProps) {
  const statusClasses =
    compilerReady === null
      ? "border-white/15 bg-white/5 text-[#c7cbe3]"
      : compilerReady
        ? "border-emerald-300/25 bg-emerald-400/10 text-emerald-200"
        : "border-rose-300/25 bg-rose-400/10 text-rose-200";

  const groupedExamples = examples.reduce<Record<string, Array<{ key: string; label: string }>>>((acc, example) => {
    if (!acc[example.group]) {
      acc[example.group] = [];
    }

    acc[example.group].push({ key: example.key, label: example.label });
    return acc;
  }, {});

  const groupNames = Object.keys(groupedExamples).sort((a, b) => a.localeCompare(b));

  return (
    <div className="relative z-10 flex min-h-12 flex-wrap items-center justify-between gap-2 border-b border-white/10 bg-[#12172a]/88 px-3 py-2 text-sm md:px-4">
      <div className="flex w-full flex-wrap items-center gap-2 md:w-auto md:gap-3">
        <button
          type="button"
          onClick={onRun}
          aria-label="Run code"
          disabled={running}
          className="inline-flex items-center gap-2 rounded-md bg-[#4c61ff] px-4 py-2 font-semibold text-white shadow-[0_10px_24px_-14px_rgba(76,97,255,0.9)] hover:bg-[#6377ff] disabled:cursor-not-allowed disabled:opacity-70"
        >
          <svg width="12" height="12" viewBox="0 0 12 12" fill="none" aria-hidden="true">
            <path d="M3 2.5 9 6 3 9.5v-7Z" fill="currentColor" />
          </svg>
          {running ? "Running..." : "Run"}
        </button>

        <span className="hidden text-[11px] uppercase tracking-[0.08em] text-[#8f95b8] lg:inline">Ctrl+Enter</span>

        <button
          type="button"
          onClick={onClear}
          aria-label="Clear editor"
          className="inline-flex items-center gap-2 rounded-md px-3 py-1.5 text-[#bcc0db] transition-colors hover:bg-white/5 hover:text-white"
        >
          <svg width="12" height="12" viewBox="0 0 24 24" fill="none" aria-hidden="true">
            <path d="M4 6h16M10 11v6m4-6v6M8 6l1-2h6l1 2m-9 0 1 14h8l1-14" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" />
          </svg>
          Clear
        </button>

        <label className="inline-flex min-w-52 items-center gap-2 rounded-md px-3 py-1.5 text-[#bcc0db] transition-colors hover:bg-white/5 hover:text-white">
          <svg width="12" height="12" viewBox="0 0 24 24" fill="none" aria-hidden="true">
            <path d="M4 6h16M4 12h12M4 18h8" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" />
          </svg>
          <span className="hidden lg:inline">Example Program</span>
          <select
            aria-label="Select example program"
            value={selectedExample}
            onChange={(event) => onSelectExample(event.target.value)}
            disabled={examplesLoading || examples.length === 0}
            className="min-w-0 flex-1 rounded border border-white/15 bg-[#12142e] px-2 py-1 text-xs text-white outline-none disabled:cursor-not-allowed disabled:opacity-60"
          >
            {examplesLoading && <option value="">Loading examples...</option>}
            {!examplesLoading && examples.length === 0 && <option value="">No examples found</option>}
            {!examplesLoading && examples.length > 0 &&
              groupNames.map((groupName) => (
                <optgroup key={groupName} label={groupName}>
                  {groupedExamples[groupName].map((example) => (
                    <option key={example.key} value={example.key}>
                      {example.label}
                    </option>
                  ))}
                </optgroup>
              ))}
          </select>
        </label>
        {examplesError && (
          <span className="max-w-72 truncate text-[11px] text-rose-200" role="status" aria-live="polite" title={examplesError}>
            {examplesError}
          </span>
        )}
      </div>

      <div className="flex w-full items-center justify-between gap-2 md:w-auto md:justify-start">
        <div className={`inline-flex items-center gap-2 rounded-md border px-2.5 py-1 text-[11px] uppercase tracking-[0.08em] ${statusClasses}`}>
          <span className={`h-1.5 w-1.5 rounded-full ${compilerReady ? "bg-emerald-300" : compilerReady === false ? "bg-rose-300" : "bg-[#c7cbe3]"}`} />
          {compilerStatusText}
        </div>
        <button
          type="button"
          onClick={onRefreshCompilerHealth}
          className="rounded-md border border-white/15 bg-[#131531] px-2.5 py-1 text-[11px] uppercase tracking-[0.08em] text-[#a8adcb] transition-colors hover:text-white"
        >
          Refresh
        </button>
      </div>

      <div className="hidden items-center gap-2 xl:flex">
        <button
          type="button"
          aria-label="Target selector"
          className="rounded-md border border-white/15 bg-[#131531] px-3 py-1.5 text-xs text-[#a8adcb]"
        >
          # Target: TAC VM
        </button>
        <button
          type="button"
          aria-label="Share"
          className="flex h-8 w-8 items-center justify-center rounded-md text-[#a8adcb] transition-colors hover:bg-white/5 hover:text-white"
        >
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" aria-hidden="true">
            <circle cx="18" cy="5" r="2" stroke="currentColor" strokeWidth="1.6" />
            <circle cx="6" cy="12" r="2" stroke="currentColor" strokeWidth="1.6" />
            <circle cx="18" cy="19" r="2" stroke="currentColor" strokeWidth="1.6" />
            <path d="m8 12 8-6M8 12l8 6" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" />
          </svg>
        </button>
      </div>
    </div>
  );
}

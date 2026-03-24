const icons = [
  {
    key: "code",
    label: "Code",
    path: (
      <path d="m9 18 6-6-6-6M15 18l6-6-6-6" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" strokeLinejoin="round" />
    ),
  },
  {
    key: "files",
    label: "Files",
    path: (
      <path d="M3 7a2 2 0 0 1 2-2h4l2 2h8a2 2 0 0 1 2 2v8a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V7Z" stroke="currentColor" strokeWidth="1.6" strokeLinejoin="round" />
    ),
  },
  {
    key: "history",
    label: "History",
    path: (
      <>
        <path d="M3 12a9 9 0 1 0 3-6.7" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" />
        <path d="M3 4v3h3" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" strokeLinejoin="round" />
      </>
    ),
  },
];

export function PlaygroundSidebar() {
  return (
    <aside className="hidden w-12 shrink-0 flex-col items-center justify-between border-r border-white/10 bg-[#0f1426] py-3 md:flex">
      <div className="flex flex-col items-center gap-2">
        {icons.map((icon, index) => (
          <button
            key={icon.key}
            type="button"
            aria-label={icon.label}
            className={`flex h-8 w-8 items-center justify-center rounded-md transition-colors ${
              index === 0
                ? "bg-[#2c2ce2]/25 text-[#7e86ff]"
                : "text-[#7f86ac] hover:bg-white/5 hover:text-[#bec4e3]"
            }`}
          >
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" aria-hidden="true">
              {icon.path}
            </svg>
          </button>
        ))}
      </div>

      <button
        type="button"
        aria-label="Settings"
        className="flex h-8 w-8 items-center justify-center rounded-md text-[#7f86ac] transition-colors hover:bg-white/5 hover:text-[#bec4e3]"
      >
        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" aria-hidden="true">
          <path d="M12 3v2M12 19v2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M3 12h2M19 12h2M4.9 19.1l1.4-1.4M17.7 6.3l1.4-1.4" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" />
          <circle cx="12" cy="12" r="3" stroke="currentColor" strokeWidth="1.6" />
        </svg>
      </button>
    </aside>
  );
}

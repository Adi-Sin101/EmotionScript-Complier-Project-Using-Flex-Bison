import Link from "next/link";

export function PlaygroundNavbar() {
  return (
    <header className="relative z-10 flex h-14 items-center justify-between border-b border-white/10 bg-[#0f1020]/95 px-4 backdrop-blur md:px-6">
      <div className="flex items-center gap-3">
        <div className="flex h-7 w-7 items-center justify-center rounded-md bg-[#2c2ce2]/20 text-[#7d81ff]">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" aria-hidden="true">
            <path d="M7 14l-2 7 7-2 7-7-5-5-7 7Z" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" strokeLinejoin="round" />
            <path d="M14 7l3-3 3 3-3 3-3-3Z" stroke="currentColor" strokeWidth="1.6" strokeLinecap="round" strokeLinejoin="round" />
          </svg>
        </div>
        <div className="flex items-baseline gap-2">
          <span className="text-sm font-semibold tracking-tight text-[#f1f3ff]">EmotionScript</span>
          <span className="text-[11px] text-[#8f95b6]">v1.0.0-beta</span>
        </div>
      </div>

      <div className="flex items-center gap-5 md:gap-7">
        <nav className="hidden items-center gap-5 text-sm text-[#bbc0dd] md:flex">
          <Link href="/docs" className="transition-colors hover:text-white">
            Docs
          </Link>
          <Link
            href="/playground"
            className="border-b border-[#4d58ff] pb-0.5 text-[#7d87ff]"
            aria-current="page"
          >
            Playground
          </Link>
          <a href="https://discord.com" target="_blank" rel="noreferrer" className="transition-colors hover:text-white">
            Community
          </a>
          <a href="https://github.com" target="_blank" rel="noreferrer" className="transition-colors hover:text-white">
            GitHub
          </a>
        </nav>

        <div className="flex items-center gap-3">
          <Link
            href="/"
            aria-label="Back to landing page"
            className="rounded-md bg-[#2c2ce2] px-4 py-1.5 text-sm font-semibold text-white shadow-[0_0_15px_rgba(44,44,226,0.4)] transition-colors hover:bg-[#3b3be8]"
          >
            Back Home
          </Link>
        </div>
      </div>
    </header>
  );
}

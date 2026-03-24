"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";

const topLinks = [
  { href: "/docs", label: "Docs" },
  { href: "/playground", label: "Playground" },
  { href: "https://github.com", label: "GitHub", external: true },
  { href: "#community-support", label: "Community" },
];

export default function DocsNavbar() {
  const pathname = usePathname();

  return (
    <header className="sticky top-0 z-50 border-b border-white/10 bg-background-dark/80 backdrop-blur-md">
      <div className="mx-auto flex h-16 w-full max-w-400 items-center justify-between gap-6 px-5 sm:px-8">
        <div className="flex items-center gap-8">
          <Link href="/" className="group inline-flex items-center gap-2.5">
            <span className="relative inline-flex h-7 w-7 items-center justify-center rounded-md border border-white/15 bg-[#151536] text-primary shadow-[0_0_30px_-12px_rgba(44,44,226,0.85)]">
              <span
                className="h-3 w-3 rounded-sm bg-current"
                style={{ clipPath: "polygon(50% 0, 100% 25%, 100% 75%, 50% 100%, 0 75%, 0 25%)" }}
              />
            </span>
              <span className="text-base font-semibold tracking-tight text-white/95">EmotionScript</span>
          </Link>

          <nav aria-label="Primary" className="hidden items-center gap-5 text-sm text-white/65 md:flex">
            {topLinks.map((item) => {
              const isActive = item.href === "/docs" ? pathname.startsWith("/docs") : false;
              return (
                <Link
                  key={item.label}
                  href={item.href}
                  target={item.external ? "_blank" : undefined}
                  rel={item.external ? "noreferrer" : undefined}
                  className={
                    isActive
                      ? "text-white"
                      : "transition-colors duration-200 hover:text-white"
                  }
                >
                  {item.label}
                </Link>
              );
            })}
          </nav>
        </div>

        <div className="flex items-center gap-3">
          <label
            htmlFor="docs-search"
            className="hidden h-10 w-[320px] items-center gap-2 rounded-md border border-white/10 bg-[#0f1230] px-3 text-sm text-white/55 lg:flex"
          >
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" aria-hidden="true" className="text-white/45">
              <path
                d="M21 21L15.8 15.8M17 10.5C17 14.0899 14.0899 17 10.5 17C6.91015 17 4 14.0899 4 10.5C4 6.91015 6.91015 4 10.5 4C14.0899 4 17 6.91015 17 10.5Z"
                stroke="currentColor"
                strokeWidth="1.8"
                strokeLinecap="round"
              />
            </svg>
            <input
              id="docs-search"
              aria-label="Search documentation"
              placeholder="Search documentation..."
              className="w-full bg-transparent text-sm text-white/80 outline-none placeholder:text-white/35"
            />
          </label>

          <button
            type="button"
            className="h-10 rounded-md bg-primary px-4 text-sm font-semibold text-white transition-transform duration-200 hover:scale-[1.02]"
          >
            Download SDK
          </button>
        </div>
      </div>
    </header>
  );
}

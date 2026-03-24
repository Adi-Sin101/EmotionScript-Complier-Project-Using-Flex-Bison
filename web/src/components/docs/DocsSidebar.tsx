"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";

const navItems = [
  { href: "/docs", label: "Introduction" },
  { href: "/docs/syntax", label: "Language Basics" },
  { href: "/docs/keywords", label: "Keywords" },
  { href: "/docs/architecture", label: "Compiler Architecture" },
];

const guideItems = [
  { href: "/docs", label: "Getting Started" },
  { href: "/playground", label: "Interactive Playground" },
  { href: "/docs/architecture", label: "Compiler Pipeline" },
];

export default function DocsSidebar() {
  const pathname = usePathname();
  const activeLabel = pathname.startsWith("/docs/architecture")
    ? "Compiler Architecture"
    : pathname.startsWith("/docs/keywords")
      ? "Keywords"
      : pathname.startsWith("/docs/syntax")
        ? "Language Basics"
        : "Introduction";

  return (
    <aside
      className="sticky top-16 hidden h-[calc(100vh-4rem)] w-72 shrink-0 border-r border-white/10 bg-[#121725]/55 px-4 py-6 backdrop-blur-sm lg:block"
      aria-label="Documentation sidebar"
    >
      <div className="h-full overflow-y-auto pr-2">
        <p className="mb-5 px-3 text-[10px] font-semibold uppercase tracking-[0.24em] text-white/35">
          Version 1.2.0
        </p>

        <nav aria-label="Docs sections">
          <ul className="space-y-1.5">
            {navItems.map((item) => {
              const isActive = item.label === activeLabel;
              return (
                <li key={`${item.label}-${item.href}`}>
                  <Link
                    href={item.href}
                    className={
                      isActive
                        ? "flex items-center rounded-md bg-primary/20 px-3 py-2 text-sm font-medium text-[#b1bfff]"
                        : "flex items-center rounded-md px-3 py-2 text-sm text-white/70 transition-colors duration-200 hover:bg-white/6 hover:text-white"
                    }
                  >
                    <span className="mr-2 h-1.5 w-1.5 rounded-full bg-current opacity-80" aria-hidden="true" />
                    {item.label}
                  </Link>
                </li>
              );
            })}
          </ul>
        </nav>

        <div className="my-7 h-px bg-white/10" />

        <section aria-label="Guides">
          <p className="mb-3 px-3 text-[10px] font-semibold uppercase tracking-[0.2em] text-white/35">Guides</p>
          <ul className="space-y-1">
            {guideItems.map((item) => (
              <li key={`${item.label}-${item.href}`}>
                <Link
                  href={item.href}
                  className="flex rounded-md px-3 py-2 text-sm text-white/65 transition-colors duration-200 hover:bg-white/4 hover:text-white"
                >
                  {item.label}
                </Link>
              </li>
            ))}
          </ul>
        </section>
      </div>
    </aside>
  );
}

"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";
import { useState } from "react";

const navLinks = [
  { label: "Playground", href: "/playground" },
  { label: "Docs", href: "/docs" },
  { label: "GitHub", href: "https://github.com", external: true },
];

export default function Navbar() {
  const pathname = usePathname();
  const [open, setOpen] = useState(false);

  const isActive = (href: string) => {
    if (href === "/docs") {
      return pathname.startsWith("/docs");
    }
    return pathname === href;
  };

  return (
    <header className="sticky top-0 z-50 border-b border-white/10 bg-[#0f121bd9] backdrop-blur-xl">
      <nav className="mx-auto flex h-16 w-full max-w-310 items-center justify-between px-4 md:px-8">
        <Link href="/" className="inline-flex items-center text-[26px] font-semibold leading-none text-white" onClick={() => setOpen(false)}>
          <span className="tracking-tight" style={{ fontFamily: "var(--font-space-grotesk)" }}>EmotionScript</span>
        </Link>

        <ul className="hidden items-center gap-8 text-sm font-semibold text-white/85 md:flex">
          {navLinks.map((item) => (
            <li key={item.label}>
              <Link
                href={item.href}
                target={item.external ? "_blank" : undefined}
                rel={item.external ? "noreferrer" : undefined}
                className={item.external
                  ? "rounded-md px-2 py-1 transition-colors hover:text-white"
                  : isActive(item.href)
                    ? "rounded-md bg-white/8 px-2 py-1 text-white"
                    : "rounded-md px-2 py-1 transition-colors hover:bg-white/6 hover:text-white"}
              >
                {item.label}
              </Link>
            </li>
          ))}
        </ul>

        <button
          type="button"
          aria-label="Toggle navigation"
          className="inline-flex h-10 w-10 items-center justify-center rounded-md border border-white/15 text-white/85 md:hidden"
          onClick={() => setOpen((value) => !value)}
        >
          <span className="sr-only">Menu</span>
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" aria-hidden="true">
            <path d="M4 7h16M4 12h16M4 17h16" stroke="currentColor" strokeWidth="1.8" strokeLinecap="round" />
          </svg>
        </button>
      </nav>

      <div className={`border-t border-white/10 bg-[#101523] px-4 py-3 md:hidden ${open ? "block" : "hidden"}`}>
        <ul className="flex flex-col gap-2 text-sm font-semibold text-white/90">
          {navLinks.map((item) => (
            <li key={item.label}>
              <Link
                href={item.href}
                target={item.external ? "_blank" : undefined}
                rel={item.external ? "noreferrer" : undefined}
                onClick={() => setOpen(false)}
                className={item.external
                  ? "block rounded-md px-3 py-2 hover:bg-white/6"
                  : isActive(item.href)
                    ? "block rounded-md bg-white/10 px-3 py-2 text-white"
                    : "block rounded-md px-3 py-2 hover:bg-white/6"}
              >
                {item.label}
              </Link>
            </li>
          ))}
        </ul>
      </div>
    </header>
  );
}

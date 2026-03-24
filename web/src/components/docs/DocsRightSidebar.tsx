"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";

const pageLinks: Record<string, Array<{ href: string; label: string }>> = {
  introduction: [
    { href: "#what-is-emotionscript", label: "What is EmotionScript?" },
    { href: "#core-philosophy", label: "Core Philosophy" },
    { href: "#quick-start", label: "Quick Start" },
    { href: "#simple-example", label: "A Simple Example" },
    { href: "#community-support", label: "Community Support" },
  ],
  syntax: [
    { href: "#program-structure", label: "Program Structure" },
    { href: "#declarations-and-assignment", label: "Declarations and Assignment" },
    { href: "#conditionals-and-loops", label: "Conditionals and Loops" },
    { href: "#functions-and-io", label: "Functions and I/O" },
  ],
  keywords: [
    { href: "#program-structure-keywords", label: "Program Structure" },
    { href: "#declarations-and-types-keywords", label: "Declarations and Types" },
    { href: "#types-keywords", label: "Types" },
    { href: "#control-flow-keywords", label: "Control Flow" },
    { href: "#functions-and-i-o-keywords", label: "Functions and I/O" },
    { href: "#modules-keywords", label: "Modules" },
    { href: "#operators-keywords", label: "Operators" },
  ],
  architecture: [
    { href: "#pipeline-overview", label: "Pipeline Overview" },
    { href: "#compiler-phases", label: "Compiler Phases" },
    { href: "#directory-structure", label: "Directory Structure" },
  ],
};

export default function DocsRightSidebar() {
  const pathname = usePathname();
  const links = pathname.startsWith("/docs/syntax")
    ? pageLinks.syntax
    : pathname.startsWith("/docs/keywords")
      ? pageLinks.keywords
      : pathname.startsWith("/docs/architecture")
        ? pageLinks.architecture
        : pageLinks.introduction;

  return (
    <aside
      className="sticky top-16 hidden h-[calc(100vh-4rem)] w-67.5 shrink-0 border-l border-white/6 bg-[#121725]/35 px-5 py-8 backdrop-blur-sm xl:block"
      aria-label="On this page"
    >
      <div className="space-y-6">
        <section>
          <h2 className="mb-3 text-[11px] font-semibold uppercase tracking-[0.18em] text-white/35">On this page</h2>
          <nav>
            <ul className="space-y-2">
              {links.map((link) => (
                <li key={link.href}>
                  <Link
                    href={link.href}
                    className="text-sm text-white/62 transition-colors duration-200 hover:text-[#aebcff]"
                  >
                    {link.label}
                  </Link>
                </li>
              ))}
            </ul>
          </nav>
        </section>

        <section className="rounded-lg border border-white/10 bg-[#131a38]/70 p-4">
          <h3 className="text-sm font-semibold text-white">Need help?</h3>
          <p className="mt-1 text-sm leading-relaxed text-white/60">
            Join our Discord community and get answers in real-time.
          </p>
          <Link
            href="https://discord.com"
            target="_blank"
            rel="noreferrer"
            className="mt-3 inline-flex items-center gap-1 text-sm font-medium text-primary transition-colors hover:text-[#6f6ff2]"
          >
            Join Discord
            <span aria-hidden="true">&gt;</span>
          </Link>
        </section>
      </div>
    </aside>
  );
}

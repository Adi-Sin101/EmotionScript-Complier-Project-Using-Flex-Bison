import Link from "next/link";

const links = [
  { label: "GitHub", href: "https://github.com", external: true },
  { label: "Playground", href: "/playground" },
  { label: "Docs", href: "/docs" },
];

export default function Footer() {
  return (
    <footer className="border-t border-white/8">
      <div className="mx-auto flex w-full max-w-310 flex-col items-center justify-between gap-6 px-4 py-10 sm:px-6 md:flex-row md:px-8">
        <div className="inline-flex items-center gap-2 text-[27px] font-semibold text-white">
          <span style={{ fontFamily: "var(--font-space-grotesk)" }}>EmotionScript</span>
        </div>

        <nav aria-label="Footer" className="flex flex-wrap items-center justify-center gap-8 text-sm text-white/70">
          {links.map((item) => (
            <Link
              key={item.label}
              href={item.href}
              target={item.external ? "_blank" : undefined}
              rel={item.external ? "noreferrer" : undefined}
              className="transition-colors hover:text-white"
            >
              {item.label}
            </Link>
          ))}
        </nav>

        <p className="text-xs text-white/40">© 2024 EmotionScript Foundation. Licensed under MIT.</p>
      </div>
    </footer>
  );
}

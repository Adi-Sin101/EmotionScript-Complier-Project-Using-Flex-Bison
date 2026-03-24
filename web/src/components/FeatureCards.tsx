const cards = [
  {
    title: "Designed for Learning Compilers",
    description:
      "A clean architecture designed to teach the fundamentals of compiler design through modular construction.",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="M4 7.5 12 4l8 3.5L12 11 4 7.5Z" />
        <path d="M4 12.5 12 16l8-3.5" />
      </svg>
    ),
  },
  {
    title: "Flex + Bison Powered",
    description:
      "Leveraging industry-standard tools for robust lexical and syntactic analysis that stands the test of time.",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <rect x="6" y="6" width="12" height="12" rx="2" />
        <path d="M9 3v3M15 3v3M9 18v3M15 18v3M3 9h3M18 9h3M3 15h3M18 15h3" />
      </svg>
    ),
  },
  {
    title: "Space Inspired Syntax",
    description:
      "A unique DSL that makes coding feel like navigating the stars with specialized primitives for telemetry.",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="m12 3 1.7 3.6L18 8.2l-3 2.9.8 4.2L12 13.5l-3.8 1.8.8-4.2-3-2.9 4.3-1.6L12 3Z" />
      </svg>
    ),
  },
];

export default function FeatureCards() {
  return (
    <section className="mx-auto w-full max-w-310 border-t border-white/8 px-4 py-16 sm:px-6 lg:px-8 lg:py-20">
      <h2 className="text-center text-[clamp(2rem,5vw,3.3rem)] font-semibold tracking-[-0.02em] text-white" style={{ fontFamily: "var(--font-space-grotesk)" }}>Engineered for the Void</h2>
      <p className="mx-auto mt-5 max-w-[66ch] text-center text-[clamp(1rem,2.2vw,1.18rem)] leading-relaxed text-[#c9d2ff]/74">
        Robust architecture meeting the demands of high-concurrency interstellar computing.
      </p>

      <div className="mt-12 grid gap-4 md:grid-cols-3 lg:gap-5">
        {cards.map((card) => (
          <article
            key={card.title}
            className="rounded-xl border border-white/12 bg-[#171d2f]/78 p-6 backdrop-blur-sm hover:-translate-y-1 hover:border-primary/40 hover:bg-[#1b2237]"
          >
            <div className="mb-5 inline-flex h-11 w-11 items-center justify-center rounded-lg border border-primary/35 bg-primary/15 text-[#9dadff]">
              {card.icon}
            </div>
            <h3 className="text-[1.38rem] font-semibold leading-tight text-white">{card.title}</h3>
            <p className="mt-3 text-[0.98rem] leading-relaxed text-[#c9d1f2]/72">{card.description}</p>
          </article>
        ))}
      </div>
    </section>
  );
}

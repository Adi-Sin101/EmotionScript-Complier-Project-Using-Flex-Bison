const steps = [
  {
    title: "Source Code",
    subtitle: "Raw text file (.ems)",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="M8 8h8M8 12h5M6 4h12a2 2 0 0 1 2 2v12l-4-2-4 2-4-2-4 2V6a2 2 0 0 1 2-2Z" />
      </svg>
    ),
  },
  {
    title: "Lexical Analysis",
    subtitle: "Flex Tokenization",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="m5 5 14 14M19 5 5 19" />
      </svg>
    ),
  },
  {
    title: "Syntax Analysis",
    subtitle: "Bison Parsing (AST)",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="M7 7h4v4H7zM13 13h4v4h-4zM13 7h4v4h-4zM7 13h4v4H7z" />
      </svg>
    ),
  },
  {
    title: "Semantic Analysis",
    subtitle: "Type Checking",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="M12 3 4.2 7.5v9L12 21l7.8-4.5v-9L12 3Z" />
        <path d="m9.5 12 1.5 1.5 3.5-3.5" />
      </svg>
    ),
  },
  {
    title: "Intermediate",
    subtitle: "Optimized Bytecode",
    icon: (
      <svg viewBox="0 0 24 24" className="h-5 w-5" fill="none" stroke="currentColor" strokeWidth="1.8">
        <path d="M12 3 4 7.5 12 12l8-4.5L12 3Zm-8 9 8 4.5 8-4.5M4 16.5 12 21l8-4.5" />
      </svg>
    ),
  },
];

export default function PipelineSection() {
  return (
    <section className="mx-auto w-full max-w-310 border-t border-white/8 px-4 py-16 sm:px-6 lg:px-8 lg:py-20">
      <h2 className="text-center text-[clamp(2rem,5vw,3.3rem)] font-semibold tracking-[-0.02em] text-white" style={{ fontFamily: "var(--font-space-grotesk)" }}>How EmotionScript Works</h2>

      <div className="relative mt-12">
        <div className="absolute left-0 top-7 hidden h-px w-full bg-linear-to-r from-transparent via-primary/60 to-transparent lg:block" />

        <div className="grid gap-8 sm:grid-cols-2 lg:grid-cols-5 lg:gap-5">
          {steps.map((step) => (
            <article key={step.title} className="relative text-center">
              <div className="mx-auto inline-flex h-12 w-12 items-center justify-center rounded-lg border border-primary/40 bg-primary/12 text-[#a1b0ff]">
                {step.icon}
              </div>
              <h3 className="mt-4 text-[1.2rem] font-semibold leading-tight text-white">{step.title}</h3>
              <p className="mt-2 text-[0.95rem] leading-relaxed text-[#c9d1f2]/70">{step.subtitle}</p>
            </article>
          ))}
        </div>
      </div>
    </section>
  );
}

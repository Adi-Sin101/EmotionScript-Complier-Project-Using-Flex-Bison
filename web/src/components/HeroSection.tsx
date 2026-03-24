import Link from "next/link";
import CodePreview from "@/components/CodePreview";

export default function HeroSection() {
  return (
    <section className="mx-auto grid w-full max-w-310 grid-cols-1 gap-12 px-4 pb-20 pt-10 sm:px-6 lg:grid-cols-2 lg:items-start lg:gap-16 lg:px-8 lg:pb-24 lg:pt-14">
      <div>
        <div className="mb-7 inline-flex items-center rounded-full border border-primary/30 bg-primary/12 px-4 py-1.5 text-[11px] font-semibold tracking-[0.16em] text-[#95a3ff]">
          V1.0.0 NOW AVAILABLE
        </div>

        <h1 className="max-w-140 text-[clamp(2.4rem,7vw,5.3rem)] font-semibold leading-[0.93] tracking-[-0.03em] text-white" style={{ fontFamily: "var(--font-space-grotesk)" }}>
          <span className="block">Explore Programming</span>
          <span className="mt-1 block bg-linear-to-r from-[#8cb0ff] via-[#6e88ff] to-[#53d5ff] bg-clip-text text-transparent">
            Through the Cosmos
          </span>
        </h1>

        <p className="mt-7 max-w-[60ch] text-[clamp(1.02rem,2.5vw,1.24rem)] leading-relaxed text-[#d0d8ff]/86">
          A professional developer tool built with Flex + Bison for the next generation of space-faring code.
          High performance, stellar syntax, and astronomical efficiency.
        </p>

        <div className="mt-9 flex flex-wrap gap-3">
          <Link
            href="/playground"
            className="rounded-lg border border-primary/40 bg-primary px-6 py-3 text-sm font-semibold text-white shadow-[0_12px_28px_-16px_rgba(76,97,255,0.7)] hover:bg-[#6276ff]"
          >
            Try Playground
          </Link>
          <Link
            href="/docs"
            className="rounded-lg border border-white/18 bg-[#171d31]/70 px-6 py-3 text-sm font-semibold text-white/92 hover:border-white/35 hover:bg-[#1d2742]"
          >
            View Documentation
          </Link>
        </div>
      </div>

      <div className="relative lg:pt-12">
        <CodePreview />
      </div>
    </section>
  );
}

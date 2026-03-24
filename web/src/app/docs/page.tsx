import DocsCallout from "@/components/docs/DocsCallout";
import DocsCodeBlock from "@/components/docs/DocsCodeBlock";

const quickStartCode = `# macOS / Linux
./scripts/build_compiler.sh
./backend/compiler/emotionscript examples/loops/for_basic.ems out.syntax.txt out.tac

# Windows (PowerShell)
.\\scripts\\build_compiler.ps1
.\\run-pipeline.ps1 -InputFile examples\\loops\\for_basic.ems -Rebuild

# Start docs and playground
cd web
npm install
npm run dev`;

const emotionExample = `mind IntroExample
awake
count i;

replay (set i -> 0; i < 3; i++)
    speak(i);
end_ruminate
sleep`;

export default function DocsIntroduction() {
  return (
    <article className="text-white/90">
      <nav aria-label="Breadcrumb" className="mb-7 text-sm text-white/45">
        <ol className="flex items-center gap-2">
          <li>Docs</li>
          <li className="text-white/30">&gt;</li>
          <li className="text-white/75">Introduction</li>
        </ol>
      </nav>

      <header>
        <h1 className="text-5xl font-extrabold tracking-tight text-white sm:text-6xl">Introduction</h1>
        <p className="mt-6 max-w-3xl text-[1.2rem] leading-9 text-white/72">
          Welcome to the official EmotionScript documentation. EmotionScript is an emotion-oriented
          programming language implemented using Flex and Bison, designed to illustrate compiler
          construction concepts from lexical analysis to optimized intermediate code.
        </p>
      </header>

      <DocsCallout title="Did you know?">
        <p>
          EmotionScript uses expressive keywords such as
          <span className="mx-1 font-mono text-primary">mind</span>,
          <span className="mx-1 font-mono text-primary">awake</span>,
          <span className="mx-1 font-mono text-primary">if_feel</span>, and
          <span className="ml-1 font-mono text-primary">replay</span>.
          These map directly to common constructs like main(), declarations, conditionals,
          and loops.
        </p>
      </DocsCallout>

      <section id="what-is-emotionscript" className="mt-12 scroll-mt-24">
        <h2 className="text-4xl font-bold tracking-tight text-white">What is EmotionScript?</h2>
        <div className="mt-3 h-px bg-white/10" />
        <p className="mt-5 text-lg leading-8 text-white/72">
          EmotionScript is an educational language where a program begins with
          <span className="mx-1 font-mono text-primary">mind</span> and
          <span className="mx-1 font-mono text-primary">awake</span>, and ends with
          <span className="mx-1 font-mono text-primary">sleep</span>. It is built
          for students and compiler enthusiasts who want to understand how a modern compiler works,
          end-to-end, through a memorable domain language.
        </p>
      </section>

      <section id="core-philosophy" className="mt-8 scroll-mt-24">
        <h3 className="text-2xl font-semibold tracking-tight text-white">Core Philosophy</h3>
        <p className="mt-4 text-lg leading-8 text-white/72">
          The language focuses on static typing, mission-based structure, compiler pipeline
          visualization, and educational compiler design. Every keyword and grammar construct is
          purposefully shaped to make lexical, syntactic, and semantic phases easier to reason
          about.
        </p>

        <div className="mt-8 grid gap-4 sm:grid-cols-2">
          <article className="rounded-xl border border-white/10 bg-slate-900/50 p-5">
            <h4 className="text-2xl font-semibold text-white">Zero-Cost Abstractions</h4>
            <p className="mt-2 text-sm leading-7 text-white/65">
              High-level mission syntax compiles down to efficient semantic actions and optimized
              three-address code.
            </p>
          </article>

          <article className="rounded-xl border border-white/10 bg-slate-900/50 p-5">
            <h4 className="text-2xl font-semibold text-white">Inspectable Compiler Pipeline</h4>
            <p className="mt-2 text-sm leading-7 text-white/65">
              Each stage, from tokenization to TAC execution, is intentionally transparent for
              debugging and learning compiler internals.
            </p>
          </article>
        </div>
      </section>

      <section id="quick-start" className="mt-14 scroll-mt-24">
        <h2 className="text-4xl font-bold tracking-tight text-white">Quick Start</h2>
        <p className="mt-4 text-lg leading-8 text-white/72">
          Install the compiler toolchain and run your first EmotionScript program locally.
        </p>
        <div className="mt-5">
          <DocsCodeBlock filename="shell" language="shell" code={quickStartCode} />
        </div>
      </section>

      <section id="simple-example" className="mt-14 scroll-mt-24">
        <h2 className="text-4xl font-bold tracking-tight text-white">A Simple Example</h2>
        <p className="mt-4 text-lg leading-8 text-white/72">
          This sample shows a complete lifecycle with declaration, conditional flow, and replay loop
          iteration.
        </p>
        <div className="mt-5">
          <DocsCodeBlock filename="main.ems" language="emotionscript" code={emotionExample} />
        </div>
        <ul className="mt-5 space-y-2 text-sm leading-7 text-white/65">
          <li>
            <span className="font-mono text-primary">mind</span> declares the program scope.
          </li>
          <li>
            <span className="font-mono text-primary">count</span> declares typed variables.
          </li>
          <li>
            <span className="font-mono text-primary">if_feel</span> represents conditional branching.
          </li>
          <li>
            <span className="font-mono text-primary">replay</span> models iterative loop execution.
          </li>
        </ul>
      </section>

      <section id="community-support" className="mt-14 scroll-mt-24">
        <h2 className="text-3xl font-bold tracking-tight text-white">Community Support</h2>
        <p className="mt-4 max-w-3xl text-base leading-8 text-white/68">
          EmotionScript is a learning-focused compiler ecosystem. Join the community to discuss
          keyword design, parser decisions, and compiler pipeline enhancements.
        </p>
      </section>
    </article>
  );
}

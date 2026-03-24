import DocsCodeBlock from "@/components/docs/DocsCodeBlock";

export default function DocsArchitecture() {
  return (
    <article className="text-white/90">
      <nav aria-label="Breadcrumb" className="mb-7 text-sm text-white/45">
        <ol className="flex items-center gap-2">
          <li>Docs</li>
          <li className="text-white/30">&gt;</li>
          <li className="text-white/75">Compiler Architecture</li>
        </ol>
      </nav>

      <header>
        <h1 className="text-5xl font-extrabold tracking-tight text-white sm:text-6xl">Compiler Architecture</h1>
        <p className="mt-6 max-w-3xl text-[1.08rem] leading-8 text-white/72">
          EmotionScript follows a classic multi-phase compiler pipeline built with Flex, Bison, and
          C. Each phase is intentionally explicit so learners can inspect language processing from
          raw source to optimized execution.
        </p>
      </header>

      <section id="pipeline-overview" className="mt-10 scroll-mt-24">
        <h2 className="text-3xl font-semibold tracking-tight text-white">Pipeline Overview</h2>
        <div className="mt-4">
          <DocsCodeBlock
            filename="pipeline.txt"
            code={`Source Code (.ems)
    |
    v
Lexer (Flex)
    |
    v
Parser (Bison)
    |
    v
Semantic Analysis (Symbol Table)
    |
    v
IR Generation (TAC)
    |
    v
Execution (TAC Interpreter)`}
          />
        </div>
      </section>

      <section id="compiler-phases" className="mt-10 space-y-8 scroll-mt-24">
        {[
          [
            "1. Lexical Analysis",
            "backend/compiler/emotionscript.l converts source code into tokens for EmotionScript keywords, identifiers, literals, and operators.",
          ],
          [
            "2. Parsing",
            "backend/compiler/emotionscript.y defines the LALR(1) grammar and semantic actions for declarations, control flow, functions, persona blocks, and expressions.",
          ],
          [
            "3. Semantic Analysis",
            "symbol_table.c, function_table.c, and persona_table.c track scoped symbols, validate declarations, and catch invalid references.",
          ],
          [
            "4. Intermediate Code",
            "intermediate_code.c emits three-address code for arithmetic, conditionals, loops, function calls, and runtime operations.",
          ],
          [
            "5. Execution",
            "The TAC interpreter executes optimized instructions with call frames, stack-based parameters, and array storage.",
          ],
        ].map((item) => (
          <article key={item[0]} className="rounded-xl border border-white/10 bg-[#12162f]/70 p-5">
            <h3 className="text-2xl font-semibold text-white">{item[0]}</h3>
            <p className="mt-2 leading-7 text-white/70">{item[1]}</p>
          </article>
        ))}
      </section>

      <section id="directory-structure" className="mt-10 scroll-mt-24">
        <h2 className="text-3xl font-semibold tracking-tight text-white">Directory Structure</h2>
        <div className="mt-4">
          <DocsCodeBlock
            filename="backend/compiler"
            code={`backend/compiler/
  emotionscript.l        # Flex lexer definition
  emotionscript.y        # Bison parser grammar
  symbol_table.c/h       # Symbol table and semantic checks
  function_table.c/h     # Function metadata and checks
  persona_table.c/h      # Persona/class metadata and checks
  intermediate_code.c/h  # Three-address code generator
  interpreter.c/h        # Runtime interpreter
  emotionscript.exe      # Compiler executable (Windows)`}
          />
        </div>
      </section>
    </article>
  );
}

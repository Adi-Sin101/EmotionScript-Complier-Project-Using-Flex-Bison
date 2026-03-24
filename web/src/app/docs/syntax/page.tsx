import DocsCodeBlock from "@/components/docs/DocsCodeBlock";
import languageRef from "@/lib/language-reference.generated.json";

export default function DocsSyntax() {
  return (
    <article className="text-white/90">
      <nav aria-label="Breadcrumb" className="mb-7 text-sm text-white/45">
        <ol className="flex items-center gap-2">
          <li>Docs</li>
          <li className="text-white/30">&gt;</li>
          <li className="text-white/75">Language Basics</li>
        </ol>
      </nav>

      <header>
        <h1 className="text-5xl font-extrabold tracking-tight text-white sm:text-6xl">Language Basics</h1>
        <p className="mt-6 max-w-3xl text-[1.08rem] leading-8 text-white/72">
          EmotionScript syntax is emotion-oriented by design. Programs begin with mind/awake,
          use semicolon-terminated statements, and express control flow using readable
          keywords.
        </p>
      </header>

      <section className="mt-10 space-y-10">
        <div id="program-structure" className="scroll-mt-24">
          <h2 className="text-3xl font-semibold tracking-tight text-white">Program Structure</h2>
          <p className="mt-3 text-white/70">Every EmotionScript program is wrapped in a mind block:</p>
          <div className="mt-4">
            <DocsCodeBlock
              filename="program.ems"
              language="emotionscript"
              code={`mind ProgramName
awake
    $$ your code here
sleep`}
            />
          </div>
        </div>

        <div id="declarations-and-assignment" className="scroll-mt-24">
          <h2 className="text-3xl font-semibold tracking-tight text-white">Declarations and Assignment</h2>
          <p className="mt-3 text-white/70">
            Use typed declarations such as count/measure/truth and assign with arrow notation.
            Statements end with semicolons.
          </p>
          <div className="mt-4">
            <DocsCodeBlock
              filename="declarations.ems"
              language="emotionscript"
              code={`count x;
measure pi;
truth isReady;
words name;

set x -> 42;
set pi -> 3.14;`}
            />
          </div>
        </div>

        <div id="conditionals-and-loops" className="scroll-mt-24">
          <h2 className="text-3xl font-semibold tracking-tight text-white">Conditionals and Loops</h2>
          <div className="mt-4">
            <DocsCodeBlock
            filename="control-flow.ems"
            language="emotionscript"
            code={`if_feel x > 0
      speak(x);
    else_feel
      speak(0);
    end_feel

    replay (set x -> 0; x < 3; x++)
      speak(x);
    end_ruminate`}
            />
          </div>
        </div>

        <div id="functions-and-io" className="scroll-mt-24">
          <h2 className="text-3xl font-semibold tracking-tight text-white">Functions and I/O</h2>
          <div className="mt-4">
            <DocsCodeBlock
              filename="functions.ems"
              language="emotionscript"
              code={`interpret add(count a, count b) yields count <<
    reflect a + b;
>>

invoke add(1, 2);
speak("done");`}
            />
          </div>
          <p className="mt-3 text-white/70">
            EmotionScript supports function definitions with <span className="font-mono text-primary">interpret</span> and returns with <span className="font-mono text-primary">reflect</span>.
          </p>
        </div>

        <div className="overflow-x-auto rounded-xl border border-white/10 bg-[#12162f]/70">
          <table className="w-full min-w-155 text-left text-sm">
            <thead className="bg-[#1b2142] text-white/75">
              <tr>
                <th className="px-4 py-3">Operation</th>
                <th className="px-4 py-3">Keyword</th>
                <th className="px-4 py-3">Symbol</th>
              </tr>
            </thead>
            <tbody>
              {languageRef.operators.arithmetic.map((row) => (
                <tr key={row.operation} className="border-t border-white/8 text-white/70">
                  <td className="px-4 py-3">{row.operation}</td>
                  <td className="px-4 py-3 font-mono text-primary">{row.keyword}</td>
                  <td className="px-4 py-3 font-mono text-primary">{row.symbol}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>

        <div className="grid gap-4 lg:grid-cols-2">
          <article className="rounded-xl border border-white/10 bg-[#12162f]/70 p-4">
            <h3 className="text-lg font-semibold text-white">Comparison Operators</h3>
            <p className="mt-3 font-mono text-sm text-primary">{languageRef.operators.comparisons.join(", ")}</p>
          </article>

          <article className="rounded-xl border border-white/10 bg-[#12162f]/70 p-4">
            <h3 className="text-lg font-semibold text-white">Logical Operators</h3>
            <p className="mt-3 font-mono text-sm text-primary">{languageRef.operators.logical.join(", ")}</p>
          </article>
        </div>

        <p className="rounded-xl border border-white/10 bg-[#12162f]/70 px-4 py-3 text-sm leading-7 text-white/75">
          {languageRef.notes.reservedMeaning}
        </p>

        <div>
          <h2 className="text-3xl font-semibold tracking-tight text-white">Orbit Times Loop</h2>
          <div className="mt-4">
            <DocsCodeBlock
              filename="replay-for.ems"
              language="emotionscript"
              code={`count i;

replay (set i -> 0; i < 5; i++)
    speak(i);
end_ruminate`}
            />
          </div>
        </div>
      </section>
    </article>
  );
}

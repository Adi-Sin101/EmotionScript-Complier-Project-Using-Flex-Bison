import languageRef from "@/lib/language-reference.generated.json";

const generatedAtText = new Date(languageRef.generatedAt).toLocaleString();

export default function DocsKeywords() {
  return (
    <article className="text-white/90">
      <nav aria-label="Breadcrumb" className="mb-7 text-sm text-white/45">
        <ol className="flex items-center gap-2">
          <li>Docs</li>
          <li className="text-white/30">&gt;</li>
          <li className="text-white/75">Keywords</li>
        </ol>
      </nav>

      <header>
        <h1 className="text-5xl font-extrabold tracking-tight text-white sm:text-6xl">Language Keywords</h1>
        <p className="mt-6 max-w-3xl text-[1.08rem] leading-8 text-white/72">
          EmotionScript maps expressive vocabulary to familiar compiler constructs. This table is your
          quick reference when translating C/C++ ideas into EmotionScript grammar.
        </p>
        <p className="mt-3 text-sm text-white/45">Generated from lexer/parser sources: {generatedAtText}</p>
      </header>

      <div className="mt-10 space-y-10">
        {languageRef.keywordSections.map((section) => (
          <section key={section.title} id={section.id} className="scroll-mt-24">
            <h2 className="mb-4 text-3xl font-semibold tracking-tight text-white">{section.title}</h2>
            <div className="overflow-x-auto rounded-xl border border-white/10 bg-[#12162f]/70">
              <table className="w-full min-w-155 text-left text-sm">
                <thead className="bg-[#1b2142] text-white/75">
                  <tr>
                    <th className="px-4 py-3">Keyword</th>
                    <th className="px-4 py-3">Purpose</th>
                    <th className="px-4 py-3">C/C++ Equivalent</th>
                    <th className="px-4 py-3">Status</th>
                  </tr>
                </thead>
                <tbody>
                  {section.rows.map((row) => (
                    <tr key={row.keyword} className="border-t border-white/8 text-white/70">
                      <td className="px-4 py-3 font-mono text-primary">{row.keyword}</td>
                      <td className="px-4 py-3">{row.purpose}</td>
                      <td className="px-4 py-3 font-mono">{row.equivalent}</td>
                      <td className="px-4 py-3">
                        <span
                          className={`inline-flex rounded-full border px-2 py-0.5 text-[11px] uppercase tracking-[0.08em] ${
                            row.status === "active"
                              ? "border-emerald-400/30 bg-emerald-400/10 text-emerald-200"
                              : "border-amber-400/30 bg-amber-400/10 text-amber-200"
                          }`}
                        >
                          {row.status}
                        </span>
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </section>
        ))}

        <section>
          <h2 className="mb-4 text-3xl font-semibold tracking-tight text-white">Reference Note</h2>
          <p className="rounded-xl border border-white/10 bg-[#12162f]/70 px-4 py-4 text-sm leading-7 text-white/80">
            {languageRef.notes.reservedMeaning}
          </p>
        </section>
      </div>
    </article>
  );
}

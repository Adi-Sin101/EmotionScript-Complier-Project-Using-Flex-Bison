const codeLines = [
  "const G = 6.674e-11;",
  "",
  "func escape_velocity(mass, radius) {",
  "  let velocity = sqrt((2 * G * mass) / radius);",
  "  emit velocity // Broadcast to mission control",
  "}",
  "",
  "// Initialize propulsion system",
  "mission Apollo11 {",
  '  target: "Moon",',
  "  payload: calc_mass()",
  "}",
];

export default function CodePreview() {
  return (
    <section className="w-full max-w-[560px] rounded-xl border border-primary/20 bg-[#070f2a]/85 shadow-[0_14px_45px_rgba(14,17,80,0.55)] backdrop-blur-sm">
      <header className="flex items-center justify-between rounded-t-xl border-b border-white/10 bg-[#131a3d]/80 px-4 py-2">
        <div className="flex items-center gap-2">
          <span className="h-2.5 w-2.5 rounded-full bg-[#ff5f57]" />
          <span className="h-2.5 w-2.5 rounded-full bg-[#ffbd2e]" />
          <span className="h-2.5 w-2.5 rounded-full bg-[#28ca42]" />
        </div>
        <span className="font-mono text-xs text-white/45">orbit_calc.ems</span>
      </header>

      <pre className="overflow-x-auto px-5 py-6 font-mono text-[14px] leading-8 text-[#c5d3ff]">
        {codeLines.map((line) => (
          <div key={line}>
            {line.includes("6.674e-11") ? (
              <>
                <span className="text-[#9cd1ff]">const</span>{" "}
                <span className="text-[#f4f6ff]">G</span>{" = "}
                <span className="text-[#ffd75e]">6.674e-11</span>;
              </>
            ) : line.includes("escape_velocity") ? (
              <>
                <span className="text-[#9cd1ff]">func</span>{" "}
                <span className="text-[#5dd4ff]">escape_velocity</span>(mass, radius) {'{'}
              </>
            ) : line.includes("sqrt") ? (
              <>
                <span className="text-white">  let velocity = </span>
                <span className="text-[#8cd8ff]">sqrt</span>
                <span className="text-white">((</span>
                <span className="text-[#ffd75e]">2 * G * mass</span>
                <span className="text-white">) / radius);</span>
              </>
            ) : line.includes("Broadcast") ? (
              <>
                <span className="text-white">  emit velocity </span>
                <span className="text-white/40">{"// Broadcast to mission control"}</span>
              </>
            ) : line.includes("Initialize") ? (
              <span className="text-white/40">{"// Initialize propulsion system"}</span>
            ) : line.includes("mission Apollo11") ? (
              <>
                <span className="text-[#9cd1ff]">mission</span>{" "}
                <span className="text-[#f5f8ff]">Apollo11</span>{" "}
                {'{'}
              </>
            ) : line.includes('target: "Moon"') ? (
              <>
                <span className="text-[#f5f8ff]">  target:</span>{" "}
                <span className="text-[#ffd75e]">&quot;Moon&quot;</span>,
              </>
            ) : line === "" ? (
              " "
            ) : (
              line
            )}
          </div>
        ))}
      </pre>
    </section>
  );
}

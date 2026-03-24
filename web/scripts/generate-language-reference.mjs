import { mkdir, readFile, writeFile } from "node:fs/promises";
import { dirname, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const repoRoot = resolve(__dirname, "..", "..");
const lexerPath = resolve(repoRoot, "backend", "compiler", "emotionscript.l");
const parserPath = resolve(repoRoot, "backend", "compiler", "emotionscript.y");
const outputPath = resolve(__dirname, "..", "src", "lib", "language-reference.generated.json");

const sectionForKeyword = {
  mind: "Program Structure",
  awake: "Program Structure",
  sleep: "Program Structure",
  abort: "Program Structure",
  scene: "Program Structure",

  thought: "Declarations and Types",
  memory: "Declarations and Types",
  trait: "Declarations and Types",
  state: "Declarations and Types",
  count: "Types",
  measure: "Types",
  truth: "Types",
  words: "Types",
  emotion: "Types",
  level: "Types",

  if_feel: "Control Flow",
  else_if_feel: "Control Flow",
  else_feel: "Control Flow",
  end_feel: "Control Flow",
  decide: "Control Flow",
  when: "Control Flow",
  otherwise: "Control Flow",
  end_decision: "Control Flow",
  while_feeling: "Control Flow",
  calm: "Control Flow",
  for: "Control Flow",
  break: "Control Flow",
  continue: "Control Flow",

  declare: "Functions and I/O",
  end_declare: "Functions and I/O",
  returns: "Functions and I/O",
  return: "Functions and I/O",
  call: "Functions and I/O",
  speak: "Functions and I/O",
  listen: "Functions and I/O",
  alert: "Functions and I/O",

  persona: "Persona and OOP",
  end_persona: "Persona and OOP",
  inherit: "Persona and OOP",
  override: "Persona and OOP",
  open: "Persona and OOP",
  guarded: "Persona and OOP",
  hidden: "Persona and OOP",
  public: "Persona and OOP",
  protected: "Persona and OOP",
  this: "Persona and OOP",

  states: "FSM",
  event: "FSM",
  current: "FSM",
  transition: "FSM",

  abs: "Built-ins and Runtime",
  sqrt: "Built-ins and Runtime",
  ceil: "Built-ins and Runtime",
  floor: "Built-ins and Runtime",
  diff: "Built-ins and Runtime",
  norm: "Built-ins and Runtime",
  pow: "Built-ins and Runtime",
  sin: "Built-ins and Runtime",
  cos: "Built-ins and Runtime",
  tan: "Built-ins and Runtime",
  asin: "Built-ins and Runtime",
  acos: "Built-ins and Runtime",
  atan: "Built-ins and Runtime",

  and: "Operators",
  or: "Operators",
  not: "Operators",
  xor: "Operators",
  true: "Operators",
  false: "Operators",
};

const keywordMeta = {
  mind: { purpose: "Program start token", equivalent: "entry point" },
  awake: { purpose: "Begin main block", equivalent: "{" },
  sleep: { purpose: "End main block", equivalent: "}" },
  abort: { purpose: "Terminate execution", equivalent: "exit" },
  scene: { purpose: "Named scope block", equivalent: "scope" },

  thought: { purpose: "Variable declaration", equivalent: "let/var" },
  memory: { purpose: "Persistent variable", equivalent: "state" },
  trait: { purpose: "Persona member declaration", equivalent: "field" },
  state: { purpose: "State declaration", equivalent: "state" },

  count: { purpose: "Integer type", equivalent: "int" },
  measure: { purpose: "Floating type", equivalent: "float" },
  truth: { purpose: "Boolean type", equivalent: "bool" },
  words: { purpose: "String type", equivalent: "string" },
  emotion: { purpose: "Emotion domain type", equivalent: "custom" },
  level: { purpose: "Level domain type", equivalent: "custom" },

  if_feel: { purpose: "Conditional branch", equivalent: "if" },
  else_if_feel: { purpose: "Secondary branch", equivalent: "else if" },
  else_feel: { purpose: "Fallback branch", equivalent: "else" },
  end_feel: { purpose: "End conditional", equivalent: "endif" },
  decide: { purpose: "Switch-like branch", equivalent: "switch" },
  when: { purpose: "Decision branch", equivalent: "case" },
  otherwise: { purpose: "Default branch", equivalent: "default" },
  end_decision: { purpose: "End decision block", equivalent: "end switch" },
  while_feeling: { purpose: "While loop", equivalent: "while" },
  calm: { purpose: "End while loop", equivalent: "end while" },
  for: { purpose: "For loop", equivalent: "for" },
  break: { purpose: "Break loop", equivalent: "break" },
  continue: { purpose: "Continue loop", equivalent: "continue" },

  declare: { purpose: "Function declaration", equivalent: "function" },
  end_declare: { purpose: "End function declaration", equivalent: "end function" },
  returns: { purpose: "Function return type", equivalent: "returns" },
  return: { purpose: "Return value", equivalent: "return" },
  call: { purpose: "Function invocation", equivalent: "call" },
  speak: { purpose: "Output text/value", equivalent: "print" },
  listen: { purpose: "Read input", equivalent: "input" },
  alert: { purpose: "Emit alert", equivalent: "warn/log" },

  persona: { purpose: "Persona declaration", equivalent: "class" },
  end_persona: { purpose: "End persona declaration", equivalent: "end class" },
  inherit: { purpose: "Inheritance", equivalent: "extends" },
  override: { purpose: "Method override", equivalent: "override" },
  open: { purpose: "Public access", equivalent: "public" },
  guarded: { purpose: "Protected access", equivalent: "protected" },
  hidden: { purpose: "Private-like access", equivalent: "private" },
  public: { purpose: "Public access", equivalent: "public" },
  protected: { purpose: "Protected access", equivalent: "protected" },
  this: { purpose: "Current instance", equivalent: "this" },

  states: { purpose: "FSM states declaration", equivalent: "states" },
  event: { purpose: "FSM event", equivalent: "event" },
  current: { purpose: "Current FSM state", equivalent: "current" },
  transition: { purpose: "FSM transition", equivalent: "transition" },

  abs: { purpose: "Absolute value", equivalent: "abs" },
  sqrt: { purpose: "Square root", equivalent: "sqrt" },
  ceil: { purpose: "Ceiling function", equivalent: "ceil" },
  floor: { purpose: "Floor function", equivalent: "floor" },
  diff: { purpose: "Difference helper", equivalent: "diff" },
  norm: { purpose: "Normalization helper", equivalent: "norm" },
  pow: { purpose: "Power function", equivalent: "pow" },
  sin: { purpose: "Sine function", equivalent: "sin" },
  cos: { purpose: "Cosine function", equivalent: "cos" },
  tan: { purpose: "Tangent function", equivalent: "tan" },
  asin: { purpose: "Arc-sine function", equivalent: "asin" },
  acos: { purpose: "Arc-cosine function", equivalent: "acos" },
  atan: { purpose: "Arc-tangent function", equivalent: "atan" },

  and: { purpose: "Logical and", equivalent: "&&" },
  or: { purpose: "Logical or", equivalent: "||" },
  not: { purpose: "Logical not", equivalent: "!" },
  xor: { purpose: "Logical xor", equivalent: "xor" },
  true: { purpose: "Boolean true literal", equivalent: "true" },
  false: { purpose: "Boolean false literal", equivalent: "false" },
};

const operatorRows = [
  { operation: "Addition", keyword: "+", symbol: "+" },
  { operation: "Subtraction", keyword: "-", symbol: "-" },
  { operation: "Multiplication", keyword: "*", symbol: "*" },
  { operation: "Division", keyword: "/", symbol: "/" },
  { operation: "Modulo", keyword: "%", symbol: "%" },
  { operation: "Power", keyword: "**", symbol: "**" },
];

function normalizeSectionKey(title) {
  return `${title.toLowerCase().replace(/[^a-z0-9]+/g, "-")}-keywords`;
}

async function generateLanguageReference() {
  const [lexerContent, parserContent] = await Promise.all([
    readFile(lexerPath, "utf8"),
    readFile(parserPath, "utf8"),
  ]);

  const grammarStart = parserContent.indexOf("%%");
  const grammarEnd = parserContent.indexOf("%%", grammarStart + 2);
  const grammarContent = grammarStart !== -1 && grammarEnd !== -1
    ? parserContent.slice(grammarStart, grammarEnd)
    : parserContent;

  const keywordRegex = /"([A-Za-z_]+)"\s*\{\s*return\s+([A-Z_]+);\s*\}/g;
  const keywords = [];

  for (const match of lexerContent.matchAll(keywordRegex)) {
    const keyword = match[1];
    const token = match[2];
    const isUsed = new RegExp(`\\b${token}\\b`).test(grammarContent);
    keywords.push({ keyword, token, isUsed });
  }

  const keywordsBySection = new Map();

  for (const entry of keywords) {
    const section = sectionForKeyword[entry.keyword] ?? "Other";
    if (!keywordsBySection.has(section)) {
      keywordsBySection.set(section, []);
    }

    const meta = keywordMeta[entry.keyword] ?? {
      purpose: "Language token",
      equivalent: "-",
    };

    keywordsBySection.get(section).push({
      keyword: entry.keyword,
      purpose: meta.purpose,
      equivalent: meta.equivalent,
      status: entry.isUsed ? "active" : "reserved",
    });
  }

  const preferredOrder = [
    "Program Structure",
    "Declarations and Types",
    "Types",
    "Control Flow",
    "Functions and I/O",
    "Modules",
    "Built-ins and Runtime",
    "Operators",
    "Other",
  ];

  const keywordSections = preferredOrder
    .filter((title) => keywordsBySection.has(title))
    .map((title) => {
      const rows = keywordsBySection
        .get(title)
        .sort((a, b) => a.keyword.localeCompare(b.keyword));
      return {
        title,
        id: normalizeSectionKey(title),
        rows,
      };
    });

  const hasReservedKeywords = keywordSections.some((section) =>
    section.rows.some((row) => row.status === "reserved")
  );

  const payload = {
    generatedAt: new Date().toISOString(),
    keywordSections,
    operators: {
      arithmetic: operatorRows,
      comparisons: ["<", ">", "<=", ">=", "==", "!="],
      logical: ["and", "or", "not", "xor"],
    },
    notes: {
      reservedMeaning: hasReservedKeywords
        ? "Reserved tokens are recognized by the lexer but currently have no grammar production in the parser."
        : "All lexer keywords are currently covered by parser productions and are active.",
    },
  };

  await mkdir(dirname(outputPath), { recursive: true });
  await writeFile(outputPath, `${JSON.stringify(payload, null, 2)}\n`, "utf8");
  console.log(`Generated language reference: ${outputPath}`);
}

generateLanguageReference().catch((error) => {
  console.error(error);
  process.exit(1);
});

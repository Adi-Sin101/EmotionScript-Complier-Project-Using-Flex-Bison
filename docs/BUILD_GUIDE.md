# EmotionScript Compiler - Complete Guide

This guide covers building and using both the **Lexer** (tokenizer) and **Parser** (syntax validator) for the EmotionScript language.

---

## 📦 Requirements

### Windows
- **GCC** (MinGW or similar)
- **Flex** (lexical analyzer generator)
- **Bison** (parser generator) - *Optional for syntax validation*

### Installing Tools on Windows

1. **Install Flex and Bison:**
   - Download from: https://github.com/lexxmark/winflexbison/releases
   - Or use Chocolatey: `choco install winflexbison3`
   - Or use Scoop: `scoop install flex bison`

2. **Install GCC:**
   - MinGW: https://www.mingw-w64.org/
   - Or use TDM-GCC: https://jmeubank.github.io/tdm-gcc/

---

## 🔨 Building the Compiler

### Option 1: Build Lexer Only (Token Generator)

```powershell
# Generate lexer
flex emotionscript.l

# Compile lexer
gcc lex.yy.c -o emotionscript_lexer.exe
```

### Option 2: Build Parser (Syntax Validator)

```powershell
# Generate parser
bison -d emotionscript.y

# Generate lexer in Bison mode
flex emotionscript.l

# Compile both together
gcc -DBISON_MODE emotionscript.tab.c lex.yy.c -o emotionscript_parser.exe
```

### Option 3: Use Build Script (Recommended)

```powershell
# Build both lexer and parser
.\build.ps1

# Build only lexer
.\build.ps1 lexer

# Build only parser
.\build.ps1 parser

# Clean build files
.\build.ps1 clean

# Run tests
.\build.ps1 test
```

---

## 🚀 Usage

### Lexer (Tokenization)

Generate tokens from an EmotionScript source file:

```powershell
.\emotionscript_lexer.exe input.ems output.tokens
```

**Example:**
```powershell
.\emotionscript_lexer.exe test.ems token.out
type token.out
```

**Output:** List of tokens with their types (PROGRAM_MIND, TYPE_COUNT, etc.)

### Parser (Syntax Validation)

Validate syntax of an EmotionScript source file:

```powershell
.\emotionscript_parser.exe input.ems output.syntax
```

**Example:**
```powershell
.\emotionscript_parser.exe test.ems syntax.out
type syntax.out
```

**Output:** Syntax validation report with errors (if any)

---

## 📝 File Structure

```
EmotionScript/
├── emotionscript.l          # Flex lexer specification
├── emotionscript.y          # Bison parser grammar
├── test.ems                 # Sample EmotionScript program
├── build.ps1               # Build automation script
├── Makefile                # Unix/Make build file
├── README.md               # Project documentation
├── TOKEN_REFERENCE.md      # Token naming reference
└── .gitignore              # Git ignore file
```

---

## 🧪 Testing

### Test the Lexer

```powershell
.\emotionscript_lexer.exe test.ems token.out
```

Check `token.out` for proper tokenization.

### Test the Parser

```powershell
.\emotionscript_parser.exe test.ems syntax.out
```

Check `syntax.out` for syntax validation results.

### Expected Output (Valid Syntax)

```
=== EmotionScript Syntax Validator ===

✓ Valid EmotionScript program

=== SYNTAX VALIDATION: SUCCESS ===
✓ No syntax errors found
✓ Program structure is valid
```

### Expected Output (Invalid Syntax)

```
=== EmotionScript Syntax Validator ===

✗ SYNTAX ERROR at line 15: unexpected token
✗ SYNTAX ERROR at line 23: missing semicolon

=== SYNTAX VALIDATION: FAILED ===
✗ Found 2 syntax error(s)
✗ Please fix the errors and try again
```

---

## 📖 Language Syntax Examples

### Basic Program Structure

```emotionscript
mind ProgramName
awake
    ## Your code here
sleep
```

### Variable Declarations

```emotionscript
count age;
measure temperature;
truth isHappy;
words message;
```

### Assignment

```emotionscript
set age -> 25;
set temperature -> 36.5;
set isHappy -> yes;
set message -> "Hello World";
```

### Functions

```emotionscript
interpret calculateSum(count x, count y) yields count
    reflect x + y;
end_interpret

invoke calculateSum(10, 20);
```

### Conditionals

```emotionscript
if_feel age == 25
    speak("Age is 25");
else_if_feel age > 25
    speak("Older than 25");
else_feel
    speak("Younger than 25");
end_feel
```

### Loops

```emotionscript
ruminate count < 10
    count++;
    speak("Counting");
end_ruminate
```

### Classes (Persona)

```emotionscript
persona Happy evolves_from Neutral
    manifest
        thought positivity;
end_personae
```

---

## 🔍 Grammar Features

The Bison grammar (`emotionscript.y`) supports:

- ✅ Program structure (mind/awake/sleep)
- ✅ All data types (count, measure, truth, words, emotion, level)
- ✅ Variable declarations with modifiers
- ✅ Assignment statements
- ✅ Arithmetic and comparison operators
- ✅ Function declarations and calls
- ✅ Conditional statements (if_feel/else_if_feel/else_feel)
- ✅ Loop constructs (ruminate/end_ruminate)
- ✅ I/O operations (speak/listen/alert)
- ✅ Class declarations (persona/end_personae)
- ✅ Inheritance (evolves_from)
- ✅ FSM constructs (transition/trigger/when)
- ✅ Math functions (sine, cosine, magnitude, etc.)
- ✅ Member access (dot operator)
- ✅ Error recovery

---

## 🧾 TAC Output (Three Address Code)

The compiler now emits a structured TAC file for every successful parse.

### CLI Format

```powershell
.\emotionscript.exe <input.ems> <output.syntax> [output.tac]
```

- `input.ems`: EmotionScript source file.
- `output.syntax`: parse/execution report file.
- `output.tac` (optional): TAC output path.
    - If omitted, TAC is written to `output.tac` in the current working directory.

### TAC Instruction Format

The generated TAC is line-oriented and uses these core patterns:

- Declarations and assignments:
    - `DECL <type> <name>`
    - `<name> = <value|temp>`
- Expression temporaries:
    - `$t1 = a + b`
    - `$t2 = x < y`
- Control flow:
    - `IF_FALSE <cond> GOTO L1`
    - `LABEL L1`
    - `GOTO L2`
- Functions:
    - `FUNC add:` / `END_FUNC add`
    - `FORMAL a`
    - `PARAM 10`
    - `$t3 = CALL add, 2`
    - `RETURN value`
- OOP/persona:
    - `CLASS Student` / `END_CLASS Student`
    - `CLASS Child EXTENDS Parent`
    - `ATTR open count id`
    - `METHOD Student.init:` / `END_METHOD Student.init`
    - `ALLOC s1, Student`
    - `CALL_METHOD_IF_EXISTS s1.init, 1`
    - `$t4 = GET_FIELD s1.id`
    - `SET_FIELD s1.id, 101`
    - `$t5 = CALL_METHOD s1.greet, 0`

### Example Commands

```powershell
# Explicit TAC output file
.\emotionscript.exe ..\..\examples\functions\function_with_params.ems ..\..\examples\functions\function_with_params.out ..\..\examples\functions\function_with_params.tac

# Default TAC output (writes output.tac in current directory)
.\emotionscript.exe ..\..\examples\variables\declare.ems ..\..\examples\variables\declare.out
```

### Sample TAC Snippets

```text
DECL count a
DECL count b
$t1 = a + b
result = $t1
```

```text
CLASS Student
ATTR open count id
METHOD Student.init:
FORMAL x
id = x
RETURN yes
END_METHOD Student.init
END_CLASS Student
ALLOC s1, Student
PARAM 101
CALL_METHOD_IF_EXISTS s1.init, 1
```

---

## 🐛 Troubleshooting

## ⚠️ Error Handling

EmotionScript now reports clearer syntax and semantic diagnostics with line numbers, and continues parsing after many syntax failures using panic-mode recovery.

### Error Message Style

- Syntax:

```text
Syntax Error at line <N>:
<detailed parser message>
Near token: '<token>'
```

- Semantic:

```text
Semantic Error at line <N>:
<meaningful semantic description>
```

### Covered Cases

- Missing semicolons after declarations/statements.
- Undeclared variables.
- Type mismatch in assignment/initialization.
- Invalid function or method calls (undefined target or wrong argument count).

### Example Outputs

Missing semicolon after declaration:

```text
Syntax Error at line 10:
Missing ';' after declaration
Near token: 'set'
```

Undeclared variable:

```text
Semantic Error at line 7:
Variable 'x' used before declaration
```

Type mismatch:

```text
Semantic Error at line 9:
Type mismatch in assignment to 'a'
```

Invalid function call:

```text
Semantic Error at line 7:
Invalid function call: function 'notExists' is not declared
```

### Panic-Mode Recovery

The parser attempts to continue after syntax errors by synchronizing at statement/block boundaries, such as:

- `;`
- `end_feel`
- `end_ruminate`
- `>>`
- `}`
- `sleep`

This allows multiple syntax errors to be reported in one run instead of stopping at the first error.

---

### "bison: command not found"

Install Bison:
```powershell
# Using Chocolatey
choco install winflexbison3

# Using Scoop
scoop install bison
```

### "flex: command not found"

Install Flex:
```powershell
# Using Chocolatey
choco install winflexbison3

# Using Scoop
scoop install flex
```

### "gcc: command not found"

Install MinGW or TDM-GCC and add to PATH.

### Compilation Errors

Make sure you're using compatible versions:
- Flex 2.6+
- Bison 3.0+
- GCC 7.0+

---

## 📚 Token Categories

See [TOKEN_REFERENCE.md](TOKEN_REFERENCE.md) for complete token documentation.

**Major categories:**
- `PROGRAM_*` - Program structure
- `TYPE_*` - Data types
- `FUNC_*` - Functions
- `COND_*` - Conditionals
- `LOOP_*` - Loops
- `CLASS_*` - Classes/OOP
- `OP_*` - Operators
- `LIT_*` - Literals
- `IO_*` - Input/Output

---

## 🎯 Next Steps

1. **Extend Grammar**: Add more language features to `emotionscript.y`
2. **Semantic Analysis**: Add type checking and symbol table
3. **Code Generation**: Generate intermediate code or target assembly
4. **Optimization**: Add optimization passes
5. **Runtime**: Build interpreter or compile to executable

---

## 📄 License

This is an educational compiler project.

---

## 👤 Author

Student ID: 2107031
Course: Compiler Design Lab
Project: EmotionScript Language Compiler

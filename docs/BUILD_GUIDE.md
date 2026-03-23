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

## 🐛 Troubleshooting

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

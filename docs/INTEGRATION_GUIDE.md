# EmotionScript Lexer-Parser Integration Architecture

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    BUILD PROCESS (3 STEPS)                      │
└─────────────────────────────────────────────────────────────────┘

Step 1: Parser Generation
┌──────────────────┐
│ emotionscript.y  │  (Bison Grammar Spec)
│  - Tokens        │
│  - Rules         │
│  - Actions       │
└──────────┬───────┘
           │ bison -d
           ↓
    ╔════════════════════════════════════╗
    ║  emotionscript.tab.c (Parser)      ║
    ║  emotionscript.tab.h (Token enum)  ║
    ╚════════════════════════════════════╝


Step 2: Lexer Generation
┌──────────────────┐
│ emotionscript.l  │  (Flex Lexer Spec)
│  - Keywords      │
│  - Patterns      │
│  - Rules         │
└──────────┬───────┘
           │ flex
           ↓
    ╔════════════════════════════════════╗
    ║  lex.yy.c (Lexer)                  ║
    ║  References emotionscript.tab.h    ║
    ╚════════════════════════════════════╝


Step 3: Compilation & Linking
╔════════════════════════════════════╗
║  emotionscript.tab.c               ║
╚────────────────┬───────────────────╝
                 │
                 │ gcc -Wall -g
    ┌────────────┴────────────┐
    ↓                         ↓
╔════════════════════╗  ╔════════════════════╗
║  lex.yy.c          ║  │  emotionscript.exe  │
║                    ║──→ (Final Executable) │
║  (Lexer impl)      ║  │  Input: source.ems   │
║                    ║  │  Output: syntax.txt  │
╚════════════════════╝  ╚════════════════════╝
```

---

## File Dependencies

```
emotionscript.y
    │
    ├─ Declares all tokens
    │  (PROGRAM_MIND, TYPE_COUNT, IDENTIFIER, etc.)
    │
    └─ Generates emotionscript.tab.h
         │
         └─> Token enum { PROGRAM_MIND = 258, ..., IDENTIFIER = ..., ... }
             YYSTYPE union { char *string_val; int int_val; ... }
             yyparse() function prototype
             extern YYSTYPE yylval;

emotionscript.l
    │
    ├─ Includes emotionscript.tab.h
    │
    ├─ Uses token values: return PROGRAM_MIND;
    │
    ├─ Sets yylval.string_val for literals
    │
    └─ Generates lex.yy.c
         │
         └─> yylex() function that:
             - Reads input character by character
             - Matches patterns from emotionscript.l
             - Returns token values to parser
             - Sets yylval for semantic values
             - Tracks line numbers in yylineno
```

---

## Data Flow: Parsing a Program

```
INPUT FILE (test.ems)
    │
    mind HelloWorld
    awake
        count x;
        set x -> 42;
    sleep
    │
    ↓
┌─────────────────────────────────┐
│   LEXER (lex.yy.c)              │
│                                 │
│  Scans input character by char  │
│  Matches patterns               │
│  Returns tokens & yylval        │
└────────────┬────────────────────┘
             │
             │ Token stream
             ↓
┌─────────────────────────────────┐
│   PARSER (emotionscript.tab.c)  │
│                                 │
│  Calls yylex() for next token   │
│  Matches grammar rules          │
│  Executes semantic actions      │
│  Reports errors with yyerror()  │
└────────────┬────────────────────┘
             │
             │ Parse results
             ↓
        OUTPUT FILE
      (syntax_output.txt)
    ✓ Valid EmotionScript program
    ✓ No syntax errors found
```

---

## Token Flow Example

### Input: `mind Program awake count x; sleep`

```
Lexer Processing:
───────────────

Input: "mind"
    ↓ Pattern matches "mind"
    → yylval = (unused)
    → return PROGRAM_MIND (token #258)

Input: "Program"
    ↓ Pattern matches {IDENTIFIER}
    → yylval.string_val = strdup("Program")
    → return IDENTIFIER (token #275)

Input: "awake"
    ↓ Pattern matches "awake"
    → yylval = (unused)
    → return PROGRAM_AWAKE (token #259)

...and so on

Parser receives:
    PROGRAM_MIND    yylval = (ignored)
    IDENTIFIER      yylval.string_val = "Program"
    PROGRAM_AWAKE   yylval = (ignored)
    TYPE_COUNT      yylval = (ignored)
    IDENTIFIER      yylval.string_val = "x"
    DELIM_SEMICOLON yylval = (ignored)
    PROGRAM_SLEEP   yylval = (ignored)

Parser matches rule:
    program:
        PROGRAM_MIND IDENTIFIER PROGRAM_AWAKE program_body PROGRAM_SLEEP
        {
            fprintf(yyout, "✓ Valid EmotionScript program\n");
        }
```

---

## Integration Points

### 1. Token Definition
**File:** `emotionscript.y`
```yacc
%token PROGRAM_MIND PROGRAM_AWAKE PROGRAM_SLEEP
%token TYPE_COUNT TYPE_MEASURE
%token <string_val> IDENTIFIER LIT_INT LIT_FLOAT LIT_STRING
```
↓ Generates `emotionscript.tab.h`
```c
#define PROGRAM_MIND 258
#define PROGRAM_AWAKE 259
#define PROGRAM_SLEEP 260
#define TYPE_COUNT 261
#define TYPE_MEASURE 262
#define IDENTIFIER 275

typedef union {
    char *string_val;
    int int_val;
    double float_val;
} YYSTYPE;
extern YYSTYPE yylval;
```

### 2. Token Recognition
**File:** `emotionscript.l`
```lex
%{
#include "emotionscript.tab.h"
%}

"mind"      { return PROGRAM_MIND; }
"awake"     { return PROGRAM_AWAKE; }
{IDENTIFIER} { yylval.string_val = strdup(yytext); return IDENTIFIER; }
```

### 3. Grammar Application
**File:** `emotionscript.y`
```yacc
program:
    PROGRAM_MIND IDENTIFIER PROGRAM_AWAKE program_body PROGRAM_SLEEP
    {
        // $2 is the yylval.string_val of IDENTIFIER
        printf("Program: %s\n", $2);
    }
```

---

## Compilation Command Explanation

```bash
gcc emotionscript.tab.c lex.yy.c -o emotionscript
│   │                   │       │
│   │                   │       └─ Output executable
│   │                   └─ Lexer implementation file
│   └─ Parser implementation file
└─ C compiler
```

**Why this order matters:**
- Parser (emotionscript.tab.c) comes first (order doesn't actually matter for these simple files)
- Both files are compiled together into a single executable
- The linker resolves all symbols between them
- yylex() calls from parser link to yylex() in lexer
- yylval variable is defined in parser header, used in lexer

---

## Error Handling Flow

```
Lexical Errors (Lexer)
│
├─ Unknown character
│   → lexical_error() called
│   → Message: "LEXICAL ERROR at line N: Unknown symbol 'X' (ASCII Y)"
│   → Output to stderr
│
└─ Output: Error message only, no token returned

Syntax Errors (Parser)
│
├─ Token doesn't match grammar rule
│   → yyerror() called with message
│   → Error recovery: error DELIM_SEMICOLON { yyerrok; }
│   → Message: "SYNTAX ERROR at line N: [unexpected token]"
│   → Output to yyout (file)
│
└─ Output: Error message in output file
```

---

## Line Number Tracking

```
emotionscript.l:
    %option yylineno    ← Automatically increments on \n

Parser receives:
    extern int yylineno ← Available in grammar actions and yyerror()

Error reporting:
    fprintf(yyout, "Error at line %d: %s\n", yylineno, message);
                                              ↑
                                    Current line being parsed
```

---

## Key Configuration

### Union Definition (Semantic Values)
```yacc
%union {
    char *string_val;       ← For IDENTIFIER, literals, strings
    int int_val;            ← For future integer attributes
    double float_val;       ← For future float attributes
}
```

### Token Association
```yacc
%token <string_val> IDENTIFIER LIT_INT LIT_FLOAT LIT_STRING LIT_BOOL
      ↑
      Tokens with string values access via strdup() in lexer
```

### Precedence (Resolves Shift/Reduce Conflicts)
```yacc
%left OP_PLUS OP_MINUS      ← Lower: left-associative
%left OP_MUL OP_DIV         ← Higher: left-associative
%right OP_POWER             ← Highest: right-associative
```

---

## Build Artifact Explanation

### Generated by Bison (`bison -d emotionscript.y`)
- **emotionscript.tab.c** ← Main parser implementation
  - yyparse() function
  - Parser state machine
  - Token shift/reduce logic
  - Error handling

- **emotionscript.tab.h** ← Token definitions
  - Token enum value assignments
  - YYSTYPE union definition
  - Function prototypes
  - Variable declarations (yylval, yynerrs, etc.)

### Generated by Flex (`flex emotionscript.l`)
- **lex.yy.c** ← Main lexer implementation
  - yylex() function
  - Pattern matching engine
  - Token production
  - Character input handling

---

## No Longer Generated

### Standalone Mode (REMOVED)
Previously, the lexer had optional standalone compilation:
```c
#ifndef BISON_MODE
int main(int argc, char **argv) { ... }  ← REMOVED
#endif
```

**Reason:** Unified parser-lexer design requires only one executable.

---

## Summary

| Component | Language | Generates | Includes |
|-----------|----------|-----------|----------|
| emotionscript.y | Bison | .tab.c, .tab.h | - |
| emotionscript.l | Flex | lex.yy.c | emotionscript.tab.h |
| Compiler | GCC | emotionscript.exe | - |

**Result:** Single unified executable that lexes and parses EmotionScript programs.

---

✓ **Integration Complete**

The lexer and parser are now properly integrated, with the Bison-generated header providing token definitions that the lexer uses to return the correct token values.

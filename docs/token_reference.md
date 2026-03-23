# EmotionScript Token Reference Guide

This document describes the structured token naming convention used in the EmotionScript lexer. The naming system groups tokens by semantic category for improved readability and debugging.

---

## Token Categories

### 1. PROGRAM Structure
Program-level constructs for defining the overall program structure.

| Keyword | Token | Description |
|---------|-------|-------------|
| `mind` | `PROGRAM_MIND` | Program declaration |
| `awake` | `PROGRAM_AWAKE` | Program start/begin |
| `sleep` | `PROGRAM_SLEEP` | Program end |
| `abort` | `PROGRAM_ABORT` | Emergency program termination |
| `scene` | `PROGRAM_SCENE` | Scene/section marker |

**Example:**
```
PROGRAM_MIND
IDENTIFIER MainProgram
PROGRAM_AWAKE
...
PROGRAM_SLEEP
```

---

### 2. TYPE Keywords
Data type declarations for variables.

| Keyword | Token | Description |
|---------|-------|-------------|
| `count` | `TYPE_COUNT` | Integer type |
| `measure` | `TYPE_MEASURE` | Float type |
| `truth` | `TYPE_TRUTH` | Boolean type |
| `words` | `TYPE_WORDS` | String type |
| `emotion` | `TYPE_EMOTION` | Emotion state type |
| `level` | `TYPE_LEVEL` | Level/intensity type |

**Example:**
```
TYPE_COUNT
IDENTIFIER age
DELIM_SEMICOLON
```

---

### 3. VAR - Variable Storage Classes
Special variable modifiers and storage classes.

| Keyword | Token | Description |
|---------|-------|-------------|
| `thought` | `VAR_THOUGHT` | Temporary/local variable |
| `memory` | `VAR_MEMORY` | Persistent/global variable |
| `trait` | `VAR_TRAIT` | Constant variable |
| `state` | `VAR_STATE` | State variable |

---

### 4. ASSIGN - Assignment
Assignment operators.

| Keyword | Token | Description |
|---------|-------|-------------|
| `set` | `ASSIGN_SET` | Assignment operator |

**Example:**
```
ASSIGN_SET
IDENTIFIER age
OP_ARROW
LIT_INT 25
DELIM_SEMICOLON
```

---

### 5. IO - Input/Output
Input/output operations.

| Keyword | Token | Description |
|---------|-------|-------------|
| `speak` | `IO_SPEAK` | Print/output function |
| `listen` | `IO_LISTEN` | Input/read function |
| `alert` | `IO_ALERT` | Warning/error output |

---

### 6. FUNC - Function Constructs
Function declaration and control.

| Keyword | Token | Description |
|---------|-------|-------------|
| `interpret` | `FUNC_DECLARE` | Function declaration start |
| `end_interpret` | `FUNC_END` | Function declaration end |
| `yields` | `FUNC_RETURNS` | Function return type |
| `reflect` | `FUNC_RETURN` | Return statement |
| `invoke` | `FUNC_CALL` | Function call |

**Example:**
```
FUNC_DECLARE
IDENTIFIER calculateSum
DELIM_LPAREN
TYPE_COUNT
IDENTIFIER x
DELIM_COMMA
TYPE_COUNT
IDENTIFIER y
DELIM_RPAREN
FUNC_RETURNS
TYPE_COUNT
    FUNC_RETURN
    IDENTIFIER x
    OP_PLUS
    IDENTIFIER y
    DELIM_SEMICOLON
FUNC_END
```

---

### 7. COND - Conditional Constructs
Conditional statements and branching.

| Keyword | Token | Description |
|---------|-------|-------------|
| `if_feel` | `COND_IF` | If statement |
| `else_if_feel` | `COND_ELSE_IF` | Else-if statement |
| `else_feel` | `COND_ELSE` | Else statement |
| `end_feel` | `COND_END` | End conditional block |
| `decide` | `COND_SWITCH` | Switch statement |
| `otherwise` | `COND_DEFAULT` | Default case |

**Example:**
```
COND_IF
IDENTIFIER age
OP_EQ
LIT_INT 25
    IO_SPEAK
    DELIM_LPAREN
    LIT_STRING "Age is 25"
    DELIM_RPAREN
    DELIM_SEMICOLON
COND_ELSE
    IO_SPEAK
    DELIM_LPAREN
    LIT_STRING "Different age"
    DELIM_RPAREN
    DELIM_SEMICOLON
COND_END
```

---

### 8. LOOP - Loop Constructs
Loop statements and control flow.

| Keyword | Token | Description |
|---------|-------|-------------|
| `ruminate` | `LOOP_WHILE` | While loop |
| `end_ruminate` | `LOOP_END` | End loop block |
| `replay` | `LOOP_FOR` | For loop |
| `break_free` | `LOOP_BREAK` | Break statement |
| `continue_flow` | `LOOP_CONTINUE` | Continue statement |

**Example:**
```
LOOP_WHILE
IDENTIFIER age
OP_LT
LIT_INT 30
    IDENTIFIER age
    OP_INC
    DELIM_SEMICOLON
LOOP_END
```

---

### 9. FSM - Finite State Machine Constructs
State machine and emotion transition constructs.

| Keyword | Token | Description |
|---------|-------|-------------|
| `composed_of` | `FSM_STATES` | Define state list |
| `trigger` | `FSM_EVENT` | Event trigger |
| `current_emotion` | `FSM_CURRENT` | Current state accessor |
| `transition` | `FSM_TRANSITION` | State transition |
| `when` | `FSM_WHEN` | Condition for transition |

---

### 10. CLASS - Object-Oriented Constructs
Class/persona definitions and OOP features.

| Keyword | Token | Description |
|---------|-------|-------------|
| `persona` | `CLASS_PERSONA` | Class declaration |
| `end_personae` | `CLASS_END` | End class definition |
| `manifest` | `CLASS_BODY` | Class body/constructor |
| `open` | `CLASS_PUBLIC` | Public access modifier |
| `guarded` | `CLASS_PROTECTED` | Protected access modifier |
| `shared` | `CLASS_STATIC` | Static member |
| `evolves_from` | `CLASS_INHERIT` | Inheritance declaration |
| `reshape` | `CLASS_OVERRIDE` | Method override |

**Example:**
```
CLASS_PERSONA
IDENTIFIER Happy
CLASS_INHERIT
IDENTIFIER Neutral
    CLASS_BODY
        VAR_THOUGHT
        IDENTIFIER positivity
        DELIM_SEMICOLON
CLASS_END
```

---

### 11. MATH - Mathematical Functions
Mathematical and trigonometric operations.

| Keyword | Token | Description |
|---------|-------|-------------|
| `magnitude` | `MATH_ABS` | Absolute value |
| `balance` | `MATH_SQRT` | Square root |
| `estimate_up` | `MATH_CEIL` | Ceiling function |
| `estimate_down` | `MATH_FLOOR` | Floor function |
| `gap` | `MATH_DIFF` | Difference/distance |
| `normalize` | `MATH_NORM` | Normalization |
| `surge` | `MATH_POW` | Power/exponentiation |
| `sine` | `MATH_SIN` | Sine function |
| `cosine` | `MATH_COS` | Cosine function |
| `tangent` | `MATH_TAN` | Tangent function |
| `arcsine` | `MATH_ASIN` | Arcsine function |
| `arccosine` | `MATH_ACOS` | Arccosine function |
| `arctangent` | `MATH_ATAN` | Arctangent function |

---

### 12. OP - Operators
All operators including arithmetic, comparison, and special operators.

#### Assignment Flow
| Symbol | Token | Description |
|--------|-------|-------------|
| `->` | `OP_ARROW` | Assignment/flow arrow |

#### Comparison Operators
| Symbol | Token | Description |
|--------|-------|-------------|
| `==` | `OP_EQ` | Equal to |
| `!=` | `OP_NEQ` | Not equal to |
| `<=` | `OP_LEQ` | Less than or equal |
| `>=` | `OP_GEQ` | Greater than or equal |
| `<` | `OP_LT` | Less than |
| `>` | `OP_GT` | Greater than |

#### Arithmetic Operators
| Symbol | Token | Description |
|--------|-------|-------------|
| `++` | `OP_INC` | Increment |
| `--` | `OP_DEC` | Decrement |
| `+` | `OP_PLUS` | Addition |
| `-` | `OP_MINUS` | Subtraction |
| `*` | `OP_MUL` | Multiplication |
| `/` | `OP_DIV` | Division |
| `%` | `OP_MOD` | Modulo |

---

### 13. DELIM - Delimiters
Punctuation and structural delimiters.

| Symbol | Token | Description |
|--------|-------|-------------|
| `(` | `DELIM_LPAREN` | Left parenthesis |
| `)` | `DELIM_RPAREN` | Right parenthesis |
| `{` | `DELIM_LBRACE` | Left brace |
| `}` | `DELIM_RBRACE` | Right brace |
| `;` | `DELIM_SEMICOLON` | Statement terminator |
| `,` | `DELIM_COMMA` | Separator |
| `:` | `DELIM_COLON` | Type/label separator |
| `.` | `DELIM_DOT` | Member access |

---

### 14. LIT - Literals
Literal values (with values).

| Pattern | Token | Description |
|---------|-------|-------------|
| `yes`/`no` | `LIT_BOOL` | Boolean literals |
| `123` | `LIT_INT <value>` | Integer literals |
| `3.14` | `LIT_FLOAT <value>` | Float literals |
| `"text"` | `LIT_STRING <value>` | String literals |

**Example:**
```
LIT_INT 25
LIT_FLOAT 36.5
LIT_BOOL
LIT_STRING "Hello World"
```

---

### 15. IDENTIFIER
User-defined identifiers (with names).

| Pattern | Token | Description |
|---------|-------|-------------|
| Variable/function names | `IDENTIFIER <name>` | User-defined names |

**Example:**
```
IDENTIFIER age
IDENTIFIER calculateSum
```

---

## Benefits of Structured Token Naming

1. **Visual Structure**: Easy to identify program structure at a glance
2. **Block Recognition**: Clear START/END pairs (FUNC_DECLARE/FUNC_END, COND_IF/COND_END, etc.)
3. **Semantic Grouping**: Related tokens share prefixes (LOOP_*, FUNC_*, CLASS_*)
4. **Debugging**: Quickly spot mismatched blocks or incorrect structure
5. **Documentation**: Token names are self-documenting
6. **Parser Development**: Clear token categories simplify grammar rules
7. **Emotional DSL Theme**: Maintained through keywords while improving clarity

---

## Reading Token Output

The token output file now reads like a structured trace of your program:

```
PROGRAM_MIND                    # Program starts
IDENTIFIER MainProgram
PROGRAM_AWAKE

    TYPE_COUNT                  # Variable declaration
    IDENTIFIER age
    DELIM_SEMICOLON

    FUNC_DECLARE                # Function starts
    IDENTIFIER calculateSum
    ...
    FUNC_END                    # Function ends

    COND_IF                     # Conditional starts
    ...
    COND_END                    # Conditional ends

    LOOP_WHILE                  # Loop starts
    ...
    LOOP_END                    # Loop ends

PROGRAM_SLEEP                   # Program ends
```

This makes it easy to verify program structure, spot errors, and understand the lexical analysis at a glance.

# EmotionScript-Compiler-Project-Using-Flex-Bison

A custom programming language compiler built with Flex and Bison that emphasizes emotional and human-centric programming concepts.

## Features

- Custom lexical analyzer using Flex
- Emotion-based keywords and syntax
- Support for multiple data types: count, measure, truth, words, emotion, level
- Control structures: if_feel, ruminate (loops)
- Function definitions with interpret/yields
- Object-oriented features with persona (classes)
- Mathematical operations including trigonometric functions

## Files

- `emotionscript.l` - Flex lexer specification
- `test.ems` - Sample EmotionScript program
- `token.out` - Token output file

## Usage

```bash
# Compile the lexer
flex emotionscript.l
gcc lex.yy.c -o emotionscript.exe

# Run the lexer
./emotionscript.exe input.ems output.tokens
```

## TAC Generation

The compiler supports TAC (Three Address Code) generation in addition to syntax/execution output.

```powershell
.\emotionscript.exe <input.ems> <output.syntax> [output.tac]
```

- If `output.tac` is omitted, TAC is written to `output.tac` in the current working directory.
- TAC covers expressions, control flow, functions, persona inheritance, object creation, member access, and method calls.

## Regression Run (All Examples)

Use the regression script to rebuild the compiler and generate `.out` + `.tac` for every `examples/**/*.ems` file:

```powershell
cd backend/compiler
.\run-regression-tac.ps1
```

Optional flags:

```powershell
.\run-regression-tac.ps1 -VerboseLogs
.\run-regression-tac.ps1 -StopOnFailure
```

## Complete Pipeline

Run the full pipeline:

`EmotionScript -> Parser -> TAC -> Execution -> Output`

```powershell
.\run-pipeline.ps1 -InputFile examples/functions/function_with_params.ems -Rebuild
```

Generated artifacts are written under `pipeline_output/<category>/`:

- `<name>.syntax.txt` : parser + semantic + execution report
- `<name>.tac` : three-address intermediate code
- `<name>.execution.txt` : extracted execution trace stage
- `<name>.output.txt` : final user-visible outputs (`[SPEAK]` / `[ALERT]`)

## Language Syntax Examples

### Variable Declaration
```
count age;
measure temperature;
truth isHappy;
words message;
```

### Functions
```
interpret calculateSum(count x, count y) yields count
    reflect x + y;
end_interpret
```

### Control Flow
```
if_feel age == 25
    speak("Age is 25");
else_feel
    speak("Different age");
end_feel
```

### Loops
```
ruminate age < 30
    age++;
end_ruminate
```

### Classes (Persona)
```
persona Happy evolves_from Neutral
    manifest
        thought positivity;
end_personae
```

## Author

Built as part of Compiler Lab project.

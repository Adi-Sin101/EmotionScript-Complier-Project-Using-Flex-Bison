# EmotionScript Quick Build Guide

## Prerequisites
You need these tools installed:
- **Bison** (parser generator)
- **Flex** (lexer generator)  
- **GCC** (C compiler)

### Windows Installation
Install **MinGW** or **MSYS2** which includes all three tools.

### Linux/macOS
```bash
# Ubuntu/Debian
sudo apt-get install bison flex gcc

# macOS
brew install bison flex gcc
```

---

## Build Instructions

### Option 1: Using Makefile (Linux/macOS/WSL)
```bash
cd EmotionScript
make              # Build the compiler
make test         # Build and test on test.ems
make clean        # Remove generated files
make help         # Show available targets
```

### Option 2: Using PowerShell (Windows)
```powershell
cd EmotionScript
.\build-new.ps1           # Build the compiler
.\build-new.ps1 test      # Build and test on test.ems
.\build-new.ps1 clean     # Remove generated files
.\build-new.ps1 help      # Show available targets
```

### Option 3: Manual Commands (Any Platform)
```bash
# Step 1: Generate parser from Bison spec
bison -d emotionscript.y

# Step 2: Generate lexer from Flex spec
flex emotionscript.l

# Step 3: Compile everything
gcc -Wall -g emotionscript.tab.c lex.yy.c -o emotionscript

# Step 4: Run the compiler
./emotionscript test.ems output.txt
cat output.txt
```

---

## Generated Files

After building, you'll have these **generated** files (can be deleted):
- `emotionscript.tab.c` - Parser implementation (from Bison)
- `emotionscript.tab.h` - Parser header with token definitions (from Bison)
- `lex.yy.c` - Lexer implementation (from Flex)
- `emotionscript` or `emotionscript.exe` - Final executable

---

## Using the Compiler

```bash
# Syntax:
emotionscript <input_file.ems> <output_file.txt>

# Example:
./emotionscript test.ems result.txt

# View results:
cat result.txt
```

---

## Troubleshooting

### "bison: command not found"
→ Install Bison or add it to your PATH

### "flex: command not found"  
→ Install Flex or add it to your PATH

### "gcc: command not found"
→ Install GCC (MinGW on Windows)

### Compilation errors
→ Check that both lexer (.l) and parser (.y) files are syntactically correct
→ Review `emotionscript.output` for grammar conflicts

---

## Architecture

The three-step build process:

```
emotionscript.y (Parser spec)
    ↓
    bison -d emotionscript.y
    ↓
    emotionscript.tab.c + emotionscript.tab.h
        ↓
        gcc emotionscript.tab.c
        lex.yy.c -o emotionscript
        ↑
    flex emotionscript.l
    ↑
    emotionscript.l (Lexer spec)
```

The lexer includes the parser header to get token definitions, then both compile together into the executable.

---

## Files Reference

| File | Purpose |
|------|---------|
| `emotionscript.y` | Parser grammar specification |
| `emotionscript.l` | Lexer/tokenizer specification |
| `Makefile` | Unix/Linux/macOS build instructions |
| `build-new.ps1` | Windows PowerShell build script |
| `test.ems` | Sample EmotionScript program |
| `TOKEN_REFERENCE.md` | Complete token reference |
| `REFACTORING_SUMMARY.md` | Detailed refactoring documentation |

---

## Next Steps

After building successfully:

1. **Test more complex programs** - Create your own .ems files
2. **Extend the grammar** - Add more rules to emotionscript.y
3. **Add semantic actions** - Add { code } blocks to parse rules
4. **Generate output** - Add code to produce AST, bytecode, or target code
5. **Improve error handling** - Add more detailed error messages

---

✓ **Ready to build!** Run `make` or `.\build-new.ps1` now.

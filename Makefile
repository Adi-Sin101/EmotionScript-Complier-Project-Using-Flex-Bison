# EmotionScript Compiler Makefile
# Compiles a lexer (Flex) and parser (Bison) into a unified executable

# Compiler and tools
CC = gcc
FLEX = flex
BISON = bison

# Compiler flags
CFLAGS = -Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function

# Source files
LEX_FILE = emotionscript.l
YACC_FILE = emotionscript.y

# Generated files
LEX_C = lex.yy.c
YACC_C = emotionscript.tab.c
YACC_H = emotionscript.tab.h

# Output executable
TARGET = emotionscript

# Default target
.PHONY: all clean test run help

all: $(TARGET)

# Build the complete compiler
$(TARGET): $(YACC_C) $(LEX_C) symbol_table.c
	@echo "[3/3] Compiling object files and linking..."
	$(CC) $(CFLAGS) $(YACC_C) $(LEX_C) symbol_table.c -o $(TARGET)
	@echo "✓ Build successful: $(TARGET)"

# Step 1: Generate parser C code and header from Bison file
$(YACC_C) $(YACC_H): $(YACC_FILE)
	@echo "[1/3] Generating parser with Bison..."
	$(BISON) -d $(YACC_FILE)
	@echo "✓ Parser generated: $(YACC_C), $(YACC_H)"

# Step 2: Generate lexer C code from Flex file
$(LEX_C): $(LEX_FILE) $(YACC_H)
	@echo "[2/3] Generating lexer with Flex..."
	$(FLEX) $(LEX_FILE)
	@echo "✓ Lexer generated: $(LEX_C)"

# Run the compiler on test file
run: $(TARGET)
	@echo "Running EmotionScript compiler on test.ems..."
	./$(TARGET) test.ems syntax_output.txt
	@echo "✓ Output written to syntax_output.txt"
	@echo ""
	@echo "=== Compilation Result ==="
	@cat syntax_output.txt

# Run tests
test: $(TARGET) run
	@echo ""
	@echo "✓ Test completed successfully"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(LEX_C) $(YACC_C) $(YACC_H)
	rm -f emotionscript.exe $(TARGET)
	rm -f *.o
	@echo "✓ Clean complete"

# Show help
help:
	@echo "EmotionScript Compiler Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all    - Build the emotionscript executable (default)"
	@echo "  run    - Build and run on test.ems"
	@echo "  test   - Run build and tests"
	@echo "  clean  - Remove all generated files"
	@echo "  help   - Show this help message"
	@echo ""
	@echo "Build Command:"
	@echo "  make"
	@echo ""
	@echo "Usage:"
	@echo "  ./emotionscript <input.ems> <output.txt>"


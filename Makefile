# EmotionScript Compiler Makefile
# Supports both Windows (gcc/flex/bison) and Unix-like systems

# Compiler and tools
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -g

# Output executables
LEXER = emotionscript_lexer.exe
PARSER = emotionscript_parser.exe

# Source files
LEX_FILE = emotionscript.l
YACC_FILE = emotionscript.y

# Generated files
LEX_C = lex.yy.c
YACC_C = emotionscript.tab.c
YACC_H = emotionscript.tab.h

.PHONY: all lexer parser clean test help

# Default target: build both lexer and parser
all: lexer parser

# Build standalone lexer (token generator)
lexer: $(LEX_FILE)
	@echo "Building standalone lexer..."
	$(FLEX) $(LEX_FILE)
	$(CC) $(CFLAGS) $(LEX_C) -o $(LEXER)
	@echo "✓ Lexer built successfully: $(LEXER)"

# Build parser with lexer (syntax validator)
parser: $(LEX_FILE) $(YACC_FILE)
	@echo "Building parser with integrated lexer..."
	$(BISON) -d $(YACC_FILE)
	$(FLEX) -DBISON_MODE $(LEX_FILE)
	$(CC) $(CFLAGS) -DBISON_MODE $(YACC_C) $(LEX_C) -o $(PARSER)
	@echo "✓ Parser built successfully: $(PARSER)"

# Test the lexer
test-lexer: lexer
	@echo "Testing lexer on test.ems..."
	./$(LEXER) test.ems token.out
	@echo "✓ Tokens generated in token.out"

# Test the parser
test-parser: parser
	@echo "Testing parser on test.ems..."
	./$(PARSER) test.ems syntax.out
	@echo "✓ Syntax validation result in syntax.out"

# Run both tests
test: test-lexer test-parser
	@echo "✓ All tests completed"

# Clean generated files
clean:
	@echo "Cleaning generated files..."
	rm -f $(LEX_C) $(YACC_C) $(YACC_H)
	rm -f $(LEXER) $(PARSER)
	rm -f *.exe *.out
	@echo "✓ Clean complete"

# Help information
help:
	@echo "EmotionScript Compiler Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make all         - Build both lexer and parser (default)"
	@echo "  make lexer       - Build standalone lexer only"
	@echo "  make parser      - Build parser with integrated lexer"
	@echo "  make test-lexer  - Build and test lexer"
	@echo "  make test-parser - Build and test parser"
	@echo "  make test        - Run all tests"
	@echo "  make clean       - Remove generated files"
	@echo "  make help        - Show this help"
	@echo ""
	@echo "Usage examples:"
	@echo "  Lexer:  ./$(LEXER) input.ems output.tokens"
	@echo "  Parser: ./$(PARSER) input.ems output.syntax"

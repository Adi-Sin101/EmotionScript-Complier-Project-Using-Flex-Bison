CC = gcc
FLEX = flex
BISON = bison

CFLAGS = -Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function

LEX_FILE = emotionscript.l
YACC_FILE = emotionscript.y

LEX_C = lex.yy.c
YACC_C = emotionscript.tab.c
YACC_H = emotionscript.tab.h

TARGET = emotionscript.exe

MODULE_SRCS = symbol_table.c interpreter.c intermediate_code.c function_table.c persona_table.c

WORKSPACE_ROOT = ../..
FILE ?= examples/variables/declare.ems
OUT ?= syntax_output.txt

RESOLVED_FILE := $(if $(wildcard $(FILE)),$(FILE),$(WORKSPACE_ROOT)/$(FILE))
RESOLVED_OUT := $(if $(wildcard $(dir $(OUT))),$(OUT),$(WORKSPACE_ROOT)/$(OUT))

.PHONY: all clean run examples help

all: $(TARGET)

$(TARGET): $(YACC_C) $(LEX_C) $(MODULE_SRCS)
	@echo "[3/3] Compiling and linking compiler modules..."
	$(CC) $(CFLAGS) $(YACC_C) $(LEX_C) $(MODULE_SRCS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

$(YACC_C) $(YACC_H): $(YACC_FILE)
	@echo "[1/3] Generating parser with Bison..."
	$(BISON) -d $(YACC_FILE)

$(LEX_C): $(LEX_FILE) $(YACC_H)
	@echo "[2/3] Generating lexer with Flex..."
	$(FLEX) $(LEX_FILE)

run: $(TARGET)
	@echo "Running $(TARGET) with $(RESOLVED_FILE)"
	./$(TARGET) $(RESOLVED_FILE) $(RESOLVED_OUT)
	@echo "Output written to $(RESOLVED_OUT)"
	@cat $(RESOLVED_OUT)

examples: $(TARGET)
	@echo "Running representative examples..."
	./$(TARGET) $(WORKSPACE_ROOT)/examples/variables/declare.ems $(WORKSPACE_ROOT)/examples/variables/declare.out
	./$(TARGET) $(WORKSPACE_ROOT)/examples/expressions/arithmetic.ems $(WORKSPACE_ROOT)/examples/expressions/arithmetic.out
	./$(TARGET) $(WORKSPACE_ROOT)/examples/conditionals/if_basic.ems $(WORKSPACE_ROOT)/examples/conditionals/if_basic.out
	./$(TARGET) $(WORKSPACE_ROOT)/examples/loops/while_basic.ems $(WORKSPACE_ROOT)/examples/loops/while_basic.out
	@echo "Example outputs generated beside each sample file."

clean:
	rm -f $(LEX_C) $(YACC_C) $(YACC_H) $(TARGET)
	rm -f $(WORKSPACE_ROOT)/examples/*/*.out

help:
	@echo "Targets:"
	@echo "  make              - build compiler"
	@echo "  make run FILE=examples/loops/while_basic.ems"
	@echo "  make examples     - run representative examples"
	@echo "  make clean        - remove generated files"

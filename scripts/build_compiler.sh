#!/usr/bin/env bash
set -euo pipefail

# EmotionScript Compiler Build Script
# Generates lexer/parser and compiles the compiler binary

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
COMPILER_DIR="$ROOT_DIR/backend/compiler"

pushd "$COMPILER_DIR" >/dev/null

echo "==> Running Flex (lexer)..."
flex emotionscript.l

echo "==> Running Bison (parser)..."
bison -d emotionscript.y

echo "==> Compiling EmotionScript compiler..."
gcc -Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function \
    emotionscript.tab.c \
    lex.yy.c \
    symbol_table.c \
    interpreter.c \
    intermediate_code.c \
    function_table.c \
    persona_table.c \
    -o emotionscript

echo "==> Build complete: $COMPILER_DIR/emotionscript"

popd >/dev/null

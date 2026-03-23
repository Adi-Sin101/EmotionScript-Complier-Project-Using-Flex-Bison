# EmotionScript Project Report

## Overview
This project contains a modular EmotionScript compiler organized into lexer/parser, symbol table, interpreter support, and intermediate code trace support.

## Folder Layout
- backend/compiler: compiler sources and build system
- docs: project and language reference docs
- examples: focused test programs grouped by feature

## Compiler Modules
- emotionscript.l: lexical analyzer (Flex)
- emotionscript.y: parser and semantic/execution driver (Bison)
- symbol_table.c/h: symbol insertion, lookup, update, cleanup
- interpreter.c/h: execution trace utilities
- intermediate_code.c/h: lightweight intermediate trace collection
- Makefile: build and run commands

## Build and Run
From backend/compiler:

make
make run FILE=examples/loops/while_basic.ems

The run target accepts workspace-relative paths under examples/.

## Validation Strategy
Examples are intentionally minimal and isolated so each file exercises one concept only, making debugging and demo presentation straightforward.

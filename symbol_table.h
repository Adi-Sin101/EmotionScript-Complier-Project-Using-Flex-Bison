#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef enum SymbolType {
    SYM_COUNT,
    SYM_MEASURE,
    SYM_TRUTH,
    SYM_WORDS,
    SYM_EMOTION,
    SYM_LEVEL,
    SYM_UNKNOWN
} SymbolType;

typedef union SymbolValue {
    int int_val;
    double float_val;
    bool bool_val;
    char *str_val;
} SymbolValue;

typedef struct Symbol {
    char *name;
    SymbolType type;
    SymbolValue value;
    bool has_value;
    struct Symbol *next;
} Symbol;

#define SYMBOL_TABLE_SIZE 211

extern Symbol *symbol_table[SYMBOL_TABLE_SIZE];

unsigned int symbol_hash(const char *name);

int insert_symbol(const char *name, SymbolType type);
Symbol *lookup_symbol(const char *name);
int update_symbol(const char *name, SymbolValue value);
void free_symbol_table(void);
const char *symbol_type_to_string(SymbolType t);

#endif // SYMBOL_TABLE_H
#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "symbol_table.h"
#include <stdbool.h>

typedef struct FunctionDef {
    char *name;
    SymbolType return_type;
    char **param_names;
    SymbolType *param_types;
    int param_count;
    void *body; /* Points to Block* */
    struct FunctionDef *next;
} FunctionDef;

typedef struct {
    FunctionDef *head;
} FunctionTable;

/* Initialize/cleanup */
FunctionTable *func_table_create(void);
void func_table_free(FunctionTable *table);

/* Register a function */
int func_table_insert(FunctionTable *table, const char *name, SymbolType return_type,
                      char **param_names, SymbolType *param_types, int param_count, void *body);

/* Lookup a function */
FunctionDef *func_table_lookup(FunctionTable *table, const char *name);

#endif // FUNCTION_TABLE_H

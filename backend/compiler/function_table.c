#include "function_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FunctionTable *func_table_create(void) {
    FunctionTable *table = (FunctionTable *)malloc(sizeof(FunctionTable));
    if (table) {
        table->head = NULL;
    }
    return table;
}

void func_table_free(FunctionTable *table) {
    if (!table) return;

    FunctionDef *iter = table->head;
    while (iter) {
        FunctionDef *next = iter->next;
        free(iter->name);
        if (iter->param_names) {
            for (int i = 0; i < iter->param_count; i++) {
                free(iter->param_names[i]);
            }
            free(iter->param_names);
        }
        free(iter->param_types);
        /* Note: body (Block*) is freed elsewhere when cleaning up the AST */
        free(iter);
        iter = next;
    }
    free(table);
}

int func_table_insert(FunctionTable *table, const char *name, SymbolType return_type,
                      char **param_names, SymbolType *param_types, int param_count, void *body) {
    if (!table || !name) return 1;

    /* Check if function already exists */
    if (func_table_lookup(table, name) != NULL) {
        fprintf(stderr, "Function '%s' already defined\n", name);
        return 1;
    }

    FunctionDef *new_func = (FunctionDef *)malloc(sizeof(FunctionDef));
    if (!new_func) return 2;

    new_func->name = strdup(name);
    new_func->return_type = return_type;
    new_func->param_count = param_count;
    new_func->body = body;

    /* Copy parameter information */
    if (param_count > 0) {
        new_func->param_names = (char **)malloc(param_count * sizeof(char *));
        new_func->param_types = (SymbolType *)malloc(param_count * sizeof(SymbolType));

        if (!new_func->param_names || !new_func->param_types) {
            free(new_func->param_names);
            free(new_func->param_types);
            free(new_func->name);
            free(new_func);
            return 2;
        }

        for (int i = 0; i < param_count; i++) {
            new_func->param_names[i] = strdup(param_names[i]);
            new_func->param_types[i] = param_types[i];
        }
    } else {
        new_func->param_names = NULL;
        new_func->param_types = NULL;
    }

    new_func->next = table->head;
    table->head = new_func;
    return 0;
}

FunctionDef *func_table_lookup(FunctionTable *table, const char *name) {
    if (!table || !name) return NULL;

    FunctionDef *iter = table->head;
    while (iter) {
        if (strcmp(iter->name, name) == 0) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

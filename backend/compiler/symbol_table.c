#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Symbol *symbol_table[SYMBOL_TABLE_SIZE] = {0};

unsigned int symbol_hash(const char *name) {
    unsigned long hash = 5381;
    int c;
    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return (unsigned int)(hash % SYMBOL_TABLE_SIZE);
}

int insert_symbol(const char *name, SymbolType type) {
    if (!name) return 2;
    unsigned int h = symbol_hash(name);
    Symbol *iter = symbol_table[h];
    while (iter) {
        if (strcmp(iter->name, name) == 0) {
            if (iter->type == type) {
                return 0; /* allowed redeclaration of same type */
            }
            return 1;
        }
        iter = iter->next;
    }

    Symbol *new_sym = (Symbol *)malloc(sizeof(Symbol));
    if (!new_sym) return 2;
    new_sym->name = strdup(name);
    new_sym->type = type;
    new_sym->has_value = false;
    new_sym->next = symbol_table[h];
    symbol_table[h] = new_sym;
    return 0;
}

Symbol *lookup_symbol(const char *name) {
    if (!name) return NULL;
    unsigned int h = symbol_hash(name);
    Symbol *iter = symbol_table[h];
    while (iter) {
        if (strcmp(iter->name, name) == 0) return iter;
        iter = iter->next;
    }
    return NULL;
}

int update_symbol(const char *name, SymbolValue value) {
    Symbol *sym = lookup_symbol(name);
    if (!sym) return 1;

    if (sym->has_value && sym->type == SYM_WORDS && sym->value.str_val) {
        free(sym->value.str_val);
        sym->value.str_val = NULL;
    }

    sym->value = value;
    sym->has_value = true;
    return 0;
}

void free_symbol_table(void) {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        Symbol *iter = symbol_table[i];
        while (iter) {
            Symbol *next = iter->next;
            free(iter->name);
            if (iter->type == SYM_WORDS && iter->has_value && iter->value.str_val) {
                free(iter->value.str_val);
            }
            free(iter);
            iter = next;
        }
        symbol_table[i] = NULL;
    }
}

const char *symbol_type_to_string(SymbolType t) {
    switch (t) {
        case SYM_COUNT: return "count";
        case SYM_MEASURE: return "measure";
        case SYM_TRUTH: return "truth";
        case SYM_WORDS: return "words";
        case SYM_EMOTION: return "emotion";
        case SYM_LEVEL: return "level";
        default: return "unknown";
    }
}

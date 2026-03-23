#ifndef PERSONA_TABLE_H
#define PERSONA_TABLE_H

#include <stdbool.h>
#include "symbol_table.h"

typedef enum AccessModifier {
    ACCESS_PRIVATE = 0,
    ACCESS_PUBLIC = 1,
    ACCESS_PROTECTED = 2
} AccessModifier;

typedef struct PersonaAttribute {
    char *name;
    SymbolType type;
    AccessModifier access;
    bool is_static;
    struct PersonaAttribute *next;
} PersonaAttribute;

typedef struct PersonaMethod {
    char *name;
    SymbolType return_type;
    char **param_names;
    SymbolType *param_types;
    int param_count;
    void *body;
    AccessModifier access;
    bool is_override;
    bool is_static;
    struct PersonaMethod *next;
} PersonaMethod;

typedef struct PersonaClass {
    char *name;
    char *parent_name;
    struct PersonaClass *next;
    PersonaAttribute *attributes;
    PersonaMethod *methods;
} PersonaClass;

typedef struct PersonaClassTable {
    PersonaClass *head;
} PersonaClassTable;

typedef struct ObjectField {
    Symbol symbol;
    AccessModifier access;
    char *declared_in_class;
    struct ObjectField *next;
} ObjectField;

typedef struct PersonaObject {
    char *name;
    PersonaClass *class_def;
    ObjectField *fields;
    struct PersonaObject *next;
} PersonaObject;

typedef struct PersonaObjectTable {
    PersonaObject *head;
} PersonaObjectTable;

PersonaClassTable *persona_class_table_create(void);
void persona_class_table_free(PersonaClassTable *table);

PersonaClass *persona_class_lookup(PersonaClassTable *table, const char *name);
PersonaClass *persona_class_lookup_parent(PersonaClassTable *table, PersonaClass *cls);

int persona_class_insert(PersonaClassTable *table, const char *name, const char *parent_name);
int persona_class_add_attribute(PersonaClass *cls, const char *name, SymbolType type,
                                AccessModifier access, bool is_static);
int persona_class_add_method(PersonaClass *cls, const char *name, SymbolType return_type,
                             char **param_names, SymbolType *param_types, int param_count,
                             void *body, AccessModifier access, bool is_override, bool is_static);

PersonaAttribute *persona_class_resolve_attribute(PersonaClassTable *table, PersonaClass *cls, const char *name);
PersonaMethod *persona_class_resolve_method(PersonaClassTable *table, PersonaClass *cls, const char *name);

PersonaObjectTable *persona_object_table_create(void);
void persona_object_table_free(PersonaObjectTable *table);
PersonaObject *persona_object_lookup(PersonaObjectTable *table, const char *name);
int persona_object_instantiate(PersonaObjectTable *objects, PersonaClassTable *classes,
                               const char *class_name, const char *object_name);
ObjectField *persona_object_field_lookup(PersonaObject *obj, const char *field_name);

#endif
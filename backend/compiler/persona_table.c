#include "persona_table.h"

#include <stdlib.h>
#include <string.h>

static void free_method(PersonaMethod *method) {
    if (!method) return;
    free(method->name);
    if (method->param_names) {
        for (int i = 0; i < method->param_count; i++) {
            free(method->param_names[i]);
        }
        free(method->param_names);
    }
    free(method->param_types);
    free(method);
}

static void free_attribute(PersonaAttribute *attr) {
    if (!attr) return;
    free(attr->name);
    free(attr);
}

static PersonaClass *persona_class_alloc(const char *name, const char *parent_name) {
    PersonaClass *cls = (PersonaClass *)calloc(1, sizeof(PersonaClass));
    if (!cls) return NULL;
    cls->name = strdup(name);
    cls->parent_name = parent_name ? strdup(parent_name) : NULL;
    return cls;
}

PersonaClassTable *persona_class_table_create(void) {
    return (PersonaClassTable *)calloc(1, sizeof(PersonaClassTable));
}

void persona_class_table_free(PersonaClassTable *table) {
    if (!table) return;
    PersonaClass *cls = table->head;
    while (cls) {
        PersonaClass *next_cls = cls->next;
        PersonaAttribute *attr = cls->attributes;
        PersonaMethod *method = cls->methods;
        free(cls->name);
        free(cls->parent_name);
        while (attr) {
            PersonaAttribute *next_attr = attr->next;
            free_attribute(attr);
            attr = next_attr;
        }
        while (method) {
            PersonaMethod *next_method = method->next;
            free_method(method);
            method = next_method;
        }
        free(cls);
        cls = next_cls;
    }
    free(table);
}

PersonaClass *persona_class_lookup(PersonaClassTable *table, const char *name) {
    if (!table || !name) return NULL;
    PersonaClass *iter = table->head;
    while (iter) {
        if (strcmp(iter->name, name) == 0) return iter;
        iter = iter->next;
    }
    return NULL;
}

PersonaClass *persona_class_lookup_parent(PersonaClassTable *table, PersonaClass *cls) {
    if (!table || !cls || !cls->parent_name) return NULL;
    return persona_class_lookup(table, cls->parent_name);
}

int persona_class_insert(PersonaClassTable *table, const char *name, const char *parent_name) {
    PersonaClass *cls;
    if (!table || !name) return 1;
    if (persona_class_lookup(table, name)) return 1;

    cls = persona_class_alloc(name, parent_name);
    if (!cls) return 2;

    cls->next = table->head;
    table->head = cls;
    return 0;
}

int persona_class_add_attribute(PersonaClass *cls, const char *name, SymbolType type,
                                AccessModifier access, bool is_static) {
    PersonaAttribute *iter;
    PersonaAttribute *attr;
    if (!cls || !name) return 1;

    iter = cls->attributes;
    while (iter) {
        if (strcmp(iter->name, name) == 0) return 1;
        iter = iter->next;
    }

    attr = (PersonaAttribute *)calloc(1, sizeof(PersonaAttribute));
    if (!attr) return 2;
    attr->name = strdup(name);
    attr->type = type;
    attr->access = access;
    attr->is_static = is_static;
    attr->next = cls->attributes;
    cls->attributes = attr;
    return 0;
}

int persona_class_add_method(PersonaClass *cls, const char *name, SymbolType return_type,
                             char **param_names, SymbolType *param_types, int param_count,
                             void *body, AccessModifier access, bool is_override, bool is_static) {
    PersonaMethod *iter;
    PersonaMethod *method;
    if (!cls || !name) return 1;

    iter = cls->methods;
    while (iter) {
        if (strcmp(iter->name, name) == 0) return 1;
        iter = iter->next;
    }

    method = (PersonaMethod *)calloc(1, sizeof(PersonaMethod));
    if (!method) return 2;

    method->name = strdup(name);
    method->return_type = return_type;
    method->param_count = param_count;
    method->body = body;
    method->access = access;
    method->is_override = is_override;
    method->is_static = is_static;

    if (param_count > 0) {
        method->param_names = (char **)calloc((size_t)param_count, sizeof(char *));
        method->param_types = (SymbolType *)calloc((size_t)param_count, sizeof(SymbolType));
        if (!method->param_names || !method->param_types) {
            free_method(method);
            return 2;
        }
        for (int i = 0; i < param_count; i++) {
            method->param_names[i] = strdup(param_names[i]);
            method->param_types[i] = param_types[i];
        }
    }

    method->next = cls->methods;
    cls->methods = method;
    return 0;
}

PersonaAttribute *persona_class_resolve_attribute(PersonaClassTable *table, PersonaClass *cls, const char *name) {
    PersonaClass *cur = cls;
    while (cur) {
        PersonaAttribute *attr = cur->attributes;
        while (attr) {
            if (strcmp(attr->name, name) == 0) return attr;
            attr = attr->next;
        }
        cur = persona_class_lookup_parent(table, cur);
    }
    return NULL;
}

PersonaMethod *persona_class_resolve_method(PersonaClassTable *table, PersonaClass *cls, const char *name) {
    PersonaClass *cur = cls;
    while (cur) {
        PersonaMethod *method = cur->methods;
        while (method) {
            if (strcmp(method->name, name) == 0) return method;
            method = method->next;
        }
        cur = persona_class_lookup_parent(table, cur);
    }
    return NULL;
}

PersonaObjectTable *persona_object_table_create(void) {
    return (PersonaObjectTable *)calloc(1, sizeof(PersonaObjectTable));
}

static void object_field_free(ObjectField *field) {
    if (!field) return;
    free(field->symbol.name);
    free(field->declared_in_class);
    if ((field->symbol.type == SYM_WORDS || field->symbol.type == SYM_EMOTION) &&
        field->symbol.has_value && field->symbol.value.str_val) {
        free(field->symbol.value.str_val);
    }
    free(field);
}

void persona_object_table_free(PersonaObjectTable *table) {
    if (!table) return;
    PersonaObject *obj = table->head;
    while (obj) {
        PersonaObject *next_obj = obj->next;
        ObjectField *field = obj->fields;
        free(obj->name);
        while (field) {
            ObjectField *next_field = field->next;
            object_field_free(field);
            field = next_field;
        }
        free(obj);
        obj = next_obj;
    }
    free(table);
}

PersonaObject *persona_object_lookup(PersonaObjectTable *table, const char *name) {
    if (!table || !name) return NULL;
    PersonaObject *iter = table->head;
    while (iter) {
        if (strcmp(iter->name, name) == 0) return iter;
        iter = iter->next;
    }
    return NULL;
}

ObjectField *persona_object_field_lookup(PersonaObject *obj, const char *field_name) {
    if (!obj || !field_name) return NULL;
    ObjectField *iter = obj->fields;
    while (iter) {
        if (strcmp(iter->symbol.name, field_name) == 0) return iter;
        iter = iter->next;
    }
    return NULL;
}

static int object_add_field(PersonaObject *obj, const char *class_name,
                            const char *name, SymbolType type, AccessModifier access) {
    ObjectField *field;
    if (!obj || !name) return 1;
    if (persona_object_field_lookup(obj, name)) return 0;

    field = (ObjectField *)calloc(1, sizeof(ObjectField));
    if (!field) return 2;
    field->symbol.name = strdup(name);
    field->symbol.type = type;
    field->symbol.has_value = false;
    field->access = access;
    field->declared_in_class = strdup(class_name ? class_name : "");
    field->next = obj->fields;
    obj->fields = field;
    return 0;
}

static int populate_fields_recursive(PersonaClassTable *classes, PersonaClass *cls, PersonaObject *obj) {
    if (!cls) return 0;
    PersonaClass *parent = persona_class_lookup_parent(classes, cls);
    if (parent) {
        int parent_result = populate_fields_recursive(classes, parent, obj);
        if (parent_result != 0) return parent_result;
    }
    PersonaAttribute *attr = cls->attributes;
    while (attr) {
        int result = object_add_field(obj, cls->name, attr->name, attr->type, attr->access);
        if (result != 0) return result;
        attr = attr->next;
    }
    return 0;
}

int persona_object_instantiate(PersonaObjectTable *objects, PersonaClassTable *classes,
                               const char *class_name, const char *object_name) {
    PersonaClass *cls;
    PersonaObject *obj;
    int result;
    if (!objects || !classes || !class_name || !object_name) return 1;
    if (persona_object_lookup(objects, object_name)) return 1;

    cls = persona_class_lookup(classes, class_name);
    if (!cls) return 1;

    obj = (PersonaObject *)calloc(1, sizeof(PersonaObject));
    if (!obj) return 2;
    obj->name = strdup(object_name);
    obj->class_def = cls;

    result = populate_fields_recursive(classes, cls, obj);
    if (result != 0) {
        free(obj->name);
        free(obj);
        return result;
    }

    obj->next = objects->head;
    objects->head = obj;
    return 0;
}
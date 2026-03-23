%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include "symbol_table.h"
#include "interpreter.h"
#include "intermediate_code.h"
#include "function_table.h"
#include "persona_table.h"

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Block Block;
typedef struct Branch Branch;
typedef struct FunctionArg FunctionArg;
typedef struct Scope Scope;
#include "emotionscript.tab.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
extern FILE *yyout;

void yyerror(const char *s);
int syntax_errors = 0;

typedef enum ValueType {
    VAL_UNDEF,
    VAL_INT,
    VAL_FLOAT,
    VAL_BOOL,
    VAL_STRING
} ValueType;

typedef struct RuntimeValue {
    ValueType type;
    union {
        int i;
        double f;
        bool b;
        char *s;
    } as;
} RuntimeValue;

typedef enum ExprKind {
    EXPR_LITERAL,
    EXPR_VAR,
    EXPR_BINARY,
    EXPR_PREFIX,
    EXPR_POSTFIX,
    EXPR_UNARY_MINUS,
    EXPR_MEMBER_ACCESS,
    EXPR_CALL_PLACEHOLDER,
    EXPR_FUNC_CALL,
    EXPR_METHOD_CALL
} ExprKind;

struct FunctionArg {
    Expr *expr;
    struct FunctionArg *next;
};

struct Expr {
    ExprKind kind;
    int op;
    char *ident;
    char *member_ident;
    RuntimeValue literal;
    Expr *left;
    Expr *right;
    
    /* For function calls */
    FunctionArg *args;
    int arg_count;
};

typedef enum StmtKind {
    ST_NOOP,
    ST_DECL,
    ST_ASSIGN,
    ST_EXPR,
    ST_IO_SPEAK,
    ST_IO_ALERT,
    ST_IO_LISTEN,
    ST_IF,
    ST_WHILE,
    ST_FOR,
    ST_BREAK,
    ST_CONTINUE,
    ST_FUNC_DECL,
    ST_FUNC_CALL,
    ST_RETURN,
    ST_PERSONA_DECL,
    ST_OBJ_ASSIGN,
    ST_METHOD_CALL
} StmtKind;

struct Branch {
    Expr *condition;
    Block *body;
    Branch *next;
};

struct Stmt {
    StmtKind kind;
    Stmt *next;

    char *name;
    char *class_name;
    char *parent_name;
    char *member_name;
    SymbolType decl_type;
    AccessModifier access;
    bool is_method;
    bool is_override;
    bool is_static;
    Expr *value_expr;

    Expr *expr;
    Expr *condition;
    Block *body;
    Branch *branches;
    Block *else_body;

    Stmt *for_init;
    Expr *for_step;
    
    /* For function declarations */
    SymbolType func_return_type;
    char **func_param_names;
    SymbolType *func_param_types;
    int func_param_count;
    Block *func_body;

    /* For persona object constructor invocation */
    FunctionArg *ctor_args;
    int ctor_arg_count;
};

struct Block {
    Stmt *head;
    Stmt *tail;
};

typedef enum ExecSignal {
    EXEC_NORMAL,
    EXEC_BREAK,
    EXEC_CONTINUE,
    EXEC_RETURN
} ExecSignal;

struct Scope {
    Symbol *local_vars[SYMBOL_TABLE_SIZE];
    RuntimeValue return_value;
    struct Scope *parent;
};

static Block *root_program = NULL;
static FunctionTable *global_function_table = NULL;
static struct Scope *current_scope = NULL;
static PersonaClassTable *global_persona_classes = NULL;
static PersonaObjectTable *global_persona_objects = NULL;

static AccessModifier temp_member_access = ACCESS_PRIVATE;
static bool temp_member_is_static = false;
static bool temp_member_is_override = false;

/* Temporary storage for function parameters during parsing */
#define MAX_PARAMS 32
static char *temp_param_names[MAX_PARAMS];
static SymbolType temp_param_types[MAX_PARAMS];
static int temp_param_count = 0;

static RuntimeValue make_undef(void) {
    RuntimeValue v;
    v.type = VAL_UNDEF;
    return v;
}

static RuntimeValue make_int(int x) {
    RuntimeValue v;
    v.type = VAL_INT;
    v.as.i = x;
    return v;
}

static RuntimeValue make_float(double x) {
    RuntimeValue v;
    v.type = VAL_FLOAT;
    v.as.f = x;
    return v;
}

static RuntimeValue make_bool(bool x) {
    RuntimeValue v;
    v.type = VAL_BOOL;
    v.as.b = x;
    return v;
}

static RuntimeValue make_string(const char *x) {
    RuntimeValue v;
    v.type = VAL_STRING;
    v.as.s = strdup(x ? x : "");
    return v;
}

static bool is_numeric(RuntimeValue v) {
    return v.type == VAL_INT || v.type == VAL_FLOAT;
}

static double as_double(RuntimeValue v) {
    if (v.type == VAL_INT) return (double)v.as.i;
    if (v.type == VAL_FLOAT) return v.as.f;
    return 0.0;
}

static bool as_bool(RuntimeValue v) {
    if (v.type == VAL_BOOL) return v.as.b;
    if (v.type == VAL_INT) return v.as.i != 0;
    if (v.type == VAL_FLOAT) return fabs(v.as.f) > 1e-12;
    if (v.type == VAL_STRING) return v.as.s && v.as.s[0] != '\0';
    return false;
}

static char *value_to_cstr(RuntimeValue v) {
    char buf[256];
    switch (v.type) {
        case VAL_INT:
            snprintf(buf, sizeof(buf), "%d", v.as.i);
            return strdup(buf);
        case VAL_FLOAT:
            snprintf(buf, sizeof(buf), "%.6f", v.as.f);
            return strdup(buf);
        case VAL_BOOL:
            return strdup(v.as.b ? "yes" : "no");
        case VAL_STRING:
            return strdup(v.as.s ? v.as.s : "");
        default:
            return strdup("<undef>");
    }
}

static void tracef(const char *fmt, ...) {
    va_list args;
    char msg[1024];

    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    interpreter_trace(yyout, "%s", msg);
}

static void semantic_error(const char *msg) {
    fprintf(yyout, "✗ Semantic Error at line %d:\n%s\n", yylineno, msg ? msg : "Unknown semantic error");
    syntax_errors++;
}

static const char *symbol_type_to_value_name(SymbolType t) {
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

static RuntimeValue symbol_to_runtime(Symbol *sym) {
    if (!sym || !sym->has_value) return make_undef();

    switch (sym->type) {
        case SYM_COUNT:
        case SYM_LEVEL:
            return make_int(sym->value.int_val);
        case SYM_MEASURE:
            return make_float(sym->value.float_val);
        case SYM_TRUTH:
            return make_bool(sym->value.bool_val);
        case SYM_WORDS:
        case SYM_EMOTION:
            return make_string(sym->value.str_val ? sym->value.str_val : "");
        default:
            return make_undef();
    }
}

static int runtime_to_symbol(Symbol *sym, RuntimeValue v) {
    SymbolValue out;

    if (!sym) return 1;

    if (sym->type == SYM_COUNT || sym->type == SYM_LEVEL) {
        if (v.type == VAL_INT) {
            out.int_val = v.as.i;
        } else if (v.type == VAL_FLOAT) {
            out.int_val = (int)v.as.f;
        } else {
            return 1;
        }
    } else if (sym->type == SYM_MEASURE) {
        if (v.type == VAL_FLOAT) {
            out.float_val = v.as.f;
        } else if (v.type == VAL_INT) {
            out.float_val = (double)v.as.i;
        } else {
            return 1;
        }
    } else if (sym->type == SYM_TRUTH) {
        out.bool_val = as_bool(v);
    } else if (sym->type == SYM_WORDS || sym->type == SYM_EMOTION) {
        char *tmp = value_to_cstr(v);
        out.str_val = tmp;
    } else {
        return 1;
    }

    /* Update the symbol directly instead of looking it up again */
    if (sym->has_value && sym->type == SYM_WORDS && sym->value.str_val) {
        free(sym->value.str_val);
        sym->value.str_val = NULL;
    }
    sym->value = out;
    sym->has_value = true;
    return 0;
}

static Expr *new_expr(void) {
    Expr *e = (Expr *)calloc(1, sizeof(Expr));
    return e;
}

static Expr *expr_literal(RuntimeValue v) {
    Expr *e = new_expr();
    e->kind = EXPR_LITERAL;
    e->literal = v;
    return e;
}

static Expr *expr_var(const char *name) {
    Expr *e = new_expr();
    e->kind = EXPR_VAR;
    e->ident = strdup(name);
    return e;
}

static Expr *expr_binary(int op, Expr *l, Expr *r) {
    Expr *e = new_expr();
    e->kind = EXPR_BINARY;
    e->op = op;
    e->left = l;
    e->right = r;
    return e;
}

static Expr *expr_prefix(int op, const char *name) {
    Expr *e = new_expr();
    e->kind = EXPR_PREFIX;
    e->op = op;
    e->ident = strdup(name);
    return e;
}

static Expr *expr_postfix(int op, const char *name) {
    Expr *e = new_expr();
    e->kind = EXPR_POSTFIX;
    e->op = op;
    e->ident = strdup(name);
    return e;
}

static Expr *expr_unary_minus(Expr *inner) {
    Expr *e = new_expr();
    e->kind = EXPR_UNARY_MINUS;
    e->left = inner;
    return e;
}

static Expr *expr_member_access(const char *object_name, const char *member_name) {
    Expr *e = new_expr();
    e->kind = EXPR_MEMBER_ACCESS;
    e->ident = strdup(object_name);
    e->member_ident = strdup(member_name);
    return e;
}

static Expr *expr_placeholder_call(void) {
    Expr *e = new_expr();
    e->kind = EXPR_CALL_PLACEHOLDER;
    return e;
}

static Expr *expr_func_call(const char *name, FunctionArg *args, int arg_count) {
    Expr *e = new_expr();
    e->kind = EXPR_FUNC_CALL;
    e->ident = strdup(name);
    e->args = args;
    e->arg_count = arg_count;
    return e;
}

static Expr *expr_method_call(const char *object_name, const char *method_name,
                              FunctionArg *args, int arg_count) {
    Expr *e = new_expr();
    e->kind = EXPR_METHOD_CALL;
    e->ident = strdup(object_name);
    e->member_ident = strdup(method_name);
    e->args = args;
    e->arg_count = arg_count;
    return e;
}

static FunctionArg *new_func_arg(Expr *expr) {
    FunctionArg *arg = (FunctionArg *)calloc(1, sizeof(FunctionArg));
    if (arg) {
        arg->expr = expr;
    }
    return arg;
}

static int count_func_args(FunctionArg *args) {
    int count = 0;
    FunctionArg *iter = args;
    while (iter) {
        count++;
        iter = iter->next;
    }
    return count;
}

static char *tac_dup(const char *s) {
    return strdup(s ? s : "");
}

static const char *tac_op_name(int op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_POWER: return "^";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LEQ: return "<=";
        case OP_GEQ: return ">=";
        default: return "?";
    }
}

static char *tac_literal(RuntimeValue v) {
    char buffer[512];
    switch (v.type) {
        case VAL_INT:
            snprintf(buffer, sizeof(buffer), "%d", v.as.i);
            return tac_dup(buffer);
        case VAL_FLOAT:
            snprintf(buffer, sizeof(buffer), "%.6f", v.as.f);
            return tac_dup(buffer);
        case VAL_BOOL:
            return tac_dup(v.as.b ? "yes" : "no");
        case VAL_STRING:
            snprintf(buffer, sizeof(buffer), "\"%s\"", v.as.s ? v.as.s : "");
            return tac_dup(buffer);
        default:
            return tac_dup("undef");
    }
}

static char *tac_emit_expr(Expr *e);

static int tac_emit_params(FunctionArg *args) {
    int count = 0;
    FunctionArg *arg = args;
    while (arg) {
        char *place;
        place = NULL;
        if (arg->expr) {
            place = tac_emit_expr(arg->expr);
            icg_emitf("PARAM %s", place);
            free(place);
        }
        count++;
        arg = arg->next;
    }
    return count;
}

static void tac_emit_stmt(Stmt *s, const char *break_label, const char *continue_label, const char *persona_name);
static void tac_emit_block(Block *b, const char *break_label, const char *continue_label, const char *persona_name);

static char *tac_emit_expr(Expr *e) {
    char *lhs;
    char *rhs;
    char *tmp;
    int argc;

    if (!e) return tac_dup("undef");

    switch (e->kind) {
        case EXPR_LITERAL:
            return tac_literal(e->literal);

        case EXPR_VAR:
            return tac_dup(e->ident);

        case EXPR_UNARY_MINUS:
            lhs = tac_emit_expr(e->left);
            tmp = icg_new_temp();
            icg_emitf("%s = - %s", tmp, lhs);
            free(lhs);
            return tmp;

        case EXPR_BINARY:
            lhs = tac_emit_expr(e->left);
            rhs = tac_emit_expr(e->right);
            tmp = icg_new_temp();
            icg_emitf("%s = %s %s %s", tmp, lhs, tac_op_name(e->op), rhs);
            free(lhs);
            free(rhs);
            return tmp;

        case EXPR_PREFIX:
            tmp = icg_new_temp();
            icg_emitf("%s = %s", tmp, e->ident);
            icg_emitf("%s = %s %s 1", e->ident, e->ident, (e->op == OP_INC ? "+" : "-"));
            free(tmp);
            return tac_dup(e->ident);

        case EXPR_POSTFIX:
            tmp = icg_new_temp();
            icg_emitf("%s = %s", tmp, e->ident);
            icg_emitf("%s = %s %s 1", e->ident, e->ident, (e->op == OP_INC ? "+" : "-"));
            return tmp;

        case EXPR_MEMBER_ACCESS:
            tmp = icg_new_temp();
            icg_emitf("%s = GET_FIELD %s.%s", tmp, e->ident, e->member_ident);
            return tmp;

        case EXPR_CALL_PLACEHOLDER:
            tmp = icg_new_temp();
            icg_emitf("%s = CALL_INTRINSIC", tmp);
            return tmp;

        case EXPR_FUNC_CALL:
            argc = tac_emit_params(e->args);
            tmp = icg_new_temp();
            icg_emitf("%s = CALL %s, %d", tmp, e->ident, argc);
            return tmp;

        case EXPR_METHOD_CALL:
            argc = tac_emit_params(e->args);
            tmp = icg_new_temp();
            icg_emitf("%s = CALL_METHOD %s.%s, %d", tmp, e->ident, e->member_ident, argc);
            return tmp;

        default:
            return tac_dup("undef");
    }
}

static void tac_emit_block(Block *b, const char *break_label, const char *continue_label, const char *persona_name) {
    Stmt *cur;
    if (!b) return;
    cur = b->head;
    while (cur) {
        tac_emit_stmt(cur, break_label, continue_label, persona_name);
        cur = cur->next;
    }
}

static void tac_emit_stmt(Stmt *s, const char *break_label, const char *continue_label, const char *persona_name) {
    char *place;

    if (!s) return;

    switch (s->kind) {
        case ST_NOOP:
            return;

        case ST_DECL:
            if (s->decl_type == SYM_UNKNOWN && s->class_name) {
                int argc;
                icg_emitf("ALLOC %s, %s", s->name, s->class_name);
                argc = tac_emit_params(s->ctor_args);
                icg_emitf("CALL_METHOD_IF_EXISTS %s.init, %d", s->name, argc);
                return;
            }

            icg_emitf("DECL %s %s", symbol_type_to_value_name(s->decl_type), s->name);
            if (s->value_expr) {
                place = tac_emit_expr(s->value_expr);
                icg_emitf("%s = %s", s->name, place);
                free(place);
            }
            return;

        case ST_ASSIGN:
            place = tac_emit_expr(s->value_expr);
            icg_emitf("%s = %s", s->name, place);
            free(place);
            return;

        case ST_OBJ_ASSIGN:
            place = tac_emit_expr(s->value_expr);
            icg_emitf("SET_FIELD %s.%s, %s", s->name, s->member_name, place);
            free(place);
            return;

        case ST_EXPR:
            place = tac_emit_expr(s->expr);
            if (place && place[0] == '$') {
                icg_emitf("DROP %s", place);
            }
            free(place);
            return;

        case ST_IO_SPEAK:
        case ST_IO_ALERT:
            place = tac_emit_expr(s->expr);
            icg_emitf("%s %s", (s->kind == ST_IO_SPEAK ? "SPEAK" : "ALERT"), place);
            free(place);
            return;

        case ST_IO_LISTEN:
            icg_emitf("LISTEN %s", s->name ? s->name : "_");
            return;

        case ST_IF: {
            Branch *br;
            char *cond;
            char *next_label = icg_new_label();
            char *end_label = icg_new_label();

            cond = tac_emit_expr(s->condition);
            icg_emitf("IF_FALSE %s GOTO %s", cond, next_label);
            free(cond);
            tac_emit_block(s->body, break_label, continue_label, persona_name);
            icg_emitf("GOTO %s", end_label);
            icg_emitf("LABEL %s", next_label);
            free(next_label);

            br = s->branches;
            while (br) {
                char *branch_next = icg_new_label();
                cond = tac_emit_expr(br->condition);
                icg_emitf("IF_FALSE %s GOTO %s", cond, branch_next);
                free(cond);
                tac_emit_block(br->body, break_label, continue_label, persona_name);
                icg_emitf("GOTO %s", end_label);
                icg_emitf("LABEL %s", branch_next);
                free(branch_next);
                br = br->next;
            }

            if (s->else_body) {
                tac_emit_block(s->else_body, break_label, continue_label, persona_name);
            }
            icg_emitf("LABEL %s", end_label);
            free(end_label);
            return;
        }

        case ST_WHILE: {
            char *start_label = icg_new_label();
            char *end_label = icg_new_label();
            char *cond = NULL;

            icg_emitf("LABEL %s", start_label);
            cond = tac_emit_expr(s->condition);
            icg_emitf("IF_FALSE %s GOTO %s", cond, end_label);
            free(cond);
            tac_emit_block(s->body, end_label, start_label, persona_name);
            icg_emitf("GOTO %s", start_label);
            icg_emitf("LABEL %s", end_label);
            free(start_label);
            free(end_label);
            return;
        }

        case ST_FOR: {
            char *start_label = icg_new_label();
            char *step_label = icg_new_label();
            char *end_label = icg_new_label();
            char *cond = NULL;

            if (s->for_init) {
                tac_emit_stmt(s->for_init, break_label, continue_label, persona_name);
            }

            icg_emitf("LABEL %s", start_label);
            if (s->condition) {
                cond = tac_emit_expr(s->condition);
                icg_emitf("IF_FALSE %s GOTO %s", cond, end_label);
                free(cond);
            }

            tac_emit_block(s->body, end_label, step_label, persona_name);
            icg_emitf("LABEL %s", step_label);
            if (s->for_step) {
                place = tac_emit_expr(s->for_step);
                free(place);
            }
            icg_emitf("GOTO %s", start_label);
            icg_emitf("LABEL %s", end_label);

            free(start_label);
            free(step_label);
            free(end_label);
            return;
        }

        case ST_BREAK:
            if (break_label) {
                icg_emitf("GOTO %s", break_label);
            }
            return;

        case ST_CONTINUE:
            if (continue_label) {
                icg_emitf("GOTO %s", continue_label);
            }
            return;

        case ST_FUNC_DECL:
            if (s->is_method) {
                icg_emitf("METHOD %s.%s:", persona_name ? persona_name : "<persona>", s->name);
                for (int i = 0; i < s->func_param_count; i++) {
                    icg_emitf("FORMAL %s", s->func_param_names[i]);
                }
                tac_emit_block(s->func_body, NULL, NULL, persona_name);
                icg_emitf("END_METHOD %s.%s", persona_name ? persona_name : "<persona>", s->name);
            } else {
                icg_emitf("FUNC %s:", s->name);
                for (int i = 0; i < s->func_param_count; i++) {
                    icg_emitf("FORMAL %s", s->func_param_names[i]);
                }
                tac_emit_block(s->func_body, NULL, NULL, persona_name);
                icg_emitf("END_FUNC %s", s->name);
            }
            return;

        case ST_FUNC_CALL:
        case ST_METHOD_CALL:
            place = tac_emit_expr(s->expr);
            if (place && place[0] == '$') {
                icg_emitf("DROP %s", place);
            }
            free(place);
            return;

        case ST_RETURN:
            place = tac_emit_expr(s->expr);
            icg_emitf("RETURN %s", place);
            free(place);
            return;

        case ST_PERSONA_DECL: {
            Stmt *member;
            if (s->parent_name) {
                icg_emitf("CLASS %s EXTENDS %s", s->class_name, s->parent_name);
            } else {
                icg_emitf("CLASS %s", s->class_name);
            }

            member = s->body ? s->body->head : NULL;
            while (member) {
                if (member->kind == ST_DECL) {
                    icg_emitf("ATTR %s %s %s%s", 
                              member->access == ACCESS_PUBLIC ? "open" :
                              (member->access == ACCESS_PROTECTED ? "guarded" : "hidden"),
                              symbol_type_to_value_name(member->decl_type),
                              member->name,
                              member->is_static ? " static" : "");
                } else if (member->kind == ST_FUNC_DECL) {
                    tac_emit_stmt(member, NULL, NULL, s->class_name);
                }
                member = member->next;
            }

            icg_emitf("END_CLASS %s", s->class_name);
            return;
        }

        default:
            return;
    }
}

static void generate_tac(Block *program) {
    icg_reset();
    icg_emit("; EmotionScript Three Address Code (TAC)");
    tac_emit_block(program, NULL, NULL, NULL);
}


/* Scope management */
static struct Scope *scope_create(struct Scope *parent) {
    struct Scope *scope = (struct Scope *)calloc(1, sizeof(struct Scope));
    if (scope) {
        scope->parent = parent;
        scope->return_value = make_undef();
        memset(scope->local_vars, 0, sizeof(scope->local_vars));
    }
    return scope;
}

static void scope_free(struct Scope *scope) {
    if (!scope) return;
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        Symbol *iter = scope->local_vars[i];
        while (iter) {
            Symbol *next = iter->next;
            free(iter->name);
            if (iter->type == SYM_WORDS && iter->has_value && iter->value.str_val) {
                free(iter->value.str_val);
            }
            free(iter);
            iter = next;
        }
    }
    free(scope);
}

static int scope_insert_symbol(struct Scope *scope, const char *name, SymbolType type) {
    if (!scope || !name) return 2;
    unsigned int h = symbol_hash(name);
    Symbol *iter = scope->local_vars[h];
    
    /* Check if already exists in this scope */
    while (iter) {
        if (strcmp(iter->name, name) == 0) {
            if (iter->type == type) return 0;
            return 1;
        }
        iter = iter->next;
    }

    Symbol *new_sym = (Symbol *)malloc(sizeof(Symbol));
    if (!new_sym) return 2;
    new_sym->name = strdup(name);
    new_sym->type = type;
    new_sym->has_value = false;
    new_sym->next = scope->local_vars[h];
    scope->local_vars[h] = new_sym;
    return 0;
}

static Symbol *scope_lookup_symbol(struct Scope *scope, const char *name) {
    if (!scope || !name) return NULL;
    
    while (scope) {
        unsigned int h = symbol_hash(name);
        Symbol *iter = scope->local_vars[h];
        while (iter) {
            if (strcmp(iter->name, name) == 0) return iter;
            iter = iter->next;
        }
        scope = scope->parent;
    }
    return NULL;
}

static Stmt *new_stmt(StmtKind kind) {
    Stmt *s = (Stmt *)calloc(1, sizeof(Stmt));
    s->kind = kind;
    return s;
}

static Block *new_block(void) {
    Block *b = (Block *)calloc(1, sizeof(Block));
    return b;
}

static Block *append_stmt(Block *b, Stmt *s) {
    if (!b) b = new_block();
    if (!s) return b;
    if (!b->head) {
        b->head = s;
        b->tail = s;
    } else {
        b->tail->next = s;
        b->tail = s;
    }
    return b;
}

static Branch *new_branch(Expr *cond, Block *body) {
    Branch *br = (Branch *)calloc(1, sizeof(Branch));
    br->condition = cond;
    br->body = body;
    return br;
}

static Branch *append_branch(Branch *head, Branch *item) {
    if (!head) return item;
    Branch *it = head;
    while (it->next) it = it->next;
    it->next = item;
    return head;
}

static RuntimeValue object_field_to_runtime(ObjectField *field) {
    if (!field) return make_undef();
    return symbol_to_runtime(&field->symbol);
}

static bool is_public_member(AccessModifier access) {
    return access == ACCESS_PUBLIC;
}

static int runtime_to_object_field(ObjectField *field, RuntimeValue v) {
    if (!field) return 1;
    return runtime_to_symbol(&field->symbol, v);
}

static void set_member_parse_context(AccessModifier access, bool is_static, bool is_override) {
    temp_member_access = access;
    temp_member_is_static = is_static;
    temp_member_is_override = is_override;
}

static void clear_member_parse_context(void) {
    temp_member_access = ACCESS_PRIVATE;
    temp_member_is_static = false;
    temp_member_is_override = false;
}

static int register_persona_stmt(Stmt *persona_stmt) {
    PersonaClass *cls;
    Stmt *member;
    int insert_result;

    if (!persona_stmt || !persona_stmt->class_name) return 1;

    if (persona_stmt->parent_name && !persona_class_lookup(global_persona_classes, persona_stmt->parent_name)) {
        semantic_error("Parent persona not declared before child persona");
        return 1;
    }

    insert_result = persona_class_insert(global_persona_classes, persona_stmt->class_name, persona_stmt->parent_name);
    if (insert_result != 0) {
        semantic_error("Persona redeclaration detected");
        return 1;
    }

    cls = persona_class_lookup(global_persona_classes, persona_stmt->class_name);
    if (!cls) {
        semantic_error("Failed to register persona");
        return 1;
    }

    member = persona_stmt->body ? persona_stmt->body->head : NULL;
    while (member) {
        if (member->kind == ST_DECL) {
            if (persona_class_add_attribute(cls, member->name, member->decl_type,
                                            member->access, member->is_static) != 0) {
                semantic_error("Duplicate persona attribute");
            }
        } else if (member->kind == ST_FUNC_DECL) {
            if (member->is_override) {
                PersonaClass *parent_cls = persona_class_lookup_parent(global_persona_classes, cls);
                PersonaMethod *parent_method = persona_class_resolve_method(global_persona_classes, parent_cls, member->name);
                if (!parent_method) {
                    semantic_error("reshape method does not match any inherited method");
                }
            }
            if (persona_class_add_method(cls, member->name, member->func_return_type,
                                         member->func_param_names, member->func_param_types,
                                         member->func_param_count, member->func_body,
                                         member->access, member->is_override, member->is_static) != 0) {
                semantic_error("Duplicate persona method");
            }
        }
        member = member->next;
    }

    tracef("persona %s registered%s", cls->name, cls->parent_name ? " with inheritance" : "");
    return 0;
}

static RuntimeValue invoke_persona_method(PersonaObject *obj, const char *method_name,
                                          FunctionArg *args, int arg_count);

static int invoke_persona_constructor(PersonaObject *obj, FunctionArg *ctor_args, int ctor_arg_count);

static RuntimeValue eval_expr(Expr *e);
static ExecSignal execute_stmt(Stmt *s);
static ExecSignal execute_block(Block *b);

static RuntimeValue apply_cmp(int op, RuntimeValue a, RuntimeValue b) {
    if (is_numeric(a) && is_numeric(b)) {
        double av = as_double(a);
        double bv = as_double(b);
        switch (op) {
            case OP_EQ: return make_bool(fabs(av - bv) < 1e-12);
            case OP_NEQ: return make_bool(fabs(av - bv) >= 1e-12);
            case OP_LT: return make_bool(av < bv);
            case OP_GT: return make_bool(av > bv);
            case OP_LEQ: return make_bool(av <= bv);
            case OP_GEQ: return make_bool(av >= bv);
            default: return make_undef();
        }
    }

    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        int cmp = strcmp(a.as.s ? a.as.s : "", b.as.s ? b.as.s : "");
        switch (op) {
            case OP_EQ: return make_bool(cmp == 0);
            case OP_NEQ: return make_bool(cmp != 0);
            default: return make_undef();
        }
    }

    if (a.type == VAL_BOOL && b.type == VAL_BOOL) {
        switch (op) {
            case OP_EQ: return make_bool(a.as.b == b.as.b);
            case OP_NEQ: return make_bool(a.as.b != b.as.b);
            default: return make_undef();
        }
    }

    return make_undef();
}



static RuntimeValue call_function(const char *func_name, FunctionArg *args, int arg_count);

/* Forward declarations for mutual recursion */
static ExecSignal execute_stmt(Stmt *s);
static ExecSignal execute_block(Block *b);

static RuntimeValue eval_expr(Expr *e) {
    RuntimeValue lhs;
    RuntimeValue rhs;

    if (!e) return make_undef();

    switch (e->kind) {
        case EXPR_LITERAL:
            return e->literal;

        case EXPR_VAR: {
            Symbol *sym = NULL;
            if (current_scope) {
                sym = scope_lookup_symbol(current_scope, e->ident);
            }
            if (!sym) {
                sym = lookup_symbol(e->ident);
            }
            if (!sym) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Variable '%s' used before declaration", e->ident);
                semantic_error(msg);
                return make_undef();
            }
            return symbol_to_runtime(sym);
        }

        case EXPR_UNARY_MINUS:
            lhs = eval_expr(e->left);
            if (lhs.type == VAL_INT) return make_int(-lhs.as.i);
            if (lhs.type == VAL_FLOAT) return make_float(-lhs.as.f);
            semantic_error("Unary minus requires numeric operand");
            return make_undef();

        case EXPR_MEMBER_ACCESS: {
            PersonaObject *obj = persona_object_lookup(global_persona_objects, e->ident);
            ObjectField *field;
            if (!obj) {
                char msg[160];
                snprintf(msg, sizeof(msg), "Object '%s' is not declared", e->ident);
                semantic_error(msg);
                return make_undef();
            }
            field = persona_object_field_lookup(obj, e->member_ident);
            if (!field) {
                char msg[160];
                snprintf(msg, sizeof(msg), "Member '%s.%s' not found", e->ident, e->member_ident);
                semantic_error(msg);
                return make_undef();
            }
            if (!is_public_member(field->access)) {
                char msg[192];
                snprintf(msg, sizeof(msg), "Cannot access private member '%s.%s'", e->ident, e->member_ident);
                semantic_error(msg);
                return make_undef();
            }
            return object_field_to_runtime(field);
        }

        case EXPR_PREFIX:
        case EXPR_POSTFIX: {
            Symbol *sym = current_scope ? scope_lookup_symbol(current_scope, e->ident) : NULL;
            RuntimeValue cur;
            RuntimeValue updated;

            if (!sym) {
                sym = lookup_symbol(e->ident);
            }

            if (!sym) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Variable '%s' used before declaration", e->ident);
                semantic_error(msg);
                return make_undef();
            }

            cur = symbol_to_runtime(sym);
            if (!is_numeric(cur)) {
                semantic_error("Increment/decrement requires numeric variable");
                return make_undef();
            }

            if (cur.type == VAL_INT) {
                updated = make_int(cur.as.i + (e->op == OP_INC ? 1 : -1));
            } else {
                updated = make_float(cur.as.f + (e->op == OP_INC ? 1.0 : -1.0));
            }

            if (runtime_to_symbol(sym, updated) != 0) {
                semantic_error("Failed to update symbol during increment/decrement");
            } else {
                char *vtxt = value_to_cstr(updated);
                tracef("%s %s => %s", (e->op == OP_INC ? "inc" : "dec"), e->ident, vtxt);
                free(vtxt);
            }

            return (e->kind == EXPR_PREFIX) ? updated : cur;
        }

        case EXPR_BINARY:
            lhs = eval_expr(e->left);
            rhs = eval_expr(e->right);

            switch (e->op) {
                case OP_PLUS:
                    if (is_numeric(lhs) && is_numeric(rhs)) {
                        if (lhs.type == VAL_FLOAT || rhs.type == VAL_FLOAT) {
                            return make_float(as_double(lhs) + as_double(rhs));
                        }
                        return make_int(lhs.as.i + rhs.as.i);
                    }
                    if (lhs.type == VAL_STRING || rhs.type == VAL_STRING) {
                        char *ls = value_to_cstr(lhs);
                        char *rs = value_to_cstr(rhs);
                        size_t n = strlen(ls) + strlen(rs) + 1;
                        char *buf = (char *)malloc(n);
                        RuntimeValue out;
                        if (!buf) {
                            free(ls);
                            free(rs);
                            return make_undef();
                        }
                        strcpy(buf, ls);
                        strcat(buf, rs);
                        out = make_string(buf);
                        free(buf);
                        free(ls);
                        free(rs);
                        return out;
                    }
                    semantic_error("Plus operator type mismatch");
                    return make_undef();

                case OP_MINUS:
                    if (is_numeric(lhs) && is_numeric(rhs)) {
                        if (lhs.type == VAL_FLOAT || rhs.type == VAL_FLOAT) {
                            return make_float(as_double(lhs) - as_double(rhs));
                        }
                        return make_int(lhs.as.i - rhs.as.i);
                    }
                    semantic_error("Minus operator type mismatch");
                    return make_undef();

                case OP_MUL:
                    if (is_numeric(lhs) && is_numeric(rhs)) {
                        if (lhs.type == VAL_FLOAT || rhs.type == VAL_FLOAT) {
                            return make_float(as_double(lhs) * as_double(rhs));
                        }
                        return make_int(lhs.as.i * rhs.as.i);
                    }
                    semantic_error("Multiply operator requires numeric operands");
                    return make_undef();

                case OP_DIV:
                    if (is_numeric(lhs) && is_numeric(rhs)) {
                        double den = as_double(rhs);
                        if (fabs(den) < 1e-12) {
                            semantic_error("Division by zero");
                            return make_undef();
                        }
                        return make_float(as_double(lhs) / den);
                    }
                    semantic_error("Division operator requires numeric operands");
                    return make_undef();

                case OP_MOD:
                    if (lhs.type == VAL_INT && rhs.type == VAL_INT) {
                        if (rhs.as.i == 0) {
                            semantic_error("Modulo by zero");
                            return make_undef();
                        }
                        return make_int(lhs.as.i % rhs.as.i);
                    }
                    semantic_error("Modulo requires count operands");
                    return make_undef();

                case OP_POWER:
                    if (is_numeric(lhs) && is_numeric(rhs)) {
                        return make_float(pow(as_double(lhs), as_double(rhs)));
                    }
                    semantic_error("Power operator requires numeric operands");
                    return make_undef();

                case OP_EQ:
                case OP_NEQ:
                case OP_LT:
                case OP_GT:
                case OP_LEQ:
                case OP_GEQ:
                    return apply_cmp(e->op, lhs, rhs);

                default:
                    return make_undef();
            }

        case EXPR_CALL_PLACEHOLDER:
            tracef("function call expression evaluated as <undef>");
            return make_undef();

        case EXPR_FUNC_CALL:
            return call_function(e->ident, e->args, e->arg_count);

        case EXPR_METHOD_CALL: {
            PersonaObject *obj = persona_object_lookup(global_persona_objects, e->ident);
            PersonaMethod *method;
            if (!obj) {
                char msg[160];
                snprintf(msg, sizeof(msg), "Object '%s' is not declared", e->ident);
                semantic_error(msg);
                return make_undef();
            }
            method = persona_class_resolve_method(global_persona_classes, obj->class_def, e->member_ident);
            if (!method) {
                char msg[192];
                snprintf(msg, sizeof(msg), "Method '%s.%s' not found", e->ident, e->member_ident);
                semantic_error(msg);
                return make_undef();
            }
            if (!is_public_member(method->access)) {
                char msg[192];
                snprintf(msg, sizeof(msg), "Cannot invoke private method '%s.%s'", e->ident, e->member_ident);
                semantic_error(msg);
                return make_undef();
            }
            return invoke_persona_method(obj, e->member_ident, e->args, e->arg_count);
        }

        default:
            return make_undef();
    }
}

static RuntimeValue call_function(const char *func_name, FunctionArg *args, int arg_count) {
    FunctionDef *func = func_table_lookup(global_function_table, func_name);
    if (!func) {
        char msg[192];
        snprintf(msg, sizeof(msg), "Invalid function call: function '%s' is not declared", func_name ? func_name : "<unknown>");
        semantic_error(msg);
        return make_undef();
    }

    if (arg_count != func->param_count) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Function '%s' expects %d arguments, got %d", 
                 func_name, func->param_count, arg_count);
        semantic_error(msg);
        return make_undef();
    }

    /* Create new scope for function execution */
    struct Scope *func_scope = scope_create(current_scope);
    struct Scope *saved_scope = current_scope;
    current_scope = func_scope;

    /* Bind parameters */
    FunctionArg *arg = args;
    for (int i = 0; i < func->param_count; i++) {
        if (!arg) break;
        RuntimeValue arg_val = eval_expr(arg->expr);
        scope_insert_symbol(func_scope, func->param_names[i], func->param_types[i]);
        Symbol *param_sym = scope_lookup_symbol(func_scope, func->param_names[i]);
        if (param_sym) {
            if (runtime_to_symbol(param_sym, arg_val) == 0) {
                param_sym->has_value = true;
                char *vtxt = value_to_cstr(arg_val);
                tracef("bind parameter %s = %s", func->param_names[i], vtxt);
                free(vtxt);
            }
        }
        arg = arg->next;
    }

    /* Execute function body */
    RuntimeValue result = make_undef();
    if (func->body) {
        ExecSignal sig = execute_block((Block *)func->body);
        if (sig == EXEC_RETURN) {
            result = func_scope->return_value;
        }
    }

    /* Restore scope */
    current_scope = saved_scope;
    scope_free(func_scope);

    return result;
}

static RuntimeValue invoke_persona_method(PersonaObject *obj, const char *method_name,
                                          FunctionArg *args, int arg_count) {
    PersonaMethod *method;
    struct Scope *method_scope;
    struct Scope *saved_scope;
    RuntimeValue result = make_undef();

    if (!obj || !method_name) return make_undef();

    method = persona_class_resolve_method(global_persona_classes, obj->class_def, method_name);
    if (!method) {
        char msg[192];
        snprintf(msg, sizeof(msg), "Invalid method call: method '%s.%s' is not defined", obj->name, method_name);
        semantic_error(msg);
        return make_undef();
    }

    if (arg_count != method->param_count) {
        char msg[192];
        snprintf(msg, sizeof(msg), "Invalid method call: method '%s.%s' expects %d argument(s), got %d",
                 obj->name, method_name, method->param_count, arg_count);
        semantic_error(msg);
        return make_undef();
    }

    method_scope = scope_create(current_scope);
    saved_scope = current_scope;
    current_scope = method_scope;

    scope_insert_symbol(method_scope, "self", SYM_WORDS);
    {
        Symbol *self_sym = scope_lookup_symbol(method_scope, "self");
        if (self_sym) {
            RuntimeValue self_val = make_string(obj->name);
            (void)runtime_to_symbol(self_sym, self_val);
        }
    }

    {
        ObjectField *field = obj->fields;
        while (field) {
            scope_insert_symbol(method_scope, field->symbol.name, field->symbol.type);
            {
                Symbol *local_field = scope_lookup_symbol(method_scope, field->symbol.name);
                if (local_field && field->symbol.has_value) {
                    RuntimeValue fv = symbol_to_runtime(&field->symbol);
                    (void)runtime_to_symbol(local_field, fv);
                }
            }
            field = field->next;
        }
    }

    {
        FunctionArg *arg = args;
        for (int i = 0; i < method->param_count; i++) {
            RuntimeValue arg_val;
            Symbol *param_sym;
            if (!arg) break;
            arg_val = eval_expr(arg->expr);
            scope_insert_symbol(method_scope, method->param_names[i], method->param_types[i]);
            param_sym = scope_lookup_symbol(method_scope, method->param_names[i]);
            if (param_sym) {
                if (runtime_to_symbol(param_sym, arg_val) != 0) {
                    semantic_error("Incorrect method/constructor parameter type");
                    current_scope = saved_scope;
                    scope_free(method_scope);
                    return make_undef();
                }
            }
            arg = arg->next;
        }
    }

    if (method->body) {
        ExecSignal sig = execute_block((Block *)method->body);
        if (sig == EXEC_RETURN) {
            result = method_scope->return_value;
        }
    }

    {
        ObjectField *field = obj->fields;
        while (field) {
            Symbol *local_field = scope_lookup_symbol(method_scope, field->symbol.name);
            if (local_field && local_field->has_value) {
                RuntimeValue new_val = symbol_to_runtime(local_field);
                (void)runtime_to_object_field(field, new_val);
            }
            field = field->next;
        }
    }

    current_scope = saved_scope;
    scope_free(method_scope);

    return result;
}

static ExecSignal execute_stmt(Stmt *s) {
    RuntimeValue v;
    Symbol *sym;
    char *txt;
    int iteration;

    if (!s) return EXEC_NORMAL;

    switch (s->kind) {
        case ST_NOOP:
            return EXEC_NORMAL;

        case ST_DECL:
            if (s->decl_type == SYM_UNKNOWN && s->class_name) {
                if (persona_object_instantiate(global_persona_objects, global_persona_classes,
                                              s->class_name, s->name) != 0) {
                    char msg[192];
                    snprintf(msg, sizeof(msg), "Failed to instantiate object '%s' of persona '%s'",
                             s->name, s->class_name);
                    semantic_error(msg);
                } else {
                    PersonaObject *obj = persona_object_lookup(global_persona_objects, s->name);
                    if (!obj || invoke_persona_constructor(obj, s->ctor_args, s->ctor_arg_count) != 0) {
                        return EXEC_NORMAL;
                    }
                    tracef("instantiate %s as %s", s->name, s->class_name);
                }
                return EXEC_NORMAL;
            }
            if (current_scope) {
                if (scope_insert_symbol(current_scope, s->name, s->decl_type) != 0) {
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Redeclaration/type conflict for '%s'", s->name);
                    semantic_error(msg);
                }
            } else if (insert_symbol(s->name, s->decl_type) != 0) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Redeclaration/type conflict for '%s'", s->name);
                semantic_error(msg);
            }
            tracef("declare %s %s", symbol_type_to_value_name(s->decl_type), s->name);

            if (s->value_expr) {
                sym = current_scope ? scope_lookup_symbol(current_scope, s->name) : NULL;
                if (!sym) {
                    sym = lookup_symbol(s->name);
                }
                v = eval_expr(s->value_expr);
                if (!sym || runtime_to_symbol(sym, v) != 0) {
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Type mismatch in initialization of '%s'", s->name);
                    semantic_error(msg);
                } else {
                    txt = value_to_cstr(v);
                    tracef("init %s = %s", s->name, txt);
                    free(txt);
                }
            }
            return EXEC_NORMAL;

        case ST_ASSIGN:
            sym = current_scope ? scope_lookup_symbol(current_scope, s->name) : NULL;
            if (!sym) {
                sym = lookup_symbol(s->name);
            }
            if (!sym) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Variable '%s' used before declaration", s->name);
                semantic_error(msg);
                return EXEC_NORMAL;
            }
            v = eval_expr(s->value_expr);
            if (runtime_to_symbol(sym, v) != 0) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Type mismatch in assignment to '%s'", s->name);
                semantic_error(msg);
            } else {
                txt = value_to_cstr(v);
                tracef("assign %s = %s", s->name, txt);
                free(txt);
            }
            return EXEC_NORMAL;

        case ST_OBJ_ASSIGN: {
            PersonaObject *obj = persona_object_lookup(global_persona_objects, s->name);
            ObjectField *field;
            if (!obj) {
                char msg[160];
                snprintf(msg, sizeof(msg), "Object '%s' is not declared", s->name);
                semantic_error(msg);
                return EXEC_NORMAL;
            }
            field = persona_object_field_lookup(obj, s->member_name);
            if (!field) {
                char msg[192];
                snprintf(msg, sizeof(msg), "Attribute '%s.%s' does not exist", s->name, s->member_name);
                semantic_error(msg);
                return EXEC_NORMAL;
            }
            if (!is_public_member(field->access)) {
                char msg[192];
                snprintf(msg, sizeof(msg), "Cannot access private member '%s.%s'", s->name, s->member_name);
                semantic_error(msg);
                return EXEC_NORMAL;
            }
            v = eval_expr(s->value_expr);
            if (runtime_to_object_field(field, v) != 0) {
                semantic_error("Type mismatch in object member assignment");
            } else {
                txt = value_to_cstr(v);
                tracef("assign %s.%s = %s", s->name, s->member_name, txt);
                free(txt);
            }
            return EXEC_NORMAL;
        }

        case ST_EXPR:
            (void)eval_expr(s->expr);
            return EXEC_NORMAL;

        case ST_IO_SPEAK:
        case ST_IO_ALERT:
            v = eval_expr(s->expr);
            txt = value_to_cstr(v);
            if (s->kind == ST_IO_SPEAK) {
                fprintf(yyout, "[SPEAK] %s\n", txt);
            } else {
                fprintf(yyout, "[ALERT] %s\n", txt);
            }
            free(txt);
            return EXEC_NORMAL;

        case ST_IO_LISTEN:
            tracef("listen(%s) skipped in non-interactive execution", s->name ? s->name : "?");
            return EXEC_NORMAL;

        case ST_IF: {
            Branch *br;
            ExecSignal sig;

            v = eval_expr(s->condition);
            tracef("if_feel condition => %s", as_bool(v) ? "true" : "false");
            if (as_bool(v)) {
                return execute_block(s->body);
            }

            br = s->branches;
            while (br) {
                v = eval_expr(br->condition);
                tracef("else_if_feel condition => %s", as_bool(v) ? "true" : "false");
                if (as_bool(v)) {
                    sig = execute_block(br->body);
                    return sig;
                }
                br = br->next;
            }

            if (s->else_body) {
                tracef("else_feel branch taken");
                return execute_block(s->else_body);
            }
            return EXEC_NORMAL;
        }

        case ST_WHILE:
            iteration = 0;
            while (1) {
                ExecSignal sig;
                v = eval_expr(s->condition);
                tracef("ruminate check #%d => %s", iteration, as_bool(v) ? "true" : "false");
                if (!as_bool(v)) break;

                sig = execute_block(s->body);
                if (sig == EXEC_BREAK) {
                    tracef("break_free exits ruminate");
                    break;
                }
                if (sig == EXEC_CONTINUE) {
                    tracef("continue_flow next ruminate iteration");
                    iteration++;
                    continue;
                }
                iteration++;
            }
            return EXEC_NORMAL;

        case ST_FOR:
            iteration = 0;
            if (s->for_init) {
                (void)execute_stmt(s->for_init);
            }
            while (1) {
                ExecSignal sig;

                if (s->condition) {
                    v = eval_expr(s->condition);
                    tracef("replay check #%d => %s", iteration, as_bool(v) ? "true" : "false");
                    if (!as_bool(v)) break;
                }

                sig = execute_block(s->body);
                if (sig == EXEC_BREAK) {
                    tracef("break_free exits replay");
                    break;
                }

                if (s->for_step) {
                    (void)eval_expr(s->for_step);
                }

                if (sig == EXEC_CONTINUE) {
                    tracef("continue_flow next replay iteration");
                }

                iteration++;
            }
            return EXEC_NORMAL;

        case ST_BREAK:
            tracef("break_free encountered");
            return EXEC_BREAK;

        case ST_CONTINUE:
            tracef("continue_flow encountered");
            return EXEC_CONTINUE;

        case ST_FUNC_DECL: {
            if (s->is_method) {
                return EXEC_NORMAL;
            }
            if (!global_function_table) {
                global_function_table = func_table_create();
            }
            
            int result = func_table_insert(global_function_table, s->name, s->func_return_type,
                                          s->func_param_names, s->func_param_types,
                                          s->func_param_count, s->func_body);
            
            if (result == 0) {
                tracef("function %s declared", s->name);
            } else {
                semantic_error("Failed to register function");
            }
            return EXEC_NORMAL;
        }

        case ST_PERSONA_DECL:
            (void)register_persona_stmt(s);
            return EXEC_NORMAL;

        case ST_RETURN: {
            RuntimeValue ret_val = eval_expr(s->expr);
            if (current_scope) {
                current_scope->return_value = ret_val;
            }
            char *vtxt = value_to_cstr(ret_val);
            tracef("reflect %s", vtxt);
            free(vtxt);
            return EXEC_RETURN;
        }

        case ST_FUNC_CALL: {
            if (s->expr && s->expr->kind == EXPR_FUNC_CALL) {
                RuntimeValue result = eval_expr(s->expr);
                char *vtxt = value_to_cstr(result);
                tracef("invoke %s => %s", s->name, vtxt);
                free(vtxt);
            }
            return EXEC_NORMAL;
        }

        case ST_METHOD_CALL: {
            if (s->expr && s->expr->kind == EXPR_METHOD_CALL) {
                RuntimeValue result = eval_expr(s->expr);
                char *vtxt = value_to_cstr(result);
                tracef("invoke %s.%s => %s", s->name, s->member_name, vtxt);
                free(vtxt);
            }
            return EXEC_NORMAL;
        }

        default:
            return EXEC_NORMAL;
    }
}

static ExecSignal execute_block(Block *b) {
    Stmt *cur;
    if (!b) return EXEC_NORMAL;

    cur = b->head;
    while (cur) {
        ExecSignal sig = execute_stmt(cur);
        if (sig != EXEC_NORMAL) {
            return sig;
        }
        cur = cur->next;
    }
    return EXEC_NORMAL;
}

static int invoke_persona_constructor(PersonaObject *obj, FunctionArg *ctor_args, int ctor_arg_count) {
    PersonaMethod *ctor;
    int errors_before;
    if (!obj || !obj->class_def) return 1;

    ctor = persona_class_resolve_method(global_persona_classes, obj->class_def, "init");
    if (!ctor) {
        if (ctor_arg_count > 0) {
            char msg[192];
            snprintf(msg, sizeof(msg), "Missing constructor init for '%s' with %d argument(s)",
                     obj->class_def->name, ctor_arg_count);
            semantic_error(msg);
            return 1;
        }
        return 0;
    }

    if (ctor_arg_count != ctor->param_count) {
        char msg[192];
        snprintf(msg, sizeof(msg), "Incorrect constructor parameters for '%s': expected %d, got %d",
                 obj->class_def->name, ctor->param_count, ctor_arg_count);
        semantic_error(msg);
        return 1;
    }

    errors_before = syntax_errors;
    (void)invoke_persona_method(obj, "init", ctor_args, ctor_arg_count);
    if (syntax_errors > errors_before) {
        return 1;
    }
    return 0;
}
%}

/* Program structure */
%error-verbose

%token PROGRAM_MIND PROGRAM_AWAKE PROGRAM_SLEEP PROGRAM_ABORT PROGRAM_SCENE

/* Data types */
%token TYPE_COUNT TYPE_MEASURE TYPE_TRUTH TYPE_WORDS TYPE_EMOTION TYPE_LEVEL

/* Variable storage classes */
%token VAR_THOUGHT VAR_MEMORY VAR_TRAIT VAR_STATE

/* Assignment */
%token ASSIGN_SET

/* I/O */
%token IO_SPEAK IO_LISTEN IO_ALERT

/* Functions */
%token FUNC_DECLARE FUNC_END FUNC_RETURNS FUNC_RETURN FUNC_CALL

/* Conditionals */
%token COND_IF COND_ELSE_IF COND_ELSE COND_END COND_SWITCH COND_DEFAULT

/* New switch-case tokens */
%token DECIDE_KW WHEN_KW OTHERWISE_KW END_DECISION_KW

/* Loops */
%token LOOP_WHILE LOOP_END LOOP_FOR LOOP_BREAK LOOP_CONTINUE
%token WHILE_FEELING_KW CALM_KW

/* FSM */
%token FSM_STATES FSM_EVENT FSM_CURRENT FSM_TRANSITION FSM_WHEN

/* Classes/Persona */
%token CLASS_PERSONA CLASS_END CLASS_BODY CLASS_PUBLIC CLASS_PROTECTED
%token CLASS_STATIC CLASS_INHERIT CLASS_OVERRIDE
%token CLASS_OPEN CLASS_GUARDED CLASS_HIDDEN
%token DELIM_LSHIFT DELIM_RSHIFT

/* Math functions */
%token MATH_ABS MATH_SQRT MATH_CEIL MATH_FLOOR MATH_DIFF MATH_NORM MATH_POW
%token MATH_SIN MATH_COS MATH_TAN MATH_ASIN MATH_ACOS MATH_ATAN

/* Operators */
%token OP_ARROW OP_EQ OP_NEQ OP_LEQ OP_GEQ OP_LT OP_GT
%token OP_INC OP_DEC OP_PLUS OP_MINUS OP_MUL OP_DIV OP_MOD OP_POWER

/* Delimiters */
%token DELIM_LPAREN DELIM_RPAREN DELIM_LBRACE DELIM_RBRACE
%token DELIM_SEMICOLON DELIM_COMMA DELIM_COLON DELIM_DOT

/* Literals and identifiers */
%token <string_val> LIT_STRING LIT_INT LIT_FLOAT LIT_BOOL IDENTIFIER

%type <symbol_type> type_specifier
%type <expr_ptr> expression primary_expression condition math_function
%type <stmt_ptr> statement declaration_stmt assignment_stmt expression_stmt
%type <stmt_ptr> conditional_stmt loop_stmt io_stmt return_stmt control_flow_stmt
%type <stmt_ptr> function_declaration switch_stmt new_switch_stmt new_while_stmt
%type <stmt_ptr> persona_declaration persona_member
%type <block_ptr> program_body statement_list function_body
%type <block_ptr> parameter_list persona_body persona_member_list
%type <block_ptr> new_case_blocks new_case_block new_default_block switch_body
%type <func_arg_ptr> argument_list
%type <branch_ptr> else_if_chain
%type <int_val> access_modifier
%type <string_val> callable_name

/* Operator precedence and associativity */
%nonassoc BARE_ID
%left OP_EQ OP_NEQ OP_LT OP_GT OP_LEQ OP_GEQ
%left OP_PLUS OP_MINUS
%left OP_MUL OP_DIV OP_MOD
%right OP_POWER
%right OP_INC OP_DEC
%left DELIM_DOT
%nonassoc DELIM_LPAREN

%union {
    char *string_val;
    int int_val;
    double float_val;
    int symbol_type;
    struct Expr *expr_ptr;
    struct Stmt *stmt_ptr;
    struct Block *block_ptr;
    struct Branch *branch_ptr;
    struct FunctionArg *func_arg_ptr;
}

%start program

%%

program:
    PROGRAM_MIND IDENTIFIER PROGRAM_AWAKE program_body PROGRAM_SLEEP
    {
        root_program = $4;
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    }
    | PROGRAM_MIND IDENTIFIER PROGRAM_AWAKE program_body PROGRAM_SLEEP DELIM_SEMICOLON
    {
        root_program = $4;
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    }
    ;

program_body:
    /* empty */ { $$ = new_block(); }
    | statement_list { $$ = $1; }
    ;

statement_list:
    statement { $$ = append_stmt(new_block(), $1); }
    | statement_list statement { $$ = append_stmt($1, $2); }
    ;

statement:
    declaration_stmt { $$ = $1; }
    | assignment_stmt { $$ = $1; }
    | expression_stmt { $$ = $1; }
    | function_declaration { $$ = $1; }
    | conditional_stmt { $$ = $1; }
    | loop_stmt { $$ = $1; }
    | io_stmt { $$ = $1; }
    | return_stmt { $$ = $1; }
    | control_flow_stmt { $$ = $1; }
    | persona_declaration { $$ = $1; }
    | fsm_declaration { $$ = new_stmt(ST_NOOP); }
    | emotion_based_stmt { $$ = new_stmt(ST_NOOP); }
    | FUNC_CALL IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN DELIM_SEMICOLON {
        Stmt *s = new_stmt(ST_FUNC_CALL);
        s->name = strdup($2);
        s->expr = expr_func_call($2, $4, count_func_args($4));
        $$ = s;
    }
    | FUNC_CALL IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_SEMICOLON {
        Stmt *s = new_stmt(ST_FUNC_CALL);
        s->name = strdup($2);
        s->expr = expr_func_call($2, NULL, 0);
        $$ = s;
    }
    | FUNC_CALL IDENTIFIER DELIM_DOT callable_name DELIM_LPAREN argument_list DELIM_RPAREN DELIM_SEMICOLON {
        Stmt *s = new_stmt(ST_METHOD_CALL);
        s->name = strdup($2);
        s->member_name = strdup($4);
        s->expr = expr_method_call($2, $4, $6, count_func_args($6));
        $$ = s;
    }
    | FUNC_CALL IDENTIFIER DELIM_DOT callable_name DELIM_LPAREN DELIM_RPAREN DELIM_SEMICOLON {
        Stmt *s = new_stmt(ST_METHOD_CALL);
        s->name = strdup($2);
        s->member_name = strdup($4);
        s->expr = expr_method_call($2, $4, NULL, 0);
        $$ = s;
    }
    | type_specifier IDENTIFIER error {
        yyerror("Missing ';' after declaration");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | ASSIGN_SET IDENTIFIER OP_ARROW expression error {
        yyerror("Missing ';' after assignment");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | ASSIGN_SET IDENTIFIER DELIM_DOT IDENTIFIER OP_ARROW expression error {
        yyerror("Missing ';' after member assignment");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | FUNC_RETURN expression error {
        yyerror("Missing ';' after reflect statement");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | IO_SPEAK DELIM_LPAREN expression DELIM_RPAREN error {
        yyerror("Missing ';' after speak(...) statement");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | IO_LISTEN DELIM_LPAREN IDENTIFIER DELIM_RPAREN error {
        yyerror("Missing ';' after listen(...) statement");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | IO_ALERT DELIM_LPAREN expression DELIM_RPAREN error {
        yyerror("Missing ';' after alert(...) statement");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | expression error {
        yyerror("Missing ';' after expression statement");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    | PROGRAM_ABORT DELIM_SEMICOLON { $$ = new_stmt(ST_NOOP); }
    | PROGRAM_SCENE IDENTIFIER DELIM_SEMICOLON { $$ = new_stmt(ST_NOOP); }
    | error DELIM_SEMICOLON { yyerrok; $$ = new_stmt(ST_NOOP); }
    | error recovery_sync {
        yyerror("Recovered from invalid statement using panic-mode synchronization");
        yyerrok;
        $$ = new_stmt(ST_NOOP);
    }
    ;

recovery_sync:
    COND_END
    | LOOP_END
    | DELIM_RSHIFT
    | DELIM_RBRACE
    | PROGRAM_SLEEP
    ;

declaration_stmt:
    type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$1;
        s->name = strdup($2);
        $$ = s;
    }
    | IDENTIFIER IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup($2);
        s->class_name = strdup($1);
        s->ctor_args = NULL;
        s->ctor_arg_count = 0;
        $$ = s;
    }
    | IDENTIFIER IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup($2);
        s->class_name = strdup($1);
        s->ctor_args = $4;
        s->ctor_arg_count = count_func_args($4);
        $$ = s;
    }
    | IDENTIFIER IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup($2);
        s->class_name = strdup($1);
        s->ctor_args = NULL;
        s->ctor_arg_count = 0;
        $$ = s;
    }
    | variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$2;
        s->name = strdup($3);
        $$ = s;
    }
    | type_specifier IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$1;
        s->name = strdup($2);
        s->value_expr = $4;
        $$ = s;
    }
    ;

type_specifier:
    TYPE_COUNT  { $$ = SYM_COUNT; }
    | TYPE_MEASURE { $$ = SYM_MEASURE; }
    | TYPE_TRUTH { $$ = SYM_TRUTH; }
    | TYPE_WORDS { $$ = SYM_WORDS; }
    | TYPE_LEVEL { $$ = SYM_LEVEL; }
    | TYPE_EMOTION { $$ = SYM_EMOTION; }
    | IDENTIFIER { $$ = SYM_UNKNOWN; }
    ;

variable_modifier:
    VAR_THOUGHT
    | VAR_MEMORY
    | VAR_TRAIT
    | VAR_STATE
    ;

assignment_stmt:
    ASSIGN_SET IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_ASSIGN);
        s->name = strdup($2);
        s->value_expr = $4;
        $$ = s;
    }
    | ASSIGN_SET IDENTIFIER DELIM_DOT IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_OBJ_ASSIGN);
        s->name = strdup($2);
        s->member_name = strdup($4);
        s->value_expr = $6;
        $$ = s;
    }
    ;

expression_stmt:
    expression DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_EXPR);
        s->expr = $1;
        $$ = s;
    }
    ;

expression:
    primary_expression { $$ = $1; }
    | expression OP_POWER expression { $$ = expr_binary(OP_POWER, $1, $3); }
    | expression OP_MUL expression { $$ = expr_binary(OP_MUL, $1, $3); }
    | expression OP_DIV expression { $$ = expr_binary(OP_DIV, $1, $3); }
    | expression OP_MOD expression { $$ = expr_binary(OP_MOD, $1, $3); }
    | expression OP_PLUS expression { $$ = expr_binary(OP_PLUS, $1, $3); }
    | expression OP_MINUS expression { $$ = expr_binary(OP_MINUS, $1, $3); }
    | expression OP_EQ expression { $$ = expr_binary(OP_EQ, $1, $3); }
    | expression OP_NEQ expression { $$ = expr_binary(OP_NEQ, $1, $3); }
    | expression OP_LT expression { $$ = expr_binary(OP_LT, $1, $3); }
    | expression OP_GT expression { $$ = expr_binary(OP_GT, $1, $3); }
    | expression OP_LEQ expression { $$ = expr_binary(OP_LEQ, $1, $3); }
    | expression OP_GEQ expression { $$ = expr_binary(OP_GEQ, $1, $3); }
    | OP_INC IDENTIFIER { $$ = expr_prefix(OP_INC, $2); }
    | OP_DEC IDENTIFIER { $$ = expr_prefix(OP_DEC, $2); }
    | OP_MINUS expression { $$ = expr_unary_minus($2); }
    ;

primary_expression:
    IDENTIFIER %prec BARE_ID { $$ = expr_var($1); }
    | IDENTIFIER OP_INC { $$ = expr_postfix(OP_INC, $1); }
    | IDENTIFIER OP_DEC { $$ = expr_postfix(OP_DEC, $1); }
    | IDENTIFIER DELIM_DOT callable_name { $$ = expr_member_access($1, $3); }
    | IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN {
        $$ = expr_func_call($1, $3, count_func_args($3));
    }
    | IDENTIFIER DELIM_LPAREN DELIM_RPAREN {
        $$ = expr_func_call($1, NULL, 0);
    }
    | FUNC_CALL IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN {
        $$ = expr_func_call($2, $4, count_func_args($4));
    }
    | FUNC_CALL IDENTIFIER DELIM_LPAREN DELIM_RPAREN {
        $$ = expr_func_call($2, NULL, 0);
    }
    | LIT_INT { $$ = expr_literal(make_int(atoi($1))); }
    | LIT_FLOAT { $$ = expr_literal(make_float(strtod($1, NULL))); }
    | LIT_STRING
      {
          size_t n = strlen($1);
          if (n >= 2 && $1[0] == '"' && $1[n - 1] == '"') {
              char *inner = (char *)malloc(n - 1);
              if (inner) {
                  memcpy(inner, $1 + 1, n - 2);
                  inner[n - 2] = '\0';
                  $$ = expr_literal(make_string(inner));
                  free(inner);
              } else {
                  $$ = expr_literal(make_string(""));
              }
          } else {
              $$ = expr_literal(make_string($1));
          }
      }
    | LIT_BOOL { $$ = expr_literal(make_bool(strcmp($1, "yes") == 0)); }
    | DELIM_LPAREN expression DELIM_RPAREN { $$ = $2; }
    | math_function { $$ = $1; }
    | FSM_CURRENT { $$ = expr_placeholder_call(); }
    ;

function_declaration:
    FUNC_DECLARE callable_name DELIM_LPAREN parameter_list DELIM_RPAREN FUNC_RETURNS type_specifier DELIM_LSHIFT function_body DELIM_RSHIFT
    {
        Stmt *s = new_stmt(ST_FUNC_DECL);
        s->name = strdup($2);
        s->access = temp_member_access;
        s->is_static = temp_member_is_static;
        s->is_override = temp_member_is_override;
        s->is_method = false;
        s->func_return_type = (SymbolType)$7;
        s->func_param_count = temp_param_count;
        
        if (temp_param_count > 0) {
            s->func_param_names = (char **)malloc(temp_param_count * sizeof(char *));
            s->func_param_types = (SymbolType *)malloc(temp_param_count * sizeof(SymbolType));
            for (int i = 0; i < temp_param_count; i++) {
                s->func_param_names[i] = temp_param_names[i];
                s->func_param_types[i] = temp_param_types[i];
            }
        } else {
            s->func_param_names = NULL;
            s->func_param_types = NULL;
        }
        s->func_body = $9;
        temp_param_count = 0;
        clear_member_parse_context();
        $$ = s;
    }
    | FUNC_DECLARE callable_name DELIM_LPAREN DELIM_RPAREN FUNC_RETURNS type_specifier DELIM_LSHIFT function_body DELIM_RSHIFT
    {
        Stmt *s = new_stmt(ST_FUNC_DECL);
        s->name = strdup($2);
        s->access = temp_member_access;
        s->is_static = temp_member_is_static;
        s->is_override = temp_member_is_override;
        s->is_method = false;
        s->func_return_type = (SymbolType)$6;
        s->func_param_names = NULL;
        s->func_param_types = NULL;
        s->func_param_count = 0;
        s->func_body = $8;
        temp_param_count = 0;
        clear_member_parse_context();
        $$ = s;
    }
    ;

parameter_list:
    parameter { $$ = new_block(); }
    | parameter_list DELIM_COMMA parameter { $$ = $1; }
    ;

parameter:
    type_specifier IDENTIFIER
    {
        if (temp_param_count < MAX_PARAMS) {
            temp_param_names[temp_param_count] = strdup($2);
            temp_param_types[temp_param_count] = (SymbolType)$1;
            temp_param_count++;
        }
    }
    ;

function_body:
    statement_list { $$ = $1; }
    ;

return_stmt:
    FUNC_RETURN expression DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_RETURN);
        s->expr = $2;
        $$ = s;
    }
    ;

argument_list:
    expression { 
        $$ = new_func_arg($1);
    }
    | argument_list DELIM_COMMA expression { 
        FunctionArg *arg = new_func_arg($3);
        FunctionArg *tail = $1;
        while (tail->next) tail = tail->next;
        tail->next = arg;
        $$ = $1;
    }
    ;

math_function:
    MATH_SIN DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_COS DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_TAN DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_ASIN DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_ACOS DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_ATAN DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_ABS DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_SQRT DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_CEIL DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_FLOOR DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_NORM DELIM_LPAREN expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_POW DELIM_LPAREN expression DELIM_COMMA expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    | MATH_DIFF DELIM_LPAREN expression DELIM_COMMA expression DELIM_RPAREN { $$ = expr_placeholder_call(); }
    ;

conditional_stmt:
    COND_IF condition statement_list COND_END
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = $2;
        s->body = $3;
        $$ = s;
    }
    | COND_IF condition statement_list else_if_chain COND_END
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = $2;
        s->body = $3;
        s->branches = $4;
        $$ = s;
    }
    | COND_IF condition statement_list COND_ELSE statement_list COND_END
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = $2;
        s->body = $3;
        s->else_body = $5;
        $$ = s;
    }
    | COND_IF condition statement_list else_if_chain COND_ELSE statement_list COND_END
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = $2;
        s->body = $3;
        s->branches = $4;
        s->else_body = $6;
        $$ = s;
    }
    | switch_stmt { $$ = $1; }
    | new_switch_stmt { $$ = $1; }
    ;

condition:
    expression { $$ = $1; }
    ;

else_if_chain:
    COND_ELSE_IF condition statement_list
    {
        $$ = new_branch($2, $3);
    }
    | else_if_chain COND_ELSE_IF condition statement_list
    {
        $$ = append_branch($1, new_branch($3, $4));
    }
    ;

switch_stmt:
    DECIDE_KW DELIM_LPAREN expression DELIM_RPAREN DELIM_LBRACE switch_body DELIM_RBRACE
    {
        (void)$3;
        (void)$6;
        $$ = new_stmt(ST_NOOP);
    }
    ;

switch_body:
    switch_body_item { $$ = new_block(); }
    | switch_body switch_body_item { $$ = $1; }
    ;

switch_body_item:
    expression DELIM_COLON
    | OTHERWISE_KW DELIM_COLON
    | statement
    ;

new_switch_stmt:
    DECIDE_KW expression new_case_blocks END_DECISION_KW DELIM_DOT
    {
        (void)$2;
        (void)$3;
        $$ = new_stmt(ST_NOOP);
    }
    ;

new_case_blocks:
    new_case_block { $$ = $1; }
    | new_case_blocks new_case_block { $$ = $1; }
    | new_case_blocks new_default_block { $$ = $1; }
    ;

new_case_block:
    WHEN_KW expression statement_list { $$ = $3; }
    ;

new_default_block:
    OTHERWISE_KW statement_list { $$ = $2; }
    ;

loop_stmt:
    LOOP_WHILE condition statement_list LOOP_END
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = $2;
        s->body = $3;
        $$ = s;
    }
    | LOOP_FOR DELIM_LPAREN assignment_stmt expression DELIM_SEMICOLON expression DELIM_RPAREN statement_list LOOP_END
    {
        Stmt *s = new_stmt(ST_FOR);
        s->for_init = $3;
        s->condition = $4;
        s->for_step = $6;
        s->body = $8;
        $$ = s;
    }
    | new_while_stmt { $$ = $1; }
    ;

new_while_stmt:
    WHILE_FEELING_KW DELIM_LPAREN condition DELIM_RPAREN statement_list CALM_KW
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = $3;
        s->body = $5;
        $$ = s;
    }
    ;

control_flow_stmt:
    LOOP_BREAK DELIM_SEMICOLON { $$ = new_stmt(ST_BREAK); }
    | LOOP_CONTINUE DELIM_SEMICOLON { $$ = new_stmt(ST_CONTINUE); }
    ;

io_stmt:
    IO_SPEAK DELIM_LPAREN expression DELIM_RPAREN DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_IO_SPEAK);
        s->expr = $3;
        $$ = s;
    }
    | IO_LISTEN DELIM_LPAREN IDENTIFIER DELIM_RPAREN DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_IO_LISTEN);
        s->name = strdup($3);
        $$ = s;
    }
    | IO_ALERT DELIM_LPAREN expression DELIM_RPAREN DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_IO_ALERT);
        s->expr = $3;
        $$ = s;
    }
    ;

callable_name:
    IDENTIFIER { $$ = strdup($1); }
    | IO_SPEAK { $$ = strdup("speak"); }
    | IO_LISTEN { $$ = strdup("listen"); }
    | IO_ALERT { $$ = strdup("alert"); }
    ;

persona_declaration:
    CLASS_PERSONA IDENTIFIER DELIM_LSHIFT persona_body DELIM_RSHIFT
    {
        Stmt *s = new_stmt(ST_PERSONA_DECL);
        s->class_name = strdup($2);
        s->body = $4;
        $$ = s;
    }
    | CLASS_PERSONA IDENTIFIER CLASS_INHERIT IDENTIFIER DELIM_LSHIFT persona_body DELIM_RSHIFT
    {
        Stmt *s = new_stmt(ST_PERSONA_DECL);
        s->class_name = strdup($2);
        s->parent_name = strdup($4);
        s->body = $6;
        $$ = s;
    }
    ;

persona_body:
    /* empty */ { $$ = new_block(); }
    | persona_member_list { $$ = $1; }
    ;

persona_member_list:
    persona_member { $$ = append_stmt(new_block(), $1); }
    | persona_member_list persona_member { $$ = append_stmt($1, $2); }
    ;

persona_member:
    access_modifier variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$3;
        s->name = strdup($4);
        s->access = (AccessModifier)$1;
        $$ = s;
    }
    | access_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$2;
        s->name = strdup($3);
        s->access = (AccessModifier)$1;
        $$ = s;
    }
    | variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$2;
        s->name = strdup($3);
        s->access = ACCESS_PRIVATE;
        $$ = s;
    }
    | type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$1;
        s->name = strdup($2);
        s->access = ACCESS_PRIVATE;
        $$ = s;
    }
    | access_modifier
      {
          set_member_parse_context((AccessModifier)$1, false, false);
      }
      function_declaration
      {
          $3->access = (AccessModifier)$1;
          $3->is_method = true;
          $$ = $3;
      }
    | function_declaration
      {
          $1->access = ACCESS_PRIVATE;
          $1->is_method = true;
          $$ = $1;
      }
    | CLASS_OVERRIDE
      {
          set_member_parse_context(ACCESS_PRIVATE, false, true);
      }
      function_declaration
      {
          $3->is_override = true;
          $3->is_method = true;
          $$ = $3;
      }
    | CLASS_STATIC type_specifier IDENTIFIER DELIM_SEMICOLON
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)$2;
        s->name = strdup($3);
        s->access = ACCESS_PRIVATE;
        s->is_static = true;
        $$ = s;
    }
    ;

access_modifier:
    CLASS_OPEN { $$ = ACCESS_PUBLIC; }
    | CLASS_GUARDED { $$ = ACCESS_PROTECTED; }
    | CLASS_HIDDEN { $$ = ACCESS_PRIVATE; }
    ;

fsm_declaration:
    FSM_TRANSITION IDENTIFIER WHEN_KW condition DELIM_SEMICOLON
    | FSM_EVENT IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_LBRACE statement_list DELIM_RBRACE
    | FSM_EVENT IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_LSHIFT statement_list DELIM_RSHIFT
    ;

emotion_based_stmt:
    TYPE_EMOTION IDENTIFIER DELIM_SEMICOLON
    | TYPE_EMOTION IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    | TYPE_EMOTION IDENTIFIER FSM_STATES DELIM_LBRACE state_list DELIM_RBRACE DELIM_SEMICOLON
    ;

state_list:
    IDENTIFIER
    | state_list DELIM_COMMA IDENTIFIER
    ;

%%

void yyerror(const char *s) {
    fprintf(yyout, "✗ Syntax Error at line %d:\n%s\n", yylineno, s ? s : "Invalid syntax");
    if (yytext && yytext[0] != '\0') {
        fprintf(yyout, "Near token: '%s'\n", yytext);
    }
    syntax_errors++;
}

int main(int argc, char **argv) {
    int result;
    const char *tac_path;

    icg_reset();
    
    /* Initialize global scope for functions */
    global_function_table = func_table_create();
    current_scope = scope_create(NULL);
    global_persona_classes = persona_class_table_create();
    global_persona_objects = persona_object_table_create();

    if (argc < 3) {
        printf("Usage: %s <input.tokens> <output.syntax>\n", argv[0]);
        printf("  OR\n");
        printf("Usage: %s <input.ems> <output.syntax>\n", argv[0]);
        printf("Optional: %s <input.ems> <output.syntax> <output.tac>\n", argv[0]);
        return 1;
    }

    tac_path = (argc >= 4) ? argv[3] : "output.tac";

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    if (!yyin || !yyout) {
        printf("Error: Cannot open files\n");
        return 1;
    }

    fprintf(yyout, "=== EmotionScript Syntax + Execution Engine ===\n\n");

    yylineno = 1;
    result = yyparse();

    if (result == 0 && syntax_errors == 0) {
        fprintf(yyout, "\n=== PARSE: SUCCESS ===\n");
        fprintf(yyout, "✓ No syntax errors found\n");
        fprintf(yyout, "✓ Program structure is valid\n");

        generate_tac(root_program);
        if (icg_write_to_file(tac_path) == 0) {
            fprintf(yyout, "✓ TAC written to %s\n", tac_path);
        } else {
            fprintf(yyout, "✗ Failed to write TAC to %s\n", tac_path);
        }

        fprintf(yyout, "\n=== EXECUTION TRACE ===\n");
        (void)execute_block(root_program);
        fprintf(yyout, "=== EXECUTION COMPLETE ===\n");
    } else {
        fprintf(yyout, "\n=== PARSE: FAILED ===\n");
        fprintf(yyout, "✗ Found %d syntax error(s)\n", syntax_errors);
        fprintf(yyout, "✗ Please fix the errors and try again\n");
    }

    fclose(yyin);
    fclose(yyout);
    free_symbol_table();
    if (global_function_table) func_table_free(global_function_table);
    if (current_scope) scope_free(current_scope);
    if (global_persona_objects) persona_object_table_free(global_persona_objects);
    if (global_persona_classes) persona_class_table_free(global_persona_classes);

    return result;
}

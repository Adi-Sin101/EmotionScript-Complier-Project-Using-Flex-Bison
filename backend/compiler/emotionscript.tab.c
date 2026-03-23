
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "emotionscript.y"

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


/* Line 189 of yacc.c  */
#line 1839 "emotionscript.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM_MIND = 258,
     PROGRAM_AWAKE = 259,
     PROGRAM_SLEEP = 260,
     PROGRAM_ABORT = 261,
     PROGRAM_SCENE = 262,
     TYPE_COUNT = 263,
     TYPE_MEASURE = 264,
     TYPE_TRUTH = 265,
     TYPE_WORDS = 266,
     TYPE_EMOTION = 267,
     TYPE_LEVEL = 268,
     VAR_THOUGHT = 269,
     VAR_MEMORY = 270,
     VAR_TRAIT = 271,
     VAR_STATE = 272,
     ASSIGN_SET = 273,
     IO_SPEAK = 274,
     IO_LISTEN = 275,
     IO_ALERT = 276,
     FUNC_DECLARE = 277,
     FUNC_END = 278,
     FUNC_RETURNS = 279,
     FUNC_RETURN = 280,
     FUNC_CALL = 281,
     COND_IF = 282,
     COND_ELSE_IF = 283,
     COND_ELSE = 284,
     COND_END = 285,
     COND_SWITCH = 286,
     COND_DEFAULT = 287,
     DECIDE_KW = 288,
     WHEN_KW = 289,
     OTHERWISE_KW = 290,
     END_DECISION_KW = 291,
     LOOP_WHILE = 292,
     LOOP_END = 293,
     LOOP_FOR = 294,
     LOOP_BREAK = 295,
     LOOP_CONTINUE = 296,
     WHILE_FEELING_KW = 297,
     CALM_KW = 298,
     FSM_STATES = 299,
     FSM_EVENT = 300,
     FSM_CURRENT = 301,
     FSM_TRANSITION = 302,
     FSM_WHEN = 303,
     CLASS_PERSONA = 304,
     CLASS_END = 305,
     CLASS_BODY = 306,
     CLASS_PUBLIC = 307,
     CLASS_PROTECTED = 308,
     CLASS_STATIC = 309,
     CLASS_INHERIT = 310,
     CLASS_OVERRIDE = 311,
     CLASS_OPEN = 312,
     CLASS_GUARDED = 313,
     CLASS_HIDDEN = 314,
     DELIM_LSHIFT = 315,
     DELIM_RSHIFT = 316,
     MATH_ABS = 317,
     MATH_SQRT = 318,
     MATH_CEIL = 319,
     MATH_FLOOR = 320,
     MATH_DIFF = 321,
     MATH_NORM = 322,
     MATH_POW = 323,
     MATH_SIN = 324,
     MATH_COS = 325,
     MATH_TAN = 326,
     MATH_ASIN = 327,
     MATH_ACOS = 328,
     MATH_ATAN = 329,
     OP_ARROW = 330,
     OP_EQ = 331,
     OP_NEQ = 332,
     OP_LEQ = 333,
     OP_GEQ = 334,
     OP_LT = 335,
     OP_GT = 336,
     OP_INC = 337,
     OP_DEC = 338,
     OP_PLUS = 339,
     OP_MINUS = 340,
     OP_MUL = 341,
     OP_DIV = 342,
     OP_MOD = 343,
     OP_POWER = 344,
     DELIM_LPAREN = 345,
     DELIM_RPAREN = 346,
     DELIM_LBRACE = 347,
     DELIM_RBRACE = 348,
     DELIM_SEMICOLON = 349,
     DELIM_COMMA = 350,
     DELIM_COLON = 351,
     DELIM_DOT = 352,
     LIT_STRING = 353,
     LIT_INT = 354,
     LIT_FLOAT = 355,
     LIT_BOOL = 356,
     IDENTIFIER = 357,
     BARE_ID = 358
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 1844 "emotionscript.y"

    char *string_val;
    int int_val;
    double float_val;
    int symbol_type;
    struct Expr *expr_ptr;
    struct Stmt *stmt_ptr;
    struct Block *block_ptr;
    struct Branch *branch_ptr;
    struct FunctionArg *func_arg_ptr;



/* Line 214 of yacc.c  */
#line 1992 "emotionscript.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 2004 "emotionscript.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3160

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  174
/* YYNRULES -- Number of states.  */
#define YYNSTATES  412

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   358

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    16,    17,    19,    21,    24,    26,
      28,    30,    32,    34,    36,    38,    40,    42,    44,    46,
      48,    55,    61,    70,    78,    82,    88,    96,   100,   106,
     112,   118,   121,   124,   128,   131,   134,   136,   138,   140,
     142,   144,   148,   152,   159,   165,   170,   176,   178,   180,
     182,   184,   186,   188,   190,   192,   194,   196,   198,   204,
     212,   215,   217,   221,   225,   229,   233,   237,   241,   245,
     249,   253,   257,   261,   265,   268,   271,   274,   276,   279,
     282,   286,   291,   295,   301,   306,   308,   310,   312,   314,
     318,   320,   322,   333,   343,   345,   349,   352,   354,   358,
     360,   364,   369,   374,   379,   384,   389,   394,   399,   404,
     409,   414,   419,   426,   433,   438,   444,   451,   459,   461,
     463,   465,   469,   474,   482,   484,   487,   490,   493,   495,
     501,   503,   506,   509,   513,   516,   521,   531,   533,   540,
     543,   546,   552,   558,   564,   566,   568,   570,   572,   578,
     586,   587,   589,   591,   594,   600,   605,   610,   614,   615,
     619,   621,   622,   626,   631,   633,   635,   637,   643,   651,
     659,   663,   669,   677,   679
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     105,     0,    -1,     3,   102,     4,   106,     5,    -1,     3,
     102,     4,   106,     5,    94,    -1,    -1,   107,    -1,   108,
      -1,   107,   108,    -1,   110,    -1,   113,    -1,   114,    -1,
     117,    -1,   124,    -1,   134,    -1,   137,    -1,   121,    -1,
     136,    -1,   139,    -1,   146,    -1,   147,    -1,    26,   102,
      90,   122,    91,    94,    -1,    26,   102,    90,    91,    94,
      -1,    26,   102,    97,   138,    90,   122,    91,    94,    -1,
      26,   102,    97,   138,    90,    91,    94,    -1,   111,   102,
       1,    -1,    18,   102,    75,   115,     1,    -1,    18,   102,
      97,   102,    75,   115,     1,    -1,    25,   115,     1,    -1,
      19,    90,   115,    91,     1,    -1,    20,    90,   102,    91,
       1,    -1,    21,    90,   115,    91,     1,    -1,   115,     1,
      -1,     6,    94,    -1,     7,   102,    94,    -1,     1,    94,
      -1,     1,   109,    -1,    30,    -1,    38,    -1,    61,    -1,
      93,    -1,     5,    -1,   111,   102,    94,    -1,   102,   102,
      94,    -1,   102,   102,    90,   122,    91,    94,    -1,   102,
     102,    90,    91,    94,    -1,   112,   111,   102,    94,    -1,
     111,   102,    75,   115,    94,    -1,     8,    -1,     9,    -1,
      10,    -1,    11,    -1,    13,    -1,    12,    -1,   102,    -1,
      14,    -1,    15,    -1,    16,    -1,    17,    -1,    18,   102,
      75,   115,    94,    -1,    18,   102,    97,   102,    75,   115,
      94,    -1,   115,    94,    -1,   116,    -1,   115,    89,   115,
      -1,   115,    86,   115,    -1,   115,    87,   115,    -1,   115,
      88,   115,    -1,   115,    84,   115,    -1,   115,    85,   115,
      -1,   115,    76,   115,    -1,   115,    77,   115,    -1,   115,
      80,   115,    -1,   115,    81,   115,    -1,   115,    78,   115,
      -1,   115,    79,   115,    -1,    82,   102,    -1,    83,   102,
      -1,    85,   115,    -1,   102,    -1,   102,    82,    -1,   102,
      83,    -1,   102,    97,   138,    -1,   102,    90,   122,    91,
      -1,   102,    90,    91,    -1,    26,   102,    90,   122,    91,
      -1,    26,   102,    90,    91,    -1,    99,    -1,   100,    -1,
      98,    -1,   101,    -1,    90,   115,    91,    -1,   123,    -1,
      46,    -1,    22,   138,    90,   118,    91,    24,   111,    60,
     120,    61,    -1,    22,   138,    90,    91,    24,   111,    60,
     120,    61,    -1,   119,    -1,   118,    95,   119,    -1,   111,
     102,    -1,   107,    -1,    25,   115,    94,    -1,   115,    -1,
     122,    95,   115,    -1,    69,    90,   115,    91,    -1,    70,
      90,   115,    91,    -1,    71,    90,   115,    91,    -1,    72,
      90,   115,    91,    -1,    73,    90,   115,    91,    -1,    74,
      90,   115,    91,    -1,    62,    90,   115,    91,    -1,    63,
      90,   115,    91,    -1,    64,    90,   115,    91,    -1,    65,
      90,   115,    91,    -1,    67,    90,   115,    91,    -1,    68,
      90,   115,    95,   115,    91,    -1,    66,    90,   115,    95,
     115,    91,    -1,    27,   125,   107,    30,    -1,    27,   125,
     107,   126,    30,    -1,    27,   125,   107,    29,   107,    30,
      -1,    27,   125,   107,   126,    29,   107,    30,    -1,   127,
      -1,   130,    -1,   115,    -1,    28,   125,   107,    -1,   126,
      28,   125,   107,    -1,    33,    90,   115,    91,    92,   128,
      93,    -1,   129,    -1,   128,   129,    -1,   115,    96,    -1,
      35,    96,    -1,   108,    -1,    33,   115,   131,    36,    97,
      -1,   132,    -1,   131,   132,    -1,   131,   133,    -1,    34,
     115,   107,    -1,    35,   107,    -1,    37,   125,   107,    38,
      -1,    39,    90,   113,   115,    94,   115,    91,   107,    38,
      -1,   135,    -1,    42,    90,   125,    91,   107,    43,    -1,
      40,    94,    -1,    41,    94,    -1,    19,    90,   115,    91,
      94,    -1,    20,    90,   102,    91,    94,    -1,    21,    90,
     115,    91,    94,    -1,   102,    -1,    19,    -1,    20,    -1,
      21,    -1,    49,   102,    60,   140,    61,    -1,    49,   102,
      55,   102,    60,   140,    61,    -1,    -1,   141,    -1,   142,
      -1,   141,   142,    -1,   145,   112,   111,   102,    94,    -1,
     145,   111,   102,    94,    -1,   112,   111,   102,    94,    -1,
     111,   102,    94,    -1,    -1,   145,   143,   117,    -1,   117,
      -1,    -1,    56,   144,   117,    -1,    54,   111,   102,    94,
      -1,    57,    -1,    58,    -1,    59,    -1,    47,   102,    34,
     125,    94,    -1,    45,   102,    90,    91,    92,   107,    93,
      -1,    45,   102,    90,    91,    60,   107,    61,    -1,    12,
     102,    94,    -1,    12,   102,    75,   115,    94,    -1,    12,
     102,    44,    92,   148,    93,    94,    -1,   102,    -1,   148,
      95,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1861,  1861,  1866,  1874,  1875,  1879,  1880,  1884,  1885,
    1886,  1887,  1888,  1889,  1890,  1891,  1892,  1893,  1894,  1895,
    1896,  1902,  1908,  1915,  1922,  1927,  1932,  1937,  1942,  1947,
    1952,  1957,  1962,  1963,  1964,  1965,  1973,  1974,  1975,  1976,
    1977,  1981,  1988,  1998,  2008,  2018,  2025,  2036,  2037,  2038,
    2039,  2040,  2041,  2042,  2046,  2047,  2048,  2049,  2053,  2060,
    2071,  2080,  2081,  2082,  2083,  2084,  2085,  2086,  2087,  2088,
    2089,  2090,  2091,  2092,  2093,  2094,  2095,  2099,  2100,  2101,
    2102,  2103,  2106,  2109,  2112,  2115,  2116,  2117,  2134,  2135,
    2136,  2137,  2141,  2168,  2188,  2189,  2193,  2204,  2208,  2217,
    2220,  2230,  2231,  2232,  2233,  2234,  2235,  2236,  2237,  2238,
    2239,  2240,  2241,  2242,  2246,  2253,  2261,  2269,  2278,  2279,
    2283,  2287,  2291,  2298,  2307,  2308,  2312,  2313,  2314,  2318,
    2327,  2328,  2329,  2333,  2337,  2341,  2348,  2357,  2361,  2371,
    2372,  2376,  2382,  2388,  2397,  2398,  2399,  2400,  2404,  2411,
    2422,  2423,  2427,  2428,  2432,  2440,  2448,  2456,  2465,  2464,
    2474,  2481,  2480,  2490,  2502,  2503,  2504,  2508,  2509,  2510,
    2514,  2515,  2516,  2520,  2521
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM_MIND", "PROGRAM_AWAKE",
  "PROGRAM_SLEEP", "PROGRAM_ABORT", "PROGRAM_SCENE", "TYPE_COUNT",
  "TYPE_MEASURE", "TYPE_TRUTH", "TYPE_WORDS", "TYPE_EMOTION", "TYPE_LEVEL",
  "VAR_THOUGHT", "VAR_MEMORY", "VAR_TRAIT", "VAR_STATE", "ASSIGN_SET",
  "IO_SPEAK", "IO_LISTEN", "IO_ALERT", "FUNC_DECLARE", "FUNC_END",
  "FUNC_RETURNS", "FUNC_RETURN", "FUNC_CALL", "COND_IF", "COND_ELSE_IF",
  "COND_ELSE", "COND_END", "COND_SWITCH", "COND_DEFAULT", "DECIDE_KW",
  "WHEN_KW", "OTHERWISE_KW", "END_DECISION_KW", "LOOP_WHILE", "LOOP_END",
  "LOOP_FOR", "LOOP_BREAK", "LOOP_CONTINUE", "WHILE_FEELING_KW", "CALM_KW",
  "FSM_STATES", "FSM_EVENT", "FSM_CURRENT", "FSM_TRANSITION", "FSM_WHEN",
  "CLASS_PERSONA", "CLASS_END", "CLASS_BODY", "CLASS_PUBLIC",
  "CLASS_PROTECTED", "CLASS_STATIC", "CLASS_INHERIT", "CLASS_OVERRIDE",
  "CLASS_OPEN", "CLASS_GUARDED", "CLASS_HIDDEN", "DELIM_LSHIFT",
  "DELIM_RSHIFT", "MATH_ABS", "MATH_SQRT", "MATH_CEIL", "MATH_FLOOR",
  "MATH_DIFF", "MATH_NORM", "MATH_POW", "MATH_SIN", "MATH_COS", "MATH_TAN",
  "MATH_ASIN", "MATH_ACOS", "MATH_ATAN", "OP_ARROW", "OP_EQ", "OP_NEQ",
  "OP_LEQ", "OP_GEQ", "OP_LT", "OP_GT", "OP_INC", "OP_DEC", "OP_PLUS",
  "OP_MINUS", "OP_MUL", "OP_DIV", "OP_MOD", "OP_POWER", "DELIM_LPAREN",
  "DELIM_RPAREN", "DELIM_LBRACE", "DELIM_RBRACE", "DELIM_SEMICOLON",
  "DELIM_COMMA", "DELIM_COLON", "DELIM_DOT", "LIT_STRING", "LIT_INT",
  "LIT_FLOAT", "LIT_BOOL", "IDENTIFIER", "BARE_ID", "$accept", "program",
  "program_body", "statement_list", "statement", "recovery_sync",
  "declaration_stmt", "type_specifier", "variable_modifier",
  "assignment_stmt", "expression_stmt", "expression", "primary_expression",
  "function_declaration", "parameter_list", "parameter", "function_body",
  "return_stmt", "argument_list", "math_function", "conditional_stmt",
  "condition", "else_if_chain", "switch_stmt", "switch_body",
  "switch_body_item", "new_switch_stmt", "new_case_blocks",
  "new_case_block", "new_default_block", "loop_stmt", "new_while_stmt",
  "control_flow_stmt", "io_stmt", "callable_name", "persona_declaration",
  "persona_body", "persona_member_list", "persona_member", "$@1", "$@2",
  "access_modifier", "fsm_declaration", "emotion_based_stmt", "state_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   109,   109,   109,
     109,   110,   110,   110,   110,   110,   110,   111,   111,   111,
     111,   111,   111,   111,   112,   112,   112,   112,   113,   113,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   117,   117,   118,   118,   119,   120,   121,   122,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   124,   124,   124,   124,   124,   124,
     125,   126,   126,   127,   128,   128,   129,   129,   129,   130,
     131,   131,   131,   132,   133,   134,   134,   134,   135,   136,
     136,   137,   137,   137,   138,   138,   138,   138,   139,   139,
     140,   140,   141,   141,   142,   142,   142,   142,   143,   142,
     142,   144,   142,   142,   145,   145,   145,   146,   146,   146,
     147,   147,   147,   148,   148
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     6,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     5,     8,     7,     3,     5,     7,     3,     5,     5,
       5,     2,     2,     3,     2,     2,     1,     1,     1,     1,
       1,     3,     3,     6,     5,     4,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     5,     7,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     1,     2,     2,
       3,     4,     3,     5,     4,     1,     1,     1,     1,     3,
       1,     1,    10,     9,     1,     3,     2,     1,     3,     1,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     6,     6,     4,     5,     6,     7,     1,     1,
       1,     3,     4,     7,     1,     2,     2,     2,     1,     5,
       1,     2,     2,     3,     2,     4,     9,     1,     6,     2,
       2,     5,     5,     5,     1,     1,     1,     1,     5,     7,
       0,     1,     1,     2,     5,     4,     4,     3,     0,     3,
       1,     0,     3,     4,     1,     1,     1,     5,     7,     7,
       3,     5,     7,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,    47,
      48,    49,    50,     0,    51,    54,    55,    56,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    85,    86,    88,    77,     0,
       0,     6,     8,     0,     0,     9,    10,     0,    61,    11,
      15,    90,    12,   118,   119,    13,   137,    16,    14,    17,
      18,    19,    40,    36,    37,    38,    39,    34,    35,    32,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   144,
       0,     0,    77,     0,     0,   120,     0,     0,     0,     0,
       0,   139,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,     0,    78,    79,     0,     0,     0,     2,
       7,     0,    52,    53,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    33,
       0,     0,   170,     0,     0,     0,     0,     0,     0,     0,
      27,    98,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,     0,     0,     0,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    82,    99,     0,    80,     0,    42,     3,    24,     0,
      41,     0,    68,    69,    72,    73,    70,    71,    66,    67,
      63,    64,    65,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,    84,     0,     0,     0,
       0,   114,     0,    89,     0,     0,     0,   131,   132,   135,
       0,     0,     0,     0,     0,     0,     0,   161,   164,   165,
     166,     0,     0,   160,     0,   151,   152,   158,   107,   108,
     109,   110,     0,   111,     0,   101,   102,   103,   104,   105,
     106,    81,     0,     0,     0,     0,    45,   173,     0,   171,
      25,    58,     0,    28,   141,    29,   142,    30,   143,     0,
      96,     0,     0,    84,     0,    21,    83,     0,     0,     0,
       0,     0,   115,     0,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,   167,   150,     0,     0,     0,     0,
     148,   153,     0,     0,     0,     0,     0,   100,    44,     0,
      46,     0,     0,     0,     0,     0,    95,    83,    20,     0,
       0,     0,   116,     0,     0,     0,   128,     0,     0,   124,
      67,     0,     0,     0,   138,     0,     0,     0,     0,   162,
     157,     0,     0,     0,   159,   113,   112,    43,   172,   174,
      26,    59,     0,     0,    23,     0,     0,   117,   127,   126,
     123,   125,     0,     0,   169,   168,   149,   163,   156,   155,
       0,     0,     0,     0,    22,     0,     0,   154,    93,     0,
     136,    92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    59,   401,    61,    88,    62,    63,    64,    65,
      66,    67,    68,    69,   233,   234,   402,    70,   203,    71,
      72,   106,   242,    73,   358,   359,    74,   177,   178,   248,
      75,    76,    77,    78,   100,    79,   264,   265,   266,   334,
     327,   267,    80,    81,   288
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -186
static const yytype_int16 yypact[] =
{
      19,   -66,    51,    62,  -186,  1108,   172,   -15,   -21,  -186,
    -186,  -186,  -186,     1,  -186,  -186,  -186,  -186,  -186,     6,
       4,    43,    45,    65,  2558,    34,  2558,  2607,  2558,    85,
      82,    87,    92,    83,  -186,    93,   107,    94,    97,   104,
     122,   123,   124,   132,   133,   141,   142,   146,   148,   152,
     128,   147,  2558,  2558,  -186,  -186,  -186,  -186,    10,   243,
    1210,  -186,  -186,   155,    59,  -186,  -186,   140,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
     164,   -41,   -19,  2558,   158,  2558,  -186,  -186,  -186,  -186,
     173,   160,    -7,   286,    21,  3071,  2230,  2558,  2634,  2230,
     246,  -186,  -186,  2558,   185,   239,    79,  2558,  2558,  2558,
    2558,  2558,  2558,  2558,  2558,  2558,  2558,  2558,  2558,  2558,
    -186,  -186,   166,  2783,  -186,  -186,  2313,    65,   -57,   184,
    -186,    23,  -186,  -186,   177,  -186,  2558,  2558,  2558,  2558,
    2558,  2558,  2558,  2558,  2558,  2558,  2558,  2558,  -186,  -186,
     188,  2558,  -186,  2558,   179,  2799,   186,  2815,    -3,   192,
    -186,  -186,  2362,    65,   496,  2831,  2558,   110,  -186,  1312,
     181,  2558,   194,   197,  2558,   187,     3,  2847,  2863,  2879,
    2895,  2648,  2911,  2668,  2927,  2943,  2959,  2975,  2991,  3007,
    -186,  -186,  3071,   -18,  -186,  2411,  -186,  -186,  -186,  2558,
    -186,   196,   183,   183,   183,   183,   183,   183,   166,   166,
     202,   202,   202,   202,   190,  2688,  2257,   220,    20,    22,
      48,   273,   198,    -4,  -186,  2460,   204,    11,   209,  2558,
    2230,  -186,   175,   210,   394,  2230,   207,  -186,  -186,  -186,
      -1,  2707,  2230,   -28,   216,   251,    59,  -186,  -186,  -186,
    -186,   212,    59,  -186,   260,     3,  -186,    30,  -186,  -186,
    -186,  -186,  2558,  -186,  2558,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  2558,   229,    52,  2726,  -186,  -186,   -45,  -186,
    -186,  -186,  2558,  -186,  -186,  -186,  -186,  -186,  -186,    59,
    -186,   300,    59,  -186,    73,  -186,   231,  2509,  2230,  1414,
    2558,  2230,  -186,  1516,  2558,   598,   700,  -186,  2558,   224,
    2558,  1618,  2230,  2230,  -186,     3,   226,   307,   236,   230,
    -186,  -186,   232,    59,   307,  3023,  3039,  3071,  -186,   237,
    -186,   241,   234,  2277,   277,    59,  -186,  -186,  -186,   244,
      78,   802,  -186,  2230,  1720,   245,  -186,   112,  1006,  -186,
     157,  2745,   258,  3055,  -186,  1822,  1924,   279,   248,  -186,
    -186,   250,   253,   247,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  2230,   285,  -186,   257,   904,  -186,  -186,  -186,
    -186,  -186,  2558,  2230,  -186,  -186,  -186,  -186,  -186,  -186,
     259,  2026,   291,  2230,  -186,  2764,  2128,  -186,  -186,   293,
    -186,  -186
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -186,  -186,  -186,    -5,    -8,  -186,  -186,    -6,  -185,   249,
    -186,     2,  -186,  -155,  -186,    53,   -47,  -186,  -170,  -186,
    -186,   -24,  -186,  -186,  -186,    18,  -186,  -186,   191,  -186,
    -186,  -186,  -186,  -186,  -110,  -186,    36,  -186,   114,  -186,
    -186,  -186,  -186,  -186,  -186
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -135
static const yytype_int16 yytable[] =
{
      60,   262,   237,   160,   109,     9,    10,    11,    12,   142,
      14,     9,    10,    11,    12,   142,    14,    15,    16,    17,
      18,   293,     1,   295,   208,    23,   103,   204,   105,   108,
     105,   263,   322,   205,   161,   284,     3,   206,     9,    10,
      11,    12,   142,    14,    15,    16,    17,    18,   341,   297,
     342,     4,   140,   162,   132,   133,   163,   256,   144,   257,
     258,   259,   260,   238,   323,   304,     5,     9,    10,    11,
      12,   142,    14,   281,   318,   134,   135,   282,   164,    89,
     262,    90,   333,   136,    96,    97,    98,   301,   231,   182,
     137,   302,   134,   135,    93,   165,   319,   167,   209,   143,
     136,   174,   306,    91,   179,   143,   282,   137,    92,   175,
     263,   172,   138,   145,   294,   105,   296,   210,   173,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   143,    94,   185,    95,   104,   350,   202,   186,
     262,   145,   298,   339,   176,   245,   246,   282,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     254,   143,   232,   225,   347,   226,   140,    99,   282,   385,
     263,   140,   369,   282,   202,   110,   111,    82,   244,   374,
     261,   112,   113,   251,   117,   114,   105,   118,   146,   147,
     148,   149,   150,   151,   119,   115,   152,   153,   154,   155,
     156,   157,    83,   310,   311,   312,   158,   202,   389,   116,
      84,   285,   120,   121,   122,   308,   146,   147,   148,   149,
     150,   151,   123,   124,   152,   153,   154,   155,   156,   157,
     130,   125,   126,    85,   158,   309,   127,   202,   128,   315,
     316,   105,   129,   154,   155,   156,   157,   321,   139,   131,
     326,   -76,   154,   155,   156,   157,   329,   141,   159,   261,
     166,   332,   169,   168,   180,    86,    87,   152,   153,   154,
     155,   156,   157,   184,   335,   183,   336,   229,   207,   211,
     224,   227,   235,   250,   337,   252,   353,   170,   253,   255,
     286,   157,   287,   344,   343,   292,   232,   299,   305,   307,
     300,   140,   313,   351,   317,   356,   354,   140,   140,   202,
     324,   325,   105,   140,   328,   357,   360,   365,   366,   261,
     361,   330,   363,   338,   345,   348,   362,   373,   368,    23,
     370,   377,   371,   392,   372,   378,   379,   382,   384,   383,
     396,   388,   397,   140,   398,   403,   140,   399,   386,   400,
     356,   404,   408,   407,   411,   346,   409,   140,   140,   181,
     357,   367,   146,   147,   148,   149,   150,   151,   247,     0,
     152,   153,   154,   155,   156,   157,   391,     0,   140,   331,
     171,     0,     0,     0,     0,     0,     0,     0,   406,     0,
       0,     0,     0,   140,   405,     6,     0,     0,   140,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,     0,    24,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,     0,    29,    30,    31,    32,     0,     0,    33,
      34,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
     146,   147,   148,   149,   150,   151,    50,    51,   152,   314,
     154,   155,   156,   157,    53,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,    57,    58,     6,     0,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
       0,    24,    25,    26,   239,   240,   241,     0,     0,    27,
       0,     0,     0,    28,     0,    29,    30,    31,    32,     0,
       0,    33,    34,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,    57,    58,     6,
       0,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,     0,    24,    25,    26,     0,     0,     0,     0,
       0,    27,  -133,  -133,  -133,    28,     0,    29,    30,    31,
      32,     0,     0,    33,    34,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,    57,
      58,     6,     0,     0,     0,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,     0,     0,
       0,     0,     0,    27,  -134,  -134,  -134,    28,     0,    29,
      30,    31,    32,     0,     0,    33,    34,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,    57,    58,     6,     0,     0,     0,     0,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,    24,    25,    26,
    -121,  -121,  -121,     0,     0,    27,     0,     0,     0,    28,
       0,    29,    30,    31,    32,     0,     0,    33,    34,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,    57,    58,     6,     0,     0,     0,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,     0,    24,
      25,    26,  -122,  -122,  -122,     0,     0,    27,     0,     0,
       0,    28,     0,    29,    30,    31,    32,     0,     0,    33,
      34,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,    57,    58,     6,     0,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
       0,    24,    25,    26,     0,     0,     0,     0,     0,    27,
       0,   355,     0,    28,     0,    29,    30,    31,    32,     0,
       0,    33,    34,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,    53,     0,     0,   390,
       0,     0,     0,     0,    54,    55,    56,    57,    58,     6,
       0,     0,     0,    -4,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,     0,    24,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,    30,    31,
      32,     0,     0,    33,    34,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,    57,
      58,     6,     0,     0,     0,    -5,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,     0,     0,
       0,     0,     0,    27,     0,     0,     0,    28,     0,    29,
      30,    31,    32,     0,     0,    33,    34,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,    57,    58,     6,     0,     0,     0,     0,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,    24,    25,    26,
       0,     0,     0,     0,     0,    27,     0,     0,     0,    28,
     249,    29,    30,    31,    32,     0,     0,    33,    34,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,    57,    58,     6,     0,     0,     0,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,     0,    24,
      25,    26,     0,     0,   352,     0,     0,    27,     0,     0,
       0,    28,     0,    29,    30,    31,    32,     0,     0,    33,
      34,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,    57,    58,     6,     0,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
       0,    24,    25,    26,     0,     0,     0,     0,     0,    27,
       0,   355,     0,    28,     0,    29,    30,    31,    32,     0,
       0,    33,    34,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,    57,    58,     6,
       0,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,     0,    24,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,    30,    31,
      32,   364,     0,    33,    34,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,    57,
      58,     6,     0,     0,     0,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,     0,     0,
     387,     0,     0,    27,     0,     0,     0,    28,     0,    29,
      30,    31,    32,     0,     0,    33,    34,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,    57,    58,     6,     0,     0,     0,     0,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,    24,    25,    26,
       0,     0,     0,     0,     0,    27,     0,     0,     0,    28,
       0,    29,    30,    31,    32,     0,     0,    33,    34,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,    57,    58,     6,     0,     0,     0,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,     0,    24,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,     0,    29,    30,    31,    32,     0,     0,    33,
      34,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,    53,     0,     0,   395,     0,     0,
       0,     0,    54,    55,    56,    57,    58,     6,     0,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
       0,    24,    25,    26,     0,     0,     0,     0,     0,    27,
       0,     0,     0,    28,     0,    29,    30,    31,    32,     0,
       0,    33,    34,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -97,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,    57,    58,     6,
       0,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,     0,    24,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,   410,    29,    30,    31,
      32,     0,     0,    33,    34,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,    57,
      58,     6,     0,     0,     0,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,   290,     0,
       0,     0,     0,    27,     0,     0,     0,    28,     0,    29,
      30,    31,    32,     0,     0,    33,    34,    35,   380,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,    57,    58,   146,   147,   148,   149,   150,   151,   101,
       0,   152,   153,   154,   155,   156,   157,     0,     0,     0,
       0,   291,     0,   146,   147,   148,   149,   150,   151,    34,
       0,   152,   153,   154,   155,   156,   157,     0,     0,     0,
       0,   381,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,   101,     0,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,    53,   201,     0,     0,     0,    34,     0,
       0,    54,    55,    56,    57,   102,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,   101,     0,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,    53,   236,     0,     0,     0,    34,     0,     0,
      54,    55,    56,    57,   102,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,   101,     0,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,    53,   283,     0,     0,     0,    34,     0,     0,    54,
      55,    56,    57,   102,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,   101,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,   303,     0,     0,     0,    34,     0,     0,    54,    55,
      56,    57,   102,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,   101,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
     349,     0,     0,     0,    34,     0,     0,    54,    55,    56,
      57,   102,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,   101,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,    34,     0,     0,    54,    55,    56,    57,
     102,     0,     0,     0,     0,     0,     0,     0,   176,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,    57,   102,
     146,   147,   148,   149,   150,   151,     0,     0,   152,   153,
     154,   155,   156,   157,   146,   147,   148,   149,   150,   151,
       0,     0,   152,   153,   154,   155,   156,   157,     0,     0,
       0,     0,     0,   272,   146,   147,   148,   149,   150,   151,
       0,     0,   152,   153,   154,   155,   156,   157,     0,     0,
       0,     0,     0,   274,   146,   147,   148,   149,   150,   151,
       0,     0,   152,   153,   154,   155,   156,   157,     0,     0,
       0,     0,   289,   146,   147,   148,   149,   150,   151,     0,
       0,   152,   153,   154,   155,   156,   157,     0,     0,     0,
       0,   320,   146,   147,   148,   149,   150,   151,     0,     0,
     152,   153,   154,   155,   156,   157,     0,     0,     0,     0,
     340,   146,   147,   148,   149,   150,   151,     0,     0,   152,
     153,   154,   155,   156,   157,     0,     0,     0,     0,   291,
     146,   147,   148,   149,   150,   151,     0,     0,   152,   153,
     154,   155,   156,   157,     0,     0,     0,     0,   381,   146,
     147,   148,   149,   150,   151,     0,     0,   152,   153,   154,
     155,   156,   157,     0,   200,   146,   147,   148,   149,   150,
     151,     0,     0,   152,   153,   154,   155,   156,   157,     0,
     228,   146,   147,   148,   149,   150,   151,     0,     0,   152,
     153,   154,   155,   156,   157,     0,   230,   146,   147,   148,
     149,   150,   151,     0,     0,   152,   153,   154,   155,   156,
     157,     0,   243,   146,   147,   148,   149,   150,   151,     0,
       0,   152,   153,   154,   155,   156,   157,     0,   268,   146,
     147,   148,   149,   150,   151,     0,     0,   152,   153,   154,
     155,   156,   157,     0,   269,   146,   147,   148,   149,   150,
     151,     0,     0,   152,   153,   154,   155,   156,   157,     0,
     270,   146,   147,   148,   149,   150,   151,     0,     0,   152,
     153,   154,   155,   156,   157,     0,   271,   146,   147,   148,
     149,   150,   151,     0,     0,   152,   153,   154,   155,   156,
     157,     0,   273,   146,   147,   148,   149,   150,   151,     0,
       0,   152,   153,   154,   155,   156,   157,     0,   275,   146,
     147,   148,   149,   150,   151,     0,     0,   152,   153,   154,
     155,   156,   157,     0,   276,   146,   147,   148,   149,   150,
     151,     0,     0,   152,   153,   154,   155,   156,   157,     0,
     277,   146,   147,   148,   149,   150,   151,     0,     0,   152,
     153,   154,   155,   156,   157,     0,   278,   146,   147,   148,
     149,   150,   151,     0,     0,   152,   153,   154,   155,   156,
     157,     0,   279,   146,   147,   148,   149,   150,   151,     0,
       0,   152,   153,   154,   155,   156,   157,     0,   280,   146,
     147,   148,   149,   150,   151,     0,     0,   152,   153,   154,
     155,   156,   157,     0,   375,   146,   147,   148,   149,   150,
     151,     0,     0,   152,   153,   154,   155,   156,   157,     0,
     376,   146,   147,   148,   149,   150,   151,     0,     0,   152,
     153,   154,   155,   156,   157,     0,   393,   146,   147,   148,
     149,   150,   151,     0,     0,   152,   153,   154,   155,   156,
     157
};

static const yytype_int16 yycheck[] =
{
       5,   186,   172,    44,    28,     8,     9,    10,    11,    12,
      13,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     1,     3,     1,     1,    22,    24,   137,    26,    27,
      28,   186,    60,    90,    75,   205,   102,    94,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    93,     1,
      95,     0,    60,    94,    52,    53,    75,    54,    64,    56,
      57,    58,    59,   173,    92,   235,     4,     8,     9,    10,
      11,    12,    13,    91,    75,    82,    83,    95,    97,    94,
     265,   102,   267,    90,    19,    20,    21,    91,    91,   113,
      97,    95,    82,    83,    90,    93,    97,    95,    75,   102,
      90,   106,    91,   102,   109,   102,    95,    97,   102,   107,
     265,    90,   102,     1,    94,   113,    94,    94,    97,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   102,    90,    55,    90,   102,   307,   136,    60,
     325,     1,    94,    91,    34,    35,    36,    95,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     184,   102,   168,   161,    91,   163,   174,   102,    95,    91,
     325,   179,   327,    95,   172,    90,    94,     5,   176,   334,
     186,    94,    90,   181,    90,   102,   184,    90,    76,    77,
      78,    79,    80,    81,    90,   102,    84,    85,    86,    87,
      88,    89,    30,    28,    29,    30,    94,   205,    96,   102,
      38,   209,    90,    90,    90,   239,    76,    77,    78,    79,
      80,    81,    90,    90,    84,    85,    86,    87,    88,    89,
     102,    90,    90,    61,    94,   240,    90,   235,    90,   244,
     245,   239,    90,    86,    87,    88,    89,   252,     5,   102,
     256,    94,    86,    87,    88,    89,   262,   102,    94,   265,
     102,   267,   102,    90,    18,    93,    94,    84,    85,    86,
      87,    88,    89,    34,   272,    90,   274,    91,    94,   102,
      92,   102,    90,   102,   282,    91,   310,     1,    91,   102,
      94,    89,   102,   299,   292,    75,   302,    24,    94,    90,
     102,   309,    92,   308,    97,   313,   311,   315,   316,   307,
      94,    60,   310,   321,   102,   313,   314,   322,   323,   325,
     318,    61,   320,    94,    24,    94,   102,   333,   102,    22,
      94,    94,   102,    75,   102,    94,   102,    60,    94,   345,
      61,    96,    94,   351,    94,    60,   354,    94,   353,   102,
     358,    94,    61,    94,    61,   302,   403,   365,   366,   110,
     358,   325,    76,    77,    78,    79,    80,    81,   177,    -1,
      84,    85,    86,    87,    88,    89,   358,    -1,   386,   265,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   393,    -1,
      -1,    -1,    -1,   401,   392,     1,    -1,    -1,   406,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,     1,    -1,    -1,
      -1,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    26,    27,    28,    29,    30,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,     1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    35,    36,    37,    -1,    39,
      40,    41,    42,    -1,    -1,    45,    46,    47,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    25,    26,    27,
      28,    29,    30,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    40,    41,    42,    -1,    -1,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,     1,    -1,    -1,
      -1,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,     1,
      -1,    -1,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,     1,    -1,    -1,    -1,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    42,    -1,    -1,    45,    46,    47,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    -1,    -1,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    30,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,     1,    -1,    -1,
      -1,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,     1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      42,    43,    -1,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      30,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    42,    -1,    -1,    45,    46,    47,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    40,    41,    42,    -1,    -1,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,     1,    -1,    -1,
      -1,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,     1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    26,    27,     1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    42,    -1,    -1,    45,    46,    47,     1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    76,    77,    78,    79,    80,    81,    26,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    -1,    76,    77,    78,    79,    80,    81,    46,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    46,    -1,
      -1,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    46,    -1,    -1,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    46,    -1,    -1,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    46,    -1,    -1,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    46,    -1,    -1,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    95,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    95,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    94,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      94,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    88,
      89
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,   105,   102,     0,     4,     1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    25,    26,    27,    33,    37,    39,
      40,    41,    42,    45,    46,    47,    49,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      82,    83,    85,    90,    98,    99,   100,   101,   102,   106,
     107,   108,   110,   111,   112,   113,   114,   115,   116,   117,
     121,   123,   124,   127,   130,   134,   135,   136,   137,   139,
     146,   147,     5,    30,    38,    61,    93,    94,   109,    94,
     102,   102,   102,    90,    90,    90,    19,    20,    21,   102,
     138,    26,   102,   115,   102,   115,   125,    90,   115,   125,
      90,    94,    94,    90,   102,   102,   102,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
     102,   102,   115,   115,    82,    83,    90,    97,   102,     5,
     108,   102,    12,   102,   111,     1,    76,    77,    78,    79,
      80,    81,    84,    85,    86,    87,    88,    89,    94,    94,
      44,    75,    94,    75,    97,   115,   102,   115,    90,   102,
       1,    94,    90,    97,   107,   115,    34,   131,   132,   107,
      18,   113,   125,    90,    34,    55,    60,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
      91,    91,   115,   122,   138,    90,    94,    94,     1,    75,
      94,   102,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,    92,   115,   115,   102,    91,    91,
      91,    91,   111,   118,   119,    90,    91,   122,   138,    28,
      29,    30,   126,    91,   115,    35,    36,   132,   133,    38,
     102,   115,    91,    91,   125,   102,    54,    56,    57,    58,
      59,   111,   112,   117,   140,   141,   142,   145,    91,    91,
      91,    91,    95,    91,    95,    91,    91,    91,    91,    91,
      91,    91,    95,    91,   122,   115,    94,   102,   148,    94,
       1,    94,    75,     1,    94,     1,    94,     1,    94,    24,
     102,    91,    95,    91,   122,    94,    91,    90,   125,   107,
      28,    29,    30,    92,    85,   107,   107,    97,    75,    97,
      94,   107,    60,    92,    94,    60,   111,   144,   102,   111,
      61,   142,   111,   112,   143,   115,   115,   115,    94,    91,
      94,    93,    95,   115,   111,    24,   119,    91,    94,    91,
     122,   107,    30,   125,   107,    35,   108,   115,   128,   129,
     115,   115,   102,   115,    43,   107,   107,   140,   102,   117,
      94,   102,   102,   111,   117,    91,    91,    94,    94,   102,
       1,    94,    60,   111,    94,    91,   107,    30,    96,    96,
      93,   129,    75,    91,    61,    93,    61,    94,    94,    94,
     102,   107,   120,    60,    94,   115,   107,    94,    61,   120,
      38,    61
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 1862 "emotionscript.y"
    {
        root_program = (yyvsp[(4) - (5)].block_ptr);
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 1867 "emotionscript.y"
    {
        root_program = (yyvsp[(4) - (6)].block_ptr);
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 1874 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 1875 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 1879 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt(new_block(), (yyvsp[(1) - (1)].stmt_ptr)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 1880 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt((yyvsp[(1) - (2)].block_ptr), (yyvsp[(2) - (2)].stmt_ptr)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 1884 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 1885 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 1886 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 1887 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 1888 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 1889 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 1890 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 1891 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 1892 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 1893 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 1894 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 1895 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 1896 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FUNC_CALL);
        s->name = strdup((yyvsp[(2) - (6)].string_val));
        s->expr = expr_func_call((yyvsp[(2) - (6)].string_val), (yyvsp[(4) - (6)].func_arg_ptr), count_func_args((yyvsp[(4) - (6)].func_arg_ptr)));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 1902 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FUNC_CALL);
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->expr = expr_func_call((yyvsp[(2) - (5)].string_val), NULL, 0);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 1908 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_METHOD_CALL);
        s->name = strdup((yyvsp[(2) - (8)].string_val));
        s->member_name = strdup((yyvsp[(4) - (8)].string_val));
        s->expr = expr_method_call((yyvsp[(2) - (8)].string_val), (yyvsp[(4) - (8)].string_val), (yyvsp[(6) - (8)].func_arg_ptr), count_func_args((yyvsp[(6) - (8)].func_arg_ptr)));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 1915 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_METHOD_CALL);
        s->name = strdup((yyvsp[(2) - (7)].string_val));
        s->member_name = strdup((yyvsp[(4) - (7)].string_val));
        s->expr = expr_method_call((yyvsp[(2) - (7)].string_val), (yyvsp[(4) - (7)].string_val), NULL, 0);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 1922 "emotionscript.y"
    {
        yyerror("Missing ';' after declaration");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 1927 "emotionscript.y"
    {
        yyerror("Missing ';' after assignment");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 1932 "emotionscript.y"
    {
        yyerror("Missing ';' after member assignment");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 1937 "emotionscript.y"
    {
        yyerror("Missing ';' after reflect statement");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 1942 "emotionscript.y"
    {
        yyerror("Missing ';' after speak(...) statement");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 1947 "emotionscript.y"
    {
        yyerror("Missing ';' after listen(...) statement");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1952 "emotionscript.y"
    {
        yyerror("Missing ';' after alert(...) statement");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1957 "emotionscript.y"
    {
        yyerror("Missing ';' after expression statement");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1962 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1963 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1964 "emotionscript.y"
    { yyerrok; (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1965 "emotionscript.y"
    {
        yyerror("Recovered from invalid statement using panic-mode synchronization");
        yyerrok;
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1982 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(1) - (3)].symbol_type);
        s->name = strdup((yyvsp[(2) - (3)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1989 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup((yyvsp[(2) - (3)].string_val));
        s->class_name = strdup((yyvsp[(1) - (3)].string_val));
        s->ctor_args = NULL;
        s->ctor_arg_count = 0;
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1999 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup((yyvsp[(2) - (6)].string_val));
        s->class_name = strdup((yyvsp[(1) - (6)].string_val));
        s->ctor_args = (yyvsp[(4) - (6)].func_arg_ptr);
        s->ctor_arg_count = count_func_args((yyvsp[(4) - (6)].func_arg_ptr));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 2009 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->class_name = strdup((yyvsp[(1) - (5)].string_val));
        s->ctor_args = NULL;
        s->ctor_arg_count = 0;
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 2019 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(2) - (4)].symbol_type);
        s->name = strdup((yyvsp[(3) - (4)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 2026 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(1) - (5)].symbol_type);
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->value_expr = (yyvsp[(4) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 2036 "emotionscript.y"
    { (yyval.symbol_type) = SYM_COUNT; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 2037 "emotionscript.y"
    { (yyval.symbol_type) = SYM_MEASURE; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 2038 "emotionscript.y"
    { (yyval.symbol_type) = SYM_TRUTH; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 2039 "emotionscript.y"
    { (yyval.symbol_type) = SYM_WORDS; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 2040 "emotionscript.y"
    { (yyval.symbol_type) = SYM_LEVEL; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 2041 "emotionscript.y"
    { (yyval.symbol_type) = SYM_EMOTION; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 2042 "emotionscript.y"
    { (yyval.symbol_type) = SYM_UNKNOWN; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 2054 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_ASSIGN);
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->value_expr = (yyvsp[(4) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 2061 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_OBJ_ASSIGN);
        s->name = strdup((yyvsp[(2) - (7)].string_val));
        s->member_name = strdup((yyvsp[(4) - (7)].string_val));
        s->value_expr = (yyvsp[(6) - (7)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 2072 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_EXPR);
        s->expr = (yyvsp[(1) - (2)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 2080 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 2081 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_POWER, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 2082 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MUL, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 2083 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_DIV, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 2084 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MOD, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 2085 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_PLUS, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 2086 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MINUS, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 2087 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_EQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 2088 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_NEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 2089 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_LT, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 2090 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_GT, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 2091 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_LEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 2092 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_GEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 2093 "emotionscript.y"
    { (yyval.expr_ptr) = expr_prefix(OP_INC, (yyvsp[(2) - (2)].string_val)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 2094 "emotionscript.y"
    { (yyval.expr_ptr) = expr_prefix(OP_DEC, (yyvsp[(2) - (2)].string_val)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 2095 "emotionscript.y"
    { (yyval.expr_ptr) = expr_unary_minus((yyvsp[(2) - (2)].expr_ptr)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 2099 "emotionscript.y"
    { (yyval.expr_ptr) = expr_var((yyvsp[(1) - (1)].string_val)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 2100 "emotionscript.y"
    { (yyval.expr_ptr) = expr_postfix(OP_INC, (yyvsp[(1) - (2)].string_val)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 2101 "emotionscript.y"
    { (yyval.expr_ptr) = expr_postfix(OP_DEC, (yyvsp[(1) - (2)].string_val)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 2102 "emotionscript.y"
    { (yyval.expr_ptr) = expr_member_access((yyvsp[(1) - (3)].string_val), (yyvsp[(3) - (3)].string_val)); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 2103 "emotionscript.y"
    {
        (yyval.expr_ptr) = expr_func_call((yyvsp[(1) - (4)].string_val), (yyvsp[(3) - (4)].func_arg_ptr), count_func_args((yyvsp[(3) - (4)].func_arg_ptr)));
    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 2106 "emotionscript.y"
    {
        (yyval.expr_ptr) = expr_func_call((yyvsp[(1) - (3)].string_val), NULL, 0);
    ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 2109 "emotionscript.y"
    {
        (yyval.expr_ptr) = expr_func_call((yyvsp[(2) - (5)].string_val), (yyvsp[(4) - (5)].func_arg_ptr), count_func_args((yyvsp[(4) - (5)].func_arg_ptr)));
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 2112 "emotionscript.y"
    {
        (yyval.expr_ptr) = expr_func_call((yyvsp[(2) - (4)].string_val), NULL, 0);
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 2115 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_int(atoi((yyvsp[(1) - (1)].string_val)))); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 2116 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_float(strtod((yyvsp[(1) - (1)].string_val), NULL))); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 2118 "emotionscript.y"
    {
          size_t n = strlen((yyvsp[(1) - (1)].string_val));
          if (n >= 2 && (yyvsp[(1) - (1)].string_val)[0] == '"' && (yyvsp[(1) - (1)].string_val)[n - 1] == '"') {
              char *inner = (char *)malloc(n - 1);
              if (inner) {
                  memcpy(inner, (yyvsp[(1) - (1)].string_val) + 1, n - 2);
                  inner[n - 2] = '\0';
                  (yyval.expr_ptr) = expr_literal(make_string(inner));
                  free(inner);
              } else {
                  (yyval.expr_ptr) = expr_literal(make_string(""));
              }
          } else {
              (yyval.expr_ptr) = expr_literal(make_string((yyvsp[(1) - (1)].string_val)));
          }
      ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 2134 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_bool(strcmp((yyvsp[(1) - (1)].string_val), "yes") == 0)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 2135 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(2) - (3)].expr_ptr); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 2136 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 2137 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 2142 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FUNC_DECL);
        s->name = strdup((yyvsp[(2) - (10)].string_val));
        s->access = temp_member_access;
        s->is_static = temp_member_is_static;
        s->is_override = temp_member_is_override;
        s->is_method = false;
        s->func_return_type = (SymbolType)(yyvsp[(7) - (10)].symbol_type);
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
        s->func_body = (yyvsp[(9) - (10)].block_ptr);
        temp_param_count = 0;
        clear_member_parse_context();
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 2169 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FUNC_DECL);
        s->name = strdup((yyvsp[(2) - (9)].string_val));
        s->access = temp_member_access;
        s->is_static = temp_member_is_static;
        s->is_override = temp_member_is_override;
        s->is_method = false;
        s->func_return_type = (SymbolType)(yyvsp[(6) - (9)].symbol_type);
        s->func_param_names = NULL;
        s->func_param_types = NULL;
        s->func_param_count = 0;
        s->func_body = (yyvsp[(8) - (9)].block_ptr);
        temp_param_count = 0;
        clear_member_parse_context();
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 2188 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 2189 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (3)].block_ptr); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 2194 "emotionscript.y"
    {
        if (temp_param_count < MAX_PARAMS) {
            temp_param_names[temp_param_count] = strdup((yyvsp[(2) - (2)].string_val));
            temp_param_types[temp_param_count] = (SymbolType)(yyvsp[(1) - (2)].symbol_type);
            temp_param_count++;
        }
    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 2204 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 2209 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_RETURN);
        s->expr = (yyvsp[(2) - (3)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 2217 "emotionscript.y"
    { 
        (yyval.func_arg_ptr) = new_func_arg((yyvsp[(1) - (1)].expr_ptr));
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 2220 "emotionscript.y"
    { 
        FunctionArg *arg = new_func_arg((yyvsp[(3) - (3)].expr_ptr));
        FunctionArg *tail = (yyvsp[(1) - (3)].func_arg_ptr);
        while (tail->next) tail = tail->next;
        tail->next = arg;
        (yyval.func_arg_ptr) = (yyvsp[(1) - (3)].func_arg_ptr);
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 2230 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 2231 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 2232 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 2233 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 2234 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 2235 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 2236 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 2237 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 2238 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 2239 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 2240 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 2241 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 2242 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 2247 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (4)].expr_ptr);
        s->body = (yyvsp[(3) - (4)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 2254 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (5)].expr_ptr);
        s->body = (yyvsp[(3) - (5)].block_ptr);
        s->branches = (yyvsp[(4) - (5)].branch_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 2262 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (6)].expr_ptr);
        s->body = (yyvsp[(3) - (6)].block_ptr);
        s->else_body = (yyvsp[(5) - (6)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 2270 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (7)].expr_ptr);
        s->body = (yyvsp[(3) - (7)].block_ptr);
        s->branches = (yyvsp[(4) - (7)].branch_ptr);
        s->else_body = (yyvsp[(6) - (7)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 2278 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 2279 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 2283 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 2288 "emotionscript.y"
    {
        (yyval.branch_ptr) = new_branch((yyvsp[(2) - (3)].expr_ptr), (yyvsp[(3) - (3)].block_ptr));
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 2292 "emotionscript.y"
    {
        (yyval.branch_ptr) = append_branch((yyvsp[(1) - (4)].branch_ptr), new_branch((yyvsp[(3) - (4)].expr_ptr), (yyvsp[(4) - (4)].block_ptr)));
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 2299 "emotionscript.y"
    {
        (void)(yyvsp[(3) - (7)].expr_ptr);
        (void)(yyvsp[(6) - (7)].block_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 2307 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 2308 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 2319 "emotionscript.y"
    {
        (void)(yyvsp[(2) - (5)].expr_ptr);
        (void)(yyvsp[(3) - (5)].block_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 2327 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 2328 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 2329 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 2333 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(3) - (3)].block_ptr); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 2337 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(2) - (2)].block_ptr); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 2342 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = (yyvsp[(2) - (4)].expr_ptr);
        s->body = (yyvsp[(3) - (4)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 2349 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FOR);
        s->for_init = (yyvsp[(3) - (9)].stmt_ptr);
        s->condition = (yyvsp[(4) - (9)].expr_ptr);
        s->for_step = (yyvsp[(6) - (9)].expr_ptr);
        s->body = (yyvsp[(8) - (9)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 2357 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 2362 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = (yyvsp[(3) - (6)].expr_ptr);
        s->body = (yyvsp[(5) - (6)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 2371 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_BREAK); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 2372 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_CONTINUE); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 2377 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_SPEAK);
        s->expr = (yyvsp[(3) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 2383 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_LISTEN);
        s->name = strdup((yyvsp[(3) - (5)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 2389 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_ALERT);
        s->expr = (yyvsp[(3) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 2397 "emotionscript.y"
    { (yyval.string_val) = strdup((yyvsp[(1) - (1)].string_val)); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 2398 "emotionscript.y"
    { (yyval.string_val) = strdup("speak"); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 2399 "emotionscript.y"
    { (yyval.string_val) = strdup("listen"); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 2400 "emotionscript.y"
    { (yyval.string_val) = strdup("alert"); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 2405 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_PERSONA_DECL);
        s->class_name = strdup((yyvsp[(2) - (5)].string_val));
        s->body = (yyvsp[(4) - (5)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 2412 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_PERSONA_DECL);
        s->class_name = strdup((yyvsp[(2) - (7)].string_val));
        s->parent_name = strdup((yyvsp[(4) - (7)].string_val));
        s->body = (yyvsp[(6) - (7)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 2422 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 2423 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 2427 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt(new_block(), (yyvsp[(1) - (1)].stmt_ptr)); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 2428 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt((yyvsp[(1) - (2)].block_ptr), (yyvsp[(2) - (2)].stmt_ptr)); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 2433 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(3) - (5)].symbol_type);
        s->name = strdup((yyvsp[(4) - (5)].string_val));
        s->access = (AccessModifier)(yyvsp[(1) - (5)].int_val);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 2441 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(2) - (4)].symbol_type);
        s->name = strdup((yyvsp[(3) - (4)].string_val));
        s->access = (AccessModifier)(yyvsp[(1) - (4)].int_val);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 2449 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(2) - (4)].symbol_type);
        s->name = strdup((yyvsp[(3) - (4)].string_val));
        s->access = ACCESS_PRIVATE;
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 2457 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(1) - (3)].symbol_type);
        s->name = strdup((yyvsp[(2) - (3)].string_val));
        s->access = ACCESS_PRIVATE;
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 2465 "emotionscript.y"
    {
          set_member_parse_context((AccessModifier)(yyvsp[(1) - (1)].int_val), false, false);
      ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 2469 "emotionscript.y"
    {
          (yyvsp[(3) - (3)].stmt_ptr)->access = (AccessModifier)(yyvsp[(1) - (3)].int_val);
          (yyvsp[(3) - (3)].stmt_ptr)->is_method = true;
          (yyval.stmt_ptr) = (yyvsp[(3) - (3)].stmt_ptr);
      ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 2475 "emotionscript.y"
    {
          (yyvsp[(1) - (1)].stmt_ptr)->access = ACCESS_PRIVATE;
          (yyvsp[(1) - (1)].stmt_ptr)->is_method = true;
          (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr);
      ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 2481 "emotionscript.y"
    {
          set_member_parse_context(ACCESS_PRIVATE, false, true);
      ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 2485 "emotionscript.y"
    {
          (yyvsp[(3) - (3)].stmt_ptr)->is_override = true;
          (yyvsp[(3) - (3)].stmt_ptr)->is_method = true;
          (yyval.stmt_ptr) = (yyvsp[(3) - (3)].stmt_ptr);
      ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 2491 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(2) - (4)].symbol_type);
        s->name = strdup((yyvsp[(3) - (4)].string_val));
        s->access = ACCESS_PRIVATE;
        s->is_static = true;
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 2502 "emotionscript.y"
    { (yyval.int_val) = ACCESS_PUBLIC; ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 2503 "emotionscript.y"
    { (yyval.int_val) = ACCESS_PROTECTED; ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 2504 "emotionscript.y"
    { (yyval.int_val) = ACCESS_PRIVATE; ;}
    break;



/* Line 1455 of yacc.c  */
#line 5524 "emotionscript.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 2524 "emotionscript.y"


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


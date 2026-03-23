
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

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Block Block;
typedef struct Branch Branch;
#include "emotionscript.tab.h"

extern int yylex(void);
extern int yylineno;
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
    EXPR_CALL_PLACEHOLDER
} ExprKind;

struct Expr {
    ExprKind kind;
    int op;
    char *ident;
    RuntimeValue literal;
    Expr *left;
    Expr *right;
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
    ST_CONTINUE
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
    SymbolType decl_type;
    Expr *value_expr;

    Expr *expr;
    Expr *condition;
    Block *body;
    Branch *branches;
    Block *else_body;

    Stmt *for_init;
    Expr *for_step;
};

struct Block {
    Stmt *head;
    Stmt *tail;
};

typedef enum ExecSignal {
    EXEC_NORMAL,
    EXEC_BREAK,
    EXEC_CONTINUE
} ExecSignal;

static Block *root_program = NULL;

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
    icg_emit(msg);
}

static void semantic_error(const char *msg) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "SEMANTIC ERROR at line %d: %s", yylineno, msg);
    yyerror(buffer);
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

    return update_symbol(sym->name, out);
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

static Expr *expr_placeholder_call(void) {
    Expr *e = new_expr();
    e->kind = EXPR_CALL_PLACEHOLDER;
    return e;
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

static RuntimeValue eval_expr(Expr *e) {
    RuntimeValue lhs;
    RuntimeValue rhs;

    if (!e) return make_undef();

    switch (e->kind) {
        case EXPR_LITERAL:
            return e->literal;

        case EXPR_VAR: {
            Symbol *sym = lookup_symbol(e->ident);
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

        case EXPR_PREFIX:
        case EXPR_POSTFIX: {
            Symbol *sym = lookup_symbol(e->ident);
            RuntimeValue cur;
            RuntimeValue updated;

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

        default:
            return make_undef();
    }
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
            if (insert_symbol(s->name, s->decl_type) != 0) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Redeclaration/type conflict for '%s'", s->name);
                semantic_error(msg);
            }
            tracef("declare %s %s", symbol_type_to_value_name(s->decl_type), s->name);

            if (s->value_expr) {
                sym = lookup_symbol(s->name);
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
            sym = lookup_symbol(s->name);
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


/* Line 189 of yacc.c  */
#line 830 "emotionscript.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
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
#line 829 "emotionscript.y"

    char *string_val;
    int int_val;
    double float_val;
    int symbol_type;
    struct Expr *expr_ptr;
    struct Stmt *stmt_ptr;
    struct Block *block_ptr;
    struct Branch *branch_ptr;



/* Line 214 of yacc.c  */
#line 982 "emotionscript.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 994 "emotionscript.tab.c"

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
#define YYLAST   3067

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  151
/* YYNRULES -- Number of states.  */
#define YYNSTATES  377

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
       0,     0,     3,     9,    10,    12,    14,    17,    19,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      48,    54,    63,    71,    74,    78,    81,    85,    89,    94,
     100,   102,   104,   106,   108,   110,   112,   114,   116,   118,
     120,   122,   128,   136,   139,   141,   145,   149,   153,   157,
     161,   165,   169,   173,   177,   181,   185,   189,   192,   195,
     198,   200,   203,   206,   210,   215,   219,   225,   230,   232,
     234,   236,   238,   242,   244,   246,   257,   267,   269,   273,
     276,   278,   282,   284,   288,   293,   298,   303,   308,   313,
     318,   323,   328,   333,   338,   343,   350,   357,   362,   368,
     375,   383,   385,   387,   389,   393,   398,   406,   408,   411,
     414,   417,   419,   425,   427,   430,   433,   437,   440,   445,
     455,   457,   464,   467,   470,   476,   482,   488,   494,   502,
     503,   505,   507,   510,   516,   521,   526,   530,   533,   535,
     538,   543,   545,   547,   549,   555,   563,   571,   575,   581,
     589,   591
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     105,     0,    -1,     3,   102,     4,   106,     5,    -1,    -1,
     107,    -1,   108,    -1,   107,   108,    -1,   109,    -1,   112,
      -1,   113,    -1,   116,    -1,   123,    -1,   133,    -1,   136,
      -1,   120,    -1,   135,    -1,   137,    -1,   142,    -1,   143,
      -1,    26,   102,    90,   121,    91,    94,    -1,    26,   102,
      90,    91,    94,    -1,    26,   102,    97,   102,    90,   121,
      91,    94,    -1,    26,   102,    97,   102,    90,    91,    94,
      -1,     6,    94,    -1,     7,   102,    94,    -1,     1,    94,
      -1,   110,   102,    94,    -1,   102,   102,    94,    -1,   111,
     110,   102,    94,    -1,   110,   102,    75,   114,    94,    -1,
       8,    -1,     9,    -1,    10,    -1,    11,    -1,    13,    -1,
      12,    -1,   102,    -1,    14,    -1,    15,    -1,    16,    -1,
      17,    -1,    18,   102,    75,   114,    94,    -1,    18,   102,
      97,   102,    75,   114,    94,    -1,   114,    94,    -1,   115,
      -1,   114,    89,   114,    -1,   114,    86,   114,    -1,   114,
      87,   114,    -1,   114,    88,   114,    -1,   114,    84,   114,
      -1,   114,    85,   114,    -1,   114,    76,   114,    -1,   114,
      77,   114,    -1,   114,    80,   114,    -1,   114,    81,   114,
      -1,   114,    78,   114,    -1,   114,    79,   114,    -1,    82,
     102,    -1,    83,   102,    -1,    85,   114,    -1,   102,    -1,
     102,    82,    -1,   102,    83,    -1,   102,    97,   102,    -1,
     102,    90,   121,    91,    -1,   102,    90,    91,    -1,    26,
     102,    90,   121,    91,    -1,    26,   102,    90,    91,    -1,
      99,    -1,   100,    -1,    98,    -1,   101,    -1,    90,   114,
      91,    -1,   122,    -1,    46,    -1,    22,   102,    90,   117,
      91,    24,   110,    60,   119,    61,    -1,    22,   102,    90,
      91,    24,   110,    60,   119,    61,    -1,   118,    -1,   117,
      95,   118,    -1,   110,   102,    -1,   107,    -1,    25,   114,
      94,    -1,   114,    -1,   121,    95,   114,    -1,    69,    90,
     114,    91,    -1,    70,    90,   114,    91,    -1,    71,    90,
     114,    91,    -1,    72,    90,   114,    91,    -1,    73,    90,
     114,    91,    -1,    74,    90,   114,    91,    -1,    62,    90,
     114,    91,    -1,    63,    90,   114,    91,    -1,    64,    90,
     114,    91,    -1,    65,    90,   114,    91,    -1,    67,    90,
     114,    91,    -1,    68,    90,   114,    95,   114,    91,    -1,
      66,    90,   114,    95,   114,    91,    -1,    27,   124,   107,
      30,    -1,    27,   124,   107,   125,    30,    -1,    27,   124,
     107,    29,   107,    30,    -1,    27,   124,   107,   125,    29,
     107,    30,    -1,   126,    -1,   129,    -1,   114,    -1,    28,
     124,   107,    -1,   125,    28,   124,   107,    -1,    33,    90,
     114,    91,    92,   127,    93,    -1,   128,    -1,   127,   128,
      -1,   114,    96,    -1,    35,    96,    -1,   108,    -1,    33,
     114,   130,    36,    97,    -1,   131,    -1,   130,   131,    -1,
     130,   132,    -1,    34,   114,   107,    -1,    35,   107,    -1,
      37,   124,   107,    38,    -1,    39,    90,   112,   114,    94,
     114,    91,   107,    38,    -1,   134,    -1,    42,    90,   124,
      91,   107,    43,    -1,    40,    94,    -1,    41,    94,    -1,
      19,    90,   114,    91,    94,    -1,    20,    90,   102,    91,
      94,    -1,    21,    90,   114,    91,    94,    -1,    49,   102,
      60,   138,    61,    -1,    49,   102,    55,   102,    60,   138,
      61,    -1,    -1,   139,    -1,   140,    -1,   139,   140,    -1,
     141,   111,   110,   102,    94,    -1,   141,   110,   102,    94,
      -1,   111,   110,   102,    94,    -1,   110,   102,    94,    -1,
     141,   116,    -1,   116,    -1,    56,   116,    -1,    54,   110,
     102,    94,    -1,    57,    -1,    58,    -1,    59,    -1,    47,
     102,    34,   124,    94,    -1,    45,   102,    90,    91,    92,
     107,    93,    -1,    45,   102,    90,    91,    60,   107,    61,
      -1,    12,   102,    94,    -1,    12,   102,    75,   114,    94,
      -1,    12,   102,    44,    92,   144,    93,    94,    -1,   102,
      -1,   144,    95,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   845,   845,   853,   854,   858,   859,   863,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     876,   877,   878,   879,   880,   881,   885,   892,   899,   906,
     917,   918,   919,   920,   921,   922,   923,   927,   928,   929,
     930,   934,   941,   951,   960,   961,   962,   963,   964,   965,
     966,   967,   968,   969,   970,   971,   972,   973,   974,   975,
     979,   980,   981,   982,   983,   984,   985,   986,   987,   988,
     989,  1006,  1007,  1008,  1009,  1013,  1017,  1024,  1025,  1029,
    1037,  1041,  1049,  1050,  1054,  1055,  1056,  1057,  1058,  1059,
    1060,  1061,  1062,  1063,  1064,  1065,  1066,  1070,  1077,  1085,
    1093,  1102,  1103,  1107,  1111,  1115,  1122,  1131,  1132,  1136,
    1137,  1138,  1142,  1151,  1152,  1153,  1157,  1161,  1165,  1172,
    1181,  1185,  1195,  1196,  1200,  1206,  1212,  1221,  1222,  1226,
    1227,  1231,  1232,  1236,  1237,  1238,  1239,  1240,  1241,  1242,
    1243,  1247,  1248,  1249,  1253,  1254,  1255,  1259,  1260,  1261,
    1265,  1266
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
  "program_body", "statement_list", "statement", "declaration_stmt",
  "type_specifier", "variable_modifier", "assignment_stmt",
  "expression_stmt", "expression", "primary_expression",
  "function_declaration", "parameter_list", "parameter", "function_body",
  "return_stmt", "argument_list", "math_function", "conditional_stmt",
  "condition", "else_if_chain", "switch_stmt", "switch_body",
  "switch_body_item", "new_switch_stmt", "new_case_blocks",
  "new_case_block", "new_default_block", "loop_stmt", "new_while_stmt",
  "control_flow_stmt", "io_stmt", "persona_declaration", "persona_body",
  "persona_member_list", "persona_member", "access_modifier",
  "fsm_declaration", "emotion_based_stmt", "state_list", 0
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
       0,   104,   105,   106,   106,   107,   107,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   109,   109,   109,
     110,   110,   110,   110,   110,   110,   110,   111,   111,   111,
     111,   112,   112,   113,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   116,   116,   117,   117,   118,
     119,   120,   121,   121,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   124,   125,   125,   126,   127,   127,   128,
     128,   128,   129,   130,   130,   130,   131,   132,   133,   133,
     133,   134,   135,   135,   136,   136,   136,   137,   137,   138,
     138,   139,   139,   140,   140,   140,   140,   140,   140,   140,
     140,   141,   141,   141,   142,   142,   142,   143,   143,   143,
     144,   144
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       5,     8,     7,     2,     3,     2,     3,     3,     4,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     7,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       1,     2,     2,     3,     4,     3,     5,     4,     1,     1,
       1,     1,     3,     1,     1,    10,     9,     1,     3,     2,
       1,     3,     1,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     6,     6,     4,     5,     6,
       7,     1,     1,     1,     3,     4,     7,     1,     2,     2,
       2,     1,     5,     1,     2,     2,     3,     2,     4,     9,
       1,     6,     2,     2,     5,     5,     5,     5,     7,     0,
       1,     1,     2,     5,     4,     4,     3,     2,     1,     2,
       4,     1,     1,     1,     5,     7,     7,     3,     5,     7,
       1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,    30,
      31,    32,    33,     0,    34,    37,    38,    39,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    70,    68,    69,    71,    60,     0,
       0,     5,     7,     0,     0,     8,     9,     0,    44,    10,
      14,    73,    11,   101,   102,    12,   120,    15,    13,    16,
      17,    18,    25,    23,     0,     0,     0,     0,     0,     0,
       0,     0,    60,     0,     0,   103,     0,     0,     0,     0,
       0,   122,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,     0,    61,    62,     0,     0,     0,     2,
       6,     0,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    24,     0,
       0,   147,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,     0,     0,     0,     0,   113,     0,     0,     0,
       0,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,    65,    82,
       0,    63,    27,     0,    26,     0,    51,    52,    55,    56,
      53,    54,    49,    50,    46,    47,    48,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
      67,     0,     0,     0,     0,    97,     0,    72,     0,     0,
       0,   114,   115,   118,     0,     0,     0,     0,     0,     0,
       0,   141,   142,   143,     0,     0,   138,     0,   130,   131,
       0,    90,    91,    92,    93,     0,    94,     0,    84,    85,
      86,    87,    88,    89,    64,     0,     0,    28,   150,     0,
     148,    41,     0,   124,   125,   126,     0,    79,     0,     0,
      67,     0,    20,    66,     0,     0,     0,     0,     0,    98,
       0,     0,     0,     0,   112,     0,     0,     0,     0,   144,
     129,     0,   139,     0,     0,   127,   132,     0,     0,   137,
       0,     0,    83,    29,     0,     0,     0,     0,     0,    78,
      66,    19,     0,     0,     0,    99,     0,     0,     0,   111,
       0,     0,   107,    50,     0,   121,     0,     0,     0,     0,
     136,     0,     0,     0,    96,    95,   149,   151,    42,     0,
       0,    22,     0,     0,   100,   110,   109,   106,   108,     0,
     146,   145,   128,   140,   135,   134,     0,     0,     0,     0,
      21,     0,   133,    76,     0,   119,    75
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    59,   367,    61,    62,    63,    64,    65,    66,
      67,    68,    69,   217,   218,   368,    70,   190,    71,    72,
      96,   226,    73,   331,   332,    74,   165,   166,   232,    75,
      76,    77,    78,    79,   247,   248,   249,   250,    80,    81,
     269
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -171
static const yytype_int16 yypact[] =
{
       9,   -87,    41,    25,  -171,  1039,   -39,   -26,   -33,  -171,
    -171,  -171,  -171,   -25,  -171,  -171,  -171,  -171,  -171,   -15,
       2,     3,    11,    -7,  2406,     7,  2406,  2455,  2406,    21,
      28,    38,    37,    53,  -171,    64,    81,    74,    95,    96,
      97,    98,    99,   100,   101,   103,   104,   105,   107,   108,
     102,   110,  2406,  2406,  -171,  -171,  -171,  -171,     0,   118,
    1141,  -171,  -171,   112,    63,  -171,  -171,  2557,  -171,  -171,
    -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,    69,   -31,   -49,  2406,   113,  2406,
     109,   114,   -40,  2576,    -1,  2978,  2161,  2406,  2482,  2161,
     183,  -171,  -171,  2406,   115,   173,    -6,  2406,  2406,  2406,
    2406,  2406,  2406,  2406,  2406,  2406,  2406,  2406,  2406,  2406,
    -171,  -171,    73,  2690,  -171,  -171,  2210,   116,   123,  -171,
    -171,   -47,  -171,  -171,   119,  2406,  2406,  2406,  2406,  2406,
    2406,  2406,  2406,  2406,  2406,  2406,  2406,  -171,  -171,   117,
    2406,  -171,  2406,   120,  2706,   129,  2722,    -3,   136,  -171,
    2259,   126,   427,  2738,  2406,    17,  -171,  1243,  2406,   138,
     141,  2406,   133,     8,  2754,  2770,  2786,  2802,  2517,  2818,
    2537,  2834,  2850,  2866,  2882,  2898,  2914,  -171,  -171,  2978,
      33,  -171,  -171,  2406,  -171,   142,    29,    29,    29,    29,
      29,    29,    73,    73,   121,   121,   121,   121,   139,  2595,
    2614,   163,   146,   149,   150,   221,   144,    42,  -171,  2308,
     153,    43,   158,  2406,  2161,  -171,   151,   157,   325,  2161,
     155,  -171,  -171,  -171,  2633,  2161,   -46,   156,   193,    63,
     232,  -171,  -171,  -171,   154,    63,  -171,   196,     8,  -171,
      23,  -171,  -171,  -171,  -171,  2406,  -171,  2406,  -171,  -171,
    -171,  -171,  -171,  -171,  -171,  2406,  2652,  -171,  -171,     5,
    -171,  -171,  2406,  -171,  -171,  -171,    63,  -171,   234,    63,
    -171,    61,  -171,   165,  2357,  2161,  1345,  2406,  2161,  -171,
    1447,  2406,   529,   631,  -171,  2406,  1549,  2161,  2161,  -171,
       8,   159,  -171,   166,   160,  -171,  -171,   162,    63,  -171,
    2930,  2946,  2978,  -171,   172,   167,  2671,   207,    63,  -171,
    -171,  -171,   179,    62,   733,  -171,  2161,  1651,   178,  -171,
    2496,   937,  -171,    18,  2962,  -171,  1753,  1855,   214,   182,
    -171,   185,   188,   184,  -171,  -171,  -171,  -171,  -171,  2161,
     224,  -171,   191,   835,  -171,  -171,  -171,  -171,  -171,  2161,
    -171,  -171,  -171,  -171,  -171,  -171,   194,  1957,   229,  2161,
    -171,  2059,  -171,  -171,   230,  -171,  -171
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -171,  -171,  -171,    -5,   -59,  -171,   -37,  -169,   195,  -171,
      32,  -171,  -170,  -171,    19,   -73,  -171,  -158,  -171,  -171,
     -17,  -171,  -171,  -171,   -32,  -171,  -171,   135,  -171,  -171,
    -171,  -171,  -171,  -171,     1,  -171,    54,  -171,  -171,  -171,
    -171
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -118
static const yytype_int16 yytable[] =
{
      60,   130,   221,   246,   245,     9,    10,    11,    12,   132,
      14,    99,     1,   149,   297,     3,     9,    10,    11,    12,
     132,    14,    15,    16,    17,    18,   152,   134,   193,     5,
      23,     9,    10,    11,    12,   132,    14,    15,    16,    17,
      18,     4,   124,   125,   150,    23,   298,   194,   153,   172,
     126,   164,   229,   230,   173,    82,    93,   127,    95,    98,
      95,   281,   239,   151,   240,   241,   242,   243,    83,    84,
     302,     9,    10,    11,    12,   132,    14,    85,   246,   245,
     309,   308,   124,   125,   122,   123,   169,    86,   215,   160,
     126,   162,    87,    88,   167,    90,   161,   127,   314,   133,
     315,    89,   128,   130,   143,   144,   145,   146,   130,    94,
     133,   100,   -59,   141,   142,   143,   144,   145,   146,   154,
     216,   156,   101,   129,   264,   133,   323,   103,   265,   163,
     246,   245,   102,   278,   283,    95,   244,   279,   265,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   320,   352,   237,   104,   265,   265,   189,   143,
     144,   145,   146,   148,   107,   133,   105,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   287,
     288,   289,   209,   106,   210,   108,   109,   110,   111,   112,
     113,   114,   189,   115,   116,   117,   228,   118,   119,   157,
     234,    19,   301,    95,   120,   170,   285,   171,   304,   208,
     146,   244,   121,   307,   131,   155,   158,   192,   191,   286,
     213,   195,   211,   292,   293,   266,   219,   130,   222,   235,
     296,   329,   236,   130,   130,   238,   267,   130,   272,   317,
     273,   268,   216,   274,   275,   276,   277,   282,   284,   290,
     299,   189,   294,   300,    23,    95,   303,   305,   318,   321,
     340,   339,   341,   244,   342,   130,   346,   349,   130,   347,
     326,   343,   329,   351,   355,   362,   363,   130,   130,   364,
     324,   350,   365,   327,   369,   370,   366,   310,   372,   311,
     373,   376,   336,   337,   130,   168,   374,   312,   319,   358,
     231,   338,   306,     0,   316,     0,     0,     0,   130,     0,
       0,     0,   130,     0,     0,     0,   189,     0,     0,    95,
       0,   353,   330,   333,     0,     0,     6,   334,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
      24,    25,    26,     0,   371,     0,     0,     0,    27,     0,
       0,     0,    28,   330,    29,    30,    31,    32,     0,     0,
      33,    34,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,   135,   136,   137,   138,   139,   140,    50,    51,   141,
     291,   143,   144,   145,   146,    53,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,    57,    58,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,    24,    25,    26,   223,   224,   225,     0,     0,
      27,     0,     0,     0,    28,     0,    29,    30,    31,    32,
       0,     0,    33,    34,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,    24,    25,    26,     0,     0,     0,
       0,     0,    27,  -116,  -116,  -116,    28,     0,    29,    30,
      31,    32,     0,     0,    33,    34,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
      57,    58,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,    24,    25,    26,     0,
       0,     0,     0,     0,    27,  -117,  -117,  -117,    28,     0,
      29,    30,    31,    32,     0,     0,    33,    34,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,    57,    58,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,    24,    25,
      26,  -104,  -104,  -104,     0,     0,    27,     0,     0,     0,
      28,     0,    29,    30,    31,    32,     0,     0,    33,    34,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,    57,    58,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
      24,    25,    26,  -105,  -105,  -105,     0,     0,    27,     0,
       0,     0,    28,     0,    29,    30,    31,    32,     0,     0,
      33,    34,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,    57,    58,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,    24,    25,    26,     0,     0,     0,     0,     0,
      27,     0,   328,     0,    28,     0,    29,    30,    31,    32,
       0,     0,    33,    34,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,    53,     0,     0,
     357,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       6,     0,     0,     0,    -3,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,    24,    25,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,    28,     0,    29,    30,
      31,    32,     0,     0,    33,    34,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
      57,    58,     6,     0,     0,     0,    -4,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,    24,    25,    26,     0,
       0,     0,     0,     0,    27,     0,     0,     0,    28,     0,
      29,    30,    31,    32,     0,     0,    33,    34,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,    57,    58,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,    24,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
      28,   233,    29,    30,    31,    32,     0,     0,    33,    34,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,    57,    58,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
      24,    25,    26,     0,     0,   325,     0,     0,    27,     0,
       0,     0,    28,     0,    29,    30,    31,    32,     0,     0,
      33,    34,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,    57,    58,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,    24,    25,    26,     0,     0,     0,     0,     0,
      27,     0,   328,     0,    28,     0,    29,    30,    31,    32,
       0,     0,    33,    34,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,    24,    25,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,    28,     0,    29,    30,
      31,    32,   335,     0,    33,    34,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
      57,    58,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,    24,    25,    26,     0,
       0,   354,     0,     0,    27,     0,     0,     0,    28,     0,
      29,    30,    31,    32,     0,     0,    33,    34,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,    57,    58,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,    24,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
      28,     0,    29,    30,    31,    32,     0,     0,    33,    34,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   360,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,    57,    58,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
      24,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,    28,     0,    29,    30,    31,    32,     0,     0,
      33,    34,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,    53,     0,     0,   361,     0,
       0,     0,     0,    54,    55,    56,    57,    58,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,    24,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,    28,     0,    29,    30,    31,    32,
       0,     0,    33,    34,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -80,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,    24,    25,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,    28,   375,    29,    30,
      31,    32,     0,     0,    33,    34,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
      57,    58,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,    24,    25,    26,     0,
       0,     0,     0,     0,    27,     0,     0,     0,    28,     0,
      29,    30,    31,    32,     0,     0,    33,    34,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    91,     0,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,    34,     0,     0,    54,
      55,    56,    57,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    91,     0,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
      53,   188,     0,     0,     0,    34,     0,     0,    54,    55,
      56,    57,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    91,     0,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,    53,
     220,     0,     0,     0,    34,     0,     0,    54,    55,    56,
      57,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    91,     0,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,    53,   280,
       0,     0,     0,    34,     0,     0,    54,    55,    56,    57,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    91,     0,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,    53,   322,     0,
       0,     0,    34,     0,     0,    54,    55,    56,    57,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    91,     0,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,    34,     0,     0,    54,    55,    56,    57,    92,     0,
       0,     0,     0,     0,     0,     0,   164,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,    57,    92,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,     0,     0,     0,
     147,     0,   356,   135,   136,   137,   138,   139,   140,     0,
       0,   141,   142,   143,   144,   145,   146,     0,     0,     0,
       0,     0,   255,   135,   136,   137,   138,   139,   140,     0,
       0,   141,   142,   143,   144,   145,   146,     0,     0,     0,
       0,     0,   257,   135,   136,   137,   138,   139,   140,     0,
       0,   141,   142,   143,   144,   145,   146,     0,     0,     0,
       0,   147,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,     0,     0,     0,
     159,   135,   136,   137,   138,   139,   140,     0,     0,   141,
     142,   143,   144,   145,   146,     0,     0,     0,     0,   270,
     135,   136,   137,   138,   139,   140,     0,     0,   141,   142,
     143,   144,   145,   146,     0,     0,     0,     0,   271,   135,
     136,   137,   138,   139,   140,     0,     0,   141,   142,   143,
     144,   145,   146,     0,     0,     0,     0,   295,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,     0,     0,     0,     0,   313,   135,   136,   137,
     138,   139,   140,     0,     0,   141,   142,   143,   144,   145,
     146,     0,     0,     0,     0,   348,   135,   136,   137,   138,
     139,   140,     0,     0,   141,   142,   143,   144,   145,   146,
       0,   187,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,   212,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,     0,   214,   135,   136,   137,   138,   139,   140,
       0,     0,   141,   142,   143,   144,   145,   146,     0,   227,
     135,   136,   137,   138,   139,   140,     0,     0,   141,   142,
     143,   144,   145,   146,     0,   251,   135,   136,   137,   138,
     139,   140,     0,     0,   141,   142,   143,   144,   145,   146,
       0,   252,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,   253,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,     0,   254,   135,   136,   137,   138,   139,   140,
       0,     0,   141,   142,   143,   144,   145,   146,     0,   256,
     135,   136,   137,   138,   139,   140,     0,     0,   141,   142,
     143,   144,   145,   146,     0,   258,   135,   136,   137,   138,
     139,   140,     0,     0,   141,   142,   143,   144,   145,   146,
       0,   259,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,   260,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,     0,   261,   135,   136,   137,   138,   139,   140,
       0,     0,   141,   142,   143,   144,   145,   146,     0,   262,
     135,   136,   137,   138,   139,   140,     0,     0,   141,   142,
     143,   144,   145,   146,     0,   263,   135,   136,   137,   138,
     139,   140,     0,     0,   141,   142,   143,   144,   145,   146,
       0,   344,   135,   136,   137,   138,   139,   140,     0,     0,
     141,   142,   143,   144,   145,   146,     0,   345,   135,   136,
     137,   138,   139,   140,     0,     0,   141,   142,   143,   144,
     145,   146,     0,   359,   135,   136,   137,   138,   139,   140,
       0,     0,   141,   142,   143,   144,   145,   146
};

static const yytype_int16 yycheck[] =
{
       5,    60,   160,   173,   173,     8,     9,    10,    11,    12,
      13,    28,     3,    44,    60,   102,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    75,    64,    75,     4,
      22,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,    82,    83,    75,    22,    92,    94,    97,    55,
      90,    34,    35,    36,    60,    94,    24,    97,    26,    27,
      28,   219,    54,    94,    56,    57,    58,    59,    94,   102,
     240,     8,     9,    10,    11,    12,    13,   102,   248,   248,
     250,   250,    82,    83,    52,    53,   103,   102,    91,    90,
      90,    96,    90,    90,    99,   102,    97,    97,    93,   102,
      95,    90,   102,   162,    86,    87,    88,    89,   167,   102,
     102,    90,    94,    84,    85,    86,    87,    88,    89,    87,
     157,    89,    94,     5,    91,   102,   284,    90,    95,    97,
     300,   300,    94,    91,    91,   103,   173,    95,    95,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    91,    91,   171,   102,    95,    95,   126,    86,
      87,    88,    89,    94,    90,   102,   102,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    28,
      29,    30,   150,   102,   152,    90,    90,    90,    90,    90,
      90,    90,   160,    90,    90,    90,   164,    90,    90,    90,
     168,    18,   239,   171,   102,    90,   223,    34,   245,    92,
      89,   248,   102,   250,   102,   102,   102,    94,   102,   224,
      91,   102,   102,   228,   229,   193,    90,   286,   102,    91,
     235,   290,    91,   292,   293,   102,    94,   296,    75,   276,
      94,   102,   279,    94,    94,    24,   102,    94,    90,    92,
      94,   219,    97,    60,    22,   223,   102,    61,    24,    94,
      94,   102,   102,   300,   102,   324,    94,    60,   327,   102,
     287,   308,   331,    94,    96,    61,    94,   336,   337,    94,
     285,   318,    94,   288,    60,    94,   102,   255,    94,   257,
      61,    61,   297,   298,   353,   100,   369,   265,   279,   331,
     165,   300,   248,    -1,   272,    -1,    -1,    -1,   367,    -1,
      -1,    -1,   371,    -1,    -1,    -1,   284,    -1,    -1,   287,
      -1,   326,   290,   291,    -1,    -1,     1,   295,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    27,    -1,   359,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,   331,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    28,    29,    30,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    26,
      27,    28,    29,    30,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    27,    28,    29,    30,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    30,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    43,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    30,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    46,    -1,    -1,    98,
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
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    46,    -1,    -1,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    26,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    46,    -1,    -1,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    26,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      94,    -1,    96,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    95,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    95,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    94,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      94,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    94,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    94,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    94,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    94,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    -1,    91,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    91,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    -1,    91,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    91,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    -1,    91,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    91,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    76,    77,
      78,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      88,    89,    -1,    91,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89
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
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     120,   122,   123,   126,   129,   133,   134,   135,   136,   137,
     142,   143,    94,    94,   102,   102,   102,    90,    90,    90,
     102,    26,   102,   114,   102,   114,   124,    90,   114,   124,
      90,    94,    94,    90,   102,   102,   102,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
     102,   102,   114,   114,    82,    83,    90,    97,   102,     5,
     108,   102,    12,   102,   110,    76,    77,    78,    79,    80,
      81,    84,    85,    86,    87,    88,    89,    94,    94,    44,
      75,    94,    75,    97,   114,   102,   114,    90,   102,    94,
      90,    97,   107,   114,    34,   130,   131,   107,   112,   124,
      90,    34,    55,    60,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,    91,    91,   114,
     121,   102,    94,    75,    94,   102,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,    92,   114,
     114,   102,    91,    91,    91,    91,   110,   117,   118,    90,
      91,   121,   102,    28,    29,    30,   125,    91,   114,    35,
      36,   131,   132,    38,   114,    91,    91,   124,   102,    54,
      56,    57,    58,    59,   110,   111,   116,   138,   139,   140,
     141,    91,    91,    91,    91,    95,    91,    95,    91,    91,
      91,    91,    91,    91,    91,    95,   114,    94,   102,   144,
      94,    94,    75,    94,    94,    94,    24,   102,    91,    95,
      91,   121,    94,    91,    90,   124,   107,    28,    29,    30,
      92,    85,   107,   107,    97,    94,   107,    60,    92,    94,
      60,   110,   116,   102,   110,    61,   140,   110,   111,   116,
     114,   114,   114,    94,    93,    95,   114,   110,    24,   118,
      91,    94,    91,   121,   107,    30,   124,   107,    35,   108,
     114,   127,   128,   114,   114,    43,   107,   107,   138,   102,
      94,   102,   102,   110,    91,    91,    94,   102,    94,    60,
     110,    94,    91,   107,    30,    96,    96,    93,   128,    91,
      61,    93,    61,    94,    94,    94,   102,   107,   119,    60,
      94,   107,    94,    61,   119,    38,    61
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
#line 846 "emotionscript.y"
    {
        root_program = (yyvsp[(4) - (5)].block_ptr);
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 853 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 854 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 858 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt(new_block(), (yyvsp[(1) - (1)].stmt_ptr)); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 859 "emotionscript.y"
    { (yyval.block_ptr) = append_stmt((yyvsp[(1) - (2)].block_ptr), (yyvsp[(2) - (2)].stmt_ptr)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 863 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 864 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 865 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 866 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 867 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 868 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 869 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 870 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 871 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 872 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 873 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 874 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 875 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 876 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 877 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 878 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 879 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 880 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 881 "emotionscript.y"
    { yyerrok; (yyval.stmt_ptr) = new_stmt(ST_NOOP); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 886 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(1) - (3)].symbol_type);
        s->name = strdup((yyvsp[(2) - (3)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 893 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = SYM_UNKNOWN;
        s->name = strdup((yyvsp[(2) - (3)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 900 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(2) - (4)].symbol_type);
        s->name = strdup((yyvsp[(3) - (4)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 907 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_DECL);
        s->decl_type = (SymbolType)(yyvsp[(1) - (5)].symbol_type);
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->value_expr = (yyvsp[(4) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 917 "emotionscript.y"
    { (yyval.symbol_type) = SYM_COUNT; ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 918 "emotionscript.y"
    { (yyval.symbol_type) = SYM_MEASURE; ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 919 "emotionscript.y"
    { (yyval.symbol_type) = SYM_TRUTH; ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 920 "emotionscript.y"
    { (yyval.symbol_type) = SYM_WORDS; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 921 "emotionscript.y"
    { (yyval.symbol_type) = SYM_LEVEL; ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 922 "emotionscript.y"
    { (yyval.symbol_type) = SYM_EMOTION; ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 923 "emotionscript.y"
    { (yyval.symbol_type) = SYM_UNKNOWN; ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 935 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_ASSIGN);
        s->name = strdup((yyvsp[(2) - (5)].string_val));
        s->value_expr = (yyvsp[(4) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 942 "emotionscript.y"
    {
        (void)(yyvsp[(2) - (7)].string_val);
        (void)(yyvsp[(4) - (7)].string_val);
        (void)(yyvsp[(6) - (7)].expr_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 952 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_EXPR);
        s->expr = (yyvsp[(1) - (2)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 960 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 961 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_POWER, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 962 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MUL, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 963 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_DIV, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 964 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MOD, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 965 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_PLUS, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 966 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_MINUS, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 967 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_EQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 968 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_NEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 969 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_LT, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 970 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_GT, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 971 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_LEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 972 "emotionscript.y"
    { (yyval.expr_ptr) = expr_binary(OP_GEQ, (yyvsp[(1) - (3)].expr_ptr), (yyvsp[(3) - (3)].expr_ptr)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 973 "emotionscript.y"
    { (yyval.expr_ptr) = expr_prefix(OP_INC, (yyvsp[(2) - (2)].string_val)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 974 "emotionscript.y"
    { (yyval.expr_ptr) = expr_prefix(OP_DEC, (yyvsp[(2) - (2)].string_val)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 975 "emotionscript.y"
    { (yyval.expr_ptr) = expr_unary_minus((yyvsp[(2) - (2)].expr_ptr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 979 "emotionscript.y"
    { (yyval.expr_ptr) = expr_var((yyvsp[(1) - (1)].string_val)); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 980 "emotionscript.y"
    { (yyval.expr_ptr) = expr_postfix(OP_INC, (yyvsp[(1) - (2)].string_val)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 981 "emotionscript.y"
    { (yyval.expr_ptr) = expr_postfix(OP_DEC, (yyvsp[(1) - (2)].string_val)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 982 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 983 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 984 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 985 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 986 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 987 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_int(atoi((yyvsp[(1) - (1)].string_val)))); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 988 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_float(strtod((yyvsp[(1) - (1)].string_val), NULL))); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 990 "emotionscript.y"
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

  case 71:

/* Line 1455 of yacc.c  */
#line 1006 "emotionscript.y"
    { (yyval.expr_ptr) = expr_literal(make_bool(strcmp((yyvsp[(1) - (1)].string_val), "yes") == 0)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1007 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(2) - (3)].expr_ptr); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 1008 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 1009 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1014 "emotionscript.y"
    {
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1018 "emotionscript.y"
    {
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1024 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1025 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (3)].block_ptr); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1030 "emotionscript.y"
    {
        (void)(yyvsp[(1) - (2)].symbol_type);
        (void)(yyvsp[(2) - (2)].string_val);
    ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 1037 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1042 "emotionscript.y"
    {
        (void)(yyvsp[(2) - (3)].expr_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1049 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1050 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (3)].block_ptr); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1054 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1055 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1056 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1057 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1058 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1059 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1060 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1061 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1062 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1063 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1064 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1065 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1066 "emotionscript.y"
    { (yyval.expr_ptr) = expr_placeholder_call(); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1071 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (4)].expr_ptr);
        s->body = (yyvsp[(3) - (4)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1078 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (5)].expr_ptr);
        s->body = (yyvsp[(3) - (5)].block_ptr);
        s->branches = (yyvsp[(4) - (5)].branch_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1086 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (6)].expr_ptr);
        s->body = (yyvsp[(3) - (6)].block_ptr);
        s->else_body = (yyvsp[(5) - (6)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1094 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IF);
        s->condition = (yyvsp[(2) - (7)].expr_ptr);
        s->body = (yyvsp[(3) - (7)].block_ptr);
        s->branches = (yyvsp[(4) - (7)].branch_ptr);
        s->else_body = (yyvsp[(6) - (7)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1102 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1103 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1107 "emotionscript.y"
    { (yyval.expr_ptr) = (yyvsp[(1) - (1)].expr_ptr); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1112 "emotionscript.y"
    {
        (yyval.branch_ptr) = new_branch((yyvsp[(2) - (3)].expr_ptr), (yyvsp[(3) - (3)].block_ptr));
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1116 "emotionscript.y"
    {
        (yyval.branch_ptr) = append_branch((yyvsp[(1) - (4)].branch_ptr), new_branch((yyvsp[(3) - (4)].expr_ptr), (yyvsp[(4) - (4)].block_ptr)));
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1123 "emotionscript.y"
    {
        (void)(yyvsp[(3) - (7)].expr_ptr);
        (void)(yyvsp[(6) - (7)].block_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1131 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1132 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1143 "emotionscript.y"
    {
        (void)(yyvsp[(2) - (5)].expr_ptr);
        (void)(yyvsp[(3) - (5)].block_ptr);
        (yyval.stmt_ptr) = new_stmt(ST_NOOP);
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1151 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1152 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1153 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1157 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(3) - (3)].block_ptr); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1161 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(2) - (2)].block_ptr); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1166 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = (yyvsp[(2) - (4)].expr_ptr);
        s->body = (yyvsp[(3) - (4)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1173 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_FOR);
        s->for_init = (yyvsp[(3) - (9)].stmt_ptr);
        s->condition = (yyvsp[(4) - (9)].expr_ptr);
        s->for_step = (yyvsp[(6) - (9)].expr_ptr);
        s->body = (yyvsp[(8) - (9)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1181 "emotionscript.y"
    { (yyval.stmt_ptr) = (yyvsp[(1) - (1)].stmt_ptr); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1186 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_WHILE);
        s->condition = (yyvsp[(3) - (6)].expr_ptr);
        s->body = (yyvsp[(5) - (6)].block_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1195 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_BREAK); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1196 "emotionscript.y"
    { (yyval.stmt_ptr) = new_stmt(ST_CONTINUE); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1201 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_SPEAK);
        s->expr = (yyvsp[(3) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1207 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_LISTEN);
        s->name = strdup((yyvsp[(3) - (5)].string_val));
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1213 "emotionscript.y"
    {
        Stmt *s = new_stmt(ST_IO_ALERT);
        s->expr = (yyvsp[(3) - (5)].expr_ptr);
        (yyval.stmt_ptr) = s;
    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1226 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1227 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (1)].block_ptr); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1231 "emotionscript.y"
    { (yyval.block_ptr) = new_block(); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1232 "emotionscript.y"
    { (yyval.block_ptr) = (yyvsp[(1) - (2)].block_ptr); ;}
    break;



/* Line 1455 of yacc.c  */
#line 4053 "emotionscript.tab.c"
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
#line 1269 "emotionscript.y"


void yyerror(const char *s) {
    fprintf(yyout, "✗ SYNTAX ERROR at line %d: %s\n", yylineno, s);
    syntax_errors++;
}

int main(int argc, char **argv) {
    int result;

    icg_reset();

    if (argc < 3) {
        printf("Usage: %s <input.tokens> <output.syntax>\n", argv[0]);
        printf("  OR\n");
        printf("Usage: %s <input.ems> <output.syntax>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    if (!yyin || !yyout) {
        printf("Error: Cannot open files\n");
        return 1;
    }

    fprintf(yyout, "=== EmotionScript Syntax + Execution Engine ===\n\n");

    result = yyparse();

    if (result == 0 && syntax_errors == 0) {
        fprintf(yyout, "\n=== PARSE: SUCCESS ===\n");
        fprintf(yyout, "✓ No syntax errors found\n");
        fprintf(yyout, "✓ Program structure is valid\n");

        fprintf(yyout, "\n=== EXECUTION TRACE ===\n");
        (void)execute_block(root_program);
        fprintf(yyout, "=== EXECUTION COMPLETE ===\n");
        icg_dump(yyout);
    } else {
        fprintf(yyout, "\n=== PARSE: FAILED ===\n");
        fprintf(yyout, "✗ Found %d syntax error(s)\n", syntax_errors);
        fprintf(yyout, "✗ Please fix the errors and try again\n");
    }

    fclose(yyin);
    fclose(yyout);
    free_symbol_table();

    return result;
}


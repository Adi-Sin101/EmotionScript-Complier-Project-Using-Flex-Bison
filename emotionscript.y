%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern FILE *yyin;
extern FILE *yyout;

void yyerror(const char *s);
int syntax_errors = 0;
%}

/* Token declarations - matching the lexer */

/* Program structure */
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

/* Operator precedence and associativity (lowest to highest) */
%nonassoc BARE_ID
%left OP_EQ OP_NEQ OP_LT OP_GT OP_LEQ OP_GEQ
%left OP_PLUS OP_MINUS
%left OP_MUL OP_DIV OP_MOD
%right OP_POWER
%right OP_INC OP_DEC
%left DELIM_DOT
%nonassoc DELIM_LPAREN

/* Union for semantic values */
%union {
    char *string_val;
    int int_val;
    double float_val;
}

%start program

%%

/* ==================== PROGRAM STRUCTURE ==================== */

program:
    PROGRAM_MIND IDENTIFIER PROGRAM_AWAKE program_body PROGRAM_SLEEP
    {
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    }
    ;

program_body:
    /* empty */
    | statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

/* ==================== STATEMENTS ==================== */

statement:
    declaration_stmt
    | assignment_stmt
    | expression_stmt
    | function_declaration
    | conditional_stmt
    | loop_stmt
    | io_stmt
    | control_flow_stmt
    | persona_declaration
    | fsm_declaration
    | emotion_based_stmt
    | PROGRAM_ABORT DELIM_SEMICOLON
    | PROGRAM_SCENE IDENTIFIER DELIM_SEMICOLON
    | error DELIM_SEMICOLON { yyerrok; }
    ;

/* ==================== DECLARATIONS ==================== */

declaration_stmt:
    type_specifier IDENTIFIER DELIM_SEMICOLON
    | variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    | type_specifier IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    ;

type_specifier:
    TYPE_COUNT
    | TYPE_MEASURE
    | TYPE_TRUTH
    | TYPE_WORDS
    | TYPE_LEVEL
    ;

variable_modifier:
    VAR_THOUGHT
    | VAR_MEMORY
    | VAR_TRAIT
    | VAR_STATE
    ;

/* ==================== ASSIGNMENT ==================== */

assignment_stmt:
    ASSIGN_SET IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    | ASSIGN_SET IDENTIFIER DELIM_DOT IDENTIFIER OP_ARROW expression DELIM_SEMICOLON
    ;

/* ==================== EXPRESSIONS ==================== */

expression_stmt:
    expression DELIM_SEMICOLON
    ;

expression:
    primary_expression
    | expression OP_POWER expression
    | expression OP_MUL expression
    | expression OP_DIV expression
    | expression OP_MOD expression
    | expression OP_PLUS expression
    | expression OP_MINUS expression
    | expression OP_EQ expression
    | expression OP_NEQ expression
    | expression OP_LT expression
    | expression OP_GT expression
    | expression OP_LEQ expression
    | expression OP_GEQ expression
    | OP_INC IDENTIFIER
    | OP_DEC IDENTIFIER
    ;

primary_expression:
    IDENTIFIER                                                       %prec BARE_ID
    | IDENTIFIER OP_INC
    | IDENTIFIER OP_DEC
    | IDENTIFIER DELIM_DOT IDENTIFIER
    | IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN
    | IDENTIFIER DELIM_LPAREN DELIM_RPAREN
    | FUNC_CALL IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN
    | FUNC_CALL IDENTIFIER DELIM_LPAREN DELIM_RPAREN
    | LIT_INT
    | LIT_FLOAT
    | LIT_STRING
    | LIT_BOOL
    | DELIM_LPAREN expression DELIM_RPAREN
    | math_function
    | FSM_CURRENT
    ;

/* ==================== FUNCTIONS ==================== */

function_declaration:
    FUNC_DECLARE IDENTIFIER DELIM_LPAREN parameter_list DELIM_RPAREN FUNC_RETURNS type_specifier DELIM_LSHIFT function_body DELIM_RSHIFT
    | FUNC_DECLARE IDENTIFIER DELIM_LPAREN DELIM_RPAREN FUNC_RETURNS type_specifier DELIM_LSHIFT function_body DELIM_RSHIFT
    ;

parameter_list:
    parameter
    | parameter_list DELIM_COMMA parameter
    ;

parameter:
    type_specifier IDENTIFIER
    ;

function_body:
    statement_list
    | statement_list FUNC_RETURN expression DELIM_SEMICOLON
    | FUNC_RETURN expression DELIM_SEMICOLON
    ;



argument_list:
    expression
    | argument_list DELIM_COMMA expression
    ;

/* ==================== MATH FUNCTIONS ==================== */

math_function:
    MATH_SIN DELIM_LPAREN expression DELIM_RPAREN
    | MATH_COS DELIM_LPAREN expression DELIM_RPAREN
    | MATH_TAN DELIM_LPAREN expression DELIM_RPAREN
    | MATH_ASIN DELIM_LPAREN expression DELIM_RPAREN
    | MATH_ACOS DELIM_LPAREN expression DELIM_RPAREN
    | MATH_ATAN DELIM_LPAREN expression DELIM_RPAREN
    | MATH_ABS DELIM_LPAREN expression DELIM_RPAREN
    | MATH_SQRT DELIM_LPAREN expression DELIM_RPAREN
    | MATH_CEIL DELIM_LPAREN expression DELIM_RPAREN
    | MATH_FLOOR DELIM_LPAREN expression DELIM_RPAREN
    | MATH_NORM DELIM_LPAREN expression DELIM_RPAREN
    | MATH_POW DELIM_LPAREN expression DELIM_COMMA expression DELIM_RPAREN
    | MATH_DIFF DELIM_LPAREN expression DELIM_COMMA expression DELIM_RPAREN
    ;

/* ==================== CONDITIONALS ==================== */

conditional_stmt:
    COND_IF condition statement_list COND_END
    | COND_IF condition statement_list else_if_chain COND_END
    | COND_IF condition statement_list COND_ELSE statement_list COND_END
    | COND_IF condition statement_list else_if_chain COND_ELSE statement_list COND_END
    | switch_stmt
    | new_switch_stmt
    ;

condition:
    expression
    ;

else_if_chain:
    COND_ELSE_IF condition statement_list
    | else_if_chain COND_ELSE_IF condition statement_list
    ;


switch_stmt:
    DECIDE_KW DELIM_LPAREN expression DELIM_RPAREN DELIM_LBRACE switch_body DELIM_RBRACE
    ;

switch_body:
    switch_body_item
    | switch_body switch_body_item
    ;

switch_body_item:
    expression DELIM_COLON
    | OTHERWISE_KW DELIM_COLON
    | statement
    ;

/* New switch-case syntax: decide x ... when ... otherwise ... end_decision */
new_switch_stmt:
    DECIDE_KW expression new_case_blocks END_DECISION_KW DELIM_DOT
    ;

new_case_blocks:
    new_case_block
    | new_case_blocks new_case_block
    | new_case_blocks new_default_block
    ;

new_case_block:
    WHEN_KW expression statement_list
    ;

new_default_block:
    OTHERWISE_KW statement_list
    ;

/* ==================== LOOPS ==================== */

loop_stmt:
    LOOP_WHILE condition statement_list LOOP_END
    | LOOP_FOR DELIM_LPAREN assignment_stmt expression DELIM_SEMICOLON expression DELIM_RPAREN statement_list LOOP_END
    | new_while_stmt
    ;

/* New while loop syntax: while_feeling (condition) ... calm */
new_while_stmt:
    WHILE_FEELING_KW DELIM_LPAREN condition DELIM_RPAREN statement_list CALM_KW
    ;

control_flow_stmt:
    LOOP_BREAK DELIM_SEMICOLON
    | LOOP_CONTINUE DELIM_SEMICOLON
    ;

/* ==================== I/O STATEMENTS ==================== */

io_stmt:
    IO_SPEAK DELIM_LPAREN expression DELIM_RPAREN DELIM_SEMICOLON
    | IO_LISTEN DELIM_LPAREN IDENTIFIER DELIM_RPAREN DELIM_SEMICOLON
    | IO_ALERT DELIM_LPAREN expression DELIM_RPAREN DELIM_SEMICOLON
    ;

/* ==================== PERSONA/CLASS ==================== */

persona_declaration:
    CLASS_PERSONA IDENTIFIER DELIM_LSHIFT persona_body DELIM_RSHIFT
    | CLASS_PERSONA IDENTIFIER CLASS_INHERIT IDENTIFIER DELIM_LSHIFT persona_body DELIM_RSHIFT
    ;

persona_body:
    /* empty */
    | persona_member_list
    ;

persona_member_list:
    persona_member
    | persona_member_list persona_member
    ;

persona_member:
    access_modifier variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    | access_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    | variable_modifier type_specifier IDENTIFIER DELIM_SEMICOLON
    | type_specifier IDENTIFIER DELIM_SEMICOLON
    | access_modifier function_declaration
    | function_declaration
    | CLASS_OVERRIDE function_declaration
    | CLASS_STATIC type_specifier IDENTIFIER DELIM_SEMICOLON
    ;

access_modifier:
    CLASS_OPEN
    | CLASS_GUARDED
    | CLASS_HIDDEN
    ;

/* ==================== FSM CONSTRUCTS ==================== */

fsm_declaration:
    FSM_TRANSITION IDENTIFIER WHEN_KW condition DELIM_SEMICOLON
    | FSM_EVENT IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_LBRACE statement_list DELIM_RBRACE
    | FSM_EVENT IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_LSHIFT statement_list DELIM_RSHIFT
    ;

/* Unified rule for TYPE_EMOTION to avoid shift/reduce conflict */
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

/* ==================== ERROR HANDLING ==================== */

void yyerror(const char *s) {
    fprintf(yyout, "✗ SYNTAX ERROR at line %d: %s\n", yylineno, s);
    syntax_errors++;
}

/* ==================== MAIN FUNCTION ==================== */

int main(int argc, char **argv) {
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

    fprintf(yyout, "=== EmotionScript Syntax Validator ===\n\n");

    int result = yyparse();

    if (result == 0 && syntax_errors == 0) {
        fprintf(yyout, "\n=== SYNTAX VALIDATION: SUCCESS ===\n");
        fprintf(yyout, "✓ No syntax errors found\n");
        fprintf(yyout, "✓ Program structure is valid\n");
    } else {
        fprintf(yyout, "\n=== SYNTAX VALIDATION: FAILED ===\n");
        fprintf(yyout, "✗ Found %d syntax error(s)\n", syntax_errors);
        fprintf(yyout, "✗ Please fix the errors and try again\n");
    }

    fclose(yyin);
    fclose(yyout);

    return result;
}

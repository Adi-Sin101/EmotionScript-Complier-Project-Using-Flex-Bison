%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For standalone lexer mode */
#ifndef BISON_MODE
#define EMIT(tok)        fprintf(yyout, "%s\n", tok)
#define EMIT_VAL(tok,v)  fprintf(yyout, "%s %s\n", tok, v)
#else
/* For Bison parser mode */
#include "emotionscript.tab.h"
#define EMIT(tok)        return tok
#define EMIT_VAL(tok,v)  do { yylval.string_val = strdup(yytext); return tok; } while(0)
#endif

void lexical_error(const char* msg) {
    fprintf(yyout, "LEXICAL_ERROR at line %d: %s\n", yylineno, msg);
}
%}


DIGIT       [0-9]
LETTER      [A-Za-z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*


INT_LITERAL     {DIGIT}+
FLOAT_LITERAL   {DIGIT}+"."{DIGIT}+

%%

"##".*                  { }
"#*"([^*]|\*+[^#])*\*+"#"     { }


[ \t\r\n]+              { }


"mind"                  { EMIT("PROGRAM_MIND"); }
"awake"                 { EMIT("PROGRAM_AWAKE"); }
"sleep"                 { EMIT("PROGRAM_SLEEP"); }


"abort"                 { EMIT("PROGRAM_ABORT"); }
"scene"                 { EMIT("PROGRAM_SCENE"); }


"count"                 { EMIT("TYPE_COUNT"); }
"measure"               { EMIT("TYPE_MEASURE"); }
"level"                 { EMIT("TYPE_LEVEL"); }
"truth"                 { EMIT("TYPE_TRUTH"); }
"words"                 { EMIT("TYPE_WORDS"); }
"emotion"               { EMIT("TYPE_EMOTION"); }


"thought"               { EMIT("VAR_THOUGHT"); }
"memory"                { EMIT("VAR_MEMORY"); }
"trait"                 { EMIT("VAR_TRAIT"); }
"state"                 { EMIT("VAR_STATE"); }
"set"                   { EMIT("ASSIGN_SET"); }


"speak"                 { EMIT("IO_SPEAK"); }
"listen"                { EMIT("IO_LISTEN"); }
"alert"                 { EMIT("IO_ALERT"); }


"yes"                   { EMIT("LIT_BOOL"); }
"no"                    { EMIT("LIT_BOOL"); }


"interpret"      { EMIT("FUNC_DECLARE"); }
"end_interpret"  { EMIT("FUNC_END"); }
"yields"         { EMIT("FUNC_RETURNS"); }
"reflect"        { EMIT("FUNC_RETURN"); }
"invoke"         { EMIT("FUNC_CALL"); }


"if_feel"        { EMIT("COND_IF"); }
"else_if_feel"   { EMIT("COND_ELSE_IF"); }
"else_feel"      { EMIT("COND_ELSE"); }
"end_feel"       { EMIT("COND_END"); }
"decide"         { EMIT("COND_SWITCH"); }
"otherwise"      { EMIT("COND_DEFAULT"); }

"decide"         { EMIT("DECIDE_KW"); }
"when"           { EMIT("WHEN_KW"); }
"otherwise"      { EMIT("OTHERWISE_KW"); }
"end_decision"   { EMIT("END_DECISION_KW"); }

"ruminate"       { EMIT("LOOP_WHILE"); }
"end_ruminate"   { EMIT("LOOP_END"); }

"while_feeling"  { EMIT("WHILE_FEELING_KW"); }
"calm"           { EMIT("CALM_KW"); }
"replay"         { EMIT("LOOP_FOR"); }
"break_free"     { EMIT("LOOP_BREAK"); }
"continue_flow"  { EMIT("LOOP_CONTINUE"); }


"composed_of"        { EMIT("FSM_STATES"); }
"trigger"            { EMIT("FSM_EVENT"); }
"current_emotion"    { EMIT("FSM_CURRENT"); }
"transition"         { EMIT("FSM_TRANSITION"); }
"when"               { EMIT("FSM_WHEN"); }


"persona"        { EMIT("CLASS_PERSONA"); }
"end_persona"   { EMIT("CLASS_END"); }
"manifest"       { EMIT("CLASS_BODY"); }
"open"           { EMIT("CLASS_OPEN"); }
"guarded"        { EMIT("CLASS_GUARDED"); }
"hidden"         { EMIT("CLASS_HIDDEN"); }
"shared"         { EMIT("CLASS_STATIC"); }
"evolves_from"   { EMIT("CLASS_INHERIT"); }
"reshape"        { EMIT("CLASS_OVERRIDE"); }


"magnitude"      { EMIT("MATH_ABS"); }
"balance"        { EMIT("MATH_SQRT"); }
"estimate_up"    { EMIT("MATH_CEIL"); }
"estimate_down"  { EMIT("MATH_FLOOR"); }
"gap"            { EMIT("MATH_DIFF"); }
"normalize"      { EMIT("MATH_NORM"); }
"surge"          { EMIT("MATH_POW"); }

"sine"           { EMIT("MATH_SIN"); }
"cosine"         { EMIT("MATH_COS"); }
"tangent"        { EMIT("MATH_TAN"); }
"arcsine"        { EMIT("MATH_ASIN"); }
"arccosine"      { EMIT("MATH_ACOS"); }
"arctangent"     { EMIT("MATH_ATAN"); }


"->"    { EMIT("OP_ARROW"); }

"=="        { EMIT("OP_EQ"); }
"!="        { EMIT("OP_NEQ"); }
"<="        { EMIT("OP_LEQ"); }
">="        { EMIT("OP_GEQ"); }
"<"         { EMIT("OP_LT"); }
">"         { EMIT("OP_GT"); }

"++"        { EMIT("OP_INC"); }
"--"        { EMIT("OP_DEC"); }

"+"         { EMIT("OP_PLUS"); }
"-"         { EMIT("OP_MINUS"); }
"*"         { EMIT("OP_MUL"); }
"/"         { EMIT("OP_DIV"); }
"%"         { EMIT("OP_MOD"); }

"^"         { EMIT("OP_POWER"); }


"("     { EMIT("DELIM_LPAREN"); }
")"     { EMIT("DELIM_RPAREN"); }
"{"     { EMIT("DELIM_LBRACE"); }
"}"     { EMIT("DELIM_RBRACE"); }
";"     { EMIT("DELIM_SEMICOLON"); }
","     { EMIT("DELIM_COMMA"); }
":"     { EMIT("DELIM_COLON"); }
"."     { EMIT("DELIM_DOT"); }

"<<"    { EMIT("DELIM_LSHIFT"); }
">>"    { EMIT("DELIM_RSHIFT"); }


{FLOAT_LITERAL}          { EMIT_VAL("LIT_FLOAT", yytext); }
{INT_LITERAL}            { EMIT_VAL("LIT_INT", yytext); }

\"([^\\\"]|\\.)*\"       { EMIT_VAL("LIT_STRING", yytext); }

{IDENTIFIER}             { EMIT_VAL("IDENTIFIER", yytext); }

. {
    char buf[64];
    snprintf(buf, sizeof(buf), "Unknown symbol '%s'", yytext);
    lexical_error(buf);
}
%%

#ifndef BISON_MODE
int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: emotionscript <input.ems> <output.tokens>\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    if (!yyin || !yyout) {
        printf("File error\n");
        return 1;
    }

    yylex();

    fclose(yyin);
    fclose(yyout);
    return 0;
}
#endif

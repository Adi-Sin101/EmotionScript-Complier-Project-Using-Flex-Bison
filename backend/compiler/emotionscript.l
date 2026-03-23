%option noyywrap
%option yylineno
%option noinput
%option nounput

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emotionscript.tab.h"

/* Semantic value union - defined in the parser */
extern YYSTYPE yylval;

void lexical_error(const char* msg) {
    fprintf(stderr, "LEXICAL ERROR at line %d: %s\n", yylineno, msg);
}
%}


DIGIT       [0-9]
LETTER      [A-Za-z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*


INT_LITERAL     {DIGIT}+
FLOAT_LITERAL   {DIGIT}+"."{DIGIT}+

%%

^[ \t]*##.*                  { /* Single-line comment */ }
^[ \t]*#.*                   { /* Single-line comment */ }
#*([^*]|\*+[^#])*\*+#       { /* Multi-line comment */ }


[ \t]+               { /* Skip spaces and tabs */ }
\r?\n                 { yylineno++; } /* Track newlines for line counter */


"mind"                  { return PROGRAM_MIND; }
"awake"                 { return PROGRAM_AWAKE; }
"sleep"                 { return PROGRAM_SLEEP; }


"abort"                 { return PROGRAM_ABORT; }
"scene"                 { return PROGRAM_SCENE; }


"count"                 { return TYPE_COUNT; }
"measure"               { return TYPE_MEASURE; }
"level"                 { return TYPE_LEVEL; }
"truth"                 { return TYPE_TRUTH; }
"words"                 { return TYPE_WORDS; }
"emotion"               { return TYPE_EMOTION; }


"thought"               { return VAR_THOUGHT; }
"memory"                { return VAR_MEMORY; }
"trait"                 { return VAR_TRAIT; }
"state"                 { return VAR_STATE; }
"set"                   { return ASSIGN_SET; }


"speak"                 { return IO_SPEAK; }
"listen"                { return IO_LISTEN; }
"alert"                 { return IO_ALERT; }
"IO_SPEAK"              { return IO_SPEAK; }
"IO_LISTEN"             { return IO_LISTEN; }
"IO_ALERT"              { return IO_ALERT; }


"yes"                   { yylval.string_val = strdup(yytext); return LIT_BOOL; }
"no"                    { yylval.string_val = strdup(yytext); return LIT_BOOL; }


"interpret"      { return FUNC_DECLARE; }
"end_interpret"  { return FUNC_END; }
"yields"         { return FUNC_RETURNS; }
"reflect"        { return FUNC_RETURN; }
"invoke"         { return FUNC_CALL; }


"if_feel"        { return COND_IF; }
"else_if_feel"   { return COND_ELSE_IF; }
"else_feel"      { return COND_ELSE; }
"end_feel"       { return COND_END; }
"decide"         { return DECIDE_KW; }
"when"           { return WHEN_KW; }
"otherwise"      { return OTHERWISE_KW; }
"end_decision"   { return END_DECISION_KW; }

"ruminate"       { return LOOP_WHILE; }
"end_ruminate"   { return LOOP_END; }

"while_feeling"  { return WHILE_FEELING_KW; }
"calm"           { return CALM_KW; }
"replay"         { return LOOP_FOR; }
"break_free"     { return LOOP_BREAK; }
"continue_flow"  { return LOOP_CONTINUE; }


"composed_of"        { return FSM_STATES; }
"trigger"            { return FSM_EVENT; }
"current_emotion"    { return FSM_CURRENT; }
"transition"         { return FSM_TRANSITION; }


"persona"        { return CLASS_PERSONA; }
"end_persona"    { return CLASS_END; }
"manifest"       { return CLASS_BODY; }
"open"           { return CLASS_OPEN; }
"guarded"        { return CLASS_GUARDED; }
"hidden"         { return CLASS_HIDDEN; }
"shared"         { return CLASS_STATIC; }
"evolves_from"   { return CLASS_INHERIT; }
"reshape"        { return CLASS_OVERRIDE; }


"magnitude"      { return MATH_ABS; }
"balance"        { return MATH_SQRT; }
"estimate_up"    { return MATH_CEIL; }
"estimate_down"  { return MATH_FLOOR; }
"gap"            { return MATH_DIFF; }
"normalize"      { return MATH_NORM; }
"surge"          { return MATH_POW; }

"sine"           { return MATH_SIN; }
"cosine"         { return MATH_COS; }
"tangent"        { return MATH_TAN; }
"arcsine"        { return MATH_ASIN; }
"arccosine"      { return MATH_ACOS; }
"arctangent"     { return MATH_ATAN; }


"->"            { return OP_ARROW; }

"=="        { return OP_EQ; }
"!="        { return OP_NEQ; }
"<="        { return OP_LEQ; }
">="        { return OP_GEQ; }
"<"         { return OP_LT; }
">"         { return OP_GT; }

"++"        { return OP_INC; }
"--"        { return OP_DEC; }

"+"         { return OP_PLUS; }
"-"         { return OP_MINUS; }
"*"         { return OP_MUL; }
"/"         { return OP_DIV; }
"%"         { return OP_MOD; }

"^"         { return OP_POWER; }


"("     { return DELIM_LPAREN; }
")"     { return DELIM_RPAREN; }
"{"     { return DELIM_LBRACE; }
"}"     { return DELIM_RBRACE; }
";"     { return DELIM_SEMICOLON; }
","     { return DELIM_COMMA; }
":"     { return DELIM_COLON; }
"."     { return DELIM_DOT; }

"<<"    { return DELIM_LSHIFT; }
">>"    { return DELIM_RSHIFT; }


{FLOAT_LITERAL}          { yylval.string_val = strdup(yytext); return LIT_FLOAT; }
{INT_LITERAL}            { yylval.string_val = strdup(yytext); return LIT_INT; }

\"([^\\\"]|\\.)*\"       { yylval.string_val = strdup(yytext); return LIT_STRING; }

{IDENTIFIER}             { yylval.string_val = strdup(yytext); return IDENTIFIER; }

. {
    char buf[128];
    snprintf(buf, sizeof(buf), "Unknown symbol '%c' (ASCII %d)", *yytext, (int)*yytext);
    lexical_error(buf);
    /* Return error token to parser or continue scanning */
}
%%

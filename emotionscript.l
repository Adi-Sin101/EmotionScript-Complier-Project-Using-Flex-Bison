%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMIT(tok)        fprintf(yyout, "%s\n", tok)
#define EMIT_VAL(tok,v)  fprintf(yyout, "%s %s\n", tok, v)

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


"mind"                  { EMIT("TOKEN_MIND"); }
"awake"                 { EMIT("TOKEN_AWAKE"); }
"sleep"                 { EMIT("TOKEN_SLEEP"); }


"abort"                 { EMIT("TOKEN_ABORT"); }
"scene"                 { EMIT("TOKEN_SCENE"); }


"count"                 { EMIT("TOKEN_TYPE_COUNT"); }
"measure"               { EMIT("TOKEN_TYPE_MEASURE"); }
"level"                 { EMIT("TOKEN_TYPE_LEVEL"); }
"truth"                 { EMIT("TOKEN_TYPE_TRUTH"); }
"words"                 { EMIT("TOKEN_TYPE_WORDS"); }
"emotion"               { EMIT("TOKEN_TYPE_EMOTION"); }


"thought"               { EMIT("TOKEN_THOUGHT"); }
"memory"                { EMIT("TOKEN_MEMORY"); }
"trait"                 { EMIT("TOKEN_TRAIT"); }
"state"                 { EMIT("TOKEN_STATE"); }
"set"                   { EMIT("TOKEN_SET"); }


"speak"                 { EMIT("TOKEN_SPEAK"); }
"listen"                { EMIT("TOKEN_LISTEN"); }
"alert"                 { EMIT("TOKEN_ALERT"); }


"yes"                   { EMIT("TOKEN_TRUTH_LITERAL"); }
"no"                    { EMIT("TOKEN_TRUTH_LITERAL"); }


"interpret"      { EMIT("TOKEN_INTERPRET"); }
"end_interpret"  { EMIT("TOKEN_END_INTERPRET"); }
"yields"         { EMIT("TOKEN_YIELDS"); }
"reflect"        { EMIT("TOKEN_REFLECT"); }
"invoke"         { EMIT("TOKEN_INVOKE"); }


"if_feel"        { EMIT("TOKEN_IF_FEEL"); }
"else_if_feel"   { EMIT("TOKEN_ELSE_IF_FEEL"); }
"else_feel"      { EMIT("TOKEN_ELSE_FEEL"); }
"end_feel"       { EMIT("TOKEN_END_FEEL"); }
"decide"         { EMIT("TOKEN_DECIDE"); }
"otherwise"      { EMIT("TOKEN_OTHERWISE"); }

"ruminate"       { EMIT("TOKEN_RUMINATE"); }
"end_ruminate"   { EMIT("TOKEN_END_RUMINATE"); }
"replay"         { EMIT("TOKEN_REPLAY"); }
"break_free"     { EMIT("TOKEN_BREAK_FREE"); }
"continue_flow"  { EMIT("TOKEN_CONTINUE_FLOW"); }


"composed_of"        { EMIT("TOKEN_COMPOSED_OF"); }
"trigger"            { EMIT("TOKEN_TRIGGER"); }
"current_emotion"    { EMIT("TOKEN_CURRENT_EMOTION"); }
"transition"         { EMIT("TOKEN_TRANSITION"); }
"when"               { EMIT("TOKEN_WHEN"); }


"persona"        { EMIT("TOKEN_PERSONA"); }
"end_persona"   { EMIT("TOKEN_END_PERSONAE"); }
"manifest"       { EMIT("TOKEN_MANIFEST"); }
"open"           { EMIT("TOKEN_OPEN"); }
"guarded"        { EMIT("TOKEN_GUARDED"); }
"shared"         { EMIT("TOKEN_SHARED"); }
"evolves_from"   { EMIT("TOKEN_EVOLVES_FROM"); }
"reshape"        { EMIT("TOKEN_RESHAPE"); }


"magnitude"      { EMIT("TOKEN_MAGNITUDE"); }
"balance"        { EMIT("TOKEN_BALANCE"); }
"estimate_up"    { EMIT("TOKEN_ESTIMATE_UP"); }
"estimate_down"  { EMIT("TOKEN_ESTIMATE_DOWN"); }
"gap"            { EMIT("TOKEN_GAP"); }
"normalize"      { EMIT("TOKEN_NORMALIZE"); }
"surge"          { EMIT("TOKEN_SURGE"); }

"sine"           { EMIT("TOKEN_SINE"); }
"cosine"         { EMIT("TOKEN_COSINE"); }
"tangent"        { EMIT("TOKEN_TANGENT"); }
"arcsine"        { EMIT("TOKEN_ARCSINE"); }
"arccosine"      { EMIT("TOKEN_ARCCOSINE"); }
"arctangent"     { EMIT("TOKEN_ARCTANGENT"); }


"->"    { EMIT("TOKEN_ARROW"); }

"=="        { EMIT("TOKEN_EQ"); }
"!="        { EMIT("TOKEN_NEQ"); }
"<="        { EMIT("TOKEN_LEQ"); }
">="        { EMIT("TOKEN_GEQ"); }
"<"         { EMIT("TOKEN_LT"); }
">"         { EMIT("TOKEN_GT"); }

"++"        { EMIT("TOKEN_INC"); }
"--"        { EMIT("TOKEN_DEC"); }

"+"         { EMIT("TOKEN_PLUS"); }
"-"         { EMIT("TOKEN_MINUS"); }
"*"         { EMIT("TOKEN_MUL"); }
"/"         { EMIT("TOKEN_DIV"); }
"%"         { EMIT("TOKEN_MOD"); }


"("     { EMIT("TOKEN_LPAREN"); }
")"     { EMIT("TOKEN_RPAREN"); }
"{"     { EMIT("TOKEN_LBRACE"); }
"}"     { EMIT("TOKEN_RBRACE"); }
";"     { EMIT("TOKEN_SEMICOLON"); }
","     { EMIT("TOKEN_COMMA"); }
":"     { EMIT("TOKEN_COLON"); }
"."     { EMIT("TOKEN_DOT"); }


{FLOAT_LITERAL}          { EMIT_VAL("TOKEN_MEASURE_LITERAL", yytext); }
{INT_LITERAL}            { EMIT_VAL("TOKEN_COUNT_LITERAL", yytext); }

\"([^\\\"]|\\.)*\"       { EMIT_VAL("TOKEN_WORDS_LITERAL", yytext); }

{IDENTIFIER}             { EMIT_VAL("TOKEN_NAME", yytext); }

. {
    char buf[64];
    snprintf(buf, sizeof(buf), "Unknown symbol '%s'", yytext);
    lexical_error(buf);
}
%%

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

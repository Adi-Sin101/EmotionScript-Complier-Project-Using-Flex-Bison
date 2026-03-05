
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 80 "emotionscript.y"

    char *string_val;
    int int_val;
    double float_val;



/* Line 1676 of yacc.c  */
#line 163 "emotionscript.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;



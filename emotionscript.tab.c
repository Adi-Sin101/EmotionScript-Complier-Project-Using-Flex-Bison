
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

extern int yylex();
extern int yylineno;
extern FILE *yyin;
extern FILE *yyout;

void yyerror(const char *s);
int syntax_errors = 0;


/* Line 189 of yacc.c  */
#line 88 "emotionscript.tab.c"

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
#line 80 "emotionscript.y"

    char *string_val;
    int int_val;
    double float_val;



/* Line 214 of yacc.c  */
#line 235 "emotionscript.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 247 "emotionscript.tab.c"

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
#define YYLAST   3010

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNRULES -- Number of states.  */
#define YYNSTATES  354

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
      23,    25,    27,    29,    31,    33,    35,    37,    39,    42,
      46,    49,    53,    58,    64,    66,    68,    70,    72,    74,
      76,    78,    80,    82,    88,    96,    99,   101,   105,   109,
     113,   117,   121,   125,   129,   133,   137,   141,   145,   149,
     152,   155,   157,   160,   163,   167,   172,   176,   182,   187,
     189,   191,   193,   195,   199,   201,   203,   214,   224,   226,
     230,   233,   235,   240,   244,   246,   250,   255,   260,   265,
     270,   275,   280,   285,   290,   295,   300,   305,   312,   319,
     324,   330,   337,   345,   347,   349,   351,   355,   360,   368,
     370,   373,   376,   379,   381,   387,   389,   392,   395,   399,
     402,   407,   417,   419,   426,   429,   432,   438,   444,   450,
     456,   464,   465,   467,   469,   472,   478,   483,   488,   492,
     495,   497,   500,   505,   507,   509,   511,   517,   525,   533,
     537,   543,   551,   553
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     105,     0,    -1,     3,   102,     4,   106,     5,    -1,    -1,
     107,    -1,   108,    -1,   107,   108,    -1,   109,    -1,   112,
      -1,   113,    -1,   116,    -1,   122,    -1,   132,    -1,   135,
      -1,   134,    -1,   136,    -1,   141,    -1,   142,    -1,     6,
      94,    -1,     7,   102,    94,    -1,     1,    94,    -1,   110,
     102,    94,    -1,   111,   110,   102,    94,    -1,   110,   102,
      75,   114,    94,    -1,     8,    -1,     9,    -1,    10,    -1,
      11,    -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,
      17,    -1,    18,   102,    75,   114,    94,    -1,    18,   102,
      97,   102,    75,   114,    94,    -1,   114,    94,    -1,   115,
      -1,   114,    89,   114,    -1,   114,    86,   114,    -1,   114,
      87,   114,    -1,   114,    88,   114,    -1,   114,    84,   114,
      -1,   114,    85,   114,    -1,   114,    76,   114,    -1,   114,
      77,   114,    -1,   114,    80,   114,    -1,   114,    81,   114,
      -1,   114,    78,   114,    -1,   114,    79,   114,    -1,    82,
     102,    -1,    83,   102,    -1,   102,    -1,   102,    82,    -1,
     102,    83,    -1,   102,    97,   102,    -1,   102,    90,   120,
      91,    -1,   102,    90,    91,    -1,    26,   102,    90,   120,
      91,    -1,    26,   102,    90,    91,    -1,    99,    -1,   100,
      -1,    98,    -1,   101,    -1,    90,   114,    91,    -1,   121,
      -1,    46,    -1,    22,   102,    90,   117,    91,    24,   110,
      60,   119,    61,    -1,    22,   102,    90,    91,    24,   110,
      60,   119,    61,    -1,   118,    -1,   117,    95,   118,    -1,
     110,   102,    -1,   107,    -1,   107,    25,   114,    94,    -1,
      25,   114,    94,    -1,   114,    -1,   120,    95,   114,    -1,
      69,    90,   114,    91,    -1,    70,    90,   114,    91,    -1,
      71,    90,   114,    91,    -1,    72,    90,   114,    91,    -1,
      73,    90,   114,    91,    -1,    74,    90,   114,    91,    -1,
      62,    90,   114,    91,    -1,    63,    90,   114,    91,    -1,
      64,    90,   114,    91,    -1,    65,    90,   114,    91,    -1,
      67,    90,   114,    91,    -1,    68,    90,   114,    95,   114,
      91,    -1,    66,    90,   114,    95,   114,    91,    -1,    27,
     123,   107,    30,    -1,    27,   123,   107,   124,    30,    -1,
      27,   123,   107,    29,   107,    30,    -1,    27,   123,   107,
     124,    29,   107,    30,    -1,   125,    -1,   128,    -1,   114,
      -1,    28,   123,   107,    -1,   124,    28,   123,   107,    -1,
      33,    90,   114,    91,    92,   126,    93,    -1,   127,    -1,
     126,   127,    -1,   114,    96,    -1,    35,    96,    -1,   108,
      -1,    33,   114,   129,    36,    97,    -1,   130,    -1,   129,
     130,    -1,   129,   131,    -1,    34,   114,   107,    -1,    35,
     107,    -1,    37,   123,   107,    38,    -1,    39,    90,   112,
     114,    94,   114,    91,   107,    38,    -1,   133,    -1,    42,
      90,   123,    91,   107,    43,    -1,    40,    94,    -1,    41,
      94,    -1,    19,    90,   114,    91,    94,    -1,    20,    90,
     102,    91,    94,    -1,    21,    90,   114,    91,    94,    -1,
      49,   102,    60,   137,    61,    -1,    49,   102,    55,   102,
      60,   137,    61,    -1,    -1,   138,    -1,   139,    -1,   138,
     139,    -1,   140,   111,   110,   102,    94,    -1,   140,   110,
     102,    94,    -1,   111,   110,   102,    94,    -1,   110,   102,
      94,    -1,   140,   116,    -1,   116,    -1,    56,   116,    -1,
      54,   110,   102,    94,    -1,    57,    -1,    58,    -1,    59,
      -1,    47,   102,    34,   123,    94,    -1,    45,   102,    90,
      91,    92,   107,    93,    -1,    45,   102,    90,    91,    60,
     107,    61,    -1,    12,   102,    94,    -1,    12,   102,    75,
     114,    94,    -1,    12,   102,    44,    92,   143,    93,    94,
      -1,   102,    -1,   143,    95,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    93,    93,    99,   101,   105,   106,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   131,   132,   133,   137,   138,   139,   140,   141,   145,
     146,   147,   148,   154,   155,   161,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   203,   204,   208,   209,
     213,   217,   218,   219,   225,   226,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   250,
     251,   252,   253,   254,   255,   259,   263,   264,   269,   273,
     274,   278,   279,   280,   285,   289,   290,   291,   295,   299,
     305,   306,   307,   312,   316,   317,   323,   324,   325,   331,
     332,   335,   337,   341,   342,   346,   347,   348,   349,   350,
     351,   352,   353,   357,   358,   359,   365,   366,   367,   372,
     373,   374,   378,   379
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
  "argument_list", "math_function", "conditional_stmt", "condition",
  "else_if_chain", "switch_stmt", "switch_body", "switch_body_item",
  "new_switch_stmt", "new_case_blocks", "new_case_block",
  "new_default_block", "loop_stmt", "new_while_stmt", "control_flow_stmt",
  "io_stmt", "persona_declaration", "persona_body", "persona_member_list",
  "persona_member", "access_modifier", "fsm_declaration",
  "emotion_based_stmt", "state_list", 0
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
     108,   109,   109,   109,   110,   110,   110,   110,   110,   111,
     111,   111,   111,   112,   112,   113,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   116,   116,   117,   117,
     118,   119,   119,   119,   120,   120,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   122,
     122,   122,   122,   122,   122,   123,   124,   124,   125,   126,
     126,   127,   127,   127,   128,   129,   129,   129,   130,   131,
     132,   132,   132,   133,   134,   134,   135,   135,   135,   136,
     136,   137,   137,   138,   138,   139,   139,   139,   139,   139,
     139,   139,   139,   140,   140,   140,   141,   141,   141,   142,
     142,   142,   143,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       2,     3,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     7,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     1,     2,     2,     3,     4,     3,     5,     4,     1,
       1,     1,     1,     3,     1,     1,    10,     9,     1,     3,
       2,     1,     4,     3,     1,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     6,     6,     4,
       5,     6,     7,     1,     1,     1,     3,     4,     7,     1,
       2,     2,     2,     1,     5,     1,     2,     2,     3,     2,
       4,     9,     1,     6,     2,     2,     5,     5,     5,     5,
       7,     0,     1,     1,     2,     5,     4,     4,     3,     2,
       1,     2,     4,     1,     1,     1,     5,     7,     7,     3,
       5,     7,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    28,    29,    30,    31,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    59,    60,    62,    51,     0,     0,     5,
       7,     0,     0,     8,     9,     0,    36,    10,    64,    11,
      93,    94,    12,   112,    14,    13,    15,    16,    17,    20,
      18,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,     0,    52,    53,     0,
       0,     2,     6,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    19,     0,
       0,   139,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    56,    74,     0,    54,     0,
      21,     0,    43,    44,    47,    48,    45,    46,    41,    42,
      38,    39,    40,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    58,     0,     0,     0,    89,
       0,    63,     0,     0,     0,   106,   107,   110,     0,     0,
       0,     0,     0,     0,     0,   133,   134,   135,     0,     0,
     130,     0,   122,   123,     0,    82,    83,    84,    85,     0,
      86,     0,    76,    77,    78,    79,    80,    81,    55,     0,
       0,    22,   142,     0,   140,    33,     0,   116,   117,   118,
       0,    70,     0,     0,    57,     0,     0,     0,     0,    90,
       0,     0,     0,   104,     0,     0,     0,     0,   136,   121,
       0,   131,     0,     0,   119,   124,     0,     0,   129,     0,
       0,    75,    23,     0,     0,     0,     0,     0,    69,     0,
      91,     0,     0,     0,   103,     0,     0,    99,     0,   113,
       0,     0,     0,     0,   128,     0,     0,     0,    88,    87,
     141,   143,    34,     0,     0,     0,    92,   102,   101,    98,
     100,     0,   138,   137,   120,   132,   127,   126,     0,     0,
       0,     0,     0,     0,   125,     0,     0,    67,     0,   111,
      73,     0,    66,    72
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    57,   340,    59,    60,    61,    62,    63,    64,
      65,    66,    67,   203,   204,   341,   177,    68,    69,    90,
     210,    70,   306,   307,    71,   152,   153,   216,    72,    73,
      74,    75,    76,   231,   232,   233,   234,    77,    78,   253
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -151
static const yytype_int16 yypact[] =
{
      13,   -73,    30,    31,  -151,  1149,   -58,   -50,   -37,  -151,
    -151,  -151,  -151,   -16,  -151,  -151,  -151,  -151,  -151,     8,
     -38,    41,    52,    42,    44,  2418,  2467,  2418,    58,   -35,
     -23,    59,    49,  -151,    50,    54,    64,    65,    67,    69,
      70,    71,    72,    73,    74,    75,    76,    87,    88,    77,
      78,  2418,  -151,  -151,  -151,  -151,   -69,   111,  1251,  -151,
    -151,    81,   128,  -151,  -151,  2495,  -151,  -151,  -151,  -151,
    -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,
    -151,    96,   -43,   -64,  2418,    89,  2418,   110,   115,   443,
    2271,  2418,   -31,  2271,   180,  -151,  -151,  2418,   116,   167,
     -40,  2418,  2418,  2418,  2418,  2418,  2418,  2418,  2418,  2418,
    2418,  2418,  2418,  2418,  -151,  -151,  2647,  -151,  -151,  2320,
     107,  -151,  -151,   -63,   109,  2418,  2418,  2418,  2418,  2418,
    2418,  2418,  2418,  2418,  2418,  2418,  2418,  -151,  -151,   118,
    2418,  -151,  2418,   117,  2663,   121,  2679,    -4,  2369,   435,
    2695,  2418,    45,  -151,  1353,  2418,   129,   131,  2418,   122,
     159,  2711,  2727,  2743,  2759,   223,  2775,  2475,  2791,  2807,
    2823,  2839,  2855,  2871,  -151,  -151,   443,   -54,  -151,  2418,
    -151,   132,   -62,   -62,   -62,   -62,   -62,   -62,    26,    26,
     134,   134,   134,   134,   123,  2514,  2533,   153,   135,   140,
     141,   212,   136,   -53,  -151,  -151,   -18,  2418,  2271,  -151,
      62,   145,   333,  2271,   142,  -151,  -151,  -151,  2552,  2271,
     -52,   146,   183,   128,   222,  -151,  -151,  -151,   143,   128,
    -151,   185,   159,  -151,    53,  -151,  -151,  -151,  -151,  2418,
    -151,  2418,  -151,  -151,  -151,  -151,  -151,  -151,  -151,  2418,
    2571,  -151,  -151,    14,  -151,  -151,  2418,  -151,  -151,  -151,
     128,  -151,   225,   128,  -151,  2271,  1455,  2418,  2271,  -151,
    1557,   537,   639,  -151,  2418,  1659,  2271,  2271,  -151,   159,
     148,  -151,   160,   155,  -151,  -151,   162,   128,  -151,  2887,
    2903,   443,  -151,   161,   163,  2590,   207,   128,  -151,   741,
    -151,  2271,  1761,   172,  -151,   108,   945,  -151,  2919,  -151,
    1863,  1965,   208,   176,  -151,   179,   181,   174,  -151,  -151,
    -151,  -151,  -151,  2067,   214,   843,  -151,  -151,  -151,  -151,
    -151,  2271,  -151,  -151,  -151,  -151,  -151,  -151,   184,  2418,
    1047,   218,  2067,  2169,  -151,  2609,  2418,  -151,   220,  -151,
    -151,  2628,  -151,  -151
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -151,  -151,  -151,    -5,   -19,  -151,    -2,  -126,   188,  -151,
      -8,  -151,  -150,  -151,    21,   -56,   149,  -151,  -151,   -25,
    -151,  -151,  -151,   -17,  -151,  -151,   138,  -151,  -151,  -151,
    -151,  -151,  -151,     9,  -151,    61,  -151,  -151,  -151,  -151
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -110
static const yytype_int16 yytable[] =
{
      58,   139,    93,   151,     9,    10,    11,    12,   276,    14,
     230,   142,   179,   117,   118,   159,     1,    89,    92,    89,
     160,   119,   131,   132,   133,   134,   135,   136,   120,     3,
       4,   180,   140,   143,   229,     5,    79,   248,   262,   122,
     277,   249,   263,   116,    80,   125,   126,   127,   128,   129,
     130,   141,    84,   131,   132,   133,   134,   135,   136,    95,
     124,     9,    10,    11,    12,    81,    14,    15,    16,    17,
      18,    96,   156,   264,   281,    23,   144,   249,   146,   151,
     213,   214,   230,   150,   288,   149,    82,   201,   154,    89,
     267,   268,   269,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   229,   293,   287,   294,
      83,   176,   133,   134,   135,   136,   121,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   230,
     122,    85,   195,   221,   196,   122,     9,    10,    11,    12,
     176,    14,    86,   212,    87,   202,    88,   218,    94,    97,
      89,    98,    99,   229,   101,   102,   100,   103,   228,   104,
     105,   106,   107,   108,   109,   110,   111,     9,    10,    11,
      12,   250,    14,    15,    16,    17,    18,   112,   113,   114,
     115,    23,   265,   123,   125,   126,   127,   128,   129,   130,
     138,   145,   131,   132,   133,   134,   135,   136,    19,    89,
     147,   158,   137,   266,   328,   148,   157,   271,   272,   178,
     194,   181,   199,   223,   275,   224,   225,   226,   227,   197,
     219,   280,   220,   136,   222,   252,   251,   283,   256,   257,
     228,   289,   286,   290,   258,   259,   260,   270,   261,   273,
     278,   291,   301,   279,    23,   282,   284,   122,   295,   297,
     313,   304,   122,   122,   314,   320,   122,   315,   296,    89,
     299,   202,   305,   302,   316,   321,   308,   323,   327,   334,
     335,   310,   311,   336,   342,   337,   338,   228,   344,   347,
     122,   352,   155,   122,   298,   317,   348,   304,   312,   330,
     215,   122,   122,   285,     0,   324,   325,   206,   305,   125,
     126,   127,   128,   129,   130,     0,   122,   131,   132,   133,
     134,   135,   136,     0,     0,     0,     0,     0,   239,     0,
       0,   122,     0,     0,   122,     0,   343,     0,     0,     0,
       0,   345,     0,     0,     6,     0,     0,     0,   351,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,     0,    24,
      25,     0,     0,     0,     0,     0,    26,     0,     0,     0,
      27,     0,    28,    29,    30,    31,     0,     0,    32,    33,
      34,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,   125,
     126,   127,   128,   129,   130,    49,    50,   131,   132,   133,
     134,   135,   136,    51,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
       0,    24,    25,   207,   208,   209,     0,     0,    26,     0,
       0,     0,    27,     0,    28,    29,    30,    31,     0,     0,
      32,    33,    34,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,    49,    50,   125,
     126,   127,   128,   129,   130,    51,     0,   131,   132,   133,
     134,   135,   136,    52,    53,    54,    55,    56,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,     0,    24,    25,     0,     0,     0,     0,     0,
      26,  -108,  -108,  -108,    27,     0,    28,    29,    30,    31,
       0,     0,    32,    33,    34,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,     0,    24,    25,     0,     0,     0,
       0,     0,    26,  -109,  -109,  -109,    27,     0,    28,    29,
      30,    31,     0,     0,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,    56,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,     0,    24,    25,   -96,
     -96,   -96,     0,     0,    26,     0,     0,     0,    27,     0,
      28,    29,    30,    31,     0,     0,    32,    33,    34,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,     0,    24,
      25,   -97,   -97,   -97,     0,     0,    26,     0,     0,     0,
      27,     0,    28,    29,    30,    31,     0,     0,    32,    33,
      34,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
       0,    24,    25,     0,     0,     0,     0,     0,    26,     0,
     303,     0,    27,     0,    28,    29,    30,    31,     0,     0,
      32,    33,    34,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,    51,     0,     0,   329,     0,
       0,     0,     0,    52,    53,    54,    55,    56,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,   346,    24,    25,     0,     0,     0,     0,     0,
      26,     0,     0,     0,    27,     0,    28,    29,    30,    31,
       0,     0,    32,    33,    34,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -71,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
       6,     0,     0,     0,    -3,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,     0,    24,    25,     0,     0,     0,
       0,     0,    26,     0,     0,     0,    27,     0,    28,    29,
      30,    31,     0,     0,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,    56,     6,     0,     0,     0,    -4,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,     0,    24,    25,     0,
       0,     0,     0,     0,    26,     0,     0,     0,    27,     0,
      28,    29,    30,    31,     0,     0,    32,    33,    34,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,     0,    24,
      25,     0,     0,     0,     0,     0,    26,     0,     0,     0,
      27,   217,    28,    29,    30,    31,     0,     0,    32,    33,
      34,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
       0,    24,    25,     0,     0,   300,     0,     0,    26,     0,
       0,     0,    27,     0,    28,    29,    30,    31,     0,     0,
      32,    33,    34,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,    56,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,     0,    24,    25,     0,     0,     0,     0,     0,
      26,     0,   303,     0,    27,     0,    28,    29,    30,    31,
       0,     0,    32,    33,    34,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,     0,    24,    25,     0,     0,     0,
       0,     0,    26,     0,     0,     0,    27,     0,    28,    29,
      30,    31,   309,     0,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,    56,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,     0,    24,    25,     0,
       0,   326,     0,     0,    26,     0,     0,     0,    27,     0,
      28,    29,    30,    31,     0,     0,    32,    33,    34,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,     6,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,     0,     0,    24,
      25,     0,     0,     0,     0,     0,    26,     0,     0,     0,
      27,     0,    28,    29,    30,    31,     0,     0,    32,    33,
      34,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,     6,     0,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,     0,
       0,    24,    25,     0,     0,     0,     0,     0,    26,     0,
       0,     0,    27,     0,    28,    29,    30,    31,     0,     0,
      32,    33,    34,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,    51,     0,     0,   333,     0,
       0,     0,     0,    52,    53,    54,    55,    56,     6,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,   339,    24,    25,     0,     0,     0,     0,     0,
      26,     0,     0,     0,    27,     0,    28,    29,    30,    31,
       0,     0,    32,    33,    34,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
       6,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,     0,     0,    24,    25,     0,     0,     0,
       0,     0,    26,     0,     0,     0,    27,   349,    28,    29,
      30,    31,     0,     0,    32,    33,    34,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,    56,     6,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,     0,    24,    25,     0,
       0,     0,     0,     0,    26,     0,     0,     0,    27,     0,
      28,    29,    30,    31,     0,     0,    32,    33,    34,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    24,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,    33,     0,     0,    52,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    24,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
      51,   175,     0,     0,     0,    33,     0,     0,    52,    53,
      54,    55,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    24,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,    51,
     205,     0,     0,     0,    33,     0,     0,    52,    53,    54,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    24,     0,     0,     0,     0,     0,     0,
      49,    50,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,    33,     0,     0,    52,    53,    54,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,    49,
      50,   125,   126,   127,   128,   129,   130,    91,     0,   131,
     132,   133,   134,   135,   136,    52,    53,    54,    55,    56,
     241,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,   135,   136,     0,     0,     0,     0,   137,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,   135,   136,     0,     0,     0,     0,   254,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   135,   136,     0,     0,     0,     0,   255,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
     135,   136,     0,     0,     0,     0,   274,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,   135,
     136,     0,     0,     0,     0,   292,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,   135,   136,
       0,     0,     0,     0,   322,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   135,   136,     0,
       0,     0,     0,   350,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,   135,   136,     0,     0,
       0,     0,   353,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   135,   136,     0,   174,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   135,   136,     0,   198,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   135,   136,     0,
     200,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,   135,   136,     0,   211,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,   135,
     136,     0,   235,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   135,   136,     0,   236,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   135,   136,     0,   237,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   135,   136,     0,
     238,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,   135,   136,     0,   240,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,   135,
     136,     0,   242,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   135,   136,     0,   243,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   135,   136,     0,   244,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   135,   136,     0,
     245,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,   135,   136,     0,   246,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,   135,
     136,     0,   247,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   135,   136,     0,   318,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   135,   136,     0,   319,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   135,   136,     0,
     331
};

static const yytype_int16 yycheck[] =
{
       5,    44,    27,    34,     8,     9,    10,    11,    60,    13,
     160,    75,    75,    82,    83,    55,     3,    25,    26,    27,
      60,    90,    84,    85,    86,    87,    88,    89,    97,   102,
       0,    94,    75,    97,   160,     4,    94,    91,    91,    58,
      92,    95,    95,    51,    94,    76,    77,    78,    79,    80,
      81,    94,    90,    84,    85,    86,    87,    88,    89,    94,
      62,     8,     9,    10,    11,   102,    13,    14,    15,    16,
      17,    94,    97,    91,   224,    22,    84,    95,    86,    34,
      35,    36,   232,    91,   234,    90,   102,    91,    93,    97,
      28,    29,    30,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   232,    93,   234,    95,
     102,   119,    86,    87,    88,    89,     5,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   279,
     149,    90,   140,   158,   142,   154,     8,     9,    10,    11,
     148,    13,    90,   151,   102,   147,   102,   155,    90,    90,
     158,   102,   102,   279,    90,    90,   102,    90,   160,    90,
      90,    90,    90,    90,    90,    90,    90,     8,     9,    10,
      11,   179,    13,    14,    15,    16,    17,    90,    90,   102,
     102,    22,   207,   102,    76,    77,    78,    79,    80,    81,
      94,   102,    84,    85,    86,    87,    88,    89,    18,   207,
      90,    34,    94,   208,    96,    90,    90,   212,   213,   102,
      92,   102,    91,    54,   219,    56,    57,    58,    59,   102,
      91,   223,    91,    89,   102,   102,    94,   229,    75,    94,
     232,   239,   234,   241,    94,    94,    24,    92,   102,    97,
      94,   249,   267,    60,    22,   102,    61,   266,   256,    24,
     102,   270,   271,   272,    94,    94,   275,   102,   260,   267,
     265,   263,   270,   268,   102,   102,   274,    60,    96,    61,
      94,   276,   277,    94,    60,    94,   102,   279,    94,    61,
     299,    61,    94,   302,   263,   287,   342,   306,   279,   306,
     152,   310,   311,   232,    -1,   297,   301,   148,   306,    76,
      77,    78,    79,    80,    81,    -1,   325,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,   340,    -1,    -1,   343,    -1,   331,    -1,    -1,    -1,
      -1,   339,    -1,    -1,     1,    -1,    -1,    -1,   346,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    76,
      77,    78,    79,    80,    81,    90,    -1,    84,    85,    86,
      87,    88,    89,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    30,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    43,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    30,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,     1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
       1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    46,    -1,    -1,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    46,    -1,    -1,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    46,    -1,    -1,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    76,    77,    78,    79,    80,    81,    90,    -1,    84,
      85,    86,    87,    88,    89,    98,    99,   100,   101,   102,
      95,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
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
      -1,    -1,    -1,    -1,    94,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    94,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    94,    76,    77,    78,    79,    80,    81,    -1,
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
      89,    -1,    91,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    76,    77,    78,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,   105,   102,     0,     4,     1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    26,    27,    33,    37,    39,    40,
      41,    42,    45,    46,    47,    49,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    82,
      83,    90,    98,    99,   100,   101,   102,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   121,   122,
     125,   128,   132,   133,   134,   135,   136,   141,   142,    94,
      94,   102,   102,   102,    90,    90,    90,   102,   102,   114,
     123,    90,   114,   123,    90,    94,    94,    90,   102,   102,
     102,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,   102,   102,   114,    82,    83,    90,
      97,     5,   108,   102,   110,    76,    77,    78,    79,    80,
      81,    84,    85,    86,    87,    88,    89,    94,    94,    44,
      75,    94,    75,    97,   114,   102,   114,    90,    90,   107,
     114,    34,   129,   130,   107,   112,   123,    90,    34,    55,
      60,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,    91,    91,   114,   120,   102,    75,
      94,   102,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,    92,   114,   114,   102,    91,    91,
      91,    91,   110,   117,   118,    91,   120,    28,    29,    30,
     124,    91,   114,    35,    36,   130,   131,    38,   114,    91,
      91,   123,   102,    54,    56,    57,    58,    59,   110,   111,
     116,   137,   138,   139,   140,    91,    91,    91,    91,    95,
      91,    95,    91,    91,    91,    91,    91,    91,    91,    95,
     114,    94,   102,   143,    94,    94,    75,    94,    94,    94,
      24,   102,    91,    95,    91,   123,   107,    28,    29,    30,
      92,   107,   107,    97,    94,   107,    60,    92,    94,    60,
     110,   116,   102,   110,    61,   139,   110,   111,   116,   114,
     114,   114,    94,    93,    95,   114,   110,    24,   118,   107,
      30,   123,   107,    35,   108,   114,   126,   127,   114,    43,
     107,   107,   137,   102,    94,   102,   102,   110,    91,    91,
      94,   102,    94,    60,   110,   107,    30,    96,    96,    93,
     127,    91,    61,    93,    61,    94,    94,    94,   102,    25,
     107,   119,    60,   107,    94,   114,    25,    61,   119,    38,
      94,   114,    61,    94
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
#line 94 "emotionscript.y"
    {
        fprintf(yyout, "✓ Valid EmotionScript program\n");
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 125 "emotionscript.y"
    { yyerrok; ;}
    break;



/* Line 1455 of yacc.c  */
#line 2314 "emotionscript.tab.c"
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
#line 382 "emotionscript.y"


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


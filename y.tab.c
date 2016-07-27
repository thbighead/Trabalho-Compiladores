/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "trabalho.y" /* yacc.c:339  */

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Tipo {
  string nome;  // O nome na sua linguagem
  string decl;  // A declaração correspondente em c-assembly
  string fmt;   // O formato para "printf"
  vector<int> dim;
};

Tipo Integer = { "integer", "int", "d" };
Tipo Float =    { "real", "float", "f" };
Tipo Double =  { "double", "double", "lf" };
Tipo String =  { "string", "char", "s" };
Tipo Char =    { "char", "char", "c" };

struct Atributo {
  string v, c;
  Tipo t;
  vector<string> lst;
}; 

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );

map<string,Tipo> ts;
map< string, map< string, Tipo > > tro; // tipo_resultado_operacao;

// contadores para variáveis temporariras
map< string, int > temp_global;
map< string, int > temp_local;
map< string, int > nlabel;
bool escopo_local = false;

string toString( int n ) {
  char buf[256] = "";
  sprintf( buf, "%d", n );
  
  return buf;
}

int toInt( string valor ) {
  int aux = 0,i=0;

  
  i=sscanf( valor.c_str(), "%d", &aux );
  if(i==0) return -1;
  else return aux;
}

string gera_nome_var( Tipo t ) {
  return "t_" + t.nome + "_" + 
   toString( ++(escopo_local ? temp_local : temp_global)[t.nome] );
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       const Atributo& s1, const Atributo& s2, const string s3) {
  ss.c = "";
  for( int i = 0; i < s2.lst.size(); i++ ) {
    if( ts.find( s2.lst[i] ) != ts.end() ) 
      erro( "Variável já declarada: " + s2.lst[i] );
    else {
      ts[ s2.lst[i] ] = s1.t;
      ss.c += s1.t.decl + " " + s2.lst[i] + s3 + ";\n"; 
    }  
  }
}

string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
   
  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
    
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = "// variáveis temporárias \n" + 
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Float, temp[Float.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] )  +
    "\n";
  
  temp.clear();
  
  return decls;
}

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  if( s1.t.nome == s3.t.nome )
  {
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
  else if( s1.t.nome == s3.t.nome &&  s1.t.nome == "string" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim[0]) + " );\n";
  }
}

void gera1Dim(const Atributo& s2, const Atributo& s4){
    for( int i = 0; i < s2.lst.size(); i++ ) {
        ts[s2.lst[i]].dim.push_back(toInt(s4.v));
    }
}

void gera2Dim(const Atributo& s2, const Atributo& s4, const Atributo& s7){
    for( int i = 0; i < s2.lst.size(); i++ ) {
        ts[s2.lst[i]].dim.push_back(toInt(s4.v));
        ts[s2.lst[i]].dim.push_back(toInt(s7.v));
    }
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts.find( s1.v ) == ts.end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ s1.v ];
    ss.v = s1.v;
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss, const Atributo& s1, const Atributo& s2, const Atributo& s3) {
  if( tro.find( s2.v ) != tro.end() ) {
    if( tro[s2.v].find( par( s1.t, s3.t ) ) != tro[s2.v].end() ) {
      ss.t = tro[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );      
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v 
             + ";\n";
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

void gera_codigo_matrix(Atributo& ss, const Atributo& s1, const Atributo& s3, const Atributo& s6, const Atributo& s9){
	string aux1, aux2, aux3, axu4; 
  if( ts.find( s1.v ) == ts.end() )
    erro( "Variável não declarada: " + s1.v );
  else if( s1.t.nome == s9.t.nome ){
    if((ts[s1.v].dim[0]-1)<toInt(s3.v) || (ts[s1.v].dim[1]-1)<toInt(s6.v)){
      erro("Segmentation Fault \n");
    }
    aux1=gera_nome_var( Integer );
    ss.c=ss.c+"  "+aux1+" = "+ s3.v + '*' + toString(ts[s1.v].dim[1])+";\n";
    aux2=gera_nome_var( Integer );
    ss.c=ss.c+"  "+aux2+ " = "+ aux1 +'+'+ s6.v+";\n";
    ss.c =s1.c + s3.c +ss.c+  "  " + s1.v + '[' + aux2 + ']' + " = " + s9.v + ";\n";    
  }
  else{
    erro("Tipo errado na atribuição");
  }
}

void gera_codigo_vetor(Atributo& ss, const Atributo& s1, const Atributo& s3, const Atributo& s6){
  if(ts.find( s1.v ) == ts.end())
    erro( "Variável não declarada: " + s1.v);
  else if( s1.t.nome == s6.t.nome ){
    if((ts[s1.v].dim[0]-1)<toInt(s3.v)){
      erro("Segmentation Fault \n");
    }
    ss.c =s1.c + s3.c +ss.c+  "  " + s1.v + '[' + s3.v + ']' + " = " + s6.v + ";\n";    
  }
  else{
    erro("Tipo errado na atribuição");
  }
}

string gera_nome_label( string cmd ) {
  return "L_" + cmd + "_" + toString( ++nlabel[cmd] );
}

void gera_cmd_if( Atributo& ss, const Atributo& exp, const Atributo& cmd_then, string cmd_else ) { 
  string lbl_then = gera_nome_label( "then" );
  string lbl_end_if = gera_nome_label( "end_if" );

  if( exp.t.nome == String.nome || exp.t.nome == Char.nome)
    erro( "A expressão do IF deve ser um numero!" );
    
  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_then + ";\n" +
         cmd_else + "  goto " + lbl_end_if + ";\n\n" +
         lbl_then + ":;\n" + 
         cmd_then.c + "\n" +
         lbl_end_if + ":;\n"; 
}

void gera_cmd_for(Atributo& ss, const Atributo& s4, const Atributo& s6, const Atributo& s8, const Atributo& s11){
	string lbl_inicio_for = gera_nome_label("inicio_for");								
	string lbl_fim_for = gera_nome_label("fim_for");									

	ss.c =  s4.c + "  " + lbl_inicio_for + ":;\n" + s6.c +
			"  if (!(" + s6.v + ")) goto " + lbl_fim_for + ";\n" +s11.c +
			"\n" + s8.c +
			"\n  goto " + lbl_inicio_for + ";\n  " +
			lbl_fim_for + ":;\n";
}

void gera_codigo_atomico(Atributo& ss,const Atributo& s1, const Atributo& s2){
	string aux;
	if(s1.t.nome==String.nome||s1.t.nome==Char.nome)
	{
		erro("Operação não permitida para esse tipo");
	}
	aux=gera_nome_var(s1.t);
	ss.c= "  " + aux + " = " + s1.v + ";\n";
	ss.c= ss.c + "  " + s1.v + " = " + aux + " + 1; \n";
}


#line 300 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _ID = 258,
    _IF = 259,
    _ELSE = 260,
    _HTPL = 261,
    _IGUALDADE = 262,
    _BARRAHTPL = 263,
    _CTE_FLOAT = 264,
    _MAISMAIS = 265,
    _DIFERENTE = 266,
    _FOR = 267,
    _FUNCTION = 268,
    _PRINT = 269,
    _INT = 270,
    _FLOAT = 271,
    _DOUBLE = 272,
    _CHAR = 273,
    _END = 274,
    _MENOSMENOS = 275,
    _MAIORIGUAL = 276,
    _STRING = 277,
    _MENORIGUAL = 278,
    _PRINTLN = 279,
    _CTE_STRING = 280,
    _CTE_INTEGER = 281
  };
#endif
/* Tokens.  */
#define _ID 258
#define _IF 259
#define _ELSE 260
#define _HTPL 261
#define _IGUALDADE 262
#define _BARRAHTPL 263
#define _CTE_FLOAT 264
#define _MAISMAIS 265
#define _DIFERENTE 266
#define _FOR 267
#define _FUNCTION 268
#define _PRINT 269
#define _INT 270
#define _FLOAT 271
#define _DOUBLE 272
#define _CHAR 273
#define _END 274
#define _MENOSMENOS 275
#define _MAIORIGUAL 276
#define _STRING 277
#define _MENORIGUAL 278
#define _PRINTLN 279
#define _CTE_STRING 280
#define _CTE_INTEGER 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 400 "y.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  148

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    34,     2,     2,
      39,    40,    32,    30,    38,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
      27,    29,    28,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,     2,     2,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   250,   250,   257,   260,   263,   264,   267,   268,   271,
     274,   276,   278,   283,   284,   285,   286,   287,   290,   291,
     294,   297,   298,   301,   302,   305,   308,   309,   312,   313,
     314,   315,   316,   319,   320,   321,   324,   325,   328,   331,
     332,   335,   336,   339,   340,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   357,   358,   359,
     360,   361
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ID", "_IF", "_ELSE", "_HTPL",
  "_IGUALDADE", "_BARRAHTPL", "_CTE_FLOAT", "_MAISMAIS", "_DIFERENTE",
  "_FOR", "_FUNCTION", "_PRINT", "_INT", "_FLOAT", "_DOUBLE", "_CHAR",
  "_END", "_MENOSMENOS", "_MAIORIGUAL", "_STRING", "_MENORIGUAL",
  "_PRINTLN", "_CTE_STRING", "_CTE_INTEGER", "'<'", "'>'", "'='", "'+'",
  "'-'", "'*'", "'/'", "'%'", "';'", "'['", "']'", "','", "'('", "')'",
  "$accept", "S", "ABRE", "FECHA", "MIOLOS", "MIOLO", "DECLS", "DECL",
  "TIPO", "ID", "FUNCTION", "ARGS", "IDS", "PRINCIPAL", "CMDS", "CMD",
  "CMD_ATRIB", "CMD_ATOM", "LVALUE", "CMD_FOR", "CMD_IF", "SAIDA", "E",
  "F", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    60,    62,    61,
      43,    45,    42,    47,    37,    59,    91,    93,    44,    40,
      41
};
# endif

#define YYPACT_NINF -64

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-64)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      32,   -64,   -64,   -64,   -64,   -64,    -4,    20,    15,    32,
     -64,   -10,    23,   -64,    27,   -64,   -64,     4,   -64,   -64,
     -64,    35,    -5,   -64,    16,    26,     2,    50,   -64,     4,
      31,    37,    -7,   -64,   -64,    39,    49,    74,    45,     7,
       7,    40,    42,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
       7,     7,   -64,    41,   -64,    79,    43,   -64,   -64,   -64,
     -64,   -64,     7,    69,   -64,    99,     7,    82,   192,   144,
      51,    48,    61,   114,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,   -64,   -64,   129,    56,   -21,
     -12,    67,    45,     4,   -64,   207,   207,   207,   207,   207,
     207,    19,    19,   -64,   -64,   -64,    70,     7,     7,     7,
      58,   -64,    85,     4,   177,   192,   159,   -64,    92,     0,
      82,    83,    86,     4,   107,    73,    75,     7,   -64,    97,
      89,    90,    91,   192,   119,   -64,     4,     4,    96,   109,
     115,   -64,   126,   131,   121,   125,   -64,   -64
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,    13,    14,    15,    16,    17,     0,     0,     0,     6,
       7,     0,     0,     8,     0,     1,     3,    27,     5,     9,
      19,    10,     0,    38,     0,     0,     0,     0,    25,    27,
       0,     0,     0,    30,    29,     0,     0,     0,    22,     0,
       0,     0,     0,     4,     2,    26,    31,    32,    36,    37,
       0,     0,    28,     0,    18,     0,     0,    21,    60,    58,
      57,    59,     0,     0,    56,     0,     0,     0,    33,     0,
      12,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,     0,     0,     0,
       0,     0,     0,    27,    61,    52,    53,    54,    55,    51,
      50,    45,    46,    47,    48,    49,     0,     0,     0,     0,
       0,    23,     0,    27,     0,    35,     0,    11,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,    20,     0,
       0,     0,     0,    34,     0,    41,    27,    27,     0,     0,
       0,    42,     0,     0,     0,     0,    40,    39
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -64,   -64,   -64,   -64,   149,   -64,   -64,   -64,   -36,   -64,
     -64,   -64,    72,   -64,   -29,   -64,   -63,    53,   101,   -64,
     -64,   -64,   -39,   -64
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,    17,    44,     8,     9,    10,    11,    12,    21,
      13,    56,    57,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    63,    64
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    65,    55,    48,    88,   123,    41,    23,    50,    14,
      58,    68,    69,    49,    42,    51,    59,   108,    24,   124,
      15,    16,    50,    73,   109,    19,    20,    87,    25,    51,
      22,    26,    60,    61,    38,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    62,     1,     2,     3,
       4,    82,    83,    84,     5,    39,    55,   125,    43,     6,
       1,     2,     3,     4,   112,    40,    46,     5,   114,   115,
     116,    36,    47,    37,    52,    53,    74,    54,    70,    66,
      75,    67,    71,    72,   119,    23,    92,    91,   133,    93,
      76,   107,    77,   110,   129,   117,    78,    79,   113,    80,
      81,    82,    83,    84,   118,   122,    74,   139,   140,    85,
      75,   130,   127,   131,   128,   132,   134,   135,   136,   137,
      76,    74,    77,   138,   141,    75,    78,    79,   142,    80,
      81,    82,    83,    84,   143,    76,    74,    77,   144,    86,
      75,    78,    79,   145,    80,    81,    82,    83,    84,   146,
      76,    74,    77,   147,    94,    75,    78,    79,    18,    80,
      81,    82,    83,    84,   111,    76,    74,    77,    89,   106,
      75,    78,    79,   126,    80,    81,    82,    83,    84,     0,
      76,    90,    77,     0,    74,     0,    78,    79,    75,    80,
      81,    82,    83,    84,     0,     0,   121,     0,    76,    74,
      77,     0,     0,    75,    78,    79,     0,    80,    81,    82,
      83,    84,   120,    76,    -1,    77,     0,     0,    -1,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    -1,     0,
      -1,     0,     0,     0,    -1,    -1,     0,    80,    81,    82,
      83,    84
};

static const yytype_int16 yycheck[] =
{
      29,    40,    38,    10,    67,     5,     4,     3,    29,    13,
       3,    50,    51,    20,    12,    36,     9,    29,    14,    19,
       0,     6,    29,    62,    36,    35,     3,    66,    24,    36,
       3,    27,    25,    26,    39,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    39,    15,    16,    17,
      18,    32,    33,    34,    22,    39,    92,   120,     8,    27,
      15,    16,    17,    18,    93,    39,    35,    22,   107,   108,
     109,    36,    35,    38,    35,    26,     7,     3,    37,    39,
      11,    39,     3,    40,   113,     3,    38,    36,   127,    28,
      21,    35,    23,    26,   123,    37,    27,    28,    28,    30,
      31,    32,    33,    34,    19,    13,     7,   136,   137,    40,
      11,     4,    29,    40,    28,    40,    19,    28,    28,    28,
      21,     7,    23,     4,    28,    11,    27,    28,    19,    30,
      31,    32,    33,    34,    19,    21,     7,    23,    12,    40,
      11,    27,    28,    12,    30,    31,    32,    33,    34,    28,
      21,     7,    23,    28,    40,    11,    27,    28,     9,    30,
      31,    32,    33,    34,    92,    21,     7,    23,    67,    40,
      11,    27,    28,   120,    30,    31,    32,    33,    34,    -1,
      21,    37,    23,    -1,     7,    -1,    27,    28,    11,    30,
      31,    32,    33,    34,    -1,    -1,    37,    -1,    21,     7,
      23,    -1,    -1,    11,    27,    28,    -1,    30,    31,    32,
      33,    34,    35,    21,     7,    23,    -1,    -1,    11,    27,
      28,    -1,    30,    31,    32,    33,    34,    -1,    21,    -1,
      23,    -1,    -1,    -1,    27,    28,    -1,    30,    31,    32,
      33,    34
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    16,    17,    18,    22,    27,    42,    45,    46,
      47,    48,    49,    51,    13,     0,     6,    43,    45,    35,
       3,    50,     3,     3,    14,    24,    27,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    36,    38,    39,    39,
      39,     4,    12,     8,    44,    55,    35,    35,    10,    20,
      29,    36,    35,    26,     3,    49,    52,    53,     3,     9,
      25,    26,    39,    63,    64,    63,    39,    39,    63,    63,
      37,     3,    40,    63,     7,    11,    21,    23,    27,    28,
      30,    31,    32,    33,    34,    40,    40,    63,    57,    59,
      37,    36,    38,    28,    40,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    40,    35,    29,    36,
      26,    53,    55,    28,    63,    63,    63,    37,    19,    55,
      35,    37,    13,     5,    19,    57,    58,    29,    28,    55,
       4,    40,    40,    63,    19,    28,    28,    28,     4,    55,
      55,    28,    19,    19,    12,    12,    28,    28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    44,    45,    45,    46,    46,    47,
      48,    48,    48,    49,    49,    49,    49,    49,    50,    50,
      51,    52,    52,    53,    53,    54,    55,    55,    56,    56,
      56,    56,    56,    57,    57,    57,    58,    58,    59,    60,
      60,    61,    61,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    64,    64,    64,
      64,    64
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     1,     2,     0,     1,     1,     2,
       2,     8,     5,     1,     1,     1,     1,     1,     3,     1,
      11,     1,     0,     4,     2,     1,     2,     0,     2,     1,
       1,     2,     2,     3,     9,     6,     2,     2,     1,    14,
      14,    10,    12,     4,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 251 "trabalho.y" /* yacc.c:1646  */
    { cout << "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n"<< declara_var_temp( temp_global ) << (yyvsp[-3]).c << "int main (){\n" <<(yyvsp[-1]).c<<"}"<<endl;
  }
#line 1595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 263 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c;}
#line 1601 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 264 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c="";}
#line 1607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 271 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 275 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-1]), (yyvsp[0]),"" ); }
#line 1619 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 277 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-7]), (yyvsp[-6]),'['+toString(toInt((yyvsp[-4]).v) *toInt((yyvsp[-1]).v))+']'); gera2Dim((yyvsp[-6]), (yyvsp[-4]), (yyvsp[-1]));}
#line 1625 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 279 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-4]), (yyvsp[-3]),'['+(yyvsp[-1]).v+']'); gera1Dim((yyvsp[-3]), (yyvsp[-1]));}
#line 1631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 283 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 1637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 284 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Float; }
#line 1643 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 285 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 1649 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 286 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 1655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 287 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 1661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 290 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 291 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1673 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 305 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c=(yyvsp[0]).c;}
#line 1679 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 308 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c=(yyvsp[-1]).c+(yyvsp[0]).c;}
#line 1685 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 309 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c="";}
#line 1691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 312 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 313 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 314 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 315 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 316 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 319 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atribuicao((yyval), (yyvsp[-2]), (yyvsp[0])); }
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 320 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_matrix((yyval),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]), (yyvsp[0]));}
#line 1733 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 321 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_vetor((yyval),(yyvsp[-5]),(yyvsp[-3]),(yyvsp[0]));}
#line 1739 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 324 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atomico((yyval),(yyvsp[-1]),(yyvsp[0]));}
#line 1745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 325 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atomico((yyval),(yyvsp[-1]),(yyvsp[0]));}
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 328 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 1757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 331 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_for((yyval),(yyvsp[-10]),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]));}
#line 1763 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 332 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_for((yyval),(yyvsp[-10]),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]));}
#line 1769 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 335 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_if( (yyval), (yyvsp[-6]), (yyvsp[-3]), "");}
#line 1775 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 336 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_if( (yyval), (yyvsp[-8]), (yyvsp[-5]), (yyvsp[-3]).c);}
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 339 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\", " + (yyvsp[-1]).v + " );\n"; }
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 340 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 343 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 344 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 345 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1811 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 346 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 347 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 348 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 349 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 350 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 351 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 352 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 353 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 357 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 1865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 358 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Float; }
#line 1871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 359 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 1877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 360 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) );  }
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 361 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1889 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1893 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 364 "trabalho.y" /* yacc.c:1906  */


#include "lex.yy.c"

void inicializa_tabela_de_resultado_de_operacoes() {
  map< string, Tipo > r;
  
  // OBS: a ordem é muito importante!!  
  r[par(Integer, Integer)] = Integer;    
  tro[ "%" ] = r;    
  
  r[par(Integer, Float)] = Float;    
  r[par(Integer, Double)] = Double;    
  r[par(Float, Integer)] = Float;    
  r[par(Float, Float)] = Float;    
  r[par(Float, Double)] = Double;    
  r[par(Double, Integer)] = Double;    
  r[par(Double, Float)] = Double;    
  r[par(Double, Double)] = Double;    

  tro[ "-" ] = r; 
  tro[ "*" ] = r; 
  tro[ "/" ] = r; 

  r[par(Char, Char)] = String;      
  r[par(String, Char)] = String;      
  r[par(Char, String)] = String;    
  r[par(String, String)] = String;    
  tro[ "+" ] = r; 
  
  r.clear();
  r[par(Integer, Integer)] = Integer; 
  r[par(Float, Float)] = Integer;    
  r[par(Float, Double)] = Integer;    
  r[par(Double, Float)] = Integer;    
  r[par(Double, Double)] = Integer;    
  r[par(Char, Char)] = Integer;      
  r[par(String, String)] = Integer;      
  tro["=="] = r;
  tro["!="] = r;
  tro[">="] = r;
  tro[">"] = r;
  tro["<"] = r;
  tro["<="] = r;
}

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     printf( "%s\nNo final do arquivo\n", st );
   else  
     printf( "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
             yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}

int main( int argc, char* argv[] )
{
  inicializa_tabela_de_resultado_de_operacoes();
  yyparse();
}


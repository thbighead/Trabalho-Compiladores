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
  string nome;  
  string decl;  
  string fmt;   
  vector<int> dim;
};

Tipo Integer = { "integer", "int", "d" };
Tipo Float =   { "float", "float", "f" };
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
map<string,Tipo> tsl;
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
  if(escopo_local){
  	for( int i = 0; i < s2.lst.size(); i++ ) {
    	if( tsl.find( s2.lst[i] ) != tsl.end() ) 
      	erro( "Variável já declarada: " + s2.lst[i] );
    	else {
        if(s1.t.nome==String.nome){
          tsl[ s2.lst[i] ] = s1.t;
      	  ss.c += s1.t.decl + " " + s2.lst[i]+ "["+toString(s1.t.dim[0]+1)+"]" + s3 + ";\n"; 
        }
        else{
          tsl[ s2.lst[i] ] = s1.t;
          ss.c += s1.t.decl + " " + s2.lst[i]+ s3 + ";\n"; 
        }
    	}  
  	}
	}
	else{
		for( int i = 0; i < s2.lst.size(); i++ ) {
    	if( ts.find( s2.lst[i] ) != ts.end() ) 
      	erro( "Variável já declarada: " + s2.lst[i] );
    	else {
      	if(s1.t.nome==String.nome){
          ts[ s2.lst[i] ] = s1.t;
          ss.c += s1.t.decl + " " + s2.lst[i]+ "["+toString(s1.t.dim[0]+1)+"]" + s3 + ";\n"; 
        }
        else{
          ts[ s2.lst[i] ] = s1.t;
          ss.c += s1.t.decl + " " + s2.lst[i]+ s3 + ";\n"; 
        }
    	}  
  	}
	}
}

string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
   
  for( int i = 1; i <= qtde; i++ )
    if(t.nome!=String.nome){
      aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
    }
    else{
      aux += t.decl + " t_" + t.nome + "_" + toString( i )+"["+toString(t.dim[0]+1)+"]"+ ";\n";
    }
    
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = 
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
  if( (s1.t.nome == s3.t.nome  || (s1.t.nome=="integer" && s3.t.nome=="float")|| (s1.t.nome=="float" && s3.t.nome=="double")
  || (s1.t.nome=="double" && s3.t.nome=="float")|| (s1.t.nome=="float" && s3.t.nome=="integer")|| (s1.t.nome=="integer" && s3.t.nome=="double")
  || (s1.t.nome=="double" && s3.t.nome=="integer")) && s1.t.nome!=String.nome )
  {
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
  else if( s1.t.nome == s3.t.nome &&  s1.t.nome == String.nome) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim[0]) + " );\n";
  }
}

void gera1Dim(const Atributo& s2, const Atributo& s4){
//comment
	if(escopo_local){
    for( int i = 0; i < s2.lst.size(); i++ ) {
      tsl[s2.lst[i]].dim.push_back(toInt(s4.v));
    }
  }
  else{//comment
  	for( int i = 0; i < s2.lst.size(); i++ ) {
      ts[s2.lst[i]].dim.push_back(toInt(s4.v));
    }
  }//comment
}

void gera2Dim(const Atributo& s2, const Atributo& s4, const Atributo& s7){
//comment
	if(escopo_local){
    for( int i = 0; i < s2.lst.size(); i++ ) {
      tsl[s2.lst[i]].dim.push_back(toInt(s4.v));
      tsl[s2.lst[i]].dim.push_back(toInt(s7.v));
    }
  }
  else{//comment
  	for( int i = 0; i < s2.lst.size(); i++ ) {
      ts[s2.lst[i]].dim.push_back(toInt(s4.v));
      ts[s2.lst[i]].dim.push_back(toInt(s7.v));
    }
  }//comment
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
//comment
	if(escopo_local){
  	    if((tsl.find( s1.v ) == tsl.end()) && (ts.find( s1.v ) == ts.end()) ){
    	    erro( "Variável não declarada: " + s1.v );
        }
  	    else {
            if(tsl.find( s1.v ) == tsl.end()){
                ss.t = ts[ s1.v ];
            }
            else{
                ss.t = tsl[ s1.v ];
            }            
            ss.v = s1.v;
  	    }
	}
	else{//comment
		if( ts.find( s1.v ) == ts.end() ) {
    	    erro( "Variável não declarada: " + s1.v );
        }
      	else {
        	ss.t = ts[ s1.v ];
        	ss.v = s1.v;
      	}
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
      if(ss.t.nome==String.nome){
        ss.c = s1.c + s3.c + "  " +"strncpy("+ ss.v +","+  s1.v +toString(s1.t.dim[0])+");\n"+ "  strncat("+ss.v+"," + s3.v + 
        toString(s3.t.dim[0])+");\n";
      }
      else{
        ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v + ";\n";
      }
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

void gera_codigo_matrix(Atributo& ss, const Atributo& s1, const Atributo& s3, const Atributo& s6, const Atributo& s9){
	string aux1, aux2, aux3, axu4; 
	//Comment
	if(escopo_local){
  	if( (tsl.find( s1.v ) == tsl.end()) && (ts.find( s1.v ) == ts.end()) )
    	erro( "Variável não declarada: " + s1.v );
  	else if( s1.t.nome == s9.t.nome ){
    	if((tsl[s1.v].dim[0]-1)<toInt(s3.v) || (tsl[s1.v].dim[1]-1)<toInt(s6.v)){
      	erro("Acesso indevido ao Array\n");
    	}
    	aux1=gera_nome_var( Integer );
    	ss.c=ss.c+"  "+aux1+" = "+ s3.v + '*' + toString(tsl[s1.v].dim[1])+";\n";
    	aux2=gera_nome_var( Integer );
    	ss.c=ss.c+"  "+aux2+ " = "+ aux1 +'+'+ s6.v+";\n";
    	ss.c =s1.c + s3.c +ss.c+  "  " + s1.v + '[' + aux2 + ']' + " = " + s9.v + ";\n";    
  	}
  	else{
    	erro("Tipo errado na atribuição");
  	}
	}
	else{//Comment
		if( ts.find( s1.v ) == ts.end() )
    	erro( "Variável não declarada: " + s1.v );
  	else if( s1.t.nome == s9.t.nome ){
    	if((ts[s1.v].dim[0]-1)<toInt(s3.v) || (ts[s1.v].dim[1]-1)<toInt(s6.v)){
      	erro("Acesso indevido ao Array\n");
    	}
    	aux1=gera_nome_var( Integer );
    	ss.c=ss.c+"  "+aux1+" = "+ s3.v + '*' + toString(ts[s1.v].dim[1])+";\n";
    	aux2=gera_nome_var( Integer );
    	ss.c=ss.c+"  "+aux2+ " = "+ aux1 +'+'+ s6.v+";\n";
    	ss.c =s1.c + s9.c +ss.c+  "  " + s1.v + '[' + aux2 + ']' + " = " + s9.v + ";\n";    
  	}
  	else{
    	erro("Tipo errado na atribuição");
  	}
	}//Comment
}

void gera_codigo_vetor(Atributo& ss, const Atributo& s1, const Atributo& s3, const Atributo& s6){
	//comment
	if(escopo_local){
		if((tsl.find( s1.v ) == tsl.end()) && (ts.find( s1.v ) == ts.end()))
    	erro( "Variável não declarada: " + s1.v);
 		else if( s1.t.nome == s6.t.nome ){
   		if((tsl[s1.v].dim[0]-1)<toInt(s3.v)){
     		erro("Acesso indevido ao Array\n");
    	}
    ss.c =s1.c + s3.c +ss.c+  "  " + s1.v + '[' + s3.v + ']' + " = " + s6.v + ";\n";    
  	}
  	else{
    	erro("Tipo errado na atribuição");
  	}
	}
	else{//comment
  	if(ts.find( s1.v ) == ts.end())
    	erro( "Variável não declarada: " + s1.v);
  	else if( s1.t.nome == s6.t.nome ){
    	if((ts[s1.v].dim[0]-1)<toInt(s3.v)){
      	erro("Acesso indevido ao Array\n");
    	}
    	ss.c =s1.c + s3.c +ss.c+  "  " + s1.v + '[' + s3.v + ']' + " = " + s6.v + ";\n";    
  	}
  	else{
    	erro("Tipo errado na atribuição");
  	}
	}//comment
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
	if(s1.t.nome==String.nome||s1.t.nome==Char.nome){
		erro("Operação não permitida para esse tipo");
	}
	else{
		aux=gera_nome_var(s1.t);
		ss.c= "  " + aux + " = " + s1.v + ";\n";
		ss.c= ss.c + "  " + s1.v + " = " + aux + " + 1; \n";
	}
}

void gera_codigo_funcao(Atributo& ss,const Atributo& s2, const Atributo& s4, const Atributo& s7, const Atributo& s10){
	ss.c=s2.t.decl+" "+s4.v+" ("+ s7.c +"){\n  "+declara_var_temp(temp_local)+"  "+s10.c+"}\n";
}


void calcula_matrix( Atributo& ss, const Atributo& s1, const Atributo& s3, const Atributo& s6 ){
	string aux1, aux2;
	if(ts.find( s1.v ) == ts.end())
    	erro( "Variável não declarada: " + s1.v);
    	if((ts[s1.v].dim[0]-1)<toInt(s3.v)){
      	erro("Acesso indevido ao Array\n");
    	}
    	ss.t=ts[s1.v];
    	aux1=gera_nome_var( ss.t );
    	ss.c="  "+aux1+" = "+ s3.v + '*' + toString(ts[s1.v].dim[1])+";\n";
    	aux2=gera_nome_var( ss.t );
    	ss.c=ss.c+"  "+aux2+ " = "+ aux1 +'+'+ s6.v+";\n";
    	ss.c =s1.c + s3.c +ss.c + "\n"; 
    	ss.v= s1.v + '[' + aux2 + ']';
}


#line 430 "y.tab.c" /* yacc.c:339  */

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
    _INPUT = 280,
    _CTE_STRING = 281,
    _CTE_INTEGER = 282
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
#define _INPUT 280
#define _CTE_STRING 281
#define _CTE_INTEGER 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 532 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   320

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   282

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    35,     2,     2,
      40,    41,    33,    31,    39,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
      28,    30,    29,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   380,   380,   387,   390,   393,   394,   397,   398,   401,
     404,   405,   408,   410,   412,   416,   417,   418,   419,   420,
     423,   424,   427,   427,   431,   432,   435,   436,   439,   442,
     443,   446,   447,   448,   449,   450,   451,   454,   455,   456,
     459,   460,   463,   466,   467,   470,   473,   474,   477,   478,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   495,   496,   497,   498,   499,   500,   501
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
  "_PRINTLN", "_INPUT", "_CTE_STRING", "_CTE_INTEGER", "'<'", "'>'", "'='",
  "'+'", "'-'", "'*'", "'/'", "'%'", "';'", "'['", "']'", "','", "'('",
  "')'", "$accept", "S", "ABRE", "FECHA", "MIOLOS", "MIOLO", "FUNC",
  "FUNC_DECLS", "DECL", "TIPO", "ID", "FUNCTION", "$@1", "ARGS", "IDS",
  "PRINCIPAL", "CMDS", "CMD", "CMD_ATRIB", "CMD_ATOM", "LVALUE", "CMD_FOR",
  "CMD_INPUT", "CMD_IF", "SAIDA", "E", "F", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,    60,    62,
      61,    43,    45,    42,    47,    37,    59,    91,    93,    44,
      40,    41
};
# endif

#define YYPACT_NINF -67

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-67)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -7,   -67,   -67,   -67,   -67,   -67,    35,     7,    11,    -7,
     -67,    16,   -67,    15,   -67,   -67,   292,   -67,   -67,    28,
      27,   -67,    18,    19,    26,     8,    47,   -67,   292,     0,
      33,    -4,   -67,    37,   -67,    43,   -67,    44,    88,   -67,
      22,    22,    91,    55,    56,   -67,   -67,   -67,   -67,   -67,
     -67,   -67,    22,    22,   -67,   -67,    59,   -67,    58,    62,
     -67,   -67,   -67,    22,    49,   -67,    82,    61,    22,    91,
     250,   155,   -14,    35,    22,   111,    22,    22,    22,    22,
      22,    22,    22,    22,    22,    22,    22,   -67,   -67,   -67,
     126,    64,    -3,    24,   -67,    77,   104,    68,   -67,   174,
     -67,   265,   265,   265,   265,   265,   265,    52,    52,   -67,
     -67,   -67,    83,    22,    22,    22,    81,    85,    92,    90,
     292,   231,   250,   193,    93,    35,   -67,    22,    -1,    91,
      98,   -67,   -67,   112,   287,   212,   292,   131,    89,    95,
      22,   125,   -67,   -67,   -67,   122,   119,   121,   124,   250,
     127,   147,   -67,   292,   292,   -67,   134,   145,   146,   -67,
     156,   157,   141,   142,   -67,   -67
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,    15,    16,    17,    18,    19,     0,     0,     0,     6,
       7,     0,     8,     0,     1,     3,    30,     5,    21,     0,
       0,    42,     0,     0,     0,     0,     0,    28,    30,     0,
       0,     0,    33,     0,    32,     0,    12,     0,     0,    22,
       0,     0,     0,     0,     0,     4,     2,    29,    34,    35,
      40,    41,     0,     0,    36,    31,     0,    20,     0,    65,
      63,    62,    64,     0,     0,    61,     0,     0,     0,     0,
      37,     0,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    45,
       0,     0,     0,     0,    14,     0,     0,     0,    24,     0,
      68,    57,    58,    59,    60,    56,    55,    50,    51,    52,
      53,    54,     0,     0,     0,     0,     0,    27,     0,    67,
      30,     0,    39,     0,     0,     0,    11,     0,     0,     0,
       0,    13,    26,     0,    30,     0,    30,     0,     0,     0,
       0,     0,    10,     9,    66,     0,     0,     0,     0,    38,
       0,     0,    46,    30,    30,    23,     0,     0,     0,    47,
       0,     0,     0,     0,    44,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   -67,   -67,   163,   -67,   -67,   -67,    39,    -5,
     -67,   -67,   -67,   -67,    50,   -67,   -28,   -67,   -66,    45,
     -37,   -67,   -67,   -67,   -67,   -39,   -67
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,    16,    46,     8,     9,   133,   134,    10,    11,
      19,    12,    58,    97,    98,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    64,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      47,    13,    66,    91,   136,    67,    50,    14,     1,     2,
       3,     4,    43,    70,    71,     5,    51,    15,   137,    18,
      44,     6,    94,    95,    75,    59,    52,    52,    20,    90,
      39,    60,    92,    53,    53,    99,    48,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    61,    62,
       1,     2,     3,     4,   114,    45,    76,     5,    40,    41,
      77,   115,    63,   138,    36,    37,    42,    38,    96,    49,
      78,    56,    79,    54,   121,   122,   123,    80,    81,    55,
      82,    83,    84,    85,    86,    84,    85,    86,   135,    76,
      87,    57,   128,    77,    21,    68,    69,    72,    73,    74,
     113,   149,    89,    78,   116,    79,   143,   117,   145,   118,
      80,    81,   120,    82,    83,    84,    85,    86,    76,   124,
      96,   126,    77,    88,   125,   157,   158,   127,   140,   131,
     147,   141,    78,    76,    79,   146,   148,    77,   150,    80,
      81,   151,    82,    83,    84,    85,    86,    78,   152,    79,
     153,   156,   100,   154,    80,    81,   155,    82,    83,    84,
      85,    86,    76,   159,   160,   161,    77,   112,   162,   163,
     164,   165,    17,   142,   139,   132,    78,     0,    79,     0,
       0,    76,     0,    80,    81,    77,    82,    83,    84,    85,
      86,     0,     0,    93,     0,    78,     0,    79,     0,     0,
      76,     0,    80,    81,    77,    82,    83,    84,    85,    86,
       0,     0,   119,     0,    78,     0,    79,     0,     0,    76,
       0,    80,    81,    77,    82,    83,    84,    85,    86,     0,
       0,   130,     0,    78,     0,    79,     0,     0,    76,     0,
      80,    81,    77,    82,    83,    84,    85,    86,     0,     0,
     144,     0,    78,     0,    79,     0,     0,    76,     0,    80,
      81,    77,    82,    83,    84,    85,    86,   129,     0,     0,
       0,    78,    -1,    79,     0,     0,    -1,     0,    80,    81,
       0,    82,    83,    84,    85,    86,    -1,     0,    -1,     0,
      21,     0,     0,    -1,    -1,    21,    82,    83,    84,    85,
      86,    22,     1,     2,     3,     4,    22,     0,     0,     5,
       0,    23,    24,     0,     0,    25,    23,    24,     0,     0,
      25
};

static const yytype_int16 yycheck[] =
{
      28,     6,    41,    69,     5,    42,    10,     0,    15,    16,
      17,    18,     4,    52,    53,    22,    20,     6,    19,     3,
      12,    28,    36,    37,    63,     3,    30,    30,    13,    68,
       3,     9,    69,    37,    37,    74,    36,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    26,    27,
      15,    16,    17,    18,    30,     8,     7,    22,    40,    40,
      11,    37,    40,   129,    36,    37,    40,    39,    73,    36,
      21,    27,    23,    36,   113,   114,   115,    28,    29,    36,
      31,    32,    33,    34,    35,    33,    34,    35,   127,     7,
      41,     3,   120,    11,     3,    40,    40,    38,    40,    37,
      36,   140,    41,    21,    27,    23,   134,     3,   136,    41,
      28,    29,    29,    31,    32,    33,    34,    35,     7,    38,
     125,    29,    11,    41,    39,   153,   154,    37,    30,    36,
      41,    19,    21,     7,    23,     4,    41,    11,    13,    28,
      29,    19,    31,    32,    33,    34,    35,    21,    29,    23,
      29,     4,    41,    29,    28,    29,    29,    31,    32,    33,
      34,    35,     7,    29,    19,    19,    11,    41,    12,    12,
      29,    29,     9,   134,   129,   125,    21,    -1,    23,    -1,
      -1,     7,    -1,    28,    29,    11,    31,    32,    33,    34,
      35,    -1,    -1,    38,    -1,    21,    -1,    23,    -1,    -1,
       7,    -1,    28,    29,    11,    31,    32,    33,    34,    35,
      -1,    -1,    38,    -1,    21,    -1,    23,    -1,    -1,     7,
      -1,    28,    29,    11,    31,    32,    33,    34,    35,    -1,
      -1,    38,    -1,    21,    -1,    23,    -1,    -1,     7,    -1,
      28,    29,    11,    31,    32,    33,    34,    35,    -1,    -1,
      38,    -1,    21,    -1,    23,    -1,    -1,     7,    -1,    28,
      29,    11,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    21,     7,    23,    -1,    -1,    11,    -1,    28,    29,
      -1,    31,    32,    33,    34,    35,    21,    -1,    23,    -1,
       3,    -1,    -1,    28,    29,     3,    31,    32,    33,    34,
      35,    14,    15,    16,    17,    18,    14,    -1,    -1,    22,
      -1,    24,    25,    -1,    -1,    28,    24,    25,    -1,    -1,
      28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    16,    17,    18,    22,    28,    43,    46,    47,
      50,    51,    53,    51,     0,     6,    44,    46,     3,    52,
      13,     3,    14,    24,    25,    28,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    36,    37,    39,     3,
      40,    40,    40,     4,    12,     8,    45,    58,    36,    36,
      10,    20,    30,    37,    36,    36,    27,     3,    54,     3,
       9,    26,    27,    40,    67,    68,    67,    62,    40,    40,
      67,    67,    38,    40,    37,    67,     7,    11,    21,    23,
      28,    29,    31,    32,    33,    34,    35,    41,    41,    41,
      67,    60,    62,    38,    36,    37,    51,    55,    56,    67,
      41,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    41,    36,    30,    37,    27,     3,    41,    38,
      29,    67,    67,    67,    38,    39,    29,    37,    58,    36,
      38,    36,    56,    48,    49,    67,     5,    19,    60,    61,
      30,    19,    50,    58,    38,    58,     4,    41,    41,    67,
      13,    19,    29,    29,    29,    29,     4,    58,    58,    29,
      19,    19,    12,    12,    29,    29
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    44,    45,    46,    46,    47,    47,    48,
      49,    49,    50,    50,    50,    51,    51,    51,    51,    51,
      52,    52,    54,    53,    55,    55,    56,    56,    57,    58,
      58,    59,    59,    59,    59,    59,    59,    60,    60,    60,
      61,    61,    62,    63,    63,    64,    65,    65,    66,    66,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    68,    68,    68,    68,    68,    68,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     1,     2,     0,     1,     1,     2,
       2,     0,     3,     9,     6,     1,     1,     1,     1,     1,
       3,     1,     0,    13,     1,     0,     4,     2,     1,     2,
       0,     2,     1,     1,     2,     2,     2,     3,     9,     6,
       2,     2,     1,    14,    14,     4,    10,    12,     4,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     7,     4,     3
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
#line 381 "trabalho.y" /* yacc.c:1646  */
    { cout << "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n"<< declara_var_temp( temp_global ) << (yyvsp[-3]).c << "int main (){\n" <<(yyvsp[-1]).c<<"}"<<endl;
  }
#line 1749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 393 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c;}
#line 1755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 394 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c="";}
#line 1761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 397 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 398 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 401 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c=(yyvsp[-1]).c + (yyvsp[0]).c;}
#line 1779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 404 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 1785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 409 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-2]), (yyvsp[-1]),"" );}
#line 1791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 411 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-8]), (yyvsp[-7]),'['+toString(toInt((yyvsp[-5]).v) *toInt((yyvsp[-2]).v))+']'); gera2Dim((yyvsp[-7]), (yyvsp[-5]), (yyvsp[-2])); }
#line 1797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 413 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-5]), (yyvsp[-4]),'['+(yyvsp[-2]).v+']'); gera1Dim((yyvsp[-4]), (yyvsp[-2]));}
#line 1803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 416 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 1809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 417 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Float; }
#line 1815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 418 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 1821 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 419 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 420 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 1833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 423 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 424 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1845 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 427 "trabalho.y" /* yacc.c:1646  */
    {escopo_local=true; tsl.clear();}
#line 1851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 428 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_funcao((yyval),(yyvsp[-11]), (yyvsp[-9]),(yyvsp[-6]),(yyvsp[-3]));	escopo_local=false; tsl.clear(); }
#line 1857 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 431 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 432 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c="";}
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 435 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c=(yyvsp[-3]).t.decl + " " + (yyvsp[-2]).v+" , "+(yyvsp[0]).c;}
#line 1875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 436 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c= (yyvsp[-1]).t.decl + " " + (yyvsp[0]).v;}
#line 1881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 439 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 442 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c=(yyvsp[-1]).c+(yyvsp[0]).c;}
#line 1893 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 443 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c="";}
#line 1899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 446 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1905 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 447 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 448 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[0]);}
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 449 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 450 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 451 "trabalho.y" /* yacc.c:1646  */
    {(yyval)=(yyvsp[-1]);}
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 454 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atribuicao((yyval), (yyvsp[-2]), (yyvsp[0])); }
#line 1941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 455 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_matrix((yyval),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]), (yyvsp[0]));}
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 456 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_vetor((yyval),(yyvsp[-5]),(yyvsp[-3]),(yyvsp[0]));}
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 459 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atomico((yyval),(yyvsp[-1]),(yyvsp[0]));}
#line 1959 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 460 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atomico((yyval),(yyvsp[-1]),(yyvsp[0]));}
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 463 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 1971 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 466 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_for((yyval),(yyvsp[-10]),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]));}
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 467 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_for((yyval),(yyvsp[-10]),(yyvsp[-8]),(yyvsp[-6]),(yyvsp[-3]));}
#line 1983 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 470 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf(\"%" +(yyvsp[-1]).t.fmt + "\", &" + (yyvsp[-1]).v + ");\n" ;}
#line 1989 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 473 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_if( (yyval), (yyvsp[-6]), (yyvsp[-3]), "");}
#line 1995 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 474 "trabalho.y" /* yacc.c:1646  */
    {gera_cmd_if( (yyval), (yyvsp[-8]), (yyvsp[-5]), (yyvsp[-3]).c);}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 477 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\", " + (yyvsp[-1]).v + " );\n"; }
#line 2007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 478 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 481 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 482 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2025 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 483 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 484 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 485 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 486 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 487 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 488 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 489 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 490 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2073 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 491 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 495 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 2085 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 496 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Float; }
#line 2091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 497 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 498 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) );  }
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 499 "trabalho.y" /* yacc.c:1646  */
    { calcula_matrix( (yyval), (yyvsp[-6]), (yyvsp[-4]), (yyvsp[-1]) );  }
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 500 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = (yyvsp[-3]).v + "[" +(yyvsp[-1]).v +"]"+";\n";  }
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 501 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2125 "y.tab.c" /* yacc.c:1646  */
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
#line 504 "trabalho.y" /* yacc.c:1906  */


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
  String.dim.push_back(255);
  inicializa_tabela_de_resultado_de_operacoes();
  yyparse();
}


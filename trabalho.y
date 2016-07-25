%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Range { 
  int inicio, fim;
};

struct Tipo {
  string nome;  // O nome na sua linguagem
  string decl;  // A declaração correspondente em c-assembly
  string fmt;   // O formato para "printf"
  vector< Range > dim; // Dimensões (se não for array, fica vazio)
};

Tipo Integer = { "integer", "int", "d" };
Tipo Float =    { "real", "float", "f" };
Tipo Double =  { "double", "double", "lf" };
Tipo Boolean = { "boolean", "int", "d" };
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
  int aux = 0;
  
  sscanf( valor.c_str(), "%d", &aux );
  
  return aux;
}

%}

%token _ID _IF _ELSE _HTPL _COMPARACAO _BARRAHTPL _CTE_FLOAT _MAIOR _MENOR _BLOCK _BARRABLOCK _MAISMAIS
%token _FOR _ATRIB _FUNCTION _PRINT _INT _FLOAT _BOOL _DOUBLE _CHAR
%token _INTEGER _STRING

%token _CTE_STRING _CTE_INTEGER

%nonassoc  _COMPARACAO _MENOR _MAIOR _MAISMAIS
%left '='
%left '+' '-'
%left '*' '/''%'

%start S

%%

S : MIOLOS ABRE PRINCIPAL FECHA
  { cout << "Aceito" << endl; }
  ;
   
ABRE : _HTPL            
     ;  

FECHA: _BARRAHTPL
     ;
   
MIOLOS : MIOLO MIOLOS 
       | 
       ;
       
MIOLO : DECLS
      | FUNCTION
      ;     

DECLS: DECL ';'
     ;

DECL: TIPO ID 
    | TIPO ID '=' _CTE_INTEGER 
    | TIPO ID '=' _CTE_FLOAT
    | TIPO ID VETOR 
    ;


TIPO: _INT
    | _FLOAT
    | _DOUBLE
    | _BOOL
    | _CHAR
    | _STRING
    ;

ID: _ID ',' ID
  | _ID
  ;

FUNCTION: '<'_FUNCTION _ID'('ARGS')''>' CMD '<''/'_FUNCTION'>'
	;

ARGS: IDS 
    |
    ;
     
IDS : TIPO _ID ',' IDS
    | TIPO _ID
    ;      
   
PRINCIPAL : CMDS 
          ;
          
CMDS : CMD  CMDS 
     | 
     ;                   
 
CMD : SAIDA';'
    | CMD_IF
    | CMD_FOR
    | BLOCO
    | CMD_ATRIB';'
    ;
    
CMD_ATRIB : ID '=' E
          ;

VETOR: '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']'
     ;

CONDICAO: E '>' E
        | E '<' E
        | E _COMPARACAO E
	;       
    
CMD_FOR : '<'_FOR '('_ID '=' E ';' CONDICAO ';' E ')''>' CMD '<''/'_FOR'>'
        ;
    
BLOCO : _BLOCK CMDS _BARRABLOCK
      ;    
    
CMD_IF : '<'_IF '('CONDICAO')' '>' CMD '<''/'_IF '>'
       | '<'_IF '('CONDICAO')' '>' CMD '<'_ELSE'>' CMD  '<''/'_IF '>'
       ;    
    
SAIDA : '<'_PRINT _CTE_STRING '>'
      | '<'_PRINT _ID '>'
      | '<'_PRINT _CTE_INTEGER '>'
      | '<'_PRINT _CTE_FLOAT '>'
      ;
   
E : E '+' E
  | E '-' E
  | E '*' E
  | E '/' E
  | E '%' E
  | E _MAISMAIS
  | F
  ;
  
F : _CTE_STRING  
  | _CTE_FLOAT  
  | _CTE_INTEGER  
  | _ID  
  | '('E')'
  ;     
 
%%

#include "lex.yy.c"

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
  yyparse();
}


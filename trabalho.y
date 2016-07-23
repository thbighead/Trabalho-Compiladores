%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct Atributo {
  string v, t, c;
};

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);

%}

%token _ID _IF _ELSE _HTPL _COMPARACAO _BARRAHTPL _CTE_FLOAT _MAIOR _MENOR _BLOCK _BARRABLOCK _MAISMAIS
%token _FOR _ATRIB _FUNCTION _PRINT _INT _FLOAT
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

DECL: DECLA
    | DECLA '=' _CTE_INTEGER 
    | DECLA '=' _CTE_FLOAT
    ;

DECLA: TIPO ID
     ; 

TIPO: _INT
    | _FLOAT
    ;

ID: _ID ',' ID
  | _ID
  ;

FUNCTION: '<'_FUNCTION _ID'('ARGS')''>' CMD '<''/'_FUNCTION'>'
	;

ARGS: IDS
    |
    ;
     
IDS : _ID ',' IDS
    | _ID
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
    
CMD_ATRIB : IDS '=' E
	  | IDS '=' VETOR
          ;

VETOR: '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
     | '[' _CTE_INTEGER ']'
     ;

CONDICAO: E _MAIOR E
        | E _MENOR E
        | E _COMPARACAO E
	;       
    
CMD_FOR : '<'_FOR _ID '=' E ';' CONDICAO ';' E '>' CMD '<''/'_FOR'>'
        ;
    
BLOCO : _BLOCK CMDS _BARRABLOCK
      ;    
    
CMD_IF : '<'_IF CONDICAO '>' CMD '<''/'_IF '>'
       | '<'_IF CONDICAO '>' CMD '<'_ELSE'>' CMD  '<''/'_IF '>'
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


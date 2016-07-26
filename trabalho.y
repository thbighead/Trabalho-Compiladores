%{
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
  vector< int > dim;
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

/*string trata_dimensoes_decl_var( const Atributo& s3 ) {
  string aux;
  if(s3 == NULL){
    for( int i = 0; i < s3.dim.size(); i++ )
      aux += "[" + toString( s3.dim[i] )+ "]";
    return aux;
  }
  else{
    return "";
  }    
}
*/

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       const Atributo& s1, const Atributo& s2, const string s3 ) {
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


void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts.find( s1.v ) == ts.end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ s1.v ];
    ss.v = s1.v;
  }
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
  { cout << "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n"<< $1.c << "int main (){\n" <<$3.c<<"}"<<endl;
  }
  ;
   
ABRE : _HTPL            
     ;  

FECHA: _BARRAHTPL
     ;
   
MIOLOS : MIOLO MIOLOS {$$.c = $1.c + $2.c;}
       | {$$.c="";}
       ;
       
MIOLO : DECLS
      | FUNCTION
      ;     

DECLS: DECL ';'
     ;

DECL: TIPO ID                   { declara_variavel( $$, $1, $2,"" ); }
    | TIPO ID '=' _CTE_INTEGER 
    | TIPO ID '=' _CTE_FLOAT
    | TIPO ID VETOR { declara_variavel( $$, $1, $2,$3.c); }
    ;


TIPO: _INT      { $$.t = Integer; }
    | _FLOAT    { $$.t = Float; }
    | _DOUBLE   { $$.t = Double; }
    | _CHAR     { $$.t = Char; }
    | _STRING   { $$.t = String; }
    ;

ID: ID ',' _ID { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
  | _ID  { $$.lst.push_back( $1.v ); }
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
        {$$.c = '['+$2.v +']'+ '[' +$5.v+']'+'[' +$8.v+']'+'[' +$11.v+']';}
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'
        {$$.c = '['+$2.v +']'+ '[' +$5.v+']'+'[' +$8.v+']';}
     | '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']' 
        {$$.c = '['+$2.v+']'+'[' + $5.v+']';}
     | '[' _CTE_INTEGER ']' 
        {$$.c = '['+$2.v+']';}
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
  
F : _CTE_STRING   { $$ = $1; $$.t = String; }
  | _CTE_FLOAT    { $$ = $1; $$.t = Float; }
  | _CTE_INTEGER  { $$ = $1; $$.t = Integer; }
  | _ID           { busca_tipo_da_variavel( $$, $1 );  }
  | '('E')'       { $$ = $2; }
  ;     
 
%%

#include "lex.yy.c"

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
  yyparse();
}


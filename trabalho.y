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
  int aux = 0;
  
  sscanf( valor.c_str(), "%d", &aux );
  
  return aux;
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

void gera_codigo_operador( Atributo& ss, 
                           const Atributo& s1, 
                           const Atributo& s2, 
                           const Atributo& s3 ) {
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

%}

%token _ID _IF _ELSE _HTPL _COMPARACAO _BARRAHTPL _CTE_FLOAT _MAIOR _MENOR _BLOCK _BARRABLOCK _MAISMAIS
%token _FOR _ATRIB _FUNCTION _PRINT _INT _FLOAT _DOUBLE _CHAR _END
%token _INTEGER _STRING

%token _CTE_STRING _CTE_INTEGER

%nonassoc  _COMPARACAO '<' '>' _MAISMAIS
%left '='
%left '+' '-'
%left '*' '/' '%'

%start S

%%

S : MIOLOS ABRE PRINCIPAL FECHA
  { cout << "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n"<< declara_var_temp( temp_global ) << $1.c << "int main (){\n" <<$3.c<<"}"<<endl;
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
    | TIPO ID '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'  
    { declara_variavel( $$, $1, $2,'['+toString(toInt($4.v) *toInt($7.v))+']'); gera2Dim($2, $4, $7);}
    | TIPO ID '[' _CTE_INTEGER ']'{ declara_variavel( $$, $1, $2,'['+$4.v+']'); gera1Dim($2, $4);}
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

FUNCTION: '<'_FUNCTION _ID'('ARGS')''>' CMDS _END _FUNCTION'>'
	;

ARGS: IDS 
    |
    ;
     
IDS : TIPO _ID ',' IDS
    | TIPO _ID  
    ;      
   
PRINCIPAL : CMDS {$$.c=$1.c;}
          ;
          
CMDS : CMD  CMDS {$$.c=$1.c+$2.c;}
     | {$$.c="";}
     ;                   
 
CMD : SAIDA';'
    | CMD_IF
    | CMD_FOR
    | CMD_ATRIB';'
    ;
    
CMD_ATRIB : LVALUE '=' E { gera_codigo_atribuicao( $$, $1, $3 ); }
          | LVALUE '['E']''['E']' '=' E
          | LVALUE '['E']' '=' E
          ;  

LVALUE: _ID { busca_tipo_da_variavel( $$, $1 ); }
      ;    
    
CMD_FOR : '<'_FOR '('_ID '=' E ';' E ';' E ')''>' CMDS _END _FOR'>'
        ;    
    
CMD_IF : '<'_IF '('E')' '>' CMD _END _IF '>'
       | '<'_IF '('E')' '>' CMD '<'_ELSE'>' CMD  _END _IF '>'
       ;    
    
SAIDA : _PRINT '(' E ')'        { $$.c = "  printf( \"%"+ $3.t.fmt + "\", " + $3.v + " );\n"; }
      ;
   
E : E '+' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '*' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '%' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MAISMAIS
  | E '>' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '<' E     { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _COMPARACAO E { gera_codigo_operador( $$, $1, $2, $3 ); }
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


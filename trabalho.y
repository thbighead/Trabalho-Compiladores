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
    	if(s2.v)
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

void gera_cmd_input (Atributo& ss, const Atributo& s3){		//CMD_INPUT : LVALUE '=' _INPUT '(' _CTE_STRING ')' ';'
	ss.c = "scanf(\"" + " %" + s3.t.fmt + "\\n\", &" + s3.v + ");" ;
}

%}

%token _ID _IF _ELSE _HTPL _IGUALDADE _BARRAHTPL _CTE_FLOAT _MAISMAIS _DIFERENTE
%token _FOR _FUNCTION _PRINT _INT _FLOAT _DOUBLE _CHAR _END _MENOSMENOS _MAIORIGUAL
%token _STRING _MENORIGUAL _PRINTLN

%token _CTE_STRING _CTE_INTEGER

%nonassoc  _IGUALDADE '<' '>' _MAISMAIS _MENOSMENOS _DIFERENTE _MENORIGUAL _MAIORIGUAL
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

DECLS: DECL ';'  {$$=$1;}
     ;

DECL: TIPO ID                   
		{ declara_variavel( $$, $1, $2,"" ); }
    | TIPO ID '[' _CTE_INTEGER ']''[' _CTE_INTEGER ']'  
    { declara_variavel( $$, $1, $2,'['+toString(toInt($4.v) *toInt($7.v))+']'); gera2Dim($2, $4, $7);}
    | TIPO ID '[' _CTE_INTEGER ']'
    { declara_variavel( $$, $1, $2,'['+$4.v+']'); gera1Dim($2, $4);}
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

FUNCTION: '<'TIPO _FUNCTION _ID'('ARGS')''>' CMDS _END _FUNCTION'>'
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
 
CMD : SAIDA';'     		{$$=$1;}
    | CMD_IF       		{$$=$1;}
    | CMD_FOR      		{$$=$1;}
    | CMD_ATRIB';'    {$$=$1;}
    | CMD_ATOM';' 		{$$=$1;}
    ;
    
CMD_ATRIB : LVALUE '=' E 								{gera_codigo_atribuicao($$, $1, $3); }
          | LVALUE '['E']''['E']' '=' E {gera_codigo_matrix($$,$1,$3,$6, $9);}
          | LVALUE '['E']' '=' E 				{gera_codigo_vetor($$,$1,$3,$6);}
          ;  

CMD_ATOM: LVALUE _MAISMAIS   {gera_codigo_atomico($$,$1,$2);}
				| LVALUE _MENOSMENOS {gera_codigo_atomico($$,$1,$2);}
				;

LVALUE: _ID { busca_tipo_da_variavel( $$, $1 ); }
      ;    
    
CMD_FOR : '<'_FOR '('CMD_ATRIB';' E ';' CMD_ATOM ')''>' CMDS _END _FOR'>' {gera_cmd_for($$,$4,$6,$8,$11);}
				| '<'_FOR '('CMD_ATRIB';' E ';' CMD_ATRIB ')''>' CMDS _END _FOR'>' {gera_cmd_for($$,$4,$6,$8,$11);}
        ;    

CMD_INPUT : _INPUT '(' LVALUE ')' ';'		{ gera_cmd_input($$, $3) ; }
					;

CMD_IF : '<'_IF '('E')' '>' CMDS _END _IF '>'             {gera_cmd_if( $$, $4, $7, "");}
       | '<'_IF '('E')' '>' CMDS _ELSE CMDS  _END _IF '>' {gera_cmd_if( $$, $4, $7, $9.c);}
       ;    
    
SAIDA : _PRINT '(' E ')'        { $$.c = "  printf( \"%"+ $3.t.fmt + "\", " + $3.v + " );\n"; }
			| _PRINTLN '(' E ')'      { $$.c = "  printf( \"%"+ $3.t.fmt + "\\n\", " + $3.v + " );\n"; }
      ;
   
E : E '+' E     		 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E     		 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '*' E     		 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E    			 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '%' E     	   { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '>' E     		 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '<' E     		 { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _IGUALDADE E   { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _DIFERENTE E   { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MAIORIGUAL E  { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MENORIGUAL E  { gera_codigo_operador( $$, $1, $2, $3 ); }
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


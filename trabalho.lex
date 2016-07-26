%{
int yyrowno = 1;
void trata_folha();
void trata_aspas_simples();
%}
WS      [\t ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*

INICIO		"<"[Hh][Tt][Pp][Ll]">"
FIM		"</"[Hh][Tt][Pp][Ll]">"
BLOCKINIT	"<"[Bb][Ll][Oo][Cc][Kk]">"
BLOCKEND	"</"[Bb][Ll][Oo][Cc][Kk]">"
END             "</"
MAIOR		[Mm][Aa][Ii][Oo][Rr]
MENOR		[Mm][Ee][Nn][Oo][Rr]
INT		[Ii][Nn][Tt]
FLOAT		[Ff][Ll][Oo][Aa][Tt]
DOUBLE		[Dd][Oo][Uu][Bb][Ll][Ee]
CHAR		[Cc][Hh][Aa][Rr]
STRING		[Ss][Tt][Rr][Ii][Nn][Gg]
VAR		[Vv][Aa][Rr]
IF		[Ii][Ff]
ELSE		[Ee][Ll][Ss][Ee]
FOR		[Ff][Oo][Rr]
FUNCTION	[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]
PRINT		[Pp][Rr][Ii][Nn][Tt]

CTE_STRING	"\""([^'\n]|"''")*"\""
CTE_FLOAT	({DIGITO}+)"."{DIGITO}+
CTE_INTEGER 	{DIGITO}+

%%
"\n" { yylineno++; yyrowno = 1; }
{WS} { yyrowno += 1; }

{INICIO} 	{ trata_folha(); return _HTPL; }
{FIM} 		{ trata_folha(); return _BARRAHTPL; }
{END}           { trata_folha(); return _END; }
{MAIOR}		{ trata_folha(); return _MAIOR; }
{MENOR}		{ trata_folha(); return _MENOR; }
{INT}		{ trata_folha(); return _INT; }
{FLOAT}		{ trata_folha(); return _FLOAT; }
{BLOCKINIT}	{ trata_folha(); return _BLOCK; }
{BLOCKEND}	{ trata_folha(); return _BARRABLOCK; }
{DOUBLE}	{ trata_folha(); return _DOUBLE; }
{CHAR}		{ trata_folha(); return _CHAR; }
{STRING} 	{ trata_folha(); return _STRING; }
{IF} 		{ trata_folha(); return _IF; }
{ELSE} 		{ trata_folha(); return _ELSE; }
{FOR} 		{ trata_folha(); return _FOR; }
{FUNCTION}  	{ trata_folha(); return _FUNCTION; }
{PRINT}		{ trata_folha(); return _PRINT; }

{CTE_STRING} 	{ trata_aspas_simples(); return _CTE_STRING; }
{CTE_INTEGER} 	{ trata_folha(); return _CTE_INTEGER; }	
{CTE_FLOAT} 	{ trata_folha(); return _CTE_FLOAT; }	

{ID}  		{ trata_folha(); return _ID; }

"=="		{ trata_folha(); return _COMPARACAO; }
"++"		{ trata_folha(); return _MAISMAIS; }

.    		{ trata_folha(); return yytext[0]; }

%%

void trata_folha() {
  yylval.v = yytext;
  yylval.t.nome = "";
  yylval.t.decl = "";
  yylval.t.fmt = "";
  yylval.c = "";
  yylval.lst.clear();
  
  yyrowno += strlen( yytext ); 
}

void trata_aspas_simples() {
  trata_folha(); 
  yylval.v = "\"" + yylval.v.substr( 1, yylval.v.length()-2 ) + "\""; 
}


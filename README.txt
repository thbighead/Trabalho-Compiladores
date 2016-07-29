** Linguagem H.T.P.L.  (Htpl Tag Programming Language) **

Nome dos integrantes do grupo:
Hugo Faria Rezende Narcizo, Lucas Murakami Rocha da Costa, Thales Nathan Caicó Matias da Silva

DREs:
112177343, 112177864, 111179740

O make, que recebe de "entrada.htpl", gera um arquivo "saida.cc".
Para rodar o programa basta compilar "saida.cc" com o gcc.


Não conseguimos implementar funções totalmente. Falta a parte do return e chamadas de função.
Também só estamos trabalhando com variáveis globais.
Nossas palavras reservadas não são Case Sensitives (Print e PrInT dão no mesmo).

--- Exemplos de uso da linguagem: ---

1) Declarações de variaveis (globais):

int x, y, z;
float a, b[20];
double d;
char c;
string str;


------------------------------------

2) Nosso "main":

<htpl>
    comandos...
</htpl>


------------------------------------

3) Nosso "for":

<FOR(i = 0; i<linhas ; i++)>
    comandos...
</FOR>

------------------------------------

4) Nosso "if":

<IF( a < b )>
    comandos...
</IF>

------------------------------------

5) Nosso "printf" (um com \n outro sem):
int a;
string str;

println("Ola, Mundo!");
print(a);
println(str);

------------------------------------

6) Nosso "scanf":
int b;

print("Digite um inteiro: ");
input(b);



all: trabalho entrada.pas
	./trabalho < entrada.pas 

lex.yy.c: trabalho.lex
	lex trabalho.lex

y.tab.c: trabalho.y
	yacc trabalho.y -v

trabalho: lex.yy.c y.tab.c
	g++ -o trabalho y.tab.c -lfl

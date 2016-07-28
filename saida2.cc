#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int t_integer_1;
int t_integer_2;
int t_integer_3;
int t_integer_4;
int t_integer_5;
int t_integer_6;
int t_integer_7;
int t_integer_8;
int t_integer_9;
int t_integer_10;
int t_integer_11;
int t_integer_12;
int t_integer_13;
int t_integer_14;
int t_integer_15;
int t_integer_16;
int t_integer_17;
int t_integer_18;
int t_integer_19;
int t_integer_20;
int t_integer_21;
int t_integer_22;
int t_integer_23;
int t_integer_24;
int t_integer_25;
int t_integer_26;

int i;
int j;
int entrada;
int linhas;
int colunas;
int x[9];
int y[9];
int main (){
  linhas = 3;
  colunas = 3;
  i = 0;
  L_inicio_for_2:;
  t_integer_1 = i<linhas;
  if (!(t_integer_1)) goto L_fim_for_2;
  j = 0;
  L_inicio_for_1:;
  t_integer_3 = j<colunas;
  if (!(t_integer_3)) goto L_fim_for_1;
  printf( "%s", "Digite o valor para X[" );
  printf( "%d", i );
  printf( "%s", "][" );
  printf( "%d", j );
  printf( "%s", "]: " );
  scanf("%d", &entrada);
  t_integer_5 = i*3;
  t_integer_6 = t_integer_5+j;
  x[t_integer_6] = entrada;

  t_integer_4 = j;
  j = t_integer_4 + 1; 

  goto L_inicio_for_1;
  L_fim_for_1:;

  t_integer_2 = i;
  i = t_integer_2 + 1; 

  goto L_inicio_for_2;
  L_fim_for_2:;
  printf( "%s\n", "X :" );
  i = 0;
  L_inicio_for_4:;
  t_integer_7 = i<linhas;
  if (!(t_integer_7)) goto L_fim_for_4;
  j = 0;
  L_inicio_for_3:;
  t_integer_9 = j<colunas;
  if (!(t_integer_9)) goto L_fim_for_3;
  t_integer_11 = i*3;
  t_integer_12 = t_integer_11+j;

  printf( "%d", x[t_integer_12] );
  printf( "%s", " " );

  t_integer_10 = j;
  j = t_integer_10 + 1; 

  goto L_inicio_for_3;
  L_fim_for_3:;
  printf( "%s\n", "" );

  t_integer_8 = i;
  i = t_integer_8 + 1; 

  goto L_inicio_for_4;
  L_fim_for_4:;
  i = 0;
  L_inicio_for_6:;
  t_integer_13 = i<linhas;
  if (!(t_integer_13)) goto L_fim_for_6;
  j = 0;
  L_inicio_for_5:;
  t_integer_15 = j<colunas;
  if (!(t_integer_15)) goto L_fim_for_5;
  t_integer_17 = j*3;
  t_integer_18 = t_integer_17+i;

  t_integer_19 = i*3;
  t_integer_20 = t_integer_19+j;
  y[t_integer_20] = x[t_integer_18];

  t_integer_16 = j;
  j = t_integer_16 + 1; 

  goto L_inicio_for_5;
  L_fim_for_5:;

  t_integer_14 = i;
  i = t_integer_14 + 1; 

  goto L_inicio_for_6;
  L_fim_for_6:;
  printf( "%s\n", "X' :" );
  i = 0;
  L_inicio_for_8:;
  t_integer_21 = i<linhas;
  if (!(t_integer_21)) goto L_fim_for_8;
  j = 0;
  L_inicio_for_7:;
  t_integer_23 = j<colunas;
  if (!(t_integer_23)) goto L_fim_for_7;
  t_integer_25 = i*3;
  t_integer_26 = t_integer_25+j;

  printf( "%d", y[t_integer_26] );
  printf( "%s", " " );

  t_integer_24 = j;
  j = t_integer_24 + 1; 

  goto L_inicio_for_7;
  L_fim_for_7:;
  printf( "%s\n", "" );

  t_integer_22 = i;
  i = t_integer_22 + 1; 

  goto L_inicio_for_8;
  L_fim_for_8:;
}

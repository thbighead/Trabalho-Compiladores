#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char t_string_1[256];

char str[256];
int main (){
  strncpy(t_string_1,"abcd",255);
  strncat(t_string_1,"efgh",255);
  strncpy( str, t_string_1, 255 );
}
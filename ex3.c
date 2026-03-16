#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main ()
{
  int n = 0;
  char string[101];
  char string2[101];
  printf("Insira uma linha csv:");
  while (scanf("%[^,]", string) != EOF );
  { 
    if(string == ';')
      printf("\n");
    if(string == ',')
      printf("%20s", string2);
    else
      string2 = {string2, string};

    
  } 

  return 0;
}
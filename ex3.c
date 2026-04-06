#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main ()
{
  char c;
  char buffer[1000];
  int i = 0;
  int j;

  printf("Insira uma linha csv:");

  while (scanf("%c", &c) != EOF )
  { 
    if(c == ';')
    {
      buffer[i] = '\0';
      printf("%s", buffer);
      printf("\n");
      i = 0;
    }
    else
      if(c == ',')
      {    
        buffer[i] = '\0';
        printf("%s", buffer);
        for(j = 0; j < (20-i); j++)
          printf(" ");
        i = 0;
      }
      else
      {
        buffer[i++] = c;
      }   
  } 
  
  buffer[i] = '\0';
  printf("%s\n", buffer);
  
  return 0;
}
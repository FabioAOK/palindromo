#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  char string1[100];
  char string2[100];
  char mais_rep[100];
  int rep_atual= 0;
  int rep_max = 0;
  FILE *f = fopen("tarefa4.txt", "r");
  while(fscanf(f, "%s", string1) != EOF)
  {
    while(fscanf(f, "%s", string2) != EOF)
    {
      if(strcmp(string2, string1) == 0)
        rep_atual++;
      if(rep_atual > rep_max)
      {
        rep_max = rep_atual;
        strcpy(mais_rep, string1);
      }
    }
  }
  printf("%s\n", mais_rep);
}
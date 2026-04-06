#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  char string1[100];
  char string2[100];
  char mais_rep[100];
  int rep_atual;
  int rep_max = 0;
  FILE *f = fopen("tarefa4.txt", "r");

  if (f == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  while(fscanf(f, "%s", string1) != EOF)
  {
    rep_atual = 0;

    rewind(f);

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
    rewind(f);

    static int pos = 0;
    for (int i = 0; i <= pos; i++)
      fscanf(f, "%99s", string1);
    pos++;
  }
  printf("%s\n", mais_rep);
  fclose(f);
}
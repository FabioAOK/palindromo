#include <stdio.h>
#include <string.h>

int main()
{
  FILE *f = fopen("mapa.txt", "r");
  char *c;
  fscanf(f, "%c", c);
  fscanf(f, "%c", c);
  char ***m;
  int i = 0;
  int j = 0;
  while(fscanf(f, "%c", m[i][j]) != EOF)
  {
    if(strcmp(m[i][j], "/n") == 0)
    {
      j++;
      i = 0;
    }
    i++;
  }

  for(i = 0; i < 23; i++)
  {
    for(j  = 0; j < 41; j++)
      printf("%s", m[i][j]);
    printf("/n");
  }
  return 0;
}
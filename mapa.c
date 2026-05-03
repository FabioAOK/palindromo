#include <stdio.h>
#include <string.h>

int main()
{
  FILE *f = fopen("mapa.txt", "r");
  char m[22][41];
  int i = 0;
  int j = 0;
  while(fscanf(f, "%c", &m[i][j]) != EOF)
  {
    if(m[i][j] == '\n')
    {
      i++;
      j = 0;
    }
    j++;
  }

  for(i = 0; i < 22; i++)
  {
    for(j  = 0; j < 41; j++)
      printf("%c", m[i][j]);
    printf("\n");
  }
  return 0;
}
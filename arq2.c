#include <stdio.h>

int main()
{
    FILE *f = fopen("arq.c","r");
    char string[100];
    fscanf(f, "%[^%d]", string, feof(f));
    printf("%s", string);
    return 0;
}
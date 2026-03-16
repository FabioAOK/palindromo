#include <stdio.h>

int main()
{
    char string[100];
    scanf("%[^\n]", string);
    FILE *fp = fopen("teste.txt", "w");
    fprintf(fp, string);
    fclose(fp);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

struct produto
{
  char *nome;
  int quantidade;
  int custo;
  struct produto *prox;
};

struct venda
{
  struct produto *prim;
  struct produto *ult;
};

struct venda *cria_venda()
{
  struct venda *v;
  if(!(v = malloc(sizeof(struct venda))))
    return NULL;

  v->ult = NULL;
  v->prim = NULL;

  return v;
}

void add_prod(struct venda *v, char *n, int q, int c)
{
  struct produto *p;
  if(!(p = malloc(sizeof(struct produto))))
    return;
  p->nome = n;
  p->quantidade = q;
  p->custo = c;
  p->prox = NULL;

  if(v->prim == NULL)
  {
    p->prox = v->prim;
    v->prim = p;
  }

  if(v->ult)
    v->ult->prox = p;
  v->ult = p;
}

void imprime_venda(struct venda *v)
{
  if(!v)
    return;
  struct produto *p = v->prim;
  while(p != NULL)
  {
    printf("%s, valor: %d, quant: %d, valor total: %d\n", p->nome, p->custo, p->quantidade, p->custo * p->quantidade);
    p = p->prox;
  }
  return;
}

void destroi_venda(struct venda *v)
{
   if(!v)
    return;
  struct produto *p = v->prim;
  struct produto *aux;
  while(p != NULL)
  {
    aux = p;
    p = p->prox;
    free(aux);
  }
  free(v);
  return;
}

int main()
{
  struct venda *v = cria_venda();
  char n[100];
  int i = 1;
  int q, c;
  while(i == 1)
  {
    printf("Adcione seu produto:\n");
    scanf("%s", n);
    printf("\n");
    printf("Adicione o custo:\n");
    scanf("%d", &c);
    printf("\n");
    printf("Adicione a quantidade:\n");
    scanf("%d", &q);
    printf("\n");
    add_prod(v, n, q, c);
    printf("Gostaria de adicionar mais um produto?\n");
    printf("SIM(1)\n");
    printf("NÃO(0)\n");
    scanf("%d", &i);
  }
  imprime_venda(v);
  destroi_venda(v);
}
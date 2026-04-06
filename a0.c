#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct maquina
{
  int numeracao;
  int prob;
  struct maquina *prox;
  struct maquina *ant;
};

struct fila
{
  struct maquina *prim;
};

struct fila *cria_fila()
{
  struct fila *f;

  if(!(f = malloc(sizeof(struct fila))))
    return NULL;

  f->prim = NULL;

  return f;
}

void insere_fila(struct fila *f, struct maquina *m)
{
  if(f == NULL || m == NULL)
    return;

  if(f->prim == NULL)
  {
    f->prim = m;
    return;
  }
  
  struct maquina *i = f->prim;
  
  while(i->prox != NULL && m->prob < i->prob)
    i = i->prox;

  if(i == f->prim)
  {
    if(i->prob <= m->prob)
    {
    m->prox = i;
    i->ant = m;
    f->prim = m;
    return;
    }
    if(i->prox == NULL)
    {
    m->ant = i;
    m->prox = i->prox;
    i->prox = m;
    return;
    }
  }

  if(i->prox == NULL)
  {
    if(i->prob <= m->prob)
    {
      m->prox = i;
      i->ant->prox = m;
      m->ant = i->ant;
      i->ant = m;
      return;
    }
    else
    {
      i->prox = m;
      m->ant = i;
      return;
    }
  }

  i->ant->prox = m;
  m->ant = i->ant;
  m->prox = i;
  i->ant = m;

  return;
}

struct maquina *cria_maq(int n)
{
  struct maquina *m;

  if(!(m = malloc(sizeof(struct maquina))))
    return NULL;

  m->ant = NULL;
  m->prox = NULL;
  m->prob = rand() % 101;
  m->numeracao = n;

  return m;

}

struct maquina *acha_maq(struct fila *f, int maq)
{
  if(f == NULL || f->prim == NULL)
    return NULL;

  struct maquina *i = f->prim;

  while(i != NULL)
  {
    if(maq == i->numeracao)
      return i;
    i = i->prox;
  }
  return NULL;
}

void remove_fila(struct fila *f, int maq)
{
  if(f == NULL || f->prim == NULL)
    return;

  struct maquina *i = f->prim;

  while(i != NULL)
  {
    if(maq == i->numeracao)
    {
      if(i->ant == NULL && i->prox == NULL)
      {
        f->prim = NULL;
        free(i);
        return;
      }
      if(i->ant == NULL)
      {
        i->prox->ant = NULL;
        f->prim = i->prox;
        free(i);
        return;
      }
      if(i->prox == NULL)
      {
        i->ant->prox = NULL;
        free(i);
        return;  
      }
      i->ant->prox = i->prox;
      i->prox->ant = i->ant;
      free(i);
      return;
    }
    i = i->prox;
  }
  return;
}

void imprime_fila(struct fila *f)
{
  if(f == NULL || f->prim == NULL)
    return;

  struct maquina *i = f->prim;

  while(i != NULL)
  {
    printf("Maquina %d, ", i->numeracao);
    printf("probabilidade %d.\n", i->prob);
    i = i->prox;
  }
  return;
}

int main()
{
  srand(time(NULL));

  struct fila *f = cria_fila();

  int n_maq = (rand() % 12) + 1;

  for(int i = 1; i <= n_maq; i++)
    insere_fila(f, cria_maq(i));

  int jogadas_pretendidas = rand() % 50;
  int maximo_de_jogadas = (rand() % 30) + 20;

  for(int i = 0; i <= jogadas_pretendidas && i <= maximo_de_jogadas; i++)
  {
    int num_maq = (rand() % n_maq) + 1;
    struct maquina *maq = acha_maq(f, num_maq);

    if(maq != NULL)
      if(rand() % 101 <= maq->prob)
        remove_fila(f, num_maq);
  }
  
  imprime_fila(f);
}
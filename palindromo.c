#include <stdio.h>
#include <stdlib.h>

struct deque_nodo
{
	char item;					// item associado ao nodo
	struct deque_nodo *prox;	// próximo nodo
  struct deque_nodo *ant;
};


struct deque
{
	struct deque_nodo *prim ;	// primeiro nodo do deque
	struct deque_nodo *ult ;	// último nodo do deque
  struct deque_nodo *cursor;
	int num ;					// número de itens na fila
};

struct deque *deque_cria ()
{
  struct deque *d;
  if(!(d = malloc(sizeof(struct deque))))
    return NULL;
  d->prim = NULL;
  d->ult = NULL;
  d->num = 0;
  d->cursor = d->prim;
  return d;
}

struct deque *deque_destroi (struct deque *d)
{
  if(d == NULL)
    return NULL;

  struct deque_nodo *n = d->prim;
  struct deque_nodo *aux; 
  while(n != NULL)
  {
    aux = n->prox;
    free(n);
    n = aux;
  }
  free(d);
  return NULL;
}

int deque_insere_final (struct deque *d, char item)
{
  if (d == NULL)
    return 0;
  
  struct deque_nodo *n = malloc(sizeof(struct deque_nodo));
  if (!n) 
    return 0;

  n->item = item;
  n->prox = NULL;
  n->ant = NULL;
  
  if(d->prim == NULL)
  {
    d->prim = n;
    d->ult = n;
    d->cursor = d->prim;
    (d->num)++;
    return 1;
  }

  (d->ult)->prox = n;
  n->ant = d->ult;
  d->ult = n;
  (d->num)++;

  return 1;
}

int deque_insere_inicio (struct deque *d, char item)
{
  if (d == NULL)
    return 0;
  
  struct deque_nodo *n = malloc(sizeof(struct deque_nodo));
  if (!n) 
    return 0;

  n->item = item;
  n->prox = NULL;
  
  if(d->prim == NULL)
  {
    d->prim = n;
    d->ult = n;
    d->cursor = d->prim;
    (d->num)++;
    return 1;
  }

  n->prox = d->prim;
  (d->prim)->ant = n;
  d->prim = n;
  
  (d->num)++;
  return 1;
}

int deque_retira_inicio (struct deque *d, char *item)
{
  if(d == NULL || d->num == 0 || !item) 
    return 0;

  if(d->cursor == d->prim)
    d->cursor = (d->prim)->prox;
  
  struct deque_nodo *n = d->prim;

  (n->prox)->ant = NULL;
  d->prim = n->prox;
  (d->num)--;
  *item = n->item;
  free(n);

  return 1;    
}

int deque_retira_final (struct deque *d, char *item)
{
  if(d == NULL || d->num == 0) 
    return 0;

  if(d->cursor == d->ult)
    d->cursor = (d->ult)->ant;
  
  struct deque_nodo *n = d->ult;

  d->ult = n->ant;
  (d->ult)->prox = NULL;
  (d->num)--;
  *item = n->item;
  free(n);

  return 1;    
}

void deque_imprime (struct deque *d)
{
  if(d == NULL || d->num == 0)
    return;

  struct deque_nodo *n = d->prim;
  
  while(n != d->ult)
  {
    printf("%d ", n->item);
    n = n->prox;
  }
  printf("%d", n->item);

  return;
}

int deque_tamanho (struct deque *d)
{
  if (d == NULL)
    return -1;
  return d->num;
}

void next (struct deque *d)
{
  if(d == NULL || d->num == 0)
    return;

  d->cursor = (d->cursor)->prox;
}

void prev (struct deque *d)
{
  if(d == NULL || d->num == 0)
    return;

  d->cursor = (d->cursor)->ant;
}

int has_next (struct deque *d)
{
  if((d->cursor)->prox == NULL)
    return 0;
  
  else
    return 1;
}

int has_prev (struct deque *d)
{
  if((d->cursor)->ant == NULL)
    return 0;
  
  else
    return 1;
}

int main ()
{
  struct deque *d = deque_cria();
  char a = 'a';
  while(a != '.')
  {
    scanf("%c", &a);
    if(a != '.')
      deque_insere_final(d, a);
  }
  char b = '1';
  char c = '1';
  while((deque_tamanho(d) > 1) && (b == c))
  {
    deque_retira_final(d, &b);
    deque_retira_inicio(d, &c);
  }
  if(b == c)
    printf("É palíndromo\n");
  else
    printf("Não é palíndromo");
  return 0;
}
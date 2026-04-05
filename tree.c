#include <stdio.h>
#include <stdlib.h>

struct no 
{
  struct no *dir;
  struct no *esq;
  struct no *pai;
  int chave;
}.

struct tree
{
  int alt;
  struct no *raiz;
}.

void inorder_tree_walk (struct no *x)
{
  if(x != NULL)
  {
    inorder_tree_walk(x->esq);
    printf("%d, ", x->chave);
    inorder_tree_walk(x->dir);
  }
  printf("\n");
  return;
}


struct no *tree_search_rec (struct no *x, int k)
{
  if((x == NULL) || (k == x->chave))
    return x;
  if(k < x->chave)
    return tree_search_rec(x->esq, k);
  else
    return tree_search_rec(x->dir, k);
}

struct no *tree_search_int (struct no *x, int k)
{
  while((x != NULL) && (k != x->chave))
  {
    if(k < x->chave)
      x = x->esq;
    else
      x = x->dir;
  }
  return x;
}

struct no *tree_minimum (struct no *x)
{
  while(x->esq != NULL)
    x = x->esq;
  return x;
}

struct no *tree_maximum (struct no *x)
{
  while(x->dir != NULL)
    x = x->dir;
  return x;
}

struct no *tree_succerssor (struct no *x)
{
  if(x->dir != NULL)
    return tree_minimum(x->dir);
  else
  {
    y = x->pai;
    while((y != NULL) && (x == y->dir))
    {
      x = y;
      y = y->pai;
    }
    return y;
  }
}

void tree_insert (struct tree *t, struct no *z)
{
  struct no *x, *y;
  x = t->raiz;
  y = NULL;
  while(x != NULL)
  {
    y = x;
    if(z->chave < x->chave)
      x = x->esq;
    else
      x = x->dir;
  }
  z->pai = y;
  z->esq = NULL;
  z->dir = NULL;

  if(y == NULL)
  t->raiz = z;
  else if(z->chave < y->chave)
    y->esq = z;
  else
    y->dir = z;
  return;
}

void transplant (struct tree *t, struct no *u, struct no *v)
{
  if(u->pai == NULL)
    t->raiz = v;
  else if(u == u->pai->esq)
    u->pai->esq = v;
  else
    u->pai->dir = v;
  if(v != NULL)
    v->pai = u->pai;
}

void tree_delete (struct tree *t, struct no *z)
{
  if(z->esq == NULL)
    transplant(t, z, z-dir);
  else if(z->dir == NULL)
    transplant(t, z, z->esq);
  else
  {
    struct no *y = tree_minimum(z->dir);
    if(y != z->dir)
    {
      transplant(t, y, y->dir);
      y->dir = z->dir;
      y->dir->pai = y;
    }
    transplant(t, z, y);
    y->esq = z->esq;
    y->esq->pai = y;
  }
}

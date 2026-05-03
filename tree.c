#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define ALT(a) ((a == NULL)?(-1):(a->altura))


struct no 
{
  struct no *dir;
  struct no *esq;
  struct no *pai;
  int chave;
  int altura;
};

struct tree
{
  struct no *raiz;
};

struct tree *cria_arvore ()
{
  struct tree *t;
  if(!(t = malloc(sizeof(struct tree))))
    return NULL;
  return t;
}

struct no *cria_no (int c)
{
  struct no *n;
  if(!(n = malloc(sizeof(struct no))))
    return NULL;
  n->altura = 0;
  n->pai = NULL;
  n->esq = NULL;
  n->dir = NULL;
  n->chave = c;
  return n;
}

void inorder_tree_walk (struct no *x)
{
  if(x != NULL)
  {
    printf("(");
    inorder_tree_walk(x->esq);
    printf("%d", x->chave);
    inorder_tree_walk(x->dir);
    printf(")");
  }
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
  struct no *y;
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

struct no* tree_insert(struct tree *t, struct no *z)
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
  if(y == NULL)
    t->raiz = z;
  else if(z->chave < y->chave)
    y->esq = z;
  else
    y->dir = z;
  return y;
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

struct no* tree_delete(struct tree *t, struct no *z)
{
  struct no *x, *y;

  x = z->pai;
  if(z->esq == NULL)
    transplant(t, z, z->dir);
  else if(z->dir == NULL)
    transplant(t, z, z->esq);
  else
  {
    y = tree_minimum(z->dir);
    x = y->pai;
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
  return x;
}



struct no *rot_esq(struct no *p)
{
  struct no *q;
  q = p->dir;
  p->dir = q->esq;
  q->esq = p;
  q->pai = p->pai;
  p->pai = q;
  p->altura = 1 + MAX(ALT(p->esq), ALT(p->dir));
  q->altura = 1 + MAX(ALT(q->esq), ALT(q->dir));
  return q;
}

struct no *rot_dir(struct no *p)
{
  struct no *q;
  q = p->esq;
  p->esq = q->dir;
  q->dir = p;
  q->pai = p->pai;
  p->pai = q;
  p->altura = 1 + MAX(ALT(p->esq), ALT(p->dir));
  q->altura = 1 + MAX(ALT(q->esq), ALT(q->dir));
  return q;
}

struct no *rot_esqdir(struct no *p)
{
  struct no *q, *r;
  q = p->esq;
  r = q->dir;
  q->dir = r->esq;
  p->esq = r->dir;
  r->esq = q;
  r->dir = p;
  r->pai = p->pai;
  p->pai = r;
  q->pai = r;
  p->altura = 1 + MAX(ALT(p->esq), ALT(p->dir));
  q->altura = 1 + MAX(ALT(q->esq), ALT(q->dir));
  return r;

}

struct no *rot_diresq(struct no *p)
{
  struct no *q, *r;
  q = p->dir;
  r = q->esq;
  q->esq = r->dir;
  p->dir = r->esq;
  r->dir = q;
  r->esq = p;
  r->pai = p->pai;
  p->pai = r;
  q->pai = r;
  p->altura = 1 + MAX(ALT(p->esq), ALT(p->dir));
  q->altura = 1 + MAX(ALT(q->esq), ALT(q->dir));
  return r;

}

struct no *avl_balance (struct no *x) 
{
  int fb;
  struct no *y;

  y = x;
  fb = ALT(x->dir) - ALT(x->esq);
  if(fb < -1)
  {
    if(ALT(x->esq->esq) < ALT(x->esq->dir))
      y = rot_esqdir(x);
    else
      y = rot_dir(x);
  }
  else if(fb > 1)
  {
    if(ALT(x->dir->dir) < ALT(x->dir->esq))
      y = rot_diresq(x);
    else
      y = rot_esq(x);
  }
  return y;
}


void avl_insert (struct tree *t, struct no *z) 
{
  int a;
  struct no *x, *y;

  z->altura = 0;
  x = tree_insert(t, z);
  while(x != NULL) 
  {
    a = 1 + MAX(ALT(x->esq), ALT(x->dir));
    y = avl_balance(x);
    if(y != x) 
    {
      transplant(t, x, y);
      break;
    }
    if(x->altura == a)
      break;
    x->altura = a;
    x = x->pai;
  }        
}


void avl_delete (struct tree *t, struct no *z) 
{
  int a;
  struct no *x, *y;
    
  x = tree_delete(t, z);
  while(x != NULL) 
  {
    a = 1 + MAX(ALT(x->esq), ALT(x->dir));
    y = avl_balance(x);
    if(y != x)
    {
      transplant(t, x, y);
      x = y;
    }
    if(x->altura == a)
      break;
    x->altura = a;
    x = x->pai;
  }        
}

void tree_destroy(struct tree *t)
{
  struct no *x, *y, *z;
  x = tree_minimum(t->raiz);
  z = tree_maximum(t->raiz);
  y = x;
  while(x->chave <= z->chave)
  {
    x = tree_succerssor(x);
    free(y);
    y = x;
  }
  free(t);
}

int main()
{
  struct tree *t = cria_arvore();
  int a, c;
  int op = 1;
  printf("Árvore criada!\n");
  printf("Qual o tipo da arvore?\n");
  printf("(0)ABB\n");
  printf("(1)AVL\n");
  scanf("%d", &a);
  while(op != 0)
  {
    switch(a)
    {
      case 0:
        printf("O que deseja fazer?\n");
        printf("(0)Encerrar programa\n");
        printf("(1)Mostrar árvore\n");
        printf("(2)Adicionar nodo\n");
        printf("(3)Remover nodo\n");
        scanf("%d", &op);
        switch(op)
        {
          case 0:
            break;

          case 1:
            inorder_tree_walk(t->raiz);
            printf("\n");
            break;

          case 2:
            printf("Insira a chave do nó:\n");
            scanf("%d", &c);
            tree_insert(t, cria_no(c));
            break;

          case 3:
            printf("Insira a chave do nó:\n");
            scanf("%d", &c);
            tree_delete(t, tree_search_rec(t->raiz, c));
            break;

          default:
            break;
        }
        break;
      case 1:  
        printf("O que deseja fazer?\n");
        printf("(0)Encerrar programa\n");
        printf("(1)Mostrar árvore\n");
        printf("(2)Adicionar nodo\n");
        printf("(3)Remover nodo\n");
        scanf("%d", &op);
        switch(op)
        {
          case 0:
            break;

          case 1:
            inorder_tree_walk(t->raiz);
            printf("\n");
            break;

          case 2:
            printf("Insira a chave do nó:\n");
            scanf("%d", &c);
            avl_insert(t, cria_no(c));
            break;

          case 3:
            printf("Insira a chave do nó:\n");
            scanf("%d", &c);
            avl_delete(t, tree_search_rec(t->raiz, c));
            break;

          default:
            break;
        }
        break;

      default:
        break;
    }
  }
  tree_destroy(t);
  return 0;
}

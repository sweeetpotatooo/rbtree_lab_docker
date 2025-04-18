#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root=NULL;
  
  p->nil=malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->key=NULL;
  p->nil->left=NULL;
  p->nil->right=NULL;
  p->nil->parent=NULL;
  return p;
}

void _deleteAll(node_t *p) {
  if (p != NULL) {
    _deleteAll(p->left);
    _deleteAll(p->right);
    free(p);
  }
}

void delete_rbtree(rbtree *t) {
  if (t == NULL) return;

  _deleteAll(t->root); 
  free(t);             
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t * current = t->root;

  while (current != t->nil) {
    if (current->key == key)
      return current;

    if (current->key < key) 
      current = current->right;
    else
      current = current->left;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  if (t->root == t->nil) {
    return NULL;
  }
  node_t * current  = t->root;

  while (current->left != t->nil) {
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  if (t->root == t->nil) {
    return NULL;
  }

  node_t * curr = t->root;

  while (curr->right != t->nil) {
    curr = curr->right;
  }
  return curr;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil) {
    return 0;
  }
  size_t cnt = 0;
  subtree_to_array(t, t->root, arr, n, &cnt); 
  
  return 0;
}

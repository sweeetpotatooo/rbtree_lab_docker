//rbtree_lab/src/rbtree.c

#include "./rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = calloc(1, sizeof(rbtree));
  if (!t) return NULL;
  
  // sentinel 노드 생성 및 초기화
  t->nil = malloc(sizeof(node_t));
  if (!t->nil) { free(t); return NULL; }
  t->nil->color  = RBTREE_BLACK;
  t->nil->key    = 0;
  t->nil->left   = t->nil;
  t->nil->right  = t->nil;
  t->nil->parent = t->nil;

  // 빈 트리의 root는 sentinel
  t->root = t->nil;
  return t;
}

static void _deleteAll(rbtree *t, node_t *p) {
  if (p == t->nil) return;  // sentinel 건너뜀
  _deleteAll(t, p->left);
  _deleteAll(t, p->right);
  free(p);
}

void delete_rbtree(rbtree *t) {
  if (!t) return;
  _deleteAll(t, t->root);
  free(t->nil);
  free(t);
}


// 좌회전
void left_rotate(rbtree* t, node_t * x){
  node_t* y; 
  y= x->right;
  x->right = y->left;

  if (y->left != t->nil){
    y->left->parent =x;
  }
  y->parent = x->parent;

  if (x->parent ==t->nil){
    t->root =y;
  }
  else if (x == x->parent->left){
    x->parent->left =y;
  }
  else
  {
    x->parent->right =y;
  }

  y->left =x;
  x->parent = y;
  
}

// 우회전
void right_rotate(rbtree *t, node_t *x){
  node_t* y; 
  y= x->left;
  x->left = y->right;

  if (y->right != t->nil){
    y->right->parent =x;
  }
  y->parent = x->parent;

  if (x->parent ==t->nil){
    t->root =y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
    y->right =x;
    x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t * y;
  while(z->parent->color == RBTREE_RED){
    //왼쪽
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      //case_1
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        //case_2: z삼각형
        if (z == z->parent->right){
          z = z->parent;
          left_rotate(t,z);
        }
        //case_3: 일직선-> 색바꾸고 우회전
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    //오른쪽
    else {
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        //case 1
        z->parent->color = RBTREE_BLACK;
        y->color          = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        // CASE 2 -> Z 삼각형
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // CASE 3 -> 일직선-> 색 바꾸고 좌회전
        z->parent->color         = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t * x; //탐색용
  node_t * y; //부모 추적용
  node_t *z; // 삽입할 노드 
  x = t->root;
  y = t->nil;
  z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  //삽입 위치 탐색 
  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
    {
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  
  z->parent = y;
  
  if (y == t->nil){
    t->root = z;
  }
  
  else if(z->key < y->key){
    y->left = z;
  }
  
  else{
    y->right = z;
  } 

  // 삽입할 노드 설정
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  
  rbtree_insert_fixup(t,z);
  
  return z;
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

// 특정 노드부터 시작해서 그 서브트리의 최소값 노드를 반환
static node_t *subtree_min(rbtree *t, node_t *n) {
  while (n != t->nil && n->left != t->nil) {
    n = n->left;
  }
  return n;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }

  // 항상 부모 포인터 갱신 (nil 포함)
  v->parent = u->parent;
}


void rbtree_erase_fixup(rbtree *t, node_t *x){
  while ((x!= t->root) && (x->color ==RBTREE_BLACK)){
    node_t *w;
    //왼쪽
    if (x== x->parent->left){
      w = x->parent->right;
      // case1
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      // case2
      if ((w->left->color == RBTREE_BLACK)&& (w->right->color == RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      
      else{
        //case3
        if(w->right->color == RBTREE_BLACK){
          w->left->color =RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        //case4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x= t->root;
      }
    } 
    else{
      // 오른쪽
      w = x->parent->left;
      // case1
      if(w->color == RBTREE_RED){
          w->color =RBTREE_BLACK;
          x->parent->color = RBTREE_RED;
          right_rotate(t,x->parent);
          w = x->parent->left;
      }
      //case2
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color =RBTREE_RED;
        x = x->parent;
      }
      else{
        //case3
        if(w->left->color == RBTREE_BLACK){
          w->right->color =RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        //case4
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->left->color = RBTREE_BLACK;
          right_rotate(t,x->parent);
          x=t->root;
      }
    }
  }
  x->color= RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

  // CASE A: 왼쪽 자식이 nil인 경우
  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  // CASE B: 오른쪽 자식이 nil인 경우
  else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  // CASE C: 두 자식 모두 있는 경우
  else {
    y = subtree_min(t, p->right); // 오른쪽 서브트리의 최소값 (successorr)
    y_original_color = y->color;
    x = y->right;

    if (y->parent != p) {
      // y를 y->right로 대체
      rbtree_transplant(t, y, y->right);
      // y의 오른쪽 자식 연결
      y->right = p->right;
      y->right->parent = y;
    } else {
      // 슈도코드에서 빠졌던 핵심 부분 추가
      // y가 p의 오른쪽 자식인 경우, x의 부모는 y여야 한다.
      x->parent = y;
    }

    // p를 y로 대체
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);
  }

  free(p); // 삭제한 노드 메모리 해제

  return 0;
}


static void subtree_to_array(const rbtree *t, node_t *cur,
  key_t *arr, const size_t n, size_t *cnt) {
if (cur == t->nil || *cnt >= n) return;

// 왼쪽 서브트리
subtree_to_array(t, cur->left,  arr, n, cnt);
if (*cnt >= n) return;

// 현재 노드
arr[(*cnt)++] = cur->key;
if (*cnt >= n) return;

// 오른쪽 서브트리
subtree_to_array(t, cur->right, arr, n, cnt);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
size_t cnt = 0;
subtree_to_array(t, t->root, arr, n, &cnt);
return (int)cnt;
}
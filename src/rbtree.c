#include "rbtree.h"

#include <stdlib.h>
node_t SENTINEL = {
  .color = RBTREE_BLACK,
  .key = 0,
  .left = NULL,
  .right = NULL,
  .parent = NULL
};

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p -> nil = &SENTINEL;
  p -> root = p -> nil;
  return p;
}

void free_node(rbtree *t, node_t *node) {
  if (node != &SENTINEL){
    free_node(t, node -> left);
    free_node(t, node -> right);
    free(node);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}



void LEFT_ROTATE(rbtree *t, node_t *parent) {
  node_t *rotateNode = parent -> right;
  parent -> right = rotateNode -> left;
  if (rotateNode -> left != t -> nil) {
    rotateNode -> left -> parent = parent;
  }
  rotateNode -> parent = parent -> parent;
  if (parent -> parent == t -> nil) {
    t -> root = rotateNode;
  } else if (parent == parent -> parent -> left) {
    parent -> parent -> left = rotateNode;
  } else {
    parent -> parent -> right = rotateNode;
  }
  rotateNode -> left = parent;
  parent -> parent = rotateNode;
}

void RIGHT_ROTATE(rbtree *t, node_t *parent) {
  node_t *rotateNode = parent -> left;
  parent -> left = rotateNode -> right;
  if (rotateNode -> right != t -> nil) {
    rotateNode -> right -> parent = parent;
  }
  rotateNode -> parent = parent -> parent;
  if (parent -> parent == t -> nil) {
    t -> root = rotateNode;
  } else if (parent == parent -> parent -> left) {
    parent -> parent -> left = rotateNode;
  } else {
    parent -> parent -> right = rotateNode;
  }
  rotateNode -> right = parent;
  parent -> parent = rotateNode;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *curNode) {
  node_t *uncle;

  while (curNode -> parent -> color == RBTREE_RED) { // 문제가 없을때까지 루프 돌린다.
    if (curNode -> parent == curNode -> parent -> parent -> left) { // 만약 우리 엄마가  우리 할아버지의 왼쪽에 있다면
      uncle = curNode -> parent -> parent -> right; //
      if (uncle -> color == RBTREE_RED) {
        curNode -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        curNode -> parent -> parent -> color = RBTREE_RED;
        curNode = curNode -> parent -> parent;
      } else {
        if (curNode == curNode -> parent -> right) { 
          curNode = curNode -> parent;
          LEFT_ROTATE(t, curNode);
        }
        curNode -> parent -> color = RBTREE_BLACK;
        curNode -> parent -> parent -> color = RBTREE_RED;
        RIGHT_ROTATE(t, curNode -> parent -> parent);
      }
    } else {
      uncle = curNode -> parent -> parent -> left;
      if (uncle -> color == RBTREE_RED) {
        curNode -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        curNode -> parent -> parent -> color = RBTREE_RED;
        curNode = curNode -> parent -> parent;
      } else {
        if (curNode == curNode -> parent -> left) { //
          curNode = curNode -> parent;
          RIGHT_ROTATE(t, curNode);
        }
        curNode -> parent -> color = RBTREE_BLACK;
        curNode -> parent -> parent -> color = RBTREE_RED;
        LEFT_ROTATE(t, curNode -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
  return curNode;
} 

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *newNode = (node_t *)malloc(sizeof(node_t));
  node_t *parent = t -> nil;
  node_t *temp = t -> root;
  newNode -> key = key;
  while (temp != t -> nil) {
     parent = temp;
     if (key < temp -> key) {
       temp = temp -> left;
     } else {
       temp = temp -> right;
     }
  }
  newNode -> parent = parent; 
  
  if (parent == t -> nil) {
    t -> root = newNode;
  } else if (key < parent-> key) {
    parent -> left = newNode;
  } else {
    parent -> right = newNode;
  }
  newNode -> left = t -> nil;
  newNode -> right = t -> nil;
  newNode -> color = RBTREE_RED;
  rbtree_insert_fixup(t, newNode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t -> root;
  while (cur != t -> nil) {
    if (cur -> key == key) {
      return cur;
    } else if (key < cur -> key) {
      cur = cur -> left;
    } else {
      cur = cur -> right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  node_t *cur = t -> nil;
  while (temp != t -> nil) {
    cur = temp;
    temp = temp -> left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t -> nil;
  node_t *temp = t -> root;
  while (temp != t -> nil) {
    cur = temp;
    temp = temp -> right;
  }
  return cur;
}

node_t *rbtree_maximum(const rbtree *t, node_t *sucChild) {
  node_t *cur = sucChild;
  while (cur -> right != t -> nil) {
    cur = cur -> right;  
  }
  return cur;
}

void rbtree_erase_fixup(rbtree *t, node_t *replaceNode) {
  node_t *broNode;
  while (replaceNode != t -> root && replaceNode -> color == RBTREE_BLACK) {
    if (replaceNode == replaceNode -> parent -> left) {   
      broNode = replaceNode -> parent -> right;
      if (broNode -> color == RBTREE_RED) {   //case 1 의 경우
        broNode -> color = RBTREE_BLACK;
        replaceNode -> parent -> color = RBTREE_RED;
        LEFT_ROTATE(t, replaceNode -> parent);
        broNode = replaceNode -> parent -> right;
      }
      if (broNode -> left -> color == RBTREE_BLACK && broNode -> right -> color == RBTREE_BLACK) {    // case 2 의 경우
        broNode -> color = RBTREE_RED;
        replaceNode = replaceNode -> parent;
      } else {
        if (broNode -> right -> color == RBTREE_BLACK) {    // case 3의 경우
          broNode -> left -> color = RBTREE_BLACK;
          broNode -> color = RBTREE_RED;
          RIGHT_ROTATE(t, broNode);
          broNode = replaceNode -> parent -> right;
        }
        broNode -> color = replaceNode -> parent -> color;    // case 4의 경우
        replaceNode -> parent -> color = RBTREE_BLACK;
        broNode -> right -> color = RBTREE_BLACK;
        LEFT_ROTATE(t, replaceNode -> parent);
        replaceNode = t -> root;
      }
    } else {
      broNode = replaceNode -> parent -> left;
      if (broNode -> color == RBTREE_RED) {
        broNode -> color = RBTREE_BLACK;
        replaceNode -> parent -> color = RBTREE_RED;
        RIGHT_ROTATE(t, replaceNode -> parent);
        broNode = replaceNode -> parent -> left;
      }
      if (broNode -> right -> color == RBTREE_BLACK && broNode -> left -> color == RBTREE_BLACK) {
        broNode -> color =RBTREE_RED;
        replaceNode = replaceNode -> parent;
      } else {
        if (broNode -> left -> color == RBTREE_BLACK) {
          broNode -> right -> color =RBTREE_BLACK;
          broNode -> color =RBTREE_RED;
          LEFT_ROTATE(t, broNode);
          broNode = replaceNode -> parent -> left;
        }
        broNode -> color = replaceNode -> parent -> color;
        replaceNode -> parent -> color = RBTREE_BLACK;
        broNode -> left -> color = RBTREE_BLACK;
        RIGHT_ROTATE(t, replaceNode -> parent);
        replaceNode = t -> root;
      }
    }
  }
  replaceNode -> color = RBTREE_BLACK;
}

void rbtree_transplant(rbtree *t, node_t *eraseNode, node_t *successor) {
  if (eraseNode -> parent == t -> nil) {
    t -> root = successor;
  } else if (eraseNode == eraseNode -> parent -> left) {
    eraseNode -> parent -> left = successor;
  } else {
    eraseNode -> parent -> right = successor;
  }
  successor -> parent = eraseNode -> parent;
}



int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *eraseNode = p;
  color_t eraseOriginalColor = eraseNode -> color;
  node_t *replaceNode;
  if (p -> left == t -> nil) {
    replaceNode = p -> right;
    rbtree_transplant(t, p, p -> right);
  } else if (p -> right == t -> nil) {
    replaceNode = p -> left;
    rbtree_transplant(t, p, p -> left);
  } else {
    eraseNode = rbtree_maximum(t, p -> left);
    eraseOriginalColor = eraseNode -> color;
    replaceNode = eraseNode -> left;
    if (eraseNode -> parent == p) {
      replaceNode -> parent = eraseNode;
    } else {
      rbtree_transplant(t, eraseNode, eraseNode -> left);
      eraseNode -> left = p -> left;
      eraseNode -> left -> parent = eraseNode;
    }
    rbtree_transplant(t, p, eraseNode);
    eraseNode -> right = p -> right;
    eraseNode -> right -> parent = eraseNode;
    eraseNode -> color = p -> color;
  }
  if (eraseOriginalColor == RBTREE_BLACK) {
    rbtree_erase_fixup(t, replaceNode);
  }
  return 0;
}

void set_array(key_t *arr, node_t *cur, int *start_idx) {
  if (cur -> left == NULL) {
    return 0;
  }
  set_array(arr, cur -> left, start_idx);
  arr[*start_idx] = cur -> key;
  (*start_idx) = (*start_idx) + 1;
  set_array(arr, cur -> right, start_idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *cur = t -> root;
  if (!cur) {
    return;
  }
  int start_idx = 0;
  set_array(arr, cur, &start_idx);
  return 0;
}

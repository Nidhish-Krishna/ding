#define NODE_H

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

struct NODE
{
  char* key;
  short type;
  char* data;
  short height;
  struct NODE *left;
  struct NODE *right;
};

typedef struct NODE Node;
typedef struct NODE BinarySearchTree;

static Node* createNode(char* key, short type, char* data)
{
  Node *new_node = (Node*) malloc(sizeof(Node));
  new_node->key = key;
  new_node->type = type;
  new_node->data = data;

  new_node->height = 1;
  new_node->left = NULL;
  new_node->right = NULL;

  return new_node;
}

static char getNodeType(Node* node)
{
  if (node == NULL) return 0;
  else return ((Node*)(node->data))->type;
}



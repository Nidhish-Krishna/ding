#include "codes/node.h"
#include "datastructures/ntree.h"

void insertNodeNTree(Node* root, Node* child)
{
  if (root->left == NULL) { root->left = child; return; }
  else {;}

  Node* curr = root->left;
  while(curr->right!=NULL) curr = curr->right;
  curr->right = child;
}

void freeNTree(Node* root)
{
  if (root==NULL) return;
  else {;}

  freeNTree(root->left);
  freeNTree(root->right);

  Node* inner_node = (Node*)(root->data);

  if (inner_node->type == RAW_VARIABLE_NODE) free(inner_node->data);
  else {;}

  if (inner_node->type == MEMBER_NODE) { free(inner_node->data); free(inner_node); }
  else {;}

  free(root);
}

/*
======================================================================
|----------------- DEBUG FUNCTIONS USED BY THE AUTHOR ---------------|
|--------------------------------------------------------------------|
|-USE THESE OR DEFINE YOUR OWN FUNCTIONS TO DEBUG DURING DEVELOPMENT-|
======================================================================

void printLevel(Node* root)
{
  if(root == NULL) {printf("NO\n");return;}
  else {printf("\n");}

  printf("<%s>\n", ((Node*)(root->data))->key);
  printf("LEFT ");
  printLevel(root->left);
  printf("RIGHT ");
  printLevel(root->right);
}
*/


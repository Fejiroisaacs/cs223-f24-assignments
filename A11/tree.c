/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/25/2024
 * Description: Program for implementing a binary tree
 ---------------------------------------------*/
#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct tree_node* find(const char* name, struct tree_node* root){
  if( root == NULL ) return NULL;
  
  if( strcmp(name, root->data.name) < 0 ) return find( name, root->left );

  else if(strcmp(name, root->data.name) > 0 ) return find( name, root->right );

  else return root;
}

struct tree_node* insert(const char* name, struct tree_node* root){
  if( root == NULL ){
    root = (struct tree_node*) malloc ( sizeof (struct tree_node) );
    strcpy(root->data.name, name);
    root->left = root->right = NULL;
  } 
  else if( strcmp(name, root->data.name) < 0 ) root->left = insert( name, root->left );

  else if( strcmp(name, root->data.name) > 0 ) root->right = insert( name, root->right );

  return root;
}

void clear(struct tree_node* root){
  if (root->left != NULL) clear(root->left);
  if (root->right != NULL) clear(root->right);
  if (root != NULL) free(root);

}

void print_help(struct tree_node* root, int level){
  char *space = malloc((sizeof(char)*level) + 1);
  for(int i = 0; i < level; ++i) space[i] = ' ';
  space[level] = '\0';
  if (root->left != NULL) printf("%sl:%s\n", space, root->left->data.name);
  if (root->right != NULL) printf("%sr:%s\n", space, root->right->data.name);
  if (root->left != NULL) print_help(root->left, level+1);
  if (root->right != NULL) print_help(root->right, level+1);

  free(space);
}

void print(struct tree_node* root){
  printf("%s\n", root->data.name);
  return print_help(root, 1);
}

void printSorted(struct tree_node* root){
  if (root->left != NULL) printSorted(root->left);
  if (root != NULL) printf("%s\n", root->data.name);
  if (root->right != NULL) printSorted(root->right);
}


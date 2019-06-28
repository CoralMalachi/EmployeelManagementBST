
#ifndef BST_H
#define BST_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct nodes {
	struct nodes* left;
	struct nodes* right;
	char* name;
}node;

typedef struct tree_t {
	node* root;	
}tree;

void menu(tree* employeeRoot);
char* copyString(char* name);
tree* createTree();
node* createNode(char* name);

node* insertNode(node* root, char* name);

void AddEmployee(tree* root);
void RemoveEmployee(tree* root);

void CreateEmployeeFile(tree* root);
void WriteNamesIntoFile(node* root, FILE* fp);

void printEmpNames(node* root);
void printTree(tree* root);

char* read_line(char* line);
/*
how delete node from BST works?
https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/
*/


node* deleteNode(node* root, char* name);
node* minValueNode(node* root); 
void _freeAllNodes(node** node_ref);
void freeAllNodes(node* node);
void freeTree(tree* t);




#endif
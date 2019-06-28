#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include "BST.h"


void menu(tree* employeeRoot)
{
	int stay = 1;
	int selection;
	do
	{
		printf("\n______________________________________________________________________________________\n");
		printf("|Option Number|                            Description                               |\n");
		printf("|_______________|____________________________________________________________________|\n");
		printf("|       1       |Print current employees                                             |\n");
		printf("|       2       |Add new employee                                                    |\n");
		printf("|       3       |Remove employee                                                     |\n");
		printf("|       4       |Create file with current employee list                              |\n");		
		printf("|       0       |Exit program                                                        |\n");
		printf("|_______________|____________________________________________________________________|\n");
		printf("\nWhat do you want to do?\n\n");

		scanf("%d", &selection);
		getchar();

		switch (selection)
		{
		case 0:
			//free allocated memory end terminate program
			freeTree(employeeRoot);
			break;
		case 1:
			printTree(employeeRoot);
			break;
		case 2:
			AddEmployee(employeeRoot);
			break;
		case 3:
			RemoveEmployee(employeeRoot);
			break;
		case 4:
			CreateEmployeeFile(employeeRoot);
			break;	

		default:
			printf("\nERROR: You did not enter a valid input. Please try again.\n\n");
			break;
		}

	} while (selection!=0);
	   
}

char* copyString(char* name)
{
	char* s = malloc(strlen(name) + 1);
	if (s != NULL) {
		strcpy(s, name);
	}
	return s;
}

tree* createTree()
{
	tree* t = malloc(sizeof(tree));
	t->root = NULL;
	return t;	
}

node* createNode(char* name)
{
	node* n = (node *)malloc(sizeof(node));
	n->name = copyString(name);
	n->left= n->right = NULL;	
	return n;
}

node* insertNode(node* root, char* name)
{
	/* Otherwise, recur down the tree */
	if (root == NULL) {
		return createNode(name);
	}
	//if name is alphanumerically less than current root
	//current employee goes to the left
	if (strcmp(name, root->name) < 0) {
		root->left = insertNode(root->left, name);
	}
	else if (strcmp(name, root->name) > 0) {
		//otherwise current employee goes to the right
		root->right = insertNode(root->right, name);
	}
	//nothing is inserted if name already exists

	return root;
}

void AddEmployee(tree* root)
{
	printf("Enter name of new employee: ");
	char* name = NULL;
	name = read_line(name);
	root->root = insertNode(root->root, name);
	free(name);
}


void RemoveEmployee(tree* root)
{
	printf("Enter name of the employee you want to delete: ");
	char* name = NULL;
	name = read_line(name);
	root->root = deleteNode(root->root, name);
	free(name);
}

void CreateEmployeeFile(tree* root)
{
	/*
	If the file exists, its contents are overwritten. 
	If the file does not exist, it will be created.
	*/
	FILE* fp = fopen("EmpList.txt", "w");
	if (fp == NULL) {
		printf("Couldn't open file.\n");
		freeTree(root);
		exit(1);
	}
	WriteNamesIntoFile(root->root, fp);
	fclose(fp);


}

void WriteNamesIntoFile(node* root, FILE* fp)
{
	if (root) {
		if (root->left) {
			WriteNamesIntoFile(root->left,fp);
		}
		fprintf(fp,"%s \n", root->name);
		if (root->right) {
			WriteNamesIntoFile(root->right, fp);
		}
	}
}

//takes root of tree and prints all employee names
void printEmpNames(node* root)
{
	if (root != NULL) {
		if (root->left)
			printEmpNames(root->left);
		printf("%s \n", root->name);
		if (root->right)
			printEmpNames(root->right);
	}
}

void printTree(tree* root)
{
	if (root->root == NULL)
		printf("%s", "tree is empty\n");
	else
	{
		printf("\n printing tree: \n");
		printEmpNames(root->root);
	}
}

char* read_line(char* line)
{
	int c;
	line = malloc(sizeof(char));
	//checking if allocation was successful or not
	if (line == NULL)
	{
		printf("%s\n", "Allocation failed");
		exit(1);
	}

	/*
	unsigned type might be seen used as loop variable as loop variables
	are typically greater than or equal to 0
	*/

	size_t i = 0;
	while ((c = getchar()) != '\n' && c != EOF)
	{
		//type casting `int` to `char`, since getchar return int
		line[i] = (char)c;
		i++;
		line = realloc(line, (i + 1) * sizeof(char));
		if (line == NULL) {
			printf("%s\n", "Reallocation failed");
			exit(1);
		}
	}
	//inserting null character at the end
	line[i] = '\0';
	return line;
	
}
/*
https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/

When we delete a node, three possibilities: 
1) Node to be deleted is leaf: Simply remove from the tree.

	          50                            50
           /     \         delete(20)      /   \
          30      70       --------->    30     70 
         /  \    /  \                     \    /  \ 
       20   40  60   80                   40  60   80

2) Node to be deleted has only one child: Copy the child to the node and delete the child
	
			 50                            50
		   /     \         delete(30)      /   \
		  30      70       --------->    40     70
			\    /  \                          /  \
			40  60   80                       60   80

3) Node to be deleted has two children:
	
	you replace said node with the left most child on its right side
	then delete the child from the bottom that it was replaced with

			 50                              60
           /     \         delete(50)      /   \
          40      70       --------->    40    70 
                 /  \                            \ 
                60   80                           80

*/
node* deleteNode(node* root, char* name)
{
	//base case:
	if (root == NULL)
	{
		printf("\n there is no employee with the following name: %s \n", name);
		return root;
	}
		
	//if name is alphanumerically less than current root
	//current employee goes to the left
	if (strcmp(name, root->name) < 0) {
		root->left = deleteNode(root->left, name);
	}//go right
	else if (strcmp(name, root->name) > 0) {
		root->right = deleteNode(root->right, name);
	}//if we found what need to be deleted
	else {
		//if node has only one child (right) or no child at all
		if (root->left == NULL) {
			node* tmp = root->right;
			//printf("%s is deleted ")
			free(root->name);
			free(root);
			
			return tmp;
			//if node has only left child
		}
		else if (root->right == NULL) {
			node* tmp = root->left;
			free(root->name);
			free(root);
			return tmp;
		}//if node has two childrens
		else {
			//we need to find the left most child on its right side
			node* tmp = minValueNode(root->right);
			free(root->name);
			//replace curent node with the left most child on its right side
			root->name = copyString(tmp->name);
			// Delete the inorder successor
			root->right = deleteNode(root->right, tmp->name);
		}

	}
	return root;
}

/*
Given a non-empty binary search tree, return the node with minimum
   key value found in that tree. Note that the entire tree does not
   need to be searched == the left most child on its right side
*/
node* minValueNode(node* root)
{
	node* cure = root;
	while (cure && cure->left )
	{
		cure = cure->left;
	}
	return cure;
}

void _freeAllNodes(node** node_ref)
{
	freeAllNodes(*node_ref);
	//set root to nul
	*node_ref = NULL;
}

/*
This function traverses tree in post order to
	to delete each and every node of the tree
	but forget to set root to null - call from _freeAllNodes
https://www.geeksforgeeks.org/write-a-c-program-to-delete-a-tree/
*/
void freeAllNodes(node* node)
{
	if (node == NULL)
		return;
	freeAllNodes(node->left);
	freeAllNodes(node->right);

	free(node->name);
	free(node);
}

void freeTree(tree* t)
{
	_freeAllNodes(&(t->root));
	free(t);
}

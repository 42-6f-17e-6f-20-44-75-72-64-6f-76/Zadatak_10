#define _CRT_SECURE_NO_WARNINGS
#define DATA_SIZE (11)
#define FILE_NAME_SIZE (1024)

#include <stdio.h>
#include <stdlib.h>

//postfix.txt

struct _node;
typedef struct _node* NodePosition;
typedef struct _node {
	char data[DATA_SIZE];
	NodePosition left;
	NodePosition right;
}Node;

struct _list;
typedef struct _list* ListPosition;
typedef struct _list {
	NodePosition data;
	ListPosition next;
}List;


NodePosition createNode(char*);
ListPosition createListNode(NodePosition);
int push(ListPosition, NodePosition);
int pushBack(ListPosition, NodePosition);
NodePosition pop(ListPosition);
NodePosition realPostfixFromFile(char*);
int isNumber(char*);
int toInfixList(ListPosition, NodePosition);

NodePosition deleteTree(NodePosition);
ListPosition deleteList(ListPosition);

int main(void) {
	NodePosition root = NULL;
	List infixResult;

	infixResult.next = NULL;

	char filename[FILE_NAME_SIZE] = { 0 };
	printf("Insert file name...");
	scanf(" %s", filename);

	root = realPostfixFromFile(filename);

	if (NULL == root) {
		return EXIT_FAILURE;
	}

	toInfixList(&infixResult, root);

	for (ListPosition p = infixResult.next; p != NULL; p = p->next) {
		printf("%s ", p->data->data);
	}


	root = deleteTree(root);
	infixResult.next = deleteList(infixResult.next);

	puts("");
	system("pause");
	return EXIT_SUCCESS;
}


NodePosition deleteTree(NodePosition root){
	if (root == NULL)return NULL;

	deleteTree(root->left);
	deleteTree(root->right);

	free(root);

	return NULL;
}

ListPosition deleteList(ListPosition head){

	if (head == NULL)return NULL;

	head->next=deleteList(head->next);

	free(head);

	return NULL;
}

int toInfixList(ListPosition head, NodePosition current) {
	if (NULL == current)return 1;

	toInfixList(head, current->left);
	pushBack(head, current);
	toInfixList(head, current->right);

	return 0;
}

int pushBack(ListPosition head, NodePosition data) {

	ListPosition p = head;
	while (p->next != NULL) {
		p = p->next;
	}

	return push(head, data);
}

int push(ListPosition head, NodePosition data) {
	ListPosition node = NULL;
	node = createListNode(data);
	if (NULL == node) {
		return -1;
	}

	node->next = head->next;
	head->next = node;
	return 0;

}

int isNumber(char* str) {
	int number = 0;

	if (sscanf(str, " %d", &number)==1) {
		return 1;
	}
	else {
		return 0;
	}

}

NodePosition pop(ListPosition head) {

	if (head->next == NULL) 
		return NULL;
	
	ListPosition first = head->next;
	NodePosition result = NULL;

	
	head->next = first->next;
	result = first->data;
	free(first);

	return result;
}

ListPosition createListNode(NodePosition data) {
	ListPosition node = NULL;
	node = (ListPosition)malloc(sizeof(List));
	if (NULL == node)
	{
		printf("Memory allocation failed!\r\n");
		return NULL;
	}
	node->data = data;

	node->next = NULL;

	return node;
}

NodePosition createNode(char* data) {

	NodePosition p = NULL;

	p = (NodePosition)malloc(sizeof(Node));

	if (NULL == p)
	{
		printf("Memory allocation failed!\r\n");
		return NULL;
	}

	strcpy(p->data, data);
	p->left = NULL;
	p->right = NULL;

	return p;
}

NodePosition realPostfixFromFile(char* filename) {

	FILE* fp = NULL;
	NodePosition result = NULL;
	char realString[DATA_SIZE] = { 0 };
	List head;

	head.data = NULL;
	head.next = NULL;

	fp = fopen(filename, "r");

	if (NULL == fp)
	{
		printf("File %s doesn't exist!", filename);
		return NULL;
	}

	while (!feof(fp)) {
		NodePosition node = NULL;
		fscanf(fp, " %s", realString);
		node = createNode(realString);
		if (NULL == node) {
			fclose(fp);
			return NULL;
		}
		if (!isNumber(node->data)) {

			node->right = pop(&head);

			if (NULL == node->right) {
				printf("Postfix isn't right, please check it!\r\n");
				return NULL;
			}

			node->left = pop(&head);

			if (NULL == node->left) {
				printf("Postfix isn't right, please check it!\r\n");
				return NULL;
			}
		}
		push(&head, node);
	}

	fclose(fp);

	result = pop(&head);
	if (NULL == result) {
		printf("File is empty!\r\n");
		return NULL;

	}
	
	if (pop(&head) != NULL) {
		printf("Postfix isn't right, please check it!\r\n");
		return NULL;
	}

	return result;
}
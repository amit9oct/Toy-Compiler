/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef STACK_H
#define STACK_H
/*
************************************************************************************************
*	Node:
*		This a generic node and this can be used to implement a generic stack which can
*		store the pNode(pointer node) for any datatype.
************************************************************************************************
*/
typedef struct node{
	void* pNode;
	struct node* next;
}Node;

/*
************************************************************************************************
*	Stack:
*		Data structure for implementing stack.
************************************************************************************************
*/
typedef struct stack{
	Node* head;
}Stack;

Stack* stackCreate();
void* top(Stack* s);
void push(Stack* s, void* val);
void pop(Stack* s);
int isEmpty(Stack* s);
#endif

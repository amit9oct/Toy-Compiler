/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
Stack* stackCreate(){
	Stack* s = (Stack*)malloc(sizeof(Stack));
	s->head = NULL;
	return s;
}

void* top(Stack* s){
	if(s->head !=NULL)
		return (s->head)->pNode;
	else
		return NULL;
}

void push(Stack* s, void* val){
	Node* newHead = (Node*)malloc(sizeof(Node));
	newHead->pNode = val;
	newHead->next = s->head;
	s->head = newHead;
}

void pop(Stack* s){
	if(s->head != NULL){
		Node* temp = s->head;
		s->head = temp->next;
		free(temp);
	}
}

int isEmpty(Stack* s){
	if(s->head == NULL)
		return 1;
	else
		return 0;
}
/*
void printS(Stack* S){
	Node* head = S->head;
	while(head!=NULL){
		printf("%c\n",*((char*)head->pNode));
		head = head->next;
	}
	printf("\n");
}

int main(){
	Stack* S = stackCreate();
	char* temp1 = (char*)malloc(sizeof(char));
	char* temp2 = (char*)malloc(sizeof(char));
	char* temp3 = (char*)malloc(sizeof(char));
	*temp1='A';
	*temp2='B';
	*temp3='C';		
	push(S,temp1);
	printS(S);
	push(S,temp2);
	push(S,temp3);
	printS(S);		
	pop(S);
	printS(S);
	return 0;
}*/

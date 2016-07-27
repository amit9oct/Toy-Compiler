/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef AST_DEF_H
#define AST_DEF_H
#include "lexer.h"

/*
*************************************************************************
*	ASTNode:
*		Data structure used to store the information about the AST node.
*************************************************************************
*/

typedef struct astNode{
	Token* token;
	int ruleNum;
	struct astNode* parent;
	struct astNode* child;
	struct astNode* next;
}ASTNode;
#endif
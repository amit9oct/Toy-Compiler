/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef AST_H
#define AST_H
#include "astDef.h"
#include "parser.h"
extern int PRINT_AST; //Flag if set the AST will be printed on the stdout
extern int AST_SIZE; // Used to store the size of AST

/*
*******************************************************************************************
*	createAST:
*		parseTree:- Parse tree node whose Abstract syntax tree needs to be created
*		parent:- ASTNode whose child would be the root of the AST created by this function
*	Usage:- Does pre-order traversal of the tree rooted at ast
*******************************************************************************************
*/
ASTNode* createAST(ParseNode* parseTree,ASTNode* parent);
#endif
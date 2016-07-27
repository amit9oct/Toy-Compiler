/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include "ast.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

char errorToken2[][30]=
{
    "TK_LE",
    "TK_ASSIGNOP",
    "TK_LT",
    "TK_EQ",
    "TK_GE",
    "TK_GT",
    "TK_NE",
    "TK_OR",
    "TK_AND",
    "TK_NOT",
    "TK_COMMENT",
    "TK_SQR",
    "TK_SQL",
    "TK_COLON",
    "TK_SEM",
    "TK_DOT",
    "TK_CL",
    "TK_OP",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_RNUM",
    "TK_NUM",
    "TK_MAIN",
    "TK_FUNID",
    "TK_ID",
    "TK_FIELDID",
    "TK_RECORDID",
    "TK_COMMA",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_INT",
    "TK_REAL",
    "TK_TYPE",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_ENDWHILE",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_EPS",
    "TK_DOLLAR",
    "RULE_END",
    "program",
    "otherFunctions",
    "mainFunction",
    "stmts",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "primitiveDatatype",
    "constructedDatatype",
    "remaining_list",
    "typeDefinitions",
    "typeDefinition",
    "fieldDefinition",
    "fieldDefinitions",
    "moreFields",
    "global_or_not",
    "declarations",
    "declaration",
    "otherStmts",
    "stmt",
    "assignmentStmt",
    "singleOrRecId",
    "new_24",
    "funCallStmt",
    "outputParameters",
    "inputParameters",
    "iterativeStmtT",
    "conditionalStmt",
    "elsePart",
    "ioStmt",
    "allVar",
    "allVarPrime",
    "arithmeticExpression",
    "expPrime",
    "termPrime",
    "all",
    "temp",
    "lowPrecedenceOperator",
    "term",
    "highPrecedenceOperator",
    "factor",
    "operator",
    "booleanExpression",
    "var",
    "logicalOp",
    "relationalOp",
    "returnStmt",
    "optionalReturn",
    "idList",
    "more_ids"
};

int PRINT_AST = 0; //Used to set flag whether to print AST or not on stdout
int AST_SIZE = 0; //Used to print the total size (in Bytes) of AST.

/*
*************************************************************************************
*	preOrder:
*		ast:- AST tree node whose pre-order traversal is to be done
*		fp:-  File pointer in which the tree information is printed
*	Usage:- Does pre-order traversal of the tree rooted at ast
*************************************************************************************
*/
void preOrder(ASTNode* ast,FILE* fp){
	ASTNode* tmp=NULL ;
	if(ast!=NULL)
		tmp= ast->child;
	while(tmp!=NULL){
		preOrder(tmp,fp);
		if(tmp!=NULL)
			tmp = tmp->next;
	}
	if(ast!=NULL)
	fprintf(fp,"%.30s %.30s %.30s\n",errorToken2[ast->token->tokenName],(ast->parent!=NULL)?errorToken2[ast->parent->token->tokenName]:"ROOT",(ast->child!=NULL)?errorToken2[ast->child->token->tokenName]:"NULL");
fflush(stdout);
}


/*
*************************************************************************************
*	inOrder:
*		ast:- AST tree node whose in-order traversal is to be done
*		fp:-  File pointer in which the tree information is printed
*	Usage:- Does in-order traversal of the tree rooted at ast
*************************************************************************************
*/
void inOrder(ASTNode* ast,FILE* fp){
	//printf("OK\n");
	ASTNode* tmp=NULL ;
	if(ast!=NULL)
		tmp= ast->child;
	if(ast!=NULL)
		fprintf(fp,"%-30s %-30s %-30s %-30s %-30s\n",
												errorToken2[ast->token->tokenName],ast->token->lexemeName,
												(ast->parent!=NULL)?errorToken2[ast->parent->token->tokenName]:"ROOT",
												(ast->child!=NULL)?errorToken2[ast->child->token->tokenName]:"NULL",
												(ast->next!=NULL)?errorToken2[ast->next->token->tokenName]:"NULL");
	while(tmp!=NULL){
		inOrder(tmp,fp);
		if(tmp!=NULL)
			tmp = tmp->next;
	}
}


/*
*************************************************************************************
*	createASTNode:
*		parent:- AST tree node whose child node has to be created
*		token:-  Token information of the child node to be created
*	Usage:- Creates a child node of the parent node. The child's token information is
			set using the token pointer
*************************************************************************************
*/
inline ASTNode* createASTNode(Token* token,ASTNode* parent){
	AST_SIZE = AST_SIZE + sizeof(ASTNode);
	ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
	newNode->token = token;
	newNode->parent = parent;
	newNode->child = NULL;
	newNode->next = NULL;
	return newNode;
}



/*
*************************************************************************************
*	getChild:
*		parseNode:- Node of the parse tree whose corresponding child is to be reported
*		nonTerminal:- Type of the terminal or non-terminal which has to be searched
*					  in the children of the parseNode-
*	Usage:- Returns a child of the parseNode whose type matches with the type of the
*			nonTerminal
*************************************************************************************
*/
inline ParseNode* getChild(ParseNode* parseNode,int nonTerminal){
	parseNode = parseNode->child;
	//printf("OK\n");
	while((parseNode->token)->tokenName!=nonTerminal){
		parseNode=parseNode->next;
	}
	//printf("OK\n");
	if((parseNode->token)->tokenName==nonTerminal)
		return parseNode;
	else
		return NULL;
}



inline ASTNode* getLast(ASTNode* curNode){
	while(curNode->next!=NULL){
		curNode = curNode->next;
	}
	return curNode;
}



/*
*******************************************************************************************
*	createAST:
*		parseTree:- Parse tree node whose Abstract syntax tree needs to be created
*		parent:- ASTNode whose child would be the root of the AST created by this function
*	Usage:- Does pre-order traversal of the tree rooted at ast
*******************************************************************************************
*/
ASTNode* createAST(ParseNode* parseTree,ASTNode* parent){
	int ruleNum = parseTree->ruleNum;
	ASTNode *root,*temp,*temp2;
	switch(ruleNum){
		case 0: root = createASTNode(parseTree->token,NULL);
				root->child = createAST(getChild(parseTree,NT_OTHER_FUNCTIONS),root);
				if(root->child == NULL){
					root->child = createAST(getChild(parseTree,NT_MAIN_FUNCTION),root);
				}
				else{
					ASTNode* lastNode = getLast(root->child);
					lastNode->next = createAST(getChild(parseTree,NT_MAIN_FUNCTION),root);
				}
				break;
		case 1: root = createASTNode(getChild(parseTree,TK_MAIN)->token,parent);
				root->child = createAST(getChild(parseTree,NT_STMTS),root);
				break;
		case 2: root = createAST(getChild(parseTree,NT_FUNCTION),parent);
				root->next = createAST(getChild(parseTree,NT_OTHER_FUNCTIONS),parent);
				break;
		case 3: root = NULL;
				break;
		case 4: root = createASTNode(getChild(parseTree,TK_FUNID)->token,parent);
				root->child = createAST(getChild(parseTree,NT_INPUT_PAR),root);
				root->child->next = createAST(getChild(parseTree,NT_OUTPUT_PAR),root);
				root->child->next->next = createAST(getChild(parseTree,NT_STMTS),root);
				break;
		case 5: root = createASTNode(parseTree->token,parent);
				root->child = createAST(getChild(parseTree,NT_PARAMETER_LIST),root);
				break;
		case 6: root = createASTNode(parseTree->token,parent);
				root->child = createAST(getChild(parseTree,NT_PARAMETER_LIST),root);
				break;
		case 7: root = NULL;
				break;
		case 8: root = createAST(getChild(parseTree,NT_DATATYPE),parent);
				root->child = createASTNode(getChild(parseTree,TK_ID)->token,root);
				root->next = createAST(getChild(parseTree,NT_REMAINING_LIST),parent);
				break;
		case 9: root = createAST(getChild(parseTree,NT_PRIMITIVE_DATATYPE),parent);
				break;
		case 10: root = createAST(getChild(parseTree,NT_CONSTRUCTED_DATATYPE),parent);
				 break;
		case 11: root = createASTNode(getChild(parseTree,TK_INT)->token,parent);
				 break;
		case 12: root = createASTNode(getChild(parseTree,TK_REAL)->token,parent);
				 break;
		case 13: root = createASTNode(getChild(parseTree,TK_RECORDID)->token,parent);
				 break;
		case 14: root = createAST(getChild(parseTree,NT_PARAMETER_LIST),parent);
				 break;
		case 15: root = NULL;
				 break;
		case 16: root = createAST(getChild(parseTree,NT_TYPE_DEFINITIONS),parent);
				 if(root==NULL){
				 	root = createASTNode(getChild(parseTree,NT_TYPE_DEFINITIONS)->token,parent);
				  }
			 	 temp = root;
				 root = getLast(root);
				 root->next = createAST(getChild(parseTree,NT_DECLARATIONS),parent);
				 if(root->next==NULL){
				 	root->next = createASTNode(getChild(parseTree,NT_DECLARATIONS)->token,parent);
				  }
				  root = getLast(root); 
				 root->next = createAST(getChild(parseTree,NT_OTHER_STMTS),parent);
				 if(root->next==NULL){
				 	root->next = createASTNode(getChild(parseTree,NT_OTHER_STMTS)->token,parent);
				 }
				 root = getLast(root);
				 root->next = createAST(getChild(parseTree,NT_RETURN_STMT),parent);
				 root = temp;
				 break;
		case 17: root = createAST(getChild(parseTree,NT_TYPE_DEFINITION),parent);
				 root->next = createAST(getChild(parseTree,NT_TYPE_DEFINITIONS),parent);
				 break;
		case 18: root = NULL;
				 break;
		case 19: root = createASTNode(getChild(parseTree,TK_RECORDID)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_FIELD_DEFINITIONS),root);
				 break;
		case 20: root = createAST(getChild(parseTree,NT_FIELD_DEFINITION),parent);
				 root->next = createAST(getChild(parseTree,NT_FIELD_DEFINITION)->next,parent);
				 root->next->next = createAST(getChild(parseTree,NT_MORE_FIELDS),parent);
				 break;
		case 21: root = createAST(getChild(parseTree,NT_PRIMITIVE_DATATYPE),parent);
				 root->child = createASTNode(getChild(parseTree,TK_FIELDID)->token,root);
				 break;
		case 22: root = createAST(getChild(parseTree,NT_FIELD_DEFINITION),parent);
				 root->next = createAST(getChild(parseTree,NT_MORE_FIELDS),parent);
				 break;
		case 23: root = NULL;
				 break;
		case 24: root = createAST(getChild(parseTree,NT_DECLARATION),parent);
				 root->next = createAST(getChild(parseTree,NT_DECLARATIONS),parent);
				 break;
		case 25: root = NULL;
				 break;
		case 26: root = createAST(getChild(parseTree,NT_DATATYPE),parent);
				 root->child = createASTNode(getChild(parseTree, TK_ID)->token,root);
				 root->child->next = createAST(getChild(parseTree,NT_GLOBAL_OR_NOT),root);
				 break;
		case 27: root = createASTNode(getChild(parseTree,TK_GLOBAL)->token,parent);
				 break;
		case 28: root = NULL;
				 break;
		case 29: root = createAST(getChild(parseTree,NT_STMT),parent);
				 root->next = createAST(getChild(parseTree,NT_OTHER_STMTS),parent);
				 break;
		case 30: root = NULL;
				 break;
		case 31: root = createAST(getChild(parseTree,NT_ASSIGNMENT_STMT),parent);
				 break;
		case 32: root = createAST(getChild(parseTree,NT_ITERATIVE_STMT),parent);
				 break;
		case 33: root = createAST(getChild(parseTree,NT_CONDITIONAL_STMT),parent);
				 break;
		case 34: root = createAST(getChild(parseTree,NT_IO_STMT),parent);
				 break;
		case 35: root = createAST(getChild(parseTree,NT_FUN_CALL_STMT),parent);
				 break;
		case 36: root = createASTNode(getChild(parseTree,TK_ASSIGNOP)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_SINGLE_OR_RECORDID),root);
				 root->child->next = createAST(getChild(parseTree,NT_ARITHMETIC_EXPRESSION),root);
				 break;
		case 37: root = createASTNode(getChild(parseTree,TK_ID)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_NEW_24),root);
				 break;
		case 38: root = NULL;
				 break;
		case 39: root = createASTNode(getChild(parseTree,TK_FIELDID)->token,parent);
				 break;
		case 40: root = createASTNode(getChild(parseTree,TK_CALL)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_OUTPUT_PARAMETERS),root);
				 root->child->next = createASTNode(getChild(parseTree,TK_FUNID)->token,root);
				 root->child->next->next = createAST(getChild(parseTree,NT_INPUT_PARAMETERS),root);
				 break;
		case 41: root = createASTNode(parseTree->token,parent);
				 root->child = createAST(getChild(parseTree,NT_ID_LIST),root);
				 break;
		case 42: root = createASTNode(parseTree->token,parent);
				 root->child = NULL;
				 break;
		case 43: root = createASTNode(parseTree->token,parent);
				 root->child = createAST(getChild(parseTree,NT_ID_LIST),root);
				 break;
		case 44: root = createASTNode(getChild(parseTree,TK_WHILE)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_BOOLEAN_EXPRESSION),root);
				 root->child->next = createAST(getChild(parseTree,NT_STMT),root);
				 root->child->next->next = createAST(getChild(parseTree,NT_OTHER_STMTS),root);
				 break;
		case 45: root = createASTNode(getChild(parseTree,TK_IF)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_BOOLEAN_EXPRESSION),root);
				 root->child->next = createAST(getChild(parseTree,NT_STMT),root);
				 root->child->next->next = createAST(getChild(parseTree,NT_OTHER_STMTS),root);
				 temp = root->child->next;
				 while(temp->next!=NULL){
				 	temp = temp->next;
				 }
				 temp->next = createAST(getChild(parseTree,NT_ELSE_PART),root);
				 break;

		case 46: root = createASTNode(getChild(parseTree,TK_ELSE)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_STMT),root);
				 root->child->next = createAST(getChild(parseTree,NT_OTHER_STMTS),root);
				 break;
		case 47: root = NULL;
				 break;
		case 48: root = createASTNode(getChild(parseTree,TK_READ)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_SINGLE_OR_RECORDID),root);
				 break;
		case 49: root = createASTNode(getChild(parseTree,TK_WRITE)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_ALL_VAR),root);
				 break;
		case 50: root = createASTNode(getChild(parseTree,TK_NUM)->token,parent);
				 break;
		case 51: root = createASTNode(getChild(parseTree,TK_RNUM)->token,parent);
				 break;
		case 52: root = createASTNode(getChild(parseTree,TK_ID)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_ALL_VAR_PRIME),root);
				 break;
		case 53: root = createASTNode(getChild(parseTree,TK_FIELDID)->token,parent);
				 break;
		case 54: root = NULL;
				 break;
		case 55: root = createAST(getChild(parseTree,NT_TERM),parent);
				 temp = createAST(getChild(parseTree,NT_EXP_PRIME),NULL); 
				 if(temp!=NULL){
				 	temp2 = temp->child;
				 	temp->child = root;
				 	temp->child->parent = temp;
				 	temp->child->next = temp2;
				 	temp->child->next->parent = temp;
					while(temp->parent!=NULL){
				 		temp = temp->parent;
				 	}
				 	root = temp;
				 }
				 break;
		case 56: root = createAST(getChild(parseTree,NT_LOW_PRECEDENCE_OPERATOR),parent);
				 root->child = createAST(getChild(parseTree,NT_TERM),root);
				 temp = createAST(getChild(parseTree,NT_EXP_PRIME),parent);
				 if(temp!=NULL){
				 	temp2 = temp->child;
				 	root->parent = temp;
				 	temp->child = root;
				 	temp2->parent = temp;
				 	temp->child->next = temp2;
				 	root = temp->child;
					return root;
				 }
				 break;
		case 57: root = NULL;
				 break;
		case 58: 
				 root = createAST(getChild(parseTree,NT_FACTOR),parent);
				 temp = createAST(getChild(parseTree,NT_TERM_PRIME),NULL);
				 if(temp!=NULL){
				 	temp2 = temp->child;
				 	temp->child = root;
				 	temp->child->parent = temp;
				 	temp->child->next = temp2;
				 	temp->child->next->parent = temp;
					while(temp->parent!=NULL){
				 		temp = temp->parent;
				 	}
				 	root = temp;
				 }
				 break;
		case 59: root = createAST(getChild(parseTree,NT_HIGH_PRECEDENCE_OPERATORS),parent);
				 root->child = createAST(getChild(parseTree,NT_FACTOR),root);
				 temp = createAST(getChild(parseTree,NT_TERM_PRIME),parent);
				 if(temp!=NULL){
				 	temp2 = temp->child;
				 	root->parent = temp;
				 	temp->child = root;
				 	temp2->parent = temp;
				 	temp->child->next = temp2;
				 	root = temp->child;
					return root;
				 }
				 break;
		case 60: root = NULL;
				 break;
		case 61: root = createAST(getChild(parseTree,NT_ARITHMETIC_EXPRESSION),parent);
		         break;
		case 62: root =  createAST(getChild(parseTree,NT_ALL),parent);
				 break;
	 	case 63: root = createASTNode(getChild(parseTree,TK_MUL)->token,parent);
	 			 break;
	 	case 64: root = createASTNode(getChild(parseTree,TK_DIV)->token,parent);
	 			 break;
	 	case 65: root = createASTNode(getChild(parseTree,TK_PLUS)->token,parent);
	 			 break;
	 	case 66: root = createASTNode(getChild(parseTree,TK_MINUS)->token,parent);
	 			 break;
	 	case 67: root = createASTNode(getChild(parseTree,TK_NUM)->token,parent);
				 break;
		case 68: root = createASTNode(getChild(parseTree,TK_RNUM)->token,parent);
				 break;
		case 69: root = createASTNode(getChild(parseTree,TK_ID)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_TEMP),root);
				 break;
		case 70: root = createASTNode(getChild(parseTree,TK_FIELDID)->token,parent);
				 break;
		case 71: root = NULL;
				 break;
		case 72: root = createAST(getChild(parseTree,NT_LOGICAL_OP),parent);
				 root->child = createAST(getChild(parseTree,NT_BOOLEAN_EXPRESSION),root);
				 root->child->next = createAST(getChild(parseTree,NT_BOOLEAN_EXPRESSION)->next->next->next->next,root);
				 break;
		case 73: root = createAST(getChild(parseTree,NT_RELATIONAL_OP),parent);
				 root->child = createAST(getChild(parseTree,NT_VAR),root);
				 root->child->next = createAST(getChild(parseTree,NT_VAR)->next->next,root);
				 break;
		case 74: root = createASTNode(getChild(parseTree,TK_NOT)->token,parent);
				 root->child = createAST(getChild(parseTree,NT_BOOLEAN_EXPRESSION),root);
				 break;
		case 75: root = createASTNode(getChild(parseTree,TK_ID)->token,parent);
				 break;
		case 76: root = createASTNode(getChild(parseTree,TK_NUM)->token,parent);
				 break;
		case 77: root = createASTNode(getChild(parseTree,TK_RNUM)->token,parent);
				 break;
		case 78: root = createASTNode(getChild(parseTree,TK_LT)->token,parent);
				 break;
		case 79: root = createASTNode(getChild(parseTree,TK_GT)->token,parent);
				 break;
		case 80: root = createASTNode(getChild(parseTree,TK_LE)->token,parent);
				 break;
		case 81: root = createASTNode(getChild(parseTree,TK_NE)->token,parent);
				 break;
		case 82: root = createASTNode(getChild(parseTree,TK_EQ)->token,parent);
				 break;
		case 83: root = createASTNode(getChild(parseTree,TK_GE)->token,parent);
				 break;
		case 84: root = createASTNode(getChild(parseTree,TK_AND)->token,parent);
				 break;
		case 85: root = createASTNode(getChild(parseTree,TK_OR)->token,parent);
				 break;
		case 86: root = createASTNode(getChild(parseTree,TK_RETURN)->token,parent);
			     root->child = createAST(getChild(parseTree,NT_OPTIONAL_RETURN),root);
				 break;
		case 87: root = createAST(getChild(parseTree,NT_ID_LIST),parent);
				 break;
		case 88: root = NULL;
				 break;
		case 89: root = createASTNode(getChild(parseTree,TK_ID)->token,parent);
				 root->next =  createAST(getChild(parseTree,NT_MORE_IDS),parent);
				 break;
		case 90: root = createAST(getChild(parseTree,NT_ID_LIST),parent);
				 break;
		case 91: root = NULL;
				 break;
	}
	if(root!=NULL)
		root->parent = parent;
	if(ruleNum==0){
		FILE* fp = stdout;
		FILE* fpIn = stdout;
		if(PRINT_AST){
			fprintf(fp,"%-30s %-30s %-30s %-30s %-30s\n","Node","LexemeName","Parent","Child","Next");
			fprintf(fp,"%-30s %-30s %-30s %-30s %-30s\n","----","----------","------","-----","----");
			inOrder(root,fpIn);
	    }
	}
	return root;
}


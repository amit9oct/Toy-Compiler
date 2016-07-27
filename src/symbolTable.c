/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include "symbolTable.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SymbolTable SYMBOL_TABLE;
SymbolTable RECORD_TABLE;
SymbolTable FUNCTION_TABLE;

int GLOBAL_WIDTH = 0; //Size of the global variables used in the code
int FUNCTION_TIME_STAMP=0; //Used for storing the order of function in the user code


/*
*******************************************************************************************
*	djb2:
*		str:- String whose hash value needs to be calculated
*		size:- size of the hashtable
*	Usage:- Returns hash value for the value  node.Uses famous djb2 algorithm for hashing. 
*******************************************************************************************
*/
int djb2(unsigned char *str, int size){//DjB2 algorithm for hashing strings
	unsigned long seed = 5381;
	int ch;
	while(ch = *str){
		if(*str=='\0')
			break;
		seed = seed*33 + ch; 
		seed %= size;
		str++;
	}
	return seed;

}


/*
*******************************************************************************************
*	symbolHash:
*		value:- Node of the SYMBOTABLE whose hash value needs to be computed
*		size:- size of the SYMBOLTABLE
*	Usage:- Returns hash value for the value(entry) node in SYMBOLTABLE
*******************************************************************************************
*/
int symbolHash(void* value, int size){//value is of type VariableNode
	VariableNode* temp = (VariableNode*)value;
	char string[2*MAX_RECORD_LEN];
	strcpy(string,temp->varTok->lexemeName);
	if(!temp->isGlobal)
		strcat(string,temp->funTok->lexemeName);
	int hash = djb2(string, size);
	return hash;
}


/*
*******************************************************************************************
*	recordHash:
*		value:- Node of the RECORDTABLE whose hash value needs to be computed
*		size:- size of the RECORDTABLE
*	Usage:- Returns hash value for the value(entry) node in RECORDTABLE
*******************************************************************************************
*/
int recordHash(void* value, int size){//value is of type VariableNode
	RecordNode* temp = (RecordNode*)value;
	int hash = djb2(temp->recTok->lexemeName, size);
	return hash;
}


/*
*******************************************************************************************
*	functionHash:
*		value:- Node of the FUNCTION_TABLE whose hash value needs to be computed
*		size:- size of the FUNCTION_TABLE
*	Usage:- Returns hash value for the value(entry) node in FUNCTION_TABLE
*******************************************************************************************
*/
int functionHash(void* value, int size){//value is of type VariableNode
	FunctionNode* temp = (FunctionNode*)value;
	int hash = djb2(temp->funTok->lexemeName, size);
	return hash;
}



/*
*******************************************************************************************
*	compareSymbol:
*		value1:- Node of the SYMBOL_TABLE
*		value2:- Node of the SYMBOL_TABLE
*	Usage:- returns compare value of value1 and value2
*******************************************************************************************
*/
int compareSymbol(void* value1, void* value2){
	VariableNode* temp1 = (VariableNode*)value1;
	VariableNode* temp2 = (VariableNode*)value2;
	if(temp1->isGlobal && temp2->isGlobal){
		if(strcmp(temp1->varTok->lexemeName, temp2->varTok->lexemeName)==0){
			return 1;
		}
	}
	else if(temp1->funTok == temp2->funTok){
		if(strcmp(temp1->varTok->lexemeName, temp2->varTok->lexemeName)==0){
			return 1;
		}
	}
	return 0;
}


/*
*******************************************************************************************
*	compareRecord:
*		value1:- Node of the RECORD_TABLE
*		value2:- Node of the RECORD_TABLE
*	Usage:- returns compare value of value1 and value2
*******************************************************************************************
*/
int compareRecord(void* value1, void* value2){
	RecordNode* temp1 = (RecordNode*)value1;
	RecordNode* temp2 = (RecordNode*)value2;
	if(strcmp(temp1->recTok->lexemeName, temp2->recTok->lexemeName)==0){
		return 1;
	}
	return 0;
}


/*
*******************************************************************************************
*	compareFunction:
*		value1:- Node of the FUNCTION_TABLE
*		value2:- Node of the FUNCTION_TABLE
*	Usage:- returns compare value of value1 and value2
*******************************************************************************************
*/
int compareFunction(void* value1, void* value2){
	FunctionNode* temp1 = (FunctionNode*)value1;
	FunctionNode* temp2 = (FunctionNode*)value2;
	if(strcmp(temp1->funTok->lexemeName, temp2->funTok->lexemeName)==0){
		return 1;
	}
	return 0;
}


/*
*******************************************************************************************
*	createSymbolTable:
*	Usage:- Initializes the SYMBOL_TABLE,RECORD_TABLE and FUNCTION_TABLE 
*******************************************************************************************
*/
void createSymbolTable(){
	SYMBOL_TABLE.hashTable = createHashTable(HASH_TABLE_SIZE,HASH_ALPHA,symbolHash,compareSymbol);
	RECORD_TABLE.hashTable = createHashTable(HASH_TABLE_SIZE,HASH_ALPHA,recordHash,compareRecord);
	FUNCTION_TABLE.hashTable = createHashTable(HASH_TABLE_SIZE,HASH_ALPHA,functionHash,compareFunction);
}


/*
*******************************************************************************************
*	createFieldList:
*		fieldType:- ASTNode whose FieldList node needs to be created
*		offset:- Offset of the FieldList in the RecordNode
*	Usage:- returns FieldList node for fieldType
*******************************************************************************************
*/
inline FieldList* createFieldList(ASTNode* fieldType, int* offset){
	FieldList* fields = (FieldList*)malloc(sizeof(FieldList));
	if(fieldType->token->tokenName==TK_REAL){
		fields->type = REAL;
		fields->width = REAL_WIDTH;
		fields->offset = *offset;
		(*offset) += REAL_WIDTH;
		fields->token = fieldType->child->token;
	}
	else{
		fields->type = INT;
		fields->width = INT_WIDTH;
		fields->offset = *offset;
		(*offset) += INT_WIDTH;
		fields->token = fieldType->child->token;
	}
	return fields;
}

/*
*******************************************************************************************
*	createRecNode:
*		fields:- Head of the FieldList type linked list to be stored in recordNode
*		token:- Token information of the record type
*	Usage:- returns RecordNode with fields and token information
*******************************************************************************************
*/
inline RecordNode* createRecNode(FieldList* fields,Token* token){
	RecordNode* rec = (RecordNode*)malloc(sizeof(RecordNode));
	rec->recTok = token;
	rec->fields = fields;
	FieldList* head = fields;
	rec->width = 0;
	while(head!=NULL){
		rec->width += head->width;
		head = head->next;
	}
	return rec;
}


/*
*******************************************************************************************
*	createVarNode:
*		varTok:- Token information of the record type
*		type:- type of the variableNode to be created
*		funTok:- Token information of the function type
*		typeToken:- Token information of the record node
*		isGlobal:- indicates whether variableNode is global or not
*		offset:- offset of the variable node in the function stack
*	Usage:- returns VariableNode with above parameters
*******************************************************************************************
*/
inline VariableNode* createVarNode(Token* varTok,Token* funTok,Token* typeToken,Type type,int isGlobal,int* offset){
	VariableNode* var = (VariableNode*)malloc(sizeof(VariableNode));
	var->varTok = varTok;
	var->funTok = funTok;
	var->isAssigned = 0;
	RecordNode* tempRecordNode = createRecNode(NULL,typeToken);
	if(tempRecordNode->recTok!=NULL){
		var->recordPtr = (RecordNode*)get(RECORD_TABLE.hashTable,tempRecordNode);
		if(var->recordPtr == NULL && type==RECORD){
			char errArguments[1][MAX_ERR_LEN];
			strcpy(errArguments[0],tempRecordNode->recTok->lexemeName);
			printSemanticError(-8,typeToken->lineNum,errArguments);
		}
	}
	else
		var->recordPtr = NULL;
	free(tempRecordNode);
	var->type = type;
	if(var->type!=RECORD){
		var->offset =  *offset;
		if(var->type==INT){
			(*offset) += INT_WIDTH;
			var->width = INT_WIDTH;
		}
		else{
			(*offset) += REAL_WIDTH;
			var->width = REAL_WIDTH;
		}
	}
	else{
		var->offset = *offset;
		if(var->recordPtr!=NULL){
			(*offset) += var->recordPtr->width;
			var->width = var->recordPtr->width;
		}
	} 
	var->isGlobal = isGlobal;
	return var;
}


/*
*******************************************************************************************
*	createInputPar:
*		token:- Token information of the record type
*	Usage:- returns InputPar list node with above token information for function node 
*******************************************************************************************
*/
InputPar* createInputPar(Token* token){
	InputPar* newNode = (InputPar*) malloc(sizeof(InputPar));
	newNode->token = token;
	newNode->next = NULL;
	return newNode;
}


/*
*******************************************************************************************
*	createOutputPar:
*		token:- Token information of the record type
*	Usage:- returns OutputPar list node with above token information for function node 
*******************************************************************************************
*/
OutputPar* createOutputPar(Token* token){
	OutputPar* newNode = (OutputPar*) malloc(sizeof(OutputPar));
	newNode->token = token;
	newNode->next = NULL;
	return newNode;
}


/*
*******************************************************************************************
*	createFunctionNode:
*		token:- Token information of the function node
*	Usage:- returns FunctionNode with this token information
*******************************************************************************************
*/
FunctionNode* createFunctionNode(Token* token){
	FunctionNode* newNode = (FunctionNode*)malloc(sizeof(FunctionNode));
	newNode->funTok = token;
	newNode->width = 0;
	return newNode;
}

int isSymbol(TokenName tokenName){
	if(tokenName==TK_RNUM||tokenName==TK_NUM||tokenName==TK_ID)
		return 0;
	else
		return 1;
}



/*
*******************************************************************************************
*	populateRecordTable:
*		root:- Root of the AST
*	Usage:- Populates the RECORD_TABLE by traversing the AST
*******************************************************************************************
*/
void populateRecordTable(ASTNode* root){
	root = root->child;
	FieldList* fields = NULL, *temp=NULL;
	ASTNode* fieldType = NULL;
	while(root!=NULL){
		ASTNode* childFun = root->child;
		while(childFun!=NULL){
			int offset = 0;
			if(childFun->token->tokenName==TK_RECORDID && childFun->child->token->tokenName!=TK_ID){
				fieldType = childFun->child;
				fields = createFieldList(childFun->child, &offset);
				fieldType = fieldType->next;
				temp = fields;
				while(fieldType!=NULL){
					temp = temp->next = createFieldList(fieldType, &offset);
					fieldType = fieldType->next;
				}
				temp->next = NULL;
				RecordNode* rec = createRecNode(fields, childFun->token);
				RecordNode* temp = NULL;
				if((temp=get(RECORD_TABLE.hashTable, rec))!=NULL){
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],rec->recTok->lexemeName);
					sprintf(errArguments[1],"%d",childFun->token->lineNum);
					printSemanticError(-9,temp->recTok->lineNum,errArguments);
				}
				else
					put(&(RECORD_TABLE.hashTable), rec);
			}
			childFun = childFun->next;
		}
		root = root->next;
	}
}


/*
*******************************************************************************************
*	populateSymbolTable:
*		root:- Root of the AST
*	Usage:- Populates the SYMBOLTABLE by traversing the AST
*******************************************************************************************
*/
void populateSymbolTable(ASTNode* root){//To be invoked only after function tables are populated
	root = root->child;
	int globalOffset = 0;
	while(root!=NULL){
		ASTNode* childFun = root->child;
		FunctionNode* funNode = (FunctionNode*)malloc(sizeof(FunctionNode));
		funNode->funTok = root->token;
		funNode = ((FunctionNode*)get(FUNCTION_TABLE.hashTable,funNode));
		int localOffset = funNode->width;
		while(childFun!=NULL){
			int isGlobal = 0;
			if(childFun->token->tokenName==TK_RECORDID && childFun->child->token->tokenName==TK_ID){
				if(childFun->child->next!=NULL){
					isGlobal = 1;
				}
				VariableNode* var = createVarNode(childFun->child->token,
												  childFun->parent->token,
												  childFun->token,
												  RECORD,
												  isGlobal,
												  (isGlobal)? &globalOffset:&localOffset);
				VariableNode* freeVar = var;
				if((var=get((SYMBOL_TABLE.hashTable), var))!=NULL){
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],var->varTok->lexemeName);
					sprintf(errArguments[1],"%d",var->varTok->lineNum);
					printSemanticError(-10,childFun->token->lineNum,errArguments);
					free(freeVar);
				}
				else if(var==NULL){
					var = freeVar;
					var->isGlobal = !(var->isGlobal);
					if((var=get((SYMBOL_TABLE.hashTable), var))!=NULL){
						char errArguments[2][MAX_ERR_LEN];
						strcpy(errArguments[0],var->varTok->lexemeName);
						sprintf(errArguments[1],"%d",var->varTok->lineNum);
						printSemanticError(-11,childFun->token->lineNum,errArguments);
						free(freeVar);
					}	
					else {
						freeVar->isGlobal = !(freeVar->isGlobal);
						put(&(SYMBOL_TABLE.hashTable), freeVar);
					}

				}

			}
			else if(childFun->token->tokenName==TK_REAL || childFun->token->tokenName==TK_INT){
				if(childFun->child->next!=NULL){
					isGlobal = 1;
				}
				VariableNode* var = createVarNode(childFun->child->token,
												  childFun->parent->token,
												  childFun->token,
												  (childFun->token->tokenName==TK_REAL)? REAL:INT,
												  isGlobal,
												  (isGlobal)? &globalOffset:&localOffset);
				VariableNode* freeVar = var;
				if((var=get((SYMBOL_TABLE.hashTable), var))!=NULL){
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],var->varTok->lexemeName);
					sprintf(errArguments[1],"%d",var->varTok->lineNum);
					printSemanticError(-10,childFun->token->lineNum,errArguments);
					free(freeVar);
				}
				else if(var==NULL){

					var = freeVar;
					var->isGlobal = !(var->isGlobal);
					if((var=get((SYMBOL_TABLE.hashTable), var))!=NULL){
						char errArguments[2][MAX_ERR_LEN];
						strcpy(errArguments[0],var->varTok->lexemeName);
						sprintf(errArguments[1],"%d",var->varTok->lineNum);
						printSemanticError(-11,childFun->token->lineNum,errArguments);
						free(freeVar);
					}	
					else{ 
						freeVar->isGlobal = !(freeVar->isGlobal);
						put(&(SYMBOL_TABLE.hashTable), freeVar);
					}
				}

			}
			childFun = childFun->next;
		}
		funNode->width = localOffset;
		root = root->next;
	}
	GLOBAL_WIDTH = globalOffset;
}


/*
********************************************************************************************************
*	populateFunctionTable:
*		root: It is of ASTNode* type. It contains the root of the program.
*	This is used to add functions along with their input and output paramters to the FUNCTION_TABLE.
*********************************************************************************************************
*/
void populateFunctionTable(ASTNode* root){
	root = root->child;
	while(root!=NULL){
		ASTNode* childFun = root->child,*parFun;
		FunctionNode* funInfo = (FunctionNode*)malloc(sizeof(FunctionNode));
		funInfo->timeStamp = FUNCTION_TIME_STAMP;
		FUNCTION_TIME_STAMP++;
		funInfo->funTok = root->token;
		InputPar* currInput = NULL;
		OutputPar* currOutput = NULL;
		int localOffset = 0;
		int width=0;
		while(childFun!=NULL){
			if(childFun->token->tokenName==NT_INPUT_PAR){
				parFun = childFun->child;
				while(parFun!=NULL){
					VariableNode* var = createVarNode(parFun->child->token,
												  childFun->parent->token,
												  parFun->token,
												  (parFun->token->tokenName==TK_REAL)? REAL:INT,
												  0,
												  &localOffset);
					if(parFun->token->tokenName==TK_RECORDID){
						var->type = RECORD;
					}
					if(get((SYMBOL_TABLE.hashTable), var)!=NULL){
						char errArguments[2][MAX_ERR_LEN];
						strcpy(errArguments[0],var->varTok->lexemeName);
						sprintf(errArguments[1],"%d",var->varTok->lineNum);
						printSemanticError(-10,childFun->token->lineNum,errArguments);
					}
					else{
						put(&(SYMBOL_TABLE.hashTable), var);
					}
					if(currInput==NULL){
						funInfo->inputPar = createInputPar(parFun->child->token);
						currInput = funInfo->inputPar;
					}
					else{
						currInput->next = createInputPar(parFun->child->token);
						currInput = currInput->next;
					}
					width += var->width;
					parFun = parFun->next;
				}
			}
			else if(childFun->token->tokenName==NT_OUTPUT_PAR){
				parFun = childFun->child;
				while(parFun!=NULL){
					VariableNode* var = createVarNode(parFun->child->token,
												  childFun->parent->token,
												  parFun->token,
												  (parFun->token->tokenName==TK_REAL)? REAL:INT,
												  0,
												  &localOffset);
					if(parFun->token->tokenName==TK_RECORDID){
						var->type = RECORD;
					}
					if(get((SYMBOL_TABLE.hashTable), var)!=NULL){
						char errArguments[2][MAX_ERR_LEN];
						strcpy(errArguments[0],var->varTok->lexemeName);
						sprintf(errArguments[1],"%d",var->varTok->lineNum);
						printSemanticError(-10,childFun->token->lineNum,errArguments);
					}
					else{
						put(&(SYMBOL_TABLE.hashTable), var);
					}
					if(currOutput==NULL){
						funInfo->outputPar = createOutputPar(parFun->child->token);
						currOutput = funInfo->outputPar;
					}
					else{
						currOutput->next = createOutputPar(parFun->child->token);
						currOutput = currOutput->next;
					}
					width += var->width;	
					parFun = parFun->next;
				}	
			}
			childFun = childFun->next;
		}
		funInfo->width = width;
		FunctionNode* temp = NULL;
		if((temp=get(FUNCTION_TABLE.hashTable, funInfo))!=NULL){
			char errArguments[2][MAX_ERR_LEN];
			strcpy(errArguments[0],funInfo->funTok->lexemeName);
			sprintf(errArguments[1],"%d",temp->funTok->lineNum);
			printSemanticError(-14,root->token->lineNum,errArguments);
		}
		else{
			funInfo->tempCount = 0;
			put(&(FUNCTION_TABLE.hashTable), funInfo);
		}
		root = root->next;
	}
}

/*
********************************************************************************************************
*	checkFunctionParameters:
*		outputParameters: It is of ASTNode* type. It contains the root of the call statement node in AST 
						  for which the output and input parameter has to be type checked.
*		funTok   : It is of Token* type. It contains the Token for function which has that function call.
*	This is used to check parameter type and count mismatches in call statement.
*********************************************************************************************************
*/
int checkFunctionParameter(ASTNode* outputParameters,Token* funTok){
	if(outputParameters==NULL)
		return 1;
	ASTNode* funid = outputParameters->next;
	ASTNode* inputParameters = funid->next->child;
	FunctionNode* callee = createFunctionNode(funTok);
	FunctionNode* freeCalle = callee;
	outputParameters = outputParameters->child;
	int localOffset = 0;
	FunctionNode* fun = createFunctionNode(funid->token);
	FunctionNode* freeFun = fun;
	fun = (FunctionNode*)get(FUNCTION_TABLE.hashTable,fun);
	free(freeFun);
	if(fun==NULL){
		char errArguments[1][MAX_ERR_LEN];
		strcpy(errArguments[0],funid->token->lexemeName);
		printSemanticError(-15,funid->token->lineNum,errArguments);
		free(freeCalle);
		return 0;
	}
	else if(funTok==fun->funTok){
		char errArguments[1][MAX_ERR_LEN];
		strcpy(errArguments[0],fun->funTok->lexemeName);
		printSemanticError(-16,funid->token->lineNum,errArguments);
		free(freeCalle);
		return 0;
	}
	else{
		callee = (FunctionNode*)get(FUNCTION_TABLE.hashTable,callee);
		free(freeCalle);
		if(callee->timeStamp<fun->timeStamp){
			char errArguments[1][MAX_ERR_LEN];
			strcpy(errArguments[0],fun->funTok->lexemeName);
			strcpy(errArguments[1],callee->funTok->lexemeName);
			printSemanticError(-17,funid->token->lineNum,errArguments);
			return 0;
		}
	}
	OutputPar* outputPar = fun->outputPar;
	while(outputParameters!=NULL || outputPar!=NULL){
		VariableNode* var = (outputPar!=NULL)? createVarNode(outputPar->token,fun->funTok,NULL,INT,0,&localOffset):NULL;
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			char errArguments[2][MAX_ERR_LEN];
			strcpy(errArguments[0],funTok->lexemeName);
			printSemanticError(-18,funid->token->lineNum,errArguments);
			break;
		}
		else{
			VariableNode* outputVar = (outputParameters!=NULL)? createVarNode(outputParameters->token,funTok,NULL,INT,0,&localOffset):NULL;
			VariableNode* freeOutputVar = outputVar;
			outputVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,outputVar);
			if(outputVar==NULL && freeOutputVar!=NULL){
				outputVar = freeOutputVar;
				outputVar->isGlobal = 1;
				outputVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,outputVar);
			}
			if(outputVar==NULL){
				if(outputParameters==NULL){
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					printSemanticError(-19,funid->token->lineNum,errArguments);
				}
				else{
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					strcpy(errArguments[1],outputParameters->token->lexemeName);
					printSemanticError(-20,funid->token->lineNum,errArguments);

				}
				break;
			}
			else{
				if(var->type==outputVar->type){
					if(var->type==2){
						if(!(var->recordPtr==outputVar->recordPtr)){
							char errArguments[2][MAX_ERR_LEN];
							strcpy(errArguments[0],funTok->lexemeName);
							strcpy(errArguments[1],outputVar->varTok->lexemeName);
							printSemanticError(-21,funid->token->lineNum,errArguments);
						}
						else
							outputVar->isAssigned = 1;
					}
					else
						outputVar->isAssigned = 1;
				}
				else{

					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					strcpy(errArguments[1],outputVar->varTok->lexemeName);
					printSemanticError(-21,funid->token->lineNum,errArguments);
				}
			}
			if(freeOutputVar!=NULL)
				free(freeOutputVar);
		}
		if(freeVar!=NULL)
			free(freeVar);
		outputPar = (outputPar!=NULL)? outputPar->next:NULL;
		outputParameters = (outputParameters!=NULL)? outputParameters->next:NULL;
	}

	InputPar* inputPar = fun->inputPar;
	while(inputParameters!=NULL || inputPar!=NULL){
		VariableNode* var = (inputPar!=NULL)? createVarNode(inputPar->token,fun->funTok,NULL,INT,0,&localOffset):NULL;
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			char errArguments[2][MAX_ERR_LEN];
			strcpy(errArguments[0],funTok->lexemeName);
			strcpy(errArguments[1],funid->token->lexemeName);
			printSemanticError(-22,funid->token->lineNum,errArguments);
			break;
		}
		else{
			VariableNode* inputVar = (inputParameters!=NULL)? createVarNode(inputParameters->token,funTok,NULL,INT,0,&localOffset):NULL;
			VariableNode* freeInputVar = inputVar;
			inputVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,inputVar);
			if(inputVar==NULL && freeInputVar!=NULL){
				inputVar = freeInputVar;
				inputVar->isGlobal = 1;
				inputVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,inputVar);
			}
			if(inputVar==NULL){
				if(inputParameters==NULL){
					char errArguments[1][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					printSemanticError(-23,funid->token->lineNum,errArguments);
				}
				else{
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					strcpy(errArguments[1],inputParameters->token->lexemeName);
					printSemanticError(-20,funid->token->lineNum,errArguments);
				}
				break;
			}
			else{
				if(var->type==inputVar->type){
					if(var->type==2){
						if(!(var->recordPtr==inputVar->recordPtr)){
							char errArguments[2][MAX_ERR_LEN];
							strcpy(errArguments[0],funTok->lexemeName);
							strcpy(errArguments[1],inputVar->varTok->lexemeName);
							printSemanticError(-24,funid->token->lineNum,errArguments);
						}
					}
				}
				else{
					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funTok->lexemeName);
					strcpy(errArguments[1],inputVar->varTok->lexemeName);
					printSemanticError(-24,funid->token->lineNum,errArguments);
				}
			}
			if(freeInputVar!=NULL)
				free(freeInputVar);
		}
		if(freeVar!=NULL)
			free(freeVar);
		inputPar = (inputPar!=NULL)? inputPar->next:NULL;
		inputParameters = (inputParameters!=NULL)? inputParameters->next:NULL;
	}
	return 0;	
}


/*
*****************************************************************************************************
*	checkArithmeticExpression:
*		child 	 : It is of ASTNode* type. It contains the root of the node in AST for which the
*			   	   arithmetic expression has to be checked.
*		funTok   : It is of Token* type. It contains the Token for function which has that arithmetic
*				   expression.
*		tempCount: It has the total amount of space which will be needed by temporary variables in
*				   order to generate code for the given arithmetic expression.
*	This is used to check type mismatches in arithmetic expression.
*****************************************************************************************************
*/
VariableNode* checkArithmeticExpression(ASTNode* child,Token* funTok,int* tempCount){
	int localOffset = 0;
	if(child->token->tokenName==TK_ID){
		ASTNode* field = NULL;
		if(child->child!=NULL){
			field = child->child;
		}
		VariableNode* var = createVarNode(child->token,funTok,NULL,INT,0,&localOffset);
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			var = freeVar;
			var->isGlobal = 1;
			var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		}
		if(var==NULL){
			free(freeVar);
			char errArguments[2][MAX_ERR_LEN];
			strcpy(errArguments[0],funTok->lexemeName);
			strcpy(errArguments[1],child->token->lexemeName);
			printSemanticError(-20,child->token->lineNum,errArguments);
			return NULL;
		}
		else{
			free(freeVar);
			if(var->type!=RECORD && field!=NULL){
				char errArguments[2][MAX_ERR_LEN];
				strcpy(errArguments[0],funTok->lexemeName);
				strcpy(errArguments[1],child->token->lexemeName);
				printSemanticError(-34,child->token->lineNum,errArguments);

			}
			else if(var->type==RECORD && field!=NULL){
				FieldList* head = var->recordPtr->fields;
				for(;head!=NULL;head=head->next){
					if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
						return createVarNode(NULL,funTok,NULL,head->type,0,&localOffset);
					}
				}
				char errArguments[3][MAX_ERR_LEN];
				strcpy(errArguments[0],funTok->lexemeName);
				strcpy(errArguments[1],field->token->lexemeName);
				strcpy(errArguments[2],var->recordPtr->recTok->lexemeName);
				printSemanticError(-25,field->token->lineNum,errArguments);
				return NULL;
			}
			return var;
		}

	}
	ASTNode* leftChild = child->child;
	ASTNode* rightChild = leftChild->next;
	Type leftType=-1,rightType=-1;
	VariableNode* leftNode = NULL;
	VariableNode* rightNode = NULL;
	leftType = (leftChild->token->tokenName==TK_NUM)? INT:-1;
	leftType = (leftChild->token->tokenName==TK_RNUM)?REAL:leftType;
	rightType = (rightChild->token->tokenName==TK_NUM)?INT:-1;
	rightType = (rightChild->token->tokenName==TK_RNUM)?REAL:rightType;
	if(leftType!=-1){
		*tempCount = *tempCount + ((leftType==INT)?INT_WIDTH:REAL_WIDTH);
	}
	if(rightType!=-1){
		*tempCount = *tempCount + ((rightType==INT)?INT_WIDTH:REAL_WIDTH);
	}
	if(	child->token->tokenName==TK_PLUS||
		child->token->tokenName==TK_MINUS||
		child->token->tokenName==TK_MUL||
		child->token->tokenName==TK_DIV||
		child->token->tokenName==TK_ASSIGNOP
	  ){
	  	if(leftType==-1)
			leftNode = checkArithmeticExpression(leftChild,funTok,tempCount);
		if(rightType==-1)
			rightNode = checkArithmeticExpression(rightChild,funTok,tempCount);
		if(leftType==-1 && leftNode!=NULL)
			leftType = leftNode->type;
		if(rightType==-1 && rightNode!=NULL)
			rightType = rightNode->type;
		if(leftType==-1||rightType==-1)
			return NULL;
	}
	if(leftType==rightType && leftType!=RECORD){
	  	if(child->token->tokenName!=TK_ASSIGNOP){
	  		*tempCount = *tempCount + ((leftType==INT)?INT_WIDTH:REAL_WIDTH);
	  	}
		return createVarNode(NULL,funTok,NULL,leftType,0,&localOffset);
	}
	else if(leftType==rightType){
		if(child->token->tokenName==TK_MUL||
			child->token->tokenName==TK_DIV
			){
			char errArguments[3][MAX_ERR_LEN];
			strcpy(errArguments[1],leftNode->varTok->lexemeName);
			strcpy(errArguments[2],rightNode->varTok->lexemeName);
			strcpy(errArguments[0],funTok->lexemeName);
			printSemanticError(-26,child->token->lineNum,errArguments);
			return NULL;
		}
		if(!(leftNode->recordPtr==rightNode->recordPtr))
			return NULL;
	  	if(child->token->tokenName!=TK_ASSIGNOP){
	  		*tempCount = *tempCount + leftNode->recordPtr->width;
	  	}
		return leftNode;
	}
	else if((leftType==RECORD && (rightType==INT||rightType==REAL))||
			(rightType==RECORD && (leftType==INT||leftType==REAL))) {
		if(child->token->tokenName==TK_DIV||child->token->tokenName==TK_MUL){
			if(leftType==RECORD){
			  	if(child->token->tokenName!=TK_ASSIGNOP){
		  			*tempCount = *tempCount + leftNode->recordPtr->width;
			  	}
				return leftNode;
			}
			else{
				if(child->token->tokenName!=TK_DIV){
				  	if(child->token->tokenName!=TK_ASSIGNOP){
	  					*tempCount = *tempCount + rightNode->recordPtr->width;
	  				}
					return rightNode;
				}

				char errArguments[1][MAX_ERR_LEN];
				strcpy(errArguments[0],funTok->lexemeName);
				printSemanticError(-27,child->token->lineNum,errArguments);
				return NULL;
			}
		}
		char errArguments[1][MAX_ERR_LEN];
		strcpy(errArguments[0],funTok->lexemeName);
		printSemanticError(-28,child->token->lineNum,errArguments);

		return NULL;
	}	
	else{
		HasErrors = 1;
		if(ShowErrors){
			printf("ERROR_30: In function %s type mismatch at line number %d between %s and %s\n",
														funTok->lexemeName,
														leftChild->token->lineNum,
														(!isSymbol(leftChild->token->tokenName))?
																leftChild->token->lexemeName: errorToken[leftChild->token->tokenName],
														(!isSymbol(rightChild->token->tokenName))?
																rightChild->token->lexemeName: errorToken[rightChild->token->tokenName]
			  );
	    }
		return NULL;
	}
}


/*
***********************************************************************************************
*	checkBooleanExpression:
*		child 	 : It is of ASTNode* type. It contains the root of the node in AST for which the
*			   	   boolean expression has to checked.
*		funTok   : It is of Token* type. It contains the Token for function which has that boolean
*				   expression.
*		tempCount: It has the total amount of space which will be needed by temporary variables in
*				   order to generate code for the given boolean expression.
*	This is used to check type mismatches in boolean expression.
************************************************************************************************
*/
VariableNode* checkBooleanExpression(ASTNode* child,Token* funTok, int* tempCount){
	int localOffset = 0;
	if(child->token->tokenName==TK_ID){
		ASTNode* field = NULL;
		if(child->child!=NULL){
			field = child->child;
		}
		VariableNode* var = createVarNode(child->token,funTok,NULL,INT,0,&localOffset);
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			var = freeVar;
			var->isGlobal = 1;
			var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		}
		if(var==NULL){
			free(freeVar);
			char errArguments[2][MAX_ERR_LEN];
			strcpy(errArguments[0],funTok->lexemeName);
			strcpy(errArguments[1],child->token->lexemeName);
			printSemanticError(-20,child->token->lineNum,errArguments);
			return NULL;
		}
		else{
			free(freeVar);
			if(var->type==RECORD && field!=NULL){
				FieldList* head = var->recordPtr->fields;
				for(;head!=NULL;head=head->next){
					if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
						return createVarNode(NULL,funTok,NULL,head->type,0,&localOffset);
					}
				}
				char errArguments[3][MAX_ERR_LEN];
				strcpy(errArguments[0],funTok->lexemeName);
				strcpy(errArguments[1],field->token->lexemeName);
				strcpy(errArguments[2],var->recordPtr->recTok->lexemeName);
				printSemanticError(-25,field->token->lineNum,errArguments);
				return NULL;
			}
			else if(var->type==RECORD){

				char errArguments[2][MAX_ERR_LEN];
				strcpy(errArguments[0],funTok->lexemeName);
				strcpy(errArguments[1],child->token->lexemeName);
				printSemanticError(-29,child->token->lineNum,errArguments);
				return NULL;
			}
			return var;
		}

	}
	if(child->token->tokenName==TK_NOT){
		return checkBooleanExpression(child->child,funTok,tempCount);
	}
	ASTNode* leftChild = child->child;
	ASTNode* rightChild = leftChild->next;
	Type leftType=-1,rightType=-1;
	VariableNode* leftNode = NULL;
	VariableNode* rightNode = NULL;
	leftType = (leftChild->token->tokenName==TK_NUM)? INT:-1;
	leftType = (leftChild->token->tokenName==TK_RNUM)?REAL:leftType;
	rightType = (rightChild->token->tokenName==TK_NUM)?INT:-1;
	rightType = (rightChild->token->tokenName==TK_RNUM)?REAL:rightType;
	if(leftType!=-1){
		*tempCount = *tempCount + ((leftType==INT)?INT_WIDTH:REAL_WIDTH);
	}
	if(rightType!=-1){
		*tempCount = *tempCount + ((rightType==INT)?INT_WIDTH:REAL_WIDTH);
	}
	if(child->token->tokenName==TK_AND||
		child->token->tokenName==TK_OR
		){
		checkBooleanExpression(leftChild,funTok,tempCount);
		return checkBooleanExpression(rightChild,funTok,tempCount);
		
	}
	if(	child->token->tokenName==TK_LT||
		child->token->tokenName==TK_LE||
		child->token->tokenName==TK_GT||
		child->token->tokenName==TK_GE||
		child->token->tokenName==TK_EQ||
		child->token->tokenName==TK_NE
	  ){
	  	if(leftType==-1)
			leftNode = checkBooleanExpression(leftChild,funTok,tempCount);
		if(rightType==-1)
			rightNode = checkBooleanExpression(rightChild,funTok,tempCount);
		if(leftType==-1 && leftNode!=NULL)
			leftType = leftNode->type;
		if(rightType==-1 && rightNode!=NULL)
			rightType = rightNode->type;
		if(leftType==-1||rightType==-1)
			return NULL;
	}
	if(leftType==rightType && leftType!=RECORD){
		return createVarNode(NULL,funTok,NULL,leftType,0,&localOffset);
	}
	else{
		HasErrors = 1;
		if(ShowErrors){
			printf("ERROR_30: In function %s type mismatch at line number %d for %s and %s\n",
															funTok->lexemeName,
														  leftChild->token->lineNum,
														(!isSymbol(leftChild->token->tokenName))?
																leftChild->token->lexemeName: errorToken[leftChild->token->tokenName],
														(!isSymbol(rightChild->token->tokenName))?
																rightChild->token->lexemeName: errorToken[rightChild->token->tokenName]
			  );
	    }
		return NULL;
	}
}


/*
***********************************************************************************************
*	checkBoolNode:
*		assignNode: It is of ASTNode* type. It contains the root of the node in AST
*				    which is at left hand side of assignment operator.
*		boolNode  : It is of ASTNode* type. It contains the boolean node in AST
*				    which is present in the boolean part(conditional part) of the while loop.
*	Function checks if the assigNode is present in the conditional part of the while loop.
*	This is a helping function used by checkWhileTerminates.
************************************************************************************************
*/
int checkBoolNode(ASTNode* assignNode, ASTNode* boolNode){
	if(strcmp(assignNode->token->lexemeName, boolNode->token->lexemeName)==0){
		return 1;
	}
	int cmpValue=0;
	boolNode = boolNode->child;
	while(boolNode!=NULL){
		cmpValue |= checkBoolNode(assignNode, boolNode);
		boolNode = boolNode->next;
	}
	return cmpValue;
}

/*
***********************************************************************************************
*	checkWhileTerminates:
*		boolNode  	  : It is of ASTNode* type. It contains the root of the while node in AST
*				        which has to be checked for termination.
*		statementNode : It has the ASTNode* of the node for statements within while loop for 
*						which we are checking.
*	This function traverses of all the nodes under the while statement check if the variable in
*	the boolean expression of the while loop is initialized or not.
************************************************************************************************
*/
int checkWhileTerminates(ASTNode* boolNode,ASTNode* statementNode){
	while(statementNode!=NULL){
		if(statementNode->token->tokenName==TK_ASSIGNOP){
			if(checkBoolNode(statementNode->child, boolNode)){
				return 1;
			}
		}
		if(statementNode->token->tokenName==TK_WHILE || statementNode->token->tokenName==TK_IF){
			if(checkWhileTerminates(boolNode, statementNode->child->next)){
				return 1;
			}
		}
		if(statementNode->token->tokenName==TK_ELSE){
			if(checkWhileTerminates(boolNode, statementNode->child)){
				return 1;
			}
		}
		statementNode = statementNode->next;
	}
	return 0;
}


/*
***********************************************************************************************
*	checkReturnExpression:
*		returnNode: It is of ASTNode* type. It contains the root of the return node in AST
*				    whose parameters are to be checked.
*		funTok	  : It has the token of the function for which the we are doing return statement
*				    checking.
*	This function traverses of all the nodes in the return statement check for their types
*	and if they are uninitialized.
***********************************************************************************************
*/
void checkReturnExpression(ASTNode* returnNode,Token* funTok){
	FunctionNode* funNode = (FunctionNode*)malloc(sizeof(FunctionNode));
	funNode->funTok = funTok;
	funNode = ((FunctionNode*)get(FUNCTION_TABLE.hashTable,funNode));
	ASTNode* childFun = returnNode;
	ASTNode* returnFun = returnNode->child;
	int localOffset = 0;
	VariableNode* freeVar = NULL;
	OutputPar* outputPar = funNode->outputPar;
	while(returnFun!=NULL || outputPar!=NULL){
		VariableNode* var = (outputPar!=NULL)? createVarNode(outputPar->token,funNode->funTok,NULL,INT,0,&localOffset):NULL;
		freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			char errArguments[1][MAX_ERR_LEN];
			strcpy(errArguments[0],funNode->funTok->lexemeName);
			printSemanticError(-12,childFun->token->lineNum,errArguments);//Extra parameters in return
			break;
		}
		else{
			VariableNode* returnVar = (returnFun!=NULL)? createVarNode(returnFun->token,funNode->funTok,NULL,INT,0,&localOffset):NULL;
			VariableNode* freeReturnVar = returnVar;
			returnVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,returnVar);
			if(returnVar==NULL && freeReturnVar!=NULL){
				returnVar = freeReturnVar;
				returnVar->isGlobal = 1;
				returnVar = (VariableNode*)get(SYMBOL_TABLE.hashTable,returnVar);
			}
			if(returnVar==NULL){
				char errArguments[1][MAX_ERR_LEN];
				strcpy(errArguments[0],funNode->funTok->lexemeName);
				printSemanticError(-13,childFun->token->lineNum,errArguments);//Less parameters in return
				break;
			}
			else{
				if(var->type==returnVar->type){
					if(var->type==2){
						if(!(var->recordPtr==returnVar->recordPtr)){
							char errArguments[2][MAX_ERR_LEN];
							strcpy(errArguments[0],funNode->funTok->lexemeName);
							strcpy(errArguments[1],returnVar->varTok->lexemeName);
							printSemanticError(-30,childFun->token->lineNum,errArguments);
						}
						else if(!(var==returnVar)){

								char errArguments[3][MAX_ERR_LEN];
								strcpy(errArguments[0],funNode->funTok->lexemeName);
								strcpy(errArguments[1],returnVar->varTok->lexemeName);
								strcpy(errArguments[2],var->varTok->lexemeName);
								printSemanticError(-31,childFun->token->lineNum,errArguments);
						}
						else if(!returnVar->isAssigned){

								char errArguments[2][MAX_ERR_LEN];
								strcpy(errArguments[0],funNode->funTok->lexemeName);
								strcpy(errArguments[1],returnVar->varTok->lexemeName);
								printSemanticError(-32,childFun->token->lineNum,errArguments);
						}
					}
					else{
						if(!(var==returnVar)){
								char errArguments[3][MAX_ERR_LEN];
								strcpy(errArguments[0],funNode->funTok->lexemeName);
								strcpy(errArguments[1],returnVar->varTok->lexemeName);
								strcpy(errArguments[2],var->varTok->lexemeName);
								printSemanticError(-31,returnVar->varTok->lineNum,errArguments);
						}
						else if(!var->isAssigned){

								char errArguments[2][MAX_ERR_LEN];
								strcpy(errArguments[0],funNode->funTok->lexemeName);
								strcpy(errArguments[1],returnVar->varTok->lexemeName);
								printSemanticError(-32,returnVar->varTok->lineNum,errArguments);
						} 
					}
				}
				else{

					char errArguments[2][MAX_ERR_LEN];
					strcpy(errArguments[0],funNode->funTok->lexemeName);
					strcpy(errArguments[1],returnVar->varTok->lexemeName);
					printSemanticError(-30,childFun->token->lineNum,errArguments);
				}
			}
			if(freeReturnVar!=NULL)
				free(freeReturnVar);
		}
		if(freeVar!=NULL)
			free(freeVar);
		outputPar = (outputPar!=NULL)? outputPar->next:NULL;
		returnFun = (returnFun!=NULL)? returnFun->next:NULL;
	}
}


/*
*******************************************************************************************
*	typeCheck:
*		root	: It is of ASTNode* type. It contains the root of the function node in AST
*				  which the type checking has to be done.
*		funTok	: It has the token of the function for which the we are doing type checking.
*	This function does post order traversal of all the nodes within the given function and
*	does all kinds of type checking including arithmetic expression, boolean expression,
*	function parameter matching, return parameter matching, while loop checking et al.
********************************************************************************************
*/
void typeCheck(ASTNode* root,Token* funTok){//root should be function TK_FUNID
	if(root==NULL){
		return;
	}
	ASTNode* child = root->child;
	FunctionNode* funInfo = (FunctionNode*)malloc(sizeof(FunctionNode));
	FunctionNode* freeFunInfo = funInfo;
	funInfo->funTok = funTok;
	funInfo = (FunctionNode*)get(FUNCTION_TABLE.hashTable,funInfo);
	free(freeFunInfo);
	VariableNode* var = NULL;
	VariableNode* freeVar = NULL;
	FieldList* field = NULL;
	int fieldFlag=0;
	while(child!=NULL){
		int tempCount=0;
		int localOffset=0;
		TokenName tokenName = child->token->tokenName;
		switch(tokenName){
			case TK_CALL: checkFunctionParameter(child->child,funTok);
						  break;
			case TK_ASSIGNOP: checkArithmeticExpression(child,funTok,&tempCount);
							  var = createVarNode(child->child->token,funTok,NULL,INT,0,&localOffset);
							  freeVar = var;
							  var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  if(var!=NULL){
							  		var->isAssigned = 1;
							  }
							  else{
							  	var = freeVar;
							  	var->isGlobal = 1;
							  	var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  	if(var!=NULL)
							  		var->isAssigned = 1;
							  }
							  free(freeVar);
							  if(funInfo->tempCount<tempCount){
							  	funInfo->tempCount = tempCount;
							  }
							break;
			case TK_IF: checkBooleanExpression(child->child,funTok,&tempCount);
						if(funInfo->tempCount<tempCount){
							funInfo->tempCount = tempCount;
						}
						break;
		    case TK_READ:
							  var = createVarNode(child->child->token,funTok,NULL,INT,0,&localOffset);
							  freeVar = var;
							  var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  fieldFlag = 0;
							  if(var!=NULL){
							  		var->isAssigned = 1;
							  }
							  else{
							  	var = freeVar;
							  	var->isGlobal = 1;
							  	var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  	if(var!=NULL){
							  		var->isAssigned = 1;
							  	}
							  	else{
									char errArguments[2][MAX_ERR_LEN];
									strcpy(errArguments[0],funTok->lexemeName);
									strcpy(errArguments[1],child->child->token->lexemeName);
									//printf("old = %d\n",var0>varTok->lineNum);
									printSemanticError(-20,child->child->token->lineNum,errArguments);
							  	}
							  }
						  		if(child->child->child!=NULL && var!=NULL){
						  			if(var->recordPtr==NULL){
										char errArguments[2][MAX_ERR_LEN];
										strcpy(errArguments[0],funTok->lexemeName);
										strcpy(errArguments[1],child->child->token->lexemeName);
										//printf("old = %d\n",var0>varTok->lineNum);
										printSemanticError(-34,child->child->token->lineNum,errArguments);
										fieldFlag = 1;

						  			}
						  			else{
							  			field = var->recordPtr->fields;
							  			for(;field!=NULL;field=field->next){
							  				if(strcmp(field->token->lexemeName,child->child->child->token->lexemeName)==0){
							  					fieldFlag = 1;
							  					break;
							  				}
							  			}
							  		}
							  		if(!fieldFlag){	  			
										char errArguments[3][MAX_ERR_LEN];
										strcpy(errArguments[0],funTok->lexemeName);
										strcpy(errArguments[1],child->child->child->token->lexemeName);
										strcpy(errArguments[2],var->recordPtr->recTok->lexemeName);
										//printf("old = %d\n",var0>varTok->lineNum);
										printSemanticError(-25,child->child->token->lineNum,errArguments);
							  		}
							  	}
							  free(freeVar);
							  break;
		    case TK_WRITE:
							  var = createVarNode(child->child->token,funTok,NULL,INT,0,&localOffset);
							  freeVar = var;
							  var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  fieldFlag = 0;
							  if(var==NULL){
							  	var = freeVar;
							  	var->isGlobal = 1;
							  	var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
							  	if(var==NULL){
									char errArguments[2][MAX_ERR_LEN];
									strcpy(errArguments[0],funTok->lexemeName);
									strcpy(errArguments[1],child->child->token->lexemeName);
									//printf("old = %d\n",var0>varTok->lineNum);
									printSemanticError(-20,child->child->token->lineNum,errArguments);
							  	}
							  }
						  		if(child->child->child!=NULL && var!=NULL){
						  			if(var->recordPtr==NULL){
										char errArguments[2][MAX_ERR_LEN];
										strcpy(errArguments[0],funTok->lexemeName);
										strcpy(errArguments[1],child->child->token->lexemeName);
										//printf("old = %d\n",var0>varTok->lineNum);
										printSemanticError(-34,child->child->token->lineNum,errArguments);
										fieldFlag = 1;
						  			}
						  			else{
							  			field = var->recordPtr->fields;
							  			for(;field!=NULL;field=field->next){
							  				if(strcmp(field->token->lexemeName,child->child->child->token->lexemeName)==0){
							  					fieldFlag = 1;
							  					break;
							  				}
							  			}
							  		}
							  		if(!fieldFlag){	  			
										char errArguments[3][MAX_ERR_LEN];
										strcpy(errArguments[0],funTok->lexemeName);
										strcpy(errArguments[1],child->child->child->token->lexemeName);
										strcpy(errArguments[2],var->recordPtr->recTok->lexemeName);
										//printf("old = %d\n",var0>varTok->lineNum);
										printSemanticError(-25,child->child->token->lineNum,errArguments);
							  		}
							  	}
							  free(freeVar);
							  break;
			case TK_WHILE: checkBooleanExpression(child->child,funTok,&tempCount);
							if(!checkWhileTerminates(child->child,child->child->next)){
								char errArguments[1][MAX_ERR_LEN];
								strcpy(errArguments[0],funTok->lexemeName);
								printSemanticError(-33,child->token->lineNum,errArguments);
							}
						  break;
			case TK_RETURN: checkReturnExpression(child,funTok);
							break;
		}
		typeCheck(child,funTok);
		child = child->next;
	}
}

/*
********************************************************************************************************
*	check:
*		root : It is of type ASTNode* which is the root of AST generated by the AST module.
*	This function calls all appropriate modules including populate symbol table,populate function table
*	and populate record table.
********************************************************************************************************
*/
void check(ASTNode* root){
	if(HasErrors){
		return;
	}
	createSymbolTable();
	populateRecordTable(root);
	populateFunctionTable(root);
	populateSymbolTable(root);
	root = root->child;
	while(root!=NULL){
		typeCheck(root,root->token);
		root = root->next;
	}
}
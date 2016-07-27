/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/ 
#include "codeGeneration.h"
#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>


/*
*******************************************************************************************
*	regString:
*		Contains the translated form of the register operands in three address code.
*		It has the actual operands which are used in the assembly language.
*		Except for 'IMM' which represents immediate data. And 'MEM' which represents 
*		the memory address incase the operand resides in memory.
*******************************************************************************************
*/
char regString[][4]={
	"ax",
	"bx",
	"rax",
	"rbx",
	"rcx",
	"eax",
	"edx",
	"esi",
	"rsp",
	"rdi",
	"rsi",
	"r8",
	"r9",
	"MEM",
	"IMM",
	""
};

/*
*******************************************************************************************
*	insString:
*		Contains the translated form of the instructions in three address code.
*		It has the actual operators which are used in the assembly language.
*******************************************************************************************
*/
char insString[][5]={
	"add",
	"sub",
	"mov",
	"mul",
	"div",
	"imul",
	"idiv",
	"cmp",
	"jl",
	"jle",
	"je",
	"jne",
	"jge",
	"jg",
	"jmp",
	"call",
	"push",
	"pop",
	"ret"
};


/*
************************************************************************************************
* data:
*	It stores data which will be printed at the data section of the assembly code.
************************************************************************************************
*/
char data[][20]={
	"format_in",
	"format_out_newline",
	"format_out_space",
	"global_var"
};

/*
***************************************************************************************************************
*	createInstruction:
*		ins 	: This is of type Instruction which is enumerated data type.
*		a   	: This is used to represent first operand or the register for the operation specified by ins.
*		b   	: This is used to represent second operand or the register for the operation specified by ins.
*		offset 	: Used incase second operand or first operand is memory location.
*		isGlobal: Tell if the memory location is global or local.
*		imm 	: Used to set immediate value if in used as operands.
*	This function returns the Three address code which is then stored in a form of linked list.
****************************************************************************************************************
*/
InstructionList* createInstruction(Instruction ins, Register a, Register b, int offset, int isGlobal,int imm){
	InstructionList* instruction = (InstructionList*)malloc(sizeof(InstructionList));
	instruction->ins = ins;
	instruction->a = a;
	instruction->b = b;
	instruction->offset = offset;
	instruction->imm = imm;
	instruction->isGlobal = isGlobal;
	instruction->isLabel = 0;
	return instruction;
}

/*
***************************************************************************************************************
*	createLabel:
*		labelType: This represents the type of label (0 or 1). 0---> F(False label) 1--->T(True label).
*		labelNum : This gives a number to the label.
*	This function is used to create labels in the code.The labels are of form T<labelNum> or F<labelNum>.
*	It stores labels in form of three address code i.e. Instruction List type.
****************************************************************************************************************
*/
InstructionList* createLabel(int labelType,int labelNum){
	InstructionList* newIns = createInstruction(-1,-1,-1,-1,-1,-1);
	newIns->isLabel = 1;
	(labelType)? sprintf(newIns->label,"T%d",labelNum):sprintf(newIns->label,"F%d",labelNum);
	return newIns;
}

/*
***************************************************************************************************************
*	addLabel:
*		labelType: This represents the type of label (0 or 1). 0---> F(False label) 1--->T(True label).
*		labelNum : This gives a number to the label.
*	This function is used to create labels in the code. These labels are temporary are not directly put into
*	generated code.
****************************************************************************************************************
*/
Label* addLabel(int labelType,int labelNum){
	Label* newLabel = (Label*)malloc(sizeof(Label));
	newLabel->labelType = labelType;
	newLabel->labelNum = labelNum;
	return newLabel;
}


/*
***************************************************************************************************************
*	generate:
*		ins: The three address code.
*	It adds the 'ins' three address code to the already existing code which has been generated till then.
*	The whole code is stored in form of linked list and CODE_head points to the head of this linked list.
****************************************************************************************************************
*/
void generate(InstructionList* ins){
	if(CODE_TAIL==NULL){
		CODE_TAIL = ins;
		CODE_TAIL->next = NULL;
		CODE_HEAD = ins;
		CODE_HEAD->next = NULL;
	}
	else{
		CODE_TAIL->next = ins;
		CODE_TAIL = CODE_TAIL->next;
		CODE_TAIL->next = NULL;
	}
}


/*
***************************************************************************************************************
*	createCallStack:
*		fun: The ASTNode of the function for which call stack has to pushed into the stack.
*	This function subtracts the width of the function(which includes the temporaries too) from the current 
*	stack pointer. Thus creating space for local and temporaries in the call stack.
****************************************************************************************************************
*/
void createCallStack(ASTNode* fun){
	FunctionNode* funNode = (FunctionNode*)malloc(sizeof(FunctionNode));
	funNode->funTok = fun->token;
	funNode = ((FunctionNode*)get(FUNCTION_TABLE.hashTable,funNode));
	generate(createInstruction(SUB,RSP,IMM,0,0,funNode->width+funNode->tempCount));
	if(GLOBAL_WIDTH!=0){
		InstructionList* globalIns = createInstruction(MOV,RCX,-1,-1,-1,-1);
		strcpy(globalIns->label,data[GLOBAL]);
		generate(globalIns);
	}
}


/*
*****************************************************************************************************************
*	generateArithmeticExpression:
*		child 		: The ASTNode of the expression for which code has to be generated.
*		funTok 	    : This has the Token(see lexerDef.h for Token details) for the function 
*				  	  which has the expression.
*		tempCount 	: This is the total space occupied by all the temporary variables used while generating
*					  code for the given arithmetic expression.
*	This function recurses and creates the code for left and right subtree of the expression and then combines
*	them.
*****************************************************************************************************************
*/
VariableNode* generateArithmeticExpression(ASTNode* child,Token* funTok,int *tempCount){
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
		free(freeVar);
		if(var->type==RECORD && field!=NULL){
			FieldList* head = var->recordPtr->fields;
			int recordOffset = var->offset;
			for(;head!=NULL;head=head->next){
				if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
					return createVarNode(NULL,funTok,NULL,head->type,0,&recordOffset);
				}
				recordOffset += head->width;
			}
		}
		return var;
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
		generate(createInstruction(MOV,MEM,IMM,*tempCount,0,leftChild->token->value.intValue));
		leftNode = createVarNode(NULL,funTok,NULL,leftType,0,tempCount);
	}
	if(rightType!=-1){
		generate(createInstruction(MOV,MEM,IMM,*tempCount,0,rightChild->token->value.intValue));
		rightNode = createVarNode(NULL,funTok,NULL,rightType,0,tempCount);
	}
	if(	child->token->tokenName==TK_PLUS||
		child->token->tokenName==TK_MINUS||
		child->token->tokenName==TK_MUL||
		child->token->tokenName==TK_DIV||
		child->token->tokenName==TK_ASSIGNOP
	  ){
	  	if(leftType==-1)
			leftNode = generateArithmeticExpression(leftChild,funTok,tempCount);
		if(rightType==-1)
			rightNode = generateArithmeticExpression(rightChild,funTok,tempCount);
		if(leftType==-1 && leftNode!=NULL)
			leftType = leftNode->type;
		if(rightType==-1 && rightNode!=NULL)
			rightType = rightNode->type;
	}
	if(child->token->tokenName==TK_ASSIGNOP){

		if(leftType!=RECORD){
			generate(createInstruction(MOV, EAX, MEM, rightNode->offset,rightNode->isGlobal,0));
			generate(createInstruction(MOV, MEM, EAX, leftNode->offset,leftNode->isGlobal,0));
		}
		else{
			FieldList* field = leftNode->recordPtr->fields;
			int leftRecordOffset = leftNode->offset;
			int rightRecordOffset = rightNode->offset;
			for(;field!=NULL;field=field->next){
				generate(createInstruction(MOV, EAX, MEM, rightRecordOffset,rightNode->isGlobal,0));
				//int temp = leftRecordOffset;
				generate(createInstruction(MOV, MEM, EAX, leftRecordOffset,leftNode->isGlobal,0));								
				rightRecordOffset+=(field->width);
				leftRecordOffset+=field->width;
				//leftRecordOffset = temp + field->width;
			}
			return NULL;
		}
	}
	if(leftType==rightType && leftType!=RECORD){

		switch(child->token->tokenName){
			case TK_PLUS:	generate(createInstruction(MOV, EAX, MEM, leftNode->offset,leftNode->isGlobal,0));
							generate(createInstruction(ADD, EAX, MEM, rightNode->offset,rightNode->isGlobal,0));
							generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
							break;
			case TK_MINUS:	generate(createInstruction(MOV, EAX, MEM, leftNode->offset,leftNode->isGlobal,0));
							generate(createInstruction(SUB, EAX, MEM, rightNode->offset,rightNode->isGlobal,0));
							generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
							break;
			case TK_MUL:	generate(createInstruction(MOV, EAX, MEM, leftNode->offset,leftNode->isGlobal,0));
							generate(createInstruction(IMUL, MEM, NONE, rightNode->offset,rightNode->isGlobal,0));
							generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
							break;
			case TK_DIV:	generate(createInstruction(MOV, EAX, MEM, leftNode->offset,leftNode->isGlobal,0));
							generate(createInstruction(MOV, EDX, IMM, 0,0,0));
							generate(createInstruction(IDIV, MEM, NONE, rightNode->offset,rightNode->isGlobal,0));
							generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
							break;
		}
		localOffset = *tempCount;
		*tempCount = *tempCount + ((leftType==INT)?INT_WIDTH:REAL_WIDTH);
		return createVarNode(NULL,funTok,NULL,leftType,0,&localOffset);
	}
	else if(leftType==rightType){
		int leftRecordOffset = leftNode->offset;
		int rightRecordOffset = rightNode->offset;
		FieldList* field = leftNode->recordPtr->fields;
		localOffset = *tempCount;
		switch(child->token->tokenName){

			case TK_PLUS:	for(;field!=NULL;field=field->next){
								generate(createInstruction(MOV, EAX, MEM, leftRecordOffset,leftNode->isGlobal,0));
								generate(createInstruction(ADD, EAX, MEM, rightRecordOffset,rightNode->isGlobal,0));
								generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
								leftRecordOffset+=field->width;
								rightRecordOffset+=field->width;
								*tempCount += field->width;
							}
							break;
			case TK_MINUS:	for(;field!=NULL;field=field->next){
								generate(createInstruction(MOV, EAX, MEM, leftRecordOffset,leftNode->isGlobal,0));
								generate(createInstruction(SUB, EAX, MEM, rightRecordOffset,rightNode->isGlobal,0));
								generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
								leftRecordOffset+=field->width;
								rightRecordOffset+=field->width;
								*tempCount += field->width;
							}
							break;
		}
		return createVarNode(NULL,funTok,leftNode->recordPtr->recTok,leftType,0,&localOffset);
	}
	else{
		int recordOffset = (leftType==INT||leftType==REAL)?rightNode->offset:leftNode->offset;
		int scalarOffset = (leftType==INT||leftType==REAL)?leftNode->offset:rightNode->offset;
		int isGlobal = (leftType==INT||leftType==REAL)?rightNode->isGlobal:leftNode->isGlobal;
		FieldList* field = (leftType==INT||leftType==REAL)?rightNode->recordPtr->fields:leftNode->recordPtr->fields;
		RecordNode* recordPtr = (leftType==INT||leftType==REAL)?rightNode->recordPtr:leftNode->recordPtr;
		localOffset = *tempCount;
		switch(child->token->tokenName){

			case TK_MUL:	for(;field!=NULL;field=field->next){
								generate(createInstruction(MOV, EAX, MEM, recordOffset,isGlobal,0));
								generate(createInstruction(IMUL, MEM, NONE, scalarOffset,0,0));
								generate(createInstruction(MOV, MEM, EAX, *tempCount,0,0));
								recordOffset+=field->width;
								*tempCount += field->width;
							}
							break;
			case TK_DIV:	for(;field!=NULL;field=field->next){
								generate(createInstruction(MOV, EAX, MEM, recordOffset, isGlobal,0));
								generate(createInstruction(MOV, EDX, IMM, 0,0,0));
								generate(createInstruction(IDIV, MEM, NONE, scalarOffset,0,0));
								generate(createInstruction(MOV, MEM, EAX, *tempCount, 0,0));
								recordOffset+=field->width;
								*tempCount += field->width;
							}
							break;
		}
		return createVarNode(NULL,funTok,recordPtr->recTok,leftType,0,&localOffset);
	}
}



/*
*****************************************************************************************************************
*	generateBooleanExpression:
*		child 		: The ASTNode of the expression for which code has to be generated.
*		funTok 	    : This has the Token(see lexerDef.h for Token details) for the function 
*				  	  which has the expression.
*		trueLabel	: The code will jump to this label if the current subtree represented 
*					  by 'child' evaluates to true.
*		falseLabel  : The code will jump to this label if the current subtree represented
*					  by 'child' evaluates to false.
*		tempCount 	: This is the total space occupied by all the temporary variables used while generating
*					  code for the given boolean expression.
*	This function recurses and creates the code for left and right subtree of the expression and then combines
*	them.
*****************************************************************************************************************
*/
VariableNode* generateBooleanExpression(ASTNode* child,Token* funTok,Label* trueLabel,Label* falseLabel,Label* currLabel,int* tempCount){
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
		free(freeVar);
		if(var->type==RECORD && field!=NULL){
			FieldList* head = var->recordPtr->fields;
			for(;head!=NULL;head=head->next){
				if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
					return createVarNode(NULL,funTok,NULL,head->type,0,&localOffset);
				}
			}
		}
		return var;
	}
	if(child->token->tokenName==TK_NOT){
		VariableNode* notVariable = generateBooleanExpression(child->child,funTok,falseLabel,trueLabel,currLabel,tempCount);
		return NULL;

	}
	ASTNode* leftChild = child->child;
	ASTNode* rightChild = leftChild->next;
	Type leftType=-1,rightType=-1;
	VariableNode* leftNode = NULL;
	VariableNode* rightNode = NULL;
	if(leftChild!=NULL){
		leftType = (leftChild->token->tokenName==TK_NUM)? INT:-1;
		leftType = (leftChild->token->tokenName==TK_RNUM)?REAL:leftType;
	}
	if(rightChild!=NULL){
		rightType = (rightChild->token->tokenName==TK_NUM)?INT:-1;
		rightType = (rightChild->token->tokenName==TK_RNUM)?REAL:rightType;
	}
	if(leftType!=-1){
		generate(createInstruction(MOV,MEM,IMM,*tempCount,0,leftChild->token->value.intValue));
		leftNode = createVarNode(NULL,funTok,NULL,leftType,0,tempCount);
	}
	if(rightType!=-1){
		generate(createInstruction(MOV,MEM,IMM,*tempCount,0,rightChild->token->value.intValue));
		rightNode = createVarNode(NULL,funTok,NULL,rightType,0,tempCount);
	}
	if(child->token->tokenName==TK_AND){
		Label* ANDLabel = addLabel(1,currLabel->labelNum);
		currLabel->labelNum = currLabel->labelNum +1;
		currLabel->labelType = 1;
		generateBooleanExpression(leftChild,funTok,ANDLabel,falseLabel,currLabel,tempCount);
		generate(createLabel(1,ANDLabel->labelNum));
		generateBooleanExpression(rightChild,funTok, trueLabel, falseLabel, currLabel,tempCount);
		return NULL;
	}
	if(child->token->tokenName==TK_OR){
		Label* ORLabel = addLabel(0,currLabel->labelNum);
		currLabel->labelNum = currLabel->labelNum +1;
		currLabel->labelType = 0;
		generateBooleanExpression(leftChild,funTok,trueLabel,ORLabel, currLabel,tempCount);
		generate(createLabel(0,ORLabel->labelNum));
		generateBooleanExpression(rightChild,funTok, trueLabel, falseLabel, currLabel,tempCount);
		return NULL;
	}
	if(leftNode==NULL){
		leftNode = generateBooleanExpression(leftChild,funTok,trueLabel,falseLabel,currLabel,tempCount);
	}
	if(rightNode==NULL){
		rightNode = generateBooleanExpression(rightChild,funTok,trueLabel,falseLabel,currLabel,tempCount);
	}
	switch(child->token->tokenName){
		case TK_LT:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JL,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JGE,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
		case TK_GT:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JG,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JLE,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
		case TK_LE:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JLE,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JG,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
		case TK_GE:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JGE,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JL,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
		case TK_EQ:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JE,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JNE,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
		case TK_NE:	generate(createInstruction(MOV,EAX,MEM,leftNode->offset,leftNode->isGlobal,0));
					generate(createInstruction(CMP,EAX,MEM,rightNode->offset,rightNode->isGlobal,0));
					generate(createInstruction(JNE,-1,-1,trueLabel->labelNum,trueLabel->labelType,0));
					generate(createInstruction(JE,-1,-1,falseLabel->labelNum,falseLabel->labelType,0));	
					break;
	}
	return NULL;
}

/*
*****************************************************************************************************************
*	generateScanf:
*		child 		: The ASTNode of the id which has to be scanned.
*		funInfo 	: This has the FunctionNode(see astDef.h for details) for the function 
*				  	  which has the 'read' function.
*	This function simply creates instruction to scan the identifier specified.
*	It handles record types too.
*****************************************************************************************************************
*/
void generateScanf(ASTNode* child,FunctionNode* funInfo){
	int localOffset = 0;
	if(child->token->tokenName==TK_ID){
		ASTNode* field = NULL;
		if(child->child!=NULL){
			field = child->child;
		}
		VariableNode* var = createVarNode(child->token,funInfo->funTok,NULL,INT,0,&localOffset);
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			var = freeVar;
			var->isGlobal = 1;
			var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		}
		free(freeVar);
		if(var->type==RECORD && field!=NULL){
			FieldList* head = var->recordPtr->fields;
			int tempOffset = var->offset;
			for(;head!=NULL;tempOffset+=head->width,head=head->next){
				if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
					generate(createInstruction(MOV,RBX,(var->isGlobal)?RCX:RSP,0,0,0));
					generate(createInstruction(PUSH,RAX,NONE,0,0,0));
					generate(createInstruction(PUSH,RCX,NONE,0,0,0));
					generate(createInstruction(ADD,RBX,IMM,0,0,tempOffset));
					generate(createInstruction(MOV,RSI,RBX,0,0,0));
					InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
					strcpy(label->label,data[FORMAT_IN]);
					generate(label);
					generate(createInstruction(MOV,RAX,IMM,0,0,0));
					InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
					strcpy(call->label,"scanf");
					generate(call);
					generate(createInstruction(POP,RCX,NONE,0,0,0));
					generate(createInstruction(POP,RAX,NONE,0,0,0));
				}
			}
		}
		else if(var->type==RECORD){
			FieldList* head = var->recordPtr->fields;
			int tempOffset = var->offset;
			for(;head!=NULL;tempOffset+=head->width,head=head->next){
				generate(createInstruction(MOV,RBX,(var->isGlobal)?RCX:RSP,0,0,0));
				generate(createInstruction(PUSH,RAX,NONE,0,0,0));
				generate(createInstruction(PUSH,RCX,NONE,0,0,0));
				generate(createInstruction(ADD,RBX,IMM,0,0,tempOffset));
				generate(createInstruction(MOV,RSI,RBX,0,0,0));
				InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
				strcpy(label->label,data[FORMAT_IN]);
				generate(label);
				generate(createInstruction(MOV,RAX,IMM,0,0,0));
				InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
				strcpy(call->label,"scanf");
				generate(call);
				generate(createInstruction(POP,RCX,NONE,0,0,0));
				generate(createInstruction(POP,RAX,NONE,0,0,0));
			}

		}
		else{
				generate(createInstruction(MOV,RBX,(var->isGlobal)?RCX:RSP,0,0,0));
				generate(createInstruction(PUSH,RAX,NONE,0,0,0));
				generate(createInstruction(PUSH,RCX,NONE,0,0,0));
				generate(createInstruction(ADD,RBX,IMM,0,0,var->offset));
				generate(createInstruction(MOV,RSI,RBX,0,0,0));
				InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
				strcpy(label->label,data[FORMAT_IN]);
				generate(label);
				generate(createInstruction(MOV,RAX,IMM,0,0,0));
				InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
				strcpy(call->label,"scanf");
				generate(call);
				generate(createInstruction(POP,RCX,NONE,0,0,0));	
				generate(createInstruction(POP,RAX,NONE,0,0,0));
		}
	}

}

/*
*****************************************************************************************************************
*	generatePrintf:
*		child 		: The ASTNode of the id which has to be printed.
*		funInfo 	: This has the FunctionNode(see astDef.h for details) for the function 
*				  	  which has the 'write' function.
*	This function simply creates instruction to write the identifier specified.
*	It handles record types too.
*****************************************************************************************************************
*/
void generatePrintf(ASTNode* child,FunctionNode* funInfo){
	int localOffset = 0;
	if(child->token->tokenName==TK_ID){
		ASTNode* field = NULL;
		if(child->child!=NULL){
			field = child->child;
		}
		VariableNode* var = createVarNode(child->token,funInfo->funTok,NULL,INT,0,&localOffset);
		VariableNode* freeVar = var;
		var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		if(var==NULL){
			var = freeVar;
			var->isGlobal = 1;
			var = (VariableNode*)get(SYMBOL_TABLE.hashTable,var);
		}
		free(freeVar);
		if(var->type==RECORD && field!=NULL){
			FieldList* head = var->recordPtr->fields;
			int tempOffset = var->offset;
			for(;head!=NULL;tempOffset+=head->width,head=head->next){
				if(strcmp(field->token->lexemeName,head->token->lexemeName)==0){
					generate(createInstruction(MOV,RSI,IMM,0,0,0));
					generate(createInstruction(MOV,ESI,MEM,tempOffset,var->isGlobal,0));
					InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
					strcpy(label->label,data[FORMAT_OUT_NEWLINE]);
					generate(label);
					generate(createInstruction(PUSH,RAX,NONE,0,0,0));
					generate(createInstruction(PUSH,RCX,NONE,0,0,0));
					generate(createInstruction(MOV,RAX,IMM,0,0,0));
					InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
					strcpy(call->label,"printf");
					generate(call);
					generate(createInstruction(POP,RCX,NONE,0,0,0));	
					generate(createInstruction(POP,RAX,NONE,0,0,0));
				}
			}
		}
		else if(var->type==RECORD){
			FieldList* head = var->recordPtr->fields;
			int tempOffset = var->offset;
			for(;head!=NULL;tempOffset+=head->width,head=head->next){
				generate(createInstruction(MOV,RSI,IMM,0,0,0));
				generate(createInstruction(MOV,ESI,MEM,tempOffset,var->isGlobal,0));
				InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
				strcpy(label->label,data[FORMAT_OUT_SPACE]);
				generate(label);
				generate(createInstruction(PUSH,RAX,NONE,0,0,0));
				generate(createInstruction(PUSH,RCX,NONE,0,0,0));
				generate(createInstruction(MOV,RAX,IMM,0,0,0));
				InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
				strcpy(call->label,"printf");
				generate(call);
				generate(createInstruction(POP,RCX,NONE,0,0,0));	
				generate(createInstruction(POP,RAX,NONE,0,0,0));
			}

		}
		else{				
				generate(createInstruction(MOV,RSI,IMM,0,0,0));
				generate(createInstruction(MOV,ESI,MEM,var->offset,var->isGlobal,0));
				InstructionList* label = createInstruction(MOV,RDI,-1,-1,-1,-1);
				strcpy(label->label,data[FORMAT_OUT_NEWLINE]);
				generate(label);
				generate(createInstruction(PUSH,RAX,NONE,0,0,0));
				generate(createInstruction(PUSH,RCX,NONE,0,0,0));
				generate(createInstruction(MOV,RAX,IMM,0,0,0));
				InstructionList* call = createInstruction(CALL,-1,-1,-1,-1,-1);
				strcpy(call->label,"printf");
				generate(call);
				generate(createInstruction(POP,RCX,NONE,0,0,0));	
				generate(createInstruction(POP,RAX,NONE,0,0,0));	
		}
	}

}



/*
*****************************************************************************************************************
*	codeGenerator:
*		child 		: The ASTNode for which the code has to be generated.
*		funInfo 	: This has the FunctionNode(see astDef.h for details) for the function 
*				  	  which has the 'child'.
*		currLabel	: Possible label which can be assigned to the current instruction.
*		parentLabel : The label of the parent node in AST.
*	This function simply does the post order traversal of a function and generates code for that function.
*	In our case it works only for the main function.
*****************************************************************************************************************
*/
void codeGenerator(ASTNode* child,FunctionNode* funInfo,Label* currLabel, int parentLabel){
	if(child==NULL){
		return;
	}
	int tempCount = funInfo->width;
	int tempLabel = currLabel->labelNum;
	InstructionList* funLabel = NULL;
	ASTNode* ifChild = NULL;
	int ifFlag=0;
	TokenName tokenName = child->token->tokenName;
	switch(tokenName){
		case TK_MAIN: funLabel = createLabel(1,0);
					  strcpy(funLabel->label,"main");
					  generate(funLabel);
					  createCallStack(child);
					  break;
		case TK_ASSIGNOP:	generateArithmeticExpression(child,funInfo->funTok,&tempCount);
							break;
		case TK_IF: currLabel->labelNum += 2;
					generateBooleanExpression(child->child,funInfo->funTok,addLabel(1,tempLabel),addLabel(0,tempLabel),currLabel,&tempCount);
					generate(createLabel(1,tempLabel));
					break;
		case TK_ELSE: generate(createInstruction(JMP,-1,-1,parentLabel+1,1,0));
					  generate(createLabel(0,parentLabel));
					  break;
		case TK_READ: generateScanf(child->child,funInfo);
					  break;
		case TK_WRITE: generatePrintf(child->child,funInfo);
						break;
		case TK_WHILE: generate(createLabel(1,currLabel->labelNum));
					   currLabel->labelNum += 2;
					   generateBooleanExpression(child->child,funInfo->funTok,addLabel(1,tempLabel+1),addLabel(0,tempLabel),currLabel,&tempCount);
					   generate(createLabel(1,tempLabel+1));
					   break;
	}
	if(tokenName==TK_IF)
		codeGenerator(child->child,funInfo,currLabel, tempLabel);
	else
		codeGenerator(child->child,funInfo,currLabel, parentLabel);
	switch(tokenName){
		case TK_MAIN:  generate(createInstruction(ADD,RSP,IMM,0,0,funInfo->width+funInfo->tempCount));
						generate(createInstruction(RET,-1,-1,-1,-1,-1));
						break;
		case TK_WHILE:  generate(createInstruction(JMP,-1,-1,tempLabel,1,0));
						generate(createLabel(0,tempLabel));
					  break;	
		case TK_IF:		generate(createLabel(1,tempLabel+1));
						ifChild = child->child;
						while(ifChild!=NULL){
							if(ifChild->token->tokenName==TK_ELSE){
								ifFlag = 1;
								break;
							}
							ifChild = ifChild->next;
						}
						if(!ifFlag)
						generate(createLabel(0,tempLabel));
						break;
	}
	codeGenerator(child->next,funInfo,currLabel, parentLabel);	
}

/*
*****************************************************************************************************************
*	startGeneration:
*		root 		: The ASTNode* generated after creating AST.
*	This function initializes the link list of three address code which is to be generated.
*	Calls codeGenerator function which does rest of the job.
*****************************************************************************************************************
*/
void startGeneration(ASTNode* root){
	CODE_HEAD = NULL;
	CODE_TAIL = NULL;
	FunctionNode* funInfo = (FunctionNode*)malloc(sizeof(FunctionNode));
	FunctionNode* freeFunInfo = funInfo;
	funInfo->funTok = root->child->token;
	funInfo = (FunctionNode*)get(FUNCTION_TABLE.hashTable,funInfo);
	Label* currLabel = (Label*)malloc(sizeof(Label));
	currLabel->labelType = 1;
	currLabel->labelNum = 0;
	codeGenerator(root->child,funInfo,currLabel,0);	
}

/*
*****************************************************************************************************************
*	printInstruction:
*		filePtr 		: The FILE* for the file which will store the ASM code.
*		instruction 	: This has the instruction list(Three address code linked list) which is used to 
*						  the final assembly code.
*	This function prints the link list of three address code to finally create the asm file.
*****************************************************************************************************************
*/
void printInstruction(FILE* filePtr,InstructionList* instruction){
	if(instruction->isLabel){
		fprintf(filePtr,"%s:\n",instruction->label);
		return;
	}
	fprintf(filePtr,"\t%s ",insString[instruction->ins]);
	if(instruction->ins==RET){
		fprintf(filePtr, "\n");
		return;
	}
	if((instruction->ins==PUSH || instruction->ins==CALL) && instruction->a==-1){
		fprintf(filePtr, " %s\n",instruction->label);
		return;
	}
	if(instruction->ins > CMP && instruction->ins < CALL){
		fprintf(filePtr," %s%d\n",(instruction->isGlobal)?"T":"F",instruction->offset);
		return;
	}
	if(instruction->a==MEM){
		fprintf(filePtr," dword[%s%d]",(instruction->isGlobal)?"rcx+":"rsp+",instruction->offset);

	}
	else if(instruction->a==IMM){
		fprintf(filePtr," %d",instruction->imm);
	}
	else{
		fprintf(filePtr," %s",regString[instruction->a]);
	}

	if(instruction->b==MEM){
		fprintf(filePtr,",dword[%s%d]\n",(instruction->isGlobal)?"rcx+":"rsp+",instruction->offset);
	}
	else if(instruction->b==IMM){
		fprintf(filePtr,",%d\n",instruction->imm);
	}
	else if(instruction->b==NONE){
		fprintf(filePtr, "\n");
	}
	else if(instruction->b==-1){
		fprintf(filePtr, ",%s\n",instruction->label);
	}
	else{
		fprintf(filePtr,",%s\n",regString[instruction->b]);		
	}
}


/*
********************************************************************************************************
*	initializeCode:
*		filePtr	: This has the file pointer of the file which will contain the final ASM code.
*	This function initializes the ASM code and write the important statements like extern and global
*	at the start of the asm file.
********************************************************************************************************
*/
void initializeCode(FILE* filePtr){
	fprintf(filePtr, "\t\tglobal main\n");
	fprintf(filePtr, "\t\textern printf\n");
	fprintf(filePtr, "\t\textern scanf\n");
	fprintf(filePtr, "\t\tsection .text\n");
}



/*
********************************************************************************************************
*	finalizeCode:
*		filePtr	: This has the file pointer of the file which will contain the final ASM code.
*	This function writes the end part of the ASM code and write the important statements 
*	like printf format, scanf format and global varibale.
********************************************************************************************************
*/
void finalizeCode(FILE* filePtr){
	fprintf(filePtr, "\n\t\tsection .data\n\n");
	fprintf(filePtr,"format_in: db \"%%d\",0\n");
	fprintf(filePtr,"format_out_newline: db \"%%d\",10,0\n");
	fprintf(filePtr,"format_out_space: db \"%%d \",0\n");
	if(GLOBAL_WIDTH!=0)
		fprintf(filePtr,"global_var: times %d db 0\n",GLOBAL_WIDTH);
}

/*
***********************************************************************************************************
*	generateASM:
*		filename : This has the filename of the file in which the ASM code will be written.
*	This function initiates the process of generation of code. This is called after
*	startGenertation which is actually responsible for creation three address code for all instructions
*	This function is responsible to print those three address code in meaningful way in the file specified.
***********************************************************************************************************
*/
void generateASM(char* filename){
	InstructionList* head = CODE_HEAD;
	FILE* filePtr = fopen(filename,"w");
	initializeCode(filePtr);
	while(head!=NULL){
		printInstruction(filePtr,head);
		head = head->next;
	}
	finalizeCode(filePtr);
	fclose(filePtr);
}
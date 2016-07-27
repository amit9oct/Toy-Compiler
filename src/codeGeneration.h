/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/ 
#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H
#define MAX_LABEL 20
#define FORMAT_IN 0
#define FORMAT_OUT_NEWLINE 1
#define FORMAT_OUT_SPACE 2
#define GLOBAL 3
#include "ast.h"
#include "symbolTable.h"


/*
**********************************************************************************************
*	Instruction:
*		This is enumerated data type for the insrtuctions available in our three address code
**********************************************************************************************
*/
typedef enum instruction{
	ADD,
	SUB,
	MOV,
	MUL,
	DIV,
	IMUL,
	IDIV,
	CMP,
	JL,
	JLE,
	JE,
	JNE,
	JGE,
	JG,
	JMP,
	CALL,
	PUSH,
	POP,
	RET
}Instruction;


/*
**************************************************************************************************
* Register:
*	This is used to represent the operand in our three address code.(Our three address code is 
*	very close to assembly laguage). We can have memory('MEM') as well as immediate data('IMM')
*	too as operands.
**************************************************************************************************
*/
typedef enum reg{
	AX,
	BX,
	RAX,
	RBX,
	RCX,
	EAX,
	EDX,
	ESI,
	RSP,
	RDI,
	RSI,
	R8,
	R9,
	MEM,
	IMM,
	NONE
}Register;


/*
**************************************************************************************************
* InstructionList:
*	Data structure used for storing the three address code.
**************************************************************************************************
*/
typedef struct instructionList{
	Instruction ins;
	Register a,b;
	int offset;
	int imm;
	int isGlobal;
	int isLabel;
	char label[MAX_LABEL];
	struct instructionList* next;
}InstructionList;

/*
**************************************************************************************************
* Label:
*	Data structure used for storing the labels while code generation.
**************************************************************************************************
*/
typedef struct label{
	int labelType;
	int labelNum;
}Label;


/*
*****************************************************************************************************
* CODE_head,CODE_tail:
*	Used to store the head and tail pointers of the Instruction List(three address code linked list).
*****************************************************************************************************
*/
InstructionList* CODE_HEAD, *CODE_TAIL;



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
void codeGenerator(ASTNode* root,FunctionNode* funInfo,Label* currLabel, int parentLabel);

/*
***********************************************************************************************************
*	generateASM:
*		filename : This has the filename of the file in which the ASM code will be written.
*	This function initiates the process of generation of code. This is called after
*	startGenertation which is actually responsible for creation three address code for all instructions
*	This function is responsible to print those three address code in meaningful way in the file specified.
***********************************************************************************************************
*/
void generateASM(char* filename);

/*
*****************************************************************************************************************
*	startGeneration:
*		root 		: The ASTNode* generated after creating AST.
*	This function initializes the link list of three address code which is to be generated.
*	Calls codeGenerator function which does rest of the job.
*****************************************************************************************************************
*/
void startGeneration(ASTNode* root);
#endif
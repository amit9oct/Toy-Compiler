/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include<string.h>
#include<stdio.h>
#include "parserDef.h"  //for basic structure of parser and parse tree
#include "set.h"    //for data structure set
#include "lexer.h"  //for the functions getNextToken()
#include "stack.h"  //for data structure stack
#include "error.h"  //for printing error


int PARSER_SIZE = 0; //used to store the size of the parse tree.

/*
*******************************************************************************************************************
*   grammar:
*   The grammar is presented in form of 2-D array. It uses the enum Alphabet (see parserDef) for representing the
*   the terminals and non-terminals.
*
*******************************************************************************************************************
*/
int grammar[][MAX_RHS_LEN]=
{
	{NT_PROGRAM,NT_OTHER_FUNCTIONS,NT_MAIN_FUNCTION,RULE_END},
	{NT_MAIN_FUNCTION,T_MAIN,NT_STMTS,T_END,RULE_END},
	{NT_OTHER_FUNCTIONS,NT_FUNCTION,NT_OTHER_FUNCTIONS,RULE_END},
	{NT_OTHER_FUNCTIONS,T_EPS,RULE_END},
	{NT_FUNCTION,T_FUNID,NT_INPUT_PAR,NT_OUTPUT_PAR,T_SEM,NT_STMTS,T_END,RULE_END},
	{NT_INPUT_PAR,T_INPUT,T_PARAMETER,T_LIST,T_SQL,NT_PARAMETER_LIST,T_SQR,RULE_END},
	{NT_OUTPUT_PAR,T_OUTPUT,T_PARAMETER,T_LIST,T_SQL,NT_PARAMETER_LIST,T_SQR,RULE_END},
	{NT_OUTPUT_PAR,T_EPS,RULE_END},
	{NT_PARAMETER_LIST,NT_DATATYPE,T_ID,NT_REMAINING_LIST,RULE_END},
	{NT_DATATYPE,NT_PRIMITIVE_DATATYPE,RULE_END},
	{NT_DATATYPE,NT_CONSTRUCTED_DATATYPE,RULE_END},
	{NT_PRIMITIVE_DATATYPE,T_INT,RULE_END},
	{NT_PRIMITIVE_DATATYPE,T_REAL,RULE_END},
	{NT_CONSTRUCTED_DATATYPE,T_RECORD,T_RECORDID,RULE_END},
	{NT_REMAINING_LIST,T_COMMA,NT_PARAMETER_LIST,RULE_END},
	{NT_REMAINING_LIST,T_EPS,RULE_END},
	{NT_STMTS,NT_TYPE_DEFINITIONS,NT_DECLARATIONS,NT_OTHER_STMTS,NT_RETURN_STMT,RULE_END},
	{NT_TYPE_DEFINITIONS,NT_TYPE_DEFINITION,NT_TYPE_DEFINITIONS,RULE_END},
	{NT_TYPE_DEFINITIONS,T_EPS,RULE_END},
	{NT_TYPE_DEFINITION,T_RECORD,T_RECORDID,NT_FIELD_DEFINITIONS,T_ENDRECORD,T_SEM,RULE_END},
	{NT_FIELD_DEFINITIONS,NT_FIELD_DEFINITION,NT_FIELD_DEFINITION,NT_MORE_FIELDS,RULE_END},
	{NT_FIELD_DEFINITION,T_TYPE,NT_PRIMITIVE_DATATYPE,T_COLON,T_FIELDID,T_SEM,RULE_END},
	{NT_MORE_FIELDS,NT_FIELD_DEFINITION,NT_MORE_FIELDS,RULE_END},
	{NT_MORE_FIELDS,T_EPS,RULE_END},
	{NT_DECLARATIONS,NT_DECLARATION,NT_DECLARATIONS,RULE_END},
	{NT_DECLARATIONS,T_EPS,RULE_END},
	{NT_DECLARATION,T_TYPE,NT_DATATYPE,T_COLON,T_ID,NT_GLOBAL_OR_NOT,T_SEM,RULE_END},
	{NT_GLOBAL_OR_NOT,T_COLON,T_GLOBAL,RULE_END},
	{NT_GLOBAL_OR_NOT,T_EPS,RULE_END},
	{NT_OTHER_STMTS,NT_STMT,NT_OTHER_STMTS,RULE_END},
	{NT_OTHER_STMTS,T_EPS,RULE_END},
	{NT_STMT,NT_ASSIGNMENT_STMT,RULE_END},
	{NT_STMT,NT_ITERATIVE_STMT,RULE_END},
	{NT_STMT,NT_CONDITIONAL_STMT,RULE_END},
	{NT_STMT,NT_IO_STMT,RULE_END},
	{NT_STMT,NT_FUN_CALL_STMT,RULE_END},
	{NT_ASSIGNMENT_STMT,NT_SINGLE_OR_RECORDID,T_ASSIGNOP,NT_ARITHMETIC_EXPRESSION,T_SEM,RULE_END},
	{NT_SINGLE_OR_RECORDID,T_ID,NT_NEW_24,RULE_END},
	{NT_NEW_24,T_EPS,RULE_END},
	{NT_NEW_24,T_DOT,T_FIELDID,RULE_END},
	{NT_FUN_CALL_STMT,NT_OUTPUT_PARAMETERS,T_CALL,T_FUNID,T_WITH,T_PARAMETERS,NT_INPUT_PARAMETERS,T_SEM,RULE_END},
	{NT_OUTPUT_PARAMETERS,T_SQL,NT_ID_LIST,T_SQR,T_ASSIGNOP,RULE_END},
	{NT_OUTPUT_PARAMETERS,T_EPS,RULE_END},
	{NT_INPUT_PARAMETERS,T_SQL,NT_ID_LIST,T_SQR,RULE_END},
	{NT_ITERATIVE_STMT,T_WHILE,T_OP,NT_BOOLEAN_EXPRESSION,T_CL,NT_STMT,NT_OTHER_STMTS,T_ENDWHILE,RULE_END},
	{NT_CONDITIONAL_STMT,T_IF,T_OP,NT_BOOLEAN_EXPRESSION,T_CL,T_THEN,NT_STMT,NT_OTHER_STMTS,NT_ELSE_PART,RULE_END},
	{NT_ELSE_PART,T_ELSE,NT_STMT,NT_OTHER_STMTS,T_ENDIF,RULE_END},
	{NT_ELSE_PART,T_ENDIF,RULE_END},
	{NT_IO_STMT,T_READ,T_OP,NT_SINGLE_OR_RECORDID,T_CL,T_SEM,RULE_END},
	{NT_IO_STMT,T_WRITE,T_OP,NT_ALL_VAR,T_CL,T_SEM,RULE_END},
	{NT_ALL_VAR,T_NUM,RULE_END},
	{NT_ALL_VAR,T_RNUM,RULE_END},
	{NT_ALL_VAR,T_ID,NT_ALL_VAR_PRIME,RULE_END},
	{NT_ALL_VAR_PRIME,T_DOT,T_FIELDID,RULE_END},
	{NT_ALL_VAR_PRIME,T_EPS,RULE_END},
	{NT_ARITHMETIC_EXPRESSION,NT_TERM,NT_EXP_PRIME,RULE_END},
	{NT_EXP_PRIME,NT_LOW_PRECEDENCE_OPERATOR,NT_TERM,NT_EXP_PRIME,RULE_END},
	{NT_EXP_PRIME,T_EPS,RULE_END},
	{NT_TERM,NT_FACTOR,NT_TERM_PRIME,RULE_END},
	{NT_TERM_PRIME,NT_HIGH_PRECEDENCE_OPERATORS,NT_FACTOR,NT_TERM_PRIME,RULE_END},
	{NT_TERM_PRIME,T_EPS,RULE_END},
	{NT_FACTOR,T_OP,NT_ARITHMETIC_EXPRESSION,T_CL,RULE_END},
	{NT_FACTOR,NT_ALL,RULE_END},
	{NT_HIGH_PRECEDENCE_OPERATORS,T_MUL,RULE_END},
	{NT_HIGH_PRECEDENCE_OPERATORS,T_DIV,RULE_END},
	{NT_LOW_PRECEDENCE_OPERATOR,T_PLUS,RULE_END},
	{NT_LOW_PRECEDENCE_OPERATOR,T_MINUS,RULE_END},
	{NT_ALL,T_NUM,RULE_END},
	{NT_ALL,T_RNUM,RULE_END},
	{NT_ALL,T_ID,NT_TEMP,RULE_END},
	{NT_TEMP,T_DOT,T_FIELDID,RULE_END},
	{NT_TEMP,T_EPS,RULE_END},
	{NT_BOOLEAN_EXPRESSION,T_OP,NT_BOOLEAN_EXPRESSION,T_CL,NT_LOGICAL_OP,T_OP,NT_BOOLEAN_EXPRESSION,T_CL,RULE_END},
	{NT_BOOLEAN_EXPRESSION,NT_VAR,NT_RELATIONAL_OP,NT_VAR,RULE_END},
	{NT_BOOLEAN_EXPRESSION,T_NOT,T_OP,NT_BOOLEAN_EXPRESSION,T_CL,RULE_END},
	{NT_VAR,T_ID,RULE_END},
	{NT_VAR,T_NUM,RULE_END},
	{NT_VAR,T_RNUM,RULE_END},
	{NT_RELATIONAL_OP,T_LT,RULE_END},
	{NT_RELATIONAL_OP,T_GT,RULE_END},
	{NT_RELATIONAL_OP,T_LE,RULE_END},
	{NT_RELATIONAL_OP,T_NE,RULE_END},
	{NT_RELATIONAL_OP,T_EQ,RULE_END},
	{NT_RELATIONAL_OP,T_GE,RULE_END},
	{NT_LOGICAL_OP,T_AND,RULE_END},
	{NT_LOGICAL_OP,T_OR,RULE_END},
	{NT_RETURN_STMT,T_RETURN,NT_OPTIONAL_RETURN,T_SEM,RULE_END},
	{NT_OPTIONAL_RETURN,T_SQL,NT_ID_LIST,T_SQR,RULE_END},
	{NT_OPTIONAL_RETURN,T_EPS,RULE_END},
	{NT_ID_LIST,T_ID,NT_MORE_IDS,RULE_END},
	{NT_MORE_IDS,T_COMMA,NT_ID_LIST,RULE_END},
	{NT_MORE_IDS,T_EPS,RULE_END}
};

int nonTerminalOffset = NT_PROGRAM; //For separating the non-terminals with terminals
int RuleList[NUM_NON_TERMINALS][10]; //RuleList[][] maps the non-termianls to the rules in which they occur as the start symbol.
int RevRuleList[NUM_NON_TERMINALS][20]; //It maps the non-termianls which occur in rhs of the rules to their respective rules.
Set First[NUM_NON_TERMINALS];   //Used for storing First sets
Set Follow[NUM_NON_TERMINALS];  //Used for storing the follow sets
int FollowStatus[NUM_NON_TERMINALS];    //Used as a flag while computing the  follow set.(Helps in DFS)
int ParseTable[NUM_NON_TERMINALS][NUM_TERMINALS]; //Stores parse table as a 2-D array.

/*
******************************************************************************************************************
*	errorToken:
*		2-D array of characters. Used for storing strings corresponding to enum types decalared
*		in parserDef.h . It is used while printing the errors after complation. It provides one to one
*		mapping of enum types in the grammar with the strings in the actual grammar.
******************************************************************************************************************
*/
char errorToken[][30]=
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


/*
**********************************************************************
* printGrammar: to print grammar rules
*
**********************************************************************
*/
void printGrammar(){
	int i;
	for(i=0;i<MAX_NUM_RULES;i++){
		int j = 1;
		printf("<%s> ---> ",errorToken[grammar[i][0]]);
		while(grammar[i][j]!=RULE_END){
			if(grammar[i][j]>=nonTerminalOffset){
				printf("<%s> ",errorToken[grammar[i][j]]);
			}
			else{
				printf("%s ",errorToken[grammar[i][j]]);	
			}
			j++;
		}
		printf("\n");
	}
}


/*
*******************************************************************************************************
*	createRuleHash:
*		void : takes no arugments
*	Populates RuleList[][] which a 2-D array which stores hash table(rather direct access table) 
*	which maps the non-termianls to the rules in which they occur as the start symbol (i.e. lhs).
*	Used while computing the first set
*******************************************************************************************************
*/
void createRuleHash(){
	int i;
	memset(RuleList, 0, sizeof(RuleList));
	for(i=0;i<MAX_NUM_RULES;i++){
		//nonTerminalOffset is the starting index of the non-terminal enum
		RuleList[grammar[i][0]-nonTerminalOffset][0]++; //RuleList[x][0] has the number of rules which are of form x-> alpha
		int x = RuleList[grammar[i][0]-nonTerminalOffset][0];
		RuleList[grammar[i][0]-nonTerminalOffset][x] = i;
	}
}

/*
*************************************************************************************************
*	createReverseHash:
*		void : takes no arugments
*	Populates RevRuleList[][] (see ParserDef.h) which a 2-D array which stores 
*	hash table(rather direct access table) which maps the non-termianls which occur in rhs of the
*	rules to their respective rules.
*	Used while computing the follow sets.
*************************************************************************************************
*/
void createReverseHash(){
	int i,j,k;
	memset(RevRuleList,0,sizeof(RevRuleList));
	for(i=0;i<MAX_NUM_RULES;i++){
		j = grammar[i][1];
		k = 2;
		while(j!=RULE_END){
			if(j>=nonTerminalOffset){
				int x = j-nonTerminalOffset;
				RevRuleList[x][0]++; //Element RevRuleList[x][0] has the number of rules in which nt is present.
				RevRuleList[x][RevRuleList[x][0]] = i;	
			}
			j = grammar[i][k];
			k++;
		}
	}
}

/*
*************************************************************************************************	
*	first:
*		nt = non-terminal (or terminal in some cases) whose first set is to be found.
*	Takes Alphabet 'nt' as an input. If alphabet is a non-terminal i.e. nt is a non-terminal
*	then returns the first set of that alphabet. If alphabet is a terminal then it returns
*	empty set.
*	It uses simple divide and conquer algorithm which recusively finds the first set.
*	The stratergy is explained in the comments along the code. 
*************************************************************************************************
*/
Set first(Alphabet nt){
	//Create an empty set
	Set ans = setCreate();

	//nonTerminalOffset is used to check if the alphabet is terminal or non-terminal
	if(nt<nonTerminalOffset)
		return ans;	//Return empty set if it is terminal
	
	int index = nt-nonTerminalOffset; //This gives the index in the hash table RuleList
	int j = RuleList[index][0];	/*Finding the number of rules in the grammar which has 
					nt as their first symbol i.e. grammar rules of form nt -> alpha*/
	int i,x,k;
	int epsFlag=0; // Flag for checking if the rule is of form A->eps
	for(i=1; i<=j; i++){
		x = RuleList[index][i]; // Selecting a grammar rule of form nt -> alpha
		k=1;
		//Looping till the end of the rule
		//selecting alphabet one by one in the lhs of the grammar rule
		while(grammar[x][k]!=RULE_END){
			//Checking whether aplhabet selected is a non-terminal
			if(grammar[x][k]<nonTerminalOffset){
				//Checking if terminal is eps or not
				if(grammar[x][k]==T_EPS)
					epsFlag=1;
				//Adding the terminal to the answer set.
				setInsert(&ans, grammar[x][k]);
				break;
			}
			else{
				//Recursing if the first symbol in the rule nt -> alpha is not
				//a terminal
				Set a = first(grammar[x][k]);
				ans = setUnion(ans, a);

				//If eps is no more in the first then stopping the process we have computed 
				//the first set successfully
				if(!setFind(a,T_EPS)){
					break;
				}
			}
			k++;
		}
	}

	//If there is no rule nt->eps then removing eps which might have come in subsequent computations
	if(!epsFlag)
		setDelete(&ans,T_EPS);

	return ans;
}

/*
******************************************************************************************************************************
*	follow:
*		nt = non-terminal (or terminal in some cases) whose follow set is to be found.
*	Takes Alphabet 'nt' as an input. If alphabet is a non-terminal i.e. nt is a non-terminal
*	then returns the follow set of that alphabet. If alphabet is a terminal then it returns
*	empty set.
*	It uses simple recusive DFS like stratergy in which it searches for the symbols following
*	nt uses first set of that non-terminal. If the non-terminal happens to be before the end of the rule 
*	then the follow set of start symbol of that rule is computed.
*	Incase of recursive rules we use a visiting(or marking) mechanism much similar to DFS.
*	The stratergy is more throughly explained in the comments along with the code. 
*	We have maintained a follow status:
*		FollowStatus[ntIndex] == 1 : Value of follow set of nt has already been computed and stored in Follow[]
*		FollowStatus[ntIndex] == 2 : Value of follow set of nt has been computed parially. Need to considered again 
					     while computing.
******************************************************************************************************************************
*/
Set follow(int nt){
	//Create an empty set
	Set ans = setCreate();

	//nonTerminalOffset is used to check if the alphabet is terminal or non-terminal
	if(nt<nonTerminalOffset)
		return ans;   //Return empty set if it is terminal
	int ntIndex = nt - nonTerminalOffset; //This gives the index in the hash table RevRuleList
	int j = RevRuleList[ntIndex][0];	/*Finding the number of rules in the grammar which has 
						  non-terminal nt in their rhs 
						  i.e. grammar rules of form <some_sybol> -> <alpha> <nt> <beta>
						  or   <some_symbol> -> <alpha> <nt>
						*/
	int i,k,l;
	int isComputed = 1;			//To check if the value of follow set is partially computed
	int alphaIndex;				//The index of alphabet being scanned
	int nextAlphaIndex;			//The index of the next alphabet in the grammar which will be scanned
	if(FollowStatus[ntIndex]==1){
		return Follow[ntIndex];
	}
	if(FollowStatus[ntIndex]==2)
		ans = Follow[ntIndex];
	//The follow set computation is in proccess.
	FollowStatus[ntIndex] = 2;
	for(i=1;i<=j;i++){
		int ruleNum = RevRuleList[ntIndex][i]; 	//For getting each grammar rule one by one
		int alpha = grammar[ruleNum][1];	//The first symbol of the grammar rule
		int nextAlpha = grammar[ruleNum][2];	
		int alphaIndex = 0;			//Index of the first grammar rule symbol
		int nextAlphaIndex = 0;			//Index of the next grammar rule symbol
		if(alpha>=nonTerminalOffset)
			alphaIndex = alpha - nonTerminalOffset;
		if(nextAlpha>=nonTerminalOffset)
			nextAlphaIndex = nextAlpha - nonTerminalOffset;
		k = 2;
		//Scanning the grammar rule specified by ruleNum
		while(alpha!=RULE_END){
			if(alpha==nt){
				//Checking if the symbol next to non-terminal is terminal or not
				if(nextAlpha<nonTerminalOffset && nextAlpha!=RULE_END){
					//inserting the terminal into the follow set of nt
					setInsert(&ans,nextAlpha);
				}
				else{
					l = k+1;
					//Keep adding the first set of non-terminals which follow the non-terminal nt
					//while we get a non=terminal which doesn't have eps in its first set.
					while(nextAlpha>=nonTerminalOffset && setFind(First[nextAlphaIndex],T_EPS)){
						ans = setUnion(ans,First[nextAlphaIndex]);
						setDelete(&ans,T_EPS);
						nextAlpha = grammar[ruleNum][l];
						nextAlphaIndex = nextAlpha - nonTerminalOffset;
						l++;
					}
                    //Handling end cases
					if(nextAlpha<nonTerminalOffset){
						if(nextAlpha!=RULE_END)
							setInsert(&ans,nextAlpha);
					}
					else{

						ans = setUnion(ans,First[nextAlphaIndex]);
					}
				}
                //If nt is followed by no terminal or non-terminal. Then look at follow set of the non-terminal present in
                //lhs of the rule. 
				if(nextAlpha==RULE_END){
						int startingNtIndex = grammar[ruleNum][0] - nonTerminalOffset;
						int tempInt;
						if(FollowStatus[startingNtIndex]==0){
							ans = setUnion(ans,follow(grammar[ruleNum][0]));
						}
						else if(FollowStatus[startingNtIndex]==1){
							ans = setUnion(ans,Follow[startingNtIndex]);
						}
						else{
							ans = setUnion(ans,Follow[startingNtIndex]);
							isComputed = 0;
						}
				 }
			}
			alpha =  grammar[ruleNum][k];
			alphaIndex = alpha - nonTerminalOffset;
			nextAlpha = grammar[ruleNum][k+1];
			nextAlphaIndex = nextAlpha - nonTerminalOffset;
			k++;
		}
	}
	if(isComputed==1)
		FollowStatus[ntIndex] = 1;
	Follow[ntIndex] = ans;
	return ans;
}

/*
**********************************************************************************
*	printAlphabet:
*		alpha = Enum alphabet
*	Prints the enum of the grammar in human readable form.
*	Used while debugging
**********************************************************************************
*/
void printAlphabet(int alpha){
	printf("%s",errorToken[alpha]);
}

/*
******************************************************************************************************************
*	extractSet:
*		s 	 = set of terminals
*		lhsIndex = non-terminal symbol's index at the lhs of the rule
*		ruleNum  = rule number of the rule
*	Takes out the terminal one by one from the set s and then mark the parse table 
*	entries corresponding to the the non-terminal specified by lhsIndex with the rule specified by ruleNum.
*
*****************************************************************************************************************
*/
void extractSet(Set s,int lhsIndex,int ruleNum){
//There are total 57 terminal symbols.So we have used two integer type variables within the set to store them.
//We have avoided using long long because they are not always gauranteed to be 64 bit.
//This code will run perfectly even on a 32bit machine. This has been done to make it downward compatible.
		//Extracting the first 32 bits		
		int first32 = s.ele[0];
		int bitPosition = 0;
		while(first32!=0){
			if(first32&1){
                if(ParseTable[lhsIndex][bitPosition]==-1)
				    ParseTable[lhsIndex][bitPosition] = ruleNum;
			}
			bitPosition++;
			first32 = (first32>>1);
		}
		//Extracting the next 32 bits
		int last32 = s.ele[1];
		bitPosition = 0;
		while(last32!=0){
			if(last32&1){
                if(ParseTable[lhsIndex][32+bitPosition]==-1)
    				ParseTable[lhsIndex][32+bitPosition] = ruleNum;
			}
			bitPosition++;
			last32 = (last32>>1);
		}
}

/*
***********************************************************************************
*	printSet:
*		s 	= Set which contains various terminals
*	Prints all the terminals present in the set 's' (comma separated output)
***********************************************************************************
*/
void printSet(Set s){
//There are total 57 terminal symbols.So we have used two integer type variables within the set to store them.
//We have avoided using long long because they are not always gauranteed to be 64 bit.
//This code will run perfectly even on a 32bit machine. This has been done to make it downward compatible.
	//Extracting the first 32 bits
	int first32 = s.ele[0];
	int bitPosition = 0;
	while(first32!=0){
		if(first32&1){
			printAlphabet(bitPosition);
			printf(",");
		}
		bitPosition++;
		first32 = (first32>>1);
	}
	//Extracting the next 32 bits
	int last32 = s.ele[1];
	bitPosition = 0;
	while(last32!=0){
		if(last32&1){
			printAlphabet(32+bitPosition);
			printf(",");
		}
		bitPosition++;
		last32 = (last32>>1);
	}
}

/*
****************************************************************************************************
*	addSyncFollow: 
*		lhsIndex	= This is the start symbol of a rule
*		rhsIndex	= This is a symbol in the right hand side of a rule
*	This function adds follow set as well as the SYNC set of the lhs symobl of a rule into
*	the SYNC set of the rhsIndex
****************************************************************************************************
*/

void addSyncFollow(int lhsIndex, int rhsIndex){
	int i;
	for(i=0;i<NUM_TERMINALS;i++){
		if(ParseTable[lhsIndex][i]==SYNC){
			if(ParseTable[rhsIndex][i]==-1){
				ParseTable[rhsIndex][i]=SYNC;
			}
		}
	}
	Set s = Follow[lhsIndex];
	int first32 = s.ele[0];
	int bitPosition = 0;
	while(first32!=0){
		if(first32&1){
			if(ParseTable[rhsIndex][bitPosition]==-1)
				ParseTable[rhsIndex][bitPosition] = SYNC;
		}
		bitPosition++;
		first32 = (first32>>1);
	}
	int last32 = s.ele[1];
	bitPosition = 0;
	while(last32!=0){
		if(last32&1){
			if(ParseTable[rhsIndex][32+bitPosition]==-1)
				ParseTable[rhsIndex][32+bitPosition] = SYNC;
		}
		bitPosition++;
		last32 = (last32>>1);
	}
}

/*
******************************************************************************************************
* 	createParseTable:
*		void	: takes no argument
*	This function basically populates the 2-D array ParseTable.
*	This function iterates over all rules and adds the first set if it encounters a non-terminal,
*	follow set if it encounters eps and the terminal itself it it encounters any terminal symbol
*	while scanning each grammar rule left to right.
******************************************************************************************************
*/
void createParseTable(){
	int i,j;
	int lhs,rhs,lhsIndex,rhsIndex;
	//Scanning through all the rules.
	for(i=0;i<MAX_NUM_RULES;i++){
		lhs = grammar[i][0];
		rhs = grammar[i][1];
		//Finding the index correspoding to the lhs of the grammar rule
		lhsIndex = lhs-nonTerminalOffset;
		j=2;
		//Scanning the grammar rule symbol by symbol left to right
		while(rhs!=RULE_END){
			rhsIndex = rhs-nonTerminalOffset;
			//checking if rhs is a non-terminal
			if(rhs<nonTerminalOffset){
				//rhs is not a non-terminal
				if(rhs == T_EPS){
					//if the rule is of form A -> eps
					Set s = Follow[lhsIndex];
					//filling the parse table with rule i for the terminals in the follow set
                    			extractSet(s,lhsIndex,i);
						
				}
				else{
					ParseTable[lhsIndex][rhs] = i;
				}
				break;
			}
			else{
				//rhs is a non-terminal
				Set s = First[rhsIndex];
				if(setFind(s,T_EPS)){
					rhs = grammar[i][j++];
					setDelete(&s,T_EPS);
				}
				else{
					rhs = RULE_END;
				}
				//filling the parse table with rule i for the terminals in the first set
                		extractSet(s,lhsIndex,i);

			}
		}
	}
	for(i=0;i<NUM_NON_TERMINALS;i++){
		Set s = Follow[i];
		//Setting the SYNC entries in the parse table
        	extractSet(s,i,SYNC);
	}
	for(i=0;i<MAX_NUM_RULES;i++){
		lhs = grammar[i][0];
		rhs = grammar[i][1];
		lhsIndex = lhs-nonTerminalOffset;
		j=2;
		while(rhs!=RULE_END){
			rhsIndex = rhs-nonTerminalOffset;
			if(rhs>=nonTerminalOffset){
				addSyncFollow(lhsIndex, rhsIndex);
			}
			rhs = grammar[i][j++];
		}
	}
}

/*
******************************************************************************************************
* 	printParseTable:
*		void	: takes no argument
*	This function prints the parse table present in the 2-D array ParseTable
*	This is a utility function and was used for debugging purpose.
*	One can print the parse table for verbosity.
******************************************************************************************************
*/
void printParseTable(){
	int i,j;
	for(i=0;i<NUM_NON_TERMINALS;i++){
		printAlphabet(i+nonTerminalOffset);
		printf("\n");
		for(j=0;j<NUM_TERMINALS;j++){
			if(ParseTable[i][j]!=-1){
				printf("\t");
				printAlphabet(j);
				printf(": %d\n",ParseTable[i][j]);
			}
		}
	}
}

/*
******************************************************************************************************
* 	printStack:
*		s	= Pointer to the stack.
*	This prints stack elements which was used while parsing.
*	This is a utility function and was used for debugging purpose.
*	One can print the parse stack for verbosity.
******************************************************************************************************
*/
void printStack(Stack* s){
	printf("\nPrinting current stack\n");
	Node* head = s->head;
	while(head!=NULL){
		ParseNode* temp = (ParseNode*)(head->pNode);
		printAlphabet((temp->token)->tokenName);
		printf("\n");
		head = head->next;
	}
}

/*
*****************************************************************************************************
*   createParseNode:
*       tokenName   = This has the token name (enum)
*       parent      = Parent node in the parse tree
*   It creates a new ParseNode* (a node of Parse Tree) in which the child and next nodes are empty but
*   parent node is set. This is used while creating the nodes of the parse tree. 
*****************************************************************************************************
*/
ParseNode* createParseNode(int tokenName,ParseNode* parent){
	PARSER_SIZE += sizeof(ParseNode);
	ParseNode* newNode = (ParseNode*)malloc(sizeof(ParseNode));
	newNode->token = (Token*)malloc(sizeof(Token));
	(newNode->token)->tokenName = tokenName;
	newNode->parent = parent;
	newNode->next = NULL;
	newNode->child = NULL;
	newNode->isLeaf = 0;
	return newNode;
}

/*
*****************************************************************************************************
*   parseInputSourceCode:
*       testcaseFile = This has the name of the source file.
*   This is the main function in this module and is responsible for creation of parse tree as well as
*   parsing the input source code. Most of the code is self explainatory. But we have comments to make
*   it more clear.
*****************************************************************************************************
*/
ParseTree parseInputSourceCode(char *testcaseFile){
	initErrorFlags();   // Initializing the error flags so that respective errors are detected. See error.h for more details.
    FILE* srcFilePtr = open(testcaseFile); // Open function returns the file pointer to the source file. See lexer.h for more details.
    if(FileNotFound){
    	if(strlen(testcaseFile)==0){
    		printError(-1,0,NULL,NULL,NULL);   // Printing error no file specified. See error.h

    	}
    	else
    		printError(0,0,testcaseFile,NULL,NULL); //File path doesn't exists. See error.h
    	return;
    }
    int forward = 0;    // pointers for fetching the next token.
    int curBuffer = 0;  // pointers for fetching the next token.
    int curState = 0;   // pointers for fetching the next token.
    int seePrevChar = 0;    // pointers for fetching the next token.
    char prevChar = '$';    // pointers for fetching the next token.
    int lineNum = 1;    // pointers for fetching the next token.
    int endOfFile = 0;  // pointers for fetching the next token.
    int isFinal = -1;   // pointers for fetching the next token.
    fillBuffer(srcFilePtr,curBuffer);   // filling the buffer for the first time
    Token* lookahead = NULL;    
    Stack* st = stackCreate();  // Stack to be used for pushing the non-terminals and terminals while verifying the source code
    ParseNode* dollarNode = createParseNode(T_DOLLAR,NULL); // Creating Dollar Token
    ParseNode* programNode = createParseNode(NT_PROGRAM,NULL); // Creating the <program> token
    push(st,dollarNode);    //Pushing dollar into the stack
    push(st,programNode);   //Pushing <program> at the top of the stack
    (programNode->token)->lineNum = lineNum;
   	lookahead = getNextToken(srcFilePtr,&isFinal,&forward, 
                             &curBuffer,&prevChar,&seePrevChar,
                             &lineNum,&endOfFile); // Fecthing the first lookahead
   	int stTop;
   	int ruleNum;
   	int ruleLen;
   	int ruleAlpha;
   	ParseNode* curTokenTop;
    while(1){
    	curTokenTop = (ParseNode*)top(st); // Getting the stack top
    	stTop = (curTokenTop->token)->tokenName; //Token Name of the stack top
    	if(stTop==T_DOLLAR && lookahead->tokenName==T_DOLLAR){
    		break;
    	}
    	else if(stTop==T_DOLLAR){
            //If stack is empty before all input symbols were consumed.
    		while(lookahead->tokenName!=T_DOLLAR){
    			lookahead = getNextToken(srcFilePtr,&isFinal,&forward, 
                             		     &curBuffer,&prevChar,&seePrevChar,
                             			 &lineNum,&endOfFile);
    		}
    		break;
    	}
    	else if(stTop<nonTerminalOffset){
            //If stack top is terminal
    		if(lookahead->tokenName==stTop){
                //If lookahead matches with the stack top
    			curTokenTop->token = lookahead;
    			lookahead = getNextToken(srcFilePtr,&isFinal,&forward, 
                             		     &curBuffer,&prevChar,&seePrevChar,
                             			 &lineNum,&endOfFile);
    			curTokenTop->isLeaf = 1;
    			pop(st);
    		}
    		else{
                //If the stack top doesn't match with lookahead print error.
    			printError(-6,lookahead->lineNum,lookahead->lexemeName,
    						errorToken[lookahead->tokenName],errorToken[stTop]);
    			pop(st);

    		}
    	}
    	else if(stTop>=nonTerminalOffset){
            // when stack top is a non-terminal
	    	(curTokenTop->token)->lineNum = lookahead->lineNum;
    		pop(st);    //poping the nonterminal from the stack
    		ruleNum = ParseTable[stTop-nonTerminalOffset][lookahead->tokenName]; //Get the rule number from ParseTable
    		curTokenTop->ruleNum = ruleNum; // Adding rule number to each node
    		if(ruleNum==-1){
                    //If no rule exists and lookahead is not in SYNC set
    				Set s = First[stTop-nonTerminalOffset];
    				setDelete(&s,T_EPS);
    				printError(-6,lookahead->lineNum,lookahead->lexemeName,errorToken[lookahead->tokenName],errorToken[stTop]);
    				printf("\n");
    			    while(ruleNum==-1 && lookahead->tokenName!=T_DOLLAR){    			
    				    lookahead = getNextToken(srcFilePtr,&isFinal,&forward, 
    	                         		     &curBuffer,&prevChar,&seePrevChar,
        	                     			 &lineNum,&endOfFile);
    				    ruleNum = ParseTable[stTop-nonTerminalOffset][lookahead->tokenName];
    			    }
    			    if(ruleNum==SYNC || lookahead->tokenName==T_DOLLAR)
    				    continue;
    		}
    		else if(ruleNum==SYNC){
                    //If the lookahead is in the SYNC set then simply continue after the non-terminal symbol have been poped.
    				printError(-6,lookahead->lineNum,lookahead->lexemeName,errorToken[lookahead->tokenName],errorToken[stTop]);
    			continue;
    		}
    		ruleLen=1;
    		ruleAlpha = grammar[ruleNum][ruleLen];
            //This is done in order to see the grammar rule in reverse order.
    		while(ruleAlpha!=RULE_END){
    			ruleLen++;
    			ruleAlpha = grammar[ruleNum][ruleLen];
    		}
    		ruleLen--;
    		ruleAlpha = grammar[ruleNum][ruleLen];

    		if(ruleLen==1 && ruleAlpha==T_EPS){
                //Case when we have a rule of form A->eps. We just make that node leaf without psuhing them into the stack
    			curTokenTop->child = createParseNode(ruleAlpha,curTokenTop);
    			((curTokenTop->child)->token)->lineNum = lineNum;
    			(curTokenTop->child)->isLeaf=1;
    		}
    		else{
    			ruleLen--;
                //Code below pushes the rules in reverse order and creates the tree children in right order.
    			ParseNode* sibling = createParseNode(ruleAlpha,curTokenTop);
    			push(st,sibling);
    			ruleAlpha = grammar[ruleNum][ruleLen];
    			ParseNode* prevSibling = NULL;
    			sibling->next = prevSibling;
    			prevSibling = sibling;
	    		while(ruleLen>=1){
                    //Add other children in tree
	    			sibling = createParseNode(ruleAlpha,curTokenTop);
	    			sibling->next = prevSibling;
	    			push(st,sibling);
	    			prevSibling = sibling;
	    			ruleLen--;
	    			ruleAlpha = grammar[ruleNum][ruleLen];
	    		}
                //Link the parent node to the first child node
    			curTokenTop->child = sibling;
    		}
    	}
    }
    //Closing the file so that the changes in the file are flush out.
    fclose(srcFilePtr);
    return programNode;
}

/*
*******************************************************************************************************************
*   dfs:
*       pt          =   The parse tree whose dfs traversal is to be printed.
*       outputFile  =   Pointer to the output file where the DFS will be printed.
*   Prints the DFS of the ParseTree pt provided to it.
*******************************************************************************************************************
*/
void dfs(ParseTree pt,FILE* outputFile){

	if(pt==NULL)
		return;
	else{
		Token* token = pt->token;
		if(!(pt->isLeaf)){
			fprintf(outputFile,"%-30s%-7d%-15s%-20s\t%-22sno\t%-26s\n",
				    "----",
					token->lineNum,
					"----",//errorToken[token->tokenName],
					"----",
					(pt->parent!=NULL)? errorToken[((pt->parent)->token)->tokenName] : "ROOT",
					errorToken[token->tokenName]
					);
		}
		else{
			if(token->tokenName==TK_NUM){
				fprintf(outputFile,"%-30s%-7d%-15s%-20d\t%-22syes\t%-26s\n",
						token->lexemeName,
						token->lineNum,
						errorToken[token->tokenName],
						(token->value).intValue,
						errorToken[((pt->parent)->token)->tokenName],
						errorToken[token->tokenName]
						);				
			}
			else if(token->tokenName==TK_RNUM){
				fprintf(outputFile,"%-30s%-7d%-15s%-20f\t%-22syes\t%-26s\n",
						token->lexemeName,
						token->lineNum,
						errorToken[token->tokenName],
						(token->value).realValue,
						errorToken[((pt->parent)->token)->tokenName],
						errorToken[token->tokenName]
						);
			}
			else{
				fprintf(outputFile,"%-30s%-7d%-15s%-20s\t%-22syes\t%-26s\n",
						token->lexemeName,
						token->lineNum,
						errorToken[token->tokenName],
						"----",
						errorToken[((pt->parent)->token)->tokenName],
						errorToken[token->tokenName]
						);
			}
		}
	}
	ParseTree siblings = pt->child;
	while(siblings!=NULL){
		dfs(siblings,outputFile);
		siblings = siblings->next;
	}
}

/*
********************************************************************************************************
*   printParseTree:
*       pt  =   ParseTree which is to be printed.
*       outputFile = String which contains the name of the file in which the DFS output will be printed.
********************************************************************************************************
*/
void printParseTree(ParseTree pt,char* outputFile){
	FILE* filePointer = (strcmp(outputFile,"stdout")==0)?stdout:fopen(outputFile,"w");
	dfs(pt,filePointer);
	if(strcmp(outputFile,"stdout")!=0)
		fclose(filePointer);
}

/*
****************************************************************************************************
*	parser:
*		argv        =   The name of the source file.
*       argv1       =   The name of the output file.
*       printOutput =   This enables printing of dfs.
*	This includes the execution of all the functions which are neccesary before execution of the
*	compiler. It also computes the first and follow sets using the first and follow functions for 
*	all non-terminals and storing them in array First[] and Follow[] 
****************************************************************************************************
*/
ParseTree parser(char* argv,char* argv1,int printOutput){
	int i;
	createRuleHash();
	createReverseHash();
	memset(FollowStatus,0,sizeof(FollowStatus));
	for(i=0;i<NUM_NON_TERMINALS;i++){
		Set s = first(nonTerminalOffset+i);
		First[i] = s;
	}
	for(i=0;i<NUM_NON_TERMINALS;i++){		
		Set s = follow(nonTerminalOffset+i);
		Follow[i] = s;
	}
	memset(ParseTable,-1,sizeof(ParseTable));
	createParseTable();

	ParseTree pt = parseInputSourceCode(argv);
	if(printOutput)
		printParseTree(pt,argv1);
	return pt;
}

/*
****************************************************************************************************
*	printFirst:
*       void : no agrguments
*      Computes and prints the first set of all non-terminals
****************************************************************************************************
*/
void printFirst(){
	createRuleHash();
	createReverseHash();
	int i;
	for(i=0;i<NUM_NON_TERMINALS;i++){
		Set s = first(nonTerminalOffset+i);
		First[i] = s;
		printf("First(%s) = {",errorToken[nonTerminalOffset+i]);
		printSet(s);
		printf("}\n");
	}	
}

/*
****************************************************************************************************
*	followFirst:
*       void : no agrguments
*      Computes and prints the follow set of all non-terminals
****************************************************************************************************
*/
void printFollow(){
	createRuleHash();
	createReverseHash();
	int i;	
	for(i=0;i<NUM_NON_TERMINALS;i++){
		printf("Follow(%s) = {",errorToken[nonTerminalOffset+i]);		
		Set s = follow(nonTerminalOffset+i);
		Follow[i] = s;
		printSet(s);
		printf("}\n");
	}
}



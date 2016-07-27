/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"
#include <stdio.h>

void printGrammar();
/*
*******************************************************************************************************
*   parseInputSourceCode:
*       testcaseFile = This has the name of the source file.
*   This is the main function in this module and is responsible for creation of parse tree as well as
*   parsing the input source code. Most of the code is self explainatory. But we have comments to make
*   it more clear.
*******************************************************************************************************
*/
ParseTree parseInputSourceCode(char *testcaseFile);


/*
********************************************************************************************************
*   printParseTree:
*       pt  =   ParseTree which is to be printed.
*       outputFile = String which contains the name of the file in which the DFS output will be printed.
********************************************************************************************************
*/
void printParseTree(ParseTree pt,char* outputFile);

/*
*****************************************************************************************************
*	parser:
*		argv        =   The name of the source file.
*      	 	argv1       =   The name of the output file.
*       	printOutput =   This enables printing of dfs.
*	This includes the execution of all the functions which are neccesary before execution of the
*	compiler. It also computes the first and follow sets using the first and follow functions for 
*	all non-terminals and storing them in array First[] and Follow[] (See parser.c for more info) 
******************************************************************************************************
*/
ParseTree parser(char* argv,char* argv1,int printOutput);

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
Set first(Alphabet nt);

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
Set follow(int nt);

/*
****************************************************************************************************
*	printFirst:
*       void : no agrguments
*      Computes and prints the first set of all non-terminals
****************************************************************************************************
*/
void printFirst();

/*
****************************************************************************************************
*	followFirst:
*       void : no agrguments
*      Computes and prints the follow set of all non-terminals
****************************************************************************************************
*/
void printFollow();

#endif

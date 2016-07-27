/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include <stdio.h>
#include <string.h>
#include "error.h"


int	FileNotFound = 0;
int	HasErrors = 0;
int	ErrorCount = 0;
int ShowErrors = 1;

/*
******************************************************************************************************************
*	initErrorFlags:
*		This functions sets all the error flags to zero. Has to be called when the parser starts parsing.
******************************************************************************************************************
*/
void initErrorFlags(){
	FileNotFound = 0;
	HasErrors = 0;
	ErrorCount = 0;
}

/*
**********************************************************************************************************
*	printError:
*		errNo 		= The error number which is to be printed.
*		lexemeName 	= lexeme where the error took place.
*		tokenName	= Token which was generated when lexeme was encountered
*		expectedToken	= Token which was expected after error handling. 
*	Prints error correspoding to the error number. Error numbers are taken to negative by convention.
**********************************************************************************************************
*/
void printError(int errNo,int lineNum,char* lexemeName,char* tokenName,char* expectedToken){
	HasErrors=1;
	switch(errNo){
		case 0 : printf("Fatal_Error:Source file <%s> doesn't exist\n",lexemeName);
				 break;
		case -1: printf("Fatal_Error:No source file passed\n");
    			 printf("Use \tparser <path to source file>\n");
    			 break;
		case -2: printf("Error_3: Unknown Pattern <%s> at line %d\n",lexemeName,lineNum);
				 break;
		case -3: printf("Error_2: Unknown Symbol <%c> at line %d\n",lexemeName[strlen(lexemeName)-1],lineNum);
				 break;
		case -4: printf("Error_1: Identifier <%s...> at line %d is longer than the prescribed length of 20 characters\n",lexemeName,lineNum);
				 break;
		case -5: printf("Error_4: Lexeme <%s...> at line %d is too long\n",lexemeName,lineNum);
				 break;
		case -6: printf("Error_5: The token <%s> for lexeme <%s> does not match at line %d. The expected token here is <%s>.\n",tokenName,lexemeName,lineNum,expectedToken);
				 break;
		case -7: printf("Error_6: The token <%s> for lexeme <%s> does not match at line %d. The expected one of the following:",tokenName,lexemeName,lineNum);
				 break;
		default: printf("\nError: Unknown Error!!\n");
	}
}


void printSemanticError(int errNo,int lineNum,char errArguments[][MAX_ERR_LEN]){
	HasErrors = 1;
	if(!ShowErrors){
		return;
	}
	switch(errNo){
		case -8 : printf("Error_7: Record Type %s doesn't exist at line %d\n",errArguments[0],lineNum);
				 break;
		case -9: printf("ERROR_8: Redeclaration of record %s at line %d and previous declaration was at line %s\n",errArguments[0],lineNum,errArguments[1]);
    			 break;
		case -10: printf("ERROR_9: Redeclaration of variable %s at line %d and previous declaration was at line %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -11: printf("ERROR_10: Redeclaration of global variable %s at line %d and previous declaration was at line %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -12: printf("ERROR_11: In the function %s extra return parameters present at line %d\n",errArguments[0],lineNum);
				  break;
		case -13: printf("ERROR_12: In function %s insufficient return parameters are provided at line %d\n",errArguments[0],lineNum);
				  break;
		case -14: printf("ERROR_13: Redeclaration of function %s at line %d and previous declaration at line %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -15: printf("ERROR_14: No such function %s exists at line %d\n",errArguments[0],lineNum);
		          break;
		case -16: printf("ERROR_15: In function %s no recursion is allowed at line %d\n",errArguments[0],lineNum);
				  break;
	    case -17: printf("ERROR_16: Function %s not defined before the callee function %s at line %d\n",errArguments[0],errArguments[1],lineNum);
	    		  break;

		case -18: printf("ERROR_19: In function %s extra output parametes in the function call at line %d\n",errArguments[0],lineNum);
				  break;
		case -19: printf("ERROR_20: In function %s insufficient output parameters are used at line %d\n",errArguments[0],lineNum);
				  break;
		case -20: printf("ERROR_21: In function %s undeclared variable %s at line %d\n",errArguments[0],errArguments[1],lineNum);
				  break;
		case -21: printf("ERROR_22: In function %s output parameter type mismatch at line %d for %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -22: printf("ERROR_23: In function %s extra input parametes used in the function call %s at line %d\n",errArguments[0],errArguments[1],lineNum);
				  break;
		case -23: printf("ERROR_24: In function %s insufficient input parameters used at line %d\n",errArguments[0],lineNum);
		 		  break;
		case -24: printf("ERROR_25: In function %s input parameter type mismatch at line %d  for %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -25: printf("ERROR_26: In function %s field %s is not present in the record type %s at line %d\n",errArguments[0],errArguments[1],errArguments[2],lineNum);
				  break;
		case -26: printf("ERROR_27: in function %s can't Multiply or Divide two records %s and %s at line %d\n",
							errArguments[0],
							errArguments[1],
							errArguments[2],
							lineNum);
				 break;
		case -27: printf("ERROR_28: In function %s we can't divide a scalar by a record at %d\n",errArguments[0],lineNum);
				  break;
		case -28: printf("ERROR_29: In function %s you can't add or subtract a scalar to a record at line %d\n",errArguments[0],lineNum);
				  break;
		case -29: printf("ERROR_31: In function %s record type variable %s cannot be part of boolean expression at line %d\n",
					errArguments[0],
					errArguments[1],
					lineNum);
		          break;
		case -30: printf("ERROR_32: In function %s return parameter type mismatch at line %d for variable %s\n",errArguments[0],lineNum,errArguments[1]);
				  break;
		case -31: printf("ERROR_33: In function %s return parameter %s is not same as output paremeter %s at line %d\n",
									errArguments[0],
									errArguments[1],
									errArguments[2],
									lineNum);
		 		  break;
		case -32: printf("ERROR_34: In function %s return parameter %s is unitialized at line %d\n",
									errArguments[0],
									errArguments[1],
									lineNum);
				  break;
		case -33: printf("ERROR_35: In function %s while loop probably would not terminate at line %d\n",errArguments[0],lineNum);
				  break;		
		case -34: printf("ERROR_36: In function %s varibale %s is not of record type at line %d\n",errArguments[0],errArguments[1],lineNum);
				  break;
		default: printf("\nError: Unknown Error!!\n");
	}
}
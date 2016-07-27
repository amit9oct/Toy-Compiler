/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef ERROR_H
#define ERROR_H
#define MAX_ERR_LEN 30

extern int FileNotFound; // This flag is set to 1 when the input file is not found or doesn't exists.
extern int HasErrors;	// This flag is set to 1 when some compilation error occurs in the code.
extern int ErrorCount;	// This flag is used to count the number of errors occured in program.
extern int ShowErrors; // This flag is used to show errors if set.

void initErrorFlags(); // This functions sets all the error flags to zero. Has to be called when the parser starts parsing.

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
void printError(int errNo,int lineNum,char* lexemeName,char* tokenName,char* expectedToken); 


void printSemanticError(int errNo,int lineNum,char errArguments[][MAX_ERR_LEN]);
#endif

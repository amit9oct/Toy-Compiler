/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef LEXER_H
#define LEXER_H
#include "lexerDef.h"
#include <stdio.h>
#include <stdlib.h>

/*
*---------------------------------------------------------------------------------
*   START FUNCTION DECLARATIONS
*---------------------------------------------------------------------------------
*/

/*
*******************************************************************************************
*   It has the DFA with 54 states (actually 53 one extra for retracting)
*   Gets the next DFA state on the basis of current state and scanned
*   character c.Reads a character from the buffer. Fills the buffer if required. Toggles 
*   the current buffer if the current buffer is full and refills it.(twin buffers)
*   Returns Token of the lexeme which is scanned.
*   Manages the tokenization and assignment of lexemeName and line number to the tokens. 
*******************************************************************************************
*/
Token* getNextToken( FILE* srcFilePtr,int* isFinal,int* forward, 
                    int* curBuffer,char* prevChar,int* seePrevChar,
                    int* lineNum,int* endOfFile);

/*
***********************************************************************************
*   This function is used to open the file in the read only mode.
***********************************************************************************
*/
FILE* open(char* srcFilename);

/*
****************************************************************************
*   Reads MAX_BUFFER_CAPACITY from the source file and appends '\0'
*   when EOF is encountered.
****************************************************************************
*/
void fillBuffer(FILE* srcFilePtr,int curBuffer);


/*
*********************************************************************************
*   Returns the TokenName of the corresponding token generated in the finalState
*********************************************************************************
*/
TokenName getTokenName(int finalState);
/*

/*
*----------------------------------------------------------------------------------
*   END FUNCTION DECLARATIONS
*----------------------------------------------------------------------------------
*/

#endif

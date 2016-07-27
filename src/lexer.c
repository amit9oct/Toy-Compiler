/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include "lexerDef.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
***********************************************************************
* tokenString: Used as a hash to get the human readable form of token.
***********************************************************************
*/
char tokenString[][20]=
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
    "eps",
    "$"
};

/*
*********************************************************************************************
*   installD:
*       lexemeName = Has the lxeme name of the keyword for which Token Name has to looked up.
*   Returns the TokenName of the corresponding to keyword in the lexemeName
*   Returns -1 if lexemeName is not a keyword.
**********************************************************************************************
*/
TokenName installD(char* lexemeName){
    if(strcmp("with",lexemeName)==0){
        return TK_WITH;
    }
    else if(strcmp("parameters",lexemeName)==0){
        return TK_PARAMETERS;
    }
    else if(strcmp("end",lexemeName)==0){
        return TK_END;
    }
    else if(strcmp("while",lexemeName)==0){
        return TK_WHILE;
    }
    else if(strcmp("int",lexemeName)==0){
        return TK_INT;
    }
    else if(strcmp("real",lexemeName)==0){
        return TK_REAL;
    }
    else if(strcmp("type",lexemeName)==0){
        return TK_TYPE;
    }
    else if(strcmp("main",lexemeName)==0){
        return TK_MAIN;
    }
    else if(strcmp("global",lexemeName)==0){
        return TK_GLOBAL;
    }
    else if(strcmp("parameter",lexemeName)==0){
        return TK_PARAMETER;
    }
    else if(strcmp("list",lexemeName)==0){
        return TK_LIST;
    }
    else if(strcmp("input",lexemeName)==0){
        return TK_INPUT;
    }
    else if(strcmp("output",lexemeName)==0){
        return TK_OUTPUT;
    }
    else if(strcmp("endwhile",lexemeName)==0){
        return TK_ENDWHILE;
    }
    else if(strcmp("if",lexemeName)==0){
        return TK_IF;
    }
    else if(strcmp("then",lexemeName)==0){
        return TK_THEN;
    }
    else if(strcmp("endif",lexemeName)==0){
        return TK_ENDIF;
    }
    else if(strcmp("read",lexemeName)==0){
        return TK_READ;
    }            
    else if(strcmp("write",lexemeName)==0){
        return TK_WRITE;
    }    
    else if(strcmp("return",lexemeName)==0){
        return TK_RETURN;
    }
    else if(strcmp("call",lexemeName)==0){
        return TK_CALL;
    }
    else if(strcmp("record",lexemeName)==0){
        return TK_RECORD;
    }
    else if(strcmp("endrecord",lexemeName)==0){
        return TK_ENDRECORD;
    }
    else if(strcmp("else",lexemeName)==0){
        return TK_ELSE;
    }
    else
        return -1;           
}

/*
*********************************************************************************
*   getTokenName:
*       finalState = One of the final states where token can be generated.
*   Returns the TokenName of the corresponding token generated in the finalState
*********************************************************************************
*/
TokenName getTokenName(int finalState){
    switch(finalState){
        case 1: return TK_LT;
        case 2: return TK_LE;
        case 5: return TK_ASSIGNOP;
        case 8: return TK_EQ;
        case 9: return TK_GT;
        case 10: return TK_GE;
        case 13: return TK_NE; 
        case 16: return TK_OR;
        case 19: return TK_AND;
        case 20: return TK_NOT;
        case 21: return TK_COMMENT;
        case 22: return TK_SQR;
        case 23: return TK_SQL;
        case 24: return TK_COLON;
        case 25: return TK_SEM;
        case 26: return TK_DOT;
        case 27: return TK_CL; 
        case 28: return TK_OP;
        case 29: return TK_PLUS;
        case 30: return TK_MINUS;
        case 31: return TK_MUL;
        case 32: return TK_DIV;
        case 33: return TK_NUM;
        case 35: return TK_RNUM;
        case 41: return TK_MAIN;
        case 42: return TK_FUNID;
        case 43: return TK_FUNID; 
        case 45: return TK_ID;
        case 46: return TK_ID;
        case 47: return TK_ID;        
        case 50: //Code for identifier lookup table 
                return TK_FIELDID;
        case 52: return TK_RECORDID; 
        case 53: return TK_COMMA;
        default: return -1;
    }
}

/*
****************************************************************************
*   fillBuffer:
*       srcFilePtr = File pointer of the source file.
*       curBuffer  = The buffer which will be filled.
*   Reads MAX_BUFFER_CAPACITY bytes from the source file and appends '\0'
*   when EOF is encountered.
****************************************************************************
*/
void fillBuffer(FILE* srcFilePtr,int curBuffer){
    //printf("\ncurBuffer = %d\n",curBuffer);
    int bytesRead = fread(buffer.buffer[curBuffer],1,MAX_BUFFER_CAPACITY,srcFilePtr);
    if(bytesRead<MAX_BUFFER_CAPACITY){
        buffer.buffer[curBuffer][bytesRead] = '\0';  
    }
}

/*
********************************************************************************************
*   getNextToken:
*       srcFilePtr     = File pointer of the source file.
*       isFinal        = Pointer to the flag which has the following meaning:
*                           *isFinal ==  -2 means error
*                           *isFinal ==  -1 means token is yet to be generated
*                           *isFinal >=   0 token to be generated
*       forward        = Pointer to the current position of lookahead in buffer. 
*       curBuffer      = Pointer to current buffer number which will be filled.
*       prevChar       = Pointer to the previous character scanned.
*       lineNum        = Pointer to the current line number.
*       *seePrevChar   = 1 -> Use prevChar as first character of lexemeName.
*                        0 -> Don't use prevChar.
*       *endOfFile     = 1 -> Reached end of file no more tokens.
*                        0 -> Yet to read the complete file.
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
                    int* lineNum,int* endOfFile
                   ){
    int longIdentifier=0,longLexeme=0;
    State q = q0;
    int moveForward=1;
    char c;
    *isFinal = -1;
    Token* newToken = (Token*)malloc(sizeof(Token));
    newToken->lineNum = *lineNum;
    int ptr = 0;
    while(*isFinal==-1 && !(*endOfFile)){
        if(*seePrevChar){
            c = *prevChar;
            *seePrevChar = 0;
        }
        else{
            if(*forward<MAX_BUFFER_CAPACITY){
                c = buffer.buffer[*curBuffer][*forward];
            }
            else{
                fillBuffer(srcFilePtr,!(*curBuffer));
                *curBuffer = !(*curBuffer);
                *forward=0;
                c = buffer.buffer[*curBuffer][*forward];
            }
            if(c=='\n'){
                *lineNum = *lineNum + 1;
            }
            *prevChar = c;
            if(c=='\0'){
                *endOfFile = 1;
                c = ' ';
            }
            if(moveForward){
                (*forward)++;
            }
        }
        if(q!=q21 && ptr<MAX_LEXEME_LEN)
            newToken->lexemeName[ptr++] = c;
        if(ptr>=MAX_IDENTIFIER_LEN && (q==q47 || q==q46)){
            longIdentifier=1;
        }
        if(ptr>=MAX_LEXEME_LEN && !longIdentifier){
            longLexeme=1;
        }
        switch(q){
            case q0: switch(c){
                        case '~': q =  q20;
                                  break;
                        case ')': q =  q27;
                                  break;
                        case '(': q =  q28;
                                  break;
                        case '&': q =  q17;
                                  break;
                        case '@': q =  q14;
                                  break;
                        case '>': q =  q9;
                                  break;
                        case '<': q =  q1;
                                  break;
                        case '=': q =  q7;
                                  break;
                        case '!': q =  q12;
                                  break;
                        case '%': q =  q21;
                                  break;
                        case ':': q =  q24;
                                  break;
                        case ';': q =  q25;
                                  break;
                        case '.': q =  q26;
                                  break;
                        case ',': q =  q53;
                                  break;
                        case ']': q =  q22;
                                  break;
                        case '[': q =  q23;
                                  break;
                        case '+': q =  q29;
                                  break;
                        case '-': q =  q30;
                                  break;
                        case '*': q =  q31;
                                  break;
                        case '/': q =  q32;
                                  break;
                        case '_': q =  q37;
                                  break;
                        case '#': q =  q51;
                                  break;
                        case 'b': q = q44;
                                  break;
                        case 'c': q = q44;
                                  break;
                        case 'd': q = q44;
                                  break;
                        default:
                                if(c>='0'&&c<='9'){
                                    q =  q33;
                                    break;
                                }
                                if(c>='a'&& c<='z'){
                                    q =  q50;
                                    break;
                                 }
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    q =  q54;
                                    break;
                                }
                                *isFinal = -3;
                     }
                     break;
            case q1: switch(c){
                        case '=': q =  q2;
                                  break;
                        case '-': q =  q3;
                                  break;
                        default:
                                *isFinal = 1; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 1;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                     }
                     break;
            case q2: switch(c){
                        default:
                                *isFinal = 2;                    
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 2;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                     }
                     break;
            case q3: switch(c){
                        case '-': q =  q4;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;
                     }
                     break;
            case q4: switch(c){
                        case '-': q =  q5;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;
                     }
                     break;
            case q5: switch(c){
                        default:
                                *isFinal = 5; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 5;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                     }
                     break;
            case q7: switch(c){
                        case '=': q =  q8;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                     }
                     break;
            case q8: switch(c){
                        default:
                                *isFinal = 8; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 8;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                             
                    }
                     break;
            case q9: switch(c){
                        case '=': q =  q10;
                                  break;
                        default:
                                *isFinal = 9; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 9;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                                
                     }
                     break;
            case q10: switch(c){
                        default:
                                 *isFinal = 10; 
                               if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 10;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                                
                      }
                     break;
            case q12: switch(c){
                        case '=': q = q13;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                      }
                     break;
            case q13: switch(c){
                        default:
                                *isFinal = 13; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 13;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                                
                      }
                     break;
            case q14: switch(c){
                        case '@': q =  q15;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                      }
                     break;
            case q15: switch(c){
                        case '@': q =  q16;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                      }
                     break;
            case q16: switch(c){
                        default:
                                *isFinal = 16; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 16;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                                
                      }
                     break;
            case q17: switch(c){
                        case '&': q =  q18;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                      }
                     break;
            case q18: switch(c){
                        case '&': q =  q19;
                                  break;
                        default:
                                *isFinal = -2;
                                *seePrevChar = 1;                    
                      }
                     break;
            case q19: switch(c){
                        default:
                                *isFinal = 19; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 19;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                      }
                     break;
            case q20: switch(c){
                        default:
                                *isFinal = 20; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 20;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                                
                      }
                     break;
            case q21: switch(c){
                        //In else case of default return to this state itself
                        default:
                                if(c=='\n'){
                                    *isFinal = 21;
                                    q = q54;
                                    break;
                                }
                                q =  q21;                    
                      }
                     break;
            case q22: switch(c){
                        default:
                                *isFinal = 22;
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 22;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q23: switch(c){
                        default:
                                *isFinal = 23; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 23;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q24: switch(c){
                        default:
                                *isFinal = 24; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 24;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q25: switch(c){
                        default:
                                *isFinal = 25; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 25;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q26: switch(c){
                        default:
                                *isFinal = 26; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 26;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q27: switch(c){
                        default:
                                *isFinal = 27; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 27;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q28: switch(c){
                        default:
                                *isFinal = 28; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 28;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q29: switch(c){
                        default:
                                *isFinal = 29; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 29;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q30: switch(c){
                        default:
                                *isFinal = 30; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 30;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q31: switch(c){
                        default:
                                *isFinal = 31; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 31;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q32: switch(c){
                        default:
                                *isFinal = 32; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 32;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q33: switch(c){
                        default: if(c>='0' && c<='9'){
                                    q =  q33;
                                    break;
                                }
                                 if(c=='.'){
                                    q =  q34;
                                    break;
                                }
                                *isFinal = 33; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 33;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
    //Delimiter and error
                      }
                     break;
            case q34: switch(c){
                        default: if(c>='0' && c<='9'){
                                    q =  q35;
                                    break;
                                }
                                *isFinal = -2;
                                *seePrevChar = 1;
                      }
                     break;
            case q35: switch(c){
                        default: if(c>='0' && c<='9'){
                                    q =  q55;
                                    break;
                                }
                                *isFinal = 35; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 35;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q55: switch(c){
                        default: 
                                *isFinal = 35; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 35;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q37: switch(c){
                        default: if(c=='m'){
                                    q =  q38;
                                    break;
                                }
                                 if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                *isFinal = -2;
                                *seePrevChar = 1;
    //Generate error
                      }
                     break;
            case q38: switch(c){
                        default: if(c=='a'){
                                    q =  q39;
                                    break;
                                }
                                 if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                 if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 42;
                                *seePrevChar = 1;
                      }
                     break;
            case q39: switch(c){
                        default: if(c=='i'){
                                    q =  q40;
                                    break;
                                }
                                 if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                 if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 42;
                                *seePrevChar = 1;
                      }
                     break;
            case q40: switch(c){
                        default: if(c=='n'){
                                    q =  q41;
                                    break;
                                }
                                 if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                 if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 42;
                                *seePrevChar = 1;
                      }
                     break;
            case q41: switch(c){
                        default: if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                 if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 41; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 41;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                      }
                     break;
            case q42: switch(c){
                        default: if( (c>='A' && c<='Z') || (c>='a' && c<='z') ){
                                    q =  q42;
                                    break;
                                }
                                 if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 42; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 42;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                      }
                     break;
            case q43: switch(c){
                        default: if(c>='0' && c<='9'){
                                    q =  q43;
                                    break;
                                }
                                *isFinal = 43; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 43;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q44: switch(c){
                        default: if(c>='2' && c<='7'){
                                    q =  q45;
                                    break;
                                }
                                 if(c>='a' && c<='z'){
                                    q =  q50;
                                    break;
                                }
                                *isFinal = 50;                            
                                *seePrevChar = 1;
                      }
                     break;
            case q45: switch(c){
                        default: if(c=='b' || c=='c' || c=='d'){
                                    q =  q46;
                                    break;
                                }
                                 if( c=='a' || (c>='e' && c<='z') ){
                                    q =  q50;
                                    break;
                                }
                                *isFinal = 45; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 45;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                      }
                     break;
            case q46: switch(c){
                        default: if(c>='2' && c<='7'){
                                    q =  q47;
                                    break;
                                }
                                 if(c=='b' || c=='c' || c=='d'){
                                    q =  q46;
                                    break;
                                }
                                 if( c=='a' || (c>='e' && c<='z') ){
                                    q =  q50;
                                    break;
                                }
                                *isFinal = 46; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 46;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                            
                      }
                     break;
            case q47: switch(c){
                        default: if(c>='2' && c<='7'){
                                    q =  q47;
                                    break;
                                }
                                *isFinal = 47; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 47;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q50: switch(c){
                        default: if( c>='a' && c<='z' ){
                                    q =  q50;
                                    break;
                                  }
                                *isFinal = 50; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 50;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q51: switch(c){
                        default: if( c>='a' && c<='z' ){
                                    q =  q52;
                                    break;
                                }
                                *isFinal = -2;
                                *seePrevChar = 1;
                      }
                     break;
            case q52: switch(c){
                        default: if( c>='a' && c<='z' ){
                                    q =  q52;
                                    break;
                                }
                                *isFinal = 52; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 52;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                                
                      }
                     break;
            case q53: switch(c){
                        default:
                                *isFinal = 53; 
                                if(c==' '||c=='\t'||c=='\n'||c=='\r'){
                                    *isFinal = 53;
                                    q = q54;
                                    break;
                                }
                                *seePrevChar = 1;                    
                      }
                     break;
            case q54:
                    *seePrevChar = 1;
                      q = q0;
                      ptr = 0;
                     break;
        }
    }
    newToken->tokenName = getTokenName(*isFinal);
    if(newToken->tokenName!=TK_COMMENT && (*isFinal)!=-3)
        newToken->lexemeName[ptr-1] = '\0';
    else{
        newToken->lexemeName[ptr] = '\0';
    }
    if(longIdentifier || longLexeme || *isFinal<-1||newToken->tokenName==TK_COMMENT){
        if(longIdentifier){
            printError(-4, newToken->lineNum, newToken->lexemeName,NULL,NULL);
            newToken->tokenName = TK_ID;
        }
        else if(longLexeme){
            printError(-5, newToken->lineNum, newToken->lexemeName,NULL,NULL);
            newToken->tokenName = TK_FIELDID;
        }
        else if(*isFinal<-1 || newToken->tokenName==TK_COMMENT){
            if(*isFinal<-1)
                printError(*isFinal,newToken->lineNum,newToken->lexemeName,NULL,NULL);
            return getNextToken(srcFilePtr,isFinal,forward, 
                                    curBuffer,prevChar,seePrevChar,
                                    lineNum,endOfFile);
        }
    }
    if(newToken->tokenName==TK_FIELDID){
        TokenName isKeyword = installD(newToken->lexemeName);
        if(isKeyword!=-1)
            newToken->tokenName = isKeyword;
    }
    if(newToken->tokenName==TK_NUM)
        newToken->value.intValue = atoi(newToken->lexemeName);
    else if(newToken->tokenName==TK_RNUM)
        newToken->value.realValue = (double)(atof(newToken->lexemeName));
    else{
        newToken->value.intValue = -1;
    }
    if((*isFinal)==-1 && (*endOfFile)){
        strcpy(newToken->lexemeName," ");
        newToken->tokenName = TK_DOLLAR;
    }
    if(c!='\n')
	newToken->lineNum = *lineNum;
    return newToken;
}


/*
*********************************************************************************** 
*   open:
*       srcFilename = File name (string). Should be the complete path to the file.
*   This function is used to open the file in the read only mode.
***********************************************************************************
*/
FILE* open(char* srcFilename){
    FILE* fp = fopen(srcFilename,"r");
    if(fp==NULL){
        FileNotFound = 1;
    }
    return fp;
}

/*
****************************************************************************************
*   printToken:
*       t : Token to be printed in string form
*   Used for debugging purpose. Prints the human readable form of the tokens generated.
****************************************************************************************
*/
void printToken(Token t){
    if(t.tokenName==TK_NUM)
    printf("tokenName=%s lexemeName=%s lineNum=%d intValue=%d\n",
        tokenString[t.tokenName],t.lexemeName,t.lineNum,t.value.intValue);
    else if(t.tokenName==TK_RNUM)
        printf("tokenName=%s lexemeName=%s lineNum=%d realValue=%f\n",
            tokenString[t.tokenName],t.lexemeName,t.lineNum,t.value.realValue);
    else
        printf("tokenName=%s lexemeName=%s lineNum=%d\n",
        tokenString[t.tokenName],t.lexemeName,t.lineNum);
}

/**
* This is a function to generate all the tokens as asked by the driver
**/
int lexer(char* srcFilename){
    //Open the file
    FILE* srcFilePtr = open(srcFilename);
    if(srcFilePtr==NULL){
        printf("Fatal Error: File doesn't exist\n");
        exit(0);
    }
    int forward = 0;
    int curBuffer = 0;
    int curState = 0;
    int seePrevChar = 0;
    char prevChar = '$';
    int lineNum = 1;
    int endOfFile = 0;
    int isFinal = -1;
    fillBuffer(srcFilePtr,curBuffer);
    while(!endOfFile){
        Token* token = getNextToken(srcFilePtr,&isFinal,&forward, 
                                    &curBuffer,&prevChar,&seePrevChar,
                                    &lineNum,&endOfFile);
        //getNextToken(srcFilePtr,&lineNum,&curState,&forward,&curBuffer,&prevChar,&seePrevChar,&endOfFile);
        if(token!=NULL)
           printToken(*token);
    }
    fclose(srcFilePtr);
}


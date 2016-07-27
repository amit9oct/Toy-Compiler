/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef LEXER_DEF_H
#define LEXER_DEF_H
#define MAX_LEXEME_LEN 31
#define MAX_IDENTIFIER_LEN 20
#define MAX_BUFFER_CAPACITY 4096


/*
*---------------------------------------------------------------------------------
*   START STRUCT DECLARATIONS
*----------------------------------------------------------------------------------
*/

/*
*******************************************************************************
*   TokenName: Used to compare two tokens without comparing the lexeme name.
*******************************************************************************
*/
typedef enum tokenname{
    TK_LE,
    TK_ASSIGNOP,
    TK_LT,
    TK_EQ,
    TK_GE,
    TK_GT,
    TK_NE,
    TK_OR,
    TK_AND,
    TK_NOT,
    TK_COMMENT,
    TK_SQR,
    TK_SQL,
    TK_COLON,
    TK_SEM,
    TK_DOT,
    TK_CL,
    TK_OP,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_RNUM,
    TK_NUM,
    TK_MAIN,
    TK_FUNID,
    TK_ID,
    TK_FIELDID,
    TK_RECORDID,
    TK_COMMA,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_INT,
    TK_REAL,
    TK_TYPE,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_EPS,
    TK_DOLLAR
}TokenName;



/*
*****************************************************************************************
*   States: Used to assign names to the States so that they are not accidentally changed
*****************************************************************************************
*/
typedef enum states{
    q0,
    q1,
    q2,
    q3,
    q4,
    q5,
    q6,
    q7,
    q8,
    q9,
    q10,
    q11,
    q12,
    q13,
    q14,
    q15,
    q16,
    q17,
    q18,
    q19,
    q20,
    q21,
    q22,
    q23,
    q24,
    q25,
    q26,
    q27,
    q28,
    q29,
    q30,
    q31,
    q32,
    q33,
    q34,
    q35,
    q36,
    q37,
    q38,
    q39,
    q40,
    q41,
    q42,
    q43,
    q44,
    q45,
    q46,
    q47,
    q48,
    q49,
    q50,
    q51,
    q52,
    q53,
    q54,
    q55
}State;


/*
********************************************************************
*   Value: Used for storing the value of the Number or Real Number
********************************************************************
*/
typedef union val{
    int intValue;
    double realValue;
}Value;

/**
*************************************************************
*   Token:  Structure of token
*************************************************************
*/
typedef struct token{
    char lexemeName[MAX_LEXEME_LEN];
    TokenName tokenName;
    Value value;
    int lineNum;
}Token;

/*
*****************************************************************************
* Buffer:Structure used for storing the scanned input and used for buffering
*****************************************************************************
*/
typedef struct buff{
    char buffer[2][MAX_BUFFER_CAPACITY];
}Buffer;
/*
*---------------------------------------------------------------------------------
*   END STRUCT DECLARATIONS
*----------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------------
*   START GLOBAL DECLARATIONS
*------------------------------------------------------------------------------------
*/

/*
* The Buffer - It is global variable and will be accessed while scanning from buffer.
*/
Buffer buffer;

/*
*------------------------------------------------------------------------------------
*   END GLOBAL DECLARATIONS
*------------------------------------------------------------------------------------
*/
#endif

/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/ 
#ifndef PARSER_DEF_H
#define PARSER_DEF_H
#define MAX_RHS_LEN 20
#define MAX_NUM_RULES 92
#define NUM_NON_TERMINALS 52
#define NUM_TERMINALS 55
#define MAX_ALPHABET 108
#define SYNC -10000
#include "set.h"
#include "lexerDef.h"

/*
********************************************************************************************************
*	ParseNode:
*		This struct is used for storing the nodes in the parse tree.
*		It stores only one child and the rest of the siblings are stored in linked list form
********************************************************************************************************
*/
typedef struct parsenode{
    Token* token;
    int isLeaf;
    int ruleNum;
    struct parsenode* parent;
    struct parsenode* child;
    struct parsenode* next;
}ParseNode;

/*
****************************************************************************************************************
*	ParseTree:
*		Pointer to the node of the parse tree. This is typically used for storing the root of the tree.
****************************************************************************************************************
*/
typedef ParseNode* ParseTree;

/*
*****************************************************************************************************************
*	Alphabet:
*		Has all the alphabets in the grammar defined as enumerated datatype.
*		T_* represents terminals and NT_* represents non-terminals. Certain extra terminal symbols like
*		T_DOLLAR are added to make the implementation more smooth.
*****************************************************************************************************************
*/
typedef enum alphabet{
    T_LE,
    T_ASSIGNOP,
    T_LT,
    T_EQ,
    T_GE,
    T_GT,
    T_NE,
    T_OR,
    T_AND,
    T_NOT,
    T_COMMENT,
    T_SQR,
    T_SQL,
    T_COLON,
    T_SEM,
    T_DOT,
    T_CL,
    T_OP,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_RNUM,
    T_NUM,
    T_MAIN,
    T_FUNID,
    T_ID,
    T_FIELDID,
    T_RECORDID,
    T_COMMA,
    T_WITH,
    T_PARAMETERS,
    T_END,
    T_WHILE,
    T_INT,
    T_REAL,
    T_TYPE,
    T_GLOBAL,
    T_PARAMETER,
    T_LIST,
    T_INPUT,
    T_OUTPUT,
    T_ENDWHILE,
    T_IF,
    T_THEN,
    T_ENDIF,
    T_READ,
    T_WRITE,
    T_RETURN,
    T_CALL,
    T_RECORD,
    T_ENDRECORD,
    T_ELSE,
    T_EPS,
    T_DOLLAR,
    RULE_END,
    NT_PROGRAM,
    NT_OTHER_FUNCTIONS,
    NT_MAIN_FUNCTION,
    NT_STMTS,
    NT_FUNCTION,
    NT_INPUT_PAR,
    NT_OUTPUT_PAR,
    NT_PARAMETER_LIST,
    NT_DATATYPE,
    NT_PRIMITIVE_DATATYPE,
    NT_CONSTRUCTED_DATATYPE,
    NT_REMAINING_LIST,
    NT_TYPE_DEFINITIONS,
    NT_TYPE_DEFINITION,
    NT_FIELD_DEFINITION,
    NT_FIELD_DEFINITIONS,
    NT_MORE_FIELDS,
    NT_GLOBAL_OR_NOT,
    NT_DECLARATIONS,
    NT_DECLARATION,
    NT_OTHER_STMTS,
    NT_STMT,
    NT_ASSIGNMENT_STMT,
    NT_SINGLE_OR_RECORDID,
    NT_NEW_24,
    NT_FUN_CALL_STMT,
    NT_OUTPUT_PARAMETERS,
    NT_INPUT_PARAMETERS,
    NT_ITERATIVE_STMT,
    NT_CONDITIONAL_STMT,
    NT_ELSE_PART,
    NT_IO_STMT,
    NT_ALL_VAR,
    NT_ALL_VAR_PRIME,
    NT_ARITHMETIC_EXPRESSION,
    NT_EXP_PRIME,
    NT_TERM_PRIME,
    NT_ALL,
    NT_TEMP,
    NT_LOW_PRECEDENCE_OPERATOR,
    NT_TERM,
    NT_HIGH_PRECEDENCE_OPERATORS,
    NT_FACTOR,
    NT_OPERATOR,
    NT_BOOLEAN_EXPRESSION,
    NT_VAR,
    NT_LOGICAL_OP,
    NT_RELATIONAL_OP,
    NT_RETURN_STMT,
    NT_OPTIONAL_RETURN,
    NT_ID_LIST,
    NT_MORE_IDS
}Alphabet;

extern int grammar[MAX_NUM_RULES][MAX_RHS_LEN];
extern char errorToken[MAX_ALPHABET][30];
extern int PARSER_SIZE;
#endif

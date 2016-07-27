/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef SET_H
#define SET_H

/*
*************************************************************************************************************************
*	Set:
*		This structure is used to implement set datastructure.
*		The current implementation of set can store at max 64 
*		elements.
*		We have avoided using long long because they are not always gauranteed to be 64 bit.
*		This code will run perfectly even on a 32bit machine. This has been done to make it downward compatible.
**************************************************************************************************************************
*/
typedef struct set{
	int ele[2];
}Set;

void setInsert(Set* s, int element);

void setDelete(Set* s, int element);

Set setCreate();

Set setUnion(Set A, Set B);

Set setIntersection(Set A, Set B);

int setFind(Set s, int element);

#endif

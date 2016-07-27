/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#include "set.h"

void setInsert(Set* s, int element){
	s->ele[element/32] = s->ele[element/32] | (1<<(element%32));
}

void setDelete(Set* s, int element){
	s->ele[element/32] = s->ele[element/32] & (~(1<<(element%32)));
}

Set setCreate(){
	Set s;
	s.ele[0] = s.ele[1] = 0;
	return s;
}

Set setUnion(Set A, Set B){
	A.ele[0] |= B.ele[0];
	A.ele[1] |= B.ele[1];
	return A;
}

Set setIntersection(Set A, Set B){
	A.ele[0] &= B.ele[0];
	A.ele[1] &= B.ele[1];
	return A;
}

int setFind(Set s, int element){
	return (s.ele[element/32]>>(element%32)) & 1; 
}

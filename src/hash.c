/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/ 
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>


HashTable* createHashTable(int size,double alpha,HashFunction hash,Comparator compare){
	HashTable* htPtr = (HashTable*)malloc(sizeof(HashTable));
	htPtr->size = size;
	htPtr->table = (Entry*)calloc(size,sizeof(Entry));
	htPtr->elem = 0;
	htPtr->alpha = alpha;
	htPtr->hash = hash;
	htPtr->compare = compare;
	return htPtr;
}

void put(HashTable** htPtrPtr,void* value){
	HashTable* htPtr = *htPtrPtr;
	HashTable ht = *htPtr;
	int key = ht.hash(value, ht.size)%ht.size;
	if(key<0)
		key+=ht.size;
	void* val = ht.table[key].value;
	if(get(&ht,value)!=NULL){
		return;
	}
	if(((double)(ht.elem+1)/ht.size)>ht.alpha){
		*htPtrPtr = rehash(htPtr);
		htPtr = *htPtrPtr;
		ht = *htPtr;
	}
	htPtr->elem = htPtr->elem + 1;
	if(val==NULL){
		ht.table[key].value = value;
		ht.table[key].next = NULL;
		ht.table[key].key = key;
	}
	else{
		Entry* ent = &(ht.table[key]);
		while(ent->next!=NULL){
			ent = ent->next;
		}
		ent->next = (Entry*)calloc(1,sizeof(Entry));
		(ent->next)->value = value;
		(ent->next)->key = key;
	}
}

void* get(HashTable* htPtr,void* value){
	HashTable ht = *htPtr;
	if(value==NULL)
		return NULL;
	int key = ht.hash(value, ht.size)%ht.size;
	void* val = ht.table[key].value;
	if(val==NULL){
		return NULL;
	}
	else{
		Entry* ent = &(ht.table[key]);
		while(ent!=NULL){
			if(ht.compare(ent->value,value)){
				return ent->value;
			}
			ent = ent->next;
		}
		return NULL;
	}
}

void delete(HashTable* htPtr,void* value){
	HashTable ht = *htPtr;
	if(get(&ht,value)==NULL){
		return;
	}
	else{
		htPtr->elem = htPtr->elem - 1;
		int key = ht.hash(value, ht.size);
		Entry* ent = &(ht.table[key]);
		Entry* prev = NULL;
		while(ent!=NULL && !ht.compare(ent->value,value)){
			prev = ent;
			ent = ent->next;
		}
		if(ent==NULL)
			return;
		else{
			if(prev!=NULL){
				prev->next = ent->next;
				free(ent);
			}
			else{
				if(ent->next!=NULL)
					ht.table[key] = *(ent->next);
				else
					ht.table[key].value = NULL;
			}
		}
	}
}

HashTable* rehash(HashTable* htPtr){
	int size = htPtr->size*2;
	while(((double)htPtr->elem+1)/size>htPtr->alpha){
		size = size*2;
	}
	HashTable* newHtPtr = createHashTable(size,htPtr->alpha,htPtr->hash,htPtr->compare);
	newHtPtr->elem = 0;
	int i;
	for(i=0;i<htPtr->size;i++){
		Entry* ent = &(htPtr->table[i]);
		if(ent->value!=NULL){
			put(&newHtPtr,ent->value);
		}
		ent = ent->next;
		while(ent!=NULL){
			if(ent->value!=NULL){
				put(&newHtPtr,ent->value);
			}
			Entry* temp = ent;
			ent = ent->next;
			free(temp);
		}
	}
	free(htPtr->table);
	free(htPtr);
	return newHtPtr;
}

/*
**************************************
*  Batch Number: 9
*  Amitayush Thakur 2012B4A7819P
**************************************
*/
#ifndef HASH_H
#define HASH_H

typedef int (*HashFunction)(void* value,int size);
typedef int (*Comparator)(void* value1,void* value2);

typedef struct entry{
	int key;
	void* value;
	struct entry* next;
}Entry;

typedef struct hashtable{
	Entry* table;
	int size;
	int elem; // Number of elements
	double alpha;
	HashFunction hash;
	Comparator compare;
}HashTable;



HashTable* createHashTable(int size,double alpha,HashFunction hash,Comparator compare);
void put(HashTable** ht,void* value);
void* get(HashTable* ht,void* value);
void delete(HashTable* ht,void* value);
HashTable* rehash(HashTable* ht);

#endif
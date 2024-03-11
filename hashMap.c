#include <stdlib.h>
#include "hashMap.h"
#include "structs.h"
#include <string.h>

char* my_strdup(const char* s) {
    char* newStr = malloc(strlen(s) + 1);
    if (newStr) strcpy(newStr, s);
    return newStr;
}

int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between 1 and 2 is on this line*/
	return r;
}

void initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

void freeMap (struct hashMap * ht)
{
    struct hashLink *current, *temp;
    int i;
    for(i = 0; i < ht->tableSize; i++) {
        current = ht->table[i];
        while(current != NULL) {
            temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(ht->table);
    ht->table = NULL;
    ht->count = 0;
}

void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
    int bucket;
    struct hashLink *current, *newLink;

    bucket = stringHash1(k) % ht->tableSize;
    if(bucket < 0) bucket += ht->tableSize;

    for(current = ht->table[bucket]; current; current = current->next) {
        if(strcmp(current->key, k) == 0) {
            current->value = v;
            return;
        }
    }

    newLink = (struct hashLink *)malloc(sizeof(struct hashLink));
    if (!newLink) return;
    newLink->key = my_strdup(k); 
    newLink->value = v;
    newLink->next = ht->table[bucket];
    ht->table[bucket] = newLink;
    ht->count++;
}

ValueType* atMap (struct hashMap * ht, KeyType k)
{
	int bucket = stringHash1(k) % ht->tableSize;
	struct hashLink *current = ht->table[bucket];
	if(bucket < 0) bucket += ht->tableSize;
	while(current != NULL) {
		if(strcmp(current->key, k) == 0) {
			return &(current->value);
		}
		current = current->next;
	}
	return NULL;

}

int containsKey (struct hashMap * ht, KeyType k)
{
	int bucket = stringHash1(k) % ht->tableSize; 
    struct hashLink *current = ht->table[bucket]; 
    while(current != NULL) {
        if(strcmp(current->key, k) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;

}

void removeKey (struct hashMap * ht, KeyType k)
{
    int bucket = stringHash1(k) % ht->tableSize;
    struct hashLink *current = ht->table[bucket], *previous = NULL;
    if(bucket < 0) bucket += ht->tableSize;

    while (current != NULL) {
        if(strcmp(current->key, k) == 0) {
            if(previous == NULL) {
                ht->table[bucket] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->key);
            free(current);
            ht->count--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

int sizeMap (struct hashMap *ht)
{
	return ht->count;
}

int capacityMap(struct hashMap *ht)
{
	return ht->tableSize;
}

int emptyBuckets(struct hashMap *ht)
{
	int emptyCount = 0;
	int i;

	for(i = 0; i < ht->tableSize; i++) {
		if(ht->table[i] == NULL) {
			emptyCount++;
		}
	}

	return emptyCount;
}

float tableLoad(struct hashMap *ht)
{
	return (float)ht->count / (float)ht->tableSize;
}

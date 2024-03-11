#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "hashMap.h"

char* getWord(FILE *file); /* prototype */


/****************************************/

int main (int argc, const char * argv[]) {
    const char* filename;
    struct hashMap hashTable;
    struct hashLink *temp;
    int tableSize = 1000;
    FILE *fileptr;
    struct timeval stop, start;
    char* word;
    int i;

    
    if(argc == 2)
        filename = argv[1];
    else
        filename = "input.txt";
    
    printf("opening file: %s\n", filename);

    gettimeofday(&start, NULL);
    

    initMap(&hashTable, tableSize);


    fileptr = fopen(filename, "r");
    if (!fileptr) {
        printf("Error opening file.\n");
        return -1;
    }



    while ((word = getWord(fileptr)) != NULL) {
        int* value = atMap(&hashTable, word);
        if (value) {
            (*value)++;
        } else {
            insertMap(&hashTable, word, 1);
        }
        free(word);
    }


    fclose(fileptr);


    for(i = 0; i < hashTable.tableSize; i++) {
        temp = hashTable.table[i];
        while(temp != NULL) {
            printf("%s:%d\n", temp->key, temp->value);
            temp = temp->next;
        }
    }

    gettimeofday(&stop, NULL);
    printf("\nYour execution time for concordance is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

    printf("Table emptyBuckets = %d\n", emptyBuckets(&hashTable));
    printf("Table size = %d\n", sizeMap(&hashTable));
    printf("Table capacity = %d\n", capacityMap(&hashTable));
    printf("Table load = %f\n", tableLoad(&hashTable));


    freeMap(&hashTable);

    return 0;
}


/* ****************************
 getWord takes a FILE pointer and returns a string which was
 the next word in the file. Words are defined (by this function) to be
 characters or numbers separated by periods, spaces, or newlines.
 
 When there are no more words in the input file this function will return NULL.
 
 getWord will malloc some memory for the char* it returns. 
 Free this memory when you no longer need it.
*/
char* getWord(FILE *file)
{
	
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = (char*)malloc(sizeof(char) * maxLength);
	assert(word != NULL);
    
	while( (character = fgetc(file)) != EOF)
	{
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   character == 39) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
    
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}
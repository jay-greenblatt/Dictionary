
#include "hashtable.h"
#include <stdlib.h>

/*
 * creates a hashtable
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for (i = 0; i < size; ++i) {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a bit of data and key into a hashtable.  To use this
 * to store strings, simply cast a char * to a void *.  EG, to store
 * the string refered to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string);
 * if you wanted to use the string both as data and as the key
 * (such as in the philspel project)
 */
void insertData(HashTable *table, void *key, void *data) {
  // HINT:
  // 1. find the right hash bucket location with table->hashFunction.
  // 2. allocate a new hash bucket struct.
  // 3. add to the head of the hash bucket list.
  unsigned int hash = table->hashFunction(key);   // finding the index of where to put the new data //
  char *stringtemp = (char *) key;
  int k = 0;
  while (stringtemp[k] != '\0') {
    k++;
  }
  char *newkey = malloc(sizeof(char) * (k + 1));
  for (int i = 0; i < k; i++) {
      newkey[i] = stringtemp[i];
  }
  newkey[k] = '\0';
  if (table->data[hash] == NULL) {   // if the index has no bucket currently there //
      table->data[hash] = malloc(sizeof(struct HashBucket));
      table->data[hash]->key = table->data[hash]->data = newkey;
      table->data[hash]->next = NULL;
  } else {
      struct HashBucket *buck =  malloc(sizeof(struct HashBucket));
      buck->data = buck->key = newkey;
      buck->next = malloc(sizeof(struct HashBucket));
      buck->next = table->data[hash];
      table->data[hash] = buck;
  }
}

/*
 * This takes a key and returns the corresponding data to that key,
 * or NULL if the key was not found.
 */
void *findData(HashTable *table, void *key) {
  // HINT:
  // 1. find the right hash bucket with table->hashFunction.
  // 2. walk the bucket list and check equality with table->equalFunction.
  unsigned int hash = table->hashFunction(key);
  char *strtemp = (char *) key;
  if (table->data[hash] == NULL) {   //checking that the appropriate bucket has something //
    return NULL;
  } else {
      struct HashBucket *dptr = table->data[hash];
      while (dptr != NULL) {
          if (table->equalFunction(dptr->data, strtemp)) {
              return dptr->data;
          }
          dptr = dptr->next;
      }
  }
  return NULL;
}

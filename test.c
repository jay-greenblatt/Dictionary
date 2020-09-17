#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include <string.h>
#include <ctype.h>

HashTable *dict;

unsigned int stringHash(void *s) {
  char *string = (char *)s;
  unsigned int hash = 0;
  while (*(string++) != '\0') {
      hash = *string + (hash << 8);
  }
  return hash % 2255;
}


int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  fprintf(stderr, "Comparing %s against %s\n", string1, string2);
  while (*string1++ == *string2++) {
    if (*string1 == '\0') {
      fprintf(stderr, "Equal\n");
      return 1;
    }
  }
  fprintf(stderr, "Not equal\n");
  return 0;
}

void hashTester1(HashTable *table) {
  printf("%s\n", "Running hashTester1:");
  char *s1 = "str";
  char *s2 = "str";
  printf("String 1: %s\n", s1);
  printf("String 2: %s\n", s2);
  unsigned int hash1 = table->hashFunction(s1);
  unsigned int hash2 = table->hashFunction(s2);
  printf("String 1 Hash: %d\n", hash1);
  printf("String 2 Hash: %d\n", hash2);
  if (stringEquals(s1, s2)) {
    printf("s1 == s2\n");
  } else {
    printf("s1 != s2\n");
  }
  if (stringEquals(s1, "dog")) {
    printf("s1 == dog\n");
  } else {
    printf("s1 != dog\n");
  }
  insertData(table, s1, s1);
  struct HashBucket *dataptr1;
  dataptr1 = findData(table, s1);
  if (dataptr1 == NULL) {
    printf("%s\n", "findData failed");
  } else {
    printf("%s\n", dataptr1->key);
    printf("%s\n", "findData succeeded for String 1");
  }
  table->data[hash1] = NULL;
  printf("%s", "\n");
}

void insertTester1(HashTable *table) {
    printf("%s\n", "Running insertTester1:");
    struct HashBucket *buck1 = malloc(sizeof(struct HashBucket));
    struct HashBucket *buck2 = malloc(sizeof(struct HashBucket));
    buck1->key = buck1->data = "buckone";
    buck2->key = buck2->data = "bucktwo";
    buck1->next = buck2->next = NULL;
    table->data[0] = buck1;
    printf("Inserted: %s\n", table->data[0]->key);
    buck2->next = table->data[0];
    table->data[0] = buck2;
    printf("Inserted: %s\n", table->data[0]->key);
    printf("Bucktwo->next->key: %s\n", table->data[0]->next->key);
    if (table->data[0]->next->next == NULL) {
      printf("buckone->next is still NULL");
    }
    table->data[0] = NULL;
    free(buck1);
    free(buck2);
}

void readDictionary(char *filename) {
  FILE * fileptr;
  fileptr = fopen(filename, "r");
  if (fileptr == NULL) {  // if file does not exist //
      fprintf(stderr, "File does not exist.\n");
      exit(0);
  } else {
    int c;               // the char variable for getting the next char in the file //
    char *stringptr;    // the array of characters to be inserted //
    stringptr = (char *) malloc(sizeof(char) * 60);   // to hold the string //
    if (stringptr == NULL) {                  // is stringptr allocated? //
      fprintf(stderr, "Not enough memory.\n");
      exit(0);
    }
    int strsize = 0; // size of the stringptr //
    while ((c = fgetc(fileptr)) != EOF) {     // checking that you're not at the end of the file//
      if (isspace(c) && strsize > 0) { // if there is a whtiespace -> store word in the dictionary //
          stringptr[strsize] = '\0';    // terminate the character array //
          insertData(dict, stringptr, stringptr); // insert the data into the dictionary as both key and data //
          strsize = 0;
      } else if (isalpha(c)){ // else add the character to stringptr //
          *(stringptr + strsize) = c; //set the next index to c //
          strsize++;    // increment strsize //
      }
    }
    free(stringptr); // get rid of stringptr //
  }
}


void main() {
  dict = createHashTable(2255, &stringHash, &stringEquals);
  readDictionary("sampleDictionary");
  char * strtemp = (char *) dict->data[186]->key;
  fprintf(stderr, "dict->data[186]->key = %s\n", strtemp);
  char *nick = malloc(sizeof(char) * 5);
  char *nick2 = malloc(sizeof(char) * 5);
  nick[0] = 'n';
  nick2[0] = 'N';
  nick[1] = nick2[1] = 'i';
  nick[2] = nick2[2] = 'c';
  nick[3] = nick2[3] = 'k';
  nick[4] = nick2[4] = '\0';
  char *str = (char *) findData(dict, nick);
  fprintf(stderr, "\n\nFound %s in dict\n\n", str);
  if (findData(dict, nick) != NULL) {
    fprintf(stderr, "nick found\n");
  } else {
    fprintf(stderr, "we good\n");
  }
  if (findData(dict, nick2) != NULL) {
    fprintf(stderr, "Nick found\n");
  } else {
    fprintf(stderr, "you messed up\n");
  }
  free(nick);
  free(nick2);
  free(dict);
}

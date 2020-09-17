/*
 * Include the provided hashtable library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * this hashtable stores the dictionary.
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process, in the same way which this does.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");
  fprintf(stderr, "Processing stdin\n");
  processInput();

  /* main in C should always return 0 as a way of telling
     whatever program invoked this that everything went OK
     */
  return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  unsigned int hash = 0;
  int c = *string;
  int i = 1;
  while (c != '\0') {
      hash = *string + (hash << 8);
      c = *(string + i);
      i++;
  }
  return hash % dictionary->size;
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  while (*string1++ == *string2++) {
    if (*string1 == '\0' && *string2 == '\0') {
      return 1;
    }
  }
  return 0;
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with returns in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
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
          insertData(dictionary, stringptr, stringptr); // insert the data into the dictionary as both key and data //
          strsize = 0;
      } else if (isalpha(c)){ // else add the character to stringptr //
          *(stringptr + strsize) = c; //set the next index to c //
          strsize++;    // increment strsize //
      }
    }
    free(stringptr); // get rid of stringptr //
  }
}

/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".  All words should be checked
 * against the dictionary as they are input, again with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the
 * error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 60 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 60 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */
void processInput() {
  int ch;                // the character variable to be stored //
  int lastcharalp = 0;    // boolean to check if the last char was an alphabetical //
  int lastcharnum = 0;    // boolean to check if the last char was not a whitespace and not alphabetical //
  int newstr = 1;         // boolean to check if we are starting a new string //
  int mallocsize = 60;
  char *strnormal = (char *) malloc(sizeof(char) * (mallocsize + 1));          // character pointer holding the normal inputted string //
  if (strnormal == NULL) {                               // is stringptr allocated? //
    fprintf(stderr, "Not enough memory.\n");
    exit(0);
  }
  int stringsize = 0;         // keeping track of how many chars are in the current array //
  char *sic = " [sic]";       // constant string to be appended to a word not in the dictionary //
  while ((ch = fgetc(stdin)) != EOF) {     // looping until the end of file //
      if (newstr) {                 // if ch is the first char in a string //
        if (!isspace(ch)) {          // if ch is a space //               // keep going //
          if (isalpha(ch)) {      // if ch is a character //
              lastcharalp = 1;      // set lastcharalp //
          } else {                // else set lastcharnum //
              lastcharnum = 1;
          }
            *strnormal = ch;    // first char in strnormal becomes ch //
            stringsize++;         // increment stringsize //
            newstr = 0;           // not starting a new string anymore //
        }
      } else if (!isalpha(ch) && lastcharalp) {  // if the character found is not alphabetical and the last one was //
          *(strnormal + stringsize) = '\0';
          if (findData(dictionary, strnormal) == NULL) { // if the regular string is not found //
              for (int i = 1; i < stringsize; i++) {
                *(strnormal + i) = tolower(*(strnormal+i)); // copying lowercase letters //
              }
              if (findData(dictionary, strnormal) == NULL) {       // if all but first lowercase version not found //
                  *strnormal = tolower(*strnormal);
                  if (findData(dictionary, strnormal) == NULL) {   // if all but first lowercase not found //
                      fputs(sic, stdout);         // append " [sic]" //
                  }
              }
          }
          if (mallocsize > 60) {
            mallocsize = 60;
            strnormal = realloc(strnormal, sizeof(char)*(mallocsize+1));
          }
          lastcharalp = stringsize = 0;      // reset lastcharalp and stringsize //
          newstr = 1;               // starting new string //
      } else if (isalpha(ch) && lastcharnum) {  // else if the end of a non-alphabetic string //
          lastcharnum = 0;
          if (mallocsize > 60) {
            mallocsize = 60;
            strnormal = realloc(strnormal, sizeof(char)*(mallocsize+1));
          }
          *strnormal = ch;              // store first character //
          lastcharalp = stringsize = 1;    // last character is alphabetical //
      } else if (lastcharalp) {    // if the current string is not done yet //
          if (stringsize == mallocsize) {
            mallocsize *= 2;
            char *temp = realloc(strnormal, sizeof(char)*(mallocsize + 1));
            if (temp == NULL) {
              fprintf(stderr, "Not enough memory.\n");
              exit(0);
            }
            strnormal = temp;
          }
          *(strnormal + stringsize++) = ch;           // add the character to strnormal at index stringsize, increment stringsize //
      }
      fputc(ch, stdout);
  }
  if (lastcharalp) {
    *(strnormal + stringsize) = '\0';
    if (findData(dictionary, strnormal) == NULL) { // if the regular string is not found //
        for (int i = 1; i < stringsize; i++) {
          *(strnormal+i) = tolower(*(strnormal+i)); // copying lowercase letters //
        }
        if (findData(dictionary, strnormal) == NULL) {       // if all but first lowercase version not found //
            *strnormal = tolower(*strnormal);
            if (findData(dictionary, strnormal) == NULL) {   // if all but first lowercase not found //
                fputs(sic, stdout);         // append " [sic]" //
            }
        }
    }
  }
  free(strnormal);  // finally finish by freeing strnormal //
}

/* 
 * memory - count_malloc and related functions 
 * 
 * 1. Replacements for malloc(), calloc(), and free(),
 *    that count the number of calls to each,
 *    so you can print reports about the current balance of memory.
 * 
 * 2. Variants that 'assert' the result is non-NULL;
 *    if NULL occurs, kick out an error and die.
 *
 * David Kotz, April 2016, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

/**************** file-local global variables ****************/
// track malloc and free across *all* calls within this program.
static int nmalloc = 0;	   // number of successful malloc calls
static int nfree = 0;	   // number of free calls
static int nfreenull = 0;  // number of free(NULL) calls


/**************** assertp ****************/
/* if pointer p is NULL, print error message and die,
 * otherwise, return p unchanged.
 */
void *
assertp(void *p, const char *message)
{
  if (p == NULL) {
    fprintf(stderr, "MALLOC FAILED: %s\n", message);
    exit (99);
  }
  return p;
}

/**************** count_malloc_assert() ****************/
/* Like malloc() but track the number of successful allocations,
 * and, if response is NULL, print error and die.
 * Unlike malloc(), it takes a 'message' parameter.
 */
void *
count_malloc_assert(size_t size, const char *message)
{
  void *ptr = assertp(malloc(size), message);
  nmalloc++;
  return ptr;
}


/**************** count_malloc() ****************/
/* Just like malloc() but track the number of successful allocations
 */
void *
count_malloc(size_t size)
{
  void *ptr = malloc(size);
  if (ptr != NULL) {
    nmalloc++;
  }
  return ptr;
}

/**************** count_calloc_assert() ****************/
/* Just like calloc() but track the number of successful allocations
 */
void *
count_calloc_assert(size_t nmemb, size_t size, const char *message)
{
  void *ptr = assertp(calloc(nmemb, size), message);
  nmalloc++;
  return ptr;
}

/**************** count_calloc() ****************/
/* Just like calloc() but track the number of successful allocations
 */
void *
count_calloc(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  if (ptr != NULL) {
    nmalloc++;
  }
  return ptr;
}

/**************** allocate() ****************/
/* my custom version of malloc that does automatic error-catching, as well as
 * memory clearing/initialization.
 */
void *allocate(size_t size) {
  void *tmp = malloc(size);
  if(!tmp) {
    printf("Malloc Failed\n");
    exit(2);
  }
  nmalloc++;
  memset(tmp, 0, size);
  return tmp;
}

/**************** callocate() ****************/
/* my custom version of calloc that does automatic error-catching, as well as
 * memory clearing/initialization.
 */
void *callocate(size_t nmemb, size_t size) {
  void *tmp = calloc(nmemb, size);
  if(!tmp) {
    printf("Calloc Failed\n");
    exit(2);
  }
  memset(tmp, 0, nmemb*size);
  nmalloc++;
  return tmp;
}

/**************** count_free() ****************/
/* Just like free() but track the number of calls
 * PERSONAL BILL TANG EDIT: prints out the "free#" of NULL-arg'd count_frees
 * to make error-checking easy.
 */
void 
count_free(void *ptr)
{
  if (ptr != NULL) {
    free(ptr);
    nfree++;
  } else {
    // it's an error to call free(NULL)!
    nfreenull++;
    printf("Freeing Null at Free#: %d", nfree);
  }
}

/**************** count_report() ****************/
/* report the current malloc/free counts
 */
void 
count_report(FILE *fp, const char *message)
{
  fprintf(fp, "%s: %d malloc, %d free, %d free(NULL), %d net\n", 
	  message, nmalloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
}

/**************** count_net() ****************/
/* report the current net malloc-free counts
 * returns positive if there are unfreed allocations,
 * returns negative if there were more free's than alloc's (!),
 * returns zero if they balance.
 */
int
count_net(void)
{
  return nmalloc - nfree - nfreenull;
}

/**************** strdup *********************/
char *strdupp(char* str){
    char* copy = allocate(strlen(str)+1);
    strcpy(copy, str);
    return copy;
}
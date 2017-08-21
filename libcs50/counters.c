/* 
 * counters.c - CS50 'counter' module 
 *
 * see counters.h for more information
 *
 * Bill Tang, July 2017
 * 
 * NOTE: Because I implemented the Counter Module as a linked-list, the majority
 * of this code was either based on, inspired by, or directly taken from bag.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"
#include <string.h>

/**************** local types ****************/
typedef struct counter {
  int key;              //Key
  int count;            //Counter
  struct counter *next; //pointer link this to next item
} counter_t;
/**************** global types ****************/
typedef struct counters {
  counter_t *head; //Gives the next counter
} counters_t;

 /*Create a new (empty) counterset*/
counters_t *counters_new(void) {
  counters_t *tmp = allocate(sizeof(counters_t)); //Allocates Memory
  return tmp;
}

/**************** local functions ****************/
/* not visible outside this file */
static counter_t *counter_new(const int key, int count);

/**************** counter_new ****************/
/* Allocate and initialize a counter */
static counter_t *counter_new(const int key, int count) {
  counter_t *tmp = allocate(sizeof(counter_t));
  tmp->count = count;
  tmp->key = key;
  tmp->next = NULL;
  return tmp;
}

/* Increment the counter indicated by key.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Ignore if ctrs is NULL or key is negative.
 * Return the new value of the counter related to the inserted key
 */
int counters_add(counters_t *ctrs, const int key) {
  if(key >= 0 && ctrs){
    //counters_set provides all necessary functionality to deal with any error
    counters_set(ctrs, key, (counters_get(ctrs, key) + 1));
    return counters_get(ctrs, key);
  } else {
    return 0;
  }
}

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key) {
  if(ctrs && key >= 0){
    //load ctrs up
    if(ctrs->head == NULL){
      return 0;
    } 
    counter_t *currnode = ctrs->head;
    //search for key
    while(currnode->key != key){
      if(currnode->next == NULL){
        //key not found
        return 0;
      }
      currnode = currnode->next;
    } //key found
    return currnode->count;
  } 
  return 0;
}

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */
void counters_set(counters_t *ctrs, const int key, int count){
  if(ctrs && key >= 0 && count >=0){ // check args

    //If ctrs is literally empty, create our first Counter
    if(ctrs->head == NULL){
      counter_t *tmpItem = counter_new(key, count); //initialize pair item
      tmpItem->next=ctrs->head;           //attach pair item to head of list
      ctrs->head = tmpItem;
      return; 
    }
    //otherwise, begin iteration
    counter_t *currnode = ctrs->head;
    //search for key
    while(currnode->key != key){
      if(currnode->next == NULL){
        //key not found - make new node and attach
        counter_t *tmpItem = counter_new(key, count); //initialize pair item
        currnode->next = tmpItem;
        return; 
      }
      currnode = currnode->next;
    } //key found - switch value
    currnode->count = count;
  }
}

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp) {
  //if file path exists
  if(fp){
    if(ctrs != NULL){  
      fputc('{', fp); //begin printing counters
      counter_t *currnode = ctrs->head;
      //while not at end of list
      do{
        fprintf(fp, "[Key: %d, Val: %d], ", currnode->key, currnode->count);
      }while((currnode = currnode->next));
      //close out list
      fputs(" }\n", fp);

    } else printf("(null)\n");
  }
}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count)){
  if (ctrs && itemfunc) {
    // call itemfunc with arg, on each item
    for (counter_t *currnode = ctrs->head; 
            currnode != NULL; currnode = currnode->next) {
      (*itemfunc)(arg, currnode->key, currnode->count); 
    }
  }
}

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs){
  if (ctrs) {
    for (counter_t *currnode = ctrs->head; currnode != NULL; ) {
      counter_t *next = currnode->next;	    // remember what comes next
      count_free(currnode);			  
      currnode = next;			    //continue iterating.
    }
    count_free(ctrs);
  }
}

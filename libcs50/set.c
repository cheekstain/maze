/* 
 * set.c - CS50 'set' module 
 *
 * see set.h for more information
 *
 * Bill Tang, July 2017
 * 
 * NOTE: Because I implemented the Set Module as a linked-list, the majority
 * of this code was either based on, inspired by, or directly taken from bag.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "memory.h"


/**************** local types ****************/
typedef struct setitem {
  void *item;		      // pointer to data for this item
  const char *key;	      // key for this item
  struct setitem *next; //pointer link this to next item
} setitem_t;

/**************** global types ****************/
typedef struct set {
  int size;         //Tracks size of Set
  setitem_t *first; //Gives the first set element
} set_t;

/**************** local functions ****************/
/* not visible outside this file */
static setitem_t *setitem_new(void *item, const char *key);

/**************** setitem_new ****************/
/* Allocate and initialize a setitem */
static setitem_t *setitem_new(void *item, const char *key) {
  setitem_t *tmp = allocate(sizeof(setitem_t));
  tmp->item = item;
  tmp->key = strdupp((char*)key);
  tmp->next = NULL;
  return tmp;
}

/**************** set_new() ****************/
set_t *set_new(void){
  set_t *tmp = allocate(sizeof(set_t)); //Allocates Memory
  return tmp;
}



/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool set_insert(set_t *set, const char *key, void *item){
  if(set && key && item){        //Iff all args are valid
    if(set_find(set, key)){       //check if key already exists in set
       //yes = error, return.
      return false;
    }    
    //no = attach the key-item pair to the end of the set.
    else{
      setitem_t *tmpItem = setitem_new(item, key); //initialize pair item
      
      tmpItem->next=set->first;           //attach pair item to head of list
      set->first = tmpItem;
      set->size++;
      return true;                        //we done
    }
  } 
  //if not all args are valid, error and exit
  else {
    return false;
  }
}


/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, const char *key) {
  //check for validity of args
  if(set && key){
    //initialize our currently examined item - if it doesn't exist, return error
    setitem_t *searchquery;
    if(set->first) {
      searchquery = set->first;
    } else {
      return NULL;
    }
    //examine for key 
    while(strcmp(searchquery->key, key) != 0 ){
      // if not found, then iterate to next item
      if(searchquery->next){
        searchquery = searchquery->next;
      }
      //If the next item doesn't exist and we haven't found it,
      //we've reached the end of the set and it doesn't exist. 
      else {
        return NULL;
      }
    }
    
    //otherwise, we've found it.
    return searchquery->item;
  } else { //if not all args are valid, error and exit
  return NULL;
  }
}


/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) ) {
  //if fp isn't NULL
  if(fp) {
    //if set exists
    if(set){
      fputc('{', fp); //begin set
      for(setitem_t *curritem = set->first; 
            curritem != NULL; curritem=curritem->next) {
        if (itemprint) {  // print the pair
          fputc(' ', fp);
          (*itemprint)(fp, curritem->key, curritem->item); 
        }
      }
      //close set
      fputs(" } \n", fp);
    }
    //set doesn't exist/is invalid
    else {
      fputs("(null)\n", fp);
    }
  }
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ) {
  if (set && itemfunc) {
    // call itemfunc with arg, on each item
    for (setitem_t *curritem = set->first; 
            curritem != NULL; curritem = curritem->next) {
      (*itemfunc)(arg, curritem->key, curritem->item); 
    }
  }
}

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) ) {
  if (set) {
    for (setitem_t *curritem = set->first; curritem != NULL; ) {
      if (itemdelete) {		    
        (*itemdelete)(curritem->item); //if itemdelete isn't broken, delete data
      }
      free((char*)curritem->key);
      setitem_t *next = curritem->next;	    // remember what comes next
      count_free(curritem);			  
      curritem = next;			    //continue iterating.
    }
    count_free(set);
  }
}

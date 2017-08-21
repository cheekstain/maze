/* 
 * hashtable.c - CS50 'hashtable' module 
 *
 * see hashtable.h for more information
 *
 * Bill Tang, July 2017
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "memory.h"
#include "jhash.h" 
#include "set.h"



/**************** global types ****************/
typedef struct hashtable {
  int size;         //Tracks number of slots
  set_t **table; //the array of slots themselves
} hashtable_t;

/* Create a new (empty) hashtable; return NULL if error. 
 * BILL TANG REVISION: Because my custom-written allocate() automatically
 * checks for allocation error, I do not return NULL for error here - otherwise
 * it's pure redundancy, and inefficient.
 **/
hashtable_t *hashtable_new(const int num_slots) {
  hashtable_t *tmp = allocate(sizeof(hashtable_t)); //Allocate Hashtable
  tmp->size = num_slots;                            //fill out size var
      //allocate the actual table
  tmp->table = allocate(sizeof(set_t*) * num_slots);
  for(int i = 0; i < num_slots; ++i){
    tmp->table[i] = set_new();
  }
  return tmp;
}

/* Insert item, identified by key (string), into the given hashtable.
 * The key string is copied for use by the hashtable.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
  if(ht && key && item){
    //get hash value for key
    const int mod = ht->size;
    unsigned long hash = JenkinsHash(key, mod);
    //get ref to hashed slot-set
    set_t *curr = ht->table[hash];
    
    //check if key already exists
    if(set_find(curr, key))
      return false;

    //insert
    set_insert(curr, key, item);
    
    //check for successful insertion
    if(set_find(curr, key))
      return true;
    else
      return false;
  } else {
    //bad args
    return false;
  }
}

/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
void *hashtable_find(hashtable_t *ht, const char *key) {
  //check for existence of both
  if(ht && key){
     //get hash value for key
    const int mod = ht->size;
    unsigned long hash = JenkinsHash(key, mod);
    set_t *curr = ht->table[hash];

    if(set_find(curr, key)){
      return set_find(curr, key);
    }
    //key doesn't exist
    return NULL;
  } else {
  return NULL;
  }
}

/* Print the whole table; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)) {
  if(fp){
    if(ht != NULL){
      fputc('{', fp); //begin print
      for(int x = 0; x < ht->size; ++x){
        set_t *curr = ht->table[x];
        if (itemprint) {  // print the hashset
          set_print(curr, fp, itemprint); 
        }
      }
      //end print
      fputs(" } \n", fp);
    } else {
      fputs("(null)\n", fp);
    }
  }
}

/* Iterate over all items in the table; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) ){
  if(ht && itemfunc){
    //call itemfunc on each item in order
    for(int x = 0; x < ht->size; ++x){
      set_t *curr = ht->table[x];    
      set_iterate(curr, arg, itemfunc); 
    }
  }
}
/* Delete the whole hashtable; ignore NULL ht.
 * Provide a function that will delete each item (may be NULL).
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) {
  if(ht){
    for(int x = 0; x < ht->size; ++x){
      //iterate and delete all items
      set_t *curr = ht->table[x];    
      set_delete(curr, itemdelete);
    }
      //destroy table
    count_free(ht->table);
      //destroy entire hashtable
    count_free(ht);
  }
}

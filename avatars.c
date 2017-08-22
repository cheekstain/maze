
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <stdbool.h>
#include "mazestruct.h"
#include "amazing.h"

/**************** structs ******************/
typedef struct data_pointer_struct {
  const char* hostname;
  const int maze_port;
  const char* filename;
  const int avatar_id;
  const maze_t* maze;
  const lastmove_t lastmove;
} pointers_t;

static bool logfile_finished = false;
/*
 * Makes deep copies of everything except maze, for obvious reasons. The free function
 * deals with everything but maze, which should be freed elsewhere.
 */
pointers_t* pointers_new(const char* hostname, 
                 const int maze_port,
                 const char* filename,
                 const int avatar_id,
                 const maze_t* maze,
                 const lastmove_t* lastmove){
  pointers_t* tmp = malloc(sizeof(pointers_t));
  memset(tmp, 0, sizeof(pointers_t));
  tmp.hostname = strdup(hostname);
  tmp.maze_port = maze_port;
  tmp.filename = strdup(filename);
  tmp.avatar_id = avatar_id;
  tmp.maze = maze;
  tmp.lastmove = lastmove;
  return tmp;
}

/*
 * Free func for pointers_t. Nota bene: this all deep copies, but not maze, which should
 * itself be freed elsewhere.
 */
void pointers_delete(pointers_t *ptr){
  free(ptr.hostname);
  free(ptr.filename);
  free(ptr);
}

/*
 * Duplicates a string, returns a pointer. Useful helper func.
 */
char *strdup(const char *c){
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}


/*
 * the primary avatar thread. it should be passed with an arg
 * and this arg is a pointer to a struct containing a ton of pointers
 * to various information that we need
 */
void* avatar_thread(void *ptr){
  pointers_t *data = ptr;
  comm_t *com = comm_new();
  send_avtatar_ready(com, get_avatarID(data));
  while(!receive_message(com)){}
  while (game_status(com) == 0){
    if (get_turnID(com) == get_avatarID(data)){
      update_last_move(data);
      move_t *next_move = maze_solve(data);
    }
  }
  if (!logfile_finished){
  finish_logfile(data);
  }
}

void finish_logfile(pointers_t *data){
  
}






















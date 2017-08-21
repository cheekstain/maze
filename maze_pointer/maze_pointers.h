/* 
 * mazepointers.h - custom module containing all pertinent info
 * for each avatar
 *
 * Bill Tang, July 2017
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"
#include <string.h>
#include <../mazestruct/mazestruct.h>

typedef struct maze_data_pointer_struct maze_pointers_t;
typedef struct lastmove{
  XYPos before;
  XYPos after;
  int avatarID;
  int direction;
} lastmove_t;
maze_pointers_t* maze_pointers_new(char* hostname, 
                         int maze_port,
                         char* filename,
                         int avatar_id,
                         maze_t* maze,
                         lastmove_t* lastmove,
                         counters_t* follow_list);
/*
 * Getter Funcs
 * at no point are ANY of these pointers to be modified. These are for
 * information purposes ONLY.
 */
const char* get_hostname(maze_pointers_t *ptr);
const int get_maze_port(maze_pointers_t *ptr);
const char* get_filename(maze_pointers_t *ptr);
const int get_avatar_id(maze_pointers_t *ptr);
const maze_t* get_maze(maze_pointers_t *ptr);
const lastmove_t* get_lastmove(maze_pointers_t *ptr);
const counters_t* get_follow_list(maze_pointers_t *ptr);

/*
 * Setter Methods - use these to change, if necessary.
 */
void set_hostname(maze_pointers_t *ptr, char* hostname);
void set_maze_port(maze_pointers_t *ptr, int maze_port);
void set_filename(maze_pointers_t *ptr, char* filename);
void set_avatar_id(maze_pointers_t *ptr, int avatar_id);
void set_maze(maze_pointers_t *ptr, maze_t* maze);
void set_lastmove(maze_pointers_t *ptr, lastmove_t* lastmove);
void set_follow_list(maze_pointers_t *ptr, counters_t* follow_list);

/*
 * Free func for pointers_t. Nota bene: this all deep copies, but not maze, lastmove, and counters, which should
 * itself be freed elsewhere.
 */
void pointers_delete(maze_pointers_t *ptr);

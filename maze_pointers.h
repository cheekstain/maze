/* 
 * mazepointers.h - custom module containing all pertinent info
 * for each avatar
 *
 * Bill Tang, July 2017
 * 
 */

#ifndef __MAZE_POINTERS_H
#define __MAZE_POINTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcs50/counters.h"
#include "libcs50/memory.h"
#include "mazestruct/mazestruct.h"
#include "avatar_comm/avatar_comm.h"

typedef struct maze_data_pointer_struct maze_pointers_t;

typedef struct lastmove {
  XYPos *before;
  XYPos *after;
  int avatarID;
  int direction;
} lastmove_t;


maze_pointers_t* maze_pointers_new(char* hostname, 
                         int maze_port,
                         char* fp,
                         int avatar_id,
                         maze_t* maze,
                         lastmove_t* lastmove,
                         counters_t* follow_list,
                         comm_t* comm,
                         int n_avatars);
/*
 * Getter Funcs
 * at no point are ANY of these pointers to be modified. These are for
 * information purposes ONLY.
 */
char* get_hostname(maze_pointers_t *ptr);
int get_maze_port(maze_pointers_t *ptr);
char* get_filestream(maze_pointers_t *ptr);
int get_avatar_id(maze_pointers_t *ptr);
maze_t* get_maze(maze_pointers_t *ptr);
lastmove_t* get_lastmove(maze_pointers_t *ptr);
counters_t* get_follow_list(maze_pointers_t *ptr);
int get_path_strength(maze_pointers_t *ptr);
comm_t *get_comm(maze_pointers_t *ptr);
int get_n_avatars(maze_pointers_t *ptr);

/*
 * Dealing with Path Strength
 */
void increment_path_strength(maze_pointers_t *ptr);

/*
 * Setter Methods - use these to change, if necessary.
 */
void set_hostname(maze_pointers_t *ptr, char* hostname);
void set_maze_port(maze_pointers_t *ptr, int maze_port);
void set_filestream(maze_pointers_t *ptr, char* fp);
void set_avatar_id(maze_pointers_t *ptr, int avatar_id);
void set_maze(maze_pointers_t *ptr, maze_t* maze);
void set_lastmove(maze_pointers_t *ptr, lastmove_t* lastmove);
void set_follow_list(maze_pointers_t *ptr, counters_t* follow_list);

/*
 * Free func for maze_pointers_t. Nota bene: this all deep copies, but not maze, lastmove, and counters, which should
 * itself be freed elsewhere.
 */
void maze_pointers_delete(void *ptr);



#endif //MAZE_POINTERS_H




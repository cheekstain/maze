/* 
 * mazepointers.c - custom module containing all pertinent info
 * for each avatar
 *
 * see mazepointers.h for more info
 *
 * Bill Tang, July 2017
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcs50/counters.h"
#include "libcs50/memory.h"
#include "maze_pointers.h"
#include "avatar_comm/avatar_comm.h"

char *strduplicate(char *c);

typedef struct maze_data_pointer_struct {
  char* hostname;
  int maze_port;
  char* fp;
  int avatar_id;
  maze_t* maze;
  lastmove_t* lastmove;
  counters_t* follow_list;
  int strength;
  comm_t *com;
  int n_avatars;
} maze_pointers_t;

/*
 * Makes deep copies of everything except maze, for obvious reasons. The free function
 * deals with everything but maze, which should be freed elsewhere.
 */
maze_pointers_t* maze_pointers_new(char* hostname, 
                         int maze_port,
                         char* fp,
                         int avatar_id,
                         maze_t* maze,
                         lastmove_t* lastmove,
                         counters_t* follow_list, 
                         comm_t *com,
                         int n_avatars)
{
  maze_pointers_t* tmp = allocate(sizeof(maze_pointers_t));
  tmp->hostname = strduplicate(hostname);
  tmp->maze_port = maze_port;
  tmp->fp = fp;
  tmp->avatar_id = avatar_id;
  tmp->maze = maze;
  tmp->lastmove = lastmove;
  tmp->follow_list = follow_list;
  tmp->strength = 0;
  tmp->com = com;
  tmp->n_avatars = n_avatars;
  return tmp;
}

/*
 * Getter Funcs
 */

char* get_hostname(maze_pointers_t *ptr)
{
  return ptr->hostname;
} 

int get_maze_port(maze_pointers_t *ptr)
{
  return ptr->maze_port;
}

char* get_filestream(maze_pointers_t *ptr)
{
  return ptr->fp;
}

int get_avatar_id(maze_pointers_t *ptr)
{
  return ptr->avatar_id;
}

maze_t* get_maze(maze_pointers_t *ptr)
{
  return ptr->maze;
}

lastmove_t* get_lastmove(maze_pointers_t *ptr){

  return ptr->lastmove;
}

counters_t* get_follow_list(maze_pointers_t *ptr)
{
  return ptr->follow_list;
}

int get_path_strength(maze_pointers_t *ptr)
{
  return ptr->strength;
}

comm_t *get_comm(maze_pointers_t *ptr)
{
  return ptr->com;
}

int get_n_avatars(maze_pointers_t *ptr)
{
  return ptr->n_avatars;
}

/*
 * Dealing with Path Strength
 */
void increment_path_strength(maze_pointers_t *ptr)
{
    ptr->strength++;
}

/*
 * Setter Methods
 */
void set_hostname(maze_pointers_t *ptr, char* hostname)
{
  ptr->hostname = strduplicate(hostname);
} 

void set_maze_port(maze_pointers_t *ptr, int maze_port)
{
  ptr->maze_port = maze_port;
}

void set_filename(maze_pointers_t *ptr, char* fp)
{
  ptr->fp = fp;
}

void set_avatar_id(maze_pointers_t *ptr, int avatar_id)
{
  ptr->avatar_id = avatar_id;
}

void set_maze(maze_pointers_t *ptr, maze_t* maze)
{
  ptr->maze = maze;
}

void set_lastmove(maze_pointers_t *ptr, lastmove_t* lastmove)
{
  ptr->lastmove = lastmove;
}

void set_follow_list(maze_pointers_t *ptr, counters_t* follow_list)
{
  ptr->follow_list = follow_list;
}

/*
 * Free func for maze_pointers_t. Nota bene: this all deep copies, but not maze, which should
 * itself be freed elsewhere.
 */
void maze_pointers_delete(void *ptr)
{
  maze_pointers_t *tmp = ptr;
  free(tmp->hostname);
  free(tmp);
}

/*
 * Duplicates a string, returns a pointer. Useful helper func.
 */
char *strduplicate(char *c)
{
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

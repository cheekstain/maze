
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

/*
 * Makes deep copies of everything except maze, for obvious reasons. The free function
 * deals with everything but maze, which should be freed elsewhere.
 */

pointers_t* pointers_new(const char* hostname, 
                 const int maze_port,
                 const char* filename,
                 const int avatar_id,
                 const maze_t* maze,
                 const lastmove_t lastmove){
  pointers_t* tmp = malloc(sizeof(pointers_t));
  memset(tmp, 0, sizeof(pointers_t));
  tmp.hostname = 
  tmp.maze_port =
  tmp.filename =
  tmp.avatar_id =
  tmp.maze =
  tmp.lastmove =
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
 * the primary avatar thread. it should be passed with an arg
 * and this arg is a pointer to a struct containing a ton of pointers
 * to various information that we need
 */
void* avatar_thread(void *ptr){
  
}




bool cleanedup;
bool setup = false;

typedef struct avatar {
  int avatar_following;
  int current_pathID;
} avatar_t;

typedef struct lastmove{
  XYPos before;
  XYPos after;
  int avatarID;
  int direction;
} lastmove_t;

void run_avatar(int avatarID){
  if (!setup){
    setup_datastructures();
    cleanedup = false;
    setup = true;
  }

  while (!is_game_over()){
    if (get_turnID() == avatarID){
      update_maze();
      update_avatars(avatarID);
      update_logfile(avatarID);
      draw_maze(maze);
      make_move(maze, avatars);
      update_last_move(avatarID);
    }
  }

  if (!cleanedup){
    finish_logfile();
    maze_delete(maze);
    set_delete(avatars, avatar_delete);
    cleanedup = true;
  }
}

void update_last_move(int avatarID);

void setup_datastructures(){
  maze = maze_new(get_maze_width(), get_maze_height(), get_num_avatars());

  avatars = set_new();
  for(int i = 0; i<get_num_avatars(); i++){
    avatar_t *new_avatar;
    new_avatar->avatar_follwing = -1;
    new_avatar-> current_pathID = 0;
    set_insert(avatars, i, new_avatar);
  }

  lastmove.avatarID = -1;
}
void update_last_move(int avatarID);
void update_avatars(int avatarID);

void update_maze(){}

void update_logfile(int avatarID);
finish_logfile();
maze_delete(maze);
void avatar_delete(void *item);
void set_lastmove_previous(XYPos pos);
void set_lastmove_next(XYPos pos);
void set_lastmove_avatarID(int avatarID);

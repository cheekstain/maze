
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

maze_t *maze;
set_t *avatars;
lastmove_t lastmove;
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

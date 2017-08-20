
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

void run_avatar(int avatarID){
  if (!setup){}
    maze = maze_new(get_maze_width(), get_maze_height(), get_num_avatars());
    setup_avatars();
    cleanedup = false;
  }

  while (!is_game_over()){
    if (get_turnID() == avatarID){
      update_last_move(avatarID);
      update_avatars(avatarID);
      update_logfile(avatarID);
      draw_maze(maze);
      make_move(maze, avatars);
    }
  }

  if (!cleanedup){
    finish_logfile();
    maze_delete(maze);
    set_delete(avatars, avatar_delete);
    free(lastmove.prev);
    free(lastmove.next);
    cleanedup = true;
  }
}

void update_last_move(int avatarID);
void
void set_lastmove_previous(XYPos pos);
void set_lastmove_next(XYPos pos);
void set_lastmove_avatarID(int avatarID);

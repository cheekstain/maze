
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
#include <maze_pointers.h>

static bool logfile_finished = false;

/*
 * the primary avatar thread. it should be passed with an arg
 * and this arg is a pointer to a struct containing a ton of pointers
 * to various information that we need
 */
void* avatar_thread(void *ptr){
  pointers_t *data = ptr;
  comm_t *com = comm_new();
  send_avatar_ready(com, get_avatar_id(data));
  while(!receive_message(com)){}
  while (game_status(com) == 0){
    if (get_turnID(com) == get_avatar_id(data)){
      int prev_strength;
      if(get_avatar_id(data) == 0){
        prev_strength = get_path_strength(data);
      } else {
        prev_strength = get_path_strength(data) + 1;
      }
      check_previous(get_maze(data), get_lastmove(data), 
               get_filestream(data), get_path_strength(data), get_follow_list(data));
      counters_t* follow_list = get_follow_list(data);
      if(counters_get(follow_list, get_avatar_id(data)) == get_avatar_id(data)){
        follower_t f;
        f.id = get_avatar_id(data);
        f.b = false;
        counters_iterate(follow_list, f, check_all_following);
        if(f.b){
          move_t* m = maze_solve(get_maze(data), get_avatar_id(data), SOMEHOW GET THE LOCATION, get_filestream(data)));
          if(m != NULL){
            #SEND M TO PLACE
          }
        } else {
          move_t* m = leader_solve(get_maze(data), get_avatar_id(data), SOMEHOW GET THE LOCATION, get_filestream(data)));
          if(m != NULL){
            #SEND M TO PLACE
          }
        }
      } else {
        move_t* m = follower_solve(get_maze(data), get_avatar_id(data), SOMEHOW GET THE LOCATION, get_filestream(data)));
        if(m != NULL){
          #SEND M TO PLACE
        }
      }
      increment_path_strength(data);
    }
  }
  if (!logfile_finished){
  finish_logfile(data);
  }
}

void finish_logfile(pointers_t *data){
  
}

/*
 * Helper struct for check_all_following et cetera functions.
 */

typedef struct following_bool {
  int id;
  bool b;
} follower_t;
 
/*
 * Helper function. Checks if the avatar in question is the last leader
 */
void check_all_following(follower_t a, const int key, int count){
  if(key != a.id){
    if(key == count){
      a.b = false;
    }
  }
}



















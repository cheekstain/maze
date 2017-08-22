
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
      check_previous(get_maze(data), get_lastmove(data), 
               get_filestream(data), get_path_strength(data), get_follow_list(data));
      counters_t* follow_list = get_follow_list(data);
      if(counters_get(follow_list, get_avatar_id(data)) == get_avatar_id(data)){
        bool last_leader = false;
        counters_iterate(follow_list, last_leader, check_all_following);
        
      }
      else{
        move_t *next_move = maze_solve(data);
      }
    }
  }
  if (!logfile_finished){
  finish_logfile(data);
  }
}

void finish_logfile(pointers_t *data){
  
}

/*
 * Helper function. Checks if all 
 */




















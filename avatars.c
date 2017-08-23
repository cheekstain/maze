#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <stdbool.h>
#include "mazestruct/mazestruct.h"
#include "amazing.h"
#include "avatar_solve/avatar_solve.h"
#include "avatar_comm/avatar_comm.h"
#include "maze_pointers.h"

/*
 * Helper struct for check_all_following et cetera functions.
 */

typedef struct following_bool {
  int id;
  bool is_last_leader;
} follower_t;

void check_all_following(void* follower, const int key, int count);

/*
 * the primary avatar thread. it should be passed with an arg
 * and this arg is a pointer to a struct containing a ton of pointers
 * to various information that we need
 */
void* avatar_thread(void *ptr){
  maze_pointers_t *data = ptr;
  comm_t *com = get_comm(data); //DELET THIS TY
  int sock = 0;
  sock = send_avatar_ready(com, get_avatar_id(data));
  if (sock == -1){
    printf("Error receiving avatar's socket, avatarID = %d", get_avatar_id(data));
    return NULL;
  }
  while(!receive_message(com, get_avatar_id(data), sock)){}
  XYPos *position = get_position_array(com);
  set_avatar_position(get_maze(data), &position[get_avatar_id(data)], get_avatar_id(data));
  bool was_my_turn = false;
  while (check_game_status(com) == 0){
    if (get_turnID(com) == get_avatar_id(data) && !was_my_turn){
      was_my_turn = true;
      usleep(70000);
      int prev_strength = 0;
      int prev_id;
      XYPos *positions = get_position_array(com);
      XYPos my_pos = positions[get_avatar_id(data)];
      if(get_avatar_id(data) == 0){
        prev_strength = get_path_strength(data);
      } else {
        prev_strength = get_path_strength(data) + 1;
      }
      if(get_avatar_id(data) == 0){
          prev_id = get_n_avatars(data) - 1;
      } else {
          prev_id = get_avatar_id(data) - 1;
      }
      XYPos prev_pos = positions[prev_id];
      lastmove_t* lm = get_lastmove(data);
      lm->after = &prev_pos;
      check_previous(get_maze(data), lm, 
               get_filestream(data), prev_strength, get_follow_list(data));
      counters_t* follow_list = get_follow_list(data);
      if(counters_get(follow_list, get_avatar_id(data)) == get_avatar_id(data)){
        follower_t f;
        f.id = get_avatar_id(data);
        f.is_last_leader = false;
        counters_iterate(follow_list, &f, check_all_following);
        if(!f.is_last_leader){
          move_t* m = maze_solve(get_maze(data), get_avatar_id(data), 
                &my_pos, get_follow_list(data), get_filestream(data));
          lm->avatarID = get_avatar_id(data);
          lm->direction = m->direction;
          lm->before = &my_pos;
          if(m != NULL){
            int move = m->direction;
            send_move(com, get_avatar_id(data), move, sock);
            free(m);
          }
        } else {
          move_t* m = leader_solve(get_maze(data), get_avatar_id(data), 
                &my_pos, get_filestream(data));
          lm->avatarID = get_avatar_id(data);
          lm->direction = m->direction;
          lm->before = &my_pos;
          if(m != NULL){
            int move = m->direction;
            send_move(com, get_avatar_id(data), move, sock);
            free(m);
          }
        }
      } else {
        move_t* m = follower_solve(get_maze(data), get_avatar_id(data), &my_pos, 
                follow_list, get_filestream(data));
        lm->avatarID = get_avatar_id(data);
        lm->direction = m->direction;
        lm->before = &my_pos;
        if(m != NULL){
          int move = m->direction;
          send_move(com, get_avatar_id(data), move, sock);
          free(m);
        }
      }
      increment_path_strength(data);
    }
    else{
      was_my_turn = false;
    }
    while (!receive_message(com, get_avatar_id(data), sock) && check_game_status(com) == 0){}
  }
  close(sock);
  return NULL;
}

/*
 * Helper function. Checks if the avatar in question is the last leader
 */
void check_all_following(void* follower, const int key, int count){
  follower_t *a = follower;
  if(key != a->id){
    if(key == count){
      a->is_last_leader = false;
    }
  }
}



















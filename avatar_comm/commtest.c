#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "amazing.h"
#include "avatar_comm.h"


comm_t *com;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* run_avatar (void *id);
/*
* All the testing is done in the main method.
*/
int main()
{
  com = comm_new();
  send_init(com, 2, 0, "flume.cs.dartmouth.edu");
  bool a = receive_message(com, -1, -1);
  if (a && is_init_successful(com)){
    printf("Connected! The mazeport is %d, the width of the maze is %d and the height of the maze is %d\n", get_mazeport(com), get_maze_width(com), get_maze_height(com));
  }
  if (!a){
    fprintf(stderr, "receive message error\n");
    return 1;
  }

  int id1 = 0;
  int id2 = 1;
  //int id3 = 2;
  pthread_t t1;
  pthread_t t2;
 // pthread_t t3;

  int iret1 = pthread_create(&t1, NULL, run_avatar, &id1);

  if (iret1) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }

  int iret2 = pthread_create(&t2, NULL, run_avatar, &id2);

  if (iret2) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }

  //int iret3 = pthread_create(&t3, NULL, run_avatar, &id3);

  //if (iret3) {
    //fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  //}

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  //pthread_join(t3, NULL);



  return 0;
}

void* run_avatar (void *ptr)
{
  
  int *avatarID = ptr;
  int sock = 0;
  if ((sock = send_avatar_ready(com, *avatarID)) == -1){
    printf("noooo!");
    return NULL;
  }
  while (!receive_message(com, *avatarID, sock)){}
  //if (check_game_status(com) == 0){
    //printf("success!");
  //}

  //else{
    //printf("failure");
  //}
  

  while (check_game_status(com) == 0){
    if (get_turnID(com) == *avatarID){
      printf("%d is the turnID\n", get_turnID(com));
      int move = rand() % 4;
      send_move(com, *avatarID, move, sock);
      printf("%d!\n", check_game_status(com));
    }
    while (!receive_message(com, *avatarID, sock)){}
  }
  printf("%d!\n", check_game_status(com));
  return NULL;
}














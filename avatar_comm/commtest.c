
#ifndef _XOPEN_SOURECE
#define _XOPEN_SOURCE 500
#endif //_XOPEN_SOURCE


#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../amazing.h"
#include "avatar_comm.h"


comm_t *com;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

void* run_avatar (void *id);
/*
* All the testing is done in the main method.
*/
int main()
{
  com = comm_new();
  send_init(com, 10, 9, "flume.cs.dartmouth.edu");
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
  int id3 = 2;
  int id4 = 3;
  int id5 = 4;
  int id6 = 5;
  int id7 = 6;
  int id8 = 7;
  int id9 = 8;
  int id10 =9;
  pthread_t t1;
  pthread_t t2;
  pthread_t t3;
  pthread_t t4;
  pthread_t t5;
  pthread_t t6;
  pthread_t t7;
  pthread_t t8;
  pthread_t t9;
  pthread_t t10;

  int iret1 = pthread_create(&t1, NULL, run_avatar, &id1);

  if (iret1) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }

  int iret2 = pthread_create(&t2, NULL, run_avatar, &id2);

  if (iret2) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret2);
  }

  int iret3 = pthread_create(&t3, NULL, run_avatar, &id3);

  if (iret3) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret3);
  }


  int iret4 = pthread_create(&t4, NULL, run_avatar, &id4);

  if (iret4) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret4);
  }

  int iret5 = pthread_create(&t5, NULL, run_avatar, &id5);

  if (iret5) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret5);
  }

  int iret6 = pthread_create(&t6, NULL, run_avatar, &id6);

  if (iret6) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret6);
  }

  int iret7 = pthread_create(&t7, NULL, run_avatar, &id7);

  if (iret7) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret7);
  }

  int iret8 = pthread_create(&t8, NULL, run_avatar, &id8);

  if (iret8) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret8);
  }

  int iret9 = pthread_create(&t9, NULL, run_avatar, &id9);

  if (iret9) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret9);
  }

  int iret10 = pthread_create(&t10, NULL, run_avatar, &id10);

  if (iret10) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret10);
  }

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);
  pthread_join(t5, NULL);
  pthread_join(t6, NULL);
  pthread_join(t7, NULL);
  pthread_join(t8, NULL);
  pthread_join(t9, NULL);
  pthread_join(t10, NULL);



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
  if (check_game_status(com) == 0){
    printf("success!");
  }

  else{
    printf("failure");
  }
  
  //bool was_my_turn = false;
  while (check_game_status(com) == 0){
    //printf("thread %d enters loop\n", *avatarID);
    
    int num_moves = 0;
    if (get_turnID(com) == *avatarID ){//&& !was_my_turn){
      usleep(70000);
      //was_my_turn = true;
      //printf("%d is the turnID\n", get_turnID(com));
      int move = rand() % 4;
      //printf("about to send move from thread %d\n", *avatarID);
      send_move(com, *avatarID, move, sock);
      //printf("%d!\n", check_game_status(com));
    }
    else{
      //was_my_turn = false;
    }
    printf("nummoves = %d", num_moves);
    while (!receive_message(com, *avatarID, sock) && check_game_status(com) == 0){}
    //printf("received message in thread %d\n", *avatarID);
  }
  printf("numer of moves made = %d", get_nMoves(com));
  return NULL;
}














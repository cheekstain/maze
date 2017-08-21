#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "amazing.h"
#include "avatar_comm.h"


/*
* All the testing is done in the main method.
*/
int main()
{
  comm_t *com = comm_new();
  send_init(com, 3, 3, "flume.cs.dartmouth.edu");
  bool a = receive_message(com);
  if (a && is_init_successful(com)){
    printf("Connected! The mazeport is %d, the width of the maze is %d and the height of the maze is %d\n", get_mazeport(com), get_maze_width(com), get_maze_height(com));
  }
  else if (!is_init_successful(com)){
    printf("init not successful error!");
  }
  if (!a){
  	fprintf(stderr, "receive message error");
  }

  int iret1 = pthread_create(&t1, NULL, , NULL);
  if (iret1) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }

  int iret2 = pthread_create(&t1, NULL, , NULL);
  if (iret1) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }

  int iret3 = pthread_create(&t1, NULL, , NULL);
  if (iret1) {
    fprintf(stderr,"Cannot create thread, rc=%d\n", iret1);
  }
  

  return 0;
}


/*
 * Bashful Brigade, July 2017
 */
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

maze_t maze;
set_t avatars;

int main (int argc, char *argv[]){
	
  /*
    PARSE PARAMETERS
    TODO: FILL IN ARG
    TODO: Use strcpys and atois >:[
   */
  if (argc != [************]){ //testing incorrect number of arguments
    fprintf(stderr, "Incorrect number of arguments!\n");
    return 1;
  }

  char *seed_url = malloc(sizeof(char) * (strlen(argv[1])+1));
  if (seed_url == NULL){
    fprintf(stderr, "Error allocating memory!");
    exit(6);
  }
  if (sscanf (argv[1], "%s", seed_url) != 1){ //testing valid seed url
    fprintf(stderr, "Invalid seed url!\n");
    free(seed_url);
    return 2;
  }

  int nAvatars, difficulty, mazeWidth, mazeHeight, mazeport;
  sscanf (argv[1], "%d", &nAvatars);
  sscanf (argv[2], "%d", &difficulty);
  sscanf (argv[3], "%d", &mazeWidth);
  sscanf (argv[4], "%d", &mazeHeight);
  sscanf (argv[6], "%d", &nAvatars);

  char hostname[strlen(argv[5])+1];
  char filename[strlen(argv[7])+1];
  sscanf (argv[5], "%s", &hostname);
  sscanf (argv[7], "%s", &filename);

  maze = maze_new(mazeWidth, mazeHeight);
  avatars = set_new();  
  initialize_avatars(avatars, nAvatars);
  
  
  /*
  if (setup_maze()){
    while(!is_game_over()){
      run_avatar();
    }
  }
  else{
    return 1;
  }
  */
}

void run_avatar(){
  make_best_move(maze, avatars);
  update_maze();
  update_avatars();
  update_logfile();
  draw_maze(maze);
}

void initialize_avatars(set_t *avatar_set, int nAvatars){
  for(int i = 0; i < nAvatars; i++){
    runThread(avatar_thread);
  }
}

void* avatar_thread(ARGSARGSARGSARGSARGSARGS){
  //LITERALLY ALL CODE GOES HERE
  
  write(socket, avatarID, size);
  while(!end){
    
  }
}

/******************HELPER FUNCTIONS AHOY************************/

int readyMessage(int avatarID, socket server){
  int result = write(avatarID, socket);
  return result;
}
 

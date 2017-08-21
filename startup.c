/* 
 * startup.c - Maze startup module
 *
 * This module initializes the game
 *
 * Bashful Brigade: Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>	      // read, write, close
#include <strings.h>	      // bcopy, bzero
#include <netdb.h>	      // socket-related structures
#include <time.h>
#include "amazing.h"
#include "avatar.c"
#include "avatar_comm.h"

/**************** functions ****************/
static bool check_parameters(int argc, char* argv[]);
char *make_log(const int num_avatars, const int difficulty, const int maze_port);

int main(int argc, char* argv[]){
	if (!check_parameters(argc, argv)) {
		exit(1);
	}

	const int n_avatars = atoi(argv[1]);
	int difficulty = atoi(argv[2]);
	char* hostname = argv[3];
	comm_t *com = comm_new();

	send_init(com, n_avatars, difficulty, hostname);
	bool recieved = receive_message(com);

	if (recieved && is_init_successful(com)){
   		printf("Connected! The mazeport is %d", get_mazeport(com));
   		printf("the width of the maze is %d", get_maze_width(com));
   		printf("and the height of the maze is %d\n", get_maze_height(com)); 
  	}
	else if (!is_init_successful(com)){
    	fprintf(stderr, "Error: init unsuccessful\n");
    	exit(2);
	}
	if (!received){
  		fprintf(stderr, "Error: no message recieved from server\n");
  		exit(3);
	}
	
	//mine data out of com structure
	int maze_port = get_mazeport(com);
	const int maze_height = get_maze_height(com);
	const int maze_width = get_maze_width(com);

	char *log_name = make_log(n_avatars, difficulty, maze_port);


	// Main "Program"
	
	//initialize the public dataset that everyone has access to
  	set_t* avatarDataSet = set_new();
  
  	//Initialize the data in the public dataset
  	//maze, followinglist, and lastmove;
  	maze_t* maze = maze_new(maze_width, maze_height, n_avatars);
  	counters_t* avatarFollowing = counters_new();
  	lastmove_t lastmove;
  
  	//array of pointer data (for the threads)
  	pointers_t* data;
  	for(int i = 0; i < n_avatars; i++){
   		//generate individual data for avatars 1, 2, 3...etc.
    	counters_set(avatarFollowing, i, i);
    	data = pointers_new(hostname, maze_port, [LOGFILENAME], i, maze, &lastmove);
    	set_insert(avatars, i, data);
    	free(data);
  	}

  	pthread_t threads[n_avatars]
  	int threadError;
  	//set the threads running
  	for(int i = 0; i < n_avatars; i++){
    	threadError = int pthread_create(&threads[i], NULL, avatar_thread, data[i]);
    	if(threadError) {
      		printf("thread creation failed, rc=%d.\n", threadError);
      		return (threadError);
    	}
  	}

  	while(check_game_status(com) == 0){ 
  	}
  
  	//post-game cleanup
  
  	//FREE EVERYTHING
  	free(log_name);
  	maze_delete(maze);
  	set_delete(avatars, pointers_delete);
  	counters_delete(avatarFollowing);
}

/******************************** check_parameters ****************************/
static bool check_parameters(int argc, char* argv[]){
	if (argc != 4) {
		fprintf(stderr, "usage: n_avatars difficulty hostname\n");
		return false;
	}

	int n;
	if (sscanf(argv[1], "%d", &n) == 0) {
		fprintf(stderr, "n_avatars must be int\n");
		return false;
	}

	if (sscanf(argv[2], "%d", &n) == 0) {
		fprintf(stderr, "difficulty must be int\n");
		return false;
	}

	if (n < 0 || n > 9) {
		fprintf(stderr, "difficulty must be between 0 and 9\n");
		return false;
	}

	char* hostname = argv[3];
	char* flume = "flume.cs.dartmouth.edu";
	if ((strcmp(hostname, flume)) != 0) {
		fprintf(stderr, "invalid hostname\n");
		return false;
	}
	return true;
}

/******************************** make_log ************************************/
char *make_log(const int num_avatars, const int difficulty, const int maze_port){
	// make log file, open for appending
	char* log_name = malloc(sizeof(char) * 50);
	sprintf(log_name, "Amazing_%d_%d_%d.log", getuid(), num_avatars, difficulty);
	
	time_t curtime;
    time(&curtime);

	FILE *fp = fopen(log_name, "a");
	fprintf(fp, "User ID: %d, MazePort: %d, Date & Time: %s", getuid(),
		maze_port, ctime(&curtime));
	fclose(fp);
	return log_name;
}




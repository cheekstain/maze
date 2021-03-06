/* 
 * startup.c - Maze startup module
 *
 * This module initializes the game
 *
 * Bashful Brigade: Christina Lu, August 2017
 * Emma Hobday, August 2017
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>	      // read, write, close
#include <strings.h>	  // bcopy, bzero
#include <netdb.h>	      // socket-related structures
#include <time.h>
#include "libcs50/set.h"
#include "amazing.h"
#include "avatars.c"
#include "avatar_comm/avatar_comm.h"
#include "mazestruct/mazestruct.h"

#define update() printf("\033[H\033[J")

/**************** functions ****************/
static bool check_parameters(int argc, char* argv[]);
static char *make_log(const int num_avatars, const int difficulty, 
						const int maze_port);
void finish_logfile(comm_t *com, char *logfile);

int main(int argc, char* argv[])
{
	if (!check_parameters(argc, argv)) {
		exit(1);
	}

	const int n_avatars = atoi(argv[1]);
	int difficulty = atoi(argv[2]);
	char* hostname = argv[3];
	comm_t *com = comm_new();

	send_init(com, n_avatars, difficulty, hostname);
	bool received = receive_message(com, -1, -1);

	if (received && is_init_successful(com)) {
   		printf("Connected! The mazeport is %d, ", get_mazeport(com));
   		printf("the width of the maze is %d, ", get_maze_width(com));
   		printf("and the height of the maze is %d.\n", get_maze_height(com)); 
   		update(); //clear the console in prep to draw the maze

  	} else if (!is_init_successful(com)) {
    		fprintf(stderr, "Error: init unsuccessful\n");
    		exit(2);
	}

	if (!received) {
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
  	set_t* avatars = set_new();
  
  	//Initialize the data in the public dataset
  	//maze, followinglist, and lastmove;
  	maze_t* maze = maze_new(maze_width, maze_height, n_avatars);
  	counters_t* avatar_following = counters_new();
  	lastmove_t* lastmove = allocate(sizeof(lastmove_t));
	lastmove->avatarID = -1;
  	maze_pointers_t* data;

  	for (int i = 0; i < n_avatars; i++) {
   		//generate individual data for avatars 1, 2, 3...etc.
		counters_set(avatar_following, i, i);
		data = maze_pointers_new(hostname, maze_port, log_name, i,
		           maze, lastmove, avatar_following, com, n_avatars);
		char str[80];
		sprintf(str, "%d", i);
		set_insert(avatars, str, data);
  	}

  	pthread_t threads[n_avatars];
  	int thread_error;

  	//set the threads running
  	for (int i = 0; i < n_avatars; i++) {
  		char str[80];
		sprintf(str, "%d", i);
		thread_error = pthread_create(&threads[i], NULL, 
				      avatar_thread, set_find(avatars, str));
		if (thread_error) {
			printf("thread creation failed, rc=%d.\n",
								 thread_error);
			return (thread_error);
		}
  	}

  	//join all the threads, so they'll end gracefully
  	for (int i = 0; i < n_avatars; i++) {
  		pthread_join(threads[i], NULL);
  	}

  	while (check_game_status(com) == 0) { 
  	}
  	
  	finish_logfile(com, log_name);

  	//post-game cleanup
    free(log_name);
  	maze_delete(maze);
  	set_delete(avatars, maze_pointers_delete);
  	counters_delete(avatar_following);
	free(lastmove);
	comm_delete(com);
}

/******************************** check_parameters ****************************/
/*
 * Checks the number of arguments, whether difficulty and number of avatars are
 * each integers between one and nine, and whether the hostname is flume.
 * 
 * If any argument is not what we expect, return false. Otherwise, return true.
 */
static bool check_parameters(int argc, char* argv[])
{
	if (argc != 4) {
		fprintf(stderr, "usage: ./AM_STARTUP n_avatars difficulty hostname\n");
		return false;
	}

	int n;
	if (sscanf(argv[1], "%d", &n) == 0) {
		fprintf(stderr, "n_avatars must be int\n");
		return false;
	}

	if (n < 2 || n > 10) {
		fprintf(stderr, "n_avatars must be between 1 and 10\n");
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
/*
 * Creates a logfile with the name "Amazing_$USER_N_D" and prints the user ID, 
 * mazeport, and date and time to it.
 *
 * Returns the string name of the logfile.
 */
static char *make_log(const int num_avatars, const int difficulty, 
							const int maze_port)
{
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

/******************************** finish_logfile  ************************************/
/*
 * Appends the final end of game message from the server to the log file. Includes
 * number of avatars, difficulty, number of moves, and hashcode.
 * 
 */

void finish_logfile(comm_t *com, char *logfile)
{
	FILE *fp = fopen(logfile, "a");
	int num_avatars = get_nAvatars(com);
	int diff = get_difficulty(com);
	int nMoves = get_nMoves(com);
	int hashcode = get_hash(com);

	fprintf(fp, "Solved maze with nAvatars = %d, difficulty = %d, nMoves = %d, and hashcode = %d\n", num_avatars, diff, nMoves, hashcode);
	fclose(fp);
	
}



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

/**************** functions ****************/
static bool check_parameters(int argc, char* argv[]);


int main(int argc, char* argv[]) 
{
	if (!check_parameters(argc, argv)) {
		exit(1);
	}

	int n_avatars = atoi(argv[1]);
	int difficulty = atoi(argv[2]);
	char* hostname = argv[3];

	// construct/send init, not sure about port # here
	FILE *comm_fp = connect_to(hostname, port);
	
	// receive init ok
	char *response = readfilep(comm_fp);
	
	// parse response?
	int maze_port =
	int maze_height =
	int maze_width = 

	free(response);
	fclose(comm_fp);

	// make log file, open for appending
	char* log_name = malloc(sizeof(char) * 50);
	char* user_id = itoa(getuid());
	strcpy(log_name, "Amazing_");
	strcat(log_name, user_id); 
	strcat(log_name, "_");
	strcat(log_name, argv[1]);
	strcat(log_name, "_")
	strcat(log_name, argv[2]);
	strcat(log_name, ".log");

	time_t curtime;
    time(&curtime);

	FILE *fp = (log_name, "a");
	fprintf(fp, "User ID: %s, MazePort: %d, Date & Time: %s", user_id,
									maze_port, ctime(&curtime));

	// thread business
	
	fclose(fp);
}

static bool check_parameters(int argc, char* argv[])
{
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






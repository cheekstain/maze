/*
 * avatar_solve.c - see avatar_solve.h for details on exported functions
 * 
 * Bashful Brigade, August 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "avatar_solve.h"
#include "avatar_comm.h"
#include "mazestruct.h"
#include "amazing.h"

/****************** local types ******************/

/*************** global functions ***************/
/* see avatar_solve.h for comments about exported functions */

/*************** local functions ****************/
void update_log(char* log); //TODO: other parameters to pass in?
void calculate_random(maze_t); //TODO: unsure of paramters
void calculate_move(void); //TODO: unsure of paramters

/****************** make_move() ******************/
void make_move(maze_t* maze, lastmove_t* move, avatars_t* avatars, char* log, int my_id)
{
	//avatar_comm function to get AM_AVATAR_TURN message
	int my_x = // parse from message
	int my_y = 
	int turn_id = 

	if (turn_id == my_id) {
		calculate_random(maze, my_x, my_y);
		//update visualization, function from mazestruct.h
		draw_maze(maze);

		//write move details to logfile
		update_log(log);

		//avatar_comm function to send AM_AVATAR_MOVE to server
		//
	}

	
}

void update_log(char* log) {

	fprintf(log, "...")
}

void calculate_move(void) {



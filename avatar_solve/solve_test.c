/*
 * solve_test.c - see avatar_solve.h for details on exported functions
 * 
 * Bashful Brigade: Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../amazing.h"
#include "../mazestruct/mazestruct.h"
#include "../maze_pointer/maze_pointers.h"
#include "avatar_solve.h"

int main()
{
	char* log = "test_file";
	int id = 0;
	uint32_t two = 2;
	uint32_t four = 4;
	
	XYPos me = { two, four };
	
	//XYPos other0 = { 3, 4 };
	XYPos other1 = { 1, 4 };
	XYPos other2 = { 2, 3 };
	XYPos other3 = { 1, 3 };
	XYPos other4 = { 0, 3 };
//	XYPos other5 = { 0, 4 };
//	XYPos other6 = { 0, 2 };

	maze_t* maze = maze_new(5, 5, 3);

	//set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 0, 0);
	set_wall(maze, &me, 0, 3);
	set_wall(maze, &other1, 0, 1);
	set_wall(maze, &other1, 1, 0);
	set_wall(maze, &other3, 0, 0);
	set_wall(maze, &other3, 1, 1);
	set_wall(maze, &other3, 1, 3);
	//set_wall(maze, &other4, 0, 1);
	set_wall(maze, &other4, 0, 2);
	
	counters_t* followers = counters_new();
	//counters_set(followers, 0, 1);
	//counters_set(followers, 1, 2);

	set_avatar_position(maze, &other2, id);

	visit(maze, &other2, 1, 2);
	visit(maze, &me, 2, 3);
	//visit(maze, &other0, 3, 5);
	//visit(maze, &other2, 1, 4);
	draw_maze(maze);	
	//move_t* move = follower_solve(maze, id, &me, followers, log);

	// last 
	// before - &other2
	// after - &me
	
	lastmove_t last = { &other2, &me, 0, 2 };
	check_previous(maze, &last, log, 7, followers);
		
	//free(move)
	//free(&last);
	maze_delete(maze);
	counters_delete(followers);
	return 0;
}


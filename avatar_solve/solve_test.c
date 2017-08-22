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
	
	XYPos other0 = { 3, 4 };
	XYPos other1 = { 1, 4 };
	XYPos other2 = { 2, 3 };
	XYPos other3 = { 1, 3 };
	XYPos other4 = { 0, 3 };
//	XYPos other5 = { 0, 4 };
//	XYPos other6 = { 0, 2 };

	maze_t* maze = maze_new(5, 5, 3);

	set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 1, 0);
	set_wall(maze, &me, 0, 3);
	set_wall(maze, &other1, 0, 1);
	set_wall(maze, &other1, 1, 0);
	set_wall(maze, &other3, 0, 0);
	set_wall(maze, &other3, 1, 1);
	set_wall(maze, &other3, 1, 3);
	//set_wall(maze, &other4, 0, 1);
	set_wall(maze, &other4, 0, 2);
	


	set_avatar_position(maze, &me, id);

	visit(maze, &me, 0, 2);
	visit(maze, &other0, 0, 1);
	visit(maze, &other2, 0, 3);
	//draw_maze(maze);	
	move_t* move = leader_solve(maze, id, &me, log);

	free(move);
	maze_delete(maze);
	return 0;
}


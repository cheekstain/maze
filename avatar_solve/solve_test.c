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
	
	XYPos other1 = { 1, 4 };
	XYPos other2 = { 2, 3 };

	maze_t* maze = maze_new(5, 5, 3);

	set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 0, 0);
	set_wall(maze, &me, 0, 3);
	set_avatar_position(maze, &me, id);

	visit(maze, &me, 0, 2);
	visit(maze, &other1, 1, 3);
	visit(maze, &other2, 2, 4);
	
	move_t* move = maze_solve(maze, id, &me, log);
	if (move != NULL) {
		printf("id: %d, direction: %d", move->avatar_id, move->direction);
	}
	free(move);
	maze_delete(maze);
	return 0;
}


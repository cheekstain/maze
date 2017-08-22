/*
 * solve_test.c - see avatar_solve.h for details on exported functions
 * 
 * Bashful Brigade: Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avatar_solve.h"
#include "../amazing.h"
#include "../mazestruct/mazestruct.h"
#include "../maze_pointer/maze_pointers.h"
#include "../libcs50/counters.h"

int main()
{
	char* log = "./test_file";
	int id = 0;
	XYPos me = { 2, 4 };
	XYPos* pos = me;
	XYPos other1 = { 1, 4 };
	XYPos other2 = { 2, 3 };

	maze_t* maze = maze_new(5, 5, 3);
	set_wall(maze, pos, 0, 0);
	set_wall(maze, pos, 1, 0);
	set_wall(maze, pos, 3, 0);
	set_avatar_position(maze, pos, id);
	visit(maze, pos, 0, 2);
	visit(maze, &other1, 1, 3);
	visit(maze, &other2, 2, 4);
	draw_maze(maze);

	move_t* move = maze_solve(maze, id, pos, log);
	printf("id: %d, direction: %d", move->avatar_id, move->direction);
	free(move);
	maze_delete(maze);
	return 0;
}


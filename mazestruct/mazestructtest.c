/*
* mazestructtest.c - test for maze structure module
*
* Basic file to test the behavior of the mazestruct module, especially
* its updating and drawing methods.
*
* Emma Hobday, August 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <mazestruct.h>

int main()
{
	//set up maze for simple test
	const int width = 4;
	const int height = 4
	const int num_avatars = 2;


	maze_t *maze = maze_new(width, height, num_avatars);

	maze_draw(maze);
}






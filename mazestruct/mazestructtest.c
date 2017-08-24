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
#include "mazestruct.h"
#include "../amazing.h"


int main()
{
	/*
    printf("%sblack\n", BLACK);
    printf("%sred\n", RED);
    printf("%sgreen\n", GREEN);
    printf("%syellow\n", YELLOW);
    printf("%sblue\n", BLUE);
    printf("%smagenta\n", MAGENTA);
    printf("%scyan\n", CYAN);
    printf("%sgray\n", GRAY);
    printf("%slight red\n", L_RED);
    printf("%slight green\n", L_GREEN);
    printf("%slight yellow\n", L_YEL);
    printf("%slight blue\n", L_BLUE);
    printf("%slight magenta\n", L_MAG);
    printf("%slight cyan\n", L_CYAN);
    printf("%swhite\n", WHITE);
    printf("%slinux reset\n", LIN_RESET);
    */

	//set up maze for simple test
	const int width = 10;
	const int height = 10;
	const int num_avatars = 4;

	printf("Creating maze...\n");

	maze_t *maze = maze_new(width, height, num_avatars);

	XYPos pos_0_0 = {0, 0};
	XYPos pos_0_1 = {0, 1};
	XYPos pos_0_2 = {0, 2};
	XYPos pos_0_3 = {0, 3};
	XYPos pos_1_0 = {1, 0};
	XYPos pos_1_1 = {1, 1};
	XYPos pos_1_2 = {1, 2};
	XYPos pos_2_1 = {2, 1};

	//test setting some south walls to 0 and 1
	printf("Set some south walls\n");
	set_wall(maze, &pos_0_0, 1, 2);
	set_wall(maze, &pos_1_0, 0, 2);

	//test setting some north walls to 0 and 1
	printf("Set some north walls\n");
	set_wall(maze, &pos_1_2, 1, 1);
	set_wall(maze, &pos_0_2, 0, 1);
	set_wall(maze, &pos_0_3, 1, 1);

	//test setting some east walls to 0 and 1
	printf("Set some east walls\n");
	set_wall(maze, &pos_1_0, 1, 3);
	set_wall(maze, &pos_0_1, 0, 3);
	set_wall(maze, &pos_0_3, 1, 3);

	//test setting some west walls to 0 and 1
	printf("Set some west walls\n");
	set_wall(maze, &pos_1_0, 0, 0);
	set_wall(maze, &pos_1_2, 1, 0);

	void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength);


	printf("Put some avatars in the maze\n");
	set_avatar_position(maze, &pos_1_1, 0);
	visit(maze, &pos_1_1, 0, 10);  //av 0, tag str 10
	set_avatar_position(maze, &pos_0_3, 1);
	visit(maze, &pos_0_3, 1, 10);  //av 1, tag str 10
	set_avatar_position(maze, &pos_0_0, 2);
	visit(maze, &pos_0_0, 2, 10);  //av 2, tag str 10
	set_avatar_position(maze, &pos_0_0, 3);

	printf("Testing collisions\n");

	int colliders[10];

	if (is_collision(maze, &pos_0_0, colliders)) {
		printf("-- Collision at 0,0\n");
	}
	else {
		printf("-- No collision at 0,0\n");
	}

	printf("Test visiting and tagging\n");
	visit(maze, &pos_1_2, 4, 10); //tag square 0,3 as avatar 4 with strength 10
	int visitor = get_tagged_by(maze, &pos_0_3);
	int tag_strength = get_tag_strength(maze, &pos_0_3);
	if (visitor != 4) {
		printf("Problem with first visitor\n");
	}
	else {
		printf("-- First visitor successful\n");
	}
	if (tag_strength != 10) {
		printf("Problem with tag strength\n");
	}
	else {
		printf("-- Tag strength successful\n");
	}


	printf("Drawing maze...(and implicitly testing getters)\n");

	draw_maze(maze);

	//printf("Have 0 successfully move east\n");
	set_avatar_position(maze, &pos_2_1, 0);
	visit(maze, &pos_2_1, 0, 11);  //av 0, tag str 11
	set_wall(maze, &pos_1_1, 0, 3);

	draw_maze(maze);

	set_avatar_position(maze, &pos_1_1, 0);
	visit(maze, &pos_1_1, 0, 12);  //av 0, tag str 12
	draw_maze(maze);
	set_avatar_position(maze, &pos_0_1, 0);
	visit(maze, &pos_0_1, 0, 13);
	draw_maze(maze);
	set_avatar_position(maze, &pos_1_0, 2);
	visit(maze, &pos_1_0, 2, 11);
	draw_maze(maze);


	printf("Deleting maze...\n");

	maze_delete(maze);
}






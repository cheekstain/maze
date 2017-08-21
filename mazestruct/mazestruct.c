/*
* mazestruct.c - maze structure module for CS50 final project
* 
* See mazestruct.h for more information
*
* Emma Hobday, August 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../amazing.h"

/**************** local types ****************/
typedef struct mazesquare {
	int north_wall;
	int east_wall;
	int south_wall;
	int west_wall;
	int tagged_by;
	int tag_strength;
	bool *avatar_here;
} mazesquare_t;

/**************** global types ****************/
typedef struct maze {
	mazesquare_t ***array;											//revisit this
	int width;
	int height;	
	int num_avatars;														
} maze_t;

/**************** functions ****************/
maze_t *maze_new(const int width, const int height, const int num_avatars);
int get_north_wall(maze_t *maze, XYPos *pos);
int get_south_wall(maze_t *maze, XYPos *pos);
int get_east_wall(maze_t *maze, XYPos *pos);
int get_west_wall(maze_t *maze, XYPos *pos);
int get_tagged_by(maze_t *maze, XYPos *pos);
int get_tag_strength(maze_t *maze, XYPos *pos);
void set_north_wall(maze_t *maze, XYPos *pos, int new_val);
void set_south_wall(maze_t *maze, XYPos *pos, int new_val);
void set_east_wall(maze_t *maze, XYPos *pos, int new_val);
void set_west_wall(maze_t *maze, XYPos *pos, int new_val);
void set_avatar_position(maze_t *maze, XYPos *pos, int avatar);
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength);
bool is_collision(maze_t *maze, XYPos *pos, int colliding_avatars[]);
void maze_delete(maze_t *maze);
void draw_maze(maze_t *maze);

/**************** static functions ****************/
static mazesquare_t *mazesquare_new(int num_avatars);
static mazesquare_t *get_square_at_coords(maze_t *maze, XYPos *pos);
static int get_num_avatars_here(maze_t *maze, XYPos *pos, int colliding_avatars[]);
static void draw_top_row(int width);
static void draw_floor(int floor_status);
static void draw_wall(int wall_status);
static void draw_people(maze_t *maze, XYPos *pos);


/******************************** mazesquare_new ******************************/
static mazesquare_t *mazesquare_new(int num_avatars)
{	
	mazesquare_t *square = malloc(sizeof(mazesquare_t));

	square->avatar_here = calloc(num_avatars, sizeof(bool));

	for (int i = 0; i < num_avatars; i++) {
		square->avatar_here[i] = false;
	}

	square->north_wall = -1;
	square->south_wall = -1;
	square->east_wall = -1;
	square->west_wall = -1;
	square->tagged_by = -1;
	square->tag_strength = -1;

	return square;
}

/******************************** maze_new ************************************/
maze_t *maze_new(const int width, const int height, const int num_avatars)
{
	//malloc for the maze itself
	maze_t *maze = malloc(sizeof(maze_t));

	//create an empty array (of the x coordinates)
	maze->array = calloc(height, sizeof(mazesquare_t));
	maze->width = width;
	maze->height = height;
	maze->num_avatars = num_avatars;

	//fill it by creating an array for each row and filling it with empty
	//mazesquare structs
	for (int y = 0; y < height; y++) {
		mazesquare_t **inner_array = calloc(width, sizeof(mazesquare_t));
		maze->array[y] = inner_array;

		for (int x = 0; x < width; x++) {
			maze->array[y][x] = mazesquare_new(num_avatars);

			XYPos pos = {x, y};
		

			if (y == 0) { //make the northernmost squares have north walls
				set_north_wall(maze, &pos, 1);
			}

			if (x == 0) { //make the westernmost squares have west walls
				set_west_wall(maze, &pos, 1);
			}

			if (y + 1 == height) { //make the southernmost squares have south walls
				set_south_wall(maze, &pos, 1);
			}	

			if (x + 1 == width)	{ //make the easternmost squares have east walls
				set_east_wall(maze, &pos, 1);
			}
		}
	}
	return maze;
}

/******************************** get_square_at_coords ************************/
static mazesquare_t *get_square_at_coords(maze_t *maze, XYPos *pos)
{
	int x_coord = pos->x;
	int y_coord = pos->y;

	mazesquare_t *square = maze->array[y_coord][x_coord];
	return square;
}

/******************************** get_north_wall ******************************/
int get_north_wall(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->north_wall;
}

/******************************** set_north_wall ******************************/
void set_north_wall(maze_t *maze, XYPos *pos, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);

	square->north_wall = new_val;

	if (pos->y != 0) {
		XYPos new_pos = {pos->x, pos->y - 1};

		mazesquare_t *northern_square = get_square_at_coords(maze, &new_pos);
		northern_square->south_wall = new_val;
	}
}

/******************************** get_south_wall ******************************/
int get_south_wall(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->south_wall;
}

/******************************** set_south_wall ******************************/
void set_south_wall(maze_t *maze, XYPos *pos, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	square->south_wall = new_val;

	if (pos->y + 1 < maze->height) {
		XYPos new_pos = {pos->x, pos->y + 1};

		mazesquare_t *southern_square = get_square_at_coords(maze, &new_pos);
		southern_square->north_wall = new_val;
	}
}

/******************************** get_east_wall ******************************/
int get_east_wall(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->east_wall;
}

/******************************** set_east_wall *******************************/
void set_east_wall(maze_t *maze, XYPos *pos, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	square->east_wall = new_val;

	if (pos->x + 1 < maze->width) {
		XYPos new_pos = {pos->x + 1, pos->y};

		mazesquare_t *eastern_square = get_square_at_coords(maze, &new_pos);
		eastern_square->west_wall = new_val;
	}
}

/******************************** get_west_wall *******************************/
int get_west_wall(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->west_wall;
}

/******************************** set_west_wall *******************************/
void set_west_wall(maze_t *maze, XYPos *pos, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	square->west_wall = new_val;

	if (pos->x != 0) {
		XYPos new_pos = {pos->x - 1, pos->y};

		mazesquare_t *western_square = get_square_at_coords(maze, &new_pos);
		western_square->east_wall = new_val;
	}
}

/******************************** get_tagged_by *******************************/
int get_tagged_by(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->tagged_by;
}

/******************************** get_tag_strength ****************************/
int get_tag_strength(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->tag_strength;
}

/******************************** set_avatar_position *************************/
void set_avatar_position(maze_t *maze, XYPos *pos, int avatar)
{
	//set that avatar being here to true for that square
	mazesquare_t *square = get_square_at_coords(maze, pos);

	square->avatar_here[avatar] = true;

	//now update all squares it could have come from so it's no longer in them
	XYPos north_pos = {pos->x, pos->y - 1};
	XYPos south_pos = {pos->x, pos->y + 1};
	XYPos east_pos = {pos->x + 1, pos->y};
	XYPos west_pos = {pos->x - 1, pos->y};

	if (pos->x != 0) {
		mazesquare_t *west_square = get_square_at_coords(maze, &west_pos);
		west_square->avatar_here[avatar] = false;
	}

	if (pos->x + 1 < maze->width) {
		mazesquare_t *east_square = get_square_at_coords(maze, &east_pos);
		east_square->avatar_here[avatar] = false;
	}

	if (pos->y != 0) {
		mazesquare_t *north_square = get_square_at_coords(maze, &north_pos);
		north_square->avatar_here[avatar] = false;
	}

	if (pos->y + 1 < maze->height) {
		mazesquare_t *south_square = get_square_at_coords(maze, &south_pos);
		south_square->avatar_here[avatar] = false;
	}
}

/******************************** visit ***************************************/
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	square->tagged_by = visitor;
	square->tag_strength = tag_strength;
}

/******************************** get_num_avatars_here ************************/
static int get_num_avatars_here(maze_t *maze, XYPos *pos, int colliding_avatars[])
{
	mazesquare_t *square = get_square_at_coords(maze, pos);

	int first_free_index = 0; //for inserting into colliding_avatars[]

	for (int i = 0; i < maze->num_avatars; i++) {
		if (square->avatar_here[i] == true) {
			colliding_avatars[first_free_index] = i;
			first_free_index += 1;
		}
	}
	return first_free_index;
}

/******************************** is_collision ********************************/
bool is_collision(maze_t *maze, XYPos *pos, int colliding_avatars[])
{
	int num_here = get_num_avatars_here(maze, pos, colliding_avatars);

	if (num_here > 1) {
		return true;
	}
	return false;
}

/******************************** maze_delete *********************************/
void maze_delete(maze_t *maze)
{
	//go through the array and delete each mazesquare struct, then each sub-array
	for (int y = 0; y < maze->height; y++) {
		for (int x = 0; x < maze->width; x++) {
			XYPos pos = {x, y};
			mazesquare_t *square = get_square_at_coords(maze, &pos);
			free(square->avatar_here);
			free(square);
		}
		free(maze->array[y]);
	}

	//delete the array
	free(maze->array);
	//delte the maze itself
	free(maze);
}

/******************************** draw_maze ***********************************/
void draw_maze(maze_t *maze)
{	
	draw_top_row(maze->width);
	
	for (int y = 0; y < maze->height; y++) {

		for (int counter = 1; counter < 4; counter++) {

			for (int x = 0; x < maze->width; x++) {

				XYPos curr_pos = {x, y};

				int wall_status = get_west_wall(maze, &curr_pos);
				draw_wall(wall_status);

				if (counter == 1) {
					printf("       ");
				}

				if (counter == 2) {
					draw_people(maze, &curr_pos);
				}
				if (counter == 3) {
					int floor_status = get_south_wall(maze, &curr_pos);
					draw_floor(floor_status);
				}
			}

			printf("|\n");
		}
	}
}

static void draw_top_row(int width)
{
	for (int i = 0; i < width ; i++) {
		printf(" _______");
	}
	printf("\n");
}

static void draw_wall(int wall_status)
{
	if (wall_status == -1) {
		printf("?");
	}
	if (wall_status == 0) {
		printf(" ");
	}

	if (wall_status == 1) {
		printf("|");
	}
}

static void draw_people(maze_t *maze, XYPos *pos)
{
	printf("   ");

	int avatars_here[maze->num_avatars];

	int num_here = get_num_avatars_here(maze, pos, avatars_here);

	//printf("Num here is %d\n", num_here);

	if (num_here == 0) {
		printf(" ");
	}

	if (num_here == 1) {
		printf("%d", avatars_here[0]);
	}

	if (num_here > 1) {
		printf("*");
	}

	printf("   ");
}


static void draw_floor(int floor_status)
{
	if (floor_status == -1) {
		printf("???????");
	}

	if (floor_status == 0) {
		printf("       ");
	}

	if (floor_status == 1) {
		printf("_______");
	}
}








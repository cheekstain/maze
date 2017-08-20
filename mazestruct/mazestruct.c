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
	int *avatars_here;
} mazesquare_t;

/**************** global types ****************/
typedef struct maze {
	mazesquare_t ***array;											//revisit this
	int width;
	int height;															
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
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength);
void maze_delete(maze_t *maze);
void draw_maze(maze_t *maze);

/**************** static functions ****************/
static mazesquare_t *mazesquare_new(int num_avatars);
static mazesquare_t *get_square_at_coords(maze_t *maze, XYPos *pos);
static void draw_top_row(int width);
static void draw_floor(int floor_status);
static void draw_wall(int wall_status);
static void draw_people(maze_t *maze);


/******************************** mazesquare_new ******************************/
static mazesquare_t *mazesquare_new(int num_avatars)
{	
	mazesquare_t *square = malloc(sizeof(mazesquare_t));

	square->avatars_here = calloc(num_avatars, sizeof(int));

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

/******************************** get_visited *********************************/
int get_tagged_by(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->tagged_by;
}

int get_tag_strength(maze_t *maze, XYPos *pos)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	return square->tag_strength;
}

/******************************** set_visited *********************************/
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength)
{
	mazesquare_t *square = get_square_at_coords(maze, pos);
	square->tagged_by = visitor;
	square->tag_strength = tag_strength;
}

/******************************** maze_delete *********************************/
void maze_delete(maze_t *maze)
{
	//go through the array and delete each mazesquare struct, then each sub-array
	for (int y = 0; y < maze->height; y++) {
		for (int x = 0; x < maze->width; x++) {
			XYPos pos = {x, y};
			mazesquare_t *square = get_square_at_coords(maze, &pos);
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
					printf("     ");
				}

				if (counter == 2) {
					draw_people(maze);
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
		printf(" _____");
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

static void draw_people(maze_t *maze)  // TODO!!!!!!!!!!!!!!
{
	printf("     ");
}


static void draw_floor(int floor_status)
{
	if (floor_status == -1) {
		printf("?????");
	}

	if (floor_status == 0) {
		printf("     ");
	}

	if (floor_status == 1) {
		printf("_____");
	}
}








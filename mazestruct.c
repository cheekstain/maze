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

/**************** global types ****************/
typedef struct maze {
	mazesquare *array[][];											//revisit this
	int width;
	int height;															
} maze_t;

/**************** local types ****************/
typedef struct mazesquare {
	int north_wall;
	int east_wall;
	int south_wall;
	int west_wall;
	bool visited;
} mazesquare_t;

/******************************** mazesquare_new ******************************/
static mazesquare_t *mazesquare_new(void)
{	
	mazesquare_t *square = malloc(sizeof(mazesquare_t));

	square->north_wall = -1;
	square->south_wall = -1;
	square->east_wall = -1;
	square->west_wall = -1;
	square->visited = false;
}

/******************************** maze_new ************************************/
maze_t *maze_new(int x_size, int y_size)
{
	//malloc for the maze itself
	maze_t *maze = malloc(sizeof(maze_t));

	//create an empty array

	//fill it by creating an empty array for each coordinate
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			maze->array[x][y] = mazesquare_new();
		}
	}
}

static mazesquare_t *get_square_at_coords(int x_coord, int y_coord)
{
	mazesquare_t *square = maze->array[x_coord][y_coord];
	return square;
}

/******************************** get_north_wall ******************************/
int get_north_wall(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->north_wall;
}

/******************************** set_north_wall ******************************/
void set_north_wall(int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	square->south_wall = new_val;

	if (y_coord != 0) {
		mazesquare_t *northern_square = get_square_at_coords(x_coord, y_coord - 1);
		northern_square->south_wall = new_val;
	}
}

/******************************** get_south_wall ******************************/
int get_south_wall(int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->south_wall;
}

/******************************** set_south_wall ******************************/
void set_south_wall(int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	square->south_wall = new_val;

	if (y_coord + 1 < maze->height) {
		mazesquare_t *southern_square = get_square_at_coords(x_coord, y_coord + 1);
		southern_square->north_wall = new_val;
	}
}

/******************************** get_east_wall ******************************/
int get_east_wall(int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->east_wall;
}

/******************************** set_east_wall *******************************/
void set_east_wall(int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	square->east_wall = new_val;

	if (x_coord + 1 < maze->width) {
		mazesquare_t *eastern_square = get_square_at_coords(x_coord + 1; y_coord);
		east_square->west_wall = new_val;
	}
}

/******************************** get_west_wall *******************************/
int get_west_wall(int x_coord, int y_coord)
{
	mazesquare_t *square = maze->array[x_coord][y_coord];
	return square->west_wall;
}

/******************************** set_west_wall *******************************/
void set_west_wall(int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	square->west_wall = new_val;

	if (x_coord != 0) {
		mazesquare_t *western_square = get_square_at_coords(x_coord - 1; y_coord);
		east_square->east_wall = new_val;
	}
}

/******************************** get_visited *********************************/
bool get_visited(int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->visited;
}

int get_first_visitor(int x_coord, y_coord)
{
	mazesquare_t *square get_square_at_coords(x_coord, y_coord);
	return square->first_visitor;
}

/******************************** set_visited *********************************/
void set_visited(maze_t *maze, int x_coord, int y_coord, int visitor)
{
	mazesquare_t *square = maze->array[x_coord][y_coord];
	square->visited = true;
	square->first_visitor = visitor;
}

/******************************** maze_delete *********************************/
void maze_delte(maze_t *maze)
{
	//go through the array and delete each mazesquare struct
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			mazesquare_t maze->array[x][y] = mazesquare_new();
		}
	}
	//delete the array
	//delte the maze itself
}




















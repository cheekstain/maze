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
	int tagged_by;
	int tag_strength;
	int *avatars_here;
} mazesquare_t;

/******************************** mazesquare_new ******************************/
static mazesquare_t *mazesquare_new(int num_avatars)
{	
	mazesquare_t *square = malloc(sizeof(mazesquare_t));

	square->avatars_here = calloc(num_avatars, sizeof(int));

	square->north_wall = -1;
	square->south_wall = -1;
	square->east_wall = -1;
	square->west_wall = -1;
	square->visited = false;
	square->tagged_by = -1;
	square->tag_strength =-1;
}

/******************************** maze_new ************************************/
maze_t *maze_new(int width, int height, int num_avatars)
{
	//malloc for the maze itself
	maze_t *maze = malloc(sizeof(maze_t));

	//create an empty array
	maze->array = calloc(width * height, sizeof(mazesquare_t));

	//fill it by creating an empty array for each coordinate
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			maze->array[x][y] = mazesquare_new(num_avatars);
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

/******************************** get_visited *********************************/
int get_tagged_by(int x_coord, y_coord)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->tagged_by;
}

int get_tag_strength()
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	return square->tagged_by;
}

/******************************** set_visited *********************************/
void set_visited(maze_t *maze, int x_coord, int y_coord, int visitor, int tag_strength)
{
	mazesquare_t *square = get_square_at_coords(x_coord, y_coord);
	square->visited = true;
	square->tagged_by = visitor;
	square->tag_strength = tag_strength;
}

/******************************** maze_delete *********************************/
void maze_delte(maze_t *maze)
{
	//go through the array and delete each mazesquare struct
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			mazesquare_t *square = get_square_at_coords(x, y);
			free(square);
		}
	}

	//delete the array
	free(maze->array);
	//delte the maze itself
	free(maze);
}

/******************************** draw_maze ***********************************/
void draw_maze(maze_t *maze)
{
	//go through the array and delete each mazesquare struct
	for (int y = 0; y < height; y++) {
		draw_floor();
		printf("\n");

		for (int x = 0; x < width; x++) {
			
		}
	}
}

void draw_floor()
{
	printf(" _____");
}

void draw_no_floor()
{
	printf("      ");
}

/******************************** draw_maze ***********************************/
/*
 * Draw only the left side and bottom of each square to avoid redrawing walls.
 */
static void draw_square(mazesquare_t *square)
{

}

/* Maze UI will look like this:
 _____ _____ _____
|                 |
|  1              |
|      _____      |
|     |           |
|     |           |
|     |_____      |
|           |     |
|           |  0  |
|_____ _____|_____|




*/


For the first row, draw a 




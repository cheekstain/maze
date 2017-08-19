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

/**************** global types ****************/
typedef struct maze {
	mazesquare_t ***array;											//revisit this
	int width;
	int height;															
} maze_t;

/**************** functions ****************/
maze_t *maze_new(const int width, const int height, const int num_avatars);
int get_north_wall(maze_t *maze, int x_coord, int y_coord);
int get_south_wall(maze_t *maze, int x_coord, int y_coord);
int get_east_wall(maze_t *maze, int x_coord, int y_coord);
int get_west_wall(maze_t *maze, int x_coord, int y_coord);
bool get_visited(maze_t *maze, int x_coord, int y_coord);
int get_tagged_by(maze_t *maze, int x_coord, int y_coord);
int get_tag_strength(maze_t *maze, int x_coord, int y_coord);
void set_north_wall(maze_t *maze, int x_coord, int y_coord, int new_val);
void set_south_wall(maze_t *maze, int x_coord, int y_coord, int new_val);
void set_east_wall(maze_t *maze, int x_coord, int y_coord, int new_val);
void set_west_wall(maze_t *maze, int x_coord, int y_coord, int new_val);
void visit(maze_t *maze, int x_coord, int y_coord, int visitor, int tag_strength);
void maze_delete(maze_t *maze);
void draw_maze(maze_t *maze);

/**************** static functions ****************/
static mazesquare_t *mazesquare_new(int num_avatars);
static mazesquare_t *get_square_at_coords(maze_t *maze, int x_coord, int y_coord);
static void draw_top_row(int width);
static void draw_floor(int floor_status);
static void draw_wall(int wall_status);


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

			if (y == 0) { //make the northernmost squares have north walls
				set_north_wall(maze, x, y, 1);
			}

			if (x == 0) { //make the westernmost squares have west walls
				set_west_wall(maze, x, y, 1);
			}

			if (y + 1 == height) { //make the southernmost squares have south walls
				set_south_wall(maze, x, y, 1);
			}	

			if (x + 1 == width)	{ //make the easternmost squares have east walls
				set_east_wall(maze, x, y, 1);
			}
		}
	}
	return maze;
}

/******************************** get_square_at_coords ************************/
static mazesquare_t *get_square_at_coords(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = maze->array[x_coord][y_coord];
	return square;
}

/******************************** get_north_wall ******************************/
int get_north_wall(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->north_wall;
}

/******************************** set_north_wall ******************************/
void set_north_wall(maze_t *maze, int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	square->south_wall = new_val;

	if (y_coord != 0) {
		mazesquare_t *northern_square = get_square_at_coords(maze, x_coord, y_coord - 1);
		northern_square->south_wall = new_val;
	}
}

/******************************** get_south_wall ******************************/
int get_south_wall(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->south_wall;
}

/******************************** set_south_wall ******************************/
void set_south_wall(maze_t *maze, int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	square->south_wall = new_val;

	if (y_coord + 1 < maze->height) {
		mazesquare_t *southern_square = get_square_at_coords(maze, x_coord, y_coord + 1);
		southern_square->north_wall = new_val;
	}
}

/******************************** get_east_wall ******************************/
int get_east_wall(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->east_wall;
}

/******************************** set_east_wall *******************************/
void set_east_wall(maze_t *maze, int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	square->east_wall = new_val;

	if (x_coord + 1 < maze->width) {
		mazesquare_t *eastern_square = get_square_at_coords(maze, x_coord + 1, y_coord);
		eastern_square->west_wall = new_val;
	}
}

/******************************** get_west_wall *******************************/
int get_west_wall(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = maze->array[x_coord][y_coord];
	return square->west_wall;
}

/******************************** set_west_wall *******************************/
void set_west_wall(maze_t *maze, int x_coord, int y_coord, int new_val)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	square->west_wall = new_val;

	if (x_coord != 0) {
		mazesquare_t *western_square = get_square_at_coords(maze, x_coord - 1, y_coord);
		western_square->east_wall = new_val;
	}
}

/******************************** get_visited *********************************/
bool get_visited(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->visited;
}

/******************************** get_visited *********************************/
int get_tagged_by(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->tagged_by;
}

int get_tag_strength(maze_t *maze, int x_coord, int y_coord)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	return square->tagged_by;
}

/******************************** set_visited *********************************/
void visit(maze_t *maze, int x_coord, int y_coord, int visitor, int tag_strength)
{
	mazesquare_t *square = get_square_at_coords(maze, x_coord, y_coord);
	square->visited = true;
	square->tagged_by = visitor;
	square->tag_strength = tag_strength;
}

/******************************** maze_delete *********************************/
void maze_delete(maze_t *maze)
{
	//go through the array and delete each mazesquare struct, then each sub-array
	for (int y = 0; y < maze->height; y++) {
		for (int x = 0; x < maze->width; x++) {
			printf("x=%d, y=%d\n", x, y);
			mazesquare_t *square = get_square_at_coords(maze, x, y);
			printf("Freeing square at x=%d y=%d\n", x, y);
			free(square);
		}
		printf("NOT Freeing mini array for row y=%d\n", y);
		//free(maze->array[y]);
	}

	printf("Freeing the top level maze array\n");
	//delete the array
	free(maze->array);
	printf("Freeing the maze itself\n");
	//delte the maze itself
	free(maze);
}

/******************************** draw_maze ***********************************/
void draw_maze(maze_t *maze)
{

	printf("Entered draw_maze\n");


	//draw the top roof
	//for each row
		//three passes
			//on pass 1 and 2, iterate through each x and draw its left wall, then a no_floor
			//on pass 3, check whether it has a floor or not and draw that.
	draw_top_row(maze->width);

	printf("Drew top row\n");

	
	for (int y = 0; y < maze->height; y++) {

		for (int counter = 1; counter < 4; counter++) {

			for (int x = 0; x < maze->width; x++) {

				//printf("y is %d, x is %d, counter is %d\n", y, x, counter);

				int wall_status = get_west_wall(maze, x, y);
				draw_wall(wall_status);

				if (counter != 3) {
					//printf("Counter is not three\n");
					printf("     ");
				}
				if (counter == 3) {
					printf("Counter is three\n");
					int floor_status = get_south_wall(maze, x, y);
					draw_floor(floor_status);
				}
			}

			printf("|\n");

			//printf("About to increment counter\n");
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

/* Maze UI will look like this:
1 _____ _____ _____
2|     ?     ?     |
3|  1  ?     ?     |
4|     ?_____??????|
5|     |           |
6|     |           |
7|     |_____      |
8|           |     |
9|           |  0  |
0|_____ _____|_____|

*/








/* 
 * mazestruct.h - header file for CS50 'mazestruct' module
 * 
 * Mazestruct holds a structure representing a maze, as well as
 * all the functions required to get and set information from that maze.
 * 
 * It also contains functionality allowing it to draw the maze.
 *
 * Emma Hobday, August 2017
 */

#ifndef __MAZESTRUCT_H
#define __MAZESTRUCT_H

#include <stdbool.h>


/**************** global types ****************/
typedef struct maze maze_t;	// opaque to users of the module

/**************** functions ****************/
maze_t *maze_new(const int width, const int height, const int num_avatars);


/**************** getters ****************/

int get_north_wall(maze_t *maze, int x_coord, int y_coord);

int get_south_wall(maze_t *maze, int x_coord, int y_coord);

int get_east_wall(maze_t *maze, int x_coord, int y_coord);

int get_west_wall(maze_t *maze, int x_coord, int y_coord);

bool get_visited(maze_t *maze, int x_coord, int y_coord);

int get_tagged_by(maze_t *maze, int x_coord, y_coord);

int get_tag_strength(maze_t *maze, int x_coord, y_coord);


/**************** setters ****************/

void set_north_wall(maze_t *maze, int x_coord, int y_coord);

void set_south_wall(maze_t *maze, int x_coord, int y_coord);

void set_east_wall(maze_t *maze, int x_coord, int y_coord, int new_val);

void set_west_wall(maze_t *maze, int x_coord, int y_coord, int new_val);

/**************** other ****************/

void visit(maze_t *maze, int x_coord, int y_coord, int visitor, int tag_strength);

void maze_delte(maze_t *maze);

void draw_maze(maze_t *maze);





#endif //MAZESTRUCT_H
/* 
 * mazestruct.h - header file for CS50 'mazestruct' module
 * 
 * Mazestruct holds a structure representing a maze, as well as
 * all the functions required to get and set information from that maze.
 * 
 * It also contains functionality allowing it to draw the maze in an external 
 * window.
 *
 * Emma Hobday, August 2017
 */

#ifndef __MAZESTRUCT_H
#define __MAZESTRUCT_H

#import "../amazing.h"

/**************** global types ****************/
typedef struct maze maze_t;	// opaque to users of the module

/**************** functions ****************/


/**************** getters ****************/

/*
 * Provided with a maze and x and y coordinates, return the wall status to the 
 * north (ie the top wall of the square).
 *
 * Returns 0 if there is no wall, 1 if there is a wall, and -1 if the wall
 * status is unknown.
 */
int get_north_wall(maze_t *maze, XYPos *pos);

/*
 * Provided with a maze and x and y coordinates, return the wall status to the 
 * south (ie the bottom wall of the square).
 *
 * Returns 0 if there is no wall, 1 if there is a wall, and -1 if the wall
 * status is unknown.
 */
int get_south_wall(maze_t *maze, XYPos *pos);

/*
 * Provided with a maze and x and y coordinates, return the wall status to the 
 * east (ie the right wall of the square).
 *
 * Returns 0 if there is no wall, 1 if there is a wall, and -1 if the wall
 * status is unknown.
 */
int get_east_wall(maze_t *maze, XYPos *pos);

/*
 * Provided with a maze and x and y coordinates, return the wall status to the 
 * west (ie the left wall of the square).
 *
 * Returns 0 if there is no wall, 1 if there is a wall, and -1 if the wall
 * status is unknown.
 */
int get_west_wall(maze_t *maze, XYPos *pos);

/*
 * Provided with a maze and x and y coordinates, return which avatar, if any, 
 * was the first to visit it and has thus "tagged it", leaving its trail behind.
 *
 * Returns the number of that avatar (0-9) if any avatar has tagged it, or -1
 * if the square is still unvisited.
 */
int get_tagged_by(maze_t *maze, XYPos *pos);

/*
 * Provided with a maze and x and y coordinates, return the strength of the tag
 * left by the tagging avatar on this square.
 *
 * Since the tag strengths left by the avatars start at zero, a tag_strength of
 * -1 means the square is unvisited.
 */
int get_tag_strength(maze_t *maze, XYPos *pos);


/**************** setters ****************/

/*
 * Provided with a maze and x and y coordinates, set the wall status to the 
 * north (ie the top wall of the square) to the given new_val.
 *
 * new_val should be 0 if there is no wall, 1 if there is a wall, and -1 if the 
 * wall status is unknown.
 */
void set_north_wall(maze_t *maze, XYPos *pos, int new_val);

/*
 * Provided with a maze and x and y coordinates, set the wall status to the 
 * south (ie the bottom wall of the square) to the given new_val.
 *
 * new_val should be 0 if there is no wall, 1 if there is a wall, and -1 if the 
 * wall status is unknown.
 */
void set_south_wall(maze_t *maze, XYPos *pos, int new_val);

/*
 * Provided with a maze and x and y coordinates, set the wall status to the 
 * east (ie the right wall of the square) to the given new_val.
 *
 * new_val should be 0 if there is no wall, 1 if there is a wall, and -1 if the 
 * wall status is unknown.
 */
void set_east_wall(maze_t *maze, XYPos *pos, int new_val);

/*
 * Provided with a maze and x and y coordinates, set the wall status to the 
 * west (ie the left wall of the square) to the given new_val.
 *
 * new_val should be 0 if there is no wall, 1 if there is a wall, and -1 if the 
 * wall status is unknown.
 */
void set_west_wall(maze_t *maze, XYPos *pos, int new_val);

/**************** other ****************/

/*
 * Creates a maze with the provided width and height, and creates a mazesquare
 * for every coordinate within it. 
 *
 * Additionally, sets the outer walls of squares at the edges of the maze to be
 * definitely walls instead of unknowns, since the maze is by definition closed.
 */
maze_t *maze_new(const int width, const int height, const int num_avatars);

/*
 * Called when an avatar is visiting a square that it must tag, usually because
 * it is the first one to visit that square.
 *
 * The parameter visitor is the id of the avatar that is visiting the square,
 * and the tag_strength is their current step number; it starts at 0 and
 * increases with every step that avatar makes.
 */
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength);

/*
 * Frees the maze as well as all of the allocated structures within it. No 
 * freeing is necessary beyond calling maze_delete.
 */
void maze_delete(maze_t *maze);

/*
 * Given a maze, draw_maze will print out ASCII art of the maze as it is at the 
 * instant of draw_maze's call to a window opened by gtk. For best use, 
 * draw_maze should be called every time there is an update to the maze, such as
 * a new wall being discovered or an avatar moving.
 *
 * This is an example of what the maze art looks like:
 * _____ _____ _____
 *|     ?     ?     |  where | and _ are walls, ? is for walls that are
 *|  1  ?     ?     |  currently unknown, and the numbers 0-9 represent the
 *|     ?_____??????|  avatars of the corresponding ids.
 *|     |           |
 *|     |           |
 *|     |_____      |
 *|           |     |
 *|           |  0  |
 *|_____ _____|_____|
 */
void draw_maze(maze_t *maze);


#endif //MAZESTRUCT_H

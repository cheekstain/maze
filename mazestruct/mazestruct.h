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

#include <stdbool.h>
#include "../amazing.h"

/**************** global types ****************/
typedef struct maze maze_t;	// opaque to users of the module

/**************** functions ****************/


/******************************** get_wall ************************************/
/*
 * Provided with a maze and position, return the wall status of the indicated 
 * direction, given by the param `wall` where 0 is West, 1 is North, 2 is South
 * and 3 is East.
 *
 * Returns 0 if there is no wall, 1 if there is a wall, and -1 if the wall
 * status is unknown.
 */
int get_wall(maze_t *maze, XYPos *pos, int wall);

/******************************** get_tagged_by *******************************/
/*
 * Provided with a maze and position, return which avatar, if any, was the first 
 * to visit it and has thus "tagged it", leaving its trail behind.
 *
 * Returns the number of that avatar (0-9) if any avatar has tagged it, or -1
 * if the square is still unvisited.
 */
int get_tagged_by(maze_t *maze, XYPos *pos);

/******************************** get_tag_strength ****************************/
/*
 * Provided with a maze and position, return the strength of the tag left by the 
 * tagging avatar on this square.
 *
 * Since the tag strengths left by the avatars start at zero, a tag_strength of
 * -1 means the square is unvisited.
 */
int get_tag_strength(maze_t *maze, XYPos *pos);

/******************************** set_wall ************************************/
/*
 * Provided with a maze and position, set the wall status of the indicated 
 * direction, given by the param `wall` where 0 is West, 1 is North, 2 is South
 * and 3 is East, to the given new_val.
 *
 * new_val should be 0 if there is no wall, 1 if there is a wall, and -1 if the 
 * wall status is unknown.
 */
void set_wall(maze_t *maze, XYPos *pos, int new_val, int wall);

/**************** other ****************/

/******************************** maze_new ************************************/
/*
 * Creates a maze with the provided width and height, and creates a mazesquare
 * for every coordinate within it. 
 *
 * Additionally, sets the outer walls of squares at the edges of the maze to be
 * definitely walls instead of unknowns, since the maze is by definition closed.
 */
maze_t *maze_new(const int width, const int height, const int num_avatars);

/******************************** visit ***************************************/
/*
 * Called when an avatar is visiting a square that it must tag, usually because
 * it is the first one to visit that square.
 *
 * The parameter visitor is the id of the avatar that is visiting the square,
 * and the tag_strength is their current step number; it starts at 0 and
 * increases with every step that avatar makes.
 */
void visit(maze_t *maze, XYPos *pos, int visitor, int tag_strength);

/******************************** set_avatar_position *************************/
/*
 * Called any time an avatar successfully moves from one square to another, this
 * function updates the maze to reflect that the avatar is now in the new square
 * and not in the old one.
 *
 * The position object provided should refer to the position of the new square
 * that the avatar inhabits.
 */
void set_avatar_position(maze_t *maze, XYPos *pos, int avatar);

/******************************** is_collision ********************************/
/*
 * Takes in an empty array that has at least as many slots as there are avatars
 * in the maze. In running is_collision, it will be filled with every avatar 
 * that is on the squre associated with the given pos. It may end up still 
 * empty if there are no avatars on that square.
 * 
 * Returns true if there are two or more avatars in the same square; returns
 * false otherwise.
 */
bool is_collision(maze_t *maze, XYPos *pos, int colliding_avatars[]);

/******************************** maze_delete *********************************/
/*
 * Frees the maze as well as all of the allocated structures within it. No 
 * freeing is necessary beyond calling maze_delete.
 */
void maze_delete(maze_t *maze);

/******************************** draw_maze *********************************/
/*
 * Given a maze, draw_maze will print out ASCII art of the maze as it is in the
 * terminal, clearing the terminal before each printout. For best use, 
 * draw_maze should be called every time there is an update to the maze, such as
 * a new wall being discovered or an avatar moving.
 *
 * This is an example of what the maze art looks like:
 * _____ _____ _____
 *|     ?     ?     |  where | and _ are walls, ? is for walls that are
 *|  1  ?     ?     |  currently unknown, and the numbers 0-9 represent the
 *|     ?_____??????|  avatars of the corresponding ids.
 *|     |           |
 *|     |           |  Note: the printable version's squares are 7 chars across, 
 *|     |_____      |  not 5 like in the diagram on the left, due to slightly
 *|           |     |  different spacing between editors and the terminal.
 *|           |  0  |
 *|_____ _____|_____|
 */

void draw_maze(maze_t *maze);


#endif //MAZESTRUCT_H

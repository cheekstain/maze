/*
 * avatar_solve.h - functions related to calculating and returning the
 * next best move
 * 
 * Bashful Brigade, August 2017
 */

#ifndef __AVATAR_SOLVE_H
#define __AVATAR_SOLVE_H

/* make_move is called within the avatar thread and determines the move to 
 * make, communicates with the server, makes the move and updates the 
 * visualization and the log file.
 *
 * it takes three parameters, the maze struct, ...?
 */
void make_move(maze_t maze, lastmove_t move, avatars_t avatars, char* log);

#endif // __AVATAR_SOLVE_H

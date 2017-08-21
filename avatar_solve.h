/*
 * avatar_solve.h - functions related to calculating and returning the
 * next best move
 * 
 * Bashful Brigade, August 2017
 */

#ifndef __AVATARSOLVE_H
#define __AVATARSOLVE_H

/* make_move is called within the avatar thread and determines the move to 
 * make, communicates with the server, makes the move and updates the 
 * visualization and the log file.
 *
 */

void make_move(maze_t* maze, avatars_t* avatars, char* log, int my_id);

/* check_previous is called in order to check if the previous move was sucessful
 * and to update the maze/logfile with it.
 *
 */

void check_previous(maze_t* maze, lastmove_t* move, char* log, int strength);

#endif // __AVATAR_SOLVE_H

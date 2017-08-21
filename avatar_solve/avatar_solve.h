/*
 * avatar_solve.h - functions related to calculating and returning the
 * next best move
 * 
 * Bashful Brigade, August 2017
 */

#ifndef __AVATARSOLVE_H
#define __AVATARSOLVE_H

/* the move structure contains the information needed to send a move to the
 * server, namely the direction attempted and the id of the avatar.
 */
typedef struct move {
	int avatar_id;
	int direction; // 0 west, 1 north, 2 south, 3 east
} move_t;

/* check_previous is called in order to check if the previous move was 
 * sucessful.
 * 
 * it updates: 
 * 	logfile - move failes/new walls, executed moves, and followings
 * 	following counter
 * 	maze structure - walls, avatar positions, tags
 *	redraws visual interface
 */
void check_previous(maze_t* maze, lastmove_t* move, char* log, 
					int strength, counters_t* followers);

/* maze_solve attempts to move on another avatar's path if possible or
 * picks a random open direction to move towards, priority given to unexplored 
 * directions. it updates the log file with the attempt.
 *
 * it returns a move struct with information necessary to send to the server.
 */

move_t* maze_solve(maze_t* maze, int id, XYPos* pos, char* log);

/* leader_solve backtracks along the avatar's own trail. it updates the log file
 * with the attempt.
 *
 * it returns a move struct with information necessary to send to the server.
 */
move_t* leader_solve(maze_t* maze, int id, XYPos* pos, char* log);

/* follower_solve checks if there is a third avatar's path we can move onto,
 * check if it belongs to the leader of our leader, and moves onto it if so.
 * otherwise, it continues to follow the path to a higher strength. it updates 
 * the log file with the attempt.
 * 
 * it returns a move struct with information necessary to send to the server.
 */
move_t* follower_solve(maze_t* maze, int id, XYPos* pos, counters_t* followers, 
								char* log);

#endif // __AVATAR_SOLVE_H

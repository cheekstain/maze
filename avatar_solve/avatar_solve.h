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
 * input:
 * 	maze_t* maze 
 * 	lastmove_t* move - the structure which contains all the information
 * 		about the previous move executed
 * 	char* log - name of the log file
 * 	int strength - the strength that the previous avatar is on, to tag
 * 	counters_t* followers - the counters struct which holds all the 
 * 		id of avatars and who they're following
 *
 * it updates: 
 * 	log file - writes move fails/new walls, executed moves, and followings
 * 	counters_t* followers - if a new path has been found
 * 	maze_t* maze - updates walls, avatar positions, tags, if necessary
 *	redraws visual interface
 */
void check_previous(maze_t* maze, lastmove_t* move, char* log, 
					int strength, counters_t* followers);

/* maze_solve attempts to move on another avatar's path if possible or
 * picks a random open direction to move towards, priority given to unexplored 
 * directions. it updates the log file with the attempt.
 *
 * input: 
 * 	maze_t* maze 
 * 	int id - the id of the current avatar
 * 	XYPos* pos - the position of the current avatar
 * 	char* log - the name of the log file
 *
 * output:
 * 	move_t* move - information about desired move to send to the server
 */

move_t* maze_solve(maze_t* maze, int id, XYPos* pos, char* log);

/* leader_solve backtracks along the avatar's own trail. it updates the log file
 * with the attempt.
 *
 * input:
 * 	maze_t* maze 
 * 	int id - the id of the current avatar
 * 	XYPos* pos - the position of the current avatar
 * 	char* log - the name of the log file
 *
 * output:
 * 	move_t* sove - information about desired move to send to the server
 */
move_t* leader_solve(maze_t* maze, int id, XYPos* pos, char* log);

/* follower_solve checks if there is a third avatar's path we can move onto,
 * check if it belongs to the leader of our leader, and moves onto it if so.
 * otherwise, it continues to follow the path to a higher strength. it updates 
 * the log file with the attempt.
 *
 * input: 
 * 	maze_t* maze
 * 	int id - the id of the current avatar
 * 	XYPos* pos - the position of the current avatar
 * 	counters_t* followers - struct that holds all the current followers
 * 	char* log - the name of the log file
 * 
 * output:
 * 	move_t* move - information about desired move to send to the server
 */
move_t* follower_solve(maze_t* maze, int id, XYPos* pos, counters_t* followers, 
								char* log);

#endif // __AVATAR_SOLVE_H

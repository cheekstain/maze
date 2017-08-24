/*
 * avatar_solve.c - see avatar_solve.h for details on exported functions
 * 
 * Bashful Brigade: Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../amazing.h"
#include "../mazestruct/mazestruct.h"
#include "../maze_pointers.h"
#include "../libcs50/counters.h"
#include "avatar_solve.h"

/****************** global types ******************/
/* see avatar_solve.h for exported types */

/*************** global functions ***************/
/* see avatar_solve.h for comments about exported functions */

/*************** local functions ****************/

bool is_following(int me, int to_find, counters_t* followers);
bool is_pos_equal(XYPos* before, XYPos* after);
bool is_pos_valid(XYPos* pos);

XYPos* get_adjacent_pos(XYPos* pos, int dir);
char* get_dir(int dir);

void log_wall(lastmove_t* move, char* log);
void log_move(lastmove_t* move, char* log);
void log_following(int me, int following, char* log);
void log_attempt(int id, int dir, XYPos* pos, char* log);

void check_previous_args(maze_t* maze, lastmove_t* move, int strength, 
											counters_t* followers);
void check_maze_solve_args(maze_t* maze, int id, XYPos* pos);
void check_leader_solve_args(maze_t* maze, int id, XYPos* pos);
void check_follower_solve_args(maze_t* maze, int id, XYPos* pos, 
												counters_t* followers);


/*************** check_previous() ***************/
void check_previous(maze_t* maze, lastmove_t* move, char* log, 
				int strength, counters_t* followers)
{
	if (move == NULL) {
		fprintf(stderr, "check_previous error: null last move\n");
		exit(1);
	}

	int prev_id = move->avatarID;

    if (prev_id != -1) { // not the first move 
    	int prev_dir = move->direction;
		XYPos* after = move->after;
		XYPos* before = move->before;

		if (prev_dir != 8) {
			check_previous_args(maze, move, strength, followers);

		    if (is_pos_equal(after, before)) {
		    	// move has not been made, must be a wall
		    	log_wall(move, log);
		    	set_wall(maze, before, 1, prev_dir);

		    } else { // move has been made
		    	log_move(move, log);
		    	set_wall(maze, before, 0, prev_dir);
		    	set_avatar_position(maze, after, prev_id); 

		    	int tagger = get_tagged_by(maze, after);

		    	if (tagger == -1 || is_following(tagger, prev_id, 
		    											followers)) { 
		    		// unvisited or previously visited by me or below
		    		// me on chain
		    		visit(maze, after, prev_id, strength); // tag square
		    	} else { // found path, set following of avatar to tagger
		    		counters_set(followers, prev_id, tagger); 
		    		log_following(prev_id, tagger, log);

		    	}	
		    }
    	}
        draw_maze(maze); // update maze
    }
}


/****************** functions for choosing moves ******************/

/****************** maze_solve() ******************/
move_t* maze_solve(maze_t* maze, int id, XYPos* pos, 
							counters_t* followers, char* log)
{	
	check_maze_solve_args(maze, id, pos);

	XYPos* final_pos = pos;
	int strength = -1;
	int dir = 4;
	int wall;
	
	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i);
			
		if (wall == 0) { // if there is no wall, check for path
			XYPos *new_pos = get_adjacent_pos(pos, i);	
			int tagger = get_tagged_by(maze, new_pos);
			
			if (!is_following(tagger, id, followers)) { 
			// if the tagger isn't me or my follower
				int new_strength = get_tag_strength(maze, 
								new_pos);

				if (new_strength > strength) {
					// if the tag strength is higher
					
					if (!is_pos_equal(final_pos, pos)) {
						free(final_pos);
					}

					strength = new_strength; 
					dir = i;
					final_pos = new_pos;

				} else {
				    free(new_pos);
				}

			} else{
			    free(new_pos);
			}
	
		} else if (wall == -1 && strength == -1) {
			// unknown wall and no path found yet
			if (!is_pos_equal(final_pos, pos)) {
				free(final_pos);
			}

			final_pos = get_adjacent_pos(pos, i);
			dir = i;
		} 
	}

	strength = get_tag_strength(maze, pos);
	if (is_pos_equal(final_pos, pos)) { // no new pos found so far
		// only options are old squares tagged by me or my follower
		// in a dead end
		// must backtrace
		for (int i = 0; i < 4; i++) {
			wall = get_wall(maze, pos, i);

			if (wall == 0) {
				XYPos *new_pos = get_adjacent_pos(pos, i);
				int new_strength = get_tag_strength(maze, new_pos);

				if (new_strength < strength) {

					if (!is_pos_equal(final_pos, pos)) {
						free(final_pos);
					}

					strength = new_strength;
					dir = i;
					final_pos = new_pos;
					
				} else {
					free(new_pos);
				}
			}
		}
	}

	if (dir == 4) {
		fprintf(stderr, "maze_solve error: no new direction found\n");
		exit(2);
	}

	if (is_pos_equal(final_pos, pos)) {
		fprintf(stderr, "maze_solve error: no new position found\n");
		exit(2);
	}

	if (!is_pos_valid(final_pos)) {
		fprintf(stderr, "maze_solve error: pos out of range\n");
		exit(2);
	}

	log_attempt(id, dir, final_pos, log);
	move_t* attempt = malloc(sizeof(move_t));
	attempt->avatar_id = id;
	attempt->direction = dir;

	return attempt;
}


/****************** leader_solve() ******************/
move_t* leader_solve(maze_t* maze, int id, XYPos* pos, char* log)
{
	check_leader_solve_args(maze, id, pos);

	XYPos* final_pos = pos;
	int dir = 4;
	int strength = get_tag_strength(maze, pos);
	int wall;

	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i); // get status of that wall;

		if (wall == 0) { // if there is no wall
			XYPos* new_pos = get_adjacent_pos(pos, i);
			int tagger = get_tagged_by(maze, new_pos);

			if (tagger == id) { // if tagger is me
				int new_strength = get_tag_strength(maze, 
								new_pos);

				if (new_strength < strength) {
					// go to lower strength square
					if (!is_pos_equal(final_pos, pos)) {
						free(final_pos);
					}

					strength = new_strength;
					dir = i;
					final_pos = new_pos;

				} else {
					free(new_pos);
				}

			} else {
				free(new_pos);
			}
		}
	}

	if (dir == 4) {
		fprintf(stderr, "leader_solve error: no new direction found\n");
		exit(3);
	}

	if (is_pos_equal(final_pos, pos)) {
		fprintf(stderr, "leader_solve error: no new position found\n");
		exit(3);
	}

	if (!is_pos_valid(final_pos)) {
		fprintf(stderr, "leader_solve error: pos out of range\n");
		exit(3);
	}

	log_attempt(id, dir, final_pos, log);
	move_t* attempt = malloc(sizeof(move_t));
	attempt->avatar_id = id;
	attempt->direction = dir;

	return attempt;

}


/****************** follower_solve() ******************/
move_t* follower_solve(maze_t* maze, int id, XYPos* pos, 
							counters_t* followers, char* log)
{
	check_follower_solve_args(maze, id, pos, followers);

	XYPos* final_pos = pos;
	int dir = 4;
	int strength = get_tag_strength(maze, pos);
	int following = counters_get(followers, id);
	bool found_new = false;
	int wall;

	// collision case with leader
	int my_square = get_tagged_by(maze, pos);
	int colliders[10];
	if (is_collision(maze, pos, colliders) && my_square == following) {
		dir = 8;
		log_attempt(id, dir, final_pos, log);
		move_t* attempt = malloc(sizeof(move_t));
		attempt->avatar_id = id;
		attempt->direction = dir;

		return attempt;
	}

	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i);

		if (wall == 0) { // no wall
			XYPos* new_pos = get_adjacent_pos(pos, i);
			int tagger = get_tagged_by(maze, new_pos);

			if (tagger != following && tagger != id) {
				// tagger is a third avatar
								
				if (is_following(following, tagger, followers)) { 
					// switch path if tagger is above me in 
					// the chain
					if (!is_pos_equal(final_pos, pos)) {
						free(final_pos);
					}

					final_pos = new_pos;
					dir = i;
					found_new = true;

				} else {
					free(new_pos);
				}

			} else if (tagger == following && !found_new) {
				// tagger was the avatar i'm following 
				// and i haven't found a new path yet
				int new_strength = get_tag_strength(maze, 
								  new_pos);
				
				if (new_strength > strength) {
					if (!is_pos_equal(final_pos, pos)) {
						free(final_pos);
					}

					strength = new_strength;
					dir = i;
					final_pos = new_pos;

				} else {
					free(new_pos);
				}

			} else {
				free(new_pos);
			}
		}
	}

	if (dir == 4) {
		fprintf(stderr, "follower_solve error: no new direction found\n");
		exit(4);
	}

	if (is_pos_equal(final_pos, pos)) {
		fprintf(stderr, "follower_solve error: no new position found\n");
		exit(4);
	}

	if (!is_pos_valid(final_pos)) {
		fprintf(stderr, "follower_solve error: pos out of range\n");
		exit(4);
	}

	log_attempt(id, dir, final_pos, log);
	
	move_t* attempt = malloc(sizeof(move_t));
	attempt->avatar_id = id;
	attempt->direction = dir;

	return attempt;
}


/****************** helper/local functions *******************/

/* is_following is a recursive helper funcrtion that returns true if
 * the to_find id is above me in the follower chain, or false 
 * otherwise.
 */

bool is_following(int me, int to_find, counters_t* followers)
{
	int leader = counters_get(followers, me); // who i'm following 

	if (leader == to_find || me == to_find) {
		return true;

	} else if (leader == me || leader < 0) {
		return false;

	} else {
		return is_following(leader, to_find, followers);
	}
}

/* is_pos_equal is a helper function that returns true if the two XYPos
 * are equal and false otherwise.
 */
bool is_pos_equal(XYPos* before, XYPos* after)
{
    if ((before->x == after->x) && (before->y == after->y)) {
    	return true;
    }
    return false;
} 

/* is_pos_valid is a helper function that returns true if the x and y
 * of the pos are between 0 and 99. otherwise, it returns false.
 */
bool is_pos_valid(XYPos* pos)
{	
	if (pos->x > 99 || pos->y > 99) {
		return false;
	}
	return true;
}

/* get_adjacent_pos returns the pos of the square at a certain adjacent
 * direction.
 */
XYPos* get_adjacent_pos(XYPos* pos, int dir) 
{
	uint32_t x = pos->x;
	uint32_t y = pos->y;

	if (dir == 0) { // west
		x -= 1;

	} else if (dir == 1) { // north
		y -= 1;

	} else if (dir == 2) { // south
		y += 1;

	} else { // east
		x += 1;
	}
	
	XYPos* adjacent = malloc(sizeof(XYPos));
	adjacent->x = x;
	adjacent->y = y;
	return adjacent;
}

/* get_dir takes an int dir (0 - west, 1 - north, 2 - south, 3 - east)
 * and returns a char string
 */
char* get_dir(int dir) {
	char* string;

	if (dir == 0) {
		string = "West";

	} else if (dir == 1) {
		string = "North";

	} else if (dir == 2) {
		string = "South";

	} else {
		string = "East";
	}

	return string;
}

/* log_wall is a function that updates the log with a wall if the move
 * has failed.
 *
 * it prints: "Move failed. [direction] wall added to (x, y)."
 */
void log_wall(lastmove_t* move, char* log)
{
	FILE *fp = fopen(log, "a");

	uint32_t x = move->before->x;
	uint32_t y = move->before->y;
	char* dir = get_dir(move->direction);

	fprintf(fp, "Move failed. %s wall added to (%d, %d).\n", dir, x, y);
	fclose(fp);
}

/* log_move is a function that updates the log with an avatar move if it
 * was sucessful.
 *
 * it prints: "Avatar [id] moved to (x, y)."
 */
void log_move(lastmove_t* move, char* log)
{
	FILE *fp = fopen(log, "a");

	int id = move->avatarID;
	uint32_t x = move->after->x;
	uint32_t y = move->after->y;

	fprintf(fp, "Avatar %d moved to (%d, %d).\n", id, x, y);
	fclose(fp);
}

/* log_following is a function that updates the log with who an avatar is
 * following if it happens on that path
 *
 * it prints: "Avatar [id] is on avatar [id]'s path."
 */
void log_following(int me, int following, char* log)
{
	FILE *fp = fopen(log, "a");

	fprintf(fp, "Avatar %d is on avatar %d's path.\n", me, following);
	fclose(fp);
}

/* log_attempt is a function that updates the log with an avatar's move
 * attempt
 *
 * it prints: "Avatar [id] attempted to move [direction] to (x, y)."
 */
void log_attempt(int id, int attempt_dir, XYPos* pos, char* log)
{
	FILE *fp = fopen(log, "a");
	
	uint32_t x = pos->x;
	uint32_t y = pos->y;
	
	if (attempt_dir == 8) {
		fprintf(fp, "Avatar %d stays at (%d, %d).\n", id, x, y);
	} else {
		char* dir = get_dir(attempt_dir);
		fprintf(fp, "Avatar %d attempted to move %s to (%d, %d).\n", 
							   id, dir, x, y);
	}
	free(pos);
	fclose(fp);
}

/* check_previous_args takes the arguments passed into check_prevous
 * and validates them. it exits with error code 1 if any are 
 * encountered.
 */
void check_previous_args(maze_t* maze, lastmove_t* move, int strength, 
											counters_t* followers)
{
	int prev_id = move->avatarID;
	int prev_dir = move->direction;
	XYPos* after = move->after;
	XYPos* before = move->before;

	if (maze == NULL) {
		fprintf(stderr, "check_previous error: null maze\n");
		exit(1);
	}

	if (after == NULL || before == NULL) {
		fprintf(stderr, "check_previous error: null position\n");
		exit(1);
	}

	if (followers == NULL) {
		fprintf(stderr, "check_previous error: null counters\n");
		exit(1);
	}

	if (prev_id < 0) {
		fprintf(stderr, "check_previous error: negative id\n");
		exit(1);
	}

	if (prev_dir < 0 || prev_dir > 3) {
		fprintf(stderr, "check_previous error: direction out of range\n");
		exit(1);
	}

	if (strength < 0) {
		fprintf(stderr, "check_previous error: negative strength\n");
		exit(1);
	}

	if (!is_pos_valid(before) || !is_pos_valid(after)) {
		fprintf(stderr, "check_previous error: pos out of range\n");
		exit(1);
	}
}

/* check_maze_solve_args takes the arguments passed into maze_solve
 * and validates them. it exits with error code 2 if any are 
 * encountered.
 */
void check_maze_solve_args(maze_t* maze, int id, XYPos* pos)
{
	if (maze == NULL) {
		fprintf(stderr, "maze_solve error: null maze\n");
		exit(2);
	}

	if (pos == NULL) {
		fprintf(stderr, "maze_solve error: null position\n");
		exit(2);
	}

	if (id < 0) {
		fprintf(stderr, "maze_solve error: negative id\n");
		exit(2);
	}

	if (!is_pos_valid(pos)) {
		fprintf(stderr, "maze_solve error: pos out of range\n");
		exit(2);
	}
}

/* check_leader_solve_args takes the arguments passed into leader_solve
 * and validates them. it exits with error code 3 if any are 
 * encountered.
 */
void check_leader_solve_args(maze_t* maze, int id, XYPos* pos)
{
	if (maze == NULL) {
		fprintf(stderr, "leader_solve error: null maze\n");
		exit(3);
	}

	if (pos == NULL) {
		fprintf(stderr, "leader_solve error: null position\n");
		exit(3);
	}

	if (id < 0) {
		fprintf(stderr, "leader_solve error: negative id\n");
		exit(3);
	}

	if (!is_pos_valid(pos)) {
		fprintf(stderr, "leader_solve error: pos out of range\n");
		exit(3);
	}
}

/* check_follower_solve_args takes the arguments passed into follower_solve
 * and validates them. it exits with error code 4 if any are 
 * encountered.
 */
void check_follower_solve_args(maze_t* maze, int id, XYPos* pos, 
												counters_t* followers)
{
	if (maze == NULL) {
		fprintf(stderr, "follower_solve error: null maze\n");
		exit(4);
	}

	if (pos == NULL) {
		fprintf(stderr, "follower_solve error: null position\n");
		exit(4);
	}

	if (followers == NULL) {
		fprintf(stderr, "follower_solve error: null counters\n");
		exit(4);
	}

	if (id < 0) {
		fprintf(stderr, "follower_solve error: negative id\n");
		exit(4);
	}

	if (!is_pos_valid(pos)) {
		fprintf(stderr, "follower_solve error: pos out of range\n");
		exit(4);
	}
}


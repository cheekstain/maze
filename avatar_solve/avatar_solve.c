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
#include "../maze_pointer/maze_pointers.h"
#include "../libcs50/counters.h"
#include "avatar_solve.h"

/****************** global types ******************/
/* see avatar_solve.h for exported types */

/*************** global functions ***************/
/* see avatar_solve.h for comments about exported functions */

/*************** local functions ****************/
bool is_pos_equal(XYPos* before, XYPos* after);
XYPos* get_adjacent_pos(XYPos* pos, int dir);
char* get_dir(int dir);

void log_wall(lastmove_t* move, char* log);
void log_move(lastmove_t* move, char* log);
void log_following(int me, int following, char* log);
void log_attempt(int id, int dir, XYPos* pos, char* log);

/*************** check_previous() ***************/
void check_previous(maze_t* maze, lastmove_t* move, char* log, 
				int strength, counters_t* followers)
{
	int prev_id = move->avatarID;
	int prev_dir = move->direction;
	XYPos* pos = move->after;

    if (prev_id != -1) { // not the first move 
        if (is_pos_equal(move->before, move->after)) {
        	// move has not been made, must be a wall
        	log_wall(move, log);
        	set_wall(maze, pos, 1, prev_dir);

        } else { // move has been made
        	log_move(move, log);
        	set_wall(maze, pos, 0, prev_dir);
        	set_avatar_position(maze, pos, prev_id); 

        	int tagger = get_tagged_by(maze, pos);

        	if (tagger == -1 || tagger == prev_id) { 
        		// unvisited or previously visited by me
        		visit(maze, pos, prev_id, strength); // tag square

        	} else { // found path, set following of avatar to tagger
        		counters_set(followers, prev_id, tagger); 
        		log_following(prev_id, tagger, log);

        	}	
        }

        draw_maze(maze); // update maze
    }
}


/****************** functions for choosing moves ******************/

/****************** maze_solve() ******************/
move_t* maze_solve(maze_t* maze, int id, XYPos* pos, char* log)
{	

	XYPos* final_pos = pos;
	int strength = -1;
	int dir = 4;
	int wall;
	
	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i);
		
		if (wall == 0) { // if there is no wall, check for path
			XYPos *new_pos = get_adjacent_pos(pos, i);
			printf("x: %d, y: %d\n", new_pos->x, new_pos->y);

	
			int tagger = get_tagged_by(maze, new_pos);
			
			if (tagger != id) { // if the tagger isn't me
				int new_strength = get_tag_strength(maze, 
								new_pos);

				if (new_strength > strength) {
					// if the tag strength is higher
					strength = new_strength; 
					dir = i;
					final_pos = new_pos;
				} else {
					free(new_pos);
				}
			} else {
				free(new_pos);
			}
			
		} else if (wall == -1 && strength == -1) {
			// unknown wall and no path found yet
			final_pos = get_adjacent_pos(pos, i);
			dir = i;
		}
	}

	log_attempt(id, dir, final_pos, log);
	move_t* attempt = malloc(sizeof(move_t));
	move_t move = { id, dir };
	attempt = &move;

	return attempt;
}

/****************** leader_solve() ******************/
move_t* leader_solve(maze_t* maze, int id, XYPos* pos, char* log)
{
	XYPos* final_pos = pos;
	int dir = 4;
	int strength = get_tag_strength(maze, pos);
	int wall;

	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i); // get status of that wall;

		if (wall == 0) { // if there is no wall
			XYPos* new_pos = get_adjacent_pos(pos, i);
			printf("%d, %d\n", (int) pos->x, (int) pos->y); 
			int tagger = get_tagged_by(maze, new_pos);

			if (tagger == id) { // if tagger is me
				int new_strength = get_tag_strength(maze, 
								new_pos);

				if (new_strength < strength) {
					// go to lower strength square
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

	log_attempt(id, dir, final_pos, log);
	move_t* attempt = malloc(sizeof(move_t));
	move_t move = { id, dir };
	attempt = &move;

	return attempt;

}

/****************** follower_solve() ******************/
move_t* follower_solve(maze_t* maze, int id, XYPos* pos, 
					counters_t* followers, char* log)
{
	XYPos* final_pos = pos;
	int dir = 4;
	int strength = get_tag_strength(maze, pos);
	int following = counters_get(followers, id);
	bool found_new = false;
	int wall;

	for (int i = 0; i < 4; i++) {
		wall = get_wall(maze, pos, i);

		if (wall == 0) { // no wall
			XYPos* new_pos = get_adjacent_pos(pos, i);
			int tagger = get_tagged_by(maze, new_pos);

			if (tagger != following && tagger != id) {
				// tagger is a third avatar
				int leader = counters_get(followers, 
								  following);
				
				if (tagger == leader) { 
					// switch path if third avatar is
					// leader of my leader
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

	log_attempt(id, dir, final_pos, log);
	
	move_t* attempt = malloc(sizeof(move_t));
	move_t move = { id, dir };
	attempt = &move;

	return attempt;
}




/****************** helper/local functions *******************/

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
	printf("x:%d, y:%d\n", x, y);
	printf("x:%d, y:%d\n", adjacent->x, adjacent->y);
	*adjacent = { x, y };
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

	uint32_t x = move->after->x;
	uint32_t y = move->after->y;
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
	char* dir = get_dir(attempt_dir);
	
		
	fprintf(fp, "Avatar %d attempted to move %s to (%d, %d).\n", 
							   id, dir, x, y);
	fclose(fp);
	if (pos != NULL) {
		free(pos);
	}
}



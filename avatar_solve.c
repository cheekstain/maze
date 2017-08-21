/*
 * avatar_solve.c - see avatar_solve.h for details on exported functions
 * 
 * Bashful Brigade: Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avatar_solve.h"
#include "avatar_comm.h"
#include "avatar.h"
#include "mazestruct.h"
#include "amazing.h"
#include "counters.h"

/****************** local types ******************/

/*************** global functions ***************/
/* see avatar_solve.h for comments about exported functions */

/*************** local functions ****************/
bool is_xypos_equal(XYPos before, XYPos after);

void log_wall(lastmove_t* move, char* log);
void log_move(lastmove_t* move, char* log);
void log_following(int me, int following, char* log);

void calculate_random(maze_t* maze); //TODO: unsure of paramters
void calculate_move(maze_t* maze); //TODO: unsure of paramters

/*************** check_previous() ***************/
void check_previous(maze_t* maze, lastmove_t* move, char* log, 
				int strength, counters_t* followers)
{
	int prev_id = move->avatarID;
	int prev_dir = move->direction;
	XYPos pos = move->after;

    if (move != NULL && receive_message()) { // not the first move 
        if (is_xypos_equal(move->before, move->after)) {
        	// move has not been made, must be a wall
        	log_wall(move, log);
        	set_wall(maze, &pos, 1, prev_dir);

        } else { // move has been made
        	log_move(move, log);
        	set_wall(maze, &pos, 0, prev_dir);
        	set_avatar_position(maze, &pos, prev_id); 

        	int tagger = get_tagged_by(maze, &pos);

        	if (tagger == -1 || tagger == prev_id) { 
        		// unvisited or previously visited by me
        		visit(maze, &pos, prev_id, strength); // tag the square

        	} else { // visited square by someone else
        		// set following of avatar to tagger
        		counters_set(followers, prev_id, tagger); // TODO revisit
        		log_following(prev_id, tagger, log);

        	}	
        }
        draw_maze(maze); // update maze
    }
}

/* is_xypos_equal is a helper function that returns true if the two XYPos
 * are equal and false otherwise.
 */
bool is_xypos_equal(XYPos before, XYPos after)
{
        if ((before->x == after->x) && (before->y == after->y)) {
        	return true;
        }
        return false;
} 

/* log_wall is a function that updates the log with a wall if the move
 * has failed.
 */
void log_wall(lastmove_t* move, char* log)
{
	int prev_dir = move->direction;
	int x = move->after->x;
	int y = move->after->y;

	char* dir;
	if (prev_dir == 0) {
		dir = "West";
	} else if (prev_dir == 1) {
		dir = "North";
	} else if (prev_dir == 2) {
		dir = "South"
	} else {
		dir = "East"
	}

	fprintf(log, "Move fail. %s wall added to (%d, %d)\n", dir, x, y);
}


/* log_move is a function that updates the log with an avatar move if it
 * was sucessful.
 */
void log_move(lastmove_t* move, char* log)
{
	int id = move->avatarID;
	int x = move->after->x;
	int y = move->after->y;

	fprintf(log, "Avatar %d moved to (%d, %d)\n", id, x, y);
}


/* log_following is a function that updates the log with who an avatar is
 * following if it happens on that path
 */
void log_following(int me, int following, char* log)
{
	fprintf(log, "Avatar %d is on avatar %d's path\n", me, following);
}


/****************** make_move() ******************/
void make_move(maze_t* maze, avatars_t* avatars, char* log, int my_id)
{
        //avatar_comm function to get AM_AVATAR_TURN message
        int my_x = // parse from message
        int my_y =
        int turn_id =
        if (turn_id == my_id) {
                calculate_random(maze, my_x, my_y);
                //update visualization, function from mazestruct.h
                draw_maze(maze);

                //write move details to logfile
                update_log(log);

                //avatar_comm function to send AM_AVATAR_MOVE to server
                //
        }

        // update/check if move failed as well?
}


void calculate_move(maze_t* maze)
{

}

void calculate_random(maze_t* maze)
{

}


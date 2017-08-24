/*
 * solve_test.c - see avatar_solve.h for details on exported functions
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
#include "avatar_solve.h"

static void maze_solve_test(char* log);
static void leader_solve_test(char* log);
static void follower_solve_test(char* log);
static void check_previous_test(char* log);
static void check_is_following(void);
static void check_is_leader_collide(void);

int main()
{
	char* log = "./test.out";
	maze_solve_test(log);
	leader_solve_test(log);
	follower_solve_test(log);
	check_previous_test(log);
	check_is_following();
	check_is_leader_collide();
	printf("test finished\n");
}

static void maze_solve_test(char* log)
{
	printf("maze_solve test:\n");
	XYPos me = { 2, 4 };
	int id = 0;

	XYPos one = { 1, 4 };
	XYPos two = { 2, 3 };
	counters_t* ctrs = counters_new();
	counters_set(ctrs, 0, 0);
	
	// create maze
	maze_t* maze = maze_new(5, 5, 3); 
	set_wall(maze, &me, 0, 0);
	set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 0, 3);
	set_avatar_position(maze, &me, id);

	// tag maze
	visit(maze, &me, 0, 2);
	visit(maze, &one, 1, 3);
	visit(maze, &two, 2, 4);
		
	move_t* move = maze_solve(maze, id, &me, ctrs, log);
	free(move);
	maze_delete(maze);
	counters_delete(ctrs);
	printf("move written to log\n");
}

static void leader_solve_test(char* log)
{
	printf("leader_solve test:\n");
	XYPos me = { 2, 4 };
	int id = 0;

	XYPos one = { 1, 4 };
	XYPos two = { 2, 3 };
	
	// create maze
	maze_t* maze = maze_new(5, 5, 3); 
	set_wall(maze, &me, 0, 0);
	set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 0, 3);
	set_avatar_position(maze, &me, id);
	
	// tag maze
	visit(maze, &me, 0, 2);
	visit(maze, &one, 0, 1);
	visit(maze, &two, 0, 3);
	
	move_t* move = leader_solve(maze, id, &me, log);
	free(move);
	maze_delete(maze);
	printf("move written to log\n");
}

static void follower_solve_test(char* log)
{
	printf("follower_solve test:\n");
	XYPos me = { 2, 4 };
	int id = 0;

	XYPos one = { 1, 4 };
	XYPos two = { 2, 3 };
	counters_t* ctrs = counters_new();
	counters_set(ctrs, 0, 1);
	counters_set(ctrs, 1, 2);
	counters_set(ctrs, 2, 2);

	// create maze
	maze_t* maze = maze_new(5, 5, 3);
	set_wall(maze, &me, 0, 0);
	set_wall(maze, &me, 0, 1);
	set_wall(maze, &me, 0, 3);
	set_avatar_position(maze, &me, id);

	// tag maze
	visit(maze, &me, 1, 2);
	visit(maze, &one, 1, 3);
	visit(maze, &two, 2, 1);
	
	move_t* move = follower_solve(maze, id, &me, ctrs, log);
	free(move);
	maze_delete(maze);
	counters_delete(ctrs);
	printf("move written to log\n");
}

static void check_previous_test(char* log)
{
	printf("check_previous test:\n");
	maze_t* maze = maze_new(5, 5, 3);
	XYPos me = { 2, 4 };
	XYPos two = { 2, 3 };
	counters_t* ctrs = counters_new();
	counters_set(ctrs, 0, 0);
		
	lastmove_t last = { &two, &me, 0, 2 };
	check_previous(maze, &last, log, 7, ctrs);
	
	maze_delete(maze);
	counters_delete(ctrs);
	printf("results written to log\n");
}

static void check_is_following(void)
{
	printf("is_following test:\n");
	counters_t* followers = counters_new();
	counters_set(followers, 5, 8);
	counters_set(followers, 8, 2);
	counters_set(followers, 2, 3);
	counters_set(followers, 3, 3);
	
	if (is_following(5, 3, followers)) {
		printf("result: true, test success\n");
	} else {
		printf("result: false, test fail\n");
	}
	
	if (is_following(5, 9, followers)) {
		printf("result: true, test fail\n");
	} else {
		printf("result: false, test success\n");
	}

	counters_delete(followers);
}

static void check_is_leader_collide(void)
{
	printf("is_leader_collide test:\n");
	XYPos me = { 2, 4 };

	int id = 0;
	int id1 = 1;
	maze_t* maze = maze_new(5, 5, 3);
	set_avatar_position(maze, &me, id);
	set_avatar_position(maze, &me, id1);

	if (is_leader_collide(maze, 2, &me)) {
		printf("result: true, test fail\n");
	} else {
		printf("result: false, test success\n");
	}
	
	maze_delete(maze);
}

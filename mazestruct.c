/*
* mazestruct.c - maze structure module for CS50 final project
* 
* See mazestruct.h for more information
*
* Emma Hobday, August 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct maze {
	mazesquare *array[][]; 																//revisit this
} maze_t;

/**************** local types ****************/
typedef struct mazesquare {
	int north_wall;
	int east_wall;
	int south_wall;
	int west_wall;
	bool visited;
} mazesquare_t;

/******************************** mazesquare_new ******************************/
static mazesquare_t *mazesquare_new()
{	

}

/******************************** maze_new ************************************/
maze_t *maze_new()
{

}

/******************************** get_north_wall ******************************/
int get_north_wall()
{

}

/******************************** set_north_wall ******************************/
void set_north_wall()
{

}

/******************************** get_south_wall ******************************/
int get_south_wall()
{

}

/******************************** set_south_wall ******************************/
void set_south_wall()
{

}

/******************************** get_east_wall ******************************/
int get_east_wall()
{

}

/******************************** set_east_wall *******************************/
void set_east_wall()
{

}

/******************************** get_west_wall *******************************/
int get_west_wall()
{

}

/******************************** set_west_wall *******************************/
void set_west_wall()
{

}

/******************************** get_visited *********************************/
bool get_visited()
{

}

/******************************** set_visited *********************************/
void set_visited() 
{

}

/******************************** maze_delete *********************************/
void maze_delte()
{

}


















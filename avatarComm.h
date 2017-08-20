/*
 * counters.h - header file for CS50 'avatarComm' module
 *
 *
  * Compiled using class materials provided by Prof Zhou, July, 2017
 * Arun Anand, July 2017
 */

#ifndef __AVATARCOMM_H
#define __AVATARCOMM_H


/**************** message sending functions ****************/
/*
*
*Sends the AM_INIT message to the server
*
*/
void send_init(int nAvatars, int difficulty);

/*
*
* Sends the AM_AVATAR_READY message to the server
*
*/
void send_avatar_ready(int avatarID);

/*
*
* Sends the AM_AVATAR_MOVE message
*
*/
void send_move(int avatarID, int Direction);

/**************** message receiving functions ****************/

/*
*
* Calling this function alerts the module that a new message should be received from the server.
* This function must be called whenever a server-to-client message is expected.
* In other words, it must be called any time a mesage is sent from the client to the server.
*
*/
void receive_message();


/**************** accessor functions ****************/

/*
*
* returns whether or not the AM_INIT message was successfully sent and the server responded with AM_INIT_OK
*
*/
bool is_init_successful();

/*
*
* Returns the mazeport
*
*/
int get_mazeport();

/*
*
* Returns the width of the maze
*
*/
int get_maze_width();

/*
*
* Returns the height of the maze
*
*/
int get_maze_height();

/*
*
* gets the turnID of the current avatar that has to make a move.
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate turnID
*
*/
int get_turnID();

/*
*
* gets the positions of all of the avatars on the maze in a position array, where the array index represents the avatarID
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate position array
*
*/
int get_position_array();

/*
*
* Returns whether the game is over because the avatars were successful in finding each other, or because the movelimit or timelimit
* was reached, or because there was an error in the messaging
* 
*
*/
bool is_game_over();






#endif // __AVATARCOMM_H

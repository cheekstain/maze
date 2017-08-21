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
* Establishes a connection to the server, and sends the AM_INIT message.
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
void send_init(int nAvatars, int difficulty, char *hostname);

/*
*
* Establishes a connection through the mazeport and sends the AM_AVATAR_READY message to the server.
* Returns true if and only if the init message was able to sent without running into errors. 
*
*/
void send_avatar_ready(int avatarID);

/*
*
* Sends the AM_AVATAR_MOVE message
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
void send_move(int avatarID, int Direction);

/**************** message receiving functions ****************/

/*
*
* Calling this function alerts the module that a new message should be received from the server.
* This function must be called whenever a server-to-client message is expected.
* In other words, it must be called any time a mesage is sent from the client to the server.
* Returns true if and only if there were no error messages returned from the server, such as "Unknown Avatar" or "Unknown Message Type."
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
XYPos *get_position_array();

/*
*
* Returns whether the game is over because the avatars were successful in finding each other, or because the movelimit or timelimit
* was reached, or because there was an error in the messaging
* Returns 0 if the game is still in progress.
* Returns 1 if there was a server timeout.
* Returns 2 if the move limit was reached.
*
*/
int check_game_status();






#endif // __AVATARCOMM_H

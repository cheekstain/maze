/*
 * counters.h - header file for CS50 'avatarComm' module
 *
 *
  * Compiled using class materials provided by Prof Zhou, July, 2017
 * Arun Anand, July 2017
 */

#ifndef __AVATARCOMM_H
#define __AVATARCOMM_H

typedef struct comm comm_t;


/**************** comm_new ****************/
comm_t *comm_new();


/**************** message sending functions ****************/

/*
*
* Establishes a connection to the server, and sends the AM_INIT message.
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
bool send_init(comm_t *com, int nAvatars, int difficulty, char *hostname);

/*
*
* Establishes a connection through the mazeport and sends the AM_AVATAR_READY message to the server.
* Returns true if and only if the init message was able to sent without running into errors. 
*
*/
int send_avatar_ready(comm_t *com, int avatarID);

/*
*
* Sends the AM_AVATAR_MOVE message
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
bool send_move(comm_t *com, int avatarID, int Direction, int sock);

/**************** message receiving functions ****************/

/*
*
* Calling this function alerts the module that a new message should be received from the server.
* This function must be called whenever a server-to-client message is expected.
* In other words, it must be called any time a mesage is sent from the client to the server.
* Returns true if and only if there were no error messages returned from the server, such as "Unknown Avatar" or "Unknown Message Type," and there
* was a message to be received.
*
*/
bool receive_message(comm_t *com, int avatarID, int sock);


/**************** accessor functions ****************/

/*
*
* returns whether or not the AM_INIT message was successfully sent and the server responded with AM_INIT_OK
*
*/
bool is_init_successful(comm_t *com);

/*
*
* Returns the mazeport
*
*/
int get_mazeport(comm_t *com);

/*
*
* Returns the width of the maze
*
*/
int get_maze_width(comm_t *com);

/*
*
* Returns the height of the maze
*
*/
int get_maze_height(comm_t *com);

/*
*
* gets the turnID of the current avatar that has to make a move.
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate turnID
*
*/
int get_turnID(comm_t *com);

/*
*
* gets the positions of all of the avatars on the maze in a position array, where the array index represents the avatarID
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate position array
*
*/
XYPos *get_position_array(comm_t *com);

/*
*
* Returns whether the game is over because the avatars were successful in finding each other, or because the movelimit or timelimit
* was reached, or because there was an error in the messaging
* Returns 0 if the game is still in progress.
* Returns 1 if there was a server timeout.
* Returns 2 if the move limit was reached.
*
*/
int check_game_status(comm_t *com);

/*
*
* A function to get the hashcode when the maze has been solved
*
*/
int get_hash(comm_t *com);

/*
*
* A function to get the number of total moves when the maze has been solved
*
*/
int get_nMoves (comm_t *com);

/*
*
* A function to get the difficulty when the maze has been solved
*
*/
int get_difficulty(comm_t *com);






#endif // __AVATARCOMM_H

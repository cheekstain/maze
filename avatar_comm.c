#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <netdb.h>
#include <stdbool.h>
#include "mazestruct.h"
#include "amazing.h"

static bool is_init_successful = false;
static int mazeport; \
static int maze_width;
static int maze_height;
static int turnID;
static XYPos positions[AM_MAX_AVATAR];
static bool is_game_over = false;
static bool is_timeout = false;
static bool is_too_many_moves = false;
static int comm_sock;
static struct sockaddr_in server;

/**************** file-local constants ****************/
#define BUFSIZE 1024     // read/write buffer size

/*
*
* Establishes a connection to the server, and sends the AM_INIT message.
*
*/
void send_init(int nAvatars, int difficulty, char *hostname)
{
	AM_Message initialize;
	initialize.type = AM_INIT
	initialize.type.Avatars = htonl(nAvatars);
	initialize.type.Difficulty = htonl(difficulty);

	comm_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (comm_sock < 0) {
		fprintf(stderr, "Error opening socket.");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(AM_SERVER_PORT);
	// Look up the hostname specified on command line
  	struct hostent *hostp = gethostbyname(hostname); // server hostname
  	if (hostp == NULL) {
  		fprintf(stderr, "%s: unknown host '%s'\n", hostname);
  		exit(2);
  	}  
  	memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  	if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
  		fprintf("Error connecting stream socket.");
  		exit(3);
  	}

  	if (write(comm_sock, &initialize, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		exit(4);
  	}
  }

/*
*
* Establishes a connection through the mazeport and sends the AM_AVATAR_READY message to the server.
*
*/
  void send_avatar_ready(int avatarID)
  {
	//server.sin_family = AF_INET;
  	server.sin_port = htonl(mazeport);
	// Look up the hostname specified on command line
  	//struct hostent *hostp = gethostbyname(hostname); // server hostname
  	//if (hostp == NULL) {
  	//	fprintf(stderr, "%s: unknown host '%s'\n", program, hostname);
  	//	exit(2);
  	//}  
  	//memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  	if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
  		fprintf("Error connecting stream socket.");
  		exit(3);
  	}
  	AM_Message msg;
  	msg.type = AM_AVATAR_READY;
  	msg.type.AvatarId = avatarID;
  	if (write(comm_sock, &msg, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		exit(4);
  	}
  }

/*
*
* Sends the AM_AVATAR_MOVE message
*
*/
  void send_move(int avatarID, int direction)
  {
  	AM_Message msg;
  	msg.type = AM_AVATAR_MOVE;
  	msg.type.AvatarId = htons(avatarID);
  	msg.type.Direction = htons(direction);
  	if (write(comm_sock, &msg, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		exit(4);
  	}
  }

/*
*
* Calling this function alerts the module that a new message should be received from the server.
* This function must be called whenever a server-to-client message is expected.
* In other words, it must be called any time a mesage is sent from the client to the server.
*
*/
  void receive_message()
  {
  	char buf [BUFSIZE];
  	if ((bytes_read = read(comm_sock, buf, BUFSIZE-1)) < 0) {
  		fprintf(stderr, "Error reading from server.");
  		exit(5);
  	}

  	AM_Message *msg = (AM_Message *) buf;
  	if (msg->type == AM_INIT_OK){
  		mazeport = ntohl(msg->init_ok.MazePort);
  		maze_width = ntohl(msg->init_ok.MazeWidth);
  		maze_height = ntohl(msg->init_ok.MazeHeight);
  		is_init_successful = true;
  	}

  	else if (msg->type == AM_INIT_FAILED){
  		is_init_successful = false;
  	}
  	else if (msg->type == AM_NO_SUCH_AVATAR){
  		fprintf(stderr, "No such Avatar!");
  		exit(6);
  	}
  	else if (msg->type == AM_AVATAR_TURN){
  		turnID = ntohl(msg->avatar_turn.TurnId);
  		positions = ntohl(msg->avatar_turn.Pos);
  	}
  	else if (msg->type == AM_UNKNOWN_MSG_TYPE){
  		fprintf(stderr, "Unknown message type!");
  		exit(7);
  	}
  	else if (msg->type == AM_UNEXPECTED_MSG_TYPE){
  		fprintf(stderr, "Unexpected message type!");
  		exit(8);
  	}
  	else if (msg->type == AM_AVATAR_OUT_OF_TURN){
  		fprintf(stderr, "Avatar out of turn!");
  		exit(9);
  	}
  	else if (msg->type == AM_TOO_MANY_MOVES){
  		is_game_over = true;
  		is_too_many_moves = true;
  	}
  	else if (msg->type == AM_SERVER_TIMEOUT){
  		is_game_over = true;
  		is_timeout = true;
  	}
  	else if (msg->type == AM_SERVER_DISK_QUOTA){
  		fprintf(stderr, "Exceeeded server disk quota!");
  		exit(9);
  	}
  	else if (msg->type == AM_SERVER_OUT_OF_MEM){
  		fprintf(stderr, "Exceeeded server memory!");
  		exit(9);
  	}
  }

/*
*
* returns whether or not the AM_INIT message was successfully sent and the server responded with AM_INIT_OK
*
*/
  bool is_init_successful()
  {
  	return is_init_successful;
  }

/*
*
* Returns the mazeport
*
*/
  int get_mazeport()
  {
  	return mazeport;
  }

/*
*
* Returns the width of the maze
*
*/
  int get_maze_width()
  {
  	return maze_width;
  }

/*
*
* Returns the height of the maze
*
*/
  int get_maze_height()
  {
  	return maze_height;
  }

/*
*
* gets the turnID of the current avatar that has to make a move.
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate turnID
*
*/
  int get_turnID()
  {
  	return turnID;
  }

/*
*
* gets the positions of all of the avatars on the maze in a position array, where the array index represents the avatarID
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate position array
*
*/
  int get_position_array()
  {
  	return positions;
  }

/*
*
* Returns whether the game is over because the avatars were successful in finding each other, or because the movelimit or timelimit
* was reached, or because there was an error in the messaging
* 
*
*/
  bool is_game_over()
  {
  	return is_game_over;
  }


/*
*
* Returns whether the game ended due to a server timeout
* 
*
*/
  bool is_timeout()
  {
  	return is_timeout;
  }

/*
*
* Returns whether the game ended because the number of moves allotted was exceeded
* 
*
*/
  bool is_moves_execeeded()
  {
  	return is_too_many_moves;
  }


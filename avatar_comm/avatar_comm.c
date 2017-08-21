#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <netdb.h>
#include <stdbool.h>
//#include "mazestruct.h"
#include "amazing.h"

 
typedef struct comm{     
bool is_init_successful;
uint16_t mazeport;
uint32_t maze_width;
uint32_t maze_height;
uint32_t turnID;
XYPos positions[AM_MAX_AVATAR];
bool is_game_over;
bool is_timeout;
bool is_too_many_moves;
int comm_sock;
struct sockaddr_in server;
int nAvatars;
int *sockets[AM_MAX_AVATAR];
char *hostname;
} comm_t;

typedef struct ready_param{
  comm_t *com;
  int avatarID;
} avatar_ready_param_t;



/**************** file-local constants ****************/
#define BUFSIZE 1024     // read/write buffer size

/**************** comm_new ****************/
comm_t *comm_new()
{
	comm_t *com = malloc(sizeof(comm_t));
	com->is_init_successful = false;
	com->is_game_over = false;
	com->is_timeout = false;
	com->is_too_many_moves = false;
	return com;
}

/**************** param_new ****************/
avatar_ready_param_t *param_new(int avatarID, comm_t *com){
  avatar_ready_param_t *p = malloc(sizeof(avatar_ready_param_t));
  p->avatarID = avatarID;
  p->com = com;
  return p;
}

/*
*
* Establishes a connection to the server, and sends the AM_INIT message.
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
bool send_init(comm_t *com, int nAvatars, int difficulty, char *hostname)
{
	AM_Message msg;
	msg.type = htonl(AM_INIT);
	msg.init.nAvatars = htonl(nAvatars);
	msg.init.Difficulty = htonl(difficulty);

	com->comm_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (com->comm_sock < 0) {
		fprintf(stderr, "Error opening socket.");
		return false;
	}

	com->server.sin_family = AF_INET;
	com->server.sin_port = htons(atoi(AM_SERVER_PORT));
	// Look up the hostname specified on command line
  	struct hostent *hostp = gethostbyname(hostname); // server hostname
  	if (hostp == NULL) {
  		fprintf(stderr, "%s: unknown host\n", hostname);
  		return false;
  	}  
  	memcpy(&(com->server.sin_addr), hostp->h_addr_list[0], hostp->h_length);

  	if (connect(com->comm_sock, (struct sockaddr *) &(com->server), sizeof(com->server)) < 0) {
  		fprintf(stderr, "Error connecting stream socket.");
  		return false;
  	}

  	if (write(com->comm_sock, &msg, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		return false;
  	}

    com->nAvatars = nAvatars;
    com->hostname = malloc(strlen(hostname)+1);
    strcpy(com->hostname, hostname);
    
  	return true;
  }

/*
*
* Establishes a connection through the mazeport and sends the AM_AVATAR_READY message to the server.
* Returns true if and only if the init message was able to sent without running into errors. 
*
*/
  bool send_avatar_ready(avatar_ready_param_t *p)
  {
    int avatarID = p->avatarID;
    comm_t *com = p->com;
	//server.sin_family = AF_INET;
  	com->server.sin_port = htons(com->mazeport);
	// Look up the hostname specified on command line
  	//struct hostent *hostp = gethostbyname(hostname); // server hostname
  	//if (hostp == NULL) {
  	//	fprintf(stderr, "%s: unknown host '%s'\n", program, hostname);
  	//	exit(2);
  	//}  
  	//memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  	//if (connect(com->comm_sock, (struct sockaddr *) &(com->server), sizeof(com->server)) < 0) {
  		//fprintf(stderr, "Error connecting stream socket.");
  		//return false;
  	//}
  int avatar_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (avatar_sock < 0) {
    fprintf(stderr, "Error opening socket.");
    return false;
  }

  com->server.sin_family = AF_INET;
  com->server.sin_port = htons(com->mazeport);
  // Look up the hostname specified on command line
    struct hostent *hostp = gethostbyname(com->hostname); // server hostname
    if (hostp == NULL) {
      fprintf(stderr, "%s: unknown host\n", com->hostname);
      return false;
    }  
    memcpy(&(com->server.sin_addr), hostp->h_addr_list[0], hostp->h_length);

    if (connect(avatar_sock, (struct sockaddr *) &(com->server), sizeof(com->server)) < 0) {
      fprintf(stderr, "Error connecting stream socket.");
      return false;
    }
  	AM_Message msg;
  	msg.type = htonl(AM_AVATAR_READY);
  	msg.avatar_ready.AvatarId = htonl(avatarID);
  	if (write(avatar_sock, &msg, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		return false;
  	}
    *(com->sockets[avatarID]) = avatar_sock;
  	return true;
  }

/*
*
* Sends the AM_AVATAR_MOVE message
* Returns true if and only if the init message was able to sent without running into errors.
*
*/
  bool send_move(comm_t *com, int avatarID, int direction)
  {
  	AM_Message msg;
  	msg.type = htonl(AM_AVATAR_MOVE);
  	msg.avatar_move.AvatarId = htonl(avatarID);
  	msg.avatar_move.Direction = htonl(direction);
    int avatar_sock = *(com->sockets[avatarID]);
  	if (write(avatar_sock, &msg, sizeof(AM_Message)) < 0)  {
  		fprintf(stderr, "Error writing on stream socket.");
  		return false;
  	}

  	return true;
  }

/*
*
* Calling this function alerts the module that a new message should be received from the server.
* This function must be called whenever a server-to-client message is expected.
* In other words, it must be called any time a mesage is sent from the client to the server.
* Returns true if and only if there were no error messages returned from the server, such as "Unknown Avatar" or "Unknown Message Type."
*
*/
  bool receive_message(comm_t *com)
  {
  	char buf [BUFSIZE];
    int bytes_read;
  	if ((bytes_read = read(com->comm_sock, buf, BUFSIZE-1)) < 0) {
  		fprintf(stderr, "Error reading from server.");
  		return false;
  	}
    printf("bytes read %d\n", bytes_read);

  	AM_Message *msg = (AM_Message *) buf;
  	if (ntohl(msg->type) == AM_INIT_OK){
  		com->mazeport = ntohl(msg->init_ok.MazePort);
  		com->maze_width = ntohl(msg->init_ok.MazeWidth);
  		com->maze_height = ntohl(msg->init_ok.MazeHeight);
  		com->is_init_successful = true;
  	}

  	else if (ntohl(msg->type) == AM_INIT_FAILED){
  		com->is_init_successful = false;
      fprintf(stderr, "Init failed!");
  	}
  	else if (ntohl(msg->type) == AM_NO_SUCH_AVATAR){
  		fprintf(stderr, "No such Avatar!");
  		return false;
  	}
  	else if (ntohl(msg->type) == AM_AVATAR_TURN){
  		com->turnID = ntohl(msg->avatar_turn.TurnId);
  		for (int i = 0; i<com->nAvatars; i++){
        uint32_t x = (msg->avatar_turn.Pos[i]).x;
      	uint32_t y = (msg->avatar_turn.Pos[i]).y;
      	XYPos new_pos;
      	new_pos.x = ntohl(x);
      	new_pos.y = ntohl(y);
      	com->positions[i] = new_pos;
      }
  	}
  	else if (ntohl(msg->type) == AM_UNKNOWN_MSG_TYPE){
  		fprintf(stderr, "Unknown message type!");
  		return false;
  	}
  	else if (ntohl(msg->type) == AM_UNEXPECTED_MSG_TYPE){
  		fprintf(stderr, "Unexpected message type!");
  		return false;
  	}
  	else if (ntohl(msg->type) == AM_AVATAR_OUT_OF_TURN){
  		fprintf(stderr, "Avatar out of turn!");
  		return false;
  	}
  	else if (ntohl(msg->type) == AM_TOO_MANY_MOVES){
  		com->is_game_over = true;
  		com->is_too_many_moves = true;
  	}
  	else if (ntohl(msg->type) == AM_SERVER_TIMEOUT){
  		com->is_game_over = true;
  		com->is_timeout = true;
  	}
  	else if (ntohl(msg->type) == AM_SERVER_DISK_QUOTA){
  		fprintf(stderr, "Exceeeded server disk quota!");
  		return false;
  	}
  	else if (ntohl(msg->type) == AM_SERVER_OUT_OF_MEM){
  		fprintf(stderr, "Exceeeded server memory!");
  		return false;
  	}

  	return true;
  }

/*
*
* returns whether or not the AM_INIT message was successfully sent and the server responded with AM_INIT_OK
*
*/
  bool is_init_successful(comm_t *com)
  {
  	return com->is_init_successful;
  }

/*
*
* Returns the mazeport
*
*/
  int get_mazeport(comm_t *com)
  {
  	return com->mazeport;
  }

/*
*
* Returns the width of the maze
*
*/
  int get_maze_width(comm_t *com)
  {
  	return com->maze_width;
  }

/*
*
* Returns the height of the maze
*
*/
  int get_maze_height(comm_t *com)
  {
  	return com->maze_height;
  }

/*
*
* gets the turnID of the current avatar that has to make a move.
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate turnID
*
*/
  int get_turnID(comm_t *com)
  {
  	return com->turnID;
  }

/*
*
* gets the positions of all of the avatars on the maze in a position array, where the array index represents the avatarID
* Precondition: receive_message() must be called after the send_move function in order to provide the accurate position array
*
*/
  XYPos *get_position_array(comm_t *com)
  {
  	return com->positions;
  }

/*
*
* Returns whether the game is over because the avatars were successful in finding each other, or because the movelimit or timelimit
* was reached, or because there was an error in the messaging
* Returns 0 if the game is still in progress.
* Returns 1 if there was a server timeout.
* Returns 2 if the move limit was reached.
* returns 3 if an error message was sent from the server.
*
*/
  bool check_game_status(comm_t *com)
  {
  	if (!(com->is_game_over)){
  		return 0;
  	}
  	else if (com->is_timeout){
  		return 1;
  	}
  	else if (com->is_too_many_moves){
  		return 2;
  	}
  	return 3;
  }

  /*
  *
  * A function to close all of the sockets when communication has ended.
  *
  */
  void close_sockets(comm_t *com)
  {
    for (int i = 0; i<com->nAvatars; i++){
      close(*com->sockets[i]);
      close(com->comm_sock);
    }
  }


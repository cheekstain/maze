# CS50 Final Project

**Arun Hari Anand, Bill Tang, Christina Lu, Emma Hobday, CS50 August 2017**

*Written using class materials provided by Prof. Zhou*

# Introduction

This program is intended to run a simulation of a set of maze-solving avatars 
whose function is to efficiently find each other, subject to the constraint that 
they may not exceed the maximum number of moves or the maximum amount of time 
allotted.

## Major Design Decisions
The maze program will be started with a C program. The avatars will be 
individual threads within a main C program with functions organized and 
categorized under across different modules.

* `avatar_comm`: refers to a function(s) related to communications with the 
server, parsing/naming messages, et cetera.
* `avatar_solve`: refers to a function(s) related to calculating and returning 
the next best move.
* `maze_struct`: refers to the data structures and related functions dealing 
with storage and logical structures of the maze.

# Startup Design

## startup.c
startup.c is a C program that takes command-line args, connects with the server 
to initialize the maze, and finally start the avatar threads to solve the maze.

### User Interface
Takes in command line arguments: `./AM_STARTUP n_avatars difficulty hostname`
* n_avatars: (`int`) the number of avatars in the maze
* difficulty: (`int`) the difficulty level, on the scale 0 (easy) to 9 
(excruciatingly difficult)
* hostname: (`char*`) the hostname of the server

### Inputs and Outputs

Inputs:
* `int n_avatars`: number of avatars within maze
* `int difficulty`: ranging from 0 to 9, the difficulty of the maze
* `char* hostname`: the hostname of the server 

Outputs:
* `file Amazing_$USER_N_D.log`: where $USER is the current userid, N is the 
value of n_avatars and D is the value of difficulty; open for appending

### Pseudocode
```
1. validate arguments
2. construct `AM_INIT` struct
3. write `AM_INIT` to server
4. wait to receive `AM_INIT_OK`
5. extract `maze_port`, `maze_height`, and `maze_width`
6. create log file Amazing_$USER_N_D.log
7. write $USER, the MazePort, and the date and time to log file
8. create three global structures, namely:
	1. a `mazestruct` data structure that contains all of the information 
	gleaned about the maze as the search has progressed. This `mazestruct` 
	module contains a few accessor and modifier methods that the avatars may use 
	in the decision-making algorithm that picks the next best move.
	2. a `set_t` data structure that uses the avatarIDs as its keys and 
	`avatar_t` structs as its items. These `avatar_t` structs contain 
	information specific to each avatar that is valuable when picking the next 
	best move. This struct will be elaborated upon in the maze-solving portion 
	of this pseudocode section.
	3. a struct named `last_move` which will hold two XYPos instances and one 
	int avatarID. `last_move` holds the last move attempted by any avatar in 
	the maze. `XYPos before` will hold the position of the avatar before the 
	move was made, and `XYPos after` holds the position of the avatar after the 
	move is attempted, provided the move is successfully made. `int avatarID` 
	will contain the avatarID of the avatar that attempted this move.
9. initialize `n_avatars` threads, call `avatar_thread` on each
10. for loop waiting for threads to terminate
11. use the `avatar_comm` module to determine success or failure, record in logfile
12. clean up: delete structs and close files
```

## Avatar Programs
### avatar_thread
avatar_thread is a method to be created as a thread that performs the majority 
of the maze-solving work by calling functions from various modules.

### User Interface
The user’s interface with the main client program is the ASCII UI, which 
displays all of the known aspects of the maze including walls that have been 
discovered, coordinate positions of all of the avatars, and colored 
representations of each avatar's trail. Aspects of the maze that are unknown 
(such as walls that may exist but have not been discovered yet) are drawn as ---?---.

### Inputs and Outputs

Inputs:
* `char* hostname`
* `int maze_port`
* `char* filename`
* `int avatar_id`

Outputs
* Log File: The first line of the log file contains the maze_port number, user 
ID, and the date and time, written by the startup program. Then, the logfile 
documents all of the actions of each of the avatars. It ends with a note on 
whether the avatars succeeded in finding each other.

### Description of Function
Each `avatar_thread` instance will, upon reaching its turn, read the current 
state of the maze, calculate the best possible move by calling functions from 
the `avatar_solve` module, and then communicate the move to the server.


### Functional Decomposition into Modules
We had the following modules: 
* *avatar_comm* Module, which communicates with the Server and retrieves 
information from the mazestruct Module
* *avatar_solve* Module, which takes information obtained from the maze_struct 
and the Server and computes the next best move, which is sent to the Server.
* *mazestruct* Module, which contains the logic for making and drawing a maze data 
structure, which will be shared among all of the client threads.  

## avatar_comm Module
This module contains all the necessary functions for communication with the 
server. It allows messages to be constructed easily and parses the messages 
received with the necessary information. These commands will be used by both 
the Avatars and the startup program.

### avatar_comm Exported Functions
This module consists of the following functions, exported through avatar_comm.h
* `comm_t *comm_new()` is a function that initializes the comm_t struct that 
holds all the information relevant to the server-client communication, as
well as the maze information passed back and forth between the server and client.
* `bool send_init(comm_t *com, int nAvatars, int difficulty, char *hostname)` 
- Establishes a connection to the server, and sends the AM_INIT message. 
Returns true if and only if the init message was able to sent without running 
into errors.
* `int send_avatar_ready(comm_t *com, int avatarID)` - Establishes a connection 
through the mazeport and sends the AM_AVATAR_READY message to the server. 
Returns true if and only if the init message was able to sent without running 
into errors. 
* `bool send_move(comm_t *com, int avatarID, int Direction, int sock)` - Sends 
the AM_AVATAR_MOVE message. Returns true if and only if the init message was 
able to sent without running into errors.
* `bool receive_message(comm_t *com, int avatarID, int sock)` - Calling this 
function alerts the module that a new message should be received from the server. 
This function must be called whenever a server-to-client message is expected. 
In other words, it must be called any time a mesage is sent from the client 
to the server. Returns true if and only if there were no error messages returned 
from the server, such as "Unknown Avatar" or "Unknown Message Type," and there 
was a message to be received.
* `bool is_init_successful(comm_t *com)` - Returns whether or not the AM_INIT 
message was successfully sent and the server responded with AM_INIT_OK.
* `int get_mazeport(comm_t *com)` - Returns the mazeport.
* `int get_maze_width(comm_t *com)` - Returns the width of the maze.
* `int get_maze_height(comm_t *com)` - Returns the height of the maze.
* `int get_turnID(comm_t *com)` -  Gets the turnID of the current avatar that 
has to make a move. Precondition: receive_message() must be called after the 
send_move function in order to provide the accurate turnID.
* `XYPos *get_position_array(comm_t *com)` - Gets the positions of all of the 
avatars on the maze in a position array, where the array index represents the 
avatarID. Precondition: receive_message() must be called after the send_move 
function in order to provide the accurate position array.
* `int check_game_status(comm_t *com)` - Returns whether the game is over 
because the avatars were successful in finding each other, or because the 
movelimit or timelimit was reached, or because there was an error in the 
messaging. Returns 0 if the game is still in progress. Returns 1 if there was a 
server timeout. Returns 2 if the move limit was reached.
* `int get_hash(comm_t *com)`- A function to get the hashcode when the maze has 
been solved.
* `int get_nMoves (comm_t *com)` - A function to get the number of total moves 
when the maze has been solved.
* `int get_difficulty(comm_t *com)` - A function to get the difficulty when the 
maze has been solved.

## avatar_solve Module
This module contains all the necessary functions for solving the maze and 
determining the next steps each Avatar should take. It contains the bulk of 
the strategy.

### avatar_solve Exported Functions
The `avatar_solve` module exports four functions, three of which are called
in different circumstances in order to decide the next move an avatar should
take.

* `check_previous` is a function which checks if the previous move was 
sucessful or not. It takes the maze structure, a lastmove structure, the
log name, the strength of the path, and the counters which keeps track of
the followers. 
* If the current move is not the first, the function will begin checking the 
outcome of the last attempted move. It will figure out if there is a wall or
an opening, and whether the avatar has found a path or not. Paths are rewritten
if they are laid by someone below the avatar in the following chain. It updates
the log file with the outcome, modifies the maze, and changes the followers 
struct if necessary. It finishes by redrawing the maze if it has been 
modified. 
* `maze_solve` is the default move selection method for an Avatar in 
*Exploration Mode*. It takes the maze structure, the ID of the avatar, a 
pointer to the XYPos of the Avatar, and the name of the log file. It checks
all four adjacent squares, and attempts to move to another Avatar's path if 
possible. Otherwise, it moves towards an unknown direction to continue
exploring. If all squares are discovered, it backtraces. It writes its attempt 
to move to the log, then returns the desired move.
* `leader_solve` is a move used for the leader of all paths. It also takes the
maze structure, the ID of the avatar, a pointer to the XYPos of the Avatar, 
and the name of the log file. It selects the move that allows the Avatar to
backtrack its own trail, and writes its attempt to the log, then returns the 
desired move.
* `follower_solve` is a move used for an Avatar that is in *Following Mode* and
on the path of another Avatar. It takes the maze structure, the Avatar ID, the
pointer to the XYPos of the Avatar, the counters that keeps track of all 
followers, and the file name of the log. If it collides with its leader, it 
freezes, in order to allow the leader to select a direction. Otherwise, it 
attempts to move to a third Avatar's path if the third Avatar is above the first
avatar in the following chain. Otherwise, it continues to follow the path it is 
on. It writes its attempt to move to the log, then returns the desired move.

### avatar_solve Data Structures
The `avatar_solve` module exports one data structure, `move_t`. This structure
is comprised of two `int` variables, one which represents the `avatar_id` and
one which represents the `direction` of the move it wishes to take, where
0 - West, 1 - North, 2 - South, 3 - East. 

`move_t` is returned by the three functions within `avatar_solve` that choose
moves for the avatar: `maze_solve`, `leader_solve`, `follower_solve`. It 
contains the information needed to pass the attempted move to the server.

### Maze-solution algorithm: high-level description
The `avatar_solve` module has two primary modes: *Exploration*, and *Following*.

In *Exploration Mode*, avatars will explore the maze randomly, while leaving 
behind a “trail” of variables under `int strength`. The count begins at 0 and 
increments with every step. The program tags the current coordinate of the 
avatar in the `maze_struct` with `strength`, and tags the coordinate with its 
`avatar_id`.

If, in its random exploration, an avatar meets the trail left by some other 
avatar, it will enter *Following Mode* and follow the trail in ascending order; 
in this way, the follower avatar is guaranteed to trace the path of the avatar 
that it follows. If the follower avatar encounters along its path the path of 
another avatar, it will switch its leader if and only if the new avatar is a 
somewhere above its current following chain.

When all but one avatar is following another avatar (indicating that all avatars 
are in a “train” of sorts), the only avatar that remains a leader will stop its 
random exploration and backtrack until it meets another avatar.

The other avatars will keep following their trail and will eventually make their 
way to the coordinates of the only remaining leader.

In order to execute this algorithm we will make use of a `counters_t` that 
contains as its keys the `avatar_id` and the id of the avatar it is following as 
the count.

## Mazestruct Module

The `maze_t` structure is a two dimensional array of `mazesquare_t` 
structs, representing a coordinate plane of squares.

Each `mazesquare_t` contains information about a single square of the maze, 
including:
* the presence or absence of walls (North, South, East, West)
* whether this square has been visited
* “tag information,” if an avatar has visited and left a trail
* whether any avatars are currently inside it

In addition to the struct itself, mazestruct contains methods to update and get 
information about each square, to be used by the avatars as they decide which 
way to go, as well as a function to draw the maze in its current known form.

### Mazestruct Exported Functions
This module consists of the following functions, exported through mazestruct.h:

* `get_wall()` returns the wall status (wall, space, unknown) of the provided 
wall at the provided position.
* `get_tagged_by()` returns which avatar has currently tagged the provided 
square.
* `get_tag_strength()` returns the numerical strength of the tag currently on
the provided square, which is the strength of the trail on that square.
* `set_wall()` sets a given wall in the maze to a given value, based on whether
an avatar was just able to walk through it or not.
* `maze_new()` creates and returns a maze object, with mazesquare objects for
each coordinate within it.
* `visit()` tags a square with a certain strength by a certain avatar. This is 
usually only called the first time a square is visited, but it is also called in
some edge cases.
* `set_avatar_position()` changes which squares contain the given avatar; it is
placed inside the provided square, and removed from any square it could have 
been in previously. This is for the sole purpose of drawing the UI.
* `is_collision()` returns whether there are more than one avatar on the
provided square, as well as filling any avatars that are there into the provided
array.
* `get_num_avatars_here()` returns how many avatars are on the provided square, 
as well as filling any avatars that are there into the provided array. This 
function should only be used instead of `is_collision()` if it's necessary to
iterate over the filled array.
* `maze_delete()` frees the maze as well as all allocated structures within it.
* `draw_maze()` draws the provided maze into the terminal each time it is called,
drawing directly over any text that may be at the top of the screen, which is 
almost always simply the previous frame of the maze. Calling it every time a 
move is made makes for an animated UI effect in the terminal.

### Major Data Structures 
`maze_t` is a struct representing the maze itself, represented by a 
two-dimensional array of `mazesquare_t` objects, each of which contains 
information about the walls, tags, and avatars within it (see mazestruct.h for 
more info about each of these structures).

`avatar_t` is a struct, which hold information about a given avatar such as its 
ID, the current `step_count` of the avatar and the `avatarID` of the avatar that 
it is following.

`avatars` is a `set_t` data structure which stores information about each avatar 
that is relevant in algorithmically determining the next best move of each 
avatar. It takes the `avatarID` as keys, holding `avatar_t` items.

 `last_move` is a struct that holds the last move attempted by any of the 
 avatars in the maze. It holds two `XYPos` variables and one `int avatarID`. 
 `int avatarID` holds the avatarID of the last avatar that attempted to make a 
 move. `XYPos before` holds the position of the avatar before the move is 
 attempted, and `XYPos after` holds the position of the avatar after the move 
 attempted. The two may be the same, meaning a wall was encountered.

### Data Flow Through All Modules

1. `startup` parses command-line arguments, then messages the server with an 
`AM_INIT` message specifying `nAvatars` and `difficulty`.
2. `startup` receives `AM_INIT_OK`, parses the message.
4. `startup` then creates the threads running the `avatar_thread` method with 
necessary parameters. the threads, `avatar_thread`, are initialized and begin 
running.
5. The threads then each send the `AM_AVATAR_READY` message via the mazeport.
6. Then, the `avatar_thread` function enters into a while loop that terminates 
only when the game has ended, as determined by the `avatar_comm` module
	1. during the while loop, they communicate back and forth with the server 
	using `avatar_comm` 
	2. each avatar also writes to the logfile and updates the GUI
7. Game ends with error messages or `AM_MAZE_SOLVED` which is written to the logfile
8. Program terminates


## Testing Plan

The modules `mazestruct`, `avatar_comm`, and `avatar_solve` have each been unit
tested; see test files in each module as well as the sections within TESTING.md.

Additionally, we performed full integration testing, running most combos of 
n_avatars and difficulty and recording our scores, looking for any outliers as
well as testing for problems.

Finally, we stress-tested our threading by starting up a high number of 10 0 
mazes at once, which could possibly cause a socket issue. It did not.



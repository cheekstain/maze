# CS50 Final Project

**Arun Hari Anand, Bill Tang, Christina Lu, Emma Hobday, CS50 August 2017**

*Written using class materials provided by Prof. Zhou*

# Introduction

This program is intended to run a simulation of a set of maze-solving avatars whose
function is to efficiently find each other, subject to the constraint that they may
not exceed the maximum number of moves or the maximum amount of time allotted.

## Major Design Decisions
The maze program will be started with a C program. The avatars will be individual threads within a main C program with functions organized and categorized under across different modules.

* `avatar_comm`: refers to a function(s) related to communications with the server, parsing/naming messages, et cetera.
* `avatar_solve`: refers to a function(s) related to calculating and returning the next best move.
* `maze_struct`: refers to the data structures and related functions dealing with storage and logical structures of the maze.

# Startup Design

## AMStartup.c
AMStartup.c is a C program that takes command-line args, connects with the server to initialize the maze, and finally start the avatar threads to solve the maze.

### User Interface
Takes in command line arguments: `./AMStartup nAvatars=... Difficult=... Hostname=...`
* nAvatars: (`int`) the number of avatars in the maze
* Difficult: (`int`) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
* Hostname: (`char*`) the hostname of the server

### Inputs and Outputs

Inputs:
* `int nAvatars`: number of avatars within maze
* `int Difficulty`: ranging from 0 to 9, the difficulty of the maze
* `char* Hostname`: the hostname of the server 

Outputs:
* `file Amazing_$USER_N_D.log`: where $USER is the current userid, N is the value of nAvatars and D is the value of Difficulty; passed to client program, avatars.c
* `int nAvatars`, `MazeHeight`, `MazeWidth`, `MazePort`; `char* Hostname`: parsed from server, passed to client program, `avatars.c`
* error messages: printed to stderr 

### Pseudocode
1. validate arguments
2. construct `AM_INIT` struct
3. write `AM_INIT` to server
4. wait to receive `AM_INIT_OK`
5. extract `MazePort`, `MazeHeight`, and `MazeWidth`
6. create log file Amazing_$USER_N_D.log
7. write $USER, the MazePort, and the date and time to log file
8. call avatars.c with nAvatars, Difficulty, MazeHeight, MazeWidth, Host name, MazePort, and the file name of the log


It initializes a maze data structure (as specified in the maze_struct module) as well as a counters data structure used for keeping track of what path each Avatar has found.

It then initializes the the nAvatars number of threads, which represent each Avatar, with incrementing int IDs, and starts them on the maze solving function.













## Avatar Program
### avatar_thread
avatar_thread is a method to be created as a thread that performs the majority of the maze-solving work by calling functions from various modules.

### User Interface
The user’s interface with the main client program is the ASCII UI, which displays all of the known aspects of the maze including walls that have been discovered, and coordinate positions of all of the avatars. Aspects of the maze that are unknown (such as walls that may exist but have not been discovered yet) will be ignored in rendering this ASCII UI.

### Inputs and Outputs

Inputs:
* `int nAvatars`
* `int Difficulty` 
* `int MazeHeight, MazeWidth`
* `char* Hostname`
* `int MazePort`
* `char* filename`

Outputs
* Log File: The first line of the log file contains the MazePort, AvatarId, and the date and time. Then, the logfile documents all of the actions of each of the avatars. It ends with a note on whether the avatars succeeded in finding each other.

### Description of Function
Each `avatar_thread` instance will, upon reaching its turn, read the current state of the maze, calculate the best possible move according to the maze-solution algorithm, and then communicate the move to the server.

### Maze-solution algorithm: high-level description
The `avatar_solve` module has two primary modes: **Exploration**, and **Following**.

In **Exploration Mode,** avatars will explore the maze randomly, while leaving behind a “trail” of variables under `int step_count`. The count begins at 0 and increments with every step. The program tags the current coordinate of the avatar in the `maze_struct` with `step_count`, and tags the coordinate with its `avatarID`.

If, in its random exploration, an avatar meets the trail left by some other avatar, it will begin to follow the trail in ascending order; in this way, the follower avatar is guaranteed to trace the path of the avatar that it follows. If the follower avatar encounters along its path the path of another avatar, it will switch its leader if and only if the new avatar is a leader of the avatar that it currently follows. 

When all but one avatar is following another avatar (indicating that all avatars are in a “train” of sorts), the only avatar that remains a leader will stop its random exploration and backtrack until it meets another avatar.

The other avatars will keep following their trail and will eventually make their way to the coordinates of the only remaining leader.

In order to execute this algorithm we will make use of a `set_t` that contains as its keys the `avatarID` and as its item a struct called `avatar_t` that encapsulates all of the information specific to an avatar that is relevant in solving the maze, including the current `step_count` with which each avatar tagged the current co-ordinate and the number of the avatar that each avatar is currently following.

### Functional Decomposition into Modules
We anticipate the following modules: 
* *avatar_comm* Module, which communicates with the Server and retrieves information from the mazestruct Module
* *avatar_solve* Module, which takes information obtained from the maze_struct and the Server and computes the next best move, which is sent to the Server.
* *maze_struct* Module, which contains the logic for making a maze data structure, which will be shared among all of the client threads.  

### maze_struct Design

The `maze_struct` structure will be a two dimensional array of `maze_square` structs, representing a coordinate plane of squares.

Each `maze_square` contains information about a single square of the maze, including:
* the presence or absence of walls (North, South, East, West)
* whether this square has been visited
* “tag information,” if an avatar has visited and left a trail
* whether any avatars are currently inside it

In addition to the struct itself, mazestruct contains methods to update and get information about each square, to be used by the avatars as they decide which way to go, as well as a function to draw the maze in its current known form.

### Major Data Structures 
`maze_t` is a struct representing the maze itself, represented by a two-dimensional array of `mazesquare_t` objects, each of which contains information about the walls, tags, and avatars within it (see mazestruct.c for more info about each of these structures).

`avatar_t` is a struct, which hold information about a given avatar such as its ID, the current `step_count` of the avatar and the `avatarID` of the avatar that it is following.

`avatars` is a `set_t` data structure which stores information about each avatar that is relevant in algorithmically determining the next best move of each avatar. It takes the `avatarID` as keys, holding `avatar_t` items.

 `last_move` is a struct that holds the last move attempted by any of the avatars in the maze. It holds two `XYPos` variables and one `int avatarID`. `int avatarID` holds the avatarID of the last avatar that attempted to make a move. `XYPos before` holds the position of the avatar before the move is attempted, and `XYPos after` holds the position of the avatar after the move attempted. The two may be the same, meaning a wall was encountered.

### Data Flow Through Modules

### Pseudocode
1. `AMStartup` parses command-line arguments, then messages the server with an `AM_INIT` message specifying `nAvatars` and `difficulty`.
2. `AMStartup` receives `AM_INIT_OK`, parses the message.
3. Calls the `avatars.c` program with necessary parameters, which begins the threads. `avatars.c` also contains three major variables, namely: 
4. A common `mazestruct` data structure that contains all of the information gleaned about the maze as the search has progressed. This `mazestruct` module contains a few accessor and modifier methods that the avatars may use in the decision-making algorithm that picks the next best move.
5. A common *set_t* data structure that uses the avatarIDs as its keys and *avatar* structs as its items. These *avatar* structs contain information specific to each avatar that is valuable when picking the next best move. This struct will be elaborated upon in the maze-solving portion of this pseudocode section.
6. A struct named *last_move* which will hold two XYPos instances and one int avatarID. *last_move* holds the last move attempted by any avatar in the maze. “XYPos before” will hold the position of the avatar before the move was made, and “XYPos after” holds the position of the avatar after the move is attempted, provided the move is successfully made. “Int avatarID” will contain the avatarID of the avatar that attempted this move.
7. The threads, `avatar_thread`, are initialized with parameters in avatar.c and begin running.
8. The threads then each send the *AM_AVATAR_READY* message via the mazeport.
9. Then, the `avatar_thread` function enters into a while loop that terminates only when the game has ended, as determined by the *avatarComm* module.  
10. The turnID is determined using the avatarComm module, and if the turnID matches my avatarID (passed in as an argument):
	1. If the last_move attempted is not null (i.e) this is not the first move to be attempted:
	2. We first check the last move attempted and use the avatarComm module to see if the last move attempted was successful. 
	3. We then update the *mazeStruct* accordingly.
	4. Then, if the avatar which attempted the last move is currently on the path of another avatar that we are not currently following, then we will record in the *avatars* data structure that the last avatar that tried to make a move is now following the avatar whose path it is currently on. 
	5. If the last avatar that made a move is not following anyone (that is, it is not on anyone else’s path that has already been tagged with a step_count:
	6. Tag the new coordinate using *mazestruct*, specifying that this avatar is the first avatar to visit this coordinate. The step_count is also incremented, and this coordinate is tagged with this new step_count.
	7. The logfile is then updated to reflect the previous move.
	8. The UI is updated.
11. Then we focus on the current turn. If the current turn avatar is not following another avatar:
	1. If this is the not only avatar remaining that is not following another avatar’s path:
		2. If we see that we are one move away from moving into the path of another avatar (as determined by the *mazestruct* module) we will attempt to move into that spot. 
		3. Otherwise, we will then eliminate all of the directions that are known to have walls that prevent us from moving into that direction.
	2. We will then check to see whether we are at a dead end (that is, three directions are blocked and the only way out is the way we came). In this case, go back the way we came by checking the coordinates on all four directions, and selecting the direction which contains the lowest step_count that has been tagged by the current avatar whose turn it is.
	3. Or else, look at all of the directions that currently unexplored and pick one of those at random to explore.
	4. Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
	5. Then, the last_move struct is updated accordingly to record the move that the avatar just attempted to make. 
12. Otherwise (meaning this is the only avatar remaining that is not following another avatar’s path):
	1. Backtrack and follow the path that the current avatar has just come from by checking the coordinates on all four directions, and selecting the direction which contains the lowest step_count that has been tagged by the current avatar whose turn it is.
	2. Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
	3. Then, the last_move struct is updated accordingly to record the move that the avatar just attempted to make. 
13. Else (meaning it is following another avatar):
	1. We will check all four directions and check if one of the neighbouring coordinates was tagged as being part of the path of another avatar that we are not currently following. 
	2. If so, and if the avatar we are currently following happens to be following the path of the newly discovered avatar, then we will attempt to move in that direction (such that we are now on the path of the newly discovered avatar)
	3. Otherwise, we will check the coordinates on all four directions to see which ones are part of the path of the avatar we are currently following. If we find multiple coordinates, we will pick the coordinate with the highest step_count.
	4. Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
	5. Then, the last_move struct is updated accordingly to record the move that this avatar just attempted to make. 
14. Once the game has ended, we use the avatarComm module to determine success or failure, and we use the logfile to record this.
15. We then clean-up all data structures and exit.


## Testing Plan

We’re planning on an integration testing approach where we start with small, simple mazes and work our way up to more difficult mazes. We will then check for inefficiencies in our algorithm and ensure that these inefficiencies are dealt with as appropriate. We will also ensure that the observed behavior of the avatars exactly match the intentions of the algorithm as laid out in the pseudocode section of this design document. If the observed behavior does diverge from the expected behavior, we will then debug our code such that these issues are sorted out and the program works as intended.
Additionally, separate modules, such as the mazestruct module, are going to be unit tested using a separate script.


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

## startup.c
startup.c is a C program that takes command-line args, connects with the server to initialize the maze, and finally start the avatar threads to solve the maze.

### User Interface
Takes in command line arguments: `./startup n_avatars=... difficulty=... hostname=...`
* n_avatars: (`int`) the number of avatars in the maze
* difficulty: (`int`) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
* hostname: (`char*`) the hostname of the server

### Inputs and Outputs

Inputs:
* `int n_avatars`: number of avatars within maze
* `int difficulty`: ranging from 0 to 9, the difficulty of the maze
* `char* hostname`: the hostname of the server 

Outputs:
* `file Amazing_$USER_N_D.log`: where $USER is the current userid, N is the value of n_avatars and D is the value of difficulty; open for appending

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
	1. a `mazestruct` data structure that contains all of the information gleaned about the maze as the search has progressed. This `mazestruct` module contains a few accessor and modifier methods that the avatars may use in the decision-making algorithm that picks the next best move.
	2. a `set_t` data structure that uses the avatarIDs as its keys and `avatar_t` structs as its items. These `avatar_t` structs contain information specific to each avatar that is valuable when picking the next best move. This struct will be elaborated upon in the maze-solving portion of this pseudocode section.
	3. a struct named `last_move` which will hold two XYPos instances and one int avatarID. `last_move` holds the last move attempted by any avatar in the maze. `XYPos before` will hold the position of the avatar before the move was made, and `XYPos after` holds the position of the avatar after the move is attempted, provided the move is successfully made. `int avatarID` will contain the avatarID of the avatar that attempted this move.
9. initialize `n_avatars` threads, call `avatar_thread` on each
10. for loop waiting for threads to terminate
11. use the `avatar_comm` module to determine success or failure, record in logfile
12. clean up: delete structs and close files
```

## Avatar Programs
### avatar_thread
avatar_thread is a method to be created as a thread that performs the majority of the maze-solving work by calling functions from various modules.

### User Interface
The user’s interface with the main client program is the ASCII UI, which displays all of the known aspects of the maze including walls that have been discovered, and coordinate positions of all of the avatars. Aspects of the maze that are unknown (such as walls that may exist but have not been discovered yet) will be ignored in rendering this ASCII UI.

### Inputs and Outputs

Inputs:
* `char* hostname`
* `int maze_port`
* `char* filename`
* `int avatar_id`

Outputs
* Log File: The first line of the log file contains the maze_port number, user ID, and the date and time, written by the startup program. Then, the logfile documents all of the actions of each of the avatars. It ends with a note on whether the avatars succeeded in finding each other.

### Description of Function
Each `avatar_thread` instance will, upon reaching its turn, read the current state of the maze, calculate the best possible move by calling functions from the `avatar_solve` module, and then communicate the move to the server.


### Functional Decomposition into Modules
We anticipate the following modules: 
* *avatar_comm* Module, which communicates with the Server and retrieves information from the mazestruct Module
* *avatar_solve* Module, which takes information obtained from the maze_struct and the Server and computes the next best move, which is sent to the Server.
* *maze_struct* Module, which contains the logic for making a maze data structure, which will be shared among all of the client threads.  

### avatar_comm Module
This module contains all the necessary functions for communication with the server. It allows messages to be constructed easily and parses the messages received with the necessary information. These commands will be used by both the Avatars and the startup program.

### avatar_solve Module
This module contains all the necessary functions for solving the maze and determining the next steps each Avatar should take. It will contain the bulk of the strategy.

#### Maze-solution algorithm: high-level description
The `avatar_solve` module has two primary modes: **Exploration**, and **Following**.

In **Exploration Mode,** avatars will explore the maze randomly, while leaving behind a “trail” of variables under `int step_count`. The count begins at 0 and increments with every step. The program tags the current coordinate of the avatar in the `maze_struct` with `step_count`, and tags the coordinate with its `avatarID`.

If, in its random exploration, an avatar meets the trail left by some other avatar, it will enter **Following Mode** and follow the trail in ascending order; in this way, the follower avatar is guaranteed to trace the path of the avatar that it follows. If the follower avatar encounters along its path the path of another avatar, it will switch its leader if and only if the new avatar is a leader of the avatar that it currently follows. 

When all but one avatar is following another avatar (indicating that all avatars are in a “train” of sorts), the only avatar that remains a leader will stop its random exploration and backtrack until it meets another avatar.

The other avatars will keep following their trail and will eventually make their way to the coordinates of the only remaining leader.

In order to execute this algorithm we will make use of a `set_t` that contains as its keys the `avatarID` and as its item a struct called `avatar_t` that encapsulates all of the information specific to an avatar that is relevant in solving the maze, including the current `step_count` with which each avatar tagged the current co-ordinate and the number of the avatar that each avatar is currently following.

#### Maze-solution Pseudocode 

```
1. The `turnID` is determined using the `avatarComm` module, and if the `turnID` matches my `avatarID`:
	1. If the `last_move` attempted is not null (i.e) this is not the first move to be attempted:
	2. We first check the last move attempted and see if it found a wall, and update the *maze_struct* accordingly.
	3. We check to see if the previous avatar’s move found a path; if so, then set the previous avatar’s `maze_solve` mode to **Following.** 
	4. If not, then tag the `maze_square` with the trail and increment step count.
	5. Update logfile, UI.
2. Then we focus on the current turn. If the current turn avatar is not following another avatar:
	1. If this is the not only avatar remaining that is not following another avatar’s path:
		1. Move onto another Avatar’s path if possible.
		2. If that has failed, eliminate all directions with known walls, and pick a random open direction to move towards, priority given to unexplored directions
		3. Communicate to the server the new move, and update `last_move` 
	2. Otherwise (meaning this is the only avatar remaining that is not following another avatar’s path)
		1. Backtrack along the avatar’s own trail.
		2. Communicate to the server the new move, and update `last_move`. 
3. Else (meaning it is following another avatar):
		1. Check if there exists a third Avatar’s path that we can move onto.
		2. Check if said path belongs to the leader of our leader. If so, move onto it and change the path that the avatar is following.
		3. Otherwise, continue following the trail.
		4. Communicate to the server the new move, and update `last_move` 
3. Update visualization
4. Write move details to logfile
```

### maze_struct Module

This module contains the functions necessary for constructing and utilizing a `maze_struct` structure. It also contains methods for the visualization of the maze in a GUI.

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

### Data Flow Through All Modules

1. `startup` parses command-line arguments, then messages the server with an `AM_INIT` message specifying `nAvatars` and `difficulty`.
2. `startup` receives `AM_INIT_OK`, parses the message.
4. `startup` then creates the threads running the `avatar_thread` method with necessary parameters. the threads, `avatar_thread`, are initialized and begin running.
5. The threads then each send the `AM_AVATAR_READY` message via the mazeport.
6. Then, the `avatar_thread` function enters into a while loop that terminates only when the game has ended, as determined by the `avatar_comm` module
	1. during the while loop, they communicate back and forth with the server using `avatar_comm` 
	2. each avatar also writes to the logfile and updates the GUI
7. Game ends with error messages or `AM_MAZE_SOLVED` which is written to the logfile
8. Program terminates


## Testing Plan

We’re planning on an integration testing approach where we start with small, simple mazes and work our way up to more difficult mazes. We will then check for inefficiencies in our algorithm and ensure that these inefficiencies are dealt with as appropriate. We will also ensure that the observed behavior of the avatars exactly match the intentions of the algorithm as laid out in the pseudocode section of this design document. If the observed behavior does diverge from the expected behavior, we will then debug our code such that these issues are sorted out and the program works as intended.

Additionally, separate modules, such as the mazestruct module, are going to be unit tested using a separate script.


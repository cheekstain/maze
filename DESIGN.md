# CS50 Final Project
## Arun Hari Anand, Bill Tang, Christina Lu, Emma Hobday, CS50 August 2017
## Written using class materials provided by Prof. Zhou

This program is intended to run a simulation of a set of maze-solving avatars whose
function is to efficiently find each other, subject to the constraint that they may
not exceed the maximum number of moves or the maximum amount of time allotted.

## Major Design Decisions
The maze program will be started with a bash script. The avatars will be individual threads within a main C program with functions organized across different modules.

[list of shared modules with brief descriptions]

## Brief Overview

[table of contents, sort of, for the design doc]
AMStartup
Avatars

## Startup Design

### AMStartup.sh
AMStartup.sh is a bash script that takes in arguments from the command line and uses them communicate with the server in order to initialize the maze and call the avatar C program which starts the threads to solve the maze.

### User Interface
Takes in command line arguments: `./AMStartup nAvatars=... Difficult=... Hostname=...`
* nAvatars: (`int`) the number of avatars in the maze
* Difficult: (`int`) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
* Hostname: (`char*`) the hostname of the server

### Inputs and Outputs

Inputs:
* int nAvatars: number of avatars within maze
* int Difficulty: ranging from 0 to 9, the difficulty of the maze
* char* Hostname: the hostname of the server

Outputs:
* file Amazing_$USER_N_D.log: where $USER is the current userid, N is the value of nAvatars and D is the value of Difficulty; passed to client program, avatars.c
* int nAvatars, MazeHeight, MazeWidth, MazePort; char* Hostname: parsed from server, passed to client program, avatars.c
* error messages: printed to stderr

### Functional Decomposition to Modules
### Major Data Structures
### Dataflow
[for the three above: none? idk if i have to add since its a script?]

### Pseudocode
[modify]
1. validate arguments
2. construct `AM_INIT` struct
3. write `AM_INIT` to server
4. wait to receive `AM_INIT_OK`
5. extract `MazePort`, `MazeHeight`, and `MazeWidth`
6. create log file Amazing_$USER_N_D.log
7. write $USER, the MazePort, and the date and time to log file
8. call avatars.c with nAvatars, Difficulty, MazeHeight, MazeWidth, Host name, MazePort, and the file name of the log















## Avatar Program

### avatars.c
avatars.c is a C program that holds the main function and performs the majority of the maze-solving work by calling functions from various modules.

### User Interface
The user’s interface with the main client program is the ASCII UI, which displays all of the known aspects of the maze including walls that have been discovered, and coordinate positions of all of the avatars. Aspects of the maze that are unknown (such as walls that may exist but have not been discovered yet) will be ignored in rendering this ASCII UI.

### Inputs and Outputs

Inputs:
* int nAvatars
* int Difficulty [unsure if necessary]
* int MazeHeight, MazeWidth
* char* Hostname
* int MazePort
* char* filename: of the log

Outputs
* writes to log file: The first line of the logfile contains the MazePort, AvatarId, and the date and time. Then, the logfile documents all of the actions of each of the avatars. It ends with
a note on whether the avatars succeeded in finding each other.

It initializes a maze data structure (as specified in the maze_struct module) as well as a counters data structure used for keeping track of what path each Avatar has found.

It then initializes the the nAvatars number of threads, which represent each Avatar, with incrementing int IDs, and starts them on the maze solving function.

After the maze has been solved, the program cleans up by freeing the memory for all data structures.



At each step in the maze, the UI will show an ASCII depiction of the maze as the avatars know it so far

### Maze-solution algorithm: high-level description
The `make_move` function uses an algorithm that depends on the avatars leaving a trail of “path_ids” on each of the maze coordinates that they occupy as they progress through a random exploration of the maze.
In their random exploration, avatars will not be allowed to explore coordinates that other avatars have explored.
These path_ids begin at zero for all of the avatars when they first begin the maze. Then, when an avatar (with avatarID of, say, 10) moves to the next coordinate in its exploration, it will increment its path_id by one. Then it will tag this coordinate in the maze (using the *mazestruct* module) with the new path_id. If it is the first avatar to explore this coordinate, it will also tag this coordinate with its own avatarID (in this case, 10), specifying that it is the first avatar to explore this coordinate.
If, in its random exploration, an avatar (with avatarID = 10 for example) happens to run into the trail of path_ids left by some other avatar (with avatarID 20, for example), then it will start to follow the trail of path_ids left by this avatar (of avatarID 20), in ascending order of path_ids. We will henceforth refer to avatarID = 20 as being the “leader avatar” and avatarID = 10 as the “follower avatar.”
The leader avatar will continue its random exploration, and the follower avatar will follow its leader by following its path_ids.
When the follower avatar (avatar 10) encounters along its path another avatar (for example avatar 30), it will switch its “leader” from avatar 20 to avatar 30 if and only if avatar 30 is a leader of the avatar you are currently following (avatar 20).
When all but one avatar is following another avatar, the only avatar that remains a leader will stop its random exploration and backtrack until it meets another avatar.
The other avatars will keep following their trail and will eventually make their way to the coordinates of the only remaining leader.
In order to execute this algorithm we will make use of a *set_t* that contains as its keys the *avatarID* and as its item a struct called *avatar_t* that encapsulates all of the information specific to an avatar that is relevant in solving the maze, including the current path_id number with which each avatar tagged the current co-ordinate and the number of the avatar that each avatar is currently following (-1 if the avatar is not currently following any avatar).

### Functional Decomposition into Modules
We anticipate the following modules:
* avatarComm Module, which communicates with the Server and retrieves information from the mazestruct Module
* avatarSolve Module, which takes information obtained from mazestruct and the Server and computes the next best move, which is sent to the Server.
*mazeStruct, which contains the logic for making a maze data structure, which will be shared among all of the client threads.  The maze structure will be a two dimensional array of mazesquare datastructures, each of which contains information about that square of the maze, such as the wall status at its north, south, east, and west; each status will be either 0 if no wall, 1 if a wall, or -1 if unknown.  It also contains a boolean declaring whether that square has been visited or not.
Additionally, each square is “tagged” by the avatar that first visits it, and with a tag_strength that increases with each step that the avatar takes, so that other avatars will be able to follow the trail-laying avatar.
Finally, each square contains an array of which avatars are currently inside it.
In addition to the struct itself, mazestruct contains methods to update and get information about each square, to be used by the avatars as they decide which way to go.
* userInterface module, which contains the logic for rendering the ASCII UI. It is given all of the details of the maze by mazeStruct, and it then draws all of the maze to stdout.

### Major Data Structures
*maze_t* is a struct representing the maze itself, represented by a two-dimensional array of mazesqaure_t objects, each of which contains information about the walls, tags, and avatars within it (see mazeStruct.c for more info about each of these structures).
*avatars* is a *set_t* data structure which stores information about each avatar that is relevant in algorithmically determining the next best move of each avatar. It takes the avatarIDs as keys, and the its corresponding items are *avatar* structs, which each hold information about the avatar such as the current path_id of the avatar and the number of the avatar that it is following (-1 if it is not following any avatars).
*last_move* is a struct that holds the last move attempted by any of the avatars in the maze. It holds two XYPos variables and one int avatarID. *int avatarID* holds the avatarID of the last avatar that attempted to make a move. *XYPos before* holds the position of the avatar before the move is attempted, and *XYPos after* holds the position of the avatar after the move attempted, provided that this move is successfully made (that is, there are no walls preventing this from occurring).

### Data Flow Through Modules


### Pseudocode
 *AMStartup* parses command-line arguments, then messages the server with an *AM_INIT* message specifying *nAvatars* and *difficulty*.
*AMStartup* receives *AM_INIT_OK*, parses the message.
 Calls the *avatars.c* program with necessary parameters, which begins the threads. *avatars.c* also contains three global variables, namely:
A common *mazeStruct* data structure that contains all of the information gleaned about the maze as the search has progressed. This *mazeStruct* module contains a few accessor and modifier methods that the avatars may use in the decision-making algorithm that picks the next best move.
A common *set_t* data structure that uses the avatarIDs as its keys and *avatar* structs as its items. These *avatar* structs contain information specific to each avatar that is valuable when picking the next best move. This struct will be elaborated upon in the maze-solving portion of this pseudocode section.
A struct named *last_move* which will hold two XYPos instances and one int avatarID. *last_move* holds the last move attempted by any avatar in the maze. “XYPos before” will hold the position of the avatar before the move was made, and “XYPos after” holds the position of the avatar after the move is attempted, provided the move is successfully made. “Int avatarID” will contain the avatarID of the avatar that attempted this move.
The threads will be made to call the `run_avatar` function, which takes as a parameter the *avatarID* of the avatar in question.
The threads then each send the *AM_AVATAR_READY* message via the mazeport.
 Then, the `run_avatar` function enters into a while loop that terminates only when the game has ended, as determined by the *avatarComm* module.  
The turnID is determined using the avatarComm module, and if the turnID matches my avatarID (passed in as an argument):
If the last_move attempted is not null (i.e) this is not the first move to be attempted:
We first check the last move attempted and use the avatarComm module to see if the last move attempted was successful.
We then update the *mazeStruct* accordingly.
Then, if the avatar which attempted the last move is currently on the path of another avatar that we are not currently following, then we will record in the *avatars* data structure that the last avatar that tried to make a move is now following the avatar whose path it is currently on.
If the last avatar that made a move is not following anyone (that is, it is not on anyone else’s path that has already been tagged with a path_id:
Tag the new coordinate using *mazestruct*, specifying that this avatar is the first avatar to visit this coordinate. The path_id is also incremented, and this coordinate is tagged with this new path_id.
The logfile is then updated to reflect the previous move.
The UI is updated.
Then we focus on the current turn. If the current turn avatar is not following another avatar:
If  this is the not only avatar remaining that is not following another avatar’s path:
If we see that we are one move away from moving into the path of another avatar (as determined by the *mazestruct* module) we will attempt to move into that spot.
Otherwise, we will then eliminate all of the directions that are known to have walls that prevent us from moving into that direction.
We will then check to see whether we are at a dead end (that is, three directions are blocked and the only way out is the way we came). In this case, go back the way we came by checking the coordinates on all four directions, and selecting the direction which contains the lowest path_id that has been tagged by the current avatar whose turn it is.
Or else, look at all of the directions that currently unexplored and pick one of those at random to explore.
Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
Then, the last_move struct is updated accordingly to record the move that the avatar just attempted to make.
Otherwise (meaning this is the only avatar remaining that is not following another avatar’s path):
Backtrack and follow the path that the current avatar has just come from by checking the coordinates on all four directions, and selecting the direction which contains the lowest path_id that has been tagged by the current avatar whose turn it is.
Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
Then, the last_move struct is updated accordingly to record the move that the avatar just attempted to make.
Else (meaning it is following another avatar):
We will check all four directions and check if one of the neighbouring coordinates was tagged as being part of the path of another avatar that we are not currently following.
If so, and if the avatar we are currently following happens to be following the path of the newly discovered avatar, then we will attempt to move in that direction (such that we are now on the path of the newly discovered avatar)
Otherwise, we will check the coordinates on all four directions to see which ones are part of the path of the avatar we are currently following. If we find multiple coordinates, we will pick the coordinate with the highest path_id.
Then the *avatarComm* module is used to communicate to the server that this is the next move that the avatar wishes to make.
Then, the last_move struct is updated accordingly to record the move that this avatar just attempted to make.
Once the game has ended, we use the avatarComm module to determine success or failure, and we use the logfile to record this.
We then clean-up all data structures and exit.

## Testing Plan

We’re planning on an integration testing approach where we start with small, simple mazes and work our way up to more difficult mazes. We will then check for inefficiencies in our algorithm and ensure that these inefficiencies are dealt with as appropriate. We will also ensure that the observed behavior of the avatars exactly match the intentions of the algorithm as laid out in the pseudocode section of this design document. If the observed behavior does diverge from the expected behavior, we will then debug our code such that these issues are sorted out and the program works as intended.
Additionally, separate modules, such as the mazestruct module, are going to be unit tested using a separate script.

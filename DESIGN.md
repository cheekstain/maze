# CS50 Final Project
## Arun Hari Anand, CS50 August 2017
## Written using class materials provided by Prof. Zhou

<High level description of the program and its function>
This program is intended to run a simulation of a set of maze-solving avatars whose
function is to efficiently find each other, subject to the constraint that they may
not exceed the maximum number of moves and the maximum amount of time allotted.

### User Interface
The program has only one interface with the user: the command-line. It must always
have three arguments: the number of avatars, a number indicating the difficulty
of the maze and the hostname. An example run is as follows: <example usage here>

### Inputs and Outputs
The primary inputs are the command line parameters. In addition to this, our client
program is provided information about the maze itself (including its dimensions and
the existence of walls) and the location of each of the avatars within the maze.

The program's output consists of a log file, which holds information about the progress
of the avatars in finding each other, including whether or not they were successful.
The first line of the logfile contains the MazePort, AvatarId, and the date and time.
Then, the logfile documents all of the actions of each of the avatars. It ends with
a note on whether the avatars succeeded in finding each other.

### Functional Decomposition into Modules
Some ideas/things to discuss:
1. Threads or processes?
2. Amazing.h
3. AMStartup.c/AMStartup.sh?
4. AMStartup then sets up the number of threads necessary and calls the main client software
5. One module that holds all of the information obtained about the grid
6. One module that holds all of the logic needed by the avatars to explore the board
7. One module that holds all of the logic to "find each other" once all of the paths intersect
8. Main client program would then call each of these helper modules


### Pseudocode for Logic/Algorithmic Flow
1. AMStartup sends initializing message to the server, extracts Mazeport, .
2. 


### Data Flow Through Modules


### Major Data Structures


### Testing Plan

## CS50 Final Project: The Maze Challenge

Avatars are generated within a true maze, with no knowledge of the maze walls. 
The goal is to navigate them through the maze until they find each other.

### Bashful Brigade

* Arun Anand
* Bill Tang
* Christina Lu
* Emma Hobday

### Usage

./AM_STARTUP n_avatars difficulty hostname
where
	- n_avatars is the number of avatars to put in the maze, between 1 and 9
	- difficulty is the numeric difficulty of the maze, between 0 and 9
	- hostname is the name of the host, which should be flume.dartmouth.edu

#### Modular Breakdown
The `avatar_comm` folder contains the files necessary to the `avatar_comm` module
which contains the methods that handle univeral communications with the server
for the program.

The `avatar_solve` folder contains the files necessary to the `avatar_solve`
module, which contains the methods, logic, and algorithms used in deciding
the navigation for the avatars at every step in the maze-solving process.

The `libcs50` folder contains our standard CS50 library which contains various
data structures from across the term.

The `mazestruct` folder contains the extremely detailed struct and functions
related to the data representation of the maze in our program. This includes
a GUI representation for the maze and avatars.

The root directory contains amazing.h, which was a provided reference for structs
and consts. The maze_pointers file contains several other structs crucial
to the functioning of the program; maze_pointers_t specifically contains a list
of references to a variety of separate structs and objects containing
information that each `avatar_thread` needs to know.

startup.c is our executable, which initializes all things and starts the main
program, which is written in the method `avatar_thread` in avatars.c; avatars.c
itself contains only the main `avatar_thread` and a few helper functions. 

`avatar_thread` makes use of the full range of modular functions from the
other files to accomplish the task at hand.

#### Assumptions

Communications with Server follow the standard laid out in the documentation. 
Any unexpected messages from the server will cause the program to crash.

The width and height of the maze will not be greater than 100.

The colors of the maze GUI only work on Linux-associated machines.

The terminal window must not be tampered with while the program is running, for
the sake of the UI. This includes resizing and zooming in or out.

#### Compilation

To compile, first `make clean`, then `make`.

To test, simply `make test`.
See [TESTING](TESTING.md) for details of testing and an example test run.

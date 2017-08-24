# CS50 Lab 5
## Bashful Brigade, CS50 Summer 2017

### Testing the Maze Project - Integration testing

In order to test, we used a testing script `solve_maze_test.sh` which tests the behavior of the program when presented with invalid inputs well as valid ones. In testing for invalid parameters, we tested whether an invalid number of parameters, an invalid number of avatars, an invalid difficulty or an invalid hostname caused the program to crash - in all these cases the program exited gracefully and printed out error messages. We then tested a number of valid inputs to see whether the maze-solving algorithm works as intended. Over the course of this integration testing, we did not discover any unexpected behavior from our program. The testing script is available for viweing, and the terminal outputs are presented in `test.out.` Additionally, the logfiles are also available for viewing.
Valgrind was also used on the tests to check to see whether any memory leaks occured. There were no memory leaks detected.

### Testing the Maze Project - Unit Testing

In addition to this integration testing, we also unit tested all of the individual modules (that lent themselves to unit testing). 

#### Avatar_comm
In testing the communications module, we first wrote a program called `commtest.c` which contains a main function. This main funciton first calls the `send_init` function, which and the success of this initialization is documented with a print statement when we receive the `AM_INIT_OK` message. Then, ten threads are created, each of which run a function that attempts to solve the maze using random moves. We then test to see whether 1. the maze is solved for two avatars with difficulty = 0, 2. whether the move limit is exceeded in harder difficulties 3. whether there are any error messages from the server and 4. whether there are any memory leaks in the communications module (using valgrind). Print statements were used in the communicaions module to monitor the progress of the program - these print statements have since been commented out. The test program and the testing output are available to view in `avatar_comm/commtest.c` and `avatar_comm/test.out` respectively.



### Testing output
All test outputs are presented in the file *test.out*



#!/bin/bash
#
# Easy way to test the overall maze solving algorithm
#
# Usage: ./solve_maze_test.sh
#
# Emma Hobday
# CS50 August 2017

echo "TESTING AM_STARTUP"

make clean
make

./AM_STARTUP 2 1 flume.cs.dartmouth.edu



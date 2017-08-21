# CS50 Tiny Search Engine (TSE) utility library

David Kotz, April 2016, 2017.
Xia Zhou, July 2016, 2017.

These modules support the TSE project.  Feel free to drop in your implementation of the data-structure modules, but **do not change any of the other source files in this directory.**

## Usage
To build `libcs50.a`, run `make`. Make sure that you have copied your set.c, counters.c, and hashtable.c here. 

To clean up, run `make clean`.

## Overview

 * bag - the *bag* data structure from Lab 3
 * counters - the *counters* data structure from Lab 3
 * file - functions to read files (includes readlinep)
 * hashtable - the *hashtable* data structure from Lab 3
 * jhash - the Jenkins Hash function used by hashtable
 * memory - handy wrappers for malloc/free
 * set - the *set* data structure from Lab 3
 * webpage - functions to load and scan web pages

#!/bin/bash
#
# AMStartup.sh - startup scripts
#
# usage: ./AMStartup.sh nAvatars=... Difficulty=... Hostname=...
#
# input: int number of avatars, int difficulty, string hostname 
#
# Christina Lu, August 2017

# variables

navatars=$1
difficulty=$2
name=$3

if [ "$#" -ne 3 ]; then
	echo "usage: ./AMStart.sh navatars=... difficulty=... hostname=..." >&2
	exit 1;
fi

re='^[0-9]+$' #regex expression
if ![ "$navatars" =~ $re ]; then
	echo "$navatars must be an integer" >&2
	exit 2;
fi

if ![ "$difficulty" =~ $re ]; then
	echo "$difficulty must be an integer" >&2
	exit 3;
fi

host "$name" 2>&1 > /dev/null
if [ $? -eq 0 ] ; then
        echo "$name is invald" >&2
        exit 4;
fi

#construct and send AM_INIT 
#wait to receive AM_INIT_OK, else exit

#extract
maze_port=
maze_width=
maze_height=

#filename
filename="Amazing_$USER_$navatars_$difficulty.log"
echo $USER, $maze_port, `date` > $filename

./avatars "$navatars $difficulty $maze_height $maze_width $name $maze_port $filename"





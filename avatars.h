/*
 * avatars.h - header file for CS50 'avatars' module
 * 
 * Module to deal with the behavior each avatar will follow once it is opened
 * as a thread.
 *
 * Bashful Brigage: Emma Hobday, August 2017
 */

#ifndef __MAZESTRUCT_H
#define __MAZESTRUCT_H

/******************************** avatar_thread *******************************/
/*
 * All the behavior that constitutes an avatar, this is what is called by each
 * thread at its creation.
 *
 * The one parameter is a pointer to a struct holding a bundle of information
 * necessary for the avatar to function, of the type `maze_pointers_t` which is
 * defined in the folder maze_pointer.
 */
void* avatar_thread(void *ptr);

/******************************** check_all_following *************************/
/*
 * Helper method to avatar_thread, this checks whether all but one avatars are 
 * following another avatar, which is important if the avatar calling this
 * is the only one not following. In that case, it should retrace its steps
 * until it runs into somebody.
 */
void check_all_following(void* follower, const int key, int count);


#endif //__MAZESTRUCT_H

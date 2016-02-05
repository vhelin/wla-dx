/*
 * Generic hashmap manipulation functions
 *
 * Originally by Elliot C Back - http://elliottback.com/wp/hashmap-implementation-in-c/
 *
 * Modified by Pete Warden to fix a serious performance problem, support strings as keys
 * and removed thread synchronization - http://petewarden.typepad.com
 */
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

/*
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */
typedef void *any_t;

/*
 * PFany is a pointer to a function that can take two any_t arguments
 * and return an integer. Returns status code..
 */
typedef int (*PFany)(any_t);

/*
 * map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

/*
 * Return an empty hashmap. Returns NULL if empty.
*/
extern map_t hashmap_new();

/*
 * An alternative method to iterate. Initializes the map's iterator variable
 * and returns the first element, or NULL if there's no element.
 */
extern any_t hashmap_begin_iteration(map_t in);

/*
 * Call after hashmap_begin_iteration. Returns the next element, or NULL for
 * the end of the hashmap.
 */
extern any_t hashmap_next_iteration(map_t in);

/*
 * Add an element to the hashmap. Return MAP_OK or MAP_OMEM.
 * The contents of the key are not copied, so they must remain available.
 */
extern int hashmap_put(map_t in, char* key, any_t value);

/*
 * Get an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int hashmap_get(map_t in, char* key, any_t *arg);

/*
 * Remove an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int hashmap_remove(map_t in, char* key);

/*
 * Free the hashmap
 */
extern void hashmap_free(map_t in);

/*
 * Free all elements in the hashmap (not the hashmap itself). Doesn't remove
 * the elements.
 */
extern void hashmap_free_all_elements(map_t in);

/*
 * Get the current size of a hashmap
 */
extern int hashmap_length(map_t in);

#endif

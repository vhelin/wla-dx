/*
 * Generic map implementation.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.h"
#include "crc32.h"

#define INITIAL_SIZE (256)

/* We need to keep keys and values */
typedef struct _hashmap_element{
  char* key;
  int in_use;
  any_t data;
  struct _hashmap_element *next;
} hashmap_element;

/* A hashmap has some maximum size and current size,
 * as well as the data to hold. */
typedef struct _hashmap_map{
  int table_size;
  int size;
  hashmap_element *data;
  int iterator;
  int iterator_lower;
} hashmap_map;

/*
 * Return an empty hashmap, or NULL on failure.
 */
map_t hashmap_new() {

  hashmap_map* m = (hashmap_map*) calloc(sizeof(hashmap_map), 1);
  if (!m)
    goto err;

  m->data = (hashmap_element*) calloc(INITIAL_SIZE, sizeof(hashmap_element));
  if (!m->data)
    goto err;

  m->table_size = INITIAL_SIZE;
  m->size = 0;

  return m;

 err:
  
  if (m)
    hashmap_free(m);

  return NULL;
}

/*
 * Hashing function for a string
 */
unsigned int hashmap_hash_int(hashmap_map * m, char* keystring) {

  unsigned long key = crc32((unsigned char*)(keystring), (unsigned int)strlen(keystring));

  /* Robert Jenkins' 32 bit Mix Function */
  key += (key << 12);
  key ^= (key >> 22);
  key += (key << 4);
  key ^= (key >> 9);
  key += (key << 10);
  key ^= (key >> 2);
  key += (key << 7);
  key ^= (key >> 12);

  /* Knuth's Multiplicative Method */
  key = (key >> 3) * 2654435761;

  return key % m->table_size;
}

/*
 * Return the pointer to the element to use for the key, or NULL to request
 * a larger table.
 */
hashmap_element* hashmap_hash(map_t in, char* key) {

  int hash;
  hashmap_element *e, *p = NULL;

  /* Cast the hashmap */
  hashmap_map* m = (hashmap_map *) in;

  /* If full, return immediately */
  if (m->size >= (m->table_size/2))
    return NULL;

  /* Find the best index */
  hash = hashmap_hash_int(m, key);

  e = &m->data[hash];
  if (e->in_use == 0)
    return e;

  /* Linear probing - check if the key exists */
  while (e != NULL) {
    if (strcmp(e->key,key)==0)
      return e;

    p = e;
    e = e->next;
  }

  p->next = calloc(1, sizeof(hashmap_element));
  p->next->next = NULL;

  return p->next;
}

/*
 * Doubles the size of the hashmap, and rehashes all the elements
 */
int hashmap_rehash(map_t in) {

  int i;
  int old_size;
  hashmap_element* curr;
  hashmap_element *e, *p;

  /* Setup the new elements */
  hashmap_map *m = (hashmap_map *) in;
  hashmap_element* temp = (hashmap_element *) calloc(2 * m->table_size, sizeof(hashmap_element));
  if (!temp)
    return MAP_OMEM;

  /* Update the array */
  curr = m->data;
  m->data = temp;

  /* Update the size */
  old_size = m->table_size;
  m->table_size = 2 * m->table_size;
  m->size = 0;

  /* Rehash the elements */
  for (i = 0; i < old_size; i++) {
    int first = 1;

    if (curr[i].in_use == 0)
      continue;

    e = &curr[i];
    while (e != NULL) {
      int status;

      status = hashmap_put(m, e->key, e->data);
      if (status != MAP_OK)
        return status;

      p = e;
      e = e->next;

      if (first == 0)
        free(p);
      first = 0;
    }
  }

  free(curr);

  return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key
 */
int hashmap_put(map_t in, char* key, any_t value) {

  hashmap_element *e;
  hashmap_map* m;

  /* Cast the hashmap */
  m = (hashmap_map *) in;

  /* Find a place to put our value */
  e = hashmap_hash(in, key);
  while (e == NULL) {
    if (hashmap_rehash(in) == MAP_OMEM) {
      return MAP_OMEM;
    }
    e = hashmap_hash(in, key);
  }

  /* Set the data */
  e->data = value;
  e->key = key;
  e->in_use = 1;

  m->size++; 

  return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key
 */
int hashmap_get(map_t in, char* key, any_t *arg) {

  int curr;
  hashmap_map* m;
  hashmap_element *e;

  /* Cast the hashmap */
  m = (hashmap_map *) in;

  /* Find data location */
  curr = hashmap_hash_int(m, key);

  if (arg != NULL)
    *arg = NULL;

  e = &m->data[curr];
  if (e->in_use == 0)
    return MAP_MISSING;

  /* Linear probing, if necessary */
  while (e != NULL) {
    if (strcmp(e->key,key)==0) {
      if (arg != NULL)
        *arg = e->data;
      return MAP_OK;
    }
    e = e->next;
  }

  /* Not found */
  return MAP_MISSING;
}

any_t hashmap_begin_iteration(map_t in) {

  int i;

  /* Cast the hashmap */
  hashmap_map* m = (hashmap_map*) in;

  /* On empty hashmap, return immediately */
  if (hashmap_length(m) <= 0)
    return NULL;        

  /* Linear probing */
  for (i = 0; i< m->table_size; i++) {
    if (m->data[i].in_use != 0) {
      m->iterator = i;
      m->iterator_lower = 1;
      return m->data[i].data;
    }
  }

  return NULL;
}

any_t hashmap_next_iteration(map_t in) {

  hashmap_element* e;
  int i,j;

  /* Cast the hashmap */
  hashmap_map* m = (hashmap_map*) in;

  /* Linear probing */
  for (i = m->iterator; i< m->table_size; i++) {
    e = &m->data[i];

    if (e->in_use == 0)
      continue;

    for (j = 0; j < m->iterator_lower; j++)
      e = e->next;
    if (e == NULL) {
      m->iterator_lower = 0;
      continue;
    }
    m->iterator = i;
    m->iterator_lower++;
    return e->data;
  }

  return NULL;
}

/*
 * Remove an element with that key from the map
 */
int hashmap_remove(map_t in, char* key) {

  int curr;
  hashmap_map* m;
  hashmap_element *e, *p;

  /* Cast the hashmap */
  m = (hashmap_map *) in;

  /* Find key */
  curr = hashmap_hash_int(m, key);

  e = &m->data[curr];
  if (e->in_use == 0)
    return MAP_MISSING;

  /* Linear probing, if necessary */
  p = NULL;
  while (e != NULL) {
    if (strcmp(e->key,key)==0) {
      /* Blank out the fields */
      e->in_use = 0;
      e->data = NULL;
      e->key = NULL;

      if (p != NULL) {
        p->next = e->next;
        free(e);
      }
      else {
        /* Located in the big array, not floating memory */
        if (e->next != NULL) {
          hashmap_element *tmp = e->next;
          memcpy(e, e->next, sizeof(hashmap_element));
          free(tmp);
        }
      }

      /* Reduce the size */
      m->size--;
      return MAP_OK;
    }
    p = e;
    e = e->next;
  }

  /* Data not found */
  return MAP_MISSING;
}

/* Deallocate the hashmap */
void hashmap_free(map_t in) {

  hashmap_map* m = (hashmap_map*) in;
  hashmap_element *e, *p;
  int i;

  for (i = 0; i < m->table_size; i++) {
    int first = 1;

    e = &m->data[i];
    if (e->in_use == 0)
      continue;
    while (e != NULL) {
      p = e;
      e = e->next;
      if (first == 0)
        free(p);
      first = 0;
    }
  }

  free(m->data);
  free(m);
}

/* free all elements */
void hashmap_free_all_elements(map_t in) {

  any_t *e;

  e = hashmap_begin_iteration(in);
  while (e != NULL) {
    free(e);
    e = hashmap_next_iteration(in);
  }
}

/* Return the length of the hashmap */
int hashmap_length(map_t in) {

  hashmap_map* m = (hashmap_map *) in;
  if (m != NULL)
    return m->size;
  else
    return 0;
}

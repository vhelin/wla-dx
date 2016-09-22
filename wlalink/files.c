
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "parse.h"
#include "files.h"
#include "analyze.h"



extern struct object_file *obj_first, *obj_last, *obj_tmp;
extern struct label *labels_first, *labels_last;
extern unsigned char *file_header, *file_footer;
extern int file_header_size, file_footer_size, use_libdir, total_flags;
char file_name_error[] = "???";
extern char ext_libdir[MAX_NAME_LENGTH];



int load_objects(char *argv[], int argc) {

  int i;


  for (i = total_flags + 1; i < argc - 1 ; i++) {
    if (load_file(argv[i]) == FAILED) return FAILED;
  }

  return SUCCEEDED;
}


int load_library(char *argv, int contains_flag) {

  char tmp_name[MAX_NAME_LENGTH];
  FILE *fop;


  /* skip the flag? */
  if (contains_flag == YES)
    argv += 2;

  if (strlen(argv) < 2)
    return FAILED;

  /* try the specified path */
  sprintf(tmp_name, "%s", argv);
  fop = fopen(argv, "rb");

  if (fop == NULL) {
	  /* append .lib extension if not found */
     sprintf(tmp_name, "%s.lib", argv);
     fop = fopen(tmp_name, "rb");

     if (fop == NULL && use_libdir == YES) {
	   /* try the lib dir if specified and not found */
       sprintf(tmp_name, "%s%s", ext_libdir, argv);
       fop = fopen(tmp_name, "rb");

       if (fop == NULL)
		 /* append .lib extension if not found */
         sprintf(tmp_name, "%s%s.lib", ext_libdir, argv);
	 }
  }

  fclose(fop);

  if (load_file(tmp_name) == FAILED) return FAILED;

  return SUCCEEDED;
}


int load_file(char *file_name) {

  struct object_file *o;
  unsigned char *data;
  static int id = 0;
  char *name;
  int size;

  
  o = malloc(sizeof(struct object_file));
  name = malloc(strlen(file_name)+1);
  if (o == NULL || name == NULL) {
    if (o != NULL)
      free(o);
    if (name != NULL)
      free(name);
    fprintf(stderr, "LOAD_FILE: Out of memory.\n");
    return FAILED;
  }

  if (load_file_data(file_name, &data, &size) == FAILED) {
    free(name);
    free(o);
    return FAILED;
  }

  /* only valid for library files; bank 0 slot 0 base 0 assumed unless library is SUPERFREE */
  o->bank = 0;
  o->slot = 0;
  o->base = 0;
  o->base_defined = OFF;

  /* init the rest of the variables */
  o->source_file_names = NULL;
  o->memorymap = NULL;
  o->exported_defines = NULL;
  o->data_blocks = NULL;
  o->source_file_names_list = NULL;

  if (obj_first == NULL) {
    obj_first = o;
    obj_last = o;
  }
  else {
    obj_last->next = o;
    obj_last = o;
  }

  o->next = NULL;
  o->size = size;
  o->data = data;

  strcpy(name, file_name);
  o->name = name;
  o->id = id++;

  return SUCCEEDED;
}


int load_file_data(char *file_name, unsigned char **data, int *size) {

  FILE *fop;


  fop = fopen(file_name, "rb");
  if (fop == NULL) {
    fprintf(stderr, "LOAD_FILE_DATA: Could not open file \"%s\".\n", file_name);
    return FAILED;
  }

  fseek(fop, 0, SEEK_END);
  *size = ftell(fop);
  fseek(fop, 0, SEEK_SET);

  *data = malloc(*size);
  if (*data == NULL)
    return FAILED;

  fread(*data, 1, *size, fop);
  fclose(fop);

  return SUCCEEDED;
}


char *get_file_name(int id) {

  static char error[] = "GET_FILE_NAME: Internal data corruption.";
  struct object_file *o;

  
  o = obj_first;
  while (o != NULL) {
    if (o->id == id)
      return o->name;
    o = o->next;
  }

  return error;
}


char *get_source_file_name(int file_id, int source_id) {

  struct source_file_name *s;
  struct object_file *o;

  
  o = obj_first;
  while (o != NULL) {
    if (o->id == file_id)
      break;
    o = o->next;
  }

  if (o == NULL)
    return file_name_error;

  s = o->source_file_names_list;
  while (s != NULL) {
    if (s->id == source_id)
      break;
    s = s->next;
  }

  if (s == NULL)
    return file_name_error;

  return s->name;
}

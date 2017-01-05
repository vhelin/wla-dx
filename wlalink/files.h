
#ifndef _FILES_H
#define _FILES_H

int load_objects(char *argv[], int argc);
int load_library(char *argv, int contains_flag);
int load_file(char *fn, char *lib_name);
int load_file_data(char *fn, unsigned char **da, int *size, char *lib_name);
char *get_file_name(int id);
char *get_source_file_name(int file_id, int source_id);

#endif

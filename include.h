
#ifndef _INCLUDE_H
#define _INCLUDE_H

int include_file(char *name, int *include_size, char *namespace, int is_full_path);
int incbin_file(char *name, int *id, int *swap, int *skip, int *read, struct macro_static **macro, int *filter_size);
int preprocess_file(char *c, char *d, char *o, int *s, char *f);
int create_full_name(char *dir, char *name);
int print_file_names(char *target_file_name);
int find_file(char *name, FILE **f, int is_full_path);
char *get_file_name(int id);

#endif

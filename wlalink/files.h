
#ifndef _FILES_H
#define _FILES_H

int load_files(char *argv[], int argc);
int load_file(char *fn, int bank, int slot, char *slot_name, int fix_slot, int base, int base_defined);
int load_file_data(char *fn, unsigned char **da, int *size);
char *get_file_name(int id);
char *get_source_file_name(int file_id, int source_id);
struct object_file *get_file(int file_id);
int convert_slot_names_and_addresses(void);

#endif

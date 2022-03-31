
#ifndef __LISTFILE_H
#define __LISTFILE_H

struct listfileitem {
  char *sourcefilename;
  int linenumber;
  int address;
  int length;
  struct section *section;
};

int listfile_write_listfiles(void);
int listfile_block_read(unsigned char **d, struct section *s);
int listfile_block_read_global(unsigned char **d, struct object_file *obj);

#endif

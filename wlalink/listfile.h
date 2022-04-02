
#ifndef __LISTFILE_H
#define __LISTFILE_H

struct listfileitem {
  char *sourcefilename;
  int  linenumber;
  int  real_linenumber;
  int  address;
  int  offset;
  int  length;
  int  base;
  int  bank;
  int  slot;
  int  running_id;
  struct section *section;
};

int listfile_write_listfiles(void);
int listfile_block_read(unsigned char **d, struct section *s);
int listfile_block_read_global(unsigned char **d, struct object_file *obj);

#endif


#ifndef __LISTFILE_H
#define __LISTFILE_H

struct listfileitem {
  char *sourcefilename;
  int linenumber;
  int address;
  int length;
  struct section *section;
};

int listfile_write_listfiles(struct section *sections, struct section *bankheader_sections);
int listfile_block_read(unsigned char **d, struct section *s);

#endif

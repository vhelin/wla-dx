
#ifndef _LISTFILE_H
#define _LISTFILE_H

int listfile_collect(void);
int listfile_block_write(FILE *file_out, struct section_def *section);

#endif

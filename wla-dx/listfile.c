
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include_file.h"
#include "listfile.h"


extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;
extern struct section_def *sections_first, *sections_last, *sec_tmp, *sec_next;
extern struct file_name_info *file_name_info_first, *file_name_info_last, *file_name_info_tmp;
extern unsigned char *rom_banks, *rom_banks_usage_table;
extern FILE *file_out_ptr;
extern char gba_tmp_name[32], *gba_tmp_last_name, tmp[4096];
extern int verbose_mode, section_status, cartridgetype, output_format;


int listfile_collect(void) {

  struct section_def *section = NULL;
  FILE *file_in;
  int add = 0, file_name_id = 0, inz, line_number = 0, command = 0;
  char c;


  if ((file_in = fopen(gba_tmp_name, "rb")) == NULL) {
    fprintf(stderr, "LISTFILE_COLLECT: Error opening file \"%s\".\n", gba_tmp_name);
    return FAILED;
  }

  while (fread(&c, 1, 1, file_in) != 0) {
    switch (c) {

    case ' ':
    case 'E':
    case 'P':
    case 'p':
      continue;

    case 'g':
      fscanf(file_in, "%*s ");
      continue;
    case 'G':
      continue;

    case 'A':
    case 'S':
      if (c == 'A')
	fscanf(file_in, "%d %*d", &inz);
      else
	fscanf(file_in, "%d ", &inz);

      section = sections_first;
      while (section->id != inz)
	section = section->next;

      add = 0;
      command = 0;

      /* allocate the listfile data */
      section->listfile_ints = malloc(sizeof(int) * section->listfile_items*2);
      section->listfile_cmds = malloc(section->listfile_items);
      if (section->listfile_ints == NULL || section->listfile_cmds == NULL) {
	fprintf(stderr, "LISTFILE_COLLECT: Out of memory error.\n");
	return FAILED;
      }

      /* add file name */
      section->listfile_cmds[command] = 'f';
      section->listfile_ints[command*2 + 0] = file_name_id;
      command++;

      continue;

    case 's':
      section = NULL;
      continue;

    case 'x':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz;
      continue;

    case 'X':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz << 1;
      continue;

#ifdef W65816
    case 'z':
    case 'q':
      fscanf(file_in, "%*s ");
      add += 3;
      continue;

    case 'T':
      fscanf(file_in, "%*d ");
      add += 3;
      continue;

    case 'b':
      fscanf(file_in, "%*d ");
      continue;
#endif

    case 'R':
    case 'Q':
    case 'd':
    case 'c':
      fscanf(file_in, "%*s ");
      add++;
      continue;

#ifdef W65816
    case 'M':
#endif
    case 'r':
      fscanf(file_in, "%*s ");
      add += 2;
      continue;

    case 'y':
    case 'C':
      fscanf(file_in, "%*d ");
      add += 2;
      continue;

    case 'D':
      fscanf(file_in, "%*d %*d %*d %d ", &inz);
      add += inz;
      continue;

    case 'O':
      fscanf(file_in, "%*d ");
      continue;

    case 'B':
      fscanf(file_in, "%*d %*d ");
      continue;

    case 'Z':
      continue;

    case 'Y':
    case 'L':
      fscanf(file_in, "%*s ");
      continue;

    case 'f':
      fscanf(file_in, "%d ", &file_name_id);

      if (section != NULL) {
	/* terminate the previous line */
	section->listfile_ints[command*2 + 1] = add;
	add = 0;

	/* add file name */
	section->listfile_cmds[command] = 'f';
	section->listfile_ints[command*2 + 0] = file_name_id;
	command++;
      }
      continue;

    case 'k':
      fscanf(file_in, "%d ", &line_number);

      if (section != NULL) {
	/* terminate the previous line */
	section->listfile_ints[command*2 + 1] = add;
	add = 0;

	/* add line number */
	section->listfile_cmds[command] = 'k';
	section->listfile_ints[command*2 + 0] = line_number;
	command++;
      }
      continue;

    default:
      fprintf(stderr, "%s: LISTFILE_COLLECT: Unknown internal symbol \"%c\".\n", get_file_name(file_name_id), c);
      return FAILED;
    }
  }

  fclose(file_in);

  return SUCCEEDED;
}


#define WRITEOUT_D fprintf(file_out, "%c%c%c%c", (d>>24)&0xFF, (d>>16)&0xFF, (d>>8)&0xFF, d&0xFF);


int listfile_block_write(FILE *file_out, struct section_def *section) {

  int i, d;


  if (file_out == NULL || section == NULL)
    return FAILED;

  fprintf(file_out, "%c", 1);

  d = section->listfile_items;
  WRITEOUT_D;

  for (i = 0; i < section->listfile_items; i++) {
    fprintf(file_out, "%c", section->listfile_cmds[i]);
    if (section->listfile_cmds[i] == 'k') {
      /* next line: line number, line length */
      d = section->listfile_ints[i*2 + 0];
      WRITEOUT_D;
      d = section->listfile_ints[i*2 + 1];
      WRITEOUT_D;
    }
    else if (section->listfile_cmds[i] == 'f') {
      /* next file: file name id */
      d = section->listfile_ints[i*2 + 0];
      WRITEOUT_D;
    }
    else {
      fprintf(stderr, "LISTFILE_BLOCK_WRITE: Unknown command %d.\n", section->listfile_cmds[i]);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

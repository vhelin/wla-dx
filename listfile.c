
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include_file.h"
#include "listfile.h"


extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern FILE *g_file_out_ptr;
extern char g_tmp_name[MAX_NAME_LENGTH + 1], g_tmp[4096];
extern int g_verbose_mode, g_section_status, g_cartridgetype, g_output_format;


int listfile_collect(void) {

  int add = 0, skip = 0, file_name_id = 0, inz, line_number = 0, command = 0, inside_macro = 0, inside_repeat = 0;
  int x, y;
  int dstruct_start = -1;
  struct section_def *section = NULL;
  FILE *file_in;
  char c;


  if ((file_in = fopen(g_tmp_name, "rb")) == NULL) {
    fprintf(stderr, "LISTFILE_COLLECT: Error opening file \"%s\".\n", g_tmp_name);
    return FAILED;
  }

  while (fread(&c, 1, 1, file_in) != 0) {
    switch (c) {

    case 'j':
      inside_repeat++;
      continue;
    case 'J':
      inside_repeat--;
      continue;

    case 'i':
      fscanf(file_in, "%*s ");
      inside_macro++;
      continue;
    case 'I':
      fscanf(file_in, "%*s ");
      inside_macro--;
      continue;

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

      section = g_sections_first;
      while (section->id != inz)
        section = section->next;

      add = 0;
      skip = 0;
      command = 0;

      /* allocate the listfile data */
      section->listfile_ints = calloc(sizeof(int) * section->listfile_items * 3, 1);
      section->listfile_cmds = calloc(section->listfile_items, 1);
      if (section->listfile_ints == NULL || section->listfile_cmds == NULL) {
        fprintf(stderr, "LISTFILE_COLLECT: Out of memory error.\n");
        return FAILED;
      }

      /* add file name */
      section->listfile_cmds[command] = 'f';
      section->listfile_ints[command*3 + 0] = file_name_id;
      command++;

      continue;

    case 's':
      section = NULL;
      continue;

    case 'x':
    case 'o':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz;
      continue;

    case 'X':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz * 2;
      continue;

    case 'h':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz * 3;
      continue;

    case 'w':
      fscanf(file_in, "%d %*d ", &inz);
      add += inz * 4;
      continue;

    case 'z':
    case 'q':
      fscanf(file_in, "%*s ");
      add += 3;
      continue;

    case 'T':
      fscanf(file_in, "%*d ");
      add += 3;
      continue;

    case 'u':
    case 'V':
      fscanf(file_in, "%*s ");
      add += 4;
      continue;

    case 'U':
      fscanf(file_in, "%*d ");
      add += 4;
      continue;

    case 'b':
      fscanf(file_in, "%*d ");
      continue;

    case 'v':
      fscanf(file_in, "%*d ");
      continue;

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

#ifdef SPC700
    case 'n':
      fscanf(file_in, "%*d %*s ");
      add += 2;
      continue;

    case 'N':
      fscanf(file_in, "%*d %*d ");
      add += 2;
      continue;
#endif

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

    case 't':
      fscanf(file_in, "%*s ");
      continue;
      
    case 'Y':
    case 'L':
      fscanf(file_in, "%*s ");
      continue;

    case 'f':
      fscanf(file_in, "%d ", &file_name_id);

      if (section != NULL) {
        /* terminate the previous line */
        section->listfile_ints[command*3 + 1] = add;
        add = 0;
        skip = 0;

        /* add file name */
        section->listfile_cmds[command] = 'f';
        section->listfile_ints[command*3 + 0] = file_name_id;
        command++;
      }
      continue;

    case 'k':
      fscanf(file_in, "%d ", &line_number);

      if (inside_macro > 0 || inside_repeat > 0) {
        skip += add;
        add = 0;
        continue;
      }

      if (section != NULL) {
        /* terminate the previous line */
        section->listfile_ints[command*3 + 1] = add;
        section->listfile_ints[command*3 + 2] = skip;

        add = 0;
        skip = 0;

        /* add line number */
        section->listfile_cmds[command] = 'k';
        section->listfile_ints[command*3 + 0] = line_number;
        command++;
      }
      continue;

    case 'e':
      fscanf(file_in, "%d %d ", &x, &y);
      if (y == -1) /* Mark start of .DSTRUCT */
        dstruct_start = add;
      else
        add = dstruct_start + x;
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
      /* next line: line number, line length, skip bytes */
      d = section->listfile_ints[i*3 + 0];
      WRITEOUT_D;
      d = section->listfile_ints[i*3 + 1];
      WRITEOUT_D;
      d = section->listfile_ints[i*3 + 2];
      WRITEOUT_D;
    }
    else if (section->listfile_cmds[i] == 'f') {
      /* next file: file name id */
      d = section->listfile_ints[i*3 + 0];
      WRITEOUT_D;
    }
    else {
      fprintf(stderr, "LISTFILE_BLOCK_WRITE: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", section->listfile_cmds[i]);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

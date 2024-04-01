
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include.h"
#include "listfile.h"
#include "phase_3.h"
#include "main.h"


extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern FILE *g_file_out_ptr;
extern char *g_tmp, *g_global_listfile_cmds;
extern int g_section_status, g_cartridgetype, g_output_format, *g_global_listfile_ints, g_global_listfile_items;
extern int *g_bankaddress;


static int _print_fscanf_error_accessing_internal_data_stream(int file_name_id, int line_number) {

  print_text(NO, "%s:%d: listfile_collect(): Could not read enough elements from the internal data stream. Please submit a bug report!\n",
          get_file_name(file_name_id), line_number);

  return FAILED;
}


int listfile_collect(void) {

  int add = 0, skip = 0, file_name_id = 0, inz, line_number = 0, command = 0, global_command = 0, inside_macro = 0, inside_repeat = 0;
  int x, y, dstruct_start = -1, bits_current = 0, base = 0, bank = 0, origin = 0, origin_old = 0, slot = 0, running_id = 0;
  int real_line_number = 0, err;
  struct section_def *section = NULL;
  char c;

  /* rewind to the beginning of the internal data stream */
  fseek(g_file_out_ptr, 0, SEEK_SET);
  
  /* allocate the global listfile data */
  g_global_listfile_ints = calloc(sizeof(int) * g_global_listfile_items * 8, 1);
  g_global_listfile_cmds = calloc(g_global_listfile_items, 1);
  if (g_global_listfile_ints == NULL || g_global_listfile_cmds == NULL) {
    print_text(NO, "LISTFILE_COLLECT: Out of memory error.\n");
    return FAILED;
  }

  while (fread(&c, 1, 1, g_file_out_ptr) != 0) {
    switch (c) {

    case 'j':
      inside_repeat++;
      /* HACK! */
      line_number += 2;
      continue;
    case 'J':
      inside_repeat--;
      continue;

    case 'i':
      err = fscanf(g_file_out_ptr, "%*d %*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      inside_macro++;
      /* HACK! */
      line_number--;
      continue;
    case 'I':
      err = fscanf(g_file_out_ptr, "%*d %*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      inside_macro--;
      continue;

    case ' ':
    case 'E':
    case 'P':
    case 'p':
      continue;

    case 'g':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;
    case 'G':
      continue;

    case 'A':
    case 'S':
      if (c == 'A') {
        err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      }
      else {
        err = fscanf(g_file_out_ptr, "%d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      }

      origin_old = origin;
      
      section = g_sections_first;
      while (section->id != inz)
        section = section->next;

      add = 0;
      skip = 0;
      command = 0;

      /* allocate the listfile data */
      section->listfile_ints = calloc(sizeof(int) * section->listfile_items * 5, 1);
      section->listfile_cmds = calloc(section->listfile_items, 1);
      if (section->listfile_ints == NULL || section->listfile_cmds == NULL) {
        print_text(NO, "LISTFILE_COLLECT: Out of memory error.\n");
        return FAILED;
      }

      /* add file name */
      section->listfile_cmds[command] = 'f';
      section->listfile_ints[command*5 + 0] = file_name_id;
      command++;

      continue;

    case 's':
      if (section != NULL && section->advance_org == NO)
        origin = origin_old;
      section = NULL;
      add = 0;
      skip = 0;
      continue;

    case 'x':
    case 'o':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      add += inz;
      origin += inz;
      continue;

    case 'X':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += inz * 2;
      origin += inz * 2;
      continue;

    case 'h':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += inz * 3;
      origin += inz * 3;
      continue;

    case 'w':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += inz * 4;
      origin += inz * 4;
      continue;

    case 'z':
    case 'q':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 3;
      origin += 3;
      continue;

    case 'T':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 3;
      origin += 3;
      continue;

    case 'u':
    case 'V':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      add += 4;
      origin += 4;
      continue;

    case 'U':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 4;
      origin += 4;
      continue;

    case 'b':
      err = fscanf(g_file_out_ptr, "%d ", &base);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'v':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'R':
    case 'Q':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add++;
      origin++;
      continue;

    case 'd':
    case 'c':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add++;
      origin++;
      continue;

#if defined(SUPERFX)
    case '*':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add++;
      origin++;
      continue;

    case '-':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      add++;
      origin++;
      continue;
#endif

    case '.':
      continue;

#if defined(W65816)
    case 'M':
#endif
    case 'r':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 2;
      origin += 2;
      continue;

    case 'y':
    case 'C':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 2;
      origin += 2;
      continue;

    case '+':
      {
        int bits_to_add;
        char type;
          
        err = fscanf(g_file_out_ptr, "%d ", &bits_to_add);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
        if (bits_to_add == 999) {
          bits_current = 0;

          continue;
        }
        else {
          if (bits_current == 0) {
            add++;
            origin++;
          }
          bits_current += bits_to_add;
          while (bits_current > 8) {
            bits_current -= 8;
            add++;
            origin++;
          }
          if (bits_to_add == 8)
            bits_current = 0;
        }

        err = fscanf(g_file_out_ptr, "%c", &type);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
        if (type == 'a') {
          err = fscanf(g_file_out_ptr, "%*d");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }
        else if (type == 'b') {
          err = fscanf(g_file_out_ptr, "%*s");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }
        else if (type == 'c') {
          err = fscanf(g_file_out_ptr, "%*d");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }
        else {
          print_text(NO, "%s: LISTFILE_COLLECT: Unknown internal .BITS data type \"%c\". Please submit a bug report!\n", get_file_name(file_name_id), type);
          return FAILED;
        }

        continue;
      }
      
#if defined(SPC700)
    case 'n':
      err = fscanf(g_file_out_ptr, "%*d %*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 2;
      origin += 2;
      continue;

    case 'N':
      err = fscanf(g_file_out_ptr, "%*d %*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += 2;
      origin += 2;
      continue;
#endif

    case 'D':
      err = fscanf(g_file_out_ptr, "%*d %*d %*d %d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      add += inz;
      origin += inz;
      continue;

    case 'O':
      err = fscanf(g_file_out_ptr, "%d ", &origin);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'B':
      err = fscanf(g_file_out_ptr, "%d %d ", &bank, &slot);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'Z':
      continue;

    case 't':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;
      
    case 'Y':
    case 'L':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'f':
      err = fscanf(g_file_out_ptr, "%d ", &file_name_id);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      if (section != NULL) {
        /* terminate the previous line */
        section->listfile_ints[command*5 + 1] = add;
        add = 0;
        skip = 0;

        /* add file name */
        section->listfile_cmds[command] = 'f';
        section->listfile_ints[command*5 + 0] = file_name_id;
        command++;
        running_id++;
      }
      else {
        /* terminate the previous line */
        g_global_listfile_ints[global_command*8 + 1] = add;
        add = 0;
        skip = 0;

        /* add file name */
        g_global_listfile_cmds[global_command] = 'f';
        g_global_listfile_ints[global_command*8 + 0] = file_name_id;
        global_command++;
        running_id++;
      }
      continue;

    case 'k':
      /* all listfile data produced by .MACROs are put where they are called from */
      if (inside_macro > 0 || inside_repeat > 0) {
        err = fscanf(g_file_out_ptr, "%d ", &real_line_number);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      }
      else {
        err = fscanf(g_file_out_ptr, "%d ", &line_number);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        real_line_number = line_number;
      }
      
      if (section != NULL) {
        /* terminate the previous line */
        section->listfile_ints[command*5 + 1] = add;
        section->listfile_ints[command*5 + 2] = skip;
        section->listfile_ints[command*5 + 3] = running_id;

        add = 0;
        skip = 0;

        /* add line number - NOTE: this 'k' terminates the list file item on the previous line, thus -1 */
        section->listfile_cmds[command] = 'k';
        section->listfile_ints[command*5 + 0] = line_number-1;
        section->listfile_ints[command*5 + 4] = real_line_number-1;
        command++;
        running_id++;
      }
      else {
        /* terminate the previous line */
        g_global_listfile_ints[global_command*8 + 1] = add;
        g_global_listfile_ints[global_command*8 + 2] = origin-add;
        g_global_listfile_ints[global_command*8 + 3] = base;
        g_global_listfile_ints[global_command*8 + 4] = bank;
        g_global_listfile_ints[global_command*8 + 5] = slot;
        g_global_listfile_ints[global_command*8 + 6] = running_id;
        
        add = 0;
        skip = 0;

        /* add line number - NOTE: this 'k' terminates the list file item on the previous line, thus -1 */
        g_global_listfile_cmds[global_command] = 'k';
        g_global_listfile_ints[global_command*8 + 0] = line_number-1;
        g_global_listfile_ints[global_command*8 + 7] = real_line_number-1;
        global_command++;
        running_id++;
      }
      continue;

    case 'e':
      err = fscanf(g_file_out_ptr, "%d %d ", &x, &y);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      if (y == -1) /* Mark start of .DSTRUCT */
        dstruct_start = add;
      else
        add = dstruct_start + x;
      continue;

    default:
      print_text(NO, "%s: LISTFILE_COLLECT: Unknown internal symbol \"%c\".\n", get_file_name(file_name_id), c);
      return FAILED;
    }
  }

  /* sanity check */
  if (g_global_listfile_items != global_command) {
    print_text(NO, "LISTFILE_COLLECT: Global final listfile item count %d doesn't match the anticipated count %d.\n", global_command, g_global_listfile_items);
    if (global_command < g_global_listfile_items) {
      print_text(NO, "LISTFILE_COLLECT: In this case it's not a problem, the buffer was large enough. But please submit a bug report!\n");
      g_global_listfile_items = global_command;
    }
    else {
      print_text(NO, "LISTFILE_COLLECT: We ran out of buffer, cannot continue. Please submit a bug report!\n");
      return FAILED;
    }
  }
  
  return SUCCEEDED;
}


#define WRITEOUT_D fprintf(file_out, "%c%c%c%c", (d>>24)&0xFF, (d>>16)&0xFF, (d>>8)&0xFF, d&0xFF);


int listfile_block_write(FILE *file_out, struct section_def *section) {

  int i, d;

  if (file_out == NULL || section == NULL)
    return FAILED;

  /* RAM sections don't write out listfile data */
  if (section->status == SECTION_STATUS_RAM_FREE ||
      section->status == SECTION_STATUS_RAM_FORCE ||
      section->status == SECTION_STATUS_RAM_SEMIFREE ||
      section->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    fprintf(file_out, "%c", 0);
    return SUCCEEDED;
  }
  
  fprintf(file_out, "%c", 1);

  d = section->listfile_items;
  WRITEOUT_D;

  for (i = 0; i < section->listfile_items; i++) {
    fprintf(file_out, "%c", section->listfile_cmds[i]);
    if (section->listfile_cmds[i] == 'k') {
      /* next line: line number, line length, skip bytes */
      d = section->listfile_ints[i*5 + 0];
      WRITEOUT_D;
      d = section->listfile_ints[i*5 + 1];
      WRITEOUT_D;
      d = section->listfile_ints[i*5 + 2];
      WRITEOUT_D;
      d = section->listfile_ints[i*5 + 3];
      WRITEOUT_D;
      d = section->listfile_ints[i*5 + 4];
      WRITEOUT_D;
    }
    else if (section->listfile_cmds[i] == 'f') {
      /* next file: file name id */
      d = section->listfile_ints[i*5 + 0];
      WRITEOUT_D;
    }
    else {
      print_text(NO, "LISTFILE_BLOCK_WRITE: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", section->listfile_cmds[i]);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int listfile_globals_write(FILE *file_out) {

  int i, d;

  if (file_out == NULL)
    return FAILED;

  d = g_global_listfile_items;
  WRITEOUT_D;

  for (i = 0; i < g_global_listfile_items; i++) {
    fprintf(file_out, "%c", g_global_listfile_cmds[i]);
    if (g_global_listfile_cmds[i] == 'k') {
      /* next line: line number, line length, offset, base, bank, slot */
      d = g_global_listfile_ints[i*8 + 0];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 1];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 2];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 3];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 4];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 5];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 6];
      WRITEOUT_D;
      d = g_global_listfile_ints[i*8 + 7];
      WRITEOUT_D;
    }
    else if (g_global_listfile_cmds[i] == 'f') {
      /* next file: file name id */
      d = g_global_listfile_ints[i*8 + 0];
      WRITEOUT_D;
    }
    else {
      print_text(NO, "LISTFILE_GLOBALS_WRITE: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", g_global_listfile_cmds[i]);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

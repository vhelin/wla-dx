
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "defines.h"
#include "listfile.h"
#include "files.h"
#include "main.h"


extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct section *g_sec_first, *g_sec_last, *g_sec_bankhd_first, *g_sec_bankhd_last;
extern struct slot g_slots[256];
extern unsigned char *g_rom;
extern int *g_bankaddress;
extern char g_version_string[];


/* read an integer from t */
#define READ_T t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24); t += 4;


static int _listfile_write_hex(FILE *f, int data) {

  if (data >= 10)
    fprintf(f, "%c", data + 'A' - 10);
  else
    fprintf(f, "%c", data + '0');

  return SUCCEEDED;
}


static int _listfileitem_sort(const void *a, const void *b) {

  struct listfileitem *sa, *sb;

  sa = *((struct listfileitem **)a);
  sb = *((struct listfileitem **)b);

  if (sa->sourcefilename > sb->sourcefilename)
    return 1;
  else if (sa->sourcefilename < sb->sourcefilename)
    return -1;

  if (sa->linenumber > sb->linenumber)
    return 1;
  else if (sa->linenumber < sb->linenumber)
    return -1;

  if (sa->running_id > sb->running_id)
    return 1;
  else if (sa->running_id < sb->running_id)
    return -1;
    
  return 0;
}


static int _get_pc(int slot, int offset) {

  return g_slots[slot].address + offset;
}


int listfile_write_listfiles(void) {

  struct listfileitem *listfileitems, **listfileitems_ptr;
  struct section *s, **selected_sections;
  struct object_file *obj;
  int count, i, j, current_linenumber, m, o, p, source_file_id = -1, wrote_line, listfile_item_count = 0, section_count = 0, chars = 60;
  char command, tmp[1024], *source_file, *source_file_name, cpu_65816 = NO;

  /* count the listfile items */
  s = g_sec_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else {
      listfile_item_count += s->listfile_items;
      section_count++;
    }

    s = s->next;
  }
  s = g_sec_bankhd_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else {
      listfile_item_count += s->listfile_items;
      section_count++;
    }

    s = s->next;
  }
  obj = g_obj_first;
  while (obj != NULL) {
    listfile_item_count += obj->listfile_items;
    obj = obj->next;
  }
  
  if (listfile_item_count <= 0) {
    /* no listfile data, so we cannot write listfiles... */
    return SUCCEEDED;
  }

  listfileitems = calloc(sizeof(struct listfileitem) * listfile_item_count, 1);
  if (listfileitems == NULL) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  listfileitems_ptr = calloc(sizeof(struct listfileitem *) * listfile_item_count, 1);
  if (listfileitems_ptr == NULL) {
    free(listfileitems);
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  selected_sections = calloc(sizeof(struct section *) * section_count, 1);
  if (listfileitems == NULL) {
    free(listfileitems);
    free(listfileitems_ptr);
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  for (i = 0; i < listfile_item_count; i++)
    listfileitems_ptr[i] = &listfileitems[i];
  
  /* collect the valid sections */
  j = 0;
  s = g_sec_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else
      selected_sections[j++] = s;

    s = s->next;
  }
  s = g_sec_bankhd_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else
      selected_sections[j++] = s;

    s = s->next;
  }  

  /* collect the list file items from the data */
  count = 0;
  for (i = 0; i < section_count; i++) {
    int add = 0;
    
    s = selected_sections[i];
    
    /* parse the list file information */
    for (j = 0; j < s->listfile_items; j++) {
      command = s->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (s->listfile_ints[j*5 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(s->file_id, source_file_id);
          listfileitems[count].linenumber = s->listfile_ints[j*5 + 0];
          listfileitems[count].length = s->listfile_ints[j*5 + 1];
          listfileitems[count].section = s;
          listfileitems[count].base = s->base;
          listfileitems[count].bank = s->bank;
          listfileitems[count].slot = s->slot;
          add += s->listfile_ints[j*5 + 2];
          listfileitems[count].offset = add + s->address;
          listfileitems[count].address = s->output_address + add;
          add += s->listfile_ints[j*5 + 1];
          listfileitems[count].running_id = s->listfile_ints[j*5 + 3];
          listfileitems[count].real_linenumber = s->listfile_ints[j*5 + 4];

          /* fix base, bank, slot, and offset for sections that were appended */
          if (s->appended_to == YES) {
            /* this loop finds the target of possibly chained appendto secions */
            struct section *s3 = s->appended_to_section;
            int offset_new = listfileitems[count].offset - s->address + s3->address + s->appended_to_offset;
            while (s3->appended_to == YES) {
              struct section *s4 = s3->appended_to_section;
              offset_new = offset_new - s3->address + s4->address + s3->appended_to_offset;
              s3 = s4;
            }

            listfileitems[count].offset = offset_new;
            listfileitems[count].base = s3->base;
            listfileitems[count].bank = s3->bank;
            listfileitems[count].slot = s3->slot;
          }

          count++;
          /*
          print_text(NO, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          print_text(NO, "LFI: k SKIPPED\n");
          */

          if (add == -0x12345678) {
            print_text(NO, "LISTFILE_WRITE_LISTFILES: add == -0x12345678! Internal error! Please submit a bug report!\n");
            free(listfileitems);
            free(selected_sections);
            return FAILED;
          }

          add += s->listfile_ints[j*5 + 2];
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = s->listfile_ints[j*5 + 0];
        /*
        print_text(NO, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        print_text(NO, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
        free(listfileitems);
        free(selected_sections);
        return FAILED;
      }
    }
  }
  obj = g_obj_first;
  while (obj != NULL) {
    if (obj->cpu_65816 == YES)
      cpu_65816 = YES;
    for (j = 0; j < obj->listfile_items; j++) {
      command = obj->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (obj->listfile_ints[j*8 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(obj->id, source_file_id);
          listfileitems[count].linenumber = obj->listfile_ints[j*8 + 0];
          listfileitems[count].length = obj->listfile_ints[j*8 + 1];
          listfileitems[count].section = NULL;
          listfileitems[count].base = obj->listfile_ints[j*8 + 3];
          listfileitems[count].bank = obj->listfile_ints[j*8 + 4];
          listfileitems[count].slot = obj->listfile_ints[j*8 + 5];
          listfileitems[count].offset = obj->listfile_ints[j*8 + 2];
          listfileitems[count].address = g_bankaddress[listfileitems[count].bank] + listfileitems[count].offset;
          listfileitems[count].running_id = obj->listfile_ints[j*8 + 6];
          listfileitems[count].real_linenumber = obj->listfile_ints[j*8 + 7];
          count++;
          /*
          print_text(NO, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          print_text(NO, "LFI: k SKIPPED\n");
          */
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = obj->listfile_ints[j*8 + 0];
        /*
        print_text(NO, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        print_text(NO, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
        free(listfileitems);
        free(selected_sections);
        return FAILED;
      }
    }
    obj = obj->next;
  }
  
  free(selected_sections);

  /* sort the listfileitems (lexical order) */
  qsort(listfileitems_ptr, count, sizeof(struct listfileitem *), _listfileitem_sort);

  /* 65816 contains "base" information as well */
  if (cpu_65816 == YES)
    chars += 5;
  
  /* write the listfiles */
  j = 0;
  while (j < count) {
    int file_size;
    FILE *f;

    source_file_name = listfileitems_ptr[j]->sourcefilename;
    f = fopen(source_file_name, "rb");
    fseek(f, 0, SEEK_END);
    file_size = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    source_file = calloc(file_size, 1);
    if (source_file == NULL) {
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
      fclose(f);
      free(listfileitems);
      free(listfileitems_ptr);
      return FAILED;
    }

    if (fread(source_file, 1, file_size, f) != (size_t)file_size) {
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not read all %d bytes of \"%s\"!", file_size, source_file_name);
      fclose(f);
      free(listfileitems);
      free(listfileitems_ptr);
      free(source_file);
      return FAILED;
    }
  
    fclose(f);

    strcpy(tmp, source_file_name);
    for (i = (int)(strlen(tmp)-1); i >= 0; i--) {
      if (tmp[i] == '.')
        break;
    }
    strcpy(&tmp[i], ".lst");

    f = fopen(tmp, "wb");
    if (f == NULL) {
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not open file \"%s\" for writing.\n", tmp);
      free(listfileitems);
      free(listfileitems_ptr);
      free(source_file);
      return FAILED;
    }

    fprintf(f, "; WLALINK (%s) listing file\n", g_version_string);
    {
      time_t timer;
      char time_buffer[32];
      struct tm* tm_info;

      timer = time(NULL);
      tm_info = localtime(&timer);

      strftime(time_buffer, 32, "%Y-%m-%d %H:%M:%S", tm_info);
      fprintf(f, "; %s\n", time_buffer);
    }
    fprintf(f, "\n");
    
    if (cpu_65816 == YES)
      fprintf(f, ";Line Base Bank Slot PC   Offset Hex\n");
    else
      fprintf(f, ";Line Bank Slot PC   Offset Hex                             Source\n");
    
    /* write the lines */
    current_linenumber = 0;
    m = 0;

    while (j < count && strcmp(listfileitems_ptr[j]->sourcefilename, source_file_name) == 0) {
      int is_behind = NO;

      /* goto line x */
      while (current_linenumber < listfileitems_ptr[j]->linenumber-1) {
        for (o = 0; o < chars; o++)
          fprintf(f, " ");
        while (1) {
          if (source_file[m] == 0xA) {
            m++;
            if (source_file[m] == 0xD)
              m++;
            current_linenumber++;
            fprintf(f, "\n");
            break;
          }
          else
            fprintf(f, "%c", source_file[m++]);
        }
      }

      if (current_linenumber > listfileitems_ptr[j]->linenumber-1)
        is_behind = YES;

      p = 0;

      fprintf(f, "%*d ", 5, listfileitems_ptr[j]->real_linenumber);
      p += 6;
      if (cpu_65816 == YES) {
        fprintf(f, "%.4X ", listfileitems_ptr[j]->base);
        p += 5;
      }
      fprintf(f, "%.4X ", listfileitems_ptr[j]->bank);
      p += 5;
      fprintf(f, "%.4X ", listfileitems_ptr[j]->slot);
      p += 5;
      fprintf(f, "%.4X ", _get_pc(listfileitems_ptr[j]->slot, listfileitems_ptr[j]->offset));
      p += 5;
      fprintf(f, "%.4X   ", listfileitems_ptr[j]->offset);
      p += 7;
      
      /* write the bytes */
      wrote_line = NO;
      for (o = 0; o < listfileitems_ptr[j]->length; o++) {
        struct section *s2 = listfileitems_ptr[j]->section;
        
        if (s2 != NULL && s2->is_bankheader_section == YES) {
          _listfile_write_hex(f, listfileitems_ptr[j]->section->data[listfileitems_ptr[j]->address + o] >> 4);
          _listfile_write_hex(f, listfileitems_ptr[j]->section->data[listfileitems_ptr[j]->address + o] & 15);
        }
        else {
          if (s2 != NULL && s2->appended_to == YES) {
            /* this loop finds the target of possibly chained appendto secions */
            struct section *s3 = s2->appended_to_section;
            int address_new = listfileitems_ptr[j]->address - s2->output_address + s3->output_address + s2->appended_to_offset;
            while (s3->appended_to == YES) {
              struct section *s4 = s3->appended_to_section;
              address_new = address_new - s3->output_address + s4->output_address + s3->appended_to_offset;
              s3 = s4;
            }
            
            _listfile_write_hex(f, g_rom[address_new + o] >> 4);
            _listfile_write_hex(f, g_rom[address_new + o] & 15);
          }
          else {
            _listfile_write_hex(f, g_rom[listfileitems_ptr[j]->address + o] >> 4);
            _listfile_write_hex(f, g_rom[listfileitems_ptr[j]->address + o] & 15);
          }
        }
        fprintf(f, " ");
        p += 3;
        if ((o % 10) == 9 && o != 0 && o < listfileitems_ptr[j]->length-1) {
          if (wrote_line == NO) {
            /* write padding */
            wrote_line = YES;
            while (p < chars) {
              fprintf(f, " ");
              p++;
            }

            if (is_behind == YES)
              fprintf(f, "\n");
            else {
              /* write the rest of the line */
              while (m < file_size) {
                if (source_file[m] == 0xA) {
                  m++;
                  if (m < file_size && source_file[m] == 0xD)
                    m++;
                  current_linenumber++;
                  fprintf(f, "\n");
                  break;
                }
                else
                  fprintf(f, "%c", source_file[m++]);
              }
            }
          }
          else
            fprintf(f, "\n");

          p = 0;
        }
      }

      if (is_behind == YES)
        fprintf(f, "\n");
      else {
        /* has the line been written already? */
        if (wrote_line == NO) {
          /* write padding */
          while (p < chars) {
            fprintf(f, " ");
            p++;
          }

          /* write the rest of the line */
          while (m < file_size) {
            if (source_file[m] == 0xA) {
              m++;
              if (m < file_size && source_file[m] == 0xD)
                m++;
              current_linenumber++;
              fprintf(f, "\n");
              break;
            }
            else
              fprintf(f, "%c", source_file[m++]);
          }
        }
        else
          fprintf(f, "\n");
      }
      
      j++;
    }

    /* write the rest of the file */
    while (m < file_size) {
      for (o = 0; o < chars; o++)
        fprintf(f, " ");
      while (m < file_size) {
        if (source_file[m] == 0xA) {
          m++;
          if (m < file_size && source_file[m] == 0xD)
            m++;
          fprintf(f, "\n");
          break;
        }
        else
          fprintf(f, "%c", source_file[m++]);
      }
    }

    fclose(f);

    if (source_file != NULL)
      free(source_file);
  }

  if (listfileitems != NULL)
    free(listfileitems);
  if (listfileitems_ptr != NULL)
    free(listfileitems_ptr);

  return SUCCEEDED;
}


int listfile_block_read(unsigned char **d, struct section *s) {

  unsigned char *t;
  int i;

  if (d == NULL || s == NULL)
    return FAILED;

  if (**d == 0) {
    /* no listfile information! */
    s->listfile_items = 0;
    s->listfile_cmds = NULL;
    s->listfile_ints = NULL;
    (*d)++;
    return SUCCEEDED;
  }

  /* we have listfile information */
  t = *d;
  t++;
  s->listfile_items = READ_T;
  s->listfile_cmds = calloc(s->listfile_items, 1);
  s->listfile_ints = calloc(sizeof(int) * s->listfile_items*5, 1);

  if (s->listfile_cmds == NULL || s->listfile_ints == NULL) {
    s->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < s->listfile_items; i++) {
    s->listfile_cmds[i] = *(t++);
    if (s->listfile_cmds[i] == 'k') {
      /* new line */
      s->listfile_ints[i*5 + 0] = READ_T;
      s->listfile_ints[i*5 + 1] = READ_T;
      s->listfile_ints[i*5 + 2] = READ_T;
      s->listfile_ints[i*5 + 3] = READ_T;
      s->listfile_ints[i*5 + 4] = READ_T;
    }
    else if (s->listfile_cmds[i] == 'f') {
      /* file name */
      s->listfile_ints[i*5 + 0] = READ_T;
    }
    else {
      s->listfile_items = 0;
      print_text(NO, "LISTFILE_BLOCK_READ: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", s->listfile_cmds[i]);
      return FAILED;
    }
  }

  *d = t;

  return SUCCEEDED;
}


int listfile_block_read_global(unsigned char **d, struct object_file *obj) {

  unsigned char *t;
  int i;

  if (d == NULL || obj == NULL)
    return FAILED;

  t = *d;
  obj->listfile_items = READ_T;
  if (obj->listfile_items == 0) {
    /* no listfile information! */
    obj->listfile_cmds = NULL;
    obj->listfile_ints = NULL;
    *d = t;
    return SUCCEEDED;
  }

  /* we have listfile information */
  obj->listfile_cmds = calloc(obj->listfile_items, 1);
  obj->listfile_ints = calloc(sizeof(int) * obj->listfile_items*8, 1);

  if (obj->listfile_cmds == NULL || obj->listfile_ints == NULL) {
    obj->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ_GLOBAL: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < obj->listfile_items; i++) {
    obj->listfile_cmds[i] = *(t++);
    if (obj->listfile_cmds[i] == 'k') {
      /* new line */
      obj->listfile_ints[i*8 + 0] = READ_T;
      obj->listfile_ints[i*8 + 1] = READ_T;
      obj->listfile_ints[i*8 + 2] = READ_T;
      obj->listfile_ints[i*8 + 3] = READ_T;
      obj->listfile_ints[i*8 + 4] = READ_T;
      obj->listfile_ints[i*8 + 5] = READ_T;
      obj->listfile_ints[i*8 + 6] = READ_T;
      obj->listfile_ints[i*8 + 7] = READ_T;
    }
    else if (obj->listfile_cmds[i] == 'f') {
      /* file name */
      obj->listfile_ints[i*8 + 0] = READ_T;
    }
    else {
      obj->listfile_items = 0;
      print_text(NO, "LISTFILE_BLOCK_READ_GLOBAL: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", obj->listfile_cmds[i]);
      return FAILED;
    }
  }

  *d = t;

  return SUCCEEDED;
}

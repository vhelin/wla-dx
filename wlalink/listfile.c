
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "listfile.h"
#include "files.h"


extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct section *g_sec_first, *g_sec_last, *g_sec_bankhd_first, *g_sec_bankhd_last;
extern unsigned char *g_rom;


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

  return 0;
}


int listfile_write_listfiles(void) {

  struct listfileitem *listfileitems = NULL, **listfileitems_ptr = NULL;
  struct section *s, **selected_sections;
  struct object_file *obj;
  int count, i, j, current_linenumber, m, o, p, source_file_id = -1, add, wrote_line, listfile_item_count = 0, section_count = 0;
  char command, tmp[1024], *source_file, *source_file_name;

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
    fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  listfileitems_ptr = calloc(sizeof(struct listfileitem *) * listfile_item_count, 1);
  if (listfileitems_ptr == NULL) {
    free(listfileitems);
    fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  selected_sections = calloc(sizeof(struct section *) * section_count, 1);
  if (listfileitems == NULL) {
    free(listfileitems);
    free(listfileitems_ptr);
    fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
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
    s = selected_sections[i];
    
    /* parse the list file information */
    add = 0;
    for (j = 0; j < s->listfile_items; j++) {
      command = s->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (s->listfile_ints[j*3 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(s->file_id, source_file_id);
          listfileitems[count].linenumber = s->listfile_ints[j*3 + 0];
          listfileitems[count].length = s->listfile_ints[j*3 + 1];
          listfileitems[count].section = s;
          add += s->listfile_ints[j*3 + 2];
          listfileitems[count].address = s->output_address + add;
          add += s->listfile_ints[j*3 + 1];
          count++;
          /*
          fprintf(stderr, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          fprintf(stderr, "LFI: k SKIPPED\n");
          */
          add += s->listfile_ints[j*3 + 2];
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = s->listfile_ints[j*3 + 0];
        /*
        fprintf(stderr, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
        free(listfileitems);
        free(selected_sections);
        return FAILED;
      }
    }
  }
  obj = g_obj_first;
  while (obj != NULL) {
    for (j = 0; j < obj->listfile_items; j++) {
      command = obj->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (obj->listfile_ints[j*3 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(obj->id, source_file_id);
          listfileitems[count].linenumber = obj->listfile_ints[j*3 + 0];
          listfileitems[count].length = obj->listfile_ints[j*3 + 1];
          listfileitems[count].section = NULL;
          listfileitems[count].address = obj->listfile_ints[j*3 + 2];
          count++;
          /*
          fprintf(stderr, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          fprintf(stderr, "LFI: k SKIPPED\n");
          */
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = obj->listfile_ints[j*3 + 0];
        /*
        fprintf(stderr, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
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
      fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
      fclose(f);
      free(listfileitems);
      free(listfileitems_ptr);
      return FAILED;
    }

    if (fread(source_file, 1, file_size, f) != (size_t)file_size) {
      fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Could not read all %d bytes of \"%s\"!", file_size, source_file_name);
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
      fprintf(stderr, "LISTFILE_WRITE_LISTFILES: Could not open file \"%s\" for writing.\n", tmp);
      free(listfileitems);
      free(listfileitems_ptr);
      free(source_file);
      return FAILED;
    }

    /* write the lines */
    current_linenumber = 0;
    m = 0;
    while (j < count && strcmp(listfileitems_ptr[j]->sourcefilename, source_file_name) == 0) {
      int is_behind = NO;

      /* goto line x */
      while (current_linenumber < listfileitems_ptr[j]->linenumber-1) {
        for (o = 0; o < 40; o++)
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
      
      /* write the bytes */
      wrote_line = NO;
      for (p = 0, o = 0; o < listfileitems_ptr[j]->length; o++) {
        struct section *s2 = listfileitems_ptr[j]->section;
        
        fprintf(f, "$");
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
        p += 4;
        if ((o % 10) == 9 && o != 0 && o < listfileitems_ptr[j]->length-1) {
          if (wrote_line == NO) {
            /* write padding */
            wrote_line = YES;
            while (p < 40) {
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
          while (p < 40) {
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
      for (o = 0; o < 40; o++)
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
  s->listfile_ints = calloc(sizeof(int) * s->listfile_items*3, 1);

  if (s->listfile_cmds == NULL || s->listfile_ints == NULL) {
    s->listfile_items = 0;
    fprintf(stderr, "LISTFILE_BLOCK_READ: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < s->listfile_items; i++) {
    s->listfile_cmds[i] = *(t++);
    if (s->listfile_cmds[i] == 'k') {
      /* new line */
      s->listfile_ints[i*3 + 0] = READ_T;
      s->listfile_ints[i*3 + 1] = READ_T;
      s->listfile_ints[i*3 + 2] = READ_T;
    }
    else if (s->listfile_cmds[i] == 'f') {
      /* file name */
      s->listfile_ints[i*3 + 0] = READ_T;
    }
    else {
      s->listfile_items = 0;
      fprintf(stderr, "LISTFILE_BLOCK_READ: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", s->listfile_cmds[i]);
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
  obj->listfile_ints = calloc(sizeof(int) * obj->listfile_items*3, 1);

  if (obj->listfile_cmds == NULL || obj->listfile_ints == NULL) {
    obj->listfile_items = 0;
    fprintf(stderr, "LISTFILE_BLOCK_READ_GLOBAL: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < obj->listfile_items; i++) {
    obj->listfile_cmds[i] = *(t++);
    if (obj->listfile_cmds[i] == 'k') {
      /* new line */
      obj->listfile_ints[i*3 + 0] = READ_T;
      obj->listfile_ints[i*3 + 1] = READ_T;
      obj->listfile_ints[i*3 + 2] = READ_T;
    }
    else if (obj->listfile_cmds[i] == 'f') {
      /* file name */
      obj->listfile_ints[i*3 + 0] = READ_T;
    }
    else {
      obj->listfile_items = 0;
      fprintf(stderr, "LISTFILE_BLOCK_READ_GLOBAL: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", obj->listfile_cmds[i]);
      return FAILED;
    }
  }

  *d = t;

  return SUCCEEDED;
}

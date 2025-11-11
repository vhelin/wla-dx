
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "defines.h"

#include "include.h"
#include "phase_3.h"
#include "printf.h"
#include "main.h"


extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern struct block_name *g_block_names;
extern struct after_section *g_after_sections;
extern struct macro_static *g_macros_first;
extern struct map_t *g_defines_map;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern FILE *g_file_out_ptr;
extern char *g_tmp, g_namespace[MAX_NAME_LENGTH + 1];
extern char *g_asm_name;
extern int g_verbose_level, g_section_status, g_output_format, g_keep_empty_sections, g_quiet;
extern int g_global_listfile_items;

struct label_def *g_label_last, *g_label_tmp, *g_labels = NULL;
struct map_t *g_global_unique_label_map = NULL;
struct label_context g_label_context, *g_label_context_first = NULL, *g_label_context_last = NULL;

static int s_dstruct_start, s_dstruct_item_offset, s_dstruct_item_size;
static struct block *s_blocks = NULL;

int g_label_context_running_number = 0;


#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")


int free_label_context_allocations(void) {

  struct label_context *lc = g_label_context_first;

  if (lc == NULL)
    return SUCCEEDED;

  /* do note that the first struct label_context is not allocated using calloc() */
  lc = lc->next;

  while (lc != NULL) {
    struct label_context *next = lc->next;
    free(lc);
    lc = next;
  }
  
  return SUCCEEDED;
}


static int _print_fscanf_error_accessing_internal_data_stream(int file_name_id, int line_number) {

  print_text(NO, "%s:%d: INTERNAL_PHASE_1: Could not read enough elements from the internal data stream. errno = %d. Please submit a bug report!\n",
          get_file_name(file_name_id), line_number, errno);

  return FAILED;
}


static int _add_label(struct label_def *l, struct section_def *s, int line_number, int file_name_id) {

  struct label_def *tmp_label;
  struct definition *tmp_def;
  int err;

  /* does a definition with the same name exist? */
  hashmap_get(g_defines_map, l->label, (void*)&tmp_def);
  if (tmp_def != NULL) {
    print_text(NO, "%s:%d: INTERNAL_PHASE_1: A definition called \"%s\" exists already, cannot add a label with the same name.\n", get_file_name(file_name_id), line_number, l->label);
    return FAILED;
  }

  if (s != NULL) {
    /* always put the label into the section's label_map */
    if (hashmap_get(s->label_map, l->label, (void *)&tmp_label) == MAP_OK) {
      print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the first time.\n", get_file_name(tmp_label->filename_id), tmp_label->linenumber, l->label);
      print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the second time.\n", get_file_name(file_name_id), line_number, l->label);
      return FAILED;
    }
    if ((err = hashmap_put(s->label_map, l->label, l)) != MAP_OK) {
      print_text(NO, "%s:%d: INTERNAL_PHASE_1: Hashmap error %d. Please send a bug report!", get_file_name(file_name_id), line_number, err);
      return FAILED;
    }
  }

  /* don't put local labels into namespaces or the global namespace */
  if (s == NULL || l->label[0] != '_') {
    if (s != NULL && s->nspace != NULL) {
      /* label in a namespace */
      if (hashmap_get(s->nspace->label_map, l->label, (void *)&tmp_label) == MAP_OK) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the first time.\n", get_file_name(tmp_label->filename_id), tmp_label->linenumber, l->label);
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the second time.\n", get_file_name(file_name_id), line_number, l->label);
        return FAILED;
      }
      if ((err = hashmap_put(s->nspace->label_map, l->label, l)) != MAP_OK) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Hashmap error %d. Please send a bug report!", get_file_name(file_name_id), line_number, err);
        return FAILED;
      }
    }
    else {
      /* global label */
      if (hashmap_get(g_global_unique_label_map, l->label, (void *)&tmp_label) == MAP_OK) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the first time.\n", get_file_name(tmp_label->filename_id), tmp_label->linenumber, l->label);
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Label \"%s\" was defined for the second time.\n", get_file_name(file_name_id), line_number, l->label);
        return FAILED;
      }
      if ((err = hashmap_put(g_global_unique_label_map, l->label, l)) != MAP_OK) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Hashmap error %d. Please send a bug report!", get_file_name(file_name_id), line_number, err);
        return FAILED;
      }
    }
  }

  if (g_labels != NULL) {
    g_label_last->next = l;
    g_label_last = l;
  }
  else {
    g_labels = l;
    g_label_last = l;
  }

  return SUCCEEDED;
}

        
int phase_3(void) {

  int bank = 0, slot = 0, address = 0, file_name_id = 0, inz, line_number = 0, o, address_old = 0, base = 0, bits_current = 0, x, y, err;
  char tmp_buffer[MAX_NAME_LENGTH + 1], c;
  struct section_def *s = NULL;
  struct label_def *l;
  struct block_name *bn;
  struct block *b;

  /* initialize label context */
  g_label_context.isolated_macro = NULL;
  g_label_context.next = NULL;
  g_label_context.prev = NULL;
  for (x = 0; x < 10; x++)
    g_label_context.parent_labels[x] = NULL;
  g_label_context_last = &g_label_context;
  g_label_context_first = &g_label_context;
  
  g_namespace[0] = 0;
  
  if (g_verbose_level >= 100)
    print_text(YES, "Internal pass 1...\n");

  /* rewind to the beginning of the internal data stream */
  fseek(g_file_out_ptr, 0, SEEK_SET);

  /* first loop */
  o = 0;
  if (g_output_format == OUTPUT_OBJECT) {
    /* OUTPUT_OBJECT */
    while (o == 0 && fread(&c, 1, 1, g_file_out_ptr) != 0) {
      switch (c) {

      case ' ':
      case 'E':
        continue;

      case 'j':
        continue;
      case 'J':
        continue;

      case 'i':
        err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (process_macro_in(inz, tmp_buffer, file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;
      case 'I':
        err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        if (process_macro_out(inz, tmp_buffer, file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;

      case 'P':
        address_old = address;
        continue;
      case 'p':
        address = address_old;
        continue;

      case 'x':
      case 'o':
        err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (g_section_status == ON) {
          address += inz;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(file_name_id), line_number);
        return FAILED;

      case 'd':
        if (g_section_status == ON) {
          err = fscanf(g_file_out_ptr, "%*d ");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

          address++;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(file_name_id), line_number);
        return FAILED;

      case 'y':
        if (g_section_status == ON) {
          err = fscanf(g_file_out_ptr, "%*d ");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

          address += 2;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(file_name_id), line_number);
        return FAILED;

#if defined(SUPERFX)

      case '*':
        if (g_section_status == ON) {
          err = fscanf(g_file_out_ptr, "%*s ");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
          address++;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(file_name_id), line_number);
        return FAILED;
      
      case '-':
        if (g_section_status == ON) {
          err = fscanf(g_file_out_ptr, "%*d ");
          if (err < 0)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

          address++;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(file_name_id), line_number);
        return FAILED;

#endif

      case '+':
        if (g_section_status == ON) {
          int bits_to_add;
          char type;
          
          err = fscanf(g_file_out_ptr, "%d ", &bits_to_add);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
          
          if (bits_to_add == 0) {
            bits_current = 0;
          }
          else {
            if (bits_current == 0)
              address++;
            bits_current += bits_to_add;
            while (bits_current > 8) {
              bits_current -= 8;
              address++;
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
            print_text(NO, "%s:%d: INTERNAL_PHASE_1: Unknown internal .BITS data type '%c'. Please submit a bug report!\n", get_file_name(file_name_id), line_number, type);
            return FAILED;
          }

          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(file_name_id), line_number);
        return FAILED;
        
      case 'v':
        err = fscanf(g_file_out_ptr, "%*d ");
        if (err < 0)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        continue;
        
      case 'b':
        err = fscanf(g_file_out_ptr, "%d ", &base);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        continue;

      case 'f':
        err = fscanf(g_file_out_ptr, "%d ", &file_name_id);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        continue;

      case 'B':
        err = fscanf(g_file_out_ptr, "%d %d ", &bank, &slot); 
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        continue;

      case 'k':
        err = fscanf(g_file_out_ptr, "%d ", &line_number);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        continue;

      case 'g':
        err = fscanf(g_file_out_ptr, "%d ", &x);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        bn = g_block_names;
        while (bn != NULL) {
          if (bn->id == x)
            break;
          bn = bn->next;
        }
        
        b = calloc(sizeof(struct block), 1);
        if (b == NULL) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: Out of memory while trying to allocate room for block \"%s\".\n",
                  get_file_name(file_name_id), line_number, bn->name);
          return FAILED;
        }

        b->filename_id = file_name_id;
        b->line_number = line_number;
        b->next = s_blocks;
        s_blocks = b;
        strcpy(b->name, bn->name);
        b->address = address;
        continue;

      case 'G':
        b = s_blocks;
        s_blocks = s_blocks->next;
        if (g_quiet == NO)
          print_text(YES, "%s:%d: INTERNAL_PHASE_1: Block \"%s\" is %d bytes in size.\n", get_file_name(file_name_id), line_number, b->name, address - b->address);
        free(b);
        continue;

      case 't':
        err = fscanf(g_file_out_ptr, "%d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (inz == 0)
          g_namespace[0] = 0;
        else {
          err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_namespace);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }
        continue;

      case 'Z': /* breakpoint */
      case 'Y': /* symbol */
      case 'L': /* label */
        {
          int mangled_label = NO;
          
          l = calloc(sizeof(struct label_def), 1);
          if (l == NULL) {
            err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
            if (err < 1)
              return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
            print_text(NO, "%s:%d: INTERNAL_PHASE_1: Out of memory while trying to allocate room for label \"%s\".\n",
                    get_file_name(file_name_id), line_number, g_tmp);
            return FAILED;
          }

          if (c == 'Y')
            l->symbol = 1;
          else if (c == 'L')
            l->symbol = 0;
          else
            l->symbol = 2;

          if (c == 'Z')
            l->label[0] = 0;
          else {
            err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, l->label);
            if (err < 1)
              return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
          }

          if (c == 'L' && is_label_anonymous(l->label) == NO) {
            /* if the label has '@' at the start, mangle the label name to make it unique */
            int n = 0, m;

            while (n < 10 && l->label[n] == '@')
              n++;
            m = n;
            while (m < 10)
              g_label_context_last->parent_labels[m++] = NULL;

            if (n < 10)
              g_label_context_last->parent_labels[n] = l;
            n--;
            while (n >= 0 && g_label_context_last->parent_labels[n] == 0)
              n--;

            if (n >= 0) {
              if (mangle_label(l->label, g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH, file_name_id, line_number) == FAILED)
                return FAILED;
              mangled_label = YES;
            }
          }

          if (c == 'L' && is_label_anonymous(l->label) == NO && g_namespace[0] != 0 && mangled_label == NO) {
            if (s == NULL || s->nspace == NULL) {
              if (add_namespace(l->label, g_namespace, sizeof(l->label), file_name_id, line_number) == FAILED)
                return FAILED;
            }
          }

          if (c == 'L' && (is_label_anonymous(l->label) == YES || l->label[0] == '_') && g_label_context_last->isolated_macro != NULL) {
            if (add_context_to_anonymous_label(l->label, sizeof(l->label), g_label_context_last, file_name_id, line_number) == FAILED)
              return FAILED;
          }
        
          l->next = NULL;
          l->section_status = ON;
          l->filename_id = file_name_id;
          l->linenumber = line_number;
          l->alive = YES;
          l->section_id = s->id;
          l->section_struct = s;
          /* section labels get a relative address */
          l->address = address;
          l->bank = s->bank;
          l->slot = s->slot;
          l->base = s->base;

          if (c == 'Z' || is_label_anonymous(l->label) == YES) {
            if (g_labels != NULL) {
              g_label_last->next = l;
              g_label_last = l;
            }
            else {
              g_labels = l;
              g_label_last = l;
            }
            continue;
          }

          if (_add_label(l, s, line_number, file_name_id) == FAILED)
            return FAILED;

          continue;
        }

      case 'S':
        err = fscanf(g_file_out_ptr, "%d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        address_old = address;

        s = g_sections_first;
        while (s != NULL && s->id != inz)
          s = s->next;

        /* a .RAMSECTION? */
        if (s->status == SECTION_STATUS_RAM_FREE) {
          s->address = 0;
          address = 0;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          g_section_status = ON;
          continue;
        }
        else if (s->status == SECTION_STATUS_RAM_FORCE) {
          if (s->address < 0)
            s->address = address;
          else
            address = s->address;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          g_section_status = ON;
          continue;
        }
        else if (s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
          if (s->address < 0)
            s->address = address;
          else
            address = s->address;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          g_section_status = ON;
          continue;
        }

        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(file_name_id), line_number);
        return FAILED;

      case 's':
        s->size = address - s->address;

        /* discard an empty section? */
        if (s->size == 0 && s->keep == NO && g_keep_empty_sections == NO) {
          struct after_section *as;
 
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: %s: Discarding an empty section \"%s\".\n", get_file_name(file_name_id), line_number, get_file_name(file_name_id), s->name);
          s->alive = NO;

          /* discard all labels which belong to this section */
          l = g_labels;
          while (l != NULL) {
            if (l->section_status == ON && l->section_id == s->id)
              l->alive = NO;
            l = l->next;
          }

          /* discard appendto/after if one exists */
          as = g_after_sections;
          while (as != NULL) {
            if (as->section == s)
              as->alive = NO;
            as = as->next;
          }
        }

        if (s->advance_org == NO)
          address = address_old;
        else
          address = address_old + s->size;

        g_section_status = OFF;
        s = NULL;
        continue;

      case 'O':
        err = fscanf(g_file_out_ptr, "%d ", &address);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        o++;
        continue;

      default:
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(file_name_id), line_number);
      }
    }
  }
  else {
    /* OUTPUT_LIBRARY */
    while (o == 0 && fread(&c, 1, 1, g_file_out_ptr) != 0) {
      switch (c) {

      case ' ':
      case 'E':
        continue;

      case 'j':
        continue;
      case 'J':
        continue;

      case 'i':
        err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        if (process_macro_in(inz, tmp_buffer, file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;
      case 'I':
        err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        if (process_macro_out(inz, tmp_buffer, file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;

      case 't':
        err = fscanf(g_file_out_ptr, "%d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (inz == 0)
          g_namespace[0] = 0;
        else {
          err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_namespace);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }
        continue;       

      case 'S':
        err = fscanf(g_file_out_ptr, "%d ", &inz);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
        address_old = address;

        s = g_sections_first;
        while (s != NULL && s->id != inz)
          s = s->next;

        if (s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_SEMISUPERFREE) {
          s->address = 0;
          address = 0;
        }
        else {
          if (s->address >= 0)
            address = s->address;
          else
            s->address = address;
        }

        if (s->address_from_dsp >= 0 && s->address_from_dsp != address) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: .SECTION (\"%s\") address sanity check ($%x vs $%x) failed! Please submit a bug report!\n", get_file_name(file_name_id), line_number, s->name, s->address_from_dsp, address);
          return FAILED;
        }

        if (s->bank < 0)
          s->bank = bank;
        if (s->slot < 0)
          s->slot = slot;
        if (s->base < 0)
          s->base = base;
        s->listfile_items = 1;
        s->listfile_ints = NULL;
        s->listfile_cmds = NULL;
        g_section_status = ON;
        o++;
        continue;

      case 'f':
        err = fscanf(g_file_out_ptr, "%d ", &file_name_id);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (s != NULL)
          s->listfile_items++;
        else
          g_global_listfile_items++;
        
        continue;

      case 'k':
        err = fscanf(g_file_out_ptr, "%d ", &line_number);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

        if (s != NULL)
          s->listfile_items++;
        else
          g_global_listfile_items++;

        continue;

      default:
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: A section must be open before any code/data can be accepted.\n", get_file_name(file_name_id), line_number);
        return FAILED;
      }
    }
  }

  /* second (major) loop */
  while (fread(&c, 1, 1, g_file_out_ptr) != 0) {
    switch (c) {

    case ' ':
    case 'E':
      continue;

    case 'j':
      continue;
    case 'J':
      continue;

    case 'i':
      err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        
      if (process_macro_in(inz, tmp_buffer, file_name_id, line_number) == FAILED)
        return FAILED;
        
      continue;
    case 'I':
      err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      if (process_macro_out(inz, tmp_buffer, file_name_id, line_number) == FAILED)
        return FAILED;
        
      continue;

    case 'P':
      address_old = address;
      continue;
    case 'p':
      address = address_old;
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

      address_old = address;

      s = g_sections_first;
      while (s != NULL && s->id != inz)
        s = s->next;

      if (s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_SEMISUPERFREE) {
        s->address = 0;
        address = 0;
      }
      else {
        if (s->address >= 0)
          address = s->address;
        else
          s->address = address;
      }
      
      if (s->bank < 0)
        s->bank = bank;
      if (s->slot < 0)
        s->slot = slot;
      if (s->base < 0)
        s->base = base;

      if (s->address_from_dsp >= 0 && s->address_from_dsp != address) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: .SECTION (\"%s\") address sanity check ($%x vs $%x) failed! Please submit a bug report!\n", get_file_name(file_name_id), line_number, s->name, s->address_from_dsp, address);
        return FAILED;
      }
          
      s->listfile_items = 1;
      s->listfile_ints = NULL;
      s->listfile_cmds = NULL;
      g_section_status = ON;
      continue;

    case 's':
      s->size = address - s->address;

      /* discard an empty section? */
      if (s->size == 0 && s->keep == NO && g_keep_empty_sections == NO) {
        struct after_section *as;
        
        if (g_verbose_level >= 1)
          print_text(NO, "DISCARD: %s: Discarding an empty section \"%s\".\n", get_file_name(file_name_id), s->name);
        s->alive = NO;

        /* discard all labels which belong to this section */
        l = g_labels;
        while (l != NULL) {
          if (l->section_status == ON && l->section_id == s->id)
            l->alive = NO;
          l = l->next;
        }

        /* discard appendto/after if one exists */
        as = g_after_sections;
        while (as != NULL) {
          if (as->section == s)
            as->alive = NO;
          as = as->next;
        }
      }

      /* some sections don't affect the ORG outside of them */
      if (s->advance_org == NO)
        address = address_old;
      else
        address = address_old + s->size;
      
      g_section_status = OFF;
      s = NULL;
      continue;

    case 'x':
    case 'o':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += inz;
      continue;

    case 'X':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += inz * 2;
      continue;

    case 'h':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += inz * 3;
      continue;

    case 'w':
      err = fscanf(g_file_out_ptr, "%d %*d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += inz * 4;
      continue;

    case 'z':
    case 'q':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 3;
      continue;

    case 'T':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 3;
      continue;

    case 'u':
    case 'V':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 4;
      continue;

    case 'U':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 4;
      continue;

    case 'v':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;
        
    case 'b':
      err = fscanf(g_file_out_ptr, "%d ", &base);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'R':
    case 'Q':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address++;
      continue;
      
    case 'd':
    case 'c':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address++;
      continue;

    case '.':
      continue;

    case '?':
    case 'M':
    case 'r':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 2;
      continue;

    case '!':
    case 'y':
    case 'C':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 2;
      continue;

#if defined(SUPERFX)

    case '*':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address++;
      continue;
      
    case '-':
      err = fscanf(g_file_out_ptr, "%*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address++;
      continue;

#endif

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
          if (bits_current == 0)
            address++;
          bits_current += bits_to_add;
          while (bits_current > 8) {
            bits_current -= 8;
            address++;
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
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: Unknown internal .BITS data type '%c'. Please submit a bug report!\n", get_file_name(file_name_id), line_number, type);
          return FAILED;
        }

        continue;
      }

#if defined(SPC700)
    case 'n':
      err = fscanf(g_file_out_ptr, "%*d %*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 2;
      continue;

    case 'N':
      err = fscanf(g_file_out_ptr, "%*d %*d ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += 2;
      continue;
#endif

    case 'D':
      err = fscanf(g_file_out_ptr, "%*d %*d %*d %d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      address += inz;
      continue;

    case 'O':
      err = fscanf(g_file_out_ptr, "%d ", &address);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'B':
      err = fscanf(g_file_out_ptr, "%d %d ", &bank, &slot);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      continue;

    case 'g':
      err = fscanf(g_file_out_ptr, "%d ", &x);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      bn = g_block_names;
      while (bn != NULL) {
        if (bn->id == x)
          break;
        bn = bn->next;
      }

      b = calloc(sizeof(struct block), 1);
      if (b == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_1: Out of memory while trying to allocate room for block \"%s\".\n",
                get_file_name(file_name_id), line_number, bn->name);
        return FAILED;
      }
      b->filename_id = file_name_id;
      b->line_number = line_number;
      b->next = s_blocks;
      s_blocks = b;
      strcpy(b->name, bn->name);
      b->address = address;
      continue;

    case 'G':
      b = s_blocks;
      s_blocks = s_blocks->next;
      if (g_quiet == NO)
        print_text(YES, "%s:%d: INTERNAL_PHASE_1: Block \"%s\" is %d bytes in size.\n", get_file_name(file_name_id), line_number, b->name, address - b->address);
      free(b);
      continue;

    case 't':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      if (inz == 0)
        g_namespace[0] = 0;
      else {
        err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_namespace);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      }
      continue;

    case 'Z': /* breakpoint */
    case 'Y': /* symbol */
    case 'L': /* label */
      {
        int mangled_label = NO;
        
        l = calloc(sizeof(struct label_def), 1);
        if (l == NULL) {
          err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: Out of memory while trying to allocate room for label \"%s\".\n",
                  get_file_name(file_name_id), line_number, g_tmp);
          return FAILED;
        }

        if (c == 'Y')
          l->symbol = 1;
        else if (c == 'L')
          l->symbol = 0;
        else
          l->symbol = 2;

        if (c == 'Z')
          l->label[0] = 0;
        else {
          err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, l->label);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
        }

        if (c == 'L' && is_label_anonymous(l->label) == NO) {
          /* if the label has '@' at the start, mangle the label name to make it unique */
          int n = 0, m;

          while (n < 10 && l->label[n] == '@')
            n++;
          m = n;
          while (m < 10)
            g_label_context_last->parent_labels[m++] = NULL;

          if (n < 10)
            g_label_context_last->parent_labels[n] = l;
          n--;
          while (n >= 0 && g_label_context_last->parent_labels[n] == 0)
            n--;

          if (n >= 0) {
            if (mangle_label(l->label, g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH, file_name_id, line_number) == FAILED)
              return FAILED;
            mangled_label = YES;
          }
        }

        if (c == 'L' && is_label_anonymous(l->label) == NO && g_namespace[0] != 0 && mangled_label == NO) {
          if (s == NULL || s->nspace == NULL) {
            if (add_namespace(l->label, g_namespace, sizeof(l->label), file_name_id, line_number) == FAILED)
              return FAILED;
          }
        }

        if (c == 'L' && (is_label_anonymous(l->label) == YES || l->label[0] == '_') && g_label_context_last->isolated_macro != NULL) {
          if (add_context_to_anonymous_label(l->label, sizeof(l->label), g_label_context_last, file_name_id, line_number) == FAILED)
            return FAILED;
        }
      
        l->next = NULL;
        l->section_status = g_section_status;
        l->filename_id = file_name_id;
        l->linenumber = line_number;
        l->alive = YES;
        if (g_section_status == ON) {
          l->section_id = s->id;
          l->section_struct = s;
          /* section labels get a relative address */
          l->address = address - s->address;
          l->bank = s->bank;
          l->slot = s->slot;
          l->base = s->base;
        }
        else {
          l->section_id = 0;
          l->section_struct = NULL;
          l->address = address;
          l->bank = bank;
          l->slot = slot;
          l->base = base;
        }

        if (c == 'Z' || is_label_anonymous(l->label) == YES) {
          if (g_labels != NULL) {
            g_label_last->next = l;
            g_label_last = l;
          }
          else {
            g_labels = l;
            g_label_last = l;
          }
          continue;
        }

        if (_add_label(l, s, line_number, file_name_id) == FAILED)
          return FAILED;

        continue;
      }

    case 'f':
      err = fscanf(g_file_out_ptr, "%d ", &file_name_id);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);
      
      if (s != NULL)
        s->listfile_items++;
      else
        g_global_listfile_items++;

      continue;

    case 'k':
      err = fscanf(g_file_out_ptr, "%d ", &line_number);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      if (s != NULL)
        s->listfile_items++;
      else
        g_global_listfile_items++;

      continue;

    case 'e':
      err = fscanf(g_file_out_ptr, "%d %d ", &x, &y);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(file_name_id, line_number);

      if (y == -1) { /* mark start of .DSTRUCT */
        s_dstruct_start = address;
        s_dstruct_item_offset = -1;
      }
      else {
        if (s_dstruct_item_offset != -1 && address - s_dstruct_item_offset > s_dstruct_item_size) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_1: %d too many bytes in struct field.\n", get_file_name(file_name_id), line_number, (address - s_dstruct_item_offset) - s_dstruct_item_size);
          return FAILED;
        }

        address = s_dstruct_start + x;
        if (y < 0)
          s_dstruct_item_offset = -1;
        else {
          s_dstruct_item_offset = address;
          s_dstruct_item_size = y;
        }
      }
      continue;

    default:
      print_text(NO, "%s:%d: INTERNAL_PHASE_1: Unknown internal symbol \"%c\"! Please submit a bug report!\n", get_file_name(file_name_id), line_number, c);
      return FAILED;
    }
  }

  if (s_blocks != NULL) {
    print_text(NO, "%s:%d: INTERNAL_PHASE_1: .BLOCK \"%s\" was left open.\n", get_file_name(s_blocks->filename_id), s_blocks->line_number, s_blocks->name);
    return FAILED;
  }

  return SUCCEEDED;
}


/* is the label of form __, -, --, ---, +, ++, +++, -:context, +:context ... ? */
int is_label_anonymous(char *label) {

  int i, length;
  char c;

  if (strcmp(label, "__") == 0)
    return YES;

  if (strlen(label) == 2 && label[0] == '_') {
    c = label[1];
    if (c == 'b' || c == 'B' || c == 'f' || c == 'F')
      return YES;
  }

  c = *label;
  if (c == '-' || c == '+')
    return YES;

  length = (int)strlen(label);
  for (i = 1; i < length; i++) {
    if (label[i] == c)
      continue;
    /* context follows? */
    if (label[i] == ':')
      return YES;

    return NO;
  }
  
  return YES;
}


int mangle_label(char *label, char *parent, int n, unsigned int label_size, int file_name_id, int line_number) {

  char buf[MAX_NAME_LENGTH*2+2];
  int len;

  if (parent == NULL) {
    print_text(NO, "%s:%d: MANGLE_LABEL: Parent label of label \"%s\" is NULL! Please submit a bug report!\n", get_file_name(file_name_id), line_number, label);
    return FAILED;
  }

  len = (int)strlen(parent);

  strcpy(buf, parent);
  strcpy(&buf[len], label+n);

  if (len+strlen(label+n)+1 > label_size) {
    print_text(NO, "%s:%d: MANGLE_LABEL: Child label expands to \"%s\" which is %d characters too large.\n", get_file_name(file_name_id), line_number, buf, (int)(strlen(buf)-label_size+1));
    return FAILED;
  }

  buf[label_size-1] = 0;
  strcpy(label, buf);

  return SUCCEEDED;
}


int add_namespace(char *label, char *name_space, unsigned int label_size, int file_name_id, int line_number) {

  char buf[MAX_NAME_LENGTH*2+2];

  /* don't add namespace to local labels */
  if (label[0] == '_')
    return SUCCEEDED;
  
  if (strncmp(label, "SECTIONSTART_", strlen("SECTIONSTART_")) == 0)
    return SUCCEEDED;
  if (strncmp(label, "SECTIONEND_", strlen("SECTIONEND_")) == 0)
    return SUCCEEDED;
  
  snprintf(buf, sizeof(buf), "%s.%s", name_space, label);
  if (strlen(buf) >= label_size) {
    print_text(NO, "%s:%d: ADD_NAMESPACE: Label expands to \"%s\" which is %d characters too large.\n", get_file_name(file_name_id), line_number, buf, (int)(strlen(buf)-label_size+1));
    return FAILED;
  }

  strcpy(label, buf);

  return SUCCEEDED;
}


static int _macro_get_by_id(int id, struct macro_static **macro_out) {

  struct macro_static *macro;

  macro = g_macros_first;
  while (macro != NULL) {
    if (macro->id == id)
      break;
    macro = macro->next;
  }

  *macro_out = macro;
  
  return SUCCEEDED;
}


int process_macro_in(int id, char *name, int file_name_id, int line_number) {

  struct macro_static *macro;
  int x;

  /* find the .MACRO */
  _macro_get_by_id(id, &macro);
  if (macro == NULL) {
    print_text(NO, "%s:%d: .MACRO %s has gone missing! Please submit a bug report!\n", get_file_name(file_name_id), line_number, name);
    return FAILED;
  }
  
  /* is it ISOLATED? */
  if (macro->isolated_local == YES || macro->isolated_unnamed == YES) {
    /* yes, create a new label context for it */
    struct label_context *lc = calloc(sizeof(struct label_context), 1);

    if (lc == NULL) {
      print_text(NO, "%s:%d: Out of memory while allocating a new label context.\n", get_file_name(file_name_id), line_number);
      return FAILED;
    }

    lc->running_number = g_label_context_running_number++;
    lc->isolated_macro = macro;
    lc->next = NULL;
    lc->prev = g_label_context_last;
    g_label_context_last->next = lc;
    g_label_context_last = lc;
    for (x = 0; x < 10; x++)
      lc->parent_labels[x] = NULL;
  }

  return SUCCEEDED;
}


int process_macro_out(int id, char *name, int file_name_id, int line_number) {

  struct macro_static *macro;

  /* find the .MACRO */
  _macro_get_by_id(id, &macro);
  if (macro == NULL) {
    print_text(NO, "%s:%d: .MACRO %s has gone missing! Please submit a bug report!\n", get_file_name(file_name_id), line_number, name);
    return FAILED;
  }
  
  /* is it ISOLATED? */
  if (macro->isolated_local == YES || macro->isolated_unnamed == YES) {
    /* yes, free the current label context */
    struct label_context *lc = g_label_context_last->prev;

    lc->next = NULL;
    free(g_label_context_last);
    g_label_context_last = lc;
  }

  return SUCCEEDED;
}


int add_context_to_anonymous_label(char *label, int label_size, struct label_context *label_context, int file_name_id, int line_number) {

  char new_label[MAX_NAME_LENGTH + 1];
  int can_do = NO;

  if (is_label_anonymous(label) == YES) {
    if (label_context->isolated_macro->isolated_unnamed == YES)
      can_do = YES;
  }
  else if (label[0] == '_') {
    if (label_context->isolated_macro->isolated_local == YES)
      can_do = YES;
  }

  if (can_do == YES) {
    int full_length = (int)strlen(label) + 1 + (int)strlen(label_context->isolated_macro->name) + 1 + (int)strlen(g_asm_name) + 10;

    if (full_length < label_size - 1 && full_length < (int)(sizeof(new_label) - 1)) {
      snprintf(new_label, sizeof(new_label), "%s:%s_%s_%d", label, g_asm_name, label_context->isolated_macro->name, label_context->running_number);
      strcpy(label, new_label);
    }
    else {
      print_text(NO, "%s:%d: Cannot add context name to the anonymous/local label, buffer is too small!\n", get_file_name(file_name_id), line_number);
      print_text(NO, "   (new label would have been \"%s:%s_%s_%d\")\n", label, g_asm_name, label_context->isolated_macro->name, label_context->running_number);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

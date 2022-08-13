
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include_file.h"
#include "pass_3.h"
#include "printf.h"


extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern struct block_name *g_block_names;
extern struct after_section *g_after_sections;
extern struct macro_static *g_macros_first;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern FILE *g_file_out_ptr;
extern char g_tmp_name[MAX_NAME_LENGTH + 1], *g_tmp, g_namespace[MAX_NAME_LENGTH + 1];
extern char *g_asm_name;
extern int g_verbose_mode, g_section_status, g_output_format, g_keep_empty_sections, g_quiet, g_sizeof_g_error_message;
extern int g_global_listfile_items;

struct label_def *g_label_last, *g_label_tmp, *g_labels = NULL;
struct map_t *g_global_unique_label_map = NULL;
struct block *g_blocks = NULL;
struct label_context g_label_context, *g_label_context_first = NULL, *g_label_context_last = NULL;

static int g_dstruct_start, g_dstruct_item_offset, g_dstruct_item_size, s_mangled_label;

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


int pass_3(void) {

  struct section_def *s = NULL;
  struct label_def *l;
  struct block_name *bn;
  struct block *b;
  char tmp_buffer[MAX_NAME_LENGTH + 1];
  FILE *f_in;
  int bank = 0, slot = 0, add = 0, g_file_name_id = 0, inz, line_number = 0, o, add_old = 0, inside_macro = 0, inside_repeat = 0;
  int base = 0x00, bits_current = 0;
  int x, y;
  char c;
  int err;

  /* initialize label context */
  g_label_context.isolated_macro = NULL;
  g_label_context.next = NULL;
  g_label_context.prev = NULL;
  for (x = 0; x < 10; x++)
    g_label_context.parent_labels[x] = NULL;
  g_label_context_last = &g_label_context;
  g_label_context_first = &g_label_context;
  
  g_namespace[0] = 0;
  
  if (g_verbose_mode == ON)
    printf("Internal pass 1...\n");

  if ((f_in = fopen(g_tmp_name, "rb")) == NULL) {
    fprintf(stderr, "INTERNAL_PASS_1: Error opening file \"%s\".\n", g_tmp_name);
    return FAILED;
  }

  /* first loop */
  o = 0;
  if (g_output_format == OUTPUT_OBJECT) {
    while (o == 0 && fread(&c, 1, 1, f_in) != 0) {
      switch (c) {

      case ' ':
      case 'E':
        continue;

      case 'j':
        inside_repeat++;
        continue;
      case 'J':
        inside_repeat--;
        continue;

      case 'i':
        fscanf(f_in, "%d %s ", &inz, tmp_buffer);
        inside_macro++;

        if (process_macro_in(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;
      case 'I':
        fscanf(f_in, "%d %s ", &inz, tmp_buffer);
        inside_macro--;

        if (process_macro_out(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;

      case 'P':
        add_old = add;
        continue;
      case 'p':
        add = add_old;
        continue;

      case 'x':
      case 'o':
        fscanf(f_in, "%d %*d ", &inz);
        if (g_section_status == ON) {
          add += inz;
          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(g_file_name_id), line_number);
        return FAILED;

      case 'd':
        if (g_section_status == ON) {
          fscanf(f_in, "%*s ");
          add++;
          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(g_file_name_id), line_number);
        return FAILED;

      case 'y':
        if (g_section_status == ON) {
          fscanf(f_in, "%*d ");
          add += 2;
          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(g_file_name_id), line_number);
        return FAILED;

#ifdef SUPERFX

      case '*':
        if (g_section_status == ON) {
          fscanf(f_in, "%*s ");
          add++;
          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(g_file_name_id), line_number);
        return FAILED;
      
      case '-':
        if (g_section_status == ON) {
          fscanf(f_in, "%*d ");
          add++;
          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n",
                get_file_name(g_file_name_id), line_number);
        return FAILED;

#endif

      case '+':
        if (g_section_status == ON) {
          int bits_to_add;
          char type;
          
          fscanf(f_in, "%d ", &bits_to_add);

          if (bits_to_add == 0) {
            bits_current = 0;
          }
          else {
            if (bits_current == 0)
              add++;
            bits_current += bits_to_add;
            while (bits_current > 8) {
              bits_current -= 8;
              add++;
            }
            if (bits_to_add == 8)
              bits_current = 0;
          }

          fscanf(f_in, "%c", &type);

          if (type == 'a')
            fscanf(f_in, "%*d");
          else if (type == 'b')
            fscanf(f_in, "%*s");
          else if (type == 'c')
            fscanf(f_in, "%*d");
          else {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Unknown internal .BITS data type '%c'. Please submit a bug report!\n", get_file_name(g_file_name_id), line_number, type);
            return FAILED;
          }

          continue;
        }

        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(g_file_name_id), line_number);
        return FAILED;
        
      case 'v':
        fscanf(f_in, "%*d ");
        continue;
        
      case 'b':
        fscanf(f_in, "%d ", &base);
        continue;

      case 'f':
        fscanf(f_in, "%d ", &g_file_name_id);
        continue;

      case 'B':
        fscanf(f_in, "%d %d ", &bank, &slot);
        continue;

      case 'k':
        fscanf(f_in, "%d ", &line_number);
        continue;

      case 'g':
        fscanf(f_in, "%d ", &x);

        bn = g_block_names;
        while (bn != NULL) {
          if (bn->id == x)
            break;
          bn = bn->next;
        }
        
        b = calloc(sizeof(struct block), 1);
        if (b == NULL) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Out of memory while trying to allocate room for block \"%s\".\n",
                  get_file_name(g_file_name_id), line_number, bn->name);
          return FAILED;
        }

        b->filename_id = g_file_name_id;
        b->line_number = line_number;
        b->next = g_blocks;
        g_blocks = b;
        strcpy(b->name, bn->name);
        b->address = add;
        continue;

      case 'G':
        b = g_blocks;
        g_blocks = g_blocks->next;
        if (g_quiet == NO)
          printf("%s:%d: INTERNAL_PASS_1: Block \"%s\" is %d bytes in size.\n", get_file_name(g_file_name_id), line_number, b->name, add - b->address);
        free(b);
        continue;

      case 't':
        fscanf(f_in, "%d ", &inz);
        if (inz == 0)
          g_namespace[0] = 0;
        else
          fscanf(f_in, STRING_READ_FORMAT, g_namespace);
        continue;

      case 'Z': /* breakpoint */
      case 'Y': /* symbol */
      case 'L': /* label */
        l = calloc(sizeof(struct label_def), 1);
        if (l == NULL) {
          fscanf(f_in, STRING_READ_FORMAT, g_tmp);
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Out of memory while trying to allocate room for label \"%s\".\n",
                  get_file_name(g_file_name_id), line_number, g_tmp);
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
        else
          fscanf(f_in, STRING_READ_FORMAT, l->label);

        s_mangled_label = NO;

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
            if (mangle_label(l->label, g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH, g_file_name_id, line_number) == FAILED)
              return FAILED;
            s_mangled_label = YES;
          }
        }

        if (c == 'L' && is_label_anonymous(l->label) == NO && g_namespace[0] != 0 && s_mangled_label == NO) {
          if (s == NULL || s->nspace == NULL) {
            if (add_namespace(l->label, g_namespace, sizeof(l->label), g_file_name_id, line_number) == FAILED)
              return FAILED;
          }
        }

        if (c == 'L' && (is_label_anonymous(l->label) == YES || l->label[0] == '_') && g_label_context_last->isolated_macro != NULL) {
          if (add_context_to_anonymous_label(l->label, sizeof(l->label), g_label_context_last, g_file_name_id, line_number) == FAILED)
            return FAILED;
        }
        
        l->next = NULL;
        l->section_status = ON;
        l->filename_id = g_file_name_id;
        l->linenumber = line_number;
        l->alive = YES;
        l->section_id = s->id;
        l->section_struct = s;
        /* section labels get a relative address */
        l->address = add;
        l->bank = s->bank;
        l->slot = s->slot;
        l->base = base;

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

        /* check the label is not already defined */

        if (s != NULL) {
          /* always put the label into the section's label_map */
          if (hashmap_get(s->label_map, l->label, NULL) == MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
            return FAILED;
          }
          if ((err = hashmap_put(s->label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
            return FAILED;
          }
        }

        /* don't put local labels into namespaces or the global namespace */
        if (s == NULL || l->label[0] != '_') {
          if (s != NULL && s->nspace != NULL) {
            /* label in a namespace */
            if (hashmap_get(s->nspace->label_map, l->label, NULL) == MAP_OK) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
              return FAILED;
            }
            if ((err = hashmap_put(s->nspace->label_map, l->label, l)) != MAP_OK) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
              return FAILED;
            }
          }
          else {
            /* global label */
            if (hashmap_get(g_global_unique_label_map, l->label, NULL) == MAP_OK) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
              return FAILED;
            }
            if ((err = hashmap_put(g_global_unique_label_map, l->label, l)) != MAP_OK) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
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

        continue;

      case 'S':
        fscanf(f_in, "%d ", &inz);

        add_old = add;

        s = g_sections_first;
        while (s != NULL && s->id != inz)
          s = s->next;

        /* a .RAMSECTION? */
        if (s->status == SECTION_STATUS_RAM_FREE) {
          s->address = 0;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          add = 0;
          g_section_status = ON;
          continue;
        }
        else if (s->status == SECTION_STATUS_RAM_FORCE) {
          if (s->address < 0)
            s->address = add;
          else
            add = s->address;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          g_section_status = ON;
          continue;
        }
        else if (s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
          s->address = add;
          s->listfile_items = 1;
          s->listfile_ints = NULL;
          s->listfile_cmds = NULL;
          g_section_status = ON;
          continue;
        }
        
        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(g_file_name_id), line_number);
        return FAILED;

      case 's':
        s->size = add - s->address;

        /* discard an empty section? */
        if (s->size == 0 && s->keep == NO && g_keep_empty_sections == NO) {
          struct after_section *as;
 
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: %s: Discarding an empty section \"%s\".\n", get_file_name(g_file_name_id), line_number, get_file_name(g_file_name_id), s->name);
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
          add = add_old;
        else
          add = add_old + s->size;

        g_section_status = OFF;
        s = NULL;
        continue;

      case 'O':
        fscanf(f_in, "%d ", &add);
        o++;
        continue;

      default:
        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n", get_file_name(g_file_name_id), line_number);
      }
    }
  }
  else {
    while (o == 0 && fread(&c, 1, 1, f_in) != 0) {
      switch (c) {

      case ' ':
      case 'E':
        continue;

      case 'j':
        inside_repeat++;
        continue;
      case 'J':
        inside_repeat--;
        continue;

      case 'i':
        fscanf(f_in, "%d %s ", &inz, tmp_buffer);
        inside_macro++;

        if (process_macro_in(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;
      case 'I':
        fscanf(f_in, "%d %s ", &inz, tmp_buffer);
        inside_macro--;

        if (process_macro_out(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
          return FAILED;
        
        continue;

      case 'f':
        fscanf(f_in, "%d ", &g_file_name_id);
        continue;

      case 't':
        fscanf(f_in, "%d ", &inz);
        if (inz == 0)
          g_namespace[0] = 0;
        else
          fscanf(f_in, STRING_READ_FORMAT, g_namespace);
        continue;       

      case 'S':
        fscanf(f_in, "%d ", &inz);

        add_old = add;

        s = g_sections_first;
        while (s != NULL && s->id != inz)
          s = s->next;

        if (s->status == SECTION_STATUS_FREE)
          add = 0;

        if (s->status == SECTION_STATUS_RAM_FORCE) {
          if (s->address < 0)
            s->address = add;
          else
            add = s->address;
        }
        else
          s->address = add;

        s->bank = bank;
        s->base = base;
        s->slot = slot;
        s->listfile_items = 1;
        s->listfile_ints = NULL;
        s->listfile_cmds = NULL;
        g_section_status = ON;
        o++;
        continue;

      case 'k':
        fscanf(f_in, "%d ", &line_number);
        continue;

      default:
        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: A section must be open before any code/data can be accepted.\n", get_file_name(g_file_name_id), line_number);
        return FAILED;
      }
    }
  }

  inside_macro = 0;
  inside_repeat = 0;

  /* second (major) loop */
  while (fread(&c, 1, 1, f_in) != 0) {
    switch (c) {

    case ' ':
    case 'E':
      continue;

    case 'j':
      inside_repeat++;
      continue;
    case 'J':
      inside_repeat--;
      continue;

    case 'i':
      fscanf(f_in, "%d %s ", &inz, tmp_buffer);
      inside_macro++;

      if (process_macro_in(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
        return FAILED;
        
      continue;
    case 'I':
      fscanf(f_in, "%d %s ", &inz, tmp_buffer);
      inside_macro--;

      if (process_macro_out(inz, tmp_buffer, g_file_name_id, line_number) == FAILED)
        return FAILED;
        
      continue;

    case 'P':
      add_old = add;
      continue;
    case 'p':
      add = add_old;
      continue;

    case 'A':
    case 'S':
      if (c == 'A')
        fscanf(f_in, "%d %*d", &inz);
      else
        fscanf(f_in, "%d ", &inz);

      add_old = add;

      s = g_sections_first;
      while (s != NULL && s->id != inz)
        s = s->next;

      if (s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_RAM_FREE)
        add = 0;

      if (s->status != SECTION_STATUS_RAM_FREE && s->status != SECTION_STATUS_RAM_FORCE && s->status != SECTION_STATUS_RAM_SEMIFREE && s->status != SECTION_STATUS_RAM_SEMISUBFREE) {
        s->bank = bank;
        s->base = base;
        s->slot = slot;
      }

      if (s->status == SECTION_STATUS_RAM_FORCE) {
        if (s->address < 0)
          s->address = add;
        else
          add = s->address;
      }
      else
        s->address = add;
      
      s->listfile_items = 1;
      s->listfile_ints = NULL;
      s->listfile_cmds = NULL;
      g_section_status = ON;
      continue;

    case 's':
      s->size = add - s->address;

      /* discard an empty section? */
      if (s->size == 0 && s->keep == NO && g_keep_empty_sections == NO) {
        struct after_section *as;
        
        if (g_verbose_mode == ON)
          fprintf(stderr, "DISCARD: %s: Discarding an empty section \"%s\".\n", get_file_name(g_file_name_id), s->name);
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
        add = add_old;
      else
        add = add_old + s->size;
      
      g_section_status = OFF;
      s = NULL;
      continue;

    case 'x':
    case 'o':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz;
      continue;

    case 'X':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 2;
      continue;

    case 'h':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 3;
      continue;

    case 'w':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 4;
      continue;

    case 'z':
    case 'q':
      fscanf(f_in, "%*s ");
      add += 3;
      continue;

    case 'T':
      fscanf(f_in, "%*d ");
      add += 3;
      continue;

    case 'u':
    case 'V':
      fscanf(f_in, "%*s ");
      add += 4;
      continue;

    case 'U':
      fscanf(f_in, "%*d ");
      add += 4;
      continue;

    case 'v':
      fscanf(f_in, "%*d ");
      continue;
        
    case 'b':
      fscanf(f_in, "%d ", &base);
      continue;

    case 'R':
    case 'Q':
    case 'd':
    case 'c':
      fscanf(f_in, "%*s ");
      add++;
      continue;

    case 'M':
    case 'r':
      fscanf(f_in, "%*s ");
      add += 2;
      continue;

    case 'y':
    case 'C':
      fscanf(f_in, "%*d ");
      add += 2;
      continue;

#ifdef SUPERFX

    case '*':
      fscanf(f_in, "%*s ");
      add++;
      continue;
      
    case '-':
      fscanf(f_in, "%*d ");
      add++;
      continue;

#endif

    case '+':
      {
        int bits_to_add;
        char type;
          
        fscanf(f_in, "%d ", &bits_to_add);

        if (bits_to_add == 999) {
          bits_current = 0;

          continue;
        }
        else {
          if (bits_current == 0)
            add++;
          bits_current += bits_to_add;
          while (bits_current > 8) {
            bits_current -= 8;
            add++;
          }
          if (bits_to_add == 8)
            bits_current = 0;
        }

        fscanf(f_in, "%c", &type);

        if (type == 'a')
          fscanf(f_in, "%*d");
        else if (type == 'b')
          fscanf(f_in, "%*s");
        else if (type == 'c')
          fscanf(f_in, "%*d");
        else {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Unknown internal .BITS data type '%c'. Please submit a bug report!\n", get_file_name(g_file_name_id), line_number, type);
          return FAILED;
        }

        continue;
      }

#ifdef SPC700
    case 'n':
      fscanf(f_in, "%*d %*s ");
      add += 2;
      continue;

    case 'N':
      fscanf(f_in, "%*d %*d ");
      add += 2;
      continue;
#endif

    case 'D':
      fscanf(f_in, "%*d %*d %*d %d ", &inz);
      add += inz;
      continue;

    case 'O':
      fscanf(f_in, "%d ", &add);
      continue;

    case 'B':
      fscanf(f_in, "%d %d ", &bank, &slot);
      continue;

    case 'g':
      fscanf(f_in, "%d ", &x);

      bn = g_block_names;
      while (bn != NULL) {
        if (bn->id == x)
          break;
        bn = bn->next;
      }

      b = calloc(sizeof(struct block), 1);
      if (b == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Out of memory while trying to allocate room for block \"%s\".\n",
                get_file_name(g_file_name_id), line_number, bn->name);
        return FAILED;
      }
      b->filename_id = g_file_name_id;
      b->line_number = line_number;
      b->next = g_blocks;
      g_blocks = b;
      strcpy(b->name, bn->name);
      b->address = add;
      continue;

    case 'G':
      b = g_blocks;
      g_blocks = g_blocks->next;
      if (g_quiet == NO) {
        printf("%s:%d: INTERNAL_PASS_1: Block \"%s\" is %d bytes in size.\n", get_file_name(g_file_name_id), line_number, b->name, add - b->address);
      }
      free(b);
      continue;

    case 't':
      fscanf(f_in, "%d ", &inz);
      if (inz == 0)
        g_namespace[0] = 0;
      else
        fscanf(f_in, STRING_READ_FORMAT, g_namespace);
      continue;

    case 'Z': /* breakpoint */
    case 'Y': /* symbol */
    case 'L': /* label */
      l = calloc(sizeof(struct label_def), 1);
      if (l == NULL) {
        fscanf(f_in, STRING_READ_FORMAT, g_tmp);
        fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Out of memory while trying to allocate room for label \"%s\".\n",
                get_file_name(g_file_name_id), line_number, g_tmp);
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
      else
        fscanf(f_in, STRING_READ_FORMAT, l->label);

      s_mangled_label = NO;
      
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
          if (mangle_label(l->label, g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH, g_file_name_id, line_number) == FAILED)
            return FAILED;
          s_mangled_label = YES;
        }
      }

      if (c == 'L' && is_label_anonymous(l->label) == NO && g_namespace[0] != 0 && s_mangled_label == NO) {
        if (s == NULL || s->nspace == NULL) {
          if (add_namespace(l->label, g_namespace, sizeof(l->label), g_file_name_id, line_number) == FAILED)
            return FAILED;
        }
      }

      if (c == 'L' && (is_label_anonymous(l->label) == YES || l->label[0] == '_') && g_label_context_last->isolated_macro != NULL) {
        if (add_context_to_anonymous_label(l->label, sizeof(l->label), g_label_context_last, g_file_name_id, line_number) == FAILED)
          return FAILED;
      }
      
      l->next = NULL;
      l->section_status = g_section_status;
      l->filename_id = g_file_name_id;
      l->linenumber = line_number;
      l->alive = YES;
      if (g_section_status == ON) {
        l->section_id = s->id;
        l->section_struct = s;
        /* section labels get a relative address */
        l->address = add - s->address;
        l->bank = s->bank;
        l->slot = s->slot;
      }
      else {
        l->section_id = 0;
        l->section_struct = NULL;
        l->address = add;
        l->bank = bank;
        l->slot = slot;
      }

      l->base = base;

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

      /* check the label is not already defined */

      if (s != NULL) {
        /* always put the label into the section's label_map */
        if (hashmap_get(s->label_map, l->label, NULL) == MAP_OK) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
          return FAILED;
        }
        if ((err = hashmap_put(s->label_map, l->label, l)) != MAP_OK) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
          return FAILED;
        }
      }

      /* don't put local labels into namespaces or the global namespace */
      if (s == NULL || l->label[0] != '_') {
        if (s != NULL && s->nspace != NULL) {
          /* label in a namespace */
          if (hashmap_get(s->nspace->label_map, l->label, NULL) == MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
            return FAILED;
          }
          if ((err = hashmap_put(s->nspace->label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
            return FAILED;
          }
        }
        else {
          /* global label */
          if (hashmap_get(g_global_unique_label_map, l->label, NULL) == MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.\n", get_file_name(g_file_name_id), line_number, l->label);
            return FAILED;
          }
          if ((err = hashmap_put(g_global_unique_label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Hashmap error %d. Please send a bug report!", get_file_name(g_file_name_id), line_number, err);
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

      continue;

    case 'f':
      fscanf(f_in, "%d ", &g_file_name_id);
      if (s != NULL)
        s->listfile_items++;
      else
        g_global_listfile_items++;
      continue;

    case 'k':
      fscanf(f_in, "%d ", &line_number);
      if (s != NULL)
        s->listfile_items++;
      else
        g_global_listfile_items++;
      continue;

    case 'e':
      fscanf(f_in, "%d %d ", &x, &y);
      if (y == -1) { /* mark start of .DSTRUCT */
        g_dstruct_start = add;
        g_dstruct_item_offset = -1;
      }
      else {
        if (g_dstruct_item_offset != -1 && add - g_dstruct_item_offset > g_dstruct_item_size) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_1: %d too many bytes in struct field.\n", get_file_name(g_file_name_id), line_number, (add - g_dstruct_item_offset) - g_dstruct_item_size);
          return FAILED;
        }

        add = g_dstruct_start + x;
        if (y < 0)
          g_dstruct_item_offset = -1;
        else {
          g_dstruct_item_offset = add;
          g_dstruct_item_size = y;
        }
      }
      continue;

    default:
      fprintf(stderr, "%s:%d: INTERNAL_PASS_1: Unknown internal symbol \"%c\" in \"%s\" at offset %ld.\n", get_file_name(g_file_name_id), line_number, c, g_tmp_name, ftell(f_in)-1);
      return FAILED;
    }
  }

  fclose(f_in);

  if (g_blocks != NULL) {
    fprintf(stderr, "%s:%d: INTERNAL_PASS_1: .BLOCK \"%s\" was left open.\n", get_file_name(g_blocks->filename_id), g_blocks->line_number, g_blocks->name);
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
    fprintf(stderr, "%s:%d: MANGLE_LABEL: Parent label of label \"%s\" is NULL! Please submit a bug report!\n", get_file_name(file_name_id), line_number, label);
    return FAILED;
  }

  len = (int)strlen(parent);

  strcpy(buf, parent);
  strcpy(&buf[len], label+n);

  if (len+strlen(label+n)+1 > label_size) {
    fprintf(stderr, "%s:%d: MANGLE_LABEL: Child label expands to \"%s\" which is %d characters too large.\n", get_file_name(file_name_id), line_number, buf, (int)(strlen(buf)-label_size+1));
    return FAILED;
  }

  buf[label_size-1] = 0;
  strcpy(label, buf);

  return SUCCEEDED;
}


int add_namespace(char *label, char *name_space, unsigned int label_size, int file_name_id, int line_number) {

  char buf[MAX_NAME_LENGTH*2+2];

  if (strncmp(label, "SECTIONSTART_", strlen("SECTIONSTART_")) == 0)
    return SUCCEEDED;
  if (strncmp(label, "SECTIONEND_", strlen("SECTIONEND_")) == 0)
    return SUCCEEDED;
  
  snprintf(buf, sizeof(buf), "%s.%s", name_space, label);
  if (strlen(buf) >= label_size) {
    fprintf(stderr, "%s:%d: ADD_NAMESPACE: Label expands to \"%s\" which is %d characters too large.\n", get_file_name(file_name_id), line_number, buf, (int)(strlen(buf)-label_size+1));
    return FAILED;
  }

  strcpy(label, buf);

  return SUCCEEDED;
}


int macro_get_by_id(int id, struct macro_static **macro_out) {

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
  macro_get_by_id(id, &macro);
  if (macro == NULL) {
    fprintf(stderr, "%s:%d: .MACRO %s has gone missing! Please submit a bug report!\n", get_file_name(file_name_id), line_number, name);
    return FAILED;
  }
  
  /* is it ISOLATED? */
  if (macro->isolated_local == YES || macro->isolated_unnamed == YES) {
    /* yes, create a new label context for it */
    struct label_context *lc = calloc(sizeof(struct label_context), 1);

    if (lc == NULL) {
      fprintf(stderr, "%s:%d: Out of memory while allocating a new label context.\n", get_file_name(file_name_id), line_number);
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
  macro_get_by_id(id, &macro);
  if (macro == NULL) {
    fprintf(stderr, "%s:%d: .MACRO %s has gone missing! Please submit a bug report!\n", get_file_name(file_name_id), line_number, name);
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
      fprintf(stderr, "%s:%d: Cannot add context name to the anonymous/local label, buffer is too small!\n", get_file_name(file_name_id), line_number);
      fprintf(stderr, "   (new label would have been \"%s:%s_%s_%d\")\n", label, g_asm_name, label_context->isolated_macro->name, label_context->running_number);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

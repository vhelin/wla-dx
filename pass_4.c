
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include_file.h"
#include "listfile.h"
#include "pass_3.h"
#include "pass_4.h"
#include "parse.h"
#include "stack.h"
#include "printf.h"


extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct export_def *g_export_first, *g_export_last;
extern struct stack *g_stacks_first, *g_stacks_tmp, *g_stacks_last, *g_stacks_header_first, *g_stacks_header_last;
extern struct label_def *g_label_tmp, *g_label_last, *g_labels;
extern struct definition *g_tmp_def;
extern struct map_t *g_defines_map, *g_namespace_map, *g_global_unique_label_map;
extern struct file_name_info *g_file_name_info_first;
extern struct slot g_slots[256];
extern struct append_section *g_append_sections;
extern struct label_sizeof *g_label_sizeofs;
extern FILE *g_file_out_ptr;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern char g_tmp_name[MAX_NAME_LENGTH + 1], g_tmp[4096], *g_final_name;
extern int g_rombanks, g_output_format, g_test_mode, g_listfile_data, g_little_endian;

#ifdef GB
extern char g_licenseecodenew_c1, g_licenseecodenew_c2;
extern int g_computechecksum_defined, g_computecomplementcheck_defined;
extern int g_cartridgetype;
extern int g_name_defined, g_computechecksum_defined, g_computecomplementcheck_defined;
extern int g_licenseecodeold_defined, g_licenseecodenew_defined;
extern int g_licenseecodeold, g_romgbc;
#endif

extern int g_romtype, g_rambanks, g_emptyfill, g_max_address;
extern int g_rambanks_defined, g_verbose_mode;
extern int g_section_status;
extern int g_banksize, g_banksize_defined;
extern int g_slots_amount;
extern int *g_banks, *g_bankaddress, g_rombankmap_defined;
extern int g_latest_stack, g_stacks_outside, g_stacks_inside;
extern int g_bankheader_status;
extern int g_smc_defined, g_makefile_rules;

#if defined(W65816)
extern int g_lorom_defined, g_hirom_defined, g_slowrom_defined, g_fastrom_defined, g_snes_mode, g_exhirom_defined;
extern int g_computesneschecksum_defined, g_sramsize, g_sramsize_defined, g_exlorom_defined;
#endif

#ifdef Z80
extern int g_computesmschecksum_defined, g_smstag_defined, g_smsheader_defined;
#endif

struct label_def *g_unknown_labels = NULL, *g_unknown_labels_last = NULL;
struct label_def *g_unknown_header_labels = NULL, *g_unknown_header_labels_last = NULL;
struct label_def *g_parent_labels[10];
struct append_section *g_append_tmp;
struct label_sizeof *g_label_sizeof_tmp;

char g_mem_insert_action[MAX_NAME_LENGTH * 3 + 1024], g_namespace[MAX_NAME_LENGTH + 1];
static int g_pc_bank = 0, g_pc_full = 0, g_rom_bank, g_mem_insert_overwrite, g_slot = 0, g_base = 0, g_pc_slot, g_pc_slot_max;
static int g_filename_id, g_line_number;
static int g_dstruct_start = -1, g_special_id = 0;


#define WRITEOUT_OV fprintf(final_ptr, "%c%c%c%c", (ov>>24)&0xFF, (ov>>16)&0xFF, (ov>>8)&0xFF, ov&0xFF);
#define WRITEOUT_DOU {                                                  \
    cp = (unsigned char *)&dou;                                         \
    fprintf(final_ptr, "%c%c%c%c%c%c%c%c", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]); \
  }

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")



struct label_def *new_unknown_reference(int type) {

  struct label_def *label;
  int n = 0, j = 0;

  if (g_tmp[0] == ':')
    j = 1;
  while (n < 10 && g_tmp[n+j] == '@')
    n++;
  n--;
  while (n >= 0 && g_parent_labels[n] == NULL)
    n--;
  if (n >= 0) {
    if (mangle_label(&g_tmp[j], g_parent_labels[n]->label, n, MAX_NAME_LENGTH-j) == FAILED)
      return NULL;
  }

  label = calloc(sizeof(struct label_def), 1);
  if (label == NULL) {
    fprintf(stderr, "%s:%d: NEW_UNKNOWN_REFERENCE: Out of memory.\n", get_file_name(g_filename_id), g_line_number);
    return NULL;
  }

  label->symbol = NO;

  strcpy(label->label, g_tmp);
  label->next = NULL;
  label->type = type;
  label->filename_id = g_filename_id;
  label->linenumber = g_line_number;
  label->section_status = g_section_status;
  if (g_section_status == ON) {
    label->section_id = g_sec_tmp->id;
    label->section_struct = g_sec_tmp;
    /* relative address, to the beginning of the section */
    label->address = g_sec_tmp->i;
  }
  else {
    label->section_id = 0;
    label->section_struct = NULL;
    /* bank address, in ROM memory */
    label->address = g_pc_bank;
  }

  label->bank = g_rom_bank;
  label->slot = g_slot;
  label->base = g_base;
  label->special_id = g_special_id;
  label->bits_position = 0;
  label->bits_to_define = 0;

  /* outside bank header section */
  if (g_bankheader_status == OFF) {
    if (g_unknown_labels_last == NULL) {
      g_unknown_labels = label;
      g_unknown_labels_last = label;
    }
    else {
      g_unknown_labels_last->next = label;
      g_unknown_labels_last = label;
    }
  }
  /* bank header section */
  else {
    if (label->label[0] == '_') {
      fprintf(stderr, "%s:%d: NEW_UNKNOWN_REFERENCE: Referring to a local label (\"%s\") from inside a bank header section is not allowed.\n",
              get_file_name(g_filename_id), g_line_number, label->label);
      return NULL;
    }
    if (g_unknown_header_labels_last == NULL) {
      g_unknown_header_labels = label;
      g_unknown_header_labels_last = label;
    }
    else {
      g_unknown_header_labels_last->next = label;
      g_unknown_header_labels_last = label;
    }
  }

  return label;
}


int mangle_stack_references(struct stack *stack) {

  int ind, n, j;

  for (ind = 0; ind < stack->stacksize; ind++) {
    if (stack->stack[ind].type == STACK_ITEM_TYPE_STRING) {
      n = 0;
      j = 0;

      if (stack->stack[ind].string[0] == ':')
        j = 1;

      while (n < 10 && stack->stack[ind].string[n+j] == '@')
        n++;

      n--;
      while (n >= 0 && g_parent_labels[n] == NULL)
        n--;

      if (n >= 0) {
        if (mangle_label(&stack->stack[ind].string[j], g_parent_labels[n]->label, n, MAX_NAME_LENGTH-j) == FAILED)
          return FAILED;
      }
    }
  }

  return SUCCEEDED;
}


int add_namespace_to_reference(char *label, char *name_space, unsigned int label_size) {

  char expanded[MAX_NAME_LENGTH*2+2];
  struct label_def *l;
  
  snprintf(expanded, sizeof(expanded), "%s.%s", name_space, label);
  if (strlen(expanded) >= label_size) {
    fprintf(stderr, "ADD_NAMESPACE_TO_REFERENCE: Label expands to \"%s\" which is %d characters too large.\n", expanded, (int)(strlen(expanded)-label_size+1));
    return FAILED;
  }

  /* use the expanded version only if we can find it */
  
  /* label in a namespace? */
  if (g_section_status == ON && g_sec_tmp != NULL && g_sec_tmp->nspace != NULL) {
    if (hashmap_get(g_sec_tmp->nspace->label_map, expanded, (void*)&l) == MAP_OK) {
      if (g_filename_id == l->filename_id) {
        strcpy(label, expanded);
        return SUCCEEDED;
      }
    }
  }

  /* global label? */
  if (hashmap_get(g_global_unique_label_map, expanded, (void*)&l) == MAP_OK) {
    if (g_filename_id == l->filename_id) {
      strcpy(label, expanded);
      return SUCCEEDED;
    }
  }

  return SUCCEEDED;
}


int add_namespace_to_stack_references(struct stack *st, char *name_space) {

  int j;

  for (j = 0; j < st->stacksize; j++) {
    if (st->stack[j].type == STACK_ITEM_TYPE_STRING) {
      if (is_label_anonymous(st->stack[j].string) == YES)
        continue;
      
      if (add_namespace_to_reference(st->stack[j].string, name_space, sizeof(st->stack[j].string)) == FAILED)
        return FAILED;
    }
  }

  return SUCCEEDED;
}


static void _bits_add_bit(int *bits_byte, int *bits_position, int *bits_to_define, int data) {

  int bit = (data >> (*bits_to_define - 1)) & 1;

  *bits_to_define = *bits_to_define - 1;

  *bits_byte = *bits_byte | (bit << (7 - *bits_position));

  *bits_position = *bits_position + 1;
}


int pass_4(void) {

  unsigned char *cp;
  FILE *final_ptr;
  double dou;
  char *t, c;
  int i, o, z, y, add_old = 0, x, q, ov, inside_macro = 0, inside_repeat = 0, inz, ind, bits_position = 0, bits_byte = 0;
  float f;

  memset(g_parent_labels, 0, sizeof(g_parent_labels));

  g_namespace[0] = 0;
  
  g_section_status = OFF;
  g_bankheader_status = OFF;
  g_mem_insert_overwrite = OFF;

  if (g_verbose_mode == ON)
    printf("Internal pass 2...\n");

  if ((g_file_out_ptr = fopen(g_tmp_name, "rb")) == NULL) {
    fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\".\n", g_tmp_name);
    return FAILED;
  }

  while (fread(&c, 1, 1, g_file_out_ptr) != 0) {
    switch (c) {

      /* SPECIAL CASE ID */
      
    case 'v':
      fscanf(g_file_out_ptr, "%d ", &g_special_id);       
      continue;
      
    case 'E':
      continue;

    case 'j':
      inside_repeat++;
      continue;
    case 'J':
      inside_repeat--;
      continue;

    case 'i':
      fscanf(g_file_out_ptr, "%*s ");
      inside_macro++;
      continue;
    case 'I':
      fscanf(g_file_out_ptr, "%*s ");
      inside_macro--;
      continue;
        
    case 'g':
      fscanf(g_file_out_ptr, "%*s ");
      continue;
    case 'G':
      continue;

    case 'f':
      fscanf(g_file_out_ptr, "%d ", &g_filename_id);
      continue;

    case 'k':
      fscanf(g_file_out_ptr, "%d ", &g_line_number);
      continue;

    case 't':
      fscanf(g_file_out_ptr, "%d ", &inz);
      if (inz == 0)
        g_namespace[0] = 0;
      else
        fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_namespace);
      continue;
        
      /* SECTION */

    case 'A':
    case 'S':
      if (c == 'A')
        fscanf(g_file_out_ptr, "%d %d ", &x, &ind);
      else
        fscanf(g_file_out_ptr, "%d ", &x);

      add_old = g_pc_bank;

      g_sec_tmp = g_sections_first;
      while (g_sec_tmp != NULL) {
        if (g_sec_tmp->id == x)
          break;
        g_sec_tmp = g_sec_tmp->next;
      }

      /* skip all dead sections */
      if (g_sec_tmp->alive == OFF)
        continue;

      if (c == 'A')
        g_sec_tmp->address = ind;

      ind = 0;
      if (g_sec_tmp->maxsize_status == ON) {
        if (g_sec_tmp->maxsize < g_sec_tmp->size) {
          fprintf(stderr, "%s: INTERNAL_PASS_2: Section \"%s\" doesn't fit into the specified %d bytes. Enlarging to %d bytes.\n",
                  get_file_name(g_filename_id), g_sec_tmp->name, g_sec_tmp->maxsize, g_sec_tmp->size);
        }
        else if (g_sec_tmp->size < g_sec_tmp->maxsize) {
          g_sec_tmp->size = g_sec_tmp->maxsize;
          ind = 1;
        }
      }

      g_sec_tmp->data = calloc(sizeof(unsigned char) * g_sec_tmp->size, 1);
      if (g_sec_tmp->data == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Out of memory when trying to allocate room for section \"%s\".\n",
                get_file_name(g_filename_id), g_line_number, g_sec_tmp->name);
        return FAILED;
      }

      /* fill the padded area with _emptyfill_ */
      if (ind == 1)
        memset(g_sec_tmp->data, g_emptyfill, g_sec_tmp->size);

      if (strcmp(g_sec_tmp->name, "BANKHEADER") == 0)
        g_bankheader_status = ON;

      g_sec_tmp->i = 0;
      g_section_status = ON;

      continue;

      /* ENDS */

    case 's':
      g_section_status = OFF;
      g_bankheader_status = OFF;

      /* some sections don't affect the ORG outside of them */
      if (g_sec_tmp->advance_org == NO) {
        g_pc_bank = add_old;
        g_pc_full = g_bankaddress[g_rom_bank] + g_pc_bank;
        g_pc_slot = g_slots[g_slot].address + g_pc_bank;
        g_pc_slot_max = g_slots[g_slot].address + g_slots[g_slot].size;
      }

      continue;

      /* DSB & DSW & DSL */
      /* ('o' skips over memory without claiming it) */

    case 'x':
    case 'o':
      fscanf(g_file_out_ptr, "%d %d", &ind, &x);

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing DSB data", get_file_name(g_filename_id), g_line_number);

      if (ind < 0) { /* going backward */
        if (g_section_status == ON)
          g_sec_tmp->i += ind;
        g_pc_bank += ind;
        g_pc_full += ind;
        g_pc_slot += ind;
        ind++;
      }
      else {
        while (ind > 0) {
          if (c == 'o') {
            if (g_section_status == ON)
              g_sec_tmp->i++;
            g_pc_bank++;
            g_pc_full++;
            g_pc_slot++;
          }
          else {
            if (mem_insert(x) == FAILED)
              return FAILED;
          }
          ind--;
        }
      }
      continue;

    case 'X':
      fscanf(g_file_out_ptr, "%d %d", &ind, &inz);
      i = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing DSW data", get_file_name(g_filename_id), g_line_number);

      while (ind > 0) {
        if (g_little_endian == YES) {
          if (mem_insert(i) == FAILED)
            return FAILED;
          if (mem_insert(inz) == FAILED)
            return FAILED;
        }
        else {
          if (mem_insert(inz) == FAILED)
            return FAILED;
          if (mem_insert(i) == FAILED)
            return FAILED;
        }
        ind--;
      }

      continue;

    case 'h':
      fscanf(g_file_out_ptr, "%d %d", &ind, &inz);
      x = inz & 0xFF;
      i = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing DSL data", get_file_name(g_filename_id), g_line_number);

      while (ind > 0) {
        if (g_little_endian == YES) {
          if (mem_insert(x) == FAILED)
            return FAILED;
          if (mem_insert(i) == FAILED)
            return FAILED;
          if (mem_insert(inz) == FAILED)
            return FAILED;
        }
        else {
          if (mem_insert(inz) == FAILED)
            return FAILED;
          if (mem_insert(i) == FAILED)
            return FAILED;
          if (mem_insert(x) == FAILED)
            return FAILED;
        }
        ind--;
      }

      continue;

    case 'w':
      fscanf(g_file_out_ptr, "%d %d", &ind, &inz);
      x = inz & 0xFF;
      i = (inz >> 8) & 0xFF;
      q = (inz >> 16) & 0xFF;
      inz = (inz >> 24) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing DSD data", get_file_name(g_filename_id), g_line_number);

      while (ind > 0) {
        if (g_little_endian == YES) {
          if (mem_insert(x) == FAILED)
            return FAILED;
          if (mem_insert(i) == FAILED)
            return FAILED;
          if (mem_insert(q) == FAILED)
            return FAILED;
          if (mem_insert(inz) == FAILED)
            return FAILED;
        }
        else {
          if (mem_insert(inz) == FAILED)
            return FAILED;
          if (mem_insert(q) == FAILED)
            return FAILED;
          if (mem_insert(i) == FAILED)
            return FAILED;
          if (mem_insert(x) == FAILED)
            return FAILED;
        }
        ind--;
      }

      continue;

      /* DATA & OPTCODE */

    case 'd':
      fscanf(g_file_out_ptr, "%d", &x);

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a byte", get_file_name(g_filename_id), g_line_number);
        
      if (mem_insert(x) == FAILED)
        return FAILED;

      continue;

    case 'y':
      fscanf(g_file_out_ptr, "%d", &inz);
      x = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing two bytes", get_file_name(g_filename_id), g_line_number);

      if (g_little_endian == YES) {
        if (mem_insert(x) == FAILED)
          return FAILED;
        if (mem_insert(inz) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert(inz) == FAILED)
          return FAILED;
        if (mem_insert(x) == FAILED)
          return FAILED;
      }
        
      continue;

    case 'b':
      fscanf(g_file_out_ptr, "%d", &g_base);
      continue;

    case 'z':
      fscanf(g_file_out_ptr, "%d", &inz);
      x = inz & 0xFF;
      ind = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing three bytes", get_file_name(g_filename_id), g_line_number);

      if (g_little_endian == YES) {
        if (mem_insert(x) == FAILED)
          return FAILED;
        if (mem_insert(ind) == FAILED)
          return FAILED;
        if (mem_insert(inz) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert(inz) == FAILED)
          return FAILED;
        if (mem_insert(ind) == FAILED)
          return FAILED;
        if (mem_insert(x) == FAILED)
          return FAILED;
      }

      continue;

    case 'u':
      fscanf(g_file_out_ptr, "%d", &inz);
      x = inz & 0xFF;
      q = (inz >> 8) & 0xFF;
      ind = (inz >> 16) & 0xFF;
      inz = (inz >> 24) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing four bytes", get_file_name(g_filename_id), g_line_number);

      if (g_little_endian == YES) {
        if (mem_insert(x) == FAILED)
          return FAILED;
        if (mem_insert(q) == FAILED)
          return FAILED;
        if (mem_insert(ind) == FAILED)
          return FAILED;
        if (mem_insert(inz) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert(inz) == FAILED)
          return FAILED;
        if (mem_insert(ind) == FAILED)
          return FAILED;
        if (mem_insert(q) == FAILED)
          return FAILED;
        if (mem_insert(x) == FAILED)
          return FAILED;
      }

      continue;

      /* BITS */

    case '+':
      {
        int bits_to_define;
        char type;
        
        fscanf(g_file_out_ptr, "%d ", &bits_to_define);

        if (bits_to_define == 999) {
          /* add the last byte if there is any data in it */

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(g_filename_id), g_line_number);

          if (bits_position > 0) {
            if (mem_insert(bits_byte) == FAILED)
              return FAILED;
          }

          bits_byte = 0;
          bits_position = 0;

          continue;
        }

        fscanf(g_file_out_ptr, "%c", &type);

        if (type == 'a') {
          int data;
          
          fscanf(g_file_out_ptr, "%d", &data);

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(g_filename_id), g_line_number);

          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits = 0;
            
            if (bits_to_define_this_byte > bits_to_define)
              bits_to_define_this_byte = bits_to_define;

            for (bits = 0; bits < bits_to_define_this_byte; bits++)
              _bits_add_bit(&bits_byte, &bits_position, &bits_to_define, data);

            if (bits_position == 8) {
              bits_position = 0;
              if (mem_insert(bits_byte) == FAILED)
                return FAILED;
              bits_byte = 0;
            }
          }
        }
        else if (type == 'b') {
          struct label_def *label;
          
          fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

          if (g_namespace[0] != 0) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
                return FAILED;
            }
          }

          x = 0;
          hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
          if (g_tmp_def != NULL) {
            if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
              return FAILED;
            }
            else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
              o = (int)g_tmp_def->value;
              x = 1;

              /* create a what-we-are-doing message for mem_insert*() warnings/errors */
              snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(g_filename_id), g_line_number);

              while (bits_to_define > 0) {
                int bits_to_define_this_byte = 8 - bits_position;
                int bits = 0;
            
                if (bits_to_define_this_byte > bits_to_define)
                  bits_to_define_this_byte = bits_to_define;

                for (bits = 0; bits < bits_to_define_this_byte; bits++)
                  _bits_add_bit(&bits_byte, &bits_position, &bits_to_define, o);

                if (bits_position == 8) {
                  bits_position = 0;
                  if (mem_insert(bits_byte) == FAILED)
                    return FAILED;
                  bits_byte = 0;
                }
              }
            }
          }

          if (x == 1)
            continue;

          label = new_unknown_reference(REFERENCE_TYPE_BITS);
          if (label == NULL)
            return FAILED;

          label->bits_position = bits_position;
          label->bits_to_define = bits_to_define;
          
          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a .BITS reference", get_file_name(g_filename_id), g_line_number);

          /* add zeroes */
          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits = 0;
            
            if (bits_to_define_this_byte > bits_to_define)
              bits_to_define_this_byte = bits_to_define;

            for (bits = 0; bits < bits_to_define_this_byte; bits++)
              _bits_add_bit(&bits_byte, &bits_position, &bits_to_define, 0);

            if (bits_position == 8) {
              bits_position = 0;
              if (mem_insert(bits_byte) == FAILED)
                return FAILED;
              bits_byte = 0;
            }
          }

          continue;
        }
        else if (type == 'c') {
          fscanf(g_file_out_ptr, "%d", &inz);

          if (g_bankheader_status == OFF)
            g_stacks_tmp = g_stacks_first;
          else
            g_stacks_tmp = g_stacks_header_first;

          while (g_stacks_tmp != NULL) {
            if (g_stacks_tmp->id == inz)
              break;
            g_stacks_tmp = g_stacks_tmp->next;
          }

          if (g_stacks_tmp == NULL) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
            return FAILED;
          }

          if (g_stacks_tmp->section_status == ON) {
            /* relative address, to the beginning of the section */
            g_stacks_tmp->address = g_sec_tmp->i;
          }
          else {
            /* complete address, in ROM memory */
            g_stacks_tmp->address = g_pc_bank;
          }

          g_stacks_tmp->bank = g_rom_bank;
          g_stacks_tmp->slot = g_slot;
          g_stacks_tmp->type = STACK_TYPE_BITS;
          g_stacks_tmp->base = g_base;
          g_stacks_tmp->bits_position = bits_position;
          g_stacks_tmp->bits_to_define = bits_to_define;

          if (mangle_stack_references(g_stacks_tmp) == FAILED)
            return FAILED;

          if (g_namespace[0] != 0) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
                return FAILED;
            }
          }
        
          /* this stack was referred from the code */
          g_stacks_tmp->position = STACK_POSITION_CODE;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for .BITS' bits", get_file_name(g_filename_id), g_line_number);

          /* add zeroes */
          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits = 0;
            
            if (bits_to_define_this_byte > bits_to_define)
              bits_to_define_this_byte = bits_to_define;

            for (bits = 0; bits < bits_to_define_this_byte; bits++)
              _bits_add_bit(&bits_byte, &bits_position, &bits_to_define, 0);

            if (bits_position == 8) {
              bits_position = 0;
              if (mem_insert(bits_byte) == FAILED)
                return FAILED;
              bits_byte = 0;
            }
          }
        }

        continue;
      }
      
      /* DATA BLOCK from .INCBIN */

    case 'D':
      fscanf(g_file_out_ptr, "%d %d %d %d", &x, &inz, &z, &y);

      g_ifd_tmp = g_incbin_file_data_first;
      for (ind = 0; ind != x; ind++)
        g_ifd_tmp = g_ifd_tmp->next;
      t = g_ifd_tmp->data + z;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .INCBIN data", get_file_name(g_filename_id), g_line_number);

      /* swap? */
      if (inz == 1) {
        inz = y / 2;
        for (ind = 0; ind < inz; ind++) {
          if (mem_insert(*(t + 1)) == FAILED)
            return FAILED;
          if (mem_insert(*t) == FAILED)
            return FAILED;
          t += 2;
        }
      }
      else {
        inz = y;
        for (ind = 0; ind < inz; ind++) {
          if (mem_insert(*(t++)) == FAILED)
            return FAILED;
        }
      }

      continue;

      /* ORIGIN & ROM BANK */

    case 'O':
    case 'B':
      if (c == 'O')
        fscanf(g_file_out_ptr, "%d", &g_pc_bank);
      else {
        fscanf(g_file_out_ptr, "%d %d", &g_rom_bank, &g_slot);
        if (g_banksize_defined == 0)
          g_banksize = g_banks[g_rom_bank];
      }

      g_pc_full = g_bankaddress[g_rom_bank] + g_pc_bank;
      g_pc_slot = g_slots[g_slot].address + g_pc_bank;
      g_pc_slot_max = g_slots[g_slot].address + g_slots[g_slot].size;

      continue;

      /* BREAKPOINT */

    case 'Z':
      continue;

      /* SYMBOL */

    case 'Y':
      /* skip the symbol */
      fscanf(g_file_out_ptr, "%*s");
      continue;

      /* LABEL */
    case 'L':
      {
        struct label_def *l;
        int m, n = 0, mangled_label = NO;

        fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

        if (is_label_anonymous(g_tmp) == NO) {
          while (n < 10 && g_tmp[n] == '@')
            n++;

          m = n + 1;
          while (m < 10)
            g_parent_labels[m++] = NULL;

          m = n;
          n--;
          while (n >= 0 && g_parent_labels[n] == NULL)
            n--;

          if (n >= 0) {
            if (mangle_label(g_tmp, g_parent_labels[n]->label, n, MAX_NAME_LENGTH) == FAILED)
              return FAILED;
            mangled_label = YES;
          }

          if (g_namespace[0] != 0 && mangled_label == NO) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (add_namespace(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
                return FAILED;
            }
          }
            
          if (m < 10 && find_label(g_tmp, g_sec_tmp, (void*)&l) == SUCCEEDED)
            g_parent_labels[m] = l;
        }
      }
      continue;

      /* 8BIT (ALSO 9BIT SHORT) COMPUTATION */

    case '-':
    case 'c':
      fscanf(g_file_out_ptr, "%d", &inz);

      if (g_bankheader_status == OFF)
        g_stacks_tmp = g_stacks_first;
      else
        g_stacks_tmp = g_stacks_header_first;

      while (g_stacks_tmp != NULL) {
        if (g_stacks_tmp->id == inz)
          break;
        g_stacks_tmp = g_stacks_tmp->next;
      }

      if (g_stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
        return FAILED;
      }

      if (g_stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        g_stacks_tmp->address = g_sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        g_stacks_tmp->address = g_pc_bank;
      }

      g_stacks_tmp->bank = g_rom_bank;
      g_stacks_tmp->slot = g_slot;
      if (c == '-')
        g_stacks_tmp->type = STACK_TYPE_9BIT_SHORT;
      else
        g_stacks_tmp->type = STACK_TYPE_8BIT;
      g_stacks_tmp->base = g_base;
      g_stacks_tmp->special_id = g_special_id;
        
      if (mangle_stack_references(g_stacks_tmp) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
            return FAILED;
        }
      }

      /* this stack was referred from the code */
      g_stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit computation", get_file_name(g_filename_id), g_line_number);
        
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 16BIT COMPUTATION */

    case 'C':
      fscanf(g_file_out_ptr, "%d", &inz);

      if (g_bankheader_status == OFF)
        g_stacks_tmp = g_stacks_first;
      else
        g_stacks_tmp = g_stacks_header_first;

      while (g_stacks_tmp != NULL) {
        if (g_stacks_tmp->id == inz)
          break;
        g_stacks_tmp = g_stacks_tmp->next;
      }

      if (g_stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
        return FAILED;
      }

      if (g_stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        g_stacks_tmp->address = g_sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        g_stacks_tmp->address = g_pc_bank;
      }

      g_stacks_tmp->bank = g_rom_bank;
      g_stacks_tmp->slot = g_slot;
      g_stacks_tmp->type = STACK_TYPE_16BIT;
      g_stacks_tmp->base = g_base;
        
      if (mangle_stack_references(g_stacks_tmp) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      g_stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit computation", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

#ifdef SPC700
      /* 13BIT COMPUTATION */

    case 'N':
      fscanf(g_file_out_ptr, "%d %d", &x, &inz);

      if (g_bankheader_status == OFF)
        g_stacks_tmp = g_stacks_first;
      else
        g_stacks_tmp = g_stacks_header_first;

      while (g_stacks_tmp != NULL) {
        if (g_stacks_tmp->id == inz)
          break;
        g_stacks_tmp = g_stacks_tmp->next;
      }

      if (g_stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
        return FAILED;
      }

      if (g_stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        g_stacks_tmp->address = g_sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        g_stacks_tmp->address = g_pc_bank;
      }

      g_stacks_tmp->bank = g_rom_bank;
      g_stacks_tmp->slot = g_slot;
      g_stacks_tmp->type = STACK_TYPE_13BIT;
      g_stacks_tmp->base = g_base;
        
      if (mangle_stack_references(g_stacks_tmp) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      g_stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 13-bit computation", get_file_name(g_filename_id), g_line_number);

      if (mem_insert(0x00) == FAILED)
        return FAILED;
      if (mem_insert(x << (13 - 8)) == FAILED)
        return FAILED;

      continue;
#endif

      /* 24BIT COMPUTATION */

    case 'T':
      fscanf(g_file_out_ptr, "%d", &inz);

      if (g_bankheader_status == OFF)
        g_stacks_tmp = g_stacks_first;
      else
        g_stacks_tmp = g_stacks_header_first;

      while (g_stacks_tmp != NULL) {
        if (g_stacks_tmp->id == inz)
          break;
        g_stacks_tmp = g_stacks_tmp->next;
      }

      if (g_stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
        return FAILED;
      }

      if (g_stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        g_stacks_tmp->address = g_sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        g_stacks_tmp->address = g_pc_bank;
      }

      g_stacks_tmp->bank = g_rom_bank;
      g_stacks_tmp->slot = g_slot;
      g_stacks_tmp->type = STACK_TYPE_24BIT;
      g_stacks_tmp->base = g_base;

      if (mangle_stack_references(g_stacks_tmp) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      g_stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 24-bit computation", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 32BIT COMPUTATION */

    case 'U':
      fscanf(g_file_out_ptr, "%d", &inz);

      if (g_bankheader_status == OFF)
        g_stacks_tmp = g_stacks_first;
      else
        g_stacks_tmp = g_stacks_header_first;

      while (g_stacks_tmp != NULL) {
        if (g_stacks_tmp->id == inz)
          break;
        g_stacks_tmp = g_stacks_tmp->next;
      }

      if (g_stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(g_filename_id), g_line_number, inz);
        return FAILED;
      }

      if (g_stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        g_stacks_tmp->address = g_sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        g_stacks_tmp->address = g_pc_bank;
      }

      g_stacks_tmp->bank = g_rom_bank;
      g_stacks_tmp->slot = g_slot;
      g_stacks_tmp->type = STACK_TYPE_32BIT;
      g_stacks_tmp->base = g_base;

      if (mangle_stack_references(g_stacks_tmp) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(g_stacks_tmp, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      g_stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 32-bit computation", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 24BIT REFERENCE */

    case 'q':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }
          
      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 24-bit reference", get_file_name(g_filename_id), g_line_number);

          if (g_little_endian == YES) {
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 16) & 0xFF) == FAILED)
              return FAILED;
          }
          else {
            if (mem_insert((o >> 16) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
          }
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_24BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 24-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 32BIT REFERENCE */

    case 'V':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }
          
      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 32-bit reference", get_file_name(g_filename_id), g_line_number);

          if (g_little_endian == YES) {
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 16) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 24) & 0xFF) == FAILED)
              return FAILED;
          }
          else {
            if (mem_insert((o >> 24) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 16) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
          }
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_32BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 32-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      
      continue;

      /* 16BIT PC RELATIVE REFERENCE */

    case 'M':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }
                
      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(g_filename_id), g_line_number);

          if (g_little_endian == YES) {
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
          }
          else {
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
          }
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_RELATIVE_16BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 16BIT REFERENCE */

    case 'r':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(g_filename_id), g_line_number);

          if (g_little_endian == YES) {
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert((o & 0xFF00) >> 8) == FAILED)
              return FAILED;
          }
          else {
            if (mem_insert((o >> 8) & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
          }
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_16BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

#ifdef SPC700
      /* 13BIT REFERENCE */

    case 'n':
      fscanf(g_file_out_ptr, "%d ", &inz);
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;
          if (o > 8191 || o < 0) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference value of \"%s\" is out of 13-bit range.\n", get_file_name(g_filename_id), g_line_number, g_tmp);
            return FAILED;
          }

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 13-bit reference", get_file_name(g_filename_id), g_line_number);

          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
          if (mem_insert(((o | inz << 13) & 0xFF00) >> 8) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_13BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 13-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert(0x00) == FAILED)
        return FAILED;
      if (mem_insert(inz << (13 - 8)) == FAILED)
        return FAILED;

      continue;
#endif

      /* 8BIT PC RELATIVE REFERENCE */

    case 'R':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(g_filename_id), g_line_number);

          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_RELATIVE_8BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 8BIT (9BIT SHORT ALSO) REFERENCE */

    case '*':
    case 'Q':
      fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(g_tmp, g_namespace, sizeof(g_tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(g_defines_map, g_tmp, (void*)&g_tmp_def);
      if (g_tmp_def != NULL) {
        if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(g_filename_id), g_line_number, g_tmp);
          return FAILED;
        }
        else if (g_tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)g_tmp_def->value;
          x = 1;

          /* 9-bit short? */
          if (c == '*') {
            if ((c & 1) == 1) {
              fprintf(stderr, "%s:%d: INTERNAL_PASS_2: The RAM address must be even.\n", get_file_name(g_filename_id), g_line_number);
              return FAILED;
            }
            o = o >> 1;
          }

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(g_filename_id), g_line_number);
            
          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (c == '*') {
        if (new_unknown_reference(REFERENCE_TYPE_DIRECT_9BIT_SHORT) == NULL)
          return FAILED;
      }
      else {
        if (new_unknown_reference(REFERENCE_TYPE_DIRECT_8BIT) == NULL)
          return FAILED;
      }

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(g_filename_id), g_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* .DSTRUCT stuff */

    case 'e':
      fscanf(g_file_out_ptr, "%d %d ", &x, &y);
      if (y == -1) {
        /* mark start of .DSTRUCT */
        g_dstruct_start = g_pc_full;
        /* make sure all data in a section gets set to emptyfill */
        if (g_section_status == ON)
          memset(g_sec_tmp->data + g_sec_tmp->i, g_emptyfill, x);
      }
      else if (y == -2) {
        /* end of .DSTRUCT. make sure all memory is claimed. */
        while (g_pc_full < g_dstruct_start + x) {
          if (g_section_status == OFF && g_rom_banks_usage_table[g_pc_full] == 0) {
            g_rom_banks_usage_table[g_pc_full] = 2;
            g_rom_banks[g_pc_full] = g_emptyfill;
          }
          if (g_section_status == ON)
            g_sec_tmp->i++;
          g_pc_bank++;
          g_pc_slot++;
          g_pc_full++;
        }
      }
      else {
        /* seek offset relative to dstruct start */
        if (g_section_status == ON)
          g_sec_tmp->i = g_sec_tmp->i + (g_dstruct_start - g_pc_full) + x;
        g_pc_bank = g_pc_bank + (g_dstruct_start - g_pc_full) + x;
        g_pc_slot = g_pc_slot + (g_dstruct_start - g_pc_full) + x;
        g_pc_full = g_dstruct_start + x;
      }
      continue;
    }
  }

  fclose(g_file_out_ptr);
  g_file_out_ptr = NULL;

  /* library file output */
  if (g_output_format == OUTPUT_LIBRARY && g_test_mode == OFF) {
    if ((final_ptr = fopen(g_final_name, "wb")) == NULL) {
      fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\" for writing.\n", g_final_name);
      return FAILED;
    }

    /* header */
    fprintf(final_ptr, "WLAC");

    /* misc bits */
    ind = 0;

    if (g_little_endian == NO)
      ind |= 1 << 0;
#if defined(W65816)
    /* 65816 bit */
    ind |= 1 << 1;
#endif
#ifdef CSG65CE02
    /* 65ce02 bit */
    ind |= 1 << 2;
#endif
    
    fprintf(final_ptr, "%c", ind);
    
    if (export_source_file_names(final_ptr) == FAILED)
      return FAILED;

    /* export definitions */
    if (export_definitions(final_ptr) == FAILED)
      return FAILED;

    /* labels, symbols and breakpoints */
    ov = 0;
    g_label_tmp = g_labels;
    while (g_label_tmp != NULL) {
      if (g_label_tmp->alive == ON)
        ov++;
      g_label_tmp = g_label_tmp->next;
    }

    WRITEOUT_OV;

    g_label_tmp = g_labels;
    while (g_label_tmp != NULL) {
      if (g_label_tmp->alive == ON) {
        if (g_label_tmp->symbol != 2)
          fprintf(final_ptr, "%s", g_label_tmp->label);
        fprintf(final_ptr, "%c", g_label_tmp->symbol);

        ov = g_label_tmp->section_id;
        WRITEOUT_OV;

        ov = g_label_tmp->filename_id;
        WRITEOUT_OV;

        ov = g_label_tmp->linenumber;
        WRITEOUT_OV;

        ov = g_label_tmp->address;
        WRITEOUT_OV;
      }
      g_label_tmp = g_label_tmp->next;
    }

    /* unknown labels (outside references) */
    ov = 0;
    g_label_tmp = g_unknown_labels;
    while (g_label_tmp != NULL) {
      ov++;
      g_label_tmp = g_label_tmp->next;
    }

    WRITEOUT_OV;

    g_label_tmp = g_unknown_labels;
    while (g_label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c", g_label_tmp->label, 0x0, g_label_tmp->type, g_label_tmp->special_id);

      ov = g_label_tmp->section_id;
      WRITEOUT_OV;

      ov = g_label_tmp->filename_id;
      WRITEOUT_OV;

      if (g_label_tmp->section_status == OFF) {
        fprintf(stderr, "INTERNAL_PASS_2: Label \"%s\" is outside all sections.\n", g_label_tmp->label);
        return FAILED;
      }

      if (g_label_tmp->type == REFERENCE_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_label_tmp->bits_position, g_label_tmp->bits_to_define);
      
      ov = g_label_tmp->linenumber;
      WRITEOUT_OV;

      ov = g_label_tmp->address; /* + (g_label_tmp->base << 16); */
      WRITEOUT_OV;

      g_label_tmp = g_label_tmp->next;
    }

    /* pending calculations */
    ov = g_stacks_outside;
    WRITEOUT_OV;

    g_stacks_tmp = g_stacks_first;
    while (g_stacks_tmp != NULL) {
      ov = g_stacks_tmp->id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", g_stacks_tmp->type | (g_stacks_tmp->relative_references << 7), g_stacks_tmp->special_id);

      ov = g_stacks_tmp->section_id;
      WRITEOUT_OV;

      ov = g_stacks_tmp->filename_id;
      WRITEOUT_OV;
      
      fprintf(final_ptr, "%c%c", g_stacks_tmp->stacksize, g_stacks_tmp->position);

      if (g_stacks_tmp->type == STACK_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_stacks_tmp->bits_position, g_stacks_tmp->bits_to_define);

      ov = g_stacks_tmp->address;
      WRITEOUT_OV;

      ov = g_stacks_tmp->linenumber;
      WRITEOUT_OV;

      for (ind = 0; ind < g_stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", g_stacks_tmp->stack[ind].type, g_stacks_tmp->stack[ind].sign);
        if (g_stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", g_stacks_tmp->stack[ind].string, 0);
        else {
          dou = g_stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      g_stacks_tmp = g_stacks_tmp->next;
    }

    /* label sizeofs */
    ov = 0;
    g_label_sizeof_tmp = g_label_sizeofs;
    while (g_label_sizeof_tmp != NULL) {
      ov++;
      g_label_sizeof_tmp = g_label_sizeof_tmp->next;
    }

    WRITEOUT_OV;

    g_label_sizeof_tmp = g_label_sizeofs;
    while (g_label_sizeof_tmp != NULL) {
      fprintf(final_ptr, "%s%c", g_label_sizeof_tmp->name, 0);

      ov = g_label_sizeof_tmp->size;
      WRITEOUT_OV;

      g_label_sizeof_tmp = g_label_sizeof_tmp->next;
    }
    
    /* append sections */
    ov = 0;
    g_append_tmp = g_append_sections;
    while (g_append_tmp != NULL) {
      ov++;
      g_append_tmp = g_append_tmp->next;
    }
    WRITEOUT_OV;

    g_append_tmp = g_append_sections;
    while (g_append_tmp != NULL) {
      fprintf(final_ptr, "%s%c", g_append_tmp->section, 0);
      fprintf(final_ptr, "%s%c", g_append_tmp->append_to, 0);
      g_append_tmp = g_append_tmp->next;
    }

    /* sections */
    g_sec_tmp = g_sections_first;
    while (g_sec_tmp != NULL) {
      if (g_sec_tmp->alive == ON) {
        fprintf(final_ptr, "%s%c%c", g_sec_tmp->name, g_sec_tmp->status, g_sec_tmp->keep);
        if (g_sec_tmp->nspace == NULL)
          fprintf(final_ptr, "%c", 0);
        else
          fprintf(final_ptr, "%s%c", g_sec_tmp->nspace->name, 0);

        ov = g_sec_tmp->id;
        WRITEOUT_OV;

        ov = g_sec_tmp->filename_id;
        WRITEOUT_OV;

        ov = g_sec_tmp->size;
        WRITEOUT_OV;
        ov = g_sec_tmp->alignment;
        WRITEOUT_OV;
        ov = g_sec_tmp->offset;
        WRITEOUT_OV;
        ov = g_sec_tmp->priority;
        WRITEOUT_OV;
        
        fwrite(g_sec_tmp->data, 1, g_sec_tmp->size, final_ptr);

        if (g_listfile_data == YES && g_sec_tmp->listfile_items > 0)
          listfile_block_write(final_ptr, g_sec_tmp);
        else
          fprintf(final_ptr, "%c", 0);
      }
      g_sec_tmp = g_sec_tmp->next;
    }

    fclose(final_ptr);
  }

  /* object file output */
  else if (g_output_format == OUTPUT_OBJECT && g_test_mode == OFF) {
    if ((final_ptr = fopen(g_final_name, "wb")) == NULL) {
      fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\" for writing.\n", g_final_name);
      return FAILED;
    }

    /* header */
    fprintf(final_ptr, "WLAd%c", g_emptyfill);

    /* misc bits */
    ind = 0;

#ifdef Z80
    if (g_computesmschecksum_defined != 0)
      ind |= 1 << 0;
#endif

#if defined(W65816)
    if (g_snes_mode != 0) {
      if (g_hirom_defined != 0)
        ind += 2;
      if (g_fastrom_defined != 0)
        ind += 8;
      if (g_computesneschecksum_defined != 0)
        ind += 64;
      /* use snes banking scheme */
      if (g_hirom_defined != 0 || g_lorom_defined != 0 || g_exhirom_defined != 0 || g_exlorom_defined != 0)
        ind += 4;
    }
#endif

#if defined(W65816)
    /* 65816 bit */
    ind += 128;
#endif

#ifdef GB
    if (g_computechecksum_defined != 0)
      ind += 16;
    if (g_computecomplementcheck_defined != 0)
      ind += 32;
#endif

    fprintf(final_ptr, "%c", ind);

    /* more bits */
    ind = 0;

    if (g_smc_defined != 0)
      ind |= 1 << 0;
#if defined(W65816)
    if (g_sramsize_defined != 0)
      ind |= (g_sramsize & 3) << 1;
#endif

#ifdef Z80
    if (g_smstag_defined != 0)
      ind |= 1 << 3;
    if (g_smsheader_defined != 0)
      ind |= 1 << 4;
#endif

#if defined(W65816)
    if (g_snes_mode != 0) {
      if (g_exhirom_defined != 0)
        ind |= 1 << 5;
      if (g_exlorom_defined != 0)
        ind |= 1 << 6;
    }
#endif

    if (g_little_endian == NO)
      ind |= 1 << 7;
    
    fprintf(final_ptr, "%c", ind);

    /* extr bits */
    ind = 0;

#ifdef CSG65CE02
    /* 65ce02 bit */
    ind |= 1 << 0;
#endif

    fprintf(final_ptr, "%c", ind);
    
    /* rom bank map */
    ov = g_rombanks;
    WRITEOUT_OV;                                   /* number of rom banks */

    if (g_rombankmap_defined != 0) {
      fprintf(final_ptr, "%c", 1);                 /* status */
      for (i = 0; i < g_rombanks; i++) {
        ov = g_banks[i];
        WRITEOUT_OV;                               /* banksize */
      }
    }
    else {
      fprintf(final_ptr, "%c", 0);                 /* status */
      ov = g_banksize;
      WRITEOUT_OV;                                 /* banksize */
    }

    /* memory map */
    fprintf(final_ptr, "%c", g_slots_amount);

    for (i = 0; i < g_slots_amount; i++) {
      if (g_slots[i].size != 0) {
        ov = g_slots[i].address;
        WRITEOUT_OV;
        ov = g_slots[i].size;
        WRITEOUT_OV;
        if (g_slots[i].name[0] == 0x0)
          fprintf(final_ptr, "%c", 0x0);
        else
          fprintf(final_ptr, "%s%c", g_slots[i].name, 0x0);
      }
    }

    /* source file names */
    if (export_source_file_names(final_ptr) == FAILED)
      return FAILED;

    /* export definitions */
    if (export_definitions(final_ptr) == FAILED)
      return FAILED;

    /* labels, symbols and breakpoints */
    ov = 0;
    g_label_tmp = g_labels;
    while (g_label_tmp != NULL) {
      if (g_label_tmp->alive == ON)
        ov++;
      g_label_tmp = g_label_tmp->next;
    }

    WRITEOUT_OV;

    g_label_tmp = g_labels;
    while (g_label_tmp != NULL) {
      if (g_label_tmp->alive == ON) {
        if (g_label_tmp->symbol != 2)
          fprintf(final_ptr, "%s", g_label_tmp->label);
        fprintf(final_ptr, "%c", g_label_tmp->symbol);

        fprintf(final_ptr, "%c", g_label_tmp->slot);

        ov = g_label_tmp->filename_id;
        WRITEOUT_OV;
        
        ov = g_label_tmp->section_id;
        WRITEOUT_OV;

        /* DEBUG
           fprintf(stderr, "LABEL: \"%s\" SLOT: %d LINE: %d\n", g_label_tmp->label, g_label_tmp->slot, g_label_tmp->linenumber);
        */

        ov = g_label_tmp->address;
        WRITEOUT_OV;

        ov = g_label_tmp->linenumber;
        WRITEOUT_OV;

        ov = g_label_tmp->bank;
        WRITEOUT_OV;

        ov = g_label_tmp->base;
        WRITEOUT_OV;
      }
      g_label_tmp = g_label_tmp->next;
    }

    /* outside references */
    ov = 0;
    g_label_tmp = g_unknown_labels;
    while (g_label_tmp != NULL) {
      ov++;
      g_label_tmp = g_label_tmp->next;
    }
    g_label_tmp = g_unknown_header_labels;
    while (g_label_tmp != NULL) {
      ov++;
      g_label_tmp = g_label_tmp->next;
    }

    WRITEOUT_OV;

    g_label_tmp = g_unknown_labels;
    while (g_label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c", g_label_tmp->label, 0x0, g_label_tmp->type, g_label_tmp->special_id);

      ov = g_label_tmp->filename_id;
      WRITEOUT_OV;

      if (g_label_tmp->type == REFERENCE_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_label_tmp->bits_position, g_label_tmp->bits_to_define);

      fprintf(final_ptr, "%c", g_label_tmp->slot);
      
      ov = g_label_tmp->section_id;
      WRITEOUT_OV;

      ov = g_label_tmp->linenumber;
      WRITEOUT_OV;

      ov = g_label_tmp->address;
      WRITEOUT_OV;

      ov = g_label_tmp->bank;
      WRITEOUT_OV;

      ov = g_label_tmp->base;
      WRITEOUT_OV;

      g_label_tmp = g_label_tmp->next;
    }

    g_label_tmp = g_unknown_header_labels;
    while (g_label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c", g_label_tmp->label, 0x0, g_label_tmp->type, 0);

      ov = g_label_tmp->filename_id;
      WRITEOUT_OV;

      if (g_label_tmp->type == REFERENCE_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_label_tmp->bits_position, g_label_tmp->bits_to_define);

      fprintf(final_ptr, "%c", g_label_tmp->slot);

      ov = g_label_tmp->section_id;
      WRITEOUT_OV;

      ov = g_label_tmp->linenumber;
      WRITEOUT_OV;

      ov = g_label_tmp->address;
      WRITEOUT_OV;

      ov = g_label_tmp->bank;
      WRITEOUT_OV;

      ov = g_label_tmp->base;
      WRITEOUT_OV;
      
      g_label_tmp = g_label_tmp->next;
    }

    /* pending calculations */
    ov = g_stacks_outside + g_stacks_inside;
    WRITEOUT_OV;

    g_stacks_tmp = g_stacks_first;
    while (g_stacks_tmp != NULL) {
      ov = g_stacks_tmp->id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", g_stacks_tmp->type | (g_stacks_tmp->relative_references << 7), g_stacks_tmp->special_id);

      ov = g_stacks_tmp->section_id;
      WRITEOUT_OV;

      ov = g_stacks_tmp->filename_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", g_stacks_tmp->stacksize, g_stacks_tmp->position);

      if (g_stacks_tmp->type == STACK_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_stacks_tmp->bits_position, g_stacks_tmp->bits_to_define);

      fprintf(final_ptr, "%c", g_stacks_tmp->slot);
      
      ov = g_stacks_tmp->address;
      WRITEOUT_OV;

      ov = g_stacks_tmp->linenumber;
      WRITEOUT_OV;

      ov = g_stacks_tmp->bank;
      WRITEOUT_OV;

      ov = g_stacks_tmp->base;
      WRITEOUT_OV;
      
      for (ind = 0; ind < g_stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", g_stacks_tmp->stack[ind].type, g_stacks_tmp->stack[ind].sign);
        if (g_stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", g_stacks_tmp->stack[ind].string, 0);
        else {
          dou = g_stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      g_stacks_tmp = g_stacks_tmp->next;
    }

    g_stacks_tmp = g_stacks_header_first;
    while (g_stacks_tmp != NULL) {
      ov = g_stacks_tmp->id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", g_stacks_tmp->type, g_stacks_tmp->special_id);

      ov = g_stacks_tmp->section_id;
      WRITEOUT_OV;

      ov = g_stacks_tmp->filename_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", g_stacks_tmp->stacksize, g_stacks_tmp->position);

      if (g_stacks_tmp->type == STACK_TYPE_BITS)
        fprintf(final_ptr, "%c%c", g_stacks_tmp->bits_position, g_stacks_tmp->bits_to_define);

      fprintf(final_ptr, "%c", g_stacks_tmp->slot);
      
      ov = g_stacks_tmp->address;
      WRITEOUT_OV;

      ov = g_stacks_tmp->linenumber;
      WRITEOUT_OV;

      ov = g_stacks_tmp->bank;
      WRITEOUT_OV;

      ov = g_stacks_tmp->base;
      WRITEOUT_OV;

      for (ind = 0; ind < g_stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", g_stacks_tmp->stack[ind].type, g_stacks_tmp->stack[ind].sign);
        if (g_stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", g_stacks_tmp->stack[ind].string, 0);
        else {
          dou = g_stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      g_stacks_tmp = g_stacks_tmp->next;
    }

    /* label sizeofs */
    ov = 0;
    g_label_sizeof_tmp = g_label_sizeofs;
    while (g_label_sizeof_tmp != NULL) {
      ov++;
      g_label_sizeof_tmp = g_label_sizeof_tmp->next;
    }

    WRITEOUT_OV;

    g_label_sizeof_tmp = g_label_sizeofs;
    while (g_label_sizeof_tmp != NULL) {
      fprintf(final_ptr, "%s%c", g_label_sizeof_tmp->name, 0);

      ov = g_label_sizeof_tmp->size;
      WRITEOUT_OV;

      g_label_sizeof_tmp = g_label_sizeof_tmp->next;
    }    
    
    /* append sections */
    ov = 0;
    g_append_tmp = g_append_sections;
    while (g_append_tmp != NULL) {
      ov++;
      g_append_tmp = g_append_tmp->next;
    }
    WRITEOUT_OV;

    g_append_tmp = g_append_sections;
    while (g_append_tmp != NULL) {
      fprintf(final_ptr, "%s%c", g_append_tmp->section, 0);
      fprintf(final_ptr, "%s%c", g_append_tmp->append_to, 0);
      g_append_tmp = g_append_tmp->next;
    }

    /* data area */
    ind = 0;
    for (inz = 0; inz < g_max_address; inz++) {
      if (g_rom_banks_usage_table[inz] != 0) {
        /* data block id */
        fprintf(final_ptr, "%c", 0x0);
        for (i = inz, ind = 0; inz < g_max_address; inz++, ind++) {
          if (g_rom_banks_usage_table[inz] == 0) {

            ov = i;
            WRITEOUT_OV;

            ov = ind;
            WRITEOUT_OV;

            fwrite(&g_rom_banks[i], 1, ind, final_ptr);

            ind = 0;
            break;
          }
        }
      }
    }

    if (ind != 0) {
      ov = i;
      WRITEOUT_OV;

      ov = ind;
      WRITEOUT_OV;

      fwrite(&g_rom_banks[i], 1, ind, final_ptr);
    }

    g_sec_tmp = g_sections_first;
    while (g_sec_tmp != NULL) {
      if (g_sec_tmp->alive == ON) {
        /* section block id */
        fprintf(final_ptr, "%c%s%c%c", 0x1, g_sec_tmp->name, g_sec_tmp->status, g_sec_tmp->keep);
        if (g_sec_tmp->nspace == NULL)
          fprintf(final_ptr, "%c", 0);
        else
          fprintf(final_ptr, "%s%c", g_sec_tmp->nspace->name, 0);

        ov = g_sec_tmp->id;
        WRITEOUT_OV;

        fprintf(final_ptr, "%c", g_sec_tmp->slot);

        ov = g_sec_tmp->filename_id;
        WRITEOUT_OV;

        ov = g_sec_tmp->address;
        WRITEOUT_OV;
        ov = g_sec_tmp->bank;
        WRITEOUT_OV;
        ov = g_sec_tmp->base;
        WRITEOUT_OV;
        ov = g_sec_tmp->size;
        WRITEOUT_OV;
        ov = g_sec_tmp->alignment;
        WRITEOUT_OV;
        ov = g_sec_tmp->offset;
        WRITEOUT_OV;
        ov = g_sec_tmp->priority;
        WRITEOUT_OV;

        fwrite(g_sec_tmp->data, 1, g_sec_tmp->size, final_ptr);

        if (g_listfile_data == YES && g_sec_tmp->listfile_items > 0)
          listfile_block_write(final_ptr, g_sec_tmp);
        else
          fprintf(final_ptr, "%c", 0);
      }
      g_sec_tmp = g_sec_tmp->next;
    }

    fclose(final_ptr);
  }

  /* output makefile rules */
  if (g_makefile_rules == YES) {
    fprintf(stdout, "%s: ", g_final_name);
    print_file_names();
  }

  /* show project information */
  if (g_verbose_mode == ON && g_output_format != OUTPUT_LIBRARY) {
    x = 0;
    inz = 1;
    for (ind = 0; ind < g_max_address; ind++) {
      if (g_rom_banks_usage_table[ind] == 0 && x == 0) {
        x = 1;
        inz = ind;
      }
      else if (g_rom_banks_usage_table[ind] != 0 && x == 1) {
        if (inz == (ind - 1))
          fprintf(stderr, "Free space at $%.4x.\n", inz);
        else
          fprintf(stderr, "Free space at $%.4x-$%.4x.\n", inz, ind - 1);
        x = 0;
      }
    }

    if (x == 1) {
      if (inz == (ind - 1))
        fprintf(stderr, "Free space at $%.4x.\n", inz);
      else
        fprintf(stderr, "Free space at $%.4x-$%.4x.\n", inz, ind - 1);
    }

    for (ind = 0, q = 0; ind < g_max_address; q++) {
      for (x = 0, inz = 0; inz < g_banks[q]; inz++) {
        if (g_rom_banks_usage_table[ind++] == 0)
          x++;
      }
      f = (((float)x)/g_banks[q]) * 100.0f;
      if (f == 100.0f)
        printf("Bank %.2d has %.5d bytes (%.1f%%) free.\n", q, x, f);
      else
        printf("Bank %.2d has %.5d bytes (%.2f%%) free.\n", q, x, f);
    }

    for (ind = 0, inz = 0; ind < g_max_address; ind++) {
      if (g_rom_banks_usage_table[ind] == 0)
        inz++;
    }
    fprintf(stderr, "%d unused bytes of total %d.\n", inz, g_max_address);

#ifndef GB
    g_sec_tmp = g_sections_first;
    while (g_sec_tmp != NULL) {
      if (g_sec_tmp->status == SECTION_STATUS_HEADER) {
        fprintf(stderr, "Bank %d header section size %d.\n", g_sec_tmp->bank, g_sec_tmp->size);
        ind += g_sec_tmp->size;
      }
      g_sec_tmp = g_sec_tmp->next;
    }

    if (ind != 0) {
      fprintf(stderr, "Total %d additional bytes (from headers and footers).\n", ind);
      fprintf(stderr, "Total size %d bytes.\n", ind + g_max_address);
    }
#endif

  }
  else if (g_verbose_mode == ON && g_output_format == OUTPUT_LIBRARY) {
    g_sec_tmp = g_sections_first;
    while (g_sec_tmp != NULL) {
      printf("Section \"%s\" size %d.\n", g_sec_tmp->name, g_sec_tmp->size);
      g_sec_tmp = g_sec_tmp->next;
    }
  }

  return SUCCEEDED;
}


int find_label(char *str, struct section_def *s, struct label_def **out) {

  char* str2;
  char* stripped;
  char prefix[MAX_NAME_LENGTH*2+2];
  struct label_def *l = NULL;
  int i;

  str2 = strchr(str, '.');
  i = (int)(str2-str);
  if (str2 == NULL) {
    stripped = str;
    prefix[0] = '\0';
  }
  else {
    stripped = str2+1;
    strncpy(prefix, str, i);
    prefix[i] = '\0';
  }

  *out = NULL;

  if (prefix[0] != '\0') {
    /* a namespace is specified (or at least there's a dot in the label) */
    struct namespace_def *nspace;

    if (hashmap_get(g_namespace_map, prefix, (void*)&nspace) == MAP_OK) {
      if (hashmap_get(nspace->label_map, stripped, (void*)&l) == MAP_OK) {
        *out = l;
        return SUCCEEDED;
      }
    }
  }
  if (s != NULL && s->nspace != NULL) {
    /* check the section's namespace */
    if (hashmap_get(s->nspace->label_map, str, (void*)&l) == MAP_OK) {
      *out = l;
      return SUCCEEDED;
    }
  }
  if (s != NULL) {
    /* check the section's labels. this is a bit redundant but it might have
       local labels (labels starting with an underscore) */
    if (hashmap_get(s->label_map, str, (void*)&l) == MAP_OK) {
      *out = l;
      return SUCCEEDED;
    }
  }
  /* check the global namespace */
  if (hashmap_get(g_global_unique_label_map, str, (void*)&l) == MAP_OK) {
    *out = l;
    return SUCCEEDED;
  }

  return FAILED;
}


int mem_insert(unsigned char x) {
  
  if (g_section_status == ON) {
    if (g_sec_tmp->i >= g_sec_tmp->size || g_sec_tmp->i < 0) {
      fprintf(stderr, "MEM_INSERT: Overflowed from section \"%s\"; Please send a bug report!\n", g_sec_tmp->name);
      return FAILED;
    }
    g_sec_tmp->data[g_sec_tmp->i] = x;
    g_sec_tmp->i++;
    g_pc_bank++;
    g_pc_full++;
    g_pc_slot++;
    return SUCCEEDED;
  }

  if (g_pc_bank >= g_banksize) {
    fprintf(stderr, "MEM_INSERT: Origin ($%x) overflows from bank (%d).\n", g_pc_bank, g_rom_bank);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (g_pc_full >= g_max_address) {
    fprintf(stderr, "MEM_INSERT: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", g_pc_full, g_max_address);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (g_pc_slot >= g_pc_slot_max) {
    fprintf(stderr, "MEM_INSERT: The current address ($%.4x) overflows from SLOT %d.\n", g_pc_slot, g_slot);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  if (g_rom_banks_usage_table[g_pc_full] != 0 && g_rom_banks[g_pc_full] != x && g_mem_insert_overwrite == OFF) {
    fprintf(stderr, "MEM_INSERT: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", g_rom_banks_usage_table[g_pc_full], g_pc_full, g_rom_banks[g_pc_full], x & 0xFF);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_banks_usage_table[g_pc_full] = 2;
  g_rom_banks[g_pc_full] = x;
  g_pc_bank++;
  g_pc_full++;
  g_pc_slot++;

  return SUCCEEDED;
}


int mem_insert_padding(void) {

  /* we'll use this function to reserve space for writes that take place later; e.g., we know that here will be
     written an 8-bit value, but we don't know the numeric value itself yet, but we'll need to reserve the
     space for the upcoming write or otherwise something else might get written here */

  if (g_section_status == ON) {
    g_sec_tmp->data[g_sec_tmp->i] = 0xCD;
    g_sec_tmp->i++;
    g_pc_bank++;
    g_pc_full++;
    g_pc_slot++;
    return SUCCEEDED;
  }

  if (g_pc_bank >= g_banksize) {
    fprintf(stderr, "MEM_INSERT_PADDING: Origin ($%x) overflows from bank (%d).\n", g_pc_bank, g_rom_bank);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (g_pc_full >= g_max_address) {
    fprintf(stderr, "MEM_INSERT_PADDING: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", g_pc_full, g_max_address);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (g_pc_slot >= g_pc_slot_max) {
    fprintf(stderr, "MEM_INSERT_PADDING: The current address ($%.4x) overflows from SLOT %d.\n", g_pc_slot, g_slot);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  /* announce the overwrite later */
  if (!(g_rom_banks_usage_table[g_pc_full] != 0 && g_mem_insert_overwrite == OFF))
    g_rom_banks_usage_table[g_pc_full] = 1;

  g_pc_bank++;
  g_pc_full++;
  g_pc_slot++;

  return SUCCEEDED;
}


int mem_insert_absolute(int add, unsigned char x) {

  if (add >= g_max_address) {
    fprintf(stderr, "MEM_INSERT_ABSOLUTE: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", add, g_max_address);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  if (g_rom_banks_usage_table[add] > 1 && g_rom_banks[add] != x && g_mem_insert_overwrite == OFF) {
    fprintf(stderr, "MEM_INSERT_ABSOLUTE: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", g_rom_banks_usage_table[add], add, g_rom_banks[add], x & 0xFF);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }
  
  g_rom_banks_usage_table[add]++;
  g_rom_banks[add] = x;

  return SUCCEEDED;
}


int export_definitions(FILE *final_ptr) {

  struct export_def *export_tmp;
  unsigned char *cp;
  double dou;
  int ov;

  ov = 0;
  export_tmp = g_export_first;
  while (export_tmp != NULL) {
    hashmap_get(g_defines_map, export_tmp->name, (void*)&g_tmp_def);
    if (g_tmp_def != NULL) {
      if (g_tmp_def->type == DEFINITION_TYPE_VALUE)
        ov++;
      if (g_tmp_def->type == DEFINITION_TYPE_STACK)
        ov++;
    }

    export_tmp = export_tmp->next;
  }

  WRITEOUT_OV;

  export_tmp = g_export_first;
  while (export_tmp != NULL) {
    hashmap_get(g_defines_map, export_tmp->name, (void*)&g_tmp_def);

    if (g_tmp_def == NULL)
      fprintf(stderr, "WARNING: Trying to export an unkonwn definition \"%s\".\n", export_tmp->name);
    else {
      if (g_tmp_def->type == DEFINITION_TYPE_VALUE) {
        fprintf(final_ptr, "%s%c", g_tmp_def->alias, 0x0);
        dou = g_tmp_def->value;
        WRITEOUT_DOU;
      }
      else if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
        fprintf(stderr, "INTERNAL_PASS_2: Definition \"%s\" is a string definition, and it cannot be exported.\n", export_tmp->name);
      }
      else if (g_tmp_def->type == DEFINITION_TYPE_STACK) {
        fprintf(final_ptr, "%s%c", g_tmp_def->alias, 0x1);
        dou = g_tmp_def->value;
        WRITEOUT_DOU;
      }
    }

    export_tmp = export_tmp->next;
  }

  return SUCCEEDED;
}


int export_source_file_names(FILE *final_ptr) {

  struct file_name_info *f;
  int ov;

  f = g_file_name_info_first;
  ov = 0;
  while (f != NULL) {
    ov++;
    f = f->next;
  }

  WRITEOUT_OV;

  f = g_file_name_info_first;
  while (f != NULL) {
    fprintf(final_ptr, "%s%c", f->name, 0x00);

    ov = f->id;
    WRITEOUT_OV;
    ov = (int)f->checksum;
    WRITEOUT_OV;

    f = f->next;
  }

  return SUCCEEDED;
}


#if defined(W65816)

int get_snes_cpu_bank(struct label_def *l) {

  struct section_def *s;
  int x, k;

  if (l->section_status == OFF)
    k = g_bankaddress[l->bank] + l->address;
  else {
    s = g_sections_first;
    while (s != NULL) {
      if (l->section_id == s->id)
        break;
      else
        s = s->next;
    }
    k = s->address + l->address + g_bankaddress[s->bank];
  }

  if (g_lorom_defined == 1)
    x = (k / 0x8000) + l->base;
  else
    x = (k / 0x10000) + l->base;

  return x;
}

#endif


#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include.h"
#include "listfile.h"
#include "phase_3.h"
#include "phase_4.h"
#include "parse.h"
#include "stack.h"
#include "printf.h"
#include "main.h"


extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct export_def *g_export_first, *g_export_last;
extern struct label_def *g_label_tmp, *g_label_last, *g_labels;
extern struct map_t *g_defines_map, *g_namespace_map, *g_global_unique_label_map;
extern struct file_name_info *g_file_name_info_first;
extern struct slot g_slots[256];
extern struct after_section *g_after_sections;
extern struct label_sizeof *g_label_sizeofs;
extern struct label_context g_label_context, *g_label_context_first, *g_label_context_last;
extern FILE *g_file_out_ptr;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern char *g_tmp, *g_final_name;
extern int g_rombanks, g_output_format, g_test_mode, g_listfile_data, g_little_endian, g_sizeof_g_tmp;
extern int g_label_context_running_number, g_continued_parsing_after_an_error, g_romheader_baseaddress;

#if defined(GB)
extern char g_licenseecodenew_c1, g_licenseecodenew_c2;
extern int g_computechecksum_defined, g_computecomplementcheck_defined;
extern int g_cartridgetype;
extern int g_name_defined, g_computechecksum_defined, g_computecomplementcheck_defined;
extern int g_licenseecodeold_defined, g_licenseecodenew_defined;
extern int g_licenseecodeold, g_romgbc;
#endif

extern int g_romtype, g_rambanks, g_emptyfill, g_max_address;
extern int g_rambanks_defined, g_verbose_level;
extern int g_section_status;
extern int g_banksize, g_banksize_defined;
extern int g_slots_amount;
extern int *g_banks, *g_bankaddress, g_rombankmap_defined;
extern int g_latest_stack, g_last_stack_id;
extern int g_bankheader_status;
extern int g_smc_defined, g_makefile_rules;

#if defined(W65816)
extern int g_lorom_defined, g_hirom_defined, g_slowrom_defined, g_fastrom_defined, g_snes_mode, g_exhirom_defined;
extern int g_computesneschecksum_defined, g_sramsize, g_sramsize_defined, g_exlorom_defined;
#endif

#if defined(Z80)
extern int g_computesmschecksum_defined, g_smstag_defined, g_smsheader_defined, g_smsforcechecksum_defined;
extern int g_smschecksumsize_defined, g_smschecksumsize;
#endif

#if defined(MC68000)
extern int g_computesmdchecksum_defined;
#endif

struct label_def *g_unknown_labels = NULL, *g_unknown_labels_last = NULL;
struct label_def *g_unknown_header_labels = NULL, *g_unknown_header_labels_last = NULL;

char g_mem_insert_action[MAX_NAME_LENGTH * 3 + 1024], g_namespace[MAX_NAME_LENGTH + 1];

static int s_pc_bank = 0, s_pc_full = 0, s_rom_bank, s_mem_insert_overwrite, s_slot = 0, s_base = 0, s_pc_slot, s_pc_slot_max;
static int s_filename_id, s_line_number, s_dstruct_start = -1, s_special_id = 0;
static struct after_section *s_after_tmp;
static struct label_sizeof *s_label_sizeof_tmp;


#define WRITEOUT_OV fprintf(final_ptr, "%c%c%c%c", (ov>>24)&0xFF, (ov>>16)&0xFF, (ov>>8)&0xFF, ov&0xFF);
#define WRITEOUT_DOU {                                                  \
    cp = (unsigned char *)&dou;                                         \
    fprintf(final_ptr, "%c%c%c%c%c%c%c%c", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]); \
  }

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")


static int _print_fscanf_error_accessing_internal_data_stream(int file_name_id, int line_number) {

  print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not read enough elements from the internal data stream. Please submit a bug report!\n",
          get_file_name(file_name_id), line_number);

  return FAILED;
}


static int _try_to_calculate_stack_calculation_define(struct definition *definition) {

  struct stack *stack = find_stack_calculation((int)definition->value, YES);

  if (stack == NULL)
    return FAILED;
  
  if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
    double r;
    
    if (compute_stack(stack, stack->stacksize, &r) == FAILED)
      return FAILED;

    /* HACK: mark it to be function body so it won't get exported */
    stack->is_function_body = YES;
    
    /* calculation succeeded -> turn the define into a value define */
    definition->type = DEFINITION_TYPE_VALUE;
    definition->value = r;
  }

  return SUCCEEDED;
}


static struct label_def *_new_unknown_reference(int type) {

  struct label_def *label;
  int n = 0, j = 0;

  if ((is_label_anonymous(g_tmp) == YES || g_tmp[0] == '_' || strcmp(g_tmp, "_b") == 0 || strcmp(g_tmp, "_f") == 0) && g_label_context_last->isolated_macro != NULL) {
    if (add_context_to_anonymous_label(g_tmp, g_sizeof_g_tmp, g_label_context_last, s_filename_id, s_line_number) == FAILED)
      return NULL;
  }

  if (g_tmp[0] == ':')
    j = 1;
  while (n < 10 && g_tmp[n+j] == '@')
    n++;
  n--;
  while (n >= 0 && g_label_context_last->parent_labels[n] == NULL)
    n--;
  if (n >= 0) {
    if (mangle_label(&g_tmp[j], g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH-j, s_filename_id, s_line_number) == FAILED)
      return NULL;
  }

  label = calloc(sizeof(struct label_def), 1);
  if (label == NULL) {
    print_text(NO, "%s:%d: _NEW_UNKNOWN_REFERENCE: Out of memory.\n", get_file_name(s_filename_id), s_line_number);
    return NULL;
  }

  label->symbol = NO;

  strcpy(label->label, g_tmp);
  label->next = NULL;
  label->type = type;
  label->filename_id = s_filename_id;
  label->linenumber = s_line_number;
  label->section_status = g_section_status;
  if (g_section_status == ON) {
    label->section_id = g_sec_tmp->id;
    label->section_struct = g_sec_tmp;
    /* relative address, to the beginning of the section */
    label->address = g_sec_tmp->i;
    label->base = g_sec_tmp->base;
    label->bank = g_sec_tmp->bank;
    label->slot = g_sec_tmp->slot;
  }
  else {
    label->section_id = 0;
    label->section_struct = NULL;
    /* bank address, in ROM memory */
    label->address = s_pc_bank;
    label->base = s_base;
    label->bank = s_rom_bank;
    label->slot = s_slot;
  }

  label->special_id = s_special_id;
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
      print_text(NO, "%s:%d: _NEW_UNKNOWN_REFERENCE: Referring to a local label (\"%s\") from inside a bank header section is not allowed.\n",
              get_file_name(s_filename_id), s_line_number, label->label);
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


static int _mangle_stack_references(struct stack *stack) {

  int ind, n, j;
  char *s;

  for (ind = 0; ind < stack->stacksize; ind++) {
    if (stack->stack_items[ind].type == STACK_ITEM_TYPE_LABEL) {
      n = 0;
      j = 0;

      if (stack->stack_items[ind].string[0] == ':')
        j = 1;

      while (n < 10 && stack->stack_items[ind].string[n+j] == '@')
        n++;

      n--;
      while (n >= 0 && g_label_context_last->parent_labels[n] == NULL)
        n--;

      if (n >= 0) {
        if (mangle_label(&stack->stack_items[ind].string[j], g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH-j, s_filename_id, s_line_number) == FAILED)
          return FAILED;
      }

      /* add current ISOLATED .MACRO context? */
      s = stack->stack_items[ind].string;
      if ((is_label_anonymous(s) == YES || s[0] == '_' || strcmp(s, "_b") == 0 || strcmp(s, "_f") == 0) && g_label_context_last->isolated_macro != NULL) {
        if (add_context_to_anonymous_label(s, sizeof(stack->stack_items[ind].string), g_label_context_last, s_filename_id, s_line_number) == FAILED)
          return FAILED;
      }
    }
  }

  return SUCCEEDED;
}


static int _add_namespace_to_reference(char *label, char *name_space, unsigned int label_size) {

  char expanded[MAX_NAME_LENGTH*2+2];
  struct label_def *l;
  
  snprintf(expanded, sizeof(expanded), "%s.%s", name_space, label);
  if (strlen(expanded) >= label_size) {
    print_text(NO, "_ADD_NAMESPACE_TO_REFERENCE: Label expands to \"%s\" which is %d characters too large.\n", expanded, (int)(strlen(expanded)-label_size+1));
    return FAILED;
  }

  /* use the expanded version only if we can find it */
  
  /* label in a namespace? */
  if (g_section_status == ON && g_sec_tmp != NULL && g_sec_tmp->nspace != NULL) {
    if (hashmap_get(g_sec_tmp->nspace->label_map, expanded, (void*)&l) == MAP_OK) {
      if (s_filename_id == l->filename_id) {
        strcpy(label, expanded);
        return SUCCEEDED;
      }
    }
  }

  /* global label? */
  if (hashmap_get(g_global_unique_label_map, expanded, (void*)&l) == MAP_OK) {
    if (s_filename_id == l->filename_id) {
      strcpy(label, expanded);
      return SUCCEEDED;
    }
  }

  return SUCCEEDED;
}


static int _add_namespace_to_stack_references(struct stack *st, char *name_space) {

  int j;

  for (j = 0; j < st->stacksize; j++) {
    if (st->stack_items[j].type == STACK_ITEM_TYPE_LABEL) {
      if (is_label_anonymous(st->stack_items[j].string) == YES)
        continue;

      if (_add_namespace_to_reference(st->stack_items[j].string, name_space, sizeof(st->stack_items[j].string)) == FAILED)
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


int phase_4(void) {

  int i, o, z, y, add_old = 0, x, q, inz, ind, bits_position = 0, bits_byte = 0, flip_endianess = NO, err;
  char *t, c, tmp_buffer[MAX_NAME_LENGTH + 1];
  struct stack *stack;
  struct definition *tmp_def;
  
  /* initialize label context */
  g_label_context.isolated_macro = NULL;
  g_label_context.next = NULL;
  g_label_context.prev = NULL;
  for (x = 0; x < 10; x++)
    g_label_context.parent_labels[x] = NULL;
  g_label_context_last = &g_label_context;
  g_label_context_first = &g_label_context;
  g_label_context_running_number = 0;
 
  g_namespace[0] = 0;
  
  g_section_status = OFF;
  g_bankheader_status = OFF;
  s_mem_insert_overwrite = OFF;

  if (g_verbose_level >= 100)
    print_text(YES, "Internal pass 2...\n");

  /* rewind to the beginning of the internal data stream */
  fseek(g_file_out_ptr, 0, SEEK_SET);
  
  while (fread(&c, 1, 1, g_file_out_ptr) != 0) {
    switch (c) {

      /* SPECIAL CASE ID */
      
    case 'v':
      err = fscanf(g_file_out_ptr, "%d ", &s_special_id);       
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;
      
    case 'E':
      continue;

    case 'j':
      continue;
    case 'J':
      continue;

    case 'i':
      err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (process_macro_in(inz, tmp_buffer, s_filename_id, s_line_number) == FAILED)
        return FAILED;
        
      continue;
    case 'I':
      err = fscanf(g_file_out_ptr, "%d %s ", &inz, tmp_buffer);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (process_macro_out(inz, tmp_buffer, s_filename_id, s_line_number) == FAILED)
        return FAILED;
        
      continue;

    case 'g':
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;
    case 'G':
      continue;

    case 'f':
      err = fscanf(g_file_out_ptr, "%d ", &s_filename_id);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;

    case 'k':
      err = fscanf(g_file_out_ptr, "%d ", &s_line_number);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;

    case 't':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      if (inz == 0)
        g_namespace[0] = 0;
      else {
        err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_namespace);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      }
      continue;
        
      /* SECTION */

    case 'A':
    case 'S':
      if (c == 'A') {
        err = fscanf(g_file_out_ptr, "%d %d ", &x, &ind);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
        inz = YES;
      }
      else {
        err = fscanf(g_file_out_ptr, "%d ", &x);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
        inz = NO;
        ind = 0x123456;
      }

      add_old = s_pc_bank;

      g_sec_tmp = g_sections_first;
      while (g_sec_tmp != NULL) {
        if (g_sec_tmp->id == x)
          break;
        g_sec_tmp = g_sec_tmp->next;
      }

      /* skip all dead sections */
      if (g_sec_tmp->alive == NO)
        continue;

      if (c == 'A') {
        if (inz == NO) {
          /* sanity check */
          print_text(NO, "%s: INTERNAL_PHASE_2: ind has not been set, but we use its value! Please submit a bug report!\n", get_file_name(s_filename_id));
          return FAILED;
        }
        g_sec_tmp->address = ind;
      }

      ind = 0;
      if (g_sec_tmp->maxsize_status == ON) {
        if (g_sec_tmp->maxsize < g_sec_tmp->size) {
          print_text(NO, "%s: INTERNAL_PHASE_2: Section \"%s\" (size %d) doesn't fit into the specified %d bytes.\n",
                  get_file_name(s_filename_id), g_sec_tmp->name, g_sec_tmp->size, g_sec_tmp->maxsize);
          return FAILED;
        }
        else if (g_sec_tmp->size < g_sec_tmp->maxsize) {
          g_sec_tmp->size = g_sec_tmp->maxsize;
          ind = 1;
        }
      }

      if (g_sec_tmp->size > 0) {
        g_sec_tmp->data = calloc(sizeof(unsigned char) * g_sec_tmp->size, 1);
        if (g_sec_tmp->data == NULL) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Out of memory when trying to allocate room for section \"%s\".\n",
                  get_file_name(s_filename_id), s_line_number, g_sec_tmp->name);
          return FAILED;
        }

        /* fill the padded area with _emptyfill_ */
        if (ind == 1)
          memset(g_sec_tmp->data, g_emptyfill, g_sec_tmp->size);
      }
      else
        g_sec_tmp->data = NULL;
      
      if (g_sec_tmp->name[0] == 'B' && strcmp(g_sec_tmp->name, "BANKHEADER") == 0)
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
        s_pc_bank = add_old;
        s_pc_full = g_bankaddress[s_rom_bank] + s_pc_bank;
        s_pc_slot = g_slots[s_slot].address + s_pc_bank;
        s_pc_slot_max = g_slots[s_slot].address + g_slots[s_slot].size;
      }

      continue;

      /* DSB & DSW & DSL */
      /* ('o' skips over memory without claiming it) */

    case 'x':
    case 'o':
      err = fscanf(g_file_out_ptr, "%d %d ", &ind, &x);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .DSB data", get_file_name(s_filename_id), s_line_number);

      if (ind < 0) { /* going backward */
        if (g_section_status == ON)
          g_sec_tmp->i += ind;
        s_pc_bank += ind;
        s_pc_full += ind;
        s_pc_slot += ind;
        ind++;
      }
      else {
        while (ind > 0) {
          if (c == 'o') {
            if (g_section_status == ON)
              g_sec_tmp->i++;
            s_pc_bank++;
            s_pc_full++;
            s_pc_slot++;
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
      err = fscanf(g_file_out_ptr, "%d %d ", &ind, &inz);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      i = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .DSW data", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, "%d %d ", &ind, &inz);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      x = inz & 0xFF;
      i = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .DSL data", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, "%d %d ", &ind, &inz);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      x = inz & 0xFF;
      i = (inz >> 8) & 0xFF;
      q = (inz >> 16) & 0xFF;
      inz = (inz >> 24) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .DSD data", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, "%d ", &x);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a byte", get_file_name(s_filename_id), s_line_number);
        
      if (mem_insert(x) == FAILED)
        return FAILED;

      continue;

    case 'y':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      x = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing two bytes", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, "%d ", &s_base);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;

    case 'z':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      x = inz & 0xFF;
      ind = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing three bytes", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      x = inz & 0xFF;
      q = (inz >> 8) & 0xFF;
      ind = (inz >> 16) & 0xFF;
      inz = (inz >> 24) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing four bytes", get_file_name(s_filename_id), s_line_number);

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
        
        err = fscanf(g_file_out_ptr, "%d ", &bits_to_define);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
        if (bits_to_define == 999) {
          /* add the last byte if there is any data in it */

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(s_filename_id), s_line_number);

          if (bits_position > 0) {
            if (mem_insert(bits_byte) == FAILED)
              return FAILED;
          }

          bits_byte = 0;
          bits_position = 0;

          continue;
        }

        err = fscanf(g_file_out_ptr, "%c", &type);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
        if (type == 'a') {
          int data;
          
          err = fscanf(g_file_out_ptr, "%d", &data);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(s_filename_id), s_line_number);

          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits;
            
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
          
          err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
          if (g_namespace[0] != 0) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
                return FAILED;
            }
          }
          
          hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
          if (tmp_def != NULL) {
            if (tmp_def->type == DEFINITION_TYPE_STRING) {
              print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
              return FAILED;
            }
            else {
              if (tmp_def->type == DEFINITION_TYPE_STACK) {
                if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
                  return FAILED;
              }
              
              if (tmp_def->type != DEFINITION_TYPE_STACK) {
                o = (int)tmp_def->value;

                /* create a what-we-are-doing message for mem_insert*() warnings/errors */
                snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .BITS' bits", get_file_name(s_filename_id), s_line_number);

                while (bits_to_define > 0) {
                  int bits_to_define_this_byte = 8 - bits_position;
                  int bits;
            
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

                continue;
              }
            }
          }

          label = _new_unknown_reference(REFERENCE_TYPE_BITS);
          if (label == NULL)
            return FAILED;

          label->bits_position = bits_position;
          label->bits_to_define = bits_to_define;
          
          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a .BITS reference", get_file_name(s_filename_id), s_line_number);

          /* add zeroes */
          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits;
            
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
          err = fscanf(g_file_out_ptr, "%d", &inz);
          if (err < 1)
            return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
          stack = find_stack_calculation(inz, NO);
          if (stack == NULL) {
            print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
            return FAILED;
          }

          if (stack->section_status == ON) {
            /* relative address, to the beginning of the section */
            stack->address = g_sec_tmp->i;
            stack->base = g_sec_tmp->base;
            stack->bank = g_sec_tmp->bank;
            stack->slot = g_sec_tmp->slot;
          }
          else {
            /* complete address, in ROM memory */
            stack->address = s_pc_bank;
            stack->base = s_base;
            stack->bank = s_rom_bank;
            stack->slot = s_slot;
          }

          stack->type = STACK_TYPE_BITS;
          stack->bits_position = bits_position;
          stack->bits_to_define = bits_to_define;

          if (_mangle_stack_references(stack) == FAILED)
            return FAILED;

          if (g_namespace[0] != 0) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
                return FAILED;
            }
          }
        
          /* this stack was referred from the code */
          stack->position = STACK_POSITION_CODE;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for .BITS' bits", get_file_name(s_filename_id), s_line_number);

          /* add zeroes */
          while (bits_to_define > 0) {
            int bits_to_define_this_byte = 8 - bits_position;
            int bits;
            
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
      err = fscanf(g_file_out_ptr, "%d %d %d %d ", &x, &inz, &z, &y);
      if (err < 4)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      g_ifd_tmp = g_incbin_file_data_first;
      for (ind = 0; ind != x; ind++)
        g_ifd_tmp = g_ifd_tmp->next;
      t = g_ifd_tmp->data + z;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing .INCBIN data", get_file_name(s_filename_id), s_line_number);

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
      if (c == 'O') {
        err = fscanf(g_file_out_ptr, "%d ", &s_pc_bank);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      }
      else {
        err = fscanf(g_file_out_ptr, "%d %d ", &s_rom_bank, &s_slot);
        if (err < 2)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

        if (g_banksize_defined == 0)
          g_banksize = g_banks[s_rom_bank];
      }

      s_pc_full = g_bankaddress[s_rom_bank] + s_pc_bank;
      s_pc_slot = g_slots[s_slot].address + s_pc_bank;
      s_pc_slot_max = g_slots[s_slot].address + g_slots[s_slot].size;

      continue;

      /* BREAKPOINT */

    case 'Z':
      continue;

      /* SYMBOL */

    case 'Y':
      /* skip the symbol */
      err = fscanf(g_file_out_ptr, "%*s ");
      if (err < 0)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      continue;

      /* LABEL */
    case 'L':
      {
        struct label_def *l;
        int m, n = 0, mangled_label = NO;

        err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
        
        if (is_label_anonymous(g_tmp) == NO) {
          while (n < 10 && g_tmp[n] == '@')
            n++;

          m = n + 1;
          while (m < 10)
            g_label_context_last->parent_labels[m++] = NULL;

          m = n;
          n--;
          while (n >= 0 && g_label_context_last->parent_labels[n] == NULL)
            n--;

          if (n >= 0) {
            if (mangle_label(g_tmp, g_label_context_last->parent_labels[n]->label, n, MAX_NAME_LENGTH, s_filename_id, s_line_number) == FAILED)
              return FAILED;
            mangled_label = YES;
          }

          if (g_namespace[0] != 0 && mangled_label == NO) {
            if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
              if (add_namespace(g_tmp, g_namespace, g_sizeof_g_tmp, s_filename_id, s_line_number) == FAILED)
                return FAILED;
            }
          }
            
          if (m < 10 && find_label(g_tmp, g_sec_tmp, (void*)&l) == SUCCEEDED)
            g_label_context_last->parent_labels[m] = l;
        }
      }
      continue;

      /* 8BIT (ALSO 9BIT SHORT) COMPUTATION */

    case '-':
    case 'c':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      stack = find_stack_calculation(inz, NO);
      if (stack == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
        return FAILED;
      }

      if (stack->section_status == ON) {
        /* relative address, to the beginning of the section */
        stack->address = g_sec_tmp->i;
        stack->base = g_sec_tmp->base;
        stack->bank = g_sec_tmp->bank;
        stack->slot = g_sec_tmp->slot;
      }
      else {
        /* complete address, in ROM memory */
        stack->address = s_pc_bank;
        stack->base = s_base;
        stack->bank = s_rom_bank;
        stack->slot = s_slot;
      }

      if (c == '-')
        stack->type = STACK_TYPE_9BIT_SHORT;
      else
        stack->type = STACK_TYPE_8BIT;
      stack->special_id = s_special_id;
        
      if (_mangle_stack_references(stack) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
            return FAILED;
        }
      }

      /* try once again to calculate the stack calculation (works for contained .DEFINEs that came lexically after this calculation) */
      if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
        double r;
        
        if (compute_stack(stack, stack->stacksize, &r) == FAILED)
          return FAILED;

        /* HACK: mark it to be function body so it won't get exported */
        stack->is_function_body = YES;

        o = (int)r;
        if (c == '-') {
          if ((o & 1) == 1) {
            print_text(NO, "%s:%d: INTERNAL_PHASE_2: The RAM address must be even.\n", get_file_name(s_filename_id), s_line_number);
            return FAILED;
          }
          o = o >> 1;
        }

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing an 8-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert(o & 0xFF) == FAILED)
          return FAILED;
      }
      else {
        /* this stack was referred from the code */
        stack->position = STACK_POSITION_CODE;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit computation", get_file_name(s_filename_id), s_line_number);
        
        if (mem_insert_padding() == FAILED)
          return FAILED;
      }

      continue;

      /* 16BIT COMPUTATION */

    case 'C':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      stack = find_stack_calculation(inz, NO);
      if (stack == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
        return FAILED;
      }

      if (stack->section_status == ON) {
        /* relative address, to the beginning of the section */
        stack->address = g_sec_tmp->i;
        stack->base = g_sec_tmp->base;
        stack->bank = g_sec_tmp->bank;
        stack->slot = g_sec_tmp->slot;
      }
      else {
        /* complete address, in ROM memory */
        stack->address = s_pc_bank;
        stack->base = s_base;
        stack->bank = s_rom_bank;
        stack->slot = s_slot;
      }

      stack->type = STACK_TYPE_16BIT;
        
      if (_mangle_stack_references(stack) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* try once again to calculate the stack calculation (works for contained .DEFINEs that came lexically after this calculation) */
      if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
        double r;
        
        if (compute_stack(stack, stack->stacksize, &r) == FAILED)
          return FAILED;

        /* HACK: mark it to be function body so it won't get exported */
        stack->is_function_body = YES;

        o = (int)r;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 16-bit computation", get_file_name(s_filename_id), s_line_number);

        /* flip endianess? */
        if (stack->special_id == 4) {
          int top, bottom;

          top = (o >> 8) & 0xFF;
          bottom = o & 0xFF;
              
          o = (bottom << 8) | top;
        }
        
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
      else {
        /* this stack was referred from the code */
        stack->position = STACK_POSITION_CODE;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
      }
      
      continue;

#if defined(SPC700)
      /* 13BIT COMPUTATION */

    case 'N':
      err = fscanf(g_file_out_ptr, "%d %d ", &x, &inz);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      stack = find_stack_calculation(inz, NO);
      if (stack == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
        return FAILED;
      }

      if (stack->section_status == ON) {
        /* relative address, to the beginning of the section */
        stack->address = g_sec_tmp->i;
        stack->base = g_sec_tmp->base;
        stack->bank = g_sec_tmp->bank;
        stack->slot = g_sec_tmp->slot;
      }
      else {
        /* complete address, in ROM memory */
        stack->address = s_pc_bank;
        stack->base = s_base;
        stack->bank = s_rom_bank;
        stack->slot = s_slot;
      }

      stack->type = STACK_TYPE_13BIT;
        
      if (_mangle_stack_references(stack) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
            return FAILED;
        }
      }

      /* try once again to calculate the stack calculation (works for contained .DEFINEs that came lexically after this calculation) */
      if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
        double r;
        
        if (compute_stack(stack, stack->stacksize, &r) == FAILED)
          return FAILED;

        /* HACK: mark it to be function body so it won't get exported */
        stack->is_function_body = YES;

        o = (int)r;
        if (o > 8191 || o < 0) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference value of %d is out of 13-bit range.\n", get_file_name(s_filename_id), s_line_number, o);
          return FAILED;
        }

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 13-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert(o & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert(((o | x << 13) & 0xFF00) >> 8) == FAILED)
          return FAILED;
      }
      else {
        /* this stack was referred from the code */
        stack->position = STACK_POSITION_CODE;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 13-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert(0x00) == FAILED)
          return FAILED;
        if (mem_insert(x << (13 - 8)) == FAILED)
          return FAILED;
      }

      continue;
#endif

      /* 24BIT COMPUTATION */

    case 'T':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      stack = find_stack_calculation(inz, NO);
      if (stack == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
        return FAILED;
      }

      if (stack->section_status == ON) {
        /* relative address, to the beginning of the section */
        stack->address = g_sec_tmp->i;
        stack->base = g_sec_tmp->base;
        stack->bank = g_sec_tmp->bank;
        stack->slot = g_sec_tmp->slot;
      }
      else {
        /* complete address, in ROM memory */
        stack->address = s_pc_bank;
        stack->base = s_base;
        stack->bank = s_rom_bank;
        stack->slot = s_slot;
      }

      stack->type = STACK_TYPE_24BIT;

      if (_mangle_stack_references(stack) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
            return FAILED;
        }
      }

      /* try once again to calculate the stack calculation (works for contained .DEFINEs that came lexically after this calculation) */
      if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
        double r;
        
        if (compute_stack(stack, stack->stacksize, &r) == FAILED)
          return FAILED;

        /* HACK: mark it to be function body so it won't get exported */
        stack->is_function_body = YES;

        o = (int)r;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 24-bit computation", get_file_name(s_filename_id), s_line_number);

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
      else {
        /* this stack was referred from the code */
        stack->position = STACK_POSITION_CODE;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 24-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
      }

      continue;

      /* 32BIT COMPUTATION */

    case 'U':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      stack = find_stack_calculation(inz, NO);
      if (stack == NULL) {
        print_text(NO, "%s:%d: INTERNAL_PHASE_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(s_filename_id), s_line_number, inz);
        return FAILED;
      }

      if (stack->section_status == ON) {
        /* relative address, to the beginning of the section */
        stack->address = g_sec_tmp->i;
        stack->base = g_sec_tmp->base;
        stack->bank = g_sec_tmp->bank;
        stack->slot = g_sec_tmp->slot;
      }
      else {
        /* complete address, in ROM memory */
        stack->address = s_pc_bank;
        stack->base = s_base;
        stack->bank = s_rom_bank;
        stack->slot = s_slot;
      }

      stack->type = STACK_TYPE_32BIT;

      if (_mangle_stack_references(stack) == FAILED)
        return FAILED;

      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_stack_references(stack, g_namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* try once again to calculate the stack calculation (works for contained .DEFINEs that came lexically after this calculation) */
      if (stack->is_function_body == NO && resolve_stack(stack->stack_items, stack->stacksize) == SUCCEEDED) {
        double r;
        
        if (compute_stack(stack, stack->stacksize, &r) == FAILED)
          return FAILED;

        /* HACK: mark it to be function body so it won't get exported */
        stack->is_function_body = YES;

        o = (int)r;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 32-bit computation", get_file_name(s_filename_id), s_line_number);

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
      else {
        /* this stack was referred from the code */
        stack->position = STACK_POSITION_CODE;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 32-bit computation", get_file_name(s_filename_id), s_line_number);

        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;
      }

      continue;

      /* 24BIT REFERENCE */

    case 'q':
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }

          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 24-bit reference", get_file_name(s_filename_id), s_line_number);

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

            continue;
          }
        }
      }

      if (_new_unknown_reference(REFERENCE_TYPE_DIRECT_24BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 24-bit reference", get_file_name(s_filename_id), s_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 32BIT REFERENCE */

    case 'V':
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }
          
          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 32-bit reference", get_file_name(s_filename_id), s_line_number);

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

            continue;
          }
        }
      }

      if (_new_unknown_reference(REFERENCE_TYPE_DIRECT_32BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 32-bit reference", get_file_name(s_filename_id), s_line_number);

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
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }

          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(s_filename_id), s_line_number);

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

            continue;
          }
        }
      }

      if (_new_unknown_reference(REFERENCE_TYPE_RELATIVE_16BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(s_filename_id), s_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* FLIP THE ENDIANESS OF NEXT 'r' */
      
    case '.':
      flip_endianess = YES;
      continue;
      
      /* 16BIT REFERENCE */

    case 'r':
      {
        struct label_def *label;
        
        err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
        if (err < 1)
          return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
        
        if (g_namespace[0] != 0) {
          if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
            if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
              return FAILED;
          }
        }

        hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
        if (tmp_def != NULL) {
          if (tmp_def->type == DEFINITION_TYPE_STRING) {
            print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
            return FAILED;
          }
          else {
            if (tmp_def->type == DEFINITION_TYPE_STACK) {
              if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
                return FAILED;
            }

            if (tmp_def->type != DEFINITION_TYPE_STACK) {
              o = (int)tmp_def->value;

              /* create a what-we-are-doing message for mem_insert*() warnings/errors */
              snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(s_filename_id), s_line_number);

              if (flip_endianess == YES) {
                int top, bottom;

                top = (o >> 8) & 0xFF;
                bottom = o & 0xFF;
              
                o = (bottom << 8) | top;

                flip_endianess = NO;
              }
            
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

              continue;
            }
          }
        }

        label = _new_unknown_reference(REFERENCE_TYPE_DIRECT_16BIT);
        if (label == NULL)
          return FAILED;

        if (flip_endianess == YES) {
          flip_endianess = NO;
          label->special_id = 4;
        }

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(s_filename_id), s_line_number);

        if (mem_insert_padding() == FAILED)
          return FAILED;
        if (mem_insert_padding() == FAILED)
          return FAILED;

        continue;
      }

#if defined(SPC700)
      /* 13BIT REFERENCE */

    case 'n':
      err = fscanf(g_file_out_ptr, "%d ", &inz);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }

          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            if (o > 8191 || o < 0) {
              print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference value of \"%s\" (%d) is out of 13-bit range.\n", get_file_name(s_filename_id), s_line_number, g_tmp, o);
              return FAILED;
            }

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing a 13-bit reference", get_file_name(s_filename_id), s_line_number);

            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;
            if (mem_insert(((o | inz << 13) & 0xFF00) >> 8) == FAILED)
              return FAILED;

            continue;
          }
        }
      }

      if (_new_unknown_reference(REFERENCE_TYPE_DIRECT_13BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for a 13-bit reference", get_file_name(s_filename_id), s_line_number);

      if (mem_insert(0x00) == FAILED)
        return FAILED;
      if (mem_insert(inz << (13 - 8)) == FAILED)
        return FAILED;

      continue;
#endif

      /* 8BIT PC RELATIVE REFERENCE */

    case 'R':
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }

          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(s_filename_id), s_line_number);

            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;

            continue;
          }
        }
      }
      
      if (_new_unknown_reference(REFERENCE_TYPE_RELATIVE_8BIT) == NULL)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(s_filename_id), s_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 8BIT (9BIT SHORT ALSO) REFERENCE */

    case '*':
    case 'Q':
      err = fscanf(g_file_out_ptr, STRING_READ_FORMAT, g_tmp);
      if (err < 1)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);
      
      if (g_namespace[0] != 0) {
        if (g_section_status == OFF || g_sec_tmp->nspace == NULL) {
          if (_add_namespace_to_reference(g_tmp, g_namespace, g_sizeof_g_tmp) == FAILED)
            return FAILED;
        }
      }

      hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          print_text(NO, "%s:%d: INTERNAL_PHASE_2: Reference to a string definition \"%s\"?\n", get_file_name(s_filename_id), s_line_number, g_tmp);
          return FAILED;
        }
        else {
          if (tmp_def->type == DEFINITION_TYPE_STACK) {
            if (_try_to_calculate_stack_calculation_define(tmp_def) == FAILED)
              return FAILED;
          }

          if (tmp_def->type != DEFINITION_TYPE_STACK) {
            o = (int)tmp_def->value;

            /* 9-bit short? */
            if (c == '*') {
              if ((o & 1) == 1) {
                print_text(NO, "%s:%d: INTERNAL_PHASE_2: The RAM address must be even.\n", get_file_name(s_filename_id), s_line_number);
                return FAILED;
              }
              o = o >> 1;
            }

            /* create a what-we-are-doing message for mem_insert*() warnings/errors */
            snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(s_filename_id), s_line_number);
            
            if (mem_insert(o & 0xFF) == FAILED)
              return FAILED;

            continue;
          }
        }
      }

      if (c == '*') {
        if (_new_unknown_reference(REFERENCE_TYPE_DIRECT_9BIT_SHORT) == NULL)
          return FAILED;
      }
      else {
        if (_new_unknown_reference(REFERENCE_TYPE_DIRECT_8BIT) == NULL)
          return FAILED;
      }

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(s_filename_id), s_line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* .DSTRUCT stuff */

    case 'e':
      err = fscanf(g_file_out_ptr, "%d %d ", &x, &y);
      if (err < 2)
        return _print_fscanf_error_accessing_internal_data_stream(s_filename_id, s_line_number);

      if (y == -1) {
        /* mark start of .DSTRUCT */
        s_dstruct_start = s_pc_full;
        /* make sure all data in a section gets set to emptyfill */
        if (g_section_status == ON)
          memset(g_sec_tmp->data + g_sec_tmp->i, g_emptyfill, x);
      }
      else if (y == -2) {
        /* end of .DSTRUCT. make sure all memory is claimed. */
        while (s_pc_full < s_dstruct_start + x) {
          if (g_section_status == OFF && g_rom_banks_usage_table[s_pc_full] == 0) {
            g_rom_banks_usage_table[s_pc_full] = 2;
            g_rom_banks[s_pc_full] = g_emptyfill;
          }
          if (g_section_status == ON)
            g_sec_tmp->i++;
          s_pc_bank++;
          s_pc_slot++;
          s_pc_full++;
        }
      }
      else {
        /* seek offset relative to dstruct start */
        if (g_section_status == ON)
          g_sec_tmp->i = g_sec_tmp->i + (s_dstruct_start - s_pc_full) + x;
        s_pc_bank = s_pc_bank + (s_dstruct_start - s_pc_full) + x;
        s_pc_slot = s_pc_slot + (s_dstruct_start - s_pc_full) + x;
        s_pc_full = s_dstruct_start + x;
      }
      continue;
    }
  }

  /* make sure that the pending calculations we export next have consecutive IDs starting from zero.
     when pending calculations are exported like this the pointer tables in the linker take less memory. */
  if (compress_stack_calculation_ids() == FAILED)
    return FAILED;

  if (g_continued_parsing_after_an_error == YES)
    return FAILED;

  if (g_output_format == OUTPUT_LIBRARY && g_test_mode == OFF) {
    /* library file output */
    if (write_library_file() == FAILED)
      return FAILED;
  }
  else if (g_output_format == OUTPUT_OBJECT && g_test_mode == OFF) {
    /* object file output */
    if (write_object_file() == FAILED)
      return FAILED;
  }

  /* output makefile rules */
  if (g_makefile_rules == YES)
    print_file_names(g_final_name);

  /* show project information */
  if (g_verbose_level >= 2 && g_output_format == OUTPUT_OBJECT)
    show_project_information_object();
  else if (g_verbose_level >= 2 && g_output_format == OUTPUT_LIBRARY)
    show_project_information_library();

  return SUCCEEDED;
}


int write_object_file(void) {

  int ind, ov, inz, i;
  FILE *final_ptr;
  unsigned char *cp;
  double dou;

  if ((final_ptr = fopen(g_final_name, "wb")) == NULL) {
    print_text(NO, "INTERNAL_PHASE_2: Error opening file \"%s\" for writing.\n", g_final_name);
    return FAILED;
  }

  /* header */
  fprintf(final_ptr, "WLAl%c", g_emptyfill);

  /* misc bits */
  ind = 0;

#if defined(Z80)
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

#if defined(GB)
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

#if defined(Z80)
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

#if defined(CSG65CE02)
  /* 65ce02 bit */
  ind |= 1 << 0;
#endif

#if defined(Z80)
  if (g_smsforcechecksum_defined != 0)
    ind |= 1 << 1;
  if (g_smschecksumsize_defined != 0)
    ind |= 1 << 2;
#endif

#if defined(MC68000)
  if (g_computesmdchecksum_defined != 0)
    ind |= 1 << 3;
#endif
  
  fprintf(final_ptr, "%c", ind);

#if defined(Z80)
  /* sms checksum calculation special range */
  ov = g_smschecksumsize;
  WRITEOUT_OV;
  /* sms ROM header base address */
  ov = g_romheader_baseaddress;
  WRITEOUT_OV;
#else
  ov = 0;
  WRITEOUT_OV;
  WRITEOUT_OV;
#endif

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
    if (g_label_tmp->alive == YES)
      ov++;
    g_label_tmp = g_label_tmp->next;
  }

  WRITEOUT_OV;

  g_label_tmp = g_labels;
  while (g_label_tmp != NULL) {
    if (g_label_tmp->alive == YES) {
      if (g_label_tmp->symbol != 2)
        fprintf(final_ptr, "%s", g_label_tmp->label);
      fprintf(final_ptr, "%c", g_label_tmp->symbol);

      fprintf(final_ptr, "%c", g_label_tmp->slot);

      ov = g_label_tmp->filename_id;
      WRITEOUT_OV;
        
      ov = g_label_tmp->section_id;
      WRITEOUT_OV;

      /* DEBUG
         print_text(NO, "LABEL: \"%s\" SLOT: %d LINE: %d\n", g_label_tmp->label, g_label_tmp->slot, g_label_tmp->linenumber);
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
  ov = 0;
  for (inz = 0; inz < g_last_stack_id; inz++) {
    struct stack *stack = find_stack_calculation(inz, NO);

    if (stack == NULL)
      continue;
    if (stack->is_function_body == NO)
      ov++;
  }
  WRITEOUT_OV;

  for (inz = 0; inz < g_last_stack_id; inz++) {
    struct stack *stack = find_stack_calculation(inz, NO);

    if (stack == NULL)
      continue;
    if (stack->is_function_body == YES)
      continue;

    ov = stack->id;
    WRITEOUT_OV;

    fprintf(final_ptr, "%c%c", stack->type | (stack->relative_references << 7), stack->special_id);

    ov = stack->section_id;
    WRITEOUT_OV;

    ov = stack->filename_id;
    WRITEOUT_OV;

    fprintf(final_ptr, "%c%c", stack->stacksize, stack->position);

    if (stack->type == STACK_TYPE_BITS)
      fprintf(final_ptr, "%c%c", stack->bits_position, stack->bits_to_define);

    fprintf(final_ptr, "%c", stack->slot);

    ov = stack->address;
    WRITEOUT_OV;

    ov = stack->linenumber;
    WRITEOUT_OV;

    ov = stack->bank;
    WRITEOUT_OV;

    ov = stack->base;
    WRITEOUT_OV;
      
    for (ind = 0; ind < stack->stacksize; ind++) {
      fprintf(final_ptr, "%c%c", stack->stack_items[ind].type, stack->stack_items[ind].sign);
      if (stack->stack_items[ind].type == STACK_ITEM_TYPE_LABEL || stack->stack_items[ind].type == STACK_ITEM_TYPE_STRING)
        fprintf(final_ptr, "%s%c", stack->stack_items[ind].string, 0);
      else {
        dou = stack->stack_items[ind].value;
        WRITEOUT_DOU;
      }
    }
  }

  /* label sizeofs */
  ov = 0;
  s_label_sizeof_tmp = g_label_sizeofs;
  while (s_label_sizeof_tmp != NULL) {
    ov++;
    s_label_sizeof_tmp = s_label_sizeof_tmp->next;
  }

  WRITEOUT_OV;

  s_label_sizeof_tmp = g_label_sizeofs;
  while (s_label_sizeof_tmp != NULL) {
    fprintf(final_ptr, "%s%c", s_label_sizeof_tmp->name, 0);

    ov = s_label_sizeof_tmp->size;
    WRITEOUT_OV;

    s_label_sizeof_tmp = s_label_sizeof_tmp->next;
  }    

  /* global list file items */
  if (g_listfile_data == YES)
    listfile_globals_write(final_ptr);
  else {
    ov = 0;
    WRITEOUT_OV;
  }
  
  /* appendto/after sections */
  ov = 0;
  s_after_tmp = g_after_sections;
  while (s_after_tmp != NULL) {
    if (s_after_tmp->alive == YES)
      ov++;
    s_after_tmp = s_after_tmp->next;
  }
  WRITEOUT_OV;

  s_after_tmp = g_after_sections;
  while (s_after_tmp != NULL) {
    if (s_after_tmp->alive == YES) {
      ov = s_after_tmp->section->id;
      WRITEOUT_OV;
      fprintf(final_ptr, "%c", s_after_tmp->is_appendto);
      fprintf(final_ptr, "%s%c", s_after_tmp->section->name, 0);
      fprintf(final_ptr, "%s%c", s_after_tmp->after, 0);
    }
      
    s_after_tmp = s_after_tmp->next;
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
    if (g_sec_tmp->alive == YES) {
      /* section block id */
      fprintf(final_ptr, "%c%s%c%c", 0x1, g_sec_tmp->name, g_sec_tmp->status, g_sec_tmp->keep);
      if (g_sec_tmp->nspace == NULL)
        fprintf(final_ptr, "%c", 0);
      else
        fprintf(final_ptr, "%s%c", g_sec_tmp->nspace->name, 0);

      if (g_sec_tmp->status == SECTION_STATUS_SEMISUPERFREE)
        fprintf(final_ptr, "%s%c", g_sec_tmp->banks, 0);
      
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
      ov = g_sec_tmp->bitwindow;
      WRITEOUT_OV;
      ov = g_sec_tmp->window_start;
      WRITEOUT_OV;
      ov = g_sec_tmp->window_end;
      WRITEOUT_OV;

      if (g_sec_tmp->size > 0)
        fwrite(g_sec_tmp->data, 1, g_sec_tmp->size, final_ptr);

      if (g_listfile_data == YES && g_sec_tmp->listfile_items > 0)
        listfile_block_write(final_ptr, g_sec_tmp);
      else
        fprintf(final_ptr, "%c", 0);
    }
    g_sec_tmp = g_sec_tmp->next;
  }

  fclose(final_ptr);

  return SUCCEEDED;
}


int write_library_file(void) {

  int ind, inz, ov;
  FILE *final_ptr;
  unsigned char *cp;
  double dou;
  
  if ((final_ptr = fopen(g_final_name, "wb")) == NULL) {
    print_text(NO, "INTERNAL_PHASE_2: Error opening file \"%s\" for writing.\n", g_final_name);
    return FAILED;
  }

  /* header */
  fprintf(final_ptr, "WLAI");

  /* misc bits */
  ind = 0;

  if (g_little_endian == NO)
    ind |= 1 << 0;
#if defined(W65816)
  /* 65816 bit */
  ind |= 1 << 1;
#endif
#if defined(CSG65CE02)
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
    if (g_label_tmp->alive == YES)
      ov++;
    g_label_tmp = g_label_tmp->next;
  }

  WRITEOUT_OV;

  g_label_tmp = g_labels;
  while (g_label_tmp != NULL) {
    if (g_label_tmp->alive == YES) {
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
      print_text(NO, "INTERNAL_PHASE_2: Label \"%s\" is outside all sections.\n", g_label_tmp->label);
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
  ov = 0;
  for (inz = 0; inz < g_last_stack_id; inz++) {
    struct stack *stack = find_stack_calculation(inz, NO);

    if (stack == NULL)
      continue;
    if (stack->is_function_body == NO)
      ov++;
  }
  WRITEOUT_OV;

  for (inz = 0; inz < g_last_stack_id; inz++) {
    struct stack *stack = find_stack_calculation(inz, NO);

    if (stack == NULL)
      continue;
    if (stack->is_function_body == YES)
      continue;

    ov = stack->id;
    WRITEOUT_OV;

    fprintf(final_ptr, "%c%c", stack->type | (stack->relative_references << 7), stack->special_id);

    ov = stack->section_id;
    WRITEOUT_OV;

    ov = stack->filename_id;
    WRITEOUT_OV;
      
    fprintf(final_ptr, "%c%c", stack->stacksize, stack->position);

    if (stack->type == STACK_TYPE_BITS)
      fprintf(final_ptr, "%c%c", stack->bits_position, stack->bits_to_define);

    ov = stack->address;
    WRITEOUT_OV;

    ov = stack->linenumber;
    WRITEOUT_OV;

    for (ind = 0; ind < stack->stacksize; ind++) {
      fprintf(final_ptr, "%c%c", stack->stack_items[ind].type, stack->stack_items[ind].sign);
      if (stack->stack_items[ind].type == STACK_ITEM_TYPE_LABEL || stack->stack_items[ind].type == STACK_ITEM_TYPE_STRING)
        fprintf(final_ptr, "%s%c", stack->stack_items[ind].string, 0);
      else {
        dou = stack->stack_items[ind].value;
        WRITEOUT_DOU;
      }
    }
  }

  /* label sizeofs */
  ov = 0;
  s_label_sizeof_tmp = g_label_sizeofs;
  while (s_label_sizeof_tmp != NULL) {
    ov++;
    s_label_sizeof_tmp = s_label_sizeof_tmp->next;
  }

  WRITEOUT_OV;

  s_label_sizeof_tmp = g_label_sizeofs;
  while (s_label_sizeof_tmp != NULL) {
    fprintf(final_ptr, "%s%c", s_label_sizeof_tmp->name, 0);

    ov = s_label_sizeof_tmp->size;
    WRITEOUT_OV;

    s_label_sizeof_tmp = s_label_sizeof_tmp->next;
  }
    
  /* global list file items */
  if (g_listfile_data == YES)
    listfile_globals_write(final_ptr);
  else {
    ov = 0;
    WRITEOUT_OV;
  }

  /* appendto/after sections */
  ov = 0;
  s_after_tmp = g_after_sections;
  while (s_after_tmp != NULL) {
    if (s_after_tmp->alive == YES)
      ov++;
    s_after_tmp = s_after_tmp->next;
  }
  WRITEOUT_OV;

  s_after_tmp = g_after_sections;
  while (s_after_tmp != NULL) {
    if (s_after_tmp->alive == YES) {
      ov = s_after_tmp->section->id;
      WRITEOUT_OV;
      fprintf(final_ptr, "%c", s_after_tmp->is_appendto);
      fprintf(final_ptr, "%s%c", s_after_tmp->section->name, 0);
      fprintf(final_ptr, "%s%c", s_after_tmp->after, 0);
    }
      
    s_after_tmp = s_after_tmp->next;
  }

  /* sections */
  g_sec_tmp = g_sections_first;
  while (g_sec_tmp != NULL) {
    if (g_sec_tmp->alive == YES) {
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
      ov = g_sec_tmp->bitwindow;
      WRITEOUT_OV;
      ov = g_sec_tmp->window_start;
      WRITEOUT_OV;
      ov = g_sec_tmp->window_end;
      WRITEOUT_OV;
      
      if (g_sec_tmp->size > 0)
        fwrite(g_sec_tmp->data, 1, g_sec_tmp->size, final_ptr);

      if (g_listfile_data == YES && g_sec_tmp->listfile_items > 0)
        listfile_block_write(final_ptr, g_sec_tmp);
      else
        fprintf(final_ptr, "%c", 0);
    }
    g_sec_tmp = g_sec_tmp->next;
  }

  fclose(final_ptr);

  return SUCCEEDED;
}


int show_project_information_object(void) {

  int total_used_ram = 0, total_used_rom = 0, i, printed_something, total_rom_size = 0;
  struct section_def *s;
  float f;
    
  fflush(stderr);
  fflush(stdout);

  s = g_sections_first;
  while (s != NULL) {
    int status = s->status;
      
    if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
        status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE)
      total_used_ram += s->size;
    else if (status == SECTION_STATUS_HEADER) {
    }
    else
      total_used_rom += s->size;
      
    s = s->next;
  }

  if (g_verbose_level >= 100) {
    print_text(YES, "-------------------------------------------------\n");
    print_text(YES, "---                   ROM                     ---\n");
    print_text(YES, "-------------------------------------------------\n");
  }
    
  for (i = 0; i < g_rombanks; i++) {  
    int bank_address = g_bankaddress[i], j, used_rom = 0, found_block, block_start, used_sections = 0, bank_size = g_banks[i];

    total_rom_size += bank_size;
    
    /* calculate ROM usage (this bank) */
    for (j = 0; j < bank_size; j++) {
      if (g_rom_banks_usage_table[bank_address + j] > 0) {
        used_rom++;
        total_used_rom++;
      }
    }

    s = g_sections_first;
    while (s != NULL) {
      int status = s->status;

      if (s->bank == i) {
        if (status == SECTION_STATUS_FREE || status == SECTION_STATUS_FORCE || status == SECTION_STATUS_OVERWRITE ||
            status == SECTION_STATUS_SEMIFREE || status == SECTION_STATUS_ABSOLUTE || status == SECTION_STATUS_SUPERFREE ||
            status == SECTION_STATUS_SEMISUBFREE || status == SECTION_STATUS_SEMISUPERFREE) {
          used_rom += s->size;
          used_sections += s->size;
        }
      }
      
      s = s->next;
    }

    if (used_rom == 0)
      continue;

    f = ((float)used_rom)/bank_size * 100.0f;
    if (g_verbose_level >= 100) {
      print_text(YES, "ROM bank %d (%d bytes (%.2f%%) used)\n", i, used_rom, f);
      print_text(YES, "  - Outside .SECTIONs (%d bytes)\n", used_rom - used_sections);
    }
    
    found_block = NO;
    block_start = 0;
    printed_something = NO;
    
    for (j = 0; j < bank_size; j++) {
      if (g_rom_banks_usage_table[bank_address + j] != 0 && found_block == NO) {
        found_block = YES;
        block_start = j;
      }
      else if (g_rom_banks_usage_table[bank_address + j] == 0 && found_block == YES) {
        if (g_verbose_level >= 100) {
          print_text(YES, "    - Used space at $%.4x-$%.4x (%d bytes).\n", block_start, j - 1, j - block_start);
          printed_something = YES;
        }
        found_block = NO;
      }
      else if (found_block == YES && j == bank_size - 1) {
        if (g_verbose_level >= 100) {
          print_text(YES, "    - Used space at $%.4x-$%.4x (%d bytes).\n", block_start, j, j - block_start + 1);
          printed_something = YES;
        }
      }
    }

    if (printed_something == NO) {
      if (g_verbose_level >= 100)
        print_text(YES, "    - No data outside .SECTIONs.\n");
    }
    
    if (g_verbose_level >= 100)
      print_text(YES, "  - Sections (%d bytes)\n", used_sections);

    printed_something = NO;
    
    s = g_sections_first;
    while (s != NULL) {
      int status = s->status;

      if (s->bank == i) {
        if (status == SECTION_STATUS_FREE || status == SECTION_STATUS_FORCE || status == SECTION_STATUS_OVERWRITE ||
            status == SECTION_STATUS_SEMIFREE || status == SECTION_STATUS_ABSOLUTE || status == SECTION_STATUS_SUPERFREE ||
            status == SECTION_STATUS_SEMISUBFREE || status == SECTION_STATUS_SEMISUPERFREE) {
          if (g_verbose_level >= 100) {
            print_text(YES, "    - .SECTION \"%s\" (%d bytes).\n", s->name, s->size);
            printed_something = YES;
          }
        }
      }
      
      s = s->next;
    }

    if (printed_something == NO) {
      if (g_verbose_level >= 100)
        print_text(YES, "    - No .SECTIONs found.\n");
    }
  }

  if (g_verbose_level >= 100) {
    print_text(YES, "-------------------------------------------------\n");
    print_text(YES, "---                   RAM                     ---\n");
    print_text(YES, "-------------------------------------------------\n");
  }
  
  if (total_used_ram == 0) {
    if (g_verbose_level >= 100)
      print_text(YES, "No .RAMSECTIONs were found, no information about RAM.\n");
  }
  else {
    char slot_name[MAX_NAME_LENGTH + 1];
    int slot, bank;
    
    for (slot = 0; slot < g_slots_amount; slot++) {
      for (bank = 0; bank < 256; bank++) {
        int ram_used = 0;

        s = g_sections_first;
        while (s != NULL) {
          if (s->slot == slot && s->bank == bank) {
            int status = s->status;

            if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
                status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE)
              ram_used += s->size;
          }
          
          s = s->next;
        }

        if (ram_used == 0)
          continue;

        /* get slot name */
        if (g_slots[slot].name[0] != 0)
          snprintf(slot_name, sizeof(slot_name), "%d (%s)", slot, g_slots[slot].name);
        else
          snprintf(slot_name, sizeof(slot_name), "%d", slot);      

        f = ((float)ram_used)/g_slots[slot].size * 100.0f;
        if (g_verbose_level >= 100)
          print_text(YES, "RAM slot %s bank %d (%d bytes (%.2f%%) used)\n", slot_name, bank, ram_used, f);

        s = g_sections_first;
        while (s != NULL) {
          if (s->slot == slot && s->bank == bank) {
            int status = s->status;

            if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
                status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE) {
              if (g_verbose_level >= 100)
                print_text(YES, "  - .RAMSECTION \"%s\" (%d bytes).\n", s->name, s->size);
            }
          }
          
          s = s->next;
        }
      }
    }
  }

  if (g_verbose_level >= 100) {
    print_text(YES, "-------------------------------------------------\n");
    print_text(YES, "---                 SUMMARY                   ---\n");
    print_text(YES, "-------------------------------------------------\n");
  }
  
  f = ((float)total_used_rom)/total_rom_size * 100.0f;
  print_text(YES, "ROM: %d bytes (%.2f%%) used.\n", total_used_rom, f);
  
  if (total_used_ram == 0)
    print_text(YES, "RAM: No .RAMSECTIONs were found, no information about RAM.\n");
  else
    print_text(YES, "RAM: %d bytes used.\n", total_used_ram);

  fflush(stderr);
  fflush(stdout);
  
  /*
  g_sec_tmp = g_sections_first;
  while (g_sec_tmp != NULL) {
    if (g_sec_tmp->status == SECTION_STATUS_HEADER) {
      print_text(YES, "Bank %d header section size %d.\n", g_sec_tmp->bank, g_sec_tmp->size);
      ind += g_sec_tmp->size;
    }
    g_sec_tmp = g_sec_tmp->next;
  }

  if (ind != 0) {
    print_text(YES, "Total %d additional bytes (from headers and footers).\n", ind);
    print_text(YES, "Total size %d bytes.\n", ind + g_max_address);
  }
  */
  
  return SUCCEEDED;
}


int show_project_information_library(void) {

  int total_used_ram = 0, total_used_rom = 0, total_rom_size = 0, i;
  struct section_def *s;
  float f;

  fflush(stderr);
  fflush(stdout);

  for (i = 0; i < g_rombanks; i++)
    total_rom_size += g_banks[i];

  s = g_sections_first;
  while (s != NULL) {
    int status = s->status;
      
    if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
        status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE)
      total_used_ram += s->size;
    else if (status == SECTION_STATUS_HEADER) {
    }
    else
      total_used_rom += s->size;
      
    s = s->next;
  }

  print_text(YES, "-------------------------------------------------\n");
  print_text(YES, "---                   ROM                     ---\n");
  print_text(YES, "-------------------------------------------------\n");

  s = g_sections_first;
  while (s != NULL) {
    int status = s->status;

    if (status == SECTION_STATUS_FREE || status == SECTION_STATUS_FORCE || status == SECTION_STATUS_OVERWRITE ||
        status == SECTION_STATUS_SEMIFREE || status == SECTION_STATUS_ABSOLUTE || status == SECTION_STATUS_SUPERFREE ||
        status == SECTION_STATUS_SEMISUBFREE || status == SECTION_STATUS_SEMISUPERFREE)
      print_text(YES, ".SECTION \"%s\" (%d bytes).\n", s->name, s->size);

    s = s->next;
  }

  print_text(YES, "-------------------------------------------------\n");
  print_text(YES, "---                   RAM                     ---\n");
  print_text(YES, "-------------------------------------------------\n");

  if (total_used_ram == 0)
    print_text(YES, "No .RAMSECTIONs were found, no information about RAM.\n");
  else {
    s = g_sections_first;
    while (s != NULL) {
      int status = s->status;

      if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
          status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE)
        print_text(YES, ".RAMSECTION \"%s\" (%d bytes).\n", s->name, s->size);

      s = s->next;
    }
  }

  print_text(YES, "-------------------------------------------------\n");
  print_text(YES, "---                 SUMMARY                   ---\n");
  print_text(YES, "-------------------------------------------------\n");

  f = ((float)total_used_rom)/total_rom_size * 100.0f;
  print_text(YES, "ROM: %d bytes (%.2f%%) used of total %d.\n", total_used_rom, f, total_rom_size);

  if (total_used_ram == 0)
    print_text(YES, "RAM: No .RAMSECTIONs were found, no information about RAM.\n");
  else
    print_text(YES, "RAM: %d bytes used.\n", total_used_ram);

  fflush(stderr);
  fflush(stdout);
  
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
      print_text(NO, "MEM_INSERT: Overflowed from section \"%s\"; Please send a bug report!\n", g_sec_tmp->name);
      return FAILED;
    }
    g_sec_tmp->data[g_sec_tmp->i] = x;
    g_sec_tmp->i++;
    s_pc_bank++;
    s_pc_full++;
    s_pc_slot++;
    return SUCCEEDED;
  }

  if (s_pc_bank >= g_banksize) {
    print_text(NO, "MEM_INSERT: Origin ($%x) overflows from bank (%d).\n", s_pc_bank, s_rom_bank);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (s_pc_full >= g_max_address) {
    print_text(NO, "MEM_INSERT: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", s_pc_full, g_max_address);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (s_pc_slot >= s_pc_slot_max) {
    print_text(NO, "MEM_INSERT: The current address ($%.4x) overflows from SLOT %d.\n", s_pc_slot, s_slot);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  if (g_rom_banks_usage_table[s_pc_full] != 0 && g_rom_banks[s_pc_full] != x && s_mem_insert_overwrite == OFF) {
    print_text(NO, "MEM_INSERT: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", g_rom_banks_usage_table[s_pc_full], s_pc_full, g_rom_banks[s_pc_full], x & 0xFF);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_banks_usage_table[s_pc_full] = 2;
  g_rom_banks[s_pc_full] = x;
  s_pc_bank++;
  s_pc_full++;
  s_pc_slot++;

  return SUCCEEDED;
}


int mem_insert_padding(void) {

  /* we'll use this function to reserve space for writes that take place later; e.g., we know that here will be
     written an 8-bit value, but we don't know the numeric value itself yet, but we'll need to reserve the
     space for the upcoming write or otherwise something else might get written here */

  if (g_section_status == ON) {
    g_sec_tmp->data[g_sec_tmp->i] = 0xCD;
    g_sec_tmp->i++;
    s_pc_bank++;
    s_pc_full++;
    s_pc_slot++;
    return SUCCEEDED;
  }

  if (s_pc_bank >= g_banksize) {
    print_text(NO, "MEM_INSERT_PADDING: Origin ($%x) overflows from bank (%d).\n", s_pc_bank, s_rom_bank);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (s_pc_full >= g_max_address) {
    print_text(NO, "MEM_INSERT_PADDING: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", s_pc_full, g_max_address);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  else if (s_pc_slot >= s_pc_slot_max) {
    print_text(NO, "MEM_INSERT_PADDING: The current address ($%.4x) overflows from SLOT %d.\n", s_pc_slot, s_slot);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  /* announce the overwrite later */
  if (!(g_rom_banks_usage_table[s_pc_full] != 0 && s_mem_insert_overwrite == OFF))
    g_rom_banks_usage_table[s_pc_full] = 1;

  s_pc_bank++;
  s_pc_full++;
  s_pc_slot++;

  return SUCCEEDED;
}


int mem_insert_absolute(int add, unsigned char x) {

  if (add >= g_max_address) {
    print_text(NO, "MEM_INSERT_ABSOLUTE: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", add, g_max_address);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  if (g_rom_banks_usage_table[add] > 1 && g_rom_banks[add] != x && s_mem_insert_overwrite == OFF) {
    print_text(NO, "MEM_INSERT_ABSOLUTE: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", g_rom_banks_usage_table[add], add, g_rom_banks[add], x & 0xFF);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
  }
  
  g_rom_banks_usage_table[add]++;
  g_rom_banks[add] = x;

  return SUCCEEDED;
}


int export_definitions(FILE *final_ptr) {

  struct export_def *export_tmp;
  struct definition *tmp_def;
  unsigned char *cp;
  double dou;
  int ov;

  ov = 0;
  export_tmp = g_export_first;
  while (export_tmp != NULL) {
    hashmap_get(g_defines_map, export_tmp->name, (void*)&tmp_def);
    if (tmp_def != NULL) {
      if (tmp_def->type == DEFINITION_TYPE_VALUE)
        ov++;
      if (tmp_def->type == DEFINITION_TYPE_STACK)
        ov++;
    }

    export_tmp = export_tmp->next;
  }

  WRITEOUT_OV;

  export_tmp = g_export_first;
  while (export_tmp != NULL) {
    hashmap_get(g_defines_map, export_tmp->name, (void*)&tmp_def);

    if (tmp_def == NULL)
      print_text(NO, "WARNING: Trying to export an unknown definition \"%s\".\n", export_tmp->name);
    else {
      if (tmp_def->type == DEFINITION_TYPE_VALUE) {
        fprintf(final_ptr, "%s%c", tmp_def->alias, 0x0);
        dou = tmp_def->value;
        WRITEOUT_DOU;
      }
      else if (tmp_def->type == DEFINITION_TYPE_STRING)
        print_text(NO, "INTERNAL_PHASE_2: Definition \"%s\" is a string definition, and it cannot be exported.\n", export_tmp->name);
      else if (tmp_def->type == DEFINITION_TYPE_STACK) {
        fprintf(final_ptr, "%s%c", tmp_def->alias, 0x1);
        dou = tmp_def->value;
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

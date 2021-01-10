
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


extern struct section_def *sections_first, *sections_last, *sec_tmp, *sec_next;
extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;
extern struct export_def *export_first, *export_last;
extern struct stack *stacks_first, *stacks_tmp, *stacks_last, *stacks_header_first, *stacks_header_last;
extern struct label_def *label_tmp, *label_last, *labels;
extern struct definition *tmp_def;
extern struct map_t *defines_map;
extern struct map_t *namespace_map;
extern struct map_t *global_unique_label_map;
extern struct file_name_info *file_name_info_first;
extern struct slot slots[256];
extern struct append_section *append_sections;
extern struct label_sizeof *label_sizeofs;
extern FILE *file_out_ptr;
extern unsigned char *rom_banks, *rom_banks_usage_table;
extern char *tmp_name, tmp[4096], name[32], *final_name;
extern int rombanks, ind, inz, output_format, test_mode, listfile_data, little_endian;

#ifdef GB
extern char licenseecodenew_c1, licenseecodenew_c2;
extern int computechecksum_defined, computecomplementcheck_defined;
extern int cartridgetype;
extern int name_defined, computechecksum_defined, computecomplementcheck_defined;
extern int licenseecodeold_defined, licenseecodenew_defined;
extern int licenseecodeold, romgbc;
#endif

extern int romtype, rambanks, emptyfill, max_address;
extern int rambanks_defined, verbose_mode;
extern int section_status;
extern int banksize, banksize_defined;
extern int slots_amount;
extern int *banks, *bankaddress, rombankmap_defined;
extern int latest_stack, stacks_outside, stacks_inside;
extern int bankheader_status;
extern int smc_defined, makefile_rules;

#ifdef W65816
extern int lorom_defined, hirom_defined, slowrom_defined, fastrom_defined, snes_mode, exhirom_defined;
extern int computesneschecksum_defined, sramsize, sramsize_defined, exlorom_defined;
#endif

#ifdef Z80
extern int computesmschecksum_defined, smstag_defined, smsheader_defined;
#endif

struct label_def *unknown_labels = NULL, *unknown_labels_last = NULL, *tul, *ltmp;
struct label_def *unknown_header_labels = NULL, *unknown_header_labels_last = NULL;
struct label_def *parent_labels[10];
struct append_section *append_tmp;
struct label_sizeof *label_sizeof_tmp;

char mem_insert_action[MAX_NAME_LENGTH*3 + 1024], namespace[MAX_NAME_LENGTH + 1];

int pc_bank = 0, pc_full = 0, rom_bank, mem_insert_overwrite, slot = 0, base = 0, pc_slot, pc_slot_max;
int filename_id, line_number;

static int dstruct_start = -1, special_id = 0;


#define WRITEOUT_OV fprintf(final_ptr, "%c%c%c%c", (ov>>24)&0xFF, (ov>>16)&0xFF, (ov>>8)&0xFF, ov&0xFF);
#define WRITEOUT_DOU {                                                  \
    cp = (unsigned char *)&dou;                                         \
    fprintf(final_ptr, "%c%c%c%c%c%c%c%c", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]); \
  }

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")



int new_unknown_reference(int type) {

  struct label_def *label;
  int n = 0, j = 0;

  if (tmp[0] == ':')
    j = 1;
  while (n < 10 && tmp[n+j] == '@')
    n++;
  n--;
  while (n >= 0 && parent_labels[n] == NULL)
    n--;
  if (n >= 0) {
    if (mangle_label(&tmp[j], parent_labels[n]->label, n, MAX_NAME_LENGTH-j) == FAILED)
      return FAILED;
  }

  label = calloc(sizeof(struct label_def), 1);
  if (label == NULL) {
    fprintf(stderr, "%s:%d: NEW_UNKNOWN_REFERENCE: Out of memory.\n", get_file_name(filename_id), line_number);
    return FAILED;
  }

  label->symbol = NO;

  strcpy(label->label, tmp);
  label->next = NULL;
  label->type = type;
  label->filename_id = filename_id;
  label->linenumber = line_number;
  label->section_status = section_status;
  if (section_status == ON) {
    label->section_id = sec_tmp->id;
    label->section_struct = sec_tmp;
    /* relative address, to the beginning of the section */
    label->address = sec_tmp->i;
  }
  else {
    label->section_id = 0;
    label->section_struct = NULL;
    /* bank address, in ROM memory */
    label->address = pc_bank;
  }

  label->bank = rom_bank;
  label->slot = slot;
  label->base = base;
  label->special_id = special_id;

  /* outside bank header section */
  if (bankheader_status == OFF) {
    if (unknown_labels_last == NULL) {
      unknown_labels = label;
      unknown_labels_last = label;
    }
    else {
      unknown_labels_last->next = label;
      unknown_labels_last = label;
    }
  }
  /* bank header section */
  else {
    if (label->label[0] == '_') {
      fprintf(stderr, "%s:%d: NEW_UNKNOWN_REFERENCE: Referring to a local label (\"%s\") from inside a bank header section is not allowed.\n",
              get_file_name(filename_id), line_number, label->label);
      return FAILED;
    }
    if (unknown_header_labels_last == NULL) {
      unknown_header_labels = label;
      unknown_header_labels_last = label;
    }
    else {
      unknown_header_labels_last->next = label;
      unknown_header_labels_last = label;
    }
  }

  return SUCCEEDED;
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
      while (n >= 0 && parent_labels[n] == NULL)
        n--;

      if (n >= 0) {
        if (mangle_label(&stack->stack[ind].string[j], parent_labels[n]->label, n, MAX_NAME_LENGTH-j) == FAILED)
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
  if (section_status == ON && sec_tmp != NULL && sec_tmp->nspace != NULL) {
    if (hashmap_get(sec_tmp->nspace->label_map, expanded, (void*)&l) == MAP_OK) {
      if (filename_id == l->filename_id) {
        strcpy(label, expanded);
        return SUCCEEDED;
      }
    }
  }

  /* global label? */
  if (hashmap_get(global_unique_label_map, expanded, (void*)&l) == MAP_OK) {
    if (filename_id == l->filename_id) {
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


int pass_4(void) {

  unsigned char *cp;
  FILE *final_ptr;
  double dou;
  char *t, c;
  int i, o, z, y, add_old = 0, x, q, ov, inside_macro = 0, inside_repeat = 0;
  float f;

  memset(parent_labels, 0, sizeof(parent_labels));

  namespace[0] = 0;
  
  section_status = OFF;
  bankheader_status = OFF;
  mem_insert_overwrite = OFF;

  if (verbose_mode == ON)
    printf("Internal pass 2...\n");

  if ((file_out_ptr = fopen(tmp_name, "rb")) == NULL) {
    fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\".\n", tmp_name);
    return FAILED;
  }

  while (fread(&c, 1, 1, file_out_ptr) != 0) {
    switch (c) {

      /* SPECIAL CASE ID */
      
    case 'v':
      fscanf(file_out_ptr, "%d ", &special_id);       
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
      fscanf(file_out_ptr, "%*s ");
      inside_macro++;
      continue;
    case 'I':
      fscanf(file_out_ptr, "%*s ");
      inside_macro--;
      continue;
        
    case 'g':
      fscanf(file_out_ptr, "%*s ");
      continue;
    case 'G':
      continue;

    case 'f':
      fscanf(file_out_ptr, "%d ", &filename_id);
      continue;

    case 'k':
      fscanf(file_out_ptr, "%d ", &line_number);
      continue;

    case 't':
      fscanf(file_out_ptr, "%d ", &inz);
      if (inz == 0)
        namespace[0] = 0;
      else
        fscanf(file_out_ptr, STRING_READ_FORMAT, namespace);
      continue;
        
      /* SECTION */

    case 'A':
    case 'S':
      if (c == 'A')
        fscanf(file_out_ptr, "%d %d ", &x, &ind);
      else
        fscanf(file_out_ptr, "%d ", &x);

      add_old = pc_bank;

      sec_tmp = sections_first;
      while (sec_tmp != NULL) {
        if (sec_tmp->id == x)
          break;
        sec_tmp = sec_tmp->next;
      }

      /* skip all dead sections */
      if (sec_tmp->alive == OFF)
        continue;

      if (c == 'A')
        sec_tmp->address = ind;

      ind = 0;
      if (sec_tmp->maxsize_status == ON) {
        if (sec_tmp->maxsize < sec_tmp->size) {
          fprintf(stderr, "%s: INTERNAL_PASS_2: Section \"%s\" doesn't fit into the specified %d bytes. Enlarging to %d bytes.\n",
                  get_file_name(filename_id), sec_tmp->name, sec_tmp->maxsize, sec_tmp->size);
        }
        else if (sec_tmp->size < sec_tmp->maxsize) {
          sec_tmp->size = sec_tmp->maxsize;
          ind = 1;
        }
      }

      sec_tmp->data = calloc(sizeof(unsigned char) * sec_tmp->size, 1);
      if (sec_tmp->data == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Out of memory when trying to allocate room for section \"%s\".\n",
                get_file_name(filename_id), line_number, sec_tmp->name);
        return FAILED;
      }

      /* fill the padded area with _emptyfill_ */
      if (ind == 1)
        memset(sec_tmp->data, emptyfill, sec_tmp->size);

      if (strcmp(sec_tmp->name, "BANKHEADER") == 0)
        bankheader_status = ON;

      sec_tmp->i = 0;
      section_status = ON;

      continue;

      /* ENDS */

    case 's':
      section_status = OFF;
      bankheader_status = OFF;

      /* some sections don't affect the ORG outside of them */
      if (sec_tmp->advance_org == NO) {
        pc_bank = add_old;
        pc_full = bankaddress[rom_bank] + pc_bank;
        pc_slot = slots[slot].address + pc_bank;
        pc_slot_max = slots[slot].address + slots[slot].size;
      }

      continue;

      /* DSB & DSW & DSL */
      /* ('o' skips over memory without claiming it) */

    case 'x':
    case 'o':
      fscanf(file_out_ptr, "%d %d", &ind, &x);

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing DSB data", get_file_name(filename_id), line_number);

      if (ind < 0) { /* going backward */
        if (section_status == ON)
          sec_tmp->i += ind;
        pc_bank += ind;
        pc_full += ind;
        pc_slot += ind;
        ind++;
      }
      else {
        while (ind > 0) {
          if (c == 'o') {
            if (section_status == ON)
              sec_tmp->i++;
            pc_bank++;
            pc_full++;
            pc_slot++;
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
      fscanf(file_out_ptr, "%d %d", &ind, &inz);
      i = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing DSW data", get_file_name(filename_id), line_number);

      while (ind > 0) {
        if (little_endian == YES) {
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

#ifdef W65816

    case 'h':
      fscanf(file_out_ptr, "%d %d", &ind, &inz);
      x = inz & 0xFF;
      i = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing DSL data", get_file_name(filename_id), line_number);

      while (ind > 0) {
        if (little_endian == YES) {
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

#endif
        
      /* DATA & OPTCODE */

    case 'd':
      fscanf(file_out_ptr, "%d", &x);

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing a byte", get_file_name(filename_id), line_number);
        
      if (mem_insert(x) == FAILED)
        return FAILED;

      continue;

    case 'y':
      fscanf(file_out_ptr, "%d", &inz);
      x = inz & 0xFF;
      inz = (inz >> 8) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing two bytes", get_file_name(filename_id), line_number);

      if (little_endian == YES) {
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
      fscanf(file_out_ptr, "%d", &base);
      continue;

#ifdef W65816

    case 'z':
      fscanf(file_out_ptr, "%d", &inz);
      x = inz & 0xFF;
      ind = (inz >> 8) & 0xFF;
      inz = (inz >> 16) & 0xFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing three bytes", get_file_name(filename_id), line_number);

      if (little_endian == YES) {
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

#endif

      /* DATA BLOCK from .INCBIN */

    case 'D':
      fscanf(file_out_ptr, "%d %d %d %d", &x, &inz, &z, &y);

      ifd_tmp = incbin_file_data_first;
      for (ind = 0; ind != x; ind++)
        ifd_tmp = ifd_tmp->next;
      t = ifd_tmp->data + z;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing .INCBIN data", get_file_name(filename_id), line_number);

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
        fscanf(file_out_ptr, "%d", &pc_bank);
      else {
        fscanf(file_out_ptr, "%d %d", &rom_bank, &slot);
        if (banksize_defined == 0)
          banksize = banks[rom_bank];
      }

      pc_full = bankaddress[rom_bank] + pc_bank;
      pc_slot = slots[slot].address + pc_bank;
      pc_slot_max = slots[slot].address + slots[slot].size;

      continue;

      /* BREAKPOINT */

    case 'Z':
      continue;

      /* SYMBOL */

    case 'Y':
      /* skip the symbol */
      fscanf(file_out_ptr, "%*s");
      continue;

      /* LABEL */
    case 'L':
      {
        struct label_def *l;
        int m, n = 0, mangled_label = NO;

        fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

        if (is_label_anonymous(tmp) == NO) {
          while (n < 10 && tmp[n] == '@')
            n++;

          m = n + 1;
          while (m < 10)
            parent_labels[m++] = NULL;

          m = n;
          n--;
          while (n >= 0 && parent_labels[n] == NULL)
            n--;

          if (n >= 0) {
            if (mangle_label(tmp, parent_labels[n]->label, n, MAX_NAME_LENGTH) == FAILED)
              return FAILED;
            mangled_label = YES;
          }

          if (namespace[0] != 0 && mangled_label == NO) {
            if (section_status == OFF || sec_tmp->nspace == NULL) {
              if (add_namespace(tmp, namespace, sizeof(tmp)) == FAILED)
                return FAILED;
            }
          }
            
          if (m < 10 && find_label(tmp, sec_tmp, (void*)&l) == SUCCEEDED)
            parent_labels[m] = l;
        }
      }
      continue;

      /* 8BIT COMPUTATION */

    case 'c':
      fscanf(file_out_ptr, "%d", &inz);

      if (bankheader_status == OFF)
        stacks_tmp = stacks_first;
      else
        stacks_tmp = stacks_header_first;

      while (stacks_tmp != NULL) {
        if (stacks_tmp->id == inz)
          break;
        stacks_tmp = stacks_tmp->next;
      }

      if (stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(filename_id), line_number, inz);
        return FAILED;
      }

      if (stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        stacks_tmp->address = sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        stacks_tmp->address = pc_bank;
      }

      stacks_tmp->bank = rom_bank;
      stacks_tmp->slot = slot;
      stacks_tmp->type = STACK_TYPE_8BIT;
      stacks_tmp->base = base;
      stacks_tmp->special_id = special_id;
        
      if (mangle_stack_references(stacks_tmp) == FAILED)
        return FAILED;

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(stacks_tmp, namespace) == FAILED)
            return FAILED;
        }
      }

      /* this stack was referred from the code */
      stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for an 8-bit computation", get_file_name(filename_id), line_number);
        
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 16BIT COMPUTATION */

    case 'C':
      fscanf(file_out_ptr, "%d", &inz);

      if (bankheader_status == OFF)
        stacks_tmp = stacks_first;
      else
        stacks_tmp = stacks_header_first;

      while (stacks_tmp != NULL) {
        if (stacks_tmp->id == inz)
          break;
        stacks_tmp = stacks_tmp->next;
      }

      if (stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(filename_id), line_number, inz);
        return FAILED;
      }

      if (stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        stacks_tmp->address = sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        stacks_tmp->address = pc_bank;
      }

      stacks_tmp->bank = rom_bank;
      stacks_tmp->slot = slot;
      stacks_tmp->type = STACK_TYPE_16BIT;
      stacks_tmp->base = base;
        
      if (mangle_stack_references(stacks_tmp) == FAILED)
        return FAILED;

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(stacks_tmp, namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 16-bit computation", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

#ifdef SPC700
      /* 13BIT COMPUTATION */

    case 'N':
      fscanf(file_out_ptr, "%d %d", &x, &inz);

      if (bankheader_status == OFF)
        stacks_tmp = stacks_first;
      else
        stacks_tmp = stacks_header_first;

      while (stacks_tmp != NULL) {
        if (stacks_tmp->id == inz)
          break;
        stacks_tmp = stacks_tmp->next;
      }

      if (stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(filename_id), line_number, inz);
        return FAILED;
      }

      if (stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        stacks_tmp->address = sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        stacks_tmp->address = pc_bank;
      }

      stacks_tmp->bank = rom_bank;
      stacks_tmp->slot = slot;
      stacks_tmp->type = STACK_TYPE_13BIT;
      stacks_tmp->base = base;
        
      if (mangle_stack_references(stacks_tmp) == FAILED)
        return FAILED;

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(stacks_tmp, namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 13-bit computation", get_file_name(filename_id), line_number);

      if (mem_insert(0x00) == FAILED)
        return FAILED;
      if (mem_insert(x << (13 - 8)) == FAILED)
        return FAILED;

      continue;
#endif

      /* 24BIT COMPUTATION */

    case 'T':
      fscanf(file_out_ptr, "%d", &inz);

      if (bankheader_status == OFF)
        stacks_tmp = stacks_first;
      else
        stacks_tmp = stacks_header_first;

      while (stacks_tmp != NULL) {
        if (stacks_tmp->id == inz)
          break;
        stacks_tmp = stacks_tmp->next;
      }

      if (stacks_tmp == NULL) {
        fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Could not find computation stack number %d. WLA corruption detected. Please send a bug report!\n", get_file_name(filename_id), line_number, inz);
        return FAILED;
      }

      if (stacks_tmp->section_status == ON) {
        /* relative address, to the beginning of the section */
        stacks_tmp->address = sec_tmp->i;
      }
      else {
        /* complete address, in ROM memory */
        stacks_tmp->address = pc_bank;
      }

      stacks_tmp->bank = rom_bank;
      stacks_tmp->slot = slot;
      stacks_tmp->type = STACK_TYPE_24BIT;
      stacks_tmp->base = base;

      if (mangle_stack_references(stacks_tmp) == FAILED)
        return FAILED;

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_stack_references(stacks_tmp, namespace) == FAILED)
            return FAILED;
        }
      }
        
      /* this stack was referred from the code */
      stacks_tmp->position = STACK_POSITION_CODE;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 24-bit computation", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 24BIT REFERENCE */

    case 'q':
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }
          
      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing a 24-bit reference", get_file_name(filename_id), line_number);

          if (little_endian == YES) {
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

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_24BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 24-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 16BIT PC RELATIVE REFERENCE */

    case 'M':
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }
                
      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(filename_id), line_number);

          if (little_endian == YES) {
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

      if (new_unknown_reference(REFERENCE_TYPE_RELATIVE_16BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 16BIT REFERENCE */

    case 'r':
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing a 16-bit reference", get_file_name(filename_id), line_number);

          if (little_endian == YES) {
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

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_16BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 16-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;
      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

#ifdef SPC700
      /* 13BIT REFERENCE */

    case 'n':
      fscanf(file_out_ptr, "%d ", &inz);
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;
          if (o > 8191 || o < 0) {
            fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference value of \"%s\" is out of 13-bit range.\n", get_file_name(filename_id), line_number, tmp);
            return FAILED;
          }

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing a 13-bit reference", get_file_name(filename_id), line_number);

          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
          if (mem_insert(((o | inz << 13) & 0xFF00) >> 8) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_13BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for a 13-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert(0x00) == FAILED)
        return FAILED;
      if (mem_insert(inz << (13 - 8)) == FAILED)
        return FAILED;

      continue;
#endif

      /* 8BIT PC RELATIVE REFERENCE */

    case 'R':
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(filename_id), line_number);

          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_RELATIVE_8BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* 8BIT REFERENCE */

    case 'Q':
      fscanf(file_out_ptr, STRING_READ_FORMAT, tmp);

      if (namespace[0] != 0) {
        if (section_status == OFF || sec_tmp->nspace == NULL) {
          if (add_namespace_to_reference(tmp, namespace, sizeof(tmp)) == FAILED)
            return FAILED;
        }
      }

      x = 0;
      hashmap_get(defines_map, tmp, (void*)&tmp_def);
      if (tmp_def != NULL) {
        if (tmp_def->type == DEFINITION_TYPE_STRING) {
          fprintf(stderr, "%s:%d: INTERNAL_PASS_2: Reference to a string definition \"%s\"?\n", get_file_name(filename_id), line_number, tmp);
          return FAILED;
        }
        else if (tmp_def->type != DEFINITION_TYPE_STACK) {
          o = (int)tmp_def->value;
          x = 1;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Writing an 8-bit reference", get_file_name(filename_id), line_number);
            
          if (mem_insert(o & 0xFF) == FAILED)
            return FAILED;
        }
      }

      if (x == 1)
        continue;

      if (new_unknown_reference(REFERENCE_TYPE_DIRECT_8BIT) == FAILED)
        return FAILED;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s:%d: Inserting padding for an 8-bit reference", get_file_name(filename_id), line_number);

      if (mem_insert_padding() == FAILED)
        return FAILED;

      continue;

      /* .DSTRUCT stuff */

    case 'e':
      fscanf(file_out_ptr, "%d %d ", &x, &y);
      if (y == -1) {
        /* mark start of .DSTRUCT */
        dstruct_start = pc_full;
        /* make sure all data in a section gets set to emptyfill */
        if (section_status == ON)
          memset(sec_tmp->data + sec_tmp->i, emptyfill, x);
      }
      else if (y == -2) {
        /* end of .DSTRUCT. make sure all memory is claimed. */
        while (pc_full < dstruct_start + x) {
          if (section_status == OFF && rom_banks_usage_table[pc_full] == 0) {
            rom_banks_usage_table[pc_full] = 2;
            rom_banks[pc_full] = emptyfill;
          }
          if (section_status == ON)
            sec_tmp->i++;
          pc_bank++;
          pc_slot++;
          pc_full++;
        }
      }
      else {
        /* seek offset relative to dstruct start */
        if (section_status == ON)
          sec_tmp->i = sec_tmp->i + (dstruct_start - pc_full) + x;
        pc_bank = pc_bank + (dstruct_start - pc_full) + x;
        pc_slot = pc_slot + (dstruct_start - pc_full) + x;
        pc_full = dstruct_start + x;
      }
      continue;
    }
  }

  fclose(file_out_ptr);
  file_out_ptr = NULL;

  /* library file output */
  if (output_format == OUTPUT_LIBRARY && test_mode == OFF) {
    if ((final_ptr = fopen(final_name, "wb")) == NULL) {
      fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\" for writing.\n", final_name);
      return FAILED;
    }

    /* header */
    fprintf(final_ptr, "WLA9");

    /* misc bits */
    ind = 0;

    if (little_endian == NO)
      ind |= 1 << 0;
#ifdef W65816
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
    label_tmp = labels;
    while (label_tmp != NULL) {
      if (label_tmp->alive == ON)
        ov++;
      label_tmp = label_tmp->next;
    }

    WRITEOUT_OV;

    label_tmp = labels;
    while (label_tmp != NULL) {
      if (label_tmp->alive == ON) {
        if (label_tmp->symbol != 2)
          fprintf(final_ptr, "%s", label_tmp->label);
        fprintf(final_ptr, "%c", label_tmp->symbol);

        ov = label_tmp->section_id;
        WRITEOUT_OV;

        fprintf(final_ptr, "%c", label_tmp->filename_id);

        ov = label_tmp->linenumber;
        WRITEOUT_OV;

        ov = label_tmp->address;
        WRITEOUT_OV;
      }
      label_tmp = label_tmp->next;
    }

    /* unknown labels (outside references) */
    ov = 0;
    label_tmp = unknown_labels;
    while (label_tmp != NULL) {
      ov++;
      label_tmp = label_tmp->next;
    }

    WRITEOUT_OV;

    label_tmp = unknown_labels;
    while (label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c", label_tmp->label, 0x0, label_tmp->type, label_tmp->special_id);

      ov = label_tmp->section_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c", label_tmp->filename_id);

      if (label_tmp->section_status == OFF) {
        fprintf(stderr, "INTERNAL_PASS_2: Label \"%s\" is outside all sections.\n", label_tmp->label);
        return FAILED;
      }

      ov = label_tmp->linenumber;
      WRITEOUT_OV;

      ov = label_tmp->address; /* + (label_tmp->base << 16); */
      WRITEOUT_OV;

      label_tmp = label_tmp->next;
    }

    /* pending calculations */
    ov = stacks_outside;
    WRITEOUT_OV;

    stacks_tmp = stacks_first;
    while (stacks_tmp != NULL) {
      ov = stacks_tmp->id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", stacks_tmp->type | (stacks_tmp->relative_references << 7), stacks_tmp->special_id);

      ov = stacks_tmp->section_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c%c", stacks_tmp->filename_id, stacks_tmp->stacksize, stacks_tmp->position);

      ov = stacks_tmp->address;
      WRITEOUT_OV;

      ov = stacks_tmp->linenumber;
      WRITEOUT_OV;

      for (ind = 0; ind < stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", stacks_tmp->stack[ind].type, stacks_tmp->stack[ind].sign);
        if (stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", stacks_tmp->stack[ind].string, 0);
        else {
          dou = stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      stacks_tmp = stacks_tmp->next;
    }

    /* label sizeofs */
    ov = 0;
    label_sizeof_tmp = label_sizeofs;
    while (label_sizeof_tmp != NULL) {
      ov++;
      label_sizeof_tmp = label_sizeof_tmp->next;
    }

    WRITEOUT_OV;

    label_sizeof_tmp = label_sizeofs;
    while (label_sizeof_tmp != NULL) {
      fprintf(final_ptr, "%s%c", label_sizeof_tmp->name, 0);

      ov = label_sizeof_tmp->size;
      WRITEOUT_OV;

      label_sizeof_tmp = label_sizeof_tmp->next;
    }
    
    /* append sections */
    ov = 0;
    append_tmp = append_sections;
    while (append_tmp != NULL) {
      ov++;
      append_tmp = append_tmp->next;
    }
    WRITEOUT_OV;

    append_tmp = append_sections;
    while (append_tmp != NULL) {
      fprintf(final_ptr, "%s%c", append_tmp->section, 0);
      fprintf(final_ptr, "%s%c", append_tmp->append_to, 0);
      append_tmp = append_tmp->next;
    }

    /* sections */
    sec_tmp = sections_first;
    while (sec_tmp != NULL) {
      if (sec_tmp->alive == ON) {
        fprintf(final_ptr, "%s%c%c", sec_tmp->name, sec_tmp->status, sec_tmp->keep);
        if (sec_tmp->nspace == NULL)
          fprintf(final_ptr, "%c", 0);
        else
          fprintf(final_ptr, "%s%c", sec_tmp->nspace->name, 0);

        ov = sec_tmp->id;
        WRITEOUT_OV;

        fprintf(final_ptr, "%c", sec_tmp->filename_id);

        ov = sec_tmp->size;
        WRITEOUT_OV;
        ov = sec_tmp->alignment;
        WRITEOUT_OV;
        ov = sec_tmp->offset;
        WRITEOUT_OV;
        ov = sec_tmp->priority;
        WRITEOUT_OV;
        
        fwrite(sec_tmp->data, 1, sec_tmp->size, final_ptr);

        if (listfile_data == YES && sec_tmp->listfile_items > 0)
          listfile_block_write(final_ptr, sec_tmp);
        else
          fprintf(final_ptr, "%c", 0);
      }
      sec_tmp = sec_tmp->next;
    }

    fclose(final_ptr);
  }

  /* object file output */
  else if (output_format == OUTPUT_OBJECT && test_mode == OFF) {
    if ((final_ptr = fopen(final_name, "wb")) == NULL) {
      fprintf(stderr, "INTERNAL_PASS_2: Error opening file \"%s\" for writing.\n", final_name);
      return FAILED;
    }

    /* header */
    fprintf(final_ptr, "WLAa%c", emptyfill);

    /* misc bits */
    ind = 0;

#ifdef Z80
    if (computesmschecksum_defined != 0)
      ind |= 1 << 0;
#endif

#ifdef W65816
    if (snes_mode != 0) {
      if (hirom_defined != 0)
        ind += 2;
      if (fastrom_defined != 0)
        ind += 8;
      if (computesneschecksum_defined != 0)
        ind += 64;
      /* use snes banking scheme */
      if (hirom_defined != 0 || lorom_defined != 0 || exhirom_defined != 0 || exlorom_defined != 0)
        ind += 4;
    }
    /* 65816 bit */
    ind += 128;
#endif

#ifdef GB
    if (computechecksum_defined != 0)
      ind += 16;
    if (computecomplementcheck_defined != 0)
      ind += 32;
#endif

    fprintf(final_ptr, "%c", ind);

    /* more bits */
    ind = 0;

    if (smc_defined != 0)
      ind |= 1 << 0;
#ifdef W65816
    if (sramsize_defined != 0)
      ind |= (sramsize & 3) << 1;
#endif

#ifdef Z80
    if (smstag_defined != 0)
      ind |= 1 << 3;
    if (smsheader_defined != 0)
      ind |= 1 << 4;
#endif

#ifdef W65816
    if (snes_mode != 0) {
      if (exhirom_defined != 0)
        ind |= 1 << 5;
      if (exlorom_defined != 0)
        ind |= 1 << 6;
    }
#endif

    if (little_endian == NO)
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
    ov = rombanks;
    WRITEOUT_OV;                                   /* number of rom banks */

    if (rombankmap_defined != 0) {
      fprintf(final_ptr, "%c", 1);                 /* status */
      for (i = 0; i < rombanks; i++) {
        ov = banks[i];
        WRITEOUT_OV;                               /* banksize */
      }
    }
    else {
      fprintf(final_ptr, "%c", 0);                 /* status */
      ov = banksize;
      WRITEOUT_OV;                                 /* banksize */
    }

    /* memory map */
    fprintf(final_ptr, "%c", slots_amount);        /* number of slots */

    for (i = 0; i < slots_amount; i++) {
      if (slots[i].size != 0) {
        ov = slots[i].address;
        WRITEOUT_OV;                               /* slot address */
        ov = slots[i].size;
        WRITEOUT_OV;                               /* slot size */
        if (slots[i].name[0] == 0x0)               /* slot name */
          fprintf(final_ptr, "%c", 0x0);
        else
          fprintf(final_ptr, "%s%c", slots[i].name, 0x0);
      }
    }

    /* source file names */
    if (export_source_file_names(final_ptr) == FAILED)
      return FAILED;

    /* export definitions */
    if (export_definitions(final_ptr) == FAILED)
      return FAILED;

    /* labels */
    ov = 0;
    label_tmp = labels;
    while (label_tmp != NULL) {
      if (label_tmp->alive == ON)
        ov++;
      label_tmp = label_tmp->next;
    }

    /* number of labels */
    WRITEOUT_OV;

    label_tmp = labels;
    while (label_tmp != NULL) {
      if (label_tmp->alive == ON) {
        if (label_tmp->symbol != 2)
          fprintf(final_ptr, "%s", label_tmp->label);
        fprintf(final_ptr, "%c", label_tmp->symbol);

        fprintf(final_ptr, "%c%c", label_tmp->slot, label_tmp->filename_id);

        ov = label_tmp->section_id;
        WRITEOUT_OV;

        /* DEBUG
           fprintf(stderr, "LABEL: \"%s\" SLOT: %d LINE: %d\n", label_tmp->label, label_tmp->slot, label_tmp->linenumber);
        */

        ov = label_tmp->address;
        WRITEOUT_OV;

        ov = label_tmp->linenumber;
        WRITEOUT_OV;

        ov = label_tmp->bank;
        WRITEOUT_OV;

        ov = label_tmp->base;
        WRITEOUT_OV;
      }
      label_tmp = label_tmp->next;
    }

    /* outside references */
    ov = 0;
    label_tmp = unknown_labels;
    while (label_tmp != NULL) {
      ov++;
      label_tmp = label_tmp->next;
    }
    label_tmp = unknown_header_labels;
    while (label_tmp != NULL) {
      ov++;
      label_tmp = label_tmp->next;
    }

    WRITEOUT_OV;

    label_tmp = unknown_labels;
    while (label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c%c%c", label_tmp->label, 0x0, label_tmp->type, label_tmp->special_id, label_tmp->filename_id, label_tmp->slot);

      ov = label_tmp->section_id;
      WRITEOUT_OV;

      ov = label_tmp->linenumber;
      WRITEOUT_OV;

      ov = label_tmp->address;
      WRITEOUT_OV;

      ov = label_tmp->bank;
      WRITEOUT_OV;

      ov = label_tmp->base;
      WRITEOUT_OV;

      label_tmp = label_tmp->next;
    }

    label_tmp = unknown_header_labels;
    while (label_tmp != NULL) {
      fprintf(final_ptr, "%s%c%c%c%c%c", label_tmp->label, 0x0, label_tmp->type, 0, label_tmp->filename_id, label_tmp->slot);

      ov = label_tmp->section_id;
      WRITEOUT_OV;

      ov = label_tmp->linenumber;
      WRITEOUT_OV;

      ov = label_tmp->address;
      WRITEOUT_OV;

      ov = label_tmp->bank;
      WRITEOUT_OV;

      ov = label_tmp->base;
      WRITEOUT_OV;
      
      label_tmp = label_tmp->next;
    }

    /* pending calculations */
    ov = stacks_outside + stacks_inside;
    WRITEOUT_OV;

    stacks_tmp = stacks_first;
    while (stacks_tmp != NULL) {
      ov = stacks_tmp->id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c", stacks_tmp->type | (stacks_tmp->relative_references << 7), stacks_tmp->special_id);

      ov = stacks_tmp->section_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c%c%c", stacks_tmp->filename_id, stacks_tmp->stacksize, stacks_tmp->position, stacks_tmp->slot);

      ov = stacks_tmp->address;
      WRITEOUT_OV;

      ov = stacks_tmp->linenumber;
      WRITEOUT_OV;

      ov = stacks_tmp->bank;
      WRITEOUT_OV;

      ov = stacks_tmp->base;
      WRITEOUT_OV;
      
      for (ind = 0; ind < stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", stacks_tmp->stack[ind].type, stacks_tmp->stack[ind].sign);
        if (stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", stacks_tmp->stack[ind].string, 0);
        else {
          dou = stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      stacks_tmp = stacks_tmp->next;
    }

    stacks_tmp = stacks_header_first;
    while (stacks_tmp != NULL) {
      fprintf(final_ptr, "%c", stacks_tmp->type);

      ov = stacks_tmp->section_id;
      WRITEOUT_OV;

      fprintf(final_ptr, "%c%c%c%c", stacks_tmp->filename_id, stacks_tmp->stacksize, stacks_tmp->position, stacks_tmp->slot);

      ov = stacks_tmp->address;
      WRITEOUT_OV;

      ov = stacks_tmp->linenumber;
      WRITEOUT_OV;

      ov = stacks_tmp->bank;
      WRITEOUT_OV;

      for (ind = 0; ind < stacks_tmp->stacksize; ind++) {
        fprintf(final_ptr, "%c%c", stacks_tmp->stack[ind].type, stacks_tmp->stack[ind].sign);
        if (stacks_tmp->stack[ind].type == STACK_ITEM_TYPE_STRING)
          fprintf(final_ptr, "%s%c", stacks_tmp->stack[ind].string, 0);
        else {
          dou = stacks_tmp->stack[ind].value;
          WRITEOUT_DOU;
        }
      }

      stacks_tmp = stacks_tmp->next;
    }

    /* label sizeofs */
    ov = 0;
    label_sizeof_tmp = label_sizeofs;
    while (label_sizeof_tmp != NULL) {
      ov++;
      label_sizeof_tmp = label_sizeof_tmp->next;
    }

    WRITEOUT_OV;

    label_sizeof_tmp = label_sizeofs;
    while (label_sizeof_tmp != NULL) {
      fprintf(final_ptr, "%s%c", label_sizeof_tmp->name, 0);

      ov = label_sizeof_tmp->size;
      WRITEOUT_OV;

      label_sizeof_tmp = label_sizeof_tmp->next;
    }    
    
    /* append sections */
    ov = 0;
    append_tmp = append_sections;
    while (append_tmp != NULL) {
      ov++;
      append_tmp = append_tmp->next;
    }
    WRITEOUT_OV;

    append_tmp = append_sections;
    while (append_tmp != NULL) {
      fprintf(final_ptr, "%s%c", append_tmp->section, 0);
      fprintf(final_ptr, "%s%c", append_tmp->append_to, 0);
      append_tmp = append_tmp->next;
    }

    /* data area */
    ind = 0;
    for (inz = 0; inz < max_address; inz++) {
      if (rom_banks_usage_table[inz] != 0) {
        /* data block id */
        fprintf(final_ptr, "%c", 0x0);
        for (i = inz, ind = 0; inz < max_address; inz++, ind++) {
          if (rom_banks_usage_table[inz] == 0) {

            ov = i;
            WRITEOUT_OV;

            ov = ind;
            WRITEOUT_OV;

            fwrite(&rom_banks[i], 1, ind, final_ptr);

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

      fwrite(&rom_banks[i], 1, ind, final_ptr);
    }

    sec_tmp = sections_first;
    while (sec_tmp != NULL) {
      if (sec_tmp->alive == ON) {
        /* section block id */
        fprintf(final_ptr, "%c%s%c%c", 0x1, sec_tmp->name, sec_tmp->status, sec_tmp->keep);
        if (sec_tmp->nspace == NULL)
          fprintf(final_ptr, "%c", 0);
        else
          fprintf(final_ptr, "%s%c", sec_tmp->nspace->name, 0);

        ov = sec_tmp->id;
        WRITEOUT_OV;

        fprintf(final_ptr, "%c%c", sec_tmp->slot, sec_tmp->filename_id);

        ov = sec_tmp->address;
        WRITEOUT_OV;
        ov = sec_tmp->bank;
        WRITEOUT_OV;
        ov = sec_tmp->base;
        WRITEOUT_OV;
        ov = sec_tmp->size;
        WRITEOUT_OV;
        ov = sec_tmp->alignment;
        WRITEOUT_OV;
        ov = sec_tmp->offset;
        WRITEOUT_OV;
        ov = sec_tmp->priority;
        WRITEOUT_OV;

        fwrite(sec_tmp->data, 1, sec_tmp->size, final_ptr);

        if (listfile_data == YES && sec_tmp->listfile_items > 0)
          listfile_block_write(final_ptr, sec_tmp);
        else
          fprintf(final_ptr, "%c", 0);
      }
      sec_tmp = sec_tmp->next;
    }

    fclose(final_ptr);
  }

  /* output makefile rules */
  if (makefile_rules == YES) {
    fprintf(stdout, "%s: ", final_name);
    print_file_names();
  }

  /* show project information */
  if (verbose_mode == ON && output_format != OUTPUT_LIBRARY) {
    x = 0;
    for (ind = 0; ind < max_address; ind++) {
      if (rom_banks_usage_table[ind] == 0 && x == 0) {
        x = 1;
        inz = ind;
      }
      else if (rom_banks_usage_table[ind] != 0 && x == 1) {
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

    for (ind = 0, q = 0; ind < max_address; q++) {
      for (x = 0, inz = 0; inz < banks[q]; inz++) {
        if (rom_banks_usage_table[ind++] == 0)
          x++;
      }
      f = (((float)x)/banks[q]) * 100.0f;
      if (f == 100.0f)
        printf("Bank %.2d has %.5d bytes (%.1f%%) free.\n", q, x, f);
      else
        printf("Bank %.2d has %.5d bytes (%.2f%%) free.\n", q, x, f);
    }

    for (ind = 0, inz = 0; ind < max_address; ind++) {
      if (rom_banks_usage_table[ind] == 0)
        inz++;
    }
    fprintf(stderr, "%d unused bytes of total %d.\n", inz, max_address);

#ifndef GB
    sec_tmp = sections_first;
    while (sec_tmp != NULL) {
      if (sec_tmp->status == SECTION_STATUS_HEADER) {
        fprintf(stderr, "Bank %d header section size %d.\n", sec_tmp->bank, sec_tmp->size);
        ind += sec_tmp->size;
      }
      sec_tmp = sec_tmp->next;
    }

    if (ind != 0) {
      fprintf(stderr, "Total %d additional bytes (from headers and footers).\n", ind);
      fprintf(stderr, "Total size %d bytes.\n", ind + max_address);
    }
#endif

  }
  else if (verbose_mode == ON && output_format == OUTPUT_LIBRARY) {
    sec_tmp = sections_first;
    while (sec_tmp != NULL) {
      printf("Section \"%s\" size %d.\n", sec_tmp->name, sec_tmp->size);
      sec_tmp = sec_tmp->next;
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

    if (hashmap_get(namespace_map, prefix, (void*)&nspace) == MAP_OK) {
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
  if (hashmap_get(global_unique_label_map, str, (void*)&l) == MAP_OK) {
    *out = l;
    return SUCCEEDED;
  }

  return FAILED;
}


int mem_insert(unsigned char x) {
  
  if (section_status == ON) {
    if (sec_tmp->i >= sec_tmp->size || sec_tmp->i < 0) {
      fprintf(stderr, "MEM_INSERT: Overflowed from section \"%s\"; Please send a bug report!\n", sec_tmp->name);
      return FAILED;
    }
    sec_tmp->data[sec_tmp->i] = x;
    sec_tmp->i++;
    pc_bank++;
    pc_full++;
    pc_slot++;
    return SUCCEEDED;
  }

  if (pc_bank >= banksize) {
    fprintf(stderr, "MEM_INSERT: Origin ($%x) overflows from bank (%d).\n", pc_bank, rom_bank);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }
  else if (pc_full >= max_address) {
    fprintf(stderr, "MEM_INSERT: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", pc_full, max_address);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }
  else if (pc_slot >= pc_slot_max) {
    fprintf(stderr, "MEM_INSERT: The current address ($%.4x) overflows from SLOT %d.\n", pc_slot, slot);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }

  if (rom_banks_usage_table[pc_full] != 0 && rom_banks[pc_full] != x && mem_insert_overwrite == OFF) {
    fprintf(stderr, "MEM_INSERT: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", rom_banks_usage_table[pc_full], pc_full, rom_banks[pc_full], x & 0xFF);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
  }

  rom_banks_usage_table[pc_full] = 2;
  rom_banks[pc_full] = x;
  pc_bank++;
  pc_full++;
  pc_slot++;

  return SUCCEEDED;
}


int mem_insert_padding(void) {

  /* we'll use this function to reserve space for writes that take place later; e.g., we know that here will be
     written an 8-bit value, but we don't know the numeric value itself yet, but we'll need to reserve the
     space for the upcoming write or otherwise something else might get written here */

  if (section_status == ON) {
    sec_tmp->data[sec_tmp->i] = 0xCD;
    sec_tmp->i++;
    pc_bank++;
    pc_full++;
    pc_slot++;
    return SUCCEEDED;
  }

  if (pc_bank >= banksize) {
    fprintf(stderr, "MEM_INSERT_PADDING: Origin ($%x) overflows from bank (%d).\n", pc_bank, rom_bank);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }
  else if (pc_full >= max_address) {
    fprintf(stderr, "MEM_INSERT_PADDING: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", pc_full, max_address);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }
  else if (pc_slot >= pc_slot_max) {
    fprintf(stderr, "MEM_INSERT_PADDING: The current address ($%.4x) overflows from SLOT %d.\n", pc_slot, slot);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }

  /* announce the overwrite later */
  if (!(rom_banks_usage_table[pc_full] != 0 && mem_insert_overwrite == OFF))
    rom_banks_usage_table[pc_full] = 1;

  pc_bank++;
  pc_full++;
  pc_slot++;

  return SUCCEEDED;
}


int mem_insert_absolute(int add, unsigned char x) {

  if (add >= max_address) {
    fprintf(stderr, "MEM_INSERT_ABSOLUTE: The current address ($%.4x) exceeds the size of the ROM ($%.4x).\n", add, max_address);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
    return FAILED;
  }

  if (rom_banks_usage_table[add] > 1 && rom_banks[add] != x && mem_insert_overwrite == OFF) {
    fprintf(stderr, "MEM_INSERT_ABSOLUTE: %d. write into $%.4x (old: $%.2x, new: $%.2x).\n", rom_banks_usage_table[add], add, rom_banks[add], x & 0xFF);
    if (mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", mem_insert_action);
  }
  
  rom_banks_usage_table[add]++;
  rom_banks[add] = x;

  return SUCCEEDED;
}


int export_definitions(FILE *final_ptr) {

  struct export_def *export_tmp;
  unsigned char *cp;
  double dou;
  int ov;

  ov = 0;
  export_tmp = export_first;
  while (export_tmp != NULL) {
    hashmap_get(defines_map, export_tmp->name, (void*)&tmp_def);
    if (tmp_def != NULL) {
      if (tmp_def->type == DEFINITION_TYPE_VALUE)
        ov++;
      if (tmp_def->type == DEFINITION_TYPE_STACK)
        ov++;
    }

    export_tmp = export_tmp->next;
  }

  WRITEOUT_OV;

  export_tmp = export_first;
  while (export_tmp != NULL) {
    hashmap_get(defines_map, export_tmp->name, (void*)&tmp_def);

    if (tmp_def == NULL)
      fprintf(stderr, "WARNING: Trying to export an unkonwn definition \"%s\".\n", export_tmp->name);
    else {
      if (tmp_def->type == DEFINITION_TYPE_VALUE) {
        fprintf(final_ptr, "%s%c", tmp_def->alias, 0x0);
        dou = tmp_def->value;
        WRITEOUT_DOU;
      }
      else if (tmp_def->type == DEFINITION_TYPE_STRING) {
        fprintf(stderr, "INTERNAL_PASS_2: Definition \"%s\" is a string definition, and it cannot be exported.\n", export_tmp->name);
      }
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

  f = file_name_info_first;
  ov = 0;
  while (f != NULL) {
    ov++;
    f = f->next;
  }

  WRITEOUT_OV;

  f = file_name_info_first;
  while (f != NULL) {
    fprintf(final_ptr, "%s%c%c", f->name, 0x00, f->id);

    ov = (int)f->checksum;
    WRITEOUT_OV;

    f = f->next;
  }

  return SUCCEEDED;
}


#ifdef W65816
int get_snes_cpu_bank(struct label_def *l) {

  struct section_def *s;
  int x, k;

  if (l->section_status == OFF)
    k = bankaddress[l->bank] + l->address;
  else {
    s = sections_first;
    while (s != NULL) {
      if (l->section_id == s->id)
        break;
      else
        s = s->next;
    }
    k = s->address + l->address + bankaddress[s->bank];
  }

  if (lorom_defined == 1)
    x = (k / 0x8000) + l->base;
  else
    x = (k / 0x10000) + l->base;

  return x;
}
#endif

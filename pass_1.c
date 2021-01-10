
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "defines.h"

#include "main.h"
#include "include_file.h"
#include "parse.h"
#include "pass_1.h"
#include "pass_2.h"
#include "pass_3.h"
#include "stack.h"
#include "hashmap.h"
#include "printf.h"


#ifdef GB
char licenseecodenew_c1, licenseecodenew_c2;
int gbheader_defined = 0;
int nintendologo_defined = 0;
int computechecksum_defined = 0, computecomplementcheck_defined = 0;
int romgbc = 0, romdmg = 0, romsgb = 0;
int cartridgetype = 0, cartridgetype_defined = 0;
int countrycode = 0, countrycode_defined = 0;
int licenseecodenew_defined = 0, licenseecodeold = 0, licenseecodeold_defined = 0;
int version = 0, version_defined = 0;
#endif

#ifdef Z80
char sdsctag_name_str[MAX_NAME_LENGTH + 1], sdsctag_notes_str[MAX_NAME_LENGTH + 1], sdsctag_author_str[MAX_NAME_LENGTH + 1];
int sdsctag_name_type, sdsctag_notes_type, sdsctag_author_type, sdsc_ma, sdsc_mi;
int sdsctag_name_value, sdsctag_notes_value, sdsctag_author_value;
int computesmschecksum_defined = 0, sdsctag_defined = 0, smstag_defined = 0;
int smsheader_defined = 0, smsversion = 0, smsversion_defined = 0, smsregioncode = 0, smsregioncode_defined = 0;
int smsproductcode_defined = 0, smsproductcode1 = 0, smsproductcode2 = 0, smsproductcode3 = 0, smsreservedspace1 = 0;
int smsreservedspace2 = 0, smsreservedspace_defined = 0, smsromsize = 0, smsromsize_defined = 0;
#endif

int org_defined = 1, background_defined = 0, background_size = 0;
int enumid_defined = 0, enumid = 0, enumid_adder = 1, enumid_export = 0;
int bank = 0, bank_defined = 1;
int rombanks = 0, rombanks_defined = 0, romtype = 0, max_address = 0;
int rambanks = 0, rambanks_defined = 0;
int emptyfill, emptyfill_defined = 0;
int section_status = OFF, section_id = 1, line_count_status = ON;
int d, i, ind, inz, ifdef = 0, t, slots_amount = 0;
int memorymap_defined = 0;
int defaultslot_defined = 0, defaultslot, current_slot = 0;
int banksize_defined = 0, banksize;
int rombankmap_defined = 0, *banks = NULL, *bankaddress = NULL;
int bankheader_status = OFF;
int macro_active = 0;
int repeat_active = 0;
int smc_defined = 0;
int asciitable_defined = 0;
int block_status = 0, block_name_id = 0;
int dstruct_status = OFF;
unsigned char asciitable[256];

int unfolded_size;
char *unfolded_buffer = NULL;
FILE *fold_out;

#ifndef GB
extern struct stack *stacks_header_first, *stacks_header_last;
extern int stacks_inside, stacks_outside;
int stack_inserted;
#endif

#ifdef W65816
char snesid[4];
int snesheader_defined = 0, snesid_defined = 0, snesromsize = 0;
int sramsize_defined = 0, sramsize = 0, country_defined = 0, country = 0;
int cartridgetype = 0, cartridgetype_defined = 0, licenseecode_defined = 0, licenseecode = 0;
int version_defined = 0, version = 0, snesnativevector_defined = 0, snesemuvector_defined = 0;
int hirom_defined = 0, lorom_defined = 0, slowrom_defined = 0, fastrom_defined = 0, snes_mode = 0;
int exlorom_defined = 0, exhirom_defined = 0;
int computesneschecksum_defined = 0, use_wdc_standard = 0;
#endif

#if defined(GB) || defined(W65816)
char name[32];
int name_defined = 0;
#endif

char tmp[4096], emsg[sizeof(tmp) + MAX_NAME_LENGTH + 1 + 1024];
char *tmp_bf, *label_stack[256];
char cp[MAX_NAME_LENGTH + 1];

unsigned char *rom_banks = NULL, *rom_banks_usage_table = NULL;

struct export_def *export_first = NULL, *export_last = NULL;
struct optcode *opt_tmp;
struct definition *tmp_def;
struct map_t *defines_map = NULL;
struct macro_static *macros_first = NULL, *macros_last = NULL;
struct section_def *sections_first = NULL, *sections_last = NULL, *sec_tmp, *sec_next;
struct macro_runtime *macro_stack = NULL, *macro_runtime_current = NULL;
struct repeat_runtime *repeat_stack = NULL;
struct slot slots[256];
struct structure *structures_first = NULL;
struct filepointer *filepointers = NULL;
struct map_t *namespace_map = NULL;
struct append_section *append_sections = NULL;
struct label_sizeof *label_sizeofs = NULL;
struct block_name *block_names = NULL;
struct stringmaptable *stringmaptables = NULL;

extern char *buffer, *unfolded_buffer, label[MAX_NAME_LENGTH + 1], *include_dir, *full_name;
extern int size, unfolded_size, input_number_error_msg, verbose_mode, output_format, open_files;
extern int stack_id, latest_stack, ss, commandline_parsing, newline_beginning, expect_calculations;
extern int extra_definitions, string_size, input_float_mode, operand_hint, operand_hint_type;
extern int include_dir_size, parse_floats, listfile_data, quiet, parsed_double_decimal_numbers;
extern int create_sizeof_definitions;
extern FILE *file_out_ptr;
extern double parsed_double;
extern char *final_name;
extern struct active_file_info *active_file_info_first, *active_file_info_last, *active_file_info_tmp;
extern struct file_name_info *file_name_info_first, *file_name_info_last, *file_name_info_tmp;
extern struct stack *stacks_first, *stacks_tmp, *stacks_last;
extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;

int macro_stack_size = 0, repeat_stack_size = 0;

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(MCS6510) || defined(W65816) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
int xbit_size = 0;
int accu_size = 8, index_size = 8;
#endif

/* vars used when in an enum, ramsection, or struct. */
/* some variables named "enum_" are used in enums, ramsections, and structs. */
int in_enum = NO, in_ramsection = NO, in_struct = NO;
int enum_exp, enum_ord;
int enum_offset; /* Offset relative to enum start where we're at right now */
int last_enum_offset;
int base_enum_offset; /* start address of enum */
int enum_sizeof_pass; /* set on second pass through enum/ramsection, generating _sizeof labels */
/* temporary struct used to build up enums/ramsections (and, of course, structs)
   this gets temporarily replaced when inside a union (each union is considered a separate struct). */
struct structure *active_struct;

int union_base_offset; /* start address of current union */
int max_enum_offset; /* highest position seen within current union group */
struct structure *union_first_struct; /* first struct in current union */
struct union_stack *union_stack; /* stores variables for nested unions */

/* for .TABLE, .DATA and .ROW */
char table_format[256];
int table_defined = 0, table_size = 0, table_index = 0;

extern int opcode_n[256], opcode_p[256];
extern struct optcode opt_table[];

#define no_library_files(name)                                          \
  do {                                                                  \
    if (output_format == OUTPUT_LIBRARY) {                              \
      print_error("Library files don't take " name ".\n", ERROR_DIR);   \
      return FAILED;                                                    \
    }                                                                   \
  } while (0)


int strcaselesscmp(char *s1, char *s2) {

  if (s1 == NULL || s2 == NULL)
    return 0;

  while (*s1 != 0) {
    if (toupper((int)*s1) != toupper((int)*s2))
      return 1;
    s1++;
    s2++;
  }

  if (*s2 != 0)
    return 1;

  return 0;
}


static int _get_slot_number_by_its_name(char *slot_name, int *number) {

  int i;
  
  for (i = 0; i < slots_amount; i++) {
    if (strcmp(slot_name, slots[i].name) == 0) {
      *number = i;
      return SUCCEEDED;
    }
  }

  snprintf(emsg, sizeof(emsg), "Cannot find SLOT \"%s\".\n", slot_name);
  print_error(emsg, ERROR_DIR);

  return FAILED;  
}


static int _get_slot_number_by_a_value(int value, int *slot) {

  int i;

  if (value < 0) {
    *slot = value;
    return FAILED;
  }
  
  if (value < slots_amount) {
    /* value can be the direct SLOT ID, but can it be a SLOT's address as well? */
    for (i = 0; i < slots_amount; i++) {
      if (slots[i].address == value && value != i) {
        snprintf(emsg, sizeof(emsg), "There is a SLOT number %d, but there also is a SLOT (with ID %d) with starting address %d ($%x)... Using SLOT %d.\n", value, i, value, value, value);
        print_error(emsg, ERROR_WRN);
        break;
      }
    }
    
    *slot = value;
    return SUCCEEDED;
  }

  for (i = 0; i < slots_amount; i++) {
    if (slots[i].address == value) {
      *slot = i;
      return SUCCEEDED;
    }
  }

  *slot = -1;

  snprintf(emsg, sizeof(emsg), "Cannot find SLOT %d.\n", value);
  print_error(emsg, ERROR_DIR);
  
  return FAILED;
}


int macro_get(char *name, int add_namespace, struct macro_static **macro_out) {

  struct macro_static *macro;
  char fullname[MAX_NAME_LENGTH + 1];

  strcpy(fullname, name);

  /* append the namespace, if this file uses if */
  if (add_namespace == YES && active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(fullname, sizeof(fullname), "MACRO") == FAILED) {
      *macro_out = NULL;
      return FAILED;
    }
  }

  macro = macros_first;
  while (macro != NULL) {
    if (strcmp(macro->name, fullname) == 0)
      break;
    macro = macro->next;
  }

  *macro_out = macro;
  
  return SUCCEEDED;
}


int macro_stack_grow(void) {

  if (macro_active == macro_stack_size) {

    struct macro_runtime *macro;
    int old_size;

    /* enlarge the macro stack */
    old_size = macro_stack_size;
    macro_stack_size = (macro_stack_size<<1)+2;

    macro = calloc(sizeof(struct macro_runtime) * macro_stack_size, 1);
    if (macro == NULL) {
      print_error("Out of memory error while enlarging macro stack buffer.\n", ERROR_ERR);
      return FAILED;
    }

    if (macro_stack != NULL) {
      memcpy(macro, macro_stack, sizeof(struct macro_runtime) * old_size);
      free(macro_stack);
    }
    macro_stack = macro;
    macro_runtime_current = &macro_stack[macro_active - 1];
  }

  return SUCCEEDED;
}


int macro_start(struct macro_static *m, struct macro_runtime *mrt, int caller, int nargs) {

  macro_runtime_current = mrt;
  macro_active++;
  m->calls++;

  /* macro call start */
  fprintf(file_out_ptr, "i%s ", m->name);
  
  mrt->caller = caller;
  mrt->macro = m;
  mrt->macro_return_i = i;
  mrt->macro_return_line = active_file_info_last->line_current;
  mrt->macro_return_filename_id = active_file_info_last->filename_id;

  if ((extra_definitions == ON) && (active_file_info_last->filename_id != m->filename_id)) {
    redefine("WLA_FILENAME", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
    redefine("wla_filename", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
  }

  active_file_info_last->line_current = m->start_line;
  active_file_info_last->filename_id = m->filename_id;
  i = m->start;

  /* redefine NARGS */
  if (redefine("NARGS", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;
  if (redefine("nargs", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int macro_start_dxm(struct macro_static *m, int caller, char *name, int first) {

  struct macro_runtime *mrt;
  int start;
  
  /* start running a macro... run until .ENDM */
  mrt = &macro_stack[macro_active];

  start = i;

  if (first == NO && mrt->string_current < mrt->string_last) {
    inz = SUCCEEDED;
    d = mrt->string[mrt->string_current++];
  }
  else {
    inz = input_number();
    if (mrt != NULL) {
      mrt->string_current = 0;
      mrt->string_last = 0;
    }
  }

  if (first == YES) {
    if (mrt != NULL)
      mrt->offset = 0;
  }
  else {
    if (caller == MACRO_CALLER_DBM)
      mrt->offset++;
    else
      mrt->offset += 2;
  }

  if (inz == INPUT_NUMBER_EOL && first == NO) {
    next_line();
    return SUCCEEDED;
  }

  if (macro_stack_grow() == FAILED)
    return FAILED;

  mrt = &macro_stack[macro_active];
  mrt->argument_data = calloc(sizeof(struct macro_argument *) << 1, 1);
  mrt->argument_data[0] = calloc(sizeof(struct macro_argument), 1);
  mrt->argument_data[1] = calloc(sizeof(struct macro_argument), 1);
  if (mrt->argument_data == NULL || mrt->argument_data[0] == NULL || mrt->argument_data[1] == NULL) {
    print_error("Out of memory error while collecting macro arguments.\n", ERROR_NONE);
    return FAILED;
  }

  mrt->argument_data[1]->type = SUCCEEDED;
  mrt->argument_data[1]->value = mrt->offset;

  /* filter all the data through that macro */
  mrt->argument_data[0]->start = start;
  mrt->argument_data[0]->type = inz;

  if (inz == FAILED)
    return FAILED;
  else if (inz == INPUT_NUMBER_EOL) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", name);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  mrt->supplied_arguments = 2;

  if (inz == INPUT_NUMBER_ADDRESS_LABEL)
    strcpy(mrt->argument_data[0]->string, label);
  else if (inz == INPUT_NUMBER_STRING) {
    mrt->argument_data[0]->type = SUCCEEDED;
    mrt->argument_data[0]->value = label[0];
    strcpy(mrt->string, label);
    mrt->string_current = 1;
    mrt->string_last = (int)strlen(label);
    /*
      fprintf(stderr, "got string %s!\n", label);
    */
  }
  else if (inz == INPUT_NUMBER_STACK)
    mrt->argument_data[0]->value = (double)latest_stack;
  else if (inz == SUCCEEDED)
    mrt->argument_data[0]->value = d;
  else
    return FAILED;

  if (macro_start(m, mrt, caller, 1) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int macro_start_incbin(struct macro_static *m, struct macro_incbin *incbin_data, int first) {

  struct macro_runtime *mrt;

  /* start running a macro... run until .ENDM */
  if (macro_stack_grow() == FAILED)
    return FAILED;

  mrt = &macro_stack[macro_active];

  if (first == YES)
    mrt->incbin_data = incbin_data;
  else
    incbin_data = mrt->incbin_data;

  if (incbin_data->left == 0)
    return SUCCEEDED;

  if (first == YES)
    mrt->offset = 0;
  else
    mrt->offset++;

  mrt->argument_data = calloc(sizeof(struct macro_argument *) << 1, 1);
  mrt->argument_data[0] = calloc(sizeof(struct macro_argument), 1);
  mrt->argument_data[1] = calloc(sizeof(struct macro_argument), 1);
  if (mrt->argument_data == NULL || mrt->argument_data[0] == NULL || mrt->argument_data[1] == NULL) {
    print_error("Out of memory error while collecting macro arguments.\n", ERROR_NONE);
    return FAILED;
  }

  /* filter all the data through that macro */
  mrt->argument_data[1]->type = SUCCEEDED;
  mrt->argument_data[1]->value = mrt->offset;
  mrt->argument_data[0]->start = i;
  mrt->argument_data[0]->type = SUCCEEDED;
  mrt->supplied_arguments = 2;

  if (incbin_data->swap != 0) {
    if (incbin_data->swap == 1) {
      mrt->argument_data[0]->value = incbin_data->data[incbin_data->position + 1];
      incbin_data->swap = 2;
    }
    else {
      mrt->argument_data[0]->value = incbin_data->data[incbin_data->position];
      incbin_data->position += 2;
      incbin_data->swap = 1;
    }
  }
  else
    mrt->argument_data[0]->value = incbin_data->data[incbin_data->position++];

  incbin_data->left--;

  if (macro_start(m, mrt, MACRO_CALLER_INCBIN, 1) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int macro_insert_byte_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(emsg, sizeof(emsg), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -128 || d->value > 255) {
      snprintf(emsg, sizeof(emsg), ".%s expects 8-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    fprintf(file_out_ptr, "d%d ", (int)d->value);
    /*
      fprintf(stderr, ".DBM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(file_out_ptr, "c%d ", (int)d->value);
    /*
      fprintf(stderr, ".DBM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(emsg, sizeof(emsg), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int macro_insert_word_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(emsg, sizeof(emsg), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -32768 || d->value > 65535) {
      snprintf(emsg, sizeof(emsg), ".%s expects 16-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    fprintf(file_out_ptr, "y%d ", (int)d->value);
    /*
      fprintf(stderr, ".DWM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(file_out_ptr, "C%d ", (int)d->value);
    /*
      fprintf(stderr, ".DWM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(emsg, sizeof(emsg), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


#if W65816

int macro_insert_long_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(emsg, sizeof(emsg), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -8388608 || d->value > 16777215) {
      snprintf(emsg, sizeof(emsg), ".%s expects 24-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    fprintf(file_out_ptr, "z%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(file_out_ptr, "T%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(emsg, sizeof(emsg), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}

#endif


struct structure* get_structure(char *name) {

  struct structure *s = structures_first;

  while (s != NULL) {
    if (strcmp(name, s->name) == 0)
      return s;
    s = s->next;
  }

  return NULL;
}


int directive_define_def_equ(void);


int pass_1(void) {

  struct macro_runtime *mrt;
  struct macro_static *m = NULL;
  int o, p, q;
  
  if (verbose_mode == ON)
    printf("Pass 1...\n");

  /* mark all slots as empty */
  for (q = 0; q < 256; q++) {
    slots[q].size = 0;
    slots[q].name[0] = 0;
  }

  /* WARNING: "i" is a global variable that we use as the char index to the source file. */
  /* Ville: this must be one of the worst programming decicions I've ever done, sorry about it... */
  /* ... but the year was something like 1998 and I had just coded 6+ years 68k asm, and moved to C... */

  /* start from the very first character */
  i = 0;

  /* BANK 0 SLOT 0 ORG 0 */
  if (output_format != OUTPUT_LIBRARY)
    fprintf(file_out_ptr, "B%d %d O%d", 0, 0, 0);

  while (1) {
    t = get_next_token();
    if (t != SUCCEEDED)
      break;
    
    q = evaluate_token();

    if (q == SUCCEEDED)
      continue;
    else if (q == EVALUATE_TOKEN_EOP)
      return SUCCEEDED;
    else if (q == EVALUATE_TOKEN_NOT_IDENTIFIED) {
      /* check if it is of the form "LABEL:XYZ" */
      for (q = 0; q < ss; q++) {
        if (tmp[q] == ':')
          break;
        if (tmp[q] == '=')
          break;
      }

      /* is it a macro? */
      if (q == ss) {
        if (macro_get(tmp, YES, &m) == FAILED)
          return FAILED;
        if (m == NULL) {
          if (macro_get(tmp, NO, &m) == FAILED)
            return FAILED;
        }
      }

      /* it is a label after all? */
      if (q != ss || (newline_beginning == ON && m == NULL)) {
        char old_tmp_q = tmp[q];
    
        tmp[q] = 0;

        /* reset the flag as there can be only one label / line */
        newline_beginning = OFF;

        if (compare_next_token("=") == SUCCEEDED || old_tmp_q == '=') {
          /* it's actually a definition! */
          i -= ss;

          if (directive_define_def_equ() == FAILED)
            return FAILED;
        }
        else {
          if (output_format == OUTPUT_LIBRARY && section_status == OFF) {
            print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
            return FAILED;
          }
          if (org_defined == 0) {
            snprintf(emsg, sizeof(emsg), "\"%s\" needs a position in memory.\n", tmp);
            print_error(emsg, ERROR_LOG);
            return FAILED;
          }
          if (ss >= MAX_NAME_LENGTH) {
            snprintf(emsg, sizeof(emsg), "The label \"%s\" is too long. Max label length is %d characters.\n", tmp, MAX_NAME_LENGTH);
            print_error(emsg, ERROR_NONE);
            return FAILED;
          }
          if (bankheader_status == ON) {
            print_error("BANKHEADER sections don't take labels.\n", ERROR_LOG);
            return FAILED;
          }

          /* check out for \@-symbols */
          if (macro_active != 0 && q >= 2) {
            if (tmp[q - 2] == '\\' && tmp[q - 1] == '@')
              snprintf(&tmp[q - 2], sizeof(tmp) - (q - 2), "%d", macro_runtime_current->macro->calls - 1);
          }

          add_label_to_label_stack(tmp);
          fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmp);

          /* move to the end of the label */
          if (q != ss)
            i -= ss - q - 1;
        }
    
        continue;
      }
      else if (compare_next_token("=") == SUCCEEDED) {
        /* it's actually a definition! */
        i -= ss;

        if (directive_define_def_equ() == FAILED)
          return FAILED;

        continue;
      }
      
      if (m == NULL) {
        snprintf(emsg, sizeof(emsg), "Unknown symbol \"%s\".\n", tmp);
        print_error(emsg, ERROR_ERR);
        return FAILED;
      }

      /* start running a macro... run until .ENDM */
      if (macro_stack_grow() == FAILED)
        return FAILED;

      mrt = &macro_stack[macro_active];
      mrt->argument_data = NULL;

      /* collect macro arguments */
      for (p = 0; 1; p++) {
        /* take away the white space */
        while (1) {
          if (buffer[i] == ' ' || buffer[i] == ',')
            i++;
          else
            break;
        }

        o = i;
        q = input_number();
        if (q == INPUT_NUMBER_EOL)
          break;

        mrt->argument_data = realloc(mrt->argument_data, (p+1)*sizeof(struct macro_argument *));
        mrt->argument_data[p] = calloc(sizeof(struct macro_argument), 1);
        if (mrt->argument_data == NULL || mrt->argument_data[p] == NULL) {
          print_error("Out of memory error while collecting macro arguments.\n", ERROR_NONE);
          return FAILED;
        }

        mrt->argument_data[p]->start = o;
        mrt->argument_data[p]->type = q;

        if (q == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(mrt->argument_data[p]->string, label);
        else if (q == INPUT_NUMBER_STRING)
          strcpy(mrt->argument_data[p]->string, label);
        else if (q == INPUT_NUMBER_STACK)
          mrt->argument_data[p]->value = (double)latest_stack;
        else if (q == SUCCEEDED)
          mrt->argument_data[p]->value = parsed_double;
        else
          return FAILED;

        /* do we have a name for this argument? */
        if (p < m->nargument_names) {
          if (q == INPUT_NUMBER_ADDRESS_LABEL)
            redefine(m->argument_names[p], 0.0, label, DEFINITION_TYPE_ADDRESS_LABEL, (int)strlen(label));
          else if (q == INPUT_NUMBER_STRING)
            redefine(m->argument_names[p], 0.0, label, DEFINITION_TYPE_STRING, (int)strlen(label));
          else if (q == INPUT_NUMBER_STACK)
            redefine(m->argument_names[p], (double)latest_stack, NULL, DEFINITION_TYPE_STACK, 0);
          else if (q == SUCCEEDED)
            redefine(m->argument_names[p], parsed_double, NULL, DEFINITION_TYPE_VALUE, 0);
        }
      }

      next_line();

      mrt->supplied_arguments = p;
      if (macro_start(m, mrt, MACRO_CALLER_NORMAL, p) == FAILED)
        return FAILED;

      continue;
    }
    else if (q == FAILED) {
      snprintf(emsg, sizeof(emsg), "Couldn't parse \"%s\".\n", tmp);
      print_error(emsg, ERROR_ERR);
      return FAILED;
    }
    else {
      printf("PASS_1: Internal error, unknown return type %d.\n", q);
      return FAILED;
    }
  }

  return FAILED;
}


void output_assembled_opcode(struct optcode *oc, const char *format, ...) {

  va_list ap;
  
  if (oc == NULL)
    return;
  
  va_start(ap, format);

  vfprintf(file_out_ptr, format, ap);
#ifdef WLA_DEBUG
  {
    char ttt[256];

    va_end(ap);
    va_start(ap, format);
    vsnprintf(ttt, sizeof(ttt), format, ap);
    printf("LINE %5d: OPCODE: %16s ::: %s\n", active_file_info_last->line_current, oc->op, ttt);
  }
#endif

  va_end(ap);
}


#if MC6809
static char error_no_u[] = "Was expecting register X/Y/S/PC/A/B/CC/DP";
static char error_no_s[] = "Was expecting register X/Y/U/PC/A/B/CC/DP";

static int parse_push_pull_registers(int accept_u) {

  int register_byte = 0, reg, y, z, prev_i;

  while (1) {
    y = i;
    i = inz;
    prev_i = i;
    z = input_number();
    inz = i;
    i = y;

    if (z == INPUT_NUMBER_EOL) {
      /* roll back to the index before EOL */
      inz = prev_i;
      break;
    }
    
    if (z != INPUT_NUMBER_ADDRESS_LABEL) {
      if (accept_u == 1) {
        snprintf(emsg, sizeof(emsg), "%s, got something else instead.\n", error_no_s);
        print_error(emsg, ERROR_ERR);
      }
      else {
        snprintf(emsg, sizeof(emsg), "%s, got something else instead.\n", error_no_u);
        print_error(emsg, ERROR_ERR);
      }
      return -1;
    }

    if (strcaselesscmp(label, "X") == 0)
      reg = 1 << 4;
    else if (strcaselesscmp(label, "Y") == 0)
      reg = 1 << 5;
    else if (strcaselesscmp(label, "U") == 0) {
      if (accept_u == 0) {
        snprintf(emsg, sizeof(emsg), "%s, got \"%s\" instead.\n", error_no_u, label);
        print_error(emsg, ERROR_ERR);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(label, "S") == 0) {
      if (accept_u == 1) {
        snprintf(emsg, sizeof(emsg), "%s, got \"%s\" instead.\n", error_no_s, label);
        print_error(emsg, ERROR_ERR);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(label, "PC") == 0)
      reg = 1 << 7;
    else if (strcaselesscmp(label, "A") == 0)
      reg = 1 << 1;
    else if (strcaselesscmp(label, "B") == 0)
      reg = 1 << 2;
    else if (strcaselesscmp(label, "CC") == 0)
      reg = 1 << 0;
    else if (strcaselesscmp(label, "DP") == 0)
      reg = 1 << 3;
    else {
      if (accept_u == 1) {
        snprintf(emsg, sizeof(emsg), "%s, got \"%s\" instead.\n", error_no_s, label);
        print_error(emsg, ERROR_ERR);
      }
      else {
        snprintf(emsg, sizeof(emsg), "%s, got \"%s\" instead.\n", error_no_u, label);
        print_error(emsg, ERROR_ERR);
      }
      return -1;
    }

    if ((register_byte & reg) != 0) {
      snprintf(emsg, sizeof(emsg), "Register \"%s\" was already defined.\n", label);
      print_error(emsg, ERROR_ERR);
      return -1;
    }

    register_byte |= reg;
  }

  if (register_byte == 0) {
    if (accept_u == 1)
      snprintf(emsg, sizeof(emsg), "%s, got nothing instead.\n", error_no_s);
    else
      snprintf(emsg, sizeof(emsg), "%s, got nothing instead.\n", error_no_u);
    print_error(emsg, ERROR_ERR);

    return -1;
  }
  
  return register_byte;
}


static int get_register_byte_from_label_exg_tfr() {

  /* 16-bit */
  if (strcaselesscmp(label, "D") == 0)
    return 0;
  if (strcaselesscmp(label, "X") == 0)
    return 1;
  if (strcaselesscmp(label, "Y") == 0)
    return 2;
  if (strcaselesscmp(label, "U") == 0)
    return 3;
  if (strcaselesscmp(label, "S") == 0)
    return 4;
  if (strcaselesscmp(label, "PC") == 0)
    return 5;

  /* 8-bit */
  if (strcaselesscmp(label, "A") == 0)
    return 8;
  if (strcaselesscmp(label, "B") == 0)
    return 9;
  if (strcaselesscmp(label, "CC") == 0)
    return 0xA;
  if (strcaselesscmp(label, "DP") == 0)
    return 0xB;

  snprintf(emsg, sizeof(emsg), "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got \"%s\" instead.\n", label);
  print_error(emsg, ERROR_ERR);

  return -1;
}


static int parse_exg_tfr_registers() {

  int register_byte = 0, data = 0, y, z;

  /* source register */
  y = i;
  i = inz;
  z = input_number();
  inz = i;
  i = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error("Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n", ERROR_ERR);
    return -1;
  }

  data = get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;
  register_byte = data;

  /* destination register */
  y = i;
  i = inz;
  z = input_number();
  inz = i;
  i = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error("Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n", ERROR_ERR);
    return -1;
  }

  data = get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;

  /* are we mixing 16-bit and 8-bit registers? */
  if ((register_byte <= 5 && data > 5) || (register_byte > 5 && data <= 5)) {
    print_error("Mixing of 8-bit and 16-bit registers is not allowed here.\n", ERROR_ERR);
    return -1;    
  }
  
  register_byte = (register_byte << 4) | data;
  
  return register_byte;
}
#endif


int evaluate_token(void) {

  int f, z, x, y;
#if defined(Z80) || defined(SPC700) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280)
  int e = 0, v = 0, h = 0;
  char labelx[MAX_NAME_LENGTH + 1];
#endif
#ifdef SPC700
  int g;
#endif
#ifdef HUC6280
  int r = 0, s, t = 0, u = 0;
  char labely[MAX_NAME_LENGTH + 1];
#endif

  /* are we in an enum, ramsection, or struct? */
  if (in_enum == YES || in_ramsection == YES || in_struct == YES)
    return parse_enum_token();

  /* is it a directive? */
  if (tmp[0] == '.') {
    x = parse_directive();
    if (x != DIRECTIVE_NOT_IDENTIFIED)
      return x;

    /* allow error messages from input_numbers() */
    input_number_error_msg = YES;

    return EVALUATE_TOKEN_NOT_IDENTIFIED;
  }

  /* is it a label? */
  if (tmp[ss - 1] == ':' && newline_beginning == ON) {
    tmp[ss - 1] = 0;
    newline_beginning = OFF;

    if (output_format == OUTPUT_LIBRARY && section_status == OFF) {
      print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
      return FAILED;
    }
    if (org_defined == 0) {
      snprintf(emsg, sizeof(emsg), "\"%s\" needs a position in memory.\n", tmp);
      print_error(emsg, ERROR_LOG);
      return FAILED;
    }
    if (ss >= MAX_NAME_LENGTH) {
      snprintf(emsg, sizeof(emsg), "The label \"%s\" is too long. Max label length is %d characters.\n", tmp, MAX_NAME_LENGTH);
      print_error(emsg, ERROR_NONE);
      return FAILED;
    }
    if (bankheader_status == ON) {
      print_error("BANKHEADER sections don't take labels.\n", ERROR_LOG);
      return FAILED;
    }

    /* check for \@-symbols */
    if (macro_active != 0) {
      if (tmp[ss - 3] == '\\' && tmp[ss - 2] == '@')
        snprintf(&tmp[ss - 3], sizeof(tmp) - (ss - 3), "%d", macro_runtime_current->macro->calls - 1);
    }

    add_label_to_label_stack(tmp);
    fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmp);

    return SUCCEEDED;
  }

  /* OPCODE? */
  {
    int op_id = tmp[0];

    if (op_id < 0) {
      print_error("Invalid value\n", ERROR_LOG);
      return FAILED;
    }

    ind = opcode_p[op_id];
  }

  opt_tmp = &opt_table[ind];

  for (f = opcode_n[(unsigned int)tmp[0]]; f > 0; f--) {
#if W65816
    if (use_wdc_standard == 0) {
      /* skip all mnemonics that contain '<', '|' and '>' */
      for (inz = 0, d = SUCCEEDED; inz < OP_SIZE_MAX; inz++) {
        char c = opt_tmp->op[inz];

        if (c == 0)
          break;
        if (c == '<' || c == '|' || c == '>') {
          d = FAILED;
          break;
        }
      }

      if (d == FAILED) {
        /* try the next mnemonic in the array */
        opt_tmp = &opt_table[++ind];
        continue;
      }
    }
#endif
    
    /* try to match the first part of the mnemonic, already read into tmp */
    for (inz = 0, d = SUCCEEDED; inz < OP_SIZE_MAX; inz++) {
      if (tmp[inz] == 0)
        break;
      if (opt_tmp->op[inz] != toupper((int)tmp[inz])) {
        d = FAILED;
        break;
      }
    }

    if (d == FAILED) {
      /* try the next mnemonic in the array */
      opt_tmp = &opt_table[++ind];
      continue;
    }

    /* beginning matches the input */
    x = inz;
    inz = i;

#ifndef GB
    /* no stack rollback */
    stack_inserted = STACK_NONE;
#endif

    switch (opt_tmp->type) {

#ifdef GB
#include "decode_gb.c"
#endif
#ifdef Z80
#include "decode_z80.c"
#endif
#ifdef MCS6502
#include "decode_6502.c"
#endif
#ifdef WDC65C02
#include "decode_65c02.c"
#endif
#ifdef CSG65CE02
#include "decode_65ce02.c"
#endif
#ifdef MCS6510
#include "decode_6510.c"
#endif
#ifdef W65816
#include "decode_65816.c"
#endif
#ifdef MC6800
#include "decode_6800.c"
#endif
#ifdef MC6801
#include "decode_6801.c"
#endif
#ifdef MC6809
#include "decode_6809.c"
#endif
#ifdef I8008
#include "decode_8008.c"
#endif
#ifdef I8080
#include "decode_8080.c"
#endif
#ifdef SPC700
#include "decode_spc700.c"
#endif
#ifdef HUC6280
#include "decode_huc6280.c"
#endif

    }

#ifndef GB
    /* perform stack rollback? */
    if (stack_inserted != STACK_NONE) {

      struct stack *s;

      if (stack_inserted == STACK_OUTSIDE) {
        if (stacks_outside == 1) {
          stacks_outside = 0;
          delete_stack(stacks_first);
          stacks_first = NULL;
          stacks_last = NULL;
        }
        else {
          s = stacks_first;
          stacks_outside--;

          for (y = 0; y < stacks_outside - 1; y++)
            s = s->next;

          delete_stack(s->next);
          s->next = NULL;
          stacks_last = s;
        }
      }
      else {
        if (stacks_inside == 1) {
          stacks_inside = 0;
          delete_stack(stacks_header_first);
          stacks_header_first = NULL;
          stacks_header_last = NULL;
        }
        else {
          s = stacks_header_first;
          stacks_inside--;

          for (y = 0; y < stacks_inside - 1; y++)
            s = s->next;

          delete_stack(s->next);
          s->next = NULL;
          stacks_header_last = s;
        }
      }
    }
#endif

    opt_tmp = &opt_table[++ind];
  }

  /* allow error messages from input_numbers() */
  input_number_error_msg = YES;

  return EVALUATE_TOKEN_NOT_IDENTIFIED;
}


int redefine(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  
  hashmap_get(defines_map, name, (void*)&d);
  
  /* it wasn't defined previously */
  if (d == NULL)
    return add_a_new_definition(name, value, string, type, size);

  d->type = type;

  if (type == DEFINITION_TYPE_VALUE)
    d->value = value;
  else if (type == DEFINITION_TYPE_STACK)
    d->value = value;
  else if (type == DEFINITION_TYPE_STRING || type == DEFINITION_TYPE_ADDRESS_LABEL) {
    memcpy(d->string, string, size);
    d->string[size] = 0;
    d->size = size;
  }

  return SUCCEEDED;
}


int undefine(char *name) {

  struct definition *d;
  
  if (hashmap_get(defines_map, name, (void*)&d) != MAP_OK)
    return FAILED;

  hashmap_remove(defines_map, name);

  free(d);

  return SUCCEEDED;
}


int add_a_new_definition(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  int err;

  /* we skip definitions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
  if (strcmp(".", name) == 0 || strcmp("_sizeof_.", name) == 0)
    return SUCCEEDED;

  hashmap_get(defines_map, name, (void*)&d);
  if (d != NULL) {
    snprintf(emsg, sizeof(emsg), "\"%s\" was defined for the second time.\n", name);
    if (commandline_parsing == OFF)
      print_error(emsg, ERROR_DIR);
    else
      fprintf(stderr, "ADD_A_NEW_DEFINITION: %s", emsg);
    return FAILED;
  }

  d = calloc(sizeof(struct definition), 1);
  if (d == NULL) {
    snprintf(emsg, sizeof(emsg), "Out of memory while trying to add a new definition (\"%s\").\n", name);
    if (commandline_parsing == OFF)
      print_error(emsg, ERROR_DIR);
    else
      fprintf(stderr, "ADD_A_NEW_DEFINITION: %s", emsg);
    return FAILED;
  }

  strcpy(d->alias, name);
  d->type = type;

  if ((err = hashmap_put(defines_map, d->alias, d)) != MAP_OK) {
    fprintf(stderr, "ADD_A_NEW_DEFINITION: Hashmap error %d\n", err);
    return FAILED;
  }

  if (type == DEFINITION_TYPE_VALUE)
    d->value = value;
  else if (type == DEFINITION_TYPE_STACK)
    d->value = value;
  else if (type == DEFINITION_TYPE_STRING || type == DEFINITION_TYPE_ADDRESS_LABEL) {
    memcpy(d->string, string, size);
    d->string[size] = 0;
    d->size = size;
  }

  return SUCCEEDED;
}


int localize_path(char *path) {

  int i;
  
  if (path == NULL)
    return FAILED;

  for (i = 0; path[i] != 0; i++) {
#if defined(MSDOS)
    /* '/' -> '\' */
    if (path[i] == '/')
      path[i] = '\\';
#else
    /* '\' -> '/' */
    if (path[i] == '\\')
      path[i] = '/';
#endif
  }

  return SUCCEEDED;
}


int verify_name_length(char *name) {

  if (strlen(name) > MAX_NAME_LENGTH) {
    snprintf(emsg, sizeof(emsg), "The label \"%s\" is too long. Max label length is %d bytes.\n", name, MAX_NAME_LENGTH);
    print_error(emsg, ERROR_NONE);
    return FAILED;
  }

  return SUCCEEDED;
}


void print_error(char *error, int type) {

  char error_dir[] = "DIRECTIVE_ERROR:";
  char error_unf[] = "UNFOLD_ALIASES:";
  char error_num[] = "INPUT_NUMBER:";
  char error_inc[] = "INCLUDE_FILE:";
  char error_inb[] = "INCBIN_FILE:";
  char error_inp[] = "INPUT_ERROR:";
  char error_log[] = "LOGIC_ERROR:";
  char error_stc[] = "STACK_CALCULATE:";
  char error_wrn[] = "WARNING:";
  char error_err[] = "ERROR:";
  char *t = NULL;

  switch (type) {
  case ERROR_LOG:
    t = error_log;
    break;
  case ERROR_UNF:
    t = error_unf;
    break;
  case ERROR_INC:
    t = error_inc;
    break;
  case ERROR_INB:
    t = error_inb;
    break;
  case ERROR_DIR:
    t = error_dir;
    break;
  case ERROR_INP:
    t = error_inp;
    break;
  case ERROR_NUM:
    t = error_num;
    break;
  case ERROR_STC:
    t = error_stc;
    break;
  case ERROR_WRN:
    t = error_wrn;
    break;
  case ERROR_ERR:
    t = error_err;
    break;
  case ERROR_NONE:
    fprintf(stderr, "%s:%d: %s", get_file_name(active_file_info_last->filename_id), active_file_info_last->line_current, error);
    return;
  }

  fprintf(stderr, "%s:%d: %s %s", get_file_name(active_file_info_last->filename_id), active_file_info_last->line_current, t, error);
  fflush(stderr);

  return;
}


#ifdef W65816

void give_snes_rom_mode_defined_error(char *prior) {

  if (lorom_defined != 0) {
    snprintf(emsg, sizeof(emsg), ".LOROM was defined prior to %s.\n", prior);
    print_error(emsg, ERROR_DIR);
  }
  else if (hirom_defined != 0) {
    snprintf(emsg, sizeof(emsg), ".HIROM was defined prior to %s.\n", prior);
    print_error(emsg, ERROR_DIR);
  }
  else if (exlorom_defined != 0) {
    snprintf(emsg, sizeof(emsg), ".EXLOROM was defined prior to %s.\n", prior);
    print_error(emsg, ERROR_DIR);
  }
  else if (exhirom_defined != 0) {
    snprintf(emsg, sizeof(emsg), ".EXHIROM was defined prior to %s.\n", prior);
    print_error(emsg, ERROR_DIR);
  }
}

#endif


void next_line(void) {

  newline_beginning = ON;

  if (line_count_status == OFF)
    return;

  if (active_file_info_last == NULL)
    return;

  /* output the file number for list file structure building */
  if (listfile_data == YES)
    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  if (active_file_info_last != NULL)
    active_file_info_last->line_current++;
}


/* used by .RAMSECTIONs only */
int add_label_sizeof(char *label, int size) {

  struct label_sizeof *ls;
  char tmpname[MAX_NAME_LENGTH + 8];

  if (create_sizeof_definitions == NO)
    return SUCCEEDED;
  
  /* we skip definitions for '_sizeof_.' (because .ENUM and .RAMSECTION use it as an anonymous label) */
  if (strcmp(".", label) == 0)
    return SUCCEEDED;

  ls = calloc(sizeof(struct label_sizeof), 1);
  if (ls == NULL) {
    print_error("Out of memory error while allocating a label sizeof structure.\n", ERROR_DIR);
    return FAILED;
  }
  
  strcpy(ls->name, label);
  ls->size = size;
  ls->next = label_sizeofs;
  label_sizeofs = ls;

  /* define locally also, since we can */
  snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", label);
  if (add_a_new_definition(tmpname, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


void free_struct(struct structure *st) {

  struct structure_item *si = st->items;

  while (si != NULL) {
    struct structure_item *tmp = si;

    if (si->type == STRUCTURE_ITEM_TYPE_UNION)
      free_struct(si->union_items);
    /* don't free si->instance for STRUCTURE_ITEM_TYPE_INSTANCE since that's a reusable
       structure */
    si = si->next;
    free(tmp);
  }

  free(st);
}


/* enum_offset and last_enum_offset should be set when calling this. */
int add_label_to_enum_or_ramsection(char *name, int size) {

  char tmp[MAX_NAME_LENGTH+10];

  /* there are two passes done when adding a temporary struct to an enum/ramsection. first
     pass is to add the labels, second is to add sizeof definitions. if done in only one
     pass, the resulting sym file is very ugly... */
  if (enum_sizeof_pass == NO) {
    if (verify_name_length(name) == FAILED)
      return FAILED;

    if (in_enum || in_struct) {
      if (add_a_new_definition(name, (double)(base_enum_offset+enum_offset), NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
      if (enum_exp == YES)
        if (export_a_definition(name) == FAILED)
          return FAILED;
    }
    else if (in_ramsection) {
      if (last_enum_offset != enum_offset) {
        /* this sometimes abuses the "dsb" implementation to move backwards in the ramsection. */
        fprintf(file_out_ptr, "x%d 0 ", enum_offset-last_enum_offset);
      }
      fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
      /* we skip label emissions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
      if (strcmp(".", name) != 0)
        fprintf(file_out_ptr, "L%s ", name);
    }
  }
  else { /* sizeof pass */
    if (in_ramsection) {
      if (add_label_sizeof(name, size) == FAILED)
        return FAILED;
    }
    else {
      if (create_sizeof_definitions == YES) {
        snprintf(tmp, sizeof(tmp), "_sizeof_%s", name);
        if (add_a_new_definition(tmp, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;
        if (in_enum == YES && enum_exp == YES) {
          if (export_a_definition(tmp) == FAILED)
            return FAILED;
        }
      }
    }
  }

  last_enum_offset = enum_offset;

  return SUCCEEDED;
}


/* add all fields from a struct at the current offset in the enum/ramsection.
   this is used to construct enums or ramsections through temporary structs, even if
   INSTANCEOF isn't used. enum_sizeof_pass should be set to YES or NO before calling. */
int enum_add_struct_fields(char *basename, struct structure *st, int reverse) {

  char tmp[MAX_NAME_LENGTH * 2 + 5];
  struct structure_item *si;
  int real_si_size, g;

  if (strlen(basename) > MAX_NAME_LENGTH) {
    snprintf(emsg, sizeof(emsg), "Name \"%s\" is too long!\n", basename);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  si = st->items;
  while (si != NULL) {
    real_si_size = si->size;
    if (si->type == STRUCTURE_ITEM_TYPE_DOTTED)
      real_si_size = 0;

    if (reverse)
      enum_offset -= real_si_size;

    /* make definition for this item */
    if (si->name[0] != '\0') {
      if (basename[0] != '\0')
        snprintf(tmp, sizeof(tmp), "%s.%s", basename, si->name);
      else
        snprintf(tmp, sizeof(tmp), "%s", si->name);

      if (verify_name_length(tmp) == FAILED)
        return FAILED;

      if (add_label_to_enum_or_ramsection(tmp, si->size) == FAILED)
        return FAILED;
    }

    /* if this struct has an .instanceof in it, we need to recurse */
    if (si->type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
      if (si->num_instances <= 1) {
        /* add definition for first (possibly only) instance of struct */
        if (enum_add_struct_fields(tmp, si->instance, 0) == FAILED)
          return FAILED;
      }
      else {
        g = si->start_from;
        while (g < si->start_from + si->num_instances) {
          if (basename[0] != '\0')
            snprintf(tmp, sizeof(tmp), "%s.%s.%d", basename, si->name, g);
          else
            snprintf(tmp, sizeof(tmp), "%s.%d", si->name, g);

          if (verify_name_length(tmp) == FAILED)
            return FAILED;
          if (add_label_to_enum_or_ramsection(tmp, si->instance->size) == FAILED)
            return FAILED;
          if (enum_add_struct_fields(tmp, si->instance, 0) == FAILED)
            return FAILED;
          g++;
        }
      }
    }
    /* if this struct has a .union in it, we treat each union block like a struct */
    else if (si->type == STRUCTURE_ITEM_TYPE_UNION) {
      int orig_offset = enum_offset;
      char union_basename[MAX_NAME_LENGTH * 2 + 5];
      struct structure *un = si->union_items;

      while (un != NULL) {
        enum_offset = orig_offset;

        if (un->name[0] != '\0') {
          if (basename[0] != '\0')
            snprintf(union_basename, sizeof(union_basename), "%s.%s", basename, un->name);
          else
            snprintf(union_basename, sizeof(union_basename), "%s", un->name);

          if (verify_name_length(union_basename) == FAILED)
            return FAILED;

          if (add_label_to_enum_or_ramsection(union_basename, un->size) == FAILED)
            return FAILED;
        }
        else
          snprintf(union_basename, sizeof(union_basename), "%s", basename);

        if (enum_add_struct_fields(union_basename, un, 0) == FAILED)
          return FAILED;

        un = un->next;
      }

      /* this is the size of the largest union */
      enum_offset = orig_offset + real_si_size;
    }
    else
      enum_offset += real_si_size;

    if (reverse) /* after defining data, go back to start, for DESC enums */
      enum_offset -= real_si_size;

    si = si->next;
  }

  return SUCCEEDED;
}


/* either "in_enum", "in_ramsection", or "in_struct" should be true when this is called. */
int parse_enum_token(void) {

  struct structure *st = NULL;
  struct structure_item *si;
  char tmpname[MAX_NAME_LENGTH + 8 + 1], bak[256];
  int type, size, q, start_from = 1;
  
  /* check for "if" directives (the only directives permitted in an enum/ramsection) */
  if (tmp[0] == '.') {
    if ((q = parse_if_directive()) != -1) {
      return q;
    }
  }

  if (strcaselesscmp(tmp, ".UNION") == 0) {
    struct union_stack *ust;
    int inz;

    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    st->items = NULL;
    st->last_item = NULL;

    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, tmp);
    else
      st->name[0] = '\0';

    /* put previous union onto the "stack" */
    ust = calloc(sizeof(struct union_stack), 1);
    if (ust == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      free(st);
      return FAILED;
    }
    ust->active_struct = active_struct;
    ust->union_first_struct = union_first_struct;
    ust->union_base_offset = union_base_offset;
    ust->max_enum_offset = max_enum_offset;
    ust->prev = union_stack;
    union_stack = ust;

    active_struct = st;
    union_first_struct = active_struct;
    union_base_offset = enum_offset;
    max_enum_offset = union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(tmp, ".NEXTU") == 0) {
    int inz;

    if (union_stack == NULL) {
      print_error("There is no open union.\n", ERROR_DIR);
      return FAILED;
    }

    if (enum_offset > max_enum_offset)
      max_enum_offset = enum_offset;
    active_struct->size = enum_offset - union_base_offset;
    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    st->items = NULL;
    st->last_item = NULL;
    
    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, tmp);
    else
      st->name[0] = '\0';

    active_struct->next = st;
    active_struct = st;
    enum_offset = union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(tmp, ".ENDU") == 0) {
    struct union_stack *ust;
    int total_size;

    if (union_stack == NULL) {
      print_error("There is no open union.\n", ERROR_DIR);
      return FAILED;
    }

    if (enum_offset > max_enum_offset)
      max_enum_offset = enum_offset;

    total_size = max_enum_offset - union_base_offset;

    active_struct->size = enum_offset - union_base_offset;
    active_struct->next = NULL;

    st = union_first_struct;

    enum_offset = max_enum_offset;

    /* pop previous union from the "stack" */
    ust = union_stack;
    active_struct = union_stack->active_struct;
    union_first_struct = union_stack->union_first_struct;
    union_base_offset = ust->union_base_offset;
    max_enum_offset = ust->max_enum_offset;
    union_stack = union_stack->prev;
    free(ust);

    /* just popped max_enum_offset; need to update it for end of union */
    if (enum_offset > max_enum_offset)
      max_enum_offset = enum_offset;

    /* create a new structure item of type STRUCTURE_ITEM_TYPE_UNION */
    si = calloc(sizeof(struct structure_item), 1);
    if (si == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    si->name[0] = '\0';
    si->type = STRUCTURE_ITEM_TYPE_UNION;
    si->size = total_size;
    si->next = NULL;
    si->union_items = st;
    if (active_struct->items == NULL)
      active_struct->items = si;
    if (active_struct->last_item != NULL)
      active_struct->last_item->next = si;
    active_struct->last_item = si;
    return SUCCEEDED;
  }
  else if (in_enum == YES && strcaselesscmp(tmp, ".ENDE") == 0) {
    if (union_stack != NULL) {
      print_error("Union not closed.\n", ERROR_DIR);
      return FAILED;
    }
    
    enum_offset = 0;
    enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", active_struct, (enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    enum_offset = 0;
    enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", active_struct, (enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    free_struct(active_struct);
    active_struct = NULL;

    in_enum = NO;
    return SUCCEEDED;
  }
  else if (in_ramsection == YES && strcaselesscmp(tmp, ".ENDS") == 0) {
    if (union_stack != NULL) {
      print_error("Union not closed.\n", ERROR_DIR);
      return FAILED;
    }

    enum_offset = 0;
    last_enum_offset = 0;
    enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", active_struct, 0) == FAILED)
      return FAILED;

    enum_offset = 0;
    last_enum_offset = 0;
    enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", active_struct, 0) == FAILED)
      return FAILED;

    if (max_enum_offset > last_enum_offset)
      fprintf(file_out_ptr, "o%d 0 ", max_enum_offset-last_enum_offset);

    /* generate a section end label? */
    if (extra_definitions == ON)
      generate_label("SECTIONEND_", sections_last->name);
    
    free_struct(active_struct);
    active_struct = NULL;

    fprintf(file_out_ptr, "s ");
    section_status = OFF;
    in_ramsection = NO;

    return SUCCEEDED;
  }
  else if (in_struct && strcaselesscmp(tmp, ".ENDST") == 0) {
    enum_offset = 0;
    last_enum_offset = 0;
    enum_sizeof_pass = NO;
    if (enum_add_struct_fields(active_struct->name, active_struct, 0) == FAILED)
      return FAILED;

    enum_offset = 0;
    last_enum_offset = 0;
    enum_sizeof_pass = YES;
    if (enum_add_struct_fields(active_struct->name, active_struct, 0) == FAILED)
      return FAILED;
    
    /* create the SIZEOF-definition for the entire struct */
    active_struct->size = max_enum_offset;

    if (create_sizeof_definitions == YES) {
      if (strlen(active_struct->name) > MAX_NAME_LENGTH - 8) {
        snprintf(emsg, sizeof(emsg), "STRUCT \"%s\"'s name is too long!\n", active_struct->name);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", active_struct->name);
      if (add_a_new_definition(tmpname, (double)active_struct->size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
    }
    
    if (active_struct->items == NULL) {
      snprintf(emsg, sizeof(emsg), "STRUCT \"%s\" is empty!\n", active_struct->name);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    active_struct->next = structures_first;
    structures_first = active_struct;

    in_struct = NO;
    active_struct = NULL;
    return SUCCEEDED;
  }

  if (tmp[strlen(tmp) - 1] == ':')
    tmp[strlen(tmp) - 1] = 0;
  strcpy(tmpname, tmp);

  /* get the size/type */
  if (get_next_token() == FAILED)
    return FAILED;
    
  type = 0;
  size = 0;

  if (strcaselesscmp(tmp, "DB") == 0 || strcaselesscmp(tmp, "BYT") == 0 || strcaselesscmp(tmp, "BYTE") == 0) {
    size = 1;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(tmp, "DW") == 0 || strcaselesscmp(tmp, "WORD") == 0 || strcaselesscmp(tmp, "ADDR") == 0) {
    size = 2;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
#ifdef W65816
  else if (strcaselesscmp(tmp, "DL") == 0 || strcaselesscmp(tmp, "LONG") == 0 || strcaselesscmp(tmp, "FARADDR") == 0) {
    size = 3;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
#endif
  else if (strcaselesscmp(tmp, "DS") == 0 || strcaselesscmp(tmp, "DSB") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DS/DSB needs size.\n", ERROR_DIR);
      return FAILED;
    }
    size = d;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(tmp, "DSW") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSW needs size.\n", ERROR_DIR);
      return FAILED;
    }
    size = 2*d;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
#ifdef W65816
  else if (strcaselesscmp(tmp, "DSL") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSL needs size.\n", ERROR_DIR);
      return FAILED;
    }
    size = 3*d;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
#endif
  /* it's an instance of a structure! */
  else if (strcaselesscmp(tmp, "INSTANCEOF") == 0) {
    type = STRUCTURE_ITEM_TYPE_INSTANCEOF;

    if (get_next_token() == FAILED)
      return FAILED;

    st = get_structure(tmp);

    if (st == NULL) {
      snprintf(emsg, sizeof(emsg), "No STRUCT named \"%s\" available.\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* get the number of structures to be made */
    inz = input_number();
    if (inz == INPUT_NUMBER_EOL) {
      next_line();
      size = st->size;
      d = 1;
    }
    else if (inz == SUCCEEDED) {
      if (d < 1) {
        print_error("The number of structures must be greater than 0.\n", ERROR_DIR);
        return FAILED;
      }

      size = st->size * d;
    }
    else {
      if (inz == INPUT_NUMBER_STRING)
        snprintf(emsg, sizeof(emsg), "Expected the number of structures, got \"%s\" instead.\n", label);
      else
        snprintf(emsg, sizeof(emsg), "Expected the number of structures.\n");
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (compare_next_token("STARTFROM") == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED) {
        if (d < 0) {
          snprintf(emsg, sizeof(emsg), "STARTFROM needs to be >= 0.\n");
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        start_from = d;
      }
      else {
        snprintf(emsg, sizeof(emsg), "STARTFROM needs a number >= 0.\n");
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
  }
  else if (strcaselesscmp(tmp, ".db") == 0 || strcaselesscmp(tmp, ".byt") == 0 ||
           strcaselesscmp(tmp, ".byte") == 0) {
    /* don't do anything for "dotted" versions */
    size = 1;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(tmp, ".dw") == 0 || strcaselesscmp(tmp, ".word") == 0 ||
           strcaselesscmp(tmp, ".addr") == 0) {
    /* don't do anything for "dotted" versions */
    size = 2;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(tmp, ".ds") == 0 || strcaselesscmp(tmp, ".dsb") == 0 || strcaselesscmp(tmp, ".dsw") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      snprintf(emsg, sizeof(emsg), "%s needs size.\n", bak);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (strcaselesscmp(bak, ".dsw") == 0)
      d *= 2;

    size = d;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
#ifdef W65816
  else if (strcaselesscmp(tmp, ".dl") == 0 || strcaselesscmp(tmp, ".long") == 0 || strcaselesscmp(tmp, ".faraddr") == 0) {
    /* don't do anything for "dotted" versions */
    size = 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(tmp, ".dsl") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".DSL needs size.\n", ERROR_DIR);
      return FAILED;
    }

    size = d * 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
#endif
  else {
    if (in_enum == YES)
      snprintf(emsg, sizeof(emsg), "Unexpected symbol \"%s\" in .ENUM.\n", tmp);
    else if (in_ramsection == YES)
      snprintf(emsg, sizeof(emsg), "Unexpected symbol \"%s\" in .RAMSECTION.\n", tmp);
    else /* struct */
      snprintf(emsg, sizeof(emsg), "Unexpected symbol \"%s\" in .STRUCT.\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* add this label/value to the struct. */
  si = calloc(sizeof(struct structure_item), 1);
  if (si == NULL) {
    print_error("Out of memory while allocating a new STRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  si->next = NULL;
  strcpy(si->name, tmpname);
  si->size = size;
  si->type = type;
  si->start_from = start_from;
  if (type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
    si->instance = st;
    si->num_instances = si->size/st->size;
  }
  else if (type == STRUCTURE_ITEM_TYPE_UNION)
    si->union_items = st;

  if (active_struct->items == NULL)
    active_struct->items = si;
  if (active_struct->last_item != NULL)
    active_struct->last_item->next = si;
  active_struct->last_item = si;

  if (type != STRUCTURE_ITEM_TYPE_DOTTED)
    enum_offset += size;

  if (enum_offset > max_enum_offset)
    max_enum_offset = enum_offset;

  return SUCCEEDED;
}


int directive_org(void) {

  int q;
  
  no_library_files(".ORG definitions");
    
  if (bank_defined == 0) {
    print_error("No .BANK is defined.\n", ERROR_LOG);
    return FAILED;
  }
  if (section_status == ON) {
    print_error("You can't issue .ORG inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (dstruct_status == ON) {
    print_error("You can't issue .ORGA inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error(".ORG needs a positive or zero integer value.\n", ERROR_DIR);
    return FAILED;
  }

  org_defined = 1;
  fprintf(file_out_ptr, "O%d ", d);

  return SUCCEEDED;
}


int directive_orga(void) {

  int q;
  
  no_library_files(".ORGA definitions");
    
  if (bank_defined == 0) {
    print_error("No .BANK is defined.\n", ERROR_LOG);
    return FAILED;
  }
  if (section_status == ON) {
    print_error("You can't issue .ORGA inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (dstruct_status == ON) {
    print_error("You can't issue .ORGA inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error(".ORGA needs a positive or zero integer value.\n", ERROR_DIR);
    return FAILED;
  }

  org_defined = 1;

  ind = slots[current_slot].address;
  if (d < ind || d > (ind + slots[current_slot].size)) {
    print_error(".ORGA is outside the current SLOT.\n", ERROR_DIR);
    return FAILED;
  }

  fprintf(file_out_ptr, "O%d ", d - ind);

  return SUCCEEDED;
}


int directive_slot(void) {

  int q;
  
  no_library_files(".SLOT definitions");
    
  if (section_status == ON) {
    print_error("You can't issue .SLOT inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (dstruct_status == ON) {
    print_error("You can't issue .SLOT inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
    /* turn the label into a number */
    if (_get_slot_number_by_its_name(label, &d) == FAILED)
      return FAILED;
    q = SUCCEEDED;
  }
  else if (q == SUCCEEDED) {
    /* is the number a direct SLOT number, or is it an address? */
    q = _get_slot_number_by_a_value(d, &d);
  }
  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error("Cannot find the SLOT.\n", ERROR_DIR);
    return FAILED;
  }

  if (d < 0 || d > 255 || slots[d].size == 0) {
    snprintf(emsg, sizeof(emsg), "There is no SLOT number %d.\n", d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  fprintf(file_out_ptr, "B%d %d ", bank, d);

  current_slot = d;

  return SUCCEEDED;
}


int directive_bank(void) {

  int q;
  
  no_library_files(".BANK definitions");
    
  if (section_status == ON) {
    snprintf(emsg, sizeof(emsg), "Section \"%s\" is open. Do not try to change the bank.\n", sections_last->name);
    print_error(emsg, ERROR_LOG);
    return FAILED;
  }
  if (dstruct_status == ON) {
    print_error("You can't use .BANK inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  if (rombanks_defined == 0 && output_format != OUTPUT_LIBRARY) {
    print_error(".ROMBANKS is not yet defined.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || d < 0) {
    print_error(".BANK number must be zero or positive.\n", ERROR_DIR);
    return FAILED;
  }

  if (rombanks <= d && output_format != OUTPUT_LIBRARY) {
    snprintf(emsg, sizeof(emsg), "ROM banks == %d, selected bank %d.\n", rombanks, d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  bank = d;
  bank_defined = 1;

  if (compare_next_token("SLOT") == SUCCEEDED) {
    skip_next_token();

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
      /* turn the label into a number */
      if (_get_slot_number_by_its_name(label, &d) == FAILED)
        return FAILED;
      q = SUCCEEDED;
    }
    else if (q == SUCCEEDED) {
      /* is the number a direct SLOT number, or is it an address? */
      q = _get_slot_number_by_a_value(d, &d);
    }
    if (q != SUCCEEDED) {
      print_error("Cannot find the SLOT.\n", ERROR_DIR);
      return FAILED;
    }

    if (slots[d].size == 0) {
      snprintf(emsg, sizeof(emsg), "There is no SLOT number %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (output_format != OUTPUT_LIBRARY)
      fprintf(file_out_ptr, "B%d %d ", bank, d);

    ind = bank;
    inz = d;
    current_slot = d;
  }
  else if (output_format != OUTPUT_LIBRARY) {
    fprintf(file_out_ptr, "B%d %d ", d, defaultslot);
    ind = d;
    inz = defaultslot;
    current_slot = defaultslot;
  }

  if (slots[inz].size < banks[ind]) {
    snprintf(emsg, sizeof(emsg), "SLOT %d's size %d < BANK %d's size %d.\n", inz, slots[inz].size, ind, banks[ind]);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
  if (slots[inz].size > banks[ind]) {
    snprintf(emsg, sizeof(emsg), "SLOT %d's size %d > BANK %d's size %d, but the bank fits inside.\n", inz, slots[inz].size, ind, banks[ind]);
    print_error(emsg, ERROR_WRN);
  }

  return SUCCEEDED;
}


int directive_dbm_dwm_dlm(void) {
  
  struct macro_static *m;
  char bak[MAX_NAME_LENGTH + 1];
  
  strcpy(bak, cp);
  inz = input_number();
  if (inz != INPUT_NUMBER_ADDRESS_LABEL) {
    snprintf(emsg, sizeof(emsg), ".%s requires macro name.\n", bak);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* find the macro */
  if (macro_get(label, YES, &m) == FAILED)
    return FAILED;
  if (m == NULL) {
    if (macro_get(label, NO, &m) == FAILED)
      return FAILED;
  }

  if (m == NULL) {
    snprintf(emsg, sizeof(emsg), "No MACRO \"%s\" defined.\n", label);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (strcaselesscmp(cp, "DBM") == 0) {
    if (macro_start_dxm(m, MACRO_CALLER_DBM, cp, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(cp, "DLM") == 0) {
    if (macro_start_dxm(m, MACRO_CALLER_DLM, cp, YES) == FAILED)
      return FAILED;
  }
  else {
    if (macro_start_dxm(m, MACRO_CALLER_DWM, cp, YES) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_table(void) {

  char bak[256];

  inz = input_number();
  for (table_size = 0; table_size < (int)sizeof(table_format) && (inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL); ) {
    if (strcaselesscmp(label, "db") == 0 || strcaselesscmp(label, "byte") == 0 || strcaselesscmp(label, "byt") == 0) {
      table_format[table_size++] = 'b';
    }
    else if (strcaselesscmp(label, "ds") == 0 || strcaselesscmp(label, "dsb") == 0) {
      strcpy(bak, label);

      inz = input_number();
      if (inz == FAILED)
        return FAILED;
      if (inz != SUCCEEDED) {
        snprintf(emsg, sizeof(emsg), "%s needs size.\n", bak);
        print_error(emsg, ERROR_INP);
        return FAILED;
      }

      for (inz = 0; inz < d && table_size < (int)sizeof(table_format); inz++)
        table_format[table_size++] = 'b';
    }
    else if (strcaselesscmp(label, "dw") == 0 || strcaselesscmp(label, "word") == 0 || strcaselesscmp(label, "addr") == 0) {
      table_format[table_size++] = 'w';
    }
    else if (strcaselesscmp(label, "dsw") == 0) {
      strcpy(bak, label);

      inz = input_number();
      if (inz == FAILED)
        return FAILED;
      if (inz != SUCCEEDED) {
        snprintf(emsg, sizeof(emsg), "%s needs size.\n", bak);
        print_error(emsg, ERROR_INP);
        return FAILED;
      }

      for (inz = 0; inz < d && table_size < (int)sizeof(table_format); inz++)
        table_format[table_size++] = 'w';
    }
#ifdef W65816
    else if (strcaselesscmp(label, "dl") == 0 || strcaselesscmp(label, "long") == 0 || strcaselesscmp(label, "faraddr") == 0) {
      table_format[table_size++] = 'l';
    }
    else if (strcaselesscmp(label, "dsl") == 0) {
      strcpy(bak, label);

      inz = input_number();
      if (inz == FAILED)
        return FAILED;
      if (inz != SUCCEEDED) {
        snprintf(emsg, sizeof(emsg), "%s needs size.\n", bak);
        print_error(emsg, ERROR_INP);
        return FAILED;
      }

      for (inz = 0; inz < d && table_size < (int)sizeof(table_format); inz++)
        table_format[table_size++] = 'l';
    }
#endif
    else {
      snprintf(emsg, sizeof(emsg), "Unknown symbol \"%s\".\n", label);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
      
    inz = input_number();
  }

  if (table_size >= (int)sizeof(table_format)) {
    snprintf(emsg, sizeof(emsg), ".TABLE is out of size.\n");
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (inz == FAILED)
    return FAILED;
  else if (inz == INPUT_NUMBER_EOL && table_size == 0) {
    snprintf(emsg, sizeof(emsg), ".TABLE needs data.\n");
    print_error(emsg, ERROR_INP);
    return FAILED;
  }
  else if (inz == INPUT_NUMBER_EOL)
    next_line();
  else {
    snprintf(emsg, sizeof(emsg), "Unknown symbol.\n");
    print_error(emsg, ERROR_DIR);
    return FAILED;      
  }

  table_defined = 1;
  table_index = 0;

  return SUCCEEDED;    
}


int directive_row_data(void) {

  char bak[256];
  int rows = 0;
  
  strcpy(bak, cp);

  if (table_defined == 0) {
    snprintf(emsg, sizeof(emsg), ".TABLE needs to be defined before .%s can be used.\n", bak);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
    
  if (strcaselesscmp(bak, "ROW") == 0) {
    if (table_index != 0) {
      snprintf(emsg, sizeof(emsg), ".ROW cannot be used here. .DATA needs to be used again to give the remaining of the row.\n");
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
  }

  inz = input_number();
  ind = 0;
  for ( ; inz == SUCCEEDED || inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ) {
    if (inz == INPUT_NUMBER_STRING) {
      if (table_format[table_index] == 'b') {
        if (strlen(label) != 1) {
          snprintf(emsg, sizeof(emsg), ".%s was expecting a byte, got %d bytes instead.\n", bak, (int)strlen(label));
          print_error(emsg, ERROR_INP);
          return FAILED;
        }

        fprintf(file_out_ptr, "d%d ", label[0]);          
      }
      else if (table_format[table_index] == 'w') {
        if (strlen(label) > 2 || strlen(label) <= 0) {
          snprintf(emsg, sizeof(emsg), ".%s was expecting a word (2 bytes), got %d bytes instead.\n", bak, (int)strlen(label));
          print_error(emsg, ERROR_INP);
          return FAILED;
        }

        fprintf(file_out_ptr, "y%d ", (label[0] << 8) | label[1]);
      }
#ifdef W65816
      else if (table_format[table_index] == 'l') {
        if (strlen(label) > 3 || strlen(label) <= 0) {
          snprintf(emsg, sizeof(emsg), ".%s was expecting a long (3 bytes), got %d bytes instead.\n", bak, (int)strlen(label));
          print_error(emsg, ERROR_INP);
          return FAILED;
        }

        fprintf(file_out_ptr, "z%d ", (label[0] << 16) | (label[1] << 8) | label[2]);
      }
#endif
      else {
        snprintf(emsg, sizeof(emsg), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, table_format[table_index]);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
    else if (inz == SUCCEEDED) {
      if (table_format[table_index] == 'b') {
        if (d < -128 || d > 255) {
          snprintf(emsg, sizeof(emsg), ".%s expects 8-bit data, %d is out of range!\n", bak, d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
    
        fprintf(file_out_ptr, "d%d ", d);
      }
      else if (table_format[table_index] == 'w') {
        if (d < -32768 || d > 65535) {
          snprintf(emsg, sizeof(emsg), ".%s expects 16-bit data, %d is out of range!\n", bak, d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        fprintf(file_out_ptr, "y%d ", d);
      }
#ifdef W65816
      else if (table_format[table_index] == 'l') {
        if (d < -8388608 || d > 16777215) {
          snprintf(emsg, sizeof(emsg), ".%s expects 24-bit data, %d is out of range!\n", bak, d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        fprintf(file_out_ptr, "z%d ", d);
      }
#endif
      else {
        snprintf(emsg, sizeof(emsg), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, table_format[table_index]);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
    else if (inz == INPUT_NUMBER_ADDRESS_LABEL) {
      if (table_format[table_index] == 'b') {
        fprintf(file_out_ptr, "k%d Q%s ", active_file_info_last->line_current, label);
      }
      else if (table_format[table_index] == 'w') {
        fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, label);
      }
#ifdef W65816
      else if (table_format[table_index] == 'l') {
        fprintf(file_out_ptr, "k%d q%s ", active_file_info_last->line_current, label);
      }
#endif
      else {
        snprintf(emsg, sizeof(emsg), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, table_format[table_index]);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
    else if (inz == INPUT_NUMBER_STACK) {
      if (table_format[table_index] == 'b') {
        fprintf(file_out_ptr, "c%d ", latest_stack);
      }
      else if (table_format[table_index] == 'w') {
        fprintf(file_out_ptr, "C%d ", latest_stack);
      }
#ifdef W65816
      else if (table_format[table_index] == 'l') {
        fprintf(file_out_ptr, "T%d ", latest_stack);
      }
#endif
      else {
        snprintf(emsg, sizeof(emsg), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, table_format[table_index]);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }

    ind++;
    table_index++;
    if (table_index >= table_size) {
      rows++;
      table_index = 0;
    }

    inz = input_number();
  }

  if (inz == FAILED)
    return FAILED;

  if (inz == INPUT_NUMBER_EOL && ind == 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (strcaselesscmp(bak, "ROW") == 0) {
    if (table_index != 0 || rows != 1) {
      snprintf(emsg, sizeof(emsg), ".ROW needs exactly one row of data, no more, no less.\n");
      print_error(emsg, ERROR_INP);
      return FAILED;
    }
  }

  if (inz == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_db_byt_byte(void) {

  char bak[256];
  int o;

  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  strcpy(bak, cp);

  inz = input_number();
  for (ind = 0; inz == SUCCEEDED || inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ind++) {
    if (inz == INPUT_NUMBER_STRING) {
      for (o = 0; o < string_size; o++) {
        /* handle '\0' */
        if (label[o] == '\\' && label[o + 1] == '0') {
          fprintf(file_out_ptr, "d%d ", '\0');
          o++;
        }
        /* handle '\x' */
        else if (label[o] == '\\' && label[o + 1] == 'x') {
          char tmp_a[8], *tmp_b;
          int tmp_c;
        
          o += 3;
          snprintf(tmp_a, sizeof(tmp_a), "%c%c", label[o-1], label[o]);
          tmp_c = (int)strtol(tmp_a, &tmp_b, 16);
          if (*tmp_b) {
            snprintf(emsg, sizeof(emsg), ".%s '\\x' needs hexadecimal byte (00-FF) data.\n", bak);
            print_error(emsg, ERROR_INP);
            return FAILED;
          }
          fprintf(file_out_ptr, "d%d ", tmp_c);
        }
        /* handle '\<' */
        else if (label[o] == '\\' && label[o + 1] == '<') {
          o += 2;
          if (o == string_size) {
            snprintf(emsg, sizeof(emsg), ".%s '\\<' needs character data.\n", bak);
            print_error(emsg, ERROR_INP);
            return FAILED;
          }
          fprintf(file_out_ptr, "d%d ", (int)label[o]|0x80);
        }
        /* handle '\>' */
        else if (label[o] == '\\' && label[o + 1] == '>' && o == 0) {
          snprintf(emsg, sizeof(emsg), ".%s '\\>' needs character data.\n", bak);
          print_error(emsg, ERROR_INP);
          return FAILED;
        }
        else if (label[o + 1] == '\\' && label[o + 2] == '>') {
          fprintf(file_out_ptr, "d%d ", (int)label[o]|0x80);
          o += 2;
        }
        /* handle '\\' */
        else if (label[o] == '\\' && label[o + 1] == '\\') {
          fprintf(file_out_ptr, "d%d ", '\\');
          o++;
        }
        else
          fprintf(file_out_ptr, "d%d ", (int)label[o]);
      }
      inz = input_number();
      continue;
    }

    if (inz == SUCCEEDED && (d < -128 || d > 255)) {
      snprintf(emsg, sizeof(emsg), ".%s expects 8-bit data, %d is out of range!\n", bak, d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (inz == SUCCEEDED)
      fprintf(file_out_ptr, "d%d ", d);
    else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "Q%s ", label);
    else if (inz == INPUT_NUMBER_STACK)
      fprintf(file_out_ptr, "c%d ", latest_stack);

    inz = input_number();
  }

  if (inz == FAILED)
    return FAILED;

  if (inz == INPUT_NUMBER_EOL && ind == 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (inz == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_asctable_asciitable(void) {
  
  int astart, aend, q, o;
  char bak[256];
  
  strcpy(bak, cp);

  /* clear the table (to the default n->n -mapping) */
  for (o = 0; o < 256; o++)
    asciitable[o] = o;

  /* read the entries */
  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDA") == 0)
      break;
    else if (strcaselesscmp(tmp, "MAP") == 0) {
      q = input_number();

      while (q == INPUT_NUMBER_EOL) {
        next_line();
        q = input_number();
      }

      if (q == FAILED)
        return FAILED;
      if (q == SUCCEEDED && (d < 0 || d > 255)) {
        print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
        return FAILED;
      }
      if (q == INPUT_NUMBER_STRING) {
        if (string_size != 1) {
          print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
          return FAILED;
        }
        else {
          d = label[0];
          if (d < 0)
            d += 256;
        }
      }

      astart = d;
      aend = d+1;

      /* do we have a range? */
      if (compare_next_token("TO") == SUCCEEDED) {
        skip_next_token();

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q == SUCCEEDED && (d < 0 || d > 255)) {
          print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
          return FAILED;
        }
        if (q == INPUT_NUMBER_STRING) {
          if (string_size != 1) {
            print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
            return FAILED;
          }
          else {
            d = label[0];
            if (d < 0)
              d += 256;
          }
        }

        aend = d+1;
      }

      if (aend <= astart) {
        print_error("The end address of the mapping must be larger than the staring address.\n", ERROR_DIR);
        return FAILED;
      }

      /* skip the "=" */
      if (compare_next_token("=") != SUCCEEDED) {
        ind = FAILED;
        break;
      }
      skip_next_token();

      /* read the starting address */
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q == SUCCEEDED && (d < 0 || d > 255)) {
        print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
        return FAILED;
      }
      if (q != SUCCEEDED) {
        print_error("The entry must be a positive 8-bit immediate value.\n", ERROR_DIR);
        return FAILED;
      }

      /* build the mapping */
      for (o = astart; o < aend; o++) {
        if (d >= 256) {
          print_error("The mapping overflows from the ASCII table!\n", ERROR_DIR);
          return FAILED;
        }
        asciitable[o] = d++;
      }
    }
    else {
      ind = FAILED;
      break;
    }
  }

  if (ind != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), "Error in .%s data structure.\n", bak);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  asciitable_defined = 1;

  return SUCCEEDED;
}


int directive_asc(void) {

  char bak[256];
  int o, q;

  strcpy(bak, cp);

  if (asciitable_defined == 0) {
    print_error("No .ASCIITABLE defined. Using the default n->n -mapping.\n", ERROR_WRN);
    for (o = 0; o < 256; o++)
      asciitable[o] = o;
  }

  while (1) {
    q = input_number();
    if (q == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }

    if (q == INPUT_NUMBER_STRING) {
      /* remap the ascii string */
      for (o = 0; o < string_size; o++) {
        ind = label[o];
        /* handle '\0' */
        if (label[o] == '\\' && label[o + 1] == '0') {
          ind = '\0';
          fprintf(file_out_ptr, "d%d ", ind);
          o++;
        }
        /* handle '\x' */
        else if (label[o] == '\\' && label[o + 1] == 'x') {
          char tmp_a[8], *tmp_b;
          int tmp_c;
      
          o += 3;
          snprintf(tmp_a, sizeof(tmp_a), "%c%c", label[o-1], label[o]);
          tmp_c = (int)strtol(tmp_a, &tmp_b, 16);
          if (*tmp_b) {
            snprintf(emsg, sizeof(emsg), ".%s '\\x' needs hexadecimal byte (00-FF) data.\n", bak);
            print_error(emsg, ERROR_INP);
            return FAILED;
          }
          ind = tmp_c;
          fprintf(file_out_ptr, "d%d ", ind);
        }
        else {
          int tmp_a = 0;
        
          /* handle '\<' */
          if (label[o] == '\\' && label[o + 1] == '<') {
            o += 2;
            if (o == string_size) {
              snprintf(emsg, sizeof(emsg), ".%s '\\<' needs character data.\n", bak);
              print_error(emsg, ERROR_INP);
              return FAILED;
            }
            tmp_a = 0x80;
            ind = label[o];
          }
          /* handle '\>' */
          else if (label[o] == '\\' && label[o + 1] == '>' && o == 0) {
            snprintf(emsg, sizeof(emsg), ".%s '\\>' needs character data.\n", bak);
            print_error(emsg, ERROR_INP);
            return FAILED;
          }
          else if (label[o + 1] == '\\' && label[o + 2] == '>') {
            tmp_a = 0x80;
            o += 2;
          }
          /* handle '\\' */
          else if (label[o] == '\\' && label[o + 1] == '\\') {
            ind = '\\';
            o++;
          }
          if (ind < 0)
            ind += 256;
          ind = (int)asciitable[ind];
          fprintf(file_out_ptr, "d%d ", ind|tmp_a);
        }
      }
    }
    else if (q == SUCCEEDED) {
      /* remap the byte */
      if (d < 0 || d > 255) {
        snprintf(emsg, sizeof(emsg), ".%s needs string / byte (0-255) data.\n", bak);
        print_error(emsg, ERROR_INP);
        return FAILED;
      }
      ind = (int)asciitable[d];
      fprintf(file_out_ptr, "d%d ", ind);
    }
    else {
      snprintf(emsg, sizeof(emsg), ".%s needs string / byte (0-255) data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_dw_word_addr(void) {

  char bak[256];

  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  strcpy(bak, cp);

  inz = input_number();
  for (ind = 0; inz == SUCCEEDED || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ind++) {
    if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
      snprintf(emsg, sizeof(emsg), ".%s expects 16-bit data, %d is out of range!\n", bak, d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (inz == SUCCEEDED)
      fprintf(file_out_ptr, "y%d", d);
    else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "r%s ", label);
    else if (inz == INPUT_NUMBER_STACK)
      fprintf(file_out_ptr, "C%d ", latest_stack);

    inz = input_number();
  }

  if (inz == FAILED)
    return FAILED;

  if ((inz == INPUT_NUMBER_EOL || inz == INPUT_NUMBER_STRING) && ind == 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (inz == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


#ifdef W65816

int directive_dl_long_faraddr(void) {

  char bak[256];

  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  strcpy(bak, cp);

  inz = input_number();
  for (ind = 0; inz == SUCCEEDED || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ind++) {
    if (inz == SUCCEEDED && (d < -8388608 || d > 16777215)) {
      snprintf(emsg, sizeof(emsg), ".%s expects 24-bit data, %d is out of range!\n", bak, d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (inz == SUCCEEDED)
      fprintf(file_out_ptr, "z%d ", d);
    else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "q%s ", label);
    else if (inz == INPUT_NUMBER_STACK)
      fprintf(file_out_ptr, "T%d ", latest_stack);

    inz = input_number();
  }

  if (inz == FAILED)
    return FAILED;

  if ((inz == INPUT_NUMBER_EOL || inz == INPUT_NUMBER_STRING) && ind == 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (inz == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_dsl(void) {

  int q;
  
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(".DSL needs size.\n", ERROR_INP);
    return FAILED;
  }

  if (d < 1 || d > 65535) {
    snprintf(emsg, sizeof(emsg), ".DSL expects a 16-bit positive integer as size, %d is out of range!\n", d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  inz = d;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(".DSL needs data.\n", ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (d < -8388608 || d > 16777215)) {
    snprintf(emsg, sizeof(emsg), ".DSL expects 24-bit data, %d is out of range!\n", d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(file_out_ptr, "h%d %d ", inz, d);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "q%s ", label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "T%d ", latest_stack);
  }

  return SUCCEEDED;
}


int directive_name_w65816(void) {
    
  no_library_files(".NAME");

  if ((ind = get_next_token()) == FAILED)
    return FAILED;

  if (ind != GET_NEXT_TOKEN_STRING) {
    print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
    return FAILED;
  }

  /* no name has been defined so far */
  if (name_defined == 0) {
    for (ind = 0; tmp[ind] != 0 && ind < 21; ind++)
      name[ind] = tmp[ind];

    if (ind == 21 && tmp[ind] != 0) {
      print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
      return FAILED;
    }

    for ( ; ind < 21; name[ind] = 0, ind++)
      ;

    name_defined = 1;
  }
  else {
    /* compare the names */
    for (ind = 0; tmp[ind] != 0 && name[ind] != 0 && ind < 21; ind++)
      if (name[ind] != tmp[ind])
        break;

    if (ind == 21 && tmp[ind] != 0) {
      print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (ind != 21 && (name[ind] != 0 || tmp[ind] != 0)) {
      print_error(".NAME was already defined.\n", ERROR_DIR);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

#endif


/* this is used for legacy .DSTRUCT syntax, and only for generating labels in the new
 * .DSTRUCT syntax. */
int parse_dstruct_entry(char *iname, struct structure *s, int *labels_only) {

  char tmpname[MAX_NAME_LENGTH*2+10];
  struct structure_item *it;
  int f, o, c, g;

  /* read the data */
  it = s->items;
  for (ind = 0; it != NULL; ind++) {
    snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
    if (verify_name_length(tmpname) == FAILED)
      return FAILED;

    if (it->type != STRUCTURE_ITEM_TYPE_UNION) { /* add field label */
      char full_label[MAX_NAME_LENGTH + 1];

      fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmpname);
    
      if (get_full_label(tmpname, full_label) == FAILED)
        return FAILED;
      if (add_label_sizeof(full_label, it->size) == FAILED)
        return FAILED;
    }

    if (it->type == STRUCTURE_ITEM_TYPE_UNION) {
      if (*labels_only == NO) {
        print_error(".DSTRUCT doesn't support structs with unions.\n", ERROR_DIR);
        return FAILED;
      }
      else {
        struct structure *us;

        us = it->union_items;
        while (us != NULL) {
          if (us->name[0] != '\0') { /* check if the union is named */
            char full_label[MAX_NAME_LENGTH + 1];

            snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, us->name);
            if (verify_name_length(tmpname) == FAILED)
              return FAILED;

            fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmpname);

            if (get_full_label(tmpname, full_label) == FAILED)
              return FAILED;
            if (add_label_sizeof(full_label, us->size) == FAILED)
              return FAILED;
          }
          else
            strcpy(tmpname, iname);

          parse_dstruct_entry(tmpname, us, labels_only);
          fprintf(file_out_ptr, "o%d 0 ", -us->size); /* rewind */
          us = us->next;
        }

        fprintf(file_out_ptr, "o%d 0 ", it->size); /* jump to union end */
      }
    }
    else if (it->type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
      /* handle .INSTANCEOF directive */
      /* update the naming prefix */
      snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
      if (verify_name_length(tmpname) == FAILED)
        return FAILED;

      if (it->num_instances == 1) {
        if (parse_dstruct_entry(tmpname, it->instance, labels_only) == FAILED)
          return FAILED;
      }
      else {
        int labels_only_tmp = YES;

        snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);

        /* we have "struct.instance" and "struct.1.instance" referencing the same data. */
        parse_dstruct_entry(tmpname, it->instance, &labels_only_tmp);

        /* return to start of struct */
        fprintf(file_out_ptr, "o%d 0 ", -it->instance->size);

        for (g = 1; g <= it->num_instances; g++) {
          snprintf(tmpname, sizeof(tmpname), "%s.%s.%d", iname, it->name, g);
          if (verify_name_length(tmpname) == FAILED)
            return FAILED;

          fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmpname);

          if (add_label_sizeof(tmpname, it->instance->size) == FAILED)
            return FAILED;
          if (parse_dstruct_entry(tmpname, it->instance, labels_only) == FAILED)
            return FAILED;
        }
      }

      it = it->next;
      continue;
    }
    else if (it->size == 0 || it->type == STRUCTURE_ITEM_TYPE_DOTTED) {
      /* don't put data into empty structure items */
      it = it->next;
      continue;
    }
    else {
      if (*labels_only == NO) {
        /* take care of the strings */
        if (inz == INPUT_NUMBER_STRING) {
          if (it->size < string_size) {
            snprintf(emsg, sizeof(emsg), "String \"%s\" doesn't fit into the %d bytes of \"%s.%s\". Discarding the overflow.\n", label, it->size, s->name, it->name);
            print_error(emsg, ERROR_WRN);
            c = it->size;
          }
          else
            c = string_size;

          /* copy the string */
          for (o = 0; o < c; o++)
            fprintf(file_out_ptr, "d%d ", (int)label[o]);
        }
        /* take care of the rest */
        else {
          if (it->size == 1) {
            if ((inz == SUCCEEDED) && (d < -128 || d > 255)) {
              snprintf(emsg, sizeof(emsg), "\"%s.%s\" expects 8-bit data, %d is out of range!\n", s->name, it->name, d);
              print_error(emsg, ERROR_DIR);
              return FAILED;
            }

            if (inz == SUCCEEDED)
              fprintf(file_out_ptr, "d%d ", d);
            else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(file_out_ptr, "k%d Q%s ", active_file_info_last->line_current, label);
            else if (inz == INPUT_NUMBER_STACK)
              fprintf(file_out_ptr, "c%d ", latest_stack);

            o = 1;
          }
          else {
            if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
              snprintf(emsg, sizeof(emsg), "\"%s.%s\" expects 16-bit data, %d is out of range!\n", s->name, it->name, d);
              print_error(emsg, ERROR_DIR);
              return FAILED;
            }

            if (inz == SUCCEEDED)
              fprintf(file_out_ptr, "y%d", d);
            else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, label);
            else if (inz == INPUT_NUMBER_STACK)
              fprintf(file_out_ptr, "C%d ", latest_stack);

            o = 2;
          }
          /* TODO: longs */
        }
        /* fill the rest of the item with emptyfill or zero */
        if (emptyfill_defined != 0)
          f = emptyfill;
        else
          f = 0;

        for (; o < it->size; o++)
          fprintf(file_out_ptr, "d%d ", f);
      }
      else { /* labels_only == YES */
        fprintf(file_out_ptr, "o%d 0 ", it->size);
      }
    }

    it = it->next;

    if (*labels_only == NO) {
      inz = input_number();
      if (!(inz == SUCCEEDED || inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK))
        *labels_only = YES; /* ran out of data to read */
    }
  }

  return SUCCEEDED;
}

/* search for "field_name" within a structure. return the corresponding structure_item and
   the offset within the structure it's located at. recurses through instanceof's and
   unions. */
int find_struct_field(struct structure *s, char *field_name, int *item_size, int *field_offset) {

  int offset = 0;
  char prefix[MAX_NAME_LENGTH + 1];
  char *after_dot;
  struct structure_item *si;

  strcpy(prefix, field_name);
  if (strchr(prefix, '.') != NULL) {
    *strchr(prefix, '.') = '\0';
    after_dot = field_name + strlen(prefix) + 1;
  }
  else
    after_dot = NULL;

  si = s->items;

  while (si != NULL) {
    if (si->type == STRUCTURE_ITEM_TYPE_UNION) {
      /* unions don't necessarily have names, so we need to check them all */
      struct structure *us;

      us = si->union_items;
      while (us != NULL) {
        if (us->name[0] != '\0') { /* has name */
          if (strcmp(field_name, us->name) == 0) {
            *item_size = us->size;
            *field_offset = offset;
            return SUCCEEDED;
          }
          if (after_dot != NULL && strcmp(prefix, us->name) == 0) {
            if (find_struct_field(us, after_dot, item_size, field_offset) == SUCCEEDED) {
              *field_offset += offset;
              return SUCCEEDED;
            }
          }
        }
        /* no name */
        else if (find_struct_field(us, field_name, item_size, field_offset) == SUCCEEDED) {
          *field_offset += offset;
          return SUCCEEDED;
        }
        us = us->next;
      }
    }
    else if (strcmp(field_name, si->name) == 0) {
      *field_offset = offset;
      *item_size = si->size;
      return SUCCEEDED;
    }
    /* look for prefix for an ".instanceof" */
    else if (after_dot != NULL && strcmp(prefix, si->name) == 0) {
      if (si->type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
        if (find_struct_field(si->instance, after_dot, item_size, field_offset) == SUCCEEDED) {
          *field_offset += offset;
          return SUCCEEDED;
        }
        /* look for ie. "struct.1.field" */
        else if (si->num_instances > 1) {
          int g;
          for (g = 1; g <= si->num_instances; g++) {
            char num_str[256];
        
            snprintf(num_str, sizeof(num_str), "%d", g);
            if (strncmp(num_str, after_dot, strlen(num_str)) == 0) {
              /* entire string matched? */
              if (strcmp(num_str, after_dot) == 0) {
                *field_offset = offset + (g-1) * si->instance->size;
                *item_size = si->instance->size;
                return SUCCEEDED;
              }
              /* only prefix matched */
              if (after_dot[strlen(num_str)] == '.' && find_struct_field(si->instance, after_dot + strlen(num_str) + 1, item_size, field_offset) == SUCCEEDED) {
                *field_offset += offset + (g-1) * si->instance->size;
                return SUCCEEDED;
              }
            }
          }
        }
      }
      /* else keep looking */
    }

    if (si->type != STRUCTURE_ITEM_TYPE_DOTTED)
      offset += si->size;

    si = si->next;
  }

  return FAILED;
}


int directive_dstruct(void) {

  char iname[MAX_NAME_LENGTH*2+5];
  struct structure *s;
  int q, q2;
  int labels_only;

  if (compare_next_token("INSTANCEOF") == SUCCEEDED) { /* nameless */
    skip_next_token();
    iname[0] = '\0';
  }
  else {
    /* get instance name */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".DSTRUCT needs a name for the instance.\n", ERROR_INP);
      return FAILED;
    }
    strcpy(iname, label);

    if (compare_next_token("INSTANCEOF") == SUCCEEDED)
      skip_next_token();
  }

  /* get structure name */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".DSTRUCT needs a structure name.\n", ERROR_INP);
    return FAILED;
  }

  /* find the structure */
  s = get_structure(label);

  if (s == NULL) {
    snprintf(emsg, sizeof(emsg), "Reference to an unidentified structure \"%s\".\n", label);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* DEBUG
     {
     struct structure_item *sS = s->items;
        
     fprintf(stderr, "STRUCT \"%s\" size %d\n", s->name, s->size);
     while (sS != NULL) {
     fprintf(stderr, "ITEM \"%s\" size %d\n", sS->name, sS->size);
     sS = sS->next;
     }
     }
  */

  if (iname[0] != '\0') {
    char full_label[MAX_NAME_LENGTH + 1];
    
    fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, iname);

    if (get_full_label(iname, full_label) == FAILED)
      return FAILED;
    if (add_label_sizeof(full_label, s->size) == FAILED)
      return FAILED;
  }

  if (compare_next_token("VALUES") == SUCCEEDED) {
    /* new syntax */

    int field_offset;
    char field_name[MAX_NAME_LENGTH + 1];
    int item_size;

    if (dstruct_status == ON) {
      print_error("You can't have nested .DSTRUCT's.\n", ERROR_DIR);
      return FAILED;
    }

    dstruct_status = ON;

    skip_next_token();

    fprintf(file_out_ptr, "e%d -1 ", s->size); /* mark start address of dstruct */

    q = get_next_token();

    while (q == SUCCEEDED) {
      if ((q2 = parse_if_directive()) != -1) {
        return q2;
      }
      if (strcaselesscmp(tmp, ".ENDST") == 0) {
        break;
      }
      else {
        if (tmp[strlen(tmp)-1] == ':')
          tmp[strlen(tmp)-1] = '\0';
        strcpy(field_name, tmp);

        if (find_struct_field(s, field_name, &item_size, &field_offset) == FAILED) {
          snprintf(emsg, sizeof(emsg), ".DSTRUCT: Couldn't find field \"%s\" in structure \"%s\".\n", field_name, s->name);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
        fprintf(file_out_ptr, "e%d %d ", field_offset, item_size);

        do {
          if ((q = get_next_token()) == FAILED) {
            print_error("Error parsing .DSTRUCT.\n", ERROR_ERR);
            return FAILED;
          }

          if (tmp[0] != '.' || strcaselesscmp(tmp, ".ENDST") == 0)
            break;

          if (parse_directive() == FAILED)
            return FAILED;
        }
        while (1);
      }
    }

    if (q != SUCCEEDED) {
      print_error("Error parsing .DSTRUCT.\n", ERROR_ERR);
      return FAILED;
    }

    /* now generate labels */
    if (iname[0] != '\0') {
      labels_only = YES;
      fprintf(file_out_ptr, "e%d -3 ", 0); /* back to start of struct */
      if (parse_dstruct_entry(iname, s, &labels_only) == FAILED)
        return FAILED;
    }

    fprintf(file_out_ptr, "e%d -3 ", 0); /* back to start of struct */
    fprintf(file_out_ptr, "e%d -2 ", s->size); /* mark end of .DSTRUCT */

    dstruct_status = OFF;

    return SUCCEEDED;
  }
  else if (compare_next_token("DATA") == SUCCEEDED)
    skip_next_token();

  /* legacy syntax */

  inz = input_number();
  labels_only = NO;
  if (parse_dstruct_entry(iname, s, &labels_only) == FAILED)
    return FAILED;

  if (inz == INPUT_NUMBER_EOL)
    next_line();
  else {
    snprintf(emsg, sizeof(emsg), "Too much data for structure \"%s\".\n", s->name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_dsb_ds(void) {

  char bak[256];
  int q;
  
  strcpy(bak, cp);

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), ".%s needs size.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (d < 1 || d > 65535) {
    snprintf(emsg, sizeof(emsg), ".%s expects a 16-bit positive integer as size, %d is out of range!\n", bak, d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  inz = d;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    snprintf(emsg, sizeof(emsg), ".%s needs data.\n", bak);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (d > 255 || d < -128)) {
    snprintf(emsg, sizeof(emsg), ".%s expects 8-bit data, %d is out of range!\n", bak, d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(file_out_ptr, "x%d %d ", inz, d);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "R%s ", label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "c%d ", latest_stack);
  }

  return SUCCEEDED;
}


int directive_dsw(void) {

  int q;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(".DSW needs size.\n", ERROR_INP);
    return FAILED;
  }

  if (d < 1 || d > 65535) {
    snprintf(emsg, sizeof(emsg), ".DSW expects a 16-bit positive integer as size, %d is out of range!\n", d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  inz = d;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(".DSW needs data.\n", ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (d < -32768 || d > 65535)) {
    snprintf(emsg, sizeof(emsg), ".DSW expects 16-bit data, %d is out of range!\n", d);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(file_out_ptr, "X%d %d ", inz, d);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "r%s ", label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < inz; q++)
      fprintf(file_out_ptr, "C%d ", latest_stack);
  }

  return SUCCEEDED;
}


int directive_incdir(void) {
  
  int q, o;
  char *c;

  expect_calculations = NO;
  o = input_number();
  expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".INCDIR needs a directory string.\n", ERROR_DIR);
    return FAILED;
  }

  q = (int)strlen(label);

  /* use the default dir? */
  if (q == 0) {
    if (include_dir != NULL)
      include_dir[0] = 0;
    return SUCCEEDED;
  }

  /* use the given dir */
  o = (int)(strlen(label) + 2);
  if (o > include_dir_size) {
    c = realloc(include_dir, o);
    if (c == NULL) {
      print_error("Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    include_dir = c;
    include_dir_size = o;
  }

  /* convert the path string to local enviroment */
  strcpy(include_dir, label);
  localize_path(include_dir);

  /* terminate the string with '/' */
#ifdef MSDOS
  if (include_dir[q - 1] != '\\') {
    include_dir[q] = '\\';
    include_dir[q + 1] = 0;
  }
#else
  if (include_dir[q - 1] != '/') {
    include_dir[q] = '/';
    include_dir[q + 1] = 0;
  }
#endif

  return SUCCEEDED;
}


int directive_include(int is_real) {

  int o, include_size = 0, accumulated_name_length = 0, character_c_position = 0, got_once = NO;
  char namespace[MAX_NAME_LENGTH + 1], path[MAX_NAME_LENGTH + 1], accumulated_name[MAX_NAME_LENGTH + 1];

  if (is_real == YES) {
    /* turn the .INCLUDE/.INC into .INDLUDE/.IND to mark it as used, if ONCE is used,
       for repetitive macro calls that contain .INCLUDE/.INC... */
    o = i;
    while (o >= 0) {
      if (toupper(buffer[o+0]) == 'I' &&
          toupper(buffer[o+1]) == 'N' &&
          toupper(buffer[o+2]) == 'C') {
        character_c_position = o+2;
        break;
      }
      o--;
    }
  }

  accumulated_name[0] = 0;

  while (1) {
    if (compare_next_token("NAMESPACE") == SUCCEEDED || compare_next_token("ONCE") == SUCCEEDED)
      break;

    expect_calculations = NO;
    o = input_number();
    expect_calculations = YES;
    
    if (o == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".INCLUDE needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    if (accumulated_name_length + strlen(label) >= sizeof(accumulated_name)) {
      print_error("The accumulated file name length >= MAX_NAME_LENGTH. Increase its size in shared.h and recompile WLA.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(&accumulated_name[accumulated_name_length], label);
    accumulated_name_length = (int)strlen(accumulated_name);
  }

  strcpy(path, accumulated_name);

  /* convert the path to local enviroment */
  localize_path(label);
  
  if (compare_next_token("NAMESPACE") != SUCCEEDED)
    namespace[0] = 0;
  else {
    skip_next_token();

    expect_calculations = NO;
    o = input_number();
    expect_calculations = YES;
    
    if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error("Namespace string is missing.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(namespace, label);
  }

  if (compare_next_token("ONCE") == SUCCEEDED) {
    skip_next_token();

    got_once = YES;
  }
  
  if (is_real == YES) {
    if (include_file(path, &include_size, namespace) == FAILED)
      return FAILED;
  
    /* WARNING: this is tricky: did we just include a file inside a macro? */
    if (macro_active != 0) {
      /* yes. note that the now we added new bytes after i, so if a macro_return_i is
         bigger than i, we'll need to add the bytes to it */
      struct macro_static *ms;
      int q, w;

      for (q = 0; q < macro_active; q++) {
        if (macro_stack[q].macro_return_i > i)
          macro_stack[q].macro_return_i += include_size;
        for (w = 0; w < macro_stack[q].supplied_arguments; w++) {
          if (macro_stack[q].argument_data[w]->start > i)
            macro_stack[q].argument_data[w]->start += include_size;
        }
      }

      /* also macro starting points that are after this position need to move forward
         in memory... */
      ms = macros_first;
      while (ms != NULL) {
        if (ms->start > i)
          ms->start += include_size;
        ms = ms->next;
      }
    }

    if (got_once == YES) {
      /* turn the .INCLUDE/.INC into .INDLUDE/.IND to mark it as used, as we got ONCE,
         for repetitive macro calls that contain .INCLUDE/.INC... */
      buffer[character_c_position] = 'd';
    }
  }
  
  return SUCCEEDED;
}


int directive_incbin(void) {

  struct macro_static *m;
  int s, r, j, o;

  if (org_defined == 0 && output_format != OUTPUT_LIBRARY) {
    print_error("Before you can .INCBIN data you'll need to use ORG.\n", ERROR_LOG);
    return FAILED;
  }
  
  expect_calculations = NO;
  o = input_number();
  expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".INCBIN needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  /* convert the path string to local enviroment */
  localize_path(label);

  if (incbin_file(label, &ind, &inz, &s, &r, &m) == FAILED)
    return FAILED;
  
  if (m == NULL) {
    /* D [id] [swap] [skip] [size] */
    fprintf(file_out_ptr, "D%d %d %d %d ", ind, inz, s, r);
  }
  else {
    /* we want to filter the data */
    struct incbin_file_data *ifd;
    struct macro_incbin *min;

    min = calloc(sizeof(struct macro_incbin), 1);
    if (min == NULL) {
      print_error("Out of memory error while starting to filter the .INCBINed data.\n", ERROR_NONE);
      return FAILED;
    }

    ifd = incbin_file_data_first;
    for (j = 0; j != ind; j++)
      ifd = ifd->next;

    min->data = (unsigned char *)ifd->data;
    min->swap = inz;
    min->position = s;
    min->left = r;

    if (macro_start_incbin(m, min, YES) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_struct(void) {

  if (dstruct_status == ON) {
    print_error("You can't use .STRUCT inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  active_struct = calloc(sizeof(struct structure), 1);
  if (active_struct == NULL) {
    print_error("Out of memory while allocating a new STRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  strcpy(active_struct->name, tmp);

  active_struct->items = NULL;
  active_struct->last_item = NULL;
  union_stack = NULL;

  enum_offset = 0;
  last_enum_offset = 0;
  max_enum_offset = 0;
  base_enum_offset = 0;
  enum_ord = 1;
  enum_exp = 0;
  in_struct = YES;

  return SUCCEEDED;
}


int directive_ramsection(void) {

  int q;
      
  if (section_status == ON) {
    snprintf(emsg, sizeof(emsg), "There is already an open section called \"%s\".\n", sections_last->name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
  if (dstruct_status == ON) {
    print_error("You can't use .RAMSECTION inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  sec_tmp = calloc(sizeof(struct section_def), 1);
  if (sec_tmp == NULL) {
    snprintf(emsg, sizeof(emsg), "Out of memory while allocating room for a new RAMSECTION \"%s\".\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  sec_tmp->priority = 0;
  sec_tmp->listfile_items = 0;
  sec_tmp->listfile_ints = NULL;
  sec_tmp->listfile_cmds = NULL;
  sec_tmp->maxsize_status = OFF;
  sec_tmp->status = SECTION_STATUS_RAM_FREE;
  sec_tmp->alive = ON;
  sec_tmp->keep = NO;
  sec_tmp->data = NULL;
  sec_tmp->filename_id = active_file_info_last->filename_id;
  sec_tmp->id = section_id;
  sec_tmp->alignment = 1;
  sec_tmp->offset = 0;
  sec_tmp->advance_org = YES;
  sec_tmp->nspace = NULL;
  section_id++;

  /* add the namespace to the ramsection's name? */
  if (active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(tmp, sizeof(tmp), "RAMSECTION") == FAILED) {
      free(sec_tmp);
      return FAILED;
    }
  }

  strcpy(sec_tmp->name, tmp);
  sec_tmp->next = NULL;

  /* look for duplicate sections */
  sec_next = sections_first;
  while (sec_next != NULL) {
    if (strcmp(sec_next->name, tmp) == 0) {
      snprintf(emsg, sizeof(emsg), "RAMSECTION \"%s\" was defined for the second time.\n", tmp);
      print_error(emsg, ERROR_DIR);
      free(sec_tmp);
      return FAILED;
    }
    sec_next = sec_next->next;
  }

  sec_tmp->label_map = hashmap_new();

  if (sections_first == NULL) {
    sections_first = sec_tmp;
    sections_last = sec_tmp;
  }
  else {
    sections_last->next = sec_tmp;
    sections_last = sec_tmp;
  }

  /* check for optional BANK */
  if (compare_next_token("BANK") != SUCCEEDED)
    sec_tmp->bank = 0;
  else {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take BANK when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    skip_next_token();

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < 0) {
      print_error("BANK number must be zero or positive.\n", ERROR_DIR);
      return FAILED;
    }

    if (d > 255 && output_format != OUTPUT_LIBRARY) {
      snprintf(emsg, sizeof(emsg), "RAM banks == 256 (0-255), selected bank %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    sec_tmp->bank = d;
  }

  if (compare_next_token("SLOT") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take SLOT when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    skip_next_token();

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
      /* turn the label into a number */
      if (_get_slot_number_by_its_name(label, &d) == FAILED)
        return FAILED;
      q = SUCCEEDED;
    }
    else if (q == SUCCEEDED) {
      /* is the number a direct SLOT number, or is it an address? */
      q = _get_slot_number_by_a_value(d, &d);
    }
    if (q != SUCCEEDED) {
      print_error("Cannot find the SLOT.\n", ERROR_DIR);
      return FAILED;
    }

    if (slots[d].size == 0) {
      snprintf(emsg, sizeof(emsg), "There is no SLOT number %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    sec_tmp->slot = d;
  }

  if (compare_next_token("ORGA") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take ORGA when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    skip_next_token();

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("Cannot get the ORGA.\n", ERROR_DIR);
      return FAILED;
    }

    ind = slots[sec_tmp->slot].address;
    if (d < ind || d >= (ind + slots[sec_tmp->slot].size)) {
      print_error("ORGA is outside the current SLOT.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->address = d - ind;
  }
  else if (compare_next_token("ORG") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take ORG when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    skip_next_token();

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("Cannot get the ORG.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->address = d;
  }
  else
    sec_tmp->address = -1;
  
  fprintf(file_out_ptr, "S%d ", sec_tmp->id);

  /* align the ramsection? */
  if (compare_next_token("ALIGN") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take ALIGN when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .RAMSECTION alignment.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->alignment = d;
  }

  /* offset the ramsection? */
  if (compare_next_token("OFFSET") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error(".RAMSECTION cannot take OFFSET when inside a library.\n", ERROR_DIR);
      return FAILED;
    }

    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .RAMSECTION offset.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->offset = d;
  }  

  /* the type of the section */
  if (compare_next_token("FORCE") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take FORCE sections.\n", ERROR_DIR);
      return FAILED;
    }
    sec_tmp->status = SECTION_STATUS_RAM_FORCE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("FREE") == SUCCEEDED) {
    sec_tmp->status = SECTION_STATUS_RAM_FREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("SEMIFREE") == SUCCEEDED) {
    sec_tmp->status = SECTION_STATUS_RAM_SEMIFREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("SEMISUBFREE") == SUCCEEDED) {
    sec_tmp->status = SECTION_STATUS_RAM_SEMISUBFREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  
  /* return the org after the section? */
  if (compare_next_token("RETURNORG") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    sec_tmp->advance_org = NO;
  }

  if (compare_next_token("APPENDTO") == SUCCEEDED) {
    struct append_section *append_tmp;
    
    if (skip_next_token() == FAILED)
      return FAILED;
    
    append_tmp = calloc(sizeof(struct append_section), 1);
    if (append_tmp == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while allocating room for a new APPENDTO \"%s\".\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
      
    /* get the target section name */
    if (get_next_token() == FAILED) {
      free(append_tmp);
      return FAILED;
    }

    /* add the namespace to the section's name? */
    if (strlen(tmp) > 2 && tmp[0] == '*' && tmp[1] == ':') {
      char buf[MAX_NAME_LENGTH + 1];
      
      /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
      if (strlen(tmp) >= sizeof(buf)) {
        snprintf(emsg, sizeof(emsg), "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", tmp);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      strcpy(buf, &tmp[2]);
      strcpy(tmp, buf);
    }
    else if (active_file_info_last->namespace[0] != 0) {
      if (add_namespace_to_string(tmp, sizeof(tmp), "APPENDTO") == FAILED)
        return FAILED;
    }
    
    strcpy(append_tmp->section, sec_tmp->name);
    strcpy(append_tmp->append_to, tmp);

    append_tmp->next = append_sections;
    append_sections = append_tmp;
  }

  if (compare_next_token("PRIORITY") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .RAMSECTION priority.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->priority = d;
  }

  if (compare_next_token("KEEP") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    sec_tmp->keep = YES;
  }

  enum_offset = 0;
  last_enum_offset = 0;
  max_enum_offset = 0;
  base_enum_offset = 0;
  enum_ord = 1;

  /* setup active_struct (ramsection vars stored here temporarily) */
  active_struct = calloc(sizeof(struct structure), 1);
  if (active_struct == NULL) {
    print_error("Out of memory while parsing .RAMSECTION.\n", ERROR_DIR);
    return FAILED;
  }
  active_struct->name[0] = '\0';
  active_struct->items = NULL;
  active_struct->last_item = NULL;
  union_stack = NULL;
  
  in_ramsection = YES;

  /* generate a section start label? */
  if (extra_definitions == ON)
    generate_label("SECTIONSTART_", sec_tmp->name);

  return SUCCEEDED;
}


int directive_section(void) {
  
  int l;

  if (dstruct_status == ON) {
    print_error("You can't set the section inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  else if (section_status == ON) {
    snprintf(emsg, sizeof(emsg), "There is already an open section called \"%s\".\n", sections_last->name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
  else if (output_format != OUTPUT_LIBRARY && bank_defined == 0) {
    print_error(".SECTION requires a predefined bank.\n", ERROR_DIR);
    return FAILED;
  }
  else if (output_format != OUTPUT_LIBRARY && org_defined == 0) {
    print_error(".SECTION requires a starting address for positioning.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  /* every library section starts @ the beginning of the bank */
  if (output_format == OUTPUT_LIBRARY)
    org_defined = 1;

  sec_tmp = calloc(sizeof(struct section_def), 1);
  if (sec_tmp == NULL) {
    snprintf(emsg, sizeof(emsg), "Out of memory while allocating room for a new SECTION \"%s\".\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
  
  sec_tmp->priority = 0;
  sec_tmp->listfile_items = 0;
  sec_tmp->listfile_ints = NULL;
  sec_tmp->listfile_cmds = NULL;
  sec_tmp->maxsize_status = OFF;
  sec_tmp->data = NULL;
  sec_tmp->alignment = 1;
  sec_tmp->offset = 0;
  sec_tmp->advance_org = YES;
  sec_tmp->nspace = NULL;
  sec_tmp->keep = NO;

  if (strcmp(tmp, "BANKHEADER") == 0) {
    no_library_files("bank header sections");
      
    sec_next = sections_first;
    while (sec_next != NULL) {
      if (strcmp(sec_next->name, tmp) == 0 && sec_next->bank == bank) {
        snprintf(emsg, sizeof(emsg), "BANKHEADER section was defined for the second time for bank %d.\n", bank);
        print_error(emsg, ERROR_DIR);
        free(sec_tmp);
        return FAILED;
      }
      sec_next = sec_next->next;
    }
  }
  else {
    sec_next = sections_first;
    while (sec_next != NULL) {
      if (strcmp(sec_next->name, tmp) == 0) {
        snprintf(emsg, sizeof(emsg), "SECTION \"%s\" was defined for the second time.\n", tmp);
        print_error(emsg, ERROR_DIR);
        free(sec_tmp);
        return FAILED;
      }
      sec_next = sec_next->next;
    }
  }

  strcpy(sec_tmp->name, tmp);
  sec_tmp->next = NULL;

  sec_tmp->label_map = hashmap_new();

  if (sections_first == NULL) {
    sections_first = sec_tmp;
    sections_last = sec_tmp;
  }
  else {
    sections_last->next = sec_tmp;
    sections_last = sec_tmp;
  }

  if (compare_next_token("NAMESPACE") == SUCCEEDED) {
    struct namespace_def *nspace;

    if (skip_next_token() == FAILED)
      return FAILED;

    /* get the name */
    if (input_next_string() == FAILED)
      return FAILED;
    if (tmp[0] == '\"' && tmp[strlen(tmp)-1] == '\"') {
      l = 0;
      while (tmp[l+1] != '\"') {
        tmp[l] = tmp[l+1];
        l++;
      }
      tmp[l] = 0;
    }

    hashmap_get(namespace_map, tmp, (void*)&nspace);
    if (nspace == NULL) {
      nspace = calloc(1, sizeof(struct namespace_def));
      if (nspace == NULL) {
        print_error("Out of memory error.\n", ERROR_DIR);
        return FAILED;
      }
      strcpy(nspace->name, tmp);
      if (hashmap_put(namespace_map, nspace->name, nspace) != MAP_OK) {
        print_error("Namespace hashmap error.\n", ERROR_DIR);
        return FAILED;
      }
    }

    nspace->label_map = hashmap_new();
    sec_tmp->nspace = nspace;
  }

  /* add the namespace to the section's name? */
  if (active_file_info_last->namespace[0] != 0 && sec_tmp->nspace == NULL) {
    if (add_namespace_to_string(sec_tmp->name, sizeof(sec_tmp->name), "SECTION") == FAILED)
      return FAILED;
  }
  
  /* the size of the section? */
  if (compare_next_token("SIZE") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the SIZE.\n", ERROR_DIR);
      return FAILED;
    }

    if (sec_tmp->maxsize_status == ON && sec_tmp->maxsize != d) {
      print_error("The SIZE of the section has already been defined.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->maxsize_status = ON;
    sec_tmp->maxsize = d;
  }

  /* align the section? */
  if (compare_next_token("ALIGN") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .SECTION alignment.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->alignment = d;
  }

  /* offset the section? */
  if (compare_next_token("OFFSET") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .SECTION offset.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->offset = d;
  }  

  /* the type of the section */
  if (compare_next_token("FORCE") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take FORCE sections.\n", ERROR_DIR);
      return FAILED;
    }
    sec_tmp->status = SECTION_STATUS_FORCE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("FREE") == SUCCEEDED) {
    sec_tmp->status = SECTION_STATUS_FREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("SUPERFREE") == SUCCEEDED) {
    sec_tmp->status = SECTION_STATUS_SUPERFREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("SEMIFREE") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take SEMIFREE sections.\n", ERROR_DIR);
      return FAILED;
    }
    sec_tmp->status = SECTION_STATUS_SEMIFREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("SEMISUBFREE") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take SEMISUBFREE sections.\n", ERROR_DIR);
      return FAILED;
    }
    sec_tmp->status = SECTION_STATUS_SEMISUBFREE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else if (compare_next_token("OVERWRITE") == SUCCEEDED) {
    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take OVERWRITE sections.\n", ERROR_DIR);
      return FAILED;
    }
    sec_tmp->status = SECTION_STATUS_OVERWRITE;
    if (skip_next_token() == FAILED)
      return FAILED;
  }
  else
    sec_tmp->status = SECTION_STATUS_FREE;

  /* return the org after the section? */
  if (compare_next_token("RETURNORG") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    sec_tmp->advance_org = NO;
  }

  if (compare_next_token("APPENDTO") == SUCCEEDED) {
    struct append_section *append_tmp;

    if (skip_next_token() == FAILED)
      return FAILED;

    append_tmp = calloc(sizeof(struct append_section), 1);
    if (append_tmp == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while allocating room for a new APPENDTO \"%s\".\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
      
    /* get the target section name */
    if (get_next_token() == FAILED) {
      free(append_tmp);
      return FAILED;
    }

    /* add the namespace to the section's name? */
    if (strlen(tmp) > 2 && tmp[0] == '*' && tmp[1] == ':') {
      char buf[MAX_NAME_LENGTH + 1];
      
      /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
      if (strlen(tmp) >= sizeof(buf)) {
        snprintf(emsg, sizeof(emsg), "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", tmp);
        print_error(emsg, ERROR_DIR);
        free(append_tmp);
        return FAILED;
      }

      strcpy(buf, &tmp[2]);
      strcpy(tmp, buf);
    }
    else if (active_file_info_last->namespace[0] != 0) {
      if (add_namespace_to_string(tmp, sizeof(tmp), "APPENDTO") == FAILED) {
        free(append_tmp);
        return FAILED;
      }
    }
    
    strcpy(append_tmp->section, sec_tmp->name);
    strcpy(append_tmp->append_to, tmp);

    append_tmp->next = append_sections;
    append_sections = append_tmp;
  }

  if (compare_next_token("PRIORITY") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error("Could not parse the .SECTION priority.\n", ERROR_DIR);
      return FAILED;
    }

    sec_tmp->priority = d;
  }

  if (compare_next_token("KEEP") == SUCCEEDED) {
    if (skip_next_token() == FAILED)
      return FAILED;

    sec_tmp->keep = YES;
  }
  
  /* bankheader section? */
  if (strcmp(sec_tmp->name, "BANKHEADER") == 0) {
    sec_tmp->status = SECTION_STATUS_HEADER;
    bankheader_status = ON;
  }

  sec_tmp->id = section_id;
  sec_tmp->alive = ON;
  sec_tmp->filename_id = active_file_info_last->filename_id;
  sec_tmp->bank = bank;
  section_id++;
  section_status = ON;
  fprintf(file_out_ptr, "S%d ", sec_tmp->id);

  /* generate a section start label? */
  if (extra_definitions == ON)
    generate_label("SECTIONSTART_", sec_tmp->name);
  
  return SUCCEEDED;
}


int directive_fopen(void) {
  
  struct filepointer *f;
  char *c;
  int o;

  expect_calculations = NO;
  o = input_number();
  expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".FOPEN needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  /* convert the path to local enviroment */
  localize_path(label);

  c = calloc(strlen(label) + 1, 1);
  if (c == NULL) {
    print_error("Out of memory error.\n", ERROR_DIR);
    return FAILED;
  }
  strcpy(c, label);

  /* get the file pointer name */
  if (get_next_token() == FAILED) {
    free(c);
    return FAILED;
  }

  /* is it defined already? */
  f = filepointers;
  while (f != NULL) {
    if (strcmp(tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f != NULL) {
    /* it exists already! close the old handle and open the new one. */
    if (f->f != NULL) {
      fclose(f->f);
      f->f = NULL;
    }
    if (f->filename != NULL) {
      free(f->filename);
      f->filename = NULL;
    }
  }
  else {
    /* allocate a new filepointer */
    f = calloc(sizeof(struct filepointer), 1);
    if (f == NULL) {
      print_error("Out of memory error.\n", ERROR_DIR);
      free(c);
      return FAILED;
    }

    f->f = NULL;
    f->filename = NULL;
    f->next = filepointers;
    filepointers = f;
  }

  f->filename = c;
  strcpy(f->name, tmp);

  /* open the file */
  f->f = fopen(f->filename, "rb");
  if (f->f == NULL) {
    snprintf(emsg, sizeof(emsg), "Error opening file \"%s\" for reading.\n", f->filename);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_fclose(void) {
  
  struct filepointer *f, **t;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  f = filepointers;
  t = &filepointers;
  while (f != NULL) {
    if (strcmp(tmp, f->name) == 0)
      break;
    t = &(f->next);
    f = f->next;
  }

  if (f == NULL) {
    snprintf(emsg, sizeof(emsg), "Couldn't find filepointer \"%s\".\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* close the file pointer */
  if (f->f != NULL) {
    fclose(f->f);
    f->f = NULL;
  }
  *t = f->next;

  free(f->filename);
  free(f);

  return SUCCEEDED;
}


int directive_fsize(void) {
  
  struct filepointer *f;
  long l, b;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  /* fetch the file pointer */
  f = filepointers;
  while (f != NULL) {
    if (strcmp(tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(emsg, sizeof(emsg), "Couldn't find filepointer \"%s\".\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  l = ftell(f->f);
  fseek(f->f, 0, SEEK_END);
  b = ftell(f->f);
  fseek(f->f, l, SEEK_SET);

  /* get the definition label */
  if (get_next_token() == FAILED)
    return FAILED;

  if (add_a_new_definition(tmp, (double)b, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_fread(void) {
  
  struct filepointer *f;
  unsigned char c;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  f = filepointers;
  while (f != NULL) {
    if (strcmp(tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(emsg, sizeof(emsg), "Couldn't find filepointer \"%s\".\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  fscanf(f->f, "%c", &c);

  /* get the definition label */
  if (get_next_token() == FAILED)
    return FAILED;

  if (redefine(tmp, (double)c, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_block(void) {

  struct block_name *b;
  
  if ((ind = get_next_token()) == FAILED)
    return FAILED;

  if (ind != GET_NEXT_TOKEN_STRING) {
    print_error(".BLOCK requires a name string.\n", ERROR_DIR);
    return FAILED;
  }

  b = calloc(sizeof(struct block_name), 1);
  if (b == NULL) {
    print_error("Out of memory while allocating block name.\n", ERROR_DIR);
    return FAILED;
  }

  strcpy(b->name, tmp);
  b->id = block_name_id++;
  b->next = block_names;
  block_names = b;
  
  block_status++;

  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
  fprintf(file_out_ptr, "g%d ", b->id);

  return SUCCEEDED;
}


int directive_shift(void) {

  struct macro_argument *ma;
  struct macro_runtime *rt;
  struct macro_static *st;
  int q, o;

  if (macro_active == 0) {
    print_error(".SHIFT can only be used inside a MACRO.\n", ERROR_DIR);
    return FAILED;
  }

  rt = &macro_stack[macro_active - 1];
  st = rt->macro;

  if (st->nargument_names <= rt->supplied_arguments)
    o = st->nargument_names;
  else
    o = rt->supplied_arguments;

  /* free the argument definitions */
  for (q = 0; q < o; q++)
    undefine(st->argument_names[q]);

  /* free the first argument data */
  free(rt->argument_data[0]);

  /* shift the arguments one down */
  for (q = 0; q < rt->supplied_arguments - 1; q++)
    rt->argument_data[q] = rt->argument_data[q + 1];

  /* remove the last one */
  rt->argument_data[q] = NULL;
  rt->supplied_arguments--;

  if (st->nargument_names <= rt->supplied_arguments)
    o = st->nargument_names;
  else
    o = rt->supplied_arguments;

  /* redo the definitions if any */
  for (q = 0; q < o; q++) {
    ma = rt->argument_data[q];
    if (ma->type == SUCCEEDED)
      redefine(st->argument_names[q], ma->value, NULL, DEFINITION_TYPE_VALUE, 0);
    else if (ma->type == INPUT_NUMBER_STACK)
      redefine(st->argument_names[q], ma->value, NULL, DEFINITION_TYPE_STACK, 0);
    else if (ma->type == INPUT_NUMBER_ADDRESS_LABEL)
      redefine(st->argument_names[q], 0.0, ma->string, DEFINITION_TYPE_ADDRESS_LABEL, (int)strlen(ma->string));
    else if (ma->type == INPUT_NUMBER_STRING)
      redefine(st->argument_names[q], 0.0, ma->string, DEFINITION_TYPE_STRING, (int)strlen(ma->string));
  }

  /* redefine NARGS */
  if (redefine("NARGS", (double)rt->supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;
  if (redefine("nargs", (double)rt->supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


#ifdef GB

int directive_name_gb(void) {
      
  no_library_files(".NAME");
  
  if ((ind = get_next_token()) == FAILED)
    return FAILED;

  if (ind != GET_NEXT_TOKEN_STRING) {
    print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
    return FAILED;
  }

  /* no name has been defined so far */
  if (name_defined == 0) {
    for (ind = 0; tmp[ind] != 0 && ind < 16; ind++)
      name[ind] = tmp[ind];

    if (ind == 16 && tmp[ind] != 0) {
      print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
      return FAILED;
    }

    for ( ; ind < 16; name[ind] = 0, ind++)
      ;

    name_defined = 1;
  }
  /* compare the names */
  else {
    for (ind = 0; tmp[ind] != 0 && name[ind] != 0 && ind < 16; ind++)
      if (name[ind] != tmp[ind])
        break;

    if (ind == 16 && tmp[ind] != 0) {
      print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (ind != 16 && (name[ind] != 0 || tmp[ind] != 0)) {
      print_error(".NAME was already defined.\n", ERROR_DIR);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

#endif


int directive_rombanks(void) {

  int q;

  no_library_files(".ROMBANKS");
    
  if (banksize_defined == 0) {
    print_error("No .ROMBANKSIZE defined.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || d < 1) {
    print_error(".ROMBANKS needs a positive integer value.\n", ERROR_DIR);
    return FAILED;
  }

#ifdef GB
  if (d != 2 && d != 4 && d != 8 && d != 16 && d != 32 && d != 64 &&
      d != 128 && d != 256 && d != 512 && d != 72 && d != 80 && d != 96) {
    print_error("Unsupported amount of ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  if (d == 2)
    romtype = 0;
  else if (d == 4)
    romtype = 1;
  else if (d == 8)
    romtype = 2;
  else if (d == 16)
    romtype = 3;
  else if (d == 32)
    romtype = 4;
  else if (d == 64)
    romtype = 5;
  else if (d == 128)
    romtype = 6;
  else if (d == 256)
    romtype = 7;
  else if (d == 512)
    romtype = 8;
  else if (d == 72)
    romtype = 0x52;
  else if (d == 80)
    romtype = 0x53;
  else if (d == 96)
    romtype = 0x54;
#endif

  /* check that the old bank map (smaller) and the new one equal as much as they can */
  if (rombanks_defined != 0) {
    if (rombanks < d)
      inz = rombanks;
    else
      inz = d;

    for (ind = 0; ind < inz; ind++) {
      if (banks[ind] != banksize) {
        print_error("The old and the new .ROMBANKMAP's don't match.\n", ERROR_DIR);
        return FAILED;
      }
    }

    if (d <= rombanks)
      return SUCCEEDED;

    snprintf(emsg, sizeof(emsg), "Upgrading from %d to %d ROM banks.\n", rombanks, d);
    print_error(emsg, ERROR_WRN);
  }

  rombanks = d;
  rombanks_defined = 1;
  max_address = d * banksize;

  if (rom_banks != NULL)
    free(rom_banks);
  if (rom_banks_usage_table != NULL)
    free(rom_banks_usage_table);

  rom_banks = calloc(sizeof(unsigned char) * max_address, 1);
  rom_banks_usage_table = calloc(sizeof(unsigned char) * max_address, 1);
  if (rom_banks == NULL || rom_banks_usage_table == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  memset(rom_banks_usage_table, 0, sizeof(unsigned char) * max_address);

  if (banks != NULL)
    free(banks);
  if (bankaddress != NULL)
    free(bankaddress);

  banks = calloc(sizeof(int) * rombanks, 1);
  bankaddress = calloc(sizeof(int) * rombanks, 1);
  if (banks == NULL || bankaddress == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  for (inz = 0, ind = 0; ind < d; ind++) {
    banks[ind] = banksize;
    bankaddress[ind] = inz;
    inz += banksize;
  }

  return SUCCEEDED;
}


int directive_rombankmap(void) {
  
  int b = 0, a = 0, bt = 0, bt_defined = 0, x, bs = 0, bs_defined = 0, o, q;

  no_library_files(".ROMBANKMAP");

  /* ROMBANKMAP has been defined previously */
  if (rombankmap_defined != 0 || rombanks_defined != 0) {
    o = 0;
    while ((ind = get_next_token()) == SUCCEEDED) {
      /* .IF directive? */
      if (tmp[0] == '.') {
        q = parse_if_directive();
        if (q == FAILED)
          return FAILED;
        else if (q == SUCCEEDED)
          continue;
        /* else q == -1, continue executing below */
      }

      if (strcaselesscmp(tmp, ".ENDRO") == 0) {
        break;
      }
      else if (strcaselesscmp(tmp, "BANKSTOTAL") == 0) {
        q = input_number();
    
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKSTOTAL needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        if (rombanks < d) {
          banks = realloc(banks, sizeof(int) * d);
          bankaddress = realloc(bankaddress, sizeof(int) * d);
          if (banks == NULL || bankaddress == NULL) {
            print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
            return FAILED;
          }
        }

        bt = d;
        bt_defined = 1;
      }
      else if (strcaselesscmp(tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error("BANKSTOTAL needs to be defined prior to BANKSIZE.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();
    
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKSIZE needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }
    
        bs = d;
        bs_defined = 1;
      }
      else if (strcaselesscmp(tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error("BANKSIZE needs to be defined prior to BANKS.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKS needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        for (x = 0; x < d; x++) {
          if (b > bt) {
            print_error("More BANKS than BANKSTOTAL suggests.\n", ERROR_DIR);
            return FAILED;
          }

          /* new banks? */
          if (x >= rombanks) {
            banks[o] = bs;
            bankaddress[o] = a;
          }
          /* compare old banks */
          else if (banks[o] != bs) {
            print_error("The old and the new ROMBANKMAPs don't match.\n", ERROR_DIR);
            return FAILED;
          }

          o++;
          b++;
          a += bs;
        }
      }
      else {
        ind = FAILED;
        break;
      }
    }
  }
  /* no ROMBANKMAP has been defined */
  else {
    o = 0;
    while ((ind = get_next_token()) == SUCCEEDED) {
      /* .IF directive? */
      if (tmp[0] == '.') {
        q = parse_if_directive();
        if (q == FAILED)
          return FAILED;
        else if (q == SUCCEEDED)
          continue;
        /* else q == -1, continue executing below */
      }
      
      if (strcaselesscmp(tmp, ".ENDRO") == 0)
        break;
      else if (strcaselesscmp(tmp, "BANKSTOTAL") == 0) {
        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKSTOTAL needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        banks = calloc(sizeof(int) * d, 1);
        bankaddress = calloc(sizeof(int) * d, 1);
        if (banks == NULL || bankaddress == NULL) {
          print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
          return FAILED;
        }

        bt = d;
        bt_defined = 1;
      }
      else if (strcaselesscmp(tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error("BANKSTOTAL needs to be defined prior to BANKSIZE.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKSIZE needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        bs = d;
        bs_defined = 1;
      }
      else if (strcaselesscmp(tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error("BANKSIZE needs to be defined prior to BANKS.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d <= 0) {
          print_error("BANKS needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        for (x = 0; x < d; x++) {
          if (b >= bt) {
            print_error("More BANKS than BANKSTOTAL suggests.\n", ERROR_DIR);
            return FAILED;
          }
          banks[o] = bs;
          bankaddress[o] = a;
          o++;
          b++;
          a += bs;
        }
      }
      else {
        ind = FAILED;
        break;
      }
    }
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .ROMBANKMAP data structure.\n", ERROR_DIR);
    return FAILED;
  }

  /* no banks definded? */
  if (bt == 0) {
    print_error("No ROM banks were defined inside the .ROMBANKMAP.\n", ERROR_DIR);
    return FAILED;      
  }
  if (bt != b) {
    print_error("Not all ROM banks were defined inside the .ROMBANKMAP.\n", ERROR_DIR);
    return FAILED;      
  }

#ifdef GB
  if (b != 2 && b != 4 && b != 8 && b != 16 && b != 32 && b != 64 &&
      b != 128 && b != 256 && b != 512 && b != 72 && b != 80 && b != 96) {
    print_error("Unsupported amount of ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  if (b == 2)
    romtype = 0;
  else if (b == 4)
    romtype = 1;
  else if (b == 8)
    romtype = 2;
  else if (b == 16)
    romtype = 3;
  else if (b == 32)
    romtype = 4;
  else if (b == 64)
    romtype = 5;
  else if (b == 128)
    romtype = 6;
  else if (b == 256)
    romtype = 7;
  else if (b == 512)
    romtype = 8;
  else if (b == 72)
    romtype = 0x52;
  else if (b == 80)
    romtype = 0x53;
  else if (b == 96)
    romtype = 0x54;
#endif

  if (rombanks_defined != 0) {
    if (b > rombanks) {
      snprintf(emsg, sizeof(emsg), "Upgrading from %d to %d ROM banks.\n", rombanks, b);
      print_error(emsg, ERROR_WRN);
    }
    else
      return SUCCEEDED;
  }

  rombanks = b;
  rombanks_defined = 1;
  for (max_address = 0, q = 0; q < rombanks; q++)
    max_address += banks[q];

  if (rom_banks != NULL)
    free(rom_banks);
  if (rom_banks_usage_table != NULL)
    free(rom_banks_usage_table);

  rom_banks = calloc(sizeof(unsigned char) * max_address, 1);
  rom_banks_usage_table = calloc(sizeof(unsigned char) * max_address, 1);
  if (rom_banks == NULL || rom_banks_usage_table == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  memset(rom_banks_usage_table, 0, sizeof(unsigned char) * max_address);
  rombankmap_defined = 1;

  return SUCCEEDED;
}


int directive_memorymap(void) {
  
  int slotsize = 0, slotsize_defined = 0, s = 0, q, o;

  if (memorymap_defined == 1) {
    print_error(".MEMORYMAP can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (output_format == OUTPUT_LIBRARY)
    print_error("Libraries don't need .MEMORYMAP.\n", ERROR_WRN);

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDME") == 0) {
      if (defaultslot_defined == 0) {
        print_error("No DEFAULTSLOT defined.\n", ERROR_DIR);
        return FAILED;
      }

      if (slots[defaultslot].size == 0) {
        snprintf(emsg, sizeof(emsg), "Unknown DEFAULTSLOT %d.\n", defaultslot);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      break;
    }
    else if (strcaselesscmp(tmp, "SLOTSIZE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("SLOTSIZE needs an immediate value.\n", ERROR_DIR);
        return FAILED;
      }

      slotsize = d;
      slotsize_defined = 1;
    }
    else if (strcaselesscmp(tmp, "DEFAULTSLOT") == 0) {
      if (defaultslot_defined != 0) {
        print_error("DEFAULTSLOT can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d > 255 || d < 0) {
        print_error("DEFAULTSLOT needs an immediate value (0-255) as an ID.\n", ERROR_DIR);
        return FAILED;
      }

      defaultslot_defined = 1;
      defaultslot = d;
    }
    else if (strcaselesscmp(tmp, "SLOT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d > 255 || d < 0) {
        print_error("SLOT needs a positive value (0-255) as an ID.\n", ERROR_DIR);
        return FAILED;
      }

      if (s != d) {
        print_error("Error in SLOT's ID. ID's start from 0.\n", ERROR_DIR);
        return FAILED;
      }

      o = d;

      /* skip "START" if present */
      if (compare_next_token("START") == SUCCEEDED)
        skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < 0) {
        print_error("The starting address has to be a non-negative value.\n", ERROR_DIR);
        return FAILED;
      }

      slots[o].address = d;

      /* skip "SIZE" if present */
      if (compare_next_token("SIZE") == SUCCEEDED)
        skip_next_token();

      q = input_number();

      if (q == INPUT_NUMBER_EOL) {
        if (slotsize_defined == 0) {
          print_error("SLOTSIZE must be defined if you don't explicitly give the size.\n", ERROR_DIR);
          return FAILED;
        }

        next_line();
    
        d = slotsize;
      }
      else {
        if (q == FAILED)
          return FAILED;
        if (q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STRING) {
          /* we got the name for the SLOT instead of its SIZE */
          strcpy(slots[o].name, label);
          d = slotsize;
        }
        else if (q != SUCCEEDED) {
          print_error("The size of the slot needs to be an immediate value.\n", ERROR_DIR);
          return FAILED;
        }
      }
      
      slots[o].size = d;

      if (q != INPUT_NUMBER_EOL) {
        /* skip "NAME" if present */
        if (compare_next_token("NAME") == SUCCEEDED)
          skip_next_token();

        q = input_number();

        if (q == INPUT_NUMBER_EOL)
          next_line();
        else {
          if (q != INPUT_NUMBER_ADDRESS_LABEL && q != INPUT_NUMBER_STRING) {
            print_error("NAME needs a label/string for name.\n", ERROR_DIR);
            return FAILED;      
          }

          strcpy(slots[o].name, label);
        }
      }
      
      slots_amount++;
      s++;
    }
    else {
      ind = FAILED;
      break;
    }
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .MEMORYMAP data structure.\n", ERROR_DIR);
    return FAILED;
  }

  memorymap_defined = 1;

  return SUCCEEDED;
}


int directive_unbackground(void) {
  
  int start, end, q;

  if (output_format != OUTPUT_OBJECT) {
    print_error(".UNBACKGROUND can only be used in OBJECT output mode.\n", ERROR_DIR);
    return FAILED;
  }
  if (background_defined == 0) {
    print_error("No .BACKGROUND specified.\n", ERROR_DIR);
    return FAILED;
  }

  /* get the starting address */
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || q < 0) {
    print_error(".UNBACKGROUND needs the block's starting address.\n", ERROR_DIR);
    return FAILED;
  }

  start = d;

  /* get the ending address */
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || q < 0) {
    print_error(".UNBACKGROUND needs the block's ending address.\n", ERROR_DIR);
    return FAILED;
  }

  end = d;

  if (end < start) {
    print_error("The block's ending address is smaller than the starting address!\n", ERROR_DIR);
    return FAILED;
  }
  if (start >= max_address) {
    snprintf(emsg, sizeof(emsg), "The block's starting address $%x is beyond the ROM's ending address $%x.\n", start, max_address-1);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }
  if (end >= max_address) {
    snprintf(emsg, sizeof(emsg), "The block's ending address $%x is beyond the ROM's ending address $%x. Using the ROM's ending address instead.\n", end, max_address-1);
    end = max_address;
    print_error(emsg, ERROR_WRN);
  }
  
  /* clear the memory [start, end] */
  memset(rom_banks + start, 0, end - start + 1);
  memset(rom_banks_usage_table + start, 0, end - start + 1);

  return SUCCEEDED;
}


int directive_background(void) {
  
  FILE *file_in_ptr;
  int q;

  if (output_format != OUTPUT_OBJECT) {
    print_error(".BACKGROUND can only be used in OBJECT output mode.\n", ERROR_DIR);
    return FAILED;
  }
  if (background_defined == 1) {
    print_error("Only one .BACKGROUND can be specified.\n", ERROR_DIR);
    return FAILED;
  }

  expect_calculations = NO;
  q = input_number();
  expect_calculations = YES;

  if (q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".BACKGROUND needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  if (memorymap_defined == 0) {
    print_error("No .MEMORYMAP defined.\n", ERROR_DIR);
    return FAILED;
  }
  if (rombanks_defined == 0) {
    print_error("No .ROMBANKS defined.\n", ERROR_DIR);
    return FAILED;
  }

  create_full_name(include_dir, label);

  if ((file_in_ptr = fopen(full_name, "rb")) == NULL) {
    snprintf(emsg, sizeof(emsg), "Error opening .BACKGROUND file \"%s\".\n", full_name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  fseek(file_in_ptr, 0, SEEK_END);
  background_size = (int)ftell(file_in_ptr);
  fseek(file_in_ptr, 0, SEEK_SET);
  
  if (background_size > max_address) {
    snprintf(emsg, sizeof(emsg), ".BACKGROUND file \"%s\" size (%d) is larger than ROM size (%d).\n", full_name, background_size, max_address);
    print_error(emsg, ERROR_DIR);
    fclose(file_in_ptr);
    return FAILED;
  }

  memset(rom_banks_usage_table, 2, background_size);
  fread(rom_banks, 1, max_address, file_in_ptr);

  background_defined = 1;
  fclose(file_in_ptr);

  return SUCCEEDED;
}


#ifdef GB

int directive_gbheader(void) {

  int q;
    
  if (gbheader_defined != 0) {
    print_error(".GBHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (computechecksum_defined != 0)
    print_error(".COMPUTEGBCHECKSUM is unnecessary when .GBHEADER is defined.\n", ERROR_WRN);
  else
    computechecksum_defined++;

  if (computecomplementcheck_defined != 0)
    print_error(".COMPUTEGBCOMPLEMENTCHECK is unnecessary when .GBHEADER is defined.\n", ERROR_WRN);
  else
    computecomplementcheck_defined++;

  if (output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .GBHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDGB") == 0)
      break;
    else if (strcaselesscmp(tmp, "NINTENDOLOGO") == 0)
      nintendologo_defined++;
    else if (strcaselesscmp(tmp, "ROMDMG") == 0) {
      if (romgbc == 1) {
        print_error(".ROMGBC was defined prior to .ROMDMG.\n", ERROR_DIR);
        return FAILED;
      }
      else if (romgbc == 2) {
        print_error(".ROMGBCONLY was defined prior to .ROMDMG.\n", ERROR_DIR);
        return FAILED;
      }
      else if (romsgb != 0) {
        print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
        return FAILED;
      }
      romdmg = 1;
    }
    else if (strcaselesscmp(tmp, "ROMGBC") == 0) {
      if (romdmg != 0) {
        print_error(".ROMDMG was defined prior to .ROMGBC.\n", ERROR_DIR);
        return FAILED;
      }
      else if (romgbc == 2) {
        print_error(".ROMGBCONLY was defined prior to .ROMGBC.\n", ERROR_DIR);
        return FAILED;
      }
      romgbc = 1;
    }
    else if (strcaselesscmp(tmp, "ROMGBCONLY") == 0) {
      if (romdmg != 0) {
        print_error(".ROMDMG was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
        return FAILED;
      }
      else if (romgbc == 1) {
        print_error(".ROMGBC was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
        return FAILED;
      }
      romgbc = 2;
    }
    else if (strcaselesscmp(tmp, "ROMSGB") == 0) {
      if (romdmg != 0) {
        print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
        return FAILED;
      }
      romsgb++;
    }
    else if (strcaselesscmp(tmp, "NAME") == 0) {
      if ((ind = get_next_token()) == FAILED)
        return FAILED;

      if (ind != GET_NEXT_TOKEN_STRING) {
        print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no name has been defined so far */
      if (name_defined == 0) {
        for (ind = 0; tmp[ind] != 0 && ind < 16; ind++)
          name[ind] = tmp[ind];
    
        if (ind == 16 && tmp[ind] != 0) {
          print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
          return FAILED;
        }

        for ( ; ind < 16; name[ind] = 0, ind++)
          ;

        name_defined = 1;
      }
      else {
        /* compare the names */
        for (ind = 0; tmp[ind] != 0 && name[ind] != 0 && ind < 16; ind++)
          if (name[ind] != tmp[ind])
            break;
    
        if (ind == 16 && tmp[ind] != 0) {
          print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (ind != 16 && (name[ind] != 0 || tmp[ind] != 0)) {
          print_error("NAME was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }
    else if (strcaselesscmp(tmp, "LICENSEECODEOLD") == 0) {
      if (licenseecodenew_defined != 0) {
        print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < -128 || d > 255) {
        snprintf(emsg, sizeof(emsg), ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (licenseecodeold_defined != 0) {
        if (licenseecodeold != d) {
          print_error(".LICENSEECODEOLD was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      licenseecodeold = d;
      licenseecodeold_defined = 1;
    }
    else if (strcaselesscmp(tmp, "LICENSEECODENEW") == 0) {
      if (licenseecodeold_defined != 0) {
        print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
        return FAILED;
      }

      if ((ind = get_next_token()) == FAILED)
        return FAILED;

      if (ind != GET_NEXT_TOKEN_STRING) {
        print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
        return FAILED;
      }
      if (!(tmp[0] != 0 && tmp[1] != 0 && tmp[2] == 0)) {
        print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
        return FAILED;
      }

      if (licenseecodenew_defined != 0) {
        if (tmp[0] != licenseecodenew_c1 || tmp[1] != licenseecodenew_c2) {
          print_error(".LICENSEECODENEW was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      licenseecodenew_c1 = tmp[0];
      licenseecodenew_c2 = tmp[1];
      licenseecodenew_defined = 1;
    }
    else if (strcaselesscmp(tmp, "CARTRIDGETYPE") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "CARTRIDGETYPE needs a 8-bit value, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (cartridgetype_defined != 0 && cartridgetype != d) {
          print_error("CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        cartridgetype = d;
        cartridgetype_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "RAMSIZE") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "RAMSIZE needs a 8-bit value, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (rambanks_defined != 0 && rambanks != d) {
          print_error("RAMSIZE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        rambanks = d;
        rambanks_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "COUNTRYCODE") == 0) {
      inz = input_number();
      
      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "COUNTRYCODE needs a non-negative value, got %d.\n\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (countrycode_defined != 0 && countrycode != d) {
          print_error("COUNTRYCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        countrycode = d;
        countrycode_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "DESTINATIONCODE") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "DESTINATIONCODE needs a non-negative value, got %d.\n\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (countrycode_defined != 0 && countrycode != d) {
          print_error("DESTINATIONCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        countrycode = d;
        countrycode_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "VERSION") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "VERSION needs a non-negative value, got %d.\n\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (version_defined != 0 && version != d) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        version = d;
        version_defined = 1;
      }
      else
        return FAILED;
    }
    else {
      ind = FAILED;
      break;
    }
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .GBHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  gbheader_defined = 1;

  return SUCCEEDED;
}

#endif


int directive_define_def_equ(void) {
  
  int j, size, export, q;
  double dou;
  char k[256];

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(tmp) == YES) {
    snprintf(emsg, sizeof(emsg), "\"%s\" is a reserved definition label and is not user definable.\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  input_float_mode = ON;
  q = get_new_definition_data(&j, k, &size, &dou, &export);
  input_float_mode = OFF;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_EOL || q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error("Could not parse the definition data.\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    q = add_a_new_definition(tmp, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    q = add_a_new_definition(tmp, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    q = add_a_new_definition(tmp, 0.0, k, DEFINITION_TYPE_STRING, size);
  else if (q == INPUT_NUMBER_STACK)
    q = add_a_new_definition(tmp, (double)j, NULL, DEFINITION_TYPE_STACK, 0);
  else if (q == INPUT_NUMBER_EOL)
    q = add_a_new_definition(tmp, 0.0, NULL, DEFINITION_TYPE_VALUE, 0);
  
  if (q == FAILED)
    return FAILED;

  if (export == YES) {
    if (export_a_definition(tmp) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_undef_undefine(void) {

  char bak[256];
  int q;

  strcpy(bak, cp);

  q = 0;
  while (1) {
    ind = input_next_string();
    if (ind == FAILED)
      return FAILED;
    if (ind == INPUT_NUMBER_EOL) {
      if (q != 0) {
        next_line();
        return SUCCEEDED;
      }
      snprintf(emsg, sizeof(emsg), ".%s requires definition name(s).\n", bak);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    q++;

    if (undefine(tmp) == FAILED) {
      snprintf(emsg, sizeof(emsg), "Could not .%s \"%s\".\n", bak, tmp);
      print_error(emsg, ERROR_WRN);
    }
  }

  return SUCCEEDED;
}


int directive_enumid(void) {
  
  int q;
  
  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q == INPUT_NUMBER_EOL) {
    print_error(".ENUMID needs a value or a definition name.\n", ERROR_DIR);
    return FAILED;
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    if (enumid_defined == 0) {
      print_error(".ENUMID needs the initial value when .ENUMID is used the first time.\n", ERROR_DIR);
      return FAILED;
    }
    
    if (is_reserved_definition(label) == YES) {
      snprintf(emsg, sizeof(emsg), "\"%s\" is a reserved definition label and is not user definable.\n", label);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (add_a_new_definition(label, (double)enumid, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    if (enumid_export == 1) {
      if (export_a_definition(label) == FAILED)
        return FAILED;
    }

    enumid += enumid_adder;

    return SUCCEEDED;
  }
  else if (q == SUCCEEDED) {
    enumid = d;
    enumid_adder = 1;
    enumid_export = 0;

    if (compare_next_token("STEP") == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;

      if (q != SUCCEEDED) {
        print_error("STEP needs a value\n", ERROR_DIR);
        return FAILED;
      }

      enumid_adder = d;
    }

    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();

      enumid_export = 1;
    }

    enumid_defined = 1;

    return SUCCEEDED;
  }
  else {
    print_error(".ENUMID needs a value or a definition name.\n", ERROR_DIR);
    return FAILED;
  }
}


int directive_input(void) {
  
  char k[256];
  int j, v;

  if (get_next_token() == FAILED)
    return FAILED;

  fgets(k, 254, stdin);

  for (j = 0; j < 254; j++) {
    if (k[j] == 0)
      break;
    if (k[j] == 0x0A) {
      k[j] = 0;
      break;
    }
    if (k[j] == 0x0D) {
      k[j] = 0;
      break;
    }
  }

  if (j == 254) {
    print_error("Error in .INPUT.\n", ERROR_DIR);
    return FAILED;
  }

  for (j = 0; j < 254; j++) {
    if (k[j] == 0) {
      print_error("No .INPUT?\n", ERROR_DIR);
      return FAILED;
    }
    if (!(k[j] == ' ' || k[j] == 0x09))
      break;
  }

  if (k[j] == '%') {
    v = 0;
    j++;
    for ( ; j < 254; j++) {
      if (k[j] == 0)
        break;
      if (k[j] == '0' || k[j] == '1')
        v = (v << 1) + k[j] - '0';
      else
        break;
    }
    if (k[j] == 0) {
      redefine(tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
      return SUCCEEDED;
    }
  }
  else if (k[j] == '$') {
    j++;
    v = 0;
    for ( ; j < 254; j++) {
      if (k[j] == 0)
        break;
      if (k[j] >= '0' && k[j] <= '9')
        v = (v << 4) + k[j] - '0';
      else if (k[j] >= 'a' && k[j] <= 'f')
        v = (v << 4) + k[j] - 'a' + 10;
      else if (k[j] >= 'A' && k[j] <= 'F')
        v = (v << 4) + k[j] - 'A' + 10;
      else
        break;
    }
    if (k[j] == 0) {
      redefine(tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
      return SUCCEEDED;
    }
  }
  else if (k[j] >= '0' && k[j] <= '9') {
    v = 0;
    for ( ; j < 254; j++) {
      if (k[j] == 0)
        break;
      if (k[j] >= '0' && k[j] <= '9')
        v = (v * 10) + k[j] - '0';
      else
        break;
    }
    if (k[j] == 0) {
      redefine(tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
      return SUCCEEDED;
    }
  }

  /* it's a string */
  redefine(tmp, 0.0, k, DEFINITION_TYPE_STRING, (int)strlen(k));

  return SUCCEEDED;
}


int directive_redefine_redef(void) {
  
  int j, size, export, q;
  double dou;
  char k[256];

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(tmp) == YES) {
    snprintf(emsg, sizeof(emsg), "\"%s\" is a reserved definition label and is not user definable.\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  input_float_mode = ON;
  q = get_new_definition_data(&j, k, &size, &dou, &export);
  input_float_mode = OFF;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error("Could not parse the definition data.\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    redefine(tmp, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    redefine(tmp, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    redefine(tmp, 0.0, k, DEFINITION_TYPE_STRING, size);
  else if (q == INPUT_NUMBER_STACK)
    redefine(tmp, (double)j, NULL, DEFINITION_TYPE_STACK, 0);

  if (export == YES) {
    if (export_a_definition(tmp) == FAILED)
      return FAILED;
  }
    
  return SUCCEEDED;
}


#ifdef Z80

int directive_smsheader(void) {
  
  int q;
    
  if (smsheader_defined != 0) {
    print_error(".SMSHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (computesmschecksum_defined != 0)
    print_error(".COMPUTESMSCHECKSUM is unnecessary when .SMSHEADER is defined.\n", ERROR_WRN);

  if (smstag_defined != 0)
    print_error(".SMSTAG is unnecessary when .SMSHEADER is defined.\n", ERROR_WRN);

  if (output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SMSHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDSMS") == 0)
      break;
    else if (strcaselesscmp(tmp, "VERSION") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < 0 || d > 15) {
        snprintf(emsg, sizeof(emsg), "VERSION needs a value between 0 and 15, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (smsversion_defined != 0) {
        if (smsversion != d) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      smsversion = d;
      smsversion_defined = 1;
    }
    else if (strcaselesscmp(tmp, "ROMSIZE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < 0 || d > 15) {
        snprintf(emsg, sizeof(emsg), "ROMSIZE needs a value between 0 and 15, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (smsromsize_defined != 0) {
        if (smsromsize != d) {
          print_error("ROMSIZE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      smsromsize = d;
      smsromsize_defined = 1;
    }
    else if (strcaselesscmp(tmp, "REGIONCODE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < 3|| d > 7) {
        snprintf(emsg, sizeof(emsg), "REGIONCODE needs a value between 3 and 7, got %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (smsregioncode_defined != 0) {
        if (smsregioncode != d) {
          print_error("REGIONCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      smsregioncode = d;
      smsregioncode_defined = 1;
    }
    else if (strcaselesscmp(tmp, "PRODUCTCODE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      smsproductcode1 = d & 255;
      smsproductcode_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      smsproductcode2 = d & 255;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      smsproductcode3 = d & 15;
    }
    else if (strcaselesscmp(tmp, "RESERVEDSPACE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("RESERVEDSPACE needs 2 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      smsreservedspace1 = d & 255;
      smsreservedspace_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("RESERVEDSPACE needs 2 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      smsreservedspace2 = d & 255;
    }
    else {
      ind = FAILED;
      break;
    }
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .SMSHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  smsheader_defined = 1;

  return SUCCEEDED;
}


int directive_sdsctag(void) {
  
  int q;

  if (sdsctag_defined != 0) {
    print_error(".SDSCTAG can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
    
  no_library_files(".SDSCTAG");

  input_float_mode = ON;
  q = input_number();
  input_float_mode = OFF;
  if (q != SUCCEEDED && q != INPUT_NUMBER_FLOAT) {
    print_error(".SDSCTAG needs the version number.\n" , ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED) {
    sdsc_ma = d;
    sdsc_mi = 0;
  }
  else {
    sdsc_ma = (int)parsed_double;
    sdsc_mi = parsed_double_decimal_numbers;
  }
  
  if (sdsc_ma >= 100 || sdsc_mi >= 100) {
    print_error(".SDSCTAG major and minor version numbers must be inside the range [0,99].\n" , ERROR_DIR);
    return FAILED;
  }

  /* program name */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == INPUT_NUMBER_STRING || q == SUCCEEDED || q == INPUT_NUMBER_STACK || q == INPUT_NUMBER_ADDRESS_LABEL)) {
    print_error(".SDSCTAG requires program name string (or a pointer to it).\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED) {
    sdsctag_name_type = TYPE_VALUE;
    sdsctag_name_value = d;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (label[0] == 0) {
      sdsctag_name_type = TYPE_VALUE;
      sdsctag_name_value = 0xFFFF; /* null string */
    }
    else {
      sdsctag_name_type = TYPE_STRING;
      strcpy(sdsctag_name_str, label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    sdsctag_name_type = TYPE_LABEL;
    strcpy(sdsctag_name_str, label);
  }
  else {
    sdsctag_name_type = TYPE_STACK_CALCULATION;
    sdsctag_name_value = latest_stack;
  }

  /* notes */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == INPUT_NUMBER_STRING || q == SUCCEEDED || q == INPUT_NUMBER_STACK || q == INPUT_NUMBER_ADDRESS_LABEL)) {
    print_error(".SDSCTAG requires program release notes string (or a pointer to it).\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED) {
    sdsctag_notes_type = TYPE_VALUE;
    sdsctag_notes_value = d;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (label[0] == 0) {
      sdsctag_notes_type = TYPE_VALUE;
      sdsctag_notes_value = 0xFFFF; /* null string */
    }
    else {
      sdsctag_notes_type = TYPE_STRING;
      strcpy(sdsctag_notes_str, label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    sdsctag_notes_type = TYPE_LABEL;
    strcpy(sdsctag_notes_str, label);
  }
  else {
    sdsctag_notes_type = TYPE_STACK_CALCULATION;
    sdsctag_notes_value = latest_stack;
  }

  /* program author */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == INPUT_NUMBER_STRING || q == SUCCEEDED || q == INPUT_NUMBER_STACK || q == INPUT_NUMBER_ADDRESS_LABEL)) {
    print_error(".SDSCTAG requires program author string (or a pointer to it).\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED) {
    sdsctag_author_type = TYPE_VALUE;
    sdsctag_author_value = d;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (label[0] == 0) {
      sdsctag_author_type = TYPE_VALUE;
      sdsctag_author_value = 0xFFFF; /* null string */
    }
    else {
      sdsctag_author_type = TYPE_STRING;
      strcpy(sdsctag_author_str, label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    sdsctag_author_type = TYPE_LABEL;
    strcpy(sdsctag_author_str, label);
  }
  else {
    sdsctag_author_type = TYPE_STACK_CALCULATION;
    sdsctag_author_value = latest_stack;
  }

  sdsctag_defined++;
  smstag_defined++;
  computesmschecksum_defined++;

  return SUCCEEDED;
}

#endif


int directive_macro(void) {

  struct macro_static *m;
  int macro_start_line;
  int q;

  if (dstruct_status == ON) {
    print_error("You can't define a macro inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  if (strcaselesscmp(cp, "ENDM") == 0) {
    print_error("A MACRO must have a name.\n", ERROR_DIR);
    return FAILED;
  }

  macro_start_line = active_file_info_last->line_current;

  /* append the namespace, if this file uses if */
  if (active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(tmp, sizeof(tmp), "MACRO") == FAILED)
      return FAILED;
  }

  if (macro_get(tmp, NO, &m) == FAILED)
    return FAILED;
  
  if (m != NULL) {
    snprintf(emsg, sizeof(emsg), "MACRO \"%s\" was defined for the second time.\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  m = calloc(sizeof(struct macro_static), 1);
  if (m == NULL) {
    print_error("Out of memory while allocating room for a new MACRO.\n", ERROR_DIR);
    return FAILED;
  }

  if (macros_first == NULL) {
    macros_first = m;
    macros_last = m;
  }
  else {
    macros_last->next = m;
    macros_last = m;
  }

  strcpy(m->name, tmp);
  m->next = NULL;
  m->calls = 0;
  m->filename_id = active_file_info_last->filename_id;
  m->argument_names = NULL;

  /* is ARGS defined? */
  q = 0;
  if (compare_next_token("ARGS") == SUCCEEDED) {
    skip_next_token();

    while (1) {
      ind = input_next_string();
      if (ind == FAILED)
        return FAILED;
      if (ind == INPUT_NUMBER_EOL) {
        if (q != 0) {
          next_line();
          break;
        }
        snprintf(emsg, sizeof(emsg), "MACRO \"%s\" is missing argument names?\n", m->name);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      q++;

      /* store the label */
      m->argument_names = realloc(m->argument_names, sizeof(char *)*q);
      if (m->argument_names == NULL) {
        print_error("Out of memory error.\n", ERROR_NONE);
        return FAILED;
      }
      m->argument_names[q-1] = calloc(strlen(tmp)+1, 1);
      if (m->argument_names[q-1] == NULL) {
        print_error("Out of memory error.\n", ERROR_NONE);
        return FAILED;
      }

      strcpy(m->argument_names[q-1], tmp);
    }
  }

  m->nargument_names = q;
  m->start = i;
  m->start_line = active_file_info_last->line_current;

  /* go to the end of the macro */
  for (; i < size; i++) {
    if (buffer[i] == 0x0A) {
      next_line();
      continue;
    }
    else if ((strncmp(&buffer[i], ".E", 2) == 0) && (buffer[i + 2] == 0x0A || buffer[i + 2] == ' ')) {
      active_file_info_last->line_current = macro_start_line;
      snprintf(emsg, sizeof(emsg), "MACRO \"%s\" wasn't terminated with .ENDM.\n", m->name);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    else if ((strncmp(&buffer[i], ".ENDM", 5) == 0 || strncmp(&buffer[i], ".endm", 5) == 0) && (buffer[i + 5] == 0x0A || buffer[i + 5] == ' ')) {
      i += 5;
      break;
    }
  }

  return SUCCEEDED;
}


int directive_rept_repeat(void) {
  
  char c[16], index_name[MAX_NAME_LENGTH + 1];
  int q;

  strcpy(c, cp);

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), ".%s needs a count.\n", c);
    print_error(emsg, ERROR_INP);
    return FAILED;
  }

  if (d < 0) {
    snprintf(emsg, sizeof(emsg), ".%s count value must be positive or zero.\n", c);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  index_name[0] = 0;
  if (compare_next_token("INDEX") == SUCCEEDED) {
    skip_next_token();

    ind = input_next_string();
    if (ind != SUCCEEDED)
      return FAILED;

    if (redefine(tmp, 0.0, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    strcpy(index_name, tmp);
  }
    
  if (d == 0) {
    int l, r, m;

    l = active_file_info_last->line_current;
    /* find the next compiling point */
    r = 1;
    m = macro_active;
    /* disable macro decoding */
    macro_active = 0;
    while (get_next_token() != FAILED) {
      if (tmp[0] == '.') {
        if (strcaselesscmp(cp, "ENDR") == 0)
          r--;
        if (strcaselesscmp(cp, "E") == 0)
          break;
        if (strcaselesscmp(cp, "REPT") == 0 || strcaselesscmp(cp, "REPEAT") == 0)
          r++;
      }
      if (r == 0) {
        macro_active = m;
        return SUCCEEDED;
      }
    }
    
    /* return the condition's line number */
    active_file_info_last->line_current = l;
    snprintf(emsg, sizeof(emsg), ".%s must end to .ENDR.\n", c);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (repeat_active == repeat_stack_size) {
    struct repeat_runtime *rr;

    repeat_stack_size = (repeat_stack_size<<1)+2;
    rr = realloc(repeat_stack, sizeof(struct repeat_runtime) * repeat_stack_size);
    if (rr == NULL) {
      print_error("Out of memory error while enlarging repeat stack buffer.\n", ERROR_ERR);
      return FAILED;
    }
    repeat_stack = rr;
  }

  repeat_stack[repeat_active].start = i;
  repeat_stack[repeat_active].counter = d;
  repeat_stack[repeat_active].repeats = 0;
  repeat_stack[repeat_active].start_line = active_file_info_last->line_current;
  strcpy(repeat_stack[repeat_active].index_name, index_name);

  repeat_active++;

  /* repeat start */
  fprintf(file_out_ptr, "j ");
      
  return SUCCEEDED;
}


int directive_endm(void) {

  int q;
  
  if (macro_active != 0) {
    macro_active--;

    /* macro call end */
    fprintf(file_out_ptr, "I%s ", macro_stack[macro_active].macro->name);
    
    /* free the arguments */
    if (macro_stack[macro_active].supplied_arguments > 0) {
      for (d = 0; d < macro_stack[macro_active].supplied_arguments; d++)
        free(macro_stack[macro_active].argument_data[d]);
      free(macro_stack[macro_active].argument_data);
      macro_stack[macro_active].argument_data = NULL;
    }

    /* free the argument definitions */
    for (q = 0; q < macro_stack[macro_active].macro->nargument_names; q++)
      undefine(macro_stack[macro_active].macro->argument_names[q]);

    i = macro_stack[macro_active].macro_return_i;

    if ((extra_definitions == ON) && (active_file_info_last->filename_id != macro_stack[macro_active].macro_return_filename_id)) {
      redefine("WLA_FILENAME", 0.0, get_file_name(macro_stack[macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(macro_stack[macro_active].macro_return_filename_id)));
      redefine("wla_filename", 0.0, get_file_name(macro_stack[macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(macro_stack[macro_active].macro_return_filename_id)));
    }

    active_file_info_last->filename_id = macro_stack[macro_active].macro_return_filename_id;
    active_file_info_last->line_current = macro_stack[macro_active].macro_return_line;

    /* was this the last macro called? */
    if (macro_active == 0) {
      /* delete NARGS */
      undefine("NARGS");
      undefine("nargs");

      macro_runtime_current = NULL;
    }
    else {
      /* redefine NARGS */
      if (redefine("NARGS", (double)macro_stack[macro_active - 1].supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
      if (redefine("nargs", (double)macro_stack[macro_active - 1].supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;

      macro_runtime_current = &macro_stack[macro_active - 1];
    }

    /* was this a DBM macro call? */
    if (macro_stack[macro_active].caller == MACRO_CALLER_DBM) {
      /* yep, get the output */
      if (macro_insert_byte_db("DBM") == FAILED)
        return FAILED;

      /* continue defining bytes */
      if (macro_start_dxm(macro_stack[macro_active].macro, MACRO_CALLER_DBM, "DBM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DWM macro call? */
    else if (macro_stack[macro_active].caller == MACRO_CALLER_DWM) {
      /* yep, get the output */
      if (macro_insert_word_db("DWM") == FAILED)
        return FAILED;

      /* continue defining words */
      if (macro_start_dxm(macro_stack[macro_active].macro, MACRO_CALLER_DWM, "DWM", NO) == FAILED)
        return FAILED;
    }
#if W65816
    /* was this a DLM macro call? */
    else if (macro_stack[macro_active].caller == MACRO_CALLER_DLM) {
      /* yep, get the output */
      if (macro_insert_long_db("DLM") == FAILED)
        return FAILED;

      /* continue defining longs */
      if (macro_start_dxm(macro_stack[macro_active].macro, MACRO_CALLER_DLM, "DLM", NO) == FAILED)
        return FAILED;
    }
#endif
    /* or was this an INCBIN with a filter macro call? */
    else if (macro_stack[macro_active].caller == MACRO_CALLER_INCBIN) {
      /* yep, get the output */
      if (macro_insert_byte_db("INCBIN") == FAILED)
        return FAILED;

      /* continue filtering the binary file */
      if (macro_start_incbin(macro_stack[macro_active].macro, NULL, NO) == FAILED)
        return FAILED;
    }

    return SUCCEEDED;
  }

  snprintf(emsg, sizeof(emsg), "No .MACRO open.\n");
  print_error(emsg, ERROR_DIR);

  return FAILED;
}


#ifdef W65816

int directive_snesheader(void) {

  int q;
  
  if (snesheader_defined != 0) {
    print_error(".SNESHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (computesneschecksum_defined != 0)
    print_error(".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER is defined.\n", ERROR_WRN);
  else
    computesneschecksum_defined++;

  if (output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDSNES") == 0)
      break;
    else if (strcaselesscmp(tmp, "ID") == 0) {
      if ((ind = get_next_token()) == FAILED)
        return FAILED;

      if (ind != GET_NEXT_TOKEN_STRING || tmp[4] != 0) {
        print_error("ID requires a string of 1 to 4 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no ID has been defined so far */
      if (snesid_defined == 0) {
        for (ind = 0; tmp[ind] != 0 && ind < 4; ind++)
          snesid[ind] = tmp[ind];

        for ( ; ind < 4; snesid[ind] = 0, ind++)
          ;

        snesid_defined = 1;
      }
      /* compare the IDs */
      else {
        for (ind = 0; tmp[ind] != 0 && snesid[ind] != 0 && ind < 4; ind++)
          if (snesid[ind] != tmp[ind])
            break;

        if (ind == 4 && tmp[ind] != 0) {
          print_error("ID requires a string of 1 to 4 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (ind != 4 && (snesid[ind] != 0 || tmp[ind] != 0)) {
          print_error("ID was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }    
    else if (strcaselesscmp(tmp, "NAME") == 0) {
      if ((ind = get_next_token()) == FAILED)
        return FAILED;

      if (ind != GET_NEXT_TOKEN_STRING) {
        print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no name has been defined so far */
      if (name_defined == 0) {
        for (ind = 0; tmp[ind] != 0 && ind < 21; ind++)
          name[ind] = tmp[ind];

        if (ind == 21 && tmp[ind] != 0) {
          print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
          return FAILED;
        }

        for ( ; ind < 21; name[ind] = 0, ind++)
          ;

        name_defined = 1;
      }
      /* compare the names */
      else {
        for (ind = 0; tmp[ind] != 0 && name[ind] != 0 && ind < 21; ind++)
          if (name[ind] != tmp[ind])
            break;

        if (ind == 21 && tmp[ind] != 0) {
          print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (ind != 21 && (name[ind] != 0 || tmp[ind] != 0)) {
          print_error("NAME was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }
    else if (strcaselesscmp(tmp, "HIROM") == 0) {
      if (lorom_defined != 0 || exlorom_defined != 0 || exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".HIROM");
        return FAILED;
      }

      hirom_defined++;
    }
    else if (strcaselesscmp(tmp, "EXHIROM") == 0) {
      if (lorom_defined != 0 || exlorom_defined != 0 || hirom_defined != 0) {
        give_snes_rom_mode_defined_error(".EXHIROM");
        return FAILED;
      }

      exhirom_defined++;
    }
    else if (strcaselesscmp(tmp, "LOROM") == 0) {
      if (hirom_defined != 0 || exlorom_defined != 0 || exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".LOROM");
        return FAILED;
      }

      lorom_defined++;
    }
    /*
      else if (strcaselesscmp(tmp, "EXLOROM") == 0) {
      if (hirom_defined != 0 || lorom_defined != 0 || exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".EXLOROM");
      return FAILED;
      }

      exlorom_defined++;
      }
    */
    else if (strcaselesscmp(tmp, "SLOWROM") == 0) {
      if (fastrom_defined != 0) {
        print_error(".FASTROM was defined prior to .SLOWROM.\n", ERROR_DIR);
        return FAILED;
      }

      slowrom_defined++;
    }
    else if (strcaselesscmp(tmp, "FASTROM") == 0) {
      if (slowrom_defined != 0) {
        print_error(".SLOWROM was defined prior to .FASTROM.\n", ERROR_DIR);
        return FAILED;
      }

      fastrom_defined++;
    }
    else if (strcaselesscmp(tmp, "CARTRIDGETYPE") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "CARTRIDGETYPE expects 8-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (cartridgetype_defined != 0 && d != cartridgetype) {
          print_error("CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        cartridgetype = d;
        cartridgetype_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "ROMSIZE") == 0) {
      if (snesromsize != 0) {
        print_error("ROMSIZE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "ROMSIZE expects 8-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED)
        snesromsize = d;
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "SRAMSIZE") == 0) {
      if (sramsize_defined != 0) {
        print_error("SRAMSIZE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < 0 || d > 3)) {
        snprintf(emsg, sizeof(emsg), "SRAMSIZE expects 0-3, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        sramsize = d;
        sramsize_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "COUNTRY") == 0) {
      if (country_defined != 0) {
        print_error("COUNTRY can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "COUNTRY expects 8-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        country = d;
        country_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "LICENSEECODE") == 0) {
      if (licenseecode_defined != 0) {
        print_error("LICENSEECODE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "LICENSEECODE expects 8-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        licenseecode = d;
        licenseecode_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(tmp, "VERSION") == 0) {
      inz = input_number();

      if (inz == SUCCEEDED && (d < -128 || d > 255)) {
        snprintf(emsg, sizeof(emsg), "VERSION expects 8-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      else if (inz == SUCCEEDED) {
        if (version_defined != 0 && version != d) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        version = d;
        version_defined++;
      }
      else
        return FAILED;
    }
    else {
      ind = FAILED;
      break; 
    } 
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .SNESHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  snesheader_defined = 1;
  snes_mode++;

  return SUCCEEDED;
}


int directive_snesnativevector(void) {
  
  int cop_defined = 0, brk_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, unused_defined = 0, irq_defined = 0, q;
  char cop[512], brk[512], abort[512], nmi[512], unused[512], irq[512];

  if (snesnativevector_defined != 0) {
    print_error(".SNESNATIVEVECTOR can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (hirom_defined == 0 && lorom_defined == 0 && exhirom_defined == 0 && exlorom_defined == 0) {
    print_error(".SNESNATIVEVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
    return FAILED;
  }
  if (output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESNATIVEVECTOR.\n", ERROR_DIR);
    return FAILED;
  }

  /* create a section for the data */
  if (create_a_new_section_structure() == FAILED)
    return FAILED;
  strcpy(sec_tmp->name, "!__WLA_SNESNATIVEVECTOR");
  sec_tmp->status = SECTION_STATUS_ABSOLUTE;
  
  if (lorom_defined || exlorom_defined)
    base_address = 0x7FE4;
  else if (hirom_defined)
    base_address = 0xFFE4;
  else if (exhirom_defined)
    base_address = 0x40FFE4;
    
  fprintf(file_out_ptr, "P O0 A%d %d ", sec_tmp->id, base_address);
  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDNATIVEVECTOR") == 0) {
      if (cop_defined == 0)
        snprintf(cop, sizeof(cop), "y%d ", 0x0000);
      if (brk_defined == 0)
        snprintf(brk, sizeof(brk), "y%d ", 0x0000);
      if (abort_defined == 0)
        snprintf(abort, sizeof(abort), "y%d ", 0x0000);
      if (nmi_defined == 0)
        snprintf(nmi, sizeof(nmi), "y%d ", 0x0000);
      if (unused_defined == 0) 
        snprintf(unused, sizeof(unused), "y%d ", 0x0000);
      if (irq_defined == 0)
        snprintf(irq, sizeof(irq), "y%d ", 0x0000);
      
      fprintf(file_out_ptr, "%s%s%s%s%s%s s p ", cop, brk, abort, nmi, unused, irq);
      
      break;
    }
    else if (strcaselesscmp(tmp, "COP") == 0) {
      if (cop_defined != 0) {
        print_error("COP can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "COP expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(cop, sizeof(cop), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(cop, sizeof(cop), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(cop, sizeof(cop), "C%d ", latest_stack);

      cop_defined++;
    }
    else if (strcaselesscmp(tmp, "BRK") == 0) {
      if (brk_defined != 0) {
        print_error("BRK can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "BRK expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(brk, sizeof(brk), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(brk, sizeof(brk), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(brk, sizeof(brk), "C%d ", latest_stack);

      brk_defined++;
    }
    else if (strcaselesscmp(tmp, "ABORT") == 0) {
      if (abort_defined != 0) {
        print_error("ABORT can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "ABORT expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(abort, sizeof(abort), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(abort, sizeof(abort), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(abort, sizeof(abort), "C%d ", latest_stack);

      abort_defined++;
    }
    else if (strcaselesscmp(tmp, "NMI") == 0) {
      if (nmi_defined != 0) {
        print_error("NMI can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "NMI expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(nmi, sizeof(nmi), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(nmi, sizeof(nmi), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(nmi, sizeof(nmi), "C%d ", latest_stack);

      nmi_defined++;
    }
    else if (strcaselesscmp(tmp, "UNUSED") == 0) {
      if (unused_defined != 0) {
        print_error("UNUSED can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "UNUSED expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(unused, sizeof(unused), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(unused, sizeof(unused), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(unused, sizeof(unused), "C%d ", latest_stack);

      unused_defined++;
    }
    else if (strcaselesscmp(tmp, "IRQ") == 0) {
      if (irq_defined != 0) {
        print_error("IRQ can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "IRQ expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(irq, sizeof(irq), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(irq, sizeof(irq), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(irq, sizeof(irq), "C%d ", latest_stack);

      irq_defined++;
    }
    else {
      ind = FAILED;
      break;
    }
  }

  if (ind != SUCCEEDED) {
    print_error("Error in .SNESNATIVEVECTOR data structure.\n", ERROR_DIR);
    return FAILED;
  }

  snesnativevector_defined = 1;
  snes_mode++;

  return SUCCEEDED;
}


int directive_snesemuvector(void) {
  
  int cop_defined = 0, unused_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, reset_defined = 0, irqbrk_defined = 0, q;
  char cop[512], unused[512], abort[512], nmi[512], reset[512], irqbrk[512];

  if (snesemuvector_defined != 0) {
    print_error(".SNESEMUVECTOR can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (hirom_defined == 0 && lorom_defined == 0 && exhirom_defined == 0 && exlorom_defined == 0) {
    print_error(".SNESEMUVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
    return FAILED;
  }
  if (output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESEMUVECTOR.\n", ERROR_DIR);
    return FAILED;
  }

  /* create a section for the data */
  if (create_a_new_section_structure() == FAILED)
    return FAILED;
  strcpy(sec_tmp->name, "!__WLA_SNESEMUVECTOR");
  sec_tmp->status = SECTION_STATUS_ABSOLUTE;

  if (lorom_defined || exlorom_defined)
    base_address = 0x7FF4;
  else if (hirom_defined)
    base_address = 0xFFF4;
  else if (exhirom_defined)
    base_address = 0x40FFF4;

  fprintf(file_out_ptr, "P O0 A%d %d ", sec_tmp->id, base_address);
  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  while ((ind = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(tmp, ".ENDEMUVECTOR") == 0) {
      if (cop_defined == 0)
        snprintf(cop, sizeof(cop), "y%d ", 0);
      if (reset_defined == 0)
        snprintf(reset, sizeof(reset), "y%d ", 0);
      if (abort_defined == 0)
        snprintf(abort, sizeof(abort), "y%d ", 0);
      if (nmi_defined == 0)
        snprintf(nmi, sizeof(nmi), "y%d ", 0);
      if (unused_defined == 0)
        snprintf(unused, sizeof(unused), "y%d ", 0);
      if (irqbrk_defined == 0)
        snprintf(irqbrk, sizeof(irqbrk), "y%d ", 0);

      fprintf(file_out_ptr, "%s%s%s%s%s%s s p ", cop, unused, abort, nmi, reset, irqbrk);
      
      break;
    }
    else if (strcaselesscmp(tmp, "COP") == 0) {
      if (cop_defined != 0) {
        print_error("COP can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "COP expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(cop, sizeof(cop), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(cop, sizeof(cop), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(cop, sizeof(cop), "C%d ", latest_stack);

      cop_defined++;
    }
    else if (strcaselesscmp(tmp, "RESET") == 0) {
      if (reset_defined != 0) {
        print_error("RESET can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "RESET expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(reset, sizeof(reset), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(reset, sizeof(reset), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(reset, sizeof(reset), "C%d ", latest_stack);

      reset_defined++;
    }
    else if (strcaselesscmp(tmp, "ABORT") == 0) {
      if (abort_defined != 0) {
        print_error("ABORT can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "ABORT expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(abort, sizeof(abort), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(abort, sizeof(abort), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(abort, sizeof(abort), "C%d ", latest_stack);

      abort_defined++;
    }
    else if (strcaselesscmp(tmp, "NMI") == 0) {
      if (nmi_defined != 0) {
        print_error("NMI can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "NMI expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(nmi, sizeof(nmi), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(nmi, sizeof(nmi), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(nmi, sizeof(nmi), "C%d ", latest_stack);

      nmi_defined++;
    }
    else if (strcaselesscmp(tmp, "UNUSED") == 0) {
      if (unused_defined != 0) {
        print_error("UNUSED can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();

      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "UNUSED expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(unused, sizeof(unused), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(unused, sizeof(unused), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(unused, sizeof(unused), "C%d ", latest_stack);

      unused_defined++;
    }
    else if (strcaselesscmp(tmp, "IRQBRK") == 0) {
      if (irqbrk_defined != 0) {
        print_error("IRQBRK can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      inz = input_number();
      
      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        snprintf(emsg, sizeof(emsg), "IRQBRK expects 16-bit data, %d is out of range!\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        snprintf(irqbrk, sizeof(irqbrk), "y%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(irqbrk, sizeof(irqbrk), "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        snprintf(irqbrk, sizeof(irqbrk), "C%d ", latest_stack);

      irqbrk_defined++;
    }
    else {
      ind = FAILED;
      break;
    }
  }
  
  if (ind != SUCCEEDED) {
    print_error("Error in .SNESEMUVECTOR data structure.\n", ERROR_DIR);
    return FAILED;
  }

  snesemuvector_defined = 1;
  snes_mode++;

  return SUCCEEDED;
}

#endif


int directive_print(void) {

  int get_value, value_type;

  while (1) {
    get_value = NO;
    value_type = 1;
      
    if (compare_next_token("HEX") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;
      
      value_type = 0;
      get_value = YES;
    }
    else if (compare_next_token("DEC") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      value_type = 1;
      get_value = YES;
    }

    inz = input_number();

    if (inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL) {
      char t[256];
    
      if (get_value == YES) {
        print_error(".PRINT was expecting a value, got a string/label instead.\n", ERROR_INP);
        return FAILED;
      }

      parse_print_string(label, t, 256);

      if (quiet == NO) {
        printf("%s", t);
        fflush(stdout);
      }
    }
    else if (inz == SUCCEEDED) {
      if (quiet == NO) {
        if (value_type == 0)
          printf("%x", d);
        else
          printf("%d", d);
        fflush(stdout);
      }
    }
    else if (inz == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else {
      print_error(".PRINT needs a string/label or (an optional) HEX/DEC plus a value.\n", ERROR_DIR);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_printt(void) {
  
  char t[256];
    
  inz = input_number();

  if (inz != INPUT_NUMBER_STRING && inz != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".PRINTT needs a string/label.\n", ERROR_DIR);
    return FAILED;
  }

  parse_print_string(label, t, 256);
    
  if (quiet == NO) {
    printf("%s", t);
    fflush(stdout);
  }

  return SUCCEEDED;
}


int directive_printv(void) {

  int m = 1, q;

  if (compare_next_token("HEX") == SUCCEEDED) {
    skip_next_token();
    m = 0;
  }
  else if (compare_next_token("DEC") == SUCCEEDED) {
    skip_next_token();
    m = 1;
  }

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    if (q == INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(emsg, sizeof(emsg), "\"%s\" is not known.\n", label);
      print_error(emsg, ERROR_DIR);
    }
    print_error(".PRINTV can only print currently known values.\n", ERROR_DIR);
    return FAILED;
  }

  if (quiet == NO) {
    if (m == 0)
      printf("%x", d);
    else
      printf("%d", d);
    fflush(stdout);
  }

  return SUCCEEDED;
}


int directive_dbrnd_dwrnd(void) {
  
  int o, c, min, max, f, q;

  /* bytes or words? */
  if (cp[1] == 'W')
    o = 1;
  else
    o = 0;

  /* get the count */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), ".%s needs the number of random numbers.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  c = d;

  if (c <= 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs that the number of random numbers is > 0.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* get min */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), ".%s needs the min value.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  min = d;

  /* get max */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(emsg, sizeof(emsg), ".%s needs the max value.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  max = d;

  if (min >= max) {
    snprintf(emsg, sizeof(emsg), ".%s needs that min < max.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* generate the numbers */
  for (f = 0; f < c; f++) {
    d = (genrand_int32() % (max-min+1)) + min;
    
    if (o == 1) {
      if (d < -32768 || d > 65535) {
        snprintf(emsg, sizeof(emsg), ".%s: Expected a 16-bit value, computed %d.\n", cp, d);
        print_error(emsg, ERROR_NONE);
        return FAILED;
      }
      fprintf(file_out_ptr, "y %d ", d);
    }
    else {
      if (d > 255 || d < -128) {
        snprintf(emsg, sizeof(emsg), ".%s: Expected a 8-bit value, computed %d.\n", cp, d);
        print_error(emsg, ERROR_NONE);
        return FAILED;
      }
      fprintf(file_out_ptr, "d%d ", d);
    }
  }

  return SUCCEEDED;
}


int directive_dwsin_dbsin_dwcos_dbcos(void) {
  
  double m, a, s, n;
  int p, c, o, f;

  if (cp[1] == 'W')
    o = 1;
  else
    o = 2;
  if (cp[2] == 'S')
    f = 1;
  else
    f = 2;

  input_float_mode = ON;
  p = input_number();
  input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(emsg, sizeof(emsg), ".%s needs a value for starting angle.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    a = d;
  else
    a = parsed_double;

  if (input_number() != SUCCEEDED || d < 0) {
    snprintf(emsg, sizeof(emsg), ".%s needs an non-negative integer value for additional angles.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  c = d;

  input_float_mode = ON;
  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(emsg, sizeof(emsg), ".%s needs a value for angle step.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    s = d;
  else
    s = parsed_double;

  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(emsg, sizeof(emsg), ".%s needs a value to multiply the result with.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    m = d;
  else
    m = parsed_double;

  p = input_number();
  input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(emsg, sizeof(emsg), ".%s needs a value to add to the result.\n", cp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    n = d;
  else
    n = parsed_double;

  for (c++; c > 0; c--) {
    while (a >= 360)
      a -= 360;

    if (f == 1)
      d = (int)((m * sin(a*M_PI/180)) + n);
    else
      d = (int)((m * cos(a*M_PI/180)) + n);

    if (o == 1) {
      if (d < -32768 || d > 65535) {
        snprintf(emsg, sizeof(emsg), ".%s: Expected a 16-bit value, computed %d.\n", cp, d);
        print_error(emsg, ERROR_NONE);
        return FAILED;
      }
      fprintf(file_out_ptr, "y %d ", d);
    }
    else {
      if (d > 255 || d < -128) {
        snprintf(emsg, sizeof(emsg), ".%s: Expected a 8-bit value, computed %d.\n", cp, d);
        print_error(emsg, ERROR_NONE);
        return FAILED;
      }
      fprintf(file_out_ptr, "d%d ", d);
    }

    a += s;
  }

  return SUCCEEDED;
}


int directive_stringmap_table(void) {

  int parse_result, line_number = 0;
  FILE* table_file;
  char line_buffer[256];
  struct stringmaptable *map;

  expect_calculations = NO;
  parse_result = input_number();
  expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".STRINGMAPTABLE needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  /* Allocate and insert at the front of the chain */
  map = calloc(sizeof(struct stringmaptable), 1);
  if (map == NULL) {
    print_error("STRINGMAPTABLE: Out of memory error.\n", ERROR_ERR);
    return FAILED;
  }
  map->next = stringmaptables;
  stringmaptables = map;

  strcpy(map->name, label);

  expect_calculations = NO;
  parse_result = input_number();
  expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".STRINGMAPTABLE needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  table_file = fopen(label, "r");
  if (table_file == NULL) {
    snprintf(emsg, sizeof(emsg), "Error opening file \"%s\".\n", label);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  while (fgets(line_buffer, 256, table_file)) {
    char* p = line_buffer, *equals_pos;
    struct stringmap_entry* entry;
    int char_count;
    unsigned char* bytes_writer;
    int accumulator = 0;

    line_number++;

    /* skip comments */
    if (*p == ';' || *p == '#')
      continue;

    equals_pos = strchr(p, '=');

    /* lines should be in the form <hex>=<text> with no whitespace. */
    if (equals_pos == NULL)
      continue;

    entry = calloc(sizeof(struct stringmap_entry), 1);
    if (entry == NULL) {
      print_error("STRINGMAPTABLE: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }

    /* insert at front of entries list */
    entry->next = map->entries;
    map->entries = entry;

    /* left of = should be a string of hex digits, for a variable whole number of bytes */
    char_count = (int)(equals_pos - p);
    if (char_count == 0) {
      snprintf(emsg, sizeof(emsg), "STRINGMAPTABLE: No text before '=' at line %d of file \"%s\".\n", line_number, label);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    entry->bytes_length = char_count / 2 + char_count % 2;
    entry->bytes = calloc(sizeof(unsigned char), entry->bytes_length);
    if (entry->bytes == NULL) {
      print_error("STRINGMAPTABLE: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    bytes_writer = entry->bytes;
    for (; p != equals_pos; ++p) {
      /* parse character as hex */
      const char c = *p;
      if (c >= '0' && c <= '9')
        accumulator |= c - '0';
      else if (c >= 'a' && c <= 'f')
        accumulator |= c - 'a' + 10;
      else if (c >= 'A' && c <= 'F')
        accumulator |= c - 'A' + 10;
      else {
        snprintf(emsg, sizeof(emsg), "STRINGMAPTABLE: Invalid hex character '%c' at line %d of file \"%s\".\n", c, line_number, label);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      /* emit to buffer or shift depending on position */
      if ((equals_pos - p) % 2 == 0) {
        /* even count -> shift */
        accumulator <<= 4;
      }
      else {
        /* odd -> finished a byte */
        *bytes_writer++ = (unsigned char)accumulator;
        accumulator = 0;
      }
    }
    /* then the string. we want to remove any trailing CRLF. */
    p[strcspn(p, "\r\n")] = 0;
    entry->text_length = (int)strlen(++p);
    if (entry->text_length == 0) {
      snprintf(emsg, sizeof(emsg), "STRINGMAPTABLE: no text after '=' at line %d of file \"%s\".\n", line_number, label);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    p = equals_pos + 1;
    entry->text = calloc(sizeof(char), strlen(p) + 1);
    if (entry->text == NULL) {
      print_error("STRINGMAPTABLE: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    strcpy(entry->text, p);
  }

  fclose(table_file);

  return SUCCEEDED;
}


int directive_stringmap(void) {

  int parse_result;
  struct stringmaptable *table;
  char *p;

  /* first get the map name */
  expect_calculations = NO;
  parse_result = input_number();
  expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".STRINGMAP needs a table name.\n", ERROR_DIR);
    return FAILED;
  }

  /* find the table */
  for (table = stringmaptables; table != NULL; table = table->next) {
    if (strcaselesscmp(table->name, label) == 0) {
      /* found it */
      break;
    }
  }
  if (table == NULL) {
    snprintf(emsg, sizeof(emsg), "STRINGMAP: could not find table called \"%s\".\n", label);
    print_error(emsg, ERROR_DIR);
    return FAILED;    
  }

  /* parse the string */
  parse_result = input_number();
  if (parse_result != INPUT_NUMBER_STRING) {
    print_error("STRINGMAP: no string given", ERROR_DIR);
    return FAILED;    
  }

  fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  /* parse it */
  for (p = label; *p != 0; /* increment in loop */) {
    struct stringmap_entry *candidate, *entry = NULL;
    int i;

    /* find the longest match for the current string position */
    for (candidate = table->entries; candidate != NULL; candidate = candidate->next) {
      /* skip candidates not longer than the current best */
      if (entry != NULL && entry->text_length >= candidate->text_length)
        continue;

      /* check for a match */
      if (strncmp(p, candidate->text, candidate->text_length) == 0)
        entry = candidate;
    }
    /* if no match was found, it's an error */
    if (entry == NULL) {
      snprintf(emsg, sizeof(emsg), "STRINGMAP: could not find a match in the table at substring \"%s\".\n", p);
      print_error(emsg, ERROR_DIR);
      return FAILED;    
    }
    /* else emit */
    for (i = 0; i < entry->bytes_length; ++i)
      fprintf(file_out_ptr, "d%d ", entry->bytes[i]);

    /* move pointer on by as much as was matched */
    p += entry->text_length;
  }

  return SUCCEEDED;
}


int parse_directive(void) {

  int q;

  if ((q = parse_if_directive()) != -1)
    return q;
  
  /* ORG */

  if (strcaselesscmp(cp, "ORG") == 0)
    return directive_org();

  /* ORGA */

  if (strcaselesscmp(cp, "ORGA") == 0)
    return directive_orga();

  /* SLOT */

  if (strcaselesscmp(cp, "SLOT") == 0)
    return directive_slot();

  /* BANK */

  if (strcaselesscmp(cp, "BANK") == 0)
    return directive_bank();

  /* DBM/DWM? */

  if (strcaselesscmp(cp, "DBM") == 0 || strcaselesscmp(cp, "DWM") == 0)
    return directive_dbm_dwm_dlm();

  /* TABLE? */

  if (strcaselesscmp(cp, "TABLE") == 0)
    return directive_table();
    
  /* ROW/DATA? */

  if (strcaselesscmp(cp, "ROW") == 0 || strcaselesscmp(cp, "DATA") == 0)
    return directive_row_data();

  /* DB/BYT/BYTE? */

  if (strcaselesscmp(cp, "DB") == 0 || strcaselesscmp(cp, "BYT") == 0 || strcaselesscmp(cp, "BYTE") == 0)
    return directive_db_byt_byte();

  /* ASCTABLE/ASCIITABLE? */

  if (strcaselesscmp(cp, "ASCTABLE") == 0 || strcaselesscmp(cp, "ASCIITABLE") == 0)
    return directive_asctable_asciitable();

  /* ASC? */

  if (strcaselesscmp(cp, "ASC") == 0)
    return directive_asc();

  /* DW/WORD/ADDR? */

  if (strcaselesscmp(cp, "DW") == 0 || strcaselesscmp(cp, "WORD") == 0 || strcaselesscmp(cp, "ADDR") == 0)
    return directive_dw_word_addr();

#ifdef W65816
  
  /* DLM? */

  if (strcaselesscmp(cp, "DLM") == 0)
    return directive_dbm_dwm_dlm();

  /* DL/LONG/FARADDR? */

  if (strcaselesscmp(cp, "DL") == 0 || strcaselesscmp(cp, "LONG") == 0 || strcaselesscmp(cp, "FARADDR") == 0)
    return directive_dl_long_faraddr();

  /* DSL? */

  if (strcaselesscmp(cp, "DSL") == 0)
    return directive_dsl();

  /* NAME */

  if (strcaselesscmp(cp, "NAME") == 0)
    return directive_name_w65816();

  /* WDC */

  if (strcaselesscmp(cp, "WDC") == 0) {
    use_wdc_standard = 1;
    return SUCCEEDED;
  }

  /* NOWDC */

  if (strcaselesscmp(cp, "NOWDC") == 0) {
    use_wdc_standard = 0;
    return SUCCEEDED;
  }
  
#endif

  /* DSTRUCT */

  if (strcaselesscmp(cp, "DSTRUCT") == 0)
    return directive_dstruct();

  /* DS/DSB? */

  if (strcaselesscmp(cp, "DSB") == 0 || strcaselesscmp(cp, "DS") == 0)
    return directive_dsb_ds();

  /* DSW? */

  if (strcaselesscmp(cp, "DSW") == 0)
    return directive_dsw();
    
  /* INCDIR */

  if (strcaselesscmp(cp, "INCDIR") == 0)
    return directive_incdir();

  /* INCLUDE/INC */

  if (strcaselesscmp(cp, "INCLUDE") == 0 || strcaselesscmp(cp, "INC") == 0)
    return directive_include(YES);

  /* INDLUDE/IND (INTERNAL) */

  if (strcaselesscmp(cp, "INDLUDE") == 0 || strcaselesscmp(cp, "IND") == 0)
    return directive_include(NO);

  /* INCBIN */

  if (strcaselesscmp(cp, "INCBIN") == 0)
    return directive_incbin();

  /* OUTNAME */

  if (strcaselesscmp(cp, "OUTNAME") == 0) {
    expect_calculations = NO;
    inz = input_number();
    expect_calculations = YES;

    if (inz != INPUT_NUMBER_STRING && inz != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".OUTNAME needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(final_name, label);

    return SUCCEEDED;
  }

  /* STRUCT */

  if (strcaselesscmp(cp, "STRUCT") == 0)
    return directive_struct();

  /* RAMSECTION */

  if (strcaselesscmp(cp, "RAMSECTION") == 0)
    return directive_ramsection();

  /* SECTION */

  if (strcaselesscmp(cp, "SECTION") == 0)
    return directive_section();

  /* FOPEN */

  if (strcaselesscmp(cp, "FOPEN") == 0)
    return directive_fopen();

  /* FCLOSE */

  if (strcaselesscmp(cp, "FCLOSE") == 0)
    return directive_fclose();

  /* FSIZE */

  if (strcaselesscmp(cp, "FSIZE") == 0)
    return directive_fsize();

  /* FREAD */

  if (strcaselesscmp(cp, "FREAD") == 0)
    return directive_fread();

  /* BLOCK */

  if (strcaselesscmp(cp, "BLOCK") == 0)
    return directive_block();

  /* ENDB */

  if (strcaselesscmp(cp, "ENDB") == 0) {
    if (block_status <= 0) {
      print_error("There is no open .BLOCK.\n", ERROR_DIR);
      return FAILED;
    }

    block_status--;
    fprintf(file_out_ptr, "G ");

    return SUCCEEDED;
  }

  /* SHIFT */

  if (strcaselesscmp(cp, "SHIFT") == 0)
    return directive_shift();

  /* ENDS */

  if (strcaselesscmp(cp, "ENDS") == 0) {
    if (section_status == OFF) {
      print_error("There is no open section.\n", ERROR_DIR);
      return FAILED;
    }
    if (dstruct_status == ON) {
      print_error("You can't close a section inside .DSTRUCT.\n", ERROR_DIR);
      return FAILED;
    }

    /* generate a section end label? */
    if (extra_definitions == ON)
      generate_label("SECTIONEND_", sections_last->name);
  
    section_status = OFF;
    bankheader_status = OFF;
    in_ramsection = NO;
    
    fprintf(file_out_ptr, "s ");

    return SUCCEEDED;
  }

  /* ROMBANKS */

  if (strcaselesscmp(cp, "ROMBANKS") == 0)
    return directive_rombanks();

  /* ROMBANKMAP */

  if (strcaselesscmp(cp, "ROMBANKMAP") == 0)
    return directive_rombankmap();

  /* MEMORYMAP */

  if (strcaselesscmp(cp, "MEMORYMAP") == 0)
    return directive_memorymap();

  /* UNBACKGROUND */

  if (strcaselesscmp(cp, "UNBACKGROUND") == 0)
    return directive_unbackground();

  /* BACKGROUND */

  if (strcaselesscmp(cp, "BACKGROUND") == 0)
    return directive_background();

#ifdef GB

  /* NINTENDOLOGO */
  
  if (strcaselesscmp(cp, "NINTENDOLOGO") == 0) {
    no_library_files(".NINTENDOLOGO");

    nintendologo_defined++;

    return SUCCEEDED;
  }
  
  /* NAME */

  if (strcaselesscmp(cp, "NAME") == 0)
    return directive_name_gb();

  /* RAMSIZE */

  if (strcaselesscmp(cp, "RAMSIZE") == 0) {
    no_library_files(".RAMSIZE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".RAMSIZE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (rambanks_defined != 0) {
      if (rambanks != d) {
        print_error(".RAMSIZE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    if (d != 0 && d != 1 && d != 2 && d != 3 && d != 4 && d != 5) {
      print_error("Unsupported RAM size.\n", ERROR_DIR);
      return FAILED;
    }

    rambanks = d;
    rambanks_defined = 1;

    return SUCCEEDED;
  }

  /* COUNTRYCODE */

  if (strcaselesscmp(cp, "COUNTRYCODE") == 0) {
    no_library_files(".COUNTRYCODE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".COUNTRYCODE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (countrycode_defined != 0) {
      if (countrycode != d) {
        print_error(".COUNTRYCODE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    countrycode = d;
    countrycode_defined = 1;

    return SUCCEEDED;
  }
    
  /* DESTINATIONCODE */

  if (strcaselesscmp(cp, "DESTINATIONCODE") == 0) {
    no_library_files(".DESTINATIONCODE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".DESTINATIONCODE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (q == SUCCEEDED && countrycode_defined != 0) {
      if (countrycode != d) {
        print_error(".DESTINATIONCODE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    countrycode = d;
    countrycode_defined = 1;

    return SUCCEEDED;
  }

  /* CARTRIDGETYPE */

  if (strcaselesscmp(cp, "CARTRIDGETYPE") == 0) {
    no_library_files(".CARTRIDGETYPE");

    q = input_number();

    if (q == SUCCEEDED && cartridgetype_defined != 0) {
      if (cartridgetype != d) {
        print_error(".CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".CARTRIDGETYPE needs an immediate value.\n", ERROR_DIR);
      return FAILED;
    }

    cartridgetype = d;
    cartridgetype_defined = 1;

    return SUCCEEDED;
  }

  /* LICENSEECODENEW */

  if (strcaselesscmp(cp, "LICENSEECODENEW") == 0) {
    no_library_files(".LICENSEECODENEW");
    
    if (licenseecodeold_defined != 0) {
      print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
      return FAILED;
    }

    if ((ind = get_next_token()) == FAILED)
      return FAILED;

    if (ind != GET_NEXT_TOKEN_STRING) {
      print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (!(tmp[0] != 0 && tmp[1] != 0 && tmp[2] == 0)) {
      print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
      return FAILED;
    }

    if (licenseecodenew_defined != 0) {
      if (tmp[0] != licenseecodenew_c1 || tmp[1] != licenseecodenew_c2) {
        print_error(".LICENSEECODENEW was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    licenseecodenew_c1 = tmp[0];
    licenseecodenew_c2 = tmp[1];
    licenseecodenew_defined = 1;

    return SUCCEEDED;
  }

  /* LICENSEECODEOLD */

  if (strcaselesscmp(cp, "LICENSEECODEOLD") == 0) {
    no_library_files(".LICENSEECODEOLD");
    
    if (licenseecodenew_defined != 0) {
      print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < -128 || d > 255) {
      snprintf(emsg, sizeof(emsg), ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (licenseecodeold_defined != 0) {
      if (licenseecodeold != d) {
        print_error(".LICENSEECODEOLD was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    licenseecodeold = d;
    licenseecodeold_defined = 1;

    return SUCCEEDED;
  }

  /* VERSION */

  if (strcaselesscmp(cp, "VERSION") == 0) {
    no_library_files(".VERSION");
    
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < -128 || d > 255) {
      snprintf(emsg, sizeof(emsg), ".VERSION needs a 8-bit value, got %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (version_defined != 0) {
      if (version != d) {
        print_error(".VERSION was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    version = d;
    version_defined = 1;

    return SUCCEEDED;
  }

  /* GBHEADER */

  if (strcmp(cp, "GBHEADER") == 0)
    return directive_gbheader();

#endif

  /* EMPTYFILL */

  if (strcaselesscmp(cp, "EMPTYFILL") == 0) {
    no_library_files(".EMPTYFILL");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < -128 || d > 255) {
      snprintf(emsg, sizeof(emsg), ".EMPTYFILL needs a 8-bit value, got %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (emptyfill_defined != 0) {
      if (emptyfill != d) {
        print_error(".EMPTYFILL was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    emptyfill = d;
    emptyfill_defined = 1;

    return SUCCEEDED;
  }

  /* DEFINE/DEF/EQU */

  if (strcaselesscmp(cp, "DEFINE") == 0 || strcaselesscmp(cp, "DEF") == 0 || strcaselesscmp(cp, "EQU") == 0)
    return directive_define_def_equ();

  /* ENUMID */

  if (strcaselesscmp(cp, "ENUMID") == 0)
    return directive_enumid();

  /* INPUT */

  if (strcaselesscmp(cp, "INPUT") == 0)
    return directive_input();

  /* REDEFINE/REDEF */

  if (strcaselesscmp(cp, "REDEFINE") == 0 || strcaselesscmp(cp, "REDEF") == 0)
    return directive_redefine_redef();

  /* EXPORT */

  if (strcaselesscmp(cp, "EXPORT") == 0) {
    q = 0;
    while (1) {
      ind = input_next_string();
      if (ind == FAILED)
        return FAILED;
      if (ind == INPUT_NUMBER_EOL) {
        if (q != 0) {
          next_line();
          return SUCCEEDED;
        }
        print_error(".EXPORT requires definition name(s).\n", ERROR_DIR);
        return FAILED;
      }

      q++;

      if (export_a_definition(tmp) == FAILED)
        return FAILED;
    }

    return FAILED;
  }

  /* SYM/SYMBOL */

  if (strcaselesscmp(cp, "SYM") == 0 || strcaselesscmp(cp, "SYMBOL") == 0) {
    ind = input_next_string();
    if (ind != SUCCEEDED) {
      print_error(".SYM requires a symbol name.\n", ERROR_DIR);
      return FAILED;
    }

    fprintf(file_out_ptr, "Y%s ", tmp);

    return SUCCEEDED;
  }

  /* BR/BREAKPOINT */

  if (strcaselesscmp(cp, "BR") == 0 || strcaselesscmp(cp, "BREAKPOINT") == 0) {
    fprintf(file_out_ptr, "Z ");
    return SUCCEEDED;
  }

  /* ENUM */

  if (strcaselesscmp(cp, "ENUM") == 0) {
    if (dstruct_status == ON) {
      print_error("You can't use start an ENUM inside .DSTRUCT.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".ENUM needs a starting value.\n", ERROR_DIR);
      return FAILED;
    }

    enum_offset = 0;
    last_enum_offset = 0;
    max_enum_offset = 0;
    base_enum_offset = d;

    /* "ASC" or "DESC"? */
    if (compare_next_token("ASC") == SUCCEEDED) {
      enum_ord = 1;
      skip_next_token();
    }
    else if (compare_next_token("DESC") == SUCCEEDED) {
      enum_ord = -1;
      skip_next_token();
    }
    else
      enum_ord = 1;

    /* do we have "EXPORT" defined? */
    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();
      enum_exp = YES;
    }
    else
      enum_exp = NO;

    /* setup active_struct (enum vars stored here temporarily) */
    active_struct = calloc(sizeof(struct structure), 1);
    if (active_struct == NULL) {
      print_error("Out of memory while parsing .ENUM.\n", ERROR_DIR);
      return FAILED;
    }
    active_struct->name[0] = '\0';
    active_struct->items = NULL;
    active_struct->last_item = NULL;
    union_stack = NULL;

    in_enum = YES;

    return SUCCEEDED;
  }

#ifdef GB

  /* COMPUTEGBCHECKSUM */

  if (strcaselesscmp(cp, "COMPUTECHECKSUM") == 0 || strcaselesscmp(cp, "COMPUTEGBCHECKSUM") == 0) {
    no_library_files(".COMPUTEGBCHECKSUM");
    
    if (gbheader_defined != 0)
      print_error(".COMPUTEGBCHECKSUM is unnecessary when GBHEADER is defined.\n", ERROR_WRN);

    computechecksum_defined = 1;

    return SUCCEEDED;
  }

  /* COMPUTEGBCOMPLEMENTCHECK */

  if (strcaselesscmp(cp, "COMPUTEGBCOMPLEMENTCHECK") == 0 || strcaselesscmp(cp, "COMPUTECOMPLEMENTCHECK") == 0) {
    no_library_files(".COMPUTEGBCOMPLEMENTCHECK");
    
    if (gbheader_defined != 0)
      print_error(".COMPUTEGBCOMPLEMENTCHECK is unnecessary when GBHEADER is defined.\n", ERROR_WRN);

    computecomplementcheck_defined = 1;

    return SUCCEEDED;
  }
  
#endif

#ifdef W65816

  /* COMPUTESNESCHECKSUM */

  if (strcaselesscmp(cp, "COMPUTESNESCHECKSUM") == 0) {
    no_library_files(".COMPUTESNESCHECKSUM");
    
    if (hirom_defined == 0 && lorom_defined == 0 && exhirom_defined == 0 && exlorom_defined == 0) {
      print_error(".COMPUTESNESCHECKSUM needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
      return FAILED;
    }
    if (snesheader_defined != 0) 
      print_error(".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER defined.\n", ERROR_WRN);

    computesneschecksum_defined = 1;

    return SUCCEEDED;
  }

#endif

#ifdef Z80

  /* COMPUTESMSCHECKSUM */

  if (strcaselesscmp(cp, "COMPUTESMSCHECKSUM") == 0) {
    no_library_files(".COMPUTESMSCHECKSUM");

    computesmschecksum_defined++;

    return SUCCEEDED;
  }

  /* SMSTAG */

  if (strcaselesscmp(cp, "SMSTAG") == 0) {
    no_library_files(".SMSTAG");

    smstag_defined++;
    computesmschecksum_defined++;

    return SUCCEEDED;
  }

  /* SMSHEADER */
  if (strcmp(cp, "SMSHEADER") == 0)
    return directive_smsheader();
  
  /* SDSCTAG */

  if (strcaselesscmp(cp, "SDSCTAG") == 0)
    return directive_sdsctag();

#endif

  /* MACRO */

  if (strcaselesscmp(cp, "MACRO") == 0)
    return directive_macro();

  /* REPT/REPEAT */

  if (strcaselesscmp(cp, "REPT") == 0 || strcaselesscmp(cp, "REPEAT") == 0)
    return directive_rept_repeat();

  /* ENDR */

  if (strcaselesscmp(cp, "ENDR") == 0) {

    struct repeat_runtime *rr;
    
    if (repeat_active == 0) {
      print_error("There is no open repetition.\n", ERROR_DIR);
      return FAILED;
    }

    rr = &repeat_stack[repeat_active - 1];

    rr->counter--;
    if (rr->counter == 0) {
      repeat_active--;
      
      /* repeat end */
      fprintf(file_out_ptr, "J ");

      if (strlen(rr->index_name) > 0) {
        if (undefine(rr->index_name) == FAILED)
          return FAILED;
      }
      return SUCCEEDED;
    }

    rr->repeats++;
    if (strlen(rr->index_name) > 0) {
      if (redefine(rr->index_name, (double)rr->repeats, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
    }
    
    i = rr->start;
    active_file_info_last->line_current = rr->start_line;

    return SUCCEEDED;
  }

  /* CHANGEFILE (INTERNAL) */
  if (strcaselesscmp(cp, "CHANGEFILE") == 0) {
    q = input_number();
    if (q != SUCCEEDED) {
      print_error("Internal error in (internal) .CHANGEFILE. Please submit a bug report...\n", ERROR_DIR);
      return FAILED;
    }
    
    active_file_info_tmp = calloc(sizeof(struct active_file_info), 1);
    if (active_file_info_tmp == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while trying allocate error tracking data structure.\n");
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    active_file_info_tmp->next = NULL;

    if (active_file_info_first == NULL) {
      active_file_info_first = active_file_info_tmp;
      active_file_info_last = active_file_info_tmp;
      active_file_info_tmp->prev = NULL;
    }
    else {
      active_file_info_tmp->prev = active_file_info_last;
      active_file_info_last->next = active_file_info_tmp;
      active_file_info_last = active_file_info_tmp;
    }

    active_file_info_tmp->line_current = 0;
    active_file_info_tmp->filename_id = d;

    if (extra_definitions == ON) {
      file_name_info_tmp = file_name_info_first;
      while (file_name_info_tmp != NULL) {
        if (file_name_info_tmp->id == d)
          break;
        file_name_info_tmp = file_name_info_tmp->next;
      }

      if (file_name_info_tmp == NULL) {
        snprintf(emsg, sizeof(emsg), "Internal error: Could not find the name of file %d.\n", d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      redefine("WLA_FILENAME", 0.0, file_name_info_tmp->name, DEFINITION_TYPE_STRING, (int)strlen(file_name_info_tmp->name));
      redefine("wla_filename", 0.0, file_name_info_tmp->name, DEFINITION_TYPE_STRING, (int)strlen(file_name_info_tmp->name));
    }

    /* output the file id */
    fprintf(file_out_ptr, "f%d ", active_file_info_tmp->filename_id);
    
    open_files++;

    if (compare_next_token("NAMESPACE") == SUCCEEDED) {
      skip_next_token();

      expect_calculations = NO;
      q = input_number();
      expect_calculations = YES;
    
      if (q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
        print_error("Internal error: Namespace string is missing.\n", ERROR_DIR);
        return FAILED;
      }

      strcpy(active_file_info_tmp->namespace, label);

      fprintf(file_out_ptr, "t1 %s ", active_file_info_tmp->namespace);
    }
    else if (compare_next_token("NONAMESPACE") == SUCCEEDED) {
      skip_next_token();
      
      active_file_info_tmp->namespace[0] = 0;

      fprintf(file_out_ptr, "t0 ");
    }
    else {
      print_error("Internal error: NAMESPACE/NONAMESPACE is missing.\n", ERROR_DIR);
      return FAILED;
    }
    
    return SUCCEEDED;
  }

  /* E (INTERNAL) */

  if (strcaselesscmp(cp, "E") == 0) {
    if (active_file_info_last != NULL) {
      active_file_info_tmp = active_file_info_last;
      active_file_info_last = active_file_info_last->prev;
      free(active_file_info_tmp);

      if (active_file_info_last == NULL)
        active_file_info_first = NULL;
      else {
        fprintf(file_out_ptr, "f%d ", active_file_info_last->filename_id);

        if (active_file_info_last->namespace[0] == 0)
          fprintf(file_out_ptr, "t0 ");
        else
          fprintf(file_out_ptr, "t1 %s ", active_file_info_last->namespace);      
      }
    }

    /* fix the line */
    if (active_file_info_last != NULL)
      active_file_info_last->line_current--;

    fprintf(file_out_ptr, "E ");
    open_files--;
    if (open_files == 0)
      return EVALUATE_TOKEN_EOP;

    if (extra_definitions == ON) {
      redefine("WLA_FILENAME", 0.0, get_file_name(active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(active_file_info_last->filename_id)));
      redefine("wla_filename", 0.0, get_file_name(active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(active_file_info_last->filename_id)));
    }

    return SUCCEEDED;
  }

  /* M */

  if (strcaselesscmp(cp, "M") == 0) {
    if (line_count_status == OFF)
      line_count_status = ON;
    else
      line_count_status = OFF;
    return SUCCEEDED;
  }

#ifdef GB

  /* ROMGBC */

  if (strcaselesscmp(cp, "ROMGBC") == 0) {
    no_library_files(".ROMGBC");
    
    if (romdmg != 0) {
      print_error(".ROMDMG was defined prior to .ROMGBC.\n", ERROR_DIR);
      return FAILED;
    }
    else if (romgbc == 2) {
      print_error(".ROMGBCONLY was defined prior to .ROMGBC.\n", ERROR_DIR);
      return FAILED;
    }

    romgbc = 1;

    return SUCCEEDED;
  }

  /* ROMGBCONLY */

  if (strcaselesscmp(cp, "ROMGBCONLY") == 0) {
    no_library_files(".ROMGBCONLY");

    if (romdmg != 0) {
      print_error(".ROMDMG was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
      return FAILED;
    }
    else if (romgbc == 1) {
      print_error(".ROMGBC was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
      return FAILED;
    }

    romgbc = 2;

    return SUCCEEDED;
  }

  /* ROMDMG */

  if (strcaselesscmp(cp, "ROMDMG") == 0) {
    no_library_files(".ROMDMG");
    
    if (romgbc == 1) {
      print_error(".ROMGBC was defined prior to .ROMDMG.\n", ERROR_DIR);
      return FAILED;
    }
    else if (romgbc == 2) {
      print_error(".ROMGBCONLY was defined prior to .ROMDMG.\n", ERROR_DIR);
      return FAILED;
    }
    else if (romsgb != 0) {
      print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
      return FAILED;
    }

    romdmg = 1;

    return SUCCEEDED;
  }

  /* ROMSGB */

  if (strcaselesscmp(cp, "ROMSGB") == 0) {
    no_library_files(".ROMSGB");
    
    if (romdmg != 0) {
      print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
      return FAILED;
    }

    romsgb = 1;

    return SUCCEEDED;
  }
#endif

  /* ROMBANKSIZE */

  if (strcaselesscmp(cp, "ROMBANKSIZE") == 0 || strcaselesscmp(cp, "BANKSIZE") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < 0) {
      print_error(".ROMBANKSIZE needs a positive integer value.\n", ERROR_DIR);
      return FAILED;
    }

    if (banksize_defined != 0) {
      if (banksize != d) {
        print_error(".ROMBANKSIZE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    banksize = d;
    banksize_defined = 1;

    return SUCCEEDED;
  }

  /* ENDM */

  if (strcaselesscmp(cp, "ENDM") == 0)
    return directive_endm();

  /* BASE */

  if (strcaselesscmp(cp, "BASE") == 0) {
    no_library_files(".BASE definitions");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < 0) {
      print_error(".BASE number must be zero or positive.\n", ERROR_DIR);
      return FAILED;
    }

    fprintf(file_out_ptr, "b%d ", d);

    return SUCCEEDED;
  }

#if defined(MCS6502) || defined(MCS6510) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)

  /* 8BIT */

  if (strcaselesscmp(cp, "8BIT") == 0) {
    xbit_size = 8;
    return SUCCEEDED;
  }

  /* 16BIT */

  if (strcaselesscmp(cp, "16BIT") == 0) {
    xbit_size = 16;
    return SUCCEEDED;
  }

#endif

#ifdef W65816

  /* 24BIT */

  if (strcaselesscmp(cp, "24BIT") == 0) {
    xbit_size = 24;
    return SUCCEEDED;
  }

  /* INDEX */

  if (strcaselesscmp(cp, "INDEX") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || !(d == 8 || d == 16)) {
      print_error("The index size must be 8 or 16.\n", ERROR_DIR);
      return FAILED;
    }

    index_size = d;

    return SUCCEEDED;
  }

  /* ACCU */

  if (strcaselesscmp(cp, "ACCU") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || !(d == 8 || d == 16)) {
      print_error("The accumulator size must be 8 or 16.\n", ERROR_DIR);
      return FAILED;
    }

    accu_size = d;

    return SUCCEEDED;
  }

  /* SMC */

  if (strcaselesscmp(cp, "SMC") == 0) {
    no_library_files(".SMC");

    smc_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* HIROM */

  if (strcaselesscmp(cp, "HIROM") == 0) {
    no_library_files(".HIROM");

    if (lorom_defined != 0 || exlorom_defined != 0 || exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".HIROM");
      return FAILED;
    }

    hirom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* EXHIROM */

  if (strcaselesscmp(cp, "EXHIROM") == 0) {
    no_library_files(".EXHIROM");

    if (lorom_defined != 0 || exlorom_defined != 0 || hirom_defined != 0) {
      give_snes_rom_mode_defined_error(".EXHIROM");
      return FAILED;
    }

    exhirom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }
  
  /* LOROM */

  if (strcaselesscmp(cp, "LOROM") == 0) {
    no_library_files(".LOROM");

    if (hirom_defined != 0 || exlorom_defined != 0 || exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".LOROM");
      return FAILED;
    }

    lorom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* EXLOROM */
  /*
    if (strcaselesscmp(cp, "EXLOROM") == 0) {
    no_library_files(".EXLOROM");

    if (hirom_defined != 0 || lorom_defined != 0 || exhirom_defined != 0) {
    give_snes_rom_mode_defined_error(".EXLOROM");
    return FAILED;
    }

    exlorom_defined++;
    snes_mode++;

    return SUCCEEDED;
    }
  */
  /* SLOWROM */

  if (strcaselesscmp(cp, "SLOWROM") == 0) {
    no_library_files(".SLOWROM");
    
    if (fastrom_defined != 0) {
      print_error(".FASTROM was defined prior to .SLOWROM.\n", ERROR_DIR);
      return FAILED;
    }

    slowrom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* FASTROM */

  if (strcaselesscmp(cp, "FASTROM") == 0) {
    no_library_files(".FASTROM");
    
    if (slowrom_defined != 0) {
      print_error(".SLOWROM was defined prior to .FASTROM.\n", ERROR_DIR);
      return FAILED;
    }

    fastrom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* SNESHEADER */

  if (strcaselesscmp(cp, "SNESHEADER") == 0)
    return directive_snesheader();

  /* SNESNATIVEVECTOR */

  if (strcaselesscmp(cp, "SNESNATIVEVECTOR") == 0)
    return directive_snesnativevector();

  /* SNESEMUVECTOR */

  if (strcaselesscmp(cp, "SNESEMUVECTOR") == 0)
    return directive_snesemuvector();

#endif

  /* PRINT */

  if (strcaselesscmp(cp, "PRINT") == 0)
    return directive_print();
  
  /* PRINTT */

  if (strcaselesscmp(cp, "PRINTT") == 0)
    return directive_printt();

  /* PRINTV */

  if (strcaselesscmp(cp, "PRINTV") == 0)
    return directive_printv();

  /* SEED */

  if (strcaselesscmp(cp, "SEED") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".SEED needs a seed value for the randon number generator.\n", ERROR_DIR);
      return FAILED;
    }

    /* reseed the random number generator */
    init_genrand(d);

    return SUCCEEDED;
  }

  /* DBRND/DWRND */

  if (strcaselesscmp(cp, "DBRND") == 0 || strcaselesscmp(cp, "DWRND") == 0)
    return directive_dbrnd_dwrnd();

  /* DWSIN/DBSIN/DWCOS/DBCOS */

  if (strcaselesscmp(cp, "DWSIN") == 0 || strcaselesscmp(cp, "DBSIN") == 0 || strcaselesscmp(cp, "DWCOS") == 0 || strcaselesscmp(cp, "DBCOS") == 0)
    return directive_dwsin_dbsin_dwcos_dbcos();

  /* FAIL */

  if (strcaselesscmp(cp, "FAIL") == 0) {
    print_error("HALT: .FAIL found.\n", ERROR_NONE);

    /* make a silent exit */
    exit(0);
  }

  /* UNDEF/UNDEFINE */

  if (strcaselesscmp(cp, "UNDEF") == 0 || strcaselesscmp(cp, "UNDEFINE") == 0)
    return directive_undef_undefine();

  /* ASM */

  if (strcaselesscmp(cp, "ASM") == 0)
    return SUCCEEDED;

  /* ENDASM */

  if (strcaselesscmp(cp, "ENDASM") == 0) {

    int endasm = 1, x;

    while (1) {
      x = i;
      q = get_next_token();
      if (q == GET_NEXT_TOKEN_STRING)
        continue;

      /* end of file? */
      if (strcmp(tmp, ".E") == 0) {
        i = x;
        return SUCCEEDED;
      }
      if (strcaselesscmp(tmp, ".ASM") == 0) {
        endasm--;
        if (endasm == 0)
          return SUCCEEDED;
      }
      if (strcaselesscmp(tmp, ".ENDASM") == 0)
        endasm++;
    }
  }

  /* STRINGMAPTABLE */

  if (strcaselesscmp(cp, "STRINGMAPTABLE") == 0)
    return directive_stringmap_table();

  /* STRINGMAP */

  if (strcaselesscmp(cp, "STRINGMAP") == 0)
    return directive_stringmap();

  return DIRECTIVE_NOT_IDENTIFIED;
}


/* parses only "if" directives. */
/* this is separate from parse_directive so that enums and ramsections can reuse this */
int parse_if_directive(void) {

  char bak[256];
  int q;

  /* ELSE */

  if (strcaselesscmp(cp, "ELSE") == 0) {

    int m, r;

    if (ifdef == 0) {
      print_error("There must be .IFxxx before .ELSE.\n", ERROR_DIR);
      return FAILED;
    }

    /* find the next compiling point */
    r = 1;
    m = macro_active;
    /* disable macro decoding */
    macro_active = 0;
    while (get_next_token() != FAILED) {
      if (tmp[0] == '.') {
        if (strcaselesscmp(cp, "ENDIF") == 0)
          r--;
        if (strcaselesscmp(cp, "E") == 0)
          break;
        if (strcaselesscmp(cp, "IFDEF") == 0 || strcaselesscmp(cp, "IFNDEF") == 0 || strcaselesscmp(cp, "IFGR") == 0 || strcaselesscmp(cp, "IFLE") == 0 || strcaselesscmp(cp, "IFEQ") == 0 ||
            strcaselesscmp(cp, "IFNEQ") == 0 || strcaselesscmp(cp, "IFDEFM") == 0 || strcaselesscmp(cp, "IFNDEFM") == 0 || strcaselesscmp(cp, "IF") == 0 || strcaselesscmp(cp, "IFEXISTS") == 0 ||
            strcaselesscmp(cp, "IFGREQ") == 0 || strcaselesscmp(cp, "IFLEEQ") == 0)
          r++;
      }
      if (r == 0) {
        ifdef--;
        macro_active = m;
        return SUCCEEDED;
      }
    }

    print_error(".ELSE must end to .ENDIF.\n", ERROR_DIR);
    return FAILED;
  }

  /* ENDIF */

  if (strcaselesscmp(cp, "ENDIF") == 0) {
    if (ifdef == 0) {
      print_error(".ENDIF was given before any .IF directive.\n", ERROR_DIR);
      return FAILED;
    }

    ifdef--;
    return SUCCEEDED;
  }

  /* IFDEF */

  if (strcaselesscmp(cp, "IFDEF") == 0) {

    struct definition *d;

    if (get_next_token() == FAILED)
      return FAILED;

    hashmap_get(defines_map, tmp, (void*)&d);
    if (d != NULL) {
      ifdef++;
      return SUCCEEDED;
    }

    return find_next_point("IFDEF");
  }

  /* IF */

  if (strcaselesscmp(cp, "IF") == 0) {

    char k[256];
    int y, o, s;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(emsg, sizeof(emsg), ".IF needs immediate data, string or an address label.\n");
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    strncpy(k, label, 255);
    k[255] = 0;
    y = d;
    s = q;

    if (get_next_token() == FAILED)
      return FAILED;

    if (strcmp(tmp, "<") == 0)
      o = 0;
    else if (strcmp(tmp, ">") == 0)
      o = 1;
    else if (strcmp(tmp, "==") == 0)
      o = 2;
    else if (strcmp(tmp, "!=") == 0)
      o = 3;
    else if (strcmp(tmp, ">=") == 0)
      o = 4;
    else if (strcmp(tmp, "<=") == 0)
      o = 5;
    else {
      print_error(".IF needs an operator. Supported operators are '<', '>', '>=', '<=', '!=' and '=='.\n", ERROR_INP);
      return FAILED;
    }

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(emsg, sizeof(emsg), ".IF needs immediate data, string or an address label.\n");
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error("The types of the compared things must be the same.\n", ERROR_INP);
      return FAILED;
    }

    /* values? */
    if (s == SUCCEEDED) {
      if ((o == 0 && y < d) || (o == 1 && y > d) || (o == 2 && y == d) || (o == 3 && y != d) || (o == 4 && y >= d) || (o == 5 && y <= d))
        q = SUCCEEDED;
      else
        q = FAILED;
    }
    /* strings? */
    else {
      if ((o == 0 && strcmp(k, label) < 0) || (o == 1 && strcmp(k, label) > 0) || (o == 2 && strcmp(k, label) == 0) || (o == 3 && strcmp(k, label) != 0) || (o == 4 && strcmp(k, label) >= 0) || (o == 5 && strcmp(k, label) <= 0))
        q = SUCCEEDED;
      else
        q = FAILED;
    }

    if (q == SUCCEEDED) {
      ifdef++;
      return SUCCEEDED;
    }
    else
      return find_next_point("IF");
  }

  /* IFGR/IFLE/IFEQ/IFNEQ/IFGREQ/IFLEEQ */

  if (strcaselesscmp(cp, "IFGR") == 0 || strcaselesscmp(cp, "IFLE") == 0 || strcaselesscmp(cp, "IFEQ") == 0 || strcaselesscmp(cp, "IFNEQ") == 0 || strcaselesscmp(cp, "IFGREQ") == 0 || strcaselesscmp(cp, "IFLEEQ") == 0) {

    char k[256];
    int y, o, s;

    strcpy(bak, cp);

    if (strcmp(&cp[2], "LE") == 0)
      o = 0;
    else if (strcmp(&cp[2], "GR") == 0)
      o = 1;
    else if (strcmp(&cp[2], "EQ") == 0)
      o = 2;
    else if (strcmp(&cp[2], "NEQ") == 0)
      o = 3;
    else if (strcmp(&cp[2], "GREQ") == 0)
      o = 4;
    else
      o = 5;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(emsg, sizeof(emsg), ".%s needs immediate data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    strncpy(k, label, 255);
    k[255] = 0;
    y = d;
    s = q;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(emsg, sizeof(emsg), ".%s needs immediate data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error("The types of the compared things must be the same.\n", ERROR_INP);
      return FAILED;
    }

    /* values? */
    if (s == SUCCEEDED) {
      if ((o == 0 && y < d) || (o == 1 && y > d) || (o == 2 && y == d) || (o == 3 && y != d) || (o == 4 && y >= d) || (o == 5 && y <= d))
        q = SUCCEEDED;
      else
        q = FAILED;
    }
    /* strings? */
    else {
      if ((o == 0 && strcmp(k, label) < 0) || (o == 1 && strcmp(k, label) > 0) || (o == 2 && strcmp(k, label) == 0) || (o == 3 && strcmp(k, label) != 0) || (o == 4 && strcmp(k, label) >= 0) || (o == 5 && strcmp(k, label) <= 0))
        q = SUCCEEDED;
      else
        q = FAILED;
    }

    if (q == SUCCEEDED) {
      ifdef++;
      return SUCCEEDED;
    }
    else {
      strcpy(k, cp);
      return find_next_point(k);
    }
  }

  /* IFEXISTS */

  if (strcaselesscmp(cp, "IFEXISTS") == 0) {

    FILE *f;

    expect_calculations = NO;
    inz = input_number();
    expect_calculations = YES;

    if (inz != INPUT_NUMBER_STRING && inz != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".IFEXISTS needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    f = fopen(label, "r");
    if (f == NULL)
      return find_next_point("IFEXISTS");

    fclose(f);
    ifdef++;

    return SUCCEEDED;
  }

  /* IFNDEF */

  if (strcaselesscmp(cp, "IFNDEF") == 0) {

    struct definition *d;

    if (get_next_token() == FAILED)
      return FAILED;

    hashmap_get(defines_map, tmp, (void*)&d);
    if (d != NULL) {
      strcpy(emsg, cp);
      return find_next_point(emsg);
    }

    ifdef++;

    return SUCCEEDED;
  }

  /* IFDEFM/IFNDEFM */

  if (strcaselesscmp(cp, "IFDEFM") == 0 || strcaselesscmp(cp, "IFNDEFM") == 0) {

    int k, o;
    char e;

    strcpy(bak, cp);

    if (macro_active == 0) {
      snprintf(emsg, sizeof(emsg), ".%s can be only used inside a macro.\n", bak);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (cp[2] == 'N')
      o = 0;
    else
      o = 1;

    for (; i < size; i++) {

      if (buffer[i] == 0x0A)
        break;
      else if (buffer[i] == '\\') {
        e = buffer[++i];
        if (e >= '0' && e <= '9') {
          d = (e - '0') * 10;
          for (k = 2; k < 8; k++, d *= 10) {
            e = buffer[++i];
            if (e >= '0' && e <= '9')
              d += e - '0';
            else
              break;
          }

          d /= 10;
          if ((o == 0 && macro_runtime_current->supplied_arguments < d) ||
              (o == 1 && macro_runtime_current->supplied_arguments >= d)) {
            ifdef++;
            return SUCCEEDED;
          }
          else {
            strcpy(emsg, cp);
            return find_next_point(emsg);
          }
        }
        break;
      }
    }

    snprintf(emsg, sizeof(emsg), ".%s needs an argument.\n", bak);
    print_error(emsg, ERROR_DIR);

    return FAILED;
  }

  /* neither success nor failure (didn't match any "if" directives) */
  return -1;
}


int find_next_point(char *name) {

  int depth, m, line_current;

  line_current = active_file_info_last->line_current;
  /* find the next compiling point */
  depth = 1;
  m = macro_active;
  /* disable macro decoding */
  macro_active = 0;
  while (get_next_token() != FAILED) {
    if (tmp[0] == '.') {
      if (strcaselesscmp(cp, "ENDIF") == 0)
        depth--;
      if (strcaselesscmp(cp, "ELSE") == 0 && depth == 1)
        depth--;
      if (strcaselesscmp(cp, "E") == 0)
        break;
      if (strcaselesscmp(cp, "IFDEF") == 0 || strcaselesscmp(cp, "IFNDEF") == 0 || strcaselesscmp(cp, "IFGR") == 0 || strcaselesscmp(cp, "IFLE") == 0 ||
          strcaselesscmp(cp, "IFEQ") == 0 || strcaselesscmp(cp, "IFNEQ") == 0 || strcaselesscmp(cp, "IFDEFM") == 0 || strcaselesscmp(cp, "IFNDEFM") == 0 ||
          strcaselesscmp(cp, "IF") == 0 || strcaselesscmp(cp, "IFGREQ") == 0 || strcaselesscmp(cp, "IFLEEQ") == 0 || strcaselesscmp(cp, "IFEXISTS") == 0)
        depth++;
    }
    if (depth == 0) {
      if (strcaselesscmp(cp, "ELSE") == 0)
        ifdef++;
      macro_active = m;
      return SUCCEEDED;
    }
  }

  /* return the condition's line number */
  active_file_info_last->line_current = line_current;
  snprintf(emsg, sizeof(emsg), ".%s must end to .ENDIF/.ELSE.\n", name);
  print_error(emsg, ERROR_DIR);

  return FAILED;
}


#ifndef GB

void delete_stack(struct stack *s) {

  if (s == NULL) {
    snprintf(emsg, sizeof(emsg), "Deleting a non-existing computation stack. This can lead to problems.\n");
    print_error(emsg, ERROR_WRN);
    return;
  }

  free(s->stack);
  free(s);
}

#endif


void parse_print_string(char *input, char *output, int output_size) {

  int s, u;

  for (s = 0, u = 0; input[s] != 0 && u < output_size-1; ) {
    if (input[s] == '\\' && input[s + 1] == 'n') {
#ifdef MSDOS
      output[u++] = '\r';
      output[u++] = '\n';
#else
      output[u++] = '\n';
#endif
      s += 2;
    }
    else if (input[s] == '\\' && input[s + 1] == '\\') {
      output[u++] = '\\';
      s += 2;
    }
    else
      output[u++] = input[s++];
  }

  output[u] = 0;
}


int is_reserved_definition(char *t) {

  if (strcaselesscmp(t, "WLA_TIME") == 0 ||
      strcaselesscmp(t, "WLA_VERSION") == 0 ||
      strcaselesscmp(t, "WLA_FILENAME") == 0 ||
      strcaselesscmp(t, "NARGS") == 0 ||
      strcaselesscmp(t, "CADDR") == 0)
    return YES;

  return NO;
}


int get_new_definition_data(int *b, char *c, int *size, double *data, int *export) {

  int a, x, n, s;

  *export = NO;
  
  x = input_number();
  a = x;
  n = 0;
  s = 0;

  if (x == INPUT_NUMBER_ADDRESS_LABEL) {
    /* address label -> stack */
    stack_create_label_stack(label);
    x = INPUT_NUMBER_STACK;
  }

  if (x == INPUT_NUMBER_STACK) {
    *b = stacks_tmp->id;
    stacks_tmp->position = STACK_POSITION_DEFINITION;

    /* export the definition? */
    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();
      *export = YES;
    }

    x = input_number();
    if (x != INPUT_NUMBER_EOL) {
      print_error("A computation cannot be output as a string.\n", ERROR_DIR);
      return SUCCEEDED;
    }
    next_line();
    return INPUT_NUMBER_STACK;
  }

  while (x != INPUT_NUMBER_EOL) {
    /* the first fetch will conserve both classes */
    if (n == 0) {
      if (x == SUCCEEDED)
        *b = d;
      else if (x == INPUT_NUMBER_STRING) {
        strcpy(c, label);
        s = (int)strlen(label);
      }
      else if (x == INPUT_NUMBER_FLOAT) {
        *data = parsed_double;
        *b = (int)parsed_double;
      }
      else
        return x;

      n++;

      /* export the definition? */
      if (compare_next_token("EXPORT") == SUCCEEDED) {
        skip_next_token();
        *export = YES;
      }

      x = input_number();
      continue;
    }
    /* the next fetches will produce strings */
    else if (n == 1 && (a == SUCCEEDED || a == INPUT_NUMBER_FLOAT)) {
      if (*b > 255) {
        c[0] = (*b & 255);
        c[1] = (*b >> 8) & 255;
        c[2] = 0;
        s = 2;
      }
      else {
        c[0] = *b;
        c[1] = 0;
        s = 1;
      }
      a = INPUT_NUMBER_STRING;
    }

    /* transform floats to integers */
    if (x == INPUT_NUMBER_FLOAT) {
      d = (int)parsed_double;
      x = SUCCEEDED;
    }

    if (x == INPUT_NUMBER_STRING) {
      strcpy(&c[s], label);
      s += (int)strlen(label);
    }
    else if (x == SUCCEEDED) {
      if (d > 255) {
        c[s] = (d & 255);
        c[s + 1] = (d >> 8) & 255;
        c[s + 2] = 0;
        s += 2;
      }
      else {
        c[s] = d;
        c[s + 1] = 0;
        s++;
      }
    }
    else
      return x;

    n++;

    /* export the definition? */
    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();
      *export = YES;
    }

    x = input_number();
  }

  next_line();

  if (a == INPUT_NUMBER_STRING)
    c[s] = 0;

  *size = s;

  return a;
}


int export_a_definition(char *name) {

  struct export_def *export;

  /* don't export it twice or more often */
  export = export_first;
  while (export != NULL) {
    if (strcmp(export->name, name) == 0) {
      snprintf(emsg, sizeof(emsg), "\"%s\" was .EXPORTed for the second time.\n", name);
      print_error(emsg, ERROR_WRN);
      return SUCCEEDED;
    }
    export = export->next;
  }

  export = calloc(sizeof(struct export_def), 1);
  if (export == NULL) {
    snprintf(emsg, sizeof(emsg), "Out of memory while allocating room for \".EXPORT %s\".\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  strcpy(export->name, name);
  export->next = NULL;

  if (export_first == NULL) {
    export_first = export;
    export_last = export;
  }
  else {
    export_last->next = export;
    export_last = export;
  }

  return SUCCEEDED;
}


/* store the labels in a label stack in which label_stack[0] is the base level label,
   label_stack[1] is the first child, label_stack[2] is the second child, and so on... */
int add_label_to_label_stack(char *l) {

  int level = 0, q;

  /* skip anonymous labels */
  if (is_label_anonymous(l) == SUCCEEDED)
    return SUCCEEDED;

  for (q = 0; q < MAX_NAME_LENGTH; q++) {
    if (l[q] == '@')
      level++;
    else
      break;
  }

  if (level >= 256) {
    print_error("ADD_LABEL_TO_LABEL_STACK: Out of label stack depth. Can handle only 255 child labels...\n", ERROR_ERR);
    return FAILED;
  }

  if (level == 0) {
    /* resetting level 0 label clears all the other levels */
    for (q = 0; q < 256; q++)
      label_stack[q][0] = 0;
    strcpy(label_stack[0], l);
  }
  else
    strcpy(label_stack[level], &l[level-1]);

  /*
    fprintf(stderr, "*************************************\n");
    fprintf(stderr, "LABEL STACK:\n");
    for (q = 0; q < 256; q++) {
    if (label_stack[q][0] != 0)
    fprintf(stderr, "%s LEVEL %d\n", label_stack[q], q);
    }
    fprintf(stderr, "*************************************\n");
  */

  return SUCCEEDED;
}


/* get the full version of a (possibly child) label */
int get_full_label(char *l, char *out) {

  char *error_message = "GET_FULL_LABEL: Constructed label size will be >= MAX_NAME_LENGTH. Edit the define in shared.h, recompile WLA and try again...\n";
  int level = 0, q;

  for (q = 0; q < MAX_NAME_LENGTH; q++) {
    if (l[q] == '@')
      level++;
    else
      break;
  }

  if (level <= 0)
    strcpy(out, l);
  else {
    /* create the full label, e.g.: "BASE@CHILD1@CHILD2" */
    strcpy(out, label_stack[0]);
    for (q = 1; q < level; q++) {
      if (strlen(out) + strlen(label_stack[q]) >= MAX_NAME_LENGTH) {
        print_error(error_message, ERROR_ERR);
        return FAILED;  
      }
      strncat(out, label_stack[q], MAX_NAME_LENGTH);
    }

    if (strlen(out) + strlen(&l[level-1]) >= MAX_NAME_LENGTH) {
      print_error(error_message, ERROR_ERR);
      return FAILED;    
    }

    strncat(out, &l[level-1], MAX_NAME_LENGTH);
  }

  return SUCCEEDED;
}


int add_namespace_to_string(char *s, int sizeof_s, char *type) {

  char buf[MAX_NAME_LENGTH + 1];
    
  snprintf(buf, sizeof(buf), "%s.%s", active_file_info_last->namespace, s);
  buf[sizeof(buf)-1] = 0;
  if (strlen(buf) >= (size_t)sizeof_s) {
    snprintf(emsg, sizeof(emsg), "The current file namespace \"%s\" cannot be added to %s's \"%s\" name - increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", active_file_info_last->namespace, type, tmp);
    print_error(emsg, ERROR_ERR);
    return FAILED;
  }

  strcpy(s, buf);

  return SUCCEEDED;
}


void generate_label(char *header, char *footer) {

  char footer2[MAX_NAME_LENGTH + 1];
  int q, o;
  
  /* check if the footer contains spaces */
  o = (int)strlen(footer);
  for (q = 0; q < o; q++) {
    if (footer[q] == ' ')
      footer2[q] = '_';
    else
      footer2[q] = footer[q];
  }
  footer2[q] = 0;

  fprintf(file_out_ptr, "L%s%s ", header, footer2);
}

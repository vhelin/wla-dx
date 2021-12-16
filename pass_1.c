
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
#include "mersenne_twister.h"


#ifdef GB
char g_licenseecodenew_c1, g_licenseecodenew_c2;
int g_gbheader_defined = 0;
int g_nintendologo_defined = 0;
int g_computechecksum_defined = 0, g_computecomplementcheck_defined = 0;
int g_romgbc = 0, g_romdmg = 0, g_romsgb = 0;
int g_cartridgetype = 0, g_cartridgetype_defined = 0;
int g_countrycode = 0, g_countrycode_defined = 0;
int g_licenseecodenew_defined = 0, g_licenseecodeold = 0, g_licenseecodeold_defined = 0;
int g_version = 0, g_version_defined = 0;
#endif

#ifdef Z80
char g_sdsctag_name_str[MAX_NAME_LENGTH + 1], g_sdsctag_notes_str[MAX_NAME_LENGTH + 1], g_sdsctag_author_str[MAX_NAME_LENGTH + 1];
int g_sdsctag_name_type, g_sdsctag_notes_type, g_sdsctag_author_type, g_sdsc_ma, g_sdsc_mi;
int g_sdsctag_name_value, g_sdsctag_notes_value, g_sdsctag_author_value;
int g_computesmschecksum_defined = 0, g_sdsctag_defined = 0, g_smstag_defined = 0;
int g_smsheader_defined = 0, g_smsversion = 0, g_smsversion_defined = 0, g_smsregioncode = 0, g_smsregioncode_defined = 0;
int g_smsproductcode_defined = 0, g_smsproductcode1 = 0, g_smsproductcode2 = 0, g_smsproductcode3 = 0, g_smsreservedspace1 = 0;
int g_smsreservedspace2 = 0, smsreservedspace_defined = 0, g_smsromsize = 0, g_smsromsize_defined = 0;
#endif

int g_org_defined = 1, g_background_defined = 0;
int g_enumid_defined = 0, g_enumid = 0, g_enumid_adder = 1, g_enumid_export = 0;
int g_bank = 0, g_bank_defined = 1;
int g_rombanks = 0, g_rombanks_defined = 0, g_romtype = 0, g_max_address = 0;
int g_rambanks = 0, g_rambanks_defined = 0;
int g_emptyfill, g_emptyfill_defined = 0;
int g_section_status = OFF, g_section_id = 1, g_line_count_status = ON;
int g_parsed_int, g_source_pointer, g_ind, g_inz, g_ifdef = 0, g_slots_amount = 0, g_skip_elifs[256];
int g_memorymap_defined = 0;
int g_defaultslot_defined = 0, g_defaultslot, g_current_slot = 0;
int g_banksize_defined = 0, g_banksize;
int g_rombankmap_defined = 0, *g_banks = NULL, *g_bankaddress = NULL;
int g_bankheader_status = OFF;
int g_macro_active = 0;
int g_repeat_active = 0;
int g_smc_defined = 0;
int g_asciitable_defined = 0;
int g_block_status = 0, g_block_name_id = 0;
int g_dstruct_status = OFF;
int g_saved_structures_count = 0, g_saved_structures_max = 0;
unsigned char g_asciitable[256];

#ifndef GB
extern struct stack *g_stacks_header_first, *g_stacks_header_last;
extern int g_stacks_inside, g_stacks_outside;
int g_stack_inserted;
#endif

#if defined(W65816)
char g_snesid[4];
int g_snesheader_defined = 0, g_snesid_defined = 0, g_snesromsize = 0;
int g_sramsize_defined = 0, g_sramsize = 0, g_country_defined = 0, g_country = 0;
int g_cartridgetype = 0, g_cartridgetype_defined = 0, g_licenseecode_defined = 0, g_licenseecode = 0;
int g_version_defined = 0, g_version = 0, g_snesnativevector_defined = 0, g_snesemuvector_defined = 0;
int g_hirom_defined = 0, g_lorom_defined = 0, g_slowrom_defined = 0, g_fastrom_defined = 0, g_snes_mode = 0;
int g_exlorom_defined = 0, g_exhirom_defined = 0;
int g_computesneschecksum_defined = 0, g_use_wdc_standard = 0;
#endif

#if defined(GB) || defined(W65816)
char g_name[32];
int g_name_defined = 0;
#endif

char g_tmp[4096], g_error_message[sizeof(g_tmp) + MAX_NAME_LENGTH + 1 + 1024];
char *g_label_stack[256];
char g_current_directive[MAX_NAME_LENGTH + 1];

unsigned char *g_rom_banks = NULL, *g_rom_banks_usage_table = NULL;

struct structure **g_saved_structures = NULL;
struct export_def *g_export_first = NULL, *g_export_last = NULL;
struct optcode *g_opt_tmp;
struct definition *g_tmp_def;
struct map_t *g_defines_map = NULL;
struct macro_static *g_macros_first = NULL, *g_macros_last = NULL;
struct section_def *g_sections_first = NULL, *g_sections_last = NULL, *g_sec_tmp, *g_sec_next;
struct macro_runtime *g_macro_stack = NULL, *g_macro_runtime_current = NULL;
struct repeat_runtime *g_repeat_stack = NULL;
struct slot g_slots[256];
struct structure *g_structures_first = NULL;
struct filepointer *g_filepointers = NULL;
struct map_t *g_namespace_map = NULL;
struct after_section *g_after_sections = NULL;
struct label_sizeof *g_label_sizeofs = NULL;
struct block_name *g_block_names = NULL;
struct stringmaptable *g_stringmaptables = NULL;
struct array *g_arrays_first = NULL;

extern char *g_buffer, *unfolded_buffer, g_label[MAX_NAME_LENGTH + 1], *g_include_dir, *g_full_name;
extern int g_size, g_input_number_error_msg, g_verbose_mode, g_output_format, g_open_files, g_input_parse_if;
extern int g_stack_id, g_latest_stack, g_ss, g_commandline_parsing, g_newline_beginning, g_expect_calculations;
extern int g_extra_definitions, g_string_size, g_input_float_mode, g_operand_hint, g_operand_hint_type;
extern int g_include_dir_size, g_parse_floats, g_listfile_data, g_quiet, g_parsed_double_decimal_numbers;
extern int g_create_sizeof_definitions, g_input_allow_leading_hashtag, g_input_has_leading_hashtag, g_input_allow_leading_ampersand;
extern int g_plus_and_minus_ends_label;
extern FILE *g_file_out_ptr;
extern double g_parsed_double;
extern char *g_final_name;

extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern struct stack *g_stacks_first, *g_stacks_tmp, *g_stacks_last;
extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern int g_makefile_rules;

static int g_macro_stack_size = 0, g_repeat_stack_size = 0;

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(MCS6510) || defined(W65816) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
int g_xbit_size = 0, g_accu_size = 8, g_index_size = 8;
#endif

/* vars used when in an enum, ramsection, or struct. */
/* some variables named "enum_" are used in enums, ramsections, and structs. */
static int g_in_enum = NO, g_in_ramsection = NO, g_in_struct = NO;
static int g_enum_exp, g_enum_ord;
static int g_enum_offset; /* Offset relative to enum start where we're at right now */
static int g_last_enum_offset;
static int g_base_enum_offset; /* start address of enum */
static int g_enum_sizeof_pass; /* set on second pass through enum/ramsection, generating _sizeof labels */
/* temporary struct used to build up enums/ramsections (and, of course, structs)
   this gets temporarily replaced when inside a union (each union is considered a separate struct). */
static struct structure *g_active_struct = NULL;

static int g_union_base_offset; /* start address of current union */
static int g_max_enum_offset; /* highest position seen within current union group */
static struct structure *g_union_first_struct = NULL; /* first struct in current union */
static struct union_stack *g_union_stack = NULL; /* stores variables for nested unions */

/* for .TABLE, .DATA and .ROW */
static char g_table_format[256];
static int g_table_defined = 0, g_table_size = 0, g_table_index = 0;

extern int g_opcode_n[256], g_opcode_p[256];
extern struct optcode g_opcodes_table[];

#define no_library_files(name)                                          \
  do {                                                                  \
    if (g_output_format == OUTPUT_LIBRARY) {                            \
      print_error("Library files don't take " name ".\n", ERROR_DIR);   \
      return FAILED;                                                    \
    }                                                                   \
  } while (0)


int strcaselesscmp(char *s1, char *s2) {

  if (s1 == NULL || s2 == NULL)
    return 0;

  while (*s1 != 0 && *s2 != 0) {
    if (toupper((int)*s1) != toupper((int)*s2))
      return 1;
    s1++;
    s2++;
  }

  if (*s2 != 0)
    return 1;
  if (*s1 != 0)
    return -1;

  return 0;
}


static int _get_slot_number_by_its_name(char *slot_name, int *number) {

  int i;
  
  for (i = 0; i < g_slots_amount; i++) {
    if (strcmp(slot_name, g_slots[i].name) == 0) {
      *number = i;
      return SUCCEEDED;
    }
  }

  snprintf(g_error_message, sizeof(g_error_message), "Cannot find SLOT \"%s\".\n", slot_name);
  print_error(g_error_message, ERROR_DIR);

  return FAILED;  
}


static int _get_slot_number_by_a_value(int value, int *slot) {

  int i;

  if (value < 0) {
    *slot = value;
    return FAILED;
  }
  
  if (value < g_slots_amount) {
    /* value can be the direct SLOT ID, but can it be a SLOT's address as well? */
    for (i = 0; i < g_slots_amount; i++) {
      if (g_slots[i].address == value && value != i) {
        snprintf(g_error_message, sizeof(g_error_message), "There is a SLOT number %d, but there also is a SLOT (with ID %d) with starting address %d ($%x)... Using SLOT %d.\n", value, i, value, value, value);
        print_error(g_error_message, ERROR_WRN);
        break;
      }
    }
    
    *slot = value;
    return SUCCEEDED;
  }

  for (i = 0; i < g_slots_amount; i++) {
    if (g_slots[i].address == value) {
      *slot = i;
      return SUCCEEDED;
    }
  }

  *slot = -1;

  snprintf(g_error_message, sizeof(g_error_message), "Cannot find SLOT %d.\n", value);
  print_error(g_error_message, ERROR_DIR);
  
  return FAILED;
}


int macro_get(char *name, int add_namespace, struct macro_static **macro_out) {

  struct macro_static *macro;
  char fullname[MAX_NAME_LENGTH + 1];

  strcpy(fullname, name);

  /* append the namespace, if this file uses if */
  if (add_namespace == YES && g_active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(fullname, sizeof(fullname), "MACRO") == FAILED) {
      *macro_out = NULL;
      return FAILED;
    }
  }

  macro = g_macros_first;
  while (macro != NULL) {
    if (strcmp(macro->name, fullname) == 0)
      break;
    macro = macro->next;
  }

  *macro_out = macro;
  
  return SUCCEEDED;
}


int macro_stack_grow(void) {

  if (g_macro_active == g_macro_stack_size) {
    struct macro_runtime *macro;
    int old_size;

    /* enlarge the macro stack */
    old_size = g_macro_stack_size;
    g_macro_stack_size = (g_macro_stack_size<<1)+2;

    macro = calloc(sizeof(struct macro_runtime) * g_macro_stack_size, 1);
    if (macro == NULL) {
      print_error("Out of memory error while enlarging macro stack buffer.\n", ERROR_ERR);
      return FAILED;
    }

    if (g_macro_stack != NULL) {
      memcpy(macro, g_macro_stack, sizeof(struct macro_runtime) * old_size);
      free(g_macro_stack);
    }

    g_macro_stack = macro;
    g_macro_runtime_current = &g_macro_stack[g_macro_active - 1];
  }

  return SUCCEEDED;
}


int macro_start(struct macro_static *m, struct macro_runtime *mrt, int caller, int nargs) {

  g_macro_runtime_current = mrt;
  g_macro_active++;
  m->calls++;

  /* macro call start */
  fprintf(g_file_out_ptr, "i%s ", m->name);
  
  mrt->caller = caller;
  mrt->macro = m;
  mrt->macro_return_i = g_source_pointer;
  mrt->macro_return_line = g_active_file_info_last->line_current;
  mrt->macro_return_filename_id = g_active_file_info_last->filename_id;

  if ((g_extra_definitions == ON) && (g_active_file_info_last->filename_id != m->filename_id)) {
    redefine("WLA_FILENAME", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
    redefine("wla_filename", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
  }

  g_active_file_info_last->line_current = m->start_line;
  g_active_file_info_last->filename_id = m->filename_id;
  g_source_pointer = m->start;

  /* redefine NARGS */
  if (redefine("NARGS", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;
  if (redefine("nargs", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int macro_start_dxm(struct macro_static *m, int caller, char *name, int first) {

  struct macro_runtime *mrt;
  int start, number_result;
  
  /* start running a macro... run until .ENDM */
  if (macro_stack_grow() == FAILED)
    return FAILED;

  mrt = &g_macro_stack[g_macro_active];

  start = g_source_pointer;

  if (first == NO && mrt->string_current < mrt->string_last) {
    number_result = SUCCEEDED;
    g_parsed_int = mrt->string[mrt->string_current++];
  }
  else {
    number_result = input_number();
    mrt->string_current = 0;
    mrt->string_last = 0;
  }

  if (first == YES)
    mrt->offset = 0;
  else
    mrt->offset++;

  if (number_result == INPUT_NUMBER_EOL && first == NO) {
    next_line();
    return SUCCEEDED;
  }

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
  mrt->argument_data[0]->type = number_result;

  if (number_result == FAILED)
    return FAILED;
  else if (number_result == INPUT_NUMBER_EOL) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", name);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  mrt->supplied_arguments = 2;

  if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
    strcpy(mrt->argument_data[0]->string, g_label);
  else if (number_result == INPUT_NUMBER_STRING) {
    mrt->argument_data[0]->type = SUCCEEDED;
    mrt->argument_data[0]->value = g_label[0];
    strcpy(mrt->string, g_label);
    mrt->string_current = 1;
    mrt->string_last = (int)strlen(g_label);
    /*
      fprintf(stderr, "got string %s!\n", label);
    */
  }
  else if (number_result == INPUT_NUMBER_STACK)
    mrt->argument_data[0]->value = (double)g_latest_stack;
  else if (number_result == SUCCEEDED)
    mrt->argument_data[0]->value = g_parsed_int;
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

  mrt = &g_macro_stack[g_macro_active];

  if (first == YES)
    mrt->incbin_data = incbin_data;
  else
    incbin_data = mrt->incbin_data;

  if (incbin_data->left == 0) {
    /* free the incbin_data structure! it seems we came here from .ENDM and just ran out of data...
       NOTE: don't free mrt->incbin_data->data as it's a copied pointer */
    free(mrt->incbin_data);
    mrt->incbin_data = NULL;
    return SUCCEEDED;
  }

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
  mrt->argument_data[0]->start = g_source_pointer;
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
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -128 || d->value > 255) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 8-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "d%d ", (int)d->value);
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "c%d ", (int)d->value);
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int macro_insert_word_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -32768 || d->value > 65535) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 16-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "y%d ", (int)d->value);
    /*
      fprintf(stderr, ".DWM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "C%d ", (int)d->value);
    /*
      fprintf(stderr, ".DWM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int macro_insert_long_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -8388608 || d->value > 16777215) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 24-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "z%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "T%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int macro_insert_double_dw(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -2147483648.0 || d->value > 2147483647.0) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 32-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "u%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "U%d ", (int)d->value);
    /*
      fprintf(stderr, ".DLM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


struct structure* get_structure(char *name) {

  struct structure *s = g_structures_first;

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
  
  if (g_verbose_mode == ON)
    printf("Pass 1...\n");

  reset_label_stack();

  /* mark all slots as empty */
  for (q = 0; q < 256; q++) {
    g_slots[q].size = 0;
    g_slots[q].name[0] = 0;
  }

  /* start from the very first character, this is the index to the source file we are about to parse... */
  g_source_pointer = 0;

  /* BANK 0 SLOT 0 ORG 0 */
  if (g_output_format != OUTPUT_LIBRARY)
    fprintf(g_file_out_ptr, "B%d %d O%d", 0, 0, 0);

  while (1) {
    q = get_next_token();
    if (q != SUCCEEDED)
      break;
    
    q = evaluate_token();

    if (q == SUCCEEDED)
      continue;
    else if (q == EVALUATE_TOKEN_EOP)
      return SUCCEEDED;
    else if (q == EVALUATE_TOKEN_NOT_IDENTIFIED) {
      /* check if it is of the form "LABEL:XYZ" */
      for (q = 0; q < g_ss; q++) {
        if (g_tmp[q] == ':')
          break;
        if (g_tmp[q] == '=')
          break;
      }

      /* is it a macro? */
      if (q == g_ss) {
        if (macro_get(g_tmp, YES, &m) == FAILED)
          return FAILED;
        if (m == NULL) {
          if (macro_get(g_tmp, NO, &m) == FAILED)
            return FAILED;
        }
      }

      /* it is a label after all? */
      if (q != g_ss || (g_newline_beginning == ON && m == NULL)) {
        char old_tmp_q = g_tmp[q];
    
        g_tmp[q] = 0;

        /* reset the flag as there can be only one label / line */
        g_newline_beginning = OFF;

        if (compare_next_token("=") == SUCCEEDED || old_tmp_q == '=') {
          /* it's actually a definition! */
          g_source_pointer -= g_ss;

          if (directive_define_def_equ() == FAILED)
            return FAILED;
        }
        else {
          if (g_output_format == OUTPUT_LIBRARY && g_section_status == OFF) {
            print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
            return FAILED;
          }
          if (g_org_defined == 0) {
            snprintf(g_error_message, sizeof(g_error_message), "\"%s\" needs a position in memory.\n", g_tmp);
            print_error(g_error_message, ERROR_LOG);
            return FAILED;
          }
          if (g_ss >= MAX_NAME_LENGTH) {
            snprintf(g_error_message, sizeof(g_error_message), "The label \"%s\" is too long. Max label length is %d characters.\n", g_tmp, MAX_NAME_LENGTH);
            print_error(g_error_message, ERROR_NONE);
            return FAILED;
          }
          if (g_bankheader_status == ON) {
            print_error("BANKHEADER sections don't take labels.\n", ERROR_LOG);
            return FAILED;
          }

          /* check out for \@-symbols */
          if (g_macro_active != 0 && q >= 2) {
            if (g_tmp[q - 2] == '\\' && g_tmp[q - 1] == '@')
              snprintf(&g_tmp[q - 2], sizeof(g_tmp) - (q - 2), "%d", g_macro_runtime_current->macro->calls - 1);
          }

          add_label_to_label_stack(g_tmp);
          fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, g_tmp);

          /* move to the end of the label */
          if (q != g_ss)
            g_source_pointer -= g_ss - q - 1;
        }
    
        continue;
      }
      else if (compare_next_token("=") == SUCCEEDED) {
        /* it's actually a definition! */
        g_source_pointer -= g_ss;

        if (directive_define_def_equ() == FAILED)
          return FAILED;

        continue;
      }
      
      if (m == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Unknown symbol \"%s\".\n", g_tmp);
        print_error(g_error_message, ERROR_ERR);
        return FAILED;
      }

      /* start running a macro... run until .ENDM */
      if (macro_stack_grow() == FAILED)
        return FAILED;

      mrt = &g_macro_stack[g_macro_active];
      mrt->argument_data = NULL;
      mrt->incbin_data = NULL;

      /* collect macro arguments */
      for (p = 0; 1; p++) {
        /* take away the white space */
        while (1) {
          if (g_buffer[g_source_pointer] == ' ' || g_buffer[g_source_pointer] == ',')
            g_source_pointer++;
          else
            break;
        }

        o = g_source_pointer;
        g_input_allow_leading_hashtag = YES;
        g_input_allow_leading_ampersand = YES;
        q = input_number();
        g_input_allow_leading_hashtag = NO;
        g_input_allow_leading_ampersand = NO;
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
        mrt->argument_data[p]->has_leading_hashtag = g_input_has_leading_hashtag;

        if (q == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(mrt->argument_data[p]->string, g_label);
        else if (q == INPUT_NUMBER_STRING)
          strcpy(mrt->argument_data[p]->string, g_label);
        else if (q == INPUT_NUMBER_STACK)
          mrt->argument_data[p]->value = (double)g_latest_stack;
        else if (q == SUCCEEDED) {
          mrt->argument_data[p]->value = g_parsed_double;
        }
        else
          return FAILED;

        /* do we have a name for this argument? */
        if (p < m->nargument_names) {
          if (q == INPUT_NUMBER_ADDRESS_LABEL)
            redefine(m->argument_names[p], 0.0, g_label, DEFINITION_TYPE_ADDRESS_LABEL, (int)strlen(g_label));
          else if (q == INPUT_NUMBER_STRING)
            redefine(m->argument_names[p], 0.0, g_label, DEFINITION_TYPE_STRING, (int)strlen(g_label));
          else if (q == INPUT_NUMBER_STACK)
            redefine(m->argument_names[p], (double)g_latest_stack, NULL, DEFINITION_TYPE_STACK, 0);
          else if (q == SUCCEEDED)
            redefine(m->argument_names[p], g_parsed_double, NULL, DEFINITION_TYPE_VALUE, 0);
        }
      }

      next_line();

      mrt->supplied_arguments = p;
      if (macro_start(m, mrt, MACRO_CALLER_NORMAL, p) == FAILED)
        return FAILED;

      continue;
    }
    else if (q == FAILED) {
      snprintf(g_error_message, sizeof(g_error_message), "Couldn't parse \"%s\".\n", g_tmp);
      print_error(g_error_message, ERROR_ERR);
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

  vfprintf(g_file_out_ptr, format, ap);
#ifdef WLA_DEBUG
  {
    char ttt[256];

    va_end(ap);
    va_start(ap, format);
    vsnprintf(ttt, sizeof(ttt), format, ap);
    printf("LINE %5d: OPCODE: %16s ::: %s\n", g_active_file_info_last->line_current, oc->op, ttt);
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
    y = g_source_pointer;
    g_source_pointer = g_inz;
    prev_i = g_source_pointer;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;

    if (z == INPUT_NUMBER_EOL) {
      /* roll back to the index before EOL */
      g_inz = prev_i;
      break;
    }
    
    if (z != INPUT_NUMBER_ADDRESS_LABEL) {
      if (accept_u == 1) {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got something else instead.\n", error_no_s);
        print_error(g_error_message, ERROR_ERR);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got something else instead.\n", error_no_u);
        print_error(g_error_message, ERROR_ERR);
      }
      return -1;
    }

    if (strcaselesscmp(g_label, "X") == 0)
      reg = 1 << 4;
    else if (strcaselesscmp(g_label, "Y") == 0)
      reg = 1 << 5;
    else if (strcaselesscmp(g_label, "U") == 0) {
      if (accept_u == 0) {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got \"%s\" instead.\n", error_no_u, g_label);
        print_error(g_error_message, ERROR_ERR);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(g_label, "S") == 0) {
      if (accept_u == 1) {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got \"%s\" instead.\n", error_no_s, g_label);
        print_error(g_error_message, ERROR_ERR);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(g_label, "PC") == 0)
      reg = 1 << 7;
    else if (strcaselesscmp(g_label, "A") == 0)
      reg = 1 << 1;
    else if (strcaselesscmp(g_label, "B") == 0)
      reg = 1 << 2;
    else if (strcaselesscmp(g_label, "CC") == 0)
      reg = 1 << 0;
    else if (strcaselesscmp(g_label, "DP") == 0)
      reg = 1 << 3;
    else {
      if (accept_u == 1) {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got \"%s\" instead.\n", error_no_s, g_label);
        print_error(g_error_message, ERROR_ERR);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), "%s, got \"%s\" instead.\n", error_no_u, g_label);
        print_error(g_error_message, ERROR_ERR);
      }
      return -1;
    }

    if ((register_byte & reg) != 0) {
      snprintf(g_error_message, sizeof(g_error_message), "Register \"%s\" was already defined.\n", g_label);
      print_error(g_error_message, ERROR_ERR);
      return -1;
    }

    register_byte |= reg;
  }

  if (register_byte == 0) {
    if (accept_u == 1)
      snprintf(g_error_message, sizeof(g_error_message), "%s, got nothing instead.\n", error_no_s);
    else
      snprintf(g_error_message, sizeof(g_error_message), "%s, got nothing instead.\n", error_no_u);
    print_error(g_error_message, ERROR_ERR);

    return -1;
  }
  
  return register_byte;
}


static int get_register_byte_from_label_exg_tfr(void) {

  /* 16-bit */
  if (strcaselesscmp(g_label, "D") == 0)
    return 0;
  if (strcaselesscmp(g_label, "X") == 0)
    return 1;
  if (strcaselesscmp(g_label, "Y") == 0)
    return 2;
  if (strcaselesscmp(g_label, "U") == 0)
    return 3;
  if (strcaselesscmp(g_label, "S") == 0)
    return 4;
  if (strcaselesscmp(g_label, "PC") == 0)
    return 5;

  /* 8-bit */
  if (strcaselesscmp(g_label, "A") == 0)
    return 8;
  if (strcaselesscmp(g_label, "B") == 0)
    return 9;
  if (strcaselesscmp(g_label, "CC") == 0)
    return 0xA;
  if (strcaselesscmp(g_label, "DP") == 0)
    return 0xB;

  snprintf(g_error_message, sizeof(g_error_message), "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got \"%s\" instead.\n", g_label);
  print_error(g_error_message, ERROR_ERR);

  return -1;
}


static int parse_exg_tfr_registers(void) {

  int register_byte = 0, data = 0, y, z;

  /* source register */
  y = g_source_pointer;
  g_source_pointer = g_inz;
  z = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error("Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n", ERROR_ERR);
    return -1;
  }

  data = get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;
  register_byte = data;

  /* destination register */
  y = g_source_pointer;
  g_source_pointer = g_inz;
  z = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = y;

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


#ifdef SUPERFX

/* parse a number [min, max] */
int parse_tiny_int(int min, int max) {

  int old_s, value, res;
  
  old_s = g_source_pointer;
  g_source_pointer = g_inz;
  res = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = old_s;

  if (res != SUCCEEDED)
    return -1;

  value = g_parsed_int;

  if (value < min || value > max)
    return -1;
  
  return value;
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
#ifdef SUPERFX
  int tiny;
#endif
  
  /* are we in an enum, ramsection, or struct? */
  if (g_in_enum == YES || g_in_ramsection == YES || g_in_struct == YES)
    return parse_enum_token();

  /* is it a directive? */
  if (g_tmp[0] == '.') {
    x = parse_directive();
    if (x != DIRECTIVE_NOT_IDENTIFIED)
      return x;

    /* allow error messages from input_numbers() */
    g_input_number_error_msg = YES;

    return EVALUATE_TOKEN_NOT_IDENTIFIED;
  }

  /* is it a label? */
  if (g_tmp[g_ss - 1] == ':' && g_newline_beginning == ON) {
    g_tmp[g_ss - 1] = 0;
    g_newline_beginning = OFF;

    if (g_output_format == OUTPUT_LIBRARY && g_section_status == OFF) {
      print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
      return FAILED;
    }
    if (g_org_defined == 0) {
      snprintf(g_error_message, sizeof(g_error_message), "\"%s\" needs a position in memory.\n", g_tmp);
      print_error(g_error_message, ERROR_LOG);
      return FAILED;
    }
    if (g_ss >= MAX_NAME_LENGTH) {
      snprintf(g_error_message, sizeof(g_error_message), "The label \"%s\" is too long. Max label length is %d characters.\n", g_tmp, MAX_NAME_LENGTH);
      print_error(g_error_message, ERROR_NONE);
      return FAILED;
    }
    if (g_bankheader_status == ON) {
      print_error("BANKHEADER sections don't take labels.\n", ERROR_LOG);
      return FAILED;
    }

    /* check for \@-symbols */
    if (g_macro_active != 0) {
      if (g_tmp[g_ss - 3] == '\\' && g_tmp[g_ss - 2] == '@')
        snprintf(&g_tmp[g_ss - 3], sizeof(g_tmp) - (g_ss - 3), "%d", g_macro_runtime_current->macro->calls - 1);
    }

    add_label_to_label_stack(g_tmp);
    fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, g_tmp);

    return SUCCEEDED;
  }

  /* OPCODE? */
  {
    int op_id = g_tmp[0];

    if (op_id < 0) {
      print_error("Invalid value\n", ERROR_LOG);
      return FAILED;
    }

    g_ind = g_opcode_p[op_id];
  }

  g_opt_tmp = &g_opcodes_table[g_ind];

  for (f = g_opcode_n[(unsigned int)g_tmp[0]]; f > 0; f--) {
#if W65816
    if (g_use_wdc_standard == 0) {
      /* skip all mnemonics that contain '<', '|' and '>' */
      for (g_inz = 0, g_parsed_int = SUCCEEDED; g_inz < OP_SIZE_MAX; g_inz++) {
        char c = g_opt_tmp->op[g_inz];

        if (c == 0)
          break;
        if (c == '<' || c == '|' || c == '>') {
          g_parsed_int = FAILED;
          break;
        }
      }

      if (g_parsed_int == FAILED) {
        /* try the next mnemonic in the array */
        g_opt_tmp = &g_opcodes_table[++g_ind];
        continue;
      }
    }
#endif
    
    /* try to match the first part of the mnemonic, already read into g_tmp */
    for (g_inz = 0, g_parsed_int = SUCCEEDED; g_inz < OP_SIZE_MAX; g_inz++) {
      if (g_tmp[g_inz] == 0)
        break;
      if (g_opt_tmp->op[g_inz] != toupper((int)g_tmp[g_inz])) {
        g_parsed_int = FAILED;
        break;
      }
    }

    if (g_parsed_int == FAILED) {
      /* try the next mnemonic in the array */
      g_opt_tmp = &g_opcodes_table[++g_ind];
      continue;
    }

    /* beginning matches the input */
    x = g_inz;
    g_inz = g_source_pointer;

#ifndef GB
    /* no stack rollback */
    g_stack_inserted = STACK_NONE;
#endif

    switch (g_opt_tmp->type) {

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
#ifdef SUPERFX
#include "decode_superfx.c"
#endif
      
    }

#ifndef GB
    /* perform stack rollback? */
    if (g_stack_inserted != STACK_NONE) {

      struct stack *s;

      if (g_stack_inserted == STACK_OUTSIDE) {
        if (g_stacks_outside == 1) {
          g_stacks_outside = 0;
          delete_stack(g_stacks_first);
          g_stacks_first = NULL;
          g_stacks_last = NULL;
        }
        else {
          s = g_stacks_first;
          g_stacks_outside--;

          for (y = 0; y < g_stacks_outside - 1; y++)
            s = s->next;

          delete_stack(s->next);
          s->next = NULL;
          g_stacks_last = s;
        }
      }
      else {
        if (g_stacks_inside == 1) {
          g_stacks_inside = 0;
          delete_stack(g_stacks_header_first);
          g_stacks_header_first = NULL;
          g_stacks_header_last = NULL;
        }
        else {
          s = g_stacks_header_first;
          g_stacks_inside--;

          for (y = 0; y < g_stacks_inside - 1; y++)
            s = s->next;

          delete_stack(s->next);
          s->next = NULL;
          g_stacks_header_last = s;
        }
      }
    }
#endif

    g_opt_tmp = &g_opcodes_table[++g_ind];
  }

  /* allow error messages from input_numbers() */
  g_input_number_error_msg = YES;

  return EVALUATE_TOKEN_NOT_IDENTIFIED;
}


int redefine(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  
  hashmap_get(g_defines_map, name, (void*)&d);
  
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
  
  if (hashmap_get(g_defines_map, name, (void*)&d) != MAP_OK)
    return FAILED;

  hashmap_remove(g_defines_map, name);

  free(d);

  return SUCCEEDED;
}


int add_a_new_definition(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  int err;

  /* we skip definitions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
  if (strcmp(".", name) == 0 || strcmp("_sizeof_.", name) == 0)
    return SUCCEEDED;

  hashmap_get(g_defines_map, name, (void*)&d);
  if (d != NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "\"%s\" was defined for the second time.\n", name);
    if (g_commandline_parsing == OFF)
      print_error(g_error_message, ERROR_DIR);
    else
      fprintf(stderr, "ADD_A_NEW_DEFINITION: %s", g_error_message);
    return FAILED;
  }

  d = calloc(sizeof(struct definition), 1);
  if (d == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Out of memory while trying to add a new definition (\"%s\").\n", name);
    if (g_commandline_parsing == OFF)
      print_error(g_error_message, ERROR_DIR);
    else
      fprintf(stderr, "ADD_A_NEW_DEFINITION: %s", g_error_message);
    return FAILED;
  }

  strcpy(d->alias, name);
  d->type = type;

  if ((err = hashmap_put(g_defines_map, d->alias, d)) != MAP_OK) {
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
  
#if defined(AMIGA)
  {
    int j;
    
    /* on Amiga we need to remove all instances of ".." (back one dir) as they are not needed */
    for (i = 0, j = 0; path[i] != 0; i++, j++) {
      if (path[i] == '.' && path[i+1] == '.') {
        i++;
        j--;
      }
      else
        path[j] = path[i];
    }

    path[j] = 0;
  }
#endif

  return SUCCEEDED;
}


int verify_name_length(char *name) {

  if (strlen(name) > MAX_NAME_LENGTH) {
    snprintf(g_error_message, sizeof(g_error_message), "The label \"%s\" is too long. Max label length is %d bytes.\n", name, MAX_NAME_LENGTH);
    print_error(g_error_message, ERROR_NONE);
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
  char error_fai[] = "FAIL:";
  char *t = NULL;

  switch (type) {
  case ERROR_FAI:
    t = error_fai;
    break;
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
    fprintf(stderr, "%s:%d: %s", get_file_name(g_active_file_info_last->filename_id), g_active_file_info_last->line_current, error);
    return;
  }

  fprintf(stderr, "%s:%d: %s %s", get_file_name(g_active_file_info_last->filename_id), g_active_file_info_last->line_current, t, error);
  fflush(stderr);

  return;
}


#if defined(W65816)

void give_snes_rom_mode_defined_error(char *prior) {

  if (g_lorom_defined != 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".LOROM was defined prior to %s.\n", prior);
    print_error(g_error_message, ERROR_DIR);
  }
  else if (g_hirom_defined != 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".HIROM was defined prior to %s.\n", prior);
    print_error(g_error_message, ERROR_DIR);
  }
  else if (g_exlorom_defined != 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".EXLOROM was defined prior to %s.\n", prior);
    print_error(g_error_message, ERROR_DIR);
  }
  else if (g_exhirom_defined != 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".EXHIROM was defined prior to %s.\n", prior);
    print_error(g_error_message, ERROR_DIR);
  }
}

#endif


void next_line(void) {

  g_newline_beginning = ON;

  if (g_line_count_status == OFF)
    return;

  if (g_active_file_info_last == NULL)
    return;

  /* output the file number for list file structure building */
  if (g_listfile_data == YES)
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  if (g_active_file_info_last != NULL)
    g_active_file_info_last->line_current++;
}


/* used by .RAMSECTIONs only */
int add_label_sizeof(char *label, int g_size) {

  struct label_sizeof *ls;
  char tmpname[MAX_NAME_LENGTH + 8];

  if (g_create_sizeof_definitions == NO)
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
  ls->size = g_size;
  ls->next = g_label_sizeofs;
  g_label_sizeofs = ls;

  /* define locally also, since we can */
  snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", label);
  if (add_a_new_definition(tmpname, (double)g_size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


/* g_enum_offset and g_last_enum_offset should be set when calling this. */
int add_label_to_enum_or_ramsection(char *name, int size) {

  char tmp[MAX_NAME_LENGTH+10];

  /* there are two passes done when adding a temporary struct to an enum/ramsection. first
     pass is to add the labels, second is to add sizeof definitions. if done in only one
     pass, the resulting sym file is very ugly... */
  if (g_enum_sizeof_pass == NO) {
    if (verify_name_length(name) == FAILED)
      return FAILED;

    if (g_in_enum || g_in_struct) {
      if (add_a_new_definition(name, (double)(g_base_enum_offset+g_enum_offset), NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
      if (g_enum_exp == YES)
        if (export_a_definition(name) == FAILED)
          return FAILED;
    }
    else if (g_in_ramsection) {
      if (g_last_enum_offset != g_enum_offset) {
        /* this sometimes abuses the "dsb" implementation to move backwards in the ramsection. */
        fprintf(g_file_out_ptr, "x%d 0 ", g_enum_offset-g_last_enum_offset);
      }
      fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
      /* we skip label emissions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
      if (strcmp(".", name) != 0)
        fprintf(g_file_out_ptr, "L%s ", name);
    }
  }
  else { /* sizeof pass */
    if (g_in_ramsection) {
      if (add_label_sizeof(name, size) == FAILED)
        return FAILED;
    }
    else {
      if (g_create_sizeof_definitions == YES) {
        snprintf(tmp, sizeof(tmp), "_sizeof_%s", name);
        if (add_a_new_definition(tmp, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;
        if (g_in_enum == YES && g_enum_exp == YES) {
          if (export_a_definition(tmp) == FAILED)
            return FAILED;
        }
      }
    }
  }

  g_last_enum_offset = g_enum_offset;

  return SUCCEEDED;
}


/* add all fields from a struct at the current offset in the enum/ramsection.
   this is used to construct enums or ramsections through temporary structs, even if
   INSTANCEOF isn't used. g_enum_sizeof_pass should be set to YES or NO before calling. */
int enum_add_struct_fields(char *basename, struct structure *st, int reverse) {

  char tmp[MAX_NAME_LENGTH * 2 + 5];
  struct structure_item *si;

  if (strlen(basename) > MAX_NAME_LENGTH) {
    snprintf(g_error_message, sizeof(g_error_message), "Name \"%s\" is too long!\n", basename);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  si = st->items;
  while (si != NULL) {
    int real_si_size = si->size;
    if (si->type == STRUCTURE_ITEM_TYPE_DOTTED)
      real_si_size = 0;

    if (reverse)
      g_enum_offset -= real_si_size;

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
    else
      snprintf(tmp, sizeof(tmp), "%s", basename);
    
    /* if this struct has an .instanceof in it, we need to recurse */
    if (si->type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
      int g;

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
      int orig_offset = g_enum_offset;
      char union_basename[MAX_NAME_LENGTH * 2 + 5];
      struct structure *un = si->union_items;

      while (un != NULL) {
        g_enum_offset = orig_offset;

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
      g_enum_offset = orig_offset + real_si_size;
    }
    else
      g_enum_offset += real_si_size;

    if (reverse) /* after defining data, go back to start, for DESC enums */
      g_enum_offset -= real_si_size;

    si = si->next;
  }

  return SUCCEEDED;
}


static void _remember_new_structure(struct structure *st) {

  int i;

  /* do we already remember the structure? */
  for (i = 0; i < g_saved_structures_count; i++) {
    if (g_saved_structures[i] == st)
      return;
  }

  if (g_saved_structures_count >= g_saved_structures_max) {
    g_saved_structures_max += 256;
    g_saved_structures = realloc(g_saved_structures, sizeof(struct structure *) * g_saved_structures_max);
    if (g_saved_structures == NULL) {
      fprintf(stderr, "_remember_new_structure(): Out of memory error.\n");
      return;
    }
  }
  
  g_saved_structures[g_saved_structures_count++] = st;
}



/* either "g_in_enum", "g_in_ramsection", or "g_in_struct" should be true when this is called. */
int parse_enum_token(void) {

  struct structure *st = NULL;
  struct structure_item *si;
  char tmpname[MAX_NAME_LENGTH + 8 + 1], bak[256];
  int type, g_size, q, start_from = 1;
  
  /* check for "if" directives (the only directives permitted in an enum/ramsection) */
  if (g_tmp[0] == '.') {
    if ((q = parse_if_directive()) != -1) {
      return q;
    }
  }

  if (strcaselesscmp(g_tmp, ".UNION") == 0) {
    struct union_stack *ust;
    int inz;

    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    st->items = NULL;
    st->last_item = NULL;
    st->alive = YES;
    _remember_new_structure(st);
  
    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, g_tmp);
    else
      st->name[0] = '\0';

    /* put previous union onto the "stack" */
    ust = calloc(sizeof(struct union_stack), 1);
    if (ust == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      free(st);
      return FAILED;
    }
    ust->active_struct = g_active_struct;
    ust->union_first_struct = g_union_first_struct;
    ust->union_base_offset = g_union_base_offset;
    ust->max_enum_offset = g_max_enum_offset;
    ust->prev = g_union_stack;
    g_union_stack = ust;

    g_active_struct = st;
    g_union_first_struct = g_active_struct;
    g_union_base_offset = g_enum_offset;
    g_max_enum_offset = g_union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(g_tmp, ".NEXTU") == 0) {
    int inz;

    if (g_union_stack == NULL) {
      print_error("There is no open .UNION.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_enum_offset > g_max_enum_offset)
      g_max_enum_offset = g_enum_offset;
    g_active_struct->size = g_enum_offset - g_union_base_offset;
    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error("PARSE_ENUM_TOKEN: Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    st->items = NULL;
    st->last_item = NULL;
    st->alive = YES;
    _remember_new_structure(st);
    
    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, g_tmp);
    else
      st->name[0] = '\0';

    g_active_struct->next = st;
    g_active_struct = st;
    g_enum_offset = g_union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(g_tmp, ".ENDU") == 0) {
    struct union_stack *ust;
    int total_size;

    if (g_union_stack == NULL) {
      print_error("There is no open .UNION.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_enum_offset > g_max_enum_offset)
      g_max_enum_offset = g_enum_offset;

    total_size = g_max_enum_offset - g_union_base_offset;

    g_active_struct->size = g_enum_offset - g_union_base_offset;
    g_active_struct->next = NULL;

    st = g_union_first_struct;

    g_enum_offset = g_max_enum_offset;

    /* pop previous union from the "stack" */
    ust = g_union_stack;
    g_active_struct = g_union_stack->active_struct;
    g_union_first_struct = g_union_stack->union_first_struct;
    g_union_base_offset = ust->union_base_offset;
    g_max_enum_offset = ust->max_enum_offset;
    g_union_stack = g_union_stack->prev;
    free(ust);

    /* just popped g_max_enum_offset; need to update it for end of union */
    if (g_enum_offset > g_max_enum_offset)
      g_max_enum_offset = g_enum_offset;

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
    if (g_active_struct->items == NULL)
      g_active_struct->items = si;
    if (g_active_struct->last_item != NULL)
      g_active_struct->last_item->next = si;
    g_active_struct->last_item = si;
    return SUCCEEDED;
  }
  else if (g_in_enum == YES && strcaselesscmp(g_tmp, ".ENDE") == 0) {
    if (g_union_stack != NULL) {
      print_error(".UNION not closed.\n", ERROR_DIR);
      return FAILED;
    }
    
    g_enum_offset = 0;
    g_enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", g_active_struct, (g_enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    g_enum_offset = 0;
    g_enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", g_active_struct, (g_enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    g_active_struct = NULL;

    g_in_enum = NO;
    return SUCCEEDED;
  }
  else if (g_in_ramsection == YES && strcaselesscmp(g_tmp, ".ENDS") == 0) {
    if (g_union_stack != NULL) {
      print_error(".UNION not closed.\n", ERROR_DIR);
      return FAILED;
    }

    g_enum_offset = 0;
    g_last_enum_offset = 0;
    g_enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", g_active_struct, 0) == FAILED)
      return FAILED;

    g_enum_offset = 0;
    g_last_enum_offset = 0;
    g_enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", g_active_struct, 0) == FAILED)
      return FAILED;

    if (g_max_enum_offset > g_last_enum_offset)
      fprintf(g_file_out_ptr, "o%d 0 ", g_max_enum_offset-g_last_enum_offset);

    /* generate a section end label? */
    if (g_extra_definitions == ON)
      generate_label("SECTIONEND_", g_sections_last->name);
      
    g_active_struct = NULL;

    fprintf(g_file_out_ptr, "s ");
    g_section_status = OFF;
    g_in_ramsection = NO;

    return SUCCEEDED;
  }
  else if (g_in_struct && strcaselesscmp(g_tmp, ".ENDST") == 0) {
    g_enum_offset = 0;
    g_last_enum_offset = 0;
    g_enum_sizeof_pass = NO;
    if (enum_add_struct_fields(g_active_struct->name, g_active_struct, 0) == FAILED)
      return FAILED;

    g_enum_offset = 0;
    g_last_enum_offset = 0;
    g_enum_sizeof_pass = YES;
    if (enum_add_struct_fields(g_active_struct->name, g_active_struct, 0) == FAILED)
      return FAILED;
    
    /* create the SIZEOF-definition for the entire struct */
    g_active_struct->size = g_max_enum_offset;

    if (g_create_sizeof_definitions == YES) {
      if (strlen(g_active_struct->name) > MAX_NAME_LENGTH - 8) {
        snprintf(g_error_message, sizeof(g_error_message), ".STRUCT \"%s\"'s name is too long!\n", g_active_struct->name);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", g_active_struct->name);
      if (add_a_new_definition(tmpname, (double)g_active_struct->size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
    }
    
    if (g_active_struct->items == NULL) {
      snprintf(g_error_message, sizeof(g_error_message), ".STRUCT \"%s\" is empty!\n", g_active_struct->name);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    g_active_struct->next = g_structures_first;
    g_structures_first = g_active_struct;

    g_in_struct = NO;
    g_active_struct = NULL;

    return SUCCEEDED;
  }

  if (strcaselesscmp(g_tmp, "INSTANCEOF") == 0) {
    /* anonymous instance! */
    tmpname[0] = 0;
  }
  else {
    if (g_tmp[strlen(g_tmp) - 1] == ':')
      g_tmp[strlen(g_tmp) - 1] = 0;
    strcpy(tmpname, g_tmp);

    /* get the size/type */
    if (get_next_token() == FAILED)
      return FAILED;
  }
  
  type = 0;
  g_size = 0;

  if (strcaselesscmp(g_tmp, "DB") == 0 || strcaselesscmp(g_tmp, "BYT") == 0 || strcaselesscmp(g_tmp, "BYTE") == 0) {
    g_size = 1;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DW") == 0 || strcaselesscmp(g_tmp, "WORD") == 0 || strcaselesscmp(g_tmp, "ADDR") == 0) {
    g_size = 2;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DL") == 0 || strcaselesscmp(g_tmp, "LONG") == 0 || strcaselesscmp(g_tmp, "FARADDR") == 0) {
    g_size = 3;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DD") == 0) {
    g_size = 4;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DS") == 0 || strcaselesscmp(g_tmp, "DSB") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DS/DSB needs size.\n", ERROR_DIR);
      return FAILED;
    }
    g_size = g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSW") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSW needs size.\n", ERROR_DIR);
      return FAILED;
    }
    g_size = 2*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSL") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSL needs size.\n", ERROR_DIR);
      return FAILED;
    }
    g_size = 3*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSD") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSD needs size.\n", ERROR_DIR);
      return FAILED;
    }
    g_size = 4*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  /* it's an instance of a structure! */
  else if (strcaselesscmp(g_tmp, "INSTANCEOF") == 0) {
    int number_result;
    type = STRUCTURE_ITEM_TYPE_INSTANCEOF;

    if (get_next_token() == FAILED)
      return FAILED;

    st = get_structure(g_tmp);

    if (st == NULL) {
      snprintf(g_error_message, sizeof(g_error_message), "No STRUCT named \"%s\" available.\n", g_tmp);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    /* get the number of structures to be made */
    number_result = input_number();
    if (number_result == INPUT_NUMBER_EOL) {
      next_line();
      g_size = st->size;
      g_parsed_int = 1;
    }
    else if (number_result == SUCCEEDED) {
      if (g_parsed_int < 1) {
        print_error("The number of structures must be greater than 0.\n", ERROR_DIR);
        return FAILED;
      }

      g_size = st->size * g_parsed_int;
    }
    else {
      if (number_result == INPUT_NUMBER_STRING)
        snprintf(g_error_message, sizeof(g_error_message), "Expected the number of structures, got \"%s\" instead.\n", g_label);
      else
        snprintf(g_error_message, sizeof(g_error_message), "Expected the number of structures.\n");
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (compare_next_token("STARTFROM") == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED) {
        if (g_parsed_int < 0) {
          snprintf(g_error_message, sizeof(g_error_message), "STARTFROM needs to be >= 0.\n");
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }
        start_from = g_parsed_int;
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), "STARTFROM needs a number >= 0.\n");
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
  }
  else if (strcaselesscmp(g_tmp, ".db") == 0 || strcaselesscmp(g_tmp, ".byt") == 0 ||
           strcaselesscmp(g_tmp, ".byte") == 0) {
    /* don't do anything for "dotted" versions */
    g_size = 1;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dw") == 0 || strcaselesscmp(g_tmp, ".word") == 0 ||
           strcaselesscmp(g_tmp, ".addr") == 0) {
    /* don't do anything for "dotted" versions */
    g_size = 2;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".ds") == 0 || strcaselesscmp(g_tmp, ".dsb") == 0 || strcaselesscmp(g_tmp, ".dsw") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, g_tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      snprintf(g_error_message, sizeof(g_error_message), "%s needs size.\n", bak);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (strcaselesscmp(bak, ".dsw") == 0)
      g_parsed_int *= 2;

    g_size = g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dl") == 0 || strcaselesscmp(g_tmp, ".long") == 0 || strcaselesscmp(g_tmp, ".faraddr") == 0) {
    /* don't do anything for "dotted" versions */
    g_size = 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dsl") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, g_tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".DSL needs size.\n", ERROR_DIR);
      return FAILED;
    }

    g_size = g_parsed_int * 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dd") == 0) {
    /* don't do anything for "dotted" versions */
    g_size = 4;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dsd") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, g_tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".DSD needs size.\n", ERROR_DIR);
      return FAILED;
    }

    g_size = g_parsed_int * 4;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else {
    if (g_in_enum == YES)
      snprintf(g_error_message, sizeof(g_error_message), "Unexpected symbol \"%s\" in .ENUM.\n", g_tmp);
    else if (g_in_ramsection == YES)
      snprintf(g_error_message, sizeof(g_error_message), "Unexpected symbol \"%s\" in .RAMSECTION.\n", g_tmp);
    else /* struct */
      snprintf(g_error_message, sizeof(g_error_message), "Unexpected symbol \"%s\" in .STRUCT.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
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
  si->size = g_size;
  si->type = type;
  si->start_from = start_from;
  if (type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
    si->instance = st;
    si->num_instances = si->size/st->size;
  }
  else if (type == STRUCTURE_ITEM_TYPE_UNION)
    si->union_items = st;

  if (g_active_struct->items == NULL)
    g_active_struct->items = si;
  if (g_active_struct->last_item != NULL)
    g_active_struct->last_item->next = si;
  g_active_struct->last_item = si;

  if (type != STRUCTURE_ITEM_TYPE_DOTTED)
    g_enum_offset += g_size;

  if (g_enum_offset > g_max_enum_offset)
    g_max_enum_offset = g_enum_offset;

  return SUCCEEDED;
}


int directive_org(void) {

  int q;
  
  no_library_files(".ORG definitions");
    
  if (g_bank_defined == 0) {
    print_error("No .BANK is defined.\n", ERROR_LOG);
    return FAILED;
  }
  if (g_section_status == ON) {
    print_error("You can't issue .ORG inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_dstruct_status == ON) {
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

  g_org_defined = 1;
  fprintf(g_file_out_ptr, "O%d ", g_parsed_int);

  return SUCCEEDED;
}


int directive_orga(void) {

  int q, current_slot_address;
  
  no_library_files(".ORGA definitions");
    
  if (g_bank_defined == 0) {
    print_error("No .BANK is defined.\n", ERROR_LOG);
    return FAILED;
  }
  if (g_section_status == ON) {
    print_error("You can't issue .ORGA inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_dstruct_status == ON) {
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

  g_org_defined = 1;

  current_slot_address = g_slots[g_current_slot].address;
  if (g_parsed_int < current_slot_address || g_parsed_int > (current_slot_address + g_slots[g_current_slot].size)) {
    print_error(".ORGA is outside the current SLOT.\n", ERROR_DIR);
    return FAILED;
  }

  fprintf(g_file_out_ptr, "O%d ", g_parsed_int - current_slot_address);

  return SUCCEEDED;
}


int directive_slot(void) {

  int q;
  
  no_library_files(".SLOT definitions");
    
  if (g_section_status == ON) {
    print_error("You can't issue .SLOT inside a .SECTION.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_dstruct_status == ON) {
    print_error("You can't issue .SLOT inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
    /* turn the label into a number */
    if (_get_slot_number_by_its_name(g_label, &g_parsed_int) == FAILED)
      return FAILED;
    q = SUCCEEDED;
  }
  else if (q == SUCCEEDED) {
    /* is the number a direct SLOT number, or is it an address? */
    q = _get_slot_number_by_a_value(g_parsed_int, &g_parsed_int);
  }
  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error("Cannot find the SLOT.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_parsed_int < 0 || g_parsed_int > 255 || g_slots[g_parsed_int].size == 0) {
    snprintf(g_error_message, sizeof(g_error_message), "There is no SLOT number %d.\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  fprintf(g_file_out_ptr, "B%d %d ", g_bank, g_parsed_int);

  g_current_slot = g_parsed_int;

  return SUCCEEDED;
}


int directive_bank(void) {

  int q, bank, slot;
  
  no_library_files(".BANK definitions");
    
  if (g_section_status == ON) {
    snprintf(g_error_message, sizeof(g_error_message), "Section \"%s\" is open. Do not try to change the bank.\n", g_sections_last->name);
    print_error(g_error_message, ERROR_LOG);
    return FAILED;
  }
  if (g_dstruct_status == ON) {
    print_error("You can't use .BANK inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_rombanks_defined == 0 && g_output_format != OUTPUT_LIBRARY) {
    print_error(".ROMBANKS is not yet defined.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 0) {
    print_error(".BANK number must be zero or positive.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_rombanks <= g_parsed_int && g_output_format != OUTPUT_LIBRARY) {
    snprintf(g_error_message, sizeof(g_error_message), "ROM banks == %d, selected bank %d.\n", g_rombanks, g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  g_bank = g_parsed_int;
  g_bank_defined = 1;

  if (compare_next_token("SLOT") == SUCCEEDED) {
    skip_next_token();

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
      /* turn the label into a number */
      if (_get_slot_number_by_its_name(g_label, &g_parsed_int) == FAILED)
        return FAILED;
      q = SUCCEEDED;
    }
    else if (q == SUCCEEDED) {
      /* is the number a direct SLOT number, or is it an address? */
      q = _get_slot_number_by_a_value(g_parsed_int, &g_parsed_int);
    }
    if (q != SUCCEEDED) {
      print_error("Cannot find the SLOT.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_slots[g_parsed_int].size == 0) {
      snprintf(g_error_message, sizeof(g_error_message), "There is no SLOT number %d.\n", g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (g_output_format != OUTPUT_LIBRARY)
      fprintf(g_file_out_ptr, "B%d %d ", g_bank, g_parsed_int);

    bank = g_bank;
    slot = g_parsed_int;
    g_current_slot = g_parsed_int;
  }
  else {
    fprintf(g_file_out_ptr, "B%d %d ", g_parsed_int, g_defaultslot);
    bank = g_parsed_int;
    slot = g_defaultslot;
    g_current_slot = g_defaultslot;
  }

  if (g_slots[slot].size < g_banks[bank]) {
    snprintf(g_error_message, sizeof(g_error_message), "SLOT %d's size %d < BANK %d's size %d.\n", slot, g_slots[slot].size, bank, g_banks[bank]);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  if (g_slots[slot].size > g_banks[bank]) {
    snprintf(g_error_message, sizeof(g_error_message), "SLOT %d's size %d > BANK %d's size %d, but the bank fits inside.\n", slot, g_slots[slot].size, bank, g_banks[bank]);
    print_error(g_error_message, ERROR_WRN);
  }

  return SUCCEEDED;
}


int directive_dbm_dwm_dlm_ddm_filter(void) {

  struct macro_static *macro;

  if (input_number() != INPUT_NUMBER_ADDRESS_LABEL) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s requires macro name.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* find the macro */
  if (macro_get(g_label, YES, &macro) == FAILED)
    return FAILED;
  if (macro == NULL) {
    if (macro_get(g_label, NO, &macro) == FAILED)
      return FAILED;
  }

  if (macro == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "No MACRO \"%s\" defined.\n", g_label);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (strcaselesscmp(g_current_directive, "DBM") == 0) {
    if (macro_start_dxm(macro, MACRO_CALLER_DBM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DLM") == 0) {
    if (macro_start_dxm(macro, MACRO_CALLER_DLM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DDM") == 0) {
    if (macro_start_dxm(macro, MACRO_CALLER_DDM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DWM") == 0) {
    if (macro_start_dxm(macro, MACRO_CALLER_DWM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "FILTER") == 0) {
    if (macro_start_dxm(macro, MACRO_CALLER_FILTER, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), "Unsupported filter macro directive \"%s\". Please submit a bug report!\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_table(void) {

  char bak[256];
  int result, i;

  result = input_number();
  for (g_table_size = 0; g_table_size < (int)sizeof(g_table_format) && (result == INPUT_NUMBER_STRING || result == INPUT_NUMBER_ADDRESS_LABEL); ) {
    if (strcaselesscmp(g_label, "db") == 0 || strcaselesscmp(g_label, "byte") == 0 || strcaselesscmp(g_label, "byt") == 0) {
      g_table_format[g_table_size++] = 'b';
    }
    else if (strcaselesscmp(g_label, "ds") == 0 || strcaselesscmp(g_label, "dsb") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        snprintf(g_error_message, sizeof(g_error_message), "%s needs size.\n", bak);
        print_error(g_error_message, ERROR_INP);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && g_table_size < (int)sizeof(g_table_format); i++)
        g_table_format[g_table_size++] = 'b';
    }
    else if (strcaselesscmp(g_label, "dw") == 0 || strcaselesscmp(g_label, "word") == 0 || strcaselesscmp(g_label, "addr") == 0) {
      g_table_format[g_table_size++] = 'w';
    }
    else if (strcaselesscmp(g_label, "dsw") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        snprintf(g_error_message, sizeof(g_error_message), "%s needs size.\n", bak);
        print_error(g_error_message, ERROR_INP);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && g_table_size < (int)sizeof(g_table_format); i++)
        g_table_format[g_table_size++] = 'w';
    }
    else if (strcaselesscmp(g_label, "dl") == 0 || strcaselesscmp(g_label, "long") == 0 || strcaselesscmp(g_label, "faraddr") == 0) {
      g_table_format[g_table_size++] = 'l';
    }
    else if (strcaselesscmp(g_label, "dsl") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        snprintf(g_error_message, sizeof(g_error_message), "%s needs size.\n", bak);
        print_error(g_error_message, ERROR_INP);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && g_table_size < (int)sizeof(g_table_format); i++)
        g_table_format[g_table_size++] = 'l';
    }
    else if (strcaselesscmp(g_label, "dd") == 0) {
      g_table_format[g_table_size++] = 'd';
    }
    else if (strcaselesscmp(g_label, "dsd") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        snprintf(g_error_message, sizeof(g_error_message), "%s needs size.\n", bak);
        print_error(g_error_message, ERROR_INP);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && g_table_size < (int)sizeof(g_table_format); i++)
        g_table_format[g_table_size++] = 'd';
    }
    else {
      snprintf(g_error_message, sizeof(g_error_message), "Unknown symbol \"%s\".\n", g_label);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
      
    result = input_number();
  }

  if (g_table_size >= (int)sizeof(g_table_format)) {
    snprintf(g_error_message, sizeof(g_error_message), ".TABLE is out of size.\n");
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (result == FAILED)
    return FAILED;
  else if (result == INPUT_NUMBER_EOL && g_table_size == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".TABLE needs data.\n");
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }
  else if (result == INPUT_NUMBER_EOL)
    next_line();
  else {
    snprintf(g_error_message, sizeof(g_error_message), "Unknown symbol.\n");
    print_error(g_error_message, ERROR_DIR);
    return FAILED;      
  }

  g_table_defined = 1;
  g_table_index = 0;

  return SUCCEEDED;    
}


int directive_row_data(void) {

  char bak[256];
  int rows = 0, result, i;
  
  strcpy(bak, g_current_directive);

  if (g_table_defined == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".TABLE needs to be defined before .%s can be used.\n", bak);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
    
  if (strcaselesscmp(bak, "ROW") == 0) {
    if (g_table_index != 0) {
      snprintf(g_error_message, sizeof(g_error_message), ".ROW cannot be used here. .DATA needs to be used again to give the remaining of the row.\n");
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
  }

  result = input_number();
  i = 0;
  for ( ; result == SUCCEEDED || result == INPUT_NUMBER_STRING || result == INPUT_NUMBER_ADDRESS_LABEL || result == INPUT_NUMBER_STACK; ) {
    if (result == INPUT_NUMBER_STRING) {
      if (g_table_format[g_table_index] == 'b') {
        if (strlen(g_label) != 1) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s was expecting a byte, got %d bytes instead.\n", bak, (int)strlen(g_label));
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "d%d ", g_label[0]);          
      }
      else if (g_table_format[g_table_index] == 'w') {
        if (strlen(g_label) > 2 || strlen(g_label) <= 0) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s was expecting a word (2 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", (g_label[0] << 8) | g_label[1]);
      }
      else if (g_table_format[g_table_index] == 'l') {
        if (strlen(g_label) > 3 || strlen(g_label) <= 0) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s was expecting a long (3 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", (g_label[0] << 16) | (g_label[1] << 8) | g_label[2]);
      }
      else if (g_table_format[g_table_index] == 'd') {
        if (strlen(g_label) > 4 || strlen(g_label) <= 0) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s was expecting a double word (4 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "u%d ", (g_label[0] << 24) | (g_label[1] << 16) | (g_label[2] << 8) | g_label[3]);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, g_table_format[g_table_index]);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
    else if (result == SUCCEEDED) {
      if (g_table_format[g_table_index] == 'b') {
        if (g_parsed_int < -128 || g_parsed_int > 255) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }
    
        fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
      }
      else if (g_table_format[g_table_index] == 'w') {
        if (g_parsed_int < -32768 || g_parsed_int > 65535) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s expects 16-bit data, %d is out of range!\n", bak, g_parsed_int);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", g_parsed_int);
      }
      else if (g_table_format[g_table_index] == 'l') {
        if (g_parsed_int < -8388608 || g_parsed_int > 16777215) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s expects 24-bit data, %d is out of range!\n", bak, g_parsed_int);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", g_parsed_int);
      }
      else if (g_table_format[g_table_index] == 'd') {
        /*
        if (g_parsed_int < -2147483648 || g_parsed_int > 2147483647) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s expects 32-bit data, %d is out of range!\n", bak, g_parsed_int);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }
        */

        fprintf(g_file_out_ptr, "u%d ", g_parsed_int);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, g_table_format[g_table_index]);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
    else if (result == INPUT_NUMBER_ADDRESS_LABEL) {
      if (g_table_format[g_table_index] == 'b') {
        fprintf(g_file_out_ptr, "k%d Q%s ", g_active_file_info_last->line_current, g_label);
      }
      else if (g_table_format[g_table_index] == 'w') {
        fprintf(g_file_out_ptr, "k%d r%s ", g_active_file_info_last->line_current, g_label);
      }
      else if (g_table_format[g_table_index] == 'l') {
        fprintf(g_file_out_ptr, "k%d q%s ", g_active_file_info_last->line_current, g_label);
      }
      else if (g_table_format[g_table_index] == 'd') {
        fprintf(g_file_out_ptr, "k%d V%s ", g_active_file_info_last->line_current, g_label);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, g_table_format[g_table_index]);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
    else if (result == INPUT_NUMBER_STACK) {
      if (g_table_format[g_table_index] == 'b') {
        fprintf(g_file_out_ptr, "c%d ", g_latest_stack);
      }
      else if (g_table_format[g_table_index] == 'w') {
        fprintf(g_file_out_ptr, "C%d ", g_latest_stack);
      }
      else if (g_table_format[g_table_index] == 'l') {
        fprintf(g_file_out_ptr, "T%d ", g_latest_stack);
      }
      else if (g_table_format[g_table_index] == 'd') {
        fprintf(g_file_out_ptr, "U%d ", g_latest_stack);
      }
      else {
        snprintf(g_error_message, sizeof(g_error_message), ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, g_table_format[g_table_index]);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }

    i++;
    g_table_index++;
    if (g_table_index >= g_table_size) {
      rows++;
      g_table_index = 0;
    }

    result = input_number();
  }

  if (result == FAILED)
    return FAILED;

  if (result == INPUT_NUMBER_EOL && i == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (strcaselesscmp(bak, "ROW") == 0) {
    if (g_table_index != 0 || rows != 1) {
      snprintf(g_error_message, sizeof(g_error_message), ".ROW needs exactly one row of data, no more, no less.\n");
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }
  }

  if (result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_db_byt_byte(void) {

  char bak[256];
  int i, char_index, number_result;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_STRING || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    if (number_result == INPUT_NUMBER_STRING) {
      for (char_index = 0; char_index < g_string_size; char_index++)
        fprintf(g_file_out_ptr, "d%d ", (int)g_label[char_index]);
      number_result = input_number();
      continue;
    }

    if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (number_result == SUCCEEDED)
      fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
    else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(g_file_out_ptr, "Q%s ", g_label);
    else if (number_result == INPUT_NUMBER_STACK)
      fprintf(g_file_out_ptr, "c%d ", g_latest_stack);

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if (number_result == INPUT_NUMBER_EOL && i == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


static int _char_to_hex(char e) {

  if (e >= '0' && e <= '9')
    return e - '0';
  else if (e >= 'A' && e <= 'F')
    return e - 'A' + 10;
  else if (e >= 'a' && e <= 'f')
    return e - 'a' + 10;
  else
    return 0xff;
}


int directive_hex(void) {

  int i, o, nybble_1 = 0, nybble_2 = 0, error, number_result;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  number_result = input_number();
  for (i = 0; number_result == INPUT_NUMBER_STRING; i++) {
    if ((g_string_size & 1) == 1) {
      print_error("The string length for .HEX must be a multiple of 2.\n", ERROR_INP);
      return FAILED;
    }
      
    for (o = 0; o < g_string_size; ) {
      error = NO;

      nybble_1 = _char_to_hex(g_label[o]);
      if (nybble_1 == 0xff)
        error = YES;
      else {
        o++;
        nybble_2 = _char_to_hex(g_label[o]);
        if (nybble_2 == 0xff)
          error = YES;
        else
          o++;
      }
        
      if (error == YES) {
        snprintf(g_error_message, sizeof(g_error_message), "'%c' does not belong to a hexadecimal value!\n", g_label[o]);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
        
      fprintf(g_file_out_ptr, "d%d ", (nybble_1 << 4) | nybble_2);
    }

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if (number_result == INPUT_NUMBER_EOL && i == 0) {
    print_error(".HEX needs data.\n", ERROR_INP);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();
  else {
    print_error(".HEX takes only strings.\n", ERROR_INP);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_bits(void) {

  int bits, q;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  q = input_number();

  if (q == FAILED)
    return FAILED;
  else if (q == SUCCEEDED) {
    /* immediate values are accepted */
  }
  else {
    print_error(".BITS needs an immediate value for the number of bits.\n", ERROR_DIR);
    return FAILED;
  }  

  bits = g_parsed_int;

  if (bits < 1 || bits > 32) {
    print_error("The number of bits needs to be between 1 and 32.\n", ERROR_DIR);
    return FAILED;    
  }
    
  /* skip DATA if present */
  if (compare_next_token("DATA") == SUCCEEDED)
    skip_next_token();

  while (1) {
    q = input_number();

    if (q == SUCCEEDED) {
      int mask = 0;

      if (bits < 32)
        mask = ~((1 << bits) - 1);
        
      fprintf(g_file_out_ptr, "+%d ", bits);

      if ((g_parsed_int & mask) != 0) {
        snprintf(g_error_message, sizeof(g_error_message), "We are defining %d bits, but the given value $%x (%d) uses more bits!\n", bits, g_parsed_int, g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      
      fprintf(g_file_out_ptr, "a%d ", g_parsed_int);
    }
    else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
      fprintf(g_file_out_ptr, "+%d ", bits);
      fprintf(g_file_out_ptr, "b%s ", g_label);
    }
    else if (q == INPUT_NUMBER_STACK) {
      fprintf(g_file_out_ptr, "+%d ", bits);
      fprintf(g_file_out_ptr, "c%d ", g_latest_stack);
    }
    else if (q == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else {
      print_error(".BITS data must be immediate values, address labels or calculation stacks.\n", ERROR_DIR);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_asctable_asciitable(void) {
  
  int astart, aend, q, o, token_result;
  char bak[256];
  
  strcpy(bak, g_current_directive);

  /* clear the table (to the default n->n -mapping) */
  for (o = 0; o < 256; o++)
    g_asciitable[o] = o;

  /* read the entries */
  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDA") == 0)
      break;
    else if (strcaselesscmp(g_tmp, "MAP") == 0) {
      q = input_number();

      while (q == INPUT_NUMBER_EOL) {
        next_line();
        q = input_number();
      }

      if (q == FAILED)
        return FAILED;
      if (q == SUCCEEDED && (g_parsed_int < 0 || g_parsed_int > 255)) {
        print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
        return FAILED;
      }
      if (q == INPUT_NUMBER_STRING) {
        if (g_string_size != 1) {
          print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
          return FAILED;
        }
        else {
          g_parsed_int = g_label[0];
          if (g_parsed_int < 0)
            g_parsed_int += 256;
        }
      }

      astart = g_parsed_int;
      aend = g_parsed_int+1;

      /* do we have a range? */
      if (compare_next_token("TO") == SUCCEEDED) {
        skip_next_token();

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q == SUCCEEDED && (g_parsed_int < 0 || g_parsed_int > 255)) {
          print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
          return FAILED;
        }
        if (q == INPUT_NUMBER_STRING) {
          if (g_string_size != 1) {
            print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
            return FAILED;
          }
          else {
            g_parsed_int = g_label[0];
            if (g_parsed_int < 0)
              g_parsed_int += 256;
          }
        }

        aend = g_parsed_int+1;
      }

      if (aend <= astart) {
        print_error("The end address of the mapping must be larger than the staring address.\n", ERROR_DIR);
        return FAILED;
      }

      /* skip the "=" */
      if (compare_next_token("=") != SUCCEEDED) {
        token_result = FAILED;
        break;
      }
      skip_next_token();

      /* read the starting address */
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q == SUCCEEDED && (g_parsed_int < 0 || g_parsed_int > 255)) {
        print_error("The entry must be a positive 8-bit immediate value or one letter string.\n", ERROR_DIR);
        return FAILED;
      }
      if (q != SUCCEEDED) {
        print_error("The entry must be a positive 8-bit immediate value.\n", ERROR_DIR);
        return FAILED;
      }

      /* build the mapping */
      for (o = astart; o < aend; o++) {
        if (g_parsed_int >= 256) {
          print_error("The mapping overflows from the ASCII table!\n", ERROR_DIR);
          return FAILED;
        }
        g_asciitable[o] = g_parsed_int++;
      }
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), "Error in .%s data structure.\n", bak);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  g_asciitable_defined = 1;

  return SUCCEEDED;
}


int directive_asc(void) {

  int o, map_only_strings = NO;
  char bak[256];

  strcpy(bak, g_current_directive);

  if (strcaselesscmp(bak, "ASCSTR") == 0)
    map_only_strings = YES;
  
  if (g_asciitable_defined == 0) {
    print_error("No .ASCIITABLE defined. Using the default n->n -mapping.\n", ERROR_WRN);
    for (o = 0; o < 256; o++)
      g_asciitable[o] = o;
  }

  while (1) {
    int character, q;

    q = input_number();
    if (q == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }

    if (q == INPUT_NUMBER_STRING) {
      /* remap the ascii string */
      for (o = 0; o < g_string_size; o++) {
        character = g_label[o];
        if (character < 0)
          character += 256;
        character = (int)g_asciitable[character];
        fprintf(g_file_out_ptr, "d%d ", character);
      }
    }
    else if (q == SUCCEEDED) {
      if (map_only_strings == YES) {
        /* pass through the byte */
        if (g_parsed_int < -128 || g_parsed_int > 255) {
          snprintf(g_error_message, sizeof(g_error_message), "Expected a byte value between -128 and 255, got %d.\n", g_parsed_int);
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }
        fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
      }
      else {
        /* remap the byte */
        if (g_parsed_int < 0 || g_parsed_int > 255) {
          snprintf(g_error_message, sizeof(g_error_message), ".%s needs string / byte (0-255) data.\n", bak);
          print_error(g_error_message, ERROR_INP);
          return FAILED;
        }
        character = (int)g_asciitable[g_parsed_int];
        fprintf(g_file_out_ptr, "d%d ", character);
      }
    }
    else {
      snprintf(g_error_message, sizeof(g_error_message), ".%s needs string / byte (0-255) data.\n", bak);
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_dw_word_addr(void) {

  int i, number_result;
  char bak[256];

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 16-bit data, %d is out of range!\n", bak, g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (number_result == SUCCEEDED)
      fprintf(g_file_out_ptr, "y%d", g_parsed_int);
    else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(g_file_out_ptr, "r%s ", g_label);
    else if (number_result == INPUT_NUMBER_STACK)
      fprintf(g_file_out_ptr, "C%d ", g_latest_stack);

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if ((number_result == INPUT_NUMBER_EOL || number_result == INPUT_NUMBER_STRING) && i == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_dl_long_faraddr(void) {

  int i, number_result;
  char bak[256];

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    if (number_result == SUCCEEDED && (g_parsed_int < -8388608 || g_parsed_int > 16777215)) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 24-bit data, %d is out of range!\n", bak, g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (number_result == SUCCEEDED)
      fprintf(g_file_out_ptr, "z%d ", g_parsed_int);
    else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(g_file_out_ptr, "q%s ", g_label);
    else if (number_result == INPUT_NUMBER_STACK)
      fprintf(g_file_out_ptr, "T%d ", g_latest_stack);

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if ((number_result == INPUT_NUMBER_EOL || number_result == INPUT_NUMBER_STRING) && i == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_dsl(void) {

  int q, parsed_int;
  
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(".DSL needs size.\n", ERROR_INP);
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSL expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(".DSL needs data.\n", ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int < -8388608 || g_parsed_int > 16777215)) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSL expects 24-bit data, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "h%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "q%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "T%d ", g_latest_stack);
  }

  return SUCCEEDED;
}


int directive_dd(void) {

  int i, number_result;
  char bak[256];

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    /*
    if (number_result == SUCCEEDED && (g_parsed_int < -2147483648 || g_parsed_int > 2147483647)) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s expects 32-bit data, %d is out of range!\n", bak, g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    */

    if (number_result == SUCCEEDED)
      fprintf(g_file_out_ptr, "u%d ", g_parsed_int);
    else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(g_file_out_ptr, "V%s ", g_label);
    else if (number_result == INPUT_NUMBER_STACK)
      fprintf(g_file_out_ptr, "U%d ", g_latest_stack);
    
    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if ((number_result == INPUT_NUMBER_EOL || number_result == INPUT_NUMBER_STRING) && i == 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_dsd(void) {

  int q, parsed_int;
  
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(".DSD needs size.\n", ERROR_INP);
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSD expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(".DSD needs data.\n", ERROR_INP);
    return FAILED;
  }

  /*
  if (q == SUCCEEDED && (g_parsed_int < -2147483648 || g_parsed_int > 2147483647)) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSD expects 32-bit data, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  */

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "w%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "V%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "U%d ", g_latest_stack);
  }

  return SUCCEEDED;
}


#if defined(W65816)

int directive_name_w65816(void) {

  int token_result;

  no_library_files(".NAME");

  if ((token_result = get_next_token()) == FAILED)
    return FAILED;

  if (token_result != GET_NEXT_TOKEN_STRING) {
    print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
    return FAILED;
  }

  /* no name has been defined so far */
  if (g_name_defined == 0) {
    int i;

    for (i = 0; g_tmp[i] != 0 && i < 21; i++)
      g_name[i] = g_tmp[i];

    if (i == 21 && g_tmp[i] != 0) {
      print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
      return FAILED;
    }

    for ( ; i < 21; g_name[i] = 0, i++)
      ;

    g_name_defined = 1;
  }
  else {
    int i;

    /* compare the names */
    for (i = 0; g_tmp[i] != 0 && g_name[i] != 0 && i < 21; i++)
      if (g_name[i] != g_tmp[i])
        break;

    if (i == 21 && g_tmp[i] != 0) {
      print_error(".NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (i != 21 && (g_name[i] != 0 || g_tmp[i] != 0)) {
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
  while (it != NULL) {
    snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
    if (verify_name_length(tmpname) == FAILED)
      return FAILED;

    if (it->type != STRUCTURE_ITEM_TYPE_UNION) { /* add field label */
      char full_label[MAX_NAME_LENGTH + 1];

      fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);
    
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

            fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);

            if (get_full_label(tmpname, full_label) == FAILED)
              return FAILED;
            if (add_label_sizeof(full_label, us->size) == FAILED)
              return FAILED;
          }
          else
            strcpy(tmpname, iname);

          parse_dstruct_entry(tmpname, us, labels_only);
          fprintf(g_file_out_ptr, "o%d 0 ", -us->size); /* rewind */
          us = us->next;
        }

        fprintf(g_file_out_ptr, "o%d 0 ", it->size); /* jump to union end */
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
        fprintf(g_file_out_ptr, "o%d 0 ", -it->instance->size);

        for (g = 1; g <= it->num_instances; g++) {
          snprintf(tmpname, sizeof(tmpname), "%s.%s.%d", iname, it->name, g);
          if (verify_name_length(tmpname) == FAILED)
            return FAILED;

          fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);

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
        if (g_inz == INPUT_NUMBER_STRING) {
          if (it->size < g_string_size) {
            snprintf(g_error_message, sizeof(g_error_message), "String \"%s\" doesn't fit into the %d bytes of \"%s.%s\". Discarding the overflow.\n", g_label, it->size, s->name, it->name);
            print_error(g_error_message, ERROR_WRN);
            c = it->size;
          }
          else
            c = g_string_size;

          /* copy the string */
          for (o = 0; o < c; o++)
            fprintf(g_file_out_ptr, "d%d ", (int)g_label[o]);
        }
        /* take care of the rest */
        else {
          if (it->size == 1) {
            if ((g_inz == SUCCEEDED) && (g_parsed_int < -128 || g_parsed_int > 255)) {
              snprintf(g_error_message, sizeof(g_error_message), "\"%s.%s\" expects 8-bit data, %d is out of range!\n", s->name, it->name, g_parsed_int);
              print_error(g_error_message, ERROR_DIR);
              return FAILED;
            }

            if (g_inz == SUCCEEDED)
              fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
            else if (g_inz == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d Q%s ", g_active_file_info_last->line_current, g_label);
            else if (g_inz == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "c%d ", g_latest_stack);

            o = 1;
          }
          else {
            if (g_inz == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
              snprintf(g_error_message, sizeof(g_error_message), "\"%s.%s\" expects 16-bit data, %d is out of range!\n", s->name, it->name, g_parsed_int);
              print_error(g_error_message, ERROR_DIR);
              return FAILED;
            }

            if (g_inz == SUCCEEDED)
              fprintf(g_file_out_ptr, "y%d", g_parsed_int);
            else if (g_inz == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d r%s ", g_active_file_info_last->line_current, g_label);
            else if (g_inz == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "C%d ", g_latest_stack);

            o = 2;
          }
          /* TODO: longs */
        }
        /* fill the rest of the item with emptyfill or zero */
        if (g_emptyfill_defined != 0)
          f = g_emptyfill;
        else
          f = 0;

        for (; o < it->size; o++)
          fprintf(g_file_out_ptr, "d%d ", f);
      }
      else { /* labels_only == YES */
        fprintf(g_file_out_ptr, "o%d 0 ", it->size);
      }
    }

    it = it->next;

    if (*labels_only == NO) {
      g_inz = input_number();
      if (!(g_inz == SUCCEEDED || g_inz == INPUT_NUMBER_STRING || g_inz == INPUT_NUMBER_ADDRESS_LABEL || g_inz == INPUT_NUMBER_STACK))
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
    strcpy(iname, g_label);

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
  s = get_structure(g_label);

  if (s == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Reference to an unidentified structure \"%s\".\n", g_label);
    print_error(g_error_message, ERROR_DIR);
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
    
    fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, iname);

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

    if (g_dstruct_status == ON) {
      print_error("You can't have nested .DSTRUCT's.\n", ERROR_DIR);
      return FAILED;
    }

    g_dstruct_status = ON;

    skip_next_token();

    fprintf(g_file_out_ptr, "e%d -1 ", s->size); /* mark start address of dstruct */

    q = get_next_token();

    while (q == SUCCEEDED) {
      if ((q2 = parse_if_directive()) != -1) {
        return q2;
      }
      if (strcaselesscmp(g_tmp, ".ENDST") == 0) {
        break;
      }
      else {
        if (g_tmp[strlen(g_tmp)-1] == ':')
          g_tmp[strlen(g_tmp)-1] = '\0';
        strcpy(field_name, g_tmp);

        if (find_struct_field(s, field_name, &item_size, &field_offset) == FAILED) {
          snprintf(g_error_message, sizeof(g_error_message), ".DSTRUCT: Couldn't find field \"%s\" in structure \"%s\".\n", field_name, s->name);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
        fprintf(g_file_out_ptr, "e%d %d ", field_offset, item_size);

        do {
          if ((q = get_next_token()) == FAILED) {
            print_error("Error parsing .DSTRUCT.\n", ERROR_ERR);
            return FAILED;
          }

          if (g_tmp[0] != '.' || strcaselesscmp(g_tmp, ".ENDST") == 0)
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
      fprintf(g_file_out_ptr, "e%d -3 ", 0); /* back to start of struct */
      if (parse_dstruct_entry(iname, s, &labels_only) == FAILED)
        return FAILED;
    }

    fprintf(g_file_out_ptr, "e%d -3 ", 0); /* back to start of struct */
    fprintf(g_file_out_ptr, "e%d -2 ", s->size); /* mark end of .DSTRUCT */

    g_dstruct_status = OFF;

    return SUCCEEDED;
  }
  else if (compare_next_token("DATA") == SUCCEEDED)
    skip_next_token();

  /* legacy syntax */

  g_inz = input_number();
  labels_only = NO;
  if (parse_dstruct_entry(iname, s, &labels_only) == FAILED)
    return FAILED;

  if (g_inz == INPUT_NUMBER_EOL)
    next_line();
  else {
    snprintf(g_error_message, sizeof(g_error_message), "Too much data for structure \"%s\".\n", s->name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_dsb_ds(void) {

  char bak[256];
  int q, parsed_int;
  
  strcpy(bak, g_current_directive);

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs size.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s expects a 16-bit positive integer as size, %d is out of range!\n", bak, g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs data.\n", bak);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "x%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "R%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "c%d ", g_latest_stack);
  }

  return SUCCEEDED;
}


int directive_dsw(void) {

  int q, parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(".DSW needs size.\n", ERROR_INP);
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSW expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(".DSW needs data.\n", ERROR_INP);
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
    snprintf(g_error_message, sizeof(g_error_message), ".DSW expects 16-bit data, %d is out of range!\n", g_parsed_int);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "X%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "r%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "C%d ", g_latest_stack);
  }

  return SUCCEEDED;
}


int directive_incdir(void) {
  
  int q, o;
  char *c;

  g_expect_calculations = NO;
  o = input_number();
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".INCDIR needs a directory string.\n", ERROR_DIR);
    return FAILED;
  }

  q = (int)strlen(g_label);

  /* use the default dir? */
  if (q == 0) {
    if (g_include_dir != NULL)
      g_include_dir[0] = 0;
    return SUCCEEDED;
  }

  /* use the given dir */
  o = (int)(strlen(g_label) + 2);
  if (o > g_include_dir_size) {
    c = realloc(g_include_dir, o);
    if (c == NULL) {
      print_error("Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    g_include_dir = c;
    g_include_dir_size = o;
  }

  /* convert the path string to local enviroment */
  strcpy(g_include_dir, g_label);
  localize_path(g_include_dir);

  /* terminate the string with '/' */
#ifdef MSDOS
  if (g_include_dir[q - 1] != '\\') {
    g_include_dir[q] = '\\';
    g_include_dir[q + 1] = 0;
  }
#else
  if (g_include_dir[q - 1] != '/') {
    g_include_dir[q] = '/';
    g_include_dir[q + 1] = 0;
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
    o = g_source_pointer;
    while (o >= 0) {
      if (toupper(g_buffer[o+0]) == 'I' &&
          toupper(g_buffer[o+1]) == 'N' &&
          toupper(g_buffer[o+2]) == 'C') {
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

    g_expect_calculations = NO;
    o = input_number();
    g_expect_calculations = YES;
    
    if (o == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".INCLUDE needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    if (accumulated_name_length + strlen(g_label) >= sizeof(accumulated_name)) {
      print_error("The accumulated file name length >= MAX_NAME_LENGTH. Increase its size in shared.h and recompile WLA.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(&accumulated_name[accumulated_name_length], g_label);
    accumulated_name_length = (int)strlen(accumulated_name);
  }

  strcpy(path, accumulated_name);

  /* convert the path to local enviroment */
  localize_path(g_label);
  
  if (compare_next_token("NAMESPACE") != SUCCEEDED)
    namespace[0] = 0;
  else {
    skip_next_token();

    g_expect_calculations = NO;
    o = input_number();
    g_expect_calculations = YES;
    
    if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error("Namespace string is missing.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(namespace, g_label);
  }

  if (compare_next_token("ONCE") == SUCCEEDED) {
    skip_next_token();

    got_once = YES;
  }
  
  if (is_real == YES) {
    if (include_file(path, &include_size, namespace) == FAILED)
      return FAILED;
  
    /* WARNING: this is tricky: did we just include a file inside a macro? */
    if (g_macro_active != 0) {
      /* yes. note that the now we added new bytes after g_source_pointer, so if a macro_return_i is
         bigger than g_source_pointer, we'll need to add the bytes to it */
      struct macro_static *ms;
      int q, w;

      for (q = 0; q < g_macro_active; q++) {
        if (g_macro_stack[q].macro_return_i > g_source_pointer)
          g_macro_stack[q].macro_return_i += include_size;
        for (w = 0; w < g_macro_stack[q].supplied_arguments; w++) {
          if (g_macro_stack[q].argument_data[w]->start > g_source_pointer)
            g_macro_stack[q].argument_data[w]->start += include_size;
        }
      }

      /* also macro starting points that are after this position need to move forward
         in memory... */
      ms = g_macros_first;
      while (ms != NULL) {
        if (ms->start > g_source_pointer)
          ms->start += include_size;
        ms = ms->next;
      }
    }

    if (got_once == YES) {
      /* turn the .INCLUDE/.INC into .INDLUDE/.IND to mark it as used, as we got ONCE,
         for repetitive macro calls that contain .INCLUDE/.INC... */
      g_buffer[character_c_position] = 'd';
    }
  }
  
  return SUCCEEDED;
}


int directive_incbin(void) {

  struct macro_static *m;
  int s, r, j, o, id, swap;

  if (g_org_defined == 0 && g_output_format != OUTPUT_LIBRARY) {
    print_error("Before you can .INCBIN data you'll need to use ORG.\n", ERROR_LOG);
    return FAILED;
  }
  
  g_expect_calculations = NO;
  o = input_number();
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".INCBIN needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  /* convert the path string to local enviroment */
  localize_path(g_label);

  if (incbin_file(g_label, &id, &swap, &s, &r, &m) == FAILED)
    return FAILED;
  
  if (m == NULL) {
    /* D [id] [swap] [skip] [size] */
    fprintf(g_file_out_ptr, "D%d %d %d %d ", id, swap, s, r);
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

    ifd = g_incbin_file_data_first;
    for (j = 0; j != id; j++)
      ifd = ifd->next;

    min->data = (unsigned char *)ifd->data;
    min->swap = swap;
    min->position = s;
    min->left = r;

    if (macro_start_incbin(m, min, YES) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_struct(void) {

  if (g_dstruct_status == ON) {
    print_error("You can't use .STRUCT inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  g_active_struct = calloc(sizeof(struct structure), 1);
  if (g_active_struct == NULL) {
    print_error("Out of memory while allocating a new .STRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  g_active_struct->items = NULL;
  g_active_struct->last_item = NULL;
  g_active_struct->alive = YES;
  _remember_new_structure(g_active_struct);

  if (get_next_token() == FAILED)
    return FAILED;

  strcpy(g_active_struct->name, g_tmp);
  
  g_union_stack = NULL;

  g_enum_offset = 0;
  g_last_enum_offset = 0;
  g_max_enum_offset = 0;
  g_base_enum_offset = 0;
  g_enum_ord = 1;
  g_enum_exp = 0;
  g_in_struct = YES;

  return SUCCEEDED;
}


int directive_ramsection(void) {

  int q, current_slot_address;
      
  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  if (g_section_status == ON) {
    snprintf(g_error_message, sizeof(g_error_message), "There is already an open section called \"%s\".\n", g_sections_last->name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  if (g_dstruct_status == ON) {
    print_error("You can't use .RAMSECTION inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  g_sec_tmp = calloc(sizeof(struct section_def), 1);
  if (g_sec_tmp == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new RAMSECTION \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  g_sec_tmp->priority = 0;
  g_sec_tmp->listfile_items = 0;
  g_sec_tmp->listfile_ints = NULL;
  g_sec_tmp->listfile_cmds = NULL;
  g_sec_tmp->maxsize_status = OFF;
  g_sec_tmp->status = SECTION_STATUS_RAM_FREE;
  g_sec_tmp->alive = YES;
  g_sec_tmp->keep = NO;
  g_sec_tmp->data = NULL;
  g_sec_tmp->filename_id = g_active_file_info_last->filename_id;
  g_sec_tmp->id = g_section_id++;
  g_sec_tmp->alignment = 1;
  g_sec_tmp->offset = 0;
  g_sec_tmp->advance_org = YES;
  g_sec_tmp->nspace = NULL;
  g_sec_tmp->bank = 0;
  g_sec_tmp->address = -1;
  g_sec_tmp->bitwindow = 0;
  g_sec_tmp->window_start = -1;
  g_sec_tmp->window_end = -1;
  
  /* add the namespace to the ramsection's name? */
  if (g_active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "RAMSECTION") == FAILED) {
      free(g_sec_tmp);
      return FAILED;
    }
  }

  strcpy(g_sec_tmp->name, g_tmp);
  g_sec_tmp->next = NULL;

  fprintf(g_file_out_ptr, "S%d ", g_sec_tmp->id);

  /* generate a section start label? */
  if (g_extra_definitions == ON)
    generate_label("SECTIONSTART_", g_sec_tmp->name);
  
  /* look for duplicate sections */
  g_sec_next = g_sections_first;
  while (g_sec_next != NULL) {
    if (strcmp(g_sec_next->name, g_tmp) == 0) {
      snprintf(g_error_message, sizeof(g_error_message), "RAMSECTION \"%s\" was defined for the second time.\n", g_tmp);
      print_error(g_error_message, ERROR_DIR);
      free(g_sec_tmp);
      return FAILED;
    }
    g_sec_next = g_sec_next->next;
  }

  g_sec_tmp->label_map = hashmap_new();

  if (g_sections_first == NULL) {
    g_sections_first = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }
  else {
    g_sections_last->next = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }

  while (1) {
    if (compare_next_token("BANK") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(".RAMSECTION cannot take BANK when inside a library.\n", ERROR_DIR);
        return FAILED;
      }

      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error("BANK number must be zero or positive.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_parsed_int > 255 && g_output_format != OUTPUT_LIBRARY) {
        snprintf(g_error_message, sizeof(g_error_message), "We can have 256 RAM banks (0-255) per slot, selected bank %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->bank = g_parsed_int;
    }
    else if (compare_next_token("SLOT") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(".RAMSECTION cannot take SLOT when inside a library.\n", ERROR_DIR);
        return FAILED;
      }

      skip_next_token();

      q = input_number();
      if (q == FAILED)
        return FAILED;
      if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
        /* turn the label into a number */
        if (_get_slot_number_by_its_name(g_label, &g_parsed_int) == FAILED)
          return FAILED;
        q = SUCCEEDED;
      }
      else if (q == SUCCEEDED) {
        /* is the number a direct SLOT number, or is it an address? */
        q = _get_slot_number_by_a_value(g_parsed_int, &g_parsed_int);
      }
      if (q != SUCCEEDED) {
        print_error("Cannot find the SLOT.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_slots[g_parsed_int].size == 0) {
        snprintf(g_error_message, sizeof(g_error_message), "There is no SLOT number %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->slot = g_parsed_int;
    }
    else if (compare_next_token("ORGA") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
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

      current_slot_address = g_slots[g_sec_tmp->slot].address;
      if (g_parsed_int < current_slot_address || g_parsed_int >= (current_slot_address + g_slots[g_sec_tmp->slot].size)) {
        print_error("ORGA is outside the current SLOT.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->address = g_parsed_int - current_slot_address;
    }
    else if (compare_next_token("ORG") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
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

      g_sec_tmp->address = g_parsed_int;
    }
    else if (compare_next_token("WINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the WINDOW starting address.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->window_start = g_parsed_int;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the WINDOW ending address.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->window_end = g_parsed_int;

      if (g_sec_tmp->window_start > g_sec_tmp->window_end) {
        snprintf(g_error_message, sizeof(g_error_message), "The start ($%.4x) of the WINDOW is bigger than the end ($%.4x).\n", g_sec_tmp->window_start, g_sec_tmp->window_end);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
    else if (compare_next_token("BITWINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the BITWINDOW mask.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_parsed_int == 0) {
        print_error("BITWINDOW 0 means that the section cannot be placed anywhere...\n", ERROR_DIR);
        return FAILED;
      }
      
      g_sec_tmp->bitwindow = g_parsed_int;
    }
    /* align the ramsection? */
    else if (compare_next_token("ALIGN") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(".RAMSECTION cannot take ALIGN when inside a library.\n", ERROR_DIR);
        return FAILED;
      }

      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .RAMSECTION alignment.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->alignment = g_parsed_int;
    }
    /* offset the ramsection? */
    else if (compare_next_token("OFFSET") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(".RAMSECTION cannot take OFFSET when inside a library.\n", ERROR_DIR);
        return FAILED;
      }

      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .RAMSECTION offset.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->offset = g_parsed_int;
    }  
    /* the type of the section */
    else if (compare_next_token("FORCE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take FORCE sections.\n", ERROR_DIR);
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_RAM_FORCE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("FREE") == SUCCEEDED) {
      g_sec_tmp->status = SECTION_STATUS_RAM_FREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMIFREE") == SUCCEEDED) {
      g_sec_tmp->status = SECTION_STATUS_RAM_SEMIFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMISUBFREE") == SUCCEEDED) {
      g_sec_tmp->status = SECTION_STATUS_RAM_SEMISUBFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    /* return the org after the section? */
    else if (compare_next_token("RETURNORG") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      g_sec_tmp->advance_org = NO;
    }
    else if (compare_next_token("APPENDTO") == SUCCEEDED) {
      struct after_section *after_tmp;
    
      if (skip_next_token() == FAILED)
        return FAILED;
    
      after_tmp = calloc(sizeof(struct after_section), 1);
      if (after_tmp == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new APPENDTO \"%s\".\n", g_tmp);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      after_tmp->alive = YES;
      after_tmp->is_appendto = YES;
    
      /* get the target section name */
      if (get_next_token() == FAILED) {
        free(after_tmp);
        return FAILED;
      }

      /* add the namespace to the section's name? */
      if (strlen(g_tmp) > 2 && g_tmp[0] == '*' && g_tmp[1] == ':') {
        char buf[MAX_NAME_LENGTH + 1];
      
        /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
        if (strlen(g_tmp) >= sizeof(buf)) {
          snprintf(g_error_message, sizeof(g_error_message), "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "APPENDTO") == FAILED)
          return FAILED;
      }
    
      after_tmp->section = g_sec_tmp;
      strcpy(after_tmp->after, g_tmp);

      after_tmp->next = g_after_sections;
      g_after_sections = after_tmp;
    }
    else if (compare_next_token("AFTER") == SUCCEEDED) {
      struct after_section *after_tmp;
    
      if (skip_next_token() == FAILED)
        return FAILED;
    
      after_tmp = calloc(sizeof(struct after_section), 1);
      if (after_tmp == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new AFTER \"%s\".\n", g_tmp);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      after_tmp->alive = YES;
      after_tmp->is_appendto = NO;
    
      /* get the target section name */
      if (get_next_token() == FAILED) {
        free(after_tmp);
        return FAILED;
      }

      /* add the namespace to the section's name? */
      if (strlen(g_tmp) > 2 && g_tmp[0] == '*' && g_tmp[1] == ':') {
        char buf[MAX_NAME_LENGTH + 1];
      
        /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
        if (strlen(g_tmp) >= sizeof(buf)) {
          snprintf(g_error_message, sizeof(g_error_message), "The AFTER string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "AFTER") == FAILED)
          return FAILED;
      }
    
      after_tmp->section = g_sec_tmp;
      strcpy(after_tmp->after, g_tmp);

      after_tmp->next = g_after_sections;
      g_after_sections = after_tmp;
    }
    else if (compare_next_token("PRIORITY") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .RAMSECTION priority.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->priority = g_parsed_int;
    }
    else if (compare_next_token("KEEP") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      g_sec_tmp->keep = YES;
    }
    else
      break;
  }

  g_enum_offset = 0;
  g_last_enum_offset = 0;
  g_max_enum_offset = 0;
  g_base_enum_offset = 0;
  g_enum_ord = 1;

  /* setup g_active_struct (ramsection vars stored here temporarily) */
  g_active_struct = calloc(sizeof(struct structure), 1);
  if (g_active_struct == NULL) {
    print_error("Out of memory while parsing .RAMSECTION.\n", ERROR_DIR);
    return FAILED;
  }
  g_active_struct->alive = YES;
  _remember_new_structure(g_active_struct);
  g_active_struct->name[0] = '\0';
  g_active_struct->items = NULL;
  g_active_struct->last_item = NULL;
  g_union_stack = NULL;
  
  g_in_ramsection = YES;

  return SUCCEEDED;
}


int directive_section(void) {
  
  int l;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  if (g_dstruct_status == ON) {
    print_error("You can't set the section inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }
  else if (g_section_status == ON) {
    snprintf(g_error_message, sizeof(g_error_message), "There is already an open section called \"%s\".\n", g_sections_last->name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  else if (g_output_format != OUTPUT_LIBRARY && g_bank_defined == 0) {
    print_error(".SECTION requires a predefined bank.\n", ERROR_DIR);
    return FAILED;
  }
  else if (g_output_format != OUTPUT_LIBRARY && g_org_defined == 0) {
    print_error(".SECTION requires a starting address for positioning.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  /* every library section starts @ the beginning of the bank */
  if (g_output_format == OUTPUT_LIBRARY)
    g_org_defined = 1;

  g_sec_tmp = calloc(sizeof(struct section_def), 1);
  if (g_sec_tmp == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new SECTION \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  
  g_sec_tmp->priority = 0;
  g_sec_tmp->listfile_items = 0;
  g_sec_tmp->listfile_ints = NULL;
  g_sec_tmp->listfile_cmds = NULL;
  g_sec_tmp->maxsize_status = OFF;
  g_sec_tmp->data = NULL;
  g_sec_tmp->alignment = 1;
  g_sec_tmp->offset = 0;
  g_sec_tmp->advance_org = YES;
  g_sec_tmp->nspace = NULL;
  g_sec_tmp->keep = NO;
  g_sec_tmp->id = g_section_id++;
  g_sec_tmp->bitwindow = 0;
  g_sec_tmp->window_start = -1;
  g_sec_tmp->window_end = -1;

  fprintf(g_file_out_ptr, "S%d ", g_sec_tmp->id);

  if (strcmp(g_tmp, "BANKHEADER") == 0) {
    no_library_files("bank header sections");
      
    g_sec_next = g_sections_first;
    while (g_sec_next != NULL) {
      if (strcmp(g_sec_next->name, g_tmp) == 0 && g_sec_next->bank == g_bank) {
        snprintf(g_error_message, sizeof(g_error_message), "BANKHEADER section was defined for the second time for bank %d.\n", g_bank);
        print_error(g_error_message, ERROR_DIR);
        free(g_sec_tmp);
        return FAILED;
      }
      g_sec_next = g_sec_next->next;
    }
  }
  else {
    g_sec_next = g_sections_first;
    while (g_sec_next != NULL) {
      if (strcmp(g_sec_next->name, g_tmp) == 0) {
        snprintf(g_error_message, sizeof(g_error_message), "SECTION \"%s\" was defined for the second time.\n", g_tmp);
        print_error(g_error_message, ERROR_DIR);
        free(g_sec_tmp);
        return FAILED;
      }
      g_sec_next = g_sec_next->next;
    }
  }

  strcpy(g_sec_tmp->name, g_tmp);
  g_sec_tmp->next = NULL;
  g_sec_tmp->status = SECTION_STATUS_FREE;

  /* generate a section start label? */
  if (g_extra_definitions == ON)
    generate_label("SECTIONSTART_", g_sec_tmp->name);
  
  g_sec_tmp->label_map = hashmap_new();

  if (g_sections_first == NULL) {
    g_sections_first = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }
  else {
    g_sections_last->next = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }
  
  while (1) {  
    if (compare_next_token("NAMESPACE") == SUCCEEDED) {
      struct namespace_def *nspace = NULL;

      if (skip_next_token() == FAILED)
        return FAILED;

      /* get the name */
      if (input_next_string() == FAILED)
        return FAILED;
      if (g_tmp[0] == '\"' && g_tmp[strlen(g_tmp)-1] == '\"') {
        l = 0;
        while (g_tmp[l+1] != '\"') {
          g_tmp[l] = g_tmp[l+1];
          l++;
        }
        g_tmp[l] = 0;
      }

      hashmap_get(g_namespace_map, g_tmp, (void*)&nspace);
      if (nspace == NULL) {
        nspace = calloc(1, sizeof(struct namespace_def));
        if (nspace == NULL) {
          print_error("Out of memory error.\n", ERROR_DIR);
          return FAILED;
        }
        strcpy(nspace->name, g_tmp);
        if (hashmap_put(g_namespace_map, nspace->name, nspace) != MAP_OK) {
          print_error("Namespace hashmap error.\n", ERROR_DIR);
          return FAILED;
        }
      }

      nspace->label_map = hashmap_new();
      g_sec_tmp->nspace = nspace;
    }
    /* the size of the section? */
    else if (compare_next_token("SIZE") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the SIZE.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_sec_tmp->maxsize_status == ON && g_sec_tmp->maxsize != g_parsed_int) {
        print_error("The SIZE of the section has already been defined.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->maxsize_status = ON;
      g_sec_tmp->maxsize = g_parsed_int;
    }
    /* align the section? */
    else if (compare_next_token("ALIGN") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .SECTION alignment.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->alignment = g_parsed_int;
    }
    /* offset the section? */
    else if (compare_next_token("OFFSET") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .SECTION offset.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->offset = g_parsed_int;
    }
    else if (compare_next_token("WINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the WINDOW starting address.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->window_start = g_parsed_int;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the WINDOW ending address.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->window_end = g_parsed_int;

      if (g_sec_tmp->window_start > g_sec_tmp->window_end) {
        snprintf(g_error_message, sizeof(g_error_message), "The start ($%.4x) of the WINDOW is bigger than the end ($%.4x).\n", g_sec_tmp->window_start, g_sec_tmp->window_end);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
    }
    else if (compare_next_token("BITWINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the BITWINDOW mask.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_parsed_int == 0) {
        print_error("BITWINDOW 0 means that the section cannot be placed anywhere...\n", ERROR_DIR);
        return FAILED;
      }
      
      g_sec_tmp->bitwindow = g_parsed_int;
    }
    /* the type of the section */
    else if (compare_next_token("FORCE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take FORCE sections.\n", ERROR_DIR);
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_FORCE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("FREE") == SUCCEEDED) {
      g_sec_tmp->status = SECTION_STATUS_FREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SUPERFREE") == SUCCEEDED) {
      g_sec_tmp->status = SECTION_STATUS_SUPERFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMIFREE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take SEMIFREE sections.\n", ERROR_DIR);
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_SEMIFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMISUBFREE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take SEMISUBFREE sections.\n", ERROR_DIR);
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_SEMISUBFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("OVERWRITE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take OVERWRITE sections.\n", ERROR_DIR);
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_OVERWRITE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    /* return the org after the section? */
    else if (compare_next_token("RETURNORG") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      g_sec_tmp->advance_org = NO;
    }
    else if (compare_next_token("APPENDTO") == SUCCEEDED) {
      struct after_section *after_tmp;

      if (skip_next_token() == FAILED)
        return FAILED;

      after_tmp = calloc(sizeof(struct after_section), 1);
      if (after_tmp == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new APPENDTO \"%s\".\n", g_tmp);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      after_tmp->alive = YES;
      after_tmp->is_appendto = YES;
    
      /* get the target section name */
      if (get_next_token() == FAILED) {
        free(after_tmp);
        return FAILED;
      }

      /* add the namespace to the section's name? */
      if (strlen(g_tmp) > 2 && g_tmp[0] == '*' && g_tmp[1] == ':') {
        char buf[MAX_NAME_LENGTH + 1];
      
        /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
        if (strlen(g_tmp) >= sizeof(buf)) {
          snprintf(g_error_message, sizeof(g_error_message), "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          print_error(g_error_message, ERROR_DIR);
          free(after_tmp);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "APPENDTO") == FAILED) {
          free(after_tmp);
          return FAILED;
        }
      }
    
      after_tmp->section = g_sec_tmp;
      strcpy(after_tmp->after, g_tmp);

      after_tmp->next = g_after_sections;
      g_after_sections = after_tmp;
    }
    else if (compare_next_token("AFTER") == SUCCEEDED) {
      struct after_section *after_tmp;
    
      if (skip_next_token() == FAILED)
        return FAILED;
    
      after_tmp = calloc(sizeof(struct after_section), 1);
      if (after_tmp == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for a new AFTER \"%s\".\n", g_tmp);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      after_tmp->alive = YES;
      after_tmp->is_appendto = NO;
    
      /* get the target section name */
      if (get_next_token() == FAILED) {
        free(after_tmp);
        return FAILED;
      }

      /* add the namespace to the section's name? */
      if (strlen(g_tmp) > 2 && g_tmp[0] == '*' && g_tmp[1] == ':') {
        char buf[MAX_NAME_LENGTH + 1];
      
        /* nope, this goes to global namespace. now '*:' has done its job, let's remove it */
        if (strlen(g_tmp) >= sizeof(buf)) {
          snprintf(g_error_message, sizeof(g_error_message), "The AFTER string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "AFTER") == FAILED)
          return FAILED;
      }
    
      after_tmp->section = g_sec_tmp;
      strcpy(after_tmp->after, g_tmp);

      after_tmp->next = g_after_sections;
      g_after_sections = after_tmp;
    }
    else if (compare_next_token("PRIORITY") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error("Could not parse the .SECTION priority.\n", ERROR_DIR);
        return FAILED;
      }

      g_sec_tmp->priority = g_parsed_int;
    }
    else if (compare_next_token("KEEP") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      g_sec_tmp->keep = YES;
    }
    else
      break;
  }
  
  /* add the namespace to the section's name? */
  if (g_active_file_info_last->namespace[0] != 0 && g_sec_tmp->nspace == NULL) {
    if (add_namespace_to_string(g_sec_tmp->name, sizeof(g_sec_tmp->name), "SECTION") == FAILED)
      return FAILED;
  }

  /* bankheader section? */
  if (strcmp(g_sec_tmp->name, "BANKHEADER") == 0) {
    g_sec_tmp->status = SECTION_STATUS_HEADER;
    g_bankheader_status = ON;
  }

  g_sec_tmp->alive = YES;
  g_sec_tmp->filename_id = g_active_file_info_last->filename_id;
  g_sec_tmp->bank = g_bank;
  g_section_status = ON;

  return SUCCEEDED;
}


int directive_fopen(void) {
  
  struct filepointer *f;
  char *c;
  int o;

  g_expect_calculations = NO;
  o = input_number();
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".FOPEN needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  /* convert the path to local enviroment */
  localize_path(g_label);

  c = calloc(strlen(g_label) + 1, 1);
  if (c == NULL) {
    print_error("Out of memory error.\n", ERROR_DIR);
    return FAILED;
  }
  strcpy(c, g_label);

  /* get the file pointer name */
  if (get_next_token() == FAILED) {
    free(c);
    return FAILED;
  }

  /* is it defined already? */
  f = g_filepointers;
  while (f != NULL) {
    if (strcmp(g_tmp, f->name) == 0)
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
    f->next = g_filepointers;
    g_filepointers = f;
  }

  f->filename = c;
  strcpy(f->name, g_tmp);

  /* open the file */
  f->f = fopen(f->filename, "rb");
  if (f->f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Error opening file \"%s\" for reading.\n", f->filename);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_fclose(void) {
  
  struct filepointer *f, **t;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  f = g_filepointers;
  t = &g_filepointers;
  while (f != NULL) {
    if (strcmp(g_tmp, f->name) == 0)
      break;
    t = &(f->next);
    f = f->next;
  }

  if (f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Couldn't find filepointer \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
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
  f = g_filepointers;
  while (f != NULL) {
    if (strcmp(g_tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Couldn't find filepointer \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  l = ftell(f->f);
  fseek(f->f, 0, SEEK_END);
  b = ftell(f->f);
  fseek(f->f, l, SEEK_SET);

  /* get the definition label */
  if (get_next_token() == FAILED)
    return FAILED;

  if (add_a_new_definition(g_tmp, (double)b, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_ftell(void) {
  
  struct filepointer *f;
  long b;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  /* fetch the file pointer */
  f = g_filepointers;
  while (f != NULL) {
    if (strcmp(g_tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Couldn't find filepointer \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  b = ftell(f->f);

  if (b < 0) {
    snprintf(g_error_message, sizeof(g_error_message), "Error getting ftell(%s).\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* get the definition label */
  if (get_next_token() == FAILED)
    return FAILED;

  if (redefine(g_tmp, (double)b, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_fseek(void) {
  
  struct filepointer *f;
  long position;
  int mode;

  /* get the file pointer name */
  g_expect_calculations = NO;
  g_plus_and_minus_ends_label = YES;
  if (input_number() != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".FSEEK needs file pointer name.\n", ERROR_DIR);
    return FAILED;
  }
  g_expect_calculations = YES;
  g_plus_and_minus_ends_label = NO;
  
  /* fetch the file pointer */
  f = g_filepointers;
  while (f != NULL) {
    if (strcmp(g_label, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Couldn't find filepointer \"%s\".\n", g_label);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* get the seek size */
  if (input_number() != SUCCEEDED) {
    print_error(".FSEEK needs immediate value for offset.\n", ERROR_DIR);
    return FAILED;
  }

  position = g_parsed_int;

  /* get mode (START/END/CURRENT) */
  if (compare_next_token("START") == SUCCEEDED)
    mode = SEEK_SET;
  else if (compare_next_token("END") == SUCCEEDED)
    mode = SEEK_END;
  else if (compare_next_token("CURRENT") == SUCCEEDED)
    mode = SEEK_CUR;
  else {
    print_error(".FSEEK needs mode (START/CURRENT/END).\n", ERROR_DIR);
    return FAILED;
  }
  
  skip_next_token();
  
  fseek(f->f, position, mode);

  return SUCCEEDED;
}


int directive_fread(void) {
  
  struct filepointer *f;
  unsigned char c;

  /* get the file pointer name */
  if (get_next_token() == FAILED)
    return FAILED;

  f = g_filepointers;
  while (f != NULL) {
    if (strcmp(g_tmp, f->name) == 0)
      break;
    f = f->next;
  }

  if (f == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Couldn't find filepointer \"%s\".\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (fscanf(f->f, "%c", &c) <= 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".FREAD couldn't read a byte from file \"%s\" (%s).\n", f->filename, g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* get the definition label */
  if (get_next_token() == FAILED)
    return FAILED;

  if (redefine(g_tmp, (double)c, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_block(void) {

  struct block_name *b;
  int token_result;
  
  if ((token_result = get_next_token()) == FAILED)
    return FAILED;

  if (token_result != GET_NEXT_TOKEN_STRING) {
    print_error(".BLOCK requires a name string.\n", ERROR_DIR);
    return FAILED;
  }

  b = calloc(sizeof(struct block_name), 1);
  if (b == NULL) {
    print_error("Out of memory while allocating block name.\n", ERROR_DIR);
    return FAILED;
  }

  strcpy(b->name, g_tmp);
  b->id = g_block_name_id++;
  b->next = g_block_names;
  g_block_names = b;
  
  g_block_status++;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
  fprintf(g_file_out_ptr, "g%d ", b->id);

  return SUCCEEDED;
}


int directive_shift(void) {

  struct macro_argument *ma;
  struct macro_runtime *rt;
  struct macro_static *st;
  int q, o;

  if (g_macro_active == 0) {
    print_error(".SHIFT can only be used inside a MACRO.\n", ERROR_DIR);
    return FAILED;
  }

  rt = &g_macro_stack[g_macro_active - 1];
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

  int token_result;

  no_library_files(".NAME");
  
  if ((token_result = get_next_token()) == FAILED)
    return FAILED;

  if (token_result != GET_NEXT_TOKEN_STRING) {
    print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
    return FAILED;
  }

  /* no name has been defined so far */
  if (g_name_defined == 0) {
    int i;

    for (i = 0; g_tmp[i] != 0 && i < 16; i++)
      g_name[i] = g_tmp[i];

    if (i == 16 && g_tmp[i] != 0) {
      print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
      return FAILED;
    }

    for ( ; i < 16; g_name[i] = 0, i++)
      ;

    g_name_defined = 1;
  }
  /* compare the names */
  else {
    int i;

    for (i = 0; g_tmp[i] != 0 && g_name[i] != 0 && i < 16; i++)
      if (g_name[i] != g_tmp[i])
        break;

    if (i == 16 && g_tmp[i] != 0) {
      print_error(".NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (i != 16 && (g_name[i] != 0 || g_tmp[i] != 0)) {
      print_error(".NAME was already defined.\n", ERROR_DIR);
      return FAILED;
    }
  }

  return SUCCEEDED;
}

#endif


int directive_rombanks(void) {

  int i, q, bank_address;

  no_library_files(".ROMBANKS");
    
  if (g_banksize_defined == 0) {
    print_error("No .ROMBANKSIZE defined.\n", ERROR_DIR);
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 1) {
    print_error(".ROMBANKS needs a positive integer value.\n", ERROR_DIR);
    return FAILED;
  }

#ifdef GB
  if (g_parsed_int != 2 && g_parsed_int != 4 && g_parsed_int != 8 && g_parsed_int != 16 && g_parsed_int != 32 && g_parsed_int != 64 &&
      g_parsed_int != 128 && g_parsed_int != 256 && g_parsed_int != 512 && g_parsed_int != 72 && g_parsed_int != 80 && g_parsed_int != 96) {
    print_error("Unsupported amount of ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_parsed_int == 2)
    g_romtype = 0;
  else if (g_parsed_int == 4)
    g_romtype = 1;
  else if (g_parsed_int == 8)
    g_romtype = 2;
  else if (g_parsed_int == 16)
    g_romtype = 3;
  else if (g_parsed_int == 32)
    g_romtype = 4;
  else if (g_parsed_int == 64)
    g_romtype = 5;
  else if (g_parsed_int == 128)
    g_romtype = 6;
  else if (g_parsed_int == 256)
    g_romtype = 7;
  else if (g_parsed_int == 512)
    g_romtype = 8;
  else if (g_parsed_int == 72)
    g_romtype = 0x52;
  else if (g_parsed_int == 80)
    g_romtype = 0x53;
  else if (g_parsed_int == 96)
    g_romtype = 0x54;
#endif

  /* check that the old bank map (smaller) and the new one equal as much as they can */
  if (g_rombanks_defined != 0) {
    int bank;

    if (g_rombanks < g_parsed_int)
      bank = g_rombanks;
    else
      bank = g_parsed_int;

    for (i = 0; i < bank; i++) {
      if (g_banks[i] != g_banksize) {
        print_error("The old and the new .ROMBANKMAP's don't match.\n", ERROR_DIR);
        return FAILED;
      }
    }

    if (g_parsed_int <= g_rombanks)
      return SUCCEEDED;

    snprintf(g_error_message, sizeof(g_error_message), "Upgrading from %d to %d ROM banks.\n", g_rombanks, g_parsed_int);
    print_error(g_error_message, ERROR_WRN);
  }

  g_rombanks = g_parsed_int;
  g_rombanks_defined = 1;
  g_max_address = g_rombanks * g_banksize;

  if (g_rom_banks != NULL)
    free(g_rom_banks);
  if (g_rom_banks_usage_table != NULL)
    free(g_rom_banks_usage_table);

  g_rom_banks = calloc(sizeof(unsigned char) * g_max_address, 1);
  g_rom_banks_usage_table = calloc(sizeof(unsigned char) * g_max_address, 1);
  if (g_rom_banks == NULL || g_rom_banks_usage_table == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  memset(g_rom_banks_usage_table, 0, sizeof(unsigned char) * g_max_address);

  if (g_banks != NULL)
    free(g_banks);
  if (g_bankaddress != NULL)
    free(g_bankaddress);

  g_banks = calloc(sizeof(int) * g_rombanks, 1);
  g_bankaddress = calloc(sizeof(int) * g_rombanks, 1);
  if (g_banks == NULL || g_bankaddress == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  for (bank_address = 0, i = 0; i < g_parsed_int; i++) {
    g_banks[i] = g_banksize;
    g_bankaddress[i] = bank_address;
    bank_address += g_banksize;
  }

  return SUCCEEDED;
}


int directive_rombankmap(void) {
  
  int b = 0, a = 0, bt = 0, bt_defined = 0, x, bs = 0, bs_defined = 0, o, q, token_result;

  no_library_files(".ROMBANKMAP");

  /* ROMBANKMAP has been defined previously */
  if (g_rombankmap_defined != 0 || g_rombanks_defined != 0) {
    o = 0;
    while ((token_result = get_next_token()) == SUCCEEDED) {
      /* .IF directive? */
      if (g_tmp[0] == '.') {
        q = parse_if_directive();
        if (q == FAILED)
          return FAILED;
        else if (q == SUCCEEDED)
          continue;
        /* else q == -1, continue executing below */
      }

      if (strcaselesscmp(g_tmp, ".ENDRO") == 0) {
        break;
      }
      else if (strcaselesscmp(g_tmp, "BANKSTOTAL") == 0) {
        q = input_number();
    
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKSTOTAL needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        if (g_rombanks < g_parsed_int) {
          g_banks = realloc(g_banks, sizeof(int) * g_parsed_int);
          g_bankaddress = realloc(g_bankaddress, sizeof(int) * g_parsed_int);
          if (g_banks == NULL || g_bankaddress == NULL) {
            print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
            return FAILED;
          }
        }

        bt = g_parsed_int;
        bt_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error("BANKSTOTAL needs to be defined prior to BANKSIZE.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();
    
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKSIZE needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }
    
        bs = g_parsed_int;
        bs_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error("BANKSIZE needs to be defined prior to BANKS.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKS needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        for (x = 0; x < g_parsed_int; x++) {
          if (b > bt) {
            print_error("More BANKS than BANKSTOTAL suggests.\n", ERROR_DIR);
            return FAILED;
          }

          /* new banks? */
          if (x >= g_rombanks) {
            g_banks[o] = bs;
            g_bankaddress[o] = a;
          }
          /* compare old banks */
          else if (g_banks[o] != bs) {
            print_error("The old and the new ROMBANKMAPs don't match.\n", ERROR_DIR);
            return FAILED;
          }

          o++;
          b++;
          a += bs;
        }
      }
      else {
        token_result = FAILED;
        break;
      }
    }
  }
  /* no ROMBANKMAP has been defined */
  else {
    o = 0;
    while ((token_result = get_next_token()) == SUCCEEDED) {
      /* .IF directive? */
      if (g_tmp[0] == '.') {
        q = parse_if_directive();
        if (q == FAILED)
          return FAILED;
        else if (q == SUCCEEDED)
          continue;
        /* else q == -1, continue executing below */
      }
      
      if (strcaselesscmp(g_tmp, ".ENDRO") == 0)
        break;
      else if (strcaselesscmp(g_tmp, "BANKSTOTAL") == 0) {
        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKSTOTAL needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        g_banks = calloc(sizeof(int) * g_parsed_int, 1);
        g_bankaddress = calloc(sizeof(int) * g_parsed_int, 1);
        if (g_banks == NULL || g_bankaddress == NULL) {
          print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
          return FAILED;
        }

        bt = g_parsed_int;
        bt_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error("BANKSTOTAL needs to be defined prior to BANKSIZE.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKSIZE needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        bs = g_parsed_int;
        bs_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error("BANKSIZE needs to be defined prior to BANKS.\n", ERROR_DIR);
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error("BANKS needs a positive value.\n", ERROR_DIR);
          return FAILED;
        }

        for (x = 0; x < g_parsed_int; x++) {
          if (b >= bt) {
            print_error("More BANKS than BANKSTOTAL suggests.\n", ERROR_DIR);
            return FAILED;
          }
          g_banks[o] = bs;
          g_bankaddress[o] = a;
          o++;
          b++;
          a += bs;
        }
      }
      else {
        token_result = FAILED;
        break;
      }
    }
  }

  if (token_result != SUCCEEDED) {
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
    g_romtype = 0;
  else if (b == 4)
    g_romtype = 1;
  else if (b == 8)
    g_romtype = 2;
  else if (b == 16)
    g_romtype = 3;
  else if (b == 32)
    g_romtype = 4;
  else if (b == 64)
    g_romtype = 5;
  else if (b == 128)
    g_romtype = 6;
  else if (b == 256)
    g_romtype = 7;
  else if (b == 512)
    g_romtype = 8;
  else if (b == 72)
    g_romtype = 0x52;
  else if (b == 80)
    g_romtype = 0x53;
  else if (b == 96)
    g_romtype = 0x54;
#endif

  if (g_rombanks_defined != 0) {
    if (b > g_rombanks) {
      snprintf(g_error_message, sizeof(g_error_message), "Upgrading from %d to %d ROM banks.\n", g_rombanks, b);
      print_error(g_error_message, ERROR_WRN);
    }
    else
      return SUCCEEDED;
  }

  g_rombanks = b;
  g_rombanks_defined = 1;
  for (g_max_address = 0, q = 0; q < g_rombanks; q++)
    g_max_address += g_banks[q];

  if (g_rom_banks != NULL)
    free(g_rom_banks);
  if (g_rom_banks_usage_table != NULL)
    free(g_rom_banks_usage_table);

  g_rom_banks = calloc(sizeof(unsigned char) * g_max_address, 1);
  g_rom_banks_usage_table = calloc(sizeof(unsigned char) * g_max_address, 1);
  if (g_rom_banks == NULL || g_rom_banks_usage_table == NULL) {
    print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
    return FAILED;
  }

  memset(g_rom_banks_usage_table, 0, sizeof(unsigned char) * g_max_address);
  g_rombankmap_defined = 1;

  return SUCCEEDED;
}


int directive_memorymap(void) {
  
  int slotsize = 0, slotsize_defined = 0, s = 0, q, o, token_result;

  if (g_memorymap_defined == 1) {
    print_error(".MEMORYMAP can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY)
    print_error("Libraries don't need .MEMORYMAP.\n", ERROR_WRN);

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDME") == 0) {
      if (g_defaultslot_defined == 0) {
        print_error("No DEFAULTSLOT defined.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_slots[g_defaultslot].size == 0) {
        snprintf(g_error_message, sizeof(g_error_message), "Unknown DEFAULTSLOT %d.\n", g_defaultslot);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      break;
    }
    else if (strcaselesscmp(g_tmp, "SLOTSIZE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("SLOTSIZE needs an immediate value.\n", ERROR_DIR);
        return FAILED;
      }

      slotsize = g_parsed_int;
      slotsize_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "DEFAULTSLOT") == 0) {
      if (g_defaultslot_defined != 0) {
        print_error("DEFAULTSLOT can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int > 255 || g_parsed_int < 0) {
        print_error("DEFAULTSLOT needs an immediate value (0-255) as an ID.\n", ERROR_DIR);
        return FAILED;
      }

      g_defaultslot_defined = 1;
      g_defaultslot = g_parsed_int;
    }
    else if (strcaselesscmp(g_tmp, "SLOT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int > 255 || g_parsed_int < 0) {
        print_error("SLOT needs a positive value (0-255) as an ID.\n", ERROR_DIR);
        return FAILED;
      }

      if (s != g_parsed_int) {
        print_error("Error in SLOT's ID. ID's start from 0.\n", ERROR_DIR);
        return FAILED;
      }

      o = g_parsed_int;

      /* skip "START" if present */
      if (compare_next_token("START") == SUCCEEDED)
        skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error("The starting address has to be a non-negative value.\n", ERROR_DIR);
        return FAILED;
      }

      g_slots[o].address = g_parsed_int;

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
    
        g_parsed_int = slotsize;
      }
      else {
        if (q == FAILED)
          return FAILED;
        if (q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STRING) {
          /* we got the name for the SLOT instead of its SIZE */
          strcpy(g_slots[o].name, g_label);
          g_parsed_int = slotsize;
        }
        else if (q != SUCCEEDED) {
          print_error("The size of the slot needs to be an immediate value.\n", ERROR_DIR);
          return FAILED;
        }
      }
      
      g_slots[o].size = g_parsed_int;

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

          strcpy(g_slots[o].name, g_label);
        }
      }
      
      g_slots_amount++;
      s++;
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error("Error in .MEMORYMAP data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_memorymap_defined = 1;

  return SUCCEEDED;
}


int directive_unbackground(void) {
  
  int start, end, q;

  if (g_output_format != OUTPUT_OBJECT) {
    print_error(".UNBACKGROUND can only be used in OBJECT output mode.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_background_defined == 0) {
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

  start = g_parsed_int;

  /* get the ending address */
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || q < 0) {
    print_error(".UNBACKGROUND needs the block's ending address.\n", ERROR_DIR);
    return FAILED;
  }

  end = g_parsed_int;

  if (end < start) {
    print_error("The block's ending address is smaller than the starting address!\n", ERROR_DIR);
    return FAILED;
  }
  if (start >= g_max_address) {
    snprintf(g_error_message, sizeof(g_error_message), "The block's starting address $%x is beyond the ROM's ending address $%x.\n", start, g_max_address-1);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  if (end >= g_max_address) {
    snprintf(g_error_message, sizeof(g_error_message), "The block's ending address $%x is beyond the ROM's ending address $%x. Using the ROM's ending address instead.\n", end, g_max_address-1);
    end = g_max_address;
    print_error(g_error_message, ERROR_WRN);
  }
  
  /* clear the memory [start, end] */
  memset(g_rom_banks + start, 0, end - start + 1);
  memset(g_rom_banks_usage_table + start, 0, end - start + 1);

  return SUCCEEDED;
}


int directive_background(void) {
  
  FILE *file_in_ptr;
  int q, background_size;

  if (g_output_format != OUTPUT_OBJECT) {
    print_error(".BACKGROUND can only be used in OBJECT output mode.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_background_defined == 1) {
    print_error("Only one .BACKGROUND can be specified.\n", ERROR_DIR);
    return FAILED;
  }

  g_expect_calculations = NO;
  q = input_number();
  g_expect_calculations = YES;

  if (q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".BACKGROUND needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_memorymap_defined == 0) {
    print_error("No .MEMORYMAP defined.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_rombanks_defined == 0) {
    print_error("No .ROMBANKS defined.\n", ERROR_DIR);
    return FAILED;
  }

  create_full_name(g_include_dir, g_label);

  if ((file_in_ptr = fopen(g_full_name, "rb")) == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Error opening .BACKGROUND file \"%s\".\n", g_full_name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  fseek(file_in_ptr, 0, SEEK_END);
  background_size = (int)ftell(file_in_ptr);
  fseek(file_in_ptr, 0, SEEK_SET);
  
  if (background_size > g_max_address) {
    snprintf(g_error_message, sizeof(g_error_message), ".BACKGROUND file \"%s\" size (%d) is larger than ROM size (%d).\n", g_full_name, background_size, g_max_address);
    print_error(g_error_message, ERROR_DIR);
    fclose(file_in_ptr);
    return FAILED;
  }

  memset(g_rom_banks_usage_table, 2, background_size);
  if (fread(g_rom_banks, 1, background_size, file_in_ptr) != (size_t) background_size) {
    snprintf(g_error_message, sizeof(g_error_message), "Could not read all %d bytes of \"%s\"!", background_size, g_full_name);
    print_error(g_error_message, ERROR_INC);
    return FAILED;
  }

  g_background_defined = 1;
  fclose(file_in_ptr);

  return SUCCEEDED;
}


#ifdef GB

int directive_gbheader(void) {

  int q, token_result;
    
  if (g_gbheader_defined != 0) {
    print_error(".GBHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_computechecksum_defined != 0)
    print_error(".COMPUTEGBCHECKSUM is unnecessary when .GBHEADER is defined.\n", ERROR_WRN);
  else
    g_computechecksum_defined++;

  if (g_computecomplementcheck_defined != 0)
    print_error(".COMPUTEGBCOMPLEMENTCHECK is unnecessary when .GBHEADER is defined.\n", ERROR_WRN);
  else
    g_computecomplementcheck_defined++;

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .GBHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDGB") == 0)
      break;
    else if (strcaselesscmp(g_tmp, "NINTENDOLOGO") == 0)
      g_nintendologo_defined++;
    else if (strcaselesscmp(g_tmp, "ROMDMG") == 0) {
      if (g_romgbc == 1) {
        print_error(".ROMGBC was defined prior to .ROMDMG.\n", ERROR_DIR);
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(".ROMGBCONLY was defined prior to .ROMDMG.\n", ERROR_DIR);
        return FAILED;
      }
      else if (g_romsgb != 0) {
        print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
        return FAILED;
      }
      g_romdmg = 1;
    }
    else if (strcaselesscmp(g_tmp, "ROMGBC") == 0) {
      if (g_romdmg != 0) {
        print_error(".ROMDMG was defined prior to .ROMGBC.\n", ERROR_DIR);
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(".ROMGBCONLY was defined prior to .ROMGBC.\n", ERROR_DIR);
        return FAILED;
      }
      g_romgbc = 1;
    }
    else if (strcaselesscmp(g_tmp, "ROMGBCONLY") == 0) {
      if (g_romdmg != 0) {
        print_error(".ROMDMG was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
        return FAILED;
      }
      else if (g_romgbc == 1) {
        print_error(".ROMGBC was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
        return FAILED;
      }
      g_romgbc = 2;
    }
    else if (strcaselesscmp(g_tmp, "ROMSGB") == 0) {
      if (g_romdmg != 0) {
        print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
        return FAILED;
      }
      g_romsgb++;
    }
    else if (strcaselesscmp(g_tmp, "NAME") == 0) {
      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no name has been defined so far */
      if (g_name_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 16; i++)
          g_name[i] = g_tmp[i];
    
        if (i == 16 && g_tmp[i] != 0) {
          print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
          return FAILED;
        }

        for ( ; i < 16; g_name[i] = 0, i++)
          ;

        g_name_defined = 1;
      }
      else {
        int i;

        /* compare the names */
        for (i = 0; g_tmp[i] != 0 && g_name[i] != 0 && i < 16; i++)
          if (g_name[i] != g_tmp[i])
            break;
    
        if (i == 16 && g_tmp[i] != 0) {
          print_error("NAME requires a string of 1 to 16 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (i != 16 && (g_name[i] != 0 || g_tmp[i] != 0)) {
          print_error("NAME was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }
    else if (strcaselesscmp(g_tmp, "LICENSEECODEOLD") == 0) {
      if (g_licenseecodenew_defined != 0) {
        print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        snprintf(g_error_message, sizeof(g_error_message), ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (g_licenseecodeold_defined != 0) {
        if (g_licenseecodeold != g_parsed_int) {
          print_error(".LICENSEECODEOLD was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      g_licenseecodeold = g_parsed_int;
      g_licenseecodeold_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "LICENSEECODENEW") == 0) {
      if (g_licenseecodeold_defined != 0) {
        print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
        return FAILED;
      }

      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
        return FAILED;
      }
      if (!(g_tmp[0] != 0 && g_tmp[1] != 0 && g_tmp[2] == 0)) {
        print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
        return FAILED;
      }

      if (g_licenseecodenew_defined != 0) {
        if (g_tmp[0] != g_licenseecodenew_c1 || g_tmp[1] != g_licenseecodenew_c2) {
          print_error(".LICENSEECODENEW was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      g_licenseecodenew_c1 = g_tmp[0];
      g_licenseecodenew_c2 = g_tmp[1];
      g_licenseecodenew_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "CARTRIDGETYPE") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "CARTRIDGETYPE needs a 8-bit value, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_cartridgetype_defined != 0 && g_cartridgetype != g_parsed_int) {
          print_error("CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_cartridgetype = g_parsed_int;
        g_cartridgetype_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "RAMSIZE") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "RAMSIZE needs a 8-bit value, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_rambanks_defined != 0 && g_rambanks != g_parsed_int) {
          print_error("RAMSIZE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_rambanks = g_parsed_int;
        g_rambanks_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "COUNTRYCODE") == 0) {
      int number_result = input_number();
      
      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "COUNTRYCODE needs a non-negative value, got %d.\n\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_countrycode_defined != 0 && g_countrycode != g_parsed_int) {
          print_error("COUNTRYCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_countrycode = g_parsed_int;
        g_countrycode_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "DESTINATIONCODE") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "DESTINATIONCODE needs a non-negative value, got %d.\n\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_countrycode_defined != 0 && g_countrycode != g_parsed_int) {
          print_error("DESTINATIONCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_countrycode = g_parsed_int;
        g_countrycode_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "VERSION") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "VERSION needs a non-negative value, got %d.\n\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_version_defined != 0 && g_version != g_parsed_int) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_version = g_parsed_int;
        g_version_defined = 1;
      }
      else
        return FAILED;
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error("Error in .GBHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_gbheader_defined = 1;

  return SUCCEEDED;
}

#endif


static struct array *_get_array(char *name) {

  struct array *arr = g_arrays_first;

  while (arr != NULL) {
    if (strcmp(name, arr->name) == 0)
      return arr;
    arr = arr->next;
  }

  return NULL;
}


static struct array *_create_array(char *name, int size) {

  struct array *arr = NULL;
  int i, *data;
  
  arr = calloc(sizeof(struct array), 1);
  if (arr == NULL) {
    print_error("Out of memory while allocating a new array.\n", ERROR_DIR);
    return NULL;
  }

  if (size < 256)
    size = 256;

  strcpy(arr->name, name);
  arr->size = size;

  arr->data = (int *)calloc(sizeof(int) * size, 1);
  if (arr->data == NULL) {
    print_error("Out of memory while allocating a new array.\n", ERROR_DIR);
    free(arr);
    return NULL;
  }

  /* initialize the array */
  data = arr->data;
  for (i = 0; i < size; i++)
    data[i] = 0;

  arr->next = g_arrays_first;
  g_arrays_first = arr;
  
  return arr;
}


int directive_arraydef_arraydefine(void) {

  char name[MAX_NAME_LENGTH + 1], bak[256];
  struct array *arr;
  int q;

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array doesn't exist */
  if (_get_array(g_tmp) != NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "\"%s\" is already defined.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  strcpy(name, g_tmp);
  
  /* skip SIZE if present */
  if (compare_next_token("SIZE") == SUCCEEDED)
    skip_next_token();
  
  q = input_number();

  if (q == FAILED)
    return FAILED;
  else if (q == SUCCEEDED) {
  }
  else {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs an immediate value for the size.\n", bak);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  arr = _create_array(name, g_parsed_int);
  if (arr == NULL)
    return FAILED;

  return SUCCEEDED;
}


int directive_arrayin(void) {

  struct array *arr;
  int index, value, q;
  
  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_tmp);
  if (arr == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Array \"%s\" doesn't exist.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* skip INDEX if present */
  if (compare_next_token("INDEX") == SUCCEEDED)
    skip_next_token();

  q = input_number();

  if (q == FAILED)
    return FAILED;
  else if (q == SUCCEEDED) {
  }
  else {
    print_error(".ARRAYIN needs an immediate value for the index.\n", ERROR_DIR);
    return FAILED;
  }

  index = g_parsed_int;

  if (index < 0) {
    print_error(".ARRAYIN needs a positive or zero value for the index.\n", ERROR_DIR);
    return FAILED;
  }

  /* skip VALUE/VALUES if present */
  if (compare_next_token("VALUE") == SUCCEEDED || compare_next_token("VALUES") == SUCCEEDED)
    skip_next_token();

  while (1) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    else if (q == SUCCEEDED) {
    }
    else if (q == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else {
      print_error(".ARRAYIN needs an immediate value for the value.\n", ERROR_DIR);
      return FAILED;
    }

    value = g_parsed_int;

    if (index < arr->size)
      arr->data[index] = value;
    else {
      /* index is out of bounds, resize the array, and then insert the value */
      arr->data = realloc(arr->data, sizeof(int) * (index + 1));
      if (arr->data == NULL) {
        print_error("Out of memory resizing the array.\n", ERROR_DIR);
        return FAILED;
      }

      arr->size = index + 1;
      arr->data[index] = value;
    }

    index++;
  }
  
  return SUCCEEDED;
}


int directive_arrayout(void) {

  struct array *arr;
  int index, q;
  
  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_tmp);
  if (arr == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Array \"%s\" doesn't exist.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* skip INDEX if present */
  if (compare_next_token("INDEX") == SUCCEEDED)
    skip_next_token();

  q = input_number();

  if (q == FAILED)
    return FAILED;
  else if (q == SUCCEEDED) {
  }
  else {
    print_error(".ARRAYOUT needs an immediate value for the index.\n", ERROR_DIR);
    return FAILED;
  }

  index = g_parsed_int;

  if (index < 0) {
    print_error(".ARRAYOUT needs a positive or zero value for the index.\n", ERROR_DIR);
    return FAILED;
  }
  if (index >= arr->size) {
    snprintf(g_error_message, sizeof(g_error_message), "Index %d is out of array \"%s\"'s size of %d items.\n", index, arr->name, arr->size);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  
  /* skip DEFINITION if present */
  if (compare_next_token("DEFINITION") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  redefine(g_tmp, (double)arr->data[index], NULL, DEFINITION_TYPE_VALUE, 0);

  return SUCCEEDED;
}


int directive_arraydb_arraydw_arraydl_arraydd(void) {

  struct array *arr;
  int index = 0, q, i = 0, data_size;
  char bak[256];

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  strcpy(bak, g_current_directive);

  if (strcaselesscmp(g_current_directive, "ARRAYDB") == 0)
    data_size = 1;
  else if (strcaselesscmp(g_current_directive, "ARRAYDW") == 0)
    data_size = 2;
  else if (strcaselesscmp(g_current_directive, "ARRAYDL") == 0)
    data_size = 3;
  else if (strcaselesscmp(g_current_directive, "ARRAYDD") == 0)
    data_size = 4;
  else {
    snprintf(g_error_message, sizeof(g_error_message), "Unsupported directive \".%s\"! Please submit a bug report!\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  
  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_tmp);
  if (arr == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Array \"%s\" doesn't exist.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* skip INDICES if present */
  if (compare_next_token("INDICES") == SUCCEEDED)
    skip_next_token();

  while (1) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    else if (q == SUCCEEDED) {
      /* immediate values are accepted */
    }
    else if (q == INPUT_NUMBER_STRING) {
      /* .ARRAYDB takes also strings */
    }
    else if (q == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else {
      snprintf(g_error_message, sizeof(g_error_message), ".%s needs an immediate value or a string for the index.\n", bak);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (q == SUCCEEDED) {
      index = g_parsed_int;

      if (index < 0) {
        snprintf(g_error_message, sizeof(g_error_message), ".%s needs a positive or zero value for the index.\n", bak);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      if (index >= arr->size) {
        snprintf(g_error_message, sizeof(g_error_message), "Index %d is out of array \"%s\"'s size of %d items.\n", index, arr->name, arr->size);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      i = arr->data[index];
    }
    
    if (data_size == 1) {
      if (q == SUCCEEDED) {
        if (i < -128 || i > 255) {
          snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d) is out of 8-bit range!\n", i, index);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "d%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            snprintf(g_error_message, sizeof(g_error_message), "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -128 || i > 255) {
            snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d, '%c') is out of 8-bit range!\n", i, index, (char)index);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          fprintf(g_file_out_ptr, "d%d ", i);
        }
      }
    }
    else if (data_size == 2) {
      if (q == SUCCEEDED) {
        if (i < -32768 || i > 65535) {
          snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d) is out of 16-bit range!\n", i, index);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            snprintf(g_error_message, sizeof(g_error_message), "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -32768 || i > 65535) {
            snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d, '%c') is out of 16-bit range!\n", i, index, (char)index);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          fprintf(g_file_out_ptr, "y%d ", i);
        }
      }
    }
    else if (data_size == 3) {
      if (q == SUCCEEDED) {
        if (i < -8388608 || i > 16777215) {
          snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d) is out of 24-bit range!\n", i, index);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            snprintf(g_error_message, sizeof(g_error_message), "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -8388608 || i > 16777215) {
            snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d, '%c') is out of 24-bit range!\n", i, index, (char)index);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          fprintf(g_file_out_ptr, "z%d ", i);
        }
      }
    }
    else if (data_size == 4) {
      if (q == SUCCEEDED) {
        /*
          if (i < -2147483648 || i > 2147483647) {
          snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array is out of 32-bit range!\n", i);
          print_error(g_error_message, ERROR_DIR);
          return FAILED;
          }
        */

        fprintf(g_file_out_ptr, "u%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            snprintf(g_error_message, sizeof(g_error_message), "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }

          i = arr->data[index];

          /*
          if (i < -2147483648 || i > 2147483647) {
            snprintf(g_error_message, sizeof(g_error_message), "The value (%d) in the array (index %d, '%c') is out of 32-bit range!\n", i, index, (char)index);
            print_error(g_error_message, ERROR_DIR);
            return FAILED;
          }
          */

          fprintf(g_file_out_ptr, "u%d ", i);
        }
      }
    }
  }
  
  return SUCCEEDED;
}


int directive_define_def_equ(void) {
  
  int j, g_size, export, q;
  double dou;
  char k[256];

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(g_tmp) == YES) {
    snprintf(g_error_message, sizeof(g_error_message), "\"%s\" is a reserved definition label and is not user definable.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  g_input_float_mode = ON;
  q = get_new_definition_data(&j, k, &g_size, &dou, &export);
  g_input_float_mode = OFF;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_EOL || q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error("Could not parse the definition data.\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    q = add_a_new_definition(g_tmp, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    q = add_a_new_definition(g_tmp, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    q = add_a_new_definition(g_tmp, 0.0, k, DEFINITION_TYPE_STRING, g_size);
  else if (q == INPUT_NUMBER_STACK)
    q = add_a_new_definition(g_tmp, (double)j, NULL, DEFINITION_TYPE_STACK, 0);
  else if (q == INPUT_NUMBER_EOL)
    q = add_a_new_definition(g_tmp, 0.0, NULL, DEFINITION_TYPE_VALUE, 0);
  
  if (q == FAILED)
    return FAILED;

  if (export == YES) {
    if (export_a_definition(g_tmp) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_undef_undefine(void) {

  char bak[256];
  int q;

  strcpy(bak, g_current_directive);

  q = 0;
  while (1) {
    int string_result = input_next_string();

    if (string_result == FAILED)
      return FAILED;
    if (string_result == INPUT_NUMBER_EOL) {
      if (q != 0) {
        next_line();
        return SUCCEEDED;
      }
      snprintf(g_error_message, sizeof(g_error_message), ".%s requires definition name(s).\n", bak);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    q++;

    if (undefine(g_tmp) == FAILED) {
      snprintf(g_error_message, sizeof(g_error_message), "Could not .%s \"%s\".\n", bak, g_tmp);
      print_error(g_error_message, ERROR_WRN);
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
    if (g_enumid_defined == 0) {
      print_error(".ENUMID needs the initial value when .ENUMID is used the first time.\n", ERROR_DIR);
      return FAILED;
    }
    
    if (is_reserved_definition(g_label) == YES) {
      snprintf(g_error_message, sizeof(g_error_message), "\"%s\" is a reserved definition label and is not user definable.\n", g_label);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (add_a_new_definition(g_label, (double)g_enumid, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    if (g_enumid_export == 1) {
      if (export_a_definition(g_label) == FAILED)
        return FAILED;
    }

    g_enumid += g_enumid_adder;

    return SUCCEEDED;
  }
  else if (q == SUCCEEDED) {
    g_enumid = g_parsed_int;
    g_enumid_adder = 1;
    g_enumid_export = 0;

    if (compare_next_token("STEP") == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;

      if (q != SUCCEEDED) {
        print_error("STEP needs a value\n", ERROR_DIR);
        return FAILED;
      }

      g_enumid_adder = g_parsed_int;
    }

    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();

      g_enumid_export = 1;
    }

    g_enumid_defined = 1;

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

  if (fgets(k, 254, stdin) == NULL) {
    print_error(".INPUT needs some data.\n", ERROR_DIR);
    return FAILED;
  }

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
      redefine(g_tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
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
      redefine(g_tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
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
      redefine(g_tmp, (double)v, NULL, DEFINITION_TYPE_VALUE, 0);
      return SUCCEEDED;
    }
  }

  /* it's a string */
  redefine(g_tmp, 0.0, k, DEFINITION_TYPE_STRING, (int)strlen(k));

  return SUCCEEDED;
}


int directive_redefine_redef(void) {
  
  int j, g_size, export, q;
  double dou;
  char k[256];

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(g_tmp) == YES) {
    snprintf(g_error_message, sizeof(g_error_message), "\"%s\" is a reserved definition label and is not user definable.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  g_input_float_mode = ON;
  q = get_new_definition_data(&j, k, &g_size, &dou, &export);
  g_input_float_mode = OFF;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error("Could not parse the definition data.\n", ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED)
    redefine(g_tmp, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    redefine(g_tmp, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    redefine(g_tmp, 0.0, k, DEFINITION_TYPE_STRING, g_size);
  else if (q == INPUT_NUMBER_STACK)
    redefine(g_tmp, (double)j, NULL, DEFINITION_TYPE_STACK, 0);

  if (export == YES) {
    if (export_a_definition(g_tmp) == FAILED)
      return FAILED;
  }
    
  return SUCCEEDED;
}


#ifdef Z80

int directive_smsheader(void) {
  
  int q, token_result;
    
  if (g_smsheader_defined != 0) {
    print_error(".SMSHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_computesmschecksum_defined != 0)
    print_error(".COMPUTESMSCHECKSUM is unnecessary when .SMSHEADER is defined.\n", ERROR_WRN);

  if (g_smstag_defined != 0)
    print_error(".SMSTAG is unnecessary when .SMSHEADER is defined.\n", ERROR_WRN);

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SMSHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDSMS") == 0)
      break;
    else if (strcaselesscmp(g_tmp, "VERSION") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0 || g_parsed_int > 15) {
        snprintf(g_error_message, sizeof(g_error_message), "VERSION needs a value between 0 and 15, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (g_smsversion_defined != 0) {
        if (g_smsversion != g_parsed_int) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      g_smsversion = g_parsed_int;
      g_smsversion_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "ROMSIZE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0 || g_parsed_int > 15) {
        snprintf(g_error_message, sizeof(g_error_message), "ROMSIZE needs a value between 0 and 15, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (g_smsromsize_defined != 0) {
        if (g_smsromsize != g_parsed_int) {
          print_error("ROMSIZE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      g_smsromsize = g_parsed_int;
      g_smsromsize_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "REGIONCODE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 3|| g_parsed_int > 7) {
        snprintf(g_error_message, sizeof(g_error_message), "REGIONCODE needs a value between 3 and 7, got %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (g_smsregioncode_defined != 0) {
        if (g_smsregioncode != g_parsed_int) {
          print_error("REGIONCODE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }
      }

      g_smsregioncode = g_parsed_int;
      g_smsregioncode_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "PRODUCTCODE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      g_smsproductcode1 = g_parsed_int & 255;
      g_smsproductcode_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      g_smsproductcode2 = g_parsed_int & 255;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("PRODUCTCODE needs 2.5 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      g_smsproductcode3 = g_parsed_int & 15;
    }
    else if (strcaselesscmp(g_tmp, "RESERVEDSPACE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("RESERVEDSPACE needs 2 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      g_smsreservedspace1 = g_parsed_int & 255;
      smsreservedspace_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error("RESERVEDSPACE needs 2 bytes of data.\n", ERROR_DIR);
        return FAILED;
      }

      g_smsreservedspace2 = g_parsed_int & 255;
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error("Error in .SMSHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_smsheader_defined = 1;

  return SUCCEEDED;
}


int directive_sdsctag(void) {
  
  int q;

  if (g_sdsctag_defined != 0) {
    print_error(".SDSCTAG can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
    
  no_library_files(".SDSCTAG");

  g_input_float_mode = ON;
  q = input_number();
  g_input_float_mode = OFF;
  if (q != SUCCEEDED && q != INPUT_NUMBER_FLOAT) {
    print_error(".SDSCTAG needs the version number.\n" , ERROR_DIR);
    return FAILED;
  }

  if (q == SUCCEEDED) {
    g_sdsc_ma = g_parsed_int;
    g_sdsc_mi = 0;
  }
  else {
    g_sdsc_ma = (int)g_parsed_double;
    g_sdsc_mi = g_parsed_double_decimal_numbers;
  }
  
  if (g_sdsc_ma >= 100 || g_sdsc_mi >= 100) {
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
    g_sdsctag_name_type = TYPE_VALUE;
    g_sdsctag_name_value = g_parsed_int;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (g_label[0] == 0) {
      g_sdsctag_name_type = TYPE_VALUE;
      g_sdsctag_name_value = 0xFFFF; /* null string */
    }
    else {
      g_sdsctag_name_type = TYPE_STRING;
      strcpy(g_sdsctag_name_str, g_label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_name_type = TYPE_LABEL;
    strcpy(g_sdsctag_name_str, g_label);
  }
  else {
    g_sdsctag_name_type = TYPE_STACK_CALCULATION;
    g_sdsctag_name_value = g_latest_stack;
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
    g_sdsctag_notes_type = TYPE_VALUE;
    g_sdsctag_notes_value = g_parsed_int;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (g_label[0] == 0) {
      g_sdsctag_notes_type = TYPE_VALUE;
      g_sdsctag_notes_value = 0xFFFF; /* null string */
    }
    else {
      g_sdsctag_notes_type = TYPE_STRING;
      strcpy(g_sdsctag_notes_str, g_label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_notes_type = TYPE_LABEL;
    strcpy(g_sdsctag_notes_str, g_label);
  }
  else {
    g_sdsctag_notes_type = TYPE_STACK_CALCULATION;
    g_sdsctag_notes_value = g_latest_stack;
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
    g_sdsctag_author_type = TYPE_VALUE;
    g_sdsctag_author_value = g_parsed_int;
  }
  else if (q == INPUT_NUMBER_STRING) {
    if (g_label[0] == 0) {
      g_sdsctag_author_type = TYPE_VALUE;
      g_sdsctag_author_value = 0xFFFF; /* null string */
    }
    else {
      g_sdsctag_author_type = TYPE_STRING;
      strcpy(g_sdsctag_author_str, g_label);
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_author_type = TYPE_LABEL;
    strcpy(g_sdsctag_author_str, g_label);
  }
  else {
    g_sdsctag_author_type = TYPE_STACK_CALCULATION;
    g_sdsctag_author_value = g_latest_stack;
  }

  g_sdsctag_defined++;
  g_smstag_defined++;
  g_computesmschecksum_defined++;

  return SUCCEEDED;
}

#endif


int directive_macro(void) {

  struct macro_static *m;
  int macro_start_line;
  int q;

  if (g_dstruct_status == ON) {
    print_error("You can't define a macro inside .DSTRUCT.\n", ERROR_DIR);
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  if (strcaselesscmp(g_current_directive, "ENDM") == 0) {
    print_error("A MACRO must have a name.\n", ERROR_DIR);
    return FAILED;
  }

  macro_start_line = g_active_file_info_last->line_current;

  /* append the namespace, if this file uses if */
  if (g_active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(g_tmp, sizeof(g_tmp), "MACRO") == FAILED)
      return FAILED;
  }

  if (macro_get(g_tmp, NO, &m) == FAILED)
    return FAILED;
  
  if (m != NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "MACRO \"%s\" was defined for the second time.\n", g_tmp);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  m = calloc(sizeof(struct macro_static), 1);
  if (m == NULL) {
    print_error("Out of memory while allocating room for a new MACRO.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_macros_first == NULL) {
    g_macros_first = m;
    g_macros_last = m;
  }
  else {
    g_macros_last->next = m;
    g_macros_last = m;
  }

  strcpy(m->name, g_tmp);
  m->next = NULL;
  m->calls = 0;
  m->filename_id = g_active_file_info_last->filename_id;
  m->argument_names = NULL;

  /* is ARGS defined? */
  q = 0;
  if (compare_next_token("ARGS") == SUCCEEDED) {
    skip_next_token();

    while (1) {
      int string_result = input_next_string();

      if (string_result == FAILED)
        return FAILED;
      if (string_result == INPUT_NUMBER_EOL) {
        if (q != 0) {
          next_line();
          break;
        }
        snprintf(g_error_message, sizeof(g_error_message), "MACRO \"%s\" is missing argument names?\n", m->name);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      q++;

      /* store the label */
      m->argument_names = realloc(m->argument_names, sizeof(char *)*q);
      if (m->argument_names == NULL) {
        print_error("Out of memory error.\n", ERROR_NONE);
        return FAILED;
      }
      m->argument_names[q-1] = calloc(strlen(g_tmp)+1, 1);
      if (m->argument_names[q-1] == NULL) {
        print_error("Out of memory error.\n", ERROR_NONE);
        return FAILED;
      }

      strcpy(m->argument_names[q-1], g_tmp);
    }
  }

  m->nargument_names = q;
  m->start = g_source_pointer;
  m->start_line = g_active_file_info_last->line_current;

  /* go to the end of the macro */
  for (; g_source_pointer < g_size; g_source_pointer++) {
    if (g_buffer[g_source_pointer] == 0x0A) {
      next_line();
      continue;
    }
    else if ((strncmp(&g_buffer[g_source_pointer], ".E", 2) == 0) && (g_buffer[g_source_pointer + 2] == 0x0A || g_buffer[g_source_pointer + 2] == ' ')) {
      g_active_file_info_last->line_current = macro_start_line;
      snprintf(g_error_message, sizeof(g_error_message), "MACRO \"%s\" wasn't terminated with .ENDM.\n", m->name);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    else if ((strncmp(&g_buffer[g_source_pointer], ".ENDM", 5) == 0 || strncmp(&g_buffer[g_source_pointer], ".endm", 5) == 0) && (g_buffer[g_source_pointer + 5] == 0x0A || g_buffer[g_source_pointer + 5] == ' ')) {
      g_source_pointer += 5;
      break;
    }
  }

  return SUCCEEDED;
}


int directive_rept_repeat(void) {
  
  char c[16], index_name[MAX_NAME_LENGTH + 1];
  int q;

  strcpy(c, g_current_directive);

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs a count.\n", c);
    print_error(g_error_message, ERROR_INP);
    return FAILED;
  }

  if (g_parsed_int < 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s count value must be positive or zero.\n", c);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  index_name[0] = 0;
  if (compare_next_token("INDEX") == SUCCEEDED) {
    skip_next_token();

    if (input_next_string() != SUCCEEDED)
      return FAILED;

    if (redefine(g_tmp, 0.0, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    strcpy(index_name, g_tmp);
  }
    
  if (g_parsed_int == 0) {
    int l, r, m;

    l = g_active_file_info_last->line_current;
    /* find the next compiling point */
    r = 1;
    m = g_macro_active;
    /* disable macro decoding */
    g_macro_active = 0;
    while (get_next_token() != FAILED) {
      if (g_tmp[0] == '.') {
        if (strcaselesscmp(g_current_directive, "ENDR") == 0)
          r--;
        if (strcaselesscmp(g_current_directive, "E") == 0)
          break;
        if (strcaselesscmp(g_current_directive, "REPT") == 0 || strcaselesscmp(g_current_directive, "REPEAT") == 0)
          r++;
      }
      if (r == 0) {
        g_macro_active = m;
        return SUCCEEDED;
      }
    }
    
    /* return the condition's line number */
    g_active_file_info_last->line_current = l;
    snprintf(g_error_message, sizeof(g_error_message), ".%s must end to .ENDR.\n", c);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (g_repeat_active == g_repeat_stack_size) {
    struct repeat_runtime *rr;

    g_repeat_stack_size = (g_repeat_stack_size<<1)+2;
    rr = realloc(g_repeat_stack, sizeof(struct repeat_runtime) * g_repeat_stack_size);
    if (rr == NULL) {
      print_error("Out of memory error while enlarging repeat stack buffer.\n", ERROR_ERR);
      return FAILED;
    }
    g_repeat_stack = rr;
  }

  g_repeat_stack[g_repeat_active].start = g_source_pointer;
  g_repeat_stack[g_repeat_active].counter = g_parsed_int;
  g_repeat_stack[g_repeat_active].repeats = 0;
  g_repeat_stack[g_repeat_active].start_line = g_active_file_info_last->line_current;
  strcpy(g_repeat_stack[g_repeat_active].index_name, index_name);

  g_repeat_active++;

  /* repeat start */
  fprintf(g_file_out_ptr, "j ");
      
  return SUCCEEDED;
}


int directive_endm(void) {

  int q;
  
  if (g_macro_active != 0) {
    g_macro_active--;

    /* macro call end */
    fprintf(g_file_out_ptr, "I%s ", g_macro_stack[g_macro_active].macro->name);
    
    /* free the arguments */
    if (g_macro_stack[g_macro_active].supplied_arguments > 0) {
      for (q = 0; q < g_macro_stack[g_macro_active].supplied_arguments; q++)
        free(g_macro_stack[g_macro_active].argument_data[q]);
      free(g_macro_stack[g_macro_active].argument_data);
      g_macro_stack[g_macro_active].argument_data = NULL;
    }

    /* free the argument definitions */
    for (q = 0; q < g_macro_stack[g_macro_active].macro->nargument_names; q++)
      undefine(g_macro_stack[g_macro_active].macro->argument_names[q]);

    g_source_pointer = g_macro_stack[g_macro_active].macro_return_i;

    if ((g_extra_definitions == ON) && (g_active_file_info_last->filename_id != g_macro_stack[g_macro_active].macro_return_filename_id)) {
      redefine("WLA_FILENAME", 0.0, get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id)));
      redefine("wla_filename", 0.0, get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id)));
    }

    g_active_file_info_last->filename_id = g_macro_stack[g_macro_active].macro_return_filename_id;
    g_active_file_info_last->line_current = g_macro_stack[g_macro_active].macro_return_line;

    /* was this the last macro called? */
    if (g_macro_active == 0) {
      /* delete NARGS */
      undefine("NARGS");
      undefine("nargs");

      g_macro_runtime_current = NULL;
    }
    else {
      /* redefine NARGS */
      if (redefine("NARGS", (double)g_macro_stack[g_macro_active - 1].supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
      if (redefine("nargs", (double)g_macro_stack[g_macro_active - 1].supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;

      g_macro_runtime_current = &g_macro_stack[g_macro_active - 1];
    }

    /* was this a DBM macro call? */
    if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DBM) {
      /* yep, get the output */
      if (macro_insert_byte_db("DBM") == FAILED)
        return FAILED;

      /* continue defining bytes */
      if (macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DBM, "DBM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DWM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DWM) {
      /* yep, get the output */
      if (macro_insert_word_db("DWM") == FAILED)
        return FAILED;

      /* continue defining words */
      if (macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DWM, "DWM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DLM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DLM) {
      /* yep, get the output */
      if (macro_insert_long_db("DLM") == FAILED)
        return FAILED;

      /* continue defining longs */
      if (macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DLM, "DLM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DDM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DDM) {
      /* yep, get the output */
      if (macro_insert_double_dw("DDM") == FAILED)
        return FAILED;

      /* continue defining double words */
      if (macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DDM, "DDM", NO) == FAILED)
        return FAILED;
    }
    /* was this a FILTER macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_FILTER) {
      /* continue running FILTER */
      if (macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_FILTER, "FILTER", NO) == FAILED)
        return FAILED;
    }
    /* or was this an INCBIN with a filter macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_INCBIN) {
      /* yep, get the output */
      if (macro_insert_byte_db("INCBIN") == FAILED)
        return FAILED;

      /* continue filtering the binary file */
      if (macro_start_incbin(g_macro_stack[g_macro_active].macro, NULL, NO) == FAILED)
        return FAILED;
    }

    return SUCCEEDED;
  }

  snprintf(g_error_message, sizeof(g_error_message), "No .MACRO open.\n");
  print_error(g_error_message, ERROR_DIR);

  return FAILED;
}


#if defined(W65816)

int directive_snesheader(void) {

  int token_result;
  
  if (g_snesheader_defined != 0) {
    print_error(".SNESHEADER can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_computesneschecksum_defined != 0)
    print_error(".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER is defined.\n", ERROR_WRN);
  else
    g_computesneschecksum_defined++;

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESHEADER.\n", ERROR_DIR);
    return FAILED;
  }

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      int q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDSNES") == 0)
      break;
    else if (strcaselesscmp(g_tmp, "ID") == 0) {
      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING || g_tmp[4] != 0) {
        print_error("ID requires a string of 1 to 4 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no ID has been defined so far */
      if (g_snesid_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 4; i++)
          g_snesid[i] = g_tmp[i];

        for ( ; i < 4; g_snesid[i] = 0, i++)
          ;

        g_snesid_defined = 1;
      }
      /* compare the IDs */
      else {
        int i;

        for (i = 0; g_tmp[i] != 0 && g_snesid[i] != 0 && i < 4; i++)
          if (g_snesid[i] != g_tmp[i])
            break;

        if (i == 4 && g_tmp[i] != 0) {
          print_error("ID requires a string of 1 to 4 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (i != 4 && (g_snesid[i] != 0 || g_tmp[i] != 0)) {
          print_error("ID was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }    
    else if (strcaselesscmp(g_tmp, "NAME") == 0) {
      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
        return FAILED;
      }

      /* no name has been defined so far */
      if (g_name_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 21; i++)
          g_name[i] = g_tmp[i];

        if (i == 21 && g_tmp[i] != 0) {
          print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
          return FAILED;
        }

        for ( ; i < 21; g_name[i] = 0, i++)
          ;

        g_name_defined = 1;
      }
      /* compare the names */
      else {
        int i;

        for (i = 0; g_tmp[i] != 0 && g_name[i] != 0 && i < 21; i++)
          if (g_name[i] != g_tmp[i])
            break;

        if (i == 21 && g_tmp[i] != 0) {
          print_error("NAME requires a string of 1 to 21 letters.\n", ERROR_DIR);
          return FAILED;
        }
        if (i != 21 && (g_name[i] != 0 || g_tmp[i] != 0)) {
          print_error("NAME was already defined.\n", ERROR_DIR);
          return FAILED;
        }
      }
    }
    else if (strcaselesscmp(g_tmp, "HIROM") == 0) {
      if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".HIROM");
        return FAILED;
      }

      g_hirom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "EXHIROM") == 0) {
      if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_hirom_defined != 0) {
        give_snes_rom_mode_defined_error(".EXHIROM");
        return FAILED;
      }

      g_exhirom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "LOROM") == 0) {
      if (g_hirom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".LOROM");
        return FAILED;
      }

      g_lorom_defined++;
    }
    /*
      else if (strcaselesscmp(g_tmp, "EXLOROM") == 0) {
      if (g_hirom_defined != 0 || g_lorom_defined != 0 || g_exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".EXLOROM");
      return FAILED;
      }

      g_exlorom_defined++;
      }
    */
    else if (strcaselesscmp(g_tmp, "SLOWROM") == 0) {
      if (g_fastrom_defined != 0) {
        print_error(".FASTROM was defined prior to .SLOWROM.\n", ERROR_DIR);
        return FAILED;
      }

      g_slowrom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "FASTROM") == 0) {
      if (g_slowrom_defined != 0) {
        print_error(".SLOWROM was defined prior to .FASTROM.\n", ERROR_DIR);
        return FAILED;
      }

      g_fastrom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "CARTRIDGETYPE") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "CARTRIDGETYPE expects 8-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_cartridgetype_defined != 0 && g_parsed_int != g_cartridgetype) {
          print_error("CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_cartridgetype = g_parsed_int;
        g_cartridgetype_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "ROMSIZE") == 0) {
      int number_result;

      if (g_snesromsize != 0) {
        print_error("ROMSIZE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "ROMSIZE expects 8-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED)
        g_snesromsize = g_parsed_int;
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "SRAMSIZE") == 0) {
      int number_result;

      if (g_sramsize_defined != 0) {
        print_error("SRAMSIZE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < 0 || g_parsed_int > 7)) {
        snprintf(g_error_message, sizeof(g_error_message), "SRAMSIZE expects 0-7, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        g_sramsize = g_parsed_int;
        g_sramsize_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "COUNTRY") == 0) {
      int number_result;

      if (g_country_defined != 0) {
        print_error("COUNTRY can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "COUNTRY expects 8-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        g_country = g_parsed_int;
        g_country_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "LICENSEECODE") == 0) {
      int number_result;

      if (g_licenseecode_defined != 0) {
        print_error("LICENSEECODE can be defined only once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "LICENSEECODE expects 8-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        g_licenseecode = g_parsed_int;
        g_licenseecode_defined++;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "VERSION") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        snprintf(g_error_message, sizeof(g_error_message), "VERSION expects 8-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_version_defined != 0 && g_version != g_parsed_int) {
          print_error("VERSION was defined for the second time.\n", ERROR_DIR);
          return FAILED;
        }

        g_version = g_parsed_int;
        g_version_defined++;
      }
      else
        return FAILED;
    }
    else {
      token_result = FAILED;
      break; 
    } 
  }

  if (token_result != SUCCEEDED) {
    print_error("Error in .SNESHEADER data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_snesheader_defined = 1;
  g_snes_mode++;

  return SUCCEEDED;
}


int directive_snesnativevector(void) {
  
  int cop_defined = 0, brk_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, unused_defined = 0, irq_defined = 0, q, token_result;
  char cop[512], brk[512], abort[512], nmi[512], unused[512], irq[512];

  if (g_snesnativevector_defined != 0) {
    print_error(".SNESNATIVEVECTOR can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
    print_error(".SNESNATIVEVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESNATIVEVECTOR.\n", ERROR_DIR);
    return FAILED;
  }

  /* create a section for the data */
  if (create_a_new_section_structure() == FAILED)
    return FAILED;
  strcpy(g_sec_tmp->name, "!__WLA_SNESNATIVEVECTOR");
  g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;
  
  if (g_lorom_defined || g_exlorom_defined)
    base_address = 0x7FE4;
  else if (g_hirom_defined)
    base_address = 0xFFE4;
  else if (g_exhirom_defined)
    base_address = 0x40FFE4;
    
  fprintf(g_file_out_ptr, "P O0 A%d %d ", g_sec_tmp->id, base_address);
  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDNATIVEVECTOR") == 0) {
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
      
      fprintf(g_file_out_ptr, "%s%s%s%s%s%s s p ", cop, brk, abort, nmi, unused, irq);
      
      break;
    }
    else if (strcaselesscmp(g_tmp, "COP") == 0) {
      int number_result;

      if (cop_defined != 0) {
        print_error("COP can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "COP expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(cop, sizeof(cop), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(cop, sizeof(cop), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(cop, sizeof(cop), "C%d ", g_latest_stack);

      cop_defined++;
    }
    else if (strcaselesscmp(g_tmp, "BRK") == 0) {
      int number_result;

      if (brk_defined != 0) {
        print_error("BRK can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "BRK expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(brk, sizeof(brk), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(brk, sizeof(brk), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(brk, sizeof(brk), "C%d ", g_latest_stack);

      brk_defined++;
    }
    else if (strcaselesscmp(g_tmp, "ABORT") == 0) {
      int number_result;

      if (abort_defined != 0) {
        print_error("ABORT can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "ABORT expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(abort, sizeof(abort), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(abort, sizeof(abort), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(abort, sizeof(abort), "C%d ", g_latest_stack);

      abort_defined++;
    }
    else if (strcaselesscmp(g_tmp, "NMI") == 0) {
      int number_result;

      if (nmi_defined != 0) {
        print_error("NMI can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "NMI expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(nmi, sizeof(nmi), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(nmi, sizeof(nmi), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(nmi, sizeof(nmi), "C%d ", g_latest_stack);

      nmi_defined++;
    }
    else if (strcaselesscmp(g_tmp, "UNUSED") == 0) {
      int number_result;

      if (unused_defined != 0) {
        print_error("UNUSED can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "UNUSED expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(unused, sizeof(unused), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(unused, sizeof(unused), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(unused, sizeof(unused), "C%d ", g_latest_stack);

      unused_defined++;
    }
    else if (strcaselesscmp(g_tmp, "IRQ") == 0) {
      int number_result;

      if (irq_defined != 0) {
        print_error("IRQ can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "IRQ expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(irq, sizeof(irq), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(irq, sizeof(irq), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(irq, sizeof(irq), "C%d ", g_latest_stack);

      irq_defined++;
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error("Error in .SNESNATIVEVECTOR data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_snesnativevector_defined = 1;
  g_snes_mode++;

  return SUCCEEDED;
}


int directive_snesemuvector(void) {
  
  int cop_defined = 0, unused_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, reset_defined = 0, irqbrk_defined = 0, q, token_result;
  char cop[512], unused[512], abort[512], nmi[512], reset[512], irqbrk[512];

  if (g_snesemuvector_defined != 0) {
    print_error(".SNESEMUVECTOR can be defined only once.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
    print_error(".SNESEMUVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY) {
    print_error("Libraries don't take .SNESEMUVECTOR.\n", ERROR_DIR);
    return FAILED;
  }

  /* create a section for the data */
  if (create_a_new_section_structure() == FAILED)
    return FAILED;
  strcpy(g_sec_tmp->name, "!__WLA_SNESEMUVECTOR");
  g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;

  if (g_lorom_defined || g_exlorom_defined)
    base_address = 0x7FF4;
  else if (g_hirom_defined)
    base_address = 0xFFF4;
  else if (g_exhirom_defined)
    base_address = 0x40FFF4;

  fprintf(g_file_out_ptr, "P O0 A%d %d ", g_sec_tmp->id, base_address);
  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  while ((token_result = get_next_token()) == SUCCEEDED) {
    /* .IF directive? */
    if (g_tmp[0] == '.') {
      q = parse_if_directive();
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED)
        continue;
      /* else q == -1, continue executing below */
    }

    if (strcaselesscmp(g_tmp, ".ENDEMUVECTOR") == 0) {
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

      fprintf(g_file_out_ptr, "%s%s%s%s%s%s s p ", cop, unused, abort, nmi, reset, irqbrk);
      
      break;
    }
    else if (strcaselesscmp(g_tmp, "COP") == 0) {
      int number_result;

      if (cop_defined != 0) {
        print_error("COP can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "COP expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(cop, sizeof(cop), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(cop, sizeof(cop), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(cop, sizeof(cop), "C%d ", g_latest_stack);

      cop_defined++;
    }
    else if (strcaselesscmp(g_tmp, "RESET") == 0) {
      int number_result;

      if (reset_defined != 0) {
        print_error("RESET can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "RESET expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(reset, sizeof(reset), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(reset, sizeof(reset), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(reset, sizeof(reset), "C%d ", g_latest_stack);

      reset_defined++;
    }
    else if (strcaselesscmp(g_tmp, "ABORT") == 0) {
      int number_result;

      if (abort_defined != 0) {
        print_error("ABORT can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "ABORT expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(abort, sizeof(abort), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(abort, sizeof(abort), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(abort, sizeof(abort), "C%d ", g_latest_stack);

      abort_defined++;
    }
    else if (strcaselesscmp(g_tmp, "NMI") == 0) {
      int number_result;

      if (nmi_defined != 0) {
        print_error("NMI can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "NMI expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(nmi, sizeof(nmi), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(nmi, sizeof(nmi), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(nmi, sizeof(nmi), "C%d ", g_latest_stack);

      nmi_defined++;
    }
    else if (strcaselesscmp(g_tmp, "UNUSED") == 0) {
      int number_result;

      if (unused_defined != 0) {
        print_error("UNUSED can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "UNUSED expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(unused, sizeof(unused), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(unused, sizeof(unused), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(unused, sizeof(unused), "C%d ", g_latest_stack);

      unused_defined++;
    }
    else if (strcaselesscmp(g_tmp, "IRQBRK") == 0) {
      int number_result;

      if (irqbrk_defined != 0) {
        print_error("IRQBRK can only be defined once.\n", ERROR_DIR);
        return FAILED;
      }

      number_result = input_number();
      
      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        snprintf(g_error_message, sizeof(g_error_message), "IRQBRK expects 16-bit data, %d is out of range!\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      if (number_result == SUCCEEDED)
        snprintf(irqbrk, sizeof(irqbrk), "y%d ", g_parsed_int);
      else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
        snprintf(irqbrk, sizeof(irqbrk), "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (number_result == INPUT_NUMBER_STACK)
        snprintf(irqbrk, sizeof(irqbrk), "C%d ", g_latest_stack);

      irqbrk_defined++;
    }
    else {
      token_result = FAILED;
      break;
    }
  }
  
  if (token_result != SUCCEEDED) {
    print_error("Error in .SNESEMUVECTOR data structure.\n", ERROR_DIR);
    return FAILED;
  }

  g_snesemuvector_defined = 1;
  g_snes_mode++;

  return SUCCEEDED;
}

#endif


int directive_print(void) {

  int get_value, value_type, number_result;

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

    number_result = input_number();

    if (number_result == INPUT_NUMBER_STRING || number_result == INPUT_NUMBER_ADDRESS_LABEL) {
      if (get_value == YES) {
        print_error(".PRINT was expecting a value, got a string/label instead.\n", ERROR_INP);
        return FAILED;
      }

      if (g_quiet == NO) {
        printf("%s", g_label);
        fflush(stdout);
      }
    }
    else if (number_result == SUCCEEDED) {
      if (g_quiet == NO) {
        if (value_type == 0)
          printf("%x", g_parsed_int);
        else
          printf("%d", g_parsed_int);
        fflush(stdout);
      }
    }
    else if (number_result == INPUT_NUMBER_EOL) {
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
  
  int number_result;
    
  number_result = input_number();

  if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".PRINTT needs a string/label.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_quiet == NO) {
    printf("%s", g_label);
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
      snprintf(g_error_message, sizeof(g_error_message), "\"%s\" is not known.\n", g_label);
      print_error(g_error_message, ERROR_DIR);
    }
    print_error(".PRINTV can only print currently known values.\n", ERROR_DIR);
    return FAILED;
  }

  if (g_quiet == NO) {
    if (m == 0)
      printf("%x", g_parsed_int);
    else
      printf("%d", g_parsed_int);
    fflush(stdout);
  }

  return SUCCEEDED;
}


int directive_dbrnd_dwrnd(void) {
  
  int o, c, min, max, f, q;

  /* bytes or words? */
  if (g_current_directive[1] == 'W')
    o = 1;
  else
    o = 0;

  /* get the count */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs the number of random numbers.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  c = g_parsed_int;

  if (c <= 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs that the number of random numbers is > 0.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* get min */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs the min value.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  min = g_parsed_int;

  /* get max */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs the max value.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  max = g_parsed_int;

  if (min >= max) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs that min < max.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  /* generate the numbers */
  for (f = 0; f < c; f++) {
    q = (genrand_int32() % (max-min+1)) + min;
    
    if (o == 1) {
      if (q < -32768 || q > 65535) {
        snprintf(g_error_message, sizeof(g_error_message), ".%s: Expected a 16-bit value, computed %d.\n", g_current_directive, q);
        print_error(g_error_message, ERROR_NONE);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "y %d ", q);
    }
    else {
      if (q > 255 || q < -128) {
        snprintf(g_error_message, sizeof(g_error_message), ".%s: Expected a 8-bit value, computed %d.\n", g_current_directive, q);
        print_error(g_error_message, ERROR_NONE);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "d%d ", q);
    }
  }

  return SUCCEEDED;
}


int directive_dwsin_dbsin_dwcos_dbcos(void) {
  
  double m, a, s, n;
  int p, c, o, f, value;

  if (g_current_directive[1] == 'W')
    o = 1;
  else
    o = 2;
  if (g_current_directive[2] == 'S')
    f = 1;
  else
    f = 2;

  g_input_float_mode = ON;
  p = input_number();
  g_input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs a value for starting angle.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    a = g_parsed_int;
  else
    a = g_parsed_double;

  if (input_number() != SUCCEEDED || g_parsed_int < 0) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs an non-negative integer value for additional angles.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  c = g_parsed_int;

  g_input_float_mode = ON;
  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs a value for angle step.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    s = g_parsed_int;
  else
    s = g_parsed_double;

  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs a value to multiply the result with.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    m = g_parsed_int;
  else
    m = g_parsed_double;

  p = input_number();
  g_input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    snprintf(g_error_message, sizeof(g_error_message), ".%s needs a value to add to the result.\n", g_current_directive);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  if (p == SUCCEEDED)
    n = g_parsed_int;
  else
    n = g_parsed_double;

  for (c++; c > 0; c--) {
    while (a >= 360)
      a -= 360;

    if (f == 1)
      value = (int)((m * sin(a*M_PI/180)) + n);
    else
      value = (int)((m * cos(a*M_PI/180)) + n);

    if (o == 1) {
      if (value < -32768 || value > 65535) {
        snprintf(g_error_message, sizeof(g_error_message), ".%s: Expected a 16-bit value, computed %d.\n", g_current_directive, value);
        print_error(g_error_message, ERROR_NONE);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "y %d ", value);
    }
    else {
      if (value > 255 || value < -128) {
        snprintf(g_error_message, sizeof(g_error_message), ".%s: Expected a 8-bit value, computed %d.\n", g_current_directive, value);
        print_error(g_error_message, ERROR_NONE);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "d%d ", value);
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

  g_expect_calculations = NO;
  parse_result = input_number();
  g_expect_calculations = YES;

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
  map->next = g_stringmaptables;
  g_stringmaptables = map;

  strcpy(map->name, g_label);

  g_expect_calculations = NO;
  parse_result = input_number();
  g_expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".STRINGMAPTABLE needs a file name string.\n", ERROR_DIR);
    return FAILED;
  }

  map->filename = calloc(strlen(g_label) + 1, 1);
  if (map->filename == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Out of memory while trying allocate info structure for file \"%s\".\n", g_full_name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }
  strcpy(map->filename, g_label);

  table_file = fopen(g_label, "r");
  if (table_file == NULL) {
    if (g_makefile_rules == YES) {
      // If in makefile mode, this is not an error. We just make an empty map.
      return SUCCEEDED;
    }
    snprintf(g_error_message, sizeof(g_error_message), "Error opening file \"%s\".\n", g_label);
    print_error(g_error_message, ERROR_DIR);
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
      snprintf(g_error_message, sizeof(g_error_message), "STRINGMAPTABLE: No text before '=' at line %d of file \"%s\".\n", line_number, g_label);
      print_error(g_error_message, ERROR_DIR);
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
        snprintf(g_error_message, sizeof(g_error_message), "STRINGMAPTABLE: Invalid hex character '%c' at line %d of file \"%s\".\n", c, line_number, g_label);
        print_error(g_error_message, ERROR_DIR);
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
      snprintf(g_error_message, sizeof(g_error_message), "STRINGMAPTABLE: no text after '=' at line %d of file \"%s\".\n", line_number, g_label);
      print_error(g_error_message, ERROR_DIR);
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
  g_expect_calculations = NO;
  parse_result = input_number();
  g_expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(".STRINGMAP needs a table name.\n", ERROR_DIR);
    return FAILED;
  }

  /* find the table */
  for (table = g_stringmaptables; table != NULL; table = table->next) {
    if (strcaselesscmp(table->name, g_label) == 0) {
      /* found it */
      break;
    }
  }
  if (table == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "STRINGMAP: could not find table called \"%s\".\n", g_label);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;    
  }

  /* parse the string */
  parse_result = input_number();
  if (parse_result != INPUT_NUMBER_STRING) {
    print_error("STRINGMAP: no string given", ERROR_DIR);
    return FAILED;    
  }

  fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);

  /* parse it */
  for (p = g_label; *p != 0; /* increment in loop */) {
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
      if (g_makefile_rules == YES)
      {
	    /* In makefile mode, it's ignored */
        return SUCCEEDED;
      }
      snprintf(g_error_message, sizeof(g_error_message), "STRINGMAP: could not find a match in the table at substring \"%s\".\n", p);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;    
    }
    /* else emit */
    for (i = 0; i < entry->bytes_length; ++i)
      fprintf(g_file_out_ptr, "d%d ", entry->bytes[i]);

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

  if (strcaselesscmp(g_current_directive, "ORG") == 0)
    return directive_org();

  /* ORGA */

  if (strcaselesscmp(g_current_directive, "ORGA") == 0)
    return directive_orga();

  /* SLOT */

  if (strcaselesscmp(g_current_directive, "SLOT") == 0)
    return directive_slot();

  /* BANK */

  if (strcaselesscmp(g_current_directive, "BANK") == 0)
    return directive_bank();

  /* DBM/DWM? */

  if (strcaselesscmp(g_current_directive, "DBM") == 0 || strcaselesscmp(g_current_directive, "DWM") == 0)
    return directive_dbm_dwm_dlm_ddm_filter();

  /* TABLE? */

  if (strcaselesscmp(g_current_directive, "TABLE") == 0)
    return directive_table();

  /* FILTER? */

  if (strcaselesscmp(g_current_directive, "FILTER") == 0)
    return directive_dbm_dwm_dlm_ddm_filter();
    
  /* ROW/DATA? */

  if (strcaselesscmp(g_current_directive, "ROW") == 0 || strcaselesscmp(g_current_directive, "DATA") == 0)
    return directive_row_data();

  /* DB/BYT/BYTE? */

  if (strcaselesscmp(g_current_directive, "DB") == 0 || strcaselesscmp(g_current_directive, "BYT") == 0 || strcaselesscmp(g_current_directive, "BYTE") == 0)
    return directive_db_byt_byte();

  /* HEX? */

  if (strcaselesscmp(g_current_directive, "HEX") == 0)
    return directive_hex();

  /* BITS? */

  if (strcaselesscmp(g_current_directive, "BITS") == 0)
    return directive_bits();

  /* ENDBITS? */

  if (strcaselesscmp(g_current_directive, "ENDBITS") == 0) {
    fprintf(g_file_out_ptr, "+999 ");

    return SUCCEEDED;
  }
  
  /* ASCTABLE/ASCIITABLE? */

  if (strcaselesscmp(g_current_directive, "ASCTABLE") == 0 || strcaselesscmp(g_current_directive, "ASCIITABLE") == 0)
    return directive_asctable_asciitable();

  /* ASC/ASCSTR? */

  if (strcaselesscmp(g_current_directive, "ASC") == 0 || strcaselesscmp(g_current_directive, "ASCSTR") == 0)
    return directive_asc();

  /* DW/WORD/ADDR? */

  if (strcaselesscmp(g_current_directive, "DW") == 0 || strcaselesscmp(g_current_directive, "WORD") == 0 || strcaselesscmp(g_current_directive, "ADDR") == 0)
    return directive_dw_word_addr();
  
  /* DLM? */

  if (strcaselesscmp(g_current_directive, "DLM") == 0)
    return directive_dbm_dwm_dlm_ddm_filter();

  /* DL/LONG/FARADDR? */

  if (strcaselesscmp(g_current_directive, "DL") == 0 || strcaselesscmp(g_current_directive, "LONG") == 0 || strcaselesscmp(g_current_directive, "FARADDR") == 0)
    return directive_dl_long_faraddr();

  /* DSL? */

  if (strcaselesscmp(g_current_directive, "DSL") == 0)
    return directive_dsl();

  /* ARRAYDEF/ARRAYDEFINE? */

  if (strcaselesscmp(g_current_directive, "ARRAYDEF") == 0 || strcaselesscmp(g_current_directive, "ARRAYDEFINE") == 0)
    return directive_arraydef_arraydefine();

  /* ARRAYIN? */

  if (strcaselesscmp(g_current_directive, "ARRAYIN") == 0)
    return directive_arrayin();

  /* ARRAYOUT? */

  if (strcaselesscmp(g_current_directive, "ARRAYOUT") == 0)
    return directive_arrayout();

  /* ARRAYDB/ARRAYDW/ARRAYDL/ARRAYDD? */

  if (strcaselesscmp(g_current_directive, "ARRAYDB") == 0 || strcaselesscmp(g_current_directive, "ARRAYDW") == 0 || strcaselesscmp(g_current_directive, "ARRAYDL") == 0 || strcaselesscmp(g_current_directive, "ARRAYDD") == 0)
    return directive_arraydb_arraydw_arraydl_arraydd();

  /* DDM? */

  if (strcaselesscmp(g_current_directive, "DDM") == 0)
    return directive_dbm_dwm_dlm_ddm_filter();

  /* DD? */

  if (strcaselesscmp(g_current_directive, "DD") == 0)
    return directive_dd();

  /* DSD? */

  if (strcaselesscmp(g_current_directive, "DSD") == 0)
    return directive_dsd();

#if defined(W65816)

  /* NAME */

  if (strcaselesscmp(g_current_directive, "NAME") == 0)
    return directive_name_w65816();

  /* WDC */

  if (strcaselesscmp(g_current_directive, "WDC") == 0) {
    g_use_wdc_standard = 1;
    return SUCCEEDED;
  }

  /* NOWDC */

  if (strcaselesscmp(g_current_directive, "NOWDC") == 0) {
    g_use_wdc_standard = 0;
    return SUCCEEDED;
  }
  
#endif

  /* DSTRUCT */

  if (strcaselesscmp(g_current_directive, "DSTRUCT") == 0)
    return directive_dstruct();

  /* DS/DSB? */

  if (strcaselesscmp(g_current_directive, "DSB") == 0 || strcaselesscmp(g_current_directive, "DS") == 0)
    return directive_dsb_ds();

  /* DSW? */

  if (strcaselesscmp(g_current_directive, "DSW") == 0)
    return directive_dsw();
    
  /* INCDIR */

  if (strcaselesscmp(g_current_directive, "INCDIR") == 0)
    return directive_incdir();

  /* INCLUDE/INC */

  if (strcaselesscmp(g_current_directive, "INCLUDE") == 0 || strcaselesscmp(g_current_directive, "INC") == 0)
    return directive_include(YES);

  /* INDLUDE/IND (INTERNAL) */

  if (strcaselesscmp(g_current_directive, "INDLUDE") == 0 || strcaselesscmp(g_current_directive, "IND") == 0)
    return directive_include(NO);

  /* INCBIN */

  if (strcaselesscmp(g_current_directive, "INCBIN") == 0)
    return directive_incbin();

  /* OUTNAME */

  if (strcaselesscmp(g_current_directive, "OUTNAME") == 0) {
    int number_result;

    g_expect_calculations = NO;
    number_result = input_number();
    g_expect_calculations = YES;

    if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".OUTNAME needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(g_final_name, g_label);

    return SUCCEEDED;
  }

  /* STRUCT */

  if (strcaselesscmp(g_current_directive, "STRUCT") == 0)
    return directive_struct();

  /* RAMSECTION */

  if (strcaselesscmp(g_current_directive, "RAMSECTION") == 0)
    return directive_ramsection();

  /* SECTION */

  if (strcaselesscmp(g_current_directive, "SECTION") == 0)
    return directive_section();

  /* FOPEN */

  if (strcaselesscmp(g_current_directive, "FOPEN") == 0)
    return directive_fopen();

  /* FCLOSE */

  if (strcaselesscmp(g_current_directive, "FCLOSE") == 0)
    return directive_fclose();

  /* FSIZE */

  if (strcaselesscmp(g_current_directive, "FSIZE") == 0)
    return directive_fsize();

  /* FREAD */

  if (strcaselesscmp(g_current_directive, "FREAD") == 0)
    return directive_fread();

  /* FTELL */
  
  if (strcaselesscmp(g_current_directive, "FTELL") == 0)
    return directive_ftell();

  /* FSEEK */

  if (strcaselesscmp(g_current_directive, "FSEEK") == 0)
    return directive_fseek();
  
  /* BLOCK */

  if (strcaselesscmp(g_current_directive, "BLOCK") == 0)
    return directive_block();

  /* ENDB */

  if (strcaselesscmp(g_current_directive, "ENDB") == 0) {
    if (g_block_status <= 0) {
      print_error("There is no open .BLOCK.\n", ERROR_DIR);
      return FAILED;
    }

    g_block_status--;
    fprintf(g_file_out_ptr, "G ");

    return SUCCEEDED;
  }

  /* SHIFT */

  if (strcaselesscmp(g_current_directive, "SHIFT") == 0)
    return directive_shift();

  /* ENDS */

  if (strcaselesscmp(g_current_directive, "ENDS") == 0) {
    if (g_section_status == OFF) {
      print_error("There is no open section.\n", ERROR_DIR);
      return FAILED;
    }
    if (g_dstruct_status == ON) {
      print_error("You can't close a section inside .DSTRUCT.\n", ERROR_DIR);
      return FAILED;
    }

    /* generate a section end label? */
    if (g_extra_definitions == ON)
      generate_label("SECTIONEND_", g_sections_last->name);
  
    g_section_status = OFF;
    g_bankheader_status = OFF;
    g_in_ramsection = NO;
    
    fprintf(g_file_out_ptr, "s ");

    return SUCCEEDED;
  }

  /* ROMBANKS */

  if (strcaselesscmp(g_current_directive, "ROMBANKS") == 0)
    return directive_rombanks();

  /* ROMBANKMAP */

  if (strcaselesscmp(g_current_directive, "ROMBANKMAP") == 0)
    return directive_rombankmap();

  /* MEMORYMAP */

  if (strcaselesscmp(g_current_directive, "MEMORYMAP") == 0)
    return directive_memorymap();

  /* UNBACKGROUND */

  if (strcaselesscmp(g_current_directive, "UNBACKGROUND") == 0)
    return directive_unbackground();

  /* BACKGROUND */

  if (strcaselesscmp(g_current_directive, "BACKGROUND") == 0)
    return directive_background();

#ifdef GB

  /* NINTENDOLOGO */
  
  if (strcaselesscmp(g_current_directive, "NINTENDOLOGO") == 0) {
    no_library_files(".NINTENDOLOGO");

    g_nintendologo_defined++;

    return SUCCEEDED;
  }
  
  /* NAME */

  if (strcaselesscmp(g_current_directive, "NAME") == 0)
    return directive_name_gb();

  /* RAMSIZE */

  if (strcaselesscmp(g_current_directive, "RAMSIZE") == 0) {
    no_library_files(".RAMSIZE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".RAMSIZE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_rambanks_defined != 0) {
      if (g_rambanks != g_parsed_int) {
        print_error(".RAMSIZE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    if (g_parsed_int != 0 && g_parsed_int != 1 && g_parsed_int != 2 && g_parsed_int != 3 && g_parsed_int != 4 && g_parsed_int != 5) {
      print_error("Unsupported RAM size.\n", ERROR_DIR);
      return FAILED;
    }

    g_rambanks = g_parsed_int;
    g_rambanks_defined = 1;

    return SUCCEEDED;
  }

  /* COUNTRYCODE */

  if (strcaselesscmp(g_current_directive, "COUNTRYCODE") == 0) {
    no_library_files(".COUNTRYCODE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".COUNTRYCODE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_countrycode_defined != 0) {
      if (g_countrycode != g_parsed_int) {
        print_error(".COUNTRYCODE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    g_countrycode = g_parsed_int;
    g_countrycode_defined = 1;

    return SUCCEEDED;
  }
    
  /* DESTINATIONCODE */

  if (strcaselesscmp(g_current_directive, "DESTINATIONCODE") == 0) {
    no_library_files(".DESTINATIONCODE");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || q < 0) {
      print_error(".DESTINATIONCODE needs a non-negative value.\n", ERROR_DIR);
      return FAILED;
    }

    if (q == SUCCEEDED && g_countrycode_defined != 0) {
      if (g_countrycode != g_parsed_int) {
        print_error(".DESTINATIONCODE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
    }

    g_countrycode = g_parsed_int;
    g_countrycode_defined = 1;

    return SUCCEEDED;
  }

  /* CARTRIDGETYPE */

  if (strcaselesscmp(g_current_directive, "CARTRIDGETYPE") == 0) {
    no_library_files(".CARTRIDGETYPE");

    q = input_number();

    if (q == SUCCEEDED && g_cartridgetype_defined != 0) {
      if (g_cartridgetype != g_parsed_int) {
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

    g_cartridgetype = g_parsed_int;
    g_cartridgetype_defined = 1;

    return SUCCEEDED;
  }

  /* LICENSEECODENEW */

  if (strcaselesscmp(g_current_directive, "LICENSEECODENEW") == 0) {
    int token_result;

    no_library_files(".LICENSEECODENEW");
    
    if (g_licenseecodeold_defined != 0) {
      print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
      return FAILED;
    }

    if ((token_result = get_next_token()) == FAILED)
      return FAILED;

    if (token_result != GET_NEXT_TOKEN_STRING) {
      print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
      return FAILED;
    }
    if (!(g_tmp[0] != 0 && g_tmp[1] != 0 && g_tmp[2] == 0)) {
      print_error(".LICENSEECODENEW requires a string of two letters.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_licenseecodenew_defined != 0) {
      if (g_tmp[0] != g_licenseecodenew_c1 || g_tmp[1] != g_licenseecodenew_c2) {
        print_error(".LICENSEECODENEW was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    g_licenseecodenew_c1 = g_tmp[0];
    g_licenseecodenew_c2 = g_tmp[1];
    g_licenseecodenew_defined = 1;

    return SUCCEEDED;
  }

  /* LICENSEECODEOLD */

  if (strcaselesscmp(g_current_directive, "LICENSEECODEOLD") == 0) {
    no_library_files(".LICENSEECODEOLD");
    
    if (g_licenseecodenew_defined != 0) {
      print_error(".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
      snprintf(g_error_message, sizeof(g_error_message), ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (g_licenseecodeold_defined != 0) {
      if (g_licenseecodeold != g_parsed_int) {
        print_error(".LICENSEECODEOLD was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    g_licenseecodeold = g_parsed_int;
    g_licenseecodeold_defined = 1;

    return SUCCEEDED;
  }

  /* VERSION */

  if (strcaselesscmp(g_current_directive, "VERSION") == 0) {
    no_library_files(".VERSION");
    
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
      snprintf(g_error_message, sizeof(g_error_message), ".VERSION needs a 8-bit value, got %d.\n", g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (g_version_defined != 0) {
      if (g_version != g_parsed_int) {
        print_error(".VERSION was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    g_version = g_parsed_int;
    g_version_defined = 1;

    return SUCCEEDED;
  }

  /* GBHEADER */

  if (strcmp(g_current_directive, "GBHEADER") == 0)
    return directive_gbheader();

#endif

  /* EMPTYFILL */

  if (strcaselesscmp(g_current_directive, "EMPTYFILL") == 0) {
    no_library_files(".EMPTYFILL");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
      snprintf(g_error_message, sizeof(g_error_message), ".EMPTYFILL needs a 8-bit value, got %d.\n", g_parsed_int);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (g_emptyfill_defined != 0) {
      if (g_emptyfill != g_parsed_int) {
        print_error(".EMPTYFILL was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    g_emptyfill = g_parsed_int;
    g_emptyfill_defined = 1;

    return SUCCEEDED;
  }

  /* DEFINE/DEF/EQU */

  if (strcaselesscmp(g_current_directive, "DEFINE") == 0 || strcaselesscmp(g_current_directive, "DEF") == 0 || strcaselesscmp(g_current_directive, "EQU") == 0)
    return directive_define_def_equ();

  /* ENUMID */

  if (strcaselesscmp(g_current_directive, "ENUMID") == 0)
    return directive_enumid();

  /* INPUT */

  if (strcaselesscmp(g_current_directive, "INPUT") == 0)
    return directive_input();

  /* REDEFINE/REDEF */

  if (strcaselesscmp(g_current_directive, "REDEFINE") == 0 || strcaselesscmp(g_current_directive, "REDEF") == 0)
    return directive_redefine_redef();

  /* EXPORT */

  if (strcaselesscmp(g_current_directive, "EXPORT") == 0) {
    q = 0;
    while (1) {
      int string_result;

      string_result = input_next_string();
      if (string_result == FAILED)
        return FAILED;
      if (string_result == INPUT_NUMBER_EOL) {
        if (q != 0) {
          next_line();
          return SUCCEEDED;
        }
        print_error(".EXPORT requires definition name(s).\n", ERROR_DIR);
        return FAILED;
      }

      q++;

      if (export_a_definition(g_tmp) == FAILED)
        return FAILED;
    }

    return FAILED;
  }

  /* SYM/SYMBOL */

  if (strcaselesscmp(g_current_directive, "SYM") == 0 || strcaselesscmp(g_current_directive, "SYMBOL") == 0) {
    if (input_next_string() != SUCCEEDED) {
      print_error(".SYM requires a symbol name.\n", ERROR_DIR);
      return FAILED;
    }

    fprintf(g_file_out_ptr, "Y%s ", g_tmp);

    return SUCCEEDED;
  }

  /* BR/BREAKPOINT */

  if (strcaselesscmp(g_current_directive, "BR") == 0 || strcaselesscmp(g_current_directive, "BREAKPOINT") == 0) {
    fprintf(g_file_out_ptr, "Z ");
    return SUCCEEDED;
  }

  /* ENUM */

  if (strcaselesscmp(g_current_directive, "ENUM") == 0) {
    if (g_dstruct_status == ON) {
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

    g_enum_offset = 0;
    g_last_enum_offset = 0;
    g_max_enum_offset = 0;
    g_base_enum_offset = g_parsed_int;

    /* "ASC" or "DESC"? */
    if (compare_next_token("ASC") == SUCCEEDED) {
      g_enum_ord = 1;
      skip_next_token();
    }
    else if (compare_next_token("DESC") == SUCCEEDED) {
      g_enum_ord = -1;
      skip_next_token();
    }
    else
      g_enum_ord = 1;

    /* do we have "EXPORT" defined? */
    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();
      g_enum_exp = YES;
    }
    else
      g_enum_exp = NO;

    /* setup g_active_struct (enum vars stored here temporarily) */
    g_active_struct = calloc(sizeof(struct structure), 1);
    if (g_active_struct == NULL) {
      print_error("Out of memory while parsing .ENUM.\n", ERROR_DIR);
      return FAILED;
    }
    g_active_struct->alive = YES;
    _remember_new_structure(g_active_struct);
    g_active_struct->name[0] = '\0';
    g_active_struct->items = NULL;
    g_active_struct->last_item = NULL;
    g_union_stack = NULL;

    g_in_enum = YES;

    return SUCCEEDED;
  }

#ifdef GB

  /* COMPUTEGBCHECKSUM */

  if (strcaselesscmp(g_current_directive, "COMPUTECHECKSUM") == 0 || strcaselesscmp(g_current_directive, "COMPUTEGBCHECKSUM") == 0) {
    no_library_files(".COMPUTEGBCHECKSUM");
    
    if (g_gbheader_defined != 0)
      print_error(".COMPUTEGBCHECKSUM is unnecessary when GBHEADER is defined.\n", ERROR_WRN);

    g_computechecksum_defined = 1;

    return SUCCEEDED;
  }

  /* COMPUTEGBCOMPLEMENTCHECK */

  if (strcaselesscmp(g_current_directive, "COMPUTEGBCOMPLEMENTCHECK") == 0 || strcaselesscmp(g_current_directive, "COMPUTECOMPLEMENTCHECK") == 0) {
    no_library_files(".COMPUTEGBCOMPLEMENTCHECK");
    
    if (g_gbheader_defined != 0)
      print_error(".COMPUTEGBCOMPLEMENTCHECK is unnecessary when GBHEADER is defined.\n", ERROR_WRN);

    g_computecomplementcheck_defined = 1;

    return SUCCEEDED;
  }
  
#endif

#if defined(W65816)

  /* COMPUTESNESCHECKSUM */

  if (strcaselesscmp(g_current_directive, "COMPUTESNESCHECKSUM") == 0) {
    no_library_files(".COMPUTESNESCHECKSUM");
    
    if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
      print_error(".COMPUTESNESCHECKSUM needs .LOROM, .HIROM or .EXHIROM defined earlier.\n", ERROR_DIR);
      return FAILED;
    }
    if (g_snesheader_defined != 0) 
      print_error(".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER defined.\n", ERROR_WRN);

    g_computesneschecksum_defined = 1;

    return SUCCEEDED;
  }

#endif

#ifdef Z80

  /* COMPUTESMSCHECKSUM */

  if (strcaselesscmp(g_current_directive, "COMPUTESMSCHECKSUM") == 0) {
    no_library_files(".COMPUTESMSCHECKSUM");

    g_computesmschecksum_defined++;

    return SUCCEEDED;
  }

  /* SMSTAG */

  if (strcaselesscmp(g_current_directive, "SMSTAG") == 0) {
    no_library_files(".SMSTAG");

    g_smstag_defined++;
    g_computesmschecksum_defined++;

    return SUCCEEDED;
  }

  /* SMSHEADER */
  if (strcmp(g_current_directive, "SMSHEADER") == 0)
    return directive_smsheader();
  
  /* SDSCTAG */

  if (strcaselesscmp(g_current_directive, "SDSCTAG") == 0)
    return directive_sdsctag();

#endif

  /* MACRO */

  if (strcaselesscmp(g_current_directive, "MACRO") == 0)
    return directive_macro();

  /* REPT/REPEAT */

  if (strcaselesscmp(g_current_directive, "REPT") == 0 || strcaselesscmp(g_current_directive, "REPEAT") == 0)
    return directive_rept_repeat();

  /* ENDR */

  if (strcaselesscmp(g_current_directive, "ENDR") == 0) {

    struct repeat_runtime *rr;
    
    if (g_repeat_active == 0) {
      print_error("There is no open repetition.\n", ERROR_DIR);
      return FAILED;
    }

    rr = &g_repeat_stack[g_repeat_active - 1];

    rr->counter--;
    if (rr->counter == 0) {
      g_repeat_active--;
      
      /* repeat end */
      fprintf(g_file_out_ptr, "J ");

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
    
    g_source_pointer = rr->start;
    g_active_file_info_last->line_current = rr->start_line;

    return SUCCEEDED;
  }

  /* CHANGEFILE (INTERNAL) */
  if (strcaselesscmp(g_current_directive, "CHANGEFILE") == 0) {
    q = input_number();
    if (q != SUCCEEDED) {
      print_error("Internal error in (internal) .CHANGEFILE. Please submit a bug report...\n", ERROR_DIR);
      return FAILED;
    }
    
    g_active_file_info_tmp = calloc(sizeof(struct active_file_info), 1);
    if (g_active_file_info_tmp == NULL) {
      snprintf(g_error_message, sizeof(g_error_message), "Out of memory while trying allocate error tracking data structure.\n");
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }
    g_active_file_info_tmp->next = NULL;

    if (g_active_file_info_first == NULL) {
      g_active_file_info_first = g_active_file_info_tmp;
      g_active_file_info_last = g_active_file_info_tmp;
      g_active_file_info_tmp->prev = NULL;
    }
    else {
      g_active_file_info_tmp->prev = g_active_file_info_last;
      g_active_file_info_last->next = g_active_file_info_tmp;
      g_active_file_info_last = g_active_file_info_tmp;
    }

    g_active_file_info_tmp->line_current = 0;
    g_active_file_info_tmp->filename_id = g_parsed_int;

    if (g_extra_definitions == ON) {
      g_file_name_info_tmp = g_file_name_info_first;
      while (g_file_name_info_tmp != NULL) {
        if (g_file_name_info_tmp->id == g_parsed_int)
          break;
        g_file_name_info_tmp = g_file_name_info_tmp->next;
      }

      if (g_file_name_info_tmp == NULL) {
        snprintf(g_error_message, sizeof(g_error_message), "Internal error: Could not find the name of file %d.\n", g_parsed_int);
        print_error(g_error_message, ERROR_DIR);
        return FAILED;
      }

      redefine("WLA_FILENAME", 0.0, g_file_name_info_tmp->name, DEFINITION_TYPE_STRING, (int)strlen(g_file_name_info_tmp->name));
      redefine("wla_filename", 0.0, g_file_name_info_tmp->name, DEFINITION_TYPE_STRING, (int)strlen(g_file_name_info_tmp->name));
    }

    /* output the file id */
    fprintf(g_file_out_ptr, "f%d ", g_active_file_info_tmp->filename_id);
    
    g_open_files++;

    if (compare_next_token("NAMESPACE") == SUCCEEDED) {
      skip_next_token();

      g_expect_calculations = NO;
      q = input_number();
      g_expect_calculations = YES;
    
      if (q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
        print_error("Internal error: Namespace string is missing.\n", ERROR_DIR);
        return FAILED;
      }

      strcpy(g_active_file_info_tmp->namespace, g_label);

      fprintf(g_file_out_ptr, "t1 %s ", g_active_file_info_tmp->namespace);
    }
    else if (compare_next_token("NONAMESPACE") == SUCCEEDED) {
      skip_next_token();
      
      g_active_file_info_tmp->namespace[0] = 0;

      fprintf(g_file_out_ptr, "t0 ");
    }
    else {
      print_error("Internal error: NAMESPACE/NONAMESPACE is missing.\n", ERROR_DIR);
      return FAILED;
    }
    
    return SUCCEEDED;
  }

  /* E (INTERNAL) */

  if (strcaselesscmp(g_current_directive, "E") == 0) {
    if (g_active_file_info_last != NULL) {
      g_active_file_info_tmp = g_active_file_info_last;
      g_active_file_info_last = g_active_file_info_last->prev;
      free(g_active_file_info_tmp);

      if (g_active_file_info_last == NULL)
        g_active_file_info_first = NULL;
      else {
        fprintf(g_file_out_ptr, "f%d ", g_active_file_info_last->filename_id);

        if (g_active_file_info_last->namespace[0] == 0)
          fprintf(g_file_out_ptr, "t0 ");
        else
          fprintf(g_file_out_ptr, "t1 %s ", g_active_file_info_last->namespace);      
      }
    }

    /* fix the line */
    if (g_active_file_info_last != NULL)
      g_active_file_info_last->line_current--;

    fprintf(g_file_out_ptr, "E ");
    g_open_files--;
    if (g_open_files == 0)
      return EVALUATE_TOKEN_EOP;

    if (g_extra_definitions == ON) {
      redefine("WLA_FILENAME", 0.0, get_file_name(g_active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(g_active_file_info_last->filename_id)));
      redefine("wla_filename", 0.0, get_file_name(g_active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
               (int)strlen(get_file_name(g_active_file_info_last->filename_id)));
    }

    return SUCCEEDED;
  }

  /* M */

  if (strcaselesscmp(g_current_directive, "M") == 0) {
    if (g_line_count_status == OFF)
      g_line_count_status = ON;
    else
      g_line_count_status = OFF;
    return SUCCEEDED;
  }

#ifdef GB

  /* ROMGBC */

  if (strcaselesscmp(g_current_directive, "ROMGBC") == 0) {
    no_library_files(".ROMGBC");
    
    if (g_romdmg != 0) {
      print_error(".ROMDMG was defined prior to .ROMGBC.\n", ERROR_DIR);
      return FAILED;
    }
    else if (g_romgbc == 2) {
      print_error(".ROMGBCONLY was defined prior to .ROMGBC.\n", ERROR_DIR);
      return FAILED;
    }

    g_romgbc = 1;

    return SUCCEEDED;
  }

  /* ROMGBCONLY */

  if (strcaselesscmp(g_current_directive, "ROMGBCONLY") == 0) {
    no_library_files(".ROMGBCONLY");

    if (g_romdmg != 0) {
      print_error(".ROMDMG was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
      return FAILED;
    }
    else if (g_romgbc == 1) {
      print_error(".ROMGBC was defined prior to .ROMGBCONLY.\n", ERROR_DIR);
      return FAILED;
    }

    g_romgbc = 2;

    return SUCCEEDED;
  }

  /* ROMDMG */

  if (strcaselesscmp(g_current_directive, "ROMDMG") == 0) {
    no_library_files(".ROMDMG");
    
    if (g_romgbc == 1) {
      print_error(".ROMGBC was defined prior to .ROMDMG.\n", ERROR_DIR);
      return FAILED;
    }
    else if (g_romgbc == 2) {
      print_error(".ROMGBCONLY was defined prior to .ROMDMG.\n", ERROR_DIR);
      return FAILED;
    }
    else if (g_romsgb != 0) {
      print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
      return FAILED;
    }

    g_romdmg = 1;

    return SUCCEEDED;
  }

  /* ROMSGB */

  if (strcaselesscmp(g_current_directive, "ROMSGB") == 0) {
    no_library_files(".ROMSGB");
    
    if (g_romdmg != 0) {
      print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
      return FAILED;
    }

    g_romsgb = 1;

    return SUCCEEDED;
  }
#endif

  /* ROMBANKSIZE */

  if (strcaselesscmp(g_current_directive, "ROMBANKSIZE") == 0 || strcaselesscmp(g_current_directive, "BANKSIZE") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < 0) {
      print_error(".ROMBANKSIZE needs a positive integer value.\n", ERROR_DIR);
      return FAILED;
    }

    if (g_banksize_defined != 0) {
      if (g_banksize != g_parsed_int) {
        print_error(".ROMBANKSIZE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    g_banksize = g_parsed_int;
    g_banksize_defined = 1;

    return SUCCEEDED;
  }

  /* ENDM */

  if (strcaselesscmp(g_current_directive, "ENDM") == 0)
    return directive_endm();

  /* BASE */

  if (strcaselesscmp(g_current_directive, "BASE") == 0) {
    no_library_files(".BASE definitions");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < 0) {
      print_error(".BASE number must be zero or positive.\n", ERROR_DIR);
      return FAILED;
    }

    fprintf(g_file_out_ptr, "b%d ", g_parsed_int);

    return SUCCEEDED;
  }

#if defined(MCS6502) || defined(MCS6510) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)

  /* 8BIT */

  if (strcaselesscmp(g_current_directive, "8BIT") == 0) {
    g_xbit_size = 8;
    return SUCCEEDED;
  }

  /* 16BIT */

  if (strcaselesscmp(g_current_directive, "16BIT") == 0) {
    g_xbit_size = 16;
    return SUCCEEDED;
  }

#endif

#ifdef W65816

  /* 24BIT */

  if (strcaselesscmp(g_current_directive, "24BIT") == 0) {
    g_xbit_size = 24;
    return SUCCEEDED;
  }

  /* INDEX */

  if (strcaselesscmp(g_current_directive, "INDEX") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || !(g_parsed_int == 8 || g_parsed_int == 16)) {
      print_error("The index size must be 8 or 16.\n", ERROR_DIR);
      return FAILED;
    }

    g_index_size = g_parsed_int;

    return SUCCEEDED;
  }

  /* ACCU */

  if (strcaselesscmp(g_current_directive, "ACCU") == 0) {
    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || !(g_parsed_int == 8 || g_parsed_int == 16)) {
      print_error("The accumulator size must be 8 or 16.\n", ERROR_DIR);
      return FAILED;
    }

    g_accu_size = g_parsed_int;

    return SUCCEEDED;
  }

#endif

#if defined(W65816)
  
  /* SMC */

  if (strcaselesscmp(g_current_directive, "SMC") == 0) {
    no_library_files(".SMC");

    g_smc_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }

  /* HIROM */

  if (strcaselesscmp(g_current_directive, "HIROM") == 0) {
    no_library_files(".HIROM");

    if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".HIROM");
      return FAILED;
    }

    g_hirom_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }

  /* EXHIROM */

  if (strcaselesscmp(g_current_directive, "EXHIROM") == 0) {
    no_library_files(".EXHIROM");

    if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_hirom_defined != 0) {
      give_snes_rom_mode_defined_error(".EXHIROM");
      return FAILED;
    }

    g_exhirom_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }
  
  /* LOROM */

  if (strcaselesscmp(g_current_directive, "LOROM") == 0) {
    no_library_files(".LOROM");

    if (g_hirom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
      give_snes_rom_mode_defined_error(".LOROM");
      return FAILED;
    }

    g_lorom_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }

  /* EXLOROM */
  /*
    if (strcaselesscmp(g_current_directive, "EXLOROM") == 0) {
    no_library_files(".EXLOROM");

    if (g_hirom_defined != 0 || g_lorom_defined != 0 || g_exhirom_defined != 0) {
    give_snes_rom_mode_defined_error(".EXLOROM");
    return FAILED;
    }

    g_exlorom_defined++;
    snes_mode++;

    return SUCCEEDED;
    }
  */
  /* SLOWROM */

  if (strcaselesscmp(g_current_directive, "SLOWROM") == 0) {
    no_library_files(".SLOWROM");
    
    if (g_fastrom_defined != 0) {
      print_error(".FASTROM was defined prior to .SLOWROM.\n", ERROR_DIR);
      return FAILED;
    }

    g_slowrom_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }

  /* FASTROM */

  if (strcaselesscmp(g_current_directive, "FASTROM") == 0) {
    no_library_files(".FASTROM");
    
    if (g_slowrom_defined != 0) {
      print_error(".SLOWROM was defined prior to .FASTROM.\n", ERROR_DIR);
      return FAILED;
    }

    g_fastrom_defined++;
    g_snes_mode++;

    return SUCCEEDED;
  }

  /* SNESHEADER */

  if (strcaselesscmp(g_current_directive, "SNESHEADER") == 0)
    return directive_snesheader();

  /* SNESNATIVEVECTOR */

  if (strcaselesscmp(g_current_directive, "SNESNATIVEVECTOR") == 0)
    return directive_snesnativevector();

  /* SNESEMUVECTOR */

  if (strcaselesscmp(g_current_directive, "SNESEMUVECTOR") == 0)
    return directive_snesemuvector();

#endif

  /* PRINT */

  if (strcaselesscmp(g_current_directive, "PRINT") == 0)
    return directive_print();
  
  /* PRINTT */

  if (strcaselesscmp(g_current_directive, "PRINTT") == 0)
    return directive_printt();

  /* PRINTV */

  if (strcaselesscmp(g_current_directive, "PRINTV") == 0)
    return directive_printv();

  /* SEED */

  if (strcaselesscmp(g_current_directive, "SEED") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".SEED needs a seed value for the randon number generator.\n", ERROR_DIR);
      return FAILED;
    }

    /* reseed the random number generator */
    init_genrand(g_parsed_int);

    return SUCCEEDED;
  }

  /* DBRND/DWRND */

  if (strcaselesscmp(g_current_directive, "DBRND") == 0 || strcaselesscmp(g_current_directive, "DWRND") == 0)
    return directive_dbrnd_dwrnd();

  /* DWSIN/DBSIN/DWCOS/DBCOS */

  if (strcaselesscmp(g_current_directive, "DWSIN") == 0 || strcaselesscmp(g_current_directive, "DBSIN") == 0 || strcaselesscmp(g_current_directive, "DWCOS") == 0 || strcaselesscmp(g_current_directive, "DBCOS") == 0)
    return directive_dwsin_dbsin_dwcos_dbcos();

  /* FAIL */

  if (strcaselesscmp(g_current_directive, "FAIL") == 0) {
    int exit_value = 1;
    
    q = input_number();
    if (q == INPUT_NUMBER_EOL)
      print_error("HALT: .FAIL found.\n", ERROR_NONE);
    else if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(g_error_message, sizeof(g_error_message), "\"%s\"\n", g_label);
      print_error(g_error_message, ERROR_FAI);

      q = input_number();
      if (q == SUCCEEDED)
        exit_value = g_parsed_int;
    }
    else if (q == SUCCEEDED)
      exit_value = g_parsed_int;
    else {
      print_error(".FAIL takes an optional string, but we got something else here...\n", ERROR_DIR);
      return FAILED;
    }

    /* make a silent exit */
    exit(exit_value);
  }

  /* UNDEF/UNDEFINE */

  if (strcaselesscmp(g_current_directive, "UNDEF") == 0 || strcaselesscmp(g_current_directive, "UNDEFINE") == 0)
    return directive_undef_undefine();

  /* ASM */

  if (strcaselesscmp(g_current_directive, "ASM") == 0)
    return SUCCEEDED;

  /* ENDASM */

  if (strcaselesscmp(g_current_directive, "ENDASM") == 0) {

    int endasm = 1, x;

    while (1) {
      x = g_source_pointer;
      q = get_next_token();
      if (q == GET_NEXT_TOKEN_STRING)
        continue;

      /* end of file? */
      if (strcmp(g_tmp, ".E") == 0) {
        g_source_pointer = x;
        return SUCCEEDED;
      }
      if (strcaselesscmp(g_tmp, ".ASM") == 0) {
        endasm--;
        if (endasm == 0)
          return SUCCEEDED;
      }
      if (strcaselesscmp(g_tmp, ".ENDASM") == 0)
        endasm++;
    }
  }

  /* STRINGMAPTABLE */

  if (strcaselesscmp(g_current_directive, "STRINGMAPTABLE") == 0)
    return directive_stringmap_table();

  /* STRINGMAP */

  if (strcaselesscmp(g_current_directive, "STRINGMAP") == 0)
    return directive_stringmap();

  return DIRECTIVE_NOT_IDENTIFIED;
}


static int _increase_ifdef(void) {

  if (g_ifdef == 255) {
    print_error("Out of .IF stack!\n", ERROR_DIR);
    return FAILED;
  }

  g_ifdef++;

  return SUCCEEDED;
}


/* parses only "if" directives. */
/* this is separate from parse_directive so that enums and ramsections can reuse this */
int parse_if_directive(void) {

  char bak[256];
  int q;

  /* ELSE */

  if (strcaselesscmp(g_current_directive, "ELSE") == 0) {
    int m, r;

    if (g_ifdef == 0) {
      print_error("There must be .IFxxx before .ELSE.\n", ERROR_DIR);
      return FAILED;
    }

    /* find the next compiling point */
    r = 1;
    m = g_macro_active;

    /* disable macro decoding */
    g_macro_active = 0;

    while (get_next_token() != FAILED) {
      if (g_tmp[0] == '.') {
        if (strcaselesscmp(g_current_directive, "ENDIF") == 0)
          r--;
        if (strcaselesscmp(g_current_directive, "E") == 0)
          break;
        if (strcaselesscmp(g_current_directive, "IFDEF") == 0 || strcaselesscmp(g_current_directive, "IFNDEF") == 0 || strcaselesscmp(g_current_directive, "IFGR") == 0 || strcaselesscmp(g_current_directive, "IFLE") == 0 || strcaselesscmp(g_current_directive, "IFEQ") == 0 ||
            strcaselesscmp(g_current_directive, "IFNEQ") == 0 || strcaselesscmp(g_current_directive, "IFDEFM") == 0 || strcaselesscmp(g_current_directive, "IFNDEFM") == 0 || strcaselesscmp(g_current_directive, "IF") == 0 || strcaselesscmp(g_current_directive, "IFEXISTS") == 0 ||
            strcaselesscmp(g_current_directive, "IFGREQ") == 0 || strcaselesscmp(g_current_directive, "IFLEEQ") == 0)
          r++;
      }
      if (r == 0) {
        g_ifdef--;
        g_macro_active = m;
        return SUCCEEDED;
      }
    }

    print_error(".ELSE must end to .ENDIF.\n", ERROR_DIR);
    return FAILED;
  }

  /* ENDIF */

  if (strcaselesscmp(g_current_directive, "ENDIF") == 0) {
    if (g_ifdef == 0) {
      print_error(".ENDIF was given before any .IF directive.\n", ERROR_DIR);
      return FAILED;
    }

    g_skip_elifs[g_ifdef] = NO;
    g_ifdef--;
    
    return SUCCEEDED;
  }

  /* IFDEF */

  if (strcaselesscmp(g_current_directive, "IFDEF") == 0) {
    struct definition *d;

    if (get_next_token() == FAILED)
      return FAILED;

    if (_increase_ifdef() == FAILED)
      return FAILED;

    hashmap_get(g_defines_map, g_tmp, (void *)&d);
    if (d != NULL) {
      g_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }

    g_skip_elifs[g_ifdef] = NO;
    
    return find_next_point("IFDEF");
  }

  /* IF */

  if (strcaselesscmp(g_current_directive, "IF") == 0) {
    g_input_parse_if = YES;
    q = input_number();
    g_input_parse_if = NO;

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      snprintf(g_error_message, sizeof(g_error_message), ".IF needs immediate data.\n");
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }

    if (_increase_ifdef() == FAILED)
      return FAILED;

    /* 0 = false, otherwise it's true */
    if (g_parsed_int != 0) {
      g_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      g_skip_elifs[g_ifdef] = NO;
      return find_next_point("IF");
    }
  }

  /* ELIF */

  if (strcaselesscmp(g_current_directive, "ELIF") == 0) {
    if (g_ifdef == 0) {
      print_error("There must be .IFxxx before .ELIF.\n", ERROR_DIR);
      return FAILED;
    }

    /* had an .if already succeeded previously? */
    if (g_skip_elifs[g_ifdef] == YES)
      return find_next_point("ELIF");

    g_input_parse_if = YES;
    q = input_number();
    g_input_parse_if = NO;

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      snprintf(g_error_message, sizeof(g_error_message), ".ELIF needs immediate data.\n");
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }

    /* 0 = false, otherwise it's true */
    if (g_parsed_int != 0) {
      g_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      g_skip_elifs[g_ifdef] = NO;
      return find_next_point("ELIF");
    }
  }

  /* IFGR/IFLE/IFEQ/IFNEQ/IFGREQ/IFLEEQ */

  if (strcaselesscmp(g_current_directive, "IFGR") == 0 || strcaselesscmp(g_current_directive, "IFLE") == 0 || strcaselesscmp(g_current_directive, "IFEQ") == 0 || strcaselesscmp(g_current_directive, "IFNEQ") == 0 || strcaselesscmp(g_current_directive, "IFGREQ") == 0 || strcaselesscmp(g_current_directive, "IFLEEQ") == 0) {

    char k[256];
    int y, o, s;

    strcpy(bak, g_current_directive);

    if (strcmp(&g_current_directive[2], "LE") == 0)
      o = 0;
    else if (strcmp(&g_current_directive[2], "GR") == 0)
      o = 1;
    else if (strcmp(&g_current_directive[2], "EQ") == 0)
      o = 2;
    else if (strcmp(&g_current_directive[2], "NEQ") == 0)
      o = 3;
    else if (strcmp(&g_current_directive[2], "GREQ") == 0)
      o = 4;
    else
      o = 5;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s needs immediate data.\n", bak);
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }

    strncpy(k, g_label, 255);
    k[255] = 0;
    y = g_parsed_int;
    s = q;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s needs immediate data.\n", bak);
      print_error(g_error_message, ERROR_INP);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error("The types of the compared things must be the same.\n", ERROR_INP);
      return FAILED;
    }

    /* values? */
    if (s == SUCCEEDED) {
      if ((o == 0 && y < g_parsed_int) || (o == 1 && y > g_parsed_int) || (o == 2 && y == g_parsed_int) || (o == 3 && y != g_parsed_int) || (o == 4 && y >= g_parsed_int) || (o == 5 && y <= g_parsed_int))
        q = SUCCEEDED;
      else
        q = FAILED;
    }
    /* strings? */
    else {
      if ((o == 0 && strcmp(k, g_label) < 0) || (o == 1 && strcmp(k, g_label) > 0) || (o == 2 && strcmp(k, g_label) == 0) || (o == 3 && strcmp(k, g_label) != 0) || (o == 4 && strcmp(k, g_label) >= 0) || (o == 5 && strcmp(k, g_label) <= 0))
        q = SUCCEEDED;
      else
        q = FAILED;
    }

    if (_increase_ifdef() == FAILED)
      return FAILED;

    if (q == SUCCEEDED) {
      g_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      g_skip_elifs[g_ifdef] = NO;
      strcpy(k, g_current_directive);
      return find_next_point(k);
    }
  }

  /* IFEXISTS */

  if (strcaselesscmp(g_current_directive, "IFEXISTS") == 0) {
    int number_result;
    FILE *f;

    g_expect_calculations = NO;
    number_result = input_number();
    g_expect_calculations = YES;

    if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".IFEXISTS needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    if (_increase_ifdef() == FAILED)
      return FAILED;

    f = fopen(g_label, "r");
    if (f == NULL) {
      g_skip_elifs[g_ifdef] = NO;
      return find_next_point("IFEXISTS");
    }

    fclose(f);

    g_skip_elifs[g_ifdef] = YES;
      
    return SUCCEEDED;
  }

  /* IFNDEF */

  if (strcaselesscmp(g_current_directive, "IFNDEF") == 0) {
    struct definition *d;

    if (get_next_token() == FAILED)
      return FAILED;

    if (_increase_ifdef() == FAILED)
      return FAILED;

    hashmap_get(g_defines_map, g_tmp, (void*)&d);
    if (d != NULL) {
      g_skip_elifs[g_ifdef] = NO;
      strcpy(g_error_message, g_current_directive);
      return find_next_point(g_error_message);
    }

    g_skip_elifs[g_ifdef] = YES;
    
    return SUCCEEDED;
  }

  /* IFDEFM/IFNDEFM */

  if (strcaselesscmp(g_current_directive, "IFDEFM") == 0 || strcaselesscmp(g_current_directive, "IFNDEFM") == 0) {
    int k, o;
    char e;

    strcpy(bak, g_current_directive);

    if (g_macro_active == 0) {
      snprintf(g_error_message, sizeof(g_error_message), ".%s can be only used inside a macro.\n", bak);
      print_error(g_error_message, ERROR_DIR);
      return FAILED;
    }

    if (g_current_directive[2] == 'N')
      o = 0;
    else
      o = 1;

    if (_increase_ifdef() == FAILED)
      return FAILED;

    for (; g_source_pointer < g_size; g_source_pointer++) {
      if (g_buffer[g_source_pointer] == 0x0A)
        break;
      else if (g_buffer[g_source_pointer] == '\\') {
        e = g_buffer[++g_source_pointer];
        if (e >= '0' && e <= '9') {
          g_parsed_int = (e - '0') * 10;
          for (k = 2; k < 8; k++, g_parsed_int *= 10) {
            e = g_buffer[++g_source_pointer];
            if (e >= '0' && e <= '9')
              g_parsed_int += e - '0';
            else
              break;
          }

          g_parsed_int /= 10;
          if ((o == 0 && g_macro_runtime_current->supplied_arguments < g_parsed_int) ||
              (o == 1 && g_macro_runtime_current->supplied_arguments >= g_parsed_int)) {
            g_skip_elifs[g_ifdef] = YES;
            return SUCCEEDED;
          }
          else {
            g_skip_elifs[g_ifdef] = NO;
            strcpy(g_error_message, g_current_directive);
            return find_next_point(g_error_message);
          }
        }
        break;
      }
    }

    snprintf(g_error_message, sizeof(g_error_message), ".%s needs an argument.\n", bak);
    print_error(g_error_message, ERROR_DIR);

    return FAILED;
  }

  /* neither success nor failure (didn't match any "if" directives) */
  return -1;
}


int find_next_point(char *name) {

  int depth = 1, m, line_current = g_active_file_info_last->line_current, source_pointer_old = g_source_pointer;

  /* find the next compiling point */
  m = g_macro_active;

  /* disable macro decoding */
  g_macro_active = 0;

  while (get_next_token() != FAILED) {
    if (g_tmp[0] == '.') {
      if (strcaselesscmp(g_current_directive, "ENDIF") == 0) {
        depth--;
        if (depth == 0)
          g_skip_elifs[g_ifdef] = NO;
        g_ifdef--;
      }
      if (strcaselesscmp(g_current_directive, "ELSE") == 0 && depth == 1) {
        depth--;
        if (g_skip_elifs[g_ifdef] == YES)
          g_source_pointer = source_pointer_old;
      }
      if (strcaselesscmp(g_current_directive, "ELIF") == 0 && depth == 1) {
        if (g_skip_elifs[g_ifdef] == NO) {
          /* go backwards so we'll actually parse .ELIF later */
          g_source_pointer = source_pointer_old;
          depth--;
        }
      }
      if (strcaselesscmp(g_current_directive, "E") == 0)
        break;
      if (strcaselesscmp(g_current_directive, "IFDEF") == 0 || strcaselesscmp(g_current_directive, "IFNDEF") == 0 || strcaselesscmp(g_current_directive, "IFGR") == 0 || strcaselesscmp(g_current_directive, "IFLE") == 0 ||
          strcaselesscmp(g_current_directive, "IFEQ") == 0 || strcaselesscmp(g_current_directive, "IFNEQ") == 0 || strcaselesscmp(g_current_directive, "IFDEFM") == 0 || strcaselesscmp(g_current_directive, "IFNDEFM") == 0 ||
          strcaselesscmp(g_current_directive, "IF") == 0 || strcaselesscmp(g_current_directive, "IFGREQ") == 0 || strcaselesscmp(g_current_directive, "IFLEEQ") == 0 || strcaselesscmp(g_current_directive, "IFEXISTS") == 0) {
        depth++;
        if (_increase_ifdef() == FAILED)
          return FAILED;
        g_skip_elifs[g_ifdef] = YES;        
      }
    }

    if (depth == 0) {
      g_macro_active = m;
      return SUCCEEDED;
    }
    else
      source_pointer_old = g_source_pointer;
  }

  /* return the condition's line number */
  g_active_file_info_last->line_current = line_current;
  snprintf(g_error_message, sizeof(g_error_message), ".%s must end to .ENDIF/.ELSE.\n", name);
  print_error(g_error_message, ERROR_DIR);

  return FAILED;
}


#ifndef GB

void delete_stack(struct stack *s) {

  if (s == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Deleting a non-existing computation stack. This can lead to problems.\n");
    print_error(g_error_message, ERROR_WRN);
    return;
  }

  free(s->stack);
  free(s);
}

#endif


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
    stack_create_label_stack(g_label);
    x = INPUT_NUMBER_STACK;
  }

  if (x == INPUT_NUMBER_STACK) {
    *b = g_stacks_tmp->id;
    g_stacks_tmp->position = STACK_POSITION_DEFINITION;

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
        *b = g_parsed_int;
      else if (x == INPUT_NUMBER_STRING) {
        strcpy(c, g_label);
        s = (int)strlen(g_label);
      }
      else if (x == INPUT_NUMBER_FLOAT) {
        *data = g_parsed_double;
        *b = (int)g_parsed_double;
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
      g_parsed_int = (int)g_parsed_double;
      x = SUCCEEDED;
    }

    if (x == INPUT_NUMBER_STRING) {
      strcpy(&c[s], g_label);
      s += (int)strlen(g_label);
    }
    else if (x == SUCCEEDED) {
      if (g_parsed_int > 255) {
        c[s] = (g_parsed_int & 255);
        c[s + 1] = (g_parsed_int >> 8) & 255;
        c[s + 2] = 0;
        s += 2;
      }
      else {
        c[s] = g_parsed_int;
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
  export = g_export_first;
  while (export != NULL) {
    if (strcmp(export->name, name) == 0) {
      snprintf(g_error_message, sizeof(g_error_message), "\"%s\" was .EXPORTed for the second time.\n", name);
      print_error(g_error_message, ERROR_WRN);
      return SUCCEEDED;
    }
    export = export->next;
  }

  export = calloc(sizeof(struct export_def), 1);
  if (export == NULL) {
    snprintf(g_error_message, sizeof(g_error_message), "Out of memory while allocating room for \".EXPORT %s\".\n", name);
    print_error(g_error_message, ERROR_DIR);
    return FAILED;
  }

  strcpy(export->name, name);
  export->next = NULL;

  if (g_export_first == NULL) {
    g_export_first = export;
    g_export_last = export;
  }
  else {
    g_export_last->next = export;
    g_export_last = export;
  }

  return SUCCEEDED;
}


/* store the labels in a label stack in which g_label_stack[0] is the base level label,
   g_label_stack[1] is the first child, g_label_stack[2] is the second child, and so on... */
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
    reset_label_stack();
    strcpy(g_label_stack[0], l);
  }
  else
    strcpy(g_label_stack[level], &l[level-1]);

  /*
    fprintf(stderr, "*************************************\n");
    fprintf(stderr, "LABEL STACK:\n");
    for (q = 0; q < 256; q++) {
    if (g_label_stack[q][0] != 0)
    fprintf(stderr, "%s LEVEL %d\n", g_label_stack[q], q);
    }
    fprintf(stderr, "*************************************\n");
  */

  return SUCCEEDED;
}


void reset_label_stack(void) {

  int q;
  
  for (q = 0; q < 256; q++)
    g_label_stack[q][0] = 0;
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
    strcpy(out, g_label_stack[0]);
    for (q = 1; q < level; q++) {
      if (strlen(out) + strlen(g_label_stack[q]) >= MAX_NAME_LENGTH) {
        print_error(error_message, ERROR_ERR);
        return FAILED;  
      }
      strncat(out, g_label_stack[q], MAX_NAME_LENGTH);
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
    
  snprintf(buf, sizeof(buf), "%s.%s", g_active_file_info_last->namespace, s);
  buf[sizeof(buf)-1] = 0;
  if (strlen(buf) >= (size_t)sizeof_s) {
    snprintf(g_error_message, sizeof(g_error_message), "The current file namespace \"%s\" cannot be added to %s's \"%s\" name - increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_active_file_info_last->namespace, type, s);
    print_error(g_error_message, ERROR_ERR);
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

  fprintf(g_file_out_ptr, "L%s%s ", header, footer2);
}

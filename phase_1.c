
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "defines.h"

#include "include.h"
#include "parse.h"
#include "phase_1.h"
#include "phase_2.h"
#include "phase_3.h"
#include "stack.h"
#include "hashmap.h"
#include "printf.h"
#include "decode.h"
#include "mersenne.h"
#include "main.h"


#if defined(GB)
char g_licenseecodenew_c1, g_licenseecodenew_c2;
int g_nintendologo_defined = 0;
int g_computechecksum_defined = 0, g_computecomplementcheck_defined = 0;
int g_romgbc = 0, g_romdmg = 0, g_romsgb = 0, g_romsize = 0, g_romsize_defined = 0;
int g_cartridgetype = 0, g_cartridgetype_defined = 0;
int g_countrycode = 0, g_countrycode_defined = 0;
int g_licenseecodenew_defined = 0, g_licenseecodeold = 0, g_licenseecodeold_defined = 0;
int g_version = 0, g_version_defined = 0;
static int s_gbheader_defined = 0;
#endif

#if defined(Z80)
char *g_sdsctag_name_str = NULL, *g_sdsctag_notes_str = NULL, *g_sdsctag_author_str = NULL;
int g_sdsctag_name_type, g_sdsctag_notes_type, g_sdsctag_author_type, g_sdsc_ma, g_sdsc_mi;
int g_sdsctag_name_value, g_sdsctag_notes_value, g_sdsctag_author_value;
int g_computesmschecksum_defined = 0, g_sdsctag_defined = 0, g_smstag_defined = 0;
int g_smsheader_defined = 0, g_smsversion = 0, g_smsversion_defined = 0, g_smsregioncode = 0, g_smsregioncode_defined = 0;
int g_smsproductcode_defined = 0, g_smsproductcode1 = 0, g_smsproductcode2 = 0, g_smsproductcode3 = 0, g_smsreservedspace1 = 0;
int g_smsreservedspace2 = 0, g_smsromsize = 0, g_smsromsize_defined = 0;
int g_smsforcechecksum = 0, g_smsforcechecksum_defined = 0, g_smschecksumsize = 0, g_smschecksumsize_defined = 0;
static int s_smsreservedspace_defined = 0;
#endif

#if defined(MC68000)
char g_smdheader_systemtype[17] = "SEGA MEGA DRIVE ";
char g_smdheader_copyright[17] = "                ";
char g_smdheader_titledomestic[49] = "                                                ";
char g_smdheader_titleoverseas[49] = "                                                ";
char g_smdheader_serialnumber[15] = "              ";
char g_smdheader_devicesupport[17] = "J               ";
int g_smdheader_romaddressrange_start = 0, g_smdheader_romaddressrange_end = -1;
int g_smdheader_ramaddressrange_start = 0xff0000, g_smdheader_ramaddressrange_end = 0xffffff;
char g_smdheader_extramemory_type_1[3] = "RA";
int g_smdheader_extramemory_type_2 = 0xa0, g_smdheader_extramemory_type_3 = 0x20, g_smdheader_extramemory_start = 0, g_smdheader_extramemory_end = 0;
char g_smdheader_modemsupport[13] = "            ";
char g_smdheader_regionsupport[4] = "JUE";
int g_computesmdchecksum_defined = 0, g_smdheader_defined = NO;
#endif

int g_org_defined = 1, g_background_defined = 0;
int g_rombanks = 0, g_rombanks_defined = 0, g_max_address = 0;
int g_rambanks = 0, g_rambanks_defined = 0;
int g_emptyfill = 0, g_emptyfill_defined = 0;
int g_section_status = OFF, g_section_id = 1;
int g_parsed_int, g_source_index, g_ifdef = 0, g_slots_amount = 0;
int g_memorymap_defined = 0;
int g_banksize_defined = 0, g_banksize = 0;
int g_rombankmap_defined = 0, *g_banks = NULL, *g_bankaddress = NULL;
int g_bankheader_status = OFF;
int g_macro_active = 0;
int g_smc_defined = 0;
int g_asciitable_defined = 0;
int g_saved_structures_count = 0;
unsigned char g_asciitable[256];

extern int g_resolve_stack_calculations;

#if defined(W65816)
char g_snesid[4];
int g_snesid_defined = 0, g_snesromsize = 0;
int g_sramsize_defined = 0, g_sramsize = 0, g_country_defined = 0, g_country = 0;
int g_cartridgetype = 0, g_cartridgetype_defined = 0, g_licenseecode_defined = 0, g_licenseecode = 0;
int g_version_defined = 0, g_version = 0;
int g_hirom_defined = 0, g_lorom_defined = 0, g_slowrom_defined = 0, g_fastrom_defined = 0, g_snes_mode = 0;
int g_exlorom_defined = 0, g_exhirom_defined = 0;
int g_computesneschecksum_defined = 0, g_use_wdc_standard = 0;
static int s_snesemuvector_defined = 0, s_snesheader_defined = 0, s_snesnativevector_defined = 0;
#endif

#if defined(GB) || defined(W65816)
char g_name[32];
int g_name_defined = 0;
#endif

#if defined(SPC700)
extern int g_input_number_expects_dot;
#endif

int g_sizeof_g_tmp = 4096, g_global_listfile_items = 0, *g_global_listfile_ints = NULL;
int g_romheader_baseaddress = -1, g_romheader_baseaddress_defined = 0;
char *g_tmp = NULL, *g_global_listfile_cmds = NULL;
char *g_label_stack[256];
char g_current_directive[MAX_NAME_LENGTH + 1];

unsigned char *g_rom_banks = NULL, *g_rom_banks_usage_table = NULL;

struct structure **g_saved_structures = NULL;
struct export_def *g_export_first = NULL, *g_export_last = NULL;
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
struct string *g_fopen_filenames_first = NULL, *g_fopen_filenames_last = NULL;
struct function *g_functions_first = NULL, *g_functions_last = NULL;
struct namespace *g_namespaces_first = NULL;

extern char *g_buffer, *unfolded_buffer, g_label[MAX_NAME_LENGTH + 1], *g_include_dir, *g_full_name;
extern int g_source_file_size, g_input_number_error_msg, g_verbose_level, g_output_format, g_open_files, g_input_parse_if;
extern int g_last_stack_id, g_latest_stack, g_ss, g_commandline_parsing, g_newline_beginning, g_expect_calculations, g_input_parse_special_chars;
extern int g_extra_definitions, g_string_size, g_input_float_mode, g_operand_hint, g_operand_hint_type, g_dsp_enable_label_address_conversion;
extern int g_include_dir_size, g_parse_floats, g_listfile_data, g_quiet, g_parsed_double_decimal_numbers;
extern int g_create_sizeof_definitions, g_input_allow_leading_hashtag, g_input_has_leading_hashtag, g_input_allow_leading_ampersand;
extern int g_plus_and_minus_ends_label, g_get_next_token_use_substitution, g_input_number_turn_values_into_strings;
extern int g_continue_parsing_after_an_error, g_continued_parsing_after_an_error, g_allow_labels_without_colon;
extern FILE *g_file_out_ptr;
extern double g_parsed_double;
extern char *g_final_name;

extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;
extern struct file_name_info *g_file_name_info_first, *g_file_name_info_last, *g_file_name_info_tmp;
extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern int g_makefile_rules, g_parsing_function_body, g_force_add_namespace, g_is_file_isolated_counter, g_force_ignore_namespace;

extern int create_tmp_file(FILE **);

static int s_macro_stack_size = 0, s_repeat_stack_size = 0;
static int s_bank = 0, s_bank_defined = 1, s_line_count_status = ON;
static int s_block_status = 0, s_block_name_id = 0, s_parse_dstruct_result;
static int s_dstruct_status = OFF, s_current_slot = 0;
static int s_enumid_defined = 0, s_enumid = 0, s_enumid_adder = 1, s_enumid_export = 0;
static int s_repeat_active = 0, s_saved_structures_max = 0, s_skip_elifs[256];

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(W65816) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
int g_xbit_size = 0, g_accu_size = 8, g_index_size = 8;
#endif

/* vars used when in an enum, ramsection, or struct. */
/* some variables named "enum_" are used in enums, ramsections, and structs. */
int g_in_enum = NO, g_in_ramsection = NO, g_in_struct = NO, g_macro_id = 0;
static int s_enum_export, s_enum_ord;
static int s_enum_offset; /* Offset relative to enum start where we're at right now */
static int s_last_enum_offset;
static int s_base_enum_offset; /* start address of enum */
static int s_enum_sizeof_pass; /* set on second pass through enum/ramsection, generating _sizeof labels */
/* temporary struct used to build up enums/ramsections (and, of course, structs)
   this gets temporarily replaced when inside a union (each union is considered a separate struct). */
static struct structure *s_active_struct = NULL;

static int s_union_base_offset; /* start address of current union */
static int s_max_enum_offset; /* highest position seen within current union group */
static struct structure *s_union_first_struct = NULL; /* first struct in current union */
static struct union_stack *s_union_stack = NULL; /* stores variables for nested unions */

/* for .TABLE, .DATA and .ROW */
static char s_table_format[256];
static int s_table_defined = 0, s_table_size = 0, s_table_index = 0;

static int s_source_index_old = 0, s_line_current_old = 0;
static int s_defaultslot_defined = 0, s_defaultslot;
static int s_autopriority = 65535;

static struct section_def *s_active_ramsection = NULL;


#define no_library_files(name)                                          \
  do {                                                                  \
    if (g_output_format == OUTPUT_LIBRARY) {                            \
      print_error(ERROR_DIR, "Library files don't take " name ".\n");   \
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


static char *_string_duplicate_size(char *p, int size) {

  char *result;

  if (p == NULL)
    return NULL;

  result = calloc(sizeof(char), size + 1);
  if (result == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new string (\"%s\").\n", p);
    return NULL;
  }

  memcpy(result, p, size);
  result[size] = 0;

  return result;
}


static char *_string_duplicate(char *p) {

  return _string_duplicate_size(p, (int)strlen(p));
}


static int _add_namespace_to_string(char *s, int sizeof_s, char *type, char *namespace) {

  char buf[MAX_NAME_LENGTH + 1];
    
  snprintf(buf, sizeof(buf), "%s.%s", namespace, s);
  buf[sizeof(buf)-1] = 0;
  if (strlen(buf) >= (size_t)sizeof_s) {
    print_error(ERROR_ERR, "The current file namespace \"%s\" cannot be added to %s's \"%s\" name - increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", namespace, type, s);
    return FAILED;
  }

  strcpy(s, buf);

  return SUCCEEDED;
}


int add_namespace_to_string(char *s, int sizeof_s, char *type) {

  if (g_active_file_info_last == NULL || g_active_file_info_last->namespace[0] == 0)
    return SUCCEEDED;
  
  return _add_namespace_to_string(s, sizeof_s, type, g_active_file_info_last->namespace);
}


static int _get_slot_number_by_its_name(char *slot_name, int *number) {

  char c1 = slot_name[0];
  int i;
  
  for (i = 0; i < g_slots_amount; i++) {
    if (c1 == g_slots[i].name[0]) {
      if (strcmp(slot_name, g_slots[i].name) == 0) {
        *number = i;
        return SUCCEEDED;
      }
    }
  }

  print_error(ERROR_DIR, "Cannot find SLOT \"%s\".\n", slot_name);

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
        print_error(ERROR_WRN, "There is a SLOT number %d, but there also is a SLOT (with ID %d) with starting address %d ($%x)... Using SLOT %d.\n", value, i, value, value, value);
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

  print_error(ERROR_DIR, "Cannot find SLOT %d.\n", value);
  
  return FAILED;
}


int macro_get(char *name, int add_namespace, struct macro_static **macro_out) {

  struct macro_static *macro;
  char fullname[MAX_NAME_LENGTH + 1];
  char c1;

  strcpy(fullname, name);

  if (add_namespace == YES) {
    /* are we inside a .MACRO that's been namespaced? */
    if (g_macro_active > 0) {
      struct macro_runtime *rt;
      struct macro_static *st;
    
      rt = &g_macro_stack[g_macro_active - 1];
      st = rt->macro;

      if (st->defined_namespace[0] != 0) {
        /* yes! try to use the .MACRO's namespace */
        if (_add_namespace_to_string(fullname, sizeof(fullname), ".MACRO", st->defined_namespace) == FAILED) {
          *macro_out = NULL;
          return FAILED;
        }
      }
    }
    else if (g_active_file_info_last->namespace[0] != 0) {
      if (add_namespace_to_string(fullname, sizeof(fullname), ".MACRO") == FAILED) {
        *macro_out = NULL;
        return FAILED;
      }
    }
  }

  c1 = fullname[0];
  macro = g_macros_first;
  while (macro != NULL) {
    if (c1 == macro->name[0]) {
      if (strcmp(macro->name, fullname) == 0)
        break;
    }
    macro = macro->next;
  }

  *macro_out = macro;
  
  return SUCCEEDED;
}


static int _macro_stack_grow(void) {

  if (g_macro_active == s_macro_stack_size) {
    struct macro_runtime *macro;
    int old_size;

    /* enlarge the macro stack */
    old_size = s_macro_stack_size;
    s_macro_stack_size = (s_macro_stack_size<<1)+2;

    macro = calloc(sizeof(struct macro_runtime) * s_macro_stack_size, 1);
    if (macro == NULL) {
      print_error(ERROR_ERR, "Out of memory error while enlarging macro stack buffer.\n");
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


static int _macro_start(struct macro_static *m, struct macro_runtime *mrt, int caller, int nargs) {

  g_macro_runtime_current = mrt;
  g_macro_active++;
  m->calls++;

  /* macro call start */
  fprintf(g_file_out_ptr, "i%d %s ", m->id, m->name);

  mrt->caller = caller;
  mrt->macro = m;
  mrt->macro_return_i = g_source_index;
  mrt->macro_return_line = g_active_file_info_last->line_current;
  mrt->macro_return_filename_id = g_active_file_info_last->filename_id;

  if ((g_extra_definitions == ON) && (g_active_file_info_last->filename_id != m->filename_id)) {
    redefine("WLA_FILENAME", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
    redefine("wla_filename", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(m->filename_id)));
  }

  g_active_file_info_last->line_current = m->start_line;
  g_active_file_info_last->filename_id = m->filename_id;
  g_source_index = m->start;

  /* redefine NARGS */
  if (redefine("NARGS", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;
  if (redefine("nargs", (double)nargs, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _macro_start_dxm(struct macro_static *m, int caller, char *name, int first) {

  struct macro_runtime *mrt;
  int start, number_result;
  
  /* start running a macro... run until .ENDM */
  if (_macro_stack_grow() == FAILED)
    return FAILED;

  mrt = &g_macro_stack[g_macro_active];

  start = g_source_index;

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
    print_error(ERROR_NONE, "Out of memory error while collecting macro arguments.\n");
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
    print_error(ERROR_INP, ".%s needs data.\n", name);
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
      print_text(NO, "got string %s!\n", label);
    */
  }
  else if (number_result == INPUT_NUMBER_STACK)
    mrt->argument_data[0]->value = (double)g_latest_stack;
  else if (number_result == SUCCEEDED)
    mrt->argument_data[0]->value = g_parsed_int;
  else
    return FAILED;

  if (_macro_start(m, mrt, caller, 1) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _macro_start_incbin(struct macro_static *m, struct macro_incbin *incbin_data, int first) {

  struct macro_runtime *mrt;

  /* start running a macro... run until .ENDM */
  if (_macro_stack_grow() == FAILED)
    return FAILED;

  mrt = &g_macro_stack[g_macro_active];

  if (first == YES)
    mrt->incbin_data = incbin_data;
  else
    incbin_data = mrt->incbin_data;

  if (incbin_data->left <= 0) {
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

  mrt->argument_data = calloc(sizeof(struct macro_argument *) * 3, 1);
  mrt->argument_data[0] = calloc(sizeof(struct macro_argument), 1);
  mrt->argument_data[1] = calloc(sizeof(struct macro_argument), 1);
  mrt->argument_data[2] = calloc(sizeof(struct macro_argument), 1);
  if (mrt->argument_data == NULL || mrt->argument_data[0] == NULL || mrt->argument_data[1] == NULL || mrt->argument_data[2] == NULL) {
    print_error(ERROR_NONE, "Out of memory error while collecting macro arguments.\n");
    return FAILED;
  }

  /* filter all the data through that macro */
  mrt->argument_data[0]->type = SUCCEEDED;
  mrt->argument_data[0]->start = g_source_index;
  mrt->argument_data[1]->type = SUCCEEDED;
  mrt->argument_data[1]->value = mrt->offset;
  mrt->argument_data[2]->type = SUCCEEDED;
  mrt->argument_data[2]->value = incbin_data->filter_size;
  mrt->supplied_arguments = 3;

  if (incbin_data->filter_size == 1) {
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
  }
  else if (incbin_data->filter_size == 2) {
    int data1, data2;

    data1 = incbin_data->data[incbin_data->position++];
    data2 = incbin_data->data[incbin_data->position++];
    
    if (incbin_data->swap != 0)
      mrt->argument_data[0]->value = (data1 << 8) | data2;
    else
      mrt->argument_data[0]->value = (data2 << 8) | data1;
  }
  else if (incbin_data->filter_size == 3) {
    int data1, data2, data3;

    data1 = incbin_data->data[incbin_data->position++];
    data2 = incbin_data->data[incbin_data->position++];
    data3 = incbin_data->data[incbin_data->position++];
    
    if (incbin_data->swap != 0)
      mrt->argument_data[0]->value = (data1 << 16) | (data2 << 8) | data3;
    else
      mrt->argument_data[0]->value = (data3 << 16) | (data2 << 8) | data1;
  }
  else if (incbin_data->filter_size == 4) {
    int data1, data2, data3, data4;

    data1 = incbin_data->data[incbin_data->position++];
    data2 = incbin_data->data[incbin_data->position++];
    data3 = incbin_data->data[incbin_data->position++];
    data4 = incbin_data->data[incbin_data->position++];
    
    if (incbin_data->swap != 0)
      mrt->argument_data[0]->value = (data1 << 24) | (data2 << 16) | (data3 << 8) | data4;
    else
      mrt->argument_data[0]->value = (data4 << 24) | (data3 << 16) | (data2 << 8) | data1;
  }
  
  incbin_data->left -= incbin_data->filter_size;

  if (_macro_start(m, mrt, MACRO_CALLER_INCBIN, 3) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _macro_insert_bytes(char *name, int size) {

  struct definition *d;
  int error = NO;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    print_error(ERROR_DIR, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    return FAILED;
  }

  if (size == 1) {
    if (d->type == DEFINITION_TYPE_VALUE) {
      if (d->value < -128 || d->value > 255) {
        print_error(ERROR_DIR, ".%s expects 8-bit data, %d is out of range!\n", name, (int)d->value);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "d%d ", (int)d->value);
    }
    else if (d->type == DEFINITION_TYPE_STACK)
      fprintf(g_file_out_ptr, "c%d ", (int)d->value);
    else
      error = YES;
  }
  else if (size == 2) {
    if (d->type == DEFINITION_TYPE_VALUE) {
      if (d->value < -32768 || d->value > 65535) {
        print_error(ERROR_DIR, ".%s expects 16-bit data, %d is out of range!\n", name, (int)d->value);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "y%d ", (int)d->value);
    }
    else if (d->type == DEFINITION_TYPE_STACK)
      fprintf(g_file_out_ptr, "C%d ", (int)d->value);
    else
      error = YES;
  }
  else if (size == 3) {
    if (d->type == DEFINITION_TYPE_VALUE) {
      if (d->value < -8388608 || d->value > 16777215) {
        print_error(ERROR_DIR, ".%s expects 24-bit data, %d is out of range!\n", name, (int)d->value);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "z%d ", (int)d->value);
    }
    else if (d->type == DEFINITION_TYPE_STACK)
      fprintf(g_file_out_ptr, "T%d ", (int)d->value);
    else
      error = YES;
  }
  else if (size == 4) {
    if (d->type == DEFINITION_TYPE_VALUE)
      fprintf(g_file_out_ptr, "u%d ", (int)d->value);
    else if (d->type == DEFINITION_TYPE_STACK)
      fprintf(g_file_out_ptr, "U%d ", (int)d->value);
    else
      error = YES;
  }
  
  if (error == YES) {
    print_error(ERROR_DIR, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    return FAILED;
  }
  
  return SUCCEEDED;
}


static int _macro_insert_word_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    print_error(ERROR_DIR, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -32768 || d->value > 65535) {
      print_error(ERROR_DIR, ".%s expects 16-bit data, %d is out of range!\n", name, (int)d->value);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "y%d ", (int)d->value);
    /*
      print_text(NO, ".DWM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "C%d ", (int)d->value);
    /*
      print_text(NO, ".DWM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    print_error(ERROR_DIR, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    return FAILED;
  }

  return SUCCEEDED;
}


static int _macro_insert_long_db(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    print_error(ERROR_DIR, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -8388608 || d->value > 16777215) {
      print_error(ERROR_DIR, ".%s expects 24-bit data, %d is out of range!\n", name, (int)d->value);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "z%d ", (int)d->value);
    /*
      print_text(NO, ".DLM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "T%d ", (int)d->value);
    /*
      print_text(NO, ".DLM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    print_error(ERROR_DIR, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    return FAILED;
  }

  return SUCCEEDED;
}


static int _macro_insert_double_dw(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, "_out", (void*)&d) != MAP_OK)
    hashmap_get(g_defines_map, "_OUT", (void*)&d);

  if (d == NULL) {
    print_error(ERROR_DIR, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -2147483648.0 || d->value > 2147483647.0) {
      print_error(ERROR_DIR, ".%s expects 32-bit data, %d is out of range!\n", name, (int)d->value);
      return FAILED;
    }
    fprintf(g_file_out_ptr, "u%d ", (int)d->value);
    /*
      print_text(NO, ".DLM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(g_file_out_ptr, "U%d ", (int)d->value);
    /*
      print_text(NO, ".DLM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    print_error(ERROR_DIR, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    return FAILED;
  }

  return SUCCEEDED;
}


static struct structure* _get_structure(char *name) {

  struct structure *s = g_structures_first;
  char c1 = name[0];

  while (s != NULL) {
    if (c1 == s->name[0]) {
      if (strcmp(name, s->name) == 0)
        return s;
    }
    s = s->next;
  }

  return NULL;
}


int directive_define_def_equ(void);


int phase_1(void) {

  struct macro_runtime *mrt;
  struct macro_static *m = NULL;
  int o, p, q;

  if (g_verbose_level >= 100)
    print_text(YES, "Pass 1...\n");

  reset_label_stack();

  /* mark all slots as empty */
  for (q = 0; q < 256; q++) {
    g_slots[q].size = 0;
    g_slots[q].name[0] = 0;
  }

  /* start from the very first character, this is the index to the source file we are about to parse... */
  g_source_index = 0;

  /* BANK 0 SLOT 0 ORG 0 */
  if (g_output_format != OUTPUT_LIBRARY)
    fprintf(g_file_out_ptr, "B%d %d O%d ", 0, 0, 0);

  while (1) {
    q = get_next_token();
    if (q == FAILED)
      return FAILED;
    else if (q == GET_NEXT_TOKEN_STRING) {
      print_error(ERROR_LOG, "Encountered quoted string \"%s\" where no quoted string should be.\n", g_tmp);
      return FAILED;
    }

    /* skip loose '\'s */
    if (strcmp("\\", g_tmp) == 0)
      continue;

    q = evaluate_token();

    /* we can again try to turn labels into address values, this was turned off in evaluate_token()
       when trying to parse instructions */
    g_dsp_enable_label_address_conversion = YES;
    
#if defined(SPC700)
    /* instruction parser might set this to YES, inside evaluate_token() */
    g_input_number_expects_dot = NO;
#endif
    
    if (q == SUCCEEDED)
      continue;
    else if (q == EVALUATE_TOKEN_EOP)
      return SUCCEEDED;
    else if (q == EVALUATE_TOKEN_NOT_IDENTIFIED) {
      int got_opening_parenthesis = NO, end_of_line = NO;

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
          g_source_index -= g_ss;

          if (directive_define_def_equ() == FAILED)
            return FAILED;
        }
        else {
          if (g_output_format == OUTPUT_LIBRARY && g_section_status == OFF) {
            print_error(ERROR_LOG, "All labels must be inside sections when compiling a library.\n");
            return FAILED;
          }
          if (g_org_defined == 0) {
            print_error(ERROR_LOG, "\"%s\" needs a position in memory.\n", g_tmp);
            return FAILED;
          }
          if (g_ss >= MAX_NAME_LENGTH) {
            print_error(ERROR_NONE, "The label \"%s\" is too long. Max label length is %d characters.\n", g_tmp, MAX_NAME_LENGTH);
            return FAILED;
          }
          if (g_bankheader_status == ON) {
            print_error(ERROR_LOG, "BANKHEADER sections don't take labels.\n");
            return FAILED;
          }

          /* is labels without ':' at the end forbidden? */
          if (g_allow_labels_without_colon == NO && q == g_ss) {
            print_error(ERROR_ERR, "Cannot process \"%s\". Syntax error?\n", g_tmp);
            return FAILED;
          }

          /* check out for \@-symbols */
          if (g_macro_active != 0 && q >= 2) {
            if (g_tmp[q - 2] == '\\' && g_tmp[q - 1] == '@')
              snprintf(&g_tmp[q - 2], g_sizeof_g_tmp - (q - 2), "%d", g_macro_runtime_current->macro->calls - 1);
          }

          if (g_macro_active != 0) {
            if (should_we_add_namespace() == YES) {
              if (add_namespace_to_a_label(g_tmp, g_sizeof_g_tmp, YES) == FAILED)
                return FAILED;
            }
          }

          if (add_label_to_label_stack(g_tmp) == FAILED)
            return FAILED;
          
          fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, g_tmp);

          /* move to the end of the label */
          if (q != g_ss)
            g_source_index -= g_ss - q - 1;
        }
    
        continue;
      }
      else if (compare_next_token("=") == SUCCEEDED) {
        /* it's actually a definition! */
        g_source_index -= g_ss;

        if (directive_define_def_equ() == FAILED)
          return FAILED;

        continue;
      }
      
      if (m == NULL) {
        print_error(ERROR_ERR, "Cannot process \"%s\". Syntax error?\n", g_tmp);
        if (g_continue_parsing_after_an_error == YES) {
          /* find the end of the line and continue parsing on the following line */
          while (g_buffer[g_source_index] != 0xA)
            g_source_index++;
          g_source_index++;
          g_continued_parsing_after_an_error = YES;
          next_line();

          continue;
        }
        else
          return FAILED;
      }

      /* start running a macro... run until .ENDM */
      if (_macro_stack_grow() == FAILED)
        return FAILED;

      mrt = &g_macro_stack[g_macro_active];
      mrt->argument_data = NULL;
      mrt->incbin_data = NULL;
      mrt->definition_storage = NULL;

      /* skip '(' */
      if (g_buffer[g_source_index] == '(' && compare_and_skip_next_symbol('(') == SUCCEEDED)
        got_opening_parenthesis = YES;

      /* collect macro arguments */
      for (p = 0; 1; p++) {
        if (got_opening_parenthesis == YES) {
          /* skip ')' */
          if (compare_and_skip_next_symbol(')') == SUCCEEDED)
            break;
        }

        /* take away the white space */
        while (1) {
          if (g_buffer[g_source_index] == ' ' || g_buffer[g_source_index] == ',')
            g_source_index++;
          else
            break;
        }

        o = g_source_index;
        g_input_allow_leading_hashtag = YES;
        g_input_allow_leading_ampersand = YES;
        g_input_float_mode = ON;
        q = input_number();
        g_input_allow_leading_hashtag = NO;
        g_input_allow_leading_ampersand = NO;
        g_input_float_mode = OFF;
        if (q == INPUT_NUMBER_EOL) {
          end_of_line = YES;
          break;
        }

        mrt->argument_data = realloc(mrt->argument_data, (p+1)*sizeof(struct macro_argument *));
        mrt->argument_data[p] = calloc(sizeof(struct macro_argument), 1);
        if (mrt->argument_data == NULL || mrt->argument_data[p] == NULL) {
          print_error(ERROR_NONE, "Out of memory error while collecting macro arguments.\n");
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
        else if (q == SUCCEEDED)
          mrt->argument_data[p]->value = (double)g_parsed_int;
        else if (q == INPUT_NUMBER_FLOAT)
          mrt->argument_data[p]->value = g_parsed_double;
        else
          return FAILED;

        /* do we have a name for this argument? */
        if (p < m->nargument_names) {
          char argument_name[MAX_NAME_LENGTH + 1];
          struct definition *definition;

          if (m->namespace[0] == 0)
            strcpy(argument_name, m->argument_names[p]);
          else
            snprintf(argument_name, sizeof(argument_name), "%s.%s", m->namespace, m->argument_names[p]);

          /* does the definition exist already? */
          hashmap_get(g_defines_map, argument_name, (void*)&definition);
          if (definition != NULL) {
            /* yes, store it! */
            struct definition_storage *storage = calloc(sizeof(struct definition_storage), 1);
            if (storage == NULL) {
              print_error(ERROR_ERR, "Out of memory error while trying to store a definition.\n");
              return FAILED;
            }

            storage->definition = definition;
            storage->next = mrt->definition_storage;
            mrt->definition_storage = storage;

            /* temporarily remove the definition, because a new one is going to replace it */
            hashmap_remove(g_defines_map, argument_name);
          }

          if (q == INPUT_NUMBER_ADDRESS_LABEL)
            redefine(argument_name, 0.0, g_label, DEFINITION_TYPE_ADDRESS_LABEL, (int)strlen(g_label));
          else if (q == INPUT_NUMBER_STRING)
            redefine(argument_name, 0.0, g_label, DEFINITION_TYPE_STRING, (int)strlen(g_label));
          else if (q == INPUT_NUMBER_STACK)
            redefine(argument_name, (double)g_latest_stack, NULL, DEFINITION_TYPE_STACK, 0);
          else if (q == SUCCEEDED)
            redefine(argument_name, g_parsed_double, NULL, DEFINITION_TYPE_VALUE, 0);
          else if (q == INPUT_NUMBER_FLOAT)
            redefine(argument_name, g_parsed_double, NULL, DEFINITION_TYPE_VALUE, 0);
        }
      }

      if (end_of_line == YES)
        next_line();

      mrt->supplied_arguments = p;
      if (_macro_start(m, mrt, MACRO_CALLER_NORMAL, p) == FAILED)
        return FAILED;

      continue;
    }
    else if (q == FAILED) {
      print_error(ERROR_ERR, "Couldn't parse \"%s\".\n", g_tmp);
      return FAILED;
    }
    else {
      print_text(YES, "PHASE_1: Internal error, unknown return type %d.\n", q);
      return FAILED;
    }
  }

  return FAILED;
}


int redefine(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  
  hashmap_get(g_defines_map, name, (void*)&d);
  
  /* it wasn't defined previously */
  if (d == NULL)
    return add_a_new_definition(name, value, string, type, size);

  d->type = type;
  free(d->string);
  d->string = NULL;

  if (type == DEFINITION_TYPE_VALUE)
    d->value = value;
  else if (type == DEFINITION_TYPE_STACK)
    d->value = value;
  else if (type == DEFINITION_TYPE_STRING || type == DEFINITION_TYPE_ADDRESS_LABEL) {
    d->string = _string_duplicate_size(string, size);
    d->size = size;
    if (d->string == NULL)
      return FAILED;
  }

  return SUCCEEDED;
}


int undefine(char *name) {

  struct definition *d;
  
  if (hashmap_get(g_defines_map, name, (void*)&d) != MAP_OK)
    return FAILED;

  hashmap_remove(g_defines_map, name);

  free(d->alias);
  free(d->string);
  free(d);

  return SUCCEEDED;
}


static int _is_a_reserved_label(char *l) {

  /* quick exit */
  if (l[0] != '_')
    return NO;
  
  if (strcmp(l, "_b") == 0 || strcmp(l, "_f") == 0 || strcmp(l, "_B") == 0 || strcmp(l, "_F") == 0) {
    print_error(ERROR_ERR, "\"%s\" is a reserved label, it cannot be defined by the user.\n", l);
    return YES;
  }

  return NO;
}


int add_a_new_definition(char *name, double value, char *string, int type, int size) {

  struct definition *d;
  int err;

  /* we skip definitions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
  if (strcmp(".", name) == 0 || strcmp("_sizeof_.", name) == 0)
    return SUCCEEDED;

  if (_is_a_reserved_label(name) == YES)
    return FAILED;
  
  hashmap_get(g_defines_map, name, (void*)&d);
  if (d != NULL) {
    if (g_commandline_parsing == OFF)
      print_error(ERROR_DIR, "\"%s\" was defined for the second time.\n", name);
    else
      print_text(NO, "ADD_A_NEW_DEFINITION: \"%s\" was defined for the second time.\n", name);
    return FAILED;
  }

  d = calloc(sizeof(struct definition), 1);
  if (d == NULL) {
    if (g_commandline_parsing == OFF)
      print_error(ERROR_DIR, "Out of memory while trying to add a new definition (\"%s\").\n", name);
    else
      print_text(NO, "ADD_A_NEW_DEFINITION: Out of memory while trying to add a new definition (\"%s\").\n", name);
    return FAILED;
  }

  d->type = type;
  d->string = NULL;
  d->alias = _string_duplicate(name);
  if (d->alias == NULL)
    return FAILED;

  if ((err = hashmap_put(g_defines_map, d->alias, d)) != MAP_OK) {
    print_text(NO, "ADD_A_NEW_DEFINITION: Hashmap error %d\n", err);
    return FAILED;
  }

  if (type == DEFINITION_TYPE_VALUE)
    d->value = value;
  else if (type == DEFINITION_TYPE_STACK)
    d->value = value;
  else if (type == DEFINITION_TYPE_STRING || type == DEFINITION_TYPE_ADDRESS_LABEL) {
    d->string = _string_duplicate_size(string, size);
    if (d->string == NULL)
      return FAILED;
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
    print_error(ERROR_NONE, "The label \"%s\" is too long. Max label length is %d bytes.\n", name, MAX_NAME_LENGTH);
    return FAILED;
  }

  return SUCCEEDED;
}


void print_error(int type, char *error, ...) {

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
  va_list args;

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
  }

  if (g_active_file_info_last != NULL)
    print_text(NO, "%s:%d: ", get_file_name(g_active_file_info_last->filename_id), g_active_file_info_last->line_current);

  if (t != NULL) {
    print_text(NO, t);
    print_text(NO, " ");
  }

  va_start(args, error);
  print_text_using_args(NO, error, args);
  va_end(args);

  fflush(stderr);

  return;
}


#if defined(W65816)

void give_snes_rom_mode_defined_error(char *prior) {

  if (g_lorom_defined != 0)
    print_error(ERROR_DIR, ".LOROM was defined prior to %s.\n", prior);
  else if (g_hirom_defined != 0)
    print_error(ERROR_DIR, ".HIROM was defined prior to %s.\n", prior);
  else if (g_exlorom_defined != 0)
    print_error(ERROR_DIR, ".EXLOROM was defined prior to %s.\n", prior);
  else if (g_exhirom_defined != 0)
    print_error(ERROR_DIR, ".EXHIROM was defined prior to %s.\n", prior);
}

#endif


void next_line(void) {

  g_newline_beginning = ON;

  if (s_line_count_status == OFF)
    return;

  if (g_active_file_info_last == NULL)
    return;

  if (g_active_file_info_last != NULL) {
    g_active_file_info_last->line_current++;
    fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
  }
}


/* NOTE! this doesn't work is every case, only for small jumps inside the same source file */
void remember_current_source_file_position(void) {

  s_source_index_old = g_source_index;
  s_line_current_old = g_active_file_info_last->line_current;
}


void roll_back_to_remembered_source_file_position(void) {

  g_source_index = s_source_index_old;
  g_active_file_info_last->line_current = s_line_current_old;
}


/* used by .RAMSECTIONs only */
int add_label_sizeof(char *label, int size) {

  char tmpname[MAX_NAME_LENGTH + 8];
  struct label_sizeof *ls;

  if (g_create_sizeof_definitions == NO)
    return SUCCEEDED;
  
  /* we skip definitions for '_sizeof_.' (because .ENUM and .RAMSECTION use it as an anonymous label) */
  if (strcmp(".", label) == 0)
    return SUCCEEDED;

  ls = calloc(sizeof(struct label_sizeof), 1);
  if (ls == NULL) {
    print_error(ERROR_DIR, "Out of memory error while allocating a label_sizeof structure.\n");
    return FAILED;
  }
  
  strcpy(ls->name, label);
  ls->size = size;
  ls->next = g_label_sizeofs;
  g_label_sizeofs = ls;

  /* define locally also, since we can */
  snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", label);
  if (add_a_new_definition(tmpname, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


/* s_enum_offset and s_last_enum_offset should be set when calling this. */
int add_label_to_enum_or_ramsection(char *name, int size) {

  char tmp[MAX_NAME_LENGTH+10];

  /* there are two passes done when adding a temporary struct to an enum/ramsection. first
     pass is to add the labels, second is to add sizeof definitions. if done in only one
     pass, the resulting sym file is very ugly... */
  if (s_enum_sizeof_pass == NO) {
    if (verify_name_length(name) == FAILED)
      return FAILED;

    if (g_in_enum == YES || g_in_struct == YES) {
      if (add_a_new_definition(name, (double)(s_base_enum_offset+s_enum_offset), NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
      if (s_enum_export == YES)
        if (export_a_definition(name) == FAILED)
          return FAILED;
    }
    else if (g_in_ramsection == YES) {
      if (s_last_enum_offset != s_enum_offset) {
        /* this sometimes abuses the "dsb" implementation to move backwards in the ramsection. */
        fprintf(g_file_out_ptr, "x%d 0 ", s_enum_offset-s_last_enum_offset);
      }
      fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
      /* we skip label emissions for "." (because .ENUM and .RAMSECTION use it as an anonymous label) */
      if (strcmp(".", name) != 0)
        fprintf(g_file_out_ptr, "L%s ", name);
    }
  }
  else {
    /* sizeof pass */
    if (g_in_ramsection == YES) {
      if (add_label_sizeof(name, size) == FAILED)
        return FAILED;
    }
    else {
      if (g_create_sizeof_definitions == YES) {
        snprintf(tmp, sizeof(tmp), "_sizeof_%s", name);
        if (add_a_new_definition(tmp, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;
        if (g_in_enum == YES && s_enum_export == YES) {
          if (export_a_definition(tmp) == FAILED)
            return FAILED;
        }
      }
    }
  }

  s_last_enum_offset = s_enum_offset;

  return SUCCEEDED;
}


static int _add_paddingof_definition(char *name, int padding) {

  char tmp[MAX_NAME_LENGTH+11+1];

  if (g_create_sizeof_definitions == NO)
    return SUCCEEDED;

  snprintf(tmp, sizeof(tmp), "_paddingof_%s", name);
  if (add_a_new_definition(tmp, (double)padding, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
    return FAILED;
  if (g_in_enum == YES && s_enum_export == YES) {
    if (export_a_definition(tmp) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


/* add all fields from a struct at the current offset in the enum/ramsection.
   this is used to construct enums or ramsections through temporary structs, even if
   INSTANCEOF isn't used. s_enum_sizeof_pass should be set to YES or NO before calling. */
int enum_add_struct_fields(char *basename, struct structure *st, int reverse) {

  char tmp[MAX_NAME_LENGTH * 2 + 5];
  struct structure_item *si;

  if (strlen(basename) > MAX_NAME_LENGTH) {
    print_error(ERROR_DIR, "Name \"%s\" is too long!\n", basename);
    return FAILED;
  }

  si = st->items;
  while (si != NULL) {
    int real_si_size = si->size;
    int old_g_enum_offset = s_enum_offset;
    int old_g_last_enum_offset = s_last_enum_offset;
    
    if (si->type == STRUCTURE_ITEM_TYPE_DOTTED || si->type == STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF)
      real_si_size = 0;

    if (reverse)
      s_enum_offset -= real_si_size;

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
    
    /* if this struct has an INSTANCEOF in it, we need to recurse */
    if (si->type == STRUCTURE_ITEM_TYPE_INSTANCEOF || si->type == STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF) {
      int g;

      if (si->num_instances <= 1) {
        /* add definition for first (possibly only) instance of struct */
        int padding = 0;

        if (si->instance->defined_size > 0)
          padding = si->instance->defined_size - si->instance->size;
        
        if (si->defined_size > 0)
          padding = si->defined_size - si->instance->size;

        if (enum_add_struct_fields(tmp, si->instance, 0) == FAILED)
          return FAILED;

        /* there is padding in the INSTANCEOF */
        if (padding > 0) {
          if (s_enum_sizeof_pass == NO) {
            if (g_in_enum == NO && g_in_struct == NO)
              fprintf(g_file_out_ptr, "x%d %d ", padding, g_emptyfill);
          }
          else {
            if (si->name[0] != '\0') {
              if (_add_paddingof_definition(tmp, padding) == FAILED)
                return FAILED;
            }
          }

          s_enum_offset += padding;
          s_last_enum_offset += padding;
        }

        if (si->type == STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF) {
          /* this doesn't advance the counters */
          s_enum_offset = old_g_enum_offset;
          s_last_enum_offset = old_g_last_enum_offset;
        }
      }
      else {
        g = si->start_from;
        while (g < si->start_from + si->num_instances) {
          int padding = 0, size = si->instance->size;

          if (si->instance->defined_size > 0) {
            padding = si->instance->defined_size - si->instance->size;
            size = si->instance->defined_size;
          }
          
          if (si->defined_size > 0) {
            padding = si->defined_size - si->instance->size;
            size = si->defined_size;
          }
          
          if (basename[0] != '\0')
            snprintf(tmp, sizeof(tmp), "%s.%s.%d", basename, si->name, g);
          else
            snprintf(tmp, sizeof(tmp), "%s.%d", si->name, g);

          if (verify_name_length(tmp) == FAILED)
            return FAILED;
          if (add_label_to_enum_or_ramsection(tmp, size) == FAILED)
            return FAILED;
          if (enum_add_struct_fields(tmp, si->instance, 0) == FAILED)
            return FAILED;
          g++;

          /* there is padding in the INSTANCEOF */
          if (padding > 0) {
            if (s_enum_sizeof_pass == NO)
              fprintf(g_file_out_ptr, "x%d %d ", padding, g_emptyfill);
            else {
              if (si->name[0] != '\0') {
                if (_add_paddingof_definition(tmp, padding) == FAILED)
                  return FAILED;
              }
            }
            
            s_enum_offset += padding;
            s_last_enum_offset += padding;
          }
        }

        if (si->type == STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF) {
          /* this doesn't advance the counters */
          s_enum_offset = old_g_enum_offset;
          s_last_enum_offset = old_g_last_enum_offset;
        }
      }
    }
    /* if this struct has a .union in it, we treat each union block like a struct */
    else if (si->type == STRUCTURE_ITEM_TYPE_UNION) {
      int orig_offset = s_enum_offset;
      char union_basename[MAX_NAME_LENGTH * 2 + 5];
      struct structure *un = si->union_items;

      while (un != NULL) {
        s_enum_offset = orig_offset;

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
      s_enum_offset = orig_offset + real_si_size;
    }
    else
      s_enum_offset += real_si_size;

    /* after defining data, go back to start, for DESC enums */
    if (reverse)
      s_enum_offset -= real_si_size;

    si = si->next;
  }

  return SUCCEEDED;
}


static int _remember_new_structure(struct structure *st) {

  int i;

  /* do we already remember the structure? */
  for (i = 0; i < g_saved_structures_count; i++) {
    if (g_saved_structures[i] == st)
      return SUCCEEDED;
  }

  if (g_saved_structures_count >= s_saved_structures_max) {
    s_saved_structures_max += 256;
    g_saved_structures = realloc(g_saved_structures, sizeof(struct structure *) * s_saved_structures_max);
    if (g_saved_structures == NULL) {
      print_text(NO, "_remember_new_structure(): Out of memory error.\n");
      return FAILED;
    }
  }
  
  g_saved_structures[g_saved_structures_count++] = st;

  return SUCCEEDED;
}


static int _add_new_stack_item(char *tmpname, int size, int defined_size, int type, int start_from, struct structure *st, int instances) {
  
  struct structure_item *si;

  /* add this label/value to the struct. */
  si = calloc(sizeof(struct structure_item), 1);
  if (si == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new STRUCT.\n");
    return FAILED;
  }
  si->next = NULL;
  si->instance = NULL;
  si->union_items = NULL;
  strcpy(si->name, tmpname);
  si->size = size;
  si->defined_size = defined_size;
  si->type = type;
  si->start_from = start_from;
  if (type == STRUCTURE_ITEM_TYPE_INSTANCEOF || type == STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF) {
    si->instance = st;
    si->num_instances = instances;
  }
  else if (type == STRUCTURE_ITEM_TYPE_UNION)
    si->union_items = st;

  if (s_active_struct->items == NULL)
    s_active_struct->items = si;
  if (s_active_struct->last_item != NULL)
    s_active_struct->last_item->next = si;
  s_active_struct->last_item = si;

  if (type != STRUCTURE_ITEM_TYPE_DOTTED && type != STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF)
    s_enum_offset += size;

  if (s_enum_offset > s_max_enum_offset)
    s_max_enum_offset = s_enum_offset;

  return SUCCEEDED;
}


/* either "g_in_enum", "g_in_ramsection", or "g_in_struct" should be YES when this is called. */
int parse_enum_token(void) {

  struct structure *st = NULL;
  char tmpname[MAX_NAME_LENGTH + 8 + 1], bak[256];
  int type, size, q, start_from = 1, instances, defined_size;
  
  /* check for "if" directives (the only directives permitted in an enum/ramsection) */
  if (g_tmp[0] == '.') {
    if ((q = parse_if_directive()) != -1)
      return q;
  }

  if (strcaselesscmp(g_tmp, ".UNION") == 0) {
    struct union_stack *ust;
    int inz;

    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error(ERROR_DIR, "PARSE_ENUM_TOKEN: Out of memory error.\n");
      return FAILED;
    }
    st->items = NULL;
    st->name[0] = '\0';
    st->last_item = NULL;
    st->alive = YES;

    if (_remember_new_structure(st) == FAILED)
      return FAILED;
  
    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, g_label);
    else if (inz == INPUT_NUMBER_EOL)
      next_line();

    /* put previous union onto the "stack" */
    ust = calloc(sizeof(struct union_stack), 1);
    if (ust == NULL) {
      print_error(ERROR_DIR, "PARSE_ENUM_TOKEN: Out of memory error.\n");
      free(st);
      return FAILED;
    }
    ust->active_struct = s_active_struct;
    ust->union_first_struct = s_union_first_struct;
    ust->union_base_offset = s_union_base_offset;
    ust->max_enum_offset = s_max_enum_offset;
    ust->prev = s_union_stack;
    s_union_stack = ust;

    s_active_struct = st;
    s_union_first_struct = s_active_struct;
    s_union_base_offset = s_enum_offset;
    s_max_enum_offset = s_union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(g_tmp, ".NEXTU") == 0) {
    int inz;

    if (s_union_stack == NULL) {
      print_error(ERROR_DIR, "There is no open .UNION.\n");
      return FAILED;
    }

    if (s_enum_offset > s_max_enum_offset)
      s_max_enum_offset = s_enum_offset;
    s_active_struct->size = s_enum_offset - s_union_base_offset;
    st = calloc(sizeof(struct structure), 1);
    if (st == NULL) {
      print_error(ERROR_DIR, "PARSE_ENUM_TOKEN: Out of memory error.\n");
      return FAILED;
    }
    st->name[0] = '\0';
    st->items = NULL;
    st->last_item = NULL;
    st->alive = YES;

    if (_remember_new_structure(st) == FAILED)
      return FAILED;
    
    inz = input_next_string();
    if (inz == FAILED) {
      free(st);
      return FAILED;
    }
    else if (inz == SUCCEEDED)
      strcpy(st->name, g_label);
    else if (inz == INPUT_NUMBER_EOL)
      next_line();

    s_active_struct->next = st;
    s_active_struct = st;
    s_enum_offset = s_union_base_offset;
    return SUCCEEDED;
  }
  else if (strcaselesscmp(g_tmp, ".ENDU") == 0) {
    struct structure_item *si;
    struct union_stack *ust;
    int total_size;

    if (s_union_stack == NULL) {
      print_error(ERROR_DIR, "There is no open .UNION.\n");
      return FAILED;
    }

    if (s_enum_offset > s_max_enum_offset)
      s_max_enum_offset = s_enum_offset;

    total_size = s_max_enum_offset - s_union_base_offset;

    s_active_struct->size = s_enum_offset - s_union_base_offset;
    s_active_struct->next = NULL;

    st = s_union_first_struct;

    s_enum_offset = s_max_enum_offset;

    /* pop previous union from the "stack" */
    ust = s_union_stack;
    s_active_struct = s_union_stack->active_struct;
    s_union_first_struct = s_union_stack->union_first_struct;
    s_union_base_offset = ust->union_base_offset;
    s_max_enum_offset = ust->max_enum_offset;
    s_union_stack = s_union_stack->prev;
    free(ust);

    /* just popped s_max_enum_offset; need to update it for end of union */
    if (s_enum_offset > s_max_enum_offset)
      s_max_enum_offset = s_enum_offset;

    /* create a new structure item of type STRUCTURE_ITEM_TYPE_UNION */
    si = calloc(sizeof(struct structure_item), 1);
    if (si == NULL) {
      print_error(ERROR_DIR, "PARSE_ENUM_TOKEN: Out of memory error.\n");
      return FAILED;
    }
    si->name[0] = '\0';
    si->type = STRUCTURE_ITEM_TYPE_UNION;
    si->size = total_size;
    si->next = NULL;
    si->union_items = st;
    if (s_active_struct->items == NULL)
      s_active_struct->items = si;
    if (s_active_struct->last_item != NULL)
      s_active_struct->last_item->next = si;
    s_active_struct->last_item = si;
    return SUCCEEDED;
  }
  else if (g_in_enum == YES && strcaselesscmp(g_tmp, ".ENDE") == 0) {
    if (s_union_stack != NULL) {
      print_error(ERROR_DIR, ".UNION not closed.\n");
      return FAILED;
    }
    
    s_enum_offset = 0;
    s_enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", s_active_struct, (s_enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    s_enum_offset = 0;
    s_enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", s_active_struct, (s_enum_ord == -1 ? 1 : 0)) == FAILED)
      return FAILED;

    s_active_struct = NULL;

    g_in_enum = NO;
    return SUCCEEDED;
  }
  else if (g_in_ramsection == YES && strcaselesscmp(g_tmp, ".ENDS") == 0) {
    if (s_union_stack != NULL) {
      print_error(ERROR_DIR, ".UNION not closed.\n");
      return FAILED;
    }

    s_enum_offset = 0;
    s_last_enum_offset = 0;
    s_enum_sizeof_pass = NO;
    if (enum_add_struct_fields("", s_active_struct, 0) == FAILED)
      return FAILED;

    s_enum_offset = 0;
    s_last_enum_offset = 0;
    s_enum_sizeof_pass = YES;
    if (enum_add_struct_fields("", s_active_struct, 0) == FAILED)
      return FAILED;

    if (s_max_enum_offset > s_last_enum_offset)
      fprintf(g_file_out_ptr, "o%d 0 ", s_max_enum_offset-s_last_enum_offset);

    /* generate a section end label? */
    if (g_extra_definitions == ON)
      generate_label("SECTIONEND_", g_sections_last->name);

    s_active_struct = NULL;
    s_active_ramsection = NULL;

    fprintf(g_file_out_ptr, "s ");
    g_section_status = OFF;
    g_in_ramsection = NO;

    return SUCCEEDED;
  }
  else if (g_in_struct && strcaselesscmp(g_tmp, ".ENDST") == 0) {
    s_enum_offset = 0;
    s_last_enum_offset = 0;
    s_enum_sizeof_pass = NO;
    if (enum_add_struct_fields(s_active_struct->name, s_active_struct, 0) == FAILED)
      return FAILED;

    s_enum_offset = 0;
    s_last_enum_offset = 0;
    s_enum_sizeof_pass = YES;
    if (enum_add_struct_fields(s_active_struct->name, s_active_struct, 0) == FAILED)
      return FAILED;
    
    /* create the SIZEOF-definition for the entire struct */
    s_active_struct->size = s_max_enum_offset;

    if (s_active_struct->defined_size > 0 && s_active_struct->size > s_active_struct->defined_size) {
      print_error(ERROR_DIR, ".STRUCT \"%s\"'s calculated size is %d, but explicitly given SIZE is %d -> make SIZE larger!\n", s_active_struct->name, s_active_struct->size, s_active_struct->defined_size);
      return FAILED;
    }
    
    if (g_create_sizeof_definitions == YES) {
      int size = s_active_struct->size;
      
      if (strlen(s_active_struct->name) > MAX_NAME_LENGTH - 8) {
        print_error(ERROR_DIR, ".STRUCT \"%s\"'s name is too long!\n", s_active_struct->name);
        return FAILED;
      }

      if (s_active_struct->defined_size > 0)
        size = s_active_struct->defined_size;
      
      snprintf(tmpname, sizeof(tmpname), "_sizeof_%s", s_active_struct->name);
      if (add_a_new_definition(tmpname, (double)size, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;
    }
    
    if (s_active_struct->items == NULL) {
      print_error(ERROR_DIR, ".STRUCT \"%s\" is empty!\n", s_active_struct->name);
      return FAILED;
    }

    s_active_struct->next = g_structures_first;
    g_structures_first = s_active_struct;

    g_in_struct = NO;
    s_active_struct = NULL;

    return SUCCEEDED;
  }
  else if (g_in_ramsection == YES && strcaselesscmp(g_tmp, ".ALIGN") == 0) {
    int remainder;
    
    /* get the alignment */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || g_parsed_int < 2) {
      print_error(ERROR_DIR, ".ALIGN needs an integer greater than 1.\n");
      return FAILED;
    }

    if ((s_active_ramsection->alignment % g_parsed_int) != 0) {
      print_error(ERROR_DIR, ".ALIGN works currently in .RAMSECTIONs that have ALIGN that is a multiple of .ALIGN.\n");
      return FAILED;
    }

    remainder = s_enum_offset % g_parsed_int;
    if (remainder > 0) {
      if (_add_new_stack_item("", g_parsed_int - remainder, -1, STRUCTURE_ITEM_TYPE_DATA, 1, NULL, 1) == FAILED)
      return FAILED;
    }
    
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
  
  defined_size = -1;
  instances = 1;

  /* notice that "type" and "size" are defined in every branch of this if-elif-else construction */
  
  if (strcaselesscmp(g_tmp, "DB") == 0 || strcaselesscmp(g_tmp, "BYT") == 0 || strcaselesscmp(g_tmp, "BYTE") == 0) {
    size = 1;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DW") == 0 || strcaselesscmp(g_tmp, "WORD") == 0 || strcaselesscmp(g_tmp, "ADDR") == 0) {
    size = 2;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DL") == 0 || strcaselesscmp(g_tmp, "LONG") == 0 || strcaselesscmp(g_tmp, "FARADDR") == 0) {
    size = 3;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DD") == 0) {
    size = 4;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DS") == 0 || strcaselesscmp(g_tmp, "DSB") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, "DS/DSB needs size.\n");
      return FAILED;
    }
    size = g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSW") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, "DSW needs size.\n");
      return FAILED;
    }
    size = 2*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSL") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, "DSL needs size.\n");
      return FAILED;
    }
    size = 3*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  else if (strcaselesscmp(g_tmp, "DSD") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, "DSD needs size.\n");
      return FAILED;
    }
    size = 4*g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DATA;
  }
  /* it's an instance of a structure! */
  else if (strcaselesscmp(g_tmp, "INSTANCEOF") == 0 || strcaselesscmp(g_tmp, ".INSTANCEOF") == 0) {
    if (g_tmp[0] == '.')
      type = STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF;
    else
      type = STRUCTURE_ITEM_TYPE_INSTANCEOF;

    q = get_next_plain_string();
    if (q == FAILED)
      return FAILED;
    if (q == INPUT_NUMBER_EOL) {
      print_error(ERROR_DIR, "INSTANCEOF need a .STRUCT name.\n");
      return FAILED;
    }
    
    st = _get_structure(g_label);

    if (st == NULL) {
      print_error(ERROR_DIR, "No .STRUCT named \"%s\" available.\n", g_label);
      return FAILED;
    }

    while (1) {
      /* is the next token SIZE? */
      if (compare_next_token("SIZE") == SUCCEEDED) {
        remember_current_source_file_position();

        skip_next_token();

        q = input_number();
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          /* SIZE was actually a field? roll back */
          roll_back_to_remembered_source_file_position();

          break;
        }
        else {
          if (g_parsed_int < 1) {
            print_error(ERROR_DIR, "SIZE must be > 0.\n");
            return FAILED;
          }
          if (g_parsed_int < st->size) {
            print_error(ERROR_DIR, ".STRUCT \"%s\"'s calculated size is %d, but explicitly given SIZE is %d -> make SIZE larger!\n", st->name, st->size, g_parsed_int);
            return FAILED;
          }
          
          defined_size = g_parsed_int;
        }
      }
      else if (compare_next_token("COUNT") == SUCCEEDED) {
        remember_current_source_file_position();

        skip_next_token();

        q = input_number();
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          /* COUNT was actually a field? roll back */
          roll_back_to_remembered_source_file_position();

          break;
        }
        else {
          if (g_parsed_int < 1) {
            print_error(ERROR_DIR, "COUNT must be > 0.\n");
            return FAILED;
          }

          instances = g_parsed_int;
        }
      }
      else if (compare_next_token("STARTFROM") == SUCCEEDED) {
        remember_current_source_file_position();
        
        skip_next_token();

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          /* STARTFROM was actually a field? roll back */
          roll_back_to_remembered_source_file_position();

          break;
        }

        if (g_parsed_int < 0) {
          print_error(ERROR_DIR, "STARTFROM needs to be >= 0.\n");
          return FAILED;
        }

        start_from = g_parsed_int;
      }
      else {
        /* get the number of structures to be made */
        q = input_number();
        if (q == INPUT_NUMBER_EOL) {
          next_line();
          break;
        }
        else if (q == SUCCEEDED) {
          if (g_parsed_int < 1) {
            print_error(ERROR_DIR, "The number of structures must be greater than 0.\n");
            return FAILED;
          }

          instances = g_parsed_int;
        }
        else {
          if (q == INPUT_NUMBER_STRING)
            print_error(ERROR_DIR, "Expected the number of structures, got \"%s\" instead.\n", g_label);
          else
            print_error(ERROR_DIR, "Expected the number of structures.\n");
          return FAILED;
        }

        /* test for EOL */
        remember_current_source_file_position();

        q = input_number();
        if (q == INPUT_NUMBER_EOL) {
          next_line();
          break;
        }
        else {
          /* this is not yet the end */
          roll_back_to_remembered_source_file_position();
        }
      }      
    }

    if (defined_size > 0)
      size = defined_size * instances;
    else {
      if (st->defined_size > 0)
        size = st->defined_size * instances;
      else
        size = st->size * instances;
    }
  }
  else if (strcaselesscmp(g_tmp, ".db") == 0 || strcaselesscmp(g_tmp, ".byt") == 0 || strcaselesscmp(g_tmp, ".byte") == 0) {
    /* don't do anything for "dotted" versions */
    size = 1;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dw") == 0 || strcaselesscmp(g_tmp, ".word") == 0 || strcaselesscmp(g_tmp, ".addr") == 0) {
    /* don't do anything for "dotted" versions */
    size = 2;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".ds") == 0 || strcaselesscmp(g_tmp, ".dsb") == 0 || strcaselesscmp(g_tmp, ".dsw") == 0) {
    /* don't do anything for "dotted" versions */
    strcpy(bak, g_tmp);
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, "%s needs size.\n", bak);
      return FAILED;
    }

    if (strcaselesscmp(bak, ".dsw") == 0)
      g_parsed_int *= 2;

    size = g_parsed_int;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dl") == 0 || strcaselesscmp(g_tmp, ".long") == 0 || strcaselesscmp(g_tmp, ".faraddr") == 0) {
    /* don't do anything for "dotted" versions */
    size = 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dsl") == 0) {
    /* don't do anything for "dotted" versions */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, ".DSL needs size.\n");
      return FAILED;
    }

    size = g_parsed_int * 3;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dd") == 0) {
    /* don't do anything for "dotted" versions */
    size = 4;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else if (strcaselesscmp(g_tmp, ".dsd") == 0) {
    /* don't do anything for "dotted" versions */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_DIR, ".DSD needs size.\n");
      return FAILED;
    }

    size = g_parsed_int * 4;
    type = STRUCTURE_ITEM_TYPE_DOTTED;
  }
  else {
    if (g_in_enum == YES)
      print_error(ERROR_DIR, "Unexpected symbol \"%s\" in .ENUM.\n", g_tmp);
    else if (g_in_ramsection == YES)
      print_error(ERROR_DIR, "Unexpected symbol \"%s\" in .RAMSECTION.\n", g_tmp);
    else /* struct */
      print_error(ERROR_DIR, "Unexpected symbol \"%s\" in .STRUCT.\n", g_tmp);
    return FAILED;
  }

  if (_add_new_stack_item(tmpname, size, defined_size, type, start_from, st, instances) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int directive_org(void) {

  int q;
  
  no_library_files(".ORG definitions");
    
  if (s_bank_defined == 0) {
    print_error(ERROR_LOG, "No .BANK is defined.\n");
    return FAILED;
  }
  if (g_section_status == ON) {
    print_error(ERROR_DIR, "You can't issue .ORG inside a .SECTION.\n");
    return FAILED;
  }
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't issue .ORGA inside .DSTRUCT.\n");
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error(ERROR_DIR, ".ORG needs a positive or zero integer value.\n");
    return FAILED;
  }

  g_org_defined = 1;
  fprintf(g_file_out_ptr, "O%d ", g_parsed_int);

  return SUCCEEDED;
}


int directive_orga(void) {

  int q, current_slot_address;
  
  no_library_files(".ORGA definitions");
    
  if (s_bank_defined == 0) {
    print_error(ERROR_LOG, "No .BANK is defined.\n");
    return FAILED;
  }
  if (g_section_status == ON) {
    print_error(ERROR_DIR, "You can't issue .ORGA inside a .SECTION.\n");
    return FAILED;
  }
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't issue .ORGA inside .DSTRUCT.\n");
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q != SUCCEEDED) {
    print_error(ERROR_DIR, ".ORGA needs a positive or zero integer value.\n");
    return FAILED;
  }

  g_org_defined = 1;

  current_slot_address = g_slots[s_current_slot].address;
  if (g_parsed_int < current_slot_address || g_parsed_int > (current_slot_address + g_slots[s_current_slot].size)) {
    print_error(ERROR_DIR, ".ORGA is outside the current SLOT.\n");
    return FAILED;
  }

  fprintf(g_file_out_ptr, "O%d ", g_parsed_int - current_slot_address);

  return SUCCEEDED;
}


int directive_slot(void) {

  int q;
  
  no_library_files(".SLOT definitions");
    
  if (g_section_status == ON) {
    print_error(ERROR_DIR, "You can't issue .SLOT inside a .SECTION.\n");
    return FAILED;
  }
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't issue .SLOT inside .DSTRUCT.\n");
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
    print_error(ERROR_DIR, "Cannot find the SLOT.\n");
    return FAILED;
  }

  if (g_parsed_int < 0 || g_parsed_int > 255 || g_slots[g_parsed_int].size == 0) {
    print_error(ERROR_DIR, "There is no SLOT number %d.\n", g_parsed_int);
    return FAILED;
  }

  fprintf(g_file_out_ptr, "B%d %d ", s_bank, g_parsed_int);

  s_current_slot = g_parsed_int;

  return SUCCEEDED;
}


int directive_bank(void) {

  int q, bank, slot;
  
  no_library_files(".BANK definitions");
    
  if (g_section_status == ON) {
    print_error(ERROR_LOG, "Section \"%s\" is open. Do not try to change the bank.\n", g_sections_last->name);
    return FAILED;
  }
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't use .BANK inside .DSTRUCT.\n");
    return FAILED;
  }
  if (g_rombanks_defined == 0 && g_output_format != OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, ".ROMBANKS is not yet defined.\n");
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 0) {
    print_error(ERROR_DIR, ".BANK number must be zero or positive.\n");
    return FAILED;
  }

  if (g_rombanks <= g_parsed_int && g_output_format != OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "ROM banks == %d, selected bank %d.\n", g_rombanks, g_parsed_int);
    return FAILED;
  }

  s_bank = g_parsed_int;
  s_bank_defined = 1;

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
      print_error(ERROR_DIR, "Cannot find the SLOT.\n");
      return FAILED;
    }

    if (g_slots[g_parsed_int].size == 0) {
      print_error(ERROR_DIR, "There is no SLOT number %d.\n", g_parsed_int);
      return FAILED;
    }

    if (g_output_format != OUTPUT_LIBRARY)
      fprintf(g_file_out_ptr, "B%d %d ", s_bank, g_parsed_int);

    bank = s_bank;
    slot = g_parsed_int;
    s_current_slot = g_parsed_int;
  }
  else {
    fprintf(g_file_out_ptr, "B%d %d ", g_parsed_int, s_defaultslot);
    bank = g_parsed_int;
    slot = s_defaultslot;
    s_current_slot = s_defaultslot;
  }

  if (g_slots[slot].size < g_banks[bank]) {
    print_error(ERROR_DIR, "SLOT %d's size %d < BANK %d's size %d.\n", slot, g_slots[slot].size, bank, g_banks[bank]);
    return FAILED;
  }
  if (g_slots[slot].size > g_banks[bank]) {
    print_error(ERROR_WRN, "SLOT %d's size %d > BANK %d's size %d, but the bank fits inside.\n", slot, g_slots[slot].size, bank, g_banks[bank]);
  }

  return SUCCEEDED;
}


int directive_dbm_dwm_dlm_ddm_filter(void) {

  struct macro_static *macro;

  if (input_number() != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".%s requires macro name.\n", g_current_directive);
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
    print_error(ERROR_DIR, "No MACRO \"%s\" defined.\n", g_label);
    return FAILED;
  }

  if (strcaselesscmp(g_current_directive, "DBM") == 0) {
    if (_macro_start_dxm(macro, MACRO_CALLER_DBM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DLM") == 0) {
    if (_macro_start_dxm(macro, MACRO_CALLER_DLM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DDM") == 0) {
    if (_macro_start_dxm(macro, MACRO_CALLER_DDM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "DWM") == 0) {
    if (_macro_start_dxm(macro, MACRO_CALLER_DWM, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else if (strcaselesscmp(g_current_directive, "FILTER") == 0) {
    if (_macro_start_dxm(macro, MACRO_CALLER_FILTER, g_current_directive, YES) == FAILED)
      return FAILED;
  }
  else {
    print_error(ERROR_DIR, "Unsupported filter macro directive \"%s\". Please submit a bug report!\n", g_current_directive);
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_table(void) {

  char bak[256];
  int result, i;

  result = input_number();
  for (s_table_size = 0; s_table_size < (int)sizeof(s_table_format) && (result == INPUT_NUMBER_STRING || result == INPUT_NUMBER_ADDRESS_LABEL); ) {
    if (strcaselesscmp(g_label, "db") == 0 || strcaselesscmp(g_label, "byte") == 0 || strcaselesscmp(g_label, "byt") == 0) {
      s_table_format[s_table_size++] = 'b';
    }
    else if (strcaselesscmp(g_label, "ds") == 0 || strcaselesscmp(g_label, "dsb") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        print_error(ERROR_INP, "%s needs size.\n", bak);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && s_table_size < (int)sizeof(s_table_format); i++)
        s_table_format[s_table_size++] = 'b';
    }
    else if (strcaselesscmp(g_label, "dw") == 0 || strcaselesscmp(g_label, "word") == 0 || strcaselesscmp(g_label, "addr") == 0) {
      s_table_format[s_table_size++] = 'w';
    }
    else if (strcaselesscmp(g_label, "dsw") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        print_error(ERROR_INP, "%s needs size.\n", bak);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && s_table_size < (int)sizeof(s_table_format); i++)
        s_table_format[s_table_size++] = 'w';
    }
    else if (strcaselesscmp(g_label, "dl") == 0 || strcaselesscmp(g_label, "long") == 0 || strcaselesscmp(g_label, "faraddr") == 0) {
      s_table_format[s_table_size++] = 'l';
    }
    else if (strcaselesscmp(g_label, "dsl") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        print_error(ERROR_INP, "%s needs size.\n", bak);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && s_table_size < (int)sizeof(s_table_format); i++)
        s_table_format[s_table_size++] = 'l';
    }
    else if (strcaselesscmp(g_label, "dd") == 0) {
      s_table_format[s_table_size++] = 'd';
    }
    else if (strcaselesscmp(g_label, "dsd") == 0) {
      strcpy(bak, g_label);

      result = input_number();
      if (result == FAILED)
        return FAILED;
      if (result != SUCCEEDED) {
        print_error(ERROR_INP, "%s needs size.\n", bak);
        return FAILED;
      }

      for (i = 0; i < g_parsed_int && s_table_size < (int)sizeof(s_table_format); i++)
        s_table_format[s_table_size++] = 'd';
    }
    else {
      print_error(ERROR_DIR, "Unknown symbol \"%s\".\n", g_label);
      return FAILED;
    }
      
    result = input_number();
  }

  if (s_table_size >= (int)sizeof(s_table_format)) {
    print_error(ERROR_DIR, ".TABLE is out of size.\n");
    return FAILED;
  }

  if (result == FAILED)
    return FAILED;
  else if (result == INPUT_NUMBER_EOL && s_table_size == 0) {
    print_error(ERROR_INP, ".TABLE needs data.\n");
    return FAILED;
  }
  else if (result == INPUT_NUMBER_EOL)
    next_line();
  else {
    print_error(ERROR_DIR, "Unknown symbol.\n");
    return FAILED;      
  }

  s_table_defined = 1;
  s_table_index = 0;

  return SUCCEEDED;    
}


int directive_row_data(void) {

  char bak[256];
  int rows = 0, result, i;
  
  strcpy(bak, g_current_directive);

  if (s_table_defined == 0) {
    print_error(ERROR_DIR, ".TABLE needs to be defined before .%s can be used.\n", bak);
    return FAILED;
  }
    
  if (strcaselesscmp(bak, "ROW") == 0) {
    if (s_table_index != 0) {
      print_error(ERROR_DIR, ".ROW cannot be used here. .DATA needs to be used again to give the remaining of the row.\n");
      return FAILED;
    }
  }

  result = input_number();
  i = 0;
  for ( ; result == SUCCEEDED || result == INPUT_NUMBER_STRING || result == INPUT_NUMBER_ADDRESS_LABEL || result == INPUT_NUMBER_STACK; ) {
    if (result == INPUT_NUMBER_STRING) {
      if (s_table_format[s_table_index] == 'b') {
        if (strlen(g_label) != 1) {
          print_error(ERROR_INP, ".%s was expecting a byte, got %d bytes instead.\n", bak, (int)strlen(g_label));
          return FAILED;
        }

        fprintf(g_file_out_ptr, "d%d ", g_label[0]);          
      }
      else if (s_table_format[s_table_index] == 'w') {
        if (strlen(g_label) > 2 || strlen(g_label) <= 0) {
          print_error(ERROR_INP, ".%s was expecting a word (2 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", (g_label[0] << 8) | g_label[1]);
      }
      else if (s_table_format[s_table_index] == 'l') {
        if (strlen(g_label) > 3 || strlen(g_label) <= 0) {
          print_error(ERROR_INP, ".%s was expecting a long (3 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", (g_label[0] << 16) | (g_label[1] << 8) | g_label[2]);
      }
      else if (s_table_format[s_table_index] == 'd') {
        if (strlen(g_label) > 4 || strlen(g_label) <= 0) {
          print_error(ERROR_INP, ".%s was expecting a double word (4 bytes), got %d bytes instead.\n", bak, (int)strlen(g_label));
          return FAILED;
        }

        fprintf(g_file_out_ptr, "u%d ", (g_label[0] << 24) | (g_label[1] << 16) | (g_label[2] << 8) | g_label[3]);
      }
      else {
        print_error(ERROR_DIR, ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, s_table_format[s_table_index]);
        return FAILED;
      }
    }
    else if (result == SUCCEEDED) {
      if (s_table_format[s_table_index] == 'b') {
        if (g_parsed_int < -128 || g_parsed_int > 255) {
          print_error(ERROR_DIR, ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
          return FAILED;
        }
    
        fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
      }
      else if (s_table_format[s_table_index] == 'w') {
        if (g_parsed_int < -32768 || g_parsed_int > 65535) {
          print_error(ERROR_DIR, ".%s expects 16-bit data, %d is out of range!\n", bak, g_parsed_int);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", g_parsed_int);
      }
      else if (s_table_format[s_table_index] == 'l') {
        if (g_parsed_int < -8388608 || g_parsed_int > 16777215) {
          print_error(ERROR_DIR, ".%s expects 24-bit data, %d is out of range!\n", bak, g_parsed_int);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", g_parsed_int);
      }
      else if (s_table_format[s_table_index] == 'd') {
        /*
        if (g_parsed_int < -2147483648 || g_parsed_int > 2147483647) {
          print_error(ERROR_DIR, ".%s expects 32-bit data, %d is out of range!\n", bak, g_parsed_int);
          return FAILED;
        }
        */

        fprintf(g_file_out_ptr, "u%d ", g_parsed_int);
      }
      else {
        print_error(ERROR_DIR, ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, s_table_format[s_table_index]);
        return FAILED;
      }
    }
    else if (result == INPUT_NUMBER_ADDRESS_LABEL) {
      if (s_table_format[s_table_index] == 'b')
        fprintf(g_file_out_ptr, "k%d Q%s ", g_active_file_info_last->line_current, g_label);
      else if (s_table_format[s_table_index] == 'w')
        fprintf(g_file_out_ptr, "k%d r%s ", g_active_file_info_last->line_current, g_label);
      else if (s_table_format[s_table_index] == 'l')
        fprintf(g_file_out_ptr, "k%d q%s ", g_active_file_info_last->line_current, g_label);
      else if (s_table_format[s_table_index] == 'd')
        fprintf(g_file_out_ptr, "k%d V%s ", g_active_file_info_last->line_current, g_label);
      else {
        print_error(ERROR_DIR, ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, s_table_format[s_table_index]);
        return FAILED;
      }
    }
    else if (result == INPUT_NUMBER_STACK) {
      if (s_table_format[s_table_index] == 'b')
        fprintf(g_file_out_ptr, "c%d ", g_latest_stack);
      else if (s_table_format[s_table_index] == 'w')
        fprintf(g_file_out_ptr, "C%d ", g_latest_stack);
      else if (s_table_format[s_table_index] == 'l')
        fprintf(g_file_out_ptr, "T%d ", g_latest_stack);
      else if (s_table_format[s_table_index] == 'd')
        fprintf(g_file_out_ptr, "U%d ", g_latest_stack);
      else {
        print_error(ERROR_DIR, ".%s has encountered an unsupported internal datatype \"%c\".\n", bak, s_table_format[s_table_index]);
        return FAILED;
      }
    }

    i++;
    s_table_index++;
    if (s_table_index >= s_table_size) {
      rows++;
      s_table_index = 0;
    }

    result = input_number();
  }

  if (result == FAILED)
    return FAILED;

  if (result == INPUT_NUMBER_EOL && i == 0) {
    print_error(ERROR_INP, ".%s needs data.\n", bak);
    return FAILED;
  }

  if (strcaselesscmp(bak, "ROW") == 0) {
    if (s_table_index != 0 || rows != 1) {
      print_error(ERROR_INP, ".ROW needs exactly one row of data, no more, no less.\n");
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
      print_error(ERROR_DIR, ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
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
    print_error(ERROR_INP, ".%s needs data.\n", bak);
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

  int i, o, nybble_2 = 0, error, number_result, is_block = NO, is_done = NO;

  if (compare_next_token("BLOCK") == SUCCEEDED) {
    skip_next_token();
    is_block = YES;
  }

  /* make sure input_number() returns things like 01 as strings, not as numbers */
  g_input_number_turn_values_into_strings = YES;

  number_result = input_number();

  for (i = 0; is_done == NO; i++) {
    if (is_block == NO) {
      if (number_result == INPUT_NUMBER_EOL)
        break;
    }
    else {
      while (number_result == INPUT_NUMBER_EOL) {
        next_line();
        number_result = input_number();
      }
    }

    if (number_result == FAILED)
      return FAILED;
    if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(ERROR_INP, ".HEX needs data in string format.\n");
      return FAILED;
    }

    if (is_block == YES) {
      if (strcaselesscmp(g_label, ".ENDHEX") == 0)
        break;
    }

    g_string_size = (int)strlen(g_label);
    if ((g_string_size & 1) == 1) {
      print_error(ERROR_INP, "The string length for .HEX must be a multiple of 2.\n");
      return FAILED;
    }
      
    for (o = 0; o < g_string_size; ) {
      int nybble_1;
      
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
        print_error(ERROR_DIR, "'%c' does not belong to a hexadecimal value!\n", g_label[o]);
        return FAILED;
      }
        
      fprintf(g_file_out_ptr, "d%d ", (nybble_1 << 4) | nybble_2);
    }

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if (number_result == INPUT_NUMBER_EOL && i == 0) {
    print_error(ERROR_INP, ".HEX needs data.\n");
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  g_input_number_turn_values_into_strings = NO;
  
  return SUCCEEDED;
}


int directive_bits(void) {

  int bits, q;

  q = input_number();

  if (q == FAILED)
    return FAILED;
  else if (q == SUCCEEDED) {
    /* immediate values are accepted */
  }
  else {
    print_error(ERROR_DIR, ".BITS needs an immediate value for the number of bits.\n");
    return FAILED;
  }  

  bits = g_parsed_int;

  if (bits < 1 || bits > 32) {
    print_error(ERROR_DIR, "The number of bits needs to be between 1 and 32.\n");
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
        print_error(ERROR_DIR, "We are defining %d bits, but the given value $%x (%d) uses more bits!\n", bits, g_parsed_int, g_parsed_int);
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
      print_error(ERROR_DIR, ".BITS data must be immediate values, address labels or calculation stacks.\n");
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_align(void) {

  int q, address, align, remainder;
  struct section_def *s;

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 2) {
    print_error(ERROR_DIR, ".ALIGN needs an integer greater than 1.\n");
    return FAILED;
  }

  align = g_parsed_int;
  
  /* fast forward to the current point to calculate our current address */
  if (data_stream_parser_parse() == FAILED)
    return FAILED;

  s = data_stream_parser_get_current_section();
  address = data_stream_parser_get_current_address();
  
  if (s != NULL) {
    /* check if the section works with .ALIGN */
    if (s->status == SECTION_STATUS_FORCE) {
      /* this is good */
    }
    else if ((s->alignment % align) == 0) {
      /* this is also good */
      address = s->alignment + address - s->address_from_dsp;
    }
    else {
      print_error(ERROR_DIR, ".ALIGN works currently in FORCE .SECTIONs and .SECTIONs that have ALIGN that is a multiple of .ALIGN.\n");
      return FAILED;
    }

    /* emptyfill until aligned... */
    remainder = address % align;

    if (remainder != 0) {
      int target = address - remainder + align;

      while (address < target) {
        fprintf(g_file_out_ptr, "d%d ", g_emptyfill);
        address++;
      }
    }
  }
  else {
    /* just redefine .ORG */
    remainder = address % align;

    if (remainder != 0)
      fprintf(g_file_out_ptr, "O%d ", address - remainder + align);
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
        print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value or one letter string.\n");
        return FAILED;
      }
      if (q == INPUT_NUMBER_STRING) {
        if (g_string_size != 1) {
          print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value or one letter string.\n");
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
          print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value or one letter string.\n");
          return FAILED;
        }
        if (q == INPUT_NUMBER_STRING) {
          if (g_string_size != 1) {
            print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value or one letter string.\n");
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
        print_error(ERROR_DIR, "The end address of the mapping must be larger than the staring address.\n");
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
        print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value or one letter string.\n");
        return FAILED;
      }
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "The entry must be a positive 8-bit immediate value.\n");
        return FAILED;
      }

      /* build the mapping */
      for (o = astart; o < aend; o++) {
        if (g_parsed_int >= 256) {
          print_error(ERROR_DIR, "The mapping overflows from the ASCII table!\n");
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
    print_error(ERROR_DIR, "Error in .%s data structure.\n", bak);
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
    print_error(ERROR_WRN, "No .ASCIITABLE defined. Using the default n->n -mapping.\n");
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
          print_error(ERROR_INP, "Expected a byte value between -128 and 255, got %d.\n", g_parsed_int);
          return FAILED;
        }
        fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
      }
      else {
        /* remap the byte */
        if (g_parsed_int < 0 || g_parsed_int > 255) {
          print_error(ERROR_INP, ".%s needs string / byte (0-255) data.\n", bak);
          return FAILED;
        }
        character = (int)g_asciitable[g_parsed_int];
        fprintf(g_file_out_ptr, "d%d ", character);
      }
    }
    else {
      print_error(ERROR_INP, ".%s needs string / byte (0-255) data.\n", bak);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_dw_word_addr(void) {

  int i, number_result;
  char bak[256];

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
      print_error(ERROR_DIR, ".%s expects 16-bit data, %d is out of range!\n", bak, g_parsed_int);
      return FAILED;
    }

    if (number_result == SUCCEEDED)
      fprintf(g_file_out_ptr, "y%d ", g_parsed_int);
    else if (number_result == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(g_file_out_ptr, "r%s ", g_label);
    else if (number_result == INPUT_NUMBER_STACK)
      fprintf(g_file_out_ptr, "C%d ", g_latest_stack);

    number_result = input_number();
  }

  if (number_result == FAILED)
    return FAILED;

  if ((number_result == INPUT_NUMBER_EOL || number_result == INPUT_NUMBER_STRING) && i == 0) {
    print_error(ERROR_INP, ".%s needs data.\n", bak);
    return FAILED;
  }

  if (number_result == INPUT_NUMBER_EOL)
    next_line();

  return SUCCEEDED;
}


int directive_dl_long_faraddr(void) {

  int i, number_result;
  char bak[256];

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    if (number_result == SUCCEEDED && (g_parsed_int < -8388608 || g_parsed_int > 16777215)) {
      print_error(ERROR_DIR, ".%s expects 24-bit data, %d is out of range!\n", bak, g_parsed_int);
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
    print_error(ERROR_INP, ".%s needs data.\n", bak);
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
    print_error(ERROR_INP, ".DSL needs size.\n");
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    print_error(ERROR_DIR, ".DSL expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_INP, ".DSL needs data.\n");
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int < -8388608 || g_parsed_int > 16777215)) {
    print_error(ERROR_DIR, ".DSL expects 24-bit data, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "h%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "q%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    int latest_stack = g_latest_stack;
    
    for (q = 0; q < parsed_int; q++) {
      if (q == 0)
        fprintf(g_file_out_ptr, "T%d ", latest_stack);
      else {
        stack_create_stack_stack(latest_stack);
        fprintf(g_file_out_ptr, "T%d ", g_latest_stack);
      }
    }
  }

  return SUCCEEDED;
}


int directive_dd(void) {

  int i, number_result;
  char bak[256];

  strcpy(bak, g_current_directive);

  number_result = input_number();
  for (i = 0; number_result == SUCCEEDED || number_result == INPUT_NUMBER_ADDRESS_LABEL || number_result == INPUT_NUMBER_STACK; i++) {
    /*
    if (number_result == SUCCEEDED && (g_parsed_int < -2147483648 || g_parsed_int > 2147483647)) {
      print_error(ERROR_DIR, ".%s expects 32-bit data, %d is out of range!\n", bak, g_parsed_int);
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
    print_error(ERROR_INP, ".%s needs data.\n", bak);
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
    print_error(ERROR_INP, ".DSD needs size.\n");
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    print_error(ERROR_DIR, ".DSD expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_INP, ".DSD needs data.\n");
    return FAILED;
  }

  /*
  if (q == SUCCEEDED && (g_parsed_int < -2147483648 || g_parsed_int > 2147483647)) {
    print_error(ERROR_DIR, ".DSD expects 32-bit data, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }
  */

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "w%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "V%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    int latest_stack = g_latest_stack;
    
    for (q = 0; q < parsed_int; q++) {
      if (q == 0)
        fprintf(g_file_out_ptr, "U%d ", latest_stack);
      else {
        stack_create_stack_stack(latest_stack);
        fprintf(g_file_out_ptr, "U%d ", g_latest_stack);
      }
    }
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
    print_error(ERROR_DIR, ".NAME requires a string of 1 to 21 letters.\n");
    return FAILED;
  }

  /* no name has been defined so far */
  if (g_name_defined == 0) {
    int i;

    for (i = 0; g_tmp[i] != 0 && i < 21; i++)
      g_name[i] = g_tmp[i];

    if (i == 21 && g_tmp[i] != 0) {
      print_error(ERROR_DIR, ".NAME requires a string of 1 to 21 letters.\n");
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
      print_error(ERROR_DIR, ".NAME requires a string of 1 to 21 letters.\n");
      return FAILED;
    }
    if (i != 21 && (g_name[i] != 0 || g_tmp[i] != 0)) {
      print_error(ERROR_DIR, ".NAME was already defined.\n");
      return FAILED;
    }
  }

  return SUCCEEDED;
}

#endif


/* this is used for legacy .DSTRUCT syntax, and only for generating labels in the new
 * .DSTRUCT syntax. */
int parse_dstruct_entry(char *iname, struct structure *s, int *labels_only, int generate_labels) {

  char tmpname[MAX_NAME_LENGTH*2+10];
  struct structure_item *it;
  int f, o, c, g;

  /* read the data */
  it = s->items;
  while (it != NULL) {
    snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
    if (verify_name_length(tmpname) == FAILED)
      return FAILED;

    if (it->type != STRUCTURE_ITEM_TYPE_UNION) {
      /* add field label */
      char full_label[MAX_NAME_LENGTH + 1];

      if (generate_labels == YES)
        fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);
    
      if (get_full_label(tmpname, full_label) == FAILED)
        return FAILED;
      if (generate_labels == YES) {
        if (add_label_sizeof(full_label, it->size) == FAILED)
          return FAILED;
      }
    }

    if (it->type == STRUCTURE_ITEM_TYPE_UNION) {
      if (*labels_only == NO) {
        print_error(ERROR_DIR, ".DSTRUCT doesn't support structs with unions.\n");
        return FAILED;
      }
      else {
        struct structure *us;

        us = it->union_items;
        while (us != NULL) {
          if (us->name[0] != '\0') {
            /* check if the union is named */
            char full_label[MAX_NAME_LENGTH + 1];

            snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, us->name);
            if (verify_name_length(tmpname) == FAILED)
              return FAILED;

            if (generate_labels == YES)
              fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);

            if (get_full_label(tmpname, full_label) == FAILED)
              return FAILED;
            if (generate_labels == YES) {
              if (add_label_sizeof(full_label, us->size) == FAILED)
                return FAILED;
            }
          }
          else
            strcpy(tmpname, iname);

          parse_dstruct_entry(tmpname, us, labels_only, generate_labels);

          /* rewind */
          fprintf(g_file_out_ptr, "o%d 0 ", -us->size);
          us = us->next;
        }

        /* jump to union end */
        fprintf(g_file_out_ptr, "o%d 0 ", it->size);
      }
    }
    else if (it->type == STRUCTURE_ITEM_TYPE_INSTANCEOF) {
      /* handle .INSTANCEOF directive */
      /* update the naming prefix */
      snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
      if (verify_name_length(tmpname) == FAILED)
        return FAILED;

      if (it->num_instances == 1) {
        if (parse_dstruct_entry(tmpname, it->instance, labels_only, generate_labels) == FAILED)
          return FAILED;
      }
      else {
        int labels_only_tmp = YES;

        snprintf(tmpname, sizeof(tmpname), "%s.%s", iname, it->name);
      
        /* we have "struct.instance" and "struct.1.instance" referencing the same data. */
        parse_dstruct_entry(tmpname, it->instance, &labels_only_tmp, generate_labels);

        /* return to start of struct */
        fprintf(g_file_out_ptr, "o%d 0 ", -it->instance->size);

        for (g = 1; g <= it->num_instances; g++) {
          int size = it->instance->size;

          if (it->defined_size > 0)
            size = it->defined_size;
          
          snprintf(tmpname, sizeof(tmpname), "%s.%s.%d", iname, it->name, g);
          if (verify_name_length(tmpname) == FAILED)
            return FAILED;

          if (generate_labels == YES)
            fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, tmpname);

          if (generate_labels == YES) {
            if (add_label_sizeof(tmpname, size) == FAILED)
              return FAILED;
          }
          if (parse_dstruct_entry(tmpname, it->instance, labels_only, generate_labels) == FAILED)
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
        if (s_parse_dstruct_result == INPUT_NUMBER_STRING) {
          if (it->size < g_string_size) {
            print_error(ERROR_WRN, "String \"%s\" doesn't fit into the %d bytes of \"%s.%s\". Discarding the overflow.\n", g_label, it->size, s->name, it->name);
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
            if ((s_parse_dstruct_result == SUCCEEDED) && (g_parsed_int < -128 || g_parsed_int > 255)) {
              print_error(ERROR_DIR, "\"%s.%s\" expects 8-bit data, %d is out of range!\n", s->name, it->name, g_parsed_int);
              return FAILED;
            }

            if (s_parse_dstruct_result == SUCCEEDED)
              fprintf(g_file_out_ptr, "d%d ", g_parsed_int);
            else if (s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d Q%s ", g_active_file_info_last->line_current, g_label);
            else if (s_parse_dstruct_result == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "c%d ", g_latest_stack);

            o = 1;
          }
          else if (it->size == 2) {
            if (s_parse_dstruct_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
              print_error(ERROR_DIR, "\"%s.%s\" expects 16-bit data, %d is out of range!\n", s->name, it->name, g_parsed_int);
              return FAILED;
            }

            if (s_parse_dstruct_result == SUCCEEDED)
              fprintf(g_file_out_ptr, "y%d ", g_parsed_int);
            else if (s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d r%s ", g_active_file_info_last->line_current, g_label);
            else if (s_parse_dstruct_result == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "C%d ", g_latest_stack);

            o = 2;
          }
          else if (it->size == 3) {
            if (s_parse_dstruct_result == SUCCEEDED && (g_parsed_int < -8388608 || g_parsed_int > 16777215)) {
              print_error(ERROR_DIR, "\"%s.%s\" expects 24-bit data, %d is out of range!\n", s->name, it->name, g_parsed_int);
              return FAILED;
            }

            if (s_parse_dstruct_result == SUCCEEDED)
              fprintf(g_file_out_ptr, "z%d ", g_parsed_int);
            else if (s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d q%s ", g_active_file_info_last->line_current, g_label);
            else if (s_parse_dstruct_result == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "T%d ", g_latest_stack);

            o = 3;
          }
          else if (it->size == 4) {
            if (s_parse_dstruct_result == SUCCEEDED)
              fprintf(g_file_out_ptr, "u%d ", g_parsed_int);
            else if (s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL)
              fprintf(g_file_out_ptr, "k%d V%s ", g_active_file_info_last->line_current, g_label);
            else if (s_parse_dstruct_result == INPUT_NUMBER_STACK)
              fprintf(g_file_out_ptr, "U%d ", g_latest_stack);

            o = 4;
          }
          else {
            print_error(ERROR_DIR, "Internal error, unhandled it->size %d. Please submit a bug report!\n", it->size);
            return FAILED;
          }
        }
        
        /* fill the rest of the item with emptyfill */
        f = g_emptyfill;

        for (; o < it->size; o++)
          fprintf(g_file_out_ptr, "d%d ", f);
      }
      else { /* labels_only == YES */
        fprintf(g_file_out_ptr, "o%d 0 ", it->size);
      }
    }

    it = it->next;

    if (*labels_only == NO) {
      s_parse_dstruct_result = input_number();
      if (!(s_parse_dstruct_result == SUCCEEDED || s_parse_dstruct_result == INPUT_NUMBER_STRING || s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL || s_parse_dstruct_result == INPUT_NUMBER_STACK))
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
              int size = si->instance->size;

              if (si->defined_size > 0)
                size = si->defined_size;

              /* entire string matched? */
              if (strcmp(num_str, after_dot) == 0) {
                *field_offset = offset + (g-1) * size;
                *item_size = size;
                return SUCCEEDED;
              }
              /* only prefix matched */
              if (after_dot[strlen(num_str)] == '.' && find_struct_field(si->instance, after_dot + strlen(num_str) + 1, item_size, field_offset) == SUCCEEDED) {
                *field_offset += offset + (g-1) * size;
                return SUCCEEDED;
              }
            }
          }
        }
      }
      /* else keep looking */
    }

    if (si->type != STRUCTURE_ITEM_TYPE_DOTTED && si->type != STRUCTURE_ITEM_TYPE_DOTTED_INSTANCEOF)
      offset += si->size;

    si = si->next;
  }

  return FAILED;
}


static void _generate_dstruct_padding(struct structure *s, int supplied_size) {

  int size;

  size = s->size;
  if (s->defined_size > 0)
    size = s->defined_size;
  if (supplied_size > 0)
    size = supplied_size;

  if (size > s->size) {
    size = size - s->size;
    while (size > 0) {
      fprintf(g_file_out_ptr, "d%d ", g_emptyfill);
      size--;
    }
  }
}


int directive_dstruct(void) {

  int q, q2, supplied_size, labels_only, generate_labels = YES;
  char iname[MAX_NAME_LENGTH*2+5];
  struct structure *s;

  if (compare_next_token("INSTANCEOF") == SUCCEEDED) {
    /* nameless */
    skip_next_token();
    iname[0] = '\0';
  }
  else {
    /* get instance name */
    if (g_is_file_isolated_counter <= 0)
      g_force_ignore_namespace = YES;
    q = input_number();
    if (g_is_file_isolated_counter <= 0)
      g_force_ignore_namespace = NO;
    if (q == FAILED)
      return FAILED;
    if (q != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(ERROR_INP, ".DSTRUCT needs a name for the instance.\n");
      return FAILED;
    }
    strcpy(iname, g_label);

    if (compare_next_token("INSTANCEOF") == SUCCEEDED)
      skip_next_token();
  }

  /* get structure name */
  if (g_is_file_isolated_counter <= 0)
    g_force_ignore_namespace = YES;
  q = input_number();
  if (g_is_file_isolated_counter <= 0)
    g_force_ignore_namespace = NO;
  if (q == FAILED)
    return FAILED;
  if (q != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_INP, ".DSTRUCT needs a structure name.\n");
    return FAILED;
  }

  /* find the structure */
  s = _get_structure(g_label);

  if (s == NULL) {
    print_error(ERROR_DIR, "Reference to an unidentified structure \"%s\".\n", g_label);
    return FAILED;
  }

  /* DEBUG
     {
     struct structure_item *sS = s->items;
        
     print_text(NO, "STRUCT \"%s\" size %d\n", s->name, s->size);
     while (sS != NULL) {
     print_text(NO, "ITEM \"%s\" size %d\n", sS->name, sS->size);
     sS = sS->next;
     }
     }
  */

  supplied_size = s->defined_size;

  while (1) {
    if (compare_next_token("SIZE") == SUCCEEDED) {
      /* we have fixed size for this instance */
      skip_next_token();
    
      q = input_number();
      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_INP, "SIZE needs an immediate value.\n");
        return FAILED;
      }

      if (g_parsed_int < 1) {
        print_error(ERROR_DIR, "SIZE must be > 0.\n");
        return FAILED;
      }
      if (g_parsed_int < s->size) {
        print_error(ERROR_DIR, ".STRUCT \"%s\"'s size is %d, but SIZE is %d which is smaller -> please increase SIZE!\n", s->name, s->size, g_parsed_int);
        return FAILED;
      }
    
      supplied_size = g_parsed_int;
    }
    else if (compare_next_token("NOLABELS") == SUCCEEDED) {
      /* no labels for this instance */
      skip_next_token();

      generate_labels = NO;
    }
    else
      break;
  }

  if (iname[0] != '\0') {
    char full_label[MAX_NAME_LENGTH + 1];
    int size = s->size;

    if (generate_labels == YES)
      fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, iname);

    if (get_full_label(iname, full_label) == FAILED)
      return FAILED;

    if (supplied_size > 0)
      size = supplied_size;
    
    if (add_label_sizeof(full_label, size) == FAILED)
      return FAILED;
    if (supplied_size > 0) {
      q = supplied_size - s->size;
      if (_add_paddingof_definition(full_label, q) == FAILED)
        return FAILED;
    }
  }
  
  if (compare_next_token("VALUES") == SUCCEEDED) {
    /* new syntax */

    int field_offset;
    char field_name[MAX_NAME_LENGTH + 1];
    int item_size;

    if (s_dstruct_status == ON) {
      print_error(ERROR_DIR, "You can't have nested .DSTRUCT's.\n");
      return FAILED;
    }

    s_dstruct_status = ON;

    skip_next_token();

    fprintf(g_file_out_ptr, "e%d -1 ", s->size); /* mark start address of dstruct */

    q = get_next_token();

    while (q == SUCCEEDED) {
      if ((q2 = parse_if_directive()) != -1)
        return q2;
      if (strcaselesscmp(g_tmp, ".ENDST") == 0)
        break;
      else {
        if (g_tmp[strlen(g_tmp)-1] == ':')
          g_tmp[strlen(g_tmp)-1] = '\0';
        strcpy(field_name, g_tmp);

        if (find_struct_field(s, field_name, &item_size, &field_offset) == FAILED) {
          print_error(ERROR_DIR, ".DSTRUCT: Couldn't find field \"%s\" in structure \"%s\".\n", field_name, s->name);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "k%d ", g_active_file_info_last->line_current);
        fprintf(g_file_out_ptr, "e%d %d ", field_offset, item_size);

        do {
          if ((q = get_next_token()) == FAILED) {
            print_error(ERROR_ERR, "Error parsing .DSTRUCT.\n");
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
      print_error(ERROR_ERR, "Error parsing .DSTRUCT.\n");
      return FAILED;
    }

    /* back to start of struct */
    fprintf(g_file_out_ptr, "e%d -3 ", 0);

    /* now generate labels */
    if (iname[0] != '\0') {
      if (generate_labels == YES) {
        labels_only = YES;
        if (parse_dstruct_entry(iname, s, &labels_only, YES) == FAILED)
          return FAILED;
      }
    }

    /* mark end of .DSTRUCT */
    fprintf(g_file_out_ptr, "e%d -2 ", s->size);

    /* generate padding */
    _generate_dstruct_padding(s, supplied_size);

    s_dstruct_status = OFF;

    return SUCCEEDED;
  }
  else if (compare_next_token("DATA") == SUCCEEDED)
    skip_next_token();

  /* legacy syntax */

  s_parse_dstruct_result = input_number();

  if (s_parse_dstruct_result == INPUT_NUMBER_ADDRESS_LABEL) {
    if (g_label[strlen(g_label)-1] == ':') {
      print_error(ERROR_ERR, "Label in a wrong place?\n");
      return FAILED;
    }
  }
  
  labels_only = NO;
  if (parse_dstruct_entry(iname, s, &labels_only, generate_labels) == FAILED)
    return FAILED;
  
  /* generate padding */
  _generate_dstruct_padding(s, supplied_size);
  
  if (s_parse_dstruct_result == INPUT_NUMBER_EOL)
    next_line();
  else {
    print_error(ERROR_DIR, "Too much data for structure \"%s\".\n", s->name);
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
    print_error(ERROR_INP, ".%s needs size.\n", bak);
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    print_error(ERROR_DIR, ".%s expects a 16-bit positive integer as size, %d is out of range!\n", bak, g_parsed_int);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_INP, ".%s needs data.\n", bak);
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
    print_error(ERROR_DIR, ".%s expects 8-bit data, %d is out of range!\n", bak, g_parsed_int);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "x%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "R%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    int latest_stack = g_latest_stack;
    
    for (q = 0; q < parsed_int; q++) {
      if (q == 0)
        fprintf(g_file_out_ptr, "c%d ", latest_stack);
      else {
        stack_create_stack_stack(latest_stack);
        fprintf(g_file_out_ptr, "c%d ", g_latest_stack);
      }
    }
  }

  return SUCCEEDED;
}


int directive_dsw(void) {

  int q, parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(ERROR_INP, ".DSW needs size.\n");
    return FAILED;
  }

  if (g_parsed_int < 1 || g_parsed_int > 65535) {
    print_error(ERROR_DIR, ".DSW expects a 16-bit positive integer as size, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }

  parsed_int = g_parsed_int;

  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_INP, ".DSW needs data.\n");
    return FAILED;
  }

  if (q == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
    print_error(ERROR_DIR, ".DSW expects 16-bit data, %d is out of range!\n", g_parsed_int);
    return FAILED;
  }

  if (q == SUCCEEDED)
    fprintf(g_file_out_ptr, "X%d %d ", parsed_int, g_parsed_int);
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    for (q = 0; q < parsed_int; q++)
      fprintf(g_file_out_ptr, "r%s ", g_label);
  }
  else if (q == INPUT_NUMBER_STACK) {
    int latest_stack = g_latest_stack;
    
    for (q = 0; q < parsed_int; q++) {
      if (q == 0)
        fprintf(g_file_out_ptr, "C%d ", latest_stack);
      else {
        stack_create_stack_stack(latest_stack);
        fprintf(g_file_out_ptr, "C%d ", g_latest_stack);
      }
    }
  }

  return SUCCEEDED;
}


int directive_incdir(void) {
  
  int q, o;
  char *c;

  g_expect_calculations = NO;
  g_input_parse_special_chars = NO;
  o = input_number();
  g_input_parse_special_chars = YES;
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".INCDIR needs a directory string.\n");
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
      print_error(ERROR_DIR, "Out of memory error.\n");
      return FAILED;
    }
    g_include_dir = c;
    g_include_dir_size = o;
  }

  /* convert the path string to local enviroment */
  strcpy(g_include_dir, g_label);
  localize_path(g_include_dir);

  /* terminate the string with '/' */
#if defined(MSDOS)
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


static int _remember_namespace(char *name) {

  struct namespace *nspace;
  
  nspace = calloc(sizeof(struct namespace), 1);
  if (nspace == NULL) {
    print_error(ERROR_DIR, "Out of memory while remembering namespace \"%s\".\n", name);
    return FAILED;
  }

  strcpy(nspace->name, name);
      
  nspace->next = g_namespaces_first;
  g_namespaces_first = nspace;

  return SUCCEEDED;
}


static int _is_namespace_valid(char *name) {

  int i = 0;

  while (1) {
    if (name[i] == 0)
      return YES;
    else if (name[i] == '.') {
      print_error(ERROR_DIR, "Namespace (here \"%s\") cannot contain a dot.\n", name);
      return NO;
    }
    i++;
  }
}


int directive_include(int is_real) {

  int o, include_size = 0, accumulated_name_length = 0, character_c_position = 0, got_once = NO;
  char namespace[MAX_NAME_LENGTH + 1], path[MAX_NAME_LENGTH + 1], accumulated_name[MAX_NAME_LENGTH + 1];

  if (is_real == YES) {
    /* turn the .INCLUDE/.INC into .INDLUDE/.IND to mark it as used, if ONCE is used,
       for repetitive macro calls that contain .INCLUDE/.INC... */
    o = g_source_index;
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
    if (compare_next_token("NAMESPACE") == SUCCEEDED || compare_next_token("ONCE") == SUCCEEDED ||
        compare_next_token("ISOLATED") == SUCCEEDED)
      break;

    g_expect_calculations = NO;
    g_input_parse_special_chars = NO;
    o = input_number();
    g_input_parse_special_chars = YES;
    g_expect_calculations = YES;
    
    if (o == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(ERROR_DIR, ".INCLUDE needs a file name string.\n");
      return FAILED;
    }

    if (accumulated_name_length + strlen(g_label) >= sizeof(accumulated_name)) {
      print_error(ERROR_DIR, "The accumulated file name length >= MAX_NAME_LENGTH. Increase its size in shared.h and recompile WLA.\n");
      return FAILED;
    }

    strcpy(&accumulated_name[accumulated_name_length], g_label);
    accumulated_name_length = (int)strlen(accumulated_name);
  }

  strcpy(path, accumulated_name);

  /* convert the path to local enviroment */
  localize_path(path);

  namespace[0] = 0;
  
  while (1) {
    if (compare_next_token("NAMESPACE") == SUCCEEDED) {
      skip_next_token();

      g_expect_calculations = NO;
      o = input_number();
      g_expect_calculations = YES;
    
      if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
        print_error(ERROR_DIR, "Namespace string is missing.\n");
        return FAILED;
      }

      if (_is_namespace_valid(g_label) == NO)
        return FAILED;
      
      strcpy(namespace, g_label);

      if (_remember_namespace(namespace) == FAILED)
        return FAILED;
    }
    else if (compare_next_token("ONCE") == SUCCEEDED) {
      skip_next_token();

      got_once = YES;
    }
    else if (compare_next_token("ISOLATED") == SUCCEEDED) {
      skip_next_token();

      if (g_is_file_isolated_counter == 0)
        g_is_file_isolated_counter++;
    }
    else
      break;
  }

  /* propagate the current file's namespace */
  if (namespace[0] == 0 && g_is_file_isolated_counter > 0)
    strcpy(namespace, g_active_file_info_last->namespace);
  
  if (is_real == YES) {
    if (include_file(path, &include_size, namespace) == FAILED)
      return FAILED;
  
    /* WARNING: this is tricky: did we just include a file inside a macro? */
    if (g_macro_active != 0) {
      /* yes. note that the now we added new bytes after g_source_index, so if a macro_return_i is
         bigger than g_source_index, we'll need to add the bytes to it */
      struct macro_static *ms;
      int q, w;

      for (q = 0; q < g_macro_active; q++) {
        if (g_macro_stack[q].macro_return_i > g_source_index)
          g_macro_stack[q].macro_return_i += include_size;
        for (w = 0; w < g_macro_stack[q].supplied_arguments; w++) {
          if (g_macro_stack[q].argument_data[w]->start > g_source_index)
            g_macro_stack[q].argument_data[w]->start += include_size;
        }
      }

      /* also macro starting points that are after this position need to move forward
         in memory... */
      ms = g_macros_first;
      while (ms != NULL) {
        if (ms->start > g_source_index)
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

  struct macro_static *macro;
  int skip, read, j, o, id, swap, filter_size;

  if (g_org_defined == 0 && g_output_format != OUTPUT_LIBRARY) {
    print_error(ERROR_LOG, "Before you can .INCBIN data you'll need to use ORG.\n");
    return FAILED;
  }
  
  g_expect_calculations = NO;
  g_input_parse_special_chars = NO;
  o = input_number();
  g_input_parse_special_chars = YES;
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".INCBIN needs a file name string.\n");
    return FAILED;
  }

  /* convert the path string to local enviroment */
  localize_path(g_label);

  if (incbin_file(g_label, &id, &swap, &skip, &read, &macro, &filter_size) == FAILED)
    return FAILED;
  
  if (macro == NULL) {
    /* D [id] [swap] [skip] [size] */
    fprintf(g_file_out_ptr, "D%d %d %d %d ", id, swap, skip, read);
  }
  else {
    /* we want to filter the data */
    struct incbin_file_data *ifd;
    struct macro_incbin *min;

    min = calloc(sizeof(struct macro_incbin), 1);
    if (min == NULL) {
      print_error(ERROR_NONE, "Out of memory error while starting to filter the .INCBINed data.\n");
      return FAILED;
    }

    ifd = g_incbin_file_data_first;
    for (j = 0; j != id; j++)
      ifd = ifd->next;

    min->data = (unsigned char *)ifd->data;
    min->swap = swap;
    min->position = skip;
    min->left = read;
    min->filter_size = filter_size;

    if (_macro_start_incbin(macro, min, YES) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int directive_struct(void) {

  int q;
  
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't use .STRUCT inside .DSTRUCT.\n");
    return FAILED;
  }

  s_active_struct = calloc(sizeof(struct structure), 1);
  if (s_active_struct == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new .STRUCT.\n");
    return FAILED;
  }
  s_active_struct->items = NULL;
  s_active_struct->last_item = NULL;
  s_active_struct->alive = YES;
  s_active_struct->defined_size = -1;

  if (_remember_new_structure(s_active_struct) == FAILED)
    return FAILED;

  if (g_is_file_isolated_counter <= 0)
    g_force_ignore_namespace = YES;
  q = get_next_plain_string();
  if (g_is_file_isolated_counter <= 0)
    g_force_ignore_namespace = NO;
  if (q == FAILED)
    return FAILED;
  
  strcpy(s_active_struct->name, g_label);

  /* SIZE defined? */
  if (compare_next_token("SIZE") == SUCCEEDED) {
    /* we have fixed size for this .STRUCT ? */
    int q;

    remember_current_source_file_position();
    
    skip_next_token();
    
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      /* SIZE was actually a field in the .STRUCT? roll back */
      roll_back_to_remembered_source_file_position();
    }
    else {
      if (g_parsed_int < 1) {
        print_error(ERROR_DIR, "SIZE must be > 0.\n");
        return FAILED;
      }

      s_active_struct->defined_size = g_parsed_int;
    }
  }
  
  s_union_stack = NULL;

  s_enum_offset = 0;
  s_last_enum_offset = 0;
  s_max_enum_offset = 0;
  s_base_enum_offset = 0;
  s_enum_ord = 1;
  s_enum_export = NO;
  g_in_struct = YES;

  return SUCCEEDED;
}


int directive_ramsection(void) {

  int q, orga_given = -1;
  char c1;

  if (g_section_status == ON) {
    print_error(ERROR_DIR, "There is already an open section called \"%s\".\n", g_sections_last->name);
    return FAILED;
  }
  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't use .RAMSECTION inside .DSTRUCT.\n");
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  g_sec_tmp = calloc(sizeof(struct section_def), 1);
  if (g_sec_tmp == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating room for a new RAMSECTION \"%s\".\n", g_tmp);
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
  g_sec_tmp->base = -1;
  g_sec_tmp->bank = -1;
  g_sec_tmp->slot = -1;
  g_sec_tmp->address = -1;
  g_sec_tmp->address_from_dsp = -1;
  g_sec_tmp->bitwindow = 0;
  g_sec_tmp->window_start = -1;
  g_sec_tmp->window_end = -1;
  g_sec_tmp->banks[0] = 0;
  
  /* add the namespace to the ramsection's name? */
  if (g_active_file_info_last->namespace[0] != 0) {
    if (add_namespace_to_string(g_tmp, g_sizeof_g_tmp, "RAMSECTION") == FAILED) {
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
  c1 = g_tmp[0];
  while (g_sec_next != NULL) {
    if (c1 == g_sec_next->name[0]) {
      if (strcmp(g_sec_next->name, g_tmp) == 0) {
        print_error(ERROR_DIR, "RAMSECTION \"%s\" was defined for the second time.\n", g_tmp);
        free(g_sec_tmp);
        return FAILED;
      }
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
        print_error(ERROR_DIR, ".RAMSECTION cannot take BANK when inside a library.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error(ERROR_DIR, "BANK number must be zero or positive.\n");
        return FAILED;
      }

      if (g_parsed_int > 255 && g_output_format != OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "We can have 256 RAM banks (0-255) per slot, selected bank %d.\n", g_parsed_int);
        return FAILED;
      }

      g_sec_tmp->bank = g_parsed_int;
    }
    else if (compare_next_token("BASE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take BASE when inside a library.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error(ERROR_DIR, "BASE must be zero or positive.\n");
        return FAILED;
      }

      g_sec_tmp->base = g_parsed_int;
    }
    else if (compare_next_token("SLOT") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take SLOT when inside a library.\n");
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
        print_error(ERROR_DIR, "Cannot find the SLOT.\n");
        return FAILED;
      }

      if (g_slots[g_parsed_int].size == 0) {
        print_error(ERROR_DIR, "There is no SLOT number %d.\n", g_parsed_int);
        return FAILED;
      }

      g_sec_tmp->slot = g_parsed_int;
    }
    /* the size of the section? */
    else if (compare_next_token("SIZE") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the SIZE.\n");
        return FAILED;
      }

      if (g_sec_tmp->maxsize_status == ON && g_sec_tmp->maxsize != g_parsed_int) {
        print_error(ERROR_DIR, "The SIZE of the section has already been defined.\n");
        return FAILED;
      }

      g_sec_tmp->maxsize_status = ON;
      g_sec_tmp->maxsize = g_parsed_int;
    }
    else if (compare_next_token("ORGA") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take ORGA when inside a library.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();
      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "Cannot get the ORGA.\n");
        return FAILED;
      }

      orga_given = g_parsed_int;
    }
    else if (compare_next_token("ORG") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take ORG when inside a library.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();
      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "Cannot get the ORG.\n");
        return FAILED;
      }

      g_sec_tmp->address = g_parsed_int;
    }
    else if (compare_next_token("WINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the WINDOW starting address.\n");
        return FAILED;
      }

      g_sec_tmp->window_start = g_parsed_int;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the WINDOW ending address.\n");
        return FAILED;
      }

      g_sec_tmp->window_end = g_parsed_int;

      if (g_sec_tmp->window_start > g_sec_tmp->window_end) {
        print_error(ERROR_DIR, "The start ($%.4x) of the WINDOW is bigger than the end ($%.4x).\n", g_sec_tmp->window_start, g_sec_tmp->window_end);
        return FAILED;
      }
    }
    else if (compare_next_token("BITWINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the BITWINDOW mask.\n");
        return FAILED;
      }

      if (g_parsed_int == 0) {
        print_error(ERROR_DIR, "BITWINDOW 0 means that the section cannot be placed anywhere...\n");
        return FAILED;
      }
      
      g_sec_tmp->bitwindow = g_parsed_int;
    }
    /* align the ramsection? */
    else if (compare_next_token("ALIGN") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take ALIGN when inside a library.\n");
        return FAILED;
      }

      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the .RAMSECTION alignment.\n");
        return FAILED;
      }

      g_sec_tmp->alignment = g_parsed_int;
    }
    /* offset the ramsection? */
    else if (compare_next_token("OFFSET") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".RAMSECTION cannot take OFFSET when inside a library.\n");
        return FAILED;
      }

      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the .RAMSECTION offset.\n");
        return FAILED;
      }

      g_sec_tmp->offset = g_parsed_int;
    }  
    /* the type of the section */
    else if (compare_next_token("FORCE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "Libraries don't take FORCE sections.\n");
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
        print_error(ERROR_DIR, "Out of memory while allocating room for a new APPENDTO \"%s\".\n", g_tmp);
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
          print_error(ERROR_DIR, "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, g_sizeof_g_tmp, "APPENDTO") == FAILED)
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
        print_error(ERROR_DIR, "Out of memory while allocating room for a new AFTER \"%s\".\n", g_tmp);
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
          print_error(ERROR_DIR, "The AFTER string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, g_sizeof_g_tmp, "AFTER") == FAILED)
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
        print_error(ERROR_DIR, "Could not parse the .RAMSECTION priority.\n");
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

  s_enum_offset = 0;
  s_last_enum_offset = 0;
  s_max_enum_offset = 0;
  s_base_enum_offset = 0;
  s_enum_ord = 1;

  /* setup s_active_struct (ramsection vars stored here temporarily) */
  s_active_struct = calloc(sizeof(struct structure), 1);
  if (s_active_struct == NULL) {
    print_error(ERROR_DIR, "Out of memory while parsing .RAMSECTION.\n");
    return FAILED;
  }
  s_active_struct->alive = YES;

  if (_remember_new_structure(s_active_struct) == FAILED)
    return FAILED;
  
  s_active_struct->name[0] = '\0';
  s_active_struct->items = NULL;
  s_active_struct->last_item = NULL;
  s_union_stack = NULL;
  
  g_in_ramsection = YES;

  s_active_ramsection = g_sec_tmp;

  if (orga_given >= 0) {
    int current_slot_address = g_slots[g_sec_tmp->slot].address;

    if (orga_given < current_slot_address || orga_given >= (current_slot_address + g_slots[g_sec_tmp->slot].size)) {
      print_error(ERROR_DIR, "ORGA is outside the current SLOT.\n");
      return FAILED;
    }

    g_sec_tmp->address = orga_given - current_slot_address;
  }
  
  return SUCCEEDED;
}


int directive_section(void) {

  int l, q, org_given = -1, orga_given = -1;
  char c1;

  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't set the section inside .DSTRUCT.\n");
    return FAILED;
  }
  else if (g_section_status == ON) {
    print_error(ERROR_DIR, "There is already an open section called \"%s\".\n", g_sections_last->name);
    return FAILED;
  }
  else if (g_output_format != OUTPUT_LIBRARY && s_bank_defined == 0) {
    print_error(ERROR_DIR, ".SECTION requires a predefined bank.\n");
    return FAILED;
  }
  else if (g_output_format != OUTPUT_LIBRARY && g_org_defined == 0) {
    print_error(ERROR_DIR, ".SECTION requires a starting address for positioning.\n");
    return FAILED;
  }

  if (get_next_token() == FAILED)
    return FAILED;

  /* every library section starts @ the beginning of the bank */
  if (g_output_format == OUTPUT_LIBRARY)
    g_org_defined = 1;

  g_sec_tmp = calloc(sizeof(struct section_def), 1);
  if (g_sec_tmp == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating room for a new SECTION \"%s\".\n", g_tmp);
    return FAILED;
  }
  
  g_sec_tmp->priority = 0;
  g_sec_tmp->address = -1;
  g_sec_tmp->address_from_dsp = -1;
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
  g_sec_tmp->base = -1;
  g_sec_tmp->bank = -1;
  g_sec_tmp->slot = -1;
  g_sec_tmp->banks[0] = 0;
  
  c1 = g_tmp[0];
  
  if (c1 == 'B' && strcmp(g_tmp, "BANKHEADER") == 0) {
    no_library_files("bank header sections");
      
    g_sec_next = g_sections_first;
    while (g_sec_next != NULL) {
      if (g_sec_next->name[0] == 'B' && strcmp(g_sec_next->name, g_tmp) == 0 && g_sec_next->bank == s_bank) {
        print_error(ERROR_DIR, "BANKHEADER section was defined for the second time for bank %d.\n", s_bank);
        free(g_sec_tmp);
        return FAILED;
      }
      g_sec_next = g_sec_next->next;
    }
  }
  else {
    g_sec_next = g_sections_first;
    while (g_sec_next != NULL) {
      if (c1 == g_sec_next->name[0] && strcmp(g_sec_next->name, g_tmp) == 0) {
        print_error(ERROR_DIR, "SECTION \"%s\" was defined for the second time.\n", g_tmp);
        free(g_sec_tmp);
        return FAILED;
      }
      g_sec_next = g_sec_next->next;
    }
  }

  strcpy(g_sec_tmp->name, g_tmp);
  g_sec_tmp->next = NULL;
  g_sec_tmp->status = SECTION_STATUS_FREE;

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
      if (g_label[0] == '\"' && g_label[strlen(g_label)-1] == '\"') {
        l = 0;
        while (g_label[l+1] != '\"') {
          g_label[l] = g_label[l+1];
          l++;
        }
        g_label[l] = 0;
      }

      if (_is_namespace_valid(g_label) == NO)
        return FAILED;
              
      hashmap_get(g_namespace_map, g_label, (void*)&nspace);
      if (nspace == NULL) {
        nspace = calloc(1, sizeof(struct namespace_def));
        if (nspace == NULL) {
          print_error(ERROR_DIR, "Out of memory error.\n");
          return FAILED;
        }
        nspace->label_map = NULL;
        strcpy(nspace->name, g_label);
        if (hashmap_put(g_namespace_map, nspace->name, nspace) != MAP_OK) {
          print_error(ERROR_DIR, "Namespace hashmap error.\n");
          return FAILED;
        }
      }

      if (nspace->label_map == NULL)
        nspace->label_map = hashmap_new();

      g_sec_tmp->nspace = nspace;
    }
    /* the size of the section? */
    else if (compare_next_token("SIZE") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the SIZE.\n");
        return FAILED;
      }

      if (g_sec_tmp->maxsize_status == ON && g_sec_tmp->maxsize != g_parsed_int) {
        print_error(ERROR_DIR, "The SIZE of the section has already been defined.\n");
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
        print_error(ERROR_DIR, "Could not parse the .SECTION alignment.\n");
        return FAILED;
      }

      g_sec_tmp->alignment = g_parsed_int;
    }
    /* org? */
    else if (compare_next_token("ORG") == SUCCEEDED) {
      no_library_files(".ORG definitions");

      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the ORG.\n");
        return FAILED;
      }

      org_given = g_parsed_int;
    }
    /* orga? */
    else if (compare_next_token("ORGA") == SUCCEEDED) {
      no_library_files(".ORGA definitions");
  
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the ORGA.\n");
        return FAILED;
      }

      orga_given = g_parsed_int;
    }
    /* base? */
    else if (compare_next_token("BASE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".SECTION cannot take BASE when inside a library.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error(ERROR_DIR, "BASE must be zero or positive.\n");
        return FAILED;
      }

      g_sec_tmp->base = g_parsed_int;
    }
    /* bank? */
    else if (compare_next_token("BANK") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".SECTION cannot take BANK when inside a library.\n");
        return FAILED;
      }
      if (g_rombanks_defined == 0) {
        print_error(ERROR_DIR, "No .ROMBANKS defined.\n");
        return FAILED;
      }

      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0 || g_parsed_int >= g_rombanks) {
        print_error(ERROR_DIR, "BANK number must be in [0, %d].\n", g_rombanks - 1);
        return FAILED;
      }

      g_sec_tmp->bank = g_parsed_int;
    }
    /* slot? */
    else if (compare_next_token("SLOT") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, ".SECTION cannot take SLOT when inside a library.\n");
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
        print_error(ERROR_DIR, "Cannot find the SLOT.\n");
        return FAILED;
      }

      if (g_slots[g_parsed_int].size == 0) {
        print_error(ERROR_DIR, "There is no SLOT number %d.\n", g_parsed_int);
        return FAILED;
      }

      g_sec_tmp->slot = g_parsed_int;
    }
    /* offset the section? */
    else if (compare_next_token("OFFSET") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the .SECTION offset.\n");
        return FAILED;
      }

      g_sec_tmp->offset = g_parsed_int;
    }
    else if (compare_next_token("WINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the WINDOW starting address.\n");
        return FAILED;
      }

      g_sec_tmp->window_start = g_parsed_int;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the WINDOW ending address.\n");
        return FAILED;
      }

      g_sec_tmp->window_end = g_parsed_int;

      if (g_sec_tmp->window_start > g_sec_tmp->window_end) {
        print_error(ERROR_DIR, "The start ($%.4x) of the WINDOW is bigger than the end ($%.4x).\n", g_sec_tmp->window_start, g_sec_tmp->window_end);
        return FAILED;
      }
    }
    else if (compare_next_token("BITWINDOW") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, "Could not parse the BITWINDOW mask.\n");
        return FAILED;
      }

      if (g_parsed_int == 0) {
        print_error(ERROR_DIR, "BITWINDOW 0 means that the section cannot be placed anywhere...\n");
        return FAILED;
      }
      
      g_sec_tmp->bitwindow = g_parsed_int;
    }
    /* the type of the section */
    else if (compare_next_token("FORCE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "Libraries don't take FORCE sections.\n");
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_FORCE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("BANKS") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      /* collect the banks list string */
      if (get_next_token() == FAILED)
        return FAILED;

      strcpy(g_sec_tmp->banks, g_tmp);
    }
    else if (compare_next_token("SEMISUPERFREE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "Libraries don't take SEMISUPERFREE sections.\n");
        return FAILED;
      }

      g_sec_tmp->status = SECTION_STATUS_SEMISUPERFREE;
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
        print_error(ERROR_DIR, "Libraries don't take SEMIFREE sections.\n");
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_SEMIFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMISUBFREE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "Libraries don't take SEMISUBFREE sections.\n");
        return FAILED;
      }
      g_sec_tmp->status = SECTION_STATUS_SEMISUBFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("OVERWRITE") == SUCCEEDED) {
      if (g_output_format == OUTPUT_LIBRARY) {
        print_error(ERROR_DIR, "Libraries don't take OVERWRITE sections.\n");
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
        print_error(ERROR_DIR, "Out of memory while allocating room for a new APPENDTO \"%s\".\n", g_tmp);
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
          print_error(ERROR_DIR, "The APPENDTO string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          free(after_tmp);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, g_sizeof_g_tmp, "APPENDTO") == FAILED) {
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
        print_error(ERROR_DIR, "Out of memory while allocating room for a new AFTER \"%s\".\n", g_tmp);
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
          print_error(ERROR_DIR, "The AFTER string \"%s\" is too long. Increase MAX_NAME_LENGTH in shared.h and recompile WLA.\n", g_tmp);
          return FAILED;
        }

        strcpy(buf, &g_tmp[2]);
        strcpy(g_tmp, buf);
      }
      else if (g_active_file_info_last->namespace[0] != 0) {
        if (add_namespace_to_string(g_tmp, g_sizeof_g_tmp, "AFTER") == FAILED)
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
        print_error(ERROR_DIR, "Could not parse the .SECTION priority.\n");
        return FAILED;
      }

      g_sec_tmp->priority = g_parsed_int;
    }
    else if (compare_next_token("AUTOPRIORITY") == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      g_sec_tmp->priority = s_autopriority--;
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
  if (g_sec_tmp->name[0] == 'B' && strcmp(g_sec_tmp->name, "BANKHEADER") == 0) {
    g_sec_tmp->status = SECTION_STATUS_HEADER;
    g_bankheader_status = ON;
  }

  g_sec_tmp->alive = YES;
  g_sec_tmp->filename_id = g_active_file_info_last->filename_id;
  if (g_sec_tmp->bank < 0)
    g_sec_tmp->bank = s_bank;
  if (g_sec_tmp->slot < 0)
    g_sec_tmp->slot = s_current_slot;
  g_section_status = ON;

  fprintf(g_file_out_ptr, "S%d ", g_sec_tmp->id);

  /* generate a section start label? */
  if (g_extra_definitions == ON)
    generate_label("SECTIONSTART_", g_sec_tmp->name);

  if (org_given >= 0)
    g_sec_tmp->address = org_given;
  else if (orga_given >= 0) {
    int current_slot_address = g_slots[g_sec_tmp->slot].address;
    
    if (orga_given < current_slot_address || orga_given > (current_slot_address + g_slots[g_sec_tmp->slot].size)) {
      print_error(ERROR_DIR, "ORGA is outside the section's SLOT.\n");
      return FAILED;
    }

    g_sec_tmp->address = orga_given - current_slot_address;
  }

  /* sanity check */
  if (g_sec_tmp->status == SECTION_STATUS_SEMISUPERFREE && g_sec_tmp->banks[0] == 0) {
    print_error(ERROR_DIR, "SEMISUPERFREE section needs BANKS list.\n");
    return FAILED;
  }
  if (g_sec_tmp->status != SECTION_STATUS_SEMISUPERFREE && g_sec_tmp->banks[0] != 0) {
    print_error(ERROR_DIR, "Only SEMISUPERFREE sections take BANKS.\n");
    return FAILED;
  }  
  
  return SUCCEEDED;
}


int directive_fopen(void) {
  
  struct filepointer *f;
  struct string *string;
  char *c;
  int o;

  g_expect_calculations = NO;
  g_input_parse_special_chars = NO;
  o = input_number();
  g_input_parse_special_chars = YES;
  g_expect_calculations = YES;

  if (o != INPUT_NUMBER_STRING && o != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".FOPEN needs a file name string.\n");
    return FAILED;
  }

  /* convert the path to local enviroment */
  localize_path(g_label);

  c = calloc(strlen(g_label) + 1, 1);
  if (c == NULL) {
    print_error(ERROR_DIR, "Out of memory error.\n");
    return FAILED;
  }
  strcpy(c, g_label);

  string = g_fopen_filenames_first;
  while (string != NULL) {
    if (strcmp(c, string->string) == 0)
      break;
    string = string->next;
  }

  if (string == NULL) {
    /* remember the file name for makefile generation */
    string = calloc(sizeof(struct string), 1);
    if (string == NULL) {
      print_error(ERROR_DIR, "Out of memory error.\n");
      free(c);
      return FAILED;
    }
    string->string = calloc(strlen(g_label) + 1, 1);
    if (string->string == NULL) {
      print_error(ERROR_DIR, "Out of memory error.\n");
      free(c);
      free(string);
      return FAILED;
    }
    strcpy(string->string, g_label);
    string->next = NULL;

    if (g_fopen_filenames_first == NULL) {
      g_fopen_filenames_first = string;
      g_fopen_filenames_last = string;
    }
    else {
      g_fopen_filenames_last->next = string;
      g_fopen_filenames_last = string;
    }
  }
  
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
      print_error(ERROR_DIR, "Out of memory error.\n");
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
    if (g_makefile_rules == YES) {
      /* lets just use a tmp file for file operations */
      create_tmp_file(&f->f);
      if (f->f == NULL) {
        print_error(ERROR_DIR, "Error creating a tmp file for \"%s\"!\n", f->filename);
        return FAILED;
      }
    }
    if (f->f == NULL) {
      print_error(ERROR_DIR, "Error opening file \"%s\" for reading.\n", f->filename);
      return FAILED;
    }
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
    print_error(ERROR_DIR, "Couldn't find filepointer \"%s\".\n", g_tmp);
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
    print_error(ERROR_DIR, "Couldn't find filepointer \"%s\".\n", g_tmp);
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
    print_error(ERROR_DIR, "Couldn't find filepointer \"%s\".\n", g_tmp);
    return FAILED;
  }

  b = ftell(f->f);

  if (b < 0) {
    print_error(ERROR_DIR, "Error getting ftell(%s).\n", g_tmp);
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
    print_error(ERROR_DIR, ".FSEEK needs file pointer name.\n");
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
    print_error(ERROR_DIR, "Couldn't find filepointer \"%s\".\n", g_label);
    return FAILED;
  }

  /* get the seek size */
  if (input_number() != SUCCEEDED) {
    print_error(ERROR_DIR, ".FSEEK needs immediate value for offset.\n");
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
    print_error(ERROR_DIR, ".FSEEK needs mode (START/CURRENT/END).\n");
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
    print_error(ERROR_DIR, "Couldn't find filepointer \"%s\".\n", g_tmp);
    return FAILED;
  }

  if (fscanf(f->f, "%c", &c) <= 0) {
    print_error(ERROR_DIR, ".FREAD couldn't read a byte from file \"%s\" (%s).\n", f->filename, g_tmp);
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
    print_error(ERROR_DIR, ".BLOCK requires a name string.\n");
    return FAILED;
  }

  b = calloc(sizeof(struct block_name), 1);
  if (b == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating block name.\n");
    return FAILED;
  }

  strcpy(b->name, g_tmp);
  b->id = s_block_name_id++;
  b->next = g_block_names;
  g_block_names = b;
  
  s_block_status++;

  fprintf(g_file_out_ptr, "g%d ", b->id);

  return SUCCEEDED;
}


int directive_shift(void) {

  struct macro_argument *ma;
  struct macro_runtime *rt;
  struct macro_static *st;
  int q, o;

  if (g_macro_active == 0) {
    print_error(ERROR_DIR, ".SHIFT can only be used inside a MACRO.\n");
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


#if defined(GB)

int directive_name_gb(void) {

  int token_result;

  no_library_files(".NAME");
  
  if ((token_result = get_next_token()) == FAILED)
    return FAILED;

  if (token_result != GET_NEXT_TOKEN_STRING) {
    print_error(ERROR_DIR, ".NAME requires a string of 1 to 16 letters.\n");
    return FAILED;
  }

  /* no name has been defined so far */
  if (g_name_defined == 0) {
    int i;

    for (i = 0; g_tmp[i] != 0 && i < 16; i++)
      g_name[i] = g_tmp[i];

    if (i == 16 && g_tmp[i] != 0) {
      print_error(ERROR_DIR, ".NAME requires a string of 1 to 16 letters.\n");
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
      print_error(ERROR_DIR, ".NAME requires a string of 1 to 16 letters.\n");
      return FAILED;
    }
    if (i != 16 && (g_name[i] != 0 || g_tmp[i] != 0)) {
      print_error(ERROR_DIR, ".NAME was already defined.\n");
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
    print_error(ERROR_DIR, "No .ROMBANKSIZE defined.\n");
    return FAILED;
  }

  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 1) {
    print_error(ERROR_DIR, ".ROMBANKS needs a positive integer value.\n");
    return FAILED;
  }

  /* check that the old bank map (smaller) and the new one equal as much as they can */
  if (g_rombanks_defined != 0) {
    int bank;

    if (g_rombanks < g_parsed_int)
      bank = g_rombanks;
    else
      bank = g_parsed_int;

    for (i = 0; i < bank; i++) {
      if (g_banks[i] != g_banksize) {
        print_error(ERROR_DIR, "The old and the new .ROMBANKMAP's don't match.\n");
        return FAILED;
      }
    }

    if (g_parsed_int <= g_rombanks)
      return SUCCEEDED;

    print_error(ERROR_WRN, "Upgrading from %d to %d ROM banks.\n", g_rombanks, g_parsed_int);
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
    print_error(ERROR_DIR, "Out of memory while allocating ROM banks.\n");
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
    print_error(ERROR_DIR, "Out of memory while allocating ROM banks.\n");
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
          print_error(ERROR_DIR, "BANKSTOTAL needs a positive value.\n");
          return FAILED;
        }

        if (g_rombanks < g_parsed_int) {
          g_banks = realloc(g_banks, sizeof(int) * g_parsed_int);
          g_bankaddress = realloc(g_bankaddress, sizeof(int) * g_parsed_int);
          if (g_banks == NULL || g_bankaddress == NULL) {
            print_error(ERROR_DIR, "Out of memory while allocating ROM banks.\n");
            return FAILED;
          }
        }

        bt = g_parsed_int;
        bt_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error(ERROR_DIR, "BANKSTOTAL needs to be defined prior to BANKSIZE.\n");
          return FAILED;
        }

        q = input_number();
    
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error(ERROR_DIR, "BANKSIZE needs a positive value.\n");
          return FAILED;
        }
    
        bs = g_parsed_int;
        bs_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error(ERROR_DIR, "BANKSIZE needs to be defined prior to BANKS.\n");
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error(ERROR_DIR, "BANKS needs a positive value.\n");
          return FAILED;
        }

        for (x = 0; x < g_parsed_int; x++) {
          if (b > bt) {
            print_error(ERROR_DIR, "More BANKS than BANKSTOTAL suggests.\n");
            return FAILED;
          }

          /* new banks? */
          if (x >= g_rombanks) {
            g_banks[o] = bs;
            g_bankaddress[o] = a;
          }
          /* compare old banks */
          else if (g_banks[o] != bs) {
            print_error(ERROR_DIR, "The old and the new ROMBANKMAPs don't match.\n");
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
          print_error(ERROR_DIR, "BANKSTOTAL needs a positive value.\n");
          return FAILED;
        }

        g_banks = calloc(sizeof(int) * g_parsed_int, 1);
        g_bankaddress = calloc(sizeof(int) * g_parsed_int, 1);
        if (g_banks == NULL || g_bankaddress == NULL) {
          print_error(ERROR_DIR, "Out of memory while allocating ROM banks.\n");
          return FAILED;
        }

        bt = g_parsed_int;
        bt_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKSIZE") == 0) {
        if (bt_defined == 0) {
          print_error(ERROR_DIR, "BANKSTOTAL needs to be defined prior to BANKSIZE.\n");
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error(ERROR_DIR, "BANKSIZE needs a positive value.\n");
          return FAILED;
        }

        bs = g_parsed_int;
        bs_defined = 1;
      }
      else if (strcaselesscmp(g_tmp, "BANKS") == 0) {
        if (bs_defined == 0) {
          print_error(ERROR_DIR, "BANKSIZE needs to be defined prior to BANKS.\n");
          return FAILED;
        }

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int <= 0) {
          print_error(ERROR_DIR, "BANKS needs a positive value.\n");
          return FAILED;
        }

        for (x = 0; x < g_parsed_int; x++) {
          if (b >= bt) {
            print_error(ERROR_DIR, "More BANKS than BANKSTOTAL suggests.\n");
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
    print_error(ERROR_DIR, "Error in .ROMBANKMAP data structure.\n");
    return FAILED;
  }

  /* no banks definded? */
  if (bt == 0) {
    print_error(ERROR_DIR, "No ROM banks were defined inside the .ROMBANKMAP.\n");
    return FAILED;      
  }
  if (bt != b) {
    print_error(ERROR_DIR, "Not all ROM banks were defined inside the .ROMBANKMAP.\n");
    return FAILED;      
  }

  if (g_rombanks_defined != 0) {
    if (b > g_rombanks)
      print_error(ERROR_WRN, "Upgrading from %d to %d ROM banks.\n", g_rombanks, b);
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
    print_error(ERROR_DIR, "Out of memory while allocating ROM banks.\n");
    return FAILED;
  }

  memset(g_rom_banks_usage_table, 0, sizeof(unsigned char) * g_max_address);
  g_rombankmap_defined = 1;

  return SUCCEEDED;
}


int directive_memorymap(void) {
  
  int slotsize = 0, slotsize_defined = 0, s = 0, q, o, token_result;

  if (g_memorymap_defined == 1) {
    print_error(ERROR_DIR, ".MEMORYMAP can be defined only once.\n");
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY)
    print_error(ERROR_WRN, "Libraries don't need .MEMORYMAP.\n");

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
      if (s_defaultslot_defined == 0) {
        print_error(ERROR_DIR, "No DEFAULTSLOT defined.\n");
        return FAILED;
      }

      if (g_slots[s_defaultslot].size == 0) {
        print_error(ERROR_DIR, "Unknown DEFAULTSLOT %d.\n", s_defaultslot);
        return FAILED;
      }

      break;
    }
    else if (strcaselesscmp(g_tmp, "SLOTSIZE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "SLOTSIZE needs an immediate value.\n");
        return FAILED;
      }

      slotsize = g_parsed_int;
      slotsize_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "DEFAULTSLOT") == 0) {
      if (s_defaultslot_defined != 0) {
        print_error(ERROR_DIR, "DEFAULTSLOT can be defined only once.\n");
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int > 255 || g_parsed_int < 0) {
        print_error(ERROR_DIR, "DEFAULTSLOT needs an immediate value (0-255) as an ID.\n");
        return FAILED;
      }

      s_defaultslot_defined = 1;
      s_defaultslot = g_parsed_int;
    }
    else if (strcaselesscmp(g_tmp, "SLOT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int > 255 || g_parsed_int < 0) {
        print_error(ERROR_DIR, "SLOT needs a positive value (0-255) as an ID.\n");
        return FAILED;
      }

      if (s != g_parsed_int) {
        print_error(ERROR_DIR, "Error in SLOT's ID. ID's start from 0.\n");
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
        print_error(ERROR_DIR, "The starting address has to be a non-negative value.\n");
        return FAILED;
      }

      g_slots[o].address = g_parsed_int;

      /* skip "SIZE" if present */
      if (compare_next_token("SIZE") == SUCCEEDED)
        skip_next_token();

      q = input_number();

      if (q == INPUT_NUMBER_EOL) {
        if (slotsize_defined == 0) {
          print_error(ERROR_DIR, "SLOTSIZE must be defined if you don't explicitly give the size.\n");
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
          print_error(ERROR_DIR, "The size of the slot needs to be an immediate value.\n");
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
            print_error(ERROR_DIR, "NAME needs a label/string for name.\n");
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
    print_error(ERROR_DIR, "Error in .MEMORYMAP data structure.\n");
    return FAILED;
  }

  g_memorymap_defined = 1;

  return SUCCEEDED;
}


int directive_unbackground(void) {
  
  int start, end, q;

  if (g_output_format != OUTPUT_OBJECT) {
    print_error(ERROR_DIR, ".UNBACKGROUND can only be used in OBJECT output mode.\n");
    return FAILED;
  }
  if (g_background_defined == 0) {
    print_error(ERROR_DIR, "No .BACKGROUND specified.\n");
    return FAILED;
  }

  /* get the starting address */
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || q < 0) {
    print_error(ERROR_DIR, ".UNBACKGROUND needs the block's starting address.\n");
    return FAILED;
  }

  start = g_parsed_int;

  /* get the ending address */
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || q < 0) {
    print_error(ERROR_DIR, ".UNBACKGROUND needs the block's ending address.\n");
    return FAILED;
  }

  end = g_parsed_int;

  if (end < start) {
    print_error(ERROR_DIR, "The block's ending address is smaller than the starting address!\n");
    return FAILED;
  }
  if (start >= g_max_address) {
    print_error(ERROR_DIR, "The block's starting address $%x is beyond the ROM's ending address $%x.\n", start, g_max_address-1);
    return FAILED;
  }
  if (end >= g_max_address) {
    end = g_max_address;
    print_error(ERROR_WRN, "The block's ending address $%x is beyond the ROM's ending address $%x. Using the ROM's ending address instead.\n", end, g_max_address-1);
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
    print_error(ERROR_DIR, ".BACKGROUND can only be used in OBJECT output mode.\n");
    return FAILED;
  }
  if (g_background_defined == 1) {
    print_error(ERROR_DIR, "Only one .BACKGROUND can be specified.\n");
    return FAILED;
  }

  g_expect_calculations = NO;
  g_input_parse_special_chars = NO;
  q = input_number();
  g_input_parse_special_chars = YES;
  g_expect_calculations = YES;

  if (q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".BACKGROUND needs a file name string.\n");
    return FAILED;
  }

  if (g_memorymap_defined == 0) {
    print_error(ERROR_DIR, "No .MEMORYMAP defined.\n");
    return FAILED;
  }
  if (g_rombanks_defined == 0) {
    print_error(ERROR_DIR, "No .ROMBANKS defined.\n");
    return FAILED;
  }

  create_full_name(g_include_dir, g_label);

  /* convert the path to local enviroment */
  localize_path(g_full_name);

  if ((file_in_ptr = fopen(g_full_name, "rb")) == NULL) {
    print_error(ERROR_DIR, "Error opening .BACKGROUND file \"%s\".\n", g_full_name);
    return FAILED;
  }

  fseek(file_in_ptr, 0, SEEK_END);
  background_size = (int)ftell(file_in_ptr);
  fseek(file_in_ptr, 0, SEEK_SET);
  
  if (background_size > g_max_address) {
    print_error(ERROR_DIR, ".BACKGROUND file \"%s\" size (%d) is larger than ROM size (%d).\n", g_full_name, background_size, g_max_address);
    fclose(file_in_ptr);
    return FAILED;
  }

  memset(g_rom_banks_usage_table, 2, background_size);
  if (fread(g_rom_banks, 1, background_size, file_in_ptr) != (size_t) background_size) {
    print_error(ERROR_INC, "Could not read all %d bytes of \"%s\"!", background_size, g_full_name);
    return FAILED;
  }

  g_background_defined = 1;
  fclose(file_in_ptr);

  return SUCCEEDED;
}


#if defined(GB)

int directive_gbheader(void) {

  int q, token_result;
    
  if (s_gbheader_defined != 0) {
    print_error(ERROR_DIR, ".GBHEADER can be defined only once.\n");
    return FAILED;
  }

  if (g_computechecksum_defined != 0)
    print_error(ERROR_WRN, ".COMPUTEGBCHECKSUM is unnecessary when .GBHEADER is defined.\n");
  else
    g_computechecksum_defined++;

  if (g_computecomplementcheck_defined != 0)
    print_error(ERROR_WRN, ".COMPUTEGBCOMPLEMENTCHECK is unnecessary when .GBHEADER is defined.\n");
  else
    g_computecomplementcheck_defined++;

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .GBHEADER.\n");
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
        print_error(ERROR_DIR, ".ROMGBC was defined prior to .ROMDMG.\n");
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(ERROR_DIR, ".ROMGBCONLY was defined prior to .ROMDMG.\n");
        return FAILED;
      }
      else if (g_romsgb != 0) {
        print_error(ERROR_DIR, ".ROMDMG and .ROMSGB cannot be mixed.\n");
        return FAILED;
      }
      g_romdmg = 1;
    }
    else if (strcaselesscmp(g_tmp, "ROMGBC") == 0) {
      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG was defined prior to .ROMGBC.\n");
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(ERROR_DIR, ".ROMGBCONLY was defined prior to .ROMGBC.\n");
        return FAILED;
      }
      g_romgbc = 1;
    }
    else if (strcaselesscmp(g_tmp, "ROMGBCONLY") == 0) {
      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG was defined prior to .ROMGBCONLY.\n");
        return FAILED;
      }
      else if (g_romgbc == 1) {
        print_error(ERROR_DIR, ".ROMGBC was defined prior to .ROMGBCONLY.\n");
        return FAILED;
      }
      g_romgbc = 2;
    }
    else if (strcaselesscmp(g_tmp, "ROMSGB") == 0) {
      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG and .ROMSGB cannot be mixed.\n");
        return FAILED;
      }
      g_romsgb++;
    }
    else if (strcaselesscmp(g_tmp, "NAME") == 0) {
      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error(ERROR_DIR, "NAME requires a string of 1 to 16 letters.\n");
        return FAILED;
      }

      /* no name has been defined so far */
      if (g_name_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 16; i++)
          g_name[i] = g_tmp[i];
    
        if (i == 16 && g_tmp[i] != 0) {
          print_error(ERROR_DIR, "NAME requires a string of 1 to 16 letters.\n");
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
          print_error(ERROR_DIR, "NAME requires a string of 1 to 16 letters.\n");
          return FAILED;
        }
        if (i != 16 && (g_name[i] != 0 || g_tmp[i] != 0)) {
          print_error(ERROR_DIR, "NAME was already defined.\n");
          return FAILED;
        }
      }
    }
    else if (strcaselesscmp(g_tmp, "LICENSEECODEOLD") == 0) {
      if (g_licenseecodenew_defined != 0) {
        print_error(ERROR_DIR, ".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n");
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        print_error(ERROR_DIR, ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_licenseecodeold_defined != 0) {
        if (g_licenseecodeold != g_parsed_int) {
          print_error(ERROR_DIR, ".LICENSEECODEOLD was defined for the second time.\n");
          return FAILED;
        }
      }

      g_licenseecodeold = g_parsed_int;
      g_licenseecodeold_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "LICENSEECODENEW") == 0) {
      if (g_licenseecodeold_defined != 0) {
        print_error(ERROR_DIR, ".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n");
        return FAILED;
      }

      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error(ERROR_DIR, ".LICENSEECODENEW requires a string of two letters.\n");
        return FAILED;
      }
      if (!(g_tmp[0] != 0 && g_tmp[1] != 0 && g_tmp[2] == 0)) {
        print_error(ERROR_DIR, ".LICENSEECODENEW requires a string of two letters.\n");
        return FAILED;
      }

      if (g_licenseecodenew_defined != 0) {
        if (g_tmp[0] != g_licenseecodenew_c1 || g_tmp[1] != g_licenseecodenew_c2) {
          print_error(ERROR_DIR, ".LICENSEECODENEW was defined for the second time.\n");
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
        print_error(ERROR_DIR, "CARTRIDGETYPE needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_cartridgetype_defined != 0 && g_cartridgetype != g_parsed_int) {
          print_error(ERROR_DIR, "CARTRIDGETYPE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "RAMSIZE needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_rambanks_defined != 0 && g_rambanks != g_parsed_int) {
          print_error(ERROR_DIR, "RAMSIZE was defined for the second time.\n");
          return FAILED;
        }

        g_rambanks = g_parsed_int;
        g_rambanks_defined = 1;
      }
      else
        return FAILED;
    }
    else if (strcaselesscmp(g_tmp, "ROMSIZE") == 0) {
      int number_result = input_number();

      if (number_result == INPUT_NUMBER_EOL) {
        next_line();
        g_parsed_int = -1;
      }
      else if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "ROMSIZE needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result != SUCCEEDED)
        return FAILED;
      
      if (g_romsize_defined != 0 && g_romsize != g_parsed_int) {
        print_error(ERROR_DIR, "ROMSIZE was defined for the second time.\n");
        return FAILED;
      }

      g_romsize = g_parsed_int;
      g_romsize_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "COUNTRYCODE") == 0) {
      int number_result = input_number();
      
      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "COUNTRYCODE needs a non-negative value, got %d.\n\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_countrycode_defined != 0 && g_countrycode != g_parsed_int) {
          print_error(ERROR_DIR, "COUNTRYCODE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "DESTINATIONCODE needs a non-negative value, got %d.\n\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_countrycode_defined != 0 && g_countrycode != g_parsed_int) {
          print_error(ERROR_DIR, "DESTINATIONCODE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "VERSION needs a non-negative value, got %d.\n\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_version_defined != 0 && g_version != g_parsed_int) {
          print_error(ERROR_DIR, "VERSION was defined for the second time.\n");
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
    print_error(ERROR_DIR, "Error in .GBHEADER data structure.\n");
    return FAILED;
  }

  s_gbheader_defined = 1;

  return SUCCEEDED;
}

#endif


static struct array *_get_array(char *name) {

  struct array *arr = g_arrays_first;
  char c1 = name[0];

  while (arr != NULL) {
    if (c1 == arr->name[0]) {
      if (strcmp(name, arr->name) == 0)
        return arr;
    }
    arr = arr->next;
  }

  return NULL;
}


static struct array *_create_array(char *name, int size) {

  struct array *arr;
  int i, *data;
  
  arr = calloc(sizeof(struct array), 1);
  if (arr == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new array.\n");
    return NULL;
  }

  if (size < 256)
    size = 256;

  strcpy(arr->name, name);
  arr->size = size;

  arr->data = (int *)calloc(sizeof(int) * size, 1);
  if (arr->data == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new array.\n");
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
  int q, name_defined = NO, size = 0;

  strcpy(bak, g_current_directive);

  if (compare_next_token("NAME") != SUCCEEDED && compare_next_token("SIZE") != SUCCEEDED) {
    /* this is the name */
    if (get_next_plain_string() == FAILED)
      return FAILED;

    /* check that the array doesn't exist */
    if (_get_array(g_label) != NULL) {
      print_error(ERROR_DIR, "\"%s\" is already defined.\n", g_label);
      return FAILED;
    }

    strcpy(name, g_label);

    name_defined = YES;
  }
  
  while (1) {
    if (compare_next_token("NAME") == SUCCEEDED) {
      skip_next_token();

      if (get_next_plain_string() == FAILED)
        return FAILED;

      /* check that the array doesn't exist */
      if (_get_array(g_label) != NULL) {
        print_error(ERROR_DIR, "\"%s\" is already defined.\n", g_label);
        return FAILED;
      }
      
      strcpy(name, g_label);

      name_defined = YES;
    }
    else if (compare_next_token("SIZE") == SUCCEEDED) {
      skip_next_token();
  
      q = input_number();
      
      if (q == FAILED)
        return FAILED;
      else if (q == SUCCEEDED) {
      }
      else {
        print_error(ERROR_DIR, ".%s needs an immediate value for the size.\n", bak);
        return FAILED;
      }

      size = g_parsed_int;
    }
    else
      break;
  }

  if (name_defined == NO) {
    print_text(NO, ".%s need a name.\n", bak);
    return FAILED;
  }

  if (size == 0) {
    q = input_number();
      
    if (q == FAILED)
      return FAILED;
    else if (q == SUCCEEDED) {
    }
    else {
      print_error(ERROR_DIR, ".%s needs an immediate value for the size.\n", bak);
      return FAILED;
    }

    size = g_parsed_int;
  }
    
  arr = _create_array(name, size);
  if (arr == NULL)
    return FAILED;

  return SUCCEEDED;
}


int directive_arrayin(void) {

  struct array *arr;
  int index, value, q;
  
  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_label);
  if (arr == NULL) {
    print_error(ERROR_DIR, "Array \"%s\" doesn't exist.\n", g_label);
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
    print_error(ERROR_DIR, ".ARRAYIN needs an immediate value for the index.\n");
    return FAILED;
  }

  index = g_parsed_int;

  if (index < 0) {
    print_error(ERROR_DIR, ".ARRAYIN needs a positive or zero value for the index.\n");
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
      print_error(ERROR_DIR, ".ARRAYIN needs an immediate value for the value.\n");
      return FAILED;
    }

    value = g_parsed_int;

    if (index < arr->size)
      arr->data[index] = value;
    else {
      /* index is out of bounds, resize the array, and then insert the value */
      arr->data = realloc(arr->data, sizeof(int) * (index + 1));
      if (arr->data == NULL) {
        print_error(ERROR_DIR, "Out of memory resizing the array.\n");
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
  
  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_label);
  if (arr == NULL) {
    print_error(ERROR_DIR, "Array \"%s\" doesn't exist.\n", g_label);
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
    print_error(ERROR_DIR, ".ARRAYOUT needs an immediate value for the index.\n");
    return FAILED;
  }

  index = g_parsed_int;

  if (index < 0) {
    print_error(ERROR_DIR, ".ARRAYOUT needs a positive or zero value for the index.\n");
    return FAILED;
  }
  if (index >= arr->size) {
    print_error(ERROR_DIR, "Index %d is out of array \"%s\"'s size of %d items.\n", index, arr->name, arr->size);
    return FAILED;
  }
  
  /* skip DEFINITION if present */
  if (compare_next_token("DEFINITION") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  redefine(g_label, (double)arr->data[index], NULL, DEFINITION_TYPE_VALUE, 0);

  return SUCCEEDED;
}


int directive_arraydb_arraydw_arraydl_arraydd(void) {

  struct array *arr;
  int index = 0, q, i = 0, data_size;
  char bak[256];

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
    print_error(ERROR_DIR, "Unsupported directive \".%s\"! Please submit a bug report!\n", g_current_directive);
    return FAILED;
  }
  
  /* skip NAME if present */
  if (compare_next_token("NAME") == SUCCEEDED)
    skip_next_token();

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* check that the array exists */
  arr = _get_array(g_label);
  if (arr == NULL) {
    print_error(ERROR_DIR, "Array \"%s\" doesn't exist.\n", g_label);
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
      print_error(ERROR_DIR, ".%s needs an immediate value or a string for the index.\n", bak);
      return FAILED;
    }

    if (q == SUCCEEDED) {
      index = g_parsed_int;

      if (index < 0) {
        print_error(ERROR_DIR, ".%s needs a positive or zero value for the index.\n", bak);
        return FAILED;
      }
      if (index >= arr->size) {
        print_error(ERROR_DIR, "Index %d is out of array \"%s\"'s size of %d items.\n", index, arr->name, arr->size);
        return FAILED;
      }

      i = arr->data[index];
    }
    
    if (data_size == 1) {
      if (q == SUCCEEDED) {
        if (i < -128 || i > 255) {
          print_error(ERROR_DIR, "The value (%d) in the array (index %d) is out of 8-bit range!\n", i, index);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "d%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            print_error(ERROR_DIR, "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -128 || i > 255) {
            print_error(ERROR_DIR, "The value (%d) in the array (index %d, '%c') is out of 8-bit range!\n", i, index, (char)index);
            return FAILED;
          }

          fprintf(g_file_out_ptr, "d%d ", i);
        }
      }
    }
    else if (data_size == 2) {
      if (q == SUCCEEDED) {
        if (i < -32768 || i > 65535) {
          print_error(ERROR_DIR, "The value (%d) in the array (index %d) is out of 16-bit range!\n", i, index);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "y%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            print_error(ERROR_DIR, "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -32768 || i > 65535) {
            print_error(ERROR_DIR, "The value (%d) in the array (index %d, '%c') is out of 16-bit range!\n", i, index, (char)index);
            return FAILED;
          }

          fprintf(g_file_out_ptr, "y%d ", i);
        }
      }
    }
    else if (data_size == 3) {
      if (q == SUCCEEDED) {
        if (i < -8388608 || i > 16777215) {
          print_error(ERROR_DIR, "The value (%d) in the array (index %d) is out of 24-bit range!\n", i, index);
          return FAILED;
        }

        fprintf(g_file_out_ptr, "z%d ", i);
      }
      else if (q == INPUT_NUMBER_STRING) {
        int k;

        for (k = 0; k < g_string_size; k++) {
          index = (int)g_label[k];

          if (index >= arr->size) {
            print_error(ERROR_DIR, "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            return FAILED;
          }

          i = arr->data[index];

          if (i < -8388608 || i > 16777215) {
            print_error(ERROR_DIR, "The value (%d) in the array (index %d, '%c') is out of 24-bit range!\n", i, index, (char)index);
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
          print_error(ERROR_DIR, "The value (%d) in the array is out of 32-bit range!\n", i);
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
            print_error(ERROR_DIR, "Index %d ('%c') is out of array \"%s\"'s size of %d items.\n", index, (char)index, arr->name, arr->size);
            return FAILED;
          }

          i = arr->data[index];

          /*
          if (i < -2147483648 || i > 2147483647) {
            print_error(ERROR_DIR, "The value (%d) in the array (index %d, '%c') is out of 32-bit range!\n", i, index, (char)index);
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


int directive_function(void) {
  
  char name[MAX_NAME_LENGTH+1];
  struct function *f;
  int res;
  char c1;

  if (get_next_plain_string() == FAILED)
    return FAILED;

  /* do we already have a function with that name? */
  c1 = g_label[0];
  f = g_functions_first;
  while (f != NULL) {
    if (c1 == f->name[0]) {
      if (strcmp(g_label, f->name) == 0) {
        print_error(ERROR_DIR, ".FUNCTION called \"%s\" exists already!\n", g_label);
        return FAILED;
      }
    }
    f = f->next;
  }

  /* also check built-in function names */
  if (strcmp("asc", g_label) == 0 ||
      strcmp("min", g_label) == 0 ||
      strcmp("max", g_label) == 0 ||
      strcmp("random", g_label) == 0 ||
      strcmp("defined", g_label) == 0 ||
      strcmp("exists", g_label) == 0 ||
      strcmp("lobyte", g_label) == 0 ||
      strcmp("hibyte", g_label) == 0 ||
      strcmp("loword", g_label) == 0 ||
      strcmp("hiword", g_label) == 0 ||
      strcmp("bankbyte", g_label) == 0 ||
      strcmp("bank", g_label) == 0 ||
      strcmp("round", g_label) == 0 ||
      strcmp("ceil", g_label) == 0 ||
      strcmp("floor", g_label) == 0 ||
      strcmp("sqrt", g_label) == 0 ||
      strcmp("cos", g_label) == 0 ||
      strcmp("sin", g_label) == 0 ||
      strcmp("tan", g_label) == 0 ||
      strcmp("acos", g_label) == 0 ||
      strcmp("asin", g_label) == 0 ||
      strcmp("atan", g_label) == 0 ||
      strcmp("atan2", g_label) == 0 ||
      strcmp("cosh", g_label) == 0 ||
      strcmp("sinh", g_label) == 0 ||
      strcmp("tanh", g_label) == 0 ||
      strcmp("log", g_label) == 0 ||
      strcmp("log10", g_label) == 0 ||
      strcmp("pow", g_label) == 0 ||
      strcmp("clamp", g_label) == 0 ||
      strcmp("sign", g_label) == 0 ||
      strcmp("abs", g_label) == 0) {
    print_error(ERROR_DIR, "You cannot redefine a built-in .FUNCTION \"%s\"!\n", g_label);
    return FAILED;
  }

  strcpy(name, g_label);
  
  f = calloc(sizeof(struct function), 1);
  if (f == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating a new function.\n");
    return FAILED;
  }

  strcpy(f->name, name);
  f->filename_id = g_active_file_info_last->filename_id;
  f->line_number = g_active_file_info_last->line_current;
  f->nargument_names = 0;
  f->stack = NULL;
  f->next = NULL;

  if (g_functions_first == NULL) {
    g_functions_first = f;
    g_functions_last = f;
  }
  else {
    g_functions_last->next = f;
    g_functions_last = f;
  }

  if (compare_and_skip_next_symbol('(') == FAILED) {
    print_error(ERROR_DIR, "Opening parenthesis is missing from function \"%s\".\n", name);
    return FAILED;
  }
  
  while (1) {
    if (compare_and_skip_next_symbol(')') == SUCCEEDED)
      break;

    res = input_next_string();
    if (res == INPUT_NUMBER_EOL)
      next_line();
    else if (res == FAILED)
      return FAILED;

    f->argument_names[f->nargument_names] = calloc((int)strlen(g_label) + 1, 1);
    if (f->argument_names[f->nargument_names] == NULL) {
      print_error(ERROR_DIR, "Out of memory while allocating a new function.\n");
      return FAILED;
    }
    strcpy(f->argument_names[f->nargument_names], g_label);
    f->nargument_names++;
  }

  /* parse the function's body */
  g_resolve_stack_calculations = NO;
  g_parsing_function_body = YES;
  g_input_float_mode = ON;
  res = input_number();
  g_parsing_function_body = NO;
  g_resolve_stack_calculations = YES;
  g_input_float_mode = OFF;

  /* turn single address labels into calculation stacks */
  if (res == INPUT_NUMBER_ADDRESS_LABEL) {
    if (stack_create_label_stack(g_label) == FAILED)
      return FAILED;
    res = INPUT_NUMBER_STACK;
  }

  /* it should either be a value or a calculation stack */
  f->type = res;
  if (res == SUCCEEDED)
    f->value = g_parsed_int;
  else if (res == INPUT_NUMBER_FLOAT)
    f->value = g_parsed_double;
  else if (res == INPUT_NUMBER_STACK) {
    f->value = g_latest_stack;
    f->stack = find_stack_calculation(g_latest_stack, YES);

    if (f->stack == NULL)
      return FAILED;

    /* mark the calculation stack as function body so we don't export it in phase_4.c */
    f->stack->is_function_body = YES;
  }
  else if (res == FAILED)
    return FAILED;
  else {
    print_error(ERROR_DIR, "The function doesn't have a working body. It should either be a value or a calculation.\n");
    return FAILED;
  }

  return SUCCEEDED;
}


int directive_define_def_equ(void) {
  
  char k[256], label[MAX_NAME_LENGTH+1];
  int j, export, q, size;
  struct definition *d;
  double dou;

  if (g_is_file_isolated_counter > 0)
    g_force_add_namespace = YES;
  if (get_next_plain_string() == FAILED)
    return FAILED;
  if (g_is_file_isolated_counter > 0)
    g_force_add_namespace = NO;
  
  strcpy(label, g_label);
  
  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(label) == YES) {
    print_error(ERROR_DIR, "\"%s\" is a reserved definition label and is not user definable.\n", label);
    return FAILED;
  }

  hashmap_get(g_defines_map, label, (void*)&d);
  if (d != NULL) {
    print_error(ERROR_DIR, "\"%s\" was defined for the second time.\n", label);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  g_input_float_mode = ON;
  g_dsp_enable_label_address_conversion = NO;
  q = get_new_definition_data(&j, k, &size, &dou, &export);
  g_input_float_mode = OFF;
  g_dsp_enable_label_address_conversion = YES;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_EOL || q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_DIR, "Could not parse the definition data.\n");
    return FAILED;
  }

  if (q == SUCCEEDED)
    q = add_a_new_definition(label, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    q = add_a_new_definition(label, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    q = add_a_new_definition(label, 0.0, k, DEFINITION_TYPE_STRING, size);
  else if (q == INPUT_NUMBER_STACK)
    q = add_a_new_definition(label, (double)j, NULL, DEFINITION_TYPE_STACK, 0);
  else if (q == INPUT_NUMBER_EOL)
    q = add_a_new_definition(label, 0.0, NULL, DEFINITION_TYPE_VALUE, 0);
  
  if (q == FAILED)
    return FAILED;

  if (export == YES) {
    if (export_a_definition(label) == FAILED)
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
    int string_result;

    if (g_is_file_isolated_counter > 0)
      g_force_add_namespace = YES;
    string_result = input_next_string();
    if (g_is_file_isolated_counter > 0)
      g_force_add_namespace = NO;
  
    if (string_result == FAILED)
      return FAILED;
    if (string_result == INPUT_NUMBER_EOL) {
      if (q != 0) {
        next_line();
        return SUCCEEDED;
      }
      print_error(ERROR_DIR, ".%s requires definition name(s).\n", bak);
      return FAILED;
    }

    q++;

    if (undefine(g_label) == FAILED)
      print_error(ERROR_WRN, "Could not .%s \"%s\".\n", bak, g_label);
  }

  return SUCCEEDED;
}


int directive_export(void) {

  int q = 0;

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
      print_error(ERROR_DIR, ".EXPORT requires definition name(s).\n");
      return FAILED;
    }

    q++;

    if (export_a_definition(g_label) == FAILED)
      return FAILED;
  }
      
  return FAILED;
}


int directive_enumid(void) {
  
  int q;
  
  q = input_number();

  if (q == FAILED)
    return FAILED;

  if (q == INPUT_NUMBER_EOL) {
    print_error(ERROR_DIR, ".ENUMID needs a value or a definition name.\n");
    return FAILED;
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    if (s_enumid_defined == 0) {
      print_error(ERROR_DIR, ".ENUMID needs the initial value when .ENUMID is used the first time.\n");
      return FAILED;
    }
    
    if (is_reserved_definition(g_label) == YES) {
      print_error(ERROR_DIR, "\"%s\" is a reserved definition label and is not user definable.\n", g_label);
      return FAILED;
    }

    if (add_a_new_definition(g_label, (double)s_enumid, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    if (s_enumid_export == 1) {
      if (export_a_definition(g_label) == FAILED)
        return FAILED;
    }

    s_enumid += s_enumid_adder;

    return SUCCEEDED;
  }
  else if (q == SUCCEEDED) {
    s_enumid = g_parsed_int;
    s_enumid_adder = 1;
    s_enumid_export = 0;

    if (compare_next_token("STEP") == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;

      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "STEP needs a value\n");
        return FAILED;
      }

      s_enumid_adder = g_parsed_int;
    }

    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();

      s_enumid_export = 1;
    }

    s_enumid_defined = 1;

    return SUCCEEDED;
  }
  else {
    print_error(ERROR_DIR, ".ENUMID needs a value or a definition name.\n");
    return FAILED;
  }
}


int directive_input(void) {
  
  char k[256];
  int j, v;

  if (get_next_token() == FAILED)
    return FAILED;

  if (fgets(k, 254, stdin) == NULL) {
    print_error(ERROR_DIR, ".INPUT needs some data.\n");
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
    print_error(ERROR_DIR, "Error in .INPUT.\n");
    return FAILED;
  }

  for (j = 0; j < 254; j++) {
    if (k[j] == 0) {
      print_error(ERROR_DIR, "No .INPUT?\n");
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
  
  char k[256], label[MAX_NAME_LENGTH+1];
  int j, export, q, size;
  double dou;

  if (g_is_file_isolated_counter > 0)
    g_force_add_namespace = YES;
  if (get_next_plain_string() == FAILED)
    return FAILED;
  if (g_is_file_isolated_counter > 0)
    g_force_add_namespace = NO;
  
  strcpy(label, g_label);

  /* check the user doesn't try to define reserved labels */
  if (is_reserved_definition(label) == YES) {
    print_error(ERROR_DIR, "\"%s\" is a reserved definition label and is not user definable.\n", label);
    return FAILED;
  }

  /* skip "=", if present */
  if (compare_next_token("=") == SUCCEEDED)
    skip_next_token();

  g_input_float_mode = ON;
  g_dsp_enable_label_address_conversion = NO;
  q = get_new_definition_data(&j, k, &size, &dou, &export);
  g_input_float_mode = OFF;
  g_dsp_enable_label_address_conversion = YES;
  if (q == FAILED)
    return FAILED;

  if (!(q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
    print_error(ERROR_DIR, "Could not parse the definition data.\n");
    return FAILED;
  }

  if (q == SUCCEEDED)
    redefine(label, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_FLOAT)
    redefine(label, dou, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (q == INPUT_NUMBER_STRING)
    redefine(label, 0.0, k, DEFINITION_TYPE_STRING, size);
  else if (q == INPUT_NUMBER_STACK)
    redefine(label, (double)j, NULL, DEFINITION_TYPE_STACK, 0);

  if (export == YES) {
    if (export_a_definition(label) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


#if defined(Z80)

int directive_smsheader(void) {
  
  int q, token_result;
    
  if (g_smsheader_defined != 0) {
    print_error(ERROR_DIR, ".SMSHEADER can be defined only once.\n");
    return FAILED;
  }

  if (g_computesmschecksum_defined != 0)
    print_error(ERROR_WRN, ".COMPUTESMSCHECKSUM is unnecessary when .SMSHEADER is defined.\n");

  if (g_smstag_defined != 0)
    print_error(ERROR_WRN, ".SMSTAG is unnecessary when .SMSHEADER is defined.\n");

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .SMSHEADER.\n");
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
        print_error(ERROR_DIR, "VERSION needs a value between 0 and 15, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_smsversion_defined != 0) {
        if (g_smsversion != g_parsed_int) {
          print_error(ERROR_DIR, "VERSION was defined for the second time.\n");
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
        print_error(ERROR_DIR, "ROMSIZE needs a value between 0 and 15, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_smsromsize_defined != 0) {
        if (g_smsromsize != g_parsed_int) {
          print_error(ERROR_DIR, "ROMSIZE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "REGIONCODE needs a value between 3 and 7, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_smsregioncode_defined != 0) {
        if (g_smsregioncode != g_parsed_int) {
          print_error(ERROR_DIR, "REGIONCODE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "PRODUCTCODE needs 2.5 bytes of data.\n");
        return FAILED;
      }

      g_smsproductcode1 = g_parsed_int & 255;
      g_smsproductcode_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "PRODUCTCODE needs 2.5 bytes of data.\n");
        return FAILED;
      }

      g_smsproductcode2 = g_parsed_int & 255;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "PRODUCTCODE needs 2.5 bytes of data.\n");
        return FAILED;
      }

      g_smsproductcode3 = g_parsed_int & 15;
    }
    else if (strcaselesscmp(g_tmp, "RESERVEDSPACE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "RESERVEDSPACE needs 2 bytes of data.\n");
        return FAILED;
      }

      g_smsreservedspace1 = g_parsed_int & 255;
      s_smsreservedspace_defined = 1;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "RESERVEDSPACE needs 2 bytes of data.\n");
        return FAILED;
      }

      g_smsreservedspace2 = g_parsed_int & 255;
    }
    else if (strcaselesscmp(g_tmp, "CHECKSUMSIZE") == 0) {
      if (g_smsforcechecksum_defined == 1) {
        print_error(ERROR_DIR, "FORCECHECKSUM is already defined.\n");
        return FAILED;
      }
      
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "CHECKSUMSIZE needs the number of bytes for calculating the checksum.\n");
        return FAILED;
      }

      g_smschecksumsize = g_parsed_int;
      g_smschecksumsize_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "FORCECHECKSUM") == 0) {
      if (g_smschecksumsize_defined == 1) {
        print_error(ERROR_DIR, "CHECKSUMSIZE is already defined.\n");
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "FORCECHECKSUM needs the checksum.\n");
        return FAILED;
      }

      g_smsforcechecksum = g_parsed_int;
      g_smsforcechecksum_defined = 1;
    }
    else if (strcaselesscmp(g_tmp, "BASEADDRESS") == 0) {
      if (g_romheader_baseaddress_defined == 1) {
        print_error(ERROR_DIR, "BASEADDRESS is already defined.\n");
        return FAILED;
      }
      
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "BASEADDRESS needs an immediate value.\n");
        return FAILED;
      }

      g_romheader_baseaddress = g_parsed_int;
      g_romheader_baseaddress_defined = 1;
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error(ERROR_DIR, "Error in .SMSHEADER data structure.\n");
    return FAILED;
  }

  g_smsheader_defined = 1;

  return SUCCEEDED;
}


int directive_sdsctag(void) {
  
  int q;

  if (g_sdsctag_defined != 0) {
    print_error(ERROR_DIR, ".SDSCTAG can be defined only once.\n");
    return FAILED;
  }
    
  no_library_files(".SDSCTAG");

  g_input_float_mode = ON;
  q = input_number();
  g_input_float_mode = OFF;
  if (q != SUCCEEDED && q != INPUT_NUMBER_FLOAT) {
    print_error(ERROR_DIR, ".SDSCTAG needs the version number.\n");
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
    print_error(ERROR_DIR, ".SDSCTAG major and minor version numbers must be inside the range [0,99].\n");
    return FAILED;
  }

  /* program name */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (!(q == INPUT_NUMBER_STRING || q == SUCCEEDED || q == INPUT_NUMBER_STACK || q == INPUT_NUMBER_ADDRESS_LABEL)) {
    print_error(ERROR_DIR, ".SDSCTAG requires program name string (or a pointer to it).\n");
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
      g_sdsctag_name_str = _string_duplicate(g_label);
      if (g_sdsctag_name_str == NULL)
        return FAILED;
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_name_type = TYPE_LABEL;
    g_sdsctag_name_str = _string_duplicate(g_label);
    if (g_sdsctag_name_str == NULL)
      return FAILED;
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
    print_error(ERROR_DIR, ".SDSCTAG requires program release notes string (or a pointer to it).\n");
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
      g_sdsctag_notes_str = _string_duplicate(g_label);
      if (g_sdsctag_notes_str == NULL)
        return FAILED;
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_notes_type = TYPE_LABEL;
    g_sdsctag_notes_str = _string_duplicate(g_label);
    if (g_sdsctag_notes_str == NULL)
      return FAILED;
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
    print_error(ERROR_DIR, ".SDSCTAG requires program author string (or a pointer to it).\n");
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
      g_sdsctag_author_str = _string_duplicate(g_label);
      if (g_sdsctag_author_str == NULL)
        return FAILED;
    }
  }
  else if (q == INPUT_NUMBER_ADDRESS_LABEL) {
    g_sdsctag_author_type = TYPE_LABEL;
    g_sdsctag_author_str = _string_duplicate(g_label);
    if (g_sdsctag_author_str == NULL)
      return FAILED;
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


#if defined(MC68000)

static void _copy_and_pad_string(char *output, char *input, int size, char padding) {

  int i = 0;

  while (i < size) {
    if (input[i] == 0)
      break;
    output[i] = input[i];
    i++;
  }
  while (i < size)
    output[i++] = padding;
}


int directive_smdheader(void) {
  
  int q, token_result;
    
  if (g_smdheader_defined == YES) {
    print_error(ERROR_DIR, ".SMDHEADER can be defined only once.\n");
    return FAILED;
  }

  if (g_computesmdchecksum_defined != 0)
    print_error(ERROR_WRN, ".COMPUTESMDCHECKSUM is unnecessary when .SMDHEADER is defined.\n");

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .SMDHEADER.\n");
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

    if (strcaselesscmp(g_tmp, ".ENDSMD") == 0)
      break;
    else if (strcaselesscmp(g_tmp, "SYSTEMTYPE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 16) {
        print_error(ERROR_DIR, "SYSTEMTYPE expects a string with maximum 16 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_systemtype, g_label, 16, ' ');
    }
    else if (strcaselesscmp(g_tmp, "COPYRIGHT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 16) {
        print_error(ERROR_DIR, "COPYRIGHT expects a string with maximum 16 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_copyright, g_label, 16, ' ');
    }
    else if (strcaselesscmp(g_tmp, "TITLEDOMESTIC") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 48) {
        print_error(ERROR_DIR, "TITLEDOMESTIC expects a string with maximum 48 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_titledomestic, g_label, 48, ' ');
    }
    else if (strcaselesscmp(g_tmp, "TITLEOVERSEAS") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 48) {
        print_error(ERROR_DIR, "TITLEOVERSEAS expects a string with maximum 48 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_titleoverseas, g_label, 48, ' ');
    }
    else if (strcaselesscmp(g_tmp, "SERIALNUMBER") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 14) {
        print_error(ERROR_DIR, "SERIALNUMBER expects a string with maximum 14 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_serialnumber, g_label, 14, ' ');
    }
    else if (strcaselesscmp(g_tmp, "DEVICESUPPORT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 16) {
        print_error(ERROR_DIR, "DEVICESUPPORT expects a string with maximum 16 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_devicesupport, g_label, 16, ' ');
    }
    else if (strcaselesscmp(g_tmp, "ROMADDRESSRANGE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "ROMADDRESSRANGE takes two integers, start and end.\n");
        return FAILED;
      }

      g_smdheader_romaddressrange_start = g_parsed_int;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "ROMADDRESSRANGE takes two integers, start and end.\n");
        return FAILED;
      }

      g_smdheader_romaddressrange_end = g_parsed_int;
    }
    else if (strcaselesscmp(g_tmp, "RAMADDRESSRANGE") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "RAMADDRESSRANGE takes two integers, start and end.\n");
        return FAILED;
      }

      g_smdheader_ramaddressrange_start = g_parsed_int;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "RAMADDRESSRANGE takes two integers, start and end.\n");
        return FAILED;
      }

      g_smdheader_ramaddressrange_end = g_parsed_int;
    }
    else if (strcaselesscmp(g_tmp, "EXTRAMEMORY") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 2) {
        print_error(ERROR_DIR, "EXTRAMEMORY expects here a string with maximum 2 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_extramemory_type_1, g_label, 2, ' ');

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        print_error(ERROR_DIR, "EXTRAMEMORY expects here a byte value (RAM type).\n");
        return FAILED;
      }

      g_smdheader_extramemory_type_2 = g_parsed_int;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        print_error(ERROR_DIR, "EXTRAMEMORY expects here a byte value (this is usually $20).\n");
        return FAILED;
      }

      g_smdheader_extramemory_type_3 = g_parsed_int;
      
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "EXTRAMEMORY expects here a start address value.\n");
        return FAILED;
      }

      g_smdheader_extramemory_start = g_parsed_int;

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "EXTRAMEMORY expects here a end address value.\n");
        return FAILED;
      }

      g_smdheader_extramemory_end = g_parsed_int;
    }
    else if (strcaselesscmp(g_tmp, "MODEMSUPPORT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 12) {
        print_error(ERROR_DIR, "MODEMSUPPORT expects a string with maximum 12 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_modemsupport, g_label, 12, ' ');
    }
    else if (strcaselesscmp(g_tmp, "REGIONSUPPORT") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != INPUT_NUMBER_STRING || strlen(g_label) > 3) {
        print_error(ERROR_DIR, "REGIONSUPPORT expects a string with maximum 3 characters.\n");
        return FAILED;
      }

      _copy_and_pad_string(g_smdheader_regionsupport, g_label, 3, ' ');
    }
    else {
      token_result = FAILED;
      break;
    }
  }

  if (token_result != SUCCEEDED) {
    print_error(ERROR_DIR, "Error in .SMDHEADER data structure.\n");
    return FAILED;
  }

  g_smdheader_defined = YES;
  g_computesmdchecksum_defined++;
      
  return SUCCEEDED;
}

#endif


static int _parse_macro_argument_names(struct macro_static *m, int *count, int is_inside_parentheses) {

  while (1) {
    int string_result;

    if (is_inside_parentheses == YES) {
      if (compare_and_skip_next_symbol(')') == SUCCEEDED)
        break;
    }

    if (g_is_file_isolated_counter > 0)
      g_force_ignore_namespace = YES;
    string_result = input_next_string();
    if (g_is_file_isolated_counter > 0)
      g_force_ignore_namespace = NO;
    
    if (string_result == FAILED)
      return FAILED;
    if (string_result == INPUT_NUMBER_EOL) {
      if (*count != 0) {
        next_line();
        break;
      }
      print_error(ERROR_DIR, "MACRO \"%s\" is missing argument names?\n", m->name);
      return FAILED;
    }
    
    (*count)++;

    /* store the label */
    m->argument_names = realloc(m->argument_names, sizeof(char *)*(*count));
    if (m->argument_names == NULL) {
      print_error(ERROR_NONE, "Out of memory error.\n");
      return FAILED;
    }
    m->argument_names[*count-1] = calloc(strlen(g_label)+1, 1);
    if (m->argument_names[*count-1] == NULL) {
      print_error(ERROR_NONE, "Out of memory error.\n");
      return FAILED;
    }

    strcpy(m->argument_names[*count-1], g_label);
  }

  return SUCCEEDED;
}


int directive_macro(void) {

  struct macro_static *m;
  int macro_start_line, q = 0;

  if (s_dstruct_status == ON) {
    print_error(ERROR_DIR, "You can't define a macro inside .DSTRUCT.\n");
    return FAILED;
  }

  g_force_add_namespace = YES;
  if (get_next_plain_string() == FAILED)
    return FAILED;
  g_force_add_namespace = NO;
  
  macro_start_line = g_active_file_info_last->line_current;

  if (macro_get(g_label, NO, &m) == FAILED)
    return FAILED;
  
  if (m != NULL) {
    print_error(ERROR_DIR, ".MACRO \"%s\" was defined for the second time.\n", g_label);
    return FAILED;
  }

  m = calloc(sizeof(struct macro_static), 1);
  if (m == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating room for a new .MACRO.\n");
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

  strcpy(m->name, g_label);
  m->next = NULL;
  m->calls = 0;
  m->filename_id = g_active_file_info_last->filename_id;
  m->argument_names = NULL;
  m->isolated_local = NO;
  m->isolated_unnamed = NO;
  m->id = g_macro_id++;

  if (g_is_file_isolated_counter > 0) {
    /* store the namespace so we'll later know in which namespace the .MACRO was defined in */
    strcpy(m->namespace, g_active_file_info_last->namespace);
  }
  else
    m->namespace[0] = 0;

  strcpy(m->defined_namespace, g_active_file_info_last->namespace);

  /* skip '(' if it exists */
  if (compare_and_skip_next_symbol('(') == SUCCEEDED) {
    if (compare_and_skip_next_symbol(')') == FAILED) {
      if (_parse_macro_argument_names(m, &q, YES) == FAILED)
        return FAILED;
    }
  }

  while (1) {
    int got_some = NO;
    
    /* is ISOLATED defined? */
    if (compare_next_token("ISOLATED") == SUCCEEDED) {
      skip_next_token();
      got_some = YES;
      m->isolated_local = YES;
      m->isolated_unnamed = YES;
    }

    /* is ISOLATELOCAL defined? */
    if (compare_next_token("ISOLATELOCAL") == SUCCEEDED) {
      skip_next_token();
      got_some = YES;
      m->isolated_local = YES;
    }

    /* is ISOLATEUNNAMED defined? */
    if (compare_next_token("ISOLATEUNNAMED") == SUCCEEDED) {
      skip_next_token();
      got_some = YES;
      m->isolated_unnamed = YES;
    }

    if (got_some == NO)
      break;
  }

  /* is ARGS defined? */
  if (compare_next_token("ARGS") == SUCCEEDED) {
    skip_next_token();

    q = 0;
    if (_parse_macro_argument_names(m, &q, NO) == FAILED)
      return FAILED;
  }

  m->nargument_names = q;
  m->start = g_source_index;
  m->start_line = g_active_file_info_last->line_current;

  /* go to the end of the macro */
  for (; g_source_index < g_source_file_size; g_source_index++) {
    if (g_buffer[g_source_index] == 0x0A) {
      next_line();
      continue;
    }
    else if ((strncmp(&g_buffer[g_source_index], ".E", 2) == 0) && (g_buffer[g_source_index + 2] == 0x0A || g_buffer[g_source_index + 2] == ' ')) {
      g_active_file_info_last->line_current = macro_start_line;
      print_error(ERROR_DIR, "MACRO \"%s\" wasn't terminated with .ENDM.\n", m->name);
      return FAILED;
    }
    else if ((strncmp(&g_buffer[g_source_index], ".ENDM", 5) == 0 || strncmp(&g_buffer[g_source_index], ".endm", 5) == 0) && (g_buffer[g_source_index + 5] == 0x0A || g_buffer[g_source_index + 5] == ' ')) {
      g_source_index += 5;
      break;
    }
  }

  return SUCCEEDED;
}


static int _find_next_endr(void) {

  int r, m;

  r = 1;
  m = g_macro_active;

  /* disable macro decoding */
  g_macro_active = 0;

  while (get_next_token() != FAILED) {
    if (g_tmp[0] == '.') {
      if (strcaselesscmp(g_current_directive, "ENDR") == 0) {
        r--;
        if (r == 0) {
          g_macro_active = m;
          return SUCCEEDED;
        }
      }
      if (strcaselesscmp(g_current_directive, "E") == 0)
        break;
      if (strcaselesscmp(g_current_directive, "REPT") == 0 || strcaselesscmp(g_current_directive, "REPEAT") == 0 ||
          strcaselesscmp(g_current_directive, "WHILE") == 0)
        r++;
    }
  }

  return FAILED;
}


int directive_rept_repeat_while(int is_while) {
  
  char c[16], index_name[MAX_NAME_LENGTH + 1];
  int q, start;

  strcpy(c, g_current_directive);

  index_name[0] = 0;

  if (is_while == NO) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_INP, ".%s needs a count.\n", c);
      return FAILED;
    }

    if (g_parsed_int < 0) {
      print_error(ERROR_DIR, ".%s count value must be positive or zero.\n", c);
      return FAILED;
    }

    if (compare_next_token("INDEX") == SUCCEEDED) {
      skip_next_token();

      if (input_next_string() != SUCCEEDED)
        return FAILED;

      if (redefine(g_label, 0.0, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
        return FAILED;

      strcpy(index_name, g_label);
    }

    start = g_source_index;
  }
  else {
    /* while */
    start = g_source_index;

    g_input_parse_if = YES;
    q = input_number();
    g_input_parse_if = NO;

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_INP, ".%s needs a condition.\n", c);
      return FAILED;
    }
  }

  if (g_parsed_int == 0) {
    int l;

    l = g_active_file_info_last->line_current;

    if (_find_next_endr() == SUCCEEDED)
      return SUCCEEDED;
    
    /* return the condition's line number */
    g_active_file_info_last->line_current = l;
    print_error(ERROR_DIR, ".%s must end to .ENDR.\n", c);
    return FAILED;
  }
  
  if (s_repeat_active == s_repeat_stack_size) {
    struct repeat_runtime *rr;

    s_repeat_stack_size = (s_repeat_stack_size<<1)+2;
    rr = realloc(g_repeat_stack, sizeof(struct repeat_runtime) * s_repeat_stack_size);
    if (rr == NULL) {
      print_error(ERROR_ERR, "Out of memory error while enlarging repeat stack buffer.\n");
      return FAILED;
    }
    g_repeat_stack = rr;
  }

  g_repeat_stack[s_repeat_active].start = start;
  g_repeat_stack[s_repeat_active].counter = g_parsed_int;
  g_repeat_stack[s_repeat_active].repeats = 0;
  g_repeat_stack[s_repeat_active].start_line = g_active_file_info_last->line_current;
  g_repeat_stack[s_repeat_active].start_ifdef = g_ifdef;
  g_repeat_stack[s_repeat_active].is_while = is_while;
  strcpy(g_repeat_stack[s_repeat_active].index_name, index_name);

  s_repeat_active++;

  /* repeat start */
  fprintf(g_file_out_ptr, "j ");
      
  return SUCCEEDED;
}


int directive_endm(void) {

  struct definition_storage *storage;
  int q;
  
  if (g_macro_active != 0) {
    g_macro_active--;

    /* macro call end */
    fprintf(g_file_out_ptr, "I%d %s ", g_macro_stack[g_macro_active].macro->id, g_macro_stack[g_macro_active].macro->name);
    
    /* free the arguments */
    if (g_macro_stack[g_macro_active].supplied_arguments > 0) {
      for (q = 0; q < g_macro_stack[g_macro_active].supplied_arguments; q++)
        free(g_macro_stack[g_macro_active].argument_data[q]);
      free(g_macro_stack[g_macro_active].argument_data);
      g_macro_stack[g_macro_active].argument_data = NULL;
    }

    /* free the argument definitions */
    for (q = 0; q < g_macro_stack[g_macro_active].macro->nargument_names; q++) {
      char argument_name[MAX_NAME_LENGTH + 1];

      if (g_macro_stack[g_macro_active].macro->namespace[0] == 0)
        strcpy(argument_name, g_macro_stack[g_macro_active].macro->argument_names[q]);
      else
        snprintf(argument_name, sizeof(argument_name), "%s.%s", g_macro_stack[g_macro_active].macro->namespace,
                 g_macro_stack[g_macro_active].macro->argument_names[q]);

      undefine(argument_name);
    }

    /* return back stored definitions */
    storage = g_macro_stack[g_macro_active].definition_storage;
    while (storage != NULL) {
      struct definition_storage *ds = storage->next;
      
      if ((q = hashmap_put(g_defines_map, storage->definition->alias, storage->definition)) != MAP_OK) {
        print_text(NO, "DIRECTIVE_ENDM: Hashmap error %d\n", q);
        return FAILED;
      }

      free(storage);

      storage = ds;
    }
    g_macro_stack[g_macro_active].definition_storage = NULL;
      
    g_source_index = g_macro_stack[g_macro_active].macro_return_i;

    /* we return to the beginning of a new line? */
    if (g_buffer[g_source_index - 1] == 0xA)
      g_newline_beginning = ON;

    if ((g_extra_definitions == ON) && (g_active_file_info_last->filename_id != g_macro_stack[g_macro_active].macro_return_filename_id)) {
      redefine("WLA_FILENAME", 0.0, get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id)));
      redefine("wla_filename", 0.0, get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(g_macro_stack[g_macro_active].macro_return_filename_id)));
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
      if (_macro_insert_bytes("DBM", 1) == FAILED)
        return FAILED;

      /* continue defining bytes */
      if (_macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DBM, "DBM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DWM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DWM) {
      /* yep, get the output */
      if (_macro_insert_word_db("DWM") == FAILED)
        return FAILED;

      /* continue defining words */
      if (_macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DWM, "DWM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DLM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DLM) {
      /* yep, get the output */
      if (_macro_insert_long_db("DLM") == FAILED)
        return FAILED;

      /* continue defining longs */
      if (_macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DLM, "DLM", NO) == FAILED)
        return FAILED;
    }
    /* was this a DDM macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_DDM) {
      /* yep, get the output */
      if (_macro_insert_double_dw("DDM") == FAILED)
        return FAILED;

      /* continue defining double words */
      if (_macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_DDM, "DDM", NO) == FAILED)
        return FAILED;
    }
    /* was this a FILTER macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_FILTER) {
      /* continue running FILTER */
      if (_macro_start_dxm(g_macro_stack[g_macro_active].macro, MACRO_CALLER_FILTER, "FILTER", NO) == FAILED)
        return FAILED;
    }
    /* or was this an INCBIN with a filter macro call? */
    else if (g_macro_stack[g_macro_active].caller == MACRO_CALLER_INCBIN) {
      /* yep, get the output */
      if (_macro_insert_bytes("INCBIN", g_macro_stack[g_macro_active].incbin_data->filter_size) == FAILED)
        return FAILED;

      /* continue filtering the binary file */
      if (_macro_start_incbin(g_macro_stack[g_macro_active].macro, NULL, NO) == FAILED)
        return FAILED;
    }

    return SUCCEEDED;
  }

  print_error(ERROR_DIR, "No .MACRO open.\n");

  return FAILED;
}


#if defined(W65816)

int directive_snesheader(void) {

  int token_result;
  
  if (s_snesheader_defined != 0) {
    print_error(ERROR_DIR, ".SNESHEADER can be defined only once.\n");
    return FAILED;
  }

  if (g_computesneschecksum_defined != 0)
    print_error(ERROR_WRN, ".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER is defined.\n");
  else
    g_computesneschecksum_defined++;

  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .SNESHEADER.\n");
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
        print_error(ERROR_DIR, "ID requires a string of 1 to 4 letters.\n");
        return FAILED;
      }

      /* no ID has been defined so far */
      if (g_snesid_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 4; i++)
          g_snesid[i] = g_tmp[i];

        for (; i < 4; i++)
          g_snesid[i] = 0;

        g_snesid_defined = 1;
      }
      /* compare the IDs */
      else {
        int i;

        for (i = 0; g_tmp[i] != 0 && g_snesid[i] != 0 && i < 4; i++)
          if (g_snesid[i] != g_tmp[i])
            break;

        if (i == 4 && g_tmp[i] != 0) {
          print_error(ERROR_DIR, "ID requires a string of 1 to 4 letters.\n");
          return FAILED;
        }
        if (i != 4 && (g_snesid[i] != 0 || g_tmp[i] != 0)) {
          print_error(ERROR_DIR, "ID was already defined.\n");
          return FAILED;
        }
      }
    }    
    else if (strcaselesscmp(g_tmp, "NAME") == 0) {
      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error(ERROR_DIR, "NAME requires a string of 1 to 21 letters.\n");
        return FAILED;
      }

      /* no name has been defined so far */
      if (g_name_defined == 0) {
        int i;

        for (i = 0; g_tmp[i] != 0 && i < 21; i++)
          g_name[i] = g_tmp[i];

        if (i == 21 && g_tmp[i] != 0) {
          print_error(ERROR_DIR, "NAME requires a string of 1 to 21 letters.\n");
          return FAILED;
        }

        for (; i < 21; i++)
          g_name[i] = 0;

        g_name_defined = 1;
      }
      /* compare the names */
      else {
        int i;

        for (i = 0; g_tmp[i] != 0 && g_name[i] != 0 && i < 21; i++)
          if (g_name[i] != g_tmp[i])
            break;

        if (i == 21 && g_tmp[i] != 0) {
          print_error(ERROR_DIR, "NAME requires a string of 1 to 21 letters.\n");
          return FAILED;
        }
        if (i != 21 && (g_name[i] != 0 || g_tmp[i] != 0)) {
          print_error(ERROR_DIR, "NAME was already defined.\n");
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
        print_error(ERROR_DIR, ".FASTROM was defined prior to .SLOWROM.\n");
        return FAILED;
      }

      g_slowrom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "FASTROM") == 0) {
      if (g_slowrom_defined != 0) {
        print_error(ERROR_DIR, ".SLOWROM was defined prior to .FASTROM.\n");
        return FAILED;
      }

      g_fastrom_defined++;
    }
    else if (strcaselesscmp(g_tmp, "CARTRIDGETYPE") == 0) {
      int number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "CARTRIDGETYPE expects 8-bit data, %d is out of range!\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_cartridgetype_defined != 0 && g_parsed_int != g_cartridgetype) {
          print_error(ERROR_DIR, "CARTRIDGETYPE was defined for the second time.\n");
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
        print_error(ERROR_DIR, "ROMSIZE can be defined only once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "ROMSIZE expects 8-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "SRAMSIZE can be defined only once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < 0 || g_parsed_int > 7)) {
        print_error(ERROR_DIR, "SRAMSIZE expects 0-7, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "COUNTRY can be defined only once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "COUNTRY expects 8-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "LICENSEECODE can be defined only once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -128 || g_parsed_int > 255)) {
        print_error(ERROR_DIR, "LICENSEECODE expects 8-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "VERSION expects 8-bit data, %d is out of range!\n", g_parsed_int);
        return FAILED;
      }
      else if (number_result == SUCCEEDED) {
        if (g_version_defined != 0 && g_version != g_parsed_int) {
          print_error(ERROR_DIR, "VERSION was defined for the second time.\n");
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
    print_error(ERROR_DIR, "Error in .SNESHEADER data structure.\n");
    return FAILED;
  }

  s_snesheader_defined = 1;
  g_snes_mode++;

  return SUCCEEDED;
}


int directive_snesnativevector(void) {
  
  int cop_defined = 0, brk_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, unused_defined = 0, irq_defined = 0, q, token_result;
  char cop[512], brk[512], abort[512], nmi[512], unused[512], irq[512];

  if (s_snesnativevector_defined != 0) {
    print_error(ERROR_DIR, ".SNESNATIVEVECTOR can be defined only once.\n");
    return FAILED;
  }
  if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
    print_error(ERROR_DIR, ".SNESNATIVEVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n");
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .SNESNATIVEVECTOR.\n");
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
        print_error(ERROR_DIR, "COP can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "COP expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "BRK can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "BRK expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "ABORT can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "ABORT expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "NMI can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "NMI expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "UNUSED can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "UNUSED expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "IRQ can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "IRQ expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
    print_error(ERROR_DIR, "Error in .SNESNATIVEVECTOR data structure.\n");
    return FAILED;
  }

  s_snesnativevector_defined = 1;
  g_snes_mode++;

  return SUCCEEDED;
}


int directive_snesemuvector(void) {
  
  int cop_defined = 0, unused_defined = 0, abort_defined = 0, base_address = 0;
  int nmi_defined = 0, reset_defined = 0, irqbrk_defined = 0, q, token_result;
  char cop[512], unused[512], abort[512], nmi[512], reset[512], irqbrk[512];

  if (s_snesemuvector_defined != 0) {
    print_error(ERROR_DIR, ".SNESEMUVECTOR can be defined only once.\n");
    return FAILED;
  }
  if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
    print_error(ERROR_DIR, ".SNESEMUVECTOR needs .LOROM, .HIROM or .EXHIROM defined earlier.\n");
    return FAILED;
  }
  if (g_output_format == OUTPUT_LIBRARY) {
    print_error(ERROR_DIR, "Libraries don't take .SNESEMUVECTOR.\n");
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
        print_error(ERROR_DIR, "COP can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "COP expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "RESET can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "RESET expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "ABORT can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "ABORT expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "NMI can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "NMI expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "UNUSED can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();

      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "UNUSED expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
        print_error(ERROR_DIR, "IRQBRK can only be defined once.\n");
        return FAILED;
      }

      number_result = input_number();
      
      if (number_result == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
        print_error(ERROR_DIR, "IRQBRK expects 16-bit data, %d is out of range!\n", g_parsed_int);
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
    print_error(ERROR_DIR, "Error in .SNESEMUVECTOR data structure.\n");
    return FAILED;
  }

  s_snesemuvector_defined = 1;
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
        print_error(ERROR_INP, ".PRINT was expecting a value, got a string/label instead.\n");
        return FAILED;
      }

      if (g_quiet == NO) {
        print_text(YES, "%s", g_label);
        fflush(stdout);
      }
    }
    else if (number_result == SUCCEEDED) {
      if (g_quiet == NO) {
        if (value_type == 0)
          print_text(YES, "%x", g_parsed_int);
        else
          print_text(YES, "%d", g_parsed_int);
        fflush(stdout);
      }
    }
    else if (number_result == INPUT_NUMBER_EOL) {
      next_line();
      break;
    }
    else {
      print_error(ERROR_DIR, ".PRINT needs a string/label or (an optional) HEX/DEC plus a value.\n");
      return FAILED;
    }
  }

  return SUCCEEDED;
}


int directive_printt(void) {
  
  int number_result;
    
  number_result = input_number();

  if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".PRINTT needs a string/label.\n");
    return FAILED;
  }

  if (g_quiet == NO) {
    print_text(YES, "%s", g_label);
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
      print_error(ERROR_DIR, "\"%s\" is not known.\n", g_label);
    }
    print_error(ERROR_DIR, ".PRINTV can only print currently known values.\n");
    return FAILED;
  }

  if (g_quiet == NO) {
    if (m == 0)
      print_text(YES, "%x", g_parsed_int);
    else
      print_text(YES, "%d", g_parsed_int);
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

  if (compare_next_token("COUNT") == SUCCEEDED)
    skip_next_token();

  /* get the count */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(ERROR_DIR, ".%s needs the number of random numbers.\n", g_current_directive);
    return FAILED;
  }

  c = g_parsed_int;

  if (c <= 0) {
    print_error(ERROR_DIR, ".%s needs that the number of random numbers is > 0.\n", g_current_directive);
    return FAILED;
  }

  if (compare_next_token("MIN") == SUCCEEDED)
    skip_next_token();
  
  /* get min */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(ERROR_DIR, ".%s needs the min value.\n", g_current_directive);
    return FAILED;
  }

  min = g_parsed_int;

  if (compare_next_token("MAX") == SUCCEEDED)
    skip_next_token();
  
  /* get max */
  q = input_number();
  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(ERROR_DIR, ".%s needs the max value.\n", g_current_directive);
    return FAILED;
  }

  max = g_parsed_int;

  if (min >= max) {
    print_error(ERROR_DIR, ".%s needs that min < max.\n", g_current_directive);
    return FAILED;
  }

  /* generate the numbers */
  for (f = 0; f < c; f++) {
    q = (genrand_int32() % (max-min+1)) + min;
    
    if (o == 1) {
      if (q < -32768 || q > 65535) {
        print_error(ERROR_NONE, ".%s: Expected a 16-bit value, computed %d.\n", g_current_directive, q);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "y %d ", q);
    }
    else {
      if (q > 255 || q < -128) {
        print_error(ERROR_NONE, ".%s: Expected a 8-bit value, computed %d.\n", g_current_directive, q);
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

  /* skip DATA if present */
  if (compare_next_token("ANGLE") == SUCCEEDED)
    skip_next_token();
  
  g_input_float_mode = ON;
  p = input_number();
  g_input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    print_error(ERROR_DIR, ".%s needs a value for starting angle.\n", g_current_directive);
    return FAILED;
  }

  if (p == SUCCEEDED)
    a = g_parsed_int;
  else
    a = g_parsed_double;

  if (input_number() != SUCCEEDED || g_parsed_int < 0) {
    print_error(ERROR_DIR, ".%s needs an non-negative integer value for additional angles.\n", g_current_directive);
    return FAILED;
  }

  c = g_parsed_int;

  g_input_float_mode = ON;
  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    print_error(ERROR_DIR, ".%s needs a value for angle step.\n", g_current_directive);
    return FAILED;
  }

  if (p == SUCCEEDED)
    s = g_parsed_int;
  else
    s = g_parsed_double;

  p = input_number();
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    print_error(ERROR_DIR, ".%s needs a value to multiply the result with.\n", g_current_directive);
    return FAILED;
  }

  if (p == SUCCEEDED)
    m = g_parsed_int;
  else
    m = g_parsed_double;

  p = input_number();
  g_input_float_mode = OFF;
  if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
    print_error(ERROR_DIR, ".%s needs a value to add to the result.\n", g_current_directive);
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
        print_error(ERROR_NONE, ".%s: Expected a 16-bit value, computed %d.\n", g_current_directive, value);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "y %d ", value);
    }
    else {
      if (value > 255 || value < -128) {
        print_error(ERROR_NONE, ".%s: Expected a 8-bit value, computed %d.\n", g_current_directive, value);
        return FAILED;
      }
      fprintf(g_file_out_ptr, "d%d ", value);
    }

    a += s;
  }

  return SUCCEEDED;
}


int directive_stringmaptable(void) {

  int parse_result, line_number = 0;
  FILE *table_file;
  char line_buffer[256];
  struct stringmaptable *map;

  g_expect_calculations = NO;
  g_input_parse_special_chars = NO;
  parse_result = input_number();
  g_input_parse_special_chars = YES;
  g_expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".STRINGMAPTABLE needs a name string.\n");
    return FAILED;
  }

  /* allocate and insert at the front of the chain */
  map = calloc(sizeof(struct stringmaptable), 1);
  if (map == NULL) {
    print_error(ERROR_ERR, "STRINGMAPTABLE: Out of memory error.\n");
    return FAILED;
  }
  map->next = g_stringmaptables;
  g_stringmaptables = map;

  map->name = _string_duplicate(g_label);
  if (map->name == NULL) {
    print_error(ERROR_ERR, "STRINGMAPTABLE: Out of memory error.\n");
    return FAILED;
  }

  g_expect_calculations = NO;
  parse_result = input_number();
  g_expect_calculations = YES;

  if (parse_result != INPUT_NUMBER_STRING && parse_result != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_DIR, ".STRINGMAPTABLE needs a file name string.\n");
    return FAILED;
  }

  /* apply any include dir and convert the path to local enviroment */
  create_full_name(g_include_dir, g_label);
  localize_path(g_full_name);

  map->filename = _string_duplicate(g_full_name);
  if (map->filename == NULL) {
    print_error(ERROR_ERR, "STRINGMAPTABLE: Out of memory error.\n");
    return FAILED;
  }

  table_file = fopen(map->filename, "r");
  if (table_file == NULL) {
    if (g_makefile_rules == YES) {
      /* if in makefile mode, this is not an error, we just make an empty map */
      return SUCCEEDED;
    }
    print_error(ERROR_DIR, "Error opening file \"%s\".\n", map->filename);
    return FAILED;
  }

  while (fgets(line_buffer, 256, table_file)) {
    char *p = line_buffer, *equals_pos;
    struct stringmap_entry* entry;
    unsigned char *bytes_writer;
    int accumulator = 0, char_count;

    line_number++;

    /* skip comments */
    if (*p == ';' || *p == '#')
      continue;

    equals_pos = strchr(p, '=');

    /* lines should be in the form <hex>=<text> with no whitespace */
    if (equals_pos == NULL)
      continue;

    entry = calloc(sizeof(struct stringmap_entry), 1);
    if (entry == NULL) {
      print_error(ERROR_DIR, "STRINGMAPTABLE: Out of memory error.\n");
      return FAILED;
    }

    /* insert at front of entries list */
    entry->next = map->entries;
    map->entries = entry;

    /* left of = should be a string of hex digits, for a variable whole number of bytes */
    char_count = (int)(equals_pos - p);
    if (char_count == 0) {
      print_error(ERROR_DIR, "STRINGMAPTABLE: No text before '=' at line %d of file \"%s\".\n", line_number, map->filename);
      return FAILED;
    }
    entry->bytes_length = char_count / 2 + char_count % 2;
    entry->bytes = calloc(sizeof(unsigned char), entry->bytes_length);
    if (entry->bytes == NULL) {
      print_error(ERROR_DIR, "STRINGMAPTABLE: Out of memory error.\n");
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
        print_error(ERROR_DIR, "STRINGMAPTABLE: Invalid hex character '%c' at line %d of file \"%s\".\n", c, line_number, map->filename);
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

    /* process special characters like "\n" */
    process_string_for_special_characters(equals_pos + 1, NULL);

    entry->text_length = (int)strlen(++p);
    if (entry->text_length == 0) {
      print_error(ERROR_DIR, "STRINGMAPTABLE: no text after '=' at line %d of file \"%s\".\n", line_number, map->filename);
      return FAILED;
    }
    p = equals_pos + 1;
    entry->text = calloc(sizeof(char), strlen(p) + 1);
    if (entry->text == NULL) {
      print_error(ERROR_DIR, "STRINGMAPTABLE: Out of memory error.\n");
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
    print_error(ERROR_DIR, ".STRINGMAP needs a table name.\n");
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
    print_error(ERROR_DIR, "STRINGMAP: Could not find table called \"%s\".\n", g_label);
    return FAILED;    
  }

  /* parse the string */
  parse_result = input_number();
  if (parse_result != INPUT_NUMBER_STRING) {
    print_error(ERROR_DIR, "STRINGMAP: No string given.\n");
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
      if (g_makefile_rules == YES) {
        /* in makefile mode, it's ignored */
        return SUCCEEDED;
      }
      print_error(ERROR_DIR, "STRINGMAP: Could not find a match in the table at substring \"%s\".\n", p);
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


int directive_assert(void) {

  int q;

  g_input_parse_if = YES;
  q = input_number();
  g_input_parse_if = NO;

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED) {
    print_error(ERROR_INP, ".ASSERT needs immediate data.\n");
    return FAILED;
  }

  /* 0 = false, otherwise it's true */
  if (g_parsed_int != 0)
    return SUCCEEDED;
  else {
    print_error(ERROR_INP, ".ASSERT failed.\n");
    return FAILED;
  }
}


int directive_rombanksize_banksize(void) {

  int q;
  
  q = input_number();

  if (q == FAILED)
    return FAILED;
  if (q != SUCCEEDED || g_parsed_int < 0) {
    print_error(ERROR_DIR, ".ROMBANKSIZE needs a positive integer value.\n");
    return FAILED;
  }

  if (g_banksize_defined != 0) {
    if (g_banksize != g_parsed_int) {
      print_error(ERROR_DIR, ".ROMBANKSIZE was defined for the second time.\n");
      return FAILED;
    }
    return SUCCEEDED;
  }

  g_banksize = g_parsed_int;
  g_banksize_defined = 1;

  return SUCCEEDED;
}


int directive_break(void) {

  int m, line_current = g_active_file_info_last->line_current;
  struct repeat_runtime *rr;
  
  if (s_repeat_active == 0) {
    print_error(ERROR_DIR, "There is no open .REPT/.REPEAT.\n");
    return FAILED;
  }

  rr = &g_repeat_stack[s_repeat_active - 1];

  s_repeat_active--;
          
  /* repeat end */
  fprintf(g_file_out_ptr, "J ");

  if (strlen(rr->index_name) > 0) {
    if (undefine(rr->index_name) == FAILED)
      return FAILED;
  }

  /* find .ENDR */
  m = g_macro_active;

  /* disable macro decoding */
  g_macro_active = 0;

  /* don't use substitution in get_next_token() */
  g_get_next_token_use_substitution = NO;

  while (get_next_token() != FAILED) {
    if (g_tmp[0] == '.') {
      if (strcaselesscmp(g_current_directive, "E") == 0)
        break;
      if (strcaselesscmp(g_current_directive, "ENDR") == 0) {
        g_macro_active = m;
        g_get_next_token_use_substitution = YES;
        g_ifdef = rr->start_ifdef;
        return SUCCEEDED;  
      }
    }
  }
  
  /* return the condition's line number */
  g_active_file_info_last->line_current = line_current;
  print_error(ERROR_DIR, ".REPEAT/.REPT must end to .ENDR.\n");

  return FAILED;
}


int directive_endr_continue(void) {

  struct repeat_runtime *rr;
  
  if (s_repeat_active == 0) {
    print_error(ERROR_DIR, "There is no open .REPT/.REPEAT.\n");
    return FAILED;
  }

  rr = &g_repeat_stack[s_repeat_active - 1];

  if (rr->is_while == YES) {
    int q;
    
    /* re-evaluate the condition for .WHILE */
    g_source_index = rr->start;
    g_active_file_info_last->line_current = rr->start_line;
    g_ifdef = rr->start_ifdef;

    g_input_parse_if = YES;
    q = input_number();
    g_input_parse_if = NO;

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_INP, ".WHILE needs a condition.\n");
      return FAILED;
    }

    if (g_parsed_int == 0) {
      int l;

      s_repeat_active--;
      l = g_active_file_info_last->line_current;

      /* repeat end */
      fprintf(g_file_out_ptr, "J ");
      
      if (_find_next_endr() == SUCCEEDED)
        return SUCCEEDED;
    
      /* return the condition's line number */
      g_active_file_info_last->line_current = l;
      print_error(ERROR_DIR, ".WHILE must end to .ENDR.\n");
      return FAILED;
    }
  }
  else {
    rr->counter--;
    if (rr->counter == 0) {
      s_repeat_active--;
          
      /* repeat end */
      fprintf(g_file_out_ptr, "J ");

      if (strlen(rr->index_name) > 0) {
        if (undefine(rr->index_name) == FAILED)
          return FAILED;
      }
      return SUCCEEDED;
    }
  }

  rr->repeats++;
  if (strlen(rr->index_name) > 0) {
    if (redefine(rr->index_name, (double)rr->repeats, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;
  }

  if (rr->is_while == NO) {
    g_source_index = rr->start;
    g_active_file_info_last->line_current = rr->start_line;
    g_ifdef = rr->start_ifdef;
  }
  
  return SUCCEEDED;
}


int parse_directive(void) {

  char c, directive_upper[MAX_NAME_LENGTH + 1];
  int q, i, length;

  if ((q = parse_if_directive()) != -1)
    return q;

  /* make valgrind happy */
  directive_upper[0] = 0;
  
  /* convert the directive to upper case */
  length = (int)strlen(g_current_directive);
  for (i = 0; i < length; i++)
    directive_upper[i] = toupper(g_current_directive[i]);
  directive_upper[i] = 0;

  c = directive_upper[0];

  switch (c) {
  case '1':

#if defined(MCS6502) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    /* 16BIT */
    if (strcmp(directive_upper, "16BIT") == 0) {
      g_xbit_size = 16;
      return SUCCEEDED;
    }
#endif

    break;

  case '2':

#if defined(W65816)
    /* 24BIT */
    if (strcmp(directive_upper, "24BIT") == 0) {
      g_xbit_size = 24;
      return SUCCEEDED;
    }
  #endif
  
    break;
    
  case '8':

#if defined(MCS6502) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    /* 8BIT */
    if (strcmp(directive_upper, "8BIT") == 0) {
      g_xbit_size = 8;
      return SUCCEEDED;
    }
#endif

    break;
    
  case 'A':

#if defined(W65816)
    /* ACCU */
    if (strcmp(directive_upper, "ACCU") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || !(g_parsed_int == 8 || g_parsed_int == 16)) {
        print_error(ERROR_DIR, "The accumulator size must be 8 or 16.\n");
        return FAILED;
      }

      g_accu_size = g_parsed_int;
      
      return SUCCEEDED;
    }
#endif

    /* ALIGN */
    if (strcmp(directive_upper, "ALIGN") == 0)
      return directive_align();
    
    /* ASCTABLE/ASCIITABLE? */
    if (strcmp(directive_upper, "ASCTABLE") == 0 || strcmp(directive_upper, "ASCIITABLE") == 0)
      return directive_asctable_asciitable();
    
    /* ASC/ASCSTR? */
    if (strcmp(directive_upper, "ASC") == 0 || strcmp(directive_upper, "ASCSTR") == 0)
      return directive_asc();

    /* ARRAYDEF/ARRAYDEFINE? */
    if (strcmp(directive_upper, "ARRAYDEF") == 0 || strcmp(directive_upper, "ARRAYDEFINE") == 0)
      return directive_arraydef_arraydefine();

    /* ARRAYIN? */
    if (strcmp(directive_upper, "ARRAYIN") == 0)
      return directive_arrayin();

    /* ARRAYOUT? */
    if (strcmp(directive_upper, "ARRAYOUT") == 0)
      return directive_arrayout();

    /* ARRAYDB/ARRAYDW/ARRAYDL/ARRAYDD? */
    if (strcmp(directive_upper, "ARRAYDB") == 0 || strcmp(directive_upper, "ARRAYDW") == 0 || strcmp(directive_upper, "ARRAYDL") == 0 || strcmp(directive_upper, "ARRAYDD") == 0)
      return directive_arraydb_arraydw_arraydl_arraydd();

    /* ADDR? */
    if (strcmp(directive_upper, "ADDR") == 0)
      return directive_dw_word_addr();
    
    /* ASM */
    if (strcmp(directive_upper, "ASM") == 0)
      return SUCCEEDED;

    /* ASSERT */
    if (strcmp(directive_upper, "ASSERT") == 0)
      return directive_assert();
    
    break;

  case 'B':

    if (length == 2) {
      /* BR? */
      if (strcmp(directive_upper, "BR") == 0) {
        fprintf(g_file_out_ptr, "Z ");
        return SUCCEEDED;
      }
    }
    else if (length == 3) {
      /* BYT? */
      if (strcmp(directive_upper, "BYT") == 0)
        return directive_db_byt_byte();
    }
    else if (length == 4) {
      /* BASE? */
      if (strcmp(directive_upper, "BASE") == 0) {
        no_library_files(".BASE definitions");

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int < 0) {
          print_error(ERROR_DIR, ".BASE must be zero or positive.\n");
          return FAILED;
        }

        fprintf(g_file_out_ptr, "b%d ", g_parsed_int);

        return SUCCEEDED;
      }

      /* BYTE? */
      if (strcmp(directive_upper, "BYTE") == 0)
        return directive_db_byt_byte();

      /* BANK */
      if (strcmp(directive_upper, "BANK") == 0)
        return directive_bank();

      /* BITS? */
      if (strcmp(directive_upper, "BITS") == 0)
        return directive_bits();
    }
    else {
      /* BREAKPOINT? */
      if (strcmp(directive_upper, "BREAKPOINT") == 0) {
        fprintf(g_file_out_ptr, "Z ");
        return SUCCEEDED;
      }

      /* BREAK */
      if (strcmp(directive_upper, "BREAK") == 0)
        return directive_break();

      /* BLOCK */
      if (strcmp(directive_upper, "BLOCK") == 0)
        return directive_block();
      
      /* BACKGROUND */
      if (strcmp(directive_upper, "BACKGROUND") == 0)
        return directive_background();

      /* BANKSIZE */
      if (strcmp(directive_upper, "BANKSIZE") == 0)
        return directive_rombanksize_banksize();
    }
    
    break;

  case 'C':

#if defined(GB)
    /* COMPUTECHECKSUM/COMPUTEGBCHECKSUM */
    if (strcmp(directive_upper, "COMPUTECHECKSUM") == 0 || strcmp(directive_upper, "COMPUTEGBCHECKSUM") == 0) {
      no_library_files(".COMPUTEGBCHECKSUM");
    
      if (s_gbheader_defined != 0)
        print_error(ERROR_WRN, ".COMPUTEGBCHECKSUM is unnecessary when GBHEADER is defined.\n");

      g_computechecksum_defined = 1;

      return SUCCEEDED;
    }

    /* COMPUTECOMPLEMENTCHECK/COMPUTEGBCOMPLEMENTCHECK */
    if (strcmp(directive_upper, "COMPUTEGBCOMPLEMENTCHECK") == 0 || strcmp(directive_upper, "COMPUTECOMPLEMENTCHECK") == 0) {
      no_library_files(".COMPUTEGBCOMPLEMENTCHECK");
    
      if (s_gbheader_defined != 0)
        print_error(ERROR_WRN, ".COMPUTEGBCOMPLEMENTCHECK is unnecessary when GBHEADER is defined.\n");

      g_computecomplementcheck_defined = 1;

      return SUCCEEDED;
    }

    /* COUNTRYCODE */
    if (strcmp(directive_upper, "COUNTRYCODE") == 0) {
      no_library_files(".COUNTRYCODE");

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || q < 0) {
        print_error(ERROR_DIR, ".COUNTRYCODE needs a non-negative value.\n");
        return FAILED;
      }

      if (g_countrycode_defined != 0) {
        if (g_countrycode != g_parsed_int) {
          print_error(ERROR_DIR, ".COUNTRYCODE was defined for the second time.\n");
          return FAILED;
        }
      }

      g_countrycode = g_parsed_int;
      g_countrycode_defined = 1;

      return SUCCEEDED;
    }
    
    /* CARTRIDGETYPE */
    if (strcmp(directive_upper, "CARTRIDGETYPE") == 0) {
      no_library_files(".CARTRIDGETYPE");

      q = input_number();

      if (q == SUCCEEDED && g_cartridgetype_defined != 0) {
        if (g_cartridgetype != g_parsed_int) {
          print_error(ERROR_DIR, ".CARTRIDGETYPE was defined for the second time.\n");
          return FAILED;
        }
      }

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, ".CARTRIDGETYPE needs an immediate value.\n");
        return FAILED;
      }

      g_cartridgetype = g_parsed_int;
      g_cartridgetype_defined = 1;

      return SUCCEEDED;
    }
#endif
    
#if defined(W65816)
    /* COMPUTESNESCHECKSUM */
    if (strcmp(directive_upper, "COMPUTESNESCHECKSUM") == 0) {
      no_library_files(".COMPUTESNESCHECKSUM");
    
      if (g_hirom_defined == 0 && g_lorom_defined == 0 && g_exhirom_defined == 0 && g_exlorom_defined == 0) {
        print_error(ERROR_DIR, ".COMPUTESNESCHECKSUM needs .LOROM, .HIROM or .EXHIROM defined earlier.\n");
        return FAILED;
      }
      if (s_snesheader_defined != 0) 
        print_error(ERROR_WRN, ".COMPUTESNESCHECKSUM is unnecessary when .SNESHEADER defined.\n");

      g_computesneschecksum_defined = 1;

      return SUCCEEDED;
    }
#endif

#if defined(MC68000)
    /* COMPUTESMDCHECKSUM */
    if (strcmp(directive_upper, "COMPUTESMDCHECKSUM") == 0) {
      no_library_files(".COMPUTESMDCHECKSUM");

      g_computesmdchecksum_defined++;

      return SUCCEEDED;
    }
#endif
    
#if defined(Z80)
    /* COMPUTESMSCHECKSUM */
    if (strcmp(directive_upper, "COMPUTESMSCHECKSUM") == 0) {
      no_library_files(".COMPUTESMSCHECKSUM");

      g_computesmschecksum_defined++;

      return SUCCEEDED;
    }
#endif
    
    /* CHANGEFILE (INTERNAL) */
    if (strcmp(directive_upper, "CHANGEFILE") == 0) {
      q = input_number();
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, "Internal error in (internal) .CHANGEFILE. Please submit a bug report...\n");
        return FAILED;
      }
    
      g_active_file_info_tmp = calloc(sizeof(struct active_file_info), 1);
      if (g_active_file_info_tmp == NULL) {
        print_error(ERROR_DIR, "Out of memory while trying allocate error tracking data structure.\n");
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
          print_error(ERROR_DIR, "Internal error: Could not find the name of file %d.\n", g_parsed_int);
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
          print_error(ERROR_DIR, "Internal error: Namespace string is missing.\n");
          return FAILED;
        }

        if (_is_namespace_valid(g_label) == NO)
          return FAILED;

        strcpy(g_active_file_info_tmp->namespace, g_label);

        fprintf(g_file_out_ptr, "t1 %s ", g_active_file_info_tmp->namespace);
      }
      else if (compare_next_token("NONAMESPACE") == SUCCEEDED) {
        skip_next_token();
      
        g_active_file_info_tmp->namespace[0] = 0;

        fprintf(g_file_out_ptr, "t0 ");
      }
      else {
        print_error(ERROR_DIR, "Internal error: NAMESPACE/NONAMESPACE is missing.\n");
        return FAILED;
      }

      /* get the isolation counter */
      g_expect_calculations = NO;
      input_number();
      g_expect_calculations = YES;
      
      g_is_file_isolated_counter = g_parsed_int;
      
      return SUCCEEDED;
    }
    
    /* CONTINUE */
    if (strcmp(directive_upper, "CONTINUE") == 0)
      return directive_endr_continue();

    break;
    
  case 'D':

    if (length == 2) {
      /* DB? */
      if (strcmp(directive_upper, "DB") == 0)
        return directive_db_byt_byte();

      /* DW? */
      if (strcmp(directive_upper, "DW") == 0)
        return directive_dw_word_addr();

      /* DD? */
      if (strcmp(directive_upper, "DD") == 0)
        return directive_dd();

      /* DL? */
      if (strcmp(directive_upper, "DL") == 0)
        return directive_dl_long_faraddr();

      /* DS? */
      if (strcmp(directive_upper, "DS") == 0)
        return directive_dsb_ds();
    }
    else if (length == 3) {
      /* DEF */
      if (strcmp(directive_upper, "DEF") == 0)
        return directive_define_def_equ();

      /* DSB? */
      if (strcmp(directive_upper, "DSB") == 0)
        return directive_dsb_ds();

      /* DSW? */
      if (strcmp(directive_upper, "DSW") == 0)
        return directive_dsw();

      /* DSL? */
      if (strcmp(directive_upper, "DSL") == 0)
        return directive_dsl();

      /* DSD? */
      if (strcmp(directive_upper, "DSD") == 0)
        return directive_dsd();

      /* DBM/DWM? */
      if (strcmp(directive_upper, "DBM") == 0 || strcmp(directive_upper, "DWM") == 0)
        return directive_dbm_dwm_dlm_ddm_filter();
      
      /* DLM? */
      if (strcmp(directive_upper, "DLM") == 0)
        return directive_dbm_dwm_dlm_ddm_filter();

      /* DDM? */
      if (strcmp(directive_upper, "DDM") == 0)
        return directive_dbm_dwm_dlm_ddm_filter();
    }
    else {
      /* DEFINE */
      if (strcmp(directive_upper, "DEFINE") == 0)
        return directive_define_def_equ();

      /* DATA? */
      if (strcmp(directive_upper, "DATA") == 0)
        return directive_row_data();

      /* DSTRUCT */
      if (strcmp(directive_upper, "DSTRUCT") == 0)
        return directive_dstruct();

      /* DBRND/DWRND */
      if (strcmp(directive_upper, "DBRND") == 0 || strcmp(directive_upper, "DWRND") == 0)
        return directive_dbrnd_dwrnd();

      /* DWSIN/DBSIN/DWCOS/DBCOS */
      if (strcmp(directive_upper, "DWSIN") == 0 || strcmp(directive_upper, "DBSIN") == 0 || strcmp(directive_upper, "DWCOS") == 0 || strcmp(directive_upper, "DBCOS") == 0)
        return directive_dwsin_dbsin_dwcos_dbcos();
    
#if defined(GB)
      /* DESTINATIONCODE */
      if (strcmp(directive_upper, "DESTINATIONCODE") == 0) {
        no_library_files(".DESTINATIONCODE");

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || q < 0) {
          print_error(ERROR_DIR, ".DESTINATIONCODE needs a non-negative value.\n");
          return FAILED;
        }

        if (q == SUCCEEDED && g_countrycode_defined != 0) {
          if (g_countrycode != g_parsed_int) {
            print_error(ERROR_DIR, ".DESTINATIONCODE was defined for the second time.\n");
            return FAILED;
          }
        }

        g_countrycode = g_parsed_int;
        g_countrycode_defined = 1;

        return SUCCEEDED;
      }
#endif
    }
  
    break;

  case 'E':

    if (length == 1) {    
      /* E (INTERNAL) */
      if (strcmp(directive_upper, "E") == 0) {
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
          redefine("WLA_FILENAME", 0.0, get_file_name(g_active_file_info_last->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(g_active_file_info_last->filename_id)));
          redefine("wla_filename", 0.0, get_file_name(g_active_file_info_last->filename_id), DEFINITION_TYPE_STRING, (int)strlen(get_file_name(g_active_file_info_last->filename_id)));
        }

        if (g_is_file_isolated_counter > 1)
          g_is_file_isolated_counter--;
        if (g_is_file_isolated_counter == 1)
          g_is_file_isolated_counter = 0;
          
        return SUCCEEDED;
      }
    }
    else if (length == 3) {
      /* EQU */
      if (strcmp(directive_upper, "EQU") == 0)
        return directive_define_def_equ();    
    }
    else if (length == 4) {
      /* ENDS */
      if (strcmp(directive_upper, "ENDS") == 0) {
        if (g_section_status == OFF) {
          print_error(ERROR_DIR, "There is no open section.\n");
          return FAILED;
        }
        if (s_dstruct_status == ON) {
          print_error(ERROR_DIR, "You can't close a section inside .DSTRUCT.\n");
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
      
      /* ENDB */
      if (strcmp(directive_upper, "ENDB") == 0) {
        if (s_block_status <= 0) {
          print_error(ERROR_DIR, "There is no open .BLOCK.\n");
          return FAILED;
        }

        s_block_status--;
        fprintf(g_file_out_ptr, "G ");

        return SUCCEEDED;
      }

      /* ENDM */
      if (strcmp(directive_upper, "ENDM") == 0)
        return directive_endm();
    
      /* ENDR */
      if (strcmp(directive_upper, "ENDR") == 0)
        return directive_endr_continue();

      /* ENUM */
      if (strcmp(directive_upper, "ENUM") == 0) {
        if (s_dstruct_status == ON) {
          print_error(ERROR_DIR, "You can't use start an .ENUM inside .DSTRUCT.\n");
          return FAILED;
        }

        q = input_number();
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          print_error(ERROR_DIR, ".ENUM needs a starting value.\n");
          return FAILED;
        }
      
        s_enum_offset = 0;
        s_last_enum_offset = 0;
        s_max_enum_offset = 0;
        s_base_enum_offset = g_parsed_int;
      
        /* "ASC" or "DESC"? */
        if (compare_next_token("ASC") == SUCCEEDED) {
          s_enum_ord = 1;
          skip_next_token();
        }
        else if (compare_next_token("DESC") == SUCCEEDED) {
          s_enum_ord = -1;
          skip_next_token();
        }
        else
          s_enum_ord = 1;

        /* do we have "EXPORT" defined? */
        if (compare_next_token("EXPORT") == SUCCEEDED) {
          skip_next_token();
          s_enum_export = YES;
        }
        else
          s_enum_export = NO;

        /* setup s_active_struct (enum vars stored here temporarily) */
        s_active_struct = calloc(sizeof(struct structure), 1);
        if (s_active_struct == NULL) {
          print_error(ERROR_DIR, "Out of memory while parsing .ENUM.\n");
          return FAILED;
        }
        s_active_struct->alive = YES;

        if (_remember_new_structure(s_active_struct) == FAILED)
          return FAILED;

        s_active_struct->name[0] = '\0';
        s_active_struct->items = NULL;
        s_active_struct->last_item = NULL;
        s_union_stack = NULL;

        g_in_enum = YES;

        return SUCCEEDED;
      }
    }
    else {    
      /* ENDBITS? */
      if (strcmp(directive_upper, "ENDBITS") == 0) {
        fprintf(g_file_out_ptr, "+999 ");

        return SUCCEEDED;
      }
    
      /* ENDASM */
      if (strcmp(directive_upper, "ENDASM") == 0) {
        int endasm = 1, x;

        while (1) {
          x = g_source_index;
          q = get_next_token();
          if (q == GET_NEXT_TOKEN_STRING)
            continue;

          /* end of file? */
          if (strcmp(g_tmp, ".E") == 0) {
            g_source_index = x;
            if (g_is_file_isolated_counter > 0)
              g_is_file_isolated_counter--;
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

      /* EMPTYFILL */
      if (strcmp(directive_upper, "EMPTYFILL") == 0) {
        no_library_files(".EMPTYFILL");

        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
          print_error(ERROR_DIR, ".EMPTYFILL needs a 8-bit value, got %d.\n", g_parsed_int);
          return FAILED;
        }

        if (g_emptyfill_defined != 0) {
          if (g_emptyfill != g_parsed_int) {
            print_error(ERROR_DIR, ".EMPTYFILL was defined for the second time.\n");
            return FAILED;
          }
          return SUCCEEDED;
        }

        g_emptyfill = g_parsed_int;
        g_emptyfill_defined = 1;
      
        return SUCCEEDED;
      }

      /* ENUMID */
      if (strcmp(directive_upper, "ENUMID") == 0)
        return directive_enumid();

      /* EXPORT */
      if (strcmp(directive_upper, "EXPORT") == 0)
        return directive_export();

#if defined(W65816)  
      /* EXHIROM */
      if (strcmp(directive_upper, "EXHIROM") == 0) {
        no_library_files(".EXHIROM");

        if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_hirom_defined != 0) {
          give_snes_rom_mode_defined_error(".EXHIROM");
          return FAILED;
        }

        g_exhirom_defined++;
        g_snes_mode++;

        return SUCCEEDED;
      }
  
      /* EXLOROM */
      /*
        if (strcmp(directive_upper, "EXLOROM") == 0) {
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
#endif
    }
    
    break;
    
  case 'F':

    /* FUNCTION */
    if (strcmp(directive_upper, "FUNCTION") == 0)
      return directive_function();

    /* FILTER? */
    if (strcmp(directive_upper, "FILTER") == 0)
      return directive_dbm_dwm_dlm_ddm_filter();

    /* FARADDR? */
    if (strcmp(directive_upper, "FARADDR") == 0)
      return directive_dl_long_faraddr();

    /* FOPEN */
    if (strcmp(directive_upper, "FOPEN") == 0)
      return directive_fopen();

    /* FCLOSE */
    if (strcmp(directive_upper, "FCLOSE") == 0)
      return directive_fclose();

    /* FSIZE */
    if (strcmp(directive_upper, "FSIZE") == 0)
      return directive_fsize();

    /* FREAD */
    if (strcmp(directive_upper, "FREAD") == 0)
      return directive_fread();

    /* FTELL */
    if (strcmp(directive_upper, "FTELL") == 0)
      return directive_ftell();

    /* FSEEK */
    if (strcmp(directive_upper, "FSEEK") == 0)
      return directive_fseek();

    /* FAIL */
    if (strcmp(directive_upper, "FAIL") == 0) {
      int exit_value = 1;
    
      q = input_number();
      if (q == INPUT_NUMBER_EOL)
        print_error(ERROR_NONE, "HALT: .FAIL found.\n");
      else if (q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_ADDRESS_LABEL) {
        print_error(ERROR_FAI, "\"%s\"\n", g_label);

        q = input_number();
        if (q == SUCCEEDED)
          exit_value = g_parsed_int;
      }
      else if (q == SUCCEEDED)
        exit_value = g_parsed_int;
      else {
        print_error(ERROR_DIR, ".FAIL takes an optional string, but we got something else here...\n");
        return FAILED;
      }

      /* make a silent exit */
      exit(exit_value);
    }

#if defined(W65816)  
    /* FASTROM */
    if (strcmp(directive_upper, "FASTROM") == 0) {
      no_library_files(".FASTROM");
    
      if (g_slowrom_defined != 0) {
        print_error(ERROR_DIR, ".SLOWROM was defined prior to .FASTROM.\n");
        return FAILED;
      }

      g_fastrom_defined++;
      g_snes_mode++;

      return SUCCEEDED;
    }
#endif
    
    break;

  case 'G':

#if defined(GB)
    /* GBHEADER */
    if (strcmp(g_current_directive, "GBHEADER") == 0)
      return directive_gbheader();
#endif

    break;
    
  case 'H':

    /* HEX? */
    if (strcmp(directive_upper, "HEX") == 0)
      return directive_hex();

#if defined(W65816)  
    /* HIROM */
    if (strcmp(directive_upper, "HIROM") == 0) {
      no_library_files(".HIROM");

      if (g_lorom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".HIROM");
        return FAILED;
      }

      g_hirom_defined++;
      g_snes_mode++;

      return SUCCEEDED;
    }
#endif
    
    break;

  case 'I':

#if defined(W65816)
    /* INDEX */
    if (strcmp(directive_upper, "INDEX") == 0) {
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || !(g_parsed_int == 8 || g_parsed_int == 16)) {
        print_error(ERROR_DIR, "The index size must be 8 or 16.\n");
        return FAILED;
      }

      g_index_size = g_parsed_int;

      return SUCCEEDED;
    }
#endif
    
    /* INCDIR */
    if (strcmp(directive_upper, "INCDIR") == 0)
      return directive_incdir();

    /* INCLUDE/INC */
    if (strcmp(directive_upper, "INCLUDE") == 0 || strcmp(directive_upper, "INC") == 0)
      return directive_include(YES);

    /* INDLUDE/IND (INTERNAL) */
    if (strcmp(directive_upper, "INDLUDE") == 0 || strcmp(directive_upper, "IND") == 0)
      return directive_include(NO);

    /* INCBIN */
    if (strcmp(directive_upper, "INCBIN") == 0)
      return directive_incbin();

    /* INPUT */
    if (strcmp(directive_upper, "INPUT") == 0)
      return directive_input();

    break;
    
  case 'L':

    /* LONG? */
    if (strcmp(directive_upper, "LONG") == 0)
      return directive_dl_long_faraddr();

#if defined(GB)
    /* LICENSEECODENEW */
    if (strcmp(directive_upper, "LICENSEECODENEW") == 0) {
      int token_result;

      no_library_files(".LICENSEECODENEW");
    
      if (g_licenseecodeold_defined != 0) {
        print_error(ERROR_DIR, ".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n");
        return FAILED;
      }

      if ((token_result = get_next_token()) == FAILED)
        return FAILED;

      if (token_result != GET_NEXT_TOKEN_STRING) {
        print_error(ERROR_DIR, ".LICENSEECODENEW requires a string of two letters.\n");
        return FAILED;
      }
      if (!(g_tmp[0] != 0 && g_tmp[1] != 0 && g_tmp[2] == 0)) {
        print_error(ERROR_DIR, ".LICENSEECODENEW requires a string of two letters.\n");
        return FAILED;
      }

      if (g_licenseecodenew_defined != 0) {
        if (g_tmp[0] != g_licenseecodenew_c1 || g_tmp[1] != g_licenseecodenew_c2) {
          print_error(ERROR_DIR, ".LICENSEECODENEW was defined for the second time.\n");
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
    if (strcmp(directive_upper, "LICENSEECODEOLD") == 0) {
      no_library_files(".LICENSEECODEOLD");
      
      if (g_licenseecodenew_defined != 0) {
        print_error(ERROR_DIR, ".LICENSEECODENEW and .LICENSEECODEOLD cannot both be defined.\n");
        return FAILED;
      }

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        print_error(ERROR_DIR, ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_licenseecodeold_defined != 0) {
        if (g_licenseecodeold != g_parsed_int) {
          print_error(ERROR_DIR, ".LICENSEECODEOLD was defined for the second time.\n");
          return FAILED;
        }
        return SUCCEEDED;
      }

      g_licenseecodeold = g_parsed_int;
      g_licenseecodeold_defined = 1;

      return SUCCEEDED;
    }
#endif

#if defined(W65816)  
    /* LOROM */
    if (strcmp(directive_upper, "LOROM") == 0) {
      no_library_files(".LOROM");

      if (g_hirom_defined != 0 || g_exlorom_defined != 0 || g_exhirom_defined != 0) {
        give_snes_rom_mode_defined_error(".LOROM");
        return FAILED;
      }

      g_lorom_defined++;
      g_snes_mode++;

      return SUCCEEDED;
    }
#endif
    
    break;

  case 'M':

    /* MACRO */
    if (strcmp(directive_upper, "MACRO") == 0)
      return directive_macro();

    /* M */
    if (strcmp(directive_upper, "M") == 0) {
      if (s_line_count_status == OFF)
        s_line_count_status = ON;
      else
        s_line_count_status = OFF;
      return SUCCEEDED;
    }

    /* MEMORYMAP */
    if (strcmp(directive_upper, "MEMORYMAP") == 0)
      return directive_memorymap();

    break;
    
  case 'N':

#if defined(W65816)
    /* NAME */
    if (strcmp(directive_upper, "NAME") == 0)
      return directive_name_w65816();

    /* NOWDC */
    if (strcmp(directive_upper, "NOWDC") == 0) {
      g_use_wdc_standard = 0;
      return SUCCEEDED;
    }
#endif

#if defined(GB)
    /* NINTENDOLOGO */
    if (strcmp(directive_upper, "NINTENDOLOGO") == 0) {
      no_library_files(".NINTENDOLOGO");

      g_nintendologo_defined++;

      return SUCCEEDED;
    }
  
    /* NAME */
    if (strcmp(directive_upper, "NAME") == 0)
      return directive_name_gb();
#endif
    
    break;
    
  case 'O':

    /* ORG */
    if (strcmp(directive_upper, "ORG") == 0)
      return directive_org();

    /* ORGA */
    if (strcmp(directive_upper, "ORGA") == 0)
      return directive_orga();

    /* OUTNAME */
    if (strcmp(directive_upper, "OUTNAME") == 0) {
      int number_result;

      g_expect_calculations = NO;
      number_result = input_number();
      g_expect_calculations = YES;

      if (number_result != INPUT_NUMBER_STRING && number_result != INPUT_NUMBER_ADDRESS_LABEL) {
        print_error(ERROR_DIR, ".OUTNAME needs a file name string.\n");
        return FAILED;
      }

      strcpy(g_final_name, g_label);

      return SUCCEEDED;
    }
    
    break;

  case 'P':

    /* PRINT */
    if (strcmp(directive_upper, "PRINT") == 0)
      return directive_print();
  
    /* PRINTT */
    if (strcmp(directive_upper, "PRINTT") == 0)
      return directive_printt();

    /* PRINTV */
    if (strcmp(directive_upper, "PRINTV") == 0)
      return directive_printv();

    break;
    
  case 'R':

    /* REPT/REPEAT */
    if (strcmp(directive_upper, "REPT") == 0 || strcmp(directive_upper, "REPEAT") == 0)
      return directive_rept_repeat_while(NO);

    /* REDEFINE/REDEF */
    if (strcmp(directive_upper, "REDEFINE") == 0 || strcmp(directive_upper, "REDEF") == 0)
      return directive_redefine_redef();

    /* ROW? */
    if (strcmp(directive_upper, "ROW") == 0)
      return directive_row_data();

    /* ROMBANKS */
    if (strcmp(directive_upper, "ROMBANKS") == 0)
      return directive_rombanks();

    /* ROMBANKMAP */
    if (strcmp(directive_upper, "ROMBANKMAP") == 0)
      return directive_rombankmap();

    /* RAMSECTION */
    if (strcmp(directive_upper, "RAMSECTION") == 0)
      return directive_ramsection();

#if defined(GB)
    /* RAMSIZE */
    if (strcmp(directive_upper, "RAMSIZE") == 0) {
      no_library_files(".RAMSIZE");

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error(ERROR_DIR, ".RAMSIZE needs a non-negative value.\n");
        return FAILED;
      }

      if (g_rambanks_defined != 0) {
        if (g_rambanks != g_parsed_int) {
          print_error(ERROR_DIR, ".RAMSIZE was defined for the second time.\n");
          return FAILED;
        }
      }

      if (g_parsed_int != 0 && g_parsed_int != 1 && g_parsed_int != 2 && g_parsed_int != 3 && g_parsed_int != 4 && g_parsed_int != 5) {
        print_error(ERROR_DIR, "Unsupported RAM size.\n");
        return FAILED;
      }

      g_rambanks = g_parsed_int;
      g_rambanks_defined = 1;

      return SUCCEEDED;
    }

    /* ROMSIZE */
    if (strcmp(directive_upper, "ROMSIZE") == 0) {
      no_library_files(".ROMSIZE");

      q = input_number();

      if (q == FAILED)
        return FAILED;
      else if (q == INPUT_NUMBER_EOL) {
        next_line();
        g_parsed_int = -1;
      }
      else if (q != SUCCEEDED || g_parsed_int < 0) {
        print_error(ERROR_DIR, ".ROMSIZE needs a non-negative value.\n");
        return FAILED;
      }

      if (g_romsize_defined != 0) {
        if (g_romsize != g_parsed_int) {
          print_error(ERROR_DIR, ".ROMSIZE was defined for the second time.\n");
          return FAILED;
        }
      }

      g_romsize = g_parsed_int;
      g_romsize_defined = 1;

      return SUCCEEDED;
    }

    /* ROMGBC */
    if (strcmp(directive_upper, "ROMGBC") == 0) {
      no_library_files(".ROMGBC");
    
      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG was defined prior to .ROMGBC.\n");
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(ERROR_DIR, ".ROMGBCONLY was defined prior to .ROMGBC.\n");
        return FAILED;
      }

      g_romgbc = 1;

      return SUCCEEDED;
    }

    /* ROMGBCONLY */
    if (strcmp(directive_upper, "ROMGBCONLY") == 0) {
      no_library_files(".ROMGBCONLY");

      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG was defined prior to .ROMGBCONLY.\n");
        return FAILED;
      }
      else if (g_romgbc == 1) {
        print_error(ERROR_DIR, ".ROMGBC was defined prior to .ROMGBCONLY.\n");
        return FAILED;
      }

      g_romgbc = 2;

      return SUCCEEDED;
    }

    /* ROMDMG */
    if (strcmp(directive_upper, "ROMDMG") == 0) {
      no_library_files(".ROMDMG");
    
      if (g_romgbc == 1) {
        print_error(ERROR_DIR, ".ROMGBC was defined prior to .ROMDMG.\n");
        return FAILED;
      }
      else if (g_romgbc == 2) {
        print_error(ERROR_DIR, ".ROMGBCONLY was defined prior to .ROMDMG.\n");
        return FAILED;
      }
      else if (g_romsgb != 0) {
        print_error(ERROR_DIR, ".ROMDMG and .ROMSGB cannot be mixed.\n");
        return FAILED;
      }

      g_romdmg = 1;

      return SUCCEEDED;
    }

    /* ROMSGB */
    if (strcmp(directive_upper, "ROMSGB") == 0) {
      no_library_files(".ROMSGB");
    
      if (g_romdmg != 0) {
        print_error(ERROR_DIR, ".ROMDMG and .ROMSGB cannot be mixed.\n");
        return FAILED;
      }

      g_romsgb = 1;

      return SUCCEEDED;
    }
#endif

    /* ROMBANKSIZE */
    if (strcmp(directive_upper, "ROMBANKSIZE") == 0)
      return directive_rombanksize_banksize();

    break;
    
  case 'S':

    /* SECTION */
    if (strcmp(directive_upper, "SECTION") == 0)
      return directive_section();

    /* STRUCT */
    if (strcmp(directive_upper, "STRUCT") == 0)
      return directive_struct();

    /* SHIFT */
    if (strcmp(directive_upper, "SHIFT") == 0)
      return directive_shift();

    /* SLOT */
    if (strcmp(directive_upper, "SLOT") == 0)
      return directive_slot();

    /* SYM/SYMBOL */
    if (strcmp(directive_upper, "SYM") == 0 || strcmp(directive_upper, "SYMBOL") == 0) {
      if (input_next_string() != SUCCEEDED) {
        print_error(ERROR_DIR, ".SYM requires a symbol name.\n");
        return FAILED;
      }

      fprintf(g_file_out_ptr, "Y%s ", g_label);

      return SUCCEEDED;
    }
    
    /* STRINGMAPTABLE */
    if (strcmp(directive_upper, "STRINGMAPTABLE") == 0)
      return directive_stringmaptable();
    
    /* STRINGMAP */
    if (strcmp(directive_upper, "STRINGMAP") == 0)
      return directive_stringmap();

    /* SEED */
    if (strcmp(directive_upper, "SEED") == 0) {
      q = input_number();
      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED) {
        print_error(ERROR_DIR, ".SEED needs a seed value for the randon number generator.\n");
        return FAILED;
      }

      /* reseed the random number generator */
      init_genrand(g_parsed_int);

      return SUCCEEDED;
    }

    /* SEEDRANDOM */
    if (strcmp(directive_upper, "SEEDRANDOM") == 0) {
      /* init the randon number generator with current time */
      init_genrand((unsigned long)time(NULL));

      return SUCCEEDED;
    }

#if defined(Z80)
    /* SMSTAG */
    if (strcmp(directive_upper, "SMSTAG") == 0) {
      no_library_files(".SMSTAG");

      g_smstag_defined++;
      g_computesmschecksum_defined++;

      return SUCCEEDED;
    }

    /* SMSHEADER */
    if (strcmp(g_current_directive, "SMSHEADER") == 0)
      return directive_smsheader();
  
    /* SDSCTAG */
    if (strcmp(directive_upper, "SDSCTAG") == 0)
      return directive_sdsctag();
#endif

#if defined(MC68000)
    /* SMDHEADER */
    if (strcmp(g_current_directive, "SMDHEADER") == 0)
      return directive_smdheader();
#endif
    
#if defined(W65816)  
    /* SMC */
    if (strcmp(directive_upper, "SMC") == 0) {
      no_library_files(".SMC");

      g_smc_defined++;
      g_snes_mode++;

      return SUCCEEDED;
    }

    /* SLOWROM */
    if (strcmp(directive_upper, "SLOWROM") == 0) {
      no_library_files(".SLOWROM");
    
      if (g_fastrom_defined != 0) {
        print_error(ERROR_DIR, ".FASTROM was defined prior to .SLOWROM.\n");
        return FAILED;
      }

      g_slowrom_defined++;
      g_snes_mode++;

      return SUCCEEDED;
    }

    /* SNESHEADER */
    if (strcmp(directive_upper, "SNESHEADER") == 0)
      return directive_snesheader();

    /* SNESNATIVEVECTOR */
    if (strcmp(directive_upper, "SNESNATIVEVECTOR") == 0)
      return directive_snesnativevector();

    /* SNESEMUVECTOR */
    if (strcmp(directive_upper, "SNESEMUVECTOR") == 0)
      return directive_snesemuvector();
#endif
    
    break;

  case 'T':

    /* TABLE? */
    if (strcmp(directive_upper, "TABLE") == 0)
      return directive_table();

    break;

  case 'U':
    
    /* UNDEF/UNDEFINE */
    if (strcmp(directive_upper, "UNDEF") == 0 || strcmp(directive_upper, "UNDEFINE") == 0)
      return directive_undef_undefine();

    /* UNBACKGROUND */
    if (strcmp(directive_upper, "UNBACKGROUND") == 0)
      return directive_unbackground();

    break;

  case 'V':

#if defined(GB)
    /* VERSION */
    if (strcmp(directive_upper, "VERSION") == 0) {
      no_library_files(".VERSION");
    
      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || g_parsed_int < -128 || g_parsed_int > 255) {
        print_error(ERROR_DIR, ".VERSION needs a 8-bit value, got %d.\n", g_parsed_int);
        return FAILED;
      }

      if (g_version_defined != 0) {
        if (g_version != g_parsed_int) {
          print_error(ERROR_DIR, ".VERSION was defined for the second time.\n");
          return FAILED;
        }
        return SUCCEEDED;
      }

      g_version = g_parsed_int;
      g_version_defined = 1;

      return SUCCEEDED;
    }
#endif
    
    break;
    
  case 'W':

    /* WHILE */
    if (strcmp(directive_upper, "WHILE") == 0)
      return directive_rept_repeat_while(YES);

    /* WORD? */
    if (strcmp(directive_upper, "WORD") == 0)
      return directive_dw_word_addr();

#if defined(W65816)
    /* WDC */
    if (strcmp(directive_upper, "WDC") == 0) {
      g_use_wdc_standard = 1;
      return SUCCEEDED;
    }
#endif
    
    break;
  }
  
  return DIRECTIVE_NOT_IDENTIFIED;
}


static int _increase_ifdef(void) {

  if (g_ifdef == 255) {
    print_error(ERROR_DIR, "Out of .IF stack!\n");
    return FAILED;
  }

  g_ifdef++;

  return SUCCEEDED;
}


static int _decrease_ifdef(void) {

  if (g_ifdef <= 0) {
    print_error(ERROR_DIR, "Out of .IF stack!\n");
    return FAILED;
  }

  g_ifdef--;

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
      print_error(ERROR_DIR, "There must be .IFxxx before .ELSE.\n");
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
        if (_decrease_ifdef() == FAILED)
          return FAILED;
        g_macro_active = m;
        return SUCCEEDED;
      }
    }

    print_error(ERROR_DIR, ".ELSE must end to .ENDIF.\n");
    return FAILED;
  }

  /* ENDIF */

  if (strcaselesscmp(g_current_directive, "ENDIF") == 0) {
    if (g_ifdef == 0) {
      print_error(ERROR_DIR, ".ENDIF was given before any .IF directive.\n");
      return FAILED;
    }

    s_skip_elifs[g_ifdef] = NO;

    if (_decrease_ifdef() == FAILED)
      return FAILED;
    
    return SUCCEEDED;
  }

  /* IFDEF */

  if (strcaselesscmp(g_current_directive, "IFDEF") == 0) {
    struct definition *d;

    if (get_next_plain_string() == FAILED)
      return FAILED;
    
    if (_increase_ifdef() == FAILED)
      return FAILED;

    hashmap_get(g_defines_map, g_label, (void *)&d);
    if (d != NULL) {
      s_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }

    s_skip_elifs[g_ifdef] = NO;

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
      print_error(ERROR_INP, ".IF needs immediate data.\n");
      return FAILED;
    }

    if (_increase_ifdef() == FAILED)
      return FAILED;

    /* 0 = false, otherwise it's true */
    if (g_parsed_int != 0) {
      s_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      s_skip_elifs[g_ifdef] = NO;
      return find_next_point("IF");
    }
  }

  /* ELIF */

  if (strcaselesscmp(g_current_directive, "ELIF") == 0) {
    if (g_ifdef == 0) {
      print_error(ERROR_DIR, "There must be .IFxxx before .ELIF.\n");
      return FAILED;
    }

    /* had an .if already succeeded previously? */
    if (s_skip_elifs[g_ifdef] == YES)
      return find_next_point("ELIF");

    g_input_parse_if = YES;
    q = input_number();
    g_input_parse_if = NO;

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(ERROR_INP, ".ELIF needs immediate data.\n");
      return FAILED;
    }

    /* 0 = false, otherwise it's true */
    if (g_parsed_int != 0) {
      s_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      s_skip_elifs[g_ifdef] = NO;
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
      print_error(ERROR_INP, ".%s needs immediate data.\n", bak);
      return FAILED;
    }

    strncpy(k, g_label, 255);
    k[255] = 0;
    y = g_parsed_int;
    s = q;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING && q != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(ERROR_INP, ".%s needs immediate data.\n", bak);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error(ERROR_INP, "The types of the compared things must be the same.\n");
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
      s_skip_elifs[g_ifdef] = YES;
      return SUCCEEDED;
    }
    else {
      s_skip_elifs[g_ifdef] = NO;
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
      print_error(ERROR_DIR, ".IFEXISTS needs a file name string.\n");
      return FAILED;
    }

    if (_increase_ifdef() == FAILED)
      return FAILED;

    f = fopen(g_label, "r");
    if (f == NULL) {
      s_skip_elifs[g_ifdef] = NO;
      return find_next_point("IFEXISTS");
    }

    fclose(f);

    s_skip_elifs[g_ifdef] = YES;
      
    return SUCCEEDED;
  }

  /* IFNDEF */

  if (strcaselesscmp(g_current_directive, "IFNDEF") == 0) {
    struct definition *d;

    if (get_next_plain_string() == FAILED)
      return FAILED;

    if (_increase_ifdef() == FAILED)
      return FAILED;

    hashmap_get(g_defines_map, g_label, (void*)&d);
    if (d != NULL) {
      char* tmp;
      int result;
      
      s_skip_elifs[g_ifdef] = NO;
      tmp = _string_duplicate(g_current_directive);
      if (tmp == NULL)
        return FAILED;
      result = find_next_point(tmp);
      free(tmp);
      return result;
    }

    s_skip_elifs[g_ifdef] = YES;
    
    return SUCCEEDED;
  }

  /* IFDEFM/IFNDEFM */

  if (strcaselesscmp(g_current_directive, "IFDEFM") == 0 || strcaselesscmp(g_current_directive, "IFNDEFM") == 0) {
    int k, o;
    char e;

    strcpy(bak, g_current_directive);

    if (g_macro_active == 0) {
      print_error(ERROR_DIR, ".%s can be only used inside a macro.\n", bak);
      return FAILED;
    }

    if (g_current_directive[2] == 'N')
      o = 0;
    else
      o = 1;

    if (_increase_ifdef() == FAILED)
      return FAILED;

    for (; g_source_index < g_source_file_size; g_source_index++) {
      if (g_buffer[g_source_index] == 0x0A)
        break;
      else if (g_buffer[g_source_index] == '\\') {
        e = g_buffer[++g_source_index];
        if (e >= '0' && e <= '9') {
          g_parsed_int = (e - '0') * 10;
          for (k = 2; k < 8; k++, g_parsed_int *= 10) {
            e = g_buffer[++g_source_index];
            if (e >= '0' && e <= '9')
              g_parsed_int += e - '0';
            else
              break;
          }

          g_parsed_int /= 10;
          if ((o == 0 && g_macro_runtime_current->supplied_arguments < g_parsed_int) ||
              (o == 1 && g_macro_runtime_current->supplied_arguments >= g_parsed_int)) {
            s_skip_elifs[g_ifdef] = YES;
            return SUCCEEDED;
          }
          else {
            char* tmp;
            int result;
            
            s_skip_elifs[g_ifdef] = NO;
            tmp = _string_duplicate(g_current_directive);
            if (tmp == NULL)
              return FAILED;
            result = find_next_point(tmp);
            free(tmp);
            return result;
          }
        }
        break;
      }
    }

    print_error(ERROR_DIR, ".%s needs an argument.\n", bak);

    return FAILED;
  }

  /* neither success nor failure (didn't match any "if" directives) */
  return -1;
}


int find_next_point(char *name) {

  int depth = 1, m, line_current = g_active_file_info_last->line_current, source_index_old = g_source_index;

  /* find the next compiling point */
  m = g_macro_active;

  /* disable macro decoding */
  g_macro_active = 0;

  /* don't use substitution in get_next_token() */
  g_get_next_token_use_substitution = NO;

  while (get_next_token() != FAILED) {
    if (g_tmp[0] == '.') {
      if (strcaselesscmp(g_current_directive, "ENDIF") == 0) {
        depth--;
        if (depth == 0)
          s_skip_elifs[g_ifdef] = NO;
        if (_decrease_ifdef() == FAILED)
          return FAILED;
      }
      if (strcaselesscmp(g_current_directive, "ELSE") == 0 && depth == 1) {
        depth--;
        if (s_skip_elifs[g_ifdef] == YES)
          g_source_index = source_index_old;
      }
      if (strcaselesscmp(g_current_directive, "ELIF") == 0 && depth == 1) {
        if (s_skip_elifs[g_ifdef] == NO) {
          /* go backwards so we'll actually parse .ELIF later */
          g_source_index = source_index_old;
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
        s_skip_elifs[g_ifdef] = YES;        
      }
    }

    if (depth == 0) {
      g_macro_active = m;
      g_get_next_token_use_substitution = YES;
      return SUCCEEDED;
    }
    else
      source_index_old = g_source_index;
  }

  /* return the condition's line number */
  g_active_file_info_last->line_current = line_current;
  print_error(ERROR_DIR, ".%s must end to .ENDIF/.ELSE.\n", name);

  return FAILED;
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
    stack_create_label_stack(g_label);
    x = INPUT_NUMBER_STACK;
  }

  if (x == INPUT_NUMBER_STACK) {
    struct stack *stack = find_stack_calculation(g_latest_stack, YES);

    if (stack == NULL)
      return FAILED;
    
    *b = g_latest_stack;
    stack->position = STACK_POSITION_DEFINITION;

    /* export the definition? */
    if (compare_next_token("EXPORT") == SUCCEEDED) {
      skip_next_token();
      *export = YES;
    }

    x = input_number();
    if (x != INPUT_NUMBER_EOL) {
      print_error(ERROR_DIR, "A computation cannot be output as a string.\n");
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
        memcpy(c, g_label, g_string_size);
        s = g_string_size;
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
      memcpy(&c[s], g_label, g_string_size);
      s += g_string_size;
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
  char c1 = name[0];

  /* don't export it twice or more often */
  export = g_export_first;
  while (export != NULL) {
    if (c1 == export->name[0]) {
      if (strcmp(export->name, name) == 0) {
        print_error(ERROR_WRN, "\"%s\" was .EXPORTed for the second time.\n", name);
        return SUCCEEDED;
      }
    }
    export = export->next;
  }

  export = calloc(sizeof(struct export_def), 1);
  if (export == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating room for \".EXPORT %s\".\n", name);
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
  struct definition *tmp_def;

  if (_is_a_reserved_label(l) == YES)
    return FAILED;

  /* cannot create a label as a definition with the same name exists */
  hashmap_get(g_defines_map, l, (void*)&tmp_def);
  if (tmp_def != NULL) {
    print_error(ERROR_ERR, "A definition called \"%s\" exists already, cannot add a label with the same name.\n", l);
    return FAILED;
  }
  
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
    print_error(ERROR_ERR, "ADD_LABEL_TO_LABEL_STACK: Out of label stack depth. Can handle only 255 child labels...\n");
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
    print_text(NO, "*************************************\n");
    print_text(NO, "LABEL STACK:\n");
    for (q = 0; q < 256; q++) {
    if (g_label_stack[q][0] != 0)
    print_text(NO, "%s LEVEL %d\n", g_label_stack[q], q);
    }
    print_text(NO, "*************************************\n");
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
        print_error(ERROR_ERR, "GET_FULL_LABEL: Constructed label size will be >= MAX_NAME_LENGTH. Edit the define in shared.h, recompile WLA and try again...\n");
        return FAILED;  
      }
      strncat(out, g_label_stack[q], MAX_NAME_LENGTH);
    }

    if (strlen(out) + strlen(&l[level-1]) >= MAX_NAME_LENGTH) {
      print_error(ERROR_ERR, "GET_FULL_LABEL: Constructed label size will be >= MAX_NAME_LENGTH. Edit the define in shared.h, recompile WLA and try again...\n");
      return FAILED;    
    }

    strncat(out, &l[level-1], MAX_NAME_LENGTH);
  }

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

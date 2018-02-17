
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
#include "stack.h"


#ifdef GB
char licenseecodenew_c1, licenseecodenew_c2;
int gbheader_defined = 0;
int nintendologo_defined = 0;
int computechecksum_defined = 0, computecomplementcheck_defined = 0;
int romgbc = 0, romdmg = 0, romsgb = 0;
int cartridgetype = 0, cartridgetype_defined = 0;
int countrycode = 0, countrycode_defined = 0;
int licenseecodenew_defined = 0, licenseecodeold = 0, licenseecodeold_defined = 0;
#endif

#ifdef Z80
char sdsctag_name_str[MAX_NAME_LENGTH], sdsctag_notes_str[MAX_NAME_LENGTH], sdsctag_author_str[MAX_NAME_LENGTH];
int sdsctag_name_type, sdsctag_notes_type, sdsctag_author_type, sdsc_ma, sdsc_mi;
int sdsctag_name_value, sdsctag_notes_value, sdsctag_author_value;
int computesmschecksum_defined = 0, sdsctag_defined = 0, smstag_defined = 0;
int smsheader_defined = 0, smsversion = 0, smsversion_defined = 0, smsregioncode = 0, smsregioncode_defined = 0, smsproductcode_defined = 0, smsproductcode1 = 0, smsproductcode2 = 0, smsproductcode3 = 0, smsreservedspace1 = 0, smsreservedspace2 = 0, smsreservedspace_defined = 0;
#endif

int org_defined = 1, background_defined = 0, background_size = 0;
int bank = 0, bank_defined = 1;
int rombanks = 0, rombanks_defined = 0, romtype = 0, max_address;
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
int block_status = 0;
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
int computesneschecksum_defined = 0;
#endif

#if defined(GB) || defined(W65816)
char name[32];
int name_defined = 0;
#endif

#if defined(MCS6502) || defined(W65816) || defined(MCS6510) || defined(WDC65C02) || defined(HUC6280)
extern int operand_hint;
#endif

char tmp[4096], emsg[256];
char *tmp_bf;
char cp[256];

unsigned char *rom_banks = NULL, *rom_banks_usage_table = NULL;

struct export_def *export_first = NULL, *export_last = NULL;
struct optcode *opt_tmp;
struct definition *tmp_def;
struct map_t *defines_map = NULL;
struct macro_static *macros_first = NULL, *macros_last;
struct section_def *sections_first = NULL, *sections_last = NULL, *sec_tmp, *sec_next;
struct macro_runtime *macro_stack = NULL, *macro_runtime_current = NULL;
struct repeat_runtime *repeat_stack = NULL;
struct slot slots[256];
struct structure *structures_first = NULL;
struct filepointer *filepointers = NULL;
struct map_t *namespace_map = NULL;
struct append_section *append_sections = NULL;

extern char *buffer, *unfolded_buffer, label[MAX_NAME_LENGTH], *include_dir, *full_name;
extern int size, unfolded_size, input_number_error_msg, verbose_mode, output_format, open_files;
extern int stack_id, latest_stack, ss, commandline_parsing, newline_beginning;
extern int extra_definitions, string_size, input_float_mode;
extern int include_dir_size, parse_floats, listfile_data, quiet, parsed_double_decimal_numbers;
extern FILE *file_out_ptr;
extern double parsed_double;
extern char *final_name;
extern struct active_file_info *active_file_info_first, *active_file_info_last, *active_file_info_tmp;
extern struct file_name_info *file_name_info_first, *file_name_info_last, *file_name_info_tmp;
extern struct stack *stacks_first, *stacks_tmp, *stacks_last;
extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;

int opcode_n[256], opcode_p[256];
int macro_stack_size = 0, repeat_stack_size = 0;

#if defined(MCS6502) || defined(WDC65C02) || defined(MCS6510) || defined(W65816) || defined(HUC6280)
int xbit_size = 0;
int accu_size = 8, index_size = 8;
#endif

/* vars used when in an enum or ramsection */
int in_enum=0, in_ramsection=0;
int enum_exp, enum_ord, enum_offset;


/* remember to run opcodesgen/gen with the proper flags defined */
/*    (GB/Z80/MCS6502/WDC65C02/MCS6510/W65816/HUC6280/SPC700)   */

#ifdef AMIGA
__far /* put the following big table in the FAR data section */
#endif

extern int opcode_n[256];
extern struct optcode opt_table[];

#define no_library_files(name)\
 do {\
 if (output_format == OUTPUT_LIBRARY) {\
    print_error("Library files don't take " name ".\n", ERROR_DIR);\
    return FAILED;\
  }\
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


struct macro_static *macro_get(char *name) {

  struct macro_static *macro;

  
  macro = macros_first;
  while (macro != NULL) {
    if (strcmp(macro->name, name) == 0)
      break;
    macro = macro->next;
  }

  return macro;
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

    memcpy(macro, macro_stack, sizeof(struct macro_runtime) * old_size);
    macro_stack = macro;
  }

  return SUCCEEDED;
}


int macro_start(struct macro_static *m, struct macro_runtime *mrt, int caller, int nargs) {

  macro_runtime_current = mrt;
  macro_active++;
  m->calls++;

  mrt->caller = caller;
  mrt->macro = m;
  mrt->macro_end = i;
  mrt->macro_end_line = active_file_info_last->line_current;
  mrt->macro_end_filename_id = active_file_info_last->filename_id;

  if ((extra_definitions == ON) && (active_file_info_last->filename_id != m->filename_id)) {
    redefine("WLA_FILENAME", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, strlen(get_file_name(m->filename_id)));
    redefine("wla_filename", 0.0, get_file_name(m->filename_id), DEFINITION_TYPE_STRING, strlen(get_file_name(m->filename_id)));
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
  mrt->argument_data = malloc(sizeof(struct macro_argument *) << 1);
  mrt->argument_data[0] = malloc(sizeof(struct macro_argument));
  mrt->argument_data[1] = malloc(sizeof(struct macro_argument));
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
    sprintf(emsg, ".%s needs data.\n", name);
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
    mrt->string_last = strlen(label);
    /*
      fprintf(stderr, "got string %s!\n", label);
    */
  }
  else if (inz == INPUT_NUMBER_STACK)
    mrt->argument_data[0]->value = latest_stack;
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

  mrt->argument_data = malloc(sizeof(struct macro_argument *) << 1);
  mrt->argument_data[0] = malloc(sizeof(struct macro_argument));
  mrt->argument_data[1] = malloc(sizeof(struct macro_argument));
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
    sprintf(emsg, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -128 || d->value > 255) {
      sprintf(emsg, ".%s expects 8-bit data, %d is out of range!\n", name, (int)d->value);
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
    sprintf(emsg, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
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
    sprintf(emsg, "No \"_OUT/_out\" defined, .%s takes its output from there.\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  if (d->type == DEFINITION_TYPE_VALUE) {
    if (d->value < -32768 || d->value > 65535) {
      sprintf(emsg, ".%s expects 16-bit data, %d is out of range!\n", name, (int)d->value);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    fprintf(file_out_ptr, "y%d ", (int)d->value);
    /*
      fprintf(stderr, ".DBM: VALUE: %d\n", (int)d->value);
    */
  }
  else if (d->type == DEFINITION_TYPE_STACK) {
    fprintf(file_out_ptr, "C%d ", (int)d->value);
    /*
      fprintf(stderr, ".DBM: STACK: %d\n", (int)d->value);
    */
  }
  else {
    sprintf(emsg, ".%s cannot handle strings in \"_OUT/_out\".\n", name);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  return SUCCEEDED;
}


struct structure* get_structure(char *name) {

  struct structure *s = structures_first;
  while (s != NULL) {
    if (strcmp(name, s->name) == 0)
      return s;
    s = s->next;
  }

  return NULL;
}


int pass_1(void) {

  struct macro_runtime *mrt = NULL;
  struct macro_static *m = NULL;
  int o, p, q;

  
  if (verbose_mode == ON)
    printf("Pass 1...\n");

  /* mark all slots as empty */
  for (q = 0; q < 256; q++)
    slots[q].size = 0;

  /* WARNING: "i" is a global variable that we use as the char index to the source file. */
  /* Ville: this must be one of the worst programming decicions I've ever done, sorry about it... */
  /* ... but the year was something like 1998 and I had just coded 6+ years 68k asm, and moved to C... */

  /* start from the very first character */
  i = 0;

  /* output the file id */
  fprintf(file_out_ptr, "f%d ", active_file_info_tmp->filename_id);

  /* BANK 0 SLOT 0 ORG 0 */
  if (output_format != OUTPUT_LIBRARY)
    fprintf(file_out_ptr, "B%d %d O%d", 0, 0, 0);

  while ((t = get_next_token()) == SUCCEEDED) {
    q = evaluate_token();

    if (q == SUCCEEDED)
      continue;
    else if (q == EVALUATE_TOKEN_EOP)
      return SUCCEEDED;
    else if (q == EVALUATE_TOKEN_NOT_IDENTIFIED) {
      /* check if it is of the form "LABEL:XYZ" */
      for (q = 0; q < ss; q++)
        if (tmp[q] == ':')
          break;

      /* is it a macro? */
      if (q == ss)
        m = macro_get(tmp);

      /* it is a label after all? */
      if (q != ss || newline_beginning == ON) {
        tmp[q] = 0;

        /* reset the flag as there can be only one label / line */
        newline_beginning = OFF;

        if (output_format == OUTPUT_LIBRARY && section_status == OFF) {
          print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
          return FAILED;
        }
        if (org_defined == 0) {
          sprintf(emsg, "\"%s\" needs a position in memory.\n", tmp);
          print_error(emsg, ERROR_LOG);
          return FAILED;
        }
        if (ss > MAX_NAME_LENGTH - 1) {
          sprintf(emsg, "The label \"%s\" is too long. Max label length is %d bytes.\n", tmp, MAX_NAME_LENGTH - 1);
          print_error(emsg, ERROR_NONE);
          return FAILED;
        }
        if (bankheader_status == ON) {
          print_error("BANKHEADER sections don't take labels.\n", ERROR_LOG);
          return FAILED;
        }

        /* check out for \@-symbols */
        if (macro_active != 0) {
          if (tmp[q - 2] == '\\' && tmp[q - 1] == '@')
            sprintf(&tmp[q - 2], "%d%c", macro_runtime_current->macro->calls - 1, 0);
        }

        fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmp);

        /* move to the end of the label */
        if (q != ss)
          i -= ss - q - 1;
        else
          i -= ss - q;

        continue;
      }

      if (m == NULL) {
        sprintf(emsg, "Unknown symbol \"%s\".\n", tmp);
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
        mrt->argument_data[p] = malloc(sizeof(struct macro_argument));
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
          mrt->argument_data[p]->value = latest_stack;
        else if (q == SUCCEEDED)
          mrt->argument_data[p]->value = d;
        else
          return FAILED;

        /* do we have a name for this argument? */
        if (p < m->nargument_names) {
          if (q == INPUT_NUMBER_ADDRESS_LABEL)
            redefine(m->argument_names[p], 0.0, label, DEFINITION_TYPE_ADDRESS_LABEL, strlen(label));
          else if (q == INPUT_NUMBER_STRING)
            redefine(m->argument_names[p], 0.0, label, DEFINITION_TYPE_STRING, strlen(label));
          else if (q == INPUT_NUMBER_STACK)
            redefine(m->argument_names[p], (double)latest_stack, NULL, DEFINITION_TYPE_STACK, 0);
          else if (q == SUCCEEDED)
            redefine(m->argument_names[p], (double)d, NULL, DEFINITION_TYPE_VALUE, 0);
        }
      }

      next_line();

      mrt->supplied_arguments = p;
      if (macro_start(m, mrt, MACRO_CALLER_NORMAL, p) == FAILED)
        return FAILED;

      continue;
    }
    else if (q == FAILED) {
      sprintf(emsg, "Couldn't parse \"%s\".\n", tmp);
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
#if WLA_DEBUG
  {
    char ttt[64];

    va_end(ap);
    va_start(ap, format);
    vsprintf(ttt, format, ap);
    printf("LINE %5d: OPCODE: %16s ::: %s\n", active_file_info_last->line_current, oc->op, ttt);
  }
#endif

  va_end(ap);
}


int evaluate_token(void) {

  int f, z = 0, x, y;
#if defined(Z80) || defined(SPC700) || defined(W65816) || defined(WDC65C02) || defined(HUC6280)
  int e = 0, v = 0, h = 0;
  char labelx[256];
#endif
#ifdef SPC700
  int g;
#endif
#ifdef HUC6280
  int r = 0, s, t = 0, u = 0;
  char labely[256];
#endif

  
  /* are we in an enum or ramsection? */
  if (in_enum == 1 || in_ramsection == 1)
    return parse_enum_token();

  /* is it a directive? */
  if (tmp[0] == '.')
    return parse_directive();

  /* is it a label? */
  if (tmp[ss - 1] == ':' && newline_beginning == ON) {
    tmp[ss - 1] = 0;
    newline_beginning = OFF;

    if (output_format == OUTPUT_LIBRARY && section_status == OFF) {
      print_error("All labels must be inside sections when compiling a library.\n", ERROR_LOG);
      return FAILED;
    }
    if (org_defined == 0) {
      sprintf(emsg, "\"%s\" needs a position in memory.\n", tmp);
      print_error(emsg, ERROR_LOG);
      return FAILED;
    }
    if (ss > MAX_NAME_LENGTH - 1) {
      sprintf(emsg, "The label \"%s\" is too long. Max label length is %d bytes.\n", tmp, MAX_NAME_LENGTH - 1);
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
        sprintf(&tmp[ss - 3], "%d%c", macro_runtime_current->macro->calls - 1, 0);
    }

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
    for (inz = 0, d = SUCCEEDED; inz < OP_SIZE_MAX; inz++) {
      if (tmp[inz] == 0 && opt_tmp->op[inz] == 0 && buffer[i] == 0x0A) {
        if (opt_tmp->type == 0)
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
        else
          output_assembled_opcode(opt_tmp, "y%d ", opt_tmp->hex);
        return SUCCEEDED;
      }
      if (tmp[inz] == 0 && opt_tmp->op[inz] == ' ' && buffer[i] == ' ')
        break;
      if (opt_tmp->op[inz] != toupper((int)tmp[inz])) {
        d = FAILED;
        break;
      }
    }

    if (d == FAILED) {
      opt_tmp = &opt_table[++ind];
      continue;
    }

    /* beginning matches the input */
    x = inz + 1;
    inz = i + 1;

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
#ifdef MCS6510
#include "decode_6510.c"
#endif
#ifdef W65816
#include "decode_65816.c"
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
  if (d == NULL) {
    return add_a_new_definition(name, value, string, type, size);
  }

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

  
  hashmap_get(defines_map, name, (void*)&d);
  if (d != NULL) {
    sprintf(emsg, "\"%s\" was defined for the second time.\n", name);
    if (commandline_parsing == OFF)
      print_error(emsg, ERROR_DIR);
    else
      fprintf(stderr, "ADD_A_NEW_DEFINITION: %s", emsg);
    return FAILED;
  }

  d = malloc(sizeof(struct definition));
  if (d == NULL) {
    sprintf(emsg, "Out of memory while trying to add a new definition (\"%s\").\n", name);
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
  return;
}


void next_line(void) {

  newline_beginning = ON;

  if (line_count_status == OFF)
    return;

  /* output the file number for list file structure building */
  if (listfile_data == YES)
    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);

  if (active_file_info_last != NULL)
    active_file_info_last->line_current++;
}


/* either "in_enum" or "in_ramsection" should be true when this is called. */
int parse_enum_token(void) {

  char tmpname[MAX_NAME_LENGTH];
  int type;
  int q;

  
  /* check for "if" directives (the only directives permitted in an enum/ramsection) */
  if (tmp[0] == '.') {
    if ((q = parse_if_directive()) != -1)
      return q;
  }

  if (in_enum && strcaselesscmp(tmp, ".ENDE") == 0) {
    in_enum = 0;
    return SUCCEEDED;
  }
  else if (in_ramsection && strcaselesscmp(tmp, ".ENDS") == 0) {
    fprintf(file_out_ptr, "s ");
    section_status = OFF;
    in_ramsection = 0;
    return SUCCEEDED;
  }

  if (tmp[strlen(tmp) - 1] == ':')
    tmp[strlen(tmp) - 1] = 0;
  if (in_ramsection)
    fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, tmp);

  strcpy(tmpname, tmp);

  /* ASC? */
  if (in_enum && enum_ord == 1) {
    if (add_a_new_definition(tmpname, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;
    if (enum_exp == YES)
      if (export_a_definition(tmpname) == FAILED)
        return FAILED;
  }

  /* get the size/type */
  if (get_next_token() == FAILED)
    return FAILED;

  type = 0;

  if (strcaselesscmp(tmp, "DB") == 0 || strcaselesscmp(tmp, "BYT") == 0 || strcaselesscmp(tmp, "BYTE") == 0) {
    if (in_enum)
      enum_offset += 1*enum_ord;
    else /* ramsection */
      fprintf(file_out_ptr, "d0 ");
  }
  else if (strcaselesscmp(tmp, "DW") == 0 || strcaselesscmp(tmp, "WORD") == 0) {
    if (in_enum)
      enum_offset += 2*enum_ord;
    else /* ramsection */
      fprintf(file_out_ptr, "y0 ");
  }
  else if (strcaselesscmp(tmp, "DS") == 0 || strcaselesscmp(tmp, "DSB") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DS/DSB needs size.\n", ERROR_DIR);
      return FAILED;
    }
    if (in_enum)
      enum_offset += d*enum_ord;
    else
      fprintf(file_out_ptr, "x%d 0 ", d);
  }
  else if (strcaselesscmp(tmp, "DSW") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error("DSW needs size.\n", ERROR_DIR);
      return FAILED;
    }
    if (in_enum)
      enum_offset += d*2*enum_ord;
    else
      fprintf(file_out_ptr, "x%d 0 ", d*2);
  }
  /* it's an instance of a structure! */
  else if (strcaselesscmp(tmp, "INSTANCEOF") == 0) {

    struct structure_item *si;
    struct structure *st;
    int g;


    type = 1;

    if (get_next_token() == FAILED)
      return FAILED;

    st = get_structure(tmp);

    if (st == NULL) {
      sprintf(emsg, "No STRUCT named \"%s\" available.\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* generate labels (different for enum vs ramsection) */
    if (in_enum) {
      /* generate labels (first the basic ones, without index number) */
      if (enum_ord == -1) {
        enum_offset -= st->size;
        if (add_a_new_definition(tmpname, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;
        if (enum_exp == YES)
          if (export_a_definition(tmpname) == FAILED)
            return FAILED;
      }

      si = st->items;
      while (si != NULL) {
        sprintf(tmp, "%s.%s%c", tmpname, si->name, 0);
        if (add_a_new_definition(tmp, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;
        if (enum_exp == YES)
          if (export_a_definition(tmp) == FAILED)
            return FAILED;
        enum_offset += si->size;
        si = si->next;
      }

      if (enum_ord == -1)
        enum_offset -= st->size;

      /* the number of structures? */
      inz = input_number();
      if (inz == INPUT_NUMBER_EOL)
        next_line();
      else if (inz == SUCCEEDED) {
        if (d < 1) {
          print_error("The number of structures must be greater than 0.\n", ERROR_DIR);
          return FAILED;
        }

        /* generate labels (now with the index numbers) */
        if (d > 1) {
          g = 1;

          if (enum_ord == 1)
            enum_offset -= st->size;

          while (d > 0) {
            si = st->items;
            sprintf(tmp, "%s.%d%c", tmpname, g, 0);
            if (add_a_new_definition(tmp, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
              return FAILED;
            if (enum_exp == YES)
              if (export_a_definition(tmp) == FAILED)
                return FAILED;
            while (si != NULL) {
              sprintf(tmp, "%s.%d.%s%c", tmpname, g, si->name, 0);
              if (add_a_new_definition(tmp, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
                return FAILED;
              if (enum_exp == YES)
                if (export_a_definition(tmp) == FAILED)
                  return FAILED;
              enum_offset += si->size;
              si = si->next;
            }
            g++;
            d--;
            if (enum_ord == -1) {
              if (d > 0)
                enum_offset -= st->size*2;
              else
                enum_offset -= st->size;
            }
          }
        }
      }
      else {
        if (inz == INPUT_NUMBER_STRING)
          sprintf(emsg, "Expected the number of structures, got \"%s\" instead.\n", label);
        else
          sprintf(emsg, "Expected the number of structures.\n");
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
    else { /* ramsection */
      /* amount of structures? */
      inz = input_number();
      if (inz == SUCCEEDED && d > 1)
        fprintf(file_out_ptr, "k%d L%s.1 ", active_file_info_last->line_current, tmpname);

      /* generate labels */
      si = st->items;
      while (si != NULL) {
        if (inz == SUCCEEDED && d > 1)
          fprintf(file_out_ptr, "k%d L%s.%s L%s.1.%s x%d 0 ", active_file_info_last->line_current, tmpname, si->name, tmpname, si->name, si->size);
        else
          fprintf(file_out_ptr, "k%d L%s.%s x%d 0 ", active_file_info_last->line_current, tmpname, si->name, si->size);
        si = si->next;
      }

      if (inz == INPUT_NUMBER_EOL)
        next_line();
      else if (inz == SUCCEEDED) {
        if (d < 1) {
          print_error("The amount of structures must be greater than 0.\n", ERROR_DIR);
          return FAILED;
        }

        g = 2;
        while (d > 1) {
          si = st->items;
          fprintf(file_out_ptr, "k%d L%s.%d ", active_file_info_last->line_current, tmpname, g);
          while (si != NULL) {
            fprintf(file_out_ptr, "k%d L%s.%d.%s x%d 0 ", active_file_info_last->line_current, tmpname, g, si->name, si->size);
            si = si->next;
          }
          g++;
          d--;
        }
      }
      else {
        if (inz == INPUT_NUMBER_STRING)
          sprintf(emsg, "Expected the amount of structures, got \"%s\" instead.\n", label);
        else
          sprintf(emsg, "Expected the amount of structures.\n");
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
    }
  }
  else if (strcaselesscmp(tmp, ".db") == 0 || strcaselesscmp(tmp, ".dw") == 0)
    ; /* Don't do anything for "dotted" versions */
  else {
    if (in_enum)
      sprintf(emsg, "Unexpected symbol \"%s\" in .ENUM.\n", tmp);
    else /* ramsection */
      sprintf(emsg, "Unexpected symbol \"%s\" in .RAMSECTION.\n", tmp);
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

  /* DESC? */
  if (in_enum && enum_ord == -1 && type == 0) {
    if (add_a_new_definition(tmpname, (double)enum_offset, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;
    if (enum_exp == YES)
      if (export_a_definition(tmpname) == FAILED)
        return FAILED;
  }

  return SUCCEEDED;
}


int parse_directive(void) {

  char bak[256];
  int o, q;

  if ((q = parse_if_directive()) != -1)
    return q;
  
  /* ORG */

  if (strcaselesscmp(cp, "ORG") == 0) {
    no_library_files(".ORG definitions");
    
    if (bank_defined == 0) {
      print_error("No .BANK is defined.\n", ERROR_LOG);
      return FAILED;
    }
    if (section_status == ON) {
      print_error("You can't issue .ORG inside a .SECTION.\n", ERROR_DIR);
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

  /* ORGA */

  if (strcaselesscmp(cp, "ORGA") == 0) {
    no_library_files(".ORGA definitions");
    
    if (bank_defined == 0) {
      print_error("No .BANK is defined.\n", ERROR_LOG);
      return FAILED;
    }
    if (section_status == ON) {
      print_error("You can't issue .ORGA inside a .SECTION.\n", ERROR_DIR);
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

  /* SLOT */

  if (strcaselesscmp(cp, "SLOT") == 0) {
    no_library_files(".SLOT definitions");
    
    if (section_status == ON) {
      print_error("You can't issue .SLOT inside a .SECTION.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();

    if (q == FAILED)
      return FAILED;

    if (q != SUCCEEDED) {
      print_error(".SLOT needs a positive or zero integer value.\n", ERROR_DIR);
      return FAILED;
    }

    if (slots[d].size == 0) {
      sprintf(emsg, "There is no SLOT number %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    fprintf(file_out_ptr, "B%d %d ", bank, d);

    current_slot = d;

    return SUCCEEDED;
  }

  /* BANK */

  if (strcaselesscmp(cp, "BANK") == 0) {
    no_library_files(".BANK definitions");
    
    if (section_status == ON) {
      sprintf(emsg, "Section \"%s\" is open. Do not try to change the bank.\n", sections_last->name);
      print_error(emsg, ERROR_LOG);
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
      sprintf(emsg, "ROM banks == %d, selected bank %d.\n", rombanks, d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    bank = d;
    bank_defined = 1;

    if (compare_next_token("SLOT", 4) == SUCCEEDED) {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d > 255 || d < 0) {
        print_error("SLOT needs an unsigned 8-bit value as an ID.\n", ERROR_DIR);
        return FAILED;
      }

      if (slots[d].size == 0) {
        sprintf(emsg, "There is no SLOT number %d.\n", d);
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
      sprintf(emsg, "SLOT %d's size %d < BANK %d's size %d.\n", inz, slots[inz].size, ind, banks[ind]);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    if (slots[inz].size > banks[ind]) {
      sprintf(emsg, "SLOT %d's size %d > BANK %d's size %d, but the bank fits inside.\n", inz, slots[inz].size, ind, banks[ind]);
      print_error(emsg, ERROR_WRN);
    }

    return SUCCEEDED;
  }

  /* DBM/DWM? */

  if (strcaselesscmp(cp, "DBM") == 0 || strcaselesscmp(cp, "DWM") == 0) {

    struct macro_static *m;

    strcpy(bak, cp);
    inz = input_number();
    if (inz != INPUT_NUMBER_ADDRESS_LABEL) {
      sprintf(emsg, ".%s requires macro name.\n", bak);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* find the macro */
    m = macro_get(label);

    if (m == NULL) {
      sprintf(emsg, "No MACRO \"%s\" defined.\n", label);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (strcaselesscmp(cp, "DBM") == 0) {
      if (macro_start_dxm(m, MACRO_CALLER_DBM, cp, YES) == FAILED)
        return FAILED;
    }
    else {
      if (macro_start_dxm(m, MACRO_CALLER_DWM, cp, YES) == FAILED)
        return FAILED;
    }

    return SUCCEEDED;
  }

  /* DB/BYT/BYTE? */

  if (strcaselesscmp(cp, "DB") == 0 || strcaselesscmp(cp, "BYT") == 0 || strcaselesscmp(cp, "BYTE") == 0) {
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
	    char tmp_a[3], *tmp_b;
	    int tmp_c;
	    
	    o += 3;
	    sprintf(tmp_a, "%c%c", label[o-1], label[o]);
	    tmp_c = strtol(tmp_a, &tmp_b, 16);
	    if (*tmp_b) {
	      sprintf(emsg, ".%s '\\x' needs hexadecimal byte (00-FF) data.\n", bak);
	      print_error(emsg, ERROR_INP);
	      return FAILED;
	    }
	    fprintf(file_out_ptr, "d%d ", tmp_c);
	  }
	  /* handle '\<' */
	  else if (label[o] == '\\' && label[o + 1] == '<') {
	    o += 2;
	    if (o == string_size) {
	      sprintf(emsg, ".%s '\\<' needs character data.\n", bak);
	      print_error(emsg, ERROR_INP);
	      return FAILED;
	    }
	    fprintf(file_out_ptr, "d%d ", (int)label[o]|0x80);
	  }
	  /* handle '\>' */
	  else if (label[o] == '\\' && label[o + 1] == '>' && o == 0) {
	    sprintf(emsg, ".%s '\\>' needs character data.\n", bak);
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
        sprintf(emsg, ".%s expects 8-bit data, %d is out of range!\n", bak, d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        fprintf(file_out_ptr, "d%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        fprintf(file_out_ptr, "k%d Q%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        fprintf(file_out_ptr, "c%d ", latest_stack);

      inz = input_number();
    }

    if (inz == FAILED)
      return FAILED;

    if (inz == INPUT_NUMBER_EOL && ind == 0) {
      sprintf(emsg, ".%s needs data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (inz == INPUT_NUMBER_EOL)
      next_line();

    return SUCCEEDED;
  }

  /* ASCTABLE/ASCIITABLE? */

  if (strcaselesscmp(cp, "ASCTABLE") == 0 || strcaselesscmp(cp, "ASCIITABLE") == 0) {

    int astart, aend;

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
        if (compare_next_token("TO", 2) == SUCCEEDED) {
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
        if (compare_next_token("=", 1) != SUCCEEDED) {
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
      sprintf(emsg, "Error in .%s data structure.\n", bak);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    asciitable_defined = 1;

    return SUCCEEDED;
  }

  /* ASC? */

  if (strcaselesscmp(cp, "ASC") == 0) {
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
	    char tmp_a[2], *tmp_b;
	    int tmp_c;
	    
	    o += 3;
	    sprintf(tmp_a, "%c%c", label[o-1], label[o]);
	    tmp_c = strtol(tmp_a, &tmp_b, 16);
	    if (*tmp_b) {
	      sprintf(emsg, ".%s '\\x' needs hexadecimal byte (00-FF) data.\n", bak);
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
	        sprintf(emsg, ".%s '\\<' needs character data.\n", bak);
	        print_error(emsg, ERROR_INP);
	        return FAILED;
	      }
	      tmp_a = 0x80;
	      ind = label[o];
	    }
	    /* handle '\>' */
	    else if (label[o] == '\\' && label[o + 1] == '>' && o == 0) {
	      sprintf(emsg, ".%s '\\>' needs character data.\n", bak);
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
	  sprintf(emsg, ".%s needs string / byte (0-255) data.\n", bak);
	  print_error(emsg, ERROR_INP);
	  return FAILED;
	}
	ind = (int)asciitable[d];
	fprintf(file_out_ptr, "d%d ", ind);
      }
      else {
	sprintf(emsg, ".%s needs string / byte (0-255) data.\n", bak);
        print_error(emsg, ERROR_INP);
        return FAILED;
      }
    }

    return SUCCEEDED;
  }

  /* DW/WORD? */

  if (strcaselesscmp(cp, "DW") == 0 || strcaselesscmp(cp, "WORD") == 0) {
    strcpy(bak, cp);

    inz = input_number();
    for (ind = 0; inz == SUCCEEDED || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ind++) {
      if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
        sprintf(emsg, ".%s expects 16-bit data, %d is out of range!\n", bak, d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        fprintf(file_out_ptr, "y%d", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        fprintf(file_out_ptr, "C%d ", latest_stack);

      inz = input_number();
    }

    if (inz == FAILED)
      return FAILED;

    if ((inz == INPUT_NUMBER_EOL || inz == INPUT_NUMBER_STRING) && ind == 0) {
      sprintf(emsg, ".%s needs data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (inz == INPUT_NUMBER_EOL)
      next_line();

    return SUCCEEDED;
  }

#ifdef W65816
  /* DL/LONG? */

  if (strcmp(cp, "DL") == 0 || strcmp(cp, "LONG") == 0) {
    strcpy(bak, cp);

    inz = input_number();
    for (ind = 0; inz == SUCCEEDED || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK; ind++) {
      if (inz == SUCCEEDED && (d < -8388608 || d > 16777215)) {
        sprintf(emsg, ".%s expects 24-bit data, %d is out of range!\n", bak, d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      if (inz == SUCCEEDED)
        fprintf(file_out_ptr, "z%d ", d);
      else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
        fprintf(file_out_ptr, "k%d q%s ", active_file_info_last->line_current, label);
      else if (inz == INPUT_NUMBER_STACK)
        fprintf(file_out_ptr, "T%d ", latest_stack);

      inz = input_number();
    }

    if (inz == FAILED)
      return FAILED;

    if ((inz == INPUT_NUMBER_EOL || inz == INPUT_NUMBER_STRING) && ind == 0) {
      sprintf(emsg, ".%s needs data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (inz == INPUT_NUMBER_EOL)
      next_line();

    return SUCCEEDED;
  }
#endif

  /* DSTRUCT */

  if (strcaselesscmp(cp, "DSTRUCT") == 0) {

    struct structure_item *it;
    struct structure *s;
    char iname[256];
    int c, f;


    /* get instance name */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != INPUT_NUMBER_ADDRESS_LABEL) {
      print_error(".DSTRUCT needs a name for the instance.\n", ERROR_INP);
      return FAILED;
    }

    strcpy(iname, label);

    if (compare_next_token("INSTANCEOF", 10) == SUCCEEDED)
      skip_next_token();

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
      sprintf(emsg, "Reference to an unidentified structure \"%s\".\n", label);
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
	
    if (compare_next_token("DATA", 4) == SUCCEEDED)
      skip_next_token();

    fprintf(file_out_ptr, "k%d L%s ", active_file_info_last->line_current, iname);

    /* read the data */
    it = s->items;
    inz = input_number();
    for (ind = 0; it != NULL && (inz == SUCCEEDED || inz == INPUT_NUMBER_STRING || inz == INPUT_NUMBER_ADDRESS_LABEL || inz == INPUT_NUMBER_STACK); ind++) {

      fprintf(file_out_ptr, "k%d L%s.%s ", active_file_info_last->line_current, iname, it->name);

      if (it->size <= 0) {
	/* don't put data into empty structure items */
	it = it->next;
	continue;
      }
	  
      /* take care of the strings */
      if (inz == INPUT_NUMBER_STRING) {
        if (it->size < string_size) {
          sprintf(emsg, "String \"%s\" doesn't fit into the %d bytes of \"%s.%s\". Discarding the overflow.\n", label, it->size, s->name, it->name);
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
            sprintf(emsg, "\"%s.%s\" expects 8-bit data, %d is out of range!\n", s->name, it->name, d);
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
            sprintf(emsg, "\"%s.%s\" expects 16-bit data, %d is out of range!\n", s->name, it->name, d);
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
      }

      /* fill the rest of the item with emptyfill or zero */
      if (emptyfill_defined != 0)
        f = emptyfill;
      else
        f = 0;

      for (; o < it->size; o++)
        fprintf(file_out_ptr, "d%d ", f);

      it = it->next;
      inz = input_number();
    }

    /* fill the remaining items with emptyfill or zero */
    if (it != NULL) {
      if (emptyfill_defined != 0)
        f = emptyfill;
      else
        f = 0;
      while (it != NULL) {
        for (o = 0; o < it->size; o++)
          fprintf(file_out_ptr, "d%d ", f);
        it = it->next;
      }
    }

    if (inz == INPUT_NUMBER_EOL)
      next_line();
    else {
      sprintf(emsg, "Too much data for structure \"%s\".\n", s->name);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    return SUCCEEDED;
  }

  /* DS/DSB? */

  if (strcaselesscmp(cp, "DSB") == 0 || strcaselesscmp(cp, "DS") == 0) {
    strcpy(bak, cp);

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      sprintf(emsg, ".%s needs size.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (d < 1 || d > 65535) {
      sprintf(emsg, ".%s expects a 16-bit positive integer as size, %d is out of range!\n", bak, d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    inz = d;

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (!(q == SUCCEEDED || q == INPUT_NUMBER_ADDRESS_LABEL || q == INPUT_NUMBER_STACK)) {
      sprintf(emsg, ".%s needs data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (q == SUCCEEDED && (d > 255 || d < -128)) {
      sprintf(emsg, ".%s expects 8-bit data, %d is out of range!\n", bak, d);
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

  /* DSW? */

  if (strcaselesscmp(cp, "DSW") == 0) {
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".DSW needs size.\n", ERROR_INP);
      return FAILED;
    }

    if (d < 1 || d > 65535) {
      sprintf(emsg, ".DSW expects a 16-bit positive integer as size, %d is out of range!\n", d);
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
      sprintf(emsg, ".DSW expects 16-bit data, %d is out of range!\n", d);
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

  /* INCDIR */

  if (strcaselesscmp(cp, "INCDIR") == 0) {

    char *c;


    if (get_next_token() != GET_NEXT_TOKEN_STRING) {
      print_error(".INCDIR needs a directory string.\n", ERROR_DIR);
      return FAILED;
    }

    q = ss;

    /* use the default dir? */
    if (q == 0) {
      if (include_dir != NULL)
        include_dir[0] = 0;
      return SUCCEEDED;
    }

    /* use the given dir */
    o = strlen(tmp) + 2;
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
    localize_path(tmp);

    strcpy(include_dir, tmp);

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

  /* INCLUDE */

  if (strcaselesscmp(cp, "INCLUDE") == 0) {
    o = input_number();
    if (o != INPUT_NUMBER_STRING) {
      print_error(".INCLUDE needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }
    if (macro_active != 0) {
      print_error("You cannot include a file inside a MACRO.\n", ERROR_DIR);
      return FAILED;
    }

    /* convert the path to local enviroment */
    localize_path(label);

    if (include_file(label) == FAILED)
      return FAILED;

    fprintf(file_out_ptr, "f%d ", active_file_info_tmp->filename_id);

    return SUCCEEDED;
  }

  /* INCBIN */

  if (strcaselesscmp(cp, "INCBIN") == 0) {

    struct macro_static *m;
    int s, r, j;

    if (org_defined == 0 && output_format != OUTPUT_LIBRARY) {
      print_error("Before you can .INCBIN data you'll need to use ORG.\n", ERROR_LOG);
      return FAILED;
    }

    o = input_number();
    if (o != INPUT_NUMBER_STRING) {
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

      min = malloc(sizeof(struct macro_incbin));
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

  /* OUTNAME */

  if (strcaselesscmp(cp, "OUTNAME") == 0) {

    inz = input_number();

    if (inz != INPUT_NUMBER_STRING) {
      print_error(".OUTNAME needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    strcpy(final_name, label);

    return SUCCEEDED;
  }

  /* STRUCT */

  if (strcaselesscmp(cp, "STRUCT") == 0) {

    struct structure_item *si, *ss, *sl = NULL;
    struct structure *st;
    char name[512];
    int ssi = 0;


    st = malloc(sizeof(struct structure));
    if (st == NULL) {
      print_error("Out of memory while allocating a new STRUCT.\n", ERROR_DIR);
      return FAILED;
    }

    if (get_next_token() == FAILED)
      return FAILED;

    strcpy(st->name, tmp);

    st->next = structures_first;
    structures_first = st;
    st->items = NULL;

    while (1) {
      if (get_next_token() == FAILED)
        return FAILED;

      /* .IF directive? */
      if (tmp[0] == '.') {
        q = parse_if_directive();
        if (q == FAILED)
          return FAILED;
        else if (q == SUCCEEDED)
          continue;
        /* else q == -1, continue executing below */
      }

      /* end of STRUCT? */
      if (strcaselesscmp(tmp, ".ENDST") == 0) {
        /* create the SIZEOF-definition */
        st->size = ssi;
        sprintf(name, "_sizeof_%s", st->name);

        if (add_a_new_definition(name, (double)ssi, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
          return FAILED;

        /* create the structure definitions */
        ssi = 0;
        ss = st->items;
        while (ss != NULL) {
          sprintf(name, "%s.%s", st->name, ss->name);
          if (add_a_new_definition(name, (double)ssi, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
            return FAILED;
          ssi += ss->size;
          ss = ss->next;
        }

        if (st->items == NULL) {
          sprintf(emsg, "STRUCT \"%s\" is empty!\n", st->name);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        return SUCCEEDED;
      }

      if (tmp[strlen(tmp) - 1] == ':')
        tmp[strlen(tmp) - 1] = 0;

      /* check for duplicate labels */
      ss = st->items;
      while (ss != NULL) {
        if (strcmp(ss->name, tmp) == 0) {
          sprintf(emsg, "Duplicate label \"%s\" inside .STRUCT \"%s\".\n", tmp, st->name);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        ss = ss->next;
      }

      si = malloc(sizeof(struct structure_item));
      if (si == NULL) {
        print_error("Out of memory while allocating a new STRUCT.\n", ERROR_DIR);
        return FAILED;
      }
      si->next = NULL;
      strcpy(si->name, tmp);

      if (st->items == NULL)
        st->items = si;
      if (sl != NULL)
        sl->next = si;
      sl = si;

      /* get the item type */
      if (get_next_token() == FAILED)
        return FAILED;

      if (strcaselesscmp(tmp, "DB") == 0 || strcaselesscmp(tmp, "BYT") == 0 || strcaselesscmp(tmp, "BYTE") == 0)
        si->size = 1;
      else if (strcaselesscmp(tmp, "DW") == 0 || strcaselesscmp(tmp, "WORD") == 0)
        si->size = 2;
      else if (strcaselesscmp(tmp, "DS") == 0 || strcaselesscmp(tmp, "DSB") == 0) {
        q = input_number();
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          print_error("DS/DSB needs size.\n", ERROR_DIR);
          return FAILED;
        }
        si->size = d;
      }
      else if (strcaselesscmp(tmp, "DSW") == 0) {
        q = input_number();
        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED) {
          print_error("DSW needs size.\n", ERROR_DIR);
          return FAILED;
        }
        si->size = d*2;
      }
      else if (strcaselesscmp(tmp, "INSTANCEOF") == 0) {
         struct structure *stt;
         int arr = 1;
         
         /* zero the size of the root struct label */
         si->size = 0;

         /* get the structure type */
         if (get_next_token() == FAILED)
           return FAILED;
         
         stt = get_structure(tmp);
         
         if (stt != NULL) {
            int j;
			
            q = input_number();
            if (q == SUCCEEDED) {
               arr = d;
            }
            else if (q == INPUT_NUMBER_EOL)
            {
                next_line();
            }
            
            if (arr == 0) {
               /* invalid structure array size */
               sprintf(emsg, "Nested STRUCT \"%s\" in \"%s\" has an invalid array size.\n", si->name, tmp);
               print_error(emsg, ERROR_DIR);
               return FAILED;
            }
            
            for (j = 0; j < arr; j++) {
               struct structure_item *sti = stt->items;
               
               while (sti != NULL) {
                  struct structure_item *sj = NULL;
                  
                  if (j == 0 && arr > 1) {
                     /* add 0 size labels for the first elements */
                     sj = malloc(sizeof(struct structure_item));
                     if (sj == NULL) {
                       print_error("Out of memory while allocating a nested STRUCT.\n", ERROR_DIR);
                       return FAILED;
                     }
                     sj->next = NULL;
                     sprintf(sj->name, "%s.%s", si->name, sti->name);
                     sj->size = 0;
                     
                     if (sl != NULL)
                       sl->next = sj;
                     sl = sj;
                  }
                  
                  /* add items for each of the structure item entries. combine si->name with the structure item name. */
                  sj = malloc(sizeof(struct structure_item));
                  if (sj == NULL) {
                    print_error("Out of memory while allocating a nested STRUCT.\n", ERROR_DIR);
                    return FAILED;
                  }
                  sj->next = NULL;
                  if (arr == 1)
                     sprintf(sj->name, "%s.%s", si->name, sti->name);
                  else
                     sprintf(sj->name, "%s.%i.%s", si->name, j + 1, sti->name);
                  sj->size = sti->size;
                  
                  if (sl != NULL)
                    sl->next = sj;
                  sl = sj;
                  
                  sti = sti->next;
               }
               
               /* adjust the total struct offset */
               ssi += stt->size;
            }
         }
         else {
            /* failed to find structure */
            sprintf(emsg, "Failed to find nested STRUCT \"%s\" type \"%s\" in .STRUCT.\n", si->name, tmp);
            print_error(emsg, ERROR_DIR);
            return FAILED;
         }
      }
      else if (strcaselesscmp(tmp, ".db") == 0 || strcaselesscmp(tmp, ".dw") == 0) {
        si->size = 0;
        continue;
      }
      else {
        sprintf(emsg, "Unexpected symbol \"%s\" in .STRUCT.\n", tmp);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }
      ssi += si->size;
    }

    return FAILED;
  }

  /* RAMSECTION */

  if (strcaselesscmp(cp, "RAMSECTION") == 0) {

    if (output_format == OUTPUT_LIBRARY) {
      print_error("Libraries don't take RAMSECTIONs.\n", ERROR_DIR);
      return FAILED;
    }
    if (section_status == ON) {
      sprintf(emsg, "There is already an open section called \"%s\".", sections_last->name);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (get_next_token() == FAILED)
      return FAILED;

    sec_tmp = calloc(sizeof(struct section_def), 1);
    if (sec_tmp == NULL) {
      sprintf(emsg, "Out of memory while allocating room for a new RAMSECTION \"%s\".\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    sec_tmp->listfile_items = 0;
    sec_tmp->listfile_ints = NULL;
    sec_tmp->listfile_cmds = NULL;
    sec_tmp->maxsize_status = OFF;
    sec_tmp->status = SECTION_STATUS_RAM;
    sec_tmp->alive = ON;
    sec_tmp->data = NULL;
    sec_tmp->filename_id = active_file_info_last->filename_id;
    sec_tmp->id = section_id;
    sec_tmp->alignment = 1;
    sec_tmp->advance_org = YES;
    sec_tmp->nspace = NULL;
    section_id++;

    strcpy(sec_tmp->name, tmp);
    sec_tmp->next = NULL;

    /* look for duplicate sections */
    sec_next = sections_first;
    while (sec_next != NULL) {
      if (strcmp(sec_next->name, tmp) == 0) {
        sprintf(emsg, "RAMSECTION \"%s\" was defined for the second time.\n", tmp);
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
    if (compare_next_token("BANK", 4) != SUCCEEDED)
      sec_tmp->bank = 0;
    else {
      skip_next_token();

      q = input_number();

      if (q == FAILED)
        return FAILED;
      if (q != SUCCEEDED || d < 0) {
        print_error("BANK number must be zero or positive.\n", ERROR_DIR);
        return FAILED;
      }

      if (rombanks <= d && output_format != OUTPUT_LIBRARY) {
        sprintf(emsg, "ROM banks == %d, selected bank %d.\n", rombanks, d);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      sec_tmp->bank = d;
    }

    if (compare_next_token("SLOT", 4) != SUCCEEDED) {
      if (get_next_token() == FAILED)
        return FAILED;
      sprintf(emsg, "Unknown symbol \"%s\".\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    skip_next_token();

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d > 255 || d < 0) {
      print_error(".RAMSECTION needs an unsigned 8-bit value as the SLOT number.\n", ERROR_DIR);
      return FAILED;
    }

    if (slots[d].size == 0) {
      sprintf(emsg, "There is no SLOT number %d.\n", d);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    sec_tmp->slot = d;
    fprintf(file_out_ptr, "S%d ", sec_tmp->id);

    /* align the ramsection? */
    if (compare_next_token("ALIGN", 5) == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      inz = input_number();
      if (inz != SUCCEEDED) {
        print_error("Could not parse the .RAMSECTION alignment.\n", ERROR_DIR);
        return FAILED;
      }

      sec_tmp->alignment = d;
    }

    in_ramsection = 1;

    return SUCCEEDED;
  }

  /* SECTION */

  if (strcaselesscmp(cp, "SECTION") == 0) {

    int l, m;


    if (section_status == ON) {
      sprintf(emsg, "There is already an open section called \"%s\".", sections_last->name);
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
      sprintf(emsg, "Out of memory while allocating room for a new SECTION \"%s\".\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    sec_tmp->listfile_items = 0;
    sec_tmp->listfile_ints = NULL;
    sec_tmp->listfile_cmds = NULL;
    sec_tmp->maxsize_status = OFF;
    sec_tmp->data = NULL;
    sec_tmp->alignment = 1;
    sec_tmp->advance_org = YES;
    sec_tmp->nspace = NULL;

    /* check if the section size is supplied inside the name */
    l = strlen(tmp) - 1;

    for (; l >= 0 && tmp[l] != '_'; l--)
      ;

    if (l >= 0 && tmp[l] == '_') {
      l++;
      if (tmp[l] == '$') {
        for (l++, m = 0; tmp[l] != 0; l++) {
          if (tmp[l] >= '0' && tmp[l] <= '9')
            m = (m << 4) + tmp[l] - '0';
          else if (tmp[l] >= 'a' && tmp[l] <= 'f')
            m = (m << 4) + tmp[l] - 'a' + 10;
          else if (tmp[l] >= 'A' && tmp[l] <= 'F')
            m = (m << 4) + tmp[l] - 'A' + 10;
          else
            break;
        }

        if (tmp[l] == 0) {
          sec_tmp->maxsize_status = ON;
          sec_tmp->maxsize = m;
        }
      }
      else if (tmp[l] >= '0' && tmp[l] <= '9') {
        for (m = 0; tmp[l] != 0; l++) {
          if (tmp[l] >= '0' && tmp[l] <= '9')
            m = (m * 10) + tmp[l] - '0';
          else
            break;
        }

        if (tmp[l] == 0) {
          sec_tmp->maxsize_status = ON;
          sec_tmp->maxsize = m;
        }
      }
    }

    if (strcmp(tmp, "BANKHEADER") == 0) {
      no_library_files("bank header sections");
      
      sec_next = sections_first;
      while (sec_next != NULL) {
        if (strcmp(sec_next->name, tmp) == 0 && sec_next->bank == bank) {
          sprintf(emsg, "BANKHEADER section was defined for the second time for bank %d.\n", bank);
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
          sprintf(emsg, "SECTION \"%s\" was defined for the second time.\n", tmp);
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

    if (compare_next_token("NAMESPACE", 9) == SUCCEEDED) {
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

    /* the size of the section? */
    if (compare_next_token("SIZE", 4) == SUCCEEDED) {
      if (sec_tmp->maxsize_status == ON) {
        print_error("The SIZE of the section has already been defined.\n", ERROR_DIR);
        return FAILED;
      }

      if (skip_next_token() == FAILED)
        return FAILED;

      inz = input_number();
      if (inz != SUCCEEDED) {
        print_error("Could not parse the SIZE.\n", ERROR_DIR);
        return FAILED;
      }

      sec_tmp->maxsize_status = ON;
      sec_tmp->maxsize = d;
    }

    /* align the section? */
    if (compare_next_token("ALIGN", 5) == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      inz = input_number();
      if (inz != SUCCEEDED) {
        print_error("Could not parse the .SECTION alignment.\n", ERROR_DIR);
        return FAILED;
      }

      sec_tmp->alignment = d;
    }

    /* the type of the section */
    if (compare_next_token("FORCE", 5) == SUCCEEDED) {
      if (output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take FORCE sections.\n", ERROR_DIR);
        return FAILED;
      }
      sec_tmp->status = SECTION_STATUS_FORCE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("FREE", 4) == SUCCEEDED) {
      sec_tmp->status = SECTION_STATUS_FREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SUPERFREE", 9) == SUCCEEDED) {
      sec_tmp->status = SECTION_STATUS_SUPERFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMIFREE", 8) == SUCCEEDED) {
      if (output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take SEMIFREE sections.\n", ERROR_DIR);
        return FAILED;
      }
      sec_tmp->status = SECTION_STATUS_SEMIFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("SEMISUBFREE", 11) == SUCCEEDED) {
      if (output_format == OUTPUT_LIBRARY) {
        print_error("Libraries don't take SEMISUBFREE sections.\n", ERROR_DIR);
        return FAILED;
      }
      sec_tmp->status = SECTION_STATUS_SEMISUBFREE;
      if (skip_next_token() == FAILED)
        return FAILED;
    }
    else if (compare_next_token("OVERWRITE", 9) == SUCCEEDED) {
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
    if (compare_next_token("RETURNORG", 9) == SUCCEEDED) {
      if (skip_next_token() == FAILED)
        return FAILED;

      sec_tmp->advance_org = NO;
    }

    if (compare_next_token("APPENDTO", 8) == SUCCEEDED) {
      struct append_section *append_tmp;
	
      if (skip_next_token() == FAILED)
        return FAILED;

      append_tmp = calloc(sizeof(struct append_section), 1);
      if (append_tmp == NULL) {
	sprintf(emsg, "Out of memory while allocating room for a new APPENDTO \"%s\".\n", tmp);
	print_error(emsg, ERROR_DIR);
	return FAILED;
      }
      
      /* get the target section name */
      if (get_next_token() == FAILED)
	return FAILED;

      strcpy(append_tmp->section, sec_tmp->name);
      strcpy(append_tmp->append_to, tmp);

      append_tmp->next = append_sections;
      append_sections = append_tmp;
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

    return SUCCEEDED;
  }

  /* FOPEN */

  if (strcaselesscmp(cp, "FOPEN") == 0) {

    struct filepointer *f;
    char *c;


    o = input_number();
    if (o != INPUT_NUMBER_STRING) {
      print_error(".FOPEN needs a file name string.\n", ERROR_DIR);
      return FAILED;
    }

    /* convert the path to local enviroment */
    localize_path(label);

    c = malloc(strlen(label) + 1);
    if (c == NULL) {
      print_error("Out of memory error.\n", ERROR_DIR);
      return FAILED;
    }
    strcpy(c, label);

    /* get the file pointer name */
    if (get_next_token() == FAILED)
      return FAILED;

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
      f = malloc(sizeof(struct filepointer));
      if (f == NULL) {
        print_error("Out of memory error.\n", ERROR_DIR);
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
      sprintf(emsg, "Error opening file \"%s\" for reading.\n", f->filename);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    return SUCCEEDED;
  }

  /* FCLOSE */

  if (strcaselesscmp(cp, "FCLOSE") == 0) {

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
      sprintf(emsg, "Couldn't find filepointer \"%s\".\n", tmp);
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

  /* FSIZE */

  if (strcaselesscmp(cp, "FSIZE") == 0) {

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
      sprintf(emsg, "Couldn't find filepointer \"%s\".\n", tmp);
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

    add_a_new_definition(tmp, (double)b, NULL, DEFINITION_TYPE_VALUE, 0);

    return SUCCEEDED;
  }

  /* FREAD */

  if (strcaselesscmp(cp, "FREAD") == 0) {

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
      sprintf(emsg, "Couldn't find filepointer \"%s\".\n", tmp);
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

  /* BLOCK */

  if (strcaselesscmp(cp, "BLOCK") == 0) {
    if ((ind = get_next_token()) == FAILED)
      return FAILED;

    if (ind != GET_NEXT_TOKEN_STRING) {
      print_error(".BLOCK requires a name string.\n", ERROR_DIR);
      return FAILED;
    }

    block_status++;

    fprintf(file_out_ptr, "k%d ", active_file_info_last->line_current);
    fprintf(file_out_ptr, "g%s ", tmp);

    return SUCCEEDED;
  }

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

  if (strcaselesscmp(cp, "SHIFT") == 0) {

    struct macro_argument *ma;
    struct macro_runtime *rt;
    struct macro_static *st;


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
        redefine(st->argument_names[q], (double)ma->value, NULL, DEFINITION_TYPE_VALUE, 0);
      else if (ma->type == INPUT_NUMBER_STACK)
        redefine(st->argument_names[q], (double)ma->value, NULL, DEFINITION_TYPE_STACK, 0);
      else if (ma->type == INPUT_NUMBER_ADDRESS_LABEL)
        redefine(st->argument_names[q], 0.0, ma->string, DEFINITION_TYPE_ADDRESS_LABEL, strlen(ma->string));
      else if (ma->type == INPUT_NUMBER_STRING)
        redefine(st->argument_names[q], 0.0, ma->string, DEFINITION_TYPE_STRING, strlen(ma->string));
    }

    /* redefine NARGS */
    if (redefine("NARGS", (double)rt->supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;
    if (redefine("nargs", (double)rt->supplied_arguments, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
      return FAILED;

    return SUCCEEDED;
  }

#ifdef GB
  /* NINTENDOLOGO */
  
  if (strcaselesscmp(cp, "NINTENDOLOGO") == 0) {
    no_library_files(".NINTENDOLOGO");

    nintendologo_defined++;

    return SUCCEEDED;
  }
  
  /* NAME */

  if (strcaselesscmp(cp, "NAME") == 0) {
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

      for ( ; ind < 16; name[ind] = 0, ind++);

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

#ifdef W65816
  /* NAME */

  if (strcaselesscmp(cp, "NAME") == 0) {
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

      for ( ; ind < 21; name[ind] = 0, ind++);

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

  /* ENDS */

  if (strcaselesscmp(cp, "ENDS") == 0) {
    if (section_status == OFF) {
      print_error("There is no open section.\n", ERROR_DIR);
      return FAILED;
    }

    section_status = OFF;
    bankheader_status = OFF;
    fprintf(file_out_ptr, "s ");

    return SUCCEEDED;
  }

  /* ROMBANKS */

  if (strcaselesscmp(cp, "ROMBANKS") == 0) {
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
        d != 128 && d != 256 && d != 72 && d != 80 && d != 96) {
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

      sprintf(emsg, "Upgrading from %d to %d ROM banks.\n", rombanks, d);
      print_error(emsg, ERROR_WRN);
    }

    rombanks = d;
    rombanks_defined = 1;
    max_address = d * banksize;

    if (rom_banks != NULL)
      free(rom_banks);
    if (rom_banks_usage_table != NULL)
      free(rom_banks_usage_table);

    rom_banks = malloc(sizeof(unsigned char) * max_address);
    rom_banks_usage_table = malloc(sizeof(unsigned char) * max_address);
    if (rom_banks == NULL || rom_banks_usage_table == NULL) {
      print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
      return FAILED;
    }

    memset(rom_banks_usage_table, 0, sizeof(unsigned char) * max_address);

    if (banks != NULL)
      free(banks);
    if (bankaddress != NULL)
      free(bankaddress);

    banks = malloc(sizeof(int) * rombanks);
    bankaddress = malloc(sizeof(int) * rombanks);
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

  /* ROMBANKMAP */

  if (strcaselesscmp(cp, "ROMBANKMAP") == 0) {

    int b = 0, a = 0, bt = 0, bt_defined = 0, x, bs = 0, bs_defined = 0;


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

          banks = malloc(sizeof(int) * d);
          bankaddress = malloc(sizeof(int) * d);
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
        b != 128 && b != 256 && b != 72 && b != 80 && b != 96) {
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
    else if (b == 72)
      romtype = 0x52;
    else if (b == 80)
      romtype = 0x53;
    else if (b == 96)
      romtype = 0x54;
#endif

    if (rombanks_defined != 0) {
      if (b > rombanks) {
        sprintf(emsg, "Upgrading from %d to %d ROM banks.\n", rombanks, b);
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

    rom_banks = malloc(sizeof(unsigned char) * max_address);
    rom_banks_usage_table = malloc(sizeof(unsigned char) * max_address);
    if (rom_banks == NULL || rom_banks_usage_table == NULL) {
      print_error("Out of memory while allocating ROM banks.\n", ERROR_DIR);
      return FAILED;
    }

    memset(rom_banks_usage_table, 0, sizeof(unsigned char) * max_address);
    rombankmap_defined = 1;

    return SUCCEEDED;
  }

  /* MEMORYMAP */

  if (strcaselesscmp(cp, "MEMORYMAP") == 0) {

    int slotsize = 0, slotsize_defined = 0, s = 0;


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
          sprintf(emsg, "Unknown DEFAULTSLOT %d.\n", defaultslot);
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
        if (compare_next_token("START", 5) == SUCCEEDED)
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
        if (compare_next_token("SIZE", 4) == SUCCEEDED)
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
          if (q != SUCCEEDED) {
            print_error("The size of the slot needs to be an immediate value.\n", ERROR_DIR);
            return FAILED;
          }
        }

        slots[o].size = d;
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

  /* UNBACKGROUND */

  if (strcaselesscmp(cp, "UNBACKGROUND") == 0) {

    int start, end;


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
      sprintf(emsg, "The block's starting address $%x is beyond the ROM's ending address $%x.\n", start, max_address-1);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }
    if (end >= max_address) {
      sprintf(emsg, "The block's ending address $%x is beyond the ROM's ending address $%x. Using the ROM's ending address instead.\n", end, max_address-1);
      end = max_address;
      print_error(emsg, ERROR_WRN);
    }

    /* clear the memory [start, end] */
    memset(rom_banks + start, 0, end - start + 1);
    memset(rom_banks_usage_table + start, 0, end - start + 1);

    return SUCCEEDED;
  }

  /* BACKGROUND */

  if (strcaselesscmp(cp, "BACKGROUND") == 0) {

    FILE *file_in_ptr;


    if (output_format != OUTPUT_OBJECT) {
      print_error(".BACKGROUND can only be used in OBJECT output mode.\n", ERROR_DIR);
      return FAILED;
    }
    if (background_defined == 1) {
      print_error("Only one .BACKGROUND can be specified.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != INPUT_NUMBER_STRING) {
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
      sprintf(emsg, "Error opening .BACKGROUND file \"%s\".\n", full_name);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    fseek(file_in_ptr, 0, SEEK_END);
    background_size = ftell(file_in_ptr);
    fseek(file_in_ptr, 0, SEEK_SET);

    if (max_address != background_size) {
      sprintf(emsg, ".BACKGROUND file \"%s\" size (%d) and ROM size (%d) don't match.\n", full_name, background_size, max_address);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    memset(rom_banks_usage_table, 2, max_address);
    fread(rom_banks, 1, max_address, file_in_ptr);

    background_defined = 1;
    fclose(file_in_ptr);

    return SUCCEEDED;
  }

#ifdef GB
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
      return SUCCEEDED;
    }

    if (d != 0 && d != 1 && d != 2 && d != 3 && d != 4) {
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
      return SUCCEEDED;
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

    if (countrycode_defined != 0) {
      if (countrycode != d) {
        print_error(".DESTINATIONCODE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
    }

    countrycode = d;
    countrycode_defined = 1;

    return SUCCEEDED;
  }

  /* CARTRIDGETYPE */

  if (strcaselesscmp(cp, "CARTRIDGETYPE") == 0) {

    no_library_files(".CARTRIDGETYPE");

    q = input_number();

    if (cartridgetype_defined != 0) {
      if (cartridgetype != d) {
        print_error(".CARTRIDGETYPE was defined for the second time.\n", ERROR_DIR);
        return FAILED;
      }
      return SUCCEEDED;
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
      sprintf(emsg, ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", d);
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

  /* GBHEADER */

  if (strcmp(cp, "GBHEADER") == 0) {
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
        romdmg++;
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

          for ( ; ind < 16; name[ind] = 0, ind++);

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
	  sprintf(emsg, ".LICENSEECODEOLD needs a 8-bit value, got %d.\n", d);
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

	  licenseecodenew_c1 = tmp[0];
	  licenseecodenew_c2 = tmp[1];
	  licenseecodenew_defined = 1;
	}
      }
      else if (strcaselesscmp(tmp, "CARTRIDGETYPE") == 0) {
        if (cartridgetype_defined != 0) {
          print_error("CARTRIDGETYPE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "CARTRIDGETYPE needs a 8-bit value, got %d.\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED) {
          cartridgetype = d;
          cartridgetype_defined++;
        }

        else return FAILED;
      }
      else if (strcaselesscmp(tmp, "RAMSIZE") == 0) {
        if (rambanks != 0) {
          print_error("RAMSIZE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "RAMSIZE needs a 8-bit value, got %d.\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED)
          rambanks = d;
        else
          return FAILED;
      }
      else if (strcaselesscmp(tmp, "COUNTRYCODE") == 0) {
        if (countrycode_defined != 0) {
          print_error("COUNTRYCODE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "COUNTRYCODE needs a non-negative value, got %d.\n\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED) {
          countrycode = d;
          countrycode_defined++;
        }
        else
          return FAILED;
      }
      else if (strcaselesscmp(tmp, "DESTINATIONCODE") == 0) {
        if (countrycode_defined != 0) {
          print_error("DESTINATIONCODE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "DESTINATIONCODE needs a non-negative value, got %d.\n\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED) {
          countrycode = d;
          countrycode_defined++;
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

  /* EMPTYFILL */

  if (strcaselesscmp(cp, "EMPTYFILL") == 0) {
    no_library_files(".EMPTYFILL");

    q = input_number();

    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED || d < -128 || d > 255) {
      sprintf(emsg, ".EMPTYFILL needs a 8-bit value, got %d.\n", d);
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

  if (strcaselesscmp(cp, "DEFINE") == 0 || strcaselesscmp(cp, "DEF") == 0 || strcaselesscmp(cp, "EQU") == 0) {

    double dou;
    char k[256];
    int j, size;

    if (get_next_token() == FAILED)
      return FAILED;

    /* check the user doesn't try to define reserved labels */
    if (strcmp(tmp, "WLA_TIME") == 0 || strcmp(tmp, "wla_time") == 0 ||
        strcmp(tmp, "WLA_VERSION") == 0 || strcmp(tmp, "wla_version") == 0 ||
        strcmp(tmp, "WLA_FILENAME") == 0 || strcmp(tmp, "wla_filename") == 0 ||
        strcmp(tmp, "NARGS") == 0 || strcmp(tmp, "nargs") == 0 ||
        strcmp(tmp, "CADDR") == 0 || strcmp(tmp, "caddr") == 0) {

      sprintf(emsg, "\"%s\" is a reserved definition label and is not user definable.\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* skip "=", if present */
    if (compare_next_token("=", 1) == SUCCEEDED)
      skip_next_token();

    input_float_mode = ON;
    q = get_new_definition_data(&j, k, &size, &dou);
    input_float_mode = OFF;
    if (q == FAILED)
      return FAILED;

    if (!(q == INPUT_NUMBER_EOL || q == INPUT_NUMBER_FLOAT || q == SUCCEEDED || q == INPUT_NUMBER_STRING || q == INPUT_NUMBER_STACK)) {
      print_error("Could not parse the definition data.\n", ERROR_DIR);
      return FAILED;
    }

    if (q == SUCCEEDED)
      add_a_new_definition(tmp, (double)j, NULL, DEFINITION_TYPE_VALUE, 0);
    else if (q == INPUT_NUMBER_FLOAT)
      add_a_new_definition(tmp, dou, NULL, DEFINITION_TYPE_VALUE, 0);
    else if (q == INPUT_NUMBER_STRING)
      add_a_new_definition(tmp, 0.0, k, DEFINITION_TYPE_STRING, size);
    else if (q == INPUT_NUMBER_STACK)
      add_a_new_definition(tmp, (double)j, NULL, DEFINITION_TYPE_STACK, 0);
    else if (q == INPUT_NUMBER_EOL)
      add_a_new_definition(tmp, 0, NULL, DEFINITION_TYPE_VALUE, 0);

    return SUCCEEDED;
  }

  /* INPUT */

  if (strcaselesscmp(cp, "INPUT") == 0) {

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
    redefine(tmp, 0.0, k, DEFINITION_TYPE_STRING, strlen(k));

    return SUCCEEDED;
  }

  /* REDEFINE/REDEF */

  if (strcaselesscmp(cp, "REDEFINE") == 0 || strcaselesscmp(cp, "REDEF") == 0) {

    double dou;
    char k[256];
    int j, size;


    if (get_next_token() == FAILED)
      return FAILED;

    /* check the user doesn't try to define reserved labels */
    if (strcmp(tmp, "WLA_TIME") == 0 || strcmp(tmp, "wla_time") == 0 ||
        strcmp(tmp, "WLA_VERSION") == 0 || strcmp(tmp, "wla_version") == 0 ||
        strcmp(tmp, "WLA_FILENAME") == 0 || strcmp(tmp, "wla_filename") == 0 ||
        strcmp(tmp, "NARGS") == 0 || strcmp(tmp, "nargs") == 0 ||
        strcmp(tmp, "CADDR") == 0 || strcmp(tmp, "caddr") == 0) {

      sprintf(emsg, "\"%s\" is a reserved definition label and is not user definable.\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* skip "=", if present */
    if (compare_next_token("=", 1) == SUCCEEDED)
      skip_next_token();

    input_float_mode = ON;
    q = get_new_definition_data(&j, k, &size, &dou);
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

    return SUCCEEDED;
  }

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

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      print_error(".ENUM needs a starting address.\n", ERROR_DIR);
      return FAILED;
    }

    enum_offset = d;

    /* "ASC" or "DESC"? */
    if (compare_next_token("ASC", 3) == SUCCEEDED) {
      enum_ord = 1;
      skip_next_token();
    }
    else if (compare_next_token("DESC", 4) == SUCCEEDED) {
      enum_ord = -1;
      skip_next_token();
    }
    else
      enum_ord = 1;

    /* do we have "EXPORT" defined? */
    if (compare_next_token("EXPORT", 6) == SUCCEEDED) {
      skip_next_token();
      enum_exp = YES;
    }
    else
      enum_exp = NO;

    in_enum = 1;

    return SUCCEEDED;
  }

#ifdef GB
  /* COMPUTEGBCHECKSUM */

  if (strcaselesscmp(cp, "COMPUTECHECKSUM") == 0 || strcaselesscmp(cp, "COMPUTEGBCHECKSUM") == 0) {

    no_library_files(".COMPUTEGBCHECKSUM");
    
    if (gbheader_defined != 0) {
      print_error(".COMPUTEGBCHECKSUM is unnecessary when GBHEADER is defined.\n", ERROR_WRN);
    }

    computechecksum_defined = 1;

    return SUCCEEDED;
  }

  /* COMPUTEGBCOMPLEMENTCHECK */

  if (strcaselesscmp(cp, "COMPUTEGBCOMPLEMENTCHECK") == 0 || strcaselesscmp(cp, "COMPUTECOMPLEMENTCHECK") == 0) {

    no_library_files(".COMPUTEGBCOMPLEMENTCHECK");
    
    if (gbheader_defined != 0) {
      print_error(".COMPUTEGBCOMPLEMENTCHECK is unnecessary when GBHEADER is defined.\n", ERROR_WRN);
    }

    computecomplementcheck_defined = 1;

    return SUCCEEDED;
  }
#endif

#ifdef W65816
  /* COMPUTESNESCHECKSUM */

  if (strcaselesscmp(cp, "COMPUTESNESCHECKSUM") == 0) {

    no_library_files(".COMPUTESNESCHECKSUM");
    
    if (hirom_defined == 0 && lorom_defined == 0) {
      print_error(".COMPUTESNESCHECKSUM needs either .LOROM or .HIROM.\n", ERROR_DIR);
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
  if (strcmp(cp, "SMSHEADER") == 0) {
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
	  sprintf(emsg, "VERSION needs a value between 0 and 15, got %d.\n", d);
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
      else if (strcaselesscmp(tmp, "REGIONCODE") == 0) {
        q = input_number();

        if (q == FAILED)
          return FAILED;
        if (q != SUCCEEDED || d < 3|| d > 7) {
	  sprintf(emsg, "REGIONCODE needs a value between 3 and 7, got %d.\n", d);
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
  
  /* SDSCTAG */

  if (strcaselesscmp(cp, "SDSCTAG") == 0) {

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

  /* MACRO */

  if (strcaselesscmp(cp, "MACRO") == 0) {

    struct macro_static *m;
    int macro_start_line;


    if (get_next_token() == FAILED)
      return FAILED;

    if (strcaselesscmp(cp, "ENDM") == 0) {
      print_error("A MACRO must have a name.\n", ERROR_DIR);
      return FAILED;
    }

    macro_start_line = active_file_info_last->line_current;

    m = macro_get(tmp);
    if (m != NULL) {
      sprintf(emsg, "MACRO \"%s\" was defined for the second time.\n", tmp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    m = malloc(sizeof(struct macro_static));
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
    if (compare_next_token("ARGS", 4) == SUCCEEDED) {
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
          sprintf(emsg, "MACRO \"%s\" is missing argument names?\n", m->name);
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
        m->argument_names[q-1] = malloc(strlen(tmp)+1);
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
        sprintf(emsg, "MACRO \"%s\" wasn't terminated with .ENDM.\n", m->name);
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

  /* REPT/REPEAT */

  if (strcaselesscmp(cp, "REPT") == 0 || strcaselesscmp(cp, "REPEAT") == 0) {

    char c[16], index_name[MAX_NAME_LENGTH];


    strcpy(c, cp);

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      sprintf(emsg, ".%s needs a count.\n", c);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    if (d < 0) {
      sprintf(emsg, ".%s count value must be positive or zero.\n", c);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    index_name[0] = 0;
    if (compare_next_token("INDEX", 5) == SUCCEEDED) {
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
      sprintf(emsg, ".%s must end to .ENDR.\n", c);
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

    return SUCCEEDED;
  }

  /* ENDR */

  if (strcaselesscmp(cp, "ENDR") == 0) {

    struct repeat_runtime *rr = NULL;
    
    if (repeat_active == 0) {
      print_error("There is no open repetition.\n", ERROR_DIR);
      return FAILED;
    }

    rr = &repeat_stack[repeat_active - 1];

    rr->counter--;
    if (rr->counter == 0) {
      repeat_active--;
      if (strlen(rr->index_name) > 0) {
          if (undefine(rr->index_name) == FAILED)
              return FAILED;
      }
      return SUCCEEDED;
    }

    rr->repeats++;
    if (strlen(rr->index_name) > 0) {
      if (redefine(rr->index_name, rr->repeats, NULL, DEFINITION_TYPE_VALUE, 0) == FAILED)
	return FAILED;
    }
    
    i = rr->start;
    active_file_info_last->line_current = rr->start_line;

    return SUCCEEDED;
  }

  /* E */

  if (strcaselesscmp(cp, "E") == 0) {
    if (active_file_info_last != NULL) {
      active_file_info_tmp = active_file_info_last;
      active_file_info_last = active_file_info_last->prev;
      free(active_file_info_tmp);
      if (active_file_info_last == NULL)
        active_file_info_first = NULL;
      else
        fprintf(file_out_ptr, "f%d ", active_file_info_last->filename_id);
    }
    fprintf(file_out_ptr, "E ");
    open_files--;
    if (open_files == 0)
      return EVALUATE_TOKEN_EOP;

    if (extra_definitions == ON) {
      redefine("WLA_FILENAME", 0.0, get_file_name(active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
          strlen(get_file_name(active_file_info_last->filename_id)));
      redefine("wla_filename", 0.0, get_file_name(active_file_info_last->filename_id), DEFINITION_TYPE_STRING,
          strlen(get_file_name(active_file_info_last->filename_id)));
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

    romdmg++;

    return SUCCEEDED;
  }

  /* ROMSGB */

  if (strcaselesscmp(cp, "ROMSGB") == 0) {
    no_library_files(".ROMSGB");
    
    if (romdmg != 0) {
      print_error(".ROMDMG and .ROMSGB cannot be mixed.\n", ERROR_DIR);
      return FAILED;
    }

    romsgb++;

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

  if (strcaselesscmp(cp, "ENDM") == 0) {
    if (macro_active != 0) {
      macro_active--;

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

      i = macro_stack[macro_active].macro_end;

      if ((extra_definitions == ON) && (active_file_info_last->filename_id != macro_stack[macro_active].macro_end_filename_id)) {
        redefine("WLA_FILENAME", 0.0, get_file_name(macro_stack[macro_active].macro_end_filename_id), DEFINITION_TYPE_STRING,
            strlen(get_file_name(macro_stack[macro_active].macro_end_filename_id)));
        redefine("wla_filename", 0.0, get_file_name(macro_stack[macro_active].macro_end_filename_id), DEFINITION_TYPE_STRING,
            strlen(get_file_name(macro_stack[macro_active].macro_end_filename_id)));
      }

      active_file_info_last->filename_id = macro_stack[macro_active].macro_end_filename_id;
      active_file_info_last->line_current = macro_stack[macro_active].macro_end_line;

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

    sprintf(emsg, "No .MACRO open.\n");
    print_error(emsg, ERROR_DIR);
    return FAILED;
  }

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

#if defined(MCS6502) || defined(MCS6510) || defined(W65816) || defined(WDC65C02) || defined(HUC6280)

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
    
    if (lorom_defined != 0) {
      print_error(".LOROM was defined prior to .HIROM.\n", ERROR_DIR);
      return FAILED;
    }

    hirom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

  /* LOROM */

  if (strcaselesscmp(cp, "LOROM") == 0) {
    no_library_files(".LOROM");
    
    if (hirom_defined != 0) {
      print_error(".HIROM was defined prior to .LOROM.\n", ERROR_DIR);
      return FAILED;
    }

    lorom_defined++;
    snes_mode++;

    return SUCCEEDED;
  }

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

  if (strcaselesscmp(cp, "SNESHEADER") == 0) {
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

          for ( ; ind < 4; snesid[ind] = 0, ind++);

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

          for ( ; ind < 21; name[ind] = 0, ind++);

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
        if (lorom_defined != 0) {
          print_error(".LOROM was defined prior to .HIROM.\n", ERROR_DIR);
          return FAILED;
        }

        hirom_defined++;
      }
      else if (strcaselesscmp(tmp, "LOROM") == 0) {
        if (hirom_defined != 0) {
          print_error(".HIROM was defined prior to .LOROM.\n", ERROR_DIR);
          return FAILED;
        }

        lorom_defined++;
      }
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
        if (cartridgetype_defined != 0) {
          print_error("CARTRIDGETYPE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "CARTRIDGETYPE expects 8-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED) {
          cartridgetype = d;
          cartridgetype_defined++;
        }

        else return FAILED;
      }
      else if (strcaselesscmp(tmp, "ROMSIZE") == 0) {
        if (snesromsize != 0) {
          print_error("ROMSIZE can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "ROMSIZE expects 8-bit data, %d is out of range!\n", d);
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

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "SRAMSIZE expects 8-bit data, %d is out of range!\n", d);
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
          sprintf(emsg, "COUNTRY expects 8-bit data, %d is out of range!\n", d);
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
          sprintf(emsg, "LICENSEECODE expects 8-bit data, %d is out of range!\n", d);
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
        if (version_defined != 0) {
          print_error("VERSION can be defined only once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -128 || d > 255)) {
          sprintf(emsg, "VERSION expects 8-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }
        else if (inz == SUCCEEDED) {
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

  /* SNESNATIVEVECTOR */

  if (strcaselesscmp(cp, "SNESNATIVEVECTOR") == 0) {

    int cop_defined = 0, brk_defined = 0, abort_defined = 0;
    int nmi_defined = 0, unused_defined = 0, irq_defined = 0;
    char cop[512], brk[512], abort[512], nmi[512], unused[512], irq[512];


    if (snesnativevector_defined != 0) {
      print_error(".SNESNATIVEVECTOR can be defined only once.\n", ERROR_DIR);
      return FAILED;
    }
    if (hirom_defined == 0 && lorom_defined == 0) {
      print_error(".SNESNATIVEVECTOR needs either .LOROM or .HIROM.\n", ERROR_DIR);
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
    fprintf(file_out_ptr, "P O0 A%d %d ", sec_tmp->id, 0x7FE4 + (hirom_defined ? 0x8000 : 0x0000));
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
          sprintf(cop, "y%d ", 0x0000);
        if (brk_defined == 0)
          sprintf(brk, "y%d ", 0x0000);
        if (abort_defined == 0)
          sprintf(abort, "y%d ", 0x0000);
        if (nmi_defined == 0)
          sprintf(nmi, "y%d ", 0x0000);
        if (unused_defined == 0) 
          sprintf(unused, "y%d ", 0x0000);
        if (irq_defined == 0)
          sprintf(irq, "y%d ", 0x0000);

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
          sprintf(emsg, "COP expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(cop, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(cop, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(cop, "C%d ", latest_stack);

        cop_defined++;
      }
      else if (strcaselesscmp(tmp, "BRK") == 0) {
        if (brk_defined != 0) {
          print_error("BRK can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "BRK expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(brk, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(brk, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(brk, "C%d ", latest_stack);

        brk_defined++;
      }
      else if (strcaselesscmp(tmp, "ABORT") == 0) {
        if (abort_defined != 0) {
          print_error("ABORT can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "ABORT expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(abort, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(abort, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(abort, "C%d ", latest_stack);

        abort_defined++;
      }
      else if (strcaselesscmp(tmp, "NMI") == 0) {
        if (nmi_defined != 0) {
          print_error("NMI can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "NMI expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(nmi, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(nmi, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(nmi, "C%d ", latest_stack);

        nmi_defined++;
      }
      else if (strcaselesscmp(tmp, "UNUSED") == 0) {
        if (unused_defined != 0) {
          print_error("UNUSED can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "UNUSED expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(unused, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(unused, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(unused, "C%d ", latest_stack);

        unused_defined++;
      }
      else if (strcaselesscmp(tmp, "IRQ") == 0) {
        if (irq_defined != 0) {
          print_error("IRQ can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "IRQ expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(irq, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(irq, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(irq, "C%d ", latest_stack);

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

  /* SNESEMUVECTOR */

  if (strcaselesscmp(cp, "SNESEMUVECTOR") == 0) {

    int cop_defined = 0, unused_defined = 0, abort_defined = 0;
    int nmi_defined = 0, reset_defined = 0, irqbrk_defined = 0;
    char cop[512], unused[512], abort[512], nmi[512], reset[512], irqbrk[512];


    if (snesemuvector_defined != 0) {
      print_error(".SNESEMUVECTOR can be defined only once.\n", ERROR_DIR);
      return FAILED;
    }
    if (hirom_defined == 0 && lorom_defined == 0) {
      print_error(".SNESEMUVECTOR needs .LOROM or .HIROM.\n", ERROR_DIR);
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
    fprintf(file_out_ptr, "P O0 A%d %d ", sec_tmp->id, 0x7FF4 + (hirom_defined ? 0x8000 : 0x0000));
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
          sprintf(cop, "y%d ", 0);
        if (reset_defined == 0)
          sprintf(reset, "y%d ", 0);
        if (abort_defined == 0)
          sprintf(abort, "y%d ", 0);
        if (nmi_defined == 0)
          sprintf(nmi, "y%d ", 0);
        if (unused_defined == 0)
          sprintf(unused, "y%d ", 0);
        if (irqbrk_defined == 0)
          sprintf(irqbrk, "y%d ", 0);

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
          sprintf(emsg, "COP expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(cop, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(cop, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(cop, "C%d ", latest_stack);

        cop_defined++;
      }
      else if (strcaselesscmp(tmp, "RESET") == 0) {
        if (reset_defined != 0) {
          print_error("RESET can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "RESET expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(reset, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(reset, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(reset, "C%d ", latest_stack);

        reset_defined++;
      }
      else if (strcaselesscmp(tmp, "ABORT") == 0) {
        if (abort_defined != 0) {
          print_error("ABORT can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "ABORT expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(abort, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(abort, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(abort, "C%d ", latest_stack);

        abort_defined++;
      }
      else if (strcaselesscmp(tmp, "NMI") == 0) {
        if (nmi_defined != 0) {
          print_error("NMI can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "NMI expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(nmi, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(nmi, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(nmi, "C%d ", latest_stack);

        nmi_defined++;
      }
      else if (strcaselesscmp(tmp, "UNUSED") == 0) {
        if (unused_defined != 0) {
          print_error("UNUSED can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "UNUSED expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(unused, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(unused, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(unused, "C%d ", latest_stack);

        unused_defined++;
      }
      else if (strcaselesscmp(tmp, "IRQBRK") == 0) {
        if (irqbrk_defined != 0) {
          print_error("IRQBRK can only be defined once.\n", ERROR_DIR);
          return FAILED;
        }

        inz = input_number();

        if (inz == SUCCEEDED && (d < -32768 || d > 65535)) {
          sprintf(emsg, "IRQBRK expects 16-bit data, %d is out of range!\n", d);
          print_error(emsg, ERROR_DIR);
          return FAILED;
        }

        if (inz == SUCCEEDED)
          sprintf(irqbrk, "y%d ", d);
        else if (inz == INPUT_NUMBER_ADDRESS_LABEL)
          sprintf(irqbrk, "k%d r%s ", active_file_info_last->line_current, label);
        else if (inz == INPUT_NUMBER_STACK)
          sprintf(irqbrk, "C%d ", latest_stack);

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

  /* PRINTT */

  if (strcaselesscmp(cp, "PRINTT") == 0) {

    char t[256];
    int s, u;


    inz = input_number();

    if (inz != INPUT_NUMBER_STRING) {
      print_error(".PRINTT needs a string.\n", ERROR_DIR);
      return FAILED;
    }

    for (s = 0, u = 0; label[s] != 0;) {
      if (label[s] == '\\' && label[s + 1] == 'n') {
#ifdef MSDOS
        t[u++] = '\r';
        t[u++] = '\n';
#else
        t[u++] = '\n';
#endif
        s += 2;
      }
      else if (label[s] == '\\' && label[s + 1] == '\\') {
        t[u++] = '\\';
        s += 2;
      }
      else
        t[u++] = label[s++];
    }

    t[u] = 0;
    if (quiet == NO)
      printf("%s", t);

    return SUCCEEDED;
  }

  /* PRINTV */

  if (strcaselesscmp(cp, "PRINTV") == 0) {

    int m;


    if (get_next_token() == FAILED)
      return FAILED;

    if (strcaselesscmp(tmp, "HEX") == 0)
      m = 0;
    else if (strcaselesscmp(tmp, "DEC") == 0)
      m = 1;
    else {
      print_error(".PRINTV needs to know the format of the output, HEX or DEC.\n", ERROR_DIR);
      return FAILED;
    }

    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      if (q == INPUT_NUMBER_ADDRESS_LABEL) {
        sprintf(emsg, "\"%s\" is not known.\n", label);
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
    }

    return SUCCEEDED;
  }

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

  if (strcaselesscmp(cp, "DBRND") == 0 || strcaselesscmp(cp, "DWRND") == 0) {

    int o, c, min, max, f;

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
      sprintf(emsg, ".%s needs the number of random numbers.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    c = d;

    if (c <= 0) {
      sprintf(emsg, ".%s needs that the number of random numbers is > 0.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* get min */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      sprintf(emsg, ".%s needs the min value.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    min = d;

    /* get max */
    q = input_number();
    if (q == FAILED)
      return FAILED;
    if (q != SUCCEEDED) {
      sprintf(emsg, ".%s needs the max value.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    max = d;

    if (min >= max) {
      sprintf(emsg, ".%s needs that min < max.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    /* generate the numbers */
    for (f = 0; f < c; f++) {
      d = (genrand_int32() % (max-min+1)) + min;

      fprintf(stderr, "got %d\n", d);

      if (o == 1) {
        if (d < -32768 || d > 65535) {
          sprintf(emsg, ".%s: Expected a 16-bit value, computed %d.\n", cp, d);
          print_error(emsg, ERROR_NONE);
          return FAILED;
        }
        fprintf(file_out_ptr, "y %d ", d);
      }
      else {
        if (d > 255 || d < -128) {
          sprintf(emsg, ".%s: Expected a 8-bit value, computed %d.\n", cp, d);
          print_error(emsg, ERROR_NONE);
          return FAILED;
        }
        fprintf(file_out_ptr, "d%d ", d);
      }
    }

    return SUCCEEDED;
  }

  /* DWSIN/DBSIN/DWCOS/DBCOS */

  if (strcaselesscmp(cp, "DWSIN") == 0 || strcaselesscmp(cp, "DBSIN") == 0 || strcaselesscmp(cp, "DWCOS") == 0 || strcaselesscmp(cp, "DBCOS") == 0) {

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
      sprintf(emsg, ".%s needs a value for starting angle.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (p == SUCCEEDED)
      a = d;
    else
      a = parsed_double;

    if (input_number() != SUCCEEDED || d < 0) {
      sprintf(emsg, ".%s needs an non-negative integer value for additional angles.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    c = d;

    input_float_mode = ON;
    p = input_number();
    if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
      sprintf(emsg, ".%s needs a value for angle step.\n", cp);
      print_error(emsg, ERROR_DIR);
      return FAILED;
    }

    if (p == SUCCEEDED)
      s = d;
    else
      s = parsed_double;

    p = input_number();
    if (p != SUCCEEDED && p != INPUT_NUMBER_FLOAT) {
      sprintf(emsg, ".%s needs a value to multiply the result with.\n", cp);
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
      sprintf(emsg, ".%s needs a value to add to the result.\n", cp);
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
          sprintf(emsg, ".%s: Expected a 16-bit value, computed %d.\n", cp, d);
          print_error(emsg, ERROR_NONE);
          return FAILED;
        }
        fprintf(file_out_ptr, "y %d ", d);
      }
      else {
        if (d > 255 || d < -128) {
          sprintf(emsg, ".%s: Expected a 8-bit value, computed %d.\n", cp, d);
          print_error(emsg, ERROR_NONE);
          return FAILED;
        }
        fprintf(file_out_ptr, "d%d ", d);
      }

      a += s;
    }

    return SUCCEEDED;
  }

  /* FAIL */

  if (strcaselesscmp(cp, "FAIL") == 0) {
    print_error("HALT: .FAIL found.\n", ERROR_NONE);

    /* make a silent exit */
    exit(0);
  }

  /* UNDEF/UNDEFINE */

  if (strcaselesscmp(cp, "UNDEF") == 0 || strcaselesscmp(cp, "UNDEFINE") == 0) {

    char c[16];


    strcpy(c, cp);

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
        sprintf(emsg, ".%s requires definition name(s).\n", c);
        print_error(emsg, ERROR_DIR);
        return FAILED;
      }

      q++;

      if (undefine(tmp) == FAILED) {
        sprintf(emsg, "Could not .%s \"%s\".\n", c, tmp);
        print_error(emsg, ERROR_WRN);
      }
    }

    return SUCCEEDED;
  }

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

  sprintf(emsg, "Unknown directive \"%s\".\n", tmp);
  print_error(emsg, ERROR_NONE);

  return FAILED;
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
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING) {
      sprintf(emsg, ".IF needs immediate data.\n");
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
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING) {
      sprintf(emsg, ".IF needs immediate data.\n");
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error("Cannot compare strings with immediate values.\n", ERROR_INP);
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
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING) {
      sprintf(emsg, ".%s needs immediate data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    strncpy(k, label, 255);
    k[255] = 0;
    y = d;
    s = q;

    q = input_number();
    if (q != SUCCEEDED && q != INPUT_NUMBER_STRING) {
      sprintf(emsg, ".%s needs immediate data.\n", bak);
      print_error(emsg, ERROR_INP);
      return FAILED;
    }

    /* different types? */
    if (s != q) {
      print_error("Cannot compare strings with immediate values.\n", ERROR_INP);
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


    inz = input_number();

    if (inz != INPUT_NUMBER_STRING) {
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
      sprintf(emsg, ".%s can be only used inside a macro.\n", bak);
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

    sprintf(emsg, ".%s needs an argument.\n", bak);
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
  sprintf(emsg, ".%s must end to .ENDIF/.ELSE.\n", name);
  print_error(emsg, ERROR_DIR);

  return FAILED;
}


#ifndef GB
void delete_stack(struct stack *s) {

  if (s == NULL) {
    sprintf(emsg, "Deleting a non-existing computation stack. This can lead to problems.\n");
    print_error(emsg, ERROR_WRN);
    return;
  }

  free(s->stack);
  free(s);
}
#endif


int get_new_definition_data(int *b, char *c, int *size, double *data) {

  int a, x, n, s;


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
        s = strlen(label);
      }
      else if (x == INPUT_NUMBER_FLOAT) {
        *data = parsed_double;
        *b = parsed_double;
      }
      else
        return x;

      n++;
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
      d = parsed_double;
      x = SUCCEEDED;
    }

    if (x == INPUT_NUMBER_STRING) {
      strcpy(&c[s], label);
      s += strlen(label);
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
      sprintf(emsg, "\"%s\" was .EXPORTed for the second time.\n", name);
      print_error(emsg, ERROR_WRN);
      return SUCCEEDED;
    }
    export = export->next;
  }

  export = malloc(sizeof(struct export_def));
  if (export == NULL) {
    sprintf(emsg, "Out of memory while allocating room for \".EXPORT %s\".\n", name);
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


/*
  wlalink - part of wla dx gb-z80/z80/z80n/6502/65c02/68000/6800/6801/6809/65816/huc6280/spc-700/8008/8080/superfx
  macro assembler package by ville helin <ville.helin@iki.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "files.h"
#include "check.h"
#include "analyze.h"
#include "write.h"
#include "compute.h"
#include "discard.h"
#include "listfile.h"
#include "parse.h"
#include "main.h"

#if defined(AMIGA)
#include "/printf.h"
#else
#include "../printf.h"
#endif

/* define this if you want to display debug information when you run WLALINK */
/*
  #define WLALINK_DEBUG 1
*/

char g_version_string[] = "$VER: wlalink 5.22a (14.9.2025)";

#if defined(AMIGA)
__near long __stack = 200000;
#endif

struct object_file *g_obj_first = NULL, *g_obj_last = NULL, *g_obj_tmp;
struct reference *g_reference_first = NULL, *g_reference_last = NULL;
struct section *g_sec_first = NULL, *g_sec_last = NULL, *g_sec_bankhd_first = NULL, *g_sec_bankhd_last = NULL;
struct stack *g_stacks_first = NULL, *g_stacks_last = NULL;
struct label *g_labels_first = NULL, *g_labels_last = NULL;
struct label **g_sorted_anonymous_labels = NULL;
struct map_t *g_global_unique_label_map = NULL;
struct map_t *g_namespace_map = NULL;
struct slot g_slots[256];
struct after_section *g_after_sections = NULL, *g_after_tmp;
struct label_sizeof *g_label_sizeofs = NULL;
struct section_fix *g_sec_fix_first = NULL, *g_sec_fix_tmp = NULL;
unsigned char *g_rom = NULL, *g_rom_usage = NULL, *g_file_header = NULL, *g_file_footer = NULL;
char g_load_address_label[MAX_NAME_LENGTH + 1], **g_ram_slots[256];
char g_program_address_start_label[MAX_NAME_LENGTH + 1], g_program_address_end_label[MAX_NAME_LENGTH + 1];
char g_ext_libdir[MAX_NAME_LENGTH + 2];
int g_load_address = 0, g_load_address_type = LOAD_ADDRESS_TYPE_UNDEFINED;
int g_program_address_start = -1, g_program_address_end = -1, g_program_address_start_type = LOAD_ADDRESS_TYPE_UNDEFINED, g_program_address_end_type = LOAD_ADDRESS_TYPE_UNDEFINED;
int g_romsize, g_rombanks, g_banksize, g_verbose_level = 0, g_section_overwrite = OFF;
int g_pc_bank, g_pc_full, g_pc_slot, g_pc_slot_max;
int g_file_header_size, g_file_footer_size, *g_banksizes = NULL, *g_bankaddress = NULL;
int g_output_mode = OUTPUT_ROM, g_discard_unreferenced_sections = OFF, g_use_libdir = NO;
int g_program_start, g_program_end, g_sms_checksum, g_smstag_defined = 0, g_snes_rom_mode = SNES_ROM_MODE_LOROM, g_snes_rom_speed = SNES_ROM_SPEED_SLOWROM;
int g_sms_header = 0, g_sms_checksum_already_written = 0, g_sms_checksum_size_defined = 0, g_sms_checksum_size = 0;
int g_gb_checksum, g_gb_complement_check, g_snes_checksum, g_snes_mode = 0, g_smd_checksum;
int g_smc_status = 0, g_snes_sramsize = 0, g_allow_duplicate_labels_and_definitions = NO;
int g_output_type = OUTPUT_TYPE_UNDEFINED, g_sort_sections = YES;
int g_num_sorted_anonymous_labels = 0, g_use_priority_only_writing_sections = NO, g_use_priority_only_writing_ramsections = NO;
int g_emptyfill = 0, g_paths_in_linkfile_are_relative_to_linkfile = NO, g_romheader_baseaddress = -1;

static int s_screen_dx = DEFAULT_SCREEN_DX, s_screen_dy = DEFAULT_SCREEN_DY, s_line_x = 0, s_line_y = 0, s_pause_text = NO;
static int s_create_sizeof_definitions = YES, s_listfile_data = NO, s_symbol_mode = SYMBOL_MODE_NONE;
static unsigned char s_output_addr_to_line = OFF;
static char s_print_text_buffer[4096];

extern struct object_file **g_object_files;
extern struct pointer_array **g_section_table_table;
extern int g_section_table_table_max;
extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];

#if WLALINK_DEBUG

static const char *s_si_operator_plus = "+";
static const char *s_si_operator_minus = "-";
static const char *s_si_operator_multiply = "*";
static const char *s_si_operator_or = "|";
static const char *s_si_operator_and = "&";
static const char *s_si_operator_divide = "/";
static const char *s_si_operator_power = "^";
static const char *s_si_operator_shift_left = "<<";
static const char *s_si_operator_shift_right = ">>";
static const char *s_si_operator_modulo = "#";
static const char *s_si_operator_xor = "~";
static const char *s_si_operator_low_byte = "<";
static const char *s_si_operator_high_byte = ">";
static const char *s_si_operator_not = "!";
static const char *s_si_operator_bank = ":";
static const char *s_si_operator_unknown = "UNKNOWN!";
static const char *s_si_operator_compare_lt = "< (LT)";
static const char *s_si_operator_compare_gt = "> (GT)";
static const char *s_si_operator_compare_eq = "==";
static const char *s_si_operator_compare_neq = "!=";
static const char *s_si_operator_compare_lte = "<=";
static const char *s_si_operator_compare_gte = ">=";
static const char *s_si_operator_logical_or = "||";
static const char *s_si_operator_logical_and = "&&";
static const char *s_si_operator_low_word = "loword(a)";
static const char *s_si_operator_high_word = "hiword(a)";
static const char *s_si_operator_bank_byte = "bankbyte(a)";
static const char *s_si_operator_round = "round(a)";
static const char *s_si_operator_floor = "floor(a)";
static const char *s_si_operator_ceil = "ceil(a)";
static const char *s_si_operator_min = "min(a,b)";
static const char *s_si_operator_max = "max(a,b)";
static const char *s_si_operator_sqrt = "sqrt(a)";
static const char *s_si_operator_abs = "abs(a)";
static const char *s_si_operator_cos = "cos(a)";
static const char *s_si_operator_sin = "sin(a)";
static const char *s_si_operator_tan = "tan(a)";
static const char *s_si_operator_cosh = "cosh(a)";
static const char *s_si_operator_sinh = "sinh(a)";
static const char *s_si_operator_tanh = "tanh(a)";
static const char *s_si_operator_acos = "acos(a)";
static const char *s_si_operator_asin = "asin(a)";
static const char *s_si_operator_atan = "atan(a)";
static const char *s_si_operator_negate = "negate(a)";
static const char *s_si_operator_atan2 = "atan2(a,b)";
static const char *s_si_operator_log = "log(a)";
static const char *s_si_operator_log10 = "log10(a)";
static const char *s_si_operator_pow = "pow(a,b)";
static const char *s_si_operator_sign = "sign(a)";
static const char *s_si_operator_clamp = "clamp(v,min,max)";

static const char *_get_stack_item_operator_name(int operator) {

  if (operator == SI_OP_ADD)
    return s_si_operator_plus;
  else if (operator == SI_OP_SUB)
    return s_si_operator_minus;
  else if (operator == SI_OP_MULTIPLY)
    return s_si_operator_multiply;
  else if (operator == SI_OP_OR)
    return s_si_operator_or;
  else if (operator == SI_OP_AND)
    return s_si_operator_and;
  else if (operator == SI_OP_DIVIDE)
    return s_si_operator_divide;
  else if (operator == SI_OP_POWER)
    return s_si_operator_power;
  else if (operator == SI_OP_SHIFT_LEFT)
    return s_si_operator_shift_left;
  else if (operator == SI_OP_SHIFT_RIGHT)
    return s_si_operator_shift_right;
  else if (operator == SI_OP_MODULO)
    return s_si_operator_modulo;
  else if (operator == SI_OP_XOR)
    return s_si_operator_xor;
  else if (operator == SI_OP_LOW_BYTE)
    return s_si_operator_low_byte;
  else if (operator == SI_OP_HIGH_BYTE)
    return s_si_operator_high_byte;
  else if (operator == SI_OP_NOT)
    return s_si_operator_not;
  else if (operator == SI_OP_BANK)
    return s_si_operator_bank;
  else if (operator == SI_OP_COMPARE_LT)
    return s_si_operator_compare_lt;
  else if (operator == SI_OP_COMPARE_GT)
    return s_si_operator_compare_gt;
  else if (operator == SI_OP_COMPARE_EQ)
    return s_si_operator_compare_eq;
  else if (operator == SI_OP_COMPARE_NEQ)
    return s_si_operator_compare_neq;
  else if (operator == SI_OP_COMPARE_LTE)
    return s_si_operator_compare_lte;
  else if (operator == SI_OP_COMPARE_GTE)
    return s_si_operator_compare_gte;
  else if (operator == SI_OP_LOGICAL_OR)
    return s_si_operator_logical_or;
  else if (operator == SI_OP_LOGICAL_AND)
    return s_si_operator_logical_and;
  else if (operator == SI_OP_LOW_WORD)
    return s_si_operator_low_word;
  else if (operator == SI_OP_HIGH_WORD)
    return s_si_operator_high_word;
  else if (operator == SI_OP_BANK_BYTE)
    return s_si_operator_bank_byte;
  else if (operator == SI_OP_ROUND)
    return s_si_operator_round;
  else if (operator == SI_OP_FLOOR)
    return s_si_operator_floor;
  else if (operator == SI_OP_CEIL)
    return s_si_operator_ceil;
  else if (operator == SI_OP_MIN)
    return s_si_operator_min;
  else if (operator == SI_OP_MAX)
    return s_si_operator_max;
  else if (operator == SI_OP_SQRT)
    return s_si_operator_sqrt;
  else if (operator == SI_OP_ABS)
    return s_si_operator_abs;
  else if (operator == SI_OP_COS)
    return s_si_operator_cos;
  else if (operator == SI_OP_SIN)
    return s_si_operator_sin;
  else if (operator == SI_OP_TAN)
    return s_si_operator_tan;
  else if (operator == SI_OP_COSH)
    return s_si_operator_cosh;
  else if (operator == SI_OP_SINH)
    return s_si_operator_sinh;
  else if (operator == SI_OP_TANH)
    return s_si_operator_tanh;
  else if (operator == SI_OP_ACOS)
    return s_si_operator_acos;
  else if (operator == SI_OP_ASIN)
    return s_si_operator_asin;
  else if (operator == SI_OP_ATAN)
    return s_si_operator_atan;
  else if (operator == SI_OP_NEGATE)
    return s_si_operator_negate;
  else if (operator == SI_OP_ATAN2)
    return s_si_operator_atan2;
  else if (operator == SI_OP_LOG)
    return s_si_operator_log;
  else if (operator == SI_OP_LOG10)
    return s_si_operator_log10;
  else if (operator == SI_OP_POW)
    return s_si_operator_pow;
  else if (operator == SI_OP_SIGN)
    return s_si_operator_sign;
  else if (operator == SI_OP_CLAMP)
    return s_si_operator_clamp;
  
  print_text(NO, "\n");
  print_text(NO, "_get_stack_item_operator_name(): ERROR: Unhandled SI_OP_* (%d)! Please submit a bug report!\n", operator);
  exit(1);

  return s_si_operator_unknown;
}

static char s_stack_item_description[512];

char *get_stack_item_description(struct stack_item *si, int file_id) {

  char *sid = s_stack_item_description;

  if (si == NULL)
    snprintf(sid, sizeof(s_stack_item_description), "NULL");
  else {
    int type = si->type;
    char sign = '+';

    if (si->sign == SI_SIGN_NEGATIVE)
      sign = '-';
    
    if (type == STACK_ITEM_TYPE_VALUE)
      snprintf(sid, sizeof(s_stack_item_description), "stack_item: value (%c)          : %f/$%x (RAM) %f/$%x (ROM)\n", sign, si->value_ram, (int)si->value_ram, si->value_rom, (int)si->value_rom);
    else if (type == STACK_ITEM_TYPE_OPERATOR)
      snprintf(sid, sizeof(s_stack_item_description), "stack_item: operator (%c)       : %s\n", sign, _get_stack_item_operator_name((int)si->value_ram));
    else if (type == STACK_ITEM_TYPE_STRING)
      snprintf(sid, sizeof(s_stack_item_description), "stack_item: string             : %s\n", si->string);
    else if (type == STACK_ITEM_TYPE_LABEL)
      snprintf(sid, sizeof(s_stack_item_description), "stack_item: label (%c)          : %s\n", sign, si->string);
    else if (type == STACK_ITEM_TYPE_STACK) {
      struct stack *st = find_stack((int)si->value_ram, file_id);

      if (st == NULL)
        snprintf(sid, sizeof(s_stack_item_description), "stack_item: calculation:       : THIS STACK CALCULATION (id = %d, %s) HAS GONE MISSING!\n", (int)si->value_ram, get_file_name(file_id));
      else {
        if (st->computed == YES)
          snprintf(sid, sizeof(s_stack_item_description), "stack_item: calculation: (%c)   : %d (result = %f/$%x (RAM) %f/$%x (ROM), %s)\n", sign, (int)si->value_ram, st->result_ram, (int)st->result_ram, st->result_rom, (int)st->result_rom, get_file_name(st->file_id));
        else
          snprintf(sid, sizeof(s_stack_item_description), "stack_item: calculation: (%c)   : %d (result = ?, %s)\n", sign, (int)si->value_ram, get_file_name(st->file_id));
      }
    }
    else
      snprintf(sid, sizeof(s_stack_item_description), "stack_item: UNKNOWN!\n");
  }
  
  return sid;
}


static void _debug_print_label(struct label *l) {

  print_text(YES, "label: \"%s\" file: %s status: %d section: %d (%d) bank: %d slot: %d base: %d address: %d/$%x alive: %d\n", l->name, get_file_name(l->file_id), l->status, l->section, l->section & 0xffff, l->bank, l->slot, l->base, (int)l->address, (int)l->address, l->alive);
}


static void _debug_print_sections(void) {

  if (g_sec_first != NULL) {
    struct section *s = g_sec_first;
    char *section_status[] = {
      "FREE",
      "FORCE",
      "OVERWRITE",
      "HEADER",
      "SEMIFREE",
      "ABSOLUTE",
      "RAM FREE",
      "SUPERFREE",
      "SEMISUBFREE",
      "RAM FORCE",
      "RAM SEMIFREE",
      "RAM SEMISUBFREE",
      "SEMISUPERFREE"
    };

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                         SECTIONS                               ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (s != NULL) {
      print_text(YES, "----------------------------------------------------------------------\n");
      print_text(YES, "name  : \"%s\"\n", s->name);
      print_text(YES, "file  : \"%s\"\n", get_file_name(s->file_id));
      print_text(YES, "id    : %d (%d)\n", s->id, s->id & 0xffff);
      print_text(YES, "addr  : %d\n", s->address);
      print_text(YES, "stat  : %d\n", s->status);
      print_text(YES, "bank  : %d\n", s->bank);
      print_text(YES, "base  : %d\n", s->base);
      print_text(YES, "slot  : %d\n", s->slot);
      print_text(YES, "size  : %d\n", s->size);
      print_text(YES, "align : %d\n", s->alignment);
      print_text(YES, "alive : %d\n", s->alive);
      print_text(YES, "status: %s\n", section_status[s->status]);
      s = s->next;
    }
    print_text(YES, "----------------------------------------------------------------------\n");
  }
}

#endif

/************************************************************************/
/* <REFACTOR>                                                           */
/************************************************************************/

/* TODO: move _pause(), _print_text(), print_text_using_args() and print_text() into
   a file of their own that is shared with the assembler and the linker - this exact same
   code can be found in assembler's main.c */

static void _pause(void) {

  fprintf(stdout, "[Press enter to continue]");
  getchar();

  s_line_y = 0;
  s_line_x = 0;
}


static void _print_text(int is_stdout, char *text) {

  if (is_stdout == YES) {
    fprintf(stdout, "%s", text);
    fflush(stdout);
  }
  else {
    fprintf(stderr, "%s", text);
    fflush(stderr);
  }
}


void print_text_using_args(int is_stdout, const char *format, va_list args) {

  int i, length, start_index = 0;
  char saved1, saved2;

  vsnprintf(s_print_text_buffer, sizeof(s_print_text_buffer), format, args);

  if (s_pause_text == YES) {
    /* logic for line counting and pausing after a screen full of text has been printed out */
    length = (int)strlen(s_print_text_buffer);

    for (i = 0; i < length; i++) {
      if (s_print_text_buffer[i] == 0xA) {
        s_line_x = 0;
        s_line_y++;

        if (i < length-1) {
          /* we have a linefeed in the middle of the string -> print everything so far */
          saved1 = s_print_text_buffer[i+1];
          s_print_text_buffer[i+1] = 0;
          
          _print_text(is_stdout, &s_print_text_buffer[start_index]);
          
          s_print_text_buffer[i+1] = saved1;
        }
        else {
          /* the end of the line has been reached -> print */
          _print_text(is_stdout, &s_print_text_buffer[start_index]);
        }

        if (s_line_y >= s_screen_dy-1)
          _pause();

        start_index = i+1;
      }
      else
        s_line_x++;
      
      if (s_line_x == s_screen_dx) {
        /* the end of the line has been reached */
        s_line_x = 0;
        s_line_y++;

        /* print the line */
        saved1 = s_print_text_buffer[i+1];
        saved2 = s_print_text_buffer[i+2];
        s_print_text_buffer[i+1] = 0xA;
        s_print_text_buffer[i+2] = 0;
          
        _print_text(is_stdout, &s_print_text_buffer[start_index]);

        if (s_line_y >= s_screen_dy-1)
          _pause();
          
        s_print_text_buffer[i+1] = saved1;
        s_print_text_buffer[i+2] = saved2;
        start_index = i+1;

        if (saved1 == 0xA && saved2 == 0) {
          start_index++;
          break;
        }
      }
    }

    /* if remaining characters exist, print them */
    if (start_index < length)
      _print_text(is_stdout, &s_print_text_buffer[start_index]);
  }
  else
    _print_text(is_stdout, s_print_text_buffer);
}


void print_text(int is_stdout, const char *format, ...) {

  va_list args;

  va_start(args, format);
  print_text_using_args(is_stdout, format, args);
  va_end(args);
}


/************************************************************************/
/* </REFACTOR>                                                           */
/************************************************************************/


static int _show_ram_information(int *free, int *total) {

  int printed_something = NO, bank_used, bank_free, slot, bank, i, area_start, area_end;
  char *slot_usage_data, slot_name[MAX_NAME_LENGTH+1];
  float f;

  if (g_verbose_level >= 100) {
    print_text(YES, "-------------------------------------------------\n");
    print_text(YES, "---                   RAM                     ---\n");
    print_text(YES, "-------------------------------------------------\n");
  }
  
  *free = 0;
  *total = 0;

  for (slot = 0; slot < 256; slot++) {
    for (bank = 0; bank < 256; bank++) {
      if (g_ram_slots[bank] == NULL)
        continue;

      slot_usage_data = g_ram_slots[bank][slot];
      if (slot_usage_data == NULL)
        continue;

      bank_used = 0;
      bank_free = 0;

      for (i = 0; i < g_slots[slot].size; i++) {
        if (slot_usage_data[i] <= 0) {
          (*free)++;
          bank_free++;
        }
        else
          bank_used++;
        (*total)++;
      }

      /* get slot name */
      if (g_slots[slot].name[0] != 0)
        snprintf(slot_name, sizeof(slot_name), "%d (%s)", slot, g_slots[slot].name);
      else
        snprintf(slot_name, sizeof(slot_name), "%d", slot);

      f = ((float)bank_free)/(bank_free + bank_used) * 100.0f;
      if (g_verbose_level >= 100)
        print_text(YES, "RAM slot %s bank %d (%d bytes (%.2f%%) free)\n", slot_name, bank, bank_free, f);

      area_start = -1;
      area_end = -1;
      
      for (i = 0; i < g_slots[slot].size; i++) {
        int print_area = NO;
        
        if (slot_usage_data[i] == 0) {
          if (area_start < 0)
            area_start = i;
          area_end = i;
          
          if (i == g_slots[slot].size-1)
            print_area = YES;
        }
        else {
          if (area_start >= 0)
            print_area = YES;
        }
        
        if (print_area == YES) {
          if (g_verbose_level >= 100)
            print_text(YES, "  - Free space at $%.4x-$%.4x (%d bytes)\n", area_start, area_end, area_end - area_start + 1);
          area_start = -1;
          area_end = -1;
        }
      }
      
      printed_something = YES;
    }
  }

  if (printed_something == NO) {
    if (g_verbose_level >= 100)
      print_text(YES, "No .RAMSECTIONs were found, no information about RAM.\n");
  }
  
  return SUCCEEDED;
}


static int _show_headers_and_footers_information(void) {

  struct section *s;
  int i, prints = 0;

  if (g_verbose_level < 100)
    return SUCCEEDED;
  
  print_text(YES, "-------------------------------------------------\n");
  print_text(YES, "---           HEADERS AND FOOTERS             ---\n");
  print_text(YES, "-------------------------------------------------\n");

  if (g_file_header_size != 0) {
    print_text(YES, "File header size %d.\n", g_file_header_size);
    prints++;
  }
  if (g_file_footer_size != 0) {
    print_text(YES, "File footer size %d.\n", g_file_footer_size);
    prints++;
  }
  
  i = g_file_header_size + g_file_footer_size;
  
  if (g_output_type == OUTPUT_TYPE_CBM_PRG) {
    print_text(YES, "2 additional bytes from the CBM PRG header.\n");
    i += 2;
    prints++;
  }
  
  if (g_smc_status != 0) {
    print_text(YES, "512 additional bytes from the SMC ROM header.\n");
    i += 512;
    prints++;
  }

  s = g_sec_bankhd_first;
  while (s != NULL) {
    print_text(YES, "ROM bank %d header section size %d.\n", s->bank, s->size);
    i += s->size;
    s = s->next;
  }

  if (i != 0) {
    if (prints > 1)
      print_text(YES, "Total %d additional bytes (from headers and footers).\n", i);
  }
  else
    print_text(YES, "No headers or footers found.\n");

  return SUCCEEDED;
}


static int _show_rom_ram_information(void) {

  int a, address, r, rom_used_bytes = 0, rom_bank_used_bytes, area_start, area_end, ram_free, ram_total;
  float f;

  if (g_verbose_level <= 1)
    return SUCCEEDED;

  fflush(stderr);
  fflush(stdout);
  
  if (g_output_mode == OUTPUT_ROM) {
    /* ROM information */

    if (g_verbose_level >= 100) {
      print_text(YES, "-------------------------------------------------\n");
      print_text(YES, "---                   ROM                     ---\n");
      print_text(YES, "-------------------------------------------------\n");
    }
    
    for (r = 0, address = 0; r < g_rombanks; r++) {
      int address_old = address;
    
      for (a = 0, rom_bank_used_bytes = 0; a < g_banksizes[r]; a++, address++) {
        if (g_rom_usage[address] != 0) {
          rom_used_bytes++;
          rom_bank_used_bytes++;
        }
      }

      f = (((float)(g_banksizes[r] - rom_bank_used_bytes))/g_banksizes[r]) * 100.0f;
      if (g_verbose_level >= 100)
        print_text(YES, "ROM bank %d (%d bytes (%.2f%%) free)\n", r, g_banksizes[r] - rom_bank_used_bytes, f);

      address = address_old;
      area_start = -1;
      area_end = -1;
      
      for (a = 0; a < g_banksizes[r]; a++, address++) {
        int print_area = NO;
        
        if (g_rom_usage[address] == 0) {
          if (area_start < 0)
            area_start = a;
          area_end = a;
          
          if (a == g_banksizes[r]-1)
            print_area = YES;
        }
        else {
          if (area_start >= 0)
            print_area = YES;
        }
        
        if (print_area == YES) {
          if (g_verbose_level >= 100)
            print_text(YES, "  - Free space at $%.4x-$%.4x (%d bytes)\n", area_start, area_end, area_end - area_start + 1);
          area_start = -1;
          area_end = -1;
        }      
      }
    }

    _show_ram_information(&ram_free, &ram_total);
    _show_headers_and_footers_information();
    
    if (g_verbose_level >= 100) {
      print_text(YES, "-------------------------------------------------\n");
      print_text(YES, "---                 SUMMARY                   ---\n");
      print_text(YES, "-------------------------------------------------\n");
    }
    
    f = (((float)(g_romsize - rom_used_bytes))/g_romsize) * 100.0f;
    print_text(YES, "ROM: %d bytes (%.2f%%) free of total %d.\n", g_romsize - rom_used_bytes, f, g_romsize);

    if (ram_free <= 0)
      print_text(YES, "RAM: No .RAMSECTIONs were found, no information about RAM.\n");
    else {
      f = (((float)ram_free)/ram_total) * 100.0f;
      print_text(YES, "RAM: %d bytes (%.2f%%) free of total %d.\n", ram_free, f, ram_total);
    }
  }
  else {
    /* PRG information */
    int prg_size = g_program_end - g_program_start + 1, used_bytes;

    if (g_verbose_level >= 100) {
      print_text(YES, "-------------------------------------------------\n");
      print_text(YES, "---                   PRG                     ---\n");
      print_text(YES, "-------------------------------------------------\n");
    }
    
    for (a = g_program_start, used_bytes = 0; a <= g_program_end; a++) {
      if (g_rom_usage[a] != 0)
        used_bytes++;
    }

    f = (((float)(prg_size - used_bytes))/prg_size) * 100.0f;
    if (g_verbose_level >= 100)
      print_text(YES, "PRG $%.4x-$%.4x (%d bytes (%.2f%%) free)\n", g_program_start, g_program_end, prg_size - used_bytes, f);

    area_start = -1;
    area_end = -1;
      
    for (a = g_program_start; a < g_program_end; a++) {
      int print_area = NO;
        
      if (g_rom_usage[a] == 0) {
        if (area_start < 0)
          area_start = a;
        area_end = a;
          
        if (a == g_program_end-1)
          print_area = YES;
      }
      else {
        if (area_start >= 0)
          print_area = YES;
      }
        
      if (print_area == YES) {
        if (g_verbose_level >= 100)
          print_text(YES, "  - Free space at $%.4x-$%.4x (%d bytes)\n", area_start, area_end, area_end - area_start + 1);
        area_start = -1;
        area_end = -1;
      }      
    }
    
    _show_ram_information(&ram_free, &ram_total);
    _show_headers_and_footers_information();

    if (g_verbose_level >= 100) {
      print_text(YES, "-------------------------------------------------\n");
      print_text(YES, "---                 SUMMARY                   ---\n");
      print_text(YES, "-------------------------------------------------\n");
    }
    
    f = (((float)(prg_size - used_bytes))/prg_size) * 100.0f;
    print_text(YES, "PRG: %d bytes (%.2f%%) free of total %d.\n", prg_size - used_bytes, f, prg_size);

    if (ram_free <= 0)
      print_text(YES, "RAM: No .RAMSECTIONs were found, no information about RAM.\n");
    else {
      f = (((float)ram_free)/ram_total) * 100.0f;
      print_text(YES, "RAM: %d bytes (%.2f%%) free of total %d.\n", ram_free, f, ram_total);
    }
  }

  fflush(stderr);
  fflush(stdout);
  
  return SUCCEEDED;
}


static void _free_section_allocations(struct section *s) {

  if (s->listfile_cmds != NULL)
    free(s->listfile_cmds);
  if (s->listfile_ints != NULL)
    free(s->listfile_ints);

  if (s->data != NULL)
    free(s->data);

  if (s->label_map != NULL)
    hashmap_free(s->label_map);

  if (s->nspace != NULL) {
    if (s->nspace->label_map != NULL) {
      hashmap_free(s->nspace->label_map);
      s->nspace->label_map = NULL;
    }
  }
}


static void _procedures_at_exit(void) {

  struct source_file_name *f, *fn;
  struct object_file *o;
  struct reference *r;
  struct section *s;
  struct stack *sta;
  struct label *l;
  struct label_sizeof *ls;
  int i, p;

  /* free all the dynamically allocated data structures */
  free(g_object_files);

  for (i = 0; i <= g_section_table_table_max; i++) {
    if (g_section_table_table[i] != NULL) {
      free(g_section_table_table[i]->ptr);
      free(g_section_table_table[i]);
    }
  }
  free(g_section_table_table);
  
  while (g_obj_first != NULL) {
    f = g_obj_first->source_file_names_list;
    while (f != NULL) {
      free(f->name);
      fn = f->next;
      free(f);
      f = fn;
    }
    free(g_obj_first->data);
    free(g_obj_first->name);
    free(g_obj_first->listfile_ints);
    free(g_obj_first->listfile_cmds);
    free(g_obj_first->stacks);
    o = g_obj_first;
    g_obj_first = g_obj_first->next;
    free(o);
  }

  while (g_labels_first != NULL) {
    l = g_labels_first;
    g_labels_first = g_labels_first->next;
    free(l);
  }

  while (g_label_sizeofs != NULL) {
    ls = g_label_sizeofs;
    g_label_sizeofs = g_label_sizeofs->next;
    free(ls);
  }

  while (g_reference_first != NULL) {
    r = g_reference_first;
    g_reference_first = g_reference_first->next;
    free(r);
  }

  while (g_stacks_first != NULL) {
    sta = g_stacks_first->next;
    free(g_stacks_first->stack_items);
    free(g_stacks_first);
    g_stacks_first = sta;
  }

  while (g_sec_first != NULL) {
    s = g_sec_first->next;
    _free_section_allocations(g_sec_first);
    free(g_sec_first);
    g_sec_first = s;
  }

  while (g_sec_bankhd_first != NULL) {
    s = g_sec_bankhd_first->next;
    _free_section_allocations(g_sec_bankhd_first);
    free(g_sec_bankhd_first);
    g_sec_bankhd_first = s;
  }

  while (g_sec_fix_first != NULL) {
    g_sec_fix_tmp = g_sec_fix_first;
    g_sec_fix_first = g_sec_fix_first->next;
    free(g_sec_fix_tmp);
  }

  g_after_tmp = g_after_sections;
  while (g_after_tmp != NULL) {
    g_after_sections = g_after_tmp->next;
    free(g_after_tmp);
    g_after_tmp = g_after_sections;
  }

  if (g_global_unique_label_map != NULL)
    hashmap_free(g_global_unique_label_map);

  if (g_namespace_map != NULL) {
    hashmap_free_all_elements(g_namespace_map);
    hashmap_free(g_namespace_map);
  }

  free(g_banksizes);
  free(g_bankaddress);

  /* free RAM slot/bank usage arrays */
  for (i = 0; i < 256; i++) {
    if (g_ram_slots[i] != NULL) {
      for (p = 0; p < 256; p++) {
        if (g_ram_slots[i][p] != NULL) {
          free(g_ram_slots[i][p]);
          g_ram_slots[i][p] = NULL;
        }
      }
      free(g_ram_slots[i]);
      g_ram_slots[i] = NULL;
    }
  }
  
  free(g_sorted_anonymous_labels);

  /* free ROM */
  free(g_rom);
  free(g_rom_usage);
}


static int _localize_path(char *path) {

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


static int _parse_and_set_libdir(char *c, int contains_flag) {

  char n[MAX_NAME_LENGTH + 1];
  int i;

  /* skip the flag? */
  if (contains_flag == YES)
    c += 2;

  if (strlen(c) < 2)
    return FAILED;
  
  for (i = 0; i < MAX_NAME_LENGTH && *c != 0; i++, c++)
    n[i] = *c;
  n[i] = 0;

  if (*c != 0)
    return FAILED;

  _localize_path(n);
#if defined(MSDOS)
  snprintf(g_ext_libdir, sizeof(g_ext_libdir), "%s\\", n);
#else
  snprintf(g_ext_libdir, sizeof(g_ext_libdir), "%s/", n);
#endif
  g_use_libdir = YES;

  return SUCCEEDED;
}


static int _parse_flags(char **flags, int flagc) {

  int output_mode_defined = 0, count, unknowns = 0;
  
  for (count = 1; count < flagc; count++) {
    if (!strcmp(flags[count], "-b")) {
      if (output_mode_defined == 1)
        return FAILED;
      output_mode_defined++;
      g_output_mode = OUTPUT_PRG;
    }
    else if (!strcmp(flags[count], "-bS")) {
      if (count + 1 < flagc) {
        /* get arg */
        if (get_next_number(flags[count + 1], &g_program_address_start, NULL) == FAILED) {
          /* address must be an address label */
          strncpy(g_program_address_start_label, flags[count + 1], MAX_NAME_LENGTH);
          g_program_address_start_type = LOAD_ADDRESS_TYPE_LABEL;
        }
        else
          g_program_address_start_type = LOAD_ADDRESS_TYPE_VALUE;
      }
      else
        return FAILED;
      count++;
    }
    else if (!strcmp(flags[count], "-bE")) {
      if (count + 1 < flagc) {
        /* get arg */
        if (get_next_number(flags[count + 1], &g_program_address_end, NULL) == FAILED) {
          /* address must be an address label */
          strncpy(g_program_address_end_label, flags[count + 1], MAX_NAME_LENGTH);
          g_program_address_end_type = LOAD_ADDRESS_TYPE_LABEL;
        }
        else
          g_program_address_end_type = LOAD_ADDRESS_TYPE_VALUE;
      }
      else
        return FAILED;
      count++;
    }
    else if (!strcmp(flags[count], "-r")) {
      if (output_mode_defined == 1)
        return FAILED;
      output_mode_defined++;
      g_output_mode = OUTPUT_ROM;
    }
    else if (!strcmp(flags[count], "-R")) {
      if (g_paths_in_linkfile_are_relative_to_linkfile == YES)
        return FAILED;
      g_paths_in_linkfile_are_relative_to_linkfile = YES;
    }
    else if (!strcmp(flags[count], "-t")) {
      if (count + 1 < flagc) {
        /* get arg */
        if (!strcmp(flags[count + 1], "CBMPRG"))
          g_output_type = OUTPUT_TYPE_CBM_PRG;
        else
          return FAILED;
      }
      else
        return FAILED;
      count++;
    }
    else if (!strcmp(flags[count], "-a")) {
      if (count + 1 < flagc) {
        /* get arg */
        if (get_next_number(flags[count + 1], &g_load_address, NULL) == FAILED) {
          /* load address must be an address label */
          strncpy(g_load_address_label, flags[count + 1], MAX_NAME_LENGTH);
          g_load_address_type = LOAD_ADDRESS_TYPE_LABEL;
        }
        else
          g_load_address_type = LOAD_ADDRESS_TYPE_VALUE;
      }
      else
        return FAILED;
      count++;
    }
    else if (!strcmp(flags[count], "-L")) {
      if (count + 1 < flagc) {
        /* get arg */
        _parse_and_set_libdir(flags[count+1], NO);
      }
      else
        return FAILED;
      count++;
    }
    else if (!strcmp(flags[count], "-c"))
      g_allow_duplicate_labels_and_definitions = YES;
    else if (!strcmp(flags[count], "-i"))
      s_listfile_data = YES;
    else if (!strcmp(flags[count], "-nS"))
      g_sort_sections = NO;
    else if (!strcmp(flags[count], "-p"))
      s_pause_text = YES;
    else if (!strcmp(flags[count], "-SX")) {
      if (count + 1 < flagc) {
        /* parse arg */
        s_screen_dx = atoi(flags[count+1]);
        count++;
      }
      else
        return FAILED;
    }
    else if (!strcmp(flags[count], "-SY")) {
      if (count + 1 < flagc) {
        /* parse arg */
        s_screen_dy = atoi(flags[count+1]);
        count++;
      }
      else
        return FAILED;
    }
    else if (!strcmp(flags[count], "-pS"))
      g_use_priority_only_writing_sections = YES;
    else if (!strcmp(flags[count], "-pR"))
      g_use_priority_only_writing_ramsections = YES;
    else if (!strcmp(flags[count], "-v"))
      g_verbose_level = 100;
    else if (!strcmp(flags[count], "-v1"))
      g_verbose_level = 1;
    else if (!strcmp(flags[count], "-v2"))
      g_verbose_level = 2;
    else if (!strcmp(flags[count], "-s"))
      s_symbol_mode = SYMBOL_MODE_NOCA5H;
    else if (!strcmp(flags[count], "-S"))
      s_symbol_mode = SYMBOL_MODE_WLA;
    else if (!strcmp(flags[count], "-sE"))
      s_symbol_mode = SYMBOL_MODE_EQU;
    else if (!strcmp(flags[count], "-A"))
      s_output_addr_to_line = ON;
    else if (!strcmp(flags[count], "-d"))
      g_discard_unreferenced_sections = ON;
    else if (!strcmp(flags[count], "-D"))
      s_create_sizeof_definitions = NO;
    else {
      /* legacy support? */
      if (strncmp(flags[count], "-L", 2) == 0) {
        /* old library directory */
        _parse_and_set_libdir(flags[count], YES);
      }
      else if (count >= flagc - 2)
        unknowns++;
      else
        return FAILED;
    }
  }

  if (unknowns == 2)  
    return SUCCEEDED;
  else
    return FAILED;
}


static int _allocate_rom(void) {

  g_rom = calloc(sizeof(unsigned char) * g_romsize, 1);
  if (g_rom == NULL) {
    print_text(NO, "_ALLOCATE_ROM: Out of memory.\n");
    return FAILED;
  }
  g_rom_usage = calloc(sizeof(unsigned char) * g_romsize, 1);
  if (g_rom_usage == NULL) {
    print_text(NO, "_ALLOCATE_ROM: Out of memory.\n");
    return FAILED;
  }
  memset(g_rom, g_emptyfill, g_romsize);
  memset(g_rom_usage, 0, g_romsize);

  return SUCCEEDED;
}


int main(int argc, char *argv[]) {

  int i;

  if (sizeof(double) != 8) {
    print_text(NO, "MAIN: sizeof(double) == %d != 8. WLALINK will not work properly.\n", (int)(sizeof(double)));
    return -1;
  }

  /* initialize RAM slots */
  for (i = 0; i < 256; i++)
    g_ram_slots[i] = NULL;

  atexit(_procedures_at_exit);

  if (argc >= 2)
    i = _parse_flags(argv, argc);
  else
    i = FAILED;

  if (i == FAILED) {
    char title[] = "WLALINK - WLA DX Macro Assembler Linker v5.22a";
    int length, left, right;

    length = (int)strlen(title);
    left = (70 - 3 - 3 - length) / 2;
    right = 70 - 3 - 3 - left - length;

    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---");
    for (i = 0; i < left; i++)
      print_text(YES, " ");
    print_text(YES, "%s", title);
    for (i = 0; i < right; i++)
      print_text(YES, " ");
    print_text(YES, "---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
#if defined(AMIGACPU)
    print_text(YES, "                         Compiled for " AMIGACPU "\n");
#endif
    print_text(YES, "                Programmed by Ville Helin in 1998-2008\n");
    print_text(YES, "        In GitHub since 2014: https://github.com/vhelin/wla-dx\n");

    length = (int)strlen(g_version_string);
    left = (70 - length) / 2;

    for (i = 0; i < left; i++)
      print_text(YES, " ");
    print_text(YES, "%s", g_version_string);
    
    print_text(YES, "\n\n");

#if defined(WLALINK_DEBUG)
    print_text(YES, "** WLALINK_DEBUG defined - this executable is running in DEBUG mode **\n");
    print_text(YES, "\n");
#endif
    print_text(YES, "USAGE: %s [OPTIONS] <LINK FILE> <OUTPUT FILE>\n\n", argv[0]);
    print_text(YES, "OPTIONS:\n");
    print_text(YES, "-a <ADDR> Load address (can also be label) for CBM PRG\n");
    print_text(YES, "-A  Add address-to-line mapping data to WLA symbol file\n");
    print_text(YES, "-b  Program file output\n");
    print_text(YES, "-bE Ending address of the program (optional)\n");
    print_text(YES, "-bS Starting address of the program (optional)\n");
    print_text(YES, "-c  Allow duplicate labels and definitions\n");
    print_text(YES, "-d  Discard unreferenced sections\n");
    print_text(YES, "-D  Don't create _sizeof_* definitions\n");
    print_text(YES, "-i  Write list files\n");
    print_text(YES, "-L <DIR> Library directory\n");
    print_text(YES, "-nS Don't sort the sections\n");
    print_text(YES, "-p  Pause printing after a screen full of text has been printed,\n");
    print_text(YES, "    use this with -SX and -SY\n");
    print_text(YES, "-pR Write .RAMSECTIONs based on PRIORITY only, ignore .RAMSECTION types\n");
    print_text(YES, "-pS Write .SECTIONs based on PRIORITY only, ignore .SECTION types\n");
    print_text(YES, "-r  ROM file output (default)\n");
    print_text(YES, "-R  Make file paths in link file relative to its location\n");
    print_text(YES, "-s  Write also a NO$GMB/NO$SNES symbol file\n");
    print_text(YES, "-sE Write also an EQU symbol file\n");
    print_text(YES, "-S  Write also a WLA symbol file\n");
    print_text(YES, "-SX <WIDTH> The number of characters per line in console (default %d)\n", DEFAULT_SCREEN_DX);
    print_text(YES, "-SY <HEIGHT> The number of lines in console (default %d)\n", DEFAULT_SCREEN_DY);
    print_text(YES, "-t <TYPE> Output type (supported types: 'CBMPRG')\n");
    print_text(YES, "-v  Verbose messages (all)\n");
    print_text(YES, "-v1 Verbose messages (only discard sections)\n");
    print_text(YES, "-v2 Verbose messages (-v1 plus short summary)\n");
    print_text(YES, "\n");
    print_text(YES, "EXAMPLE: %s -d -v -S linkfile linked.rom\n\n", argv[0]);
    return 0;
  }

  /* initialize some variables */
  g_mem_insert_action[0] = 0;  
  g_global_unique_label_map = hashmap_new();
  g_namespace_map = hashmap_new();

  /* load files */
  if (load_files(argv, argc) == FAILED)
    return 1;

  /* check file types */
  if (check_file_types() == FAILED)
    return 1;

  /* check object headers */
  if (check_headers() == FAILED)
    return 1;

  /* obtain the amount of rom banks */
  if (obtain_rombanks() == FAILED)
    return 1;

  g_banksizes = calloc(sizeof(int) * g_rombanks, 1);
  if (g_banksizes == NULL) {
    print_text(NO, "MAIN: Out of memory error.\n");
    return 1;
  }
  g_bankaddress = calloc(sizeof(int) * g_rombanks, 1);
  if (g_bankaddress == NULL) {
    print_text(NO, "MAIN: Out of memory error.\n");
    return 1;
  }

  /* obtain rom bank map and check the project integrity */
  if (obtain_rombankmap() == FAILED)
    return 1;

  /* obtain memory map and check the project integrity */
  if (obtain_memorymap() == FAILED)
    return 1;

  /* convert slot names and addresses to slot numbers */
  if (convert_slot_names_and_addresses() == FAILED)
    return 1;
  
  /* calculate romsize */
  for (g_romsize = 0, i = 0; i < g_rombanks; i++)
    g_romsize += g_banksizes[i];

  /* obtain source file names used in compiling */
  if (obtain_source_file_names() == FAILED)
    return 1;

  /* collect all defines, labels, outside references and pending (stack) calculations */
  if (collect_dlr() == FAILED)
    return 1;

  /* make sure all SECTIONSTART_* and SECTIONEND_* labels have no duplicates */
  /*
  if (fix_sectionstartend_labels() == FAILED)
    return 1;
  */

  /* take rom size and allocate memory */
  if (_allocate_rom() == FAILED)
    return 1;

  /* parse data blocks */
  if (parse_data_blocks() == FAILED)
    return 1;

  /* fix the library bank, slot and org/orga of sections, if specified in linkfile */
  if (fix_all_sections() == FAILED)
    return 1;

  /* check that all library RAM sections are given a bank and a slot */
  if (check_ramsections() == FAILED)
    return 1;

#if defined(WLALINK_DEBUG)
  print_text(YES, "\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "*** LOADED LOADED LOADED LOADED LOADED LOADED LOADED LOADED LOADED ***\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
#endif

#if defined(WLALINK_DEBUG)
  _debug_print_sections();
#endif

  /* sort the sections by priority first and then by size, biggest first */
  if (sort_sections() == FAILED)
    return 1;
  
  /* append sections */
  if (append_sections() == FAILED)
    return 1;

  /* clean up the structures */
  if (clean_up_dlr() == FAILED)
    return 1;

  /* associate labels with their sections */
  if (fix_label_sections() == FAILED)
    return 1;

  /* drop all unreferenced sections */
  if (g_discard_unreferenced_sections == ON) {
    if (discard_unused_sections() == FAILED)
      return 1;
    /* remove dropped labels */
    discard_dropped_labels();
  }
  
  /* correct non-zero-BASE library section addresses */
  if (correct_65816_library_sections() == FAILED)
    return 1;

  /* if ROM size < 32KBs, correct SDSC tag sections' addresses */
  if (g_smstag_defined != 0) {
    int tag_address = get_sms_tag_address();
    struct section *s = g_sec_first;

    while (s != NULL) {
      if (strcmp(s->name, "!__WLA_SDSCTAG_STRINGS") == 0)
        s->address = tag_address - 6;
      else if (strcmp(s->name, "!__WLA_SDSCTAG_TIMEDATE") == 0)
        s->address = tag_address - 16;

      s = s->next;
    }
  }

#if defined(WLALINK_DEBUG)
  if (g_labels_first != NULL) {
    struct label *l = g_labels_first;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                         LABELS                                 ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (l != NULL) {
      if (l->alive == YES)
        _debug_print_label(l);
      l = l->next;
    }
  }
#endif

#if defined(WLALINK_DEBUG)
  if (g_label_sizeofs != NULL) {
    struct label_sizeof *ls = g_label_sizeofs;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                      LABEL SIZEOFS                             ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (ls != NULL) {
      print_text(YES, "----------------------------------------------------------------------\n");
      print_text(YES, "name: \"%s\" file: %s\n", ls->name, get_file_name(ls->file_id));
      print_text(YES, "size: %d\n", ls->size);
      ls = ls->next;
    }
    print_text(YES, "----------------------------------------------------------------------\n");
  }
#endif
      
#if defined(WLALINK_DEBUG)
  if (g_stacks_first != NULL) {
    struct stack *s = g_stacks_first;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                    (STACK) CALCULATIONS                        ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (s != NULL) {
      print_text(YES, "----------------------------------------------------------------------\n");
      {
        int z;
        
        for (z = 0; z < s->stacksize; z++) {
          struct stack_item *si = &s->stack_items[z];
          if (si->stack_file_id >= 0)
            print_text(YES, "%s", get_stack_item_description(si, si->stack_file_id));
          else
            print_text(YES, "%s", get_stack_item_description(si, s->file_id));
        }
      }
      print_text(YES, "id: %d file: %s line: %d type: %d bank: %d position: %d section_status: %d section: %d (%d)\n", s->id, get_file_name(s->file_id), s->linenumber, s->type, s->bank, s->position, s->section_status, s->section, s->section & 0xffff);
      s = s->next;
    }
    print_text(YES, "----------------------------------------------------------------------\n");
  }
#endif

  /* reserve the bytes the checksummers will use, so no (free type) sections will be placed there */
  reserve_checksum_bytes();
  
#if defined(WLALINK_DEBUG)
  print_text(YES, "\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "*** RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED ***\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
  print_text(YES, "**********************************************************************\n");
#endif

  /* insert sections */
  if (insert_sections() == FAILED)
    return 1;

#if defined(WLALINK_DEBUG)
  _debug_print_sections();
#endif
  
  /* compute the labels' addresses */
  if (fix_label_addresses() == FAILED)
    return 1;

  /* generate RAM bank usage labels (RAM_USAGE_SLOT_x_BANK_y_START + RAM_USAGE_SLOT_x_BANK_y_END) */
  if (generate_ram_bank_usage_labels() == FAILED)
    return 1;
  
  /* generate _sizeof_[label] definitions */
  if (s_create_sizeof_definitions == YES) {
    if (generate_sizeof_label_definitions() == FAILED)
      return 1;
  }

  /* sort anonymous labels to speed up searching for them */
  if (sort_anonymous_labels() == FAILED)
    return 1;

#if defined(WLALINK_DEBUG)
  if (g_labels_first != NULL) {
    struct label *l = g_labels_first;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                         LABELS                                 ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (l != NULL) {
      if (l->alive == YES)
        _debug_print_label(l);
      l = l->next;
    }
  }
#endif
  
  /* compute pending calculations */
  if (compute_pending_calculations() == FAILED)
    return 1;

#if defined(WLALINK_DEBUG)
  if (g_stacks_first != NULL) {
    struct stack *s = g_stacks_first;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                    (STACK) CALCULATIONS                        ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (s != NULL) {
      print_text(YES, "----------------------------------------------------------------------\n");
      {
        int z;
        
        for (z = 0; z < s->stacksize; z++) {
          struct stack_item *si = &s->stack_items[z];
          if (si->stack_file_id >= 0)
            print_text(YES, "%s", get_stack_item_description(si, si->stack_file_id));
          else
            print_text(YES, "%s", get_stack_item_description(si, s->file_id));
        }
      }
      print_text(YES, "id: %d file: %s line: %d type: %d bank: %d position: %d section_status: %d section: %d (%d) result: %f/$%x (ROM) %f/$%x (RAM)\n", s->id, get_file_name(s->file_id), s->linenumber, s->type, s->bank, s->position, s->section_status, s->section, s->section & 0xffff, s->result_rom, (int)s->result_rom, s->result_ram, (int)s->result_ram);
      s = s->next;
    }
    print_text(YES, "----------------------------------------------------------------------\n");
  }
#endif

  /* transform computation stack definitions to ordinary definitions */
  if (transform_stack_definitions() == FAILED)
    return 1;

  /* fix references */
  if (fix_references() == FAILED)
    return 1;

#if defined(WLALINK_DEBUG)
  if (g_reference_first != NULL) {
    struct reference *r = g_reference_first;

    print_text(YES, "\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "---                          REFERENCES                            ---\n");
    print_text(YES, "----------------------------------------------------------------------\n");
    print_text(YES, "\n");

    while (r != NULL) {
      print_text(YES, "name: \"%s\" file: %s\n", r->name, get_file_name(r->file_id));
      r = r->next;
    }
  }
#endif

  if (g_snes_mode != 0)
    finalize_snes_rom();

  /* write checksums and other last minute data */
  if (compute_checksums() == FAILED)
    return 1;

  /* write rom file */
  if (write_rom_file(argv[argc - 1]) == FAILED)
    return 1;

  /* export symbolic information file */
  if (s_symbol_mode != SYMBOL_MODE_NONE) {
    if (write_symbol_file(argv[argc - 1], s_symbol_mode, s_output_addr_to_line) == FAILED)
      return 1;
  }

  /* write list files */
  if (s_listfile_data == YES) {
    if (listfile_write_listfiles() == FAILED)
      return 1;
  }

  /* show rom & ram information */
  _show_rom_ram_information();

  return 0;
}


/*
  wlalink - part of wla dx gb-z80/z80/6502/65c02/6510/6800/6801/6809/65816/huc6280/spc-700/8008/8080
  macro assembler package by ville helin <ville.helin@iki.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "main.h"
#include "memory.h"
#include "files.h"
#include "check.h"
#include "analyze.h"
#include "write.h"
#include "compute.h"
#include "discard.h"
#include "listfile.h"
#include "parse.h"

#ifdef AMIGA
#include "/printf.h"
#else
#include "../printf.h"
#endif

/* define this if you want to display debug information when you run WLALINK */
/*
  #define WLALINK_DEBUG
*/

char version_string[] = "$VER: wlalink 5.15a (21.3.2021)";

#ifdef AMIGA
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
struct append_section *g_append_sections = NULL, *g_append_tmp;
struct label_sizeof *g_label_sizeofs = NULL;
struct section_fix *g_sec_fix_first = NULL, *g_sec_fix_tmp = NULL;
unsigned char *g_rom, *g_rom_usage, *g_file_header = NULL, *g_file_footer = NULL;
char g_load_address_label[MAX_NAME_LENGTH + 1];
int g_load_address = 0, g_load_address_type = LOAD_ADDRESS_TYPE_UNDEFINED;
char g_program_address_start_label[MAX_NAME_LENGTH + 1], g_program_address_end_label[MAX_NAME_LENGTH + 1];
int g_program_address_start = -1, g_program_address_end = -1, g_program_address_start_type = LOAD_ADDRESS_TYPE_UNDEFINED, g_program_address_end_type = LOAD_ADDRESS_TYPE_UNDEFINED;
int g_romsize, g_rombanks, g_banksize, g_verbose_mode = OFF, g_section_overwrite = OFF, g_symbol_mode = SYMBOL_MODE_NONE;
int g_pc_bank, g_pc_full, g_pc_slot, g_pc_slot_max;
int g_file_header_size, g_file_footer_size, *g_banksizes = NULL, *g_bankaddress = NULL;
int g_output_mode = OUTPUT_ROM, g_discard_unreferenced_sections = OFF, g_use_libdir = NO;
int g_program_start, g_program_end, g_sms_checksum, g_smstag_defined = 0, g_snes_rom_mode = SNES_ROM_MODE_LOROM, g_snes_rom_speed = SNES_ROM_SPEED_SLOWROM, g_sms_header = 0;
int g_gb_checksum, g_gb_complement_check, g_snes_checksum, g_snes_mode = 0;
int g_smc_status = 0, g_snes_sramsize = 0;
int g_output_type = OUTPUT_TYPE_UNDEFINED, g_sort_sections = YES;
int g_num_sorted_anonymous_labels = 0;
int g_emptyfill = 0;

static int g_create_sizeof_definitions = YES, g_listfile_data = NO, g_output_addr_to_line = OFF;

extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
char g_ext_libdir[MAX_NAME_LENGTH + 2];

#ifdef WLALINK_DEBUG

static const char *g_si_operator_plus = "+";
static const char *g_si_operator_minus = "-";
static const char *g_si_operator_multiply = "*";
static const char *g_si_operator_or = "|";
static const char *g_si_operator_and = "&";
static const char *g_si_operator_divide = "/";
static const char *g_si_operator_power = "^";
static const char *g_si_operator_shift_left = "<<";
static const char *g_si_operator_shift_right = ">>";
static const char *g_si_operator_modulo = "#";
static const char *g_si_operator_xor = "~";
static const char *g_si_operator_low_byte = "<";
static const char *g_si_operator_high_byte = ">";
static const char *g_si_operator_bank = ":";
static const char *g_si_operator_unknown = "UNKNOWN!";

static const char *get_stack_item_operator_name(int operator) {

  if (operator == SI_OP_PLUS)
    return g_si_operator_plus;
  else if (operator == SI_OP_MINUS)
    return g_si_operator_minus;
  else if (operator == SI_OP_MULTIPLY)
    return g_si_operator_multiply;
  else if (operator == SI_OP_OR)
    return g_si_operator_or;
  else if (operator == SI_OP_AND)
    return g_si_operator_and;
  else if (operator == SI_OP_DIVIDE)
    return g_si_operator_divide;
  else if (operator == SI_OP_POWER)
    return g_si_operator_power;
  else if (operator == SI_OP_SHIFT_LEFT)
    return g_si_operator_shift_left;
  else if (operator == SI_OP_SHIFT_RIGHT)
    return g_si_operator_shift_right;
  else if (operator == SI_OP_MODULO)
    return g_si_operator_modulo;
  else if (operator == SI_OP_XOR)
    return g_si_operator_xor;
  else if (operator == SI_OP_LOW_BYTE)
    return g_si_operator_low_byte;
  else if (operator == SI_OP_HIGH_BYTE)
    return g_si_operator_high_byte;
  else if (operator == SI_OP_BANK)
    return g_si_operator_bank;

  return g_si_operator_unknown;
}

static char g_stack_item_description[512];

char *get_stack_item_description(struct stack_item *si, int file_id) {

  char *sid = g_stack_item_description;

  if (si == NULL)
    snprintf(sid, sizeof(g_stack_item_description), "NULL");
  else {
    int type = si->type;
    
    if (type == STACK_ITEM_TYPE_VALUE)
      snprintf(sid, sizeof(g_stack_item_description), "stack_item: value              : %f/$%x (RAM) %f/$%x (ROM)\n", si->value_ram, (int)si->value_ram, si->value_rom, (int)si->value_rom);
    else if (type == STACK_ITEM_TYPE_OPERATOR)
      snprintf(sid, sizeof(g_stack_item_description), "stack_item: operator           : %s\n", get_stack_item_operator_name((int)si->value_ram));
    else if (type == STACK_ITEM_TYPE_STRING)
      snprintf(sid, sizeof(g_stack_item_description), "stack_item: label              : %s\n", si->string);
    else if (type == STACK_ITEM_TYPE_STACK) {
      struct stack *st = find_stack((int)si->value_ram, file_id);

      if (st->computed == YES)
        snprintf(sid, sizeof(g_stack_item_description), "stack_item: (stack) calculation: %d (result = %d/$%x (RAM) %d/$%x (ROM))\n", (int)si->value_ram, st->result_ram, st->result_ram, st->result_rom, st->result_rom);
      else
        snprintf(sid, sizeof(g_stack_item_description), "stack_item: (stack) calculation: %d (result = ?)\n", (int)si->value_ram);
    }
    else
      snprintf(sid, sizeof(g_stack_item_description), "stack_item: UNKNOWN!");
  }
  
  return sid;
}

static void debug_print_label(struct label *l) {

  printf("label: \"%s\" file: %s status: %d section: %d bank: %d slot: %d base: %d address: %d/$%x alive: %d\n", l->name, get_file_name(l->file_id), l->status, l->section, l->bank, l->slot, l->base, (int)l->address, (int)l->address, l->alive);
}

#endif


int main(int argc, char *argv[]) {

  int i, x, y, q, inz;
  struct section *s;
  float f;

  if (sizeof(double) != 8) {
    fprintf(stderr, "MAIN: sizeof(double) == %d != 8. WLALINK will not work properly.\n", (int)(sizeof(double)));
    return -1;
  }

  atexit(procedures_at_exit);

  if (argc > 2)
    x = parse_flags(argv, argc);
  else
    x = FAILED;

  if (x == FAILED) {
    printf("\nWLALINK GB-Z80/Z80/6502/65C02/65CE02/6510/65816/6800/6801/6809/8008/8080/HUC6280/SPC-700 WLA Macro Assembler Linker v5.15a\n");
    printf("Written by Ville Helin in 2000-2008 - In GitHub since 2014: https://github.com/vhelin/wla-dx\n");
#ifdef WLALINK_DEBUG
    printf("*** WLALINK_DEBUG defined - this executable is running in DEBUG mode ***\n");
#endif
    printf("%s\n\n", version_string);
    printf("USAGE: %s [OPTIONS] <LINK FILE> <OUTPUT FILE>\n\n", argv[0]);
    printf("Options:\n");
    printf("-b  Program file output\n");
    printf("-bS Starting address of the program (optional)\n");
    printf("-bE Ending address of the program (optional)\n");
    printf("-d  Discard unreferenced sections\n");
    printf("-D  Don't create _sizeof_* definitions\n");
    printf("-nS Don't sort the sections\n");
    printf("-i  Write list files\n");
    printf("-r  ROM file output (default)\n");
    printf("-s  Write also a NO$GMB/NO$SNES symbol file\n");
    printf("-S  Write also a WLA symbol file\n");
    printf("-A  Add address-to-line mapping data to WLA symbol file\n");
    printf("-v  Verbose messages\n");
    printf("-L <DIR>  Library directory\n");
    printf("-t <TYPE> Output type (supported types: 'CBMPRG')\n");
    printf("-a <ADDR> Load address for CBM PRG\n\n");
    printf("EXAMPLE: %s -d -v -S linkfile linked.rom\n\n", argv[0]);
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
    fprintf(stderr, "MAIN: Out of memory error.\n");
    return 1;
  }
  g_bankaddress = calloc(sizeof(int) * g_rombanks, 1);
  if (g_bankaddress == NULL) {
    fprintf(stderr, "MAIN: Out of memory error.\n");
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
  for (g_romsize = 0, x = 0; x < g_rombanks; x++)
    g_romsize += g_banksizes[x];

  /* obtain source file names used in compiling */
  if (obtain_source_file_names() == FAILED)
    return 1;

  /* collect all defines, labels, outside references and pending (stack) calculations */
  if (collect_dlr() == FAILED)
    return 1;

  /* make sure all SECTIONSTART_* and SECTIONEND_* labels have no duplicates */
  if (fix_sectionstartend_labels() == FAILED)
    return 1;

  /* take rom size and allocate memory */
  if (allocate_rom() == FAILED)
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

  /* sort the sections by priority first and then by size, biggest first */
  if (sort_sections() == FAILED)
    return 1;
  
  /* append sections */
  if (merge_sections() == FAILED)
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
    discard_drop_labels();
  }
  
  /* correct non-zero-BASE library section addresses */
  if (correct_65816_library_sections() == FAILED)
    return 1;

  /* if ROM size < 32KBs, correct SDSC tag sections' addresses */
  if (g_smstag_defined != 0 && g_romsize < 0x8000) {
    struct section *s = g_sec_first;
    int sub = 0x4000; /* assume 16KB ROM size */

    if (g_romsize < 0x4000)
      sub = 0x6000; /* assume 8KB ROM size */
    
    while (s != NULL) {
      if (strcmp(s->name, "!__WLA_SDSCTAG_STRINGS") == 0 ||
          strcmp(s->name, "!__WLA_SDSCTAG_TIMEDATE") == 0) {
        /* these sections would originally go to 0x7Fnm, but as we now
           assume that the ROM is smaller, we'll bring them down */
        s->address -= sub;
      }

      s = s->next;
    }
  }

#ifdef WLALINK_DEBUG
  printf("\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("*** LOADED LOADED LOADED LOADED LOADED LOADED LOADED LOADED LOADED ***\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
#endif

#ifdef WLALINK_DEBUG
  if (g_labels_first != NULL) {
    struct label *l = g_labels_first;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                         LABELS                                 ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (l != NULL) {
      if (l->alive == YES)
        debug_print_label(l);
      l = l->next;
    }
  }
#endif

#ifdef WLALINK_DEBUG
  if (g_label_sizeofs != NULL) {
    struct label_sizeof *ls = g_label_sizeofs;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                      LABEL SIZEOFS                             ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (ls != NULL) {
      printf("----------------------------------------------------------------------\n");
      printf("name: \"%s\" file: %s\n", ls->name, get_file_name(ls->file_id));
      printf("size: %d\n", ls->size);
      ls = ls->next;
    }
    printf("----------------------------------------------------------------------\n");
  }
#endif
      
#ifdef WLALINK_DEBUG
  if (g_stacks_first != NULL) {
    struct stack *s = g_stacks_first;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                    (STACK) CALCULATIONS                        ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (s != NULL) {
      printf("----------------------------------------------------------------------\n");
      {
        int z;
        
        for (z = 0; z < s->stacksize; z++) {
          struct stack_item *si = &s->stack[z];
          printf(get_stack_item_description(si, s->file_id));
        }
      }
      printf("id: %d file: %s line: %d type: %d bank: %d position: %d section_status: %d section: %d\n", s->id, get_file_name(s->file_id), s->linenumber, s->type, s->bank, s->position, s->section_status, s->section);
      s = s->next;
    }
    printf("----------------------------------------------------------------------\n");
  }
#endif

  /* reserve the bytes the checksummers will use, so no (free type) sections will be placed there */
  reserve_checksum_bytes();
  
#ifdef WLALINK_DEBUG
  printf("\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("*** RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED RESOLVED ***\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
  printf("**********************************************************************\n");
#endif

  /* insert sections */
  if (insert_sections() == FAILED)
    return 1;

#ifdef WLALINK_DEBUG
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
      "RAM SEMISUBFREE"
    };

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                         SECTIONS                               ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (s != NULL) {
      printf("----------------------------------------------------------------------\n");
      printf("name  : \"%s\"\n", s->name);
      printf("file  : \"%s\"\n", get_file_name(s->file_id));
      printf("id    : %d\n", s->id);
      printf("addr  : %d\n", s->address);
      printf("stat  : %d\n", s->status);
      printf("bank  : %d\n", s->bank);
      printf("base  : %d\n", s->base);
      printf("slot  : %d\n", s->slot);
      printf("size  : %d\n", s->size);
      printf("align : %d\n", s->alignment);
      printf("alive : %d\n", s->alive);
      printf("status: %s\n", section_status[s->status]);
      s = s->next;
    }
    printf("----------------------------------------------------------------------\n");
  }
#endif

  /* compute the labels' addresses */
  if (fix_label_addresses() == FAILED)
    return 1;

  /* generate _sizeof_[label] definitions */
  if (g_create_sizeof_definitions == YES) {
    if (generate_sizeof_label_definitions() == FAILED)
      return 1;
  }

  /* sort anonymous labels to speed up searching for them */
  if (sort_anonymous_labels() == FAILED)
    return 1;

#ifdef WLALINK_DEBUG
  if (g_labels_first != NULL) {
    struct label *l = g_labels_first;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                         LABELS                                 ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (l != NULL) {
      if (l->alive == YES)
        debug_print_label(l);
      l = l->next;
    }
  }
#endif
  
  /* compute pending calculations */
  if (compute_pending_calculations() == FAILED)
    return 1;

#ifdef WLALINK_DEBUG
  if (g_stacks_first != NULL) {
    struct stack *s = g_stacks_first;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                    (STACK) CALCULATIONS                        ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (s != NULL) {
      printf("----------------------------------------------------------------------\n");
      {
        int z;
        
        for (z = 0; z < s->stacksize; z++) {
          struct stack_item *si = &s->stack[z];
          printf(get_stack_item_description(si, s->file_id));
        }
      }
      printf("id: %d file: %s line: %d type: %d bank: %d position: %d section_status: %d section: %d result: %d/$%x (ROM) %d/$%x (RAM)\n", s->id, get_file_name(s->file_id), s->linenumber, s->type, s->bank, s->position, s->section_status, s->section, s->result_rom, s->result_rom, s->result_ram, s->result_ram);
      s = s->next;
    }
    printf("----------------------------------------------------------------------\n");
  }
#endif

  /* transform computation stack definitions to ordinary definitions */
  if (transform_stack_definitions() == FAILED)
    return 1;

  /* fix references */
  if (fix_references() == FAILED)
    return 1;

#ifdef WLALINK_DEBUG
  if (g_reference_first != NULL) {
    struct reference *r = g_reference_first;

    printf("\n");
    printf("----------------------------------------------------------------------\n");
    printf("---                          REFERENCES                            ---\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");

    while (r != NULL) {
      printf("name: \"%s\" file: %s\n", r->name, get_file_name(r->file_id));
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
  if (g_symbol_mode != SYMBOL_MODE_NONE) {
    if (write_symbol_file(argv[argc - 1], g_symbol_mode, g_output_addr_to_line) == FAILED)
      return FAILED;
  }

  /* write list files */
  if (g_listfile_data == YES) {
    if (listfile_write_listfiles(g_sec_first) == FAILED)
      return FAILED;
  }

  /* show rom information */
  y = 0;
  if (g_verbose_mode == ON) {
    x = 0;
    for (i = 0; i < g_romsize; i++) {
      if (g_rom_usage[i] == 0 && x == 0) {
        x = 1;
        y = i;
      }
      else if (g_rom_usage[i] != 0 && x == 1) {
        if (y == (i - 1))
          fprintf(stderr, "Free space at $%.4x.\n", y);
        else
          fprintf(stderr, "Free space at $%.4x-$%.4x.\n", y, i - 1);
        x = 0;
      }
    }

    if (x == 1) {
      if (y == (i - 1))
        fprintf(stderr, "Free space at $%.4x.\n", y);
      else
        fprintf(stderr, "Free space at $%.4x-$%.4x.\n", y, i - 1);
    }

    for (y = 0, q = 0; y < g_romsize; q++) {
      for (x = 0, inz = 0; inz < g_banksizes[q]; inz++) {
        if (g_rom_usage[y++] == 0)
          x++;
      }
      f = (((float)x)/g_banksizes[q]) * 100.0f;
      if (f == 100.0f)
        fprintf(stderr, "Bank %.2d has %.5d bytes (%.1f%%) free.\n", q, x, f);
      else
        fprintf(stderr, "Bank %.2d has %.5d bytes (%.2f%%) free.\n", q, x, f);
    }

    /* ROM data */
    if (g_output_mode == OUTPUT_ROM) {
      for (i = 0, y = 0; i < g_romsize; i++) {
        if (g_rom_usage[i] == 0)
          y++;
      }

      fprintf(stderr, "%d unused bytes of total %d.\n", y, g_romsize);
      q = g_romsize;
    }
    /* program file data */
    else {
      for (i = g_program_start, y = 0; i < g_program_end; i++) {
        if (g_rom_usage[i] == 0)
          y++;
      }

      q = g_program_end - g_program_start + 1;
      fprintf(stderr, "%d unused bytes (%.2f%%) of total %d.\n", y, (((double)y) / q) * 100, q);
    }

    if (g_file_header_size != 0)
      fprintf(stderr, "File header size %d.\n", g_file_header_size);
    if (g_file_footer_size != 0)
      fprintf(stderr, "File footer size %d.\n", g_file_footer_size);

    if (g_smc_status != 0) {
      fprintf(stderr, "512 additional bytes from the SMC ROM header.\n");
      i = g_file_header_size + g_file_footer_size + 512;
    }
    else
      i = g_file_header_size + g_file_footer_size;

    s = g_sec_bankhd_first;
    while (s != NULL) {
      fprintf(stderr, "Bank %d header section size %d.\n", s->bank, s->size);
      i += s->size;
      s = s->next;
    }

    if (i != 0) {
      fprintf(stderr, "Total %d additional bytes (from headers and footers).\n", i);
      fprintf(stderr, "Total size %d bytes.\n", i + q);
    }
  }

  return 0;
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


void procedures_at_exit(void) {

  struct source_file_name *f, *fn;
  struct object_file *o;
  struct reference *r;
  struct section *s;
  struct stack *sta;
  struct label *l;
  struct label_sizeof *ls;

  /* free all the dynamically allocated data structures */
  while (g_obj_first != NULL) {
    f = g_obj_first->source_file_names_list;
    while (f != NULL) {
      if (f->name != NULL)
        free(f->name);
      fn = f->next;
      free(f);
      f = fn;
    }
    if (g_obj_first->data != NULL)
      free(g_obj_first->data);
    if (g_obj_first->name != NULL)
      free(g_obj_first->name);
    o = g_obj_first;
    g_obj_first = g_obj_first->next;
    free(o);
  }

  if (g_global_unique_label_map != NULL)
    hashmap_free(g_global_unique_label_map);

  if (g_namespace_map != NULL) {
    hashmap_free_all_elements(g_namespace_map);
    hashmap_free(g_namespace_map);
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
    free(g_stacks_first->stack);
    free(g_stacks_first);
    g_stacks_first = sta;
  }

  while (g_sec_first != NULL) {
    s = g_sec_first->next;
    if (g_sec_first->listfile_cmds != NULL)
      free(g_sec_first->listfile_cmds);
    if (g_sec_first->listfile_ints != NULL)
      free(g_sec_first->listfile_ints);
    if (g_sec_first->data != NULL)
      free(g_sec_first->data);
    hashmap_free(g_sec_first->label_map);
    free(g_sec_first);
    g_sec_first = s;
  }

  while (g_sec_bankhd_first != NULL) {
    s = g_sec_bankhd_first->next;
    free(g_sec_bankhd_first);
    g_sec_bankhd_first = s;
  }

  while (g_sec_fix_first != NULL) {
    g_sec_fix_tmp = g_sec_fix_first;
    g_sec_fix_first = g_sec_fix_first->next;
    free(g_sec_fix_tmp);
  }

  g_append_tmp = g_append_sections;
  while (g_append_tmp != NULL) {
    g_append_sections = g_append_tmp->next;
    free(g_append_tmp);
    g_append_tmp = g_append_sections;
  }

  if (g_banksizes != NULL)
    free(g_banksizes);
  if (g_bankaddress != NULL)
    free(g_bankaddress);

  if (g_sorted_anonymous_labels != NULL)
    free(g_sorted_anonymous_labels);
}


int parse_flags(char **flags, int flagc) {

  int output_mode_defined = 0;
  int count;
  
  for (count = 1; count < flagc - 2; count++) {
    if (!strcmp(flags[count], "-b")) {
      if (output_mode_defined == 1)
        return FAILED;
      output_mode_defined++;
      g_output_mode = OUTPUT_PRG;
      continue;
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
      continue;
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
      continue;
    }
    else if (!strcmp(flags[count], "-r")) {
      if (output_mode_defined == 1)
        return FAILED;
      output_mode_defined++;
      g_output_mode = OUTPUT_ROM;
      continue;
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
      continue;      
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
      continue;      
    }
    else if (!strcmp(flags[count], "-L")) {
      if (count + 1 < flagc) {
        /* get arg */
        parse_and_set_libdir(flags[count+1], NO);
      }
      else
        return FAILED;
      count++;
      continue;
    }
    else if (!strcmp(flags[count], "-i")) {
      g_listfile_data = YES;
      continue;
    }
    else if (!strcmp(flags[count], "-nS")) {
      g_sort_sections = NO;
      continue;
    }
    else if (!strcmp(flags[count], "-v")) {
      g_verbose_mode = ON;
      continue;
    }
    else if (!strcmp(flags[count], "-s")) {
      g_symbol_mode = SYMBOL_MODE_NOCA5H;
      continue;
    }
    else if (!strcmp(flags[count], "-S")) {
      g_symbol_mode = SYMBOL_MODE_WLA;
      continue;
    }
    else if (!strcmp(flags[count], "-A")) {
      g_output_addr_to_line = ON;
      continue;
    }
    else if (!strcmp(flags[count], "-d")) {
      g_discard_unreferenced_sections = ON;
      continue;
    }
    else if (!strcmp(flags[count], "-D")) {
      g_create_sizeof_definitions = NO;
      continue;
    }
    else {
      /* legacy support? */
      if (strncmp(flags[count], "-L", 2) == 0) {
        /* old library directory */
        parse_and_set_libdir(flags[count], YES);
        continue;
      }
      else
        return FAILED;
    }
  }
  
  return SUCCEEDED;
}


int parse_and_set_libdir(char *c, int contains_flag) {

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

  localize_path(n);
#if defined(MSDOS)
  snprintf(g_ext_libdir, sizeof(g_ext_libdir), "%s\\", n);
#else
  snprintf(g_ext_libdir, sizeof(g_ext_libdir), "%s/", n);
#endif
  g_use_libdir = YES;

  return SUCCEEDED;
}


int allocate_rom(void) {

  g_rom = calloc(sizeof(unsigned char) * g_romsize, 1);
  if (g_rom == NULL) {
    fprintf(stderr, "ALLOCATE_ROM: Out of memory.\n");
    return FAILED;
  }
  g_rom_usage = calloc(sizeof(unsigned char) * g_romsize, 1);
  if (g_rom_usage == NULL) {
    fprintf(stderr, "ALLOCATE_ROM: Out of memory.\n");
    return FAILED;
  }
  memset(g_rom, g_emptyfill, g_romsize);
  memset(g_rom_usage, 0, g_romsize);

  return SUCCEEDED;
}

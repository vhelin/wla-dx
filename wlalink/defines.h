
#ifdef AMIGA
#include "/hashmap.h"
#include "/shared.h"
#else
#include "../hashmap.h"
#include "../shared.h"
#endif

#ifndef _DEFINES_H
#define _DEFINES_H

#define SECTION_TYPES_COUNT    9
#define RAMSECTION_TYPES_COUNT 4
#define SECTION_TYPES_COUNT_ALL (SECTION_TYPES_COUNT + RAMSECTION_TYPES_COUNT)

#define OBJ_FORMAT_INFO        0
#define OBJ_EMPTY_FILL         4
#define OBJ_MISC_BITS          5
#define OBJ_MORE_BITS          6
#define OBJ_EXTR_BITS          7
#define OBJ_SMS_CHECKSUM_SIZE  8
#define OBJ_ROMBANKS          16
#define OBJ_ROMBANKMAP        20

#define LIB_FORMAT_INFO       0
#define LIB_MISC_BITS         4
#define LIB_SOURCE_FILE_NAMES 5

#define OUTPUT_ROM 0
#define OUTPUT_PRG 1

#define OUTPUT_TYPE_UNDEFINED 0
#define OUTPUT_TYPE_CBM_PRG   1

#define LOAD_ADDRESS_TYPE_UNDEFINED 0
#define LOAD_ADDRESS_TYPE_VALUE     1
#define LOAD_ADDRESS_TYPE_LABEL     2

#define SNES_ROM_MODE_LOROM 0
#define SNES_ROM_MODE_HIROM 2
#define SNES_ROM_MODE_EXHIROM 32
#define SNES_ROM_MODE_EXLOROM 64

#define SNES_ROM_SPEED_SLOWROM 0
#define SNES_ROM_SPEED_FASTROM 8

#define WLA_VERSION_OBJ 0
#define WLA_VERSION_LIB 1

#define DATA_TYPE_BLOCK   0
#define DATA_TYPE_SECTION 1

#define STRING_TYPE_LABEL  0
#define STRING_TYPE_STRING 1

#define STATE_NONE        0
#define STATE_OBJECT      1
#define STATE_LIBRARY     2
#define STATE_HEADER      3
#define STATE_FOOTER      4
#define STATE_DEFINITION  5
#define STATE_RAMSECTIONS 6
#define STATE_SECTIONS    7
#define STATE_SECTION_WRITE_ORDER    8
#define STATE_RAMSECTION_WRITE_ORDER 9

#define SYMBOL_MODE_NONE    0
#define SYMBOL_MODE_NOCA5H  1
#define SYMBOL_MODE_WLA     2
#define SYMBOL_MODE_EQU     3

struct source_file_name {
  char *name;
  int  id;
  unsigned long checksum;
  struct source_file_name *next;
};

struct object_file {
  unsigned char *data;
  unsigned char *memorymap;
  unsigned char *source_file_names;
  unsigned char *exported_defines;
  unsigned char *data_blocks;
  char slot_name[MAX_NAME_LENGTH + 1];
  char *name;
  int rom_banks;
  int format;
  int bank;
  int slot;
  int size;
  int base;
  int base_defined;
  int little_endian;
  int cpu_65816;
  int cpu_65ce02;
  int id;
  int fix_slot;
  int listfile_items;
  int *listfile_ints;
  char *listfile_cmds;
  struct source_file_name *source_file_names_list;
  struct stack **stacks;
  int stacks_max;
  int stacks_array_max;
  struct object_file *next;
};

struct after_section {
  int section_id;
  int file_id;
  char section[MAX_NAME_LENGTH + 1];
  char after[MAX_NAME_LENGTH + 1];
  char is_appendto;
  char alive;
  char inserted;
  struct section *section_s;
  struct section *after_s;
  struct after_section *next;
};

struct sort_capsule {
  char alive;
  struct section *section;
  struct after_section *after_section;
  struct sort_capsule *next, *prev;
  struct sort_capsule *children;
};

#define LABEL_STATUS_LABEL      0
#define LABEL_STATUS_DEFINE     1
#define LABEL_STATUS_STACK      2
#define LABEL_STATUS_SYMBOL     3
#define LABEL_STATUS_BREAKPOINT 4
#define LABEL_STATUS_STRING     5

struct label {
  char name[MAX_NAME_LENGTH + 1];
  char context[MAX_NAME_LENGTH + 1];
  char string[MAX_NAME_LENGTH + 1];
  int file_id;
  int file_id_source;
  int linenumber;
  int address_in_section;
  int section;
  int section_status;
  int bank;
  int base;
  int slot;
  int status;
  int alive;
  double address;
  double rom_address;
  struct section *section_struct;
  struct label *next;
  struct label *prev;
};

struct reference {
  char name[MAX_NAME_LENGTH + 1];
  char context[MAX_NAME_LENGTH + 1];
  int  address;
  int  type;
  int  section;
  int  section_status;
  int  bank;
  int  slot;
  int  base;
  int  file_id;
  int  file_id_source;
  int  linenumber;
  int  special_id;
  int  bits_position;
  int  bits_to_define;
  struct reference *next;
  struct reference *prev;
};

struct section {
  char name[MAX_NAME_LENGTH + 1];
  char banks[MAX_NAME_LENGTH + 1];
  int  file_id;
  int  file_id_source;
  int  priority;
  int  address;
  int  output_address;
  int  status;
  int  bitwindow;
  int  window_start;
  int  window_end;
  int  keep;
  int  bank;
  int  slot;
  int  size;
  int  base;
  int  base_defined;
  int  library_status;
  int  id;
  int  i;
  int  referenced;
  int  alive;
  int  appended_to;
  int  alignment;
  int  offset;
  int  appended_to_offset;
  int  listfile_items;
  int  *listfile_ints;
  char marked;
  char placed;
  char is_bankheader_section;
  char *listfile_cmds;
  unsigned char *data;
  struct namespace_def *nspace;
  struct map_t *label_map;
  struct section *after;
  struct section *next;
  struct section *prev;
  struct section *appended_to_section;
};

struct section_fix {
  char name[MAX_NAME_LENGTH + 1];
  char file_name[MAX_NAME_LENGTH + 1];
  char slot_name[MAX_NAME_LENGTH + 1];
  char banks[MAX_NAME_LENGTH + 1];
  int  line_number;
  int  keep;
  int  bank;
  int  slot;
  int  orga;
  int  org;
  int  status;
  int  alignment;
  int  offset;
  int  priority;
  int  priority_defined;
  int  size;
  int  is_ramsection;
  int  bitwindow;
  int  window_start;
  int  window_end;
  struct section_fix *next;
};

struct slot {
  int address;
  int size;
  int usage;
  char name[MAX_NAME_LENGTH + 1];
};

struct stack {
  struct stack_item *stack_items;
  struct stack *next;
  struct stack *prev;
  int id;
  int relative_references;
  int under_work;
  int computed;
  double result_ram;
  double result_rom;
  int result_slot;
  int result_base;
  int result_bank;
  int position;
  int file_id;
  int file_id_source;
  int stacksize;
  int linenumber;
  int type;
  int bank;
  int slot;
  int base;
  int section_status;
  int section;
  int address;
  int memory_address;
  int special_id;
  int bits_position;
  int bits_to_define;
};

struct stack_item {
  int type;
  int sign;
  int slot;
  int base;
  int bank;
  int stack_file_id;
  double value_ram;
  double value_rom;
  char string[MAX_NAME_LENGTH + 1];
};

struct pointer_array {
  void **ptr;
  int  max;
  int  array_max;
};

#endif /* _DEFINES_H */

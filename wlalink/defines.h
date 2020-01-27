
#ifdef AMIGA
#include "/hashmap.h"
#include "/shared.h"
#else
#include "../hashmap.h"
#include "../shared.h"
#endif

#ifndef _DEFINES_H
#define _DEFINES_H

#define OBJ_FORMAT_INFO  0
#define OBJ_EMPTY_FILL   4
#define OBJ_MISC_BITS    5
#define OBJ_MORE_BITS    6
#define OBJ_ROMBANKS     7
#define OBJ_ROMBANKMAP  11

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

#define STATE_NONE        0
#define STATE_OBJECT      1
#define STATE_LIBRARY     2
#define STATE_HEADER      3
#define STATE_FOOTER      4
#define STATE_DEFINITION  5
#define STATE_RAMSECTIONS 6

#define SYMBOL_MODE_NONE   0
#define SYMBOL_MODE_NOCA5H 1
#define SYMBOL_MODE_WLA    2

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
  int id;
  struct source_file_name *source_file_names_list;
  struct object_file *next;
};

struct append_section {
  char section[MAX_NAME_LENGTH + 1];
  char append_to[MAX_NAME_LENGTH + 1];
  struct append_section *next;
};

#define LABEL_STATUS_LABEL      0
#define LABEL_STATUS_DEFINE     1
#define LABEL_STATUS_STACK      2
#define LABEL_STATUS_SYMBOL     3
#define LABEL_STATUS_BREAKPOINT 4

struct label {
  char name[MAX_NAME_LENGTH + 1];
  int file_id;
  int file_id_source;
  int linenumber;
  int rom_address;
  int address_in_section;
  int section;
  int section_status;
  int bank;
  int base;
  int slot;
  int status;
  int alive;
  double address;
  struct section *section_struct;
  struct label *next;
  struct label *prev;
};

struct reference {
  char name[MAX_NAME_LENGTH + 1];
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
  struct reference *next;
  struct reference *prev;
};

struct section {
  char name[MAX_NAME_LENGTH + 1];
  int  file_id;
  int  file_id_source;
  int  priority;
  int  address;
  int  output_address;
  int  status;
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
  int  alignment;
  int  listfile_items;
  int  *listfile_ints;
  char *listfile_cmds;
  unsigned char *data;
  struct namespace_def *nspace;
  struct map_t *label_map;
  struct section *next;
  struct section *prev;
};

struct section_fix {
  char name[MAX_NAME_LENGTH + 1];
  char file_name[MAX_NAME_LENGTH + 1];
  int  line_number;
  int  bank;
  int  slot;
  struct section_fix *next;
};

struct slot {
  int address;
  int size;
  int usage;
};

struct stack {
  struct stack_item *stack;
  struct stack *next;
  struct stack *prev;
  int id;
  int relative_references;
  int under_work;
  int computed;
  int result;
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
  int using_op_bank;
  int base_in_labels;
};

#endif /* _DEFINES_H */

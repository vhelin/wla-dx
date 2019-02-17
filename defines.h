
#include "hashmap.h"
#include "shared.h"

#ifndef _DEFINES_H
#define _DEFINES_H

#define HINT_NONE  0
#define HINT_8BIT  1
#define HINT_16BIT 2
#define HINT_24BIT 3

#define STACK_CALCULATE_DELAY 2
#define STACK_RETURN_LABEL 1024

#define STACK_NONE    0
#define STACK_INSIDE  1
#define STACK_OUTSIDE 2

#define INPUT_NUMBER_EOL           2
#define INPUT_NUMBER_ADDRESS_LABEL 3
#define INPUT_NUMBER_STRING        4
#define INPUT_NUMBER_STACK         5
#define INPUT_NUMBER_FLOAT         6

#define GET_NEXT_TOKEN_STRING         2
#define EVALUATE_TOKEN_NOT_IDENTIFIED 2
#define EVALUATE_TOKEN_EOP            6

#define OUTPUT_OBJECT  0
#define OUTPUT_LIBRARY 1
#define OUTPUT_NONE    2

#define ERROR_NONE 0
#define ERROR_LOG  1
#define ERROR_DIR  2
#define ERROR_NUM  3
#define ERROR_INC  4
#define ERROR_INB  5
#define ERROR_UNF  6
#define ERROR_INP  7
#define ERROR_STC  8
#define ERROR_WRN  9
#define ERROR_ERR 10

/**************************************************************/
/* wla internal datatypes                                     */
/**************************************************************/

/* A - absolute section    */
/* c - stack (1 byte)      */
/* C - stack (2 bytes)     */
/* T - stack (3 bytes)     */
/* d - data (1 byte)       */
/* y - data (2 bytes)      */
/* z - data (3 bytes)      */
/* D - data block          */
/* O - origin              */
/* o - absolute origin     */
/* B - ROM bank            */
/* b - BASE                */
/* L - label               */
/* r - 16-bit reference    */
/* R - 8-bit pc ref        */
/* M - 16-bit pc ref       */
/* Q - 8-bit reference     */
/* q - 24-bit reference    */
/* S - section             */
/* s - end of section      */
/* x - dsb                 */
/* X - dsw                 */
/* E - end of file         */
/* f - file name id        */
/* k - line number         */
/* P - add_old = add       */
/* p - add = add_old       */
/* g - block (start)       */
/* G - block (end)         */
/* Y - symbol              */
/* Z - breakpoint          */

/**************************************************************/
/* gb-z80                                                     */
/**************************************************************/

#ifdef GB

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 8 - *           8b */
/* 9 - *          16b */

#define OP_SIZE_MAX 16

#endif

/**************************************************************/
/* 6502                                                       */
/**************************************************************/

#ifdef MCS6502

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 4 - x (absolute)   */

#define OP_SIZE_MAX 12
#define BASED_ON_6502 1

#endif

/**************************************************************/
/* 65C02                                                      */
/**************************************************************/

#ifdef WDC65C02

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 4 - x (absolute)   */
/* 5 - x-abs x-rel    */

#define OP_SIZE_MAX 12
#define BASED_ON_6502 1

#endif

/**************************************************************/
/* huc6280                                                    */
/**************************************************************/

#ifdef HUC6280

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 4 - ? ? ?          */
/* 5 - x x            */
/* 6 - x ?            */
/* 7 - x (absolute)   */
/* 8 - x-abs x-rel    */

#define OP_SIZE_MAX 12
#define BASED_ON_6502 1

#endif

/**************************************************************/
/* 6510                                                       */
/**************************************************************/

#ifdef MCS6510

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 4 - x (absolute)   */

#define OP_SIZE_MAX 12
#define BASED_ON_6502 1

#endif

/**************************************************************/
/* spc700                                                     */
/**************************************************************/

#ifdef SPC700

/* opcode types */

/* 0 - plain text   */
/* 1 - x            */
/* 2 - ?            */
/* 3 - x ~ (odd)    */
/* 4 - x ~ (even)   */
/* 5 - ~ (direct)   */
/* a - x x          [1, 2] */
/* b - x x          [2, 1] */
/* c - x ~ x (odd)  [1, 2] */
/* d - x ~ x (even) [1, 2] */
/* f - ? (13-bit) ~ */

#define OP_SIZE_MAX 12

#endif

/**************************************************************/
/* z80                                                        */
/**************************************************************/

#ifdef Z80

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - plain text 16b */
/* 4 - x              */
/* 5 - x          24b */
/* 6 - ?          16b */
/* 7 - x x        16b */
/* 8 - *           8b */
/* 9 - *          16b */
/* a - * x        24b */

#define OP_SIZE_MAX 20

#endif

/**************************************************************/
/* 65816                                                      */
/**************************************************************/

#ifdef W65816

/* opcode types */

/* 0 - plain text  8b */
/* 1 - x              */
/* 2 - ?              */
/* 3 - &              */
/* 4 - x/? (mem/acc)  */
/* 5 - x x            */
/* 6 - REP/SEP        */
/* 7 - x/? (index)    */
/* 8 - plain text 16b */
/* 9 - relative ?     */
/* a - x (absolute)   */

#define OP_SIZE_MAX 16

#endif


#ifdef AMIGA
#define amiga_short short
#else
#define amiga_short
#endif

struct optcode {
  char *op;
  int  hex;
  int  type;
#if defined(GB) || defined(Z80)
  amiga_short int value;
#endif
#ifdef Z80
  amiga_short int hex_x;
#endif
#ifdef BASED_ON_6502
  amiga_short int skip_8bit;
#endif
#ifdef W65816
  amiga_short int skip_xbit;
#endif
};


#define DEFINITION_TYPE_VALUE         0
#define DEFINITION_TYPE_STRING        1
#define DEFINITION_TYPE_STACK         2
#define DEFINITION_TYPE_ADDRESS_LABEL 3

struct definition {
  char   alias[MAX_NAME_LENGTH + 1];
  char   string[MAX_NAME_LENGTH + 1];
  double value;
  int    type;
  int    size;
};

struct append_section {
  char section[MAX_NAME_LENGTH + 1];
  char append_to[MAX_NAME_LENGTH + 1];
  struct append_section *next;
};

struct macro_argument {
  int type;
  double value;
  int start;
  char string[MAX_NAME_LENGTH + 1];
};

struct macro_static {
  char name[MAX_NAME_LENGTH + 1];
  int  start;
  int  calls;
  int  filename_id;
  int  start_line;
  int  nargument_names;
  char **argument_names;
  struct macro_static *next;
};

struct macro_incbin {
  unsigned char *data;
  int swap;
  int position;
  int left;
};

#define MACRO_CALLER_NORMAL 0
#define MACRO_CALLER_DBM    1
#define MACRO_CALLER_DWM    2
#define MACRO_CALLER_INCBIN 3

struct macro_runtime {
  struct macro_static *macro;
  int  macro_end;
  int  macro_end_line;
  int  macro_end_filename_id;
  int  supplied_arguments;
  int  caller;
  char string[MAX_NAME_LENGTH + 1];
  int  string_current;
  int  string_last;
  int  offset;
  struct macro_incbin *incbin_data;
  struct macro_argument **argument_data;
};

struct label_def {
  char label[MAX_NAME_LENGTH + 1];
  unsigned char section_status;
  unsigned char alive;
  unsigned char type;
  unsigned char symbol;
  int  section_id;
  int  address; /* in bank */
  int  bank;
  int  slot;
  int  base;
  int  filename_id;
  int  linenumber;
  struct section_def *section_struct;
  struct label_def *next;
};

struct section_def {
  char name[MAX_NAME_LENGTH + 1];
  int  alignment;
  int  address; /* in bank */
  int  bank;
  int  base;
  int  slot;
  int  size;
  int  status; /* see SECTION_STATUS_* */
  int  alive;
  int  id;
  int  i;
  int  filename_id;
  int  advance_org;
  int  maxsize_status; /* ON/OFF */
  int  maxsize;
  int  listfile_items;
  int  *listfile_ints;
  char *listfile_cmds;
  unsigned char *data;
  struct namespace_def *nspace;
  struct map_t *label_map;
  struct section_def *next;
};

struct incbin_file_data {
  struct incbin_file_data *next;
  char *data;
  char *name;
  int  size;
};

struct export_def {
  char   name[MAX_NAME_LENGTH + 1];
  struct export_def *next;
};

struct active_file_info {
  int    filename_id;
  int    line_current;
  struct active_file_info *next;
  struct active_file_info *prev;
};

struct file_name_info {
  char   *name;
  int    id;
  unsigned long checksum;
  struct file_name_info *next;
};

struct slot {
  int address;
  int size;
  struct slot *next;
};

struct block {
  char name[MAX_NAME_LENGTH + 1];
  int  address;
  int  filename_id;
  int  line_number;
  struct block *next;
};

struct stack {
  struct stack_item *stack;
  struct stack *next;
  int id;
  int position;
  int filename_id;
  int stacksize;
  int linenumber;
  int type;
  int bank;
  int slot;
  int relative_references;
  int base;
  int section_status;
  int section_id;
  int address;
};

struct structure_item {
  char name[MAX_NAME_LENGTH + 1];
  int size;
  struct structure_item *next;
};

struct structure {
  char name[MAX_NAME_LENGTH + 1];
  struct structure_item *items;
  int size;
  struct structure *next;
};

struct repeat_runtime {
  int start;
  int start_line;
  int counter;
  int repeats;
  char index_name[MAX_NAME_LENGTH + 1];
};

struct filepointer {
  char name[MAX_NAME_LENGTH + 1];
  char *filename;
  FILE *f;
  struct filepointer *next;
};

#define TYPE_STRING            0
#define TYPE_VALUE             1
#define TYPE_LABEL             2
#define TYPE_STACK_CALCULATION 3

#endif /* _DEFINES_H */


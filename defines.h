#include "hashmap.h"

#ifndef _DEFINES_H
#define _DEFINES_H

/* want to use longer strings and labels? change this */

#define MAX_NAME_LENGTH 255

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FAILED    0
#define SUCCEEDED 1

#define OFF 0
#define ON  1

#define NO  0
#define YES 1

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)

enum {
  HINT_NONE,
  HINT_8BIT,
  HINT_16BIT,
  HINT_24BIT
};

#define STACK_CALCULATE_DELAY 2
#define STACK_RETURN_LABEL 1024

enum {
  STACK_NONE,
  STACK_INSIDE,
  STACK_OUTSIDE
};

enum {
  SECTION_STATUS_FREE,
  SECTION_STATUS_FORCE,
  SECTION_STATUS_OVERWRITE,
  SECTION_STATUS_HEADER,
  SECTION_STATUS_SEMIFREE,
  SECTION_STATUS_ABSOLUTE,
  SECTION_STATUS_RAM,
  SECTION_STATUS_SUPERFREE,
  SECTION_STATUS_SEMISUBFREE
};

enum {
  INPUT_NUMBER_EOL = 2,
  INPUT_NUMBER_ADDRESS_LABEL,
  INPUT_NUMBER_STRING,
  INPUT_NUMBER_STACK,
  INPUT_NUMBER_FLOAT
};

enum {
  GET_NEXT_TOKEN_STRING = 2,
  EVALUATE_TOKEN_NOT_IDENTIFIED = 2,
  EVALUATE_TOKEN_EOP = 6
};

enum {
  OUTPUT_OBJECT,
  OUTPUT_LIBRARY,
  OUTPUT_NONE
};

enum {
  REFERENCE_TYPE_DIRECT_16BIT,
  REFERENCE_TYPE_RELATIVE_8BIT,
  REFERENCE_TYPE_DIRECT_8BIT,
  REFERENCE_TYPE_DIRECT_24BIT,
  REFERENCE_TYPE_RELATIVE_16BIT
};

enum {
  ERROR_NONE,
  ERROR_LOG,
  ERROR_DIR,
  ERROR_NUM,
  ERROR_INC,
  ERROR_INB,
  ERROR_UNF,
  ERROR_INP,
  ERROR_STC,
  ERROR_WRN,
  ERROR_ERR
};

#ifndef NDEBUG
  #define WLA_DEBUG 1
  #define WLALINK_DEBUG 1
#endif

#ifdef _CRT_SECURE_NO_WARNINGS
  #pragma warning(disable:4996) /* Just in case */
#endif

#ifdef AMIGA
  #define optcode_int short int
#else
  #define optcode_int int
#endif

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
#define CPU_STR "GB-Z80"
#define WLA_NAME "gb"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int value;
};

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
#define CPU_STR "6502"
#define WLA_NAME "6502"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int skip_8bit;
};

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
#define CPU_STR "WDC65C02"
#define WLA_NAME "65c02"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int skip_8bit;
};

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
#define CPU_STR "HUC6280"
#define WLA_NAME "huc6280"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int skip_8bit;
};

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
#define CPU_STR "MCS6510"
#define WLA_NAME "6512"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int skip_8bit;
};

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

#define OP_SIZE_MAX 12
#define CPU_STR "SPC700"
#define WLA_NAME "spc700"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
};

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
#define CPU_STR "Z80"
#define WLA_NAME "z80"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int hex_x;
  optcode_int value;
};

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
#define CPU_STR "W65816"
#define WLA_NAME "65816"

struct optcode {
  char *op;
  int  hex;
  optcode_int type;
  optcode_int skip_xbit;
};

#endif

#ifndef WLA_NAME
  #error "Unknown WLA_NAME!"
#endif
#ifndef CPU_STR
  #error "Unknown CPU_STR!"
#endif

enum {
  DEFINITION_TYPE_VALUE,
  DEFINITION_TYPE_STRING,
  DEFINITION_TYPE_STACK,
  DEFINITION_TYPE_ADDRESS_LABEL
};

struct definition {
  char   alias[MAX_NAME_LENGTH];
  char   string[MAX_NAME_LENGTH];
  double value;
  int    type;
  int    size;
};

struct append_section {
  char section[MAX_NAME_LENGTH];
  char append_to[MAX_NAME_LENGTH];
  struct append_section *next;
};

struct macro_argument {
  int type;
  int value;
  int start;
  char string[MAX_NAME_LENGTH];
};

struct macro_static {
  char name[MAX_NAME_LENGTH];
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

enum {
  MACRO_CALLER_NORMAL,
  MACRO_CALLER_DBM,
  MACRO_CALLER_DWM,
  MACRO_CALLER_INCBIN
};

struct macro_runtime {
  struct macro_static *macro;
  int  macro_end;
  int  macro_end_line;
  int  macro_end_filename_id;
  int  supplied_arguments;
  int  caller;
  char string[MAX_NAME_LENGTH];
  int  string_current;
  int  string_last;
  int  offset;
  struct macro_incbin *incbin_data;
  struct macro_argument **argument_data;
};

struct label_def {
  char label[MAX_NAME_LENGTH];
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
  char name[MAX_NAME_LENGTH];
  int  alignment;
  int  address; /* in bank */
  int  bank;
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

struct namespace_def {
  char name[MAX_NAME_LENGTH];
  struct map_t *label_map;
};

struct incbin_file_data {
  struct incbin_file_data *next;
  char *data;
  char *name;
  int  size;
};

struct export_def {
  char   name[MAX_NAME_LENGTH];
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
  struct file_name_info *next;
};

struct slot {
  int address;
  int size;
  struct slot *next;
};

struct block {
  char name[MAX_NAME_LENGTH];
  int  address;
  int  filename_id;
  int  line_number;
  struct block *next;
};

enum {
  STACK_ITEM_TYPE_VALUE,
  STACK_ITEM_TYPE_OPERATOR,
  STACK_ITEM_TYPE_STRING,
  STACK_ITEM_TYPE_DELETED,
  STACK_ITEM_TYPE_STACK
};

enum {
  SI_OP_PLUS,
  SI_OP_MINUS,
  SI_OP_MULTIPLY,
  SI_OP_LEFT,
  SI_OP_RIGHT,
  SI_OP_OR,
  SI_OP_AND,
  SI_OP_DIVIDE,
  SI_OP_POWER,
  SI_OP_SHIFT_LEFT,
  SI_OP_SHIFT_RIGHT,
  SI_OP_MODULO,
  SI_OP_XOR,
  SI_OP_LOW_BYTE,
  SI_OP_HIGH_BYTE
};

enum {
  SI_SIGN_POSITIVE,
  SI_SIGN_NEGATIVE
};

enum {
  STACKS_TYPE_8BIT,
  STACKS_TYPE_16BIT,
  STACKS_TYPE_24BIT,
  STACKS_TYPE_UNKNOWN
};

enum {
  STACK_POSITION_DEFINITION,
  STACK_POSITION_CODE
};

struct stack_item {
  int type;
  int sign;
  double value;
  char string[MAX_NAME_LENGTH];
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
  char name[MAX_NAME_LENGTH];
  int size;
  struct structure_item *next;
};

struct structure {
  char name[MAX_NAME_LENGTH];
  struct structure_item *items;
  int size;
  struct structure *next;
};

struct repeat_runtime {
  int start;
  int start_line;
  int counter;
  int repeats;
  char index_name[MAX_NAME_LENGTH];
};

struct filepointer {
  char name[MAX_NAME_LENGTH];
  char *filename;
  FILE *f;
  struct filepointer *next;
};

enum {
  TYPE_STRING,
  TYPE_VALUE,
  TYPE_LABEL,
  TYPE_STACK_CALCULATION
};

#endif

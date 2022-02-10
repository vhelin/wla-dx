
#ifndef _SHARED_H
#define _SHARED_H

/* want to use longer strings and labels? change this - PS. it doesn't contain the null terminator */
#if defined(AMIGA) || (defined(MSDOS) && !defined(WIN32))
#define MAX_NAME_LENGTH 511
#else
#define MAX_NAME_LENGTH 2047
#endif

#define MAX_FLOAT_DIGITS 25
#define INT_MAX_DECIMAL_DIGITS 10

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef NDEBUG
  #define WLA_DEBUG 1
  #define WLALINK_DEBUG 1
#endif

#ifdef _CRT_SECURE_NO_WARNINGS
  #pragma warning(disable:4996) /* Just in case */
#endif

#define FAILED    0
#define SUCCEEDED 1

#define OFF 0
#define ON  1

#define NO  0
#define YES 1

#define TRUE  1
#define FALSE 0

#define STACK_ITEM_TYPE_VALUE    0
#define STACK_ITEM_TYPE_OPERATOR 1
#define STACK_ITEM_TYPE_STRING   2
#define STACK_ITEM_TYPE_DELETED  3
#define STACK_ITEM_TYPE_STACK    4

#define SI_OP_ADD          0
#define SI_OP_SUB          1
#define SI_OP_MULTIPLY     2
#define SI_OP_LEFT         3
#define SI_OP_RIGHT        4
#define SI_OP_OR           5
#define SI_OP_AND          6
#define SI_OP_DIVIDE       7
#define SI_OP_POWER        8
#define SI_OP_SHIFT_LEFT   9
#define SI_OP_SHIFT_RIGHT 10
#define SI_OP_MODULO      11
#define SI_OP_XOR         12
#define SI_OP_LOW_BYTE    13
#define SI_OP_HIGH_BYTE   14
#define SI_OP_NOT         15
#define SI_OP_BANK        16
#define SI_OP_COMPARE_LT  17
#define SI_OP_COMPARE_GT  18
#define SI_OP_COMPARE_EQ  19
#define SI_OP_COMPARE_NEQ 20
#define SI_OP_COMPARE_LTE 21
#define SI_OP_COMPARE_GTE 22
#define SI_OP_LOGICAL_OR  23
#define SI_OP_LOGICAL_AND 24
#define SI_OP_LOW_WORD    25
#define SI_OP_HIGH_WORD   26
#define SI_OP_BANK_BYTE   27

#define SI_SIGN_POSITIVE 0
#define SI_SIGN_NEGATIVE 1

#define STACK_TYPE_8BIT    0
#define STACK_TYPE_16BIT   1
#define STACK_TYPE_24BIT   2
#define STACK_TYPE_UNKNOWN 3
#define STACK_TYPE_13BIT   4
#define STACK_TYPE_32BIT   5
#define STACK_TYPE_BITS    6
#define STACK_TYPE_9BIT_SHORT 7

#define STACK_POSITION_DEFINITION 0
#define STACK_POSITION_CODE       1

struct namespace_def {
  char name[MAX_NAME_LENGTH + 1];
  map_t *label_map;
};

struct label_sizeof {
  char name[MAX_NAME_LENGTH + 1];
  int size;
  int file_id;
  struct label_sizeof *next;
};

#define SECTION_STATUS_FREE             0
#define SECTION_STATUS_FORCE            1
#define SECTION_STATUS_OVERWRITE        2
#define SECTION_STATUS_HEADER           3
#define SECTION_STATUS_SEMIFREE         4
#define SECTION_STATUS_ABSOLUTE         5
#define SECTION_STATUS_RAM_FREE         6
#define SECTION_STATUS_SUPERFREE        7
#define SECTION_STATUS_SEMISUBFREE      8
#define SECTION_STATUS_RAM_FORCE        9
#define SECTION_STATUS_RAM_SEMIFREE    10
#define SECTION_STATUS_RAM_SEMISUBFREE 11

#define REFERENCE_TYPE_DIRECT_16BIT   0
#define REFERENCE_TYPE_RELATIVE_8BIT  1
#define REFERENCE_TYPE_DIRECT_8BIT    2
#define REFERENCE_TYPE_DIRECT_24BIT   3
#define REFERENCE_TYPE_RELATIVE_16BIT 4
#define REFERENCE_TYPE_DIRECT_13BIT   5
#define REFERENCE_TYPE_DIRECT_32BIT   6
#define REFERENCE_TYPE_BITS           7
#define REFERENCE_TYPE_DIRECT_9BIT_SHORT 8

#endif /* _SHARED_H */


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

#define DEFAULT_SCREEN_DX 80
#define DEFAULT_SCREEN_DY 24

#ifndef NDEBUG
  #define WLA_DEBUG 1
  #define WLALINK_DEBUG 1
#endif

#ifdef _CRT_SECURE_NO_WARNINGS
  #pragma warning(disable:4996) /* Just in case */
#endif

#ifdef PROFILE_FUNCTIONS
#include <time.h>
#include <sys/time.h>
#define PROFILE_MAX_ENTRIES (1024*1024)
#define PROFILE_GLOBALS() double g_profile_times_ms[PROFILE_MAX_ENTRIES]; \
  char *g_profile_function_names[PROFILE_MAX_ENTRIES]; \
  int g_profile_entry_id = 0
#define PROFILE_GLOBALS_EXTERN() extern double g_profile_times_ms[PROFILE_MAX_ENTRIES]; \
  extern char *g_profile_function_names[PROFILE_MAX_ENTRIES]; \
  extern int g_profile_entry_id
#define PROFILE_AT_EXIT() { \
  if (g_profile_entry_id > 0) { \
    FILE *file_names, *file_times; \
    int loop; \
    unsigned char *cp; \
    file_names = fopen("_profile_function_names.wla", "ab"); \
    file_times = fopen("_profile_function_times.wla", "ab"); \
    fprintf(file_names, "0 "); \
    fprintf(file_times, "%c%c%c%c%c%c%c%c", 0, 0, 0, 0, 0, 0, 0, 0); \
    for (loop = 0; loop < g_profile_entry_id; loop++) { \
      fprintf(file_names, "%s ", g_profile_function_names[loop]); \
      cp = (unsigned char *)&g_profile_times_ms[loop]; \
      fprintf(file_times, "%c%c%c%c%c%c%c%c", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]); \
    } \
  } \
}
/* CSV:
  if (g_profile_entry_id > 0) { \
    FILE *file_cvs; \
    int loop; \
    file_cvs = fopen("_profile_function_cvs.cvs", "ab"); \
    for (loop = 0; loop < g_profile_entry_id; loop++) \
      fprintf(file_cvs, "%s,%f\n", g_profile_function_names[loop], g_profile_times_ms[loop]);  \
  } \
}
*/
#define PROFILE_VARIABLES() struct timeval time_begin, time_end; \
  static int is_profiling = NO; \
  int output_profiling_data = NO;
#define PROFILE_START() if (is_profiling == NO) { is_profiling = YES; output_profiling_data = YES; gettimeofday(&time_begin, NULL); }
#define PROFILE_END(function_name) if (output_profiling_data == YES) { gettimeofday(&time_end, NULL); \
  g_profile_times_ms[g_profile_entry_id] = (time_end.tv_sec + time_end.tv_usec / 1e6 - time_begin.tv_sec - time_begin.tv_usec / 1e6) * 1000.0; \
  if (g_profile_times_ms[g_profile_entry_id] < 0.0) g_profile_times_ms[g_profile_entry_id] = 0.0; \
  g_profile_function_names[g_profile_entry_id++] = function_name; \
  is_profiling = NO; output_profiling_data = NO; }
/*
#define PROFILE_VARIABLES() struct timespec time_begin, time_end;
#define PROFILE_START() clock_gettime(CLOCK_MONOTONIC, &time_begin);
#define PROFILE_END(function_name) clock_gettime(CLOCK_MONOTONIC, &time_end); \
  fprintf(stderr, function_name " = %fms.\n", (time_end.tv_nsec - time_begin.tv_nsec) / 1000000.0 + (time_end.tv_sec  - time_begin.tv_sec));
*/
/*
#define PROFILE_VARIABLES() clock_t clock_start, clock_time;
#define PROFILE_START() clock_start = clock();
#define PROFILE_END(function_name) clock_time = ((double) (clock() - clock_start)) / (CLOCKS_PER_SEC / 1000.0); \
  fprintf(stderr, function_name " = %fms.\n", (double)clock_time);
*/
#else
#define PROFILE_GLOBALS() extern int g_profile_entry_id
#define PROFILE_GLOBALS_EXTERN() extern int g_profile_entry_id
#define PROFILE_AT_EXIT()
#define PROFILE_VARIABLES()
#define PROFILE_START()
#define PROFILE_END(function_name)
#endif

#define FAILED    0
#define SUCCEEDED 1

#define OFF 0
#define ON  1

#define NO  0
#define YES 1

#define TRUE  1
#define FALSE 0

#define NOT_APPLICABLE 2

#define STACK_ITEM_TYPE_VALUE    0
#define STACK_ITEM_TYPE_OPERATOR 1
#define STACK_ITEM_TYPE_LABEL    2
#define STACK_ITEM_TYPE_DELETED  3
#define STACK_ITEM_TYPE_STACK    4
#define STACK_ITEM_TYPE_STRING   5

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
#define SI_OP_ROUND       28
#define SI_OP_CEIL        29
#define SI_OP_FLOOR       30
#define SI_OP_MIN         31
#define SI_OP_MAX         32
#define SI_OP_SQRT        33
#define SI_OP_ABS         34
#define SI_OP_COS         35
#define SI_OP_SIN         36
#define SI_OP_TAN         37
#define SI_OP_ACOS        38
#define SI_OP_ASIN        39
#define SI_OP_ATAN        40
#define SI_OP_NEGATE      41
#define SI_OP_ATAN2       42
#define SI_OP_COSH        43
#define SI_OP_SINH        44
#define SI_OP_TANH        45
#define SI_OP_LOG         46
#define SI_OP_LOG10       47
#define SI_OP_POW         48
#define SI_OP_CLAMP       49
#define SI_OP_SIGN        50

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
#define STACK_TYPE_16BIT_WRAP_AROUND 8

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
#define SECTION_STATUS_SEMISUPERFREE   12

#define REFERENCE_TYPE_DIRECT_16BIT   0
#define REFERENCE_TYPE_RELATIVE_8BIT  1
#define REFERENCE_TYPE_DIRECT_8BIT    2
#define REFERENCE_TYPE_DIRECT_24BIT   3
#define REFERENCE_TYPE_RELATIVE_16BIT 4
#define REFERENCE_TYPE_DIRECT_13BIT   5
#define REFERENCE_TYPE_DIRECT_32BIT   6
#define REFERENCE_TYPE_BITS           7
#define REFERENCE_TYPE_DIRECT_9BIT_SHORT 8
#define REFERENCE_TYPE_RELATIVE_16BIT_WRAP_AROUND 9

#endif /* _SHARED_H */

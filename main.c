
/*
  wla - part of wla dx gb-z80/z80/6502/65c02/6510/6800/6801/6809/65816/huc6280/spc-700/8008/8080/SuperFX
  macro assembler package by ville helin <ville.helin@iki.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef UNIX
#include <unistd.h>
#elif defined(WIN32)
/* Windows.h can't be included since it relies on compiler extensions */
typedef unsigned long DWORD;
DWORD __stdcall GetCurrentProcessId(void);
#endif

#include "main.h"
#include "defines.h"

#include "parse.h"
#include "include_file.h"
#include "pass_1.h"
#include "pass_2.h"
#include "pass_3.h"
#include "pass_4.h"
#include "listfile.h"
#include "hashmap.h"
#include "printf.h"
#include "mersenne_twister.h"
#include "stack.h"


FILE *g_file_out_ptr = NULL;

/* amiga specific definitions */

#ifdef AMIGA
__near long __stack = 200000;
#endif

char g_version_string[] = "$VER: wla-" WLA_NAME " 10.1a (6.2.2022)";
char g_wla_version[] = "10.1";

char g_tmp_name[MAX_NAME_LENGTH + 1], g_makefile_tmp_name[MAX_NAME_LENGTH + 1];

extern struct incbin_file_data *g_incbin_file_data_first, *g_ifd_tmp;
extern struct file_name_info *g_file_name_info_first;
extern struct label_def *g_label_tmp, *g_labels;
extern struct map_t *g_global_unique_label_map;
extern struct macro_static *g_macros_first;
extern struct definition *g_tmp_def;
extern struct map_t *g_defines_map;
extern struct export_def *g_export_first, *g_export_last;
extern struct stack *g_stacks_first, *g_stacks_tmp, *g_stacks_last, *g_stacks_header_first, *g_stacks_header_last;
extern struct repeat_runtime *g_repeat_stack;
extern struct section_def *g_sections_first;
extern struct macro_runtime *g_macro_stack;
extern struct label_def *g_unknown_labels;
extern struct label_def *g_unknown_header_labels;
extern struct filepointer *g_filepointers;
extern struct map_t *g_namespace_map;
extern struct after_section *g_after_sections;
extern struct label_sizeof *g_label_sizeofs;
extern struct block_name *g_block_names;
extern struct stringmaptable *g_stringmaptables;
extern struct array *g_arrays_first;
extern struct structure *g_structures_first;
extern struct structure **g_saved_structures;
extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern char *g_label_stack[256];
extern char *g_include_in_tmp, *g_tmp_a;
extern char *g_rom_banks, *g_rom_banks_usage_table;
extern char *g_include_dir, *g_buffer, *g_full_name;
extern int g_include_in_tmp_size, g_tmp_a_size, *g_banks, *g_bankaddress;
extern int g_saved_structures_count, g_saved_structures_max2;

int g_output_format = OUTPUT_NONE, g_verbose_mode = OFF, g_test_mode = OFF;
int g_extra_definitions = OFF, g_commandline_parsing = ON, g_makefile_rules = NO;
int g_listfile_data = NO, g_quiet = NO, g_use_incdir = NO, g_little_endian = YES;
int g_create_sizeof_definitions = YES, g_global_label_hint = HINT_NONE, g_keep_empty_sections = NO;
int g_saved_structures_count2 = 0, g_saved_structures_max2 = 0;

char *g_final_name = NULL, *g_asm_name = NULL;

struct ext_include_collection g_ext_incdirs;
struct structure **g_saved_structures2 = NULL;



int main(int argc, char *argv[]) {

  int parse_flags_result, print_usage = YES, n_ctr, q, include_size = 0;

  if (sizeof(double) != 8) {
    fprintf(stderr, "MAIN: sizeof(double) == %d != 8. WLA will not work properly.\n", (int)sizeof(double));
    return 1;
  }

  atexit(procedures_at_exit);

  /* init the randon number generator */
  init_genrand((unsigned long)time(NULL));

  /* zero the tmp name for internal symbol stream and makefile generation */
  g_tmp_name[0] = 0;
  g_makefile_tmp_name[0] = 0;
  
  /* initialize our external include dir collection */
  g_ext_incdirs.count = 0;
  g_ext_incdirs.names = NULL;
  g_ext_incdirs.max_name_size_bytes = MAX_NAME_LENGTH + 1;

  /* select little/big endianess */
#if defined(MC6800) || defined(MC6801) || defined(MC6809)
  g_little_endian = NO;
#else
  g_little_endian = YES;
#endif
  
  /* init mem_insert() buffer */
  g_mem_insert_action[0] = 0;
  
  /* init hashmaps */
  g_defines_map = hashmap_new();
  g_global_unique_label_map = hashmap_new();
  g_namespace_map = hashmap_new();

  /* init label stack */
  for (q = 0; q < 256; q++)
    g_label_stack[q] = NULL;
  for (q = 0; q < 256; q++) {
    g_label_stack[q] = calloc(MAX_NAME_LENGTH + 1, 1);
    if (g_label_stack[q] == NULL)
      return 1;
  }
  
  parse_flags_result = FAILED;
  if (argc >= 2) {
    parse_flags_result = parse_flags(argv, argc, &print_usage);

    if (parse_flags_result == FAILED && print_usage == NO)
      return 1;
    
    if (g_output_format == OUTPUT_NONE) {
      if (parse_flags_result == SUCCEEDED) {
        /* assume object file output name */
        g_output_format = OUTPUT_OBJECT;
        g_final_name = calloc(strlen(g_asm_name)+1, 1);
        for (n_ctr = 0; n_ctr < (int)strlen(g_asm_name) && *((g_asm_name) + n_ctr) != '.'; n_ctr++)
          g_final_name[n_ctr] = *((g_asm_name) + n_ctr);
        g_final_name[n_ctr++] = '.';
        g_final_name[n_ctr++] = 'o';
        g_final_name[n_ctr] = 0;
      }
    }
  }
  
  if (g_output_format == OUTPUT_NONE || parse_flags_result == FAILED) {
    char title[] = "WLA " ARCH_STR " Macro Assembler v10.1a";
    int length, left, right;

    length = (int)strlen(title);
    left = (70 - 3 - 3 - length) / 2;
    right = 70 - 3 - 3 - left - length;

    printf("----------------------------------------------------------------------\n");
    printf("---");
    for (q = 0; q < left; q++)
      printf(" ");
    printf("%s", title);
    for (q = 0; q < right; q++)
      printf(" ");
    printf("---\n");
    printf("----------------------------------------------------------------------\n");
    printf("                Programmed by Ville Helin in 1998-2008\n");
    printf("        In GitHub since 2014: https://github.com/vhelin/wla-dx\n");

    length = (int)strlen(g_version_string);
    left = (70 - length) / 2;

    for (q = 0; q < left; q++)
      printf(" ");
    printf("%s", g_version_string);
    
    printf("\n\n");

#ifdef WLA_DEBUG
    printf("***  WLA_DEBUG defined - this executable is running in DEBUG mode  ***\n");
    printf("\n");
#endif

    printf("USAGE: %s [OPTIONS] <OUTPUT> <ASM FILE>\n\n", argv[0]);
    printf("OPTIONS:\n");
    printf("-h  Assume all label references are 16-bit by default (size hints\n");
    printf("    still work)\n");
    printf("-i  Add list file information\n");
    printf("-k  Keep empty sections\n");
    printf("-M  Output makefile rules\n");
    printf("-q  Quiet\n");
    printf("-s  Don't create _sizeof_* definitions\n");
    printf("-t  Test compile\n");
    printf("-v  Verbose messages\n");
    printf("-x  Extra compile time labels & definitions\n");
    printf("-I <DIR>  Include directory\n");
    printf("-D <DEF>  Declare definition\n\n");
    printf("OUTPUT:\n");
    printf("-o <FILE>  Output object file\n");
    printf("-l <FILE>  Output library file\n\n");
    printf("EXAMPLE: %s -D VER=1 -D TWO=2 -v -o main.o main.asm\n\n", argv[0]);
    return 0;
  }

  if (strcmp(g_asm_name, g_final_name) == 0) {
    fprintf(stderr, "MAIN: Input and output files share the same name!\n");
    return 1;
  }

  generate_tmp_name(g_tmp_name);

  g_file_out_ptr = fopen(g_tmp_name, "wb+");
  if (g_file_out_ptr == NULL) {
    fprintf(stderr, "MAIN: Error opening file \"%s\" for reading and writing.\n", g_tmp_name);
    return 1;
  }

  /* small inits */
  if (g_extra_definitions == ON)
    generate_extra_definitions();

  g_commandline_parsing = OFF;

  /* start the process */
  if (include_file(g_asm_name, &include_size, NULL) == FAILED)
    return 1;

  if (pass_1() == FAILED)
    return 1;
  if (pass_2() == FAILED)
    return 1;
  if (pass_3() == FAILED)
    return 1;
  if (g_listfile_data == YES) {
    if (listfile_collect() == FAILED)
      return 1;
  }
  if (pass_4() == FAILED)
    return 1;

  return 0;
}


int parse_flags(char **flags, int flagc, int *print_usage) {

  int asm_name_def = 0, count;
  char *str_build;
  
  for (count = 1; count < flagc; count++) {
    if (!strcmp(flags[count], "-o")) {
      if (g_output_format != OUTPUT_NONE)
        return FAILED;
      g_output_format = OUTPUT_OBJECT;
      if (count + 1 < flagc) {
        /* set output */
        g_final_name = calloc(strlen(flags[count+1])+1, 1);
        strcpy(g_final_name, flags[count+1]);
      }
      else
        return FAILED;

      count++;
      continue;
    }
    else if (!strcmp(flags[count], "-l")) {
      if (g_output_format != OUTPUT_NONE)
        return FAILED;
      g_output_format = OUTPUT_LIBRARY;
      if (count + 1 < flagc) {
        /* set output */
        g_final_name = calloc(strlen(flags[count+1])+1, 1);
        strcpy(g_final_name, flags[count+1]);
      }
      else
        return FAILED;

      count++;
      continue;
    }
    else if (!strcmp(flags[count], "-D")) {
      if (count + 1 < flagc) {
        if (count + 3 < flagc) {
          if (!strcmp(flags[count+2], "=")) {
            int length = (int)strlen(flags[count+1])+(int)strlen(flags[count+3])+2;
            str_build = calloc(length, 1);
            snprintf(str_build, length, "%s=%s", flags[count+1], flags[count+3]);
            if (parse_and_add_definition(str_build, NO) == FAILED) {
              *print_usage = NO;
              free(str_build);
              return FAILED;
            }
            free(str_build);
            count += 2;
          }
          else {
            if (parse_and_add_definition(flags[count+1], NO) == FAILED) {
              *print_usage = NO;
              return FAILED;
            }
          }
        }
        else {
          if (parse_and_add_definition(flags[count+1], NO) == FAILED) {
            *print_usage = NO;
            return FAILED;
          }
        }
      }
      else
        return FAILED;

      count++;
      continue;
    }
    else if (!strcmp(flags[count], "-I")) {
      if (count + 1 < flagc) {
        /* get arg */
        parse_and_add_incdir(flags[count+1], NO);
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
    else if (!strcmp(flags[count], "-k")) {
      g_keep_empty_sections = YES;
      continue;
    }
    else if (!strcmp(flags[count], "-v")) {
      g_verbose_mode = ON;
      continue;
    }
    else if (!strcmp(flags[count], "-h")) {
      g_global_label_hint = HINT_16BIT;
      continue;
    }
    else if (!strcmp(flags[count], "-s")) {
      g_create_sizeof_definitions = NO;
      continue;
    }
    else if (!strcmp(flags[count], "-t")) {
      g_test_mode = ON;
      continue;
    }
    else if (!strcmp(flags[count], "-M")) {
      g_makefile_rules = YES;
      g_test_mode = ON;
      g_verbose_mode = OFF;
      g_quiet = YES;
      /* if file loading requires a tmp file, this will be its name */
      generate_tmp_name(g_makefile_tmp_name);
      continue;
    }
    else if (!strcmp(flags[count], "-q")) {
      g_quiet = YES;
      continue;
    }
    else if (!strcmp(flags[count], "-x")) {
      g_extra_definitions = ON;
      continue;
    }
    else {
      if (count == flagc - 1) {
        g_asm_name = calloc(strlen(flags[count]) + 1, 1);
        strcpy(g_asm_name, flags[count]);
        count++;
        asm_name_def++;
      }
      else {
        /* legacy support? */
        if (strncmp(flags[count], "-D", 2) == 0) {
          /* old define */
          if (parse_and_add_definition(flags[count], YES) == FAILED) {
            *print_usage = NO;
            return FAILED;
          }
          continue;
        }
        else if (strncmp(flags[count], "-I", 2) == 0) {
          /* old include directory */
          parse_and_add_incdir(flags[count], YES);
          continue;
        }
        else
          return FAILED;
      }
    }
  }
  
  if (asm_name_def <= 0)
    return FAILED;
  
  return SUCCEEDED;
}


static void _remember_deletable_structure(struct structure *st) {

  int i;

  /* do we already remember the structure? */
  for (i = 0; i < g_saved_structures_count2; i++) {
    if (g_saved_structures2[i] == st)
      return;
  }

  if (g_saved_structures_count2 >= g_saved_structures_max2) {
    g_saved_structures_max2 += 256;
    g_saved_structures2 = realloc(g_saved_structures2, sizeof(struct structure *) * g_saved_structures_max2);
    if (g_saved_structures2 == NULL) {
      fprintf(stderr, "_remember_deletable_structure(): Out of memory error.\n");
      return;
    }
  }
  
  g_saved_structures2[g_saved_structures_count2++] = st;
}


static void _mark_struct_deleted(struct structure *st) {

  struct structure_item *si = st->items;

  if (st->alive == NO)
    return;

  st->items = NULL;
  st->alive = NO;
  
  while (si != NULL) {
    struct structure_item *tmp = si;

    if (si->type == STRUCTURE_ITEM_TYPE_UNION)
      _mark_struct_deleted(si->union_items);
    /* don't free si->instance for STRUCTURE_ITEM_TYPE_INSTANCE since that's a reusable
       structure */
    si = si->next;
    free(tmp);
  }

  _remember_deletable_structure(st);
}


void procedures_at_exit(void) {

  struct file_name_info *f, *ft;
  struct export_def *export_tmp;
  struct section_def *s1, *s2;
  struct label_def *l1, *l2;
  struct macro_static *m;
  struct filepointer *f1, *f2;
  struct after_section *as;
  struct label_sizeof *ls;
  struct block_name *bn;
  struct array *ar1, *ar2;
  int i, index;
  
  /* free all the dynamically allocated data structures and close open files */
  if (g_file_out_ptr != NULL)
    fclose(g_file_out_ptr);

  free(g_macro_stack);
  free(g_repeat_stack);
  free(g_final_name);
  free(g_asm_name);
  free(g_include_dir);
  free(g_full_name);

  /* NOTE: the implementation of structures leaked memory and the only way i (Ville Helin) was able to
     fix the leak was to collect all allocated structures, and then unroll them here and free them
     all here, finally... if you come up with a better solution, thank you */
  for (i = 0; i < g_saved_structures_count; i++)
    _mark_struct_deleted(g_saved_structures[i]);
  for (i = 0; i < g_saved_structures_count2; i++)
    free(g_saved_structures2[i]);
  free(g_saved_structures);
  free(g_saved_structures2);

  /*
  st = g_structures_first;
  while (st != NULL) {
    struct structure *st2 = st->next;
    _free_struct(st);
    st = st2;
  }
  */

  for (i = 0; i < 256; i++)
    free(g_label_stack[i]);

  /* NOTE! freeing the sections must come before freeing g_defines_map, as section's nspace
     requires that. don't really know why, but Valgrind pointed to that direction, it has
     something to do with section->nspace->label_map... */
  s1 = g_sections_first;
  while (s1 != NULL) {
    free(s1->data);
    free(s1->listfile_cmds);
    free(s1->listfile_ints);
    if (s1->label_map != NULL) {
      hashmap_free(s1->label_map);
      s1->label_map = NULL;
    }
    if (s1->nspace != NULL) {
      if (s1->nspace->label_map != NULL) {
        hashmap_free(s1->nspace->label_map);
        s1->nspace->label_map = NULL;
      }
    }
    s2 = s1->next;
    free(s1);
    s1 = s2;
  }

  if (g_defines_map != NULL) {
    hashmap_free_all_elements(g_defines_map);
    hashmap_free(g_defines_map);
  }

  if (g_global_unique_label_map != NULL) {
    /* don't free_all_elements, since labels contains _all_ labels. */
    hashmap_free(g_global_unique_label_map);
  }

  if (g_namespace_map != NULL) {
    hashmap_free_all_elements(g_namespace_map);
    hashmap_free(g_namespace_map);
  }

  m = g_macros_first;
  while (m != NULL) {
    /* free the argument labels */
    if (m->nargument_names > 0) {
      for (i = 0; i < m->nargument_names; i++)
        free(m->argument_names[i]);
      free(m->argument_names);
    }
    g_macros_first = m->next;
    free(m);
    m = g_macros_first;
  }

  g_label_tmp = g_labels;
  while (g_label_tmp != NULL) {
    g_labels = g_label_tmp->next;
    free(g_label_tmp);
    g_label_tmp = g_labels;
  }

  l1 = g_unknown_labels;
  while (l1 != NULL) {
    l2 = l1->next;
    free(l1);
    l1 = l2;
  }

  l1 = g_unknown_header_labels;
  while (l1 != NULL) {
    l2 = l1->next;
    free(l1);
    l1 = l2;
  }

  ls = g_label_sizeofs;
  while (ls != NULL) {
    g_label_sizeofs = ls->next;
    free(ls);
    ls = g_label_sizeofs;
  }

  bn = g_block_names;
  while (bn != NULL) {
    g_block_names = bn->next;
    free(bn);
    bn = g_block_names;
  }
  
  export_tmp = g_export_first;
  while (export_tmp != NULL) {
    g_export_last = export_tmp->next;
    free(export_tmp);
    export_tmp = g_export_last;
  }

  g_ifd_tmp = g_incbin_file_data_first;
  while(g_ifd_tmp != NULL) {
    g_incbin_file_data_first = g_ifd_tmp->next;
    free(g_ifd_tmp->data);
    free(g_ifd_tmp->name);
    free(g_ifd_tmp);
    g_ifd_tmp = g_incbin_file_data_first;
  }

  g_stacks_tmp = g_stacks_first;
  while (g_stacks_tmp != NULL) {
    free(g_stacks_tmp->stack);
    g_stacks_first = g_stacks_tmp->next;
    free(g_stacks_tmp);
    g_stacks_tmp = g_stacks_first;
  }

  g_stacks_tmp = g_stacks_header_first;
  while (g_stacks_tmp != NULL) {
    free(g_stacks_tmp->stack);
    g_stacks_first = g_stacks_tmp->next;
    free(g_stacks_tmp);
    g_stacks_tmp = g_stacks_first;
  }

  as = g_after_sections;
  while (as != NULL) {
    g_after_sections = as->next;
    free(as);
    as = g_after_sections;
  }

  free(g_buffer);
  free(g_include_in_tmp);
  free(g_tmp_a);
  free(g_rom_banks);
  free(g_rom_banks_usage_table);
  free(g_banks);
  free(g_bankaddress);

  f = g_file_name_info_first;
  while (f != NULL) {
    free(f->name);
    ft = f->next;
    free(f);
    f = ft;
  }

  f1 = g_filepointers;
  while (f1 != NULL) {
    f2 = f1->next;
    if (f1->f != NULL)
      fclose(f1->f);
    free(f1->filename);
    free(f1);
    f1 = f2;
  }

  ar1 = g_arrays_first;
  while (ar1 != NULL) {
    ar2 = ar1->next;
    free(ar1->data);
    free(ar1);
    ar1 = ar2;
  }
  
  while (g_stringmaptables != NULL) {
    struct stringmaptable *sm = g_stringmaptables;
    while (sm->entries != NULL) {
      struct stringmap_entry *e = sm->entries;
      free(e->bytes);
      free(e->text);
      sm->entries = e->next;
      free(e);
    }
    free(sm->filename);
    g_stringmaptables = sm->next;
    free(sm);
  }

  data_stream_parser_free();
  
  /* remove the tmp files */
  if (g_tmp_name[0] != 0)
    remove(g_tmp_name);
  if (g_makefile_tmp_name[0] != 0)
    remove(g_makefile_tmp_name);

  /* cleanup any incdirs we added */
  for (index = 0; index < g_ext_incdirs.count; index++)
    free(g_ext_incdirs.names[index]);
  free(g_ext_incdirs.names);
}


/* NOTE: filename must contain at least 32 bytes */
int generate_tmp_name(char *filename) {

  static int running_id = 0;
  char name[32]; /* should be enough */
  int status;
  int pid;

#if defined(UNIX)
  char header[] = ".wla";
  
  pid = (int)getpid();
#elif defined(WIN32)
  char header[] = ".wla";

  pid = GetCurrentProcessId();
#else /* for Amiga, MSDOS... */
  char header[] = "wla";

  pid = 0;
#endif

  status = snprintf(name, sizeof(name)-1, "%s%da%d", header, pid, running_id) + 1;
  running_id++;
  if (status >= (int)sizeof(name)) {
    fprintf(stderr, "MAIN: Temp filename exceeded limit: %d >= %d! "
            "Aborting...\n", status, (int)sizeof(name));
    abort();
  }

  /* copy the filename out */
  strcpy(filename, name);

  return SUCCEEDED;
}


int generate_extra_definitions(void) {

  char *q, tmp[256];
  time_t ti;
  int i, length;

  /* generate WLA_TIME */
  time(&ti);
  q = ctime(&ti);
  strcpy(tmp, q);
  /* remove the linefeed */
  length = (int)strlen(tmp);
  for (i = 0; i < length; i++) {
    if (tmp[i] == 0x0A) {
      tmp[i] = 0;
      break;
    }
  }

  if (add_a_new_definition("WLA_TIME", 0.0, tmp, DEFINITION_TYPE_STRING, (int)strlen(tmp)) == FAILED)
    return FAILED;
  if (add_a_new_definition("wla_time", 0.0, tmp, DEFINITION_TYPE_STRING, (int)strlen(tmp)) == FAILED)
    return FAILED;
  if (add_a_new_definition("WLA_VERSION", 0.0, g_wla_version, DEFINITION_TYPE_STRING, (int)strlen(g_wla_version)) == FAILED)
    return FAILED;
  if (add_a_new_definition("wla_version", 0.0, g_wla_version, DEFINITION_TYPE_STRING, (int)strlen(g_wla_version)) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int parse_and_add_definition(char *c, int contains_flag) {

  char n[MAX_NAME_LENGTH + 1], *value;
  int i, k;

  /* skip the flag? */
  if (contains_flag == YES)
    c += 2;
  
  for (i = 0; i < MAX_NAME_LENGTH && *c != 0 && *c != '='; i++, c++)
    n[i] = *c;
  n[i] = 0;

  if (*c == 0)
    return add_a_new_definition(n, 0.0, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (*c == '=') {
    c++;
    if (*c == 0)
      return FAILED;

    value = c;

    /* hexadecimal value? */
    if (*c == '$' || (*c == '0' && (*(c+1) == 'x' || *(c+1) == 'X')) || ((c[strlen(c)-1] == 'h' || c[strlen(c)-1] == 'H') && ((*c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'f') || (*c >= 'A' && *c <= 'F')))) {
      if (*c == '$')
        c++;
      else if (*c == '0' && (*(c+1) == 'x' || *(c+1) == 'X'))
        c += 2;
      for (i = 0; *c != 0; c++) {
        if (*c >= '0' && *c <= '9')
          i = (i << 4) + *c - '0';
        else if (*c >= 'a' && *c <= 'f')
          i = (i << 4) + *c - 'a' + 10;
        else if (*c >= 'A' && *c <= 'F')
          i = (i << 4) + *c - 'A' + 10;
        else if ((*c == 'h' || *c == 'H') && *(c+1) == 0)
          break;
        else {
          fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Error in value (%s).\n", value);
          return FAILED;
        }
      }
      return add_a_new_definition(n, (double)i, NULL, DEFINITION_TYPE_VALUE, 0);
    }

    /* binary value? */
    if (*c == '%' || (*c == '0' && (*(c+1) == 'b' || *(c+1) == 'B'))) {
      if (*c == '%')
        c++;
      else
        c += 2;
      for (i = 0, k = 0; *c != 0 && k < 32; k++, c++) {
        if (*c == '0' || *c == '1')
          i = (i << 1) + *c - '0';
        else {
          fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Error in value (%s).\n", value);
          return FAILED;
        }
      }
      return add_a_new_definition(n, (double)i, NULL, DEFINITION_TYPE_VALUE, 0);
    }
    
    /* decimal value? */
    if (*c >= '0' && *c <= '9') {
      for (i = 0; *c != 0; c++) {
        if (*c >= '0' && *c <= '9')
          i = (i * 10) + *c - '0';
        else {
          fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Error in value (%s).\n", value);
          return FAILED;
        }
      }
      return add_a_new_definition(n, (double)i, NULL, DEFINITION_TYPE_VALUE, 0);
    }

    /* quoted string? */
    if (*c == '"' && c[strlen(c) - 1] == '"') {
      char *s = calloc(strlen(c) + 1, 1);
      int result, t;

      c++;
      for (t = 0; *c != 0; c++, t++) {
        if (*c == '\\' && *(c + 1) == '"') {
          c++;
          s[t] = '"';
        }
        else if (*c == '"') {
          c++;
          break;
        }
        else
          s[t] = *c;
      }
      s[t] = 0;
      
      if (*c == 0)
        result = add_a_new_definition(n, 0.0, s, DEFINITION_TYPE_STRING, (int)strlen(s));
      else {
        fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Incorrectly terminated quoted string (%s).\n", value);
        result = FAILED;
      }
      
      free(s);
      return result;
    }

    /* unquoted string */
    return add_a_new_definition(n, 0.0, c, DEFINITION_TYPE_STRING, (int)strlen(c));
  }

  return FAILED;
}


int parse_and_add_incdir(char* c, int contains_flag) {

  int old_count = g_ext_incdirs.count, index, buffer_size, j;
  char **new_array;
  char n[MAX_NAME_LENGTH + 1];

  /* increment for the new entry, then re-allocate the array */
  g_ext_incdirs.count++;
  new_array = calloc(g_ext_incdirs.count * sizeof(char*), 1);
  for (index = 0; index < old_count; index++)
    new_array[index] = g_ext_incdirs.names[index];
 
  free(g_ext_incdirs.names);
  g_ext_incdirs.names = new_array;
  buffer_size = g_ext_incdirs.max_name_size_bytes;
  g_ext_incdirs.names[old_count] = calloc(buffer_size, 1);

  /* skip the flag? */
  if (contains_flag == YES)
    c += 2;

  for (j = 0; j < MAX_NAME_LENGTH && *c != 0; j++, c++)
    n[j] = *c;
  n[j] = 0;

  if (*c != 0)
    return FAILED;

  localize_path(n);

#if defined(MSDOS)
  snprintf(g_ext_incdirs.names[old_count], buffer_size, "%s\\", n);
#else
  snprintf(g_ext_incdirs.names[old_count], buffer_size, "%s/", n);
#endif

  g_use_incdir = YES;

  return SUCCEEDED;
}

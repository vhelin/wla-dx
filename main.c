
/*
  wla - part of wla dx gb-z80/z80/6502/65c02/6510/65816/huc6280/spc-700
  macro assembler package by ville helin <vhelin@iki.fi>.
  this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef UNIX
#include <unistd.h>
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


FILE *file_out_ptr = NULL;

/* amiga specific definitions */

#ifdef AMIGA
long __stack = 65536;
#endif

#ifdef AMIGA
#ifdef GB
char version_string[] = "$VER: WLA-GB 9.7 (05.02.2016)";
#endif
#ifdef Z80
char version_string[] = "$VER: WLA-Z80 9.7 (05.02.2016)";
#endif
#ifdef MCS6502
char version_string[] = "$VER: WLA-6502 9.7 (05.02.2016)";
#endif
#ifdef WDC65C02
char version_string[] = "$VER: WLA-65C02 9.7 (05.02.2016)";
#endif
#ifdef W65816
char version_string[] = "$VER: WLA-65816 9.7 (05.02.2016)";
#endif
#ifdef MCS6510
char version_string[] = "$VER: WLA-6510 9.7 (05.02.2016)";
#endif
#ifdef SPC700
char version_string[] = "$VER: WLA-SPC700 9.7 (05.02.2016)";
#endif
#ifdef HUC6280
char version_string[] = "$VER: WLA-HuC6280 9.7 (05.02.2016)";
#endif
#endif

char wla_version[] = "9.7";

char gba_tmp_name[32], gba_unfolded_name[32];

extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;
extern struct file_name_info *file_name_info_first;
extern struct label_def *label_tmp, *labels;
extern struct map_t *global_unique_label_map;
extern struct macro_static *macros_first;
extern struct definition *tmp_def;
extern struct map_t *defines_map;
extern struct export_def *export_first, *export_last;
extern struct stack *stacks_first, *stacks_tmp, *stacks_last, *stacks_header_first, *stacks_header_last;
extern struct repeat_runtime *repeat_stack;
extern struct section_def *sections_first;
extern struct macro_runtime *macro_stack;
extern struct label_def *unknown_labels;
extern struct filepointer *filepointers;
extern struct map_t *namespace_map;
extern char *unfolded_buffer;
extern char *include_in_tmp, *tmp_a;
extern char *rom_banks, *rom_banks_usage_table;
extern char *include_dir, *buffer, *full_name;
extern int include_in_tmp_size, tmp_a_size, *banks, *bankaddress;

int output_format = OUTPUT_NONE, verbose_mode = OFF, test_mode = OFF;
int extra_definitions = OFF, commandline_parsing = ON, makefile_rules = NO;
int listfile_data = NO, quiet = NO, use_incdir = NO;

char *final_name = NULL, *asm_name = NULL, ext_incdir[MAX_NAME_LENGTH];


int main(int argc, char *argv[]) {

  int i = SUCCEEDED;

  if (sizeof(double) != 8) {
    fprintf(stderr, "MAIN: sizeof(double) == %d != 8. WLA will not work properly.\n", (int)sizeof(double));
    return -1;
  }

  atexit(procedures_at_exit);

  /* init the randon number generator */
  init_genrand(time(NULL));

  /* Init hashmaps */
  defines_map = hashmap_new();
  global_unique_label_map = hashmap_new();
  namespace_map = hashmap_new();

  if (argc >= 3) {
    if (parse_flags(argv[1]) == SUCCEEDED) {
      if (parse_defines_and_get_final_name(argv + 2, argc - 2) == FAILED)
	return 1;
    }
    else
      i = FAILED;
  }
  else
    i = FAILED;

  if (i == FAILED || output_format == OUTPUT_NONE) {
#ifdef GB
    printf("\nWLA GB-Z80 Macro Assembler v9.7b\n");
#endif
#ifdef Z80
    printf("\nWLA Z80 Macro Assembler v9.7b\n");
#endif
#ifdef MCS6502
    printf("\nWLA 6502 Macro Assembler v9.7b\n");
#endif
#ifdef WDC65C02
    printf("\nWLA 65C02 Macro Assembler v9.7b\n");
#endif
#ifdef W65816
    printf("\nWLA 65816 Macro Assembler v9.7b\n");
#endif
#ifdef MCS6510
    printf("\nWLA 6510 Macro Assembler v9.7b\n");
#endif
#ifdef SPC700
    printf("\nWLA SPC-700 Macro Assembler v9.7b\n");
#endif
#ifdef HUC6280
    printf("\nWLA HuC6280 Macro Assembler v9.7b\n");
#endif
    printf("Written by Ville Helin in 1998-2008 - In GitHub since 2014: https://github.com/vhelin/wla-dx\n");
    printf("USAGE: %s -[iMqtvx]{lo} -I[INCDIR] [DEFINITIONS] <ASM FILE> [OUTPUT FILE]\n", argv[0]);
    printf("Commands:             Options:\n");
    printf("l  Library file       i  Add list file information\n");
    printf("o  Object file        M  Output makefile rules\n");
    printf("                      q  Quiet\n");
    printf("                      t  Test compile\n");
    printf("                      v  Verbose messages\n");
    printf("                      x  Extra compile time definitions\n");
    printf("                      I  Include directory\n\n");

    return 0;
  }

  if (strcmp(asm_name, final_name) == 0) {
    fprintf(stderr, "MAIN: Input and output files share the same name!\n");
    return 1;
  }

  generate_tmp_names();

  file_out_ptr = fopen(gba_tmp_name, "wb");
  if (file_out_ptr == NULL) {
    fprintf(stderr, "MAIN: Error opening file \"%s\".\n", gba_tmp_name);
    return 1;
  }

  /* small inits */
  if (extra_definitions == ON)
    generate_extra_definitions();

  commandline_parsing = OFF;

  /* start the process */
  if (include_file(asm_name) == FAILED)
    return 1;

  if (pass_1() == FAILED)
    return 1;
  if (pass_2() == FAILED)
    return 1;
  if (pass_3() == FAILED)
    return 1;
  if (listfile_data == YES) {
    if (listfile_collect() == FAILED)
      return 1;
  }
  if (pass_4() == FAILED)
    return 1;

  return 0;
}


int parse_flags(char *flags) {

  int count;

  if (*flags != '-')
    return FAILED;

  count = strlen(flags);
  if (count <= 1)
    return FAILED;

  for (flags++, count--; count > 0; count--, flags++) {
    switch (*flags) {

    case 'o':
      if (output_format != OUTPUT_NONE)
	return FAILED;
      output_format = OUTPUT_OBJECT;
      continue;

    case 'l':
      if (output_format != OUTPUT_NONE)
	return FAILED;
      output_format = OUTPUT_LIBRARY;
      continue;

    case 'i':
      listfile_data = YES;
      continue;

    case 'v':
      verbose_mode = ON;
      continue;

    case 't':
      test_mode = ON;
      continue;

    case 'M':
      makefile_rules = YES;
      test_mode = ON;
      verbose_mode = OFF;
      quiet = YES;
      continue;

    case 'q':
      quiet = YES;
      continue;

    case 'x':
      extra_definitions = ON;
      continue;

    default:
      return FAILED;
    }
  }

  return SUCCEEDED;
}

void procedures_at_exit(void) {

  struct file_name_info *f, *ft;
  struct export_def *export_tmp;
  struct section_def *s1, *s2;
  struct label_def *l1, *l2;
  struct macro_static *m;
  struct filepointer *f1, *f2;
  int i;

  /* free all the dynamically allocated data structures and close open files */
  if (file_out_ptr != NULL)
    fclose(file_out_ptr);

  if (macro_stack != NULL)
    free(macro_stack);

  if (repeat_stack != NULL)
    free(repeat_stack);

  if (final_name != NULL)
    free(final_name);

  if (asm_name != NULL)
    free(asm_name);

  if (include_dir != NULL)
    free(include_dir);

  if (full_name != NULL)
    free(full_name);

  if (defines_map != NULL) {
      hashmap_free_all_elements(defines_map);
      hashmap_free(defines_map);
  }

  if (global_unique_label_map != NULL) {
      /* don't free_all_elements, since labels contains _all_ labels. */
      hashmap_free(global_unique_label_map);
  }

  if (namespace_map != NULL) {
      hashmap_free_all_elements(namespace_map);
      hashmap_free(namespace_map);
  }

  m = macros_first;
  while (m != NULL) {
    /* free the argument labels */
    if (m->nargument_names > 0) {
      for (i = 0; i < m->nargument_names; i++)
	free(m->argument_names[i]);
      free(m->argument_names);
    }
    macros_first = m->next;
    free(m);
    m = macros_first;
  }

  label_tmp = labels;
  while (label_tmp != NULL) {
    labels = label_tmp->next;
    free(label_tmp);
    label_tmp = labels;
  }

  l1 = unknown_labels;
  while (l1 != NULL) {
    l2 = l1->next;
    free(l1);
    l1 = l2;
  }

  export_tmp = export_first;
  while (export_tmp != NULL) {
    export_last = export_tmp->next;
    free(export_tmp);
    export_tmp = export_last;
  }

  ifd_tmp = incbin_file_data_first;
  while(ifd_tmp != NULL) {
    incbin_file_data_first = ifd_tmp->next;
    if (ifd_tmp->data != NULL)
      free(ifd_tmp->data);
    if (ifd_tmp->name != NULL)
      free(ifd_tmp->name);
    free(ifd_tmp);
    ifd_tmp = incbin_file_data_first;
  }

  stacks_tmp = stacks_first;
  while (stacks_tmp != NULL) {
    free(stacks_tmp->stack);
    stacks_first = stacks_tmp->next;
    free(stacks_tmp);
    stacks_tmp = stacks_first;
  }

  stacks_tmp = stacks_header_first;
  while (stacks_tmp != NULL) {
    free(stacks_tmp->stack);
    stacks_first = stacks_tmp->next;
    free(stacks_tmp);
    stacks_tmp = stacks_first;
  }

  if (unfolded_buffer != NULL)
    free(unfolded_buffer);

  if (buffer != NULL)
    free(buffer);

  if (include_in_tmp != NULL)
    free(include_in_tmp);
  if (tmp_a != NULL)
    free(tmp_a);

  if (rom_banks != NULL)
    free(rom_banks);
  if (rom_banks_usage_table != NULL)
    free(rom_banks_usage_table);
  if (banks != NULL)
    free(banks);
  if (bankaddress != NULL)
    free(bankaddress);

  f = file_name_info_first;
  while (f != NULL) {
    if (f->name != NULL)
      free(f->name);
    ft = f->next;
    free(f);
    f = ft;
  }

  s1 = sections_first;
  while (s1 != NULL) {
    if (s1->data != NULL)
      free(s1->data);
    if (s1->listfile_cmds != NULL)
      free(s1->listfile_cmds);
    if (s1->listfile_ints != NULL)
      free(s1->listfile_ints);
    hashmap_free(s1->label_map);
    s2 = s1->next;
    free(s1);
    s1 = s2;
  }

  f1 = filepointers;
  while (f1 != NULL) {
    f2 = f1->next;
    if (f1->f != NULL)
      fclose(f1->f);
    if (f1->filename != NULL)
      free(f1->filename);
    free(f1);
    f1 = f2;
  }

  /* remove the tmp files */
  remove(gba_tmp_name);
  remove(gba_unfolded_name);
}


int generate_tmp_names(void) {

#ifdef UNIX
  sprintf(gba_tmp_name, ".wla%da", (int)getpid());
  sprintf(gba_unfolded_name, ".wla%db", (int)getpid());
#endif

#ifdef AMIGA
  sprintf(gba_tmp_name, "wla_a.tmp");
  sprintf(gba_unfolded_name, "wla_b.tmp");
#endif

#ifdef MSDOS
#if 1 /*ndef WIN32*/
  sprintf(gba_tmp_name, "wla_a.tmp");
  sprintf(gba_unfolded_name, "wla_b.tmp");
#else
  sprintf(gba_tmp_name, ".wla%lda", GetCurrentProcessId());
  sprintf(gba_unfolded_name, ".wla%ldb", GetCurrentProcessId());
#endif  
#endif

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
  length = strlen(tmp);
  for (i = 0; i < length; i++) {
    if (tmp[i] == 0x0A) {
      tmp[i] = 0;
      break;
    }
  }

  if (add_a_new_definition("WLA_TIME", 0.0, tmp, DEFINITION_TYPE_STRING, strlen(tmp)) == FAILED)
    return FAILED;
  if (add_a_new_definition("wla_time", 0.0, tmp, DEFINITION_TYPE_STRING, strlen(tmp)) == FAILED)
    return FAILED;
  if (add_a_new_definition("WLA_VERSION", 0.0, wla_version, DEFINITION_TYPE_STRING, strlen(wla_version)) == FAILED)
    return FAILED;
  if (add_a_new_definition("wla_version", 0.0, wla_version, DEFINITION_TYPE_STRING, strlen(wla_version)) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int parse_defines_and_get_final_name(char **c, int n) {

  int x;

  while (1) {
    if (n == 0)
      break;
    if (strlen(*c) > 2) {
      if (**c != '-' || *((*c) + 1) != 'I')
	break;
      else
	if (parse_and_set_incdir(*c) == FAILED)
	  return FAILED;
    }
    c++;
    n--;
    break;
  }

  while (1) {
    if (n == 0)
      break;
    if (strlen(*c) > 2) {
      if (**c != '-' || *((*c) + 1) != 'D')
	break;
      else
	if (parse_and_add_definition(*c) == FAILED)
	  return FAILED;
    }
    if (strlen(*c) <= 2)
      break;
    c++;
    n--;
  }

  /* allocate room for names */
  if (n == 1 || n == 2) {
    asm_name = malloc(strlen(*c)+1);
    if (n == 2)
      final_name = malloc(strlen(*(c+1))+1);
    else
      final_name = malloc(strlen(*c)+1+4);

    if (asm_name == NULL || final_name == NULL) {
      if (asm_name != NULL) {
	free(asm_name);
	asm_name = NULL;
      }
      if (final_name != NULL) {
	free(final_name);
	final_name = NULL;
      }
      fprintf(stderr, "PARSE_DEFINES_AND_GET_FINAL_NAME: Out of memory error.\n");
      return FAILED;
    }
  }

  /* both infile and outfile were given */
  if (n == 2) {
    strcpy(asm_name, *c);
    c++;
    strcpy(final_name, *c);
    return SUCCEEDED;
  }
  /* only the infile was given -> construct the outfile name */
  else if (n == 1) {
    strcpy(asm_name, *c);
    for (x = 0; x < (int)strlen(*c) && *((*c) + x) != '.'; x++)
      final_name[x] = *((*c) + x);
    final_name[x++] = '.';
    if (output_format == OUTPUT_OBJECT) {
      final_name[x++] = 'o';
      final_name[x] = 0;
    }
    else if (output_format == OUTPUT_LIBRARY) {
      final_name[x++] = 'l';
      final_name[x++] = 'i';
      final_name[x++] = 'b';
      final_name[x] = 0;
    }
    return SUCCEEDED;
  }

  fprintf(stderr, "PARSE_DEFINES_AND_GET_FINAL_NAME: Error in commandline options.\n");
  return FAILED;
}


int parse_and_add_definition(char *c) {

  char n[MAX_NAME_LENGTH];
  int i;

  c += 2;
  for (i = 0; i < (MAX_NAME_LENGTH - 1) && *c != 0 && *c != '='; i++, c++)
    n[i] = *c;
  n[i] = 0;

  if (*c == 0)
    return add_a_new_definition(n, 0.0, NULL, DEFINITION_TYPE_VALUE, 0);
  else if (*c == '=') {
    c++;
    if (*c == 0)
      return FAILED;

    /* hexadecimal value? */
    if (*c == '$' || ((c[strlen(c)-1] == 'h' || c[strlen(c)-1] == 'H') && (*c >= '0' && *c <= '9'))) {
      if (*c == '$')
	c++;
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
	  fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Error in value.\n");
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
	  fprintf(stderr, "PARSE_AND_ADD_DEFINITION: Error in value.\n");
	  return FAILED;
	}
      }
      return add_a_new_definition(n, (double)i, NULL, DEFINITION_TYPE_VALUE, 0);
    }

    /* string definition */
    return add_a_new_definition(n, 0.0, c, DEFINITION_TYPE_STRING, strlen(c));
  }

  return FAILED;
}

int parse_and_set_incdir(char *c) {

  char n[MAX_NAME_LENGTH];
  int i;

  c += 2;
  for (i = 0; i < (MAX_NAME_LENGTH - 1) && *c != 0; i++, c++)
    n[i] = *c;
  n[i] = 0;

  if (*c == 0) {
    localize_path(n);
#if defined(MSDOS)
    sprintf(ext_incdir, "%s\\", n);
#else
    sprintf(ext_incdir, "%s/", n);
#endif
	use_incdir = YES;
    return SUCCEEDED;
  }

  return FAILED;
}

/*
 *
 *
 * MERSENNE TWISTER
 *
 *
 */

/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s) {
  
  mt[0]= s & 0xffffffffUL;
  for (mti=1; mti < N; mti++) {
    mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void) {
  
  unsigned long y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N) { /* generate N words at one time */
    int kk;

    if (mti == N+1)   /* if init_genrand() has not been called, */
      init_genrand(5489UL); /* a default initial seed is used */

    for (kk = 0; kk < N-M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
      mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < N-1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
      mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mt[N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mti = 0;
  }
  
  y = mt[mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

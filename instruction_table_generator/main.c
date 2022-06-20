
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(AMIGA)
#include "/defines.h"
#else
#include "../defines.h"
#endif

#include "defines.h"

extern struct instruction g_instructions_table[];

/* this program is used to generate the instruction decoding speedup tables */

int print_table(FILE *f, int *table) {

  int i;

  for (i = 0; i < 256; i++) {
    if ((i % 8) == 0)
      fprintf(f, "  ");
    fprintf(f, "%d", table[i]);
    if ((i % 8) == 7) {
      if (i == 255)
        fprintf(f, " };\n");
      else
        fprintf(f, ",\n");
    }
    else
      fprintf(f, ", ");
  }

  return SUCCEEDED;
}


int main(int argc, char *argv[]) {

  struct instruction *opt_tmp;
  FILE *out;
  char max_name[256];
  unsigned int max = 0;
  int i, upper_char, lower_char, last_lower_char;

  /* table containing the the number of entries we have in g_instructions_table starting with each ASCII characher */
  int counts[256], count;

  /* table containing the index in g_instructions_table where the entries with each starting character start */
  int indexes[256], index;

  char *outname = NULL;

  if (argc > 2 && argv != NULL)
    fprintf(stderr, "WARNING: \"%s\" got too many arguments!\n", argv[0]);
  if (argc > 1)
    outname = argv[1];

  /* generate instruction decoding jump tables */
  for (i = 0; i < 256; i++) {
    counts[i] = 0;
    indexes[i] = 0;
  }

  opt_tmp = g_instructions_table;
  upper_char = 'A';
  lower_char = 'a';
  last_lower_char = 'a';
  index = 0;
  count = 0;
  indexes[(int)upper_char] = 0;
  indexes[(int)lower_char] = 0;

  while (g_instructions_table[index].type != 0xff) {
    if (strlen(g_instructions_table[index].string) > max) {
      max = (unsigned int)strlen(g_instructions_table[index].string);
      strcpy(max_name, g_instructions_table[index].string);
    }

    if (opt_tmp[index].string[0] != upper_char) {
      counts[(int)upper_char] = count;
      counts[(int)lower_char] = count;
      upper_char = opt_tmp[index].string[0];
      lower_char = tolower((int)upper_char);

      if (last_lower_char > lower_char) {
        fprintf(stderr, "MAIN: Instructions are NOT in alphabetical order (first letter): '%c' -> '%c'.\n", last_lower_char, lower_char);
        return 1;
      }

      last_lower_char = lower_char;
      indexes[(int)upper_char] = index;
      indexes[(int)lower_char] = index;
      count = 1;
      index++;
    }
    else {
      count++;
      index++;
    }
  }

  counts[(int)upper_char] = count;
  counts[(int)lower_char] = count;

  out = stdout;
  if (outname) {
    out = fopen(outname, "wb");
    if (out == NULL) {
      fprintf(stderr, "MAIN: Unable to open file \"%s\": %s\n", outname, strerror(errno));
      return 1;
    }
  }

  fprintf(out, "int g_instruction_n[256] = {\n");
  print_table(out, counts);

  fprintf(out, "int g_instruction_p[256] = {\n");
  print_table(out, indexes);

  if (outname)
    fclose(out);

#if !defined(WLA_QUIET)
  printf("MAIN: Max instruction.string (\"%s\") length was %u bytes.\n", max_name, max);
#endif

  return 0;
}

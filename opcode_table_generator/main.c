
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

extern struct optcode g_opcodes_table[];

/* this program is used to generate the opcode decoding speedup tables */

int print_table(FILE *f, int *d) {

  int x;


  for (x = 0; x < 256; x++) {
    if ((x % 8) == 0)
      fprintf(f, "  ");
    fprintf(f, "%d", d[x]);
    if ((x % 8) == 7) {
      if (x == 255)
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

  struct optcode *opt_tmp;
  FILE *out;
  char max_name[256];
  unsigned int max = 0;
  int x, opcode_n[256], opcode_p[256], upper_char, lower_char, n, last_lower_char;


  char *outname = NULL;
  if (argc > 2 && argv != NULL)
    fprintf(stderr, "WARNING: \"%s\" got too many arguments!\n", argv[0]);
  if (argc > 1)
    outname = argv[1];

  /* generate opcode decoding jump tables */
  for (x = 0; x < 256; x++) {
    opcode_n[x] = 0;
    opcode_p[x] = 0;
  }

  opt_tmp = g_opcodes_table;
  upper_char = 'A';
  lower_char = 'a';
  last_lower_char = 'a';
  n = 0;
  x = 0;
  opcode_p[(int)upper_char] = 0;
  opcode_p[(int)lower_char] = 0;
  while (g_opcodes_table[n].type != 0xff) {
    if (strlen(g_opcodes_table[n].op) > max) {
      max = (unsigned int)strlen(g_opcodes_table[n].op);
      strcpy(max_name, g_opcodes_table[n].op);
    }
    if (opt_tmp[n].op[0] != upper_char) {
      opcode_n[(int)upper_char] = x;
      opcode_n[(int)lower_char] = x;
      upper_char = opt_tmp[n].op[0];
      lower_char = tolower((int)upper_char);
      if (last_lower_char > lower_char) {
        fprintf(stderr, "MAIN: Instruction are NOT in alphabetical order (first letter): '%c' -> '%c'.\n", last_lower_char, lower_char);
        return 1;
      }
      last_lower_char = lower_char;
      opcode_p[(int)upper_char] = n;
      opcode_p[(int)lower_char] = n;
      x = 1;
      n++;
    }
    else {
      x++;
      n++;
    }
  }
  opcode_n[(int)upper_char] = x;
  opcode_n[(int)lower_char] = x;

  out = stdout;
  if (outname) {
    out = fopen(outname, "wb");
    if (out == NULL) {
      fprintf(stderr, "MAIN: Unable to open file \"%s\": %s\n", outname, strerror(errno));
      return 1;
    }
  }

  /* opcode_n[256] */
  fprintf(out, "int g_opcode_n[256] = {\n");
  print_table(out, opcode_n);

  /* opcode_p[256] */
  fprintf(out, "int g_opcode_p[256] = {\n");
  print_table(out, opcode_p);

  if (outname)
    fclose(out);

#if !defined(WLA_QUIET)
  printf("MAIN: max opt[] (\"%s\") length was %u bytes.\n", max_name, max);
#endif

  return 0;
}

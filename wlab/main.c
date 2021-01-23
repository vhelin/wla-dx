
/*
  wlab - part of wla dx gb-z80/z80/6502/6510/65816 macro assembler package by
  ville helin <vhelin@cc.hut.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "defines.h"



#ifdef AMIGA
char version_string[] = "$VER: WLAB 1.2 (31.03.2003)";
#endif


int header = ON, mode = MOD_NONE, address = OFF, a, as, ae;



int main(int argc, char *argv[]) {

  FILE *fp = NULL;
  unsigned char *in;
  char *name;
  int local_i, fs, t;


  local_i = SUCCEEDED;

  if (!(argc == 2 || argc == 3))
    local_i = FAILED;

  if (argc == 3)
    local_i = parse_flags(argv[1]);

  if (mode == MOD_NONE)
    local_i = FAILED;

  if (local_i == FAILED) {
    fprintf(stderr, "\nWLAB Binary to WLA DB Converter v1.2\n");
    fprintf(stderr, "Written by Ville Helin 2000-2003\n");
    fprintf(stderr, "USAGE: %s -[ap]{bdh} <BIN FILE>\n", argv[0]);
    fprintf(stderr, "Commands:             Options:\n");
    fprintf(stderr, "b  Output binary      a  Print address\n");
    fprintf(stderr, "d  Output decimal     p  Skip header\n");
    fprintf(stderr, "h  Output hex\n\n");
    return 1;
  }

  name = argv[argc - 1];

  fp = fopen(name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "MAIN: Could not open file \"%s\".\n", name);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  fs = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  in = calloc(sizeof(char) * fs, 1);
  if (in == NULL) {
    fprintf(stderr, "MAIN: Out of memory. Could not allocate %d bytes.\n", fs);
    fclose(fp);
    return 1;
  }

  fread(in, 1, fs, fp);
  fclose(fp);

  if (fs < 0x10)
    as = 1;
  else if (fs < 0x100)
    as = 2;
  else if (fs < 0x1000)
    as = 3;
  else
    as = 4;

  /* output header */

  if (header == ON) {
    fprintf(stdout, "; this file was created with wlab\n");
    fprintf(stdout, "; by ville helin <vhelin@cc.hut.fi>.\n");
    fprintf(stdout, "; listing of file \"%s\".\n", name);
  }

  a = 0;

  /* print data */

  if (mode == MOD_HEX) {

    /* output hexadecimal data */

    t = fs >> 3;
    fs = fs - (t << 3);

    for (local_i = 0; t > 0; t--, local_i += 8) {
      fprintf(stdout, ".DB $%.2x, $%.2x, $%.2x, $%.2x, $%.2x, $%.2x, $%.2x, $%.2x", in[local_i], in[local_i + 1], in[local_i + 2], in[local_i + 3], in[local_i + 4], in[local_i + 5], in[local_i + 6], in[local_i + 7]);
      if (address == ON) {
        ae = a + 8-1;
        print_address();
        a += 8;
      }
      else
        fprintf(stdout, "\n");
    }

    if (fs != 0) {
      fprintf(stdout, ".DB");
      for (; fs > 0; fs--) {
        fprintf(stdout, " $%.2x", in[local_i++]);
        if (fs > 1)
          fprintf(stdout, ",");
      }
      if (address == ON) {
        ae = local_i-1;
        print_address();
      }
      else
        fprintf(stdout, "\n");
    }

  }
  else if (mode == MOD_DEC) {

    /* output decimal data */

    t = fs >> 3;
    fs = fs - (t << 3);

    for (local_i = 0; t > 0; t--, local_i += 8) {
      fprintf(stdout, ".DB %d, %d, %d, %d, %d, %d, %d, %d", in[local_i], in[local_i + 1], in[local_i + 2], in[local_i + 3], in[local_i + 4], in[local_i + 5], in[local_i + 6], in[local_i + 7]);
      if (address == ON) {
        ae = a + 8-1;
        print_address();
        a += 8;
      }
      else
        fprintf(stdout, "\n");
    }

    if (fs != 0) {
      fprintf(stdout, ".DB");
      for (; fs > 0; fs--) {
        fprintf(stdout, " %d", in[local_i++]);
        if (fs > 1)
          fprintf(stdout, ",");
      }
      if (address == ON) {
        ae = local_i-1;
        print_address();
      }
      else
        fprintf(stdout, "\n");
    }

  }
  else if (mode == MOD_BIN) {

    int b, c, d;

    /* output binary data */

    for (local_i = 0; local_i < fs; local_i++) {
      fprintf(stdout, ".DB %%");
      d = 0;
      for (t = 7; t > -1; t--) {
        b = 1 << t;
        c = in[local_i] & b;
        d = c >> t;
        if (d == 0)
          fprintf(stdout, "0");
        else
          fprintf(stdout, "1");
      }
      if (address == ON) {
        ae = a;
        print_address();
        a++;
      }
      else
        fprintf(stdout, "\n");
    }

  }

  free(in);

  return 0;
}


int print_address(void) {

  if (a == ae) {
    switch (as) {
    case 1:
      fprintf(stdout, "\t; $%.1x\n", a);
      break;
    case 2:
      fprintf(stdout, "\t; $%.2x\n", a);
      break;
    case 3:
      fprintf(stdout, "\t; $%.3x\n", a);
      break;
    case 4:
      fprintf(stdout, "\t; $%.4x\n", a);
      break;
    }

    return SUCCEEDED;
  }

  switch (as) {
  case 1:
    fprintf(stdout, "\t; $%.1x-$%.1x\n", a, ae);
    break;
  case 2:
    fprintf(stdout, "\t; $%.2x-$%.2x\n", a, ae);
    break;
  case 3:
    fprintf(stdout, "\t; $%.3x-$%.3x\n", a, ae);
    break;
  case 4:
    fprintf(stdout, "\t; $%.4x-$%.4x\n", a, ae);
    break;
  }

  return SUCCEEDED;
}


int parse_flags(char *f) {

  int l;


  if (*f != '-') {
    return FAILED;
  }

  l = (int)strlen(f);
  if (l == 1)
    return FAILED;

  for (f++, l--; l > 0; l--, f++) {
    switch (*f) {

    case 'p':
      header = OFF;
      continue;

    case 'd':
      if (mode != MOD_NONE)
        return FAILED;
      mode = MOD_DEC;
      continue;

    case 'h':
      if (mode != MOD_NONE)
        return FAILED;
      mode = MOD_HEX;
      continue;

    case 'b':
      if (mode != MOD_NONE)
        return FAILED;
      mode = MOD_BIN;
      continue;

    case 'a':
      address = ON;
      continue;

    default:
      return FAILED;
    }
  }

  return SUCCEEDED;
}

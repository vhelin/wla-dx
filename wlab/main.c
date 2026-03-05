
/*
  wlab - part of wla dx macro assembler package by
  ville helin <ville.helin@iki.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "defines.h"


#if defined(AMIGA)
char version_string[] = "$VER: WLAB 1.3 (6.3.2026)";
#endif


int g_header = ON, g_mode = MODE_NONE, g_print_address = OFF, g_address_size, g_bytes_per_line = 8;


int main(int argc, char *argv[]) {

  FILE *fp = NULL;
  unsigned char *in;
  char *name;
  int i, j, file_size, t, address = 0, result;

  result = SUCCEEDED;

  if (argc < 3)
    result = FAILED;

  for (i = 1; i < argc - 1 && result == SUCCEEDED; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      i++;
      if (i >= argc - 1) {
        result = FAILED;
        break;
      }
      {
        int n = atoi(argv[i]);
        if (n < 1) {
          result = FAILED;
          break;
        }
        g_bytes_per_line = n;
      }
    }
    else {
      result = parse_flags(argv[i]);
    }
  }

  if (g_mode == MODE_NONE)
    result = FAILED;

  if (result == FAILED) {
    fprintf(stderr, "\nWLAB Binary to WLA DB Converter v1.3\n");
    fprintf(stderr, "Written by Ville Helin 2000-2026\n");
    fprintf(stderr, "USAGE: %s [-a] [-p] [-n <NUM>] {-b|-d|-h} <BIN FILE>\n", argv[0]);
    fprintf(stderr, "Commands:             Options:\n");
    fprintf(stderr, "b  Output binary      a  Print address\n");
    fprintf(stderr, "d  Output decimal     p  Skip header\n");
    fprintf(stderr, "h  Output hex         n  Bytes per line (default: 8)\n\n");
    return 1;
  }

  name = argv[argc - 1];

  fp = fopen(name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "MAIN: Could not open file \"%s\".\n", name);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  in = calloc(sizeof(char) * file_size, 1);
  if (in == NULL) {
    fprintf(stderr, "MAIN: Out of memory. Could not allocate %d bytes.\n", file_size);
    fclose(fp);
    return 1;
  }

  if (fread(in, 1, file_size, fp) != (size_t) file_size) {
    fprintf(stderr, "Could not read all %d bytes of \"%s\"!", file_size, name);
    free(in);
    fclose(fp);
    return FAILED;
  }

  fclose(fp);

  if (file_size < 0x10)
    g_address_size = 1;
  else if (file_size < 0x100)
    g_address_size = 2;
  else if (file_size < 0x1000)
    g_address_size = 3;
  else if (file_size < 0x10000)
    g_address_size = 4;
  else if (file_size < 0x100000)
    g_address_size = 5;
  else if (file_size < 0x1000000)
    g_address_size = 6;
  else if (file_size < 0x10000000)
    g_address_size = 7;
  else
    g_address_size = 8;

  /* output header */

  if (g_header == ON) {
    fprintf(stdout, "; this file was created with wlab\n");
    fprintf(stdout, "; by ville helin <ville.helin@iki.fi>.\n");
    fprintf(stdout, "; listing of file \"%s\".\n", name);
  }

  address = 0;

  /* print data */

  if (g_mode == MODE_HEX) {

    /* output hexadecimal data */

    t = file_size / g_bytes_per_line;
    file_size = file_size - t * g_bytes_per_line;

    for (i = 0; t > 0; t--, i += g_bytes_per_line) {
      fprintf(stdout, ".DB");
      for (j = 0; j < g_bytes_per_line; j++) {
        if (j > 0)
          fprintf(stdout, ",");
        fprintf(stdout, " $%.2x", in[i + j]);
      }
      if (g_print_address == ON) {
        print_address(address, address + g_bytes_per_line-1);
        address += g_bytes_per_line;
      }
      else
        fprintf(stdout, "\n");
    }

    if (file_size != 0) {
      fprintf(stdout, ".DB");
      for (; file_size > 0; file_size--) {
        fprintf(stdout, " $%.2x", in[i++]);
        if (file_size > 1)
          fprintf(stdout, ",");
      }
      if (g_print_address == ON)
        print_address(address, i-1);
      else
        fprintf(stdout, "\n");
    }
  }
  else if (g_mode == MODE_DEC) {

    /* output decimal data */

    t = file_size / g_bytes_per_line;
    file_size = file_size - t * g_bytes_per_line;

    for (i = 0; t > 0; t--, i += g_bytes_per_line) {
      fprintf(stdout, ".DB");
      for (j = 0; j < g_bytes_per_line; j++) {
        if (j > 0)
          fprintf(stdout, ",");
        fprintf(stdout, " %d", in[i + j]);
      }
      if (g_print_address == ON) {
        print_address(address, address + g_bytes_per_line-1);
        address += g_bytes_per_line;
      }
      else
        fprintf(stdout, "\n");
    }

    if (file_size != 0) {
      fprintf(stdout, ".DB");
      for (; file_size > 0; file_size--) {
        fprintf(stdout, " %d", in[i++]);
        if (file_size > 1)
          fprintf(stdout, ",");
      }
      if (g_print_address == ON)
        print_address(address, i-1);
      else
        fprintf(stdout, "\n");
    }
  }
  else if (g_mode == MODE_BIN) {
    int b, c, d, k;

    /* output binary data */

    t = file_size / g_bytes_per_line;
    file_size = file_size - t * g_bytes_per_line;

    for (i = 0; t > 0; t--, i += g_bytes_per_line) {
      fprintf(stdout, ".DB");
      for (j = 0; j < g_bytes_per_line; j++) {
        if (j > 0)
          fprintf(stdout, ",");
        fprintf(stdout, " %%");
        for (k = 7; k > -1; k--) {
          b = 1 << k;
          c = in[i + j] & b;
          d = c >> k;
          if (d == 0)
            fprintf(stdout, "0");
          else
            fprintf(stdout, "1");
        }
      }
      if (g_print_address == ON) {
        print_address(address, address + g_bytes_per_line-1);
        address += g_bytes_per_line;
      }
      else
        fprintf(stdout, "\n");
    }

    if (file_size != 0) {
      fprintf(stdout, ".DB");
      for (; file_size > 0; file_size--) {
        fprintf(stdout, " %%");
        for (k = 7; k > -1; k--) {
          b = 1 << k;
          c = in[i] & b;
          d = c >> k;
          if (d == 0)
            fprintf(stdout, "0");
          else
            fprintf(stdout, "1");
        }
        i++;
        if (file_size > 1)
          fprintf(stdout, ",");
      }
      if (g_print_address == ON)
        print_address(address, i-1);
      else
        fprintf(stdout, "\n");
    }
  }

  free(in);

  return 0;
}


int print_address(int address, int address_end) {

  if (address == address_end) {
    switch (g_address_size) {
    case 1:
      fprintf(stdout, "    ; $%.1x\n", address);
      break;
    case 2:
      fprintf(stdout, "    ; $%.2x\n", address);
      break;
    case 3:
      fprintf(stdout, "    ; $%.3x\n", address);
      break;
    case 4:
      fprintf(stdout, "    ; $%.4x\n", address);
      break;
    case 5:
      fprintf(stdout, "    ; $%.5x\n", address);
      break;
    case 6:
      fprintf(stdout, "    ; $%.6x\n", address);
      break;
    case 7:
      fprintf(stdout, "    ; $%.7x\n", address);
      break;
    case 8:
      fprintf(stdout, "    ; $%.8x\n", address);
      break;
    }

    return SUCCEEDED;
  }

  switch (g_address_size) {
  case 1:
    fprintf(stdout, "    ; $%.1x-$%.1x\n", address, address_end);
    break;
  case 2:
    fprintf(stdout, "    ; $%.2x-$%.2x\n", address, address_end);
    break;
  case 3:
    fprintf(stdout, "    ; $%.3x-$%.3x\n", address, address_end);
    break;
  case 4:
    fprintf(stdout, "    ; $%.4x-$%.4x\n", address, address_end);
    break;
  case 5:
    fprintf(stdout, "    ; $%.5x-$%.5x\n", address, address_end);
    break;
  case 6:
    fprintf(stdout, "    ; $%.6x-$%.6x\n", address, address_end);
    break;
  case 7:
    fprintf(stdout, "    ; $%.7x-$%.7x\n", address, address_end);
    break;
  case 8:
    fprintf(stdout, "    ; $%.8x-$%.8x\n", address, address_end);
    break;
  }

  return SUCCEEDED;
}


int parse_flags(char *f) {

  int l;

  if (*f != '-')
    return FAILED;

  l = (int)strlen(f);
  if (l == 1)
    return FAILED;

  for (f++, l--; l > 0; l--, f++) {
    switch (*f) {
    case 'p':
      g_header = OFF;
      continue;

    case 'd':
      if (g_mode != MODE_NONE)
        return FAILED;
      g_mode = MODE_DEC;
      continue;

    case 'h':
      if (g_mode != MODE_NONE)
        return FAILED;
      g_mode = MODE_HEX;
      continue;

    case 'b':
      if (g_mode != MODE_NONE)
        return FAILED;
      g_mode = MODE_BIN;
      continue;

    case 'a':
      g_print_address = ON;
      continue;

    default:
      return FAILED;
    }
  }

  return SUCCEEDED;
}

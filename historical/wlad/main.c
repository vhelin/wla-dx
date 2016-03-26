
/*
  wlad - part of wla dx gb-z80/z80/6502/6510/65816 macro assembler package by
  ville helin <vhelin@cc.hut.fi>. this is gpl software.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "defines.h"
#include "opcodes_gb.c"



#ifdef AMIGA
char version_string[] = "$VER: WLAD 1.3 (21.10.2000)";
#endif

int databanks = OFF, strings = OFF, address = ON, code_hex = ON, tab_width = 8;
int bank_first_size, bank_rest_size, slot;
int bank_start = -1, bank_end = -1;


int main(int argc, char *argv[]) {

  FILE *fp = NULL;
  unsigned char *in;
  int i, fs, a, b;
  char *name;


  i = SUCCEEDED;

  if (argc < 2)
    i = FAILED;
  else
    i = parse_flags(argc, argv);

  if (databanks == ON && strings == ON)
    i = FAILED;

  if (i == FAILED) {
    fprintf(stderr, "\nWLAD GB-Z80 Disassembler v1.3\n");
    fprintf(stderr, "Written by Ville Helin in 2000\n");
    fprintf(stderr, "Improved by Matthew Stewart in 2013\n");
    fprintf(stderr, "USAGE: %s [-4abdhs] <BIN FILE>\n", argv[0]);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "4  Optimize address output for tab width of 4\n");
    fprintf(stderr, "a  Disable address output\n");
    fprintf(stderr, "b  Disassemble only specified banks (ex. \"-b 16-31\" or \"-b 0x10-0x1f\")\n");
    fprintf(stderr, "d  Disassemble upper banks (one and up) as data\n");
    fprintf(stderr, "h  Don't print hex data with address output\n");
    fprintf(stderr, "s  Disassemble upper banks as data with string detection\n\n");

    return 1;
  }

  name = argv[argc - 1];

  fp = fopen(name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", name);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  fs = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  in = malloc(sizeof(char) * fs);
  if (in == NULL) {
    fprintf(stderr, "Out of memory. Could not allocate %d bytes.\n", fs);
    fclose(fp);
    return 1;
  }

  fread(in, 1, fs, fp);

  if (in[0x147] == 0xBE) {
    /* pocket voice */
    bank_first_size = 24 * 1024;
    bank_rest_size = 8 * 1024;
  }
  else {
    /* some other */
    bank_first_size = 16 * 1024;
    bank_rest_size = 16 * 1024;
  }

  a = fs / (1024 * 16);
  b = fs - (a * 1024 * 16);
  if (b != 0 || (a != 2 && a != 4 && a != 8 && a != 16 && a != 32 && a != 64 && a != 128 && a != 256 && a != 72 && a != 80 && a != 96)) {
    fprintf(stderr, "Unsupported ROM size.\n");
    return 1;
  }

  if (a == 2)
    b = 0;
  else if (a == 4)
    b = 1;
  else if (a == 8)
    b = 2;
  else if (a == 16)
    b = 3;
  else if (a == 32)
    b = 4;
  else if (a == 64)
    b = 5;
  else if (a == 128)
    b = 6;
  else if (a == 256)
    b = 7;
  else if (a == 72)
    b = 0x52;
  else if (a == 80)
    b = 0x53;
  else if (a == 96)
    b = 0x54;

  /* output header */

  fprintf(stdout, "; this file was created with wlad - yet another gb-z80\n");
  fprintf(stdout, "; disassembler by ville helin <vhelin@cc.hut.fi>.\n");
  fprintf(stdout, "; Disassembly of \"%s\".\n\n", name);
  fprintf(stdout, ".MEMORYMAP\n");
  fprintf(stdout, "SLOTSIZE $4000\n");
  fprintf(stdout, "DEFAULTSLOT 1\n");
  fprintf(stdout, "SLOT 0 $0000\n");
  fprintf(stdout, "SLOT 1 $4000\n");
  fprintf(stdout, ".ENDME\n\n");
  fprintf(stdout, ".BANKSIZE $4000\n");
  fprintf(stdout, ".ROMBANKS %d\n", a);

  if (bank_start == -1 && bank_end == -1) {
    bank_start = 0;
    bank_end = a-1;
  }
  else if (bank_end == -1)
    bank_end = bank_start;

  if (bank_end >= a) {
    fprintf(stderr, "Bank number (%d) is invalid.\n", bank_end);
    return 1;
  }

  b = bank_start;
  if (bank_start == 0) {
    i = 0;
    slot = 0;
  }
  else {
    i = bank_first_size + (bank_start-1)*bank_rest_size;
    slot = 1;
  }

  /* output bank data */

  if (databanks == OFF && strings == OFF) {
    if (b == 0)
      output_bank_opcodes(&b, in, bank_first_size, &i);
    while (b <= bank_end)
      output_bank_opcodes(&b, in, bank_rest_size, &i);
  }
  else if (databanks == ON && strings == OFF) {
    if (b == 0)
      output_bank_opcodes(&b, in, bank_first_size, &i);
    while (b <= bank_end)
      output_bank_data(&b, in, bank_rest_size, &i);
  }
  else if (databanks == OFF && strings == ON) {
    if (b == 0)
      output_bank_opcodes(&b, in, bank_first_size, &i);
    while (b <= bank_end)
      output_bank_data_detect_strings(&b, in, bank_rest_size, &i);
  }

  free(in);
  fclose(fp);

  return 0;
}

int output_bank_opcodes(int *b, unsigned char *in, int bank_size, int *i) {

  char bu[256], tm[256];
  int q, oa, p, t, x, a, ad, tabs, rom_pos, label_pos, num_labels=0;
  struct optcode *ot;

  int labels[BANK_SIZE_MAX];
  char needs_label[BANK_SIZE_MAX];

  memset(labels, -1, sizeof(labels));
  memset(needs_label, 0, sizeof(needs_label));

  a = 0;
  ad = *i + bank_size;

  rom_pos = *i;

  /* Search for where labels are needed */
  while (rom_pos < ad) {
    if (in[rom_pos] == 0xcb)
      ot = &opt_table_ext[in[rom_pos+1]];
    else
      ot = &opt_table[in[rom_pos]];

    if (strlen(ot->op) == 0) {
      rom_pos++;
      a++;
    }
    else if (ot->type == 0) {
      rom_pos++;
      a++;
    }
    else if (ot->type == 1 || ot->type == 3) {
      rom_pos+=2;
      a+=2;
    }
    else if (ot->type == 2) {
      rom_pos+=3;
      a+=3;
    }
    else if (ot->type == 4) {
      label_pos = a + (signed char)in[rom_pos+1] + 2;
      if (label_pos >= 0 && label_pos < bank_size)
        needs_label[label_pos] = 1;
      rom_pos+=2;
      a+=2;
    }
    else if (ot->type == 5) {
      rom_pos+=2;
      a+=2;
    }
  }

  rom_pos = *i;
  a = 0;

  /* Assign labels (when they align with the code) */
  while (rom_pos < ad) {
    if (needs_label[a]) {
      labels[a] = num_labels++;
    }

    if (in[rom_pos] == 0xcb)
      ot = &opt_table_ext[in[rom_pos+1]];
    else
      ot = &opt_table[in[rom_pos]];

    if (strlen(ot->op) == 0) {
      rom_pos++;
      a++;
    }
    else if (ot->type == 0) {
      rom_pos++;
      a++;
    }
    else if (ot->type == 1 || ot->type == 3) {
      rom_pos+=2;
      a+=2;
    }
    else if (ot->type == 2) {
      rom_pos+=3;
      a+=3;
    }
    else if (ot->type == 4) {
      rom_pos+=2;
      a+=2;
    }
    else if (ot->type == 5) {
      rom_pos+=2;
      a+=2;
    }
  }

  a = 0;
  fprintf(stdout, "\n.BANK $%.2x SLOT %d\n.ORG 0\n\n", *b, slot);

  for ( ; *i < ad; ) {
    oa = a;
    if (in[*i] == 0xcb)
      ot = &opt_table_ext[in[*i+1]];
    else
      ot = &opt_table[in[*i]];

    if (labels[a] != -1) {
      fprintf(stdout, "label_%.2x.%.3d:\n", *b, labels[a]);
    }

    if (strlen(ot->op) != 0) {

      /* type 0 */

      if (ot->type == 0) {
        (*i)++;
        a++;
        if (address == ON) {
          fprintf(stdout, "\t%s", ot->op);
          tabs = strlen(ot->op)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x", in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", ot->op);
        x = 0;
      }

      /* type 1: 8-bit parameter */

      else if (ot->type == 1) {
        (*i)++;
        a++;
        for (t = strlen(ot->op), x = 0, p = 0; x < t; ) {
          if (ot->op[x] == '?') {
            bu[p] = 0;
            sprintf(tm, "$%.2x", in[(*i)++]);
            strcat(bu, tm);
            p += 3;
            x++;
            a++;
          }
          else
            bu[p++] = ot->op[x++];
        }

        bu[p] = 0;
        if (address == ON) {
          fprintf(stdout, "\t%s", bu);
          tabs = strlen(bu)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x $%.2x", in[*i-2], in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", bu);
        x = 0;
      }

      /* type 2: 16-bit parameter */

      else if (ot->type == 2) {
        (*i)++;
        a++;
        for (t = strlen(ot->op), x = 0, p = 0; x < t; ) {
          if (ot->op[x] == '?') {
            bu[p] = 0;
            q = in[(*i)++];
            q += in[(*i)++] << 8;
            sprintf(tm, "$%.4x", q);
            strcat(bu, tm);
            p += 5;
            x++;
            a += 2;
          }
          else
            bu[p++] = ot->op[x++];
        }

        bu[p] = 0;
        if (address == ON) {
          fprintf(stdout, "\t%s", bu);
          tabs = strlen(bu)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x $%.2x $%.2x", in[*i-3], in[*i-2], in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", bu);
        x = 0;
      }

      /* type 3: extended (16-bit) opcode */

      else if (ot->type == 3) {
        *i += 2;
        a += 2;
        if (address == ON) {
          fprintf(stdout, "\t%s", ot->op);
          tabs = strlen(ot->op)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x $%.2x", in[*i-2], in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", ot->op);
        x = 0;
      }

      /* type 4: relative branch */

      else if (ot->type == 4) {
        (*i)++;
        a++;
        for (t = strlen(ot->op), x = 0, p = 0; x < t; ) {
          if (ot->op[x] == '?') {
            bu[p] = 0;
            label_pos = oa+(signed char)in[*i]+2;
            if (label_pos >= 0 && label_pos < bank_size && labels[label_pos] != -1) {
              sprintf(tm, "label_%.2x.%.3d", *b, labels[label_pos]);
              (*i)++;
            }
            else {
              if (in[(*i)] < 128)
                sprintf(tm, "$%.2x", in[(*i)++]);
              else {
                if (p > 0 && bu[p-1] == '+') {
                  bu[p-1] = '-';
                  sprintf(tm, "$%.2x", 0x100-in[(*i)++]);
                }
                else
                  sprintf(tm, "-$%.2x", 0x100-in[(*i)++]);
              }
            }

            strcat(bu, tm);
            p += strlen(tm);
            x++;
            a++;
          }
          else
            bu[p++] = ot->op[x++];
        }

        bu[p] = 0;
        if (address == ON) {
          fprintf(stdout, "\t%s", bu);
          tabs = strlen(bu)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x $%.2x", in[*i-2], in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", bu);
        x = 0;
      }

      /* type 5: signed 8-bit parameter */

      else if (ot->type == 5) {
        (*i)++;
        a++;
        for (t = strlen(ot->op), x = 0, p = 0; x < t; ) {
          if (ot->op[x] == '?') {
            bu[p] = 0;
            if (in[(*i)] < 128)
              sprintf(tm, "$%.2x", in[(*i)++]);
            else {
              if (p > 0 && bu[p-1] == '+') {
                bu[p-1] = '-';
                sprintf(tm, "$%.2x", 0x100-in[(*i)++]);
              }
              else
                sprintf(tm, "-$%.2x", 0x100-in[(*i)++]);
            }
            strcat(bu, tm);
            p += strlen(tm);
            x++;
            a++;
          }
          else
            bu[p++] = ot->op[x++];
        }

        bu[p] = 0;
        if (address == ON) {
          fprintf(stdout, "\t%s", bu);
          tabs = strlen(bu)/tab_width+1;
          while (tabs++ < 32/tab_width)
            fprintf(stdout, "\t");
          fprintf(stdout, "; $%.4x", oa);
          if (code_hex)
            fprintf(stdout, ": $%.2x $%.2x", in[*i-2], in[*i-1]);
          fprintf(stdout, "\n");
        }
        else
          fprintf(stdout, "\t%s\n", bu);
        x = 0;
      }
    }
    else {
      if (address == ON) {
        sprintf(bu, ".DB $%.2x", in[(*i)++]);
        fprintf(stdout, "%s", bu);
        tabs=strlen(bu)/tab_width;
        while (tabs++ < 32/tab_width)
          fprintf(stdout, "\t");
        fprintf(stdout, "; $%.4x", oa);
        if (code_hex)
          fprintf(stdout, ": $%.2x", in[*i-1]);
        fprintf(stdout, "\n");
      }
      else
        fprintf(stdout, ".DB $%.2x\n", in[(*i)++]);
      a++;
    }
  }

  (*b)++;
  slot = 1;

  return SUCCEEDED;
}


int output_bank_data(int *b, unsigned char *in, int bank_size, int *i) {

  int ad;


  fprintf(stdout, "\n.BANK $%.2x SLOT %d\n.ORG 0\n\n", *b, slot);

  if (address == ON) {
    for (ad = *i + bank_size; *i < ad; *i += 8) {
      fprintf(stdout, ".DB $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x\t; $%.4x\n", in[*i], in[*i + 1], in[*i + 2], in[*i + 3], in[*i + 4], in[*i + 5], in[*i + 6], in[*i + 7], *i);
    }
  }
  else {
    for (ad = *i + bank_size; *i < ad; *i += 8) {
      fprintf(stdout, ".DB $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x $%.2x\n", in[*i], in[*i + 1], in[*i + 2], in[*i + 3], in[*i + 4], in[*i + 5], in[*i + 6], in[*i + 7]);
    }
  }

  (*b)++;
  slot = 1;

  return SUCCEEDED;
}


int output_bank_data_detect_strings(int *b, unsigned char *in, int bank_size, int *i) {

  int ad, t, x;
  char c;


  fprintf(stdout, "\n.BANK $%.2x SLOT %d\n.ORG 0\n\n", *b, slot);

  for (ad = *i + bank_size; *i < ad; ) {
    if (letter_check(in[*i]) == SUCCEEDED && letter_check(in[*i + 1]) == SUCCEEDED && letter_check(in[*i + 2]) == SUCCEEDED) {
      t = *i;
      fprintf(stdout, ".DB \"");
      c = in[(*i)++];
      while (letter_check(c) == SUCCEEDED) {
        fprintf(stdout, "%c", c);
        c = in[(*i)++];
      }
      (*i)--;

      if (address == ON)
        fprintf(stdout, "\"\t; $%.4x\n", t);
      else
        fprintf(stdout, "\"\n");
    }
    else {
      fprintf(stdout, ".DB");
      t = *i;
      for (x = 0; *i < ad && x < 8 && (x == 0 || letter_check(in[*i]) == FAILED); x++, (*i)++)
        fprintf(stdout, " $%.2x", in[*i]);

      if (address == ON)
        fprintf(stdout, "\t; $%.4x\n", t);
      else
        fprintf(stdout, "\n");
    }
  }

  (*b)++;
  slot = 1;

  return SUCCEEDED;
}


int letter_check(unsigned char c) {

  /* here we assume that "c" is an ASCII char */

  if (c >= 'a' && c <= 'z')
    return SUCCEEDED;
  else if (c >= 'A' && c <= 'Z')
    return SUCCEEDED;
  else if (c >= '0' && c <= '9')
    return SUCCEEDED;
  else if (c == ' ' || c == '!' || c == '?' || c == ',' || c == '.' || c == '-' || c == '*' || c == '|' || c == '&' || c == '/' || c == '\\' || c == '\'' || c == '"' || c == '@' || c == '#' || c == '$' || c == '%' || c == '(' || c == ')' || c == '[' || c == ']')
    return SUCCEEDED;

  /* 163 - pound sign */
  if (c == 163)
    return SUCCEEDED;

  return FAILED;
}


int parse_int(char *s) {

  int i;
  char *e;

  while (*s == ' ')
    s++;

  if (*s == '0' && (*(s+1) == 'x' || *(s+1) == 'X')) {
    s+=2;
    i = strtol(s, &e, 16);
    if (*e != '\0')
      return -1;
  }
  else {
    i = strtol(s, &e, 10);
    if (*e != '\0')
      return -1;
  }

  return i;
}


int parse_flags(int argc, char *argv[]) {

  int l, arg;
  char last_arg = 0;
  char *f, *e;
  char bu[256];

  arg = 0;

  while (++arg < argc-1) {
    f = argv[arg];

    if (*f == '-') {
      l = strlen(f);
      if (l == 1)
        return FAILED;
      last_arg = 0;

      for (f++, l--; l > 0; l--, f++) {
        switch (*f) {

          case '4':
            tab_width = 4;
            continue;

          case 'a':
            address = OFF;
            continue;

          case 'b':
            last_arg = 'b';
            continue;

          case 'd':
            databanks = ON;
            continue;

          case 'h':
            code_hex = OFF;
            continue;

          case 's':
            strings = ON;
            continue;

          default:
            return FAILED;
        }
      }
    }
    else {
      if (last_arg == 'b') {
        e = strchr(f, '-');
        if (e == NULL) {
          bank_start = parse_int(f);
          if (bank_start == -1)
            return FAILED;
        }
        else {
          strncpy(bu, f, e-f);
          bu[e-f] = '\0';
          bank_start = parse_int(bu);
          if (bank_start == -1) {
            return FAILED;
          }
          bank_end = parse_int(e+1);
          if (bank_end == -1)
            return FAILED;
        }
      }
      else
        return FAILED;

      last_arg = 0;
    }
  }

  if (last_arg != 0)
    return FAILED;

  return SUCCEEDED;
}

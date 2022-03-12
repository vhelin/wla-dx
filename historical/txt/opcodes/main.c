
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../defines.h"


#ifdef GB
#include "../../../opcodes_gb.c"
#endif
#ifdef Z80
#include "../../../opcodes_z80.c"
#endif
#ifdef MCS6502
#include "../../../opcodes_6502.c"
#endif
#ifdef WDC65C02
#include "../../../opcodes_65c02.c"
#endif
#ifdef CSG65CE02
#include "../../../opcodes_65ce02.c"
#endif
#ifdef W65816
#include "../../../opcodes_65816.c"
#endif
#ifdef SPC700
#include "../../../opcodes_spc700.c"
#endif
#ifdef SUPERFX
#include "../../../opcodes_superfx.c"
#endif
#ifdef HUC6280
#include "../../../opcodes_huc6280.c"
#endif
#ifdef I8008
#include "../../../opcodes_8008.c"
#endif
#ifdef I8080
#include "../../../opcodes_8080.c"
#endif
#ifdef MC6800
#include "../../../opcodes_6800.c"
#endif
#ifdef MC6801
#include "../../../opcodes_6801.c"
#endif
#ifdef MC6809
#include "../../../opcodes_6809.c"
#endif


/* this program is used to print out the mnemonic tables */


int main(int argc, char *argv[]) {

  int i, l, n;
#if defined(SUPERFX) && defined(NICELIST2)
  int printed_something;
#endif

  i = 0;
  while (1) {
    if (strcmp(g_opcodes_table[i].op, "E") == 0)
      break;

    /* filter *.B, *.W and *.L away */
    l = strlen(g_opcodes_table[i].op);
    for (n = 0; n < l; n++) {
      if (g_opcodes_table[i].op[n] == '.')
        break;
    }

    if (n < l && (g_opcodes_table[i].op[n+1] == 'B' || g_opcodes_table[i].op[n+1] == 'W' || g_opcodes_table[i].op[n+1] == 'L')) {
      i++;
      continue;
    }

#if defined(I8008) || defined(I8080)
    if (g_opcodes_table[i].type == 100) {
      i++;
      continue;
    }
#endif

#ifdef REARRANGE

#if defined(GB) || defined(I8008) || defined(I8080)
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_opcodes_table[i].op, g_opcodes_table[i].hex, g_opcodes_table[i].type, g_opcodes_table[i].value);
#endif
#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_opcodes_table[i].op, g_opcodes_table[i].hex, g_opcodes_table[i].type, g_opcodes_table[i].skip_8bit);
#endif
#ifdef Z80
    printf("  { \"%s\", 0x%X, %d, %d, %d },\n", g_opcodes_table[i].op, g_opcodes_table[i].hex, g_opcodes_table[i].type, g_opcodes_table[i].hex_x, g_opcodes_table[i].value);
#endif
#ifdef W65816
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_opcodes_table[i].op, g_opcodes_table[i].hex, g_opcodes_table[i].type, g_opcodes_table[i].skip_xbit);
#endif
#ifdef SUPERFX
    printf("  { \"%s\", %d, 0x%X, 0x%X, %d, %d },\n", g_opcodes_table[i].op, g_opcodes_table[i].type, g_opcodes_table[i].hex, g_opcodes_table[i].prefix, g_opcodes_table[i].min, g_opcodes_table[i].max);
#endif
    
#endif



#ifdef NICELIST1 /* alphabetical order */

#if defined(GB)
    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_opcodes_table[i].op);

    if ((g_opcodes_table[i].hex & 0xFF) == 0xCB)
      printf("$%.2X%.2X\n", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X\n", g_opcodes_table[i].hex);
#endif

#if defined(I8008) || defined(I8080)
    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_opcodes_table[i].op);

    printf("$%.2X\n", g_opcodes_table[i].hex);
#endif

#ifdef Z80
    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9 || g_opcodes_table[i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_opcodes_table[i].op);

    if (g_opcodes_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 10 || g_opcodes_table[i].type == 5)
      printf(" x $%.2X", g_opcodes_table[i].hex_x);

    printf("\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    printf("\"%s\" ", g_opcodes_table[i].op);

    if (g_opcodes_table[i].hex & 0xFF00)
      printf("$%.2X%.2X\n", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X\n", g_opcodes_table[i].hex);
#endif

#ifdef W65816
    printf("\"%s\" ", g_opcodes_table[i].op);

    if (g_opcodes_table[i].hex & 0xFF00 && g_opcodes_table[i].type != 8)
      printf("$%.2X%.2X\n", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else if (g_opcodes_table[i].type == 8)
      printf("$%.4x\n", g_opcodes_table[i].hex);
    else
      printf("$%.2X\n", g_opcodes_table[i].hex);
#endif

#ifdef SPC700
    printf("\"%s\" ", g_opcodes_table[i].op);
    printf("$%.2X\n", g_opcodes_table[i].hex);
#endif

#ifdef SUPERFX
    printf("\"%s\"", g_opcodes_table[i].op);

    if (g_opcodes_table[i].prefix != 0) {
      if (g_opcodes_table[i].type == 6)
        printf(" $%.2X|*", g_opcodes_table[i].prefix);
      else if (g_opcodes_table[i].type == 0 || g_opcodes_table[i].type == 1 || g_opcodes_table[i].type == 3 || g_opcodes_table[i].type == 4 ||
          g_opcodes_table[i].type == 5 || g_opcodes_table[i].type == 14 || g_opcodes_table[i].type == 15) {
        printf(" $%.2X", g_opcodes_table[i].prefix);
      }
    }

    if (g_opcodes_table[i].type == 0)
      printf(" $%.2X", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 1)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 2)
      printf(" $%.2X", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 3)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 4)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 5)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 6)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 7)
      printf(" $A0|* or $F0|*");
    else if (g_opcodes_table[i].type == 8)
      printf(" $3D $A0|* or $3D $F0|*");
    else if (g_opcodes_table[i].type == 9)
      printf(" $3E $F0|* or $3D $F0|*");
    else if (g_opcodes_table[i].type == 10)
      printf(" $3D $40|* or $10|* $3D $40|*");
    else if (g_opcodes_table[i].type == 11)
      printf(" $3D $30|* or $B0|* $3D $30|*");
    else if (g_opcodes_table[i].type == 12)
      printf(" $40|* or $10|* $40|*");
    else if (g_opcodes_table[i].type == 13)
      printf(" $30|* or $B0|* $30|*");
    else if (g_opcodes_table[i].type == 14)
      printf(" $%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 15)
      printf(" $%.2X|*", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 11 || g_opcodes_table[i].type == 13)
      printf(" [0, 11]");
    if (g_opcodes_table[i].min > 0 || g_opcodes_table[i].max > 0)
      printf(" [%d, %d]", g_opcodes_table[i].min, g_opcodes_table[i].max);
    if (g_opcodes_table[i].type == 10 || g_opcodes_table[i].type == 12)
      printf(" [0, 11]");
    printf("\n");
#endif
    
#endif



#ifdef NICELIST2 /* opcode order */

#if defined(GB)
    if ((g_opcodes_table[i].hex & 0xFF) == 0xCB)
      printf("$%.2X%.2X ", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X ", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", g_opcodes_table[i].op);
#endif

#if defined(I8008) || defined(I8080)
    printf("$%.2X ", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", g_opcodes_table[i].op);
#endif

#ifdef Z80
    if (g_opcodes_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 10 || g_opcodes_table[i].type == 5)
      printf(" x $%.2X", g_opcodes_table[i].hex_x);

    printf(" ");

    if (g_opcodes_table[i].type == 8 || g_opcodes_table[i].type == 9 || g_opcodes_table[i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_opcodes_table[i].op[k] == '*')
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
      k++;
      printf("%d", g_opcodes_table[i].value);
      while (1) {
        if (g_opcodes_table[i].op[k] == 0)
          break;
        else
          printf("%c", g_opcodes_table[i].op[k]);
        k++;
      }
    }
    else
      printf("\"%s", g_opcodes_table[i].op);

    printf("\"\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    if (g_opcodes_table[i].hex & 0xFF00)
      printf("$%.2X%.2X ", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X ", g_opcodes_table[i].hex);

    printf("\"%s\"\n", g_opcodes_table[i].op);
#endif

#ifdef W65816
    if (g_opcodes_table[i].hex & 0xFF00 && g_opcodes_table[i].type != 8)
      printf("$%.2X%.2X ", g_opcodes_table[i].hex & 0xFF, (g_opcodes_table[i].hex >> 8) & 0xFF);
    else if (g_opcodes_table[i].type == 8)
      printf("$%.4x ", g_opcodes_table[i].hex);
    else
      printf("$%.2X ", g_opcodes_table[i].hex);

    printf("\"%s\"\n", g_opcodes_table[i].op);
#endif

#ifdef SPC700
    printf("$%.2X ", g_opcodes_table[i].hex);
    printf("\"%s\"\n", g_opcodes_table[i].op);
#endif

#ifdef SUPERFX
    printed_something = NO;

    if (g_opcodes_table[i].prefix != 0) {
      if (g_opcodes_table[i].type == 6) {
        printf("$%.2X|*", g_opcodes_table[i].prefix);
        printed_something = YES;
      }
      else if (g_opcodes_table[i].type == 0 || g_opcodes_table[i].type == 1 || g_opcodes_table[i].type == 3 || g_opcodes_table[i].type == 4 ||
          g_opcodes_table[i].type == 5 || g_opcodes_table[i].type == 14 || g_opcodes_table[i].type == 15) {
        printf("$%.2X", g_opcodes_table[i].prefix);
        printed_something = YES;
      }
    }

    if (printed_something == YES)
      printf(" ");
    
    if (g_opcodes_table[i].type == 0)
      printf("$%.2X", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 1)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 2)
      printf("$%.2X", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 3)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 4)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 5)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 6)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 7)
      printf("$A0|* or $F0|*");
    else if (g_opcodes_table[i].type == 8)
      printf("$3D $A0|* or $3D $F0|*");
    else if (g_opcodes_table[i].type == 9)
      printf("$3E $F0|* or $3D $F0|*");
    else if (g_opcodes_table[i].type == 10)
      printf("$3D $40|* or $10|* $3D $40|*");
    else if (g_opcodes_table[i].type == 11)
      printf("$3D $30|* or $B0|* $3D $30|*");
    else if (g_opcodes_table[i].type == 12)
      printf("$40|* or $10|* $40|*");
    else if (g_opcodes_table[i].type == 13)
      printf("$30|* or $B0|* $30|*");
    else if (g_opcodes_table[i].type == 14)
      printf("$%.2X|*", g_opcodes_table[i].hex);
    else if (g_opcodes_table[i].type == 15)
      printf("$%.2X|*", g_opcodes_table[i].hex);

    if (g_opcodes_table[i].type == 11 || g_opcodes_table[i].type == 13)
      printf(" [0, 11]");
    if (g_opcodes_table[i].min > 0 || g_opcodes_table[i].max > 0)
      printf(" [%d, %d]", g_opcodes_table[i].min, g_opcodes_table[i].max);
    if (g_opcodes_table[i].type == 10 || g_opcodes_table[i].type == 12)
      printf(" [0, 11]");

    printf(" \"%s\"", g_opcodes_table[i].op);
    
    printf("\n");
#endif

#endif

    i++;
  }

  return 0;
}

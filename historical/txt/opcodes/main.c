
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../defines.h"


#ifdef GB
#include "../../../igb.c"
#endif
#ifdef Z80
#include "../../../iz80.c"
#endif
#ifdef Z80N
#include "../../../iz80n.c"
#endif
#ifdef MCS6502
#include "../../../i6502.c"
#endif
#ifdef WDC65C02
#include "../../../i65c02.c"
#endif
#ifdef CSG65CE02
#include "../../../i65ce02.c"
#endif
#ifdef W65816
#include "../../../i65816.c"
#endif
#ifdef SPC700
#include "../../../ispc700.c"
#endif
#ifdef SUPERFX
#include "../../../isuperfx.c"
#endif
#ifdef HUC6280
#include "../../../ihuc6280.c"
#endif
#ifdef I8008
#include "../../../i8008.c"
#endif
#ifdef I8080
#include "../../../i8080.c"
#endif
#ifdef MC68000
#include "../../../i68000.c"
#endif
#ifdef MC6800
#include "../../../i6800.c"
#endif
#ifdef MC6801
#include "../../../i6801.c"
#endif
#ifdef MC6809
#include "../../../i6809.c"
#endif


/* this program is used to print out the mnemonic tables */


int main(int argc, char *argv[]) {

  int i, l, n;
#if defined(SUPERFX) && defined(NICELIST2)
  int printed_something;
#endif

  i = 0;
  while (1) {
    if (strcmp(g_instructions_table[i].string, "E") == 0)
      break;

    /* filter *.B, *.W and *.L away */
    l = strlen(g_instructions_table[i].string);
    for (n = 0; n < l; n++) {
      if (g_instructions_table[i].string[n] == '.')
        break;
    }

    if (n < l && (g_instructions_table[i].string[n+1] == 'B' || g_instructions_table[i].string[n+1] == 'W' || g_instructions_table[i].string[n+1] == 'L')) {
      i++;
      continue;
    }

#if defined(I8008) || defined(I8080)
    if (g_instructions_table[i].type == 100) {
      i++;
      continue;
    }
#endif

#if defined(REARRANGE)

#if defined(GB) || defined(I8008) || defined(I8080)
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_instructions_table[i].string, g_instructions_table[i].hex, g_instructions_table[i].type, g_instructions_table[i].value);
#endif
#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_instructions_table[i].string, g_instructions_table[i].hex, g_instructions_table[i].type, g_instructions_table[i].skip_8bit);
#endif
#if defined(MC68000)
    printf("  { \"%s\", 0x%X, %d },\n", g_instructions_table[i].string, g_instructions_table[i].hex, g_instructions_table[i].type);
#endif
#if defined(Z80) || defined(Z80N)
    printf("  { \"%s\", 0x%X, %d, %d, %d },\n", g_instructions_table[i].string, g_instructions_table[i].hex, g_instructions_table[i].type, g_instructions_table[i].hex_x, g_instructions_table[i].value);
#endif
#if defined(W65816)
    printf("  { \"%s\", 0x%X, %d, %d },\n", g_instructions_table[i].string, g_instructions_table[i].hex, g_instructions_table[i].type, g_instructions_table[i].skip_xbit);
#endif
#if defined(SUPERFX)
    printf("  { \"%s\", %d, 0x%X, 0x%X, %d, %d },\n", g_instructions_table[i].string, g_instructions_table[i].type, g_instructions_table[i].hex, g_instructions_table[i].prefix, g_instructions_table[i].min, g_instructions_table[i].max);
#endif
    
#endif



#if defined(NICELIST1) /* alphabetical order */

#if defined(GB)
    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_instructions_table[i].string);

    if ((g_instructions_table[i].hex & 0xFF) == 0xCB)
      printf("$%.2X%.2X\n", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(I8008) || defined(I8080)
    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_instructions_table[i].string);

    printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(Z80) || defined(Z80N)
    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9 || g_instructions_table[i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", g_instructions_table[i].string);

    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 10 || g_instructions_table[i].type == 5)
      printf(" x $%.2X", g_instructions_table[i].hex_x);

    printf("\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280)
    printf("\"%s\" ", g_instructions_table[i].string);

    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X\n", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(MC6800) || defined(MC6801)
    printf("\"%s\" ", g_instructions_table[i].string);
    printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(MC68000)
    printf("\"%s\" ", g_instructions_table[i].string);
    printf("$%.4X\n", g_instructions_table[i].hex);
#endif

#if defined(MC6809)
    printf("\"%s\" ", g_instructions_table[i].string);

    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", (g_instructions_table[i].hex >> 8) & 0xFF, g_instructions_table[i].hex & 0xFF);
    else
      printf("$%.2X", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 6 ||
        g_instructions_table[i].type == 7 ||
        g_instructions_table[i].type == 8 ||
        g_instructions_table[i].type == 9)
      printf("%.2X", g_instructions_table[i].addressing_mode_bits);
    
    printf("\n");
#endif
    
#if defined(W65816)
    printf("\"%s\" ", g_instructions_table[i].string);

    if (g_instructions_table[i].hex & 0xFF00 && g_instructions_table[i].type != 8)
      printf("$%.2X%.2X\n", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else if (g_instructions_table[i].type == 8)
      printf("$%.4x\n", g_instructions_table[i].hex);
    else
      printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(SPC700)
    printf("\"%s\" ", g_instructions_table[i].string);
    printf("$%.2X\n", g_instructions_table[i].hex);
#endif

#if defined(SUPERFX)
    printf("\"%s\"", g_instructions_table[i].string);

    if (g_instructions_table[i].prefix != 0) {
      if (g_instructions_table[i].type == 6)
        printf(" $%.2X|*", g_instructions_table[i].prefix);
      else if (g_instructions_table[i].type == 0 || g_instructions_table[i].type == 1 || g_instructions_table[i].type == 3 || g_instructions_table[i].type == 4 ||
          g_instructions_table[i].type == 5 || g_instructions_table[i].type == 14 || g_instructions_table[i].type == 15) {
        printf(" $%.2X", g_instructions_table[i].prefix);
      }
    }

    if (g_instructions_table[i].type == 0)
      printf(" $%.2X", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 1)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 2)
      printf(" $%.2X", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 3)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 4)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 5)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 6)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 7)
      printf(" $A0|* or $F0|*");
    else if (g_instructions_table[i].type == 8)
      printf(" $3D $A0|* or $3D $F0|*");
    else if (g_instructions_table[i].type == 9)
      printf(" $3E $F0|* or $3D $F0|*");
    else if (g_instructions_table[i].type == 10)
      printf(" $3D $40|* or $10|* $3D $40|*");
    else if (g_instructions_table[i].type == 11)
      printf(" $3D $30|* or $B0|* $3D $30|*");
    else if (g_instructions_table[i].type == 12)
      printf(" $40|* or $10|* $40|*");
    else if (g_instructions_table[i].type == 13)
      printf(" $30|* or $B0|* $30|*");
    else if (g_instructions_table[i].type == 14)
      printf(" $%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 15)
      printf(" $%.2X|*", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 11 || g_instructions_table[i].type == 13)
      printf(" [0, 11]");
    if (g_instructions_table[i].min > 0 || g_instructions_table[i].max > 0)
      printf(" [%d, %d]", g_instructions_table[i].min, g_instructions_table[i].max);
    if (g_instructions_table[i].type == 10 || g_instructions_table[i].type == 12)
      printf(" [0, 11]");
    printf("\n");
#endif
    
#endif



#if defined(NICELIST2) /* instruction order */

#if defined(GB)
    if ((g_instructions_table[i].hex & 0xFF) == 0xCB)
      printf("$%.2X%.2X ", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X ", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(I8008) || defined(I8080)
    printf("$%.2X ", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(Z80) || defined(Z80N)
    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 10 || g_instructions_table[i].type == 5)
      printf(" x $%.2X", g_instructions_table[i].hex_x);

    printf(" ");

    if (g_instructions_table[i].type == 8 || g_instructions_table[i].type == 9 || g_instructions_table[i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
        if (g_instructions_table[i].string[k] == '*')
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
      k++;
      printf("%d", g_instructions_table[i].value);
      while (1) {
        if (g_instructions_table[i].string[k] == 0)
          break;
        else
          printf("%c", g_instructions_table[i].string[k]);
        k++;
      }
    }
    else
      printf("\"%s", g_instructions_table[i].string);

    printf("\"\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280)
    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X ", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else
      printf("$%.2X ", g_instructions_table[i].hex);

    printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(MC6800) || defined(MC6801)
    printf("$%.2X ", g_instructions_table[i].hex);
    printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(MC68000)
    printf("$%.4X ", g_instructions_table[i].hex);
    printf("\"%s\"\n", g_instructions_table[i].string);
#endif
    
#if defined(MC6809)
    if (g_instructions_table[i].hex & 0xFF00)
      printf("$%.2X%.2X", (g_instructions_table[i].hex >> 8) & 0xFF, g_instructions_table[i].hex & 0xFF);
    else
      printf("$%.2X", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 6 ||
        g_instructions_table[i].type == 7 ||
        g_instructions_table[i].type == 8 ||
        g_instructions_table[i].type == 9)
      printf("%.2X", g_instructions_table[i].addressing_mode_bits);
    
    printf(" \"%s\"\n", g_instructions_table[i].string);
#endif
    
#if defined(W65816)
    if (g_instructions_table[i].hex & 0xFF00 && g_instructions_table[i].type != 8)
      printf("$%.2X%.2X ", g_instructions_table[i].hex & 0xFF, (g_instructions_table[i].hex >> 8) & 0xFF);
    else if (g_instructions_table[i].type == 8)
      printf("$%.4x ", g_instructions_table[i].hex);
    else
      printf("$%.2X ", g_instructions_table[i].hex);

    printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(SPC700)
    printf("$%.2X ", g_instructions_table[i].hex);
    printf("\"%s\"\n", g_instructions_table[i].string);
#endif

#if defined(SUPERFX)
    printed_something = NO;

    if (g_instructions_table[i].prefix != 0) {
      if (g_instructions_table[i].type == 6) {
        printf("$%.2X|*", g_instructions_table[i].prefix);
        printed_something = YES;
      }
      else if (g_instructions_table[i].type == 0 || g_instructions_table[i].type == 1 || g_instructions_table[i].type == 3 || g_instructions_table[i].type == 4 ||
          g_instructions_table[i].type == 5 || g_instructions_table[i].type == 14 || g_instructions_table[i].type == 15) {
        printf("$%.2X", g_instructions_table[i].prefix);
        printed_something = YES;
      }
    }

    if (printed_something == YES)
      printf(" ");
    
    if (g_instructions_table[i].type == 0)
      printf("$%.2X", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 1)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 2)
      printf("$%.2X", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 3)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 4)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 5)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 6)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 7)
      printf("$A0|* or $F0|*");
    else if (g_instructions_table[i].type == 8)
      printf("$3D $A0|* or $3D $F0|*");
    else if (g_instructions_table[i].type == 9)
      printf("$3E $F0|* or $3D $F0|*");
    else if (g_instructions_table[i].type == 10)
      printf("$3D $40|* or $10|* $3D $40|*");
    else if (g_instructions_table[i].type == 11)
      printf("$3D $30|* or $B0|* $3D $30|*");
    else if (g_instructions_table[i].type == 12)
      printf("$40|* or $10|* $40|*");
    else if (g_instructions_table[i].type == 13)
      printf("$30|* or $B0|* $30|*");
    else if (g_instructions_table[i].type == 14)
      printf("$%.2X|*", g_instructions_table[i].hex);
    else if (g_instructions_table[i].type == 15)
      printf("$%.2X|*", g_instructions_table[i].hex);

    if (g_instructions_table[i].type == 11 || g_instructions_table[i].type == 13)
      printf(" [0, 11]");
    if (g_instructions_table[i].min > 0 || g_instructions_table[i].max > 0)
      printf(" [%d, %d]", g_instructions_table[i].min, g_instructions_table[i].max);
    if (g_instructions_table[i].type == 10 || g_instructions_table[i].type == 12)
      printf(" [0, 11]");

    printf(" \"%s\"", g_instructions_table[i].string);
    
    printf("\n");
#endif

#endif

    i++;
  }

  return 0;
}

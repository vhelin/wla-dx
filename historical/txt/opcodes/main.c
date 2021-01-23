
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
#ifdef MCS6510
#include "../../../opcodes_6510.c"
#endif
#ifdef W65816
#include "../../../opcodes_65816.c"
#endif
#ifdef SPC700
#include "../../../opcodes_spc700.c"
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

  int local_i, l, n;


  local_i = 0;
  while (1) {
    if (strcmp(opt_table[local_i].op, "E") == 0)
      break;

    /* filter *.B, *.W and *.L away */
    l = strlen(opt_table[local_i].op);
    for (n = 0; n < l; n++) {
      if (opt_table[local_i].op[n] == '.')
	break;
    }

    if (n < l && (opt_table[local_i].op[n+1] == 'B' || opt_table[local_i].op[n+1] == 'W' || opt_table[local_i].op[n+1] == 'L')) {
      local_i++;
      continue;
    }

#if defined(I8008) || defined(I8080)
    if (opt_table[local_i].type == 100) {
      local_i++;
      continue;
	}
#endif

#ifdef REARRANGE

#if defined(GB) || defined(I8008) || defined(I8080)
    printf("  { \"%s\", 0x%x, %d, %d },\n", opt_table[local_i].op, opt_table[local_i].hex, opt_table[local_i].type, opt_table[local_i].value);
#endif
#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MCS6510) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    printf("  { \"%s\", 0x%x, %d, %d },\n", opt_table[local_i].op, opt_table[local_i].hex, opt_table[local_i].type, opt_table[local_i].skip_8bit);
#endif
#ifdef Z80
    printf("  { \"%s\", 0x%x, %d, %d, %d },\n", opt_table[local_i].op, opt_table[local_i].hex, opt_table[local_i].type, opt_table[local_i].hex_x, opt_table[local_i].value);
#endif
#ifdef W65816
    printf("  { \"%s\", 0x%x, %d, %d },\n", opt_table[local_i].op, opt_table[local_i].hex, opt_table[local_i].type, opt_table[local_i].skip_xbit);
#endif

#endif



#ifdef NICELIST1

#if defined(GB)
	if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", opt_table[local_i].op);

    if ((opt_table[local_i].hex & 0xFF) == 0xCB)
      printf("$%.2x%.2x\n", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x\n", opt_table[local_i].hex);
#endif

#if defined(I8008) || defined(I8080)
	if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", opt_table[local_i].op);

    printf("$%.2x\n", opt_table[local_i].hex);
#endif

#ifdef Z80
    if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9 || opt_table[local_i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      printf("\" ");
    }
    else
      printf("\"%s\" ", opt_table[local_i].op);

    if (opt_table[local_i].hex & 0xFF00)
      printf("$%.2x%.2x", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x", opt_table[local_i].hex);

    if (opt_table[local_i].type == 10 || opt_table[local_i].type == 5)
      printf(" x $%.2x", opt_table[local_i].hex_x);

    printf("\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MCS6510) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    printf("\"%s\" ", opt_table[local_i].op);

    if (opt_table[local_i].hex & 0xFF00)
      printf("$%.2x%.2x\n", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x\n", opt_table[local_i].hex);
#endif

#ifdef W65816
    printf("\"%s\" ", opt_table[local_i].op);

    if (opt_table[local_i].hex & 0xFF00 && opt_table[local_i].type != 8)
      printf("$%.2x%.2x\n", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else if (opt_table[local_i].type == 8)
      printf("$%.4x\n", opt_table[local_i].hex);
    else
      printf("$%.2x\n", opt_table[local_i].hex);
#endif

#ifdef SPC700
    printf("\"%s\" ", opt_table[local_i].op);
    printf("$%.2x\n", opt_table[local_i].hex);
#endif

#endif



#ifdef NICELIST2

#if defined(GB)
	if ((opt_table[local_i].hex & 0xFF) == 0xCB)
      printf("$%.2x%.2x ", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x ", opt_table[local_i].hex);

    if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", opt_table[local_i].op);
#endif

#if defined(I8008) || defined(I8080)
    printf("$%.2x ", opt_table[local_i].hex);

    if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      printf("\"\n");
    }
    else
      printf("\"%s\"\n", opt_table[local_i].op);
#endif

#ifdef Z80
    if (opt_table[local_i].hex & 0xFF00)
      printf("$%.2x%.2x", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x", opt_table[local_i].hex);

    if (opt_table[local_i].type == 10 || opt_table[local_i].type == 5)
      printf(" x $%.2x", opt_table[local_i].hex_x);

    printf(" ");

    if (opt_table[local_i].type == 8 || opt_table[local_i].type == 9 || opt_table[local_i].type == 10) {
      int k;

      k = 0;
      printf("\"");
      while (1) {
	if (opt_table[local_i].op[k] == '*')
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
      k++;
      printf("%d", opt_table[local_i].value);
      while (1) {
	if (opt_table[local_i].op[k] == 0)
	  break;
	else
	  printf("%c", opt_table[local_i].op[k]);
	k++;
      }
    }
    else
      printf("\"%s", opt_table[local_i].op);

    printf("\"\n");
#endif

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280) || defined(MCS6510) || defined(MC6800) || defined(MC6801) || defined(MC6809)
    if (opt_table[local_i].hex & 0xFF00)
      printf("$%.2x%.2x ", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else
      printf("$%.2x ", opt_table[local_i].hex);

    printf("\"%s\"\n", opt_table[local_i].op);
#endif

#ifdef W65816
    if (opt_table[local_i].hex & 0xFF00 && opt_table[local_i].type != 8)
      printf("$%.2x%.2x ", opt_table[local_i].hex & 0xFF, (opt_table[local_i].hex >> 8) & 0xFF);
    else if (opt_table[local_i].type == 8)
      printf("$%.4x ", opt_table[local_i].hex);
    else
      printf("$%.2x ", opt_table[local_i].hex);

    printf("\"%s\"\n", opt_table[local_i].op);
#endif

#ifdef SPC700
    printf("$%.2x ", opt_table[local_i].hex);
    printf("\"%s\"\n", opt_table[local_i].op);
#endif

#endif

    local_i++;
  }

  return 0;
}

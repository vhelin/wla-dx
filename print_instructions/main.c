
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../defines.h"
#include "defines.h"


#ifdef GB
#include "../igb.c"
#endif
#ifdef Z80
#include "../iz80.c"
#endif
#ifdef MCS6502
#include "../i6502.c"
#endif
#ifdef WDC65C02
#include "../i65c02.c"
#endif
#ifdef W65816
#include "../i65816.c"
#endif
#ifdef SPC700
#include "../ispc700.c"
#endif


/* this program is used to print the instructions */


int main(int argc, char *argv[]) {

  int i;


  i = 0;
  while (TRUE) {
    printf("%s\n", opt_table[i].op);
    i++;
    if (opt_table[i].type == -1)
      break;
  }

  return 0;
}

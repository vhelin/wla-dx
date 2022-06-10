#define FILE void
#include "defines.h"
struct instruction g_instructions_table[] = {
  { "ADC R*", 1, 0x50, 0x3D, 0, 15 },
  { "ADC #*", 1, 0x50, 0x3F, 0, 15 },
  { "ADD R*", 1, 0x50, 0x00, 0, 15 },
  { "ADD #*", 1, 0x50, 0x3E, 0, 15 },
  { "ALT1", 0, 0x3D, 0x00, 0, 0 },
  { "ALT2", 0, 0x3E, 0x00, 0, 0 },
  { "ALT3", 0, 0x3F, 0x00, 0, 0 },
  { "AND R*", 1, 0x70, 0x00, 1, 15 },
  { "AND #*", 1, 0x70, 0x3E, 1, 15 },
  { "ASR", 0, 0x96, 0x00, 0, 0 },

  { "BCC e", 2, 0x0C, 0x00, 0, 0 },
  { "BCS e", 2, 0x0D, 0x00, 0, 0 },
  { "BEQ e", 2, 0x09, 0x00, 0, 0 },
  { "BGE e", 2, 0x06, 0x00, 0, 0 },
  { "BIC R*", 1, 0x70, 0x3D, 1, 15 },
  { "BIC #*", 1, 0x70, 0x3F, 1, 15 },
  { "BLT e", 2, 0x07, 0x00, 0, 0 },
  { "BMI e", 2, 0x0B, 0x00, 0, 0 },
  { "BNE e", 2, 0x08, 0x00, 0, 0 },
  { "BPL e", 2, 0x0A, 0x00, 0, 0 },
  { "BRA e", 2, 0x05, 0x00, 0, 0 },
  { "BVC e", 2, 0x0E, 0x00, 0, 0 },
  { "BVS e", 2, 0x0F, 0x00, 0, 0 },

  { "CACHE", 0, 0x02, 0x00, 0, 0 },
  { "CMODE", 0, 0x4E, 0x3D, 0, 0 },
  { "CMP R*", 1, 0x60, 0x3F, 0, 15 },
  { "COLOR", 0, 0x4E, 0x00, 0, 0 },

  { "DEC R*", 1, 0xE0, 0x00, 0, 14 },
  { "DIV2", 0, 0x96, 0x3D, 0, 0 },

  { "FMULT", 0, 0x9F, 0x00, 0, 0 },
  { "FROM R*", 1, 0xB0, 0x00, 0, 15 },

  { "GETBH", 0, 0xEF, 0x3D, 0, 0 },
  { "GETBL", 0, 0xEF, 0x3E, 0, 0 },
  { "GETBS", 0, 0xEF, 0x3F, 0, 0 },
  { "GETB", 0, 0xEF, 0x00, 0, 0 },
  { "GETC", 0, 0xDF, 0x00, 0, 0 },

  { "HIB", 0, 0xC0, 0x00, 0, 0 },

  { "IBT R*,#x", 3, 0xA0, 0x00, 0, 15 },
  { "INC R*", 1, 0xD0, 0x00, 0, 14 },  
  { "IWT R*,#?", 4, 0xF0, 0x00, 0, 15 },

  { "JMP R*", 1, 0x90, 0x00, 8, 13 },

  { "LDB (R*)", 1, 0x40, 0x3D, 0, 11 },
  { "LDW (R*)", 1, 0x40, 0x00, 0, 11 },
  { "LEA R*,?", 4, 0xF0, 0x00, 0, 15 },
  { "LINK #*", 1, 0x90, 0x00, 1, 4 },
  { "LJMP R*", 1, 0x90, 0x3D, 8, 13 },
  { "LM R*,(?)", 4, 0xF0, 0x3D, 0, 15 },
  { "LMS R*,(y)", 5, 0xA0, 0x3D, 0, 15 },
  { "LMULT", 0, 0x9F, 0x3D, 0, 0 },
  { "LOB", 0, 0x9E, 0x00, 0, 0 },
  { "LOOP", 0, 0x3C, 0x00, 0, 0 },
  { "LSR", 0, 0x03, 0x00, 0, 0 },

  { "MERGE", 0, 0x70, 0x00, 0, 0 },
  { "MOVE R*,R*", 6, 0x10, 0x20, 0, 15 },
  { "MOVE R*,#?", 7, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVE R*,(?)", 8, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVE (?),R*", 9, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVEB R*,(R*)", 10, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVEB (R*),R*", 11, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVES R*,R*", 6, 0xB0, 0x20, 0, 15 },
  { "MOVEW R*,(R*)", 12, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MOVEW (R*),R*", 13, 0x00, 0x00, 0, 15 }, /* MACRO INSTRUCTION */
  { "MULT R*", 1, 0x80, 0x00, 0, 15 },
  { "MULT #*", 1, 0x80, 0x3E, 0, 15 },  
  
  { "NOP", 0, 0x01, 0x00, 0, 0 },
  { "NOT", 0, 0x4F, 0x00, 0, 0 },

  { "OR R*", 1, 0xC0, 0x00, 1, 15 },
  { "OR #*", 1, 0xC0, 0x3E, 1, 15 },

  { "PLOT", 0, 0x4C, 0x00, 0, 0 },

  { "RAMB", 0, 0xDF, 0x3E, 0, 0 },
  { "ROL", 0, 0x04, 0x00, 0, 0 },
  { "ROMB", 0, 0xDF, 0x3F, 0, 0 },
  { "ROR", 0, 0x97, 0x00, 0, 0 },
  { "RPIX", 0, 0x4C, 0x3D, 0, 0 },

  { "SBC R*", 1, 0x60, 0x3D, 0, 15 },
  { "SBK", 0, 0x90, 0x00, 0, 0 },
  { "SEX", 0, 0x95, 0x00, 0, 0 },
  { "SM (?),R*", 14, 0xF0, 0x3E, 0, 15 },
  { "SMS (y),R*", 15, 0xA0, 0x3E, 0, 15 },
  { "STB (R*)", 1, 0x30, 0x3D, 0, 11 },
  { "STOP", 0, 0x00, 0x00, 0, 0 },
  { "STW (R*)", 1, 0x30, 0x00, 0, 11 },
  { "SUB R*", 1, 0x60, 0x00, 0, 15 },
  { "SUB #*", 1, 0x60, 0x3E, 0, 15 },
  { "SWAP", 0, 0x4D, 0x00, 0, 0 },

  { "TO R*", 1, 0x10, 0x00, 0, 15 },

  { "UMULT R*", 1, 0x80, 0x3D, 0, 15 },
  { "UMULT #*", 1, 0x80, 0x3F, 0, 15 },

  { "WITH R*", 1, 0x20, 0x00, 0, 15 },

  { "XOR R*", 1, 0xC0, 0x3D, 1, 15 },
  { "XOR #*", 1, 0xC0, 0x3F, 1, 15 },
  
  { "E", 0xFF, 0, 0, 0, 0 }
};

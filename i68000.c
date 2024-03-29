#define FILE void
#include "defines.h"
struct instruction g_instructions_table[] = {

  { "ABCD", B16(11000001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },
  { "ABCD.B", B16(11000001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },

  { "ADD", B16(11010000, 00000000), 2, B8(00000001), MC68000_MODE_ALL },
  { "ADD.B", B16(11010000, 00000000), 2, B8(00000000), MC68000_MODE_ALL },
  { "ADD.W", B16(11010000, 00000000), 2, B8(00000001), MC68000_MODE_ALL },
  { "ADD.L", B16(11010000, 00000000), 2, B8(00000010), MC68000_MODE_ALL },

  { "ADDA", B16(11010000, 00000000), 2, B8(00000001), MC68000_MODE_A },
  { "ADDA.W", B16(11010000, 00000000), 2, B8(00000001), MC68000_MODE_A },
  { "ADDA.L", B16(11010000, 00000000), 2, B8(00000010), MC68000_MODE_A },

  { "ADDI", B16(00000110, 00000000), 2, B8(00000001), MC68000_MODE_I },
  { "ADDI.B", B16(00000110, 00000000), 2, B8(00000000), MC68000_MODE_I },
  { "ADDI.W", B16(00000110, 00000000), 2, B8(00000001), MC68000_MODE_I },
  { "ADDI.L", B16(00000110, 00000000), 2, B8(00000010), MC68000_MODE_I },

  { "ADDQ", B16(01010000, 00000000), 2, B8(00000001), MC68000_MODE_Q },
  { "ADDQ.B", B16(01010000, 00000000), 2, B8(00000000), MC68000_MODE_Q },
  { "ADDQ.W", B16(01010000, 00000000), 2, B8(00000001), MC68000_MODE_Q },
  { "ADDQ.L", B16(01010000, 00000000), 2, B8(00000010), MC68000_MODE_Q },

  { "ADDX", B16(11010001, 01000000), 1, B8(00000001), MC68000_MODE_ALL },
  { "ADDX.B", B16(11010001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },
  { "ADDX.W", B16(11010001, 01000000), 1, B8(00000001), MC68000_MODE_ALL },
  { "ADDX.L", B16(11010001, 10000000), 1, B8(00000010), MC68000_MODE_ALL },

  { "AND", B16(11000000, 00000000), 3, B8(00000001), MC68000_MODE_ALL },
  { "AND.B", B16(11000000, 00000000), 3, B8(00000000), MC68000_MODE_ALL },
  { "AND.W", B16(11000000, 00000000), 3, B8(00000001), MC68000_MODE_ALL },
  { "AND.L", B16(11000000, 00000000), 3, B8(00000010), MC68000_MODE_ALL },

  { "ANDI", B16(00000010, 00000000), 3, B8(00000001), MC68000_MODE_I },
  { "ANDI.B", B16(00000010, 00000000), 3, B8(00000000), MC68000_MODE_I },
  { "ANDI.W", B16(00000010, 00000000), 3, B8(00000001), MC68000_MODE_I },
  { "ANDI.L", B16(00000010, 00000000), 3, B8(00000010), MC68000_MODE_I },

  { "ASL", B16(11100001, 00000000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ASL.B", B16(11100001, 00000000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ASL.W", B16(11100001, 00000000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ASL.L", B16(11100001, 00000000), 4, B8(00000010), MC68000_MODE_ALL },

  { "ASR", B16(11100000, 00000000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ASR.B", B16(11100000, 00000000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ASR.W", B16(11100000, 00000000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ASR.L", B16(11100000, 00000000), 4, B8(00000010), MC68000_MODE_ALL },

  { "BCC", B16(01100100, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BCC.B", B16(01100100, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BCC.W", B16(01100100, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BCS", B16(01100101, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BCS.B", B16(01100101, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BCS.W", B16(01100101, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BEQ", B16(01100111, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BEQ.B", B16(01100111, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BEQ.W", B16(01100111, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BGE", B16(01101100, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BGE.B", B16(01101100, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BGE.W", B16(01101100, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BGT", B16(01101110, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BGT.B", B16(01101110, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BGT.W", B16(01101110, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BHI", B16(01100010, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BHI.B", B16(01100010, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BHI.W", B16(01100010, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLE", B16(01101111, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLE.B", B16(01101111, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BLE.W", B16(01101111, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLS", B16(01100011, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLS.B", B16(01100011, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BLS.W", B16(01100011, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLT", B16(01101101, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BLT.B", B16(01101101, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BLT.W", B16(01101101, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BMI", B16(01101011, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BMI.B", B16(01101011, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BMI.W", B16(01101011, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BNE", B16(01100110, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BNE.B", B16(01100110, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BNE.W", B16(01100110, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BPL", B16(01101010, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BPL.B", B16(01101010, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BPL.W", B16(01101010, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BVC", B16(01101000, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BVC.B", B16(01101000, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BVC.W", B16(01101000, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BVS", B16(01101001, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BVS.B", B16(01101001, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BVS.W", B16(01101001, 00000000), 5, B8(00000001), MC68000_MODE_ALL },

  { "BCHG", B16(00000001, 01000000), 6, B8(00000000), MC68000_MODE_ALL },
  { "BCLR", B16(00000001, 10000000), 6, B8(00000000), MC68000_MODE_ALL },

  { "BRA", B16(01100000, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BRA.B", B16(01100000, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BRA.W", B16(01100000, 00000000), 5, B8(00000001), MC68000_MODE_ALL },

  { "BSET", B16(00000001, 11000000), 6, B8(00000000), MC68000_MODE_ALL },
  { "BTST", B16(00000001, 00000000), 6, B8(00000000), MC68000_MODE_ALL },  

  { "BSR", B16(01100001, 00000000), 5, B8(00000001), MC68000_MODE_ALL },
  { "BSR.B", B16(01100001, 00000000), 5, B8(00000000), MC68000_MODE_ALL },
  { "BSR.W", B16(01100001, 00000000), 5, B8(00000001), MC68000_MODE_ALL },

  { "CHK", B16(01000000, 00000000), 7, B8(00000001), MC68000_MODE_ALL },

  { "CLR", B16(01000010, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "CLR.B", B16(01000010, 00000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "CLR.W", B16(01000010, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "CLR.L", B16(01000010, 10000000), 8, B8(00000010), MC68000_MODE_ALL },

  { "CMP", B16(10110000, 00000000), 9, B8(00000001), MC68000_MODE_ALL },
  { "CMP.B", B16(10110000, 00000000), 9, B8(00000000), MC68000_MODE_ALL },
  { "CMP.W", B16(10110000, 00000000), 9, B8(00000001), MC68000_MODE_ALL },
  { "CMP.L", B16(10110000, 00000000), 9, B8(00000010), MC68000_MODE_ALL },

  { "CMPA", B16(10110000, 00000000), 9, B8(00000001), MC68000_MODE_A },
  { "CMPA.W", B16(10110000, 00000000), 9, B8(00000001), MC68000_MODE_A },
  { "CMPA.L", B16(10110000, 00000000), 9, B8(00000010), MC68000_MODE_A },

  { "CMPI", B16(00001100, 00000000), 9, B8(00000001), MC68000_MODE_I },
  { "CMPI.B", B16(00001100, 00000000), 9, B8(00000000), MC68000_MODE_I },
  { "CMPI.W", B16(00001100, 00000000), 9, B8(00000001), MC68000_MODE_I },
  { "CMPI.L", B16(00001100, 00000000), 9, B8(00000010), MC68000_MODE_I },
  
  { "CMPM", B16(10110001, 00001000), 9, B8(00000001), MC68000_MODE_M },
  { "CMPM.B", B16(10110001, 00001000), 9, B8(00000000), MC68000_MODE_M },
  { "CMPM.W", B16(10110001, 00001000), 9, B8(00000001), MC68000_MODE_M },
  { "CMPM.L", B16(10110001, 00001000), 9, B8(00000010), MC68000_MODE_M },

  { "DBCC", B16(01010100, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBCS", B16(01010101, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBEQ", B16(01010111, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBF", B16(01010001, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBGE", B16(01011100, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBGT", B16(01011110, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBHI", B16(01010010, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBLE", B16(01011111, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBLS", B16(01010011, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBLT", B16(01011101, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBMI", B16(01011011, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBNE", B16(01010110, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBPL", B16(01011010, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBRA", B16(01010001, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBT", B16(01010000, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBVC", B16(01011000, 11001000), 10, B8(00000001), MC68000_MODE_ALL },
  { "DBVS", B16(01011001, 11001000), 10, B8(00000001), MC68000_MODE_ALL },

  { "DIVS", B16(10000001, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "DIVS.W", B16(10000001, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "DIVU", B16(10000000, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "DIVU.W", B16(10000000, 11000000), 11, B8(00000001), MC68000_MODE_ALL },

  { "EOR", B16(10110000, 00000000), 12, B8(00000001), MC68000_MODE_ALL },
  { "EOR.B", B16(10110000, 00000000), 12, B8(00000000), MC68000_MODE_ALL },
  { "EOR.W", B16(10110000, 00000000), 12, B8(00000001), MC68000_MODE_ALL },
  { "EOR.L", B16(10110000, 00000000), 12, B8(00000010), MC68000_MODE_ALL },

  { "EORI", B16(00001010, 00000000), 12, B8(00000001), MC68000_MODE_I },
  { "EORI.B", B16(00001010, 00000000), 12, B8(00000000), MC68000_MODE_I },
  { "EORI.W", B16(00001010, 00000000), 12, B8(00000001), MC68000_MODE_I },
  { "EORI.L", B16(00001010, 00000000), 12, B8(00000010), MC68000_MODE_I },

  { "EXG", B16(11000001, 00000000), 13, B8(00000010), MC68000_MODE_ALL },

  { "EXT", B16(01001000, 00000000), 14, B8(00000010), MC68000_MODE_ALL },
  { "EXT.W", B16(01001000, 00000000), 14, B8(00000010), MC68000_MODE_ALL },
  { "EXT.L", B16(01001000, 00000000), 14, B8(00000011), MC68000_MODE_ALL },
  
  /* 4A FC */
  { "ILLEGAL", B16(01001010, 11111100), 0, 0, MC68000_MODE_ALL },

  { "JMP", B16(01001110, 11000000), 15, B8(00000010), MC68000_MODE_ALL },

  { "JSR", B16(01001110, 10000000), 15, B8(00000010), MC68000_MODE_ALL },

  { "LEA", B16(01000001, 11000000), 16, B8(00000010), MC68000_MODE_ALL },

  { "LINK", B16(01001110, 01010000), 17, B8(00000001), MC68000_MODE_ALL },

  { "LSL", B16(11100001, 00001000), 4, B8(00000001), MC68000_MODE_ALL },
  { "LSL.B", B16(11100001, 00001000), 4, B8(00000000), MC68000_MODE_ALL },
  { "LSL.W", B16(11100001, 00001000), 4, B8(00000001), MC68000_MODE_ALL },
  { "LSL.L", B16(11100001, 00001000), 4, B8(00000010), MC68000_MODE_ALL },

  { "LSR", B16(11100000, 00001000), 4, B8(00000001), MC68000_MODE_ALL },
  { "LSR.B", B16(11100000, 00001000), 4, B8(00000000), MC68000_MODE_ALL },
  { "LSR.W", B16(11100000, 00001000), 4, B8(00000001), MC68000_MODE_ALL },
  { "LSR.L", B16(11100000, 00001000), 4, B8(00000010), MC68000_MODE_ALL },

  { "MOVE", B16(00000000, 00000000), 23, B8(00000001), MC68000_MODE_ALL },
  { "MOVE.B", B16(00000000, 00000000), 23, B8(00000000), MC68000_MODE_ALL },
  { "MOVE.W", B16(00000000, 00000000), 23, B8(00000001), MC68000_MODE_ALL },
  { "MOVE.L", B16(00000000, 00000000), 23, B8(00000010), MC68000_MODE_ALL },

  { "MOVEA", B16(00000000, 01000000), 23, B8(00000001), MC68000_MODE_A },
  { "MOVEA.W", B16(00000000, 01000000), 23, B8(00000001), MC68000_MODE_A },
  { "MOVEA.L", B16(00000000, 01000000), 23, B8(00000010), MC68000_MODE_A },

  { "MOVEM", B16(01001000, 10000000), 25, B8(00000001), MC68000_MODE_ALL },
  { "MOVEM.W", B16(01001000, 10000000), 25, B8(00000001), MC68000_MODE_ALL },
  { "MOVEM.L", B16(01001000, 11000000), 25, B8(00000010), MC68000_MODE_ALL },
  
  { "MOVEP", B16(00000000, 00001000), 24, B8(00000001), MC68000_MODE_ALL },
  { "MOVEP.W", B16(00000000, 00001000), 24, B8(00000001), MC68000_MODE_ALL },
  { "MOVEP.L", B16(00000000, 00001000), 24, B8(00000010), MC68000_MODE_ALL },

  { "MOVEQ", B16(01110000, 00000000), 23, B8(00000010), MC68000_MODE_Q },
  { "MOVEQ.L", B16(01110000, 00000000), 23, B8(00000010), MC68000_MODE_Q },
  
  { "MULS", B16(11000001, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "MULS.W", B16(11000001, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "MULU", B16(11000000, 11000000), 11, B8(00000001), MC68000_MODE_ALL },
  { "MULU.W", B16(11000000, 11000000), 11, B8(00000001), MC68000_MODE_ALL },

  { "NBCD", B16(01001000, 00000000), 18, B8(00000000), MC68000_MODE_ALL },

  { "NEG", B16(01000100, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NEG.B", B16(01000100, 00000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "NEG.W", B16(01000100, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NEG.L", B16(01000100, 10000000), 8, B8(00000010), MC68000_MODE_ALL },

  { "NEGX", B16(01000000, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NEGX.B", B16(01000000, 00000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "NEGX.W", B16(01000000, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NEGX.L", B16(01000000, 10000000), 8, B8(00000010), MC68000_MODE_ALL },

  /* 4E 71 */
  { "NOP", B16(01001110, 01110001), 0, 0, MC68000_MODE_ALL },

  { "NOT", B16(01000110, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NOT.B", B16(01000110, 00000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "NOT.W", B16(01000110, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "NOT.L", B16(01000110, 10000000), 8, B8(00000010), MC68000_MODE_ALL },
  
  { "OR", B16(10000000, 00000000), 3, B8(00000001), MC68000_MODE_ALL },
  { "OR.B", B16(10000000, 00000000), 3, B8(00000000), MC68000_MODE_ALL },
  { "OR.W", B16(10000000, 00000000), 3, B8(00000001), MC68000_MODE_ALL },
  { "OR.L", B16(10000000, 00000000), 3, B8(00000010), MC68000_MODE_ALL },

  { "ORI", B16(00000000, 00000000), 3, B8(00000001), MC68000_MODE_I },
  { "ORI.B", B16(00000000, 00000000), 3, B8(00000000), MC68000_MODE_I },
  { "ORI.W", B16(00000000, 00000000), 3, B8(00000001), MC68000_MODE_I },
  { "ORI.L", B16(00000000, 00000000), 3, B8(00000010), MC68000_MODE_I },

  { "PEA", B16(01001000, 01000000), 15, B8(00000010), MC68000_MODE_ALL },
  
  /* 4E 70 */
  { "RESET", B16(01001110, 01110000), 0, 0, MC68000_MODE_ALL },

  { "ROL", B16(11100001, 00011000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROL.B", B16(11100001, 00011000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ROL.W", B16(11100001, 00011000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROL.L", B16(11100001, 00011000), 4, B8(00000010), MC68000_MODE_ALL },

  { "ROR", B16(11100000, 00011000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROR.B", B16(11100000, 00011000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ROR.W", B16(11100000, 00011000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROR.L", B16(11100000, 00011000), 4, B8(00000010), MC68000_MODE_ALL },

  { "ROXL", B16(11100001, 00010000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROXL.B", B16(11100001, 00010000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ROXL.W", B16(11100001, 00010000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROXL.L", B16(11100001, 00010000), 4, B8(00000010), MC68000_MODE_ALL },

  { "ROXR", B16(11100000, 00010000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROXR.B", B16(11100000, 00010000), 4, B8(00000000), MC68000_MODE_ALL },
  { "ROXR.W", B16(11100000, 00010000), 4, B8(00000001), MC68000_MODE_ALL },
  { "ROXR.L", B16(11100000, 00010000), 4, B8(00000010), MC68000_MODE_ALL },
  
  /* 4E 73 */
  { "RTE", B16(01001110, 01110011), 0, 0, MC68000_MODE_ALL },

  /* 4E 77 */
  { "RTR", B16(01001110, 01110111), 0, 0, MC68000_MODE_ALL },

  /* 4E 75 */
  { "RTS", B16(01001110, 01110101), 0, 0, MC68000_MODE_ALL },

  { "SBCD", B16(10000001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },
  { "SBCD.B", B16(10000001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },

  { "SCC", B16(01010100, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SCC.B", B16(01010100, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SCS", B16(01010101, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SCS.B", B16(01010101, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SEQ", B16(01010111, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SEQ.B", B16(01010111, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SF", B16(01010001, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SF.B", B16(01010001, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SGE", B16(01011100, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SGE.B", B16(01011100, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SGT", B16(01011110, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SGT.B", B16(01011110, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SHI", B16(01010010, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SHI.B", B16(01010010, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SLE", B16(01011111, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SLE.B", B16(01011111, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SLS", B16(01010011, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SLS.B", B16(01010011, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SLT", B16(01011101, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SLT.B", B16(01011101, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SMI", B16(01011011, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SMI.B", B16(01011011, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SNE", B16(01010110, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SNE.B", B16(01010110, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SPL", B16(01011010, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SPL.B", B16(01011010, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "ST", B16(01010000, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "ST.B", B16(01010000, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SVC", B16(01011000, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SVC.B", B16(01011000, 11000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "SVS", B16(01011001, 11000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "SVS.B", B16(01011001, 11000000), 8, B8(00000000), MC68000_MODE_ALL },

  { "STOP", B16(01001110, 01110010), 19, B8(00000001), MC68000_MODE_ALL },

  { "SUB", B16(10010000, 00000000), 2, B8(00000001), MC68000_MODE_ALL },
  { "SUB.B", B16(10010000, 00000000), 2, B8(00000000), MC68000_MODE_ALL },
  { "SUB.W", B16(10010000, 00000000), 2, B8(00000001), MC68000_MODE_ALL },
  { "SUB.L", B16(10010000, 00000000), 2, B8(00000010), MC68000_MODE_ALL },

  { "SUBA", B16(10010000, 00000000), 2, B8(00000001), MC68000_MODE_A },
  { "SUBA.W", B16(10010000, 00000000), 2, B8(00000001), MC68000_MODE_A },
  { "SUBA.L", B16(10010000, 00000000), 2, B8(00000010), MC68000_MODE_A },

  { "SUBI", B16(00000100, 00000000), 2, B8(00000001), MC68000_MODE_I },
  { "SUBI.B", B16(00000100, 00000000), 2, B8(00000000), MC68000_MODE_I },
  { "SUBI.W", B16(00000100, 00000000), 2, B8(00000001), MC68000_MODE_I },
  { "SUBI.L", B16(00000100, 00000000), 2, B8(00000010), MC68000_MODE_I },

  { "SUBQ", B16(01010001, 00000000), 2, B8(00000001), MC68000_MODE_Q },
  { "SUBQ.B", B16(01010001, 00000000), 2, B8(00000000), MC68000_MODE_Q },
  { "SUBQ.W", B16(01010001, 00000000), 2, B8(00000001), MC68000_MODE_Q },
  { "SUBQ.L", B16(01010001, 00000000), 2, B8(00000010), MC68000_MODE_Q },

  { "SUBX", B16(10010001, 01000000), 1, B8(00000001), MC68000_MODE_ALL },
  { "SUBX.B", B16(10010001, 00000000), 1, B8(00000000), MC68000_MODE_ALL },
  { "SUBX.W", B16(10010001, 01000000), 1, B8(00000001), MC68000_MODE_ALL },
  { "SUBX.L", B16(10010001, 10000000), 1, B8(00000010), MC68000_MODE_ALL },

  { "SWAP", B16(01001000, 01000000), 20, B8(00000010), MC68000_MODE_ALL },

  { "TAS", B16(01001010, 11000000), 8, B8(00000000), MC68000_MODE_ALL },

  { "TRAP", B16(01001110, 01000000), 21, B8(00000000), MC68000_MODE_ALL },
  
  /* 4E 76 */
  { "TRAPV", B16(01001110, 01110110), 0, 0, MC68000_MODE_ALL },

  { "TST", B16(01001010, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "TST.B", B16(01001010, 00000000), 8, B8(00000000), MC68000_MODE_ALL },
  { "TST.W", B16(01001010, 01000000), 8, B8(00000001), MC68000_MODE_ALL },
  { "TST.L", B16(01001010, 10000000), 8, B8(00000010), MC68000_MODE_ALL },

  { "UNLK", B16(01001110, 01011000), 22, B8(00000010), MC68000_MODE_ALL },
  
  { "E", 0x100, 0xFF, 0, MC68000_MODE_ALL },
};

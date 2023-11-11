
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "defines.h"

#include "stack.h"
#include "phase_1.h"
#include "parse.h"
#include "printf.h"
#include "main.h"


extern FILE *g_file_out_ptr;
extern char *g_buffer, *unfolded_buffer, g_label[MAX_NAME_LENGTH + 1], *g_include_dir, *g_full_name, *g_tmp;
extern int g_in_enum, g_in_ramsection, g_in_struct, g_macro_id, g_sizeof_g_tmp, g_fail_quetly_on_non_found_functions;
extern int g_ss, g_source_index, g_parsed_int, g_section_status, g_org_defined, g_bankheader_status, g_macro_active;
extern int g_source_file_size, g_input_number_error_msg, g_verbose_level, g_output_format, g_open_files;
extern int g_last_stack_id, g_latest_stack, g_ss, g_commandline_parsing, g_newline_beginning, g_expect_calculations, g_input_parse_special_chars;
extern int g_instruction_n[256], g_instruction_p[256], g_dsp_enable_label_address_conversion;
extern int g_extra_definitions, g_string_size, g_input_float_mode, g_operand_hint, g_operand_hint_type, g_parse_floats;
extern struct instruction g_instructions_table[];
extern struct macro_runtime *g_macro_stack, *g_macro_runtime_current;
extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;

#if defined(MCS6502) || defined(WDC65C02) || defined(CSG65CE02) || defined(W65816) || defined(HUC6280) || defined(MC6800) || defined(MC6801) || defined(MC6809)
extern int g_xbit_size, g_accu_size, g_index_size;
#endif

#if defined(W65816)
extern int g_use_wdc_standard;
#endif

#if defined(SPC700)
extern int g_input_number_expects_dot;
#endif

static struct instruction *s_instruction_tmp;
static int s_parser_source_index;

#define IS_THE_MATCH_COMPLETE(x) (s_instruction_tmp->string[x] == 0 && (g_buffer[s_parser_source_index] == 0x0A || g_buffer[s_parser_source_index] == '\\' || (g_buffer[s_parser_source_index] == ' ' && g_buffer[s_parser_source_index+1] == '\\')))


static void _output_assembled_instruction(struct instruction *instruction, const char *format, ...) {

  va_list ap;
  
  if (instruction == NULL)
    return;
  
  va_start(ap, format);

  vfprintf(g_file_out_ptr, format, ap);
#if defined(WLA_DEBUG)
  {
    char ttt[256];

    va_end(ap);
    va_start(ap, format);
    vsnprintf(ttt, sizeof(ttt), format, ap);
    print_text(YES, "LINE %5d: INSTRUCTION: %16s ::: %s\n", g_active_file_info_last->line_current, instruction->string, ttt);
  }
#endif

  va_end(ap);
}


#if MC6809

static char s_error_no_u[] = "Was expecting register X/Y/S/PC/A/B/CC/DP";
static char s_error_no_s[] = "Was expecting register X/Y/U/PC/A/B/CC/DP";

static int _parse_push_pull_registers(int accept_u) {

  int register_byte = 0, reg, y, z, prev_i;

  while (1) {
    y = g_source_index;
    g_source_index = s_parser_source_index;
    prev_i = g_source_index;
    z = input_number();
    s_parser_source_index = g_source_index;
    g_source_index = y;

    if (z == INPUT_NUMBER_EOL) {
      /* roll back to the index before EOL */
      s_parser_source_index = prev_i;
      break;
    }
    
    if (z != INPUT_NUMBER_ADDRESS_LABEL) {
      if (accept_u == 1)
        print_error(ERROR_ERR, "%s, got something else instead.\n", s_error_no_s);
      else
        print_error(ERROR_ERR, "%s, got something else instead.\n", s_error_no_u);
      return -1;
    }

    if (strcaselesscmp(g_label, "X") == 0)
      reg = 1 << 4;
    else if (strcaselesscmp(g_label, "Y") == 0)
      reg = 1 << 5;
    else if (strcaselesscmp(g_label, "U") == 0) {
      if (accept_u == 0) {
        print_error(ERROR_ERR, "%s, got \"%s\" instead.\n", s_error_no_u, g_label);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(g_label, "S") == 0) {
      if (accept_u == 1) {
        print_error(ERROR_ERR, "%s, got \"%s\" instead.\n", s_error_no_s, g_label);
        return -1;
      }
      reg = 1 << 6;
    }
    else if (strcaselesscmp(g_label, "PC") == 0)
      reg = 1 << 7;
    else if (strcaselesscmp(g_label, "A") == 0)
      reg = 1 << 1;
    else if (strcaselesscmp(g_label, "B") == 0)
      reg = 1 << 2;
    else if (strcaselesscmp(g_label, "CC") == 0)
      reg = 1 << 0;
    else if (strcaselesscmp(g_label, "DP") == 0)
      reg = 1 << 3;
    else {
      if (accept_u == 1)
        print_error(ERROR_ERR, "%s, got \"%s\" instead.\n", s_error_no_s, g_label);
      else
        print_error(ERROR_ERR, "%s, got \"%s\" instead.\n", s_error_no_u, g_label);
      return -1;
    }

    if ((register_byte & reg) != 0) {
      print_error(ERROR_ERR, "Register \"%s\" was already defined.\n", g_label);
      return -1;
    }

    register_byte |= reg;
  }

  if (register_byte == 0) {
    if (accept_u == 1)
      print_error(ERROR_ERR, "%s, got nothing instead.\n", s_error_no_s);
    else
      print_error(ERROR_ERR, "%s, got nothing instead.\n", s_error_no_u);

    return -1;
  }
  
  return register_byte;
}


static int _get_register_byte_from_label_exg_tfr(void) {

  /* 16-bit */
  if (strcaselesscmp(g_label, "D") == 0)
    return 0;
  if (strcaselesscmp(g_label, "X") == 0)
    return 1;
  if (strcaselesscmp(g_label, "Y") == 0)
    return 2;
  if (strcaselesscmp(g_label, "U") == 0)
    return 3;
  if (strcaselesscmp(g_label, "S") == 0)
    return 4;
  if (strcaselesscmp(g_label, "PC") == 0)
    return 5;

  /* 8-bit */
  if (strcaselesscmp(g_label, "A") == 0)
    return 8;
  if (strcaselesscmp(g_label, "B") == 0)
    return 9;
  if (strcaselesscmp(g_label, "CC") == 0)
    return 0xA;
  if (strcaselesscmp(g_label, "DP") == 0)
    return 0xB;

  print_error(ERROR_ERR, "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got \"%s\" instead.\n", g_label);

  return -1;
}


static int _parse_exg_tfr_registers(void) {

  int register_byte, data, y, z;

  /* source register */
  y = g_source_index;
  g_source_index = s_parser_source_index;
  z = input_number();
  s_parser_source_index = g_source_index;
  g_source_index = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_ERR, "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n");
    return -1;
  }

  data = _get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;
  register_byte = data;

  /* destination register */
  y = g_source_index;
  g_source_index = s_parser_source_index;
  z = input_number();
  s_parser_source_index = g_source_index;
  g_source_index = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_ERR, "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n");
    return -1;
  }

  data = _get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;

  /* are we mixing 16-bit and 8-bit registers? */
  if ((register_byte <= 5 && data > 5) || (register_byte > 5 && data <= 5)) {
    print_error(ERROR_ERR, "Mixing of 8-bit and 16-bit registers is not allowed here.\n");
    return -1;    
  }
  
  register_byte = (register_byte << 4) | data;
  
  return register_byte;
}

#endif


#if defined(SUPERFX)

/* parse a number [min, max] */
int _parse_tiny_int(int min, int max) {

  int old_i, value, res;
  
  old_i = g_source_index;
  g_source_index = s_parser_source_index;
  res = input_number();
  s_parser_source_index = g_source_index;
  g_source_index = old_i;

  if (res != SUCCEEDED)
    return -1;

  value = g_parsed_int;

  if (value < min || value > max)
    return -1;
  
  return value;
}

#endif


#if defined(MC68000)

static int _mc68000_size_check(int data, int size) {

  if (size == B8(00000000)) {
    /* .b */
    if (data > 255 || data < -128) {
      print_error(ERROR_NUM, "Out of 8-bit range.\n");
      return FAILED;
    }
  }
  else if (size == B8(00000001)) {
    /* .w */
    if (data > 65535 || data < -32768) {
      print_error(ERROR_NUM, "Out of 16-bit range.\n");
      return FAILED;
    }
  }

  return SUCCEEDED;
}


static void _mc68000_emit_extra_data(int mode, int reg1, int reg2, int data_type, int data, char *label, int size) {

  if (mode == B8(00000111) && reg1 == B8(00000100)) {
    /* #<data> */
    if (size == B8(00000000)) {
      /* .b */
      if (data_type == SUCCEEDED)
        _output_assembled_instruction(s_instruction_tmp, "d0 d%d ", data);
      else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
        _output_assembled_instruction(s_instruction_tmp, "k%d d0 R%s ", g_active_file_info_last->line_current, label);
      else
        _output_assembled_instruction(s_instruction_tmp, "d0 c%d ", data);
    }
    else if (size == B8(00000001)) {
      /* .w */
      if (data_type == SUCCEEDED)
        _output_assembled_instruction(s_instruction_tmp, "y%d ", data);
      else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
        _output_assembled_instruction(s_instruction_tmp, "k%d r%s ", g_active_file_info_last->line_current, label);
      else
        _output_assembled_instruction(s_instruction_tmp, "C%d ", data);
    }
    else {
      /* .l */
      if (data_type == SUCCEEDED)
        _output_assembled_instruction(s_instruction_tmp, "u%d ", data);
      else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
        _output_assembled_instruction(s_instruction_tmp, "k%d V%s ", g_active_file_info_last->line_current, label);
      else
        _output_assembled_instruction(s_instruction_tmp, "U%d ", data);
    }
  }
  else if (mode == B8(00000101) || (mode == B8(00000111) && reg1 == B8(00000000))) {
    /* 16-bit */
    if (data_type == SUCCEEDED)
      _output_assembled_instruction(s_instruction_tmp, "y%d ", data);
    else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
      _output_assembled_instruction(s_instruction_tmp, "k%d r%s ", g_active_file_info_last->line_current, label);
    else
      _output_assembled_instruction(s_instruction_tmp, "C%d ", data);
  }
  else if (mode == B8(00000111) && reg1 == B8(00000010)) {
    /* (d16,PC) */
    stack_create_stack_caddr_offset(data_type, data, label);
    _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
  }
  else if ((mode == B8(00000111) && reg1 == B8(00000001)) || (mode == B8(00000111) && reg1 == B8(00000100))) {
    /* 32-bit */
    if (data_type == SUCCEEDED)
      _output_assembled_instruction(s_instruction_tmp, "u%d ", data);
    else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
      _output_assembled_instruction(s_instruction_tmp, "k%d V%s ", g_active_file_info_last->line_current, label);
    else
      _output_assembled_instruction(s_instruction_tmp, "U%d ", data);
  }
  else if (mode == B8(00000111) && reg1 == B8(00000011)) {
    /* (d8,Dn,PC) */
    if (data_type == SUCCEEDED)
      _output_assembled_instruction(s_instruction_tmp, "d%d ", reg2 << 4);
    else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
      _output_assembled_instruction(s_instruction_tmp, "k%d d%d ", g_active_file_info_last->line_current, reg2 << 4);
    else
      _output_assembled_instruction(s_instruction_tmp, "d%d ", reg2 << 4);

    stack_create_stack_caddr_offset_plus_n(data_type, data, label, 1);
    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
  }
  else if (mode == B8(00000110)) {
    /* (d8,An,Dn) */
    if (data_type == SUCCEEDED)
      _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", reg2 << 4, data);
    else if (data_type == INPUT_NUMBER_ADDRESS_LABEL)
      _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, reg2 << 4, label);
    else
      _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", reg2 << 4, data);
  }
}


static int _mc68000_reverse_register_list(int list) {

  int i, reversed = 0;

  for (i = 0; i < 16; i++) {
    if ((list & (1 << i)) != 0)
      reversed |= 1 << (15 - i);
  }

  return reversed;
}


/*
   mode  reg1  reg2  addressing mode
   000 - nnn - ??? - Dn
   001 - nnn - ??? - An
   010 - nnn - ??? - (An)
   011 - nnn - ??? - (An)+
   100 - nnn - ??? - -(An)
   101 - nnn - ??? - (d16,An) / d16(An)
   110 - nnn - mmm - (d8,An,Dm)
   111 - 000 - ??? - (xxx).W
   111 - 001 - ??? - (xxx).L
   111 - 100 - ??? - #<data>
   111 - 010 - ??? - (d16,PC) / d16(PC)
   111 - 011 - nnn - (d8,PC,Dn)
  1000 - ??? - ??? - CCR
  1001 - ??? - ??? - SR
  1010 - ??? - ??? - USP
  1011 - lst - ??? - register list, a7-a0/d7-d0
*/

static int _mc68000_parse_ea(char *code, int *index, int *reg1, int *reg2, int *mode, int *data, int *data_type, char *label, unsigned int extras) {

  int i = *index, saved_i, predecrement = NO, postincrement = NO, y, z;
  char c;

  *data_type = FAILED;

  if (code[i] == ' ')
    i++;
  
  if (code[i] == '-') {
    predecrement = YES;
    i++;
  }

  saved_i = i;
  c = toupper((int)code[i]);

  if (extras == YES) {
    if (c == 'C' && toupper((int)code[i+1]) == 'C' && toupper((int)code[i+2]) == 'R' && (code[i+3] == 0xA || (code[i+3] == ' ' && code[i+4] == '\\'))) {
      i += 3;

      *mode = B8(00001000);
      *index = i;

      return SUCCEEDED;
    }
    else if (c == 'S' && toupper((int)code[i+1]) == 'R' && (code[i+2] == 0xA || code[i+2] == ',' || (code[i+2] == ' ' && code[i+3] == '\\'))) {
      i += 2;

      *mode = B8(00001001);
      *index = i;

      return SUCCEEDED;
    }
    else if (c == 'U' && toupper((int)code[i+1]) == 'S' && toupper((int)code[i+2]) == 'P' && (code[i+3] == 0xA || code[i+3] == ',' || (code[i+3] == ' ' && code[i+4] == '\\'))) {
      i += 3;

      *mode = B8(00001010);
      *index = i;

      return SUCCEEDED;
    }
    else if ((c == 'A' || c == 'D') && (code[i+1] >= '0' && code[i+1] <= '7')) {
      /* try to parse a register list, could be a single register though */
      int old_i = i, registers = 0, list = 0, success = YES, number, c2, number2;

      while (1) {
        if (code[i] == 0xA || code[i] == ',' || (code[i] == ' ' && code[i+1] == '\\'))
          break;

        c = toupper((int)code[i]);
        i++;
        if (code[i] >= '0' && code[i] <= '7')
          number = code[i++] - '0';
        else {
          success = NO;
          break;
        }

        if (code[i] == '/' || code[i] == 0xA || code[i] == ',' || (code[i] == ' ' && code[i+1] == '\\')) {
          /* add this register to the list */
          if (c == 'A') {
            if ((list & (1 << (8 + number))) == 0) {
              registers++;
              list |= (1 << (8 + number));
            }
          }
          else if (c == 'D') {
            if ((list & (1 << number)) == 0) {
              registers++;
              list |= 1 << number;
            }
          }
          else {
            success = NO;
            break;
          }

          if (code[i] == '/')
            i++;
        }
        else if (code[i] == '-') {
          /* it's a range! */
          i++;
          
          c2 = toupper((int)code[i]);
          i++;

          if (c2 != c) {
            /* different register type */
            print_error(ERROR_NUM, "Unsupported register range.\n");
            return FAILED;
          }

          if (code[i] >= '0' && code[i] <= '7')
            number2 = code[i++] - '0';
          else {
            print_error(ERROR_NUM, "Malformed register range.\n");
            return FAILED;
          }

          if (number2 < number) {
            /* make number to be less than number2 */
            c2 = number;
            number = number2;
            number2 = c2;
          }

          /* add registers to the list */
          while (number <= number2) {
            if (c == 'A') {
              if ((list & (1 << (8 + number))) == 0) {
                registers++;
                list |= (1 << (8 + number));
              }
            }
            else if (c == 'D') {
              if ((list & (1 << number)) == 0) {
                registers++;
                list |= 1 << number;
              }
            }
            else {
              success = NO;
              break;
            }
            
            number++;
          }

          if (code[i] == '/')
            i++;
        }
        else {
          success = NO;
          break;
        }
      }
      
      /* did we get a single register? */
      if (registers <= 1 || success == NO) {
        /* yes, break and continue */
        i = old_i;

        c = toupper((int)code[i]);
      }
      else {
        /* no, we got a list! */
        *reg1 = list;
        *mode = B8(00001011);
        *index = i;

        return SUCCEEDED;
      }
    }
  }

  if (c == 'A' || c == 'D') {
    /* An OR Dn */
    if (predecrement == YES)
      return FAILED;
    
    i++;
    if (code[i] >= '0' && code[i] <= '7') {
      *reg1 = code[i] - '0';

      i++;
      if (code[i] == 0xA || code[i] == ',' || (code[i] == ' ' && code[i+1] == '\\')) {
        if (c == 'A')
          *mode = B8(00000001);
        else
          *mode = B8(00000000);

        *index = i;
      
        return SUCCEEDED;
      }
    }
  }
  else if (c == '#') {
    /* #<data> */
    if (predecrement == YES)
      return FAILED;
    
    i++;

    y = g_source_index;
    g_source_index = i;
    z = input_number();
    i = g_source_index;
    g_source_index = y;

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    *data_type = z;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(label, g_label);
    else if (z == INPUT_NUMBER_STACK)
      *data = g_latest_stack;
    else
      *data = g_parsed_int;

    *mode = B8(00000111);
    *reg1 = B8(00000100);
    
    *index = i;

    return SUCCEEDED;    
  }

  i = saved_i;
  
  if (c == '(') {
    /* (An), (d16,An) etc? */
    i++;

    c = toupper((int)code[i]);

    /* An? */
    if (c == 'A') {
      i++;
      if (code[i] >= '0' && code[i] <= '7') {
        *reg1 = code[i] - '0';

        i++;
        if (code[i] == ')') {
          i++;

          if (code[i] == '+') {
            if (predecrement == YES)
              return FAILED;
            postincrement = YES;
            i++;
          }

          if (code[i] != 0xA && code[i] != ',' && code[i] != '\\' && (!(code[i] == ' ' && code[i+1] == '\\')))
            return FAILED;

          if (predecrement == YES)
            *mode = B8(00000100);
          else if (postincrement == YES)
            *mode = B8(00000011);
          else
            *mode = B8(00000010);

          *index = i;
      
          return SUCCEEDED;
        }
      }
    }
    
    /* d8 OR d16 here? */
    y = g_source_index;
    g_source_index = i;
    z = input_number();
    i = g_source_index;
    g_source_index = y;
    
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    /* we could have one of the following addressing modes here
       101 - nnn - ??? - (d16,An)
       110 - nnn - mmm - (d8,An,Dm)
       111 - 010 - ??? - (d16,PC)
       111 - 011 - nnn - (d8,PC,Dn) */

    *data_type = z;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(label, g_label);
    else if (z == INPUT_NUMBER_STACK)
      *data = g_latest_stack;
    else
      *data = g_parsed_int;

    if (code[i] == ',') {
      i++;
      
      c = toupper((int)code[i]);

      if (c == 'A') {
        /* (d16,An) OR (d8,An,Dm) */
        if (predecrement == YES)
          return FAILED;

        i++;
        if (code[i] >= '0' && code[i] <= '7') {
          *reg1 = code[i] - '0';

          i++;
          if (code[i] == ')') {
            i++;
            
            if (z == SUCCEEDED && (*data > 32767 || *data < -32768)) {
              print_error(ERROR_NUM, "Out of signed 16-bit range.\n");
              return FAILED;
            }

            *mode = B8(00000101);
            
            *index = i;
      
            return SUCCEEDED;
          }
          else if (code[i] == ',') {
            i++;

            c = toupper((int)code[i]);
            
            if (c == 'D') {
              /* (d8,An,Dm) */
              i++;
              if (code[i] >= '0' && code[i] <= '7') {
                *mode = B8(00000110);
                *reg2 = code[i] - '0';

                i++;

                if (z == SUCCEEDED && (*data > 127 || *data < -128)) {
                  print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
                  return FAILED;
                }
                
                if (code[i] != ')')
                  return FAILED;

                i++;
                *index = i;

                return SUCCEEDED;
              }
            }
          }
        }
      }
      else if (c == 'P') {
        /* (d16,PC) OR (d8,PC,Dn) */
        i++;
        
        if (predecrement == YES)
          return FAILED;
        
        c = toupper((int)code[i]);

        if (c == 'C') {
          i++;

          if (code[i] == ')') {
            i++;
            
            if (z == SUCCEEDED && (*data > 32767 || *data < -32768)) {
              print_error(ERROR_NUM, "Out of signed 16-bit range.\n");
              return FAILED;
            }

            *mode = B8(00000111);
            *reg1 = B8(00000010);

            *index = i;
      
            return SUCCEEDED;
          }
          else if (code[i] == ',') {
            i++;

            c = toupper((int)code[i]);            
          
            if (c == 'D') {
              /* (d8,PC,Dn) */
              i++;
              if (code[i] >= '0' && code[i] <= '7') {
                *reg2 = code[i] - '0';

                i++;
                if (code[i] == ')') {
                  i++;
            
                  if (z == SUCCEEDED && (*data > 127 || *data < -128)) {
                    print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
                    return FAILED;
                  }

                  *mode = B8(00000111);
                  *reg1 = B8(00000011);

                  *index = i;
      
                  return SUCCEEDED;
                }
              }
            }
          }
        }
      }
    }
  }

  i = saved_i;

  /* what remains is
     101 - nnn - ??? - d16(An)
     111 - 000 - ??? - (xxx).W
     111 - 001 - ??? - (xxx).L
     111 - 010 - ??? - d16(PC) */

  y = g_source_index;
  g_source_index = i;
  g_fail_quetly_on_non_found_functions = YES;
  z = input_number();
  g_fail_quetly_on_non_found_functions = NO;
  i = g_source_index;
  g_source_index = y;

  if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
    return FAILED;

  *data_type = z;
  if (z == INPUT_NUMBER_ADDRESS_LABEL)
    strcpy(label, g_label);
  else if (z == INPUT_NUMBER_STACK)
    *data = g_latest_stack;
  else
    *data = g_parsed_int;

  if (code[i] == 0xA || code[i] == ',' || code[i] == '\\' || (code[i] == ' ' && code[i+1] == '\\')) {
    /* we have
       111 - 000 - ??? - (xxx).W
       111 - 001 - ??? - (xxx).L */
    *mode = B8(00000111);

    if (g_operand_hint == HINT_16BIT)
      *reg1 = B8(00000000);
    else
      *reg1 = B8(00000001);

    *index = i;

    return SUCCEEDED;    
  }
  else if (code[i] == '(') {
    /* we have
     101 - nnn - ??? - d16(An)
     111 - 010 - ??? - d16(PC) */    
    i++;

    if (z == SUCCEEDED && (*data > 32767 || *data < -32768)) {
      print_error(ERROR_NUM, "Out of signed 16-bit range.\n");
      return FAILED;
    }

    c = toupper((int)code[i]);

    if (c == 'P') {
      /* d16(PC) */    
      i++;

      c = toupper((int)code[i]);

      if (c == 'C') {
        i++;
        if (code[i] == ')') {
          i++;
            
          *mode = B8(00000111);
          *reg1 = B8(00000010);

          *index = i;
          
          return SUCCEEDED;
        }
      }
    }
    else if (c == 'A') {
      /* d16(An) */
      i++;
      if (code[i] >= '0' && code[i] <= '7') {
        *mode = B8(00000101);
        *reg1 = code[i] - '0';

        i++;
                
        if (code[i] != ')')
          return FAILED;
        
        i++;
        *index = i;

        return SUCCEEDED;
      }
    }    
  }

  return FAILED;
}


/*
   mode  reg   addressing mode
   000 - nnn - Dn
   001 - nnn - An
   010 - nnn - (An)
   011 - nnn - (An)+
   100 - nnn - -(An)
*/

static int _mc68000_parse_register(char *code, int *index, int *reg, int *mode) {

  int i = *index, predecrement = NO, postincrement = NO;
  char c;

  if (code[i] == ' ')
    i++;

  if (code[i] == '-') {
    predecrement = YES;
    i++;
  }

  c = toupper((int)code[i]);

  if (c == 'A' || c == 'D') {
    if (predecrement == YES)
      return FAILED;
    
    i++;
    if (code[i] >= '0' && code[i] <= '7') {
      *reg = code[i] - '0';

      i++;
      if (code[i] != 0xA && code[i] != ',' && code[i] != '\\' && (!(code[i] == ' ' && code[i+1] == '\\')))
        return FAILED;

      if (c == 'A')
        *mode = B8(00000001);
      else
        *mode = B8(00000000);
    }
  }
  else if (c == '(') {
    i++;

    c = toupper((int)code[i]);

    if (c == 'A') {
      i++;
      if (code[i] >= '0' && code[i] <= '7') {
        *reg = code[i] - '0';

        i++;
        if (code[i] != ')')
          return FAILED;
        i++;

        if (code[i] == '+') {
          if (predecrement == YES)
            return FAILED;
          postincrement = YES;
          i++;
        }

        if (code[i] != 0xA && code[i] != ',' && code[i] != '\\' && (!(code[i] == ' ' && code[i+1] == '\\')))
          return FAILED;

        if (predecrement == YES)
          *mode = B8(00000100);
        else if (postincrement == YES)
          *mode = B8(00000011);
        else
          *mode = B8(00000010);
      }
      else
        return FAILED;
    }
    else
      return FAILED;
  }
  else
    return FAILED;

  *index = i;
      
  return SUCCEEDED;
}

#endif


int evaluate_token(void) {

  int f, x, y, last_stack_id_backup, instruction_i;
#if defined(Z80) || defined(Z80N) || defined(SPC700) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280)
  int e, v, h;
  char labelx[MAX_NAME_LENGTH + 1];
#endif
#if defined(SPC700)
  int g;
#endif
#if !defined(MC68000)
  int z;
#endif
#if defined(HUC6280)
  int r = 0, s, t = 0, u = 0;
  char labely[MAX_NAME_LENGTH + 1];
#endif
#if defined(SUPERFX)
  int tiny;
#endif
  
  /* are we in an enum, ramsection, or struct? */
  if (g_in_enum == YES || g_in_ramsection == YES || g_in_struct == YES)
    return parse_enum_token();

  /* is it a directive? */
  if (g_tmp[0] == '.') {
    x = parse_directive();
    if (x != DIRECTIVE_NOT_IDENTIFIED)
      return x;

    /* allow error messages from input_numbers() */
    g_input_number_error_msg = YES;

    return EVALUATE_TOKEN_NOT_IDENTIFIED;
  }

  /* is it a label? */
  if (g_tmp[g_ss - 1] == ':' && g_newline_beginning == ON) {
    g_tmp[g_ss - 1] = 0;
    g_newline_beginning = OFF;

    if (g_output_format == OUTPUT_LIBRARY && g_section_status == OFF) {
      print_error(ERROR_LOG, "All labels must be inside sections when compiling a library.\n");
      return FAILED;
    }
    if (g_org_defined == 0) {
      print_error(ERROR_LOG, "\"%s\" needs a position in memory.\n", g_tmp);
      return FAILED;
    }
    if (g_ss >= MAX_NAME_LENGTH) {
      print_error(ERROR_NONE, "The label \"%s\" is too long. Max label length is %d characters.\n", g_tmp, MAX_NAME_LENGTH);
      return FAILED;
    }
    if (g_bankheader_status == ON) {
      print_error(ERROR_LOG, "BANKHEADER sections don't take labels.\n");
      return FAILED;
    }

    /* check for \@-symbols */
    if (g_macro_active != 0) {
      if (g_tmp[g_ss - 3] == '\\' && g_tmp[g_ss - 2] == '@')
        snprintf(&g_tmp[g_ss - 3], g_sizeof_g_tmp - (g_ss - 3), "%d", g_macro_runtime_current->macro->calls - 1);
    }

    if (g_macro_active != 0) {
      if (should_we_add_namespace() == YES) {
        if (add_namespace_to_a_label(g_tmp, g_sizeof_g_tmp, YES) == FAILED)
          return FAILED;
      }
    }

    if (add_label_to_label_stack(g_tmp) == FAILED)
      return FAILED;
    
    fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, g_tmp);

    return SUCCEEDED;
  }

  /* INSTRUCTION? */
  instruction_i = g_instruction_p[(unsigned char)g_tmp[0]];
  s_instruction_tmp = &g_instructions_table[instruction_i];

  /* here we need to postpone all labels to the linker as the assembler currently doesn't
     handle e.g., relative addresses at this stage */
  g_dsp_enable_label_address_conversion = NO;
  
  for (f = g_instruction_n[(unsigned char)g_tmp[0]]; f > 0; f--) {
    int result;
    
#if W65816
    if (g_use_wdc_standard == 0) {
      /* skip all instructions that contain '<', '|' and '>' */
      for (x = 0, result = SUCCEEDED; x < INSTRUCTION_STRING_LENGTH_MAX; x++) {
        char c = s_instruction_tmp->string[x];

        if (c == 0)
          break;
        if (c == '<' || c == '|' || c == '>') {
          result = FAILED;
          break;
        }
      }

      if (result == FAILED) {
        /* try the next instruction in the array */
        s_instruction_tmp = &g_instructions_table[++instruction_i];
        continue;
      }
    }
#endif
    
    /* try to match the first part of the instruction, already read into g_tmp */
    for (x = 0, result = SUCCEEDED; x < INSTRUCTION_STRING_LENGTH_MAX; x++) {
      if (g_tmp[x] == 0)
        break;
      if (s_instruction_tmp->string[x] != toupper((int)g_tmp[x])) {
        result = FAILED;
        break;
      }
    }

    if (result == FAILED) {
      /* try the next instruction in the array */
      s_instruction_tmp = &g_instructions_table[++instruction_i];
      continue;
    }

    /* beginning of the instruction matches what we have in the source code */
    s_parser_source_index = g_source_index;

    /* remember the last stack calculation created -> if we create new stack calculations in the
       following switch() and need to roll back after that we'll delete all stack calculations created
       after this stack id... */
    last_stack_id_backup = g_last_stack_id;

#if defined(SPC700)
    /* does the instruction contain a dot? */
    /* NOTE: as instruction decoders call return, we'll need to set this variable later back to NO */
    g_input_number_expects_dot = s_instruction_tmp->has_dot;
#endif

    switch (s_instruction_tmp->type) {

#if defined(GB)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <GB-Z80> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;

          /* give a warning when assembling "JP (HL)"! */
          /*
            if (s_instruction_tmp->hex == 0xE9 && strcmp(s_instruction_tmp->string, "JP (HL)") == 0)
            print_error(ERROR_WRN, "\"JP (HL)\" is semantically incorrect. Please use \"JP HL\" instead.\n");
          */

          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          if (x > 0 && (s_instruction_tmp->string[x-1] == '+' || s_instruction_tmp->string[x-1] == '-'))
            g_source_index--;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
          
                if (s_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d v1 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);       
              
              /* reset to "no special case" */
              _output_assembled_instruction(s_instruction_tmp, "v0 ");
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </GB-Z80> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif
 
#if defined(Z80) || defined(Z80N)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <Z80/Z80N> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          
          /* give a warning when assembling "JP (HL)"! */
          /*
            if (s_instruction_tmp->hex == 0xE9 && strcmp(s_instruction_tmp->string, "JP (HL)") == 0)
            print_error(ERROR_WRN, "\"JP (HL)\" is semantically incorrect. Please use \"JP HL\" instead.\n");
          */
          /* give a warning when assembling "JP (HL')"! */
          /*
            if (s_instruction_tmp->hex == 0xE9 && strcmp(s_instruction_tmp->string, "JP (HL')") == 0)
            print_error(ERROR_WRN, "\"JP (HL')\" is semantically incorrect. Please use \"JP HL'\" instead.\n");
          */
          
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 11:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 11) {
                  /* 11 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          
          /* give a warning when assembling "JP (IX)"! */
          /*
            if (s_instruction_tmp->hex == 0xE9DD && strcmp(s_instruction_tmp->string, "JP (IX)") == 0)
            print_error(ERROR_WRN, "\"JP (IX)\" is semantically incorrect. Please use \"JP IX\" instead.\n");
          */
          /* give a warning when assembling "JP (IY)"! */
          /*
            if (s_instruction_tmp->hex == 0xE9FD && strcmp(s_instruction_tmp->string, "JP (IY)") == 0)
            print_error(ERROR_WRN, "\"JP (IY)\" is semantically incorrect. Please use \"JP IY\" instead.\n");
          */
          
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "y%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d y%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "y%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "y%d d%d d%d ", s_instruction_tmp->hex, g_parsed_int, s_instruction_tmp->hex_x);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label, s_instruction_tmp->hex_x);
              else
                _output_assembled_instruction(s_instruction_tmp, "y%d c%d d%d ", s_instruction_tmp->hex, g_latest_stack, s_instruction_tmp->hex_x);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              /* "PUSH ?" hack */
              if (s_instruction_tmp->hex == 0x8AED) {
                /* high bytes comes first */
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "y%d d%d d%d ", s_instruction_tmp->hex, ((g_parsed_int) >> 8) & 0xFF, g_parsed_int & 0xFF);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d y%d . r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else {
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
                  stack->special_id = 4;

                  _output_assembled_instruction(s_instruction_tmp, "y%d C%d ", s_instruction_tmp->hex, g_latest_stack);
                }
              }
              else {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "y%d y%d ", s_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d y%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(s_instruction_tmp, "y%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "y%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "y%d R%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "y%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 10:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              
              if (z == SUCCEEDED) {
                if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                    (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
                  print_error(ERROR_NUM, "Out of 8-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "y%d d%d d%d ", s_instruction_tmp->hex, g_parsed_int, s_instruction_tmp->hex_x);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label, s_instruction_tmp->hex_x);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "y%d c%d d%d ", s_instruction_tmp->hex, g_latest_stack, s_instruction_tmp->hex_x);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d v1 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);       
              
              /* reset to "no special case" */
              _output_assembled_instruction(s_instruction_tmp, "v0 ");
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </Z80/Z80N> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif

#if defined(MCS6502)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6502> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 5:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
            if (s_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </6502> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(WDC65C02)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65C02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 6:
    case 1:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 6) {
                  /* 6 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </65C02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(CSG65CE02)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65CE02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 6:
    case 1:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 6) {
                  /* 6 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 7:
      /* 16-bit relative */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 32767 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of signed 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d M%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                struct stack *stack;
                
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
                
                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);
                
                if (stack == NULL)
                  return FAILED;
                
                stack->relative_references = 1;
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </65CE02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(W65816)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65816> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 11:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 11) {
                  /* 11 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      if (g_xbit_size > 16 && s_instruction_tmp->skip_xbit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_24BIT && s_instruction_tmp->skip_xbit == 1)
            break;
          if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
            if (s_instruction_tmp->skip_xbit == 1)
              break;
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '&') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_STACK || z == INPUT_NUMBER_ADDRESS_LABEL))
            return FAILED;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d z%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d T%d ", s_instruction_tmp->hex, g_latest_stack);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /* IMMEDIATE VALUES: ACCUMULATOR */

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;

          /* select the mode */
          if (g_operand_hint_type == HINT_TYPE_GIVEN) {
            if (g_operand_hint == HINT_8BIT)
              y = 0;
            else
              y = 1;
          }
          else {
            if (g_accu_size == 8)
              y = 0;
            else
              y = 1;
          }
    
          if (y == 0) {
            if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          else {
            if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
              print_error(ERROR_NUM, "Out of 16-bit range.\n");
              return FAILED;
            }
          }
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              /* 8BIT */
              if (y == 0) {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              }
              /* 16BIT */
              else {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /* MVN & MVP */

    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  /* NOTE: in the source code it's "MVP/MVN x1, x2", but we output "MVP/MVN x2, x1" */
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
            
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /* SEP & REP */

    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED) {
            if (s_instruction_tmp->skip_xbit == 0)
              print_error(ERROR_LOG, "REP needs immediate data.\n");
            else
              print_error(ERROR_LOG, "SEP needs immediate data.\n");
            return FAILED;
          }
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
        
              _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
        
              /* REP */
              if (s_instruction_tmp->skip_xbit == 0) {
                if (g_parsed_int & 16)
                  g_index_size = 16;
                if (g_parsed_int & 32)
                  g_accu_size = 16;
              }
              /* SEP */
              else {
                if (g_parsed_int & 16)
                  g_index_size = 8;
                if (g_parsed_int & 32)
                  g_accu_size = 8;
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /* IMMEDIATE VALUES: INDEX */

    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;

          /* select the mode */
          if (g_operand_hint_type == HINT_TYPE_GIVEN) {
            if (g_operand_hint == HINT_8BIT)
              y = 0;
            else
              y = 1;
          }
          else {
            if (g_index_size == 8)
              y = 0;
            else
              y = 1;
          }
    
          if (y == 0) {
            if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          else {
            if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
              print_error(ERROR_NUM, "Out of 16-bit range.\n");
              return FAILED;
            }
          }
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              /* 8BIT */
              if (y == 0) {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              }
              /* 16BIT */
              else {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 32767))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d M%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xA:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_xbit == 2)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if ((g_operand_hint == HINT_16BIT || g_operand_hint == HINT_24BIT) && s_instruction_tmp->skip_xbit == 2)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
            if (s_instruction_tmp->skip_xbit == 2)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </65816> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(MC68000)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <68000> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 1:
      /* ABCD, SBCD, ADDX, SUBX */
      {
        int done = NO, register_y = 0, register_y_mode = 0, register_x = 0, register_x_mode = 0, opcode;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        /* "Dy, Dx" OR "-(Ay),-(Ax)" */
        if (_mc68000_parse_register(g_buffer, &s_parser_source_index, &register_y, &register_y_mode) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_register(g_buffer, &s_parser_source_index, &register_x, &register_x_mode) == FAILED)
          break;
        
        if (register_y_mode == B8(00000000) && register_x_mode == B8(00000000))
          opcode = s_instruction_tmp->hex | (register_x << 9) | register_y;
        else if (register_y_mode == B8(00000100) && register_x_mode == B8(00000100))
          opcode = s_instruction_tmp->hex | (register_x << 9) | (1 << 3) | register_y;
        else
          break;

        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);
        
        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 2:
      /* ADD, SUB */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO, mode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        mode = s_instruction_tmp->mode;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100)) {
          immediate = YES;

          /* is immediate value [1, 8]? */
          if (mode == MC68000_MODE_ALL && data_type_y == SUCCEEDED && data_y >= 1 && data_y <= 8) {
            mode = MC68000_MODE_Q;

            /* yes, switch add -> addq */
            if (opcode == B16(11010000, 00000000))
              opcode = B16(01010000, 00000000);
            /* sub -> subq */
            else if (opcode == B16(10010000, 00000000))
              opcode = B16(01010001, 00000000);            
          }
        }
        
        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }
          
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

	/* source or target must be Dn */
	if (immediate == NO && mode == MC68000_MODE_ALL && register_x_mode != B8(00000001)) {
	  if (register_x_mode != B8(00000000) && register_y_mode != B8(00000000)) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }
	}
        /* no .B with An */
        if (register_x_mode == B8(00000001) || register_y_mode == B8(00000001)) {
          if (size == B8(00000000)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* PC relative ea cannot be destination */
        if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
            (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate ea cannot be destination */
        if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate source expected? */
        if (mode == MC68000_MODE_I || mode == MC68000_MODE_Q) {
          if (immediate == NO) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* destination An? */
        if (mode == MC68000_MODE_A) {
          if (register_x_mode != B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* immediate source and destination An? */
        if (mode == MC68000_MODE_I) {
          if (register_x_mode == B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        
        /* choose the size and opmode */
        if (mode == MC68000_MODE_Q) {
          /* size */
          opcode |= size << 6;
        }
        else if (register_x_mode == B8(00000001)) {
          /* destination is An */
          if (size == B8(00000001)) {
            /* .w */
            opcode |= B8(00000011) << 6;
          }
          else {
            /* .l */
            opcode |= B8(00000111) << 6;
          }
        }
        else if (immediate == YES) {
          /* immediate source */

          /* switch add -> addi */
          if (opcode == B16(11010000, 00000000))
            opcode = B16(00000110, 00000000);
          /* sub -> subi */
          else if (opcode == B16(10010000, 00000000))
            opcode = B16(00000100, 00000000);
          
          /* size */
          opcode |= size << 6;
        }
        else if (register_x_mode == B8(00000000)) {
          /* destination is Dn */
          opcode |= size << 6;
        }
        else {
          /* destination is ea */
          opcode |= (size | (1 << 2)) << 6;
        }
        
        /* set register */
        if (mode == MC68000_MODE_Q) {
          /* actually here we encode the data (1-8) */
          if (data_type_y != SUCCEEDED) {
            print_error(ERROR_NUM, "The value [1, 8] must be known at this stage, it cannot be postponed to the linker.\n");
            return FAILED;
          }
          if (data_y < 1 || data_y > 8) {
            print_error(ERROR_NUM, "The value %d is out of range [1, 8].\n", data_y);
            return FAILED;
          }

          /* 8 here is encoded as 0! */
          if (data_y < 8)
            opcode |= data_y << 9;

          /* eat data_y so _mc68000_emit_extra_data() will not emit anything */
          register_y_mode = 0;
        }
        else if (register_x_mode == B8(00000001))
          opcode |= register_x1 << 9;
        else if (immediate == YES) {
          /* immediate */
        }
        else if (register_x_mode == B8(00000000))
          opcode |= register_x1 << 9;
        else
          opcode |= register_y1 << 9;
        
        /* set ea */
        if (mode == MC68000_MODE_Q) {
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }
        else if (register_x_mode == B8(00000001)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        else if (immediate == YES) {
          /* immediate source */
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }
        else if (register_x_mode == B8(00000000)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        else {
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 3:
      /* AND, ANDI, OR, ORI */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO, mode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        mode = s_instruction_tmp->mode;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100))
          immediate = YES;

        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }
          
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	if (immediate) {
	  if (register_x_mode == B8(00001000)) {
	    /* #<data>, CCR */

	    /* not .b? */
	    if (size != B8(00000000)) {
	      print_error(ERROR_NUM, "Invalid addressing mode.\n");
	      return FAILED;
	    }

	    /* andi */
	    if (opcode == B16(00000010, 00000000))
	      opcode = B16(00000010, 00111100);
            /* ori */
            else if (opcode == B16(00000000, 00000000))
	      opcode = B16(00000000, 00111100);
            
	    /* emit opcode */
	    _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	    /* emit extra data */
	    _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

	    g_source_index = s_parser_source_index;

	    return SUCCEEDED;
	  }
	  else if (register_x_mode == B8(00001001)) {
	    /* #<data>, SR */

	    /* not .w? */
	    if (size != B8(00000001)) {
	      print_error(ERROR_NUM, "Invalid addressing mode.\n");
	      return FAILED;
	    }

	    /* andi */
	    if (opcode == B16(00000010, 00000000))
	      opcode = B16(00000010, 01111100);
            /* ori */
	    else if (opcode == B16(00000000, 00000000))
	      opcode = B16(00000000, 01111100);
            
	    /* emit opcode */
	    _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	    /* emit extra data */
	    _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

	    g_source_index = s_parser_source_index;

	    return SUCCEEDED;
	  }
	}

	/* source or target must be Dn */
	if (immediate == NO) {
	  if (register_x_mode != B8(00000000) && register_y_mode != B8(00000000)) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }
	}
        /* no An */
        if (register_x_mode == B8(00000001) || register_y_mode == B8(00000001)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
        }
        /* PC relative ea cannot be destination */
        if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
            (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate ea cannot be destination */
        if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate source expected? */
        if (mode == MC68000_MODE_I) {
          if (immediate == NO) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        
        if (immediate == YES) {
          /* immediate source */

          /* switch and -> andi */
          if (opcode == B16(11000000, 00000000))
            opcode = B16(00000010, 00000000);
          /* switch or -> ori */
          else if (opcode == B16(10000000, 00000000))
            opcode = B16(00000000, 00000000);
          
          /* size */
          opcode |= size << 6;
        }
        else if (register_x_mode == B8(00000000)) {
          /* destination is Dn */
          opcode |= size << 6;
        }
        else {
          /* destination is ea */
          opcode |= (size | (1 << 2)) << 6;
        }
        
        /* set register */
        if (immediate == YES) {
          /* immediate */
        }
        else if (register_x_mode == B8(00000000))
          opcode |= register_x1 << 9;
        else
          opcode |= register_y1 << 9;
        
        /* set ea */
        if (immediate == YES) {
          /* immediate source */
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }
        else if (register_x_mode == B8(00000000)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        else {
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 4:
      /* ASR/ASL, LSR/LSL, ROR/ROL, ROXR/ROXL */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100))
          immediate = YES;
	else if (register_y_mode != B8(00000000)) {
	  /* <ea> */

	  /* needs to be .w */
	  if (size != B8(00000001)) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }
	  /* no An */
	  if (register_y_mode == B8(00000001)) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }
	  /* PC relative ea cannot be source */
	  if ((register_y_mode == B8(00000111) && register_y1 == B8(00000010)) ||
	      (register_y_mode == B8(00000111) && register_y1 == B8(00000011))) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }

          /* lsl/lsr? */
          if (opcode == B16(11100001, 00001000))
            opcode = B16(11100011, 00000000);
          else if (opcode == B16(11100000, 00001000))
            opcode = B16(11100010, 00000000);
          /* rol/ror? */
          else if (opcode == B16(11100001, 00011000))
            opcode = B16(11100111, 11000000);
          else if (opcode == B16(11100000, 00011000))
            opcode = B16(11100110, 11000000);
          /* roxl/roxr? */
          else if (opcode == B16(11100001, 00010000))
            opcode = B16(11100101, 11000000);
          else if (opcode == B16(11100000, 00010000))
            opcode = B16(11100100, 11000000);
          
	  /* size */
	  opcode |= B8(11000000);

	  /* set ea */
	  opcode |= register_y_mode << 3;
	  opcode |= register_y1;

	  /* emit opcode */
	  _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	  /* emit extra data */
	  _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

	  g_source_index = s_parser_source_index;

	  return SUCCEEDED;
	}

        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }
          
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	/* if source is not immediate then it must be Dn */
	if (immediate == NO && register_y_mode != B8(00000000)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}
	/* target must be Dn */
	if (register_x_mode != B8(00000000)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

	if (immediate == YES) {
          /* actually here we encode the data (1-8) */
          if (data_type_y != SUCCEEDED) {
            print_error(ERROR_NUM, "The value [1, 8] must be known at this stage, it cannot be postponed to the linker.\n");
            return FAILED;
          }
          if (data_y < 1 || data_y > 8) {
            print_error(ERROR_NUM, "The value %d is out of range [1, 8].\n", data_y);
            return FAILED;
          }

          /* 8 here is encoded as 0! */
          if (data_y < 8)
            opcode |= data_y << 9;
	}
	else {
	  /* encode the source register number */
	  opcode |= register_y1 << 9;
	  opcode |= 1 << 5;
	}

	/* size */
	opcode |= size << 6;
        
        /* set destination register */
	opcode |= register_x1;
        
	/* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 5:
      /* Bcc */
      {
        int done = NO, opcode, size, backup, res;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

	backup = g_source_index;
	g_source_index = s_parser_source_index;
	res = input_number();
	s_parser_source_index = g_source_index;
	g_source_index = backup;

	if (!(res == SUCCEEDED || res == INPUT_NUMBER_ADDRESS_LABEL || res == INPUT_NUMBER_STACK))
	  return FAILED;

	if (res == INPUT_NUMBER_STACK) {
	  /* let's configure the stack so that all label references inside are relative */
	  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
	  if (stack == NULL)
	    return FAILED;
    
	  stack->relative_references = 1;
	}

	/* emit opcode */
	_output_assembled_instruction(s_instruction_tmp, "d%d ", opcode >> 8);

	if (size == B8(00000000) || g_operand_hint == HINT_8BIT) {
	  /* 8-bit */
	  if (res == INPUT_NUMBER_ADDRESS_LABEL)
	    _output_assembled_instruction(s_instruction_tmp, "k%d R%s ", g_active_file_info_last->line_current, g_label);
	  else if (res == INPUT_NUMBER_STACK)
	    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
	  else
	    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
	}
	else {
	  /* 16-bit */
	  _output_assembled_instruction(s_instruction_tmp, "d0 ");
	  
	  if (res == INPUT_NUMBER_ADDRESS_LABEL) {
	    struct stack *stack;
	    
	    if (stack_create_label_stack(g_label) == FAILED)
	      return FAILED;
	    if (stack_add_offset_plus_n_to_stack(g_latest_stack, 2) == FAILED)
	      return FAILED;

	    /* let's configure the stack so that all label references inside are relative */
	    stack = find_stack_calculation(g_latest_stack, YES);
	    if (stack == NULL)
	      return FAILED;
    
	    stack->relative_references = 1;
	  
	    _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
	  }
	  else if (res == INPUT_NUMBER_STACK) {
	    if (does_stack_contain_one_label(g_latest_stack) == YES) {
	      if (stack_add_offset_plus_n_to_stack(g_latest_stack, 2) == FAILED)
		return FAILED;
	    }

	    _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
	  }
	  else
	    _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);
	}

	g_source_index = s_parser_source_index;
	
	return SUCCEEDED;
      }
      break;

    case 6:
      /* BCHG, BCLR, BSET, BTST */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100))
          immediate = YES;
	else if (register_y_mode != B8(00000000)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

	/* no destination An */
	if (register_x_mode == B8(00000001)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}
	/* PC relative ea cannot be target */
	if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
	    (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}
	/* no immediate target */
        if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

	if (immediate == YES) {
          if (data_type_y != SUCCEEDED) {
	    if (register_x_mode == B8(00000000))
	      print_error(ERROR_NUM, "The value [1, 32] must be known at this stage, it cannot be postponed to the linker.\n");
	    else
	      print_error(ERROR_NUM, "The value [1, 8] must be known at this stage, it cannot be postponed to the linker.\n");
	    return FAILED;
          }

	  if (register_x_mode == B8(00000000)) {
	    if (data_y < 1 || data_y > 32) {
	      print_error(ERROR_NUM, "The value %d is out of range [1, 32].\n", data_y);
	      return FAILED;
	    }
	  }
	  else {
	    if (data_y < 1 || data_y > 8) {
	      print_error(ERROR_NUM, "The value %d is out of range [1, 8].\n", data_y);
	      return FAILED;
	    }
	  }

	  /* BCHG */
	  if (opcode == B16(00000001, 01000000))
	    opcode = B16(00001000, 01000000);
	  /* BCLR */
	  else if (opcode == B16(00000001, 10000000))
	    opcode = B16(00001000, 10000000);
	  /* BSET */
	  else if (opcode == B16(00000001, 11000000))
	    opcode = B16(00001000, 11000000);
	  /* BTST */
	  else if (opcode == B16(00000001, 00000000))
	    opcode = B16(00001000, 00000000);
	}
	else {	  
	  /* register */
	  opcode |= register_y1 << 9;
	}

	if (register_x_mode == B8(00000000))
	  size = B8(00000010);
	else
	  size = B8(00000000);
	
	/* encode mode */
	opcode |= register_x_mode << 3;
	
        /* set destination register */
	opcode |= register_x1;
        
	/* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	if (immediate == YES) {
	  /* output bit number 1-8/1-32 */
	  _output_assembled_instruction(s_instruction_tmp, "d0 d%d ", data_y);
	}

	/* emit extra data */
	_mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 7:
      /* CHK */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* source cannot be An */
	if (register_y_mode == B8(00000001)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

	/* target can only be Dn */
	if (register_x_mode != B8(00000000)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}
	
	/* register */
	opcode |= register_x1 << 9;

	/* encode mode */
	opcode |= register_y_mode << 3;
	
        /* set destination register */
	opcode |= register_y1;

	/* size */
	if (size == B8(00000001))
	  opcode |= B8(00000011) << 7;
	else
	  opcode |= B8(00000010) << 7;
	
	/* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	/* emit extra data */
	_mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 8:
      /* CLR, NEG, NEGX, SCC, TAS, TST */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode, data_y = 0, data_type_y = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* target cannot be An, immediate... */
	if (register_y_mode == B8(00000001) ||
	    (register_y_mode == B8(00000111) && register_y1 == B8(00000100)) ||
	    (register_y_mode == B8(00000111) && register_y1 == B8(00000010)) ||
	    (register_y_mode == B8(00000111) && register_y1 == B8(00000011))) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

	/* encode mode */
	opcode |= register_y_mode << 3;
	
        /* set destination register */
	opcode |= register_y1;

	/* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	/* emit extra data */
	_mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 9:
      /* CMP */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO, mode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        mode = s_instruction_tmp->mode;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100))
          immediate = YES;
        
        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }
          
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

        /* An destination? */
        if (register_x_mode == B8(00000001)) {
          if (size == B8(00000000)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* immediate? */
        else if (immediate == YES) {
          /* An cannot be destination */
          if (register_x_mode == B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
          /* PC relative ea cannot be destination */
          if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
              (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
          /* immediate cannot be destination */
          if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          /* cmp -> cmpi? */
          if (opcode == B16(10110000, 00000000))
            opcode = B16(00001100, 00000000);
        }
        else {
          /* only Dn, An and (An)+ targets are accepted */

          /* cmp -> cmp? */
          if (register_x_mode == B8(00000000)) {
          }
          /* cmp -> cmpm? */
          else if (register_y_mode == B8(00000011) && register_x_mode == B8(00000011)) {
            if (opcode == B16(10110000, 00000000))
              opcode = B16(10110001, 00001000);
          }
          else {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* immediate source expected? */
        if (mode == MC68000_MODE_I) {
          if (immediate == NO) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* destination An? */
        if (mode == MC68000_MODE_A) {
          if (register_x_mode != B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* (An)+, (Am)+ ? */
        if (mode == MC68000_MODE_M) {
          if (register_y_mode != B8(00000011) || register_x_mode != B8(00000011)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }

        /* choose the size and opmode */
        if (register_x_mode == B8(00000001)) {
          /* destination is An */
          if (size == B8(00000001)) {
            /* .w */
            opcode |= B8(00000011) << 6;
          }
          else if (size == B8(00000010)) {
            /* .l */
            opcode |= B8(00000111) << 6;
          }
          else {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        else if (immediate == YES) {
          /* immediate source */

          /* size */
          opcode |= size << 6;
        }
        else if (register_x_mode == B8(00000000)) {
          /* destination is Dn */
          opcode |= size << 6;
        }
        else if (register_y_mode == B8(00000011) && register_x_mode == B8(00000011)) {
          /* (An)+, (Am)+ */
          opcode |= size << 6;
        }
        else {
          /* destination is ea */
          opcode |= (size | (1 << 2)) << 6;
        }
        
        /* set register */
        if (register_x_mode == B8(00000001))
          opcode |= register_x1 << 9;
        else if (immediate == YES) {
          /* immediate */
        }
        else if (register_x_mode == B8(00000000))
          opcode |= register_x1 << 9;
        else if (register_y_mode == B8(00000011) && register_x_mode == B8(00000011)) {
          /* (An)+, (Am)+ */
          opcode |= register_x1 << 9;
          opcode |= register_y1;
        }
        else
          opcode |= register_y1 << 9;
        
        /* set ea */
        if (register_x_mode == B8(00000001)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        else if (immediate == YES) {
          /* immediate source */
          opcode |= register_x_mode << 3;
          opcode |= register_x1;
        }
        else if (register_y_mode == B8(00000011) && register_x_mode == B8(00000011)) {
          /* (An)+, (Am)+ */
        }
        else if (register_x_mode == B8(00000000)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;
      
    case 10:
      /* DBcc */
      {
        int register_y1 = 0, register_y2 = 0, register_y_mode = 0, data_y = 0, data_type_y = FAILED;
        int done = NO, opcode, backup, res;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        /* source needs to be Dn */
        if (register_y_mode != B8(00000000)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        
	backup = g_source_index;
	g_source_index = s_parser_source_index;
	res = input_number();
	s_parser_source_index = g_source_index;
	g_source_index = backup;

	if (!(res == SUCCEEDED || res == INPUT_NUMBER_ADDRESS_LABEL || res == INPUT_NUMBER_STACK))
	  return FAILED;

	if (res == INPUT_NUMBER_STACK) {
	  /* let's configure the stack so that all label references inside are relative */
	  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
	  if (stack == NULL)
	    return FAILED;
    
	  stack->relative_references = 1;
	}

        /* encode register */
        opcode |= register_y1;

	/* emit opcode */
	_output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* 16-bit */
        if (res == INPUT_NUMBER_ADDRESS_LABEL) {
          struct stack *stack;
	    
          if (stack_create_label_stack(g_label) == FAILED)
            return FAILED;
          if (stack_add_offset_plus_n_to_stack(g_latest_stack, 2) == FAILED)
            return FAILED;

          /* let's configure the stack so that all label references inside are relative */
          stack = find_stack_calculation(g_latest_stack, YES);
          if (stack == NULL)
            return FAILED;
    
          stack->relative_references = 1;
	  
          _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
        }
        else if (res == INPUT_NUMBER_STACK) {
          if (does_stack_contain_one_label(g_latest_stack) == YES) {
            if (stack_add_offset_plus_n_to_stack(g_latest_stack, 2) == FAILED)
              return FAILED;
          }

          _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
        }
        else
          _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);

	g_source_index = s_parser_source_index;
	
	return SUCCEEDED;
      }
      break;

    case 11:
      /* DIVS, DIVU, MULS, MULU */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        /* source cannot be An */
	if (register_y_mode == B8(00000001)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}
        
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

        /* target must be Dn */
	if (register_x_mode != B8(00000000)) {
	  print_error(ERROR_NUM, "Invalid addressing mode.\n");
	  return FAILED;
	}

        /* register */
        opcode |= register_x1 << 9;

	/* encode mode */
	opcode |= register_y_mode << 3;
	
        /* set destination register */
	opcode |= register_y1;
        
	/* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	/* emit extra data */
	_mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 12:
      /* EOR */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO, mode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        mode = s_instruction_tmp->mode;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100))
          immediate = YES;

        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }
          
        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	if (immediate) {
	  if (register_x_mode == B8(00001000)) {
	    /* #<data>, CCR */

	    /* not .b? */
	    if (size != B8(00000000)) {
	      print_error(ERROR_NUM, "Invalid addressing mode.\n");
	      return FAILED;
	    }

	    /* eori */
	    if (opcode == B16(00001010, 00000000))
	      opcode = B16(00001010, 00111100);

	    /* emit opcode */
	    _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	    /* emit extra data */
	    _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

	    g_source_index = s_parser_source_index;

	    return SUCCEEDED;
	  }
	  else if (register_x_mode == B8(00001001)) {
	    /* #<data>, SR */

	    /* not .w? */
	    if (size != B8(00000001)) {
	      print_error(ERROR_NUM, "Invalid addressing mode.\n");
	      return FAILED;
	    }

	    /* eori */
	    if (opcode == B16(00001010, 00000000))
	      opcode = B16(00001010, 01111100);

	    /* emit opcode */
	    _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

	    /* emit extra data */
	    _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

	    g_source_index = s_parser_source_index;

	    return SUCCEEDED;
	  }
	}

	/* source must be Dn */
	if (immediate == NO) {
	  if (register_y_mode != B8(00000000)) {
	    print_error(ERROR_NUM, "Invalid addressing mode.\n");
	    return FAILED;
	  }
	}
        else {
          /* no An */
          if (register_x_mode == B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
          /* PC relative ea cannot be destination */
          if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
              (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
          /* immediate ea cannot be destination */
          if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* immediate source expected? */
        if (mode == MC68000_MODE_I) {
          if (immediate == NO) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        
        if (immediate == YES) {
          /* immediate source */

          /* switch eor -> eori */
          if (opcode == B16(10110000, 00000000))
            opcode = B16(00001010, 00000000);

          /* size */
          opcode |= size << 6;
        }
        else if (register_y_mode == B8(00000000)) {
          /* source is Dn */
          opcode |= (B8(00000100) | size) << 6;
        }
        
        /* set register */
        if (register_y_mode == B8(00000000))
          opcode |= register_y1 << 9;
        
        /* set ea */
        opcode |= register_x_mode << 3;
        opcode |= register_x1;

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 13:
      /* EXG */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, opmode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	/* Dn, Dm */
        if (register_y_mode == B8(00000000) && register_x_mode == B8(00000000))
          opmode = B8(01000);
        /* An, Am */
        else if (register_y_mode == B8(00000001) && register_x_mode == B8(00000001))
          opmode = B8(01001);
        /* Dn, Am */
        else if (register_y_mode == B8(00000000) && register_x_mode == B8(00000001))
          opmode = B8(10001);
        else {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* opmode */
        opcode |= opmode << 3;
        
        /* set registers */
        opcode |= register_y1 << 9;
        opcode |= register_x1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 14:
      /* EXT */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* only Dn is accepted */
        if (register_y_mode != B8(00000000)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* opmode */
        opcode |= size << 6;
        
        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 15:
      /* JMP, JSR, PEA */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* Dn or An or (An)+ or -(An) or immediate */
        if (register_y_mode == B8(00000000) || register_y_mode == B8(00000001) ||
            register_y_mode == B8(00000011) || register_y_mode == B8(00000100) ||
            (register_y_mode == B8(00000111) && register_y1 == B8(00000100))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* mode */
        opcode |= register_y_mode << 3;
        
        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 16:
      /* LEA */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	/* Dn or An or (An)+ or -(An) or immediate as source? */
        if (register_y_mode == B8(00000000) || register_y_mode == B8(00000001) ||
            register_y_mode == B8(00000011) || register_y_mode == B8(00000100) ||
            (register_y_mode == B8(00000111) && register_y1 == B8(00000100))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* something else than An as target? */
        if (register_x_mode != B8(00000001)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* mode */
        opcode |= register_y_mode << 3;
        
        /* set registers */
        opcode |= register_y1;
        opcode |= register_x1 << 9;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 17:
      /* LINK */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

	/* An source? */
        if (register_y_mode != B8(00000001)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate target? */
        if (register_x_mode != B8(00000111) || register_x1 != B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 18:
      /* NBCD */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* invalid addressing modes? */
        if (register_y_mode == B8(00000001) ||
            (register_y_mode == B8(00000111) && register_y1 == B8(00000100)) ||
            (register_y_mode == B8(00000111) && register_y1 == B8(00000010)) ||
            (register_y_mode == B8(00000111) && register_y1 == B8(00000011))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* mode */
        opcode |= register_y_mode << 3;
        
        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 19:
      /* STOP */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        /* immediate? */
        if (register_y_mode != B8(00000111) || register_y1 != B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 20:
      /* SWAP */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* only Dn is accepted */
        if (register_y_mode != B8(00000000)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 21:
      /* TRAP */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* only immediate is accepted */
        if (register_y_mode != B8(00000111) || register_y1 != B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* only [0, 15] is accepted */
        if (data_type_y != SUCCEEDED) {
          print_error(ERROR_NUM, "The value [0, 15] must be known at this stage, it cannot be postponed to the linker.\n");
          return FAILED;
        }
        if (data_y < 0 || data_y > 15) {
          print_error(ERROR_NUM, "The value %d is out of range [0, 15].\n", data_y);
          return FAILED;
        }

        /* vector */
        opcode |= data_y;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 22:
      /* UNLK */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED;
        char label_y[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

	/* only An is accepted */
        if (register_y_mode != B8(00000001)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* register */
        opcode |= register_y1;
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 23:
      /* MOVE, MOVEA, MOVEQ */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size, immediate = NO, mode;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        mode = s_instruction_tmp->mode;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, YES) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

        /* PC relative ea cannot be destination */
        if ((register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
            (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate ea cannot be destination */
        if (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }

        /* move to CCR? */
        if (register_x_mode == B8(00001000)) {
          if (register_y_mode > B8(00000111) || register_y_mode == B8(00000001) || mode != MC68000_MODE_ALL) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          opcode = B16(01000100, 11000000);

          /* mode */
          opcode |= register_y_mode << 3;

          /* register */
          opcode |= register_y1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit extra data */
          _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

          g_source_index = s_parser_source_index;

          return SUCCEEDED;          
        }
        /* move from SR? */
        else if (register_y_mode == B8(00001001)) {
          if (register_x_mode > B8(00000111) || register_x_mode == B8(00000001) || mode != MC68000_MODE_ALL) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          opcode = B16(01000000, 11000000);

          /* mode */
          opcode |= register_x_mode << 3;

          /* register */
          opcode |= register_x1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit extra data */
          _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

          g_source_index = s_parser_source_index;

          return SUCCEEDED;
        }
        /* move to SR? */
        else if (register_x_mode == B8(00001001)) {
          if (register_y_mode > B8(00000111) || register_y_mode == B8(00000001) || mode != MC68000_MODE_ALL) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          opcode = B16(01000110, 11000000);

          /* mode */
          opcode |= register_y_mode << 3;

          /* register */
          opcode |= register_y1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit extra data */
          _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

          g_source_index = s_parser_source_index;

          return SUCCEEDED;
        }
        /* move from USP? */
        else if (register_y_mode == B8(00001010) && register_x_mode == B8(00000001)) {
          if (mode != MC68000_MODE_ALL) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          opcode = B16(01001110, 01101000);

          /* register */
          opcode |= register_x1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit extra data */
          _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

          g_source_index = s_parser_source_index;

          return SUCCEEDED;
        }
        /* move to USP? */
        else if (register_x_mode == B8(00001010) && register_y_mode == B8(00000001)) {
          if (mode != MC68000_MODE_ALL) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          opcode = B16(01001110, 01100000);

          /* register */
          opcode |= register_y1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit extra data */
          _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);

          g_source_index = s_parser_source_index;

          return SUCCEEDED;
        }

        if (register_y_mode == B8(00000111) && register_y1 == B8(00000100)) {
          immediate = YES;

          /* is immediate value [-128, 127]? */
          if (register_x_mode == B8(00000000) && mode == MC68000_MODE_ALL && data_type_y == SUCCEEDED && data_y >= -128 && data_y <= 127) {
            mode = MC68000_MODE_Q;

            /* yes, switch move -> moveq */
            if (opcode == B16(00000000, 00000000))
              opcode = B16(01110000, 00000000);
          }
        }

        /* no special source or target */
        if (register_x_mode > B8(00000111) || register_y_mode > B8(00000111)) {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        /* immediate ea size checks */
        if (data_type_y == SUCCEEDED && immediate == YES) {
          if (_mc68000_size_check(data_y, size) == FAILED)
            return FAILED;
        }

        /* no .B with An */
        if (register_x_mode == B8(00000001) || register_y_mode == B8(00000001)) {
          if (size == B8(00000000)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* immediate source expected? */
        if (mode == MC68000_MODE_Q) {
          if (immediate == NO) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
          /* destination must be Dn */
          if (register_x_mode != B8(00000000)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        /* destination An? */
        if (mode == MC68000_MODE_A) {
          if (register_x_mode != B8(00000001)) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }
        }
        
        /* choose the size and opmode */
        if (mode == MC68000_MODE_Q) {
        }
        else if (register_x_mode == B8(00000001)) {
          /* destination is An */
          opcode |= B8(01000000);
          
          if (size == B8(00000001)) {
            /* .w */
            opcode |= B8(00000011) << 12;
          }
          else {
            /* .l */
            opcode |= B8(00000010) << 12;
          }
        }
        else {
          /* destination is ea */
          if (size == B8(00000000))
            opcode |= B8(00000001) << 12;
          else if (size == B8(00000001))
            opcode |= B8(00000011) << 12;
          else
            opcode |= size << 12;
        }
        
        /* set source */
        if (mode == MC68000_MODE_Q) {
          /* actually here we encode the data (-128 - 127) */
          if (data_type_y != SUCCEEDED) {
            print_error(ERROR_NUM, "The value [-128, 127] must be known at this stage, it cannot be postponed to the linker.\n");
            return FAILED;
          }
          if (data_y < -128 || data_y > 127) {
            print_error(ERROR_NUM, "The value %d is out of range [-128, 127].\n", data_y);
            return FAILED;
          }

          opcode |= data_y & 0xff;

          /* eat data_y so _mc68000_emit_extra_data() will not emit anything */
          register_y_mode = 0;
        }
        else if (register_x_mode == B8(00000001))
          opcode |= register_x1 << 9;
        else {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        
        /* set ea */
        if (mode == MC68000_MODE_Q)
          opcode |= register_x1 << 9;
        else if (register_x_mode == B8(00000001)) {
          opcode |= register_y_mode << 3;
          opcode |= register_y1;
        }
        else {
          opcode |= register_x_mode << 6;
          opcode |= register_x1 << 9;
        }

        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 24:
      /* MOVEP */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, NO) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, NO) == FAILED)
          break;

        /* Dx, (d16, Ay)? */
        if (register_y_mode == B8(00000000) && register_x_mode == B8(00000101)) {
          /* Dx */
          opcode |= register_y1 << 9;

          /* Ay */
          opcode |= register_x1;

          if (size == B8(00000001))
            opcode |= B8(00000110) << 6;
          else
            opcode |= B8(00000111) << 6;
        }
        /* (d16, Ay), Dx? */
        else if (register_x_mode == B8(00000000) && register_y_mode == B8(00000101)) {
          /* Dx */
          opcode |= register_x1 << 9;

          /* Ay */
          opcode |= register_y1;

          if (size == B8(00000001))
            opcode |= B8(00000100) << 6;
          else
            opcode |= B8(00000101) << 6;
        }
        else {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        
        /* emit opcode */
        _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

        /* emit extra data */
        _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);

        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;

    case 25:
      /* MOVEM */
      {
        int done = NO, register_y1 = 0, register_y2 = 0, register_y_mode = 0, register_x1 = 0, register_x2 = 0, register_x_mode = 0, opcode;
        int data_y = 0, data_type_y = FAILED, data_x = 0, data_type_x = FAILED, size;
        char label_y[MAX_NAME_LENGTH + 1], label_x[MAX_NAME_LENGTH + 1];
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 0) {
	    if (g_buffer[s_parser_source_index] == ' ') {
	      done = YES;
	      break;
	    }
	    else
	      break;
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }

        if (done == NO)
          break;

        size = s_instruction_tmp->size;
        opcode = s_instruction_tmp->hex;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_y1, &register_y2, &register_y_mode, &data_y, &data_type_y, label_y, YES) == FAILED)
          break;

        if (g_buffer[s_parser_source_index] != ',')
          break;
        s_parser_source_index++;

        if (_mc68000_parse_ea(g_buffer, &s_parser_source_index, &register_x1, &register_x2, &register_x_mode, &data_x, &data_type_x, label_x, YES) == FAILED)
          break;

        /* turn register source into a register list */
        if (register_y_mode == B8(00000000)) {
          /* Dn */
          register_y_mode = B8(00001011);
          register_y1 = 1 << register_y1;
        }
        if (register_y_mode == B8(00000001)) {
          /* An */
          register_y_mode = B8(00001011);
          register_y1 = 1 << (register_y1 + 8);
        }

        /* turn register target into a register list */
        if (register_x_mode == B8(00000000)) {
          /* Dn */
          register_x_mode = B8(00001011);
          register_x1 = 1 << register_x1;
        }
        if (register_x_mode == B8(00000001)) {
          /* An */
          register_x_mode = B8(00001011);
          register_x1 = 1 << (register_x1 + 8);
        }
        
        /* register source? */
        if (register_y_mode == B8(00001011)) {
          /* register to memory */

          /* no Dn, An, immediate or PC relative modes */
          if (register_x_mode == B8(00000000) || register_x_mode == B8(00000001) ||
              register_x_mode == B8(00000011) || (register_x_mode == B8(00000111) && register_x1 == B8(00000100)) ||
              (register_x_mode == B8(00000111) && register_x1 == B8(00000010)) ||
              (register_x_mode == B8(00000111) && register_x1 == B8(00000011))) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          /* -(An)? */
          if (register_x_mode == B8(00000100))
            register_y1 = _mc68000_reverse_register_list(register_y1);

          /* mode */
          opcode |= register_x_mode << 3;

          /* register */
          opcode |= register_x1;

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit register list */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", register_y1);
                    
          /* emit extra data */
          _mc68000_emit_extra_data(register_x_mode, register_x1, register_x2, data_type_x, data_x, label_x, size);
        }
        /* register target? */
        else if (register_x_mode == B8(00001011)) {
          /* memory to register */
          opcode |= 1 << 10;

          /* no Dn, An, immediate */
          if (register_y_mode == B8(00000000) || register_y_mode == B8(00000001) ||
              register_y_mode == B8(00000100) || (register_y_mode == B8(00000111) && register_y1 == B8(00000100))) {
            print_error(ERROR_NUM, "Invalid addressing mode.\n");
            return FAILED;
          }

          /* mode */
          opcode |= register_y_mode << 3;

          /* register */
          opcode |= register_y1;          

          /* emit opcode */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", opcode);

          /* emit register list */
          _output_assembled_instruction(s_instruction_tmp, "y%d ", register_x1);
          
          /* emit extra data */
          _mc68000_emit_extra_data(register_y_mode, register_y1, register_y2, data_type_y, data_y, label_y, size);
        }
        else {
          print_error(ERROR_NUM, "Invalid addressing mode.\n");
          return FAILED;
        }
        
        g_source_index = s_parser_source_index;

        return SUCCEEDED;
      }
      break;
      
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </68000> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif
      
#if defined(MC6800)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6800> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (s_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </6800> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif
      
#if defined(MC6801)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6801> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (s_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </6801> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif
      
#if defined(MC6809)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6809> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          if (s_instruction_tmp->hex > 0xFF)
            _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
          else
            _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                if (s_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "r%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (s_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 6:
      /* 5-bit signed operand, absolute address + post op byte code */
      if (g_xbit_size >= 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;

          /* TODO: add a mechanism so that we could use 5-bit addreslabel references and calculations */
          /* currently we just fall back to 8-bit/16-bit ones */
          if (z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)
            break;
          if (z != SUCCEEDED)
            return FAILED;
          if (g_operand_hint == HINT_8BIT || g_operand_hint == HINT_16BIT)
            break;
          if (g_parsed_int > 15 || g_parsed_int < -16)
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);

              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->addressing_mode_bits | (g_parsed_int & 0x1F));
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 7:
      /* 8-bit signed operand, relative address + post op byte code */
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            if (s_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
                
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->addressing_mode_bits, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d R%s ", s_instruction_tmp->addressing_mode_bits, g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->addressing_mode_bits, g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 8:
      /* 16-bit operand + post op byte code */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);

              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->addressing_mode_bits, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d r%s ", s_instruction_tmp->addressing_mode_bits, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->addressing_mode_bits, g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 9:
      /* plain text 8-bit + post op byte code */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

          if (s_instruction_tmp->hex > 0xFF)
            _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
          else
            _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);

          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->addressing_mode_bits);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 10:
      /* EXG / TFR */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'r') {
          g_parsed_int = _parse_exg_tfr_registers();
          if (g_parsed_int < 0)
            return FAILED;

          _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
        
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 11:
      /* PSHS / PSHU / PULS / PULU */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'r') {
          g_parsed_int = _parse_push_pull_registers(s_instruction_tmp->addressing_mode_bits);
          if (g_parsed_int < 0)
            return FAILED;

          _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
        
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 12:
      /* 16-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (s_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", (s_instruction_tmp->hex >> 8) & 0xFF, s_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "M%s ", g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </6809> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif
      
#if defined(I8008)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <8008> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          if (x > 0 && (s_instruction_tmp->string[x-1] == '+' || s_instruction_tmp->string[x-1] == '-'))
            g_source_index--;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
          
                if (s_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d v2 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);       

              /* reset to "no special case" */
              _output_assembled_instruction(s_instruction_tmp, "v0 ");
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </8008> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif

#if defined(I8080)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <8080> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x' || s_instruction_tmp->string[x] == 's') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          if (x > 0 && (s_instruction_tmp->string[x-1] == '+' || s_instruction_tmp->string[x-1] == '-'))
            g_source_index--;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((s_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (s_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
          
                if (s_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED || g_parsed_int != s_instruction_tmp->value)
            break;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d v3 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);       

              /* reset to "no special case" */
              _output_assembled_instruction(s_instruction_tmp, "v0 ");
        
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </8080> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif

#if defined(SPC700)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <SPC-700> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d r%s ", s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '~') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | (g_parsed_int << 5));

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '~') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | (((g_parsed_int << 1) + 1) << 4));

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '~') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (z != SUCCEEDED)
            break;
          if (g_parsed_int > 0xF || g_parsed_int < 0)
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | (g_parsed_int << 4));
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xA:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);

                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                    if (stack == NULL)
                      return FAILED;
    
                    stack->relative_references = 1;

                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                  }

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xB:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xC:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '~') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;
              g = g_parsed_int;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (s_instruction_tmp->string[x] == 'x') {
                  y = g_source_index;
                  g_source_index = s_parser_source_index;
                  z = input_number();
                  s_parser_source_index = g_source_index;
                  g_source_index = y;
                  if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                    return FAILED;
                  if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                    break;

                  for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                    if (IS_THE_MATCH_COMPLETE(x)) {
                      _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                      _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | (g << 5));

                      if (v == SUCCEEDED)
                        _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                      else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                      else
                        _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                      if (z == SUCCEEDED)
                        _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                      else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                      else {
                        struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                        if (stack == NULL)
                          return FAILED;

                        stack->relative_references = 1;

                        _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                      }

                      g_source_index = s_parser_source_index;
                      return SUCCEEDED;
                    }
                    if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                      break;
                  }
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xD:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '~') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;
              g = g_parsed_int;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (s_instruction_tmp->string[x] == 'x') {
                  y = g_source_index;
                  g_source_index = s_parser_source_index;
                  z = input_number();
                  s_parser_source_index = g_source_index;
                  g_source_index = y;
                  if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                    return FAILED;
                  if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                    break;

                  for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                    if (IS_THE_MATCH_COMPLETE(x)) {
                      _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                      _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | (((g << 1) + 1) << 4));

                      if (v == SUCCEEDED)
                        _output_assembled_instruction(s_instruction_tmp, "d%d ", e);
                      else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(s_instruction_tmp, "Q%s ", labelx);
                      else
                        _output_assembled_instruction(s_instruction_tmp, "c%d ", h);

                      if (z == SUCCEEDED)
                        _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                      else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                      else {
                        struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                        if (stack == NULL)
                          return FAILED;

                        stack->relative_references = 1;

                        _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                      }

                      g_source_index = s_parser_source_index;
                      return SUCCEEDED;
                    }
                    if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                      break;
                  }
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xE:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d R%s ", s_instruction_tmp->hex, g_label);
              else {
                struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;

                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              }

              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 0xF:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 8191 || g_parsed_int < 0))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '~') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, e | g_parsed_int << 13);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d n%d %s ", s_instruction_tmp->hex, g_parsed_int, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d N%d %d ", s_instruction_tmp->hex, g_parsed_int, h);

                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </SPC-700> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(HUC6280)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <HUC-6280> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 9:
    case 1:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
                if (s_instruction_tmp->type == 9) {
                  /* 9 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "y%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      v = -1;
      e = -1;
      h = -1;
      z = -1;
      s = 0;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          s = 1;
          break;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      if (s == 0)
        break;

      for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          r = g_parsed_int;
          t = z;
          u = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labely, g_label);
          s = 2;
          break;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      if (s == 1)
        break;

      for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          s = 3;
          break;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      if (s == 2)
        break;

      for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);

          /* first operand */
          if (v == SUCCEEDED)
            _output_assembled_instruction(s_instruction_tmp, "y%d ", e);
          else if (v == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(s_instruction_tmp, "r%s ", labelx);
          else if (v == INPUT_NUMBER_STACK)
            _output_assembled_instruction(s_instruction_tmp, "C%d ", h);
          else {
            print_text(NO, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }

          /* second operand */
          if (t == SUCCEEDED)
            _output_assembled_instruction(s_instruction_tmp, "y%d ", r);
          else if (t == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(s_instruction_tmp, "r%s ", labely);
          else if (t == INPUT_NUMBER_STACK)
            _output_assembled_instruction(s_instruction_tmp, "C%d ", u);
          else {
            print_text(NO, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }
          
          /* third operand */
          if (z == SUCCEEDED)
            _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(s_instruction_tmp, "r%s ", g_label);
          else if (z == INPUT_NUMBER_STACK)
            _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
          else {
            print_text(NO, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }
          
          g_source_index = s_parser_source_index;
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 5:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              if (g_operand_hint == HINT_16BIT)
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "Q%s ", g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
    
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '?') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
                print_error(ERROR_NUM, "Out of 16-bit range.\n");
                return FAILED;
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "y%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "r%s ", g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "C%d ", g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 7:
      if (g_xbit_size > 8 && s_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, s_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'x') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d Q%s ", s_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(s_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </HUC-6280> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif

#if defined(SUPERFX)

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <SUPER-FX> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (IS_THE_MATCH_COMPLETE(x)) {
          _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
          if (s_instruction_tmp->prefix != 0)
            _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
          
          _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex);
          g_source_index = s_parser_source_index;
          
          return SUCCEEDED;
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
              if (s_instruction_tmp->prefix != 0)
                _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
              
              _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | tiny);
              
              g_source_index = s_parser_source_index;
              
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == 'e') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if (g_parsed_int > 127 || g_parsed_int < -128) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (IS_THE_MATCH_COMPLETE(x)) {
              _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
              if (z == SUCCEEDED)
                _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(s_instruction_tmp, "d%d R%s ", s_instruction_tmp->hex, g_label);
              else {
                struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                
                if (stack == NULL)
                  return FAILED;
                
                stack->relative_references = 1;
                
                _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex, g_latest_stack);
              }
              
              g_source_index = s_parser_source_index;
              return SUCCEEDED;
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'x') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 255 || g_parsed_int < -128) {
                  print_error(ERROR_NUM, "Out of 8-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (s_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex | tiny, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d R%s ", s_instruction_tmp->hex | tiny, g_label);
                  else {
                    struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                    
                    _output_assembled_instruction(s_instruction_tmp, "d%d c%d ", s_instruction_tmp->hex | tiny, g_latest_stack);
                  }
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '?') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (s_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex | tiny, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d r%s ", s_instruction_tmp->hex | tiny, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex | tiny, g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == 'y') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 510 || g_parsed_int < 0) {
                  print_error(ERROR_NUM, "Out of 9-bit range.\n");
                  return FAILED;
                }
                if ((g_parsed_int & 1) == 1) {
                  print_error(ERROR_NUM, "The RAM address must be even.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (s_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex | tiny, g_parsed_int >> 1);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d *%s ", s_instruction_tmp->hex | tiny, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d -%d ", s_instruction_tmp->hex | tiny, g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  if (s_instruction_tmp->hex == 0xB0) {
                    /* MOVES */
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix | tiny);
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | z);
                  }
                  else {
                    /* OTHERWISE */
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix | z);
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->hex | tiny);
                  }
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '?') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if (g_parsed_int >= -128 && g_parsed_int <= 127) {
                      /* use IBT */
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", 0xA0 | tiny, g_parsed_int);
                    }
                    else {
                      /* use IWT */
                      _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", 0xF0 | tiny, g_parsed_int);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d r%s ", 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", 0xF0 | tiny, g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '?') {
              y = g_source_index;
              g_source_index = s_parser_source_index;
              z = input_number();
              s_parser_source_index = g_source_index;
              g_source_index = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if ((g_parsed_int >= 0 && g_parsed_int <= 510) && ((g_parsed_int & 1) == 0)) {
                      /* use LMS */
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d d%d ", 0x3D, 0xA0 | tiny, g_parsed_int >> 1);
                    }
                    else {
                      /* use LM */
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d y%d ", 0x3D, 0xF0 | tiny, g_parsed_int);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d r%s ", 0x3D, 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d C%d ", 0x3D, 0xF0 | tiny, g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '?') {
          y = g_source_index;
          g_source_index = s_parser_source_index;
          z = input_number();
          s_parser_source_index = g_source_index;
          g_source_index = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if (g_parsed_int > 65535 || g_parsed_int < -32768) {
              print_error(ERROR_NUM, "Out of 16-bit range.\n");
              return FAILED;
            }
            y = g_parsed_int;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
              if (tiny < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if ((y >= 0 && y <= 510) && ((y & 1) == 0)) {
                      /* use SMS */
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d d%d ", 0x3E, 0xA0 | tiny, y >> 1);
                    }
                    else {
                      /* use SM */
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d y%d ", 0x3E, 0xF0 | tiny, y);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d r%s ", 0x3E, 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d C%d ", 0x3E, 0xF0 | tiny, g_latest_stack);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 10:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(0, 11);
              if (z < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (tiny == 0) /* LDB (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", 0x3D, 0x40 | z);
                  else /* TO R* + LDB (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d d%d ", 0x10 | tiny, 0x3D, 0x40 | z);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 11:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(0, 11);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == 0) /* STB (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", 0x3D, 0x30 | tiny);
                  else /* FROM R* + STB (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d d%d ", 0xB0 | z, 0x3D, 0x30 | tiny);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 12:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(0, 11);
              if (z < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (tiny == 0) /* LDW (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", 0x40 | z);
                  else /* TO R* + LDW (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", 0x10 | tiny, 0x40 | z);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;
      
    case 13:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
        if (s_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(0, 11);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
            if (s_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                if (IS_THE_MATCH_COMPLETE(x)) {
                  _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == 0) /* STW (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d ", 0x30 | tiny);
                  else /* FROM R* + STW (R*) */
                    _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", 0xB0 | z, 0x30 | tiny);
                  
                  g_source_index = s_parser_source_index;
                  return SUCCEEDED;
                }
                if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                  break;
              }
            }
            if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
              break;
          }
        }
        if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
          break;
      }
      break;

    case 14:
      {
        char labelx[MAX_NAME_LENGTH + 1];
        int v, s;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == '?') {
            y = g_source_index;
            g_source_index = s_parser_source_index;
            z = input_number();
            s_parser_source_index = g_source_index;
            g_source_index = y;
            if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
              return FAILED;
            if (z == SUCCEEDED) {
              if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                print_error(ERROR_NUM, "Out of 16-bit range.\n");
                return FAILED;
              }
            }
            
            /* backup as _parse_tiny_int() will overwrite these */
            v = g_parsed_int;
            strcpy(labelx, g_label);
            s = g_latest_stack;
            
            for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
              if (s_instruction_tmp->string[x] == '*') {
                tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
                if (tiny < 0)
                  return FAILED;
                
                for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                  if (IS_THE_MATCH_COMPLETE(x)) {
                    _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                    
                    if (s_instruction_tmp->prefix != 0)
                      _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
                    
                    if (z == SUCCEEDED)
                      _output_assembled_instruction(s_instruction_tmp, "d%d y%d ", s_instruction_tmp->hex | tiny, v);
                    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                      _output_assembled_instruction(s_instruction_tmp, "d%d r%s ", s_instruction_tmp->hex | tiny, labelx);
                    else
                      _output_assembled_instruction(s_instruction_tmp, "d%d C%d ", s_instruction_tmp->hex | tiny, s);
                    
                    g_source_index = s_parser_source_index;
                    return SUCCEEDED;
                  }
                  if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                    break;
                }
              }
              if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                break;
            }
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }
      }
      break;
      
    case 15:
      {
        char labelx[MAX_NAME_LENGTH + 1];
        int v, s;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
          if (s_instruction_tmp->string[x] == 'y') {
            y = g_source_index;
            g_source_index = s_parser_source_index;
            z = input_number();
            s_parser_source_index = g_source_index;
            g_source_index = y;
            if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
              return FAILED;
            if (z == SUCCEEDED) {
              if (g_parsed_int > 510 || g_parsed_int < 0) {
                print_error(ERROR_NUM, "Out of 9-bit range.\n");
                return FAILED;
              }
              if ((g_parsed_int & 1) == 1) {
                print_error(ERROR_NUM, "The RAM address must be even.\n");
                return FAILED;
              }
            }
            
            /* backup as _parse_tiny_int() will overwrite these */
            v = g_parsed_int;
            strcpy(labelx, g_label);
            s = g_latest_stack;
            
            for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
              if (s_instruction_tmp->string[x] == '*') {
                tiny = _parse_tiny_int(s_instruction_tmp->min, s_instruction_tmp->max);
                if (tiny < 0)
                  return FAILED;
                
                for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; s_parser_source_index++, x++) {
                  if (IS_THE_MATCH_COMPLETE(x)) {
                    _output_assembled_instruction(s_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                    
                    if (s_instruction_tmp->prefix != 0)
                      _output_assembled_instruction(s_instruction_tmp, "d%d ", s_instruction_tmp->prefix);
                    
                    if (z == SUCCEEDED)
                      _output_assembled_instruction(s_instruction_tmp, "d%d d%d ", s_instruction_tmp->hex | tiny, v >> 1);
                    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                      _output_assembled_instruction(s_instruction_tmp, "d%d *%s ", s_instruction_tmp->hex | tiny, labelx);
                    else
                      _output_assembled_instruction(s_instruction_tmp, "d%d -%d ", s_instruction_tmp->hex | tiny, s);
                    
                    g_source_index = s_parser_source_index;
                    return SUCCEEDED;
                  }
                  if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                    break;
                }
              }
              if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
                break;
            }
          }
          if (s_instruction_tmp->string[x] != toupper((int)g_buffer[s_parser_source_index]))
            break;
        }
      }
      break;
      
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </SUPER-FX> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
#endif
      
    }

    /* perform stack rollback? */
    if (g_last_stack_id > last_stack_id_backup) {
      /* we have created new stack calculations in vain as we didn't find a matching instruction ->
         delete all unnecessary stack calculations */
      for (y = last_stack_id_backup; y < g_last_stack_id; y++) {
        struct stack *s = find_stack_calculation(y, YES);

        if (s == NULL)
          return FAILED;

        delete_stack_calculation_struct(s);
      }

      g_last_stack_id = last_stack_id_backup;
    }

    s_instruction_tmp = &g_instructions_table[++instruction_i];
  }

  /* allow error messages from input_numbers() */
  g_input_number_error_msg = YES;

  return EVALUATE_TOKEN_NOT_IDENTIFIED;
}

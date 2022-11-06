
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "defines.h"

#include "stack.h"
#include "pass_1.h"
#include "parse.h"
#include "printf.h"


extern FILE *g_file_out_ptr;
extern char *g_buffer, *unfolded_buffer, g_label[MAX_NAME_LENGTH + 1], *g_include_dir, *g_full_name;
extern int g_in_enum, g_in_ramsection, g_in_struct, g_macro_id, g_sizeof_g_tmp;
extern int g_ss, g_source_pointer, g_parsed_int, g_section_status, g_org_defined, g_bankheader_status, g_macro_active;
extern int g_source_file_size, g_input_number_error_msg, g_verbose_level, g_output_format, g_open_files, g_input_parse_if;
extern int g_last_stack_id, g_latest_stack, g_ss, g_commandline_parsing, g_newline_beginning, g_expect_calculations, g_input_parse_special_chars;
extern char *g_tmp;
extern int g_instruction_n[256], g_instruction_p[256];
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

#ifdef SPC700
extern int g_input_number_expects_dot;
#endif

/* NOTE: get rid of these two! */
extern int g_inz, g_ind;

static struct instruction *g_instruction_tmp;


static void _output_assembled_instruction(struct instruction *instruction, const char *format, ...) {

  va_list ap;
  
  if (instruction == NULL)
    return;
  
  va_start(ap, format);

  vfprintf(g_file_out_ptr, format, ap);
#ifdef WLA_DEBUG
  {
    char ttt[256];

    va_end(ap);
    va_start(ap, format);
    vsnprintf(ttt, sizeof(ttt), format, ap);
    printf("LINE %5d: INSTRUCTION: %16s ::: %s\n", g_active_file_info_last->line_current, instruction->string, ttt);
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
    y = g_source_pointer;
    g_source_pointer = g_inz;
    prev_i = g_source_pointer;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;

    if (z == INPUT_NUMBER_EOL) {
      /* roll back to the index before EOL */
      g_inz = prev_i;
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
  y = g_source_pointer;
  g_source_pointer = g_inz;
  z = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = y;

  if (z != INPUT_NUMBER_ADDRESS_LABEL) {
    print_error(ERROR_ERR, "Was expecting register D/X/Y/U/S/PC/A/B/CC/DP, got something else instead.\n");
    return -1;
  }

  data = _get_register_byte_from_label_exg_tfr();
  if (data < 0)
    return -1;
  register_byte = data;

  /* destination register */
  y = g_source_pointer;
  g_source_pointer = g_inz;
  z = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = y;

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


#ifdef SUPERFX

/* parse a number [min, max] */
int _parse_tiny_int(int min, int max) {

  int old_s, value, res;
  
  old_s = g_source_pointer;
  g_source_pointer = g_inz;
  res = input_number();
  g_inz = g_source_pointer;
  g_source_pointer = old_s;

  if (res != SUCCEEDED)
    return -1;

  value = g_parsed_int;

  if (value < min || value > max)
    return -1;
  
  return value;
}

#endif


int evaluate_token(void) {

  int f, z, x, y, last_stack_id_backup;
#if defined(Z80) || defined(SPC700) || defined(W65816) || defined(WDC65C02) || defined(CSG65CE02) || defined(HUC6280)
  int e, v, h;
  char labelx[MAX_NAME_LENGTH + 1];
#endif
#ifdef SPC700
  int g;
#endif
#ifdef HUC6280
  int r = 0, s, t = 0, u = 0;
  char labely[MAX_NAME_LENGTH + 1];
#endif
#ifdef SUPERFX
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

    add_label_to_label_stack(g_tmp);
    fprintf(g_file_out_ptr, "k%d L%s ", g_active_file_info_last->line_current, g_tmp);

    return SUCCEEDED;
  }

  /* INSTRUCTION? */
  g_ind = g_instruction_p[(unsigned char)g_tmp[0]];
  g_instruction_tmp = &g_instructions_table[g_ind];

  for (f = g_instruction_n[(unsigned char)g_tmp[0]]; f > 0; f--) {
#if W65816
    if (g_use_wdc_standard == 0) {
      /* skip all mnemonics that contain '<', '|' and '>' */
      for (g_inz = 0, g_parsed_int = SUCCEEDED; g_inz < INSTRUCTION_STRING_LENGTH_MAX; g_inz++) {
        char c = g_instruction_tmp->string[g_inz];

        if (c == 0)
          break;
        if (c == '<' || c == '|' || c == '>') {
          g_parsed_int = FAILED;
          break;
        }
      }

      if (g_parsed_int == FAILED) {
        /* try the next mnemonic in the array */
        g_instruction_tmp = &g_instructions_table[++g_ind];
        continue;
      }
    }
#endif
    
    /* try to match the first part of the mnemonic, already read into g_tmp */
    for (g_inz = 0, g_parsed_int = SUCCEEDED; g_inz < INSTRUCTION_STRING_LENGTH_MAX; g_inz++) {
      if (g_tmp[g_inz] == 0)
        break;
      if (g_instruction_tmp->string[g_inz] != toupper((int)g_tmp[g_inz])) {
        g_parsed_int = FAILED;
        break;
      }
    }

    if (g_parsed_int == FAILED) {
      /* try the next mnemonic in the array */
      g_instruction_tmp = &g_instructions_table[++g_ind];
      continue;
    }

    /* beginning matches the input */
    x = g_inz;
    g_inz = g_source_pointer;

    /* remember the last stack calculation created -> if we create new stack calculations in the
       following switch() and need to roll back after that we'll delete all stack calculations created
       after this stack id... */
    last_stack_id_backup = g_last_stack_id;

#ifdef SPC700
    /* does the instruction contain a dot? */
    /* NOTE: as instruction decoders call return, we'll need to set this variable later back to NO */
    g_input_number_expects_dot = g_instruction_tmp->has_dot;
#endif
    
    switch (g_instruction_tmp->type) {

#ifdef GB

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <GB-Z80> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;

          /* give a warning when assembling "JP (HL)"! */
          /*
            if (g_instruction_tmp->hex == 0xE9 && strcmp(g_instruction_tmp->string, "JP (HL)") == 0)
            print_error(ERROR_WRN, "\"JP (HL)\" is semantically incorrect. Please use \"JP HL\" instead.\n");
          */

          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          if (x > 0 && (g_instruction_tmp->string[x-1] == '+' || g_instruction_tmp->string[x-1] == '-'))
            g_source_pointer--;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
          
                if (g_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d v1 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);       
              
              /* reset to "no special case" */
              _output_assembled_instruction(g_instruction_tmp, "v0 ");
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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
 
#ifdef Z80

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <Z80> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          
          /* give a warning when assembling "JP (HL)"! */
          /*
            if (g_instruction_tmp->hex == 0xE9 && strcmp(g_instruction_tmp->string, "JP (HL)") == 0)
            print_error(ERROR_WRN, "\"JP (HL)\" is semantically incorrect. Please use \"JP HL\" instead.\n");
          */
          /* give a warning when assembling "JP (HL')"! */
          /*
            if (g_instruction_tmp->hex == 0xE9 && strcmp(g_instruction_tmp->string, "JP (HL')") == 0)
            print_error(ERROR_WRN, "\"JP (HL')\" is semantically incorrect. Please use \"JP HL'\" instead.\n");
          */
          
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 11:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 11) {
                  /* 11 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          
          /* give a warning when assembling "JP (IX)"! */
          /*
            if (g_instruction_tmp->hex == 0xE9DD && strcmp(g_instruction_tmp->string, "JP (IX)") == 0)
            print_error(ERROR_WRN, "\"JP (IX)\" is semantically incorrect. Please use \"JP IX\" instead.\n");
          */
          /* give a warning when assembling "JP (IY)"! */
          /*
            if (g_instruction_tmp->hex == 0xE9FD && strcmp(g_instruction_tmp->string, "JP (IY)") == 0)
            print_error(ERROR_WRN, "\"JP (IY)\" is semantically incorrect. Please use \"JP IY\" instead.\n");
          */
          
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "y%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "y%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "y%d d%d d%d ", g_instruction_tmp->hex, g_parsed_int, g_instruction_tmp->hex_x);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label, g_instruction_tmp->hex_x);
              else
                _output_assembled_instruction(g_instruction_tmp, "y%d c%d d%d ", g_instruction_tmp->hex, g_latest_stack, g_instruction_tmp->hex_x);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "y%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d y%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "y%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "y%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "y%d R%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "y%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 10:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              
              if (z == SUCCEEDED) {
                if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                    (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
                  print_error(ERROR_NUM, "Out of 8-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "y%d d%d d%d ", g_instruction_tmp->hex, g_parsed_int, g_instruction_tmp->hex_x);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label, g_instruction_tmp->hex_x);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "y%d c%d d%d ", g_instruction_tmp->hex, g_latest_stack, g_instruction_tmp->hex_x);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d v1 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);       
              
              /* reset to "no special case" */
              _output_assembled_instruction(g_instruction_tmp, "v0 ");
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* </Z80> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

#endif

#ifdef MCS6502

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6502> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 5:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
            if (g_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef WDC65C02

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65C02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 6:
    case 1:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 6) {
                  /* 6 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef CSG65CE02

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65CE02> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 6:
    case 1:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 6) {
                  /* 6 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 4:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 7:
      /* 16-bit relative */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 32767 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of signed 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d M%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                struct stack *stack;
                
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
                
                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);
                
                if (stack == NULL)
                  return FAILED;
                
                stack->relative_references = 1;
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef W65816

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <65816> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 11:
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 11) {
                  /* 11 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      if (g_xbit_size > 16 && g_instruction_tmp->skip_xbit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_24BIT && g_instruction_tmp->skip_xbit == 1)
            break;
          if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
            if (g_instruction_tmp->skip_xbit == 1)
              break;
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '&') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_STACK || z == INPUT_NUMBER_ADDRESS_LABEL))
            return FAILED;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d z%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d T%d ", g_instruction_tmp->hex, g_latest_stack);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

      /* IMMEDIATE VALUES: ACCUMULATOR */

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
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
    
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              /* 8BIT */
              if (y == 0) {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              }
              /* 16BIT */
              else {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

      /* MVN & MVP */

    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  /* NOTE: in the source code it's "MVP/MVN x1, x2", but we output "MVP/MVN x2, x1" */
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
            
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

      /* SEP & REP */

    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED) {
            if (g_instruction_tmp->skip_xbit == 0)
              print_error(ERROR_LOG, "REP needs immediate data.\n");
            else
              print_error(ERROR_LOG, "SEP needs immediate data.\n");
            return FAILED;
          }
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
        
              _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        
              /* REP */
              if (g_instruction_tmp->skip_xbit == 0) {
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
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

      /* IMMEDIATE VALUES: INDEX */

    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
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
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              /* 8BIT */
              if (y == 0) {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              }
              /* 16BIT */
              else {
                if (z == SUCCEEDED)
                  _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
                else
                  _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 32767))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d M%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xA:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_xbit == 2)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if ((g_operand_hint == HINT_16BIT || g_operand_hint == HINT_24BIT) && g_instruction_tmp->skip_xbit == 2)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
            if (g_instruction_tmp->skip_xbit == 2)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
    
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef MC6800

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6800> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (g_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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
      
#ifdef MC6801

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6801> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (g_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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
      
#ifdef MC6809

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <6809> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      /* plain text 8-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          if (g_instruction_tmp->hex > 0xFF)
            _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
          else
            _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 5:
    case 1:
      /* 8-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            print_error(ERROR_NUM, "Out of signed 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                if (g_instruction_tmp->type == 5) {
                  /* 5 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      /* 16-bit operand */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "r%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      /* plain text 16-bit */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      /* 8-bit unsigned operand, absolute address */
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
    
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < 0)) {
            if (g_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 6:
      /* 5-bit signed operand, absolute address + post op byte code */
      if (g_xbit_size >= 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;

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

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->addressing_mode_bits | (g_parsed_int & 0x1F));
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 7:
      /* 8-bit signed operand, relative address + post op byte code */
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;

          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (g_operand_hint == HINT_16BIT)
            break;
          if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
            if (g_instruction_tmp->skip_8bit == 1)
              break;
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
                
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->addressing_mode_bits, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->addressing_mode_bits, g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->addressing_mode_bits, g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 8:
      /* 16-bit operand + post op byte code */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->addressing_mode_bits, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->addressing_mode_bits, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->addressing_mode_bits, g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 9:
      /* plain text 8-bit + post op byte code */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

          if (g_instruction_tmp->hex > 0xFF)
            _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
          else
            _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->addressing_mode_bits);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 10:
      /* EXG / TFR */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'r') {
          g_parsed_int = _parse_exg_tfr_registers();
          if (g_parsed_int < 0)
            return FAILED;

          _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 11:
      /* PSHS / PSHU / PULS / PULU */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'r') {
          g_parsed_int = _parse_push_pull_registers(g_instruction_tmp->addressing_mode_bits);
          if (g_parsed_int < 0)
            return FAILED;

          _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 12:
      /* 16-bit signed operand, relative address */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_instruction_tmp->hex > 0xFF)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "M%s ", g_label);
              else {
                struct stack *stack;
          
                _output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);

                /* let's configure the stack so that all label references inside are relative */
                stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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
      
#ifdef I8008

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <8008> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          if (x > 0 && (g_instruction_tmp->string[x-1] == '+' || g_instruction_tmp->string[x-1] == '-'))
            g_source_pointer--;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
          
                if (g_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
    
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d v2 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);       

              /* reset to "no special case" */
              _output_assembled_instruction(g_instruction_tmp, "v0 ");
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef I8080

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <8080> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x' || g_instruction_tmp->string[x] == 's') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          if (x > 0 && (g_instruction_tmp->string[x-1] == '+' || g_instruction_tmp->string[x-1] == '-'))
            g_source_pointer--;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if ((g_instruction_tmp->string[x] == 'x' && (g_parsed_int > 255 || g_parsed_int < -128)) ||
                (g_instruction_tmp->string[x] == 's' && (g_parsed_int > 127 || g_parsed_int < -128))) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
          
                if (g_instruction_tmp->type == 4) {
                  /* 4 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                  if (stack == NULL)
                    return FAILED;
    
                  stack->relative_references = 1;
                }
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED || g_parsed_int != g_instruction_tmp->value)
            break;
    
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 100:
      /* "RST *" that gets delayed to WLALINK */
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            break;

          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d v3 ", g_active_file_info_last->line_current);
              if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);       

              /* reset to "no special case" */
              _output_assembled_instruction(g_instruction_tmp, "v0 ");
        
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef SPC700

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <SPC-700> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '~') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g_parsed_int << 5));

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '~') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (((g_parsed_int << 1) + 1) << 4));

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '~') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (z != SUCCEEDED)
            break;
          if (g_parsed_int > 0xF || g_parsed_int < 0)
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g_parsed_int << 4));
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xA:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);

                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                    if (stack == NULL)
                      return FAILED;
    
                    stack->relative_references = 1;

                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                  }

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xB:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);

                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xC:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '~') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;
              g = g_parsed_int;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 'x') {
                  y = g_source_pointer;
                  g_source_pointer = g_inz;
                  z = input_number();
                  g_inz = g_source_pointer;
                  g_source_pointer = y;
                  if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                    return FAILED;
                  if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                    break;

                  for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                    if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                      _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                      _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g << 5));

                      if (v == SUCCEEDED)
                        _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                      else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                      else
                        _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                      if (z == SUCCEEDED)
                        _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                      else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                      else {
                        struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                        if (stack == NULL)
                          return FAILED;

                        stack->relative_references = 1;

                        _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                      }

                      g_source_pointer = g_inz;
                      return SUCCEEDED;
                    }
                    if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                      break;
                  }
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xD:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '~') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;
              g = g_parsed_int;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 'x') {
                  y = g_source_pointer;
                  g_source_pointer = g_inz;
                  z = input_number();
                  g_inz = g_source_pointer;
                  g_source_pointer = y;
                  if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                    return FAILED;
                  if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                    break;

                  for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                    if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                      _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                      _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (((g << 1) + 1) << 4));

                      if (v == SUCCEEDED)
                        _output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                      else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                      else
                        _output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                      if (z == SUCCEEDED)
                        _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                      else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                        _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                      else {
                        struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                        if (stack == NULL)
                          return FAILED;

                        stack->relative_references = 1;

                        _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                      }

                      g_source_pointer = g_inz;
                      return SUCCEEDED;
                    }
                    if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                      break;
                  }
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xE:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->hex, g_label);
              else {
                struct stack *stack = find_stack_calculation(g_latest_stack, YES);

                if (stack == NULL)
                  return FAILED;
    
                stack->relative_references = 1;

                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              }

              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 0xF:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          g_parse_floats = NO;
          z = input_number();
          g_parse_floats = YES;
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 8191 || g_parsed_int < 0))
            break;

          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '~') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (z != SUCCEEDED)
                break;
              if (g_parsed_int > 7 || g_parsed_int < 0)
                break;

              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, e | g_parsed_int << 13);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d n%d %s ", g_instruction_tmp->hex, g_parsed_int, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d N%d %d ", g_instruction_tmp->hex, g_parsed_int, h);

                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef HUC6280

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <HUC-6280> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      
    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 9:
    case 1:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else {
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
                if (g_instruction_tmp->type == 9) {
                  /* 9 -> let's configure the stack so that all label references inside are relative */
                  struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                  
                  if (stack == NULL)
                    return FAILED;
                  
                  stack->relative_references = 1;
                }
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      v = -1;
      e = -1;
      h = -1;
      z = -1;
      s = 0;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
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
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      if (s == 0)
        break;

      for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
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
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      if (s == 1)
        break;

      for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
          s = 3;
          break;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      if (s == 2)
        break;

      for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

          /* first operand */
          if (v == SUCCEEDED)
            _output_assembled_instruction(g_instruction_tmp, "y%d ", e);
          else if (v == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(g_instruction_tmp, "r%s ", labelx);
          else if (v == INPUT_NUMBER_STACK)
            _output_assembled_instruction(g_instruction_tmp, "C%d ", h);
          else {
            fprintf(stderr, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }

          /* second operand */
          if (t == SUCCEEDED)
            _output_assembled_instruction(g_instruction_tmp, "y%d ", r);
          else if (t == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(g_instruction_tmp, "r%s ", labely);
          else if (t == INPUT_NUMBER_STACK)
            _output_assembled_instruction(g_instruction_tmp, "C%d ", u);
          else {
            fprintf(stderr, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }
          
          /* third operand */
          if (z == SUCCEEDED)
            _output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            _output_assembled_instruction(g_instruction_tmp, "r%s ", g_label);
          else if (z == INPUT_NUMBER_STACK)
            _output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);
          else {
            fprintf(stderr, "evaluate_token(): Sanity check failed! Please submit a bug report!\n");
            return FAILED;
          }
          
          g_source_pointer = g_inz;
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 5:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              if (g_operand_hint == HINT_16BIT)
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
    
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);

          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '?') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
                print_error(ERROR_NUM, "Out of 16-bit range.\n");
                return FAILED;
              }
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "r%s ", g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 7:
      if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
        break;
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          if (g_operand_hint == HINT_16BIT)
            break;
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
              else
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'x') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
            break;
          
          e = g_parsed_int;
          v = z;
          h = g_latest_stack;
          if (z == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(labelx, g_label);
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
                break;
              
              for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (v == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
                  else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                  else {
                    struct stack *stack;
                    
                    _output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
                    
                    /* let's configure the stack so that all label references inside are relative */
                    stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                  }
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

#ifdef SUPERFX

      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/
      /* <SUPER-FX> */
      /*************************************************************************************************/
      /*************************************************************************************************/
      /*************************************************************************************************/

    case 0:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
          _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
          if (g_instruction_tmp->prefix != 0)
            _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
          
          _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
          g_source_pointer = g_inz;
          
          return SUCCEEDED;
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 1:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
              if (g_instruction_tmp->prefix != 0)
                _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
              
              _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | tiny);
              
              g_source_pointer = g_inz;
              
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 2:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == 'e') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if (g_parsed_int > 127 || g_parsed_int < -128) {
              print_error(ERROR_NUM, "Out of 8-bit range.\n");
              return FAILED;
            }
          }
          
          for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
              _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
              if (z == SUCCEEDED)
                _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                _output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->hex, g_label);
              else {
                struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                
                if (stack == NULL)
                  return FAILED;
                
                stack->relative_references = 1;
                
                _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
              }
              
              g_source_pointer = g_inz;
              return SUCCEEDED;
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 3:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'x') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 255 || g_parsed_int < -128) {
                  print_error(ERROR_NUM, "Out of 8-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (g_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex | tiny, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->hex | tiny, g_label);
                  else {
                    struct stack *stack = find_stack_calculation(g_latest_stack, YES);
                    
                    if (stack == NULL)
                      return FAILED;
                    
                    stack->relative_references = 1;
                    
                    _output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex | tiny, g_latest_stack);
                  }
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 4:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '?') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (g_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex | tiny, g_parsed_int);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->hex | tiny, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex | tiny, g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 5:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == 'y') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
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
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (g_instruction_tmp->prefix != 0)
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
                  
                  if (z == SUCCEEDED)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex | tiny, g_parsed_int >> 1);
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d *%s ", g_instruction_tmp->hex | tiny, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d -%d ", g_instruction_tmp->hex | tiny, g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 6:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  if (g_instruction_tmp->hex == 0xB0) {
                    /* MOVES */
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix | tiny);
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | z);
                  }
                  else {
                    /* OTHERWISE */
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix | z);
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | tiny);
                  }
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 7:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '?') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if (g_parsed_int >= -128 && g_parsed_int <= 127) {
                      /* use IBT */
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", 0xA0 | tiny, g_parsed_int);
                    }
                    else {
                      /* use IWT */
                      _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", 0xF0 | tiny, g_parsed_int);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d r%s ", 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", 0xF0 | tiny, g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 8:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '?') {
              y = g_source_pointer;
              g_source_pointer = g_inz;
              z = input_number();
              g_inz = g_source_pointer;
              g_source_pointer = y;
              if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
                return FAILED;
              if (z == SUCCEEDED) {
                if (g_parsed_int > 65535 || g_parsed_int < -32768) {
                  print_error(ERROR_NUM, "Out of 16-bit range.\n");
                  return FAILED;
                }
              }
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if ((g_parsed_int >= 0 && g_parsed_int <= 510) && ((g_parsed_int & 1) == 0)) {
                      /* use LMS */
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d d%d ", 0x3D, 0xA0 | tiny, g_parsed_int >> 1);
                    }
                    else {
                      /* use LM */
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d y%d ", 0x3D, 0xF0 | tiny, g_parsed_int);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d r%s ", 0x3D, 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d C%d ", 0x3D, 0xF0 | tiny, g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 9:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '?') {
          y = g_source_pointer;
          g_source_pointer = g_inz;
          z = input_number();
          g_inz = g_source_pointer;
          g_source_pointer = y;
          if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
            return FAILED;
          if (z == SUCCEEDED) {
            if (g_parsed_int > 65535 || g_parsed_int < -32768) {
              print_error(ERROR_NUM, "Out of 16-bit range.\n");
              return FAILED;
            }
            y = g_parsed_int;
          }
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
              if (tiny < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == SUCCEEDED) {
                    if ((y >= 0 && y <= 510) && ((y & 1) == 0)) {
                      /* use SMS */
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d d%d ", 0x3E, 0xA0 | tiny, y >> 1);
                    }
                    else {
                      /* use SM */
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d y%d ", 0x3E, 0xF0 | tiny, y);
                    }
                  }
                  else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d r%s ", 0x3E, 0xF0 | tiny, g_label);
                  else
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d C%d ", 0x3E, 0xF0 | tiny, g_latest_stack);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 10:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(0, 11);
              if (z < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (tiny == 0) /* LDB (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", 0x3D, 0x40 | z);
                  else /* TO R* + LDB (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d d%d ", 0x10 | tiny, 0x3D, 0x40 | z);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 11:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(0, 11);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == 0) /* STB (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", 0x3D, 0x30 | tiny);
                  else /* FROM R* + STB (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d d%d ", 0xB0 | z, 0x3D, 0x30 | tiny);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 12:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(0, 11);
              if (z < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (tiny == 0) /* LDW (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", 0x40 | z);
                  else /* TO R* + LDW (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", 0x10 | tiny, 0x40 | z);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;
      
    case 13:
      for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
        if (g_instruction_tmp->string[x] == '*') {
          tiny = _parse_tiny_int(0, 11);
          if (tiny < 0)
            return FAILED;
          
          for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
            if (g_instruction_tmp->string[x] == '*') {
              z = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
              if (z < 0)
                return FAILED;
              
              for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                  _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                  
                  if (z == 0) /* STW (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d ", 0x30 | tiny);
                  else /* FROM R* + STW (R*) */
                    _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", 0xB0 | z, 0x30 | tiny);
                  
                  g_source_pointer = g_inz;
                  return SUCCEEDED;
                }
                if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                  break;
              }
            }
            if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
              break;
          }
        }
        if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
          break;
      }
      break;

    case 14:
      {
        char labelx[MAX_NAME_LENGTH + 1];
        int v, s;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
          if (g_instruction_tmp->string[x] == '?') {
            y = g_source_pointer;
            g_source_pointer = g_inz;
            z = input_number();
            g_inz = g_source_pointer;
            g_source_pointer = y;
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
            
            for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
              if (g_instruction_tmp->string[x] == '*') {
                tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
                if (tiny < 0)
                  return FAILED;
                
                for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                  if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                    _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                    
                    if (g_instruction_tmp->prefix != 0)
                      _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
                    
                    if (z == SUCCEEDED)
                      _output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex | tiny, v);
                    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                      _output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->hex | tiny, labelx);
                    else
                      _output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex | tiny, s);
                    
                    g_source_pointer = g_inz;
                    return SUCCEEDED;
                  }
                  if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                    break;
                }
              }
              if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                break;
            }
          }
          if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
            break;
        }
      }
      break;
      
    case 15:
      {
        char labelx[MAX_NAME_LENGTH + 1];
        int v, s;
        
        for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
          if (g_instruction_tmp->string[x] == 'y') {
            y = g_source_pointer;
            g_source_pointer = g_inz;
            z = input_number();
            g_inz = g_source_pointer;
            g_source_pointer = y;
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
            
            for (x++; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
              if (g_instruction_tmp->string[x] == '*') {
                tiny = _parse_tiny_int(g_instruction_tmp->min, g_instruction_tmp->max);
                if (tiny < 0)
                  return FAILED;
                
                for (x++ ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
                  if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
                    _output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                    
                    if (g_instruction_tmp->prefix != 0)
                      _output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->prefix);
                    
                    if (z == SUCCEEDED)
                      _output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex | tiny, v >> 1);
                    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                      _output_assembled_instruction(g_instruction_tmp, "d%d *%s ", g_instruction_tmp->hex | tiny, labelx);
                    else
                      _output_assembled_instruction(g_instruction_tmp, "d%d -%d ", g_instruction_tmp->hex | tiny, s);
                    
                    g_source_pointer = g_inz;
                    return SUCCEEDED;
                  }
                  if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                    break;
                }
              }
              if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
                break;
            }
          }
          if (g_instruction_tmp->string[x] != toupper((int)g_buffer[g_inz]))
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

    g_instruction_tmp = &g_instructions_table[++g_ind];
  }

  /* allow error messages from input_numbers() */
  g_input_number_error_msg = YES;

  return EVALUATE_TOKEN_NOT_IDENTIFIED;
}

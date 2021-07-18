
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "parse.h"
#include "pass_1.h"
#include "stack.h"
#include "include_file.h"
#include "printf.h"


int parse_string_length(char *end);

int g_input_number_error_msg = YES, g_ss, g_string_size, g_input_float_mode = OFF, g_parse_floats = YES;
int g_expect_calculations = YES, g_input_parse_if = NO, g_input_allow_leading_hashtag = NO, g_input_has_leading_hashtag = NO;
int g_input_allow_leading_ampersand = NO, g_plus_and_minus_ends_label = NO;
int g_newline_beginning = ON, g_parsed_double_decimal_numbers = 0, g_operand_hint, g_operand_hint_type;
char g_label[MAX_NAME_LENGTH + 1], g_xyz[512];
char g_unevaluated_expression[256];
char g_expanded_macro_string[MAX_NAME_LENGTH + 1];
double g_parsed_double;

extern int g_source_pointer, g_size, g_parsed_int, g_macro_active;
extern char *g_buffer, g_tmp[4096], g_current_directive[256], *g_label_stack[256];
extern unsigned char g_asciitable[256];
extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;
extern struct definition *g_tmp_def;
extern struct map_t *g_defines_map;
extern struct macro_runtime *g_macro_stack, *g_macro_runtime_current;
extern int g_latest_stack, g_asciitable_defined, g_global_label_hint;


int is_string_ending_with(char *s, char *e) {

  int length_s, length_e, k;
  
  if (s == NULL || e == NULL)
    return -1;

  length_s = (int)strlen(s);
  length_e = (int)strlen(e);

  if (length_e > length_s)
    return -1;

  for (k = 0; k < length_e; k++) {
    if (s[length_s - length_e + k] != e[k])
      return -1;
  }

  return 1;
}


int strcaselesscmpn(char *s1, char *s2, int length) {

  int n = 0;
  
  if (s1 == NULL || s2 == NULL)
    return 0;

  while (*s1 != 0 && *s2 != 0) {
    if (toupper((int)*s1) != toupper((int)*s2))
      return 1;
    s1++;
    s2++;

    n++;
    if (n >= length)
      return 0;
  }

  if (*s2 != 0)
    return 1;
  if (*s1 != 0)
    return -1;

  return 0;
}


int compare_next_token(char *token) {

  int ii, t, d, k, length;
  char e;
  
  length = (int)strlen(token);

  /* skip white space */
  ii = g_source_pointer;
  for (e = g_buffer[ii]; e == ' ' || e == ',' || e == '\\' || e == 0x0A; e = g_buffer[++ii]) {
    if (e == '\\' && g_buffer[ii + 1] != 0x0A)
      break;
  }

  /* HACK: if token == '=', then we'll take this route. in general compare_next_token() should be
     called with an alphabetical string... */
  if (length == 1 && token[0] == '=') {
    if (e == '=')
      return SUCCEEDED;
  }
  
  /* MACRO mode? */
  if (g_macro_active != 0 && e == '\\') {
    if (g_buffer[ii + 1] == '@') {
      char tmp_buffer[64];

      snprintf(tmp_buffer, sizeof(tmp_buffer), "%d", g_macro_runtime_current->macro->calls - 1);

      e = tmp_buffer[0];
      for (t = 0; t < length && e != 0; ) {
        if (toupper((int)token[t]) != toupper((int)e))
          return FAILED;
        t++;
        e = tmp_buffer[t];
      }
    }
    else if (g_buffer[ii + 1] == '?') {
      /* TODO: do we need to implement this? */
      return FAILED;
    }
    else {
      for (d = 0, k = 0; k < 16; k++) {
        e = g_buffer[++ii];
        if (e >= '0' && e <= '9')
          d = (d * 10) + e - '0';
        else
          break;
      }

      if (d > g_macro_runtime_current->supplied_arguments) {
        if (g_input_number_error_msg == YES) {
          snprintf(g_xyz, sizeof(g_xyz), "COMPARE_NEXT_SYMBOL: Macro \"%s\" wasn't called with enough arguments.\n", g_macro_runtime_current->macro->name);
          print_error(g_xyz, ERROR_NONE);
        }
        return FAILED;
      }

      ii = g_macro_runtime_current->argument_data[d - 1]->start;

      e = g_buffer[ii];
      for (t = 0; t < length && e != ' ' && e != ',' && e != 0x0A; ) {
        if (toupper((int)token[t]) != toupper((int)e))
          return FAILED;
        t++;
        e = g_buffer[++ii];
      }
    }
  }
  /* not in MACRO mode */
  else {
    for (t = 0; t < length && e != ' ' && e != ',' && e != 0x0A; ) {
      if (toupper((int)token[t]) != toupper((int)e))
        return FAILED;
      t++;
      e = g_buffer[++ii];
    }

    /* token has been terminated, but the other string has not? */
    if (t == length && e != ' ' && e != ',' && e != 0x0A)
      return FAILED;
  }

  if (t == length)
    return SUCCEEDED;

  return FAILED;
}


int input_next_string(void) {

  char e;
  int k;
  
  /* skip white space */
  for (e = g_buffer[g_source_pointer++]; e == ' ' || e == ','; e = g_buffer[g_source_pointer++])
    ;

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  /* we assume it is a label */
  g_tmp[0] = e;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = g_buffer[g_source_pointer++];
    if (e == 0x0A || e == ',') {
      g_source_pointer--;
      break;
    }
    else if (e == ' ')
      break;
    g_tmp[k] = e;
  }

  if (k == MAX_NAME_LENGTH) {
    if (g_input_number_error_msg == YES) {
      snprintf(g_xyz, sizeof(g_xyz), "The string is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
      print_error(g_xyz, ERROR_NUM);
    }
    return FAILED;
  }

  g_tmp[k] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_tmp) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


static int _input_number_return_definition(struct definition *def) {

  if (def->type == DEFINITION_TYPE_VALUE) {
    g_parsed_int = (int)def->value;

    if (g_operand_hint == HINT_NONE) {
      if (g_parsed_int > 0xFFFFFF)
        g_operand_hint = HINT_32BIT;
      else if (g_parsed_int > 0xFFFF && g_parsed_int <= 0xFFFFFF)
        g_operand_hint = HINT_24BIT;
      else if (g_parsed_int > 0xFF)
        g_operand_hint = HINT_16BIT;
      else
        g_operand_hint = HINT_8BIT;
      
      g_operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
      /* 5-bit values need this */
      if (g_parsed_int >= -16 && g_parsed_int <= 15) {
        g_operand_hint = HINT_NONE;
        g_operand_hint_type = HINT_TYPE_NONE;
      }
#endif
    }
      
    g_parsed_double = (double)g_parsed_int;
    
    return SUCCEEDED;
  }
  else if (def->type == DEFINITION_TYPE_STACK) {
    /* wrap the referenced, existing stack calculation inside a new stack calculation as stack
       calculation contains a write. the 2nd, 3rd etc. reference don't do anything by themselves.
       but wrapping creates a new stack calculation that also makes a write */
    stack_create_stack_stack((int)def->value);
    return INPUT_NUMBER_STACK;
  }
  else if (def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
    if (g_label[0] == ':') {
      /* we need to keep the ':' prefix */
      if (strlen(def->string) >= MAX_NAME_LENGTH-1) {
        if (g_input_number_error_msg == YES) {
          snprintf(g_xyz, sizeof(g_xyz), "The label is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
          print_error(g_xyz, ERROR_NUM);
        }
        return FAILED;          
      }
      snprintf(g_label, sizeof(g_label), ":%.254s", def->string);
      g_string_size = def->size + 1;
    }
    else {
      g_string_size = def->size;
      memcpy(g_label, def->string, g_string_size);
      g_label[g_string_size] = 0;
    }
  }
  else {
    g_string_size = def->size;
    memcpy(g_label, def->string, g_string_size);
    g_label[g_string_size] = 0;
    
    return INPUT_NUMBER_STRING;
  }

  /* are labels 16-bit by default? */
  if (g_operand_hint_type != HINT_TYPE_GIVEN && g_global_label_hint == HINT_16BIT) {
    g_operand_hint = HINT_16BIT;
    g_operand_hint_type = HINT_TYPE_GIVEN;
  }

  process_special_labels(g_label);
  
  return INPUT_NUMBER_ADDRESS_LABEL;      
}


int input_number(void) {

  char label_tmp[MAX_NAME_LENGTH + 1];
  unsigned char e, ee;
  int k, p, q, spaces = 0, curly_braces = 0, check_if_a_definition = YES, can_have_calculations = YES;
  double decimal_mul;
#ifdef SPC700
  int dot = 0;
#endif


  g_operand_hint = HINT_NONE;
  g_operand_hint_type = HINT_TYPE_NONE;
  g_input_has_leading_hashtag = NO;
      
  /* skip white space */
  for (e = g_buffer[g_source_pointer++]; e == ' ' || e == ',' || e == '\\'; e = g_buffer[g_source_pointer++]) {
    if (e == '\\' && g_buffer[g_source_pointer] == 0xA) {
      g_source_pointer++;
      next_line();
    }
    else if (e == '\\' && g_buffer[g_source_pointer] != 0xA)
      break;
  }

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  /* are we parsing a macro argument, and could it begin with a '#' (ARG_IMMEDIATE)? */
  if (g_input_allow_leading_hashtag == YES) {
    if (e == '#') {
      g_input_has_leading_hashtag = YES;
      e = g_buffer[g_source_pointer++];
    }
  }

  /* are we parsing a macro argument, and could it begin with a '&' (return
     label as it is, don't check if it's a definition) */
  if (g_input_allow_leading_ampersand == YES) {
    if (e == '&') {
      check_if_a_definition = NO;
      can_have_calculations = NO;
      e = g_buffer[g_source_pointer++];
    }
  }

  if (g_expect_calculations == YES && can_have_calculations == YES) {
    /* check the type of the expression */
    p = g_source_pointer;
    ee = e;
    while (ee != 0x0A) {
      if (ee == '{')
        curly_braces++;
      else if (ee == '}')
        curly_braces--;
      /* string / symbol -> no calculating */
      else if (g_input_parse_if == NO && ee == '"')
        break;
      else if (ee == ',')
        break;
      else if (g_input_parse_if == NO && ((ee == '=' && g_buffer[p] == '=') || (ee == '!' && g_buffer[p] == '=')))
        break;
      else if (ee == ' ')
        spaces++;
      else if (curly_braces <= 0 && (ee == '-' || ee == '+' || ee == '*' || ee == '/' || ee == '&' || ee == '|' || ee == '^' || ee == '(' ||
                                     ee == '<' || ee == '>' || ee == '#' || ee == '~' || ee == ':' || ee == '!' || (g_input_parse_if == YES && ee == '='))) {
        if (ee == ':' && spaces > 0)
          break;
        
        /* launch stack calculator */
        p = stack_calculate(&g_buffer[g_source_pointer - 1], &g_parsed_int);

        if (p == STACK_CALCULATE_DELAY)
          break;
        else if (p == STACK_RETURN_LABEL)
          return INPUT_NUMBER_ADDRESS_LABEL;
        else
          return p;
      }
      ee = g_buffer[p];
      p++;
    }
  }

  /* MACRO */
  if (g_macro_active != 0 && e == '?' && g_buffer[g_source_pointer] >= '1' && g_buffer[g_source_pointer] <= '9') {
    struct macro_argument *ma;
    
    for (g_parsed_int = 0, k = 0; k < 4; k++) {
      e = g_buffer[g_source_pointer++];
      if (e >= '0' && e <= '9')
        g_parsed_int = (g_parsed_int * 10) + (e - '0');
      else {
        g_source_pointer--;
        break;
      }
    }

    if (g_parsed_int > g_macro_runtime_current->supplied_arguments) {
      snprintf(g_xyz, sizeof(g_xyz), "Referencing argument number %d inside macro \"%s\". The macro has only %d arguments.\n", g_parsed_int, g_macro_runtime_current->macro->name, g_macro_runtime_current->supplied_arguments);
      print_error(g_xyz, ERROR_NUM);
      return FAILED;
    }
    if (g_parsed_int == 0) {
      snprintf(g_xyz, sizeof(g_xyz), "Referencing argument number %d inside macro \"%s\". Macro arguments are counted from 1.\n", g_parsed_int, g_macro_runtime_current->macro->name);
      print_error(g_xyz, ERROR_NUM);
      return FAILED;
    }

    /* use the macro argument to find its definition */
    ma = g_macro_runtime_current->argument_data[g_parsed_int - 1];
    k = ma->type;

    if (k == INPUT_NUMBER_ADDRESS_LABEL) {
      strcpy(g_label, ma->string);

      hashmap_get(g_defines_map, g_label, (void*)&g_tmp_def);
      if (g_tmp_def != NULL)
        return _input_number_return_definition(g_tmp_def);
      else {
        snprintf(g_xyz, sizeof(g_xyz), "Cannot find definition for \"%s\".\n", g_label);
        print_error(g_xyz, ERROR_NUM);
        return FAILED;
      }
    }
    else {
      print_error("? can be only used to evaluate definitions.\n", ERROR_ERR);
      return FAILED;
    }
  }
  if (g_macro_active != 0 && e == '\\') {
    struct macro_argument *ma;
    int exit_here = YES;
    int start_i = g_source_pointer;
    unsigned char start_e = e;

    if (g_buffer[g_source_pointer] == '@') {
      g_source_pointer++;
      g_parsed_int = g_macro_runtime_current->macro->calls - 1;

      if (g_buffer[g_source_pointer] != ' ' && g_buffer[g_source_pointer] != 0xA && g_buffer[g_source_pointer] != ',')
        exit_here = NO;
      else
        return SUCCEEDED;
    }
    else if (g_buffer[g_source_pointer] >= '0' && g_buffer[g_source_pointer] <= '9') {
      for (g_parsed_int = 0, k = 0; k < 4; k++) {
        e = g_buffer[g_source_pointer++];
        if (e >= '0' && e <= '9')
          g_parsed_int = (g_parsed_int * 10) + (e - '0');
        else {
          g_source_pointer--;
          break;
        }
      }

      if (g_buffer[g_source_pointer] != ' ' && g_buffer[g_source_pointer] != 0xA && g_buffer[g_source_pointer] != ',' && g_buffer[g_source_pointer] != '.')
        exit_here = NO;
    }
    else
      exit_here = NO;

    if (exit_here == YES) {
      if (g_parsed_int > g_macro_runtime_current->supplied_arguments) {
        snprintf(g_xyz, sizeof(g_xyz), "Referencing argument number %d inside macro \"%s\". The macro has only %d arguments.\n", g_parsed_int, g_macro_runtime_current->macro->name, g_macro_runtime_current->supplied_arguments);
        print_error(g_xyz, ERROR_NUM);
        return FAILED;
      }
      if (g_parsed_int == 0) {
        snprintf(g_xyz, sizeof(g_xyz), "Referencing argument number %d inside macro \"%s\". Macro arguments are counted from 1.\n", g_parsed_int, g_macro_runtime_current->macro->name);
        print_error(g_xyz, ERROR_NUM);
        return FAILED;
      }

      /* return the macro argument */
      ma = g_macro_runtime_current->argument_data[g_parsed_int - 1];
      k = ma->type;
      g_input_has_leading_hashtag = ma->has_leading_hashtag;
      
      if (k == INPUT_NUMBER_ADDRESS_LABEL) {
        strcpy(g_label, ma->string);
        process_special_labels(g_label);
      }
      else if (k == INPUT_NUMBER_STRING) {
        strcpy(g_label, ma->string);
        g_string_size = (int)strlen(ma->string);
      }
      else if (k == INPUT_NUMBER_STACK)
        g_latest_stack = (int)ma->value;
      else if (k == SUCCEEDED) {
        g_parsed_int = (int)ma->value;
        g_parsed_double = ma->value;
      }
      else {
        print_error("Macro argument list has been corrupted! Please send a bug report!\n", ERROR_ERR);
        return FAILED;
      }

      /* does the MACRO argument number end with a .b/.w/.l/.d? */
      if (e == '.') {
        e = g_buffer[g_source_pointer+1];
        if (e == 'b' || e == 'B') {
          g_operand_hint = HINT_8BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
        else if (e == 'w' || e == 'W') {
          g_operand_hint = HINT_16BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
        else if (e == 'l' || e == 'L') {
          g_operand_hint = HINT_24BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
        else if (e == 'd' || e == 'D') {
          g_operand_hint = HINT_32BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
      }

      return k;
    }
    else {
      g_source_pointer = start_i;
      e = start_e;
    }
  }

  /* is it a hexadecimal value? */
  g_parsed_int = 0;
  if (e >= '0' && e <= '9') {
    for (k = 0; 1; k++) {
      if (g_buffer[g_source_pointer+k] >= '0' && g_buffer[g_source_pointer+k] <= '9')
        continue;
      if (g_buffer[g_source_pointer+k] >= 'a' && g_buffer[g_source_pointer+k] <= 'f')
        continue;
      if (g_buffer[g_source_pointer+k] >= 'A' && g_buffer[g_source_pointer+k] <= 'F')
        continue;
      if (g_buffer[g_source_pointer+k] == 'h' || g_buffer[g_source_pointer+k] == 'H') {
        g_parsed_int = 1;
        break;
      }
      break;
    }
  }

  if (e == '$' || g_parsed_int == 1 || (e == '0' && (g_buffer[g_source_pointer] == 'x' || g_buffer[g_source_pointer] == 'X'))) {
    if (g_parsed_int == 1)
      g_source_pointer--;
    else if (e == '0')
      g_source_pointer++;
    for (g_parsed_int = 0, k = 0; k < 8; k++, g_source_pointer++) {
      e = g_buffer[g_source_pointer];
      if (e >= '0' && e <= '9')
        g_parsed_int = (g_parsed_int << 4) + e - '0';
      else if (e >= 'A' && e <= 'F')
        g_parsed_int = (g_parsed_int << 4) + e - 'A' + 10;
      else if (e >= 'a' && e <= 'f')
        g_parsed_int = (g_parsed_int << 4) + e - 'a' + 10;
      else if (e == 'h' || e == 'H') {
        g_source_pointer++;
        e = g_buffer[g_source_pointer];
        break;
      }
      else
        break;
    }

    if (e == '.') {
      e = g_buffer[g_source_pointer+1];
      if (e == 'b' || e == 'B') {
        g_operand_hint = HINT_8BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'w' || e == 'W') {
        g_operand_hint = HINT_16BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'l' || e == 'L') {
        g_operand_hint = HINT_24BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'd' || e == 'D') {
        g_operand_hint = HINT_32BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
    }

    if (g_operand_hint == HINT_NONE) {
      if (g_parsed_int > 0xFFFFFF)
        g_operand_hint = HINT_32BIT;
      else if (g_parsed_int > 0xFFFF && g_parsed_int <= 0xFFFFFF)
        g_operand_hint = HINT_24BIT;
      else if (g_parsed_int > 0xFF)
        g_operand_hint = HINT_16BIT;
      else
        g_operand_hint = HINT_8BIT;

      g_operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
      /* 5-bit values need this */
      if (g_parsed_int >= -16 && g_parsed_int <= 15) {
        g_operand_hint = HINT_NONE;
        g_operand_hint_type = HINT_TYPE_NONE;
      }
#endif
    }
    
    g_parsed_double = (double)g_parsed_int;
    
    return SUCCEEDED;
  }

  if (e == '%' || (e == '0' && (g_buffer[g_source_pointer] == 'b' || g_buffer[g_source_pointer] == 'B'))) {
    if (e == '0')
      g_source_pointer++;
    for (g_parsed_int = 0, k = 0; k < 32; k++, g_source_pointer++) {
      e = g_buffer[g_source_pointer];
      if (e == '0' || e == '1')
        g_parsed_int = (g_parsed_int << 1) + e - '0';
      else
        break;
    }

    if (e == '.') {
      e = g_buffer[g_source_pointer+1];
      if (e == 'b' || e == 'B') {
        g_operand_hint = HINT_8BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'w' || e == 'W') {
        g_operand_hint = HINT_16BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'l' || e == 'L') {
        g_operand_hint = HINT_24BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'd' || e == 'D') {
        g_operand_hint = HINT_32BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
    }

    g_parsed_double = (double)g_parsed_int;

    return SUCCEEDED;
  }
  
  if (e >= '0' && e <= '9') {
    int max_digits = 10;
    
    /* we are parsing decimals when q == 1 */
    q = 0;
    g_parsed_double = e-'0';
    g_parsed_double_decimal_numbers = 0;
    decimal_mul = 0.1;
    for (k = 0; k < max_digits; k++, g_source_pointer++) {
      e = g_buffer[g_source_pointer];
      if (e >= '0' && e <= '9') {
        if (k == max_digits - 1) {
          if (q == 0)
            print_error("Too many digits in the integer value. Max 10 is supported.\n", ERROR_NUM);
          else {
            snprintf(g_xyz, sizeof(g_xyz), "Too many digits in the floating point value. Max %d is supported.\n", MAX_FLOAT_DIGITS);
            print_error(g_xyz, ERROR_NUM);
          }
          return FAILED;
        }
        
        if (q == 0) {
          /* still parsing an integer */
          g_parsed_double = g_parsed_double*10 + e-'0';
        }
        else {
          g_parsed_double = g_parsed_double + decimal_mul*(e-'0');
          decimal_mul /= 10.0;
          g_parsed_double_decimal_numbers = g_parsed_double_decimal_numbers*10 + (e-'0');
        }
      }
      else if (e == '.') {
        if (q == 1) {
          print_error("Syntax error.\n", ERROR_NUM);
          return FAILED;
        }
        e = g_buffer[g_source_pointer+1];
        if (e >= '0' && e <= '9') {
          /* float mode, read decimals */
          if (g_parse_floats == NO)
            break;
          q = 1;
          max_digits = MAX_FLOAT_DIGITS+1;
        }
        else if (e == 'b' || e == 'B') {
          g_operand_hint = HINT_8BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
        else if (e == 'w' || e == 'W') {
          g_operand_hint = HINT_16BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
        else if (e == 'l' || e == 'L') {
          g_operand_hint = HINT_24BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
        else if (e == 'd' || e == 'D') {
          g_operand_hint = HINT_32BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
      }
      else if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z')) {
        /* a number directly followed by a letter when parsing a integer/float -> syntax error */
        print_error("Syntax error.\n", ERROR_NUM);
        return FAILED;
      }
      else
        break;
    }

    /* drop the decimals */
    g_parsed_int = (int)g_parsed_double;

    if (g_operand_hint == HINT_NONE) {
      if (g_parsed_int > 0xFFFFFF)
        g_operand_hint = HINT_32BIT;
      else if (g_parsed_int > 0xFFFF && g_parsed_int <= 0xFFFFFF)
        g_operand_hint = HINT_24BIT;
      else if (g_parsed_int > 0xFF)
        g_operand_hint = HINT_16BIT;
      else
        g_operand_hint = HINT_8BIT;

      g_operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
      /* 5-bit values need this */
      if (g_parsed_int >= -16 && g_parsed_int <= 15) {
        g_operand_hint = HINT_NONE;
        g_operand_hint_type = HINT_TYPE_NONE;
      }
#endif
    }
    
    if (q == 1 && g_input_float_mode == ON)
      return INPUT_NUMBER_FLOAT;

    return SUCCEEDED;
  }
  
  if (e == '\'') {
    if (g_buffer[g_source_pointer + 1] == '\'') {
      /* '?' */
      g_parsed_int = g_buffer[g_source_pointer];
      g_source_pointer += 2;
    }
    else if (g_buffer[g_source_pointer] == '\\' && (g_buffer[g_source_pointer+1] == 't' ||
                                                    g_buffer[g_source_pointer+1] == 'r' ||
                                                    g_buffer[g_source_pointer+1] == 'n' ||
                                                    g_buffer[g_source_pointer+1] == '0') && g_buffer[g_source_pointer+2] == '\'') {
      /* '\?' */
      g_source_pointer++;
      if (g_buffer[g_source_pointer] == 't')
        g_parsed_int = '\t';
      else if (g_buffer[g_source_pointer] == 'r')
        g_parsed_int = '\r';
      else if (g_buffer[g_source_pointer] == 'n')
        g_parsed_int = '\n';
      else if (g_buffer[g_source_pointer] == '0')
        g_parsed_int = '\0';
      g_source_pointer += 2;
    }
    else {
      if (g_input_number_error_msg == YES)
        print_error("Malformed '?' detected!\n", ERROR_NUM);
      return FAILED;
    }

    g_parsed_double = (double)g_parsed_int;
    
    return SUCCEEDED;
  }

  if (e == '"') {
    for (k = 0; k < MAX_NAME_LENGTH; ) {
      e = g_buffer[g_source_pointer++];

      if (e == '\\' && g_buffer[g_source_pointer] == '"') {
        g_label[k++] = '"';
        g_source_pointer++;
        continue;
      }

      if (e == '"') {
        /* check for "string".length */
        if (g_buffer[g_source_pointer+0] == '.' &&
            (g_buffer[g_source_pointer+1] == 'l' || g_buffer[g_source_pointer+1] == 'L') &&
            (g_buffer[g_source_pointer+2] == 'e' || g_buffer[g_source_pointer+2] == 'E') &&
            (g_buffer[g_source_pointer+3] == 'n' || g_buffer[g_source_pointer+3] == 'N') &&
            (g_buffer[g_source_pointer+4] == 'g' || g_buffer[g_source_pointer+4] == 'G') &&
            (g_buffer[g_source_pointer+5] == 't' || g_buffer[g_source_pointer+5] == 'T') &&
            (g_buffer[g_source_pointer+6] == 'h' || g_buffer[g_source_pointer+6] == 'H')) {
          /* yes, we've got it! calculate the length and return the integer */
          g_source_pointer += 7;
          g_label[k] = 0;
          g_parsed_int = (int)get_label_length(g_label);
          g_parsed_double = (double)g_parsed_int;

          return SUCCEEDED;
        }

        if (e == '"')
          break;
      }
      
      if (e == 0 || e == 0x0A) {
        print_error("String wasn't terminated properly.\n", ERROR_NUM);
        return FAILED;
      }

      g_label[k++] = e;
    }

    g_label[k] = 0;

    /* expand e.g., \1 and \@ */
    if (g_macro_active != 0) {
      if (expand_macro_arguments(g_label) == FAILED)
        return FAILED;
      k = (int)strlen(g_label);
    }

    if (process_string_for_special_characters(g_label, &k) == FAILED)
      return FAILED;

    if (k >= MAX_NAME_LENGTH) {
      if (g_input_number_error_msg == YES) {
        snprintf(g_xyz, sizeof(g_xyz), "The string is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
        print_error(g_xyz, ERROR_NUM);
      }
      return FAILED;
    }

    g_label[k] = 0;
    g_string_size = k;

    return INPUT_NUMBER_STRING;
  }

  /* the last choice is a label */
  g_label[0] = e;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = g_buffer[g_source_pointer++];
    if (e == 0x0A || e == ')' || e == ',' || e == ']') {
      g_source_pointer--;
      break;
    }
    if (g_plus_and_minus_ends_label == YES && (e == '-' || e == '+')) {
      g_source_pointer--;
      break;
    }
#ifdef SPC700
    else if (e == '.')
      dot = k;
#endif
    else if (e == ' ')
      break;
    g_label[k] = e;

    /* is it actually a function? */
    if (k == 3 && strcaselesscmpn(g_label, "asc('", 4) == 0) {
      int parsed_chars = 0;
      
      if (parse_function_asc(&g_buffer[g_source_pointer], &g_parsed_int, &parsed_chars) == FAILED)
        return FAILED;
      
      g_source_pointer += parsed_chars;
      g_parsed_double = (double)g_parsed_int;

      return SUCCEEDED;
    }
    if (k == 7 && strcaselesscmpn(g_label, "defined(", 8) == 0) {
      int parsed_chars = 0;
          
      if (parse_function_defined(&g_buffer[g_source_pointer], &g_parsed_int, &parsed_chars) == FAILED)
        return FAILED;
      
      g_source_pointer += parsed_chars;
      g_parsed_double = (double)g_parsed_int;

      return SUCCEEDED;
    }
    if (k == 6 && strcaselesscmpn(g_label, "exists(", 7) == 0) {
      int parsed_chars = 0;
          
      if (parse_function_exists(&g_buffer[g_source_pointer], &g_parsed_int, &parsed_chars) == FAILED)
        return FAILED;
      
      g_source_pointer += parsed_chars;
      g_parsed_double = (double)g_parsed_int;

      return SUCCEEDED;
    }
  }

  if (k == MAX_NAME_LENGTH) {
    if (g_input_number_error_msg == YES) {
      snprintf(g_xyz, sizeof(g_xyz), "The label is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
      print_error(g_xyz, ERROR_NUM);
    }
    return FAILED;
  }

  /* size hint? */
  if (g_label[k-2] == '.') {
    if (g_label[k-1] == 'b' || g_label[k-1] == 'B') {
      g_operand_hint = HINT_8BIT;
      g_operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
    else if (g_label[k-1] == 'w' || g_label[k-1] == 'W') {
      g_operand_hint = HINT_16BIT;
      g_operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
    else if (g_label[k-1] == 'l' || g_label[k-1] == 'L') {
      g_operand_hint = HINT_24BIT;
      g_operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
    else if (g_label[k-1] == 'd' || g_label[k-1] == 'D') {
      g_operand_hint = HINT_32BIT;
      g_operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
#ifdef SPC700
    else if (g_label[k-1] >= '0' && g_label[k-1] <= '7') {
      k -= 2;
      g_source_pointer -= 2;
    }
#endif
  }
#ifdef SPC700
  else if (dot > 0) {
    g_source_pointer -= k - dot;
    k -= k - dot;
  }
#endif
  
  g_label[k] = 0;
  
  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_label) == FAILED)
      return FAILED;
    k = (int)strlen(g_label);
  }

  /* label_tmp contains the label without possible prefix ':' */
  if (strlen(g_label) > 1 && g_label[0] == ':')
    strcpy(label_tmp, &g_label[1]);
  else
    strcpy(label_tmp, g_label);

  /* check for "string".length */
  if (strstr(g_label, ".length") != NULL) {
    parse_string_length(strstr(g_label, ".length"));
    return SUCCEEDED;
  }
  else if (strstr(g_label, ".LENGTH") != NULL) {
    parse_string_length(strstr(g_label, ".LENGTH"));
    return SUCCEEDED;
  }

  if (check_if_a_definition == YES) {
    /* check if the label is actually a definition */
    if (hashmap_get(g_defines_map, g_label, (void*)&g_tmp_def) != MAP_OK)
      hashmap_get(g_defines_map, label_tmp, (void*)&g_tmp_def);
    if (g_tmp_def != NULL)
      return _input_number_return_definition(g_tmp_def);
  }

  /* are labels 16-bit by default? */
  if (g_operand_hint_type != HINT_TYPE_GIVEN && g_global_label_hint == HINT_16BIT) {
    g_operand_hint = HINT_16BIT;
    g_operand_hint_type = HINT_TYPE_GIVEN;
  }

  process_special_labels(g_label);
  
  return INPUT_NUMBER_ADDRESS_LABEL;
}


int process_special_labels(char *label) {

  if (strcaselesscmp(label, "__label__") == 0) {
    /* we have a reference to g_label_stack[0]! */
    strcpy(label, g_label_stack[0]);

    /* if we are processing g_label, update g_string_size as well */
    if (label == g_label)
      g_string_size = (int)strlen(label);
  }

  return SUCCEEDED;
}


int parse_string_length(char *end) {

  /* remove ".length" from the end of label (end points to inside of label) */
  end[0] = 0;

  /* check if the label is actually a definition - it should be or else we'll give an error */
  hashmap_get(g_defines_map, g_label, (void*)&g_tmp_def);
  
  if (g_tmp_def != NULL) {
    if (g_tmp_def->type == DEFINITION_TYPE_VALUE) {
      if (g_input_number_error_msg == YES) {
        print_error(".length of a value does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else if (g_tmp_def->type == DEFINITION_TYPE_STACK) {
      if (g_input_number_error_msg == YES) {
        print_error(".length of a pending computation does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else if (g_tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      if (g_input_number_error_msg == YES) {
        print_error(".length of an address label does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else {
      g_string_size = g_tmp_def->size;
      memcpy(g_label, g_tmp_def->string, g_string_size);
      g_label[g_string_size] = 0;

      g_parsed_int = (int)strlen(g_label);
      g_parsed_double = (double)g_parsed_int;
          
      return SUCCEEDED;
    }
  }

  return FAILED;
}


void skip_whitespace(void) {

  while (1) {
    if (g_source_pointer >= g_size)
      break;
    if (g_buffer[g_source_pointer] == ' ' || (g_buffer[g_source_pointer] == '\\' && g_buffer[g_source_pointer+1] == 0xA)) {
      g_source_pointer++;
      g_newline_beginning = OFF;
      continue;
    }
    if (g_buffer[g_source_pointer] == 0xA) {
      g_source_pointer++;
      next_line();
      continue;
    }
    break;
  }
}


int get_next_plain_string(void) {

  char c;
  
  skip_whitespace();

  g_ss = 0;
  while (1) {
    if (g_ss >= MAX_NAME_LENGTH) {
      print_error("GET_NEXT_PLAIN_STRING: Too long for a string.\n", ERROR_NONE);
      return FAILED;
    }

    c = g_buffer[g_source_pointer];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '\\' || c == '@' || c == ':') {
      g_tmp[g_ss] = c;
      g_ss++;
      g_source_pointer++;
    }
    else
      break;
  }

  g_tmp[g_ss] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_tmp) == FAILED)
      return FAILED;
    g_ss = (int)strlen(g_tmp);
  }

  return SUCCEEDED;
}


int get_next_token(void) {

  skip_whitespace();

  /* skip leading commas */
  while (g_buffer[g_source_pointer] == ',')
    g_source_pointer++;
  
  /* "string"? */
  if (g_buffer[g_source_pointer] == '"') {
    for (g_ss = 0, g_source_pointer++; g_buffer[g_source_pointer] != 0xA && g_buffer[g_source_pointer] != '"'; ) {
      if (g_buffer[g_source_pointer] == '\\' && g_buffer[g_source_pointer + 1] == '"') {
        g_tmp[g_ss++] = '"';
        g_source_pointer += 2;
      }
      else
        g_tmp[g_ss++] = g_buffer[g_source_pointer++];
    }

    if (g_buffer[g_source_pointer] == 0xA) {
      print_error("GET_NEXT_TOKEN: String wasn't terminated properly.\n", ERROR_NONE);
      return FAILED;
    }
    g_tmp[g_ss] = 0;
    g_source_pointer++;

    /* expand e.g., \1 and \@ */
    if (g_macro_active != 0) {
      if (expand_macro_arguments(g_tmp) == FAILED)
        return FAILED;
      g_ss = (int)strlen(g_tmp);
    }

    if (process_string_for_special_characters(g_tmp, &g_ss) == FAILED)
      return FAILED;

    return GET_NEXT_TOKEN_STRING;
  }

  if (g_buffer[g_source_pointer] == '.') {
    g_tmp[0] = '.';
    g_source_pointer++;
    for (g_ss = 1; g_buffer[g_source_pointer] != 0x0A && g_buffer[g_source_pointer] != ' ' && g_buffer[g_source_pointer] != '-' && g_ss < MAX_NAME_LENGTH; ) {
      g_tmp[g_ss] = g_buffer[g_source_pointer];
      g_current_directive[g_ss - 1] = toupper((int)g_buffer[g_source_pointer]);
      g_source_pointer++;
      g_ss++;
    }
    g_current_directive[g_ss - 1] = 0;
  }
  else if (g_buffer[g_source_pointer] == '=' || g_buffer[g_source_pointer] == '>' || g_buffer[g_source_pointer] == '<' || g_buffer[g_source_pointer] == '!') {
    for (g_ss = 0; g_buffer[g_source_pointer] != 0xA && (g_buffer[g_source_pointer] == '=' || g_buffer[g_source_pointer] == '!' || g_buffer[g_source_pointer] == '<' || g_buffer[g_source_pointer] == '>')
           && g_ss < MAX_NAME_LENGTH; g_tmp[g_ss++] = g_buffer[g_source_pointer++]);
  }
  else {
    for (g_ss = 0; g_buffer[g_source_pointer] != 0xA && g_buffer[g_source_pointer] != ',' && g_buffer[g_source_pointer] != ' ' && g_ss < MAX_NAME_LENGTH; ) {
      g_tmp[g_ss] = g_buffer[g_source_pointer];
      g_ss++;
      g_source_pointer++;
    }
  }

  if (g_ss >= MAX_NAME_LENGTH) {
    print_error("GET_NEXT_TOKEN: Too long for a token.\n", ERROR_NONE);
    return FAILED;
  }

  g_tmp[g_ss] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_tmp) == FAILED)
      return FAILED;
    g_ss = (int)strlen(g_tmp);
  }

  return SUCCEEDED;
}


int skip_next_token(void) {

  int c;
  
  for (c = g_buffer[g_source_pointer]; c == ' ' || c == ',' || (c == '\\' && g_buffer[g_source_pointer+1] == 0xA) || c == 0xA; c = g_buffer[++g_source_pointer]) {
    if (c == 0xA)
      next_line();
  }

  if (g_buffer[g_source_pointer] == '"') {
    for (g_source_pointer++; g_buffer[g_source_pointer] != 0x0A && g_buffer[g_source_pointer] != '"'; g_source_pointer++)
      ;
    if (g_buffer[g_source_pointer] == 0x0A) {
      print_error("SKIP_NEXT_TOKEN: String wasn't terminated properly.\n", ERROR_NONE);
      return FAILED;
    }
    g_source_pointer++;

    return SUCCEEDED;
  }
  else if (g_buffer[g_source_pointer] == '=' || g_buffer[g_source_pointer] == '>' || g_buffer[g_source_pointer] == '<' || g_buffer[g_source_pointer] == '!') {
    for (; g_buffer[g_source_pointer] != 0xA && (g_buffer[g_source_pointer] == '=' || g_buffer[g_source_pointer] == '!' || g_buffer[g_source_pointer] == '<' || g_buffer[g_source_pointer] == '>'); g_source_pointer++)
      ;
  }
  else {
    for (; g_buffer[g_source_pointer] != 0x0A && g_buffer[g_source_pointer] != ' ' && g_buffer[g_source_pointer] != ','; g_source_pointer++)
      ;
  }
  
  return SUCCEEDED;
}


int _expand_macro_arguments_one_pass(char *in, int *expands, int *move_up) {

  char t[MAX_NAME_LENGTH + 1];
  int i, j, k, adder;


  memset(g_expanded_macro_string, 0, MAX_NAME_LENGTH + 1);

  for (i = 0, k = 0; i < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; i++) {
    if (in[i] == '?' && in[i + 1] >= '1' && in[i + 1] <= '9') {
      struct macro_argument *ma;
      int d = 0;

      (*expands)++;
      i++;

      for (; i < MAX_NAME_LENGTH && in[i] != 0; i++) {
        if (in[i] >= '0' && in[i] <= '9')
          d = (d * 10) + in[i] - '0';
        else
          break;
      }
      i--;

      if (d <= 0 || d > g_macro_runtime_current->supplied_arguments) {
        if (g_input_number_error_msg == YES) {
          snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, ?%d is out of range.\n", g_macro_runtime_current->macro->name, d);
          print_error(g_xyz, ERROR_NUM);
        }
    
        return FAILED;
      }

      /* use the macro argument to find its definition */
      ma = g_macro_runtime_current->argument_data[d - 1];

      if (ma->type == INPUT_NUMBER_ADDRESS_LABEL) {
        strcpy(g_label, ma->string);

        hashmap_get(g_defines_map, g_label, (void*)&g_tmp_def);
        if (g_tmp_def != NULL) {
          int type = _input_number_return_definition(g_tmp_def);

          if (type == SUCCEEDED)
            snprintf(t, sizeof(t), "%d", g_parsed_int);
          else if (type == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(t, g_label);
          else if (type == INPUT_NUMBER_STRING)
            strcpy(t, g_label);
          else {
            print_error("The definition cannot be converted to a string.\n", ERROR_ERR);
            return FAILED;
          }
        
          for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
            g_expanded_macro_string[k] = t[j];
            if (t[j] == 0)
              break;
          }
        }
        else {
          snprintf(g_xyz, sizeof(g_xyz), "Cannot find definition for \"%s\".\n", g_label);
          print_error(g_xyz, ERROR_NUM);
          return FAILED;
        }
      }
      else {
        print_error("? can be only used to evaluate definitions.\n", ERROR_ERR);
        return FAILED;
      }
    }
    else if (in[i] == '\\') {
      if (in[i + 1] == '"' || in[i + 1] == 'n' || in[i + 1] == '\\' || in[i + 1] == 'r' || in[i + 1] == 't' || in[i + 1] == 'x' || in[i + 1] == '0') {
        g_expanded_macro_string[k++] = in[i];
        i++;
        g_expanded_macro_string[k++] = in[i];
      }
      /* HACK: "\?1" question works only at the beginning of a string, elsewhere "\?" -> "?" */
      else if (i > 0 && in[i + 1] == '?') {
        i++;
        g_expanded_macro_string[k++] = in[i];
      }
      else if (in[i + 1] == '@') {
        /* we found '@' -> expand! */
        (*expands)++;
        i++;

        adder = 0;
        if (i > 1 && in[i - 2] == '{' && in[i + 1] == '-' && in[i + 2] >= '0' && in[i + 2] <= '9' && in[i + 3] == '}') {
          /* found "{\@-1}" and alike */
          adder = -(in[i + 2] - '0');
          i += 3;
          k--;
        }
        else if (i > 1 && in[i - 2] == '{' && in[i + 1] == '+' && in[i + 2] >= '0' && in[i + 2] <= '9' && in[i + 3] == '}') {
          /* found "{\@+1}" and alike */
          adder = in[i + 2] - '0';
          i += 3;
          k--;
        }
        
        snprintf(t, sizeof(t), "%d", g_macro_runtime_current->macro->calls - 1 + adder);
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          g_expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] == '?') {
        /* we found '?' -> expand! */
        int d = 0, type;
        
        (*expands)++;
        i++;

        i++;
        for (; i < MAX_NAME_LENGTH && in[i] != 0; i++) {
          if (in[i] >= '0' && in[i] <= '9')
            d = (d * 10) + in[i] - '0';
          else
            break;
        }
        i--;

        if (d <= 0 || d > g_macro_runtime_current->supplied_arguments) {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, \\?%d is out of range.\n", g_macro_runtime_current->macro->name, d);
            print_error(g_xyz, ERROR_NUM);
          }
    
          return FAILED;
        }

        type = g_macro_runtime_current->argument_data[d-1]->type;
        if (type == SUCCEEDED) {
          if (g_macro_runtime_current->argument_data[d-1]->has_leading_hashtag == YES)
            strcpy(t, "ARG_IMMEDIATE");
          else
            strcpy(t, "ARG_NUMBER");
        }
        else if (type == INPUT_NUMBER_FLOAT) {
          if (g_macro_runtime_current->argument_data[d-1]->has_leading_hashtag == YES)
            strcpy(t, "ARG_IMMEDIATE");
          else
            strcpy(t, "ARG_NUMBER");
        }
        else if (type == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(t, "ARG_LABEL");
        else if (type == INPUT_NUMBER_STRING)
          strcpy(t, "ARG_STRING");
        else if (type == INPUT_NUMBER_STACK)
          strcpy(t, "ARG_PENDING_CALCULATION");
        else
          strcpy(t, "???");
        
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          g_expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] == '.') {
        /* we found '.' -> expand! */
        (*expands)++;
        i++;

        snprintf(t, sizeof(t), "%s", g_macro_runtime_current->macro->name);
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          g_expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] == '!') {
        /* we found '!' -> expand! */
        (*expands)++;
        i++;

        snprintf(t, sizeof(t), "%s", get_file_name(g_active_file_info_last->filename_id));
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          g_expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] >= '0' && in[i + 1] <= '9') {
        /* handle numbers, e.g., \1 */
        int d = 0, type;

        (*expands)++;
        (*move_up)++;
        i++;
        for (; i < MAX_NAME_LENGTH && in[i] != 0; i++) {
          if (in[i] >= '0' && in[i] <= '9')
            d = (d * 10) + in[i] - '0';
          else
            break;
        }
        i--;

        if (d > g_macro_runtime_current->supplied_arguments) {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, \\%d is out of range.\n", g_macro_runtime_current->macro->name, d);
            print_error(g_xyz, ERROR_NUM);
          }
    
          return FAILED;
        }

        type = g_macro_runtime_current->argument_data[d - 1]->type;

        /* replace e.g., \1 in the string with processed macro argument */
        if (type == SUCCEEDED) {
          char tmp_string[32];
          int i;

          snprintf(tmp_string, sizeof(tmp_string), "%d", (int)g_macro_runtime_current->argument_data[d - 1]->value);
          for (i = 0; k < MAX_NAME_LENGTH && i < (int)sizeof(tmp_string); i++, k++) {
            if (tmp_string[i] == 0)
              break;
            g_expanded_macro_string[k] = tmp_string[i];
          }
        }
        else if (type == INPUT_NUMBER_ADDRESS_LABEL || type == INPUT_NUMBER_STRING) {
          int i;

          for (i = 0; k < MAX_NAME_LENGTH; i++, k++) {
            char c = g_macro_runtime_current->argument_data[d - 1]->string[i];
            if (c == 0)
              break;
            g_expanded_macro_string[k] = c;
          }
        }
        else if (type == INPUT_NUMBER_STACK) {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Macro argument \\%d is a pending stack calculation and cannot be expanded into a string.\n", d);
            print_error(g_xyz, ERROR_NUM);
          }
    
          return FAILED;
        }
        else {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Macro argument \\%d is of unknown type, please submit a bug report.\n", d);
            print_error(g_xyz, ERROR_NUM);
          }
    
          return FAILED;
        }
      }
      else {
        if (g_input_number_error_msg == YES) {
          snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: Unsupported special character '%c'.\n", in[i + 1]);
          print_error(g_xyz, ERROR_NUM);
        }
    
        return FAILED;
      }
    }
    else
      g_expanded_macro_string[k++] = in[i];

    if (in[i] == 0)
      break;
  }

  if (k >= MAX_NAME_LENGTH) {
    if (g_input_number_error_msg == YES) {
      snprintf(g_xyz, sizeof(g_xyz), "EXPAND_MACRO_ARGUMENTS: The result string is too large, increase MAX_NAME_LENGTH and compile WLA DX again.\n");
      print_error(g_xyz, ERROR_NUM);
    }
    
    return FAILED;
  }

  memcpy(in, g_expanded_macro_string, MAX_NAME_LENGTH);
  in[MAX_NAME_LENGTH] = '\0';

  return SUCCEEDED;
}


int _expand_macro_arguments(char *in, int *expands) {

  int move_up = 0;

  if (_expand_macro_arguments_one_pass(in, expands, &move_up) == FAILED)
    return FAILED;

  /* macro argument numbers? if we find and expand some, we'll need to recursively call this function */
  if (move_up > 0) {
    /* move up one macro call in the hierarchy */
    g_macro_active--;
    if (g_macro_active > 0) {
      g_macro_runtime_current = &g_macro_stack[g_macro_active - 1];
      /* recursive call to self */
      return _expand_macro_arguments(in, expands);
    }
  }

  return SUCCEEDED;
}
  

int expand_macro_arguments(char *in) {

  /* save the current macro_runtime pointers */
  struct macro_runtime* mr = g_macro_runtime_current;
  int ma = g_macro_active, expands = 0, ret;

  ret = _expand_macro_arguments(in, &expands);

  /* return the current macro_runtime as recursive _expand_macro_arguments() might have modified it */
  g_macro_runtime_current = mr;
  g_macro_active = ma;

  return ret;
}


int process_string_for_special_characters(char *label, int *string_size) {

  int size, read, write;

  if (string_size == NULL)
    size = (int)strlen(label);
  else
    size = *string_size;
  
  for (read = 0, write = 0; read < size; ) {
    /* handle '\0' */
    if (label[read] == '\\' && label[read + 1] == '0') {
      label[write++] = 0;
      read += 2;
    }
    /* handle '\t' */
    else if (label[read] == '\\' && label[read + 1] == 't') {
      label[write++] = '\t';
      read += 2;
    }
    /* handle '\r' */
    else if (label[read] == '\\' && label[read + 1] == 'r') {
      label[write++] = '\r';
      read += 2;
    }
    /* handle '\n' */
    else if (label[read] == '\\' && label[read + 1] == 'n') {
      label[write++] = '\n';
      read += 2;
    }
    /* handle '\"' */
    else if (label[read] == '\\' && label[read + 1] == '\"') {
      label[write++] = '\"';
      read += 2;
    }
    /* handle '\\' */
    else if (label[read] == '\\' && label[read + 1] == '\\') {
      label[write++] = '\\';
      read += 2;
    }
    /* handle '\x' */
    else if (label[read] == '\\' && label[read + 1] == 'x') {
      char tmp_a[8], *tmp_b;
      int tmp_c;

      read += 2;
      snprintf(tmp_a, sizeof(tmp_a), "%c%c", label[read], label[read + 1]);
      
      tmp_c = (int)strtol(tmp_a, &tmp_b, 16);
      if (*tmp_b) {
        print_error("'\\x' needs hexadecimal byte (00-FF) data.\n", ERROR_INP);
        return FAILED;
      }

      label[write++] = tmp_c;
      read += 2;
    }
    /* handle '\<' */
    else if (label[read] == '\\' && label[read + 1] == '<') {
      read += 2;
      if (read >= size) {
        print_error("'\\<' needs character data.\n", ERROR_INP);
        return FAILED;
      }
      label[write++] = label[read] | 0x80;
      read++;
    }
    /* handle '\>' */
    else if (read == 0 && label[read] == '\\' && label[read + 1] == '>') {
      print_error("'\\>' needs character data (previous byte).\n", ERROR_INP);
      return FAILED;
    }
    else if (read < size - 2 && label[read + 1] == '\\' && label[read + 2] == '>') {
      label[write++] = label[read] | 0x80;
      read += 3;
    }
    else
      label[write++] = label[read++];
  }

  /* terminate the string */
  label[write] = 0;

  if (string_size != NULL)
    *string_size = write;

  return SUCCEEDED;
}


int parse_function_asc(char *in, int *result, int *parsed_chars) {

  int res, old_expect = g_expect_calculations, source_pointer_original = g_source_pointer, source_pointer_backup;
  
  /* NOTE! we assume that 'in' is actually '&g_buffer[xyz]', so
     let's update g_source_pointer for input_number() */

  g_source_pointer = (int)(in - g_buffer);
  source_pointer_backup = g_source_pointer;

  g_expect_calculations = NO;
  res = input_number();
  g_expect_calculations = old_expect;

  if (res != SUCCEEDED || g_parsed_int < 0 || g_parsed_int > 255) {
    print_error("asc() requires an immediate value between 0 and 255.\n", ERROR_NUM);
    return FAILED;
  }
  
  if (g_buffer[g_source_pointer] != ')') {
    print_error("Malformed \"asc(?)\" detected!\n", ERROR_NUM);
    return FAILED;
  }

  /* skip ')' */
  g_source_pointer++;

  /* count the parsed chars */
  *parsed_chars = (int)(g_source_pointer - source_pointer_backup);

  /* return g_source_pointer */
  g_source_pointer = source_pointer_original;
  
  if (g_asciitable_defined == 0) {
    print_error("No .ASCIITABLE defined. Using the default n->n -mapping.\n", ERROR_WRN);
    *result = g_parsed_int;
  }
  else
    *result = (int)g_asciitable[g_parsed_int];

  return SUCCEEDED;
}


int parse_function_defined(char *in, int *result, int *parsed_chars) {

  char name[MAX_NAME_LENGTH+1];
  struct definition *d;
  int i;

  for (i = 0; i < MAX_NAME_LENGTH && *in != ')' && *in != 0xA && *in != 0; i++, in++) {
    name[i] = *in;
    (*parsed_chars)++;
  }
  name[i] = 0;

  if (*in != ')') {
    print_error("Malformed \"defined(?)\" detected!\n", ERROR_NUM);
    return FAILED;
  }

  /* skip ')' */
  (*parsed_chars)++;

  /* try to find the definition */
  hashmap_get(g_defines_map, name, (void*)&d);

  if (d != NULL)
    *result = 1;
  else
    *result = 0;
  
  return SUCCEEDED;
}


int parse_function_exists(char *in, int *result, int *parsed_chars) {

  int res, old_expect = g_expect_calculations, source_pointer_original = g_source_pointer, source_pointer_backup;
  FILE *f;
  
  /* NOTE! we assume that 'in' is actually '&g_buffer[xyz]', so
     let's update g_source_pointer for input_number() */

  g_source_pointer = (int)(in - g_buffer);
  source_pointer_backup = g_source_pointer;

  g_expect_calculations = NO;
  res = input_number();
  g_expect_calculations = old_expect;

  if (res != INPUT_NUMBER_ADDRESS_LABEL && res != INPUT_NUMBER_STRING) {
    print_error("exists() requires a file name string.\n", ERROR_NUM);
    return FAILED;
  }
  
  if (g_buffer[g_source_pointer] != ')') {
    print_error("Malformed \"exists(?)\" detected!\n", ERROR_NUM);
    return FAILED;
  }

  /* skip ')' */
  g_source_pointer++;

  /* count the parsed chars */
  *parsed_chars = (int)(g_source_pointer - source_pointer_backup);

  /* return g_source_pointer */
  g_source_pointer = source_pointer_original;
  
  f = fopen(g_label, "rb");
  if (f == NULL)
    *result = 0;
  else {
    *result = 1;

    fclose(f);
  }
  
  return SUCCEEDED;
}

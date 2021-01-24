
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

int input_number_error_msg = YES, ss, string_size, input_float_mode = OFF, parse_floats = YES, expect_calculations = YES;
int newline_beginning = ON, parsed_double_decimal_numbers = 0, operand_hint, operand_hint_type;
char label[MAX_NAME_LENGTH + 1], xyz[512];
char unevaluated_expression[256];
char expanded_macro_string[MAX_NAME_LENGTH + 1];
double parsed_double;

extern int g_source_pointer, size, d, macro_active;
extern char *buffer, tmp[4096], cp[256];
extern struct active_file_info *active_file_info_first, *active_file_info_last, *active_file_info_tmp;
extern struct definition *tmp_def;
extern struct map_t *defines_map;
extern struct macro_runtime *macro_stack, *macro_runtime_current;
extern int latest_stack;



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


int compare_next_token(char *token) {

  int ii, t, d, k, length;
  char e;

  
  length = (int)strlen(token);

  /* skip white space */
  ii = g_source_pointer;
  for (e = buffer[ii]; e == ' ' || e == ',' || e == 0x0A; e = buffer[++ii])
    ;

  /* MACRO mode? */
  if (macro_active != 0 && e == '\\') {
    if (buffer[ii + 1] == '@') {
      char tmp_buffer[64];

      snprintf(tmp_buffer, sizeof(tmp_buffer), "%d", macro_runtime_current->macro->calls - 1);

      e = tmp_buffer[0];
      for (t = 0; t < length && e != 0; ) {
        if (toupper((int)token[t]) != toupper((int)e))
          return FAILED;
        t++;
        e = tmp_buffer[t];
      }
    }
    else if (buffer[ii + 1] == '?') {
      /* TODO: do we need to implement this? */
      return FAILED;
    }
    else {
      for (d = 0, k = 0; k < 16; k++) {
        e = buffer[++ii];
        if (e >= '0' && e <= '9')
          d = (d * 10) + e - '0';
        else
          break;
      }

      if (d > macro_runtime_current->supplied_arguments) {
        if (input_number_error_msg == YES) {
          snprintf(xyz, sizeof(xyz), "COMPARE_NEXT_SYMBOL: Macro \"%s\" wasn't called with enough arguments.\n", macro_runtime_current->macro->name);
          print_error(xyz, ERROR_NONE);
        }
        return FAILED;
      }

      ii = macro_runtime_current->argument_data[d - 1]->start;

      e = buffer[ii];
      for (t = 0; t < length && e != ' ' && e != ',' && e != 0x0A; ) {
        if (toupper((int)token[t]) != toupper((int)e))
          return FAILED;
        t++;
        e = buffer[++ii];
      }
    }
  }
  /* not in MACRO mode */
  else {
    for (t = 0; t < length && e != ' ' && e != ',' && e != 0x0A; ) {
      if (toupper((int)token[t]) != toupper((int)e))
        return FAILED;
      t++;
      e = buffer[++ii];
    }
  }

  if (t == length)
    return SUCCEEDED;

  return FAILED;
}


int input_next_string(void) {

  char e;
  int k;

  
  /* skip white space */
  for (e = buffer[g_source_pointer++]; e == ' ' || e == ','; e = buffer[g_source_pointer++])
    ;

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  /* last choice is a label */
  tmp[0] = e;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = buffer[g_source_pointer++];
    if (e == 0x0A || e == ',') {
      g_source_pointer--;
      break;
    }
    else if (e == ' ')
      break;
    tmp[k] = e;
  }

  if (k == MAX_NAME_LENGTH) {
    if (input_number_error_msg == YES) {
      snprintf(xyz, sizeof(xyz), "The string is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
      print_error(xyz, ERROR_NUM);
    }
    return FAILED;
  }

  tmp[k] = 0;

  /* expand e.g., \1 and \@ */
  if (macro_active != 0) {
    if (expand_macro_arguments(tmp) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int input_number(void) {

  char label_tmp[MAX_NAME_LENGTH + 1];
  unsigned char e, ee;
  int k, p, q, spaces = 0, curly_braces = 0;
  double decimal_mul;


  operand_hint = HINT_NONE;
  operand_hint_type = HINT_TYPE_NONE;

  /* skip white space */
  for (e = buffer[g_source_pointer++]; e == ' ' || e == ','; e = buffer[g_source_pointer++])
    ;

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  if (expect_calculations == YES) {
    /* check the type of the expression */
    p = g_source_pointer;
    ee = e;
    while (ee != 0x0A) {
      if (ee == '{')
        curly_braces++;
      else if (ee == '}')
        curly_braces--;
      /* string / symbol -> no calculating */
      else if (ee == '"' || ee == ',' || (ee == '=' && buffer[p] == '=') || (ee == '!' && buffer[p] == '='))
        break;
      else if (ee == ' ')
        spaces++;
      else if (curly_braces <= 0 && (ee == '-' || ee == '+' || ee == '*' || ee == '/' || ee == '&' || ee == '|' || ee == '^' ||
                                     ee == '<' || ee == '>' || ee == '#' || ee == '~' || ee == ':')) {
        if (ee == ':' && spaces > 0)
          break;
        
        /* launch stack calculator */
        p = stack_calculate(&buffer[g_source_pointer - 1], &d);

        if (p == STACK_CALCULATE_DELAY)
          break;
        else if (p == STACK_RETURN_LABEL)
          return INPUT_NUMBER_ADDRESS_LABEL;
        else
          return p;
      }
      ee = buffer[p];
      p++;
    }
  }

  /* MACRO */
  if (macro_active != 0 && e == '\\') {
    struct macro_argument *ma;
    int exit_here = YES;
    int start_i = g_source_pointer;
    unsigned char start_e = e;

    if (buffer[g_source_pointer] == '@') {
      g_source_pointer++;
      d = macro_runtime_current->macro->calls - 1;

      if (buffer[g_source_pointer] != ' ' && buffer[g_source_pointer] != 0xA && buffer[g_source_pointer] != ',')
        exit_here = NO;
      else
        return SUCCEEDED;
    }
    else if (buffer[g_source_pointer] >= '0' && buffer[g_source_pointer] <= '9') {
      for (d = 0, k = 0; k < 4; k++) {
        e = buffer[g_source_pointer++];
        if (e >= '0' && e <= '9')
          d = (d * 10) + (e - '0');
        else {
          g_source_pointer--;
          break;
        }
      }

      if (buffer[g_source_pointer] != ' ' && buffer[g_source_pointer] != 0xA && buffer[g_source_pointer] != ',' && buffer[g_source_pointer] != '.')
        exit_here = NO;
    }
    else
      exit_here = NO;

    if (exit_here == YES) {
      if (d > macro_runtime_current->supplied_arguments) {
        snprintf(xyz, sizeof(xyz), "Referencing argument number %d inside macro \"%s\". The macro has only %d arguments.\n", d, macro_runtime_current->macro->name, macro_runtime_current->supplied_arguments);
        print_error(xyz, ERROR_NUM);
        return FAILED;
      }
      if (d == 0) {
        snprintf(xyz, sizeof(xyz), "Referencing argument number %d inside macro \"%s\". Macro arguments are counted from 1.\n", d, macro_runtime_current->macro->name);
        print_error(xyz, ERROR_NUM);
        return FAILED;
      }

      /* return the macro argument */
      ma = macro_runtime_current->argument_data[d - 1];
      k = ma->type;

      if (k == INPUT_NUMBER_ADDRESS_LABEL)
        strcpy(label, ma->string);
      else if (k == INPUT_NUMBER_STRING) {
        strcpy(label, ma->string);
        string_size = (int)strlen(ma->string);
      }
      else if (k == INPUT_NUMBER_STACK)
        latest_stack = (int)ma->value;
      else if (k == SUCCEEDED) {
        d = (int)ma->value;
        parsed_double = ma->value;
      }
      else {
        print_error("Macro argument list has been corrupted! Please send a bug report!\n", ERROR_ERR);
        return FAILED;
      }

      /* does the MACRO argument number end with a .b/.w/.l? */
      if (e == '.') {
        e = buffer[g_source_pointer+1];
        if (e == 'b' || e == 'B') {
          operand_hint = HINT_8BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
        else if (e == 'w' || e == 'W') {
          operand_hint = HINT_16BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
        }
        else if (e == 'l' || e == 'L') {
          operand_hint = HINT_24BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
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
  d = 0;
  if (e >= '0' && e <= '9') {
    for (k = 0; 1; k++) {
      if (buffer[g_source_pointer+k] >= '0' && buffer[g_source_pointer+k] <= '9')
        continue;
      if (buffer[g_source_pointer+k] >= 'a' && buffer[g_source_pointer+k] <= 'f')
        continue;
      if (buffer[g_source_pointer+k] >= 'A' && buffer[g_source_pointer+k] <= 'F')
        continue;
      if (buffer[g_source_pointer+k] == 'h' || buffer[g_source_pointer+k] == 'H') {
        d = 1;
        break;
      }
      break;
    }
  }

  if (e == '$' || d == 1) {
    if (d == 1)
      g_source_pointer--;
    for (d = 0, k = 0; k < 8; k++, g_source_pointer++) {
      e = buffer[g_source_pointer];
      if (e >= '0' && e <= '9')
        d = (d << 4) + e - '0';
      else if (e >= 'A' && e <= 'F')
        d = (d << 4) + e - 'A' + 10;
      else if (e >= 'a' && e <= 'f')
        d = (d << 4) + e - 'a' + 10;
      else if (e == 'h' || e == 'H') {
        g_source_pointer++;
        e = buffer[g_source_pointer];
        break;
      }
      else
        break;
    }

    if (e == '.') {
      e = buffer[g_source_pointer+1];
      if (e == 'b' || e == 'B') {
        operand_hint = HINT_8BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'w' || e == 'W') {
        operand_hint = HINT_16BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'l' || e == 'L') {
        operand_hint = HINT_24BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
    }

    if (operand_hint == HINT_NONE) {
      if (d > 0xFFFF && d <= 0xFFFFFF)
        operand_hint = HINT_24BIT;
      else if (d > 0xFF)
        operand_hint = HINT_16BIT;
      else
        operand_hint = HINT_8BIT;

      operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
      /* 5-bit values need this */
      if (d >= -16 && d <= 15) {
        operand_hint = HINT_NONE;
        operand_hint_type = HINT_TYPE_NONE;
      }
#endif
    }
    
    parsed_double = (double)d;
    
    return SUCCEEDED;
  }

  if (e >= '0' && e <= '9') {
    int max_digits = 10;
    
    /* we are parsing decimals when q == 1 */
    q = 0;
    parsed_double = e-'0';
    parsed_double_decimal_numbers = 0;
    decimal_mul = 0.1;
    for (k = 0; k < max_digits; k++, g_source_pointer++) {
      e = buffer[g_source_pointer];
      if (e >= '0' && e <= '9') {
        if (k == max_digits - 1) {
          if (q == 0)
            print_error("Too many digits in the integer value. Max 10 is supported.\n", ERROR_NUM);
          else {
            snprintf(xyz, sizeof(xyz), "Too many digits in the floating point value. Max %d is supported.\n", MAX_FLOAT_DIGITS);
            print_error(xyz, ERROR_NUM);
          }
          return FAILED;
        }
        
        if (q == 0) {
          /* still parsing an integer */
          parsed_double = parsed_double*10 + e-'0';
        }
        else {
          parsed_double = parsed_double + decimal_mul*(e-'0');
          decimal_mul /= 10.0;
          parsed_double_decimal_numbers = parsed_double_decimal_numbers*10 + (e-'0');
        }
      }
      else if (e == '.') {
        if (q == 1) {
          print_error("Syntax error.\n", ERROR_NUM);
          return FAILED;
        }
        e = buffer[g_source_pointer+1];
        if (e >= '0' && e <= '9') {
          /* float mode, read decimals */
          if (parse_floats == NO)
            break;
          q = 1;
          max_digits = MAX_FLOAT_DIGITS+1;
        }
        else if (e == 'b' || e == 'B') {
          operand_hint = HINT_8BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
        else if (e == 'w' || e == 'W') {
          operand_hint = HINT_16BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
          g_source_pointer += 2;
          break;
        }
        else if (e == 'l' || e == 'L') {
          operand_hint = HINT_24BIT;
          operand_hint_type = HINT_TYPE_GIVEN;
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
    d = (int)parsed_double;

    if (operand_hint == HINT_NONE) {
      if (d > 0xFFFF && d <= 0xFFFFFF)
        operand_hint = HINT_24BIT;
      else if (d > 0xFF)
        operand_hint = HINT_16BIT;
      else
        operand_hint = HINT_8BIT;

      operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
      /* 5-bit values need this */
      if (d >= -16 && d <= 15) {
        operand_hint = HINT_NONE;
        operand_hint_type = HINT_TYPE_NONE;
      }
#endif
    }
    
    if (q == 1 && input_float_mode == ON)
      return INPUT_NUMBER_FLOAT;

    return SUCCEEDED;
  }

  if (e == '%') {
    for (d = 0, k = 0; k < 32; k++, g_source_pointer++) {
      e = buffer[g_source_pointer];
      if (e == '0' || e == '1')
        d = (d << 1) + e - '0';
      else
        break;
    }

    if (e == '.') {
      e = buffer[g_source_pointer+1];
      if (e == 'b' || e == 'B') {
        operand_hint = HINT_8BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'w' || e == 'W') {
        operand_hint = HINT_16BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
      else if (e == 'l' || e == 'L') {
        operand_hint = HINT_24BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        g_source_pointer += 2;
      }
    }

    parsed_double = (double)d;

    return SUCCEEDED;
  }

  if (e == '\'') {
    d = buffer[g_source_pointer++];
    e = buffer[g_source_pointer];
    if (e != '\'') {
      if (input_number_error_msg == YES) {
        snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected \"'\".\n", e, e);
        print_error(xyz, ERROR_NUM);
      }
      return FAILED;
    }
    g_source_pointer++;

    parsed_double = (double)d;
    
    return SUCCEEDED;
  }

  if (e == '"' || e == '{') {
    int curly_braces = 0;

    if (e == '{')
      curly_braces++;
    
    for (k = 0; k < MAX_NAME_LENGTH; ) {
      e = buffer[g_source_pointer++];

      if (e == '\\' && buffer[g_source_pointer] == '"') {
        label[k++] = '"';
        g_source_pointer++;
        continue;
      }

      if (e == '{') {
        curly_braces++;
        continue;
      }
      
      if (e == '"' || e == '}') {
        if (e == '}')
          curly_braces--;

        /* check for "string".length */
        if (buffer[g_source_pointer+0] == '.' &&
            (buffer[g_source_pointer+1] == 'l' || buffer[g_source_pointer+1] == 'L') &&
            (buffer[g_source_pointer+2] == 'e' || buffer[g_source_pointer+2] == 'E') &&
            (buffer[g_source_pointer+3] == 'n' || buffer[g_source_pointer+3] == 'N') &&
            (buffer[g_source_pointer+4] == 'g' || buffer[g_source_pointer+4] == 'G') &&
            (buffer[g_source_pointer+5] == 't' || buffer[g_source_pointer+5] == 'T') &&
            (buffer[g_source_pointer+6] == 'h' || buffer[g_source_pointer+6] == 'H')) {
          /* yes, we've got it! calculate the length and return the integer */
          g_source_pointer += 7;
          label[k] = 0;
          d = (int)get_label_length(label);
          parsed_double = (double)d;

          return SUCCEEDED;
        }

        if (e == '"')
          break;
        if (e == '}' && curly_braces <= 0)
          break;
      }
      
      if (e == 0 || e == 0x0A) {
        print_error("String wasn't terminated properly.\n", ERROR_NUM);
        return FAILED;
      }

      label[k++] = e;
    }

    label[k] = 0;

    /* expand e.g., \1 and \@ */
    if (macro_active != 0) {
      if (expand_macro_arguments(label) == FAILED)
        return FAILED;
      k = (int)strlen(label);
    }

    if (k == MAX_NAME_LENGTH) {
      if (input_number_error_msg == YES) {
        snprintf(xyz, sizeof(xyz), "The string is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
        print_error(xyz, ERROR_NUM);
      }
      return FAILED;
    }

    label[k] = 0;
    string_size = k;
    
    return INPUT_NUMBER_STRING;
  }

  /* the last choice is a label */
  label[0] = e;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = buffer[g_source_pointer++];
    if (e == 0x0A || e == ')' || e == ',' || e == ']') {
      g_source_pointer--;
      break;
    }
    else if (e == ' ')
      break;
    label[k] = e;
  }

  if (k == MAX_NAME_LENGTH) {
    if (input_number_error_msg == YES) {
      snprintf(xyz, sizeof(xyz), "The label is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
      print_error(xyz, ERROR_NUM);
    }
    return FAILED;
  }

  /* size hint? */
  if (label[k-2] == '.') {
    if (label[k-1] == 'b' || label[k-1] == 'B') {
      operand_hint = HINT_8BIT;
      operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
    else if (label[k-1] == 'w' || label[k-1] == 'W') {
      operand_hint = HINT_16BIT;
      operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
    else if (label[k-1] == 'l' || label[k-1] == 'L') {
      operand_hint = HINT_24BIT;
      operand_hint_type = HINT_TYPE_GIVEN;
      k -= 2;
    }
  }

  label[k] = 0;

  /* expand e.g., \1 and \@ */
  if (macro_active != 0) {
    if (expand_macro_arguments(label) == FAILED)
      return FAILED;
  }

  /* label_tmp contains the label without possible prefix ':' */
  if (strlen(label) > 1 && label[0] == ':')
    strcpy(label_tmp, &label[1]);
  else
    strcpy(label_tmp, label);

  /* check for "string".length */
  if (strstr(label, ".length") != NULL) {
    parse_string_length(strstr(label, ".length"));
    return SUCCEEDED;
  }
  else if (strstr(label, ".LENGTH") != NULL) {
    parse_string_length(strstr(label, ".LENGTH"));
    return SUCCEEDED;
  }

  /* check if the label is actually a definition */
  if (hashmap_get(defines_map, label, (void*)&tmp_def) != MAP_OK)
    hashmap_get(defines_map, label_tmp, (void*)&tmp_def);
  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_VALUE) {
      d = (int)tmp_def->value;

      if (operand_hint == HINT_NONE) {
        if (d > 0xFFFF && d <= 0xFFFFFF)
          operand_hint = HINT_24BIT;
        else if (d > 0xFF)
          operand_hint = HINT_16BIT;
        else
          operand_hint = HINT_8BIT;

        operand_hint_type = HINT_TYPE_DEDUCED;

#if defined(MC6809)
        /* 5-bit values need this */
        if (d >= -16 && d <= 15) {
          operand_hint = HINT_NONE;
          operand_hint_type = HINT_TYPE_NONE;
        }
#endif
      }
      
      parsed_double = (double)d;

      return SUCCEEDED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_STACK) {
      /* wrap the referenced, existing stack calculation inside a new stack calculation as stack
         calculation contains a write. the 2nd, 3rd etc. reference don't do anything by themselves.
         but wrapping creates a new stack calculation that also makes a write */
      stack_create_stack_stack((int)tmp_def->value);
      return INPUT_NUMBER_STACK;
    }
    else if (tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      if (label[0] == ':') {
        /* we need to keep the ':' prefix */
        if (strlen(tmp_def->string) >= MAX_NAME_LENGTH-1) {
          if (input_number_error_msg == YES) {
            snprintf(xyz, sizeof(xyz), "The label is too long (max %d characters allowed).\n", MAX_NAME_LENGTH);
            print_error(xyz, ERROR_NUM);
          }
          return FAILED;          
        }
        snprintf(label, sizeof(label), ":%.254s", tmp_def->string);
        string_size = tmp_def->size + 1;
      }
      else {
        string_size = tmp_def->size;
        memcpy(label, tmp_def->string, string_size);
        label[string_size] = 0;
      }
      return INPUT_NUMBER_ADDRESS_LABEL;
    }
    else {
      string_size = tmp_def->size;
      memcpy(label, tmp_def->string, string_size);
      label[string_size] = 0;
      
      return INPUT_NUMBER_STRING;
    }
  }

  return INPUT_NUMBER_ADDRESS_LABEL;
}


int parse_string_length(char *end) {

  /* remove ".length" from the end of label (end points to inside of label) */
  end[0] = 0;

  /* check if the label is actually a definition - it should be or else we'll give an error */
  hashmap_get(defines_map, label, (void*)&tmp_def);
  
  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_VALUE) {
      if (input_number_error_msg == YES) {
        print_error(".length of a value does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_STACK) {
      if (input_number_error_msg == YES) {
        print_error(".length of a pending computation does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      if (input_number_error_msg == YES) {
        print_error(".length of an address label does not make any sense.\n", ERROR_NUM);
      }
      return FAILED;
    }
    else {
      string_size = tmp_def->size;
      memcpy(label, tmp_def->string, string_size);
      label[string_size] = 0;

      d = (int)strlen(label);
      parsed_double = (double)d;
          
      return SUCCEEDED;
    }
  }

  return FAILED;
}


void skip_whitespace(void) {

  while (1) {
    if (g_source_pointer == size)
      break;
    if (buffer[g_source_pointer] == ' ') {
      g_source_pointer++;
      newline_beginning = OFF;
      continue;
    }
    if (buffer[g_source_pointer] == 0xA) {
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

  ss = 0;
  while (1) {
    if (ss >= MAX_NAME_LENGTH) {
      print_error("GET_NEXT_PLAIN_STRING: Too long for a string.\n", ERROR_NONE);
      return FAILED;
    }

    c = buffer[g_source_pointer];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '\\' || c == '@' || c == ':') {
      tmp[ss] = c;
      ss++;
      g_source_pointer++;
    }
    else
      break;
  }

  tmp[ss] = 0;

  /* expand e.g., \1 and \@ */
  if (macro_active != 0) {
    if (expand_macro_arguments(tmp) == FAILED)
      return FAILED;
    ss = (int)strlen(tmp);
  }

  return SUCCEEDED;
}


int get_next_token(void) {

  skip_whitespace();

  /* skip leading commas */
  while (buffer[g_source_pointer] == ',')
    g_source_pointer++;
  
  /* "string"? */
  if (buffer[g_source_pointer] == '"') {
    for (ss = 0, g_source_pointer++; buffer[g_source_pointer] != 0xA && buffer[g_source_pointer] != '"'; ) {
      if (buffer[g_source_pointer] == '\\' && buffer[g_source_pointer + 1] == '"') {
        tmp[ss++] = '"';
        g_source_pointer += 2;
      }
      else
        tmp[ss++] = buffer[g_source_pointer++];
    }

    if (buffer[g_source_pointer] == 0xA) {
      print_error("GET_NEXT_TOKEN: String wasn't terminated properly.\n", ERROR_NONE);
      return FAILED;
    }
    tmp[ss] = 0;
    g_source_pointer++;

    /* expand e.g., \1 and \@ */
    if (macro_active != 0) {
      if (expand_macro_arguments(tmp) == FAILED)
        return FAILED;
      ss = (int)strlen(tmp);
    }

    return GET_NEXT_TOKEN_STRING;
  }

  if (buffer[g_source_pointer] == '.') {
    tmp[0] = '.';
    g_source_pointer++;
    for (ss = 1; buffer[g_source_pointer] != 0x0A && buffer[g_source_pointer] != ' ' && buffer[g_source_pointer] != '-' && ss < MAX_NAME_LENGTH; ) {
      tmp[ss] = buffer[g_source_pointer];
      cp[ss - 1] = toupper((int)buffer[g_source_pointer]);
      g_source_pointer++;
      ss++;
    }
    cp[ss - 1] = 0;
  }
  else if (buffer[g_source_pointer] == '=' || buffer[g_source_pointer] == '>' || buffer[g_source_pointer] == '<' || buffer[g_source_pointer] == '!') {
    for (ss = 0; buffer[g_source_pointer] != 0xA && (buffer[g_source_pointer] == '=' || buffer[g_source_pointer] == '!' || buffer[g_source_pointer] == '<' || buffer[g_source_pointer] == '>')
           && ss < MAX_NAME_LENGTH; tmp[ss++] = buffer[g_source_pointer++]);
  }
  else {
    for (ss = 0; buffer[g_source_pointer] != 0xA && buffer[g_source_pointer] != ',' && buffer[g_source_pointer] != ' ' && ss < MAX_NAME_LENGTH; ) {
      tmp[ss] = buffer[g_source_pointer];
      ss++;
      g_source_pointer++;
    }
  }

  if (ss >= MAX_NAME_LENGTH) {
    print_error("GET_NEXT_TOKEN: Too long for a token.\n", ERROR_NONE);
    return FAILED;
  }

  tmp[ss] = 0;

  /* expand e.g., \1 and \@ */
  if (macro_active != 0) {
    if (expand_macro_arguments(tmp) == FAILED)
      return FAILED;
    ss = (int)strlen(tmp);
  }

  return SUCCEEDED;
}


int skip_next_token(void) {

  for (; buffer[g_source_pointer] == ' ' || buffer[g_source_pointer] == ','; g_source_pointer++)
    ;

  if (buffer[g_source_pointer] == 0x0A)
    return FAILED;

  if (buffer[g_source_pointer] == '"') {
    for (g_source_pointer++; buffer[g_source_pointer] != 0x0A && buffer[g_source_pointer] != '"'; g_source_pointer++)
      ;
    if (buffer[g_source_pointer] == 0x0A) {
      print_error("SKIP_NEXT_TOKEN: String wasn't terminated properly.\n", ERROR_NONE);
      return FAILED;
    }
    g_source_pointer++;

    return SUCCEEDED;
  }
  else if (buffer[g_source_pointer] == '=' || buffer[g_source_pointer] == '>' || buffer[g_source_pointer] == '<' || buffer[g_source_pointer] == '!') {
    for (; buffer[g_source_pointer] != 0xA && (buffer[g_source_pointer] == '=' || buffer[g_source_pointer] == '!' || buffer[g_source_pointer] == '<' || buffer[g_source_pointer] == '>'); g_source_pointer++)
      ;
  }
  else {
    for (; buffer[g_source_pointer] != 0x0A && buffer[g_source_pointer] != ' ' && buffer[g_source_pointer] != ','; g_source_pointer++)
      ;
  }
  
  return SUCCEEDED;
}


int _expand_macro_arguments_one_pass(char *in, int *expands, int *move_up) {

  char t[MAX_NAME_LENGTH + 1];
  int i, j, k, adder;


  memset(expanded_macro_string, 0, MAX_NAME_LENGTH + 1);

  for (i = 0, k = 0; i < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; i++) {
    if (in[i] == '\\') {
      if (in[i + 1] == '"' || in[i + 1] == 'n' || in[i + 1] == '\\') {
        expanded_macro_string[k++] = in[i];
        i++;
        expanded_macro_string[k++] = in[i];
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
        
        snprintf(t, sizeof(t), "%d", macro_runtime_current->macro->calls - 1 + adder);
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          expanded_macro_string[k] = t[j];
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

        if (d <= 0 || d > macro_runtime_current->supplied_arguments) {
          if (input_number_error_msg == YES) {
            snprintf(xyz, sizeof(xyz), "Macro \"%s\" wasn't called with enough arguments, \\?%d is out of range.\n", macro_runtime_current->macro->name, d);
            print_error(xyz, ERROR_NUM);
          }
    
          return FAILED;
        }

        type = macro_runtime_current->argument_data[d-1]->type;
        if (type == SUCCEEDED)
          strcpy(t, "ARG_NUMBER");
        else if (type == INPUT_NUMBER_FLOAT)
          strcpy(t, "ARG_NUMBER");
        else if (type == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(t, "ARG_LABEL");
        else if (type == INPUT_NUMBER_STRING)
          strcpy(t, "ARG_STRING");
        else if (type == INPUT_NUMBER_STACK)
          strcpy(t, "ARG_PENDING_CALCULATION");
        else
          strcpy(t, "???");
        
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] == '.') {
        /* we found '.' -> expand! */
        (*expands)++;
        i++;

        snprintf(t, sizeof(t), "%s", macro_runtime_current->macro->name);
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] == '!') {
        /* we found '!' -> expand! */
        (*expands)++;
        i++;

        snprintf(t, sizeof(t), "%s", get_file_name(active_file_info_last->filename_id));
        for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
          expanded_macro_string[k] = t[j];
          if (t[j] == 0)
            break;
        }
      }
      else if (in[i + 1] >= '0' && in[i + 1] <= '9') {
        /* handle numbers, e.g., \1 */
        int d = 0;

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

        if (d > macro_runtime_current->supplied_arguments) {
          if (input_number_error_msg == YES) {
            snprintf(xyz, sizeof(xyz), "Macro \"%s\" wasn't called with enough arguments, \\%d is out of range.\n", macro_runtime_current->macro->name, d);
            print_error(xyz, ERROR_NUM);
          }
    
          return FAILED;
        }

        d = macro_runtime_current->argument_data[d - 1]->start;

        for (; k < MAX_NAME_LENGTH; d++, k++) {
          if (buffer[d] == 0 || buffer[d] == ' ' || buffer[d] == 0x0A || buffer[d] == ',')
            break;
          expanded_macro_string[k] = buffer[d];
        }
      }
      else {
        if (input_number_error_msg == YES) {
          snprintf(xyz, sizeof(xyz), "EXPAND_MACRO_ARGUMENTS: Unsupported special character '%c'.\n", in[i + 1]);
          print_error(xyz, ERROR_NUM);
        }
    
        return FAILED;
      }
    }
    else
      expanded_macro_string[k++] = in[i];

    if (in[i] == 0)
      break;
  }

  if (k >= MAX_NAME_LENGTH) {
    if (input_number_error_msg == YES) {
      snprintf(xyz, sizeof(xyz), "EXPAND_MACRO_ARGUMENTS: The result string is too large, increase MAX_NAME_LENGTH and compile WLA DX again.\n");
      print_error(xyz, ERROR_NUM);
    }
    
    return FAILED;
  }

  memcpy(in, expanded_macro_string, MAX_NAME_LENGTH);
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
    macro_active--;
    if (macro_active > 0) {
      macro_runtime_current = &macro_stack[macro_active - 1];
      /* recursive call to self */
      return _expand_macro_arguments(in, expands);
    }
  }

  return SUCCEEDED;
}
  

int expand_macro_arguments(char *in) {

  /* save the current macro_runtime pointers */
  struct macro_runtime* mr = macro_runtime_current;
  int ma = macro_active, expands = 0, ret;

  ret = _expand_macro_arguments(in, &expands);

  /* return the current macro_runtime as recursive _expand_macro_arguments() might have modified it */
  macro_runtime_current = mr;
  macro_active = ma;

  return ret;
}

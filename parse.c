
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "parse.h"
#include "pass_1.h"
#include "stack.h"
#include "include_file.h"

struct operator_type {
  int operator;
  char *string;
  int string_len;
};

#define OPERATOR_COUNT 15

const struct operator_type operator_list[OPERATOR_COUNT] = {
  { SI_OP_PLUS, "+", 1 },
  { SI_OP_MINUS, "-", 1 },
  { SI_OP_MULTIPLY, "*", 1 },
  { SI_OP_LEFT, "(", 1 },
  { SI_OP_RIGHT, ")", 1 },
  { SI_OP_OR, "|", 1 },
  { SI_OP_AND, "&", 1 },
  { SI_OP_DIVIDE, "/", 1 },
  { SI_OP_POWER, "^", 1 },
  { SI_OP_SHIFT_LEFT, "<<", 2 },
  { SI_OP_SHIFT_RIGHT, ">>", 2 },
  { SI_OP_MODULO, "#", 1 },
  { SI_OP_XOR, "~", 1 },
  { SI_OP_LOW_BYTE, "<", 1 },
  { SI_OP_HIGH_BYTE, ">", 1 }
};

int input_number_error_msg = YES, ss, string_size, input_float_mode = OFF, parse_floats = YES;
int newline_beginning = ON, parsed_double_decimal_numbers = 0;
char label[MAX_NAME_LENGTH], xyz[256];
char unevaluated_expression[256];
char expanded_macro_string[MAX_NAME_LENGTH+1];
double parsed_double;

extern int i, size, d, macro_active, collecting_macro_args;
extern char *buffer, tmp[4096], cp[256];
extern struct active_file_info *active_file_info_first, *active_file_info_last, *active_file_info_tmp;
extern struct definition *tmp_def;
extern struct map_t *defines_map;
extern struct macro_runtime *macro_stack, *macro_runtime_current;
extern int latest_stack;

extern struct token_stack_root *buffer_stack;

int operand_hint = HINT_NONE;


int compare_next_token(char *token, int length) {

  int t;
  char e;

  token_stack_push_state(buffer_stack);

  /* skip white space */
  parse_while_any_token(buffer_stack, " ,\x0A");
  e = token_stack_get_current_token(buffer_stack);

  for (t = 0; t < length && e != ' ' && e != ',' && e != 0x0A; ) {
    if (toupper((int)token[t]) != toupper((int)e)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    t++;
    token_stack_move(buffer_stack, 1);
    e = token_stack_get_current_token(buffer_stack);
  }

  token_stack_pop_state(buffer_stack, 0);

  if (t == length)
    return SUCCEEDED;

  return FAILED;
}


int input_next_string(void) {

  char e;
  int parse_result;

  /* skip white space */
  parse_while_any_token(buffer_stack, " ,");

  e = token_stack_get_current_token(buffer_stack);
  if (e == 0x0A) {
    token_stack_move(buffer_stack, 1);
    return INPUT_NUMBER_EOL;
  }

  /* parse the next token as a label. */
  parse_result = parse_type_label(buffer_stack, tmp, MAX_NAME_LENGTH);

  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;

  return SUCCEEDED;
}


int input_number(void) {
  
  /* TODO: expand input_number_token, to remove dependancy on globals:
     d, label, string_size, parsed_double, parsed_double_decimal_numbers, operand_hint, string_size, tmp_def */
  return input_number_token(buffer_stack);
}


int input_number_token(struct token_stack_root *token_stack) {

  unsigned char e;
  int parse_result;

#if defined(MCS6502) || defined(W65816) || defined(MCS6510) || defined(WDC65C02) || defined(HUC6280)
  operand_hint = HINT_NONE;
#endif

  /* skip white space */
  parse_while_any_token(token_stack, " ,");
  e = token_stack_get_current_token(token_stack);

  /* Found end of line. */
  if (e == 0x0A) {
    token_stack_move(token_stack, 1);
    return INPUT_NUMBER_EOL;
  }

  /* Attempt to parse a mathmatical expression. */
  parse_result = parse_type_expression(token_stack, &d, label, MAX_NAME_LENGTH);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* validate any unhandled macro indicies. */
  parse_result = parse_validate_macro_bounds(token_stack);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;

  /* is it a hexadecimal value? */
  parse_result = parse_type_hex_value(token_stack, &d, &operand_hint);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* is it an integer or double value? */
  parse_result = parse_type_double_value(token_stack, &parsed_double, &parsed_double_decimal_numbers, &d, &operand_hint);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* is it a binary value? */
  parse_result = parse_type_binary_value(token_stack, &d, &operand_hint);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* Attempt to parse a character literal. */
  parse_result = parse_type_char(token_stack, &d);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* Attempt to parse a string literal. */
  parse_result = parse_type_string(token_stack, label, MAX_NAME_LENGTH, &string_size);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return parse_result;

  /* the last choice is a label */
  parse_result = parse_type_label(token_stack, label, MAX_NAME_LENGTH);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED) {
    /* Attempt to resolve the parsed label if a known value is availble. */
    parse_result = parse_resolve_label(label, &string_size, &d, &operand_hint, &tmp_def);
    if (parse_result == FAILED_WITH_ERROR)
      return FAILED;
  }

  return parse_result;
}


int get_next_token(void) {

  char token;
  int parse_result;

  while (1) {
    token = token_stack_get_current_token(buffer_stack);

    /* Check for the end of the buffer. */
    if (token == '\0')
      break;

    /* Skip whitespace. */
    if (token == ' ') {
      token_stack_move(buffer_stack, 1);
      newline_beginning = OFF;
      continue;
    }

    /* Check for a newline. */
    if (token == 0xA) {
      token_stack_move(buffer_stack, 1);
      next_line();
      continue;
    }
    break;
  }

  /* Parse a string literal.  */
  parse_result = parse_type_string(buffer_stack, tmp, 4096, &ss);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return GET_NEXT_TOKEN_STRING;

  token = token_stack_get_current_token(buffer_stack);
  /* Parse a directive. */
  if (token == '.') {
    int tmp_index;

    ss = parse_until_any_token(buffer_stack, " \x0A", tmp, MAX_NAME_LENGTH + 1);
    tmp[ss] = '\0';

    /* Make cp uppercase. */
    for (tmp_index = 1; tmp_index < ss; ++tmp_index)
      cp[tmp_index - 1] = toupper(tmp[tmp_index]);
    cp[ss - 1] = '\0';
  }
  /* Parse a multi character operator. */
  else if (token == '=' || token == '>' || token == '<' || token == '!') {
    for (ss = 0; token != 0xA && (token == '=' || token == '!' || token == '<' || token == '>')
       && ss <= MAX_NAME_LENGTH;) {
      tmp[ss++] = token;
      token_stack_move(buffer_stack, 1);
      token = token_stack_get_current_token(buffer_stack);
    }
    tmp[ss] = 0;
  }
  else {
    /* Parse anything else (delimit on ',', ' ', 0x0A). */
    ss = parse_until_any_token(buffer_stack, ", \x0A", tmp, MAX_NAME_LENGTH + 1);
    tmp[ss] = 0;

    token = token_stack_get_current_token(buffer_stack);
    if (token == ',')
      token_stack_move(buffer_stack, 1);
  }

  if (ss > MAX_NAME_LENGTH) {
    print_error("GET_NEXT_TOKEN: Too long for a token.\n", ERROR_NONE);
    return FAILED;
  }

  return SUCCEEDED;
}


int skip_next_token(void) {

  char temp_string[MAX_NAME_LENGTH];
  char token;
  int parse_result, string_length = 0;

  /* Skip whitespace. */
  parse_while_any_token(buffer_stack, " ,");

  token = token_stack_get_current_token(buffer_stack);
  if (token == 0x0A)
    return FAILED;

  /* Attempt to read a string literal. */
  parse_result = parse_type_string(buffer_stack, temp_string, MAX_NAME_LENGTH, &string_length);
  if (parse_result == FAILED_WITH_ERROR)
    return FAILED;
  else if (parse_result != FAILED)
    return SUCCEEDED;

  /* Read until the end of the token (delimited by ' ', ',', 0x0A). */
  parse_until_any_token(buffer_stack, " ,\x0A", NULL, 0);

  return SUCCEEDED;
}


void parse_while_any_token(struct token_stack_root *token_stack, char *tokens) {

  int token_length = strlen(tokens);
  int read = 1;

  while (read != 0) {
    int token_index;
    char compare = token_stack_get_current_token(token_stack);

    read = 0;

    /* Test the current token against the compare tokens. */
    for (token_index = 0; token_index < token_length && read == 0; token_index++)
      read |= (compare == tokens[token_index]);

    if (read != 0)
      token_stack_move(token_stack, 1);
  }
}


int parse_until_any_token(struct token_stack_root *token_stack, char *tokens, char *store, int size) {

  int token_length = strlen(tokens);
  int read = 0;
  int read_count = 0;

  while (read == 0) {
    int token_index;
    char compare = token_stack_get_current_token(token_stack);

    /* Test the current token against the compare tokens. */
    for (token_index = 0; token_index < token_length && read == 0; token_index++)
      read |= (compare == tokens[token_index]);

    if (read == 0) {
      if (store == NULL || read_count < size) {
        if (store != NULL) {
          store[read_count] = compare;
          read_count++;
        }
        token_stack_move(token_stack, 1);
      }
      else {
        /* Exceeded the buffer storage size. */
        break;
      }
    }
  }

  return read_count;
}


int parse_type_expression(struct token_stack_root *token_stack, int *value, char *string, int max_buffer) {

  char ee = token_stack_get_current_token(token_stack);
  int pop_state, p;

  /* check the type of the expression */
  token_stack_push_state(token_stack);
  pop_state = 1;
  
  if (collecting_macro_args == 0) {
    while (ee != 0x0A && ee != '\0') {
      char next_token = token_stack_get_current_token(token_stack);
      
      /* string / symbol -> no calculating */
      if (ee == '"' || ee == ',' || (ee == '=' && next_token == '=') || (ee == '!' && next_token == '='))
        break;
      if (ee == '-' || ee == '+' || ee == '*' || ee == '/' || ee == '&' || ee == '|' || ee == '^' ||
        ee == '<' || ee == '>' || ee == '#' || ee == '~') {
        int stack_read_count = 0;
	
        /* launch stack calculator */

        /* Extract the line. */
        char *line_buffer = NULL;
        int token_index = 0;
        char token;

        token_stack_pop_state(token_stack, 0);
        token_stack_push_state(token_stack);
        token_stack_push_state(token_stack);

        while (1) {
          token = token_stack_get_current_token(token_stack);
          if (token == '\0' || token == 0x0A) {
            break;
          }
          token_stack_move(token_stack, 1);
          token_index++;
        }

        token_stack_pop_state(token_stack, 0);

        line_buffer = malloc((token_index + 2) * sizeof(char));

        if (line_buffer != NULL) {
          int cur_token;
	  
          for (cur_token = 0; cur_token < token_index; ++cur_token) {
            token = token_stack_get_current_token(token_stack);
            line_buffer[cur_token] = token;
            token_stack_move(token_stack, 1);
          }
          line_buffer[token_index] = 0x0A;
          line_buffer[token_index + 1] = '\0';
        }
        else {
          print_error("Out of memory failed to allocate buffer for expression.\n", ERROR_ERR);
          return FAILED_WITH_ERROR;
        }

        token_stack_pop_state(token_stack, 0);

        pop_state = 0;
        p = stack_calculate(line_buffer, value, &stack_read_count);
        token_stack_move(token_stack, stack_read_count);

        free(line_buffer);

        if (p == STACK_CALCULATE_DELAY)
          break;
        else if (p == STACK_RETURN_LABEL)
          return INPUT_NUMBER_ADDRESS_LABEL;
        else {
          if (p == FAILED)
            p = FAILED_WITH_ERROR;
          return p;
        }
      }
      ee = next_token;
      token_stack_move(token_stack, 1);
    }
  }
  else {
    int result = INPUT_MACRO_ARGUMENT;
    
    token_stack_pop_state(token_stack, 0);
    pop_state = 0;

    result = parse_type_argument_to_string(token_stack, string, max_buffer);

    if (result != FAILED_WITH_ERROR)
      result = INPUT_MACRO_ARGUMENT;

    return result;
  }

  if (pop_state == 1)
    token_stack_pop_state(token_stack, 0);

  return FAILED;
}


int parse_type_hex_value(struct token_stack_root *token_stack, int *value, int *hint) {

  char e = token_stack_get_current_token(token_stack);
  int working_value, hex_flag = 0, k;

  /* is it a hexadecimal value? */
  if (e >= '0' && e <= '9') {
    token_stack_push_state(token_stack);
    token_stack_move(token_stack, 1);
    for (k = 0; 1; k++) {
      char token_char = token_stack_get_current_token(token_stack);
      if (token_char >= '0' && token_char <= '9') {
        token_stack_move(token_stack, 1);
        continue;
      }
      if (token_char >= 'a' && token_char <= 'f') {
        token_stack_move(token_stack, 1);
        continue;
      }
      if (token_char >= 'A' && token_char <= 'F') {
        token_stack_move(token_stack, 1);
        continue;
      }
      if (token_char == 'h' || token_char == 'H') {
        hex_flag = 1;
        break;
      }
      break;
    }
    token_stack_pop_state(token_stack, 0);
  }

  if (e == '$' || hex_flag == 1) {
    if (hex_flag == 0)
      token_stack_move(token_stack, 1);

    working_value = 0;
    for (k = 0; k <= 8; k++) {
      e = token_stack_get_current_token(token_stack);
      if (e >= '0' && e <= '9')
        working_value = (working_value << 4) + e - '0';
      else if (e >= 'A' && e <= 'F')
        working_value = (working_value << 4) + e - 'A' + 10;
      else if (e >= 'a' && e <= 'f')
        working_value = (working_value << 4) + e - 'a' + 10;
      else if (e == 'h' || e == 'H') {
        token_stack_move(token_stack, 1);
        e = token_stack_get_current_token(token_stack);
        break;
      }
      else
        break;
      token_stack_move(token_stack, 1);
    }

    *value = working_value;
    *hint = parse_hint_attribute (token_stack, *hint);
    *hint = parse_hint_by_value (*value, *hint);
  
    return SUCCEEDED;
  }

  return FAILED;
}


int parse_type_double_value(struct token_stack_root *token_stack, double *value, int *dec_value, int *int_value, int *hint) {

  char e = token_stack_get_current_token(token_stack);
  int is_double;

  if (e >= '0' && e <= '9') {
    double decimal_mul = 0.1;
    int k;

    token_stack_move(token_stack, 1);

    /* we are parsing decimals when is_double=1 */
    is_double = 0;
    *value = e - '0';
    *dec_value = 0;
    for (k = 0; k < 9; k++) {
      e = token_stack_get_current_token(token_stack);
      if (e >= '0' && e <= '9') {
        if (is_double == 0) {
          /* still parsing an integer */
          *value = *value * 10 + e - '0';
        }
        else {
          *value = *value + decimal_mul*(e - '0');
          decimal_mul /= 10.0;
          *dec_value = *dec_value * 10 + (e - '0');
        }
      }
      else if (e == '.') {
        if (is_double == 1) {
          print_error("Syntax error.\n", ERROR_NUM);
          return FAILED_WITH_ERROR;
        }
        e = token_stack_peek_token(token_stack, 1);
        if (e >= '0' && e <= '9') {
          /* float mode, read decimals */
          if (parse_floats == NO)
            break;
          is_double = 1;
        }
        else{
          *hint = parse_hint_attribute(token_stack, *hint);

          if (*hint == HINT_NONE) {
            print_error("Syntax error.\n", ERROR_NUM);
            return FAILED_WITH_ERROR;
          }
          else
            break;
        }
      }
      else if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z')) {
        /* a number directly followed by a letter when parsing a integer/float -> syntax error */
        print_error("Syntax error.\n", ERROR_NUM);
        return FAILED_WITH_ERROR;
      }
      else
        break;

      token_stack_move(token_stack, 1);
    }

    /* drop the decimals */
    *int_value = (int)*value;
    *hint = parse_hint_by_value(*int_value, *hint);

    if (is_double == 1 && input_float_mode == ON)
      return INPUT_NUMBER_FLOAT;

    return SUCCEEDED;
  }

  return FAILED;
}


int parse_type_binary_value(struct token_stack_root *token_stack, int *value, int *hint) {

  char e = token_stack_get_current_token(token_stack);

  if (e == '%') {
    int k;

    token_stack_move(token_stack, 1);
    *value = 0;
    for (k = 0; k < 32; k++) {
      e = token_stack_get_current_token(token_stack);
      if (e == '0' || e == '1')
        *value = ((*value) << 1) + e - '0';
      else
        break;
      token_stack_move(token_stack, 1);
    }

    *hint = parse_hint_attribute(token_stack, *hint);

    return SUCCEEDED;
  }

  return FAILED;
}


int parse_type_char(struct token_stack_root *token_stack, int *result) {

  char e = token_stack_get_current_token(token_stack);

  if (e == '\'') {
    token_stack_move(token_stack, 1);
    d = token_stack_get_current_token(token_stack);
    token_stack_move(token_stack, 1);
    e = token_stack_get_current_token(token_stack);
    if (e != '\'') {
      if (input_number_error_msg == YES) {
        sprintf(xyz, "Got '%c' (%d) when expected \"'\".\n", e, e);
        print_error(xyz, ERROR_NUM);
      }
      return FAILED_WITH_ERROR;
    }
    token_stack_move(token_stack, 1);

    if (result != NULL)
      *result = d;

    return SUCCEEDED;
  }

  return FAILED;
}


int parse_type_string(struct token_stack_root *token_stack, char *string, int buffer_max, int *length) {

  char e = token_stack_get_current_token(token_stack);

  if(string != NULL && buffer_max > 0 && e == '"') {
    int k = 0;

    token_stack_set_state_flag(token_stack, TS_STATE_IN_STRING, TS_STATE_FLAG_GLOBAL);
    token_stack_move(token_stack, 1);

    for (k = 0; k < buffer_max - 1;) {
      e = token_stack_get_current_token(token_stack);
      token_stack_move(token_stack, 1);

      if (e == '\\' && token_stack_get_current_token(token_stack) == '"') {
        string[k++] = '"';
        token_stack_move(token_stack, 1);
        continue;
      }

      if (e == '"')
        break;

      if (e == 0 || e == 0x0A) {
        token_stack_clear_state_flag(token_stack, TS_STATE_IN_STRING, TS_STATE_FLAG_GLOBAL);
        print_error("String wasn't terminated properly.\n", ERROR_NUM);
        return FAILED_WITH_ERROR;
      }

      string[k++] = e;
    }

    token_stack_clear_state_flag(token_stack, TS_STATE_IN_STRING, TS_STATE_FLAG_GLOBAL);
    string[k] = 0;

    if (k == buffer_max - 1) {
      if (input_number_error_msg == YES) {
        sprintf(xyz, "The string is too long (max %d bytes).\n", buffer_max - 1);
        print_error(xyz, ERROR_NUM);
      }
      return FAILED_WITH_ERROR;
    }

    string[k] = 0;
    if (length != NULL)
      *length = k;
    return INPUT_NUMBER_STRING;
  }

  return FAILED;
}


const struct operator_type *parse_type_operator(struct token_stack_root *token_stack) {

  int operator_index;

  /* Determine if the next token is an operator. */
  for (operator_index = 0; operator_index < OPERATOR_COUNT; ++operator_index) {
    const struct operator_type *cur_op = &operator_list[operator_index];

    if (token_stack_strncmp(token_stack, cur_op->string, cur_op->string_len, 0) == 0)
      return cur_op;
  }

  return NULL;
}


int parse_type_argument_to_string(struct token_stack_root *token_stack, char *string, int buffer_max) {

  int paren_stack = 0;
  int token_index = 0;
  int result = FAILED;

  /* Read the contents of token_stack and store them in string. */
  char current_token = token_stack_get_current_token(token_stack);
  while (current_token != ',' && current_token != '\x0A' && current_token != '\0') {
    if (paren_stack > 0) {
      /* Ignore whitespace if we are within a () expression. */
      parse_while_any_token(token_stack, " ");
    }
    else if (paren_stack == 0) {
      if (current_token == ' ') {
        /* End this argument. */
        break;
      }
    }

    if (current_token == '"') {
      int string_len = 0;
      
      string[token_index] = '"';
      /* Parse this token as a string. */
      result = parse_type_string(token_stack, &string[token_index + 1], buffer_max - (token_index + 1), &string_len);

      if (result == INPUT_NUMBER_STRING) {
        token_index += string_len + 1;
        string[token_index] = '"';
        token_index++;
        result = SUCCEEDED;
      }
      else if (result == FAILED_WITH_ERROR) {
        /* Exit the loop error parsing the stack. */
        break;
      }
    }
    else {
      const struct operator_type *operator_result = parse_type_operator(token_stack);

      if (operator_result != NULL) {
        if (operator_result->operator == SI_OP_LEFT) {
          /* Increment the stack count. */
          paren_stack++;
        }
        else if (operator_result->operator == SI_OP_RIGHT) {
          /* Decrement the stack count. */
          paren_stack--;

          if (paren_stack < 0) {
            /* Print an error, too many closing parens. */
            print_error("Unbalanced parentheses.\n", ERROR_STC);
            result = FAILED_WITH_ERROR;
            break;
          }
        }

        /* Append the operator contents to the buffer. */
        sprintf(&string[token_index], "%s", operator_result->string);
        token_index += operator_result->string_len;
        result = SUCCEEDED;
      }
      else {
        /* Save the character and move to the next entry. */
        string[token_index] = current_token;
        token_index++;
        token_stack_move(token_stack, 1);
        result = SUCCEEDED;
      }
    }

    current_token = token_stack_get_current_token(token_stack);
  }

  if (result == SUCCEEDED && paren_stack > 0) {
    /* Print an error, too many closing parens. */
    print_error("Unbalanced parentheses.\n", ERROR_STC);
    result = FAILED_WITH_ERROR;
  }

  if (result != SUCCEEDED)
    token_index = 0;

  string[token_index] = '\0';

  return result;
}


int parse_type_label(struct token_stack_root *token_stack, char *string, int buffer_max) {

  char e = token_stack_get_current_token(token_stack);
  int k;

  if (string != NULL && buffer_max > 0) {
    token_stack_move(token_stack, 1);
    string[0] = e;
    for (k = 1; k < buffer_max - 1; k++) {
      e = token_stack_get_current_token(token_stack);
      token_stack_move(token_stack, 1);
      if (e == '\0' || e == 0x0A || e == ',' || e == ')' || e == ']') {
        token_stack_move(token_stack, -1);
        break;
      }
      else if (e == ' ')
        break;
      string[k] = e;
    }

    if (k == buffer_max - 1) {
      if (input_number_error_msg == YES) {
        sprintf(xyz, "The label is too long (max %d bytes).\n", buffer_max - 1);
        print_error(xyz, ERROR_NUM);
      }
      return FAILED_WITH_ERROR;
    }

    string[k] = 0;

    return INPUT_NUMBER_ADDRESS_LABEL;
  }

  return FAILED;
}


int parse_hint_attribute(struct token_stack_root *token_stack, int hint) {

  char e = token_stack_get_current_token(token_stack);

  if (e == '.') {
    e = token_stack_peek_token(token_stack, 1);
    if (e == 'b' || e == 'B') {
      token_stack_move(token_stack, 2);
      hint = HINT_8BIT;
    }
    else if (e == 'w' || e == 'W') {
      token_stack_move(token_stack, 2);
      hint = HINT_16BIT;
    }
#if defined(W65816)
    else if (e == 'l' || e == 'L') {
      token_stack_move(token_stack, 2);
      hint = HINT_24BIT;
    }
#endif
  }

  return hint;
}


int parse_hint_by_value(int value, int hint) {

#if defined(W65816)
  if (value > 0xFFFF && value <= 0xFFFFFF)
    hint = HINT_24BIT;
  else if (value > 0xFF)
    hint = HINT_16BIT;
  else
    hint = HINT_8BIT;
#elif defined(MCS6502) || defined(MCS6510) || defined(WDC65C02) || defined(HUC6280) || defined(GB) || defined(SPC700) || defined(Z80)
  if (value > 0xFF && value <= 0xFFFF)
    hint = HINT_16BIT;
  else
    hint = HINT_8BIT;
#endif

  return hint;
}


int parse_resolve_label(char *string, int *string_len, int *value, int *hint, struct definition **label_def) {

  char label_tmp[MAX_NAME_LENGTH];
  int source_length = strlen(string);
  int local_hint = HINT_NONE;
  struct token_stack_root *token_stack = token_stack_new(string, source_length);

  /* Seek backwards to parse the attribute. */
  token_stack_push_state(token_stack);
  token_stack_move(token_stack, -2);

  local_hint = parse_hint_attribute(token_stack, local_hint);

  /* Drop the hint if one was found. */
  if (local_hint != HINT_NONE) {
    *hint = local_hint;
    string[source_length - 2] = '\0';
  }

  token_stack_pop_state(token_stack, 0);

  /* label_tmp contains the label without possible prefix ':' */
  if (strlen(string) > 1 && string[0] == ':')
    strcpy(label_tmp, &string[1]);
  else
    strcpy(label_tmp, string);

  /* check if the label is actually a definition */
  if (hashmap_get(defines_map, string, (void*)label_def) != MAP_OK)
    hashmap_get(defines_map, label_tmp, (void*)label_def);

  if ((*label_def) != NULL) {
    if ((*label_def)->type == DEFINITION_TYPE_VALUE) {
      (*value) = (int)(*label_def)->value;

      *hint = parse_hint_by_value(*value, *hint);
      return SUCCEEDED;
    }
    else if ((*label_def)->type == DEFINITION_TYPE_STACK) {
      /* skip stack definitions -> use its name instead */
    }
    else if ((*label_def)->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      if (string[0] == ':') {
        /* we need to keep the ':' prefix */
        sprintf(string, ":%s%c", (*label_def)->string, 0);
        *string_len = (*label_def)->size + 1;
      }
      else {
        *string_len = (*label_def)->size;
        memcpy(string, (*label_def)->string, *string_len);
        string[*string_len] = 0;
      }
      return INPUT_NUMBER_ADDRESS_LABEL;
    }
    else if ((*label_def)->type == DEFINTIION_TYPE_MACRO_ARG) {
      int parsed_macro = FAILED_WITH_ERROR;
      struct token_stack_root *macro_stack = token_stack_new((*label_def)->string, 0);
      char backup_string[MAX_NAME_LENGTH];

      strcpy(backup_string, string);

      parsed_macro = input_number_token(macro_stack);

      token_stack_free(macro_stack);

      if (parsed_macro == FAILED)
        parsed_macro = FAILED_WITH_ERROR;
      else if (parsed_macro == INPUT_NUMBER_STACK)
        strcpy(string, backup_string);
      else if (parsed_macro == INPUT_NUMBER_ADDRESS_LABEL) {
        if (backup_string[0] == ':') {
          /* we need to keep the ':' prefix */
          sprintf(backup_string, ":%s%c", label, 0);
          strcpy(string, backup_string);
          *string_len = strlen(string);
        }
        else {
          strcpy(string, label);
          *string_len = strlen(string);
        }
      }

      return parsed_macro;
    }
    else {
      *string_len = (*label_def)->size;
      memcpy(string, (*label_def)->string, *string_len);
      string[*string_len] = 0;
      return INPUT_NUMBER_STRING;
    }
  }

  return INPUT_NUMBER_ADDRESS_LABEL;
}


int parse_validate_macro_bounds(struct token_stack_root *token_stack) {

  /* The normal macro flow should never reach this point.
     Finding a macro at this point means there was an error with the bounds. */
  if (macro_active != 0) {
    int macro_index = 0;
    int valid_number = 0;
    char current_token = token_stack_get_current_token(token_stack);

    if (current_token == '\\') {
      token_stack_push_state(token_stack);
      token_stack_move(token_stack, 1);
      current_token = token_stack_get_current_token(token_stack);

      while (current_token >= '0' && current_token <= '9') {
        /* Get the argument number. */
        macro_index *= 10;
        macro_index += current_token - '0';
        valid_number = 1;

        token_stack_move(token_stack, 1);
        current_token = token_stack_get_current_token(token_stack);
      }

      if (valid_number != 0) {
        if (macro_index == 0 || macro_index > macro_runtime_current->supplied_arguments) {
          sprintf(xyz, "Attemped to index invalid macro argument \"%i\" in macro \"%s\".\n", macro_index, 
            macro_runtime_current->macro->name);
          print_error(xyz, ERROR_ERR);
          token_stack_pop_state(token_stack, 1);
          return FAILED_WITH_ERROR;
        }
      }
      token_stack_pop_state(token_stack, 0);
    }
  }

  return FAILED;
}

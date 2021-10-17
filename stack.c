
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "defines.h"

#include "hashmap.h"
#include "parse.h"
#include "pass_1.h"
#include "stack.h"
#include "include_file.h"
#include "printf.h"


extern int g_input_number_error_msg, g_bankheader_status, g_input_float_mode, g_global_label_hint, g_input_parse_if;
extern int g_source_pointer, g_size, g_parsed_int, g_macro_active, g_string_size, g_section_status, g_parse_floats;
extern char g_xyz[512], *g_buffer, g_tmp[4096], g_expanded_macro_string[256], g_label[MAX_NAME_LENGTH + 1];
extern struct definition *g_tmp_def;
extern struct map_t *g_defines_map;
extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;
extern struct macro_runtime *g_macro_runtime_current;
extern struct section_def *g_sec_tmp;
extern double g_parsed_double;
extern int g_operand_hint, g_operand_hint_type;

int g_latest_stack = 0, g_stacks_inside = 0, g_stacks_outside = 0, g_stack_id = 0;
struct stack *g_stacks_first = NULL, *g_stacks_tmp = NULL, *g_stacks_last = NULL;
struct stack *g_stacks_header_first = NULL, *g_stacks_header_last = NULL;

#ifndef GB
extern int g_stack_inserted;
#endif

static int g_is_calculating_deltas = NO, g_delta_counter = 0, g_delta_section = -1, g_delta_address = -1, g_delta_old_type = 0;
static struct stack_item *g_delta_old_pointer;


static int _stack_insert(void) {

  /* outside bankheader sections */
  if (g_bankheader_status == OFF) {
    if (g_stacks_first == NULL) {
      g_stacks_first = g_stacks_tmp;
      g_stacks_last = g_stacks_tmp;
    }
    else {
      g_stacks_last->next = g_stacks_tmp;
      g_stacks_last = g_stacks_tmp;
    }
    g_stacks_outside++;

#ifndef GB
    g_stack_inserted = STACK_OUTSIDE;
#endif
  }
  /* inside a bankheader section */
  else {
    if (g_stacks_header_first == NULL) {
      g_stacks_header_first = g_stacks_tmp;
      g_stacks_header_last = g_stacks_tmp;
    }
    else {
      g_stacks_header_last->next = g_stacks_tmp;
      g_stacks_header_last = g_stacks_tmp;
    }
    g_stacks_inside++;

#ifndef GB
    g_stack_inserted = STACK_INSIDE;
#endif
  }

  g_stacks_tmp->id = g_stack_id;
  g_stacks_tmp->section_status = g_section_status;
  if (g_section_status == ON)
    g_stacks_tmp->section_id = g_sec_tmp->id;
  else
    g_stacks_tmp->section_id = 0;

  g_latest_stack = g_stack_id;
  g_stack_id++;

  return SUCCEEDED;
}


static int _break_before_value_or_string(int i, struct stack_item *si) {

  /* we use this function to test if the previous item in the stack
     is something that cannot be followed by a value or a string.
     in such a case we'll stop adding items to this stack computation */
  
  if (i <= 0)
    return FAILED;

  si = &si[i-1];
  if (si->type == STACK_ITEM_TYPE_VALUE)
    return SUCCEEDED;
  if (si->type == STACK_ITEM_TYPE_STRING)
    return SUCCEEDED;
  if (si->type == STACK_ITEM_TYPE_OPERATOR && si->value == SI_OP_RIGHT)
    return SUCCEEDED;

  return FAILED;
}


#if WLA_DEBUG
static void _debug_print_stack(int line_number, int stack_id, struct stack_item *ta, int count, int id) {

  int k;
  
  printf("LINE %5d: ID = %d (STACK) CALCULATION ID = %d (c%d): ", line_number, id, stack_id, stack_id);

  for (k = 0; k < count; k++) {
    char ar[] = "+-*()|&/^01%~<>!:<>";

    if (ta[k].type == STACK_ITEM_TYPE_OPERATOR) {
      int value = (int)ta[k].value;

      if (value == SI_OP_SHIFT_LEFT)
        printf("<<");
      else if (value == SI_OP_SHIFT_RIGHT)
        printf(">>");
      else if (value == SI_OP_COMPARE_EQ)
        printf("==");
      else if (value == SI_OP_COMPARE_NEQ)
        printf("!=");
      else if (value == SI_OP_COMPARE_LTE)
        printf("<=");
      else if (value == SI_OP_COMPARE_GTE)
        printf(">=");
      else if (value == SI_OP_LOGICAL_OR)
        printf("||");
      else if (value == SI_OP_LOGICAL_AND)
        printf("&&");
      else if (value == SI_OP_LOW_WORD)
        printf("loword()");
      else if (value == SI_OP_HIGH_WORD)
        printf("hiword()");
      else if (value == SI_OP_BANK_BYTE)
        printf("bankbyte()");
      else {
        if (value >= (int)strlen(ar)) {
          printf("ERROR!\n");
          printf("_debug_print_stack(): ERROR: Unhandled SI_OP_* (%d)! Please submit a bug report!\n", value);
          exit(1);
        }
        printf("%c", ar[value]);
      }
    }
    else if (ta[k].type == STACK_ITEM_TYPE_VALUE)
      printf("V(%f)", ta[k].value);
    else if (ta[k].type == STACK_ITEM_TYPE_STACK)
      printf("C(%d)", (int)ta[k].value);
    else
      printf("S(%s)", ta[k].string);

    if (k < count-1)
      printf(", ");
  }
  printf("\n");
}
#endif


int get_label_length(char *l) {

  int length;
  
  hashmap_get(g_defines_map, l, (void*)&g_tmp_def);

  if (g_tmp_def != NULL) {
    if (g_tmp_def->type == DEFINITION_TYPE_STRING)
      return (int)strlen(g_tmp_def->string);
    else {
      snprintf(g_xyz, sizeof(g_xyz), "Definition \"%s\" is not a string definition. .length returns 0 for that...\n", l);
      print_error(g_xyz, ERROR_NUM);
      return 0;
    }
  }

  length = (int)strlen(l);

  if (l[0] == '"' && l[length-1] == '"')
    length -= 2;

  return length;
}


static struct stack_item_priority_item g_stack_item_priority_items[] = {
  { SI_OP_LOGICAL_OR, 10 },
  { SI_OP_LOGICAL_AND, 20 },
  { SI_OP_OR, 30 },
  { SI_OP_XOR, 40 },
  { SI_OP_AND, 50 },
  { SI_OP_COMPARE_EQ, 60 },
  { SI_OP_COMPARE_NEQ, 60 },
  { SI_OP_COMPARE_LT, 70 },
  { SI_OP_COMPARE_GT, 70 },
  { SI_OP_COMPARE_LTE, 70 },
  { SI_OP_COMPARE_GTE, 70 },
  { SI_OP_SHIFT_LEFT, 80 },
  { SI_OP_SHIFT_RIGHT, 80 },
  { SI_OP_ADD, 90 },
  { SI_OP_SUB, 90 },
  { SI_OP_MULTIPLY, 100 },
  { SI_OP_DIVIDE, 100 },
  { SI_OP_MODULO, 100 },
  { SI_OP_POWER, 100 },
  { SI_OP_LOW_BYTE, 110 },
  { SI_OP_HIGH_BYTE, 110 },
  { SI_OP_LOW_WORD, 110 },
  { SI_OP_HIGH_WORD, 110 },
  { SI_OP_BANK, 110 },
  { SI_OP_BANK_BYTE, 110 },
  { SI_OP_NOT, 120 },
  { 999, 999 }
};


static int _get_op_priority(int op) {

  int i = 0;
  
  while (g_stack_item_priority_items[i].op < 999) {
    if (g_stack_item_priority_items[i].op == op)
      return g_stack_item_priority_items[i].priority;
    i++;
  }

  fprintf(stderr, "_get_op_priority(): No priority for OP %d! Please submit a bug report\n", op);

  return 0;
}


int stack_calculate(char *in, int *value) {

  int q = 0, b = 0, d, k, op[256], n, o, l, curly_braces = 0, got_label = NO;
  struct stack_item si[256], ta[256];
  struct stack s;
  unsigned char e;
  double dou = 0.0, dom;


  /* initialize (from Amiga's SAS/C) */
  for (k = 0; k < 256; k++) {
    si[k].type = STACK_ITEM_TYPE_VALUE;
    si[k].sign = SI_SIGN_POSITIVE;
    si[k].value = 0.0;
    si[k].string[0] = 0;
  }

  /* slice the data into infix format */
  while (*in != 0xA) {
    if (q >= 255) {
      print_error("Out of stack space.\n", ERROR_STC);
      return FAILED;
    }

    /* init the stack item */
    si[q].type = 0x123456;
    si[q].sign = 0x123456;
    si[q].value = 0x123456;
    si[q].string[0] = 0;

    if (*in == ' ') {
      in++;
      continue;
    }
    else if (*in == '-') {
      if (*(in + 1) == '-') {
        si[q].type = STACK_ITEM_TYPE_STRING;
        si[q].sign = SI_SIGN_POSITIVE;
        for (k = 0; *in == '-' && k < 32; k++, in++) {
          si[q].string[k] = '-';
        }
        si[q].string[k] = 0;
      }
      else {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_SUB;
        in++;
      }
      q++;
    }
    else if (*in == '+') {
      if (*(in + 1) == '+') {
        si[q].type = STACK_ITEM_TYPE_STRING;
        si[q].sign = SI_SIGN_POSITIVE;
        for (k = 0; *in == '+' && k < 32; k++, in++)
          si[q].string[k] = '+';
        si[q].string[k] = 0;
      }
      else {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_ADD;
        in++;
      }
      q++;
    }
    else if (*in == '*') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_MULTIPLY;
      q++;
      in++;
    }
    else if (*in == '/') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_DIVIDE;
      q++;
      in++;
    }
    else if (*in == '|' && *(in + 1) == '|') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_LOGICAL_OR;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '|') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_OR;
      q++;
      in++;
    }
    else if (*in == '&' && *(in + 1) == '&') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_LOGICAL_AND;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '&') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_AND;
      q++;
      in++;
    }
    else if (*in == '^') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_POWER;
      q++;
      in++;
    }
    else if (*in == '#') {
      if (q == 0) {
        if (g_input_number_error_msg == YES)
          print_error("Syntax error. Invalid use of modulo.\n", ERROR_STC);
        return FAILED;
      }
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_MODULO;
      q++;
      in++;
    }
    else if (*in == '<' && *(in + 1) == '<') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_SHIFT_LEFT;
      q++;
      in += 2;
    }
    else if (*in == '<' && *(in + 1) == '=') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_LTE;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '<') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_LT;
        q++;
        in++;
      }
      else {
        /* should we end parsing here? */
        if (b == 0 && q > 0) {
          if ((si[q-1].type == STACK_ITEM_TYPE_OPERATOR && si[q-1].value == SI_OP_RIGHT) ||
              si[q-1].type == STACK_ITEM_TYPE_VALUE || si[q-1].type == STACK_ITEM_TYPE_STRING)
            break;
        }

        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_LOW_BYTE;
        q++;
        in++;
      }
    }
    else if (*in == '>' && *(in + 1) == '>') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_SHIFT_RIGHT;
      q++;
      in += 2;
    }
    else if (*in == '>' && *(in + 1) == '=') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_GTE;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '>') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_GT;
        q++;
        in++;
      }
      else {
        /* should we end parsing here? */
        if (b == 0 && q > 0) {
          if ((si[q-1].type == STACK_ITEM_TYPE_OPERATOR && si[q-1].value == SI_OP_RIGHT) ||
              si[q-1].type == STACK_ITEM_TYPE_VALUE || si[q-1].type == STACK_ITEM_TYPE_STRING)
            break;
        }

        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_HIGH_BYTE;
        q++;
        in++;
      }
    }
    else if (*in == '~') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_XOR;
      q++;
      in++;
    }
    else if (*in == ':') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_BANK;
      q++;
      in++;
    }
    else if (*in == '=' && *(in + 1) == '=') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_EQ;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '!' && *(in + 1) == '=') {
      if (g_input_parse_if == YES) {
        si[q].type = STACK_ITEM_TYPE_OPERATOR;
        si[q].value = SI_OP_COMPARE_NEQ;
        q++;
        in += 2;
      }
      else
        break;
    }
    else if (*in == '!') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_NOT;
      q++;
      in++;
    }
    else if (*in == '(') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_LEFT;
      q++;
      b++;
      in++;
    }
    else if (*in == '.' && (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L' || *(in+1) == 'd' || *(in+1) == 'D')) {
      in++;
      d = g_operand_hint;
      if (*in == 'b' || *in == 'B') {
        g_operand_hint = HINT_8BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else if (*in == 'w' || *in == 'W') {
        g_operand_hint = HINT_16BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else if (*in == 'l' || *in == 'L') {
        g_operand_hint = HINT_24BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else if (*in == 'd' || *in == 'D') {
        g_operand_hint = HINT_32BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else
        break;

      if (d != HINT_NONE && d != g_operand_hint) {
        print_error("Confusing operand hint!\n", ERROR_STC);
        in++;
      }
    }
    else if (*in == ')') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_RIGHT;
      /* end of expression? */
      if (b == 0)
        break;
      b--;
      q++;
      in++;
    }
    else if (*in == ',')
      break;
    else if (*in == ']')
      break;
    else if (*in == '%' || (*in == '0' && (*(in+1) == 'b' || *(in+1) == 'B'))) {
      if (*in == '0')
        in++;
      d = 0;
      for (k = 0; k < 32; k++) {
        in++;
        e = *in;
        if (e == '0' || e == '1')
          d = (d << 1) + (e - '0');
        else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' || e == '*' ||
                 e == '/' || e == ',' || e == '^' || e == '<' || e == '>' || e == '#' || e == '~' ||
                 e == ']' || e == '.' || e == '=' || e == '!' || e == 0xA)
          break;
        else {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "Got '%c' (%d) when expected a 0 or 1.\n", e, e);
            print_error(g_xyz, ERROR_NUM);
          }
          return FAILED;
        }
      }

      if (k == 32)
        in++;

      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in == '\'') {
      in++;
      if (*in == '\\' && (*(in+1) == 't' || *(in+1) == 'r' || *(in+1) == 'n' || *(in+1) == '0')) {
        in++;
        if (*in == 't')
          d = '\t';
        else if (*in == 'r')
          d = '\r';
        else if (*in == 'n')
          d = '\n';
        else if (*in == '0')
          d = '\0';
      }
      else
        d = *in;
      in++;
      if (*in != '\'') {
        snprintf(g_xyz, sizeof(g_xyz), "Got '%c' (%d) when expected \"'\".\n", *in, *in);
        print_error(g_xyz, ERROR_NUM);
        return FAILED;
      }
      in++;

      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in == '$' || (*in == '0' && (*(in+1) == 'x' || *(in+1) == 'X'))) {
      int needs_shifting = NO;
      
      /* we'll break if the previous item in the stack was a value or a string / label */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;

      if (*in == '0')
        in++;
      
      d = 0;
      for (k = 0, in++; k < 8; k++, in++) {
        e = *in;
        if (e >= '0' && e <= '9')
          d += e - '0';
        else if (e >= 'A' && e <= 'F')
          d += e - 'A' + 10;
        else if (e >= 'a' && e <= 'f')
          d += e - 'a' + 10;
        else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' ||
                 e == '*' || e == '/' || e == ',' || e == '^' || e == '<' || e == '>' ||
                 e == '#' || e == '~' || e == ']' || e == '.' || e == '=' || e == '!' || e == 0xA) {
          needs_shifting = YES;
          break;
        }
        else {
          if (g_input_number_error_msg == YES) {
            snprintf(g_xyz, sizeof(g_xyz), "Got '%c' (%d) when expected [0-F].\n", e, e);
            print_error(g_xyz, ERROR_NUM);
          }
          return FAILED;
        }

        if (k < 7)
          d = d << 4;
      }

      if (needs_shifting == YES)
        d = d >> 4;

      if (*in == 'h' || *in == 'H')
        in++;
      
      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in >= '0' && *in <= '9') {
      /* we'll break if the previous item in the stack was a value or a string / label */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;

      /* is it a hexadecimal value after all? */
      n = 0;
      for (k = 0; k < 9; k++) {
        if (in[k] >= '0' && in[k] <= '9')
          continue;
        if (in[k] >= 'a' && in[k] <= 'f') {
          n = 1;
          break;
        }
        if (in[k] >= 'A' && in[k] <= 'F') {
          n = 1;
          break;
        }
        if (in[k] == 'h' || in[k] == 'H') {
          n = 1;
          break;
        }
        break;
      }

      if (n == 1) {
        /* it's hex */
        int needs_shifting = NO;

        d = 0;
        for (k = 0; k < 8; k++, in++) {
          e = *in;
          if (e >= '0' && e <= '9')
            d += e - '0';
          else if (e >= 'A' && e <= 'F')
            d += e - 'A' + 10;
          else if (e >= 'a' && e <= 'f')
            d += e - 'a' + 10;
          else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' ||
                   e == '*' || e == '/' || e == ',' || e == '^' || e == '<' || e == '>' ||
                   e == '#' || e == '~' || e == ']' || e == '.' || e == 'h' || e == 'H' ||
                   e == '=' || e == '!' || e == 0xA) {
            needs_shifting = YES;
            break;
          }
          else {
            if (g_input_number_error_msg == YES) {
              snprintf(g_xyz, sizeof(g_xyz), "Got '%c' (%d) when expected [0-F].\n", e, e);
              print_error(g_xyz, ERROR_NUM);
            }
            return FAILED;
          }

          if (k < 7)
            d = d << 4;
        }

        if (needs_shifting == YES)
          d = d >> 4;

        if (*in == 'h' || *in == 'H')
          in++;

        si[q].type = STACK_ITEM_TYPE_VALUE;
        si[q].value = d;
        si[q].sign = SI_SIGN_POSITIVE;
        q++;
      }
      else {
        int max_digits = 10;
        
        /* it's decimal */
        dou = (*in - '0')*10.0;
        dom = 1.0;
        n = 0;
        for (k = 0; k < max_digits; k++) {
          in++;
          e = *in;
          if (e >= '0' && e <= '9') {
            if (k == max_digits - 1) {
              if (n == 0)
                print_error("Too many digits in the integer value. Max 10 is supported.\n", ERROR_NUM);
              else {
                snprintf(g_xyz, sizeof(g_xyz), "Too many digits in the floating point value. Max %d is supported.\n", MAX_FLOAT_DIGITS);
                print_error(g_xyz, ERROR_NUM);
              }
              return FAILED;
            }

            if (n == 0) {
              dou += e - '0';
              dou *= 10.0;
            }
            else if (n == 1) {
              dou += dom*(e - '0');
              dom /= 10.0;
            }
          }
          else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' || e == '*' ||
                   e == '/' || e == ',' || e == '^' || e == '<' || e == '>' || e == '#' || e == '~' ||
                   e == ']' || e == '=' || e == '!' || e == 0xA)
            break;
          else if (e == '.') {
            if (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L' || *(in+1) == 'd' || *(in+1) == 'D')
              break;
            if (g_parse_floats == NO)
              break;
            if (n == 1) {
              if (g_input_number_error_msg == YES)
                print_error("Syntax error.\n", ERROR_NUM);
              return FAILED;
            }
            n = 1;
            max_digits = MAX_FLOAT_DIGITS+1;
          }
          else {
            if (g_input_number_error_msg == YES) {
              snprintf(g_xyz, sizeof(g_xyz), "Got '%c' (%d) when expected [0-9].\n", e, e);
              print_error(g_xyz, ERROR_NUM);
            }
            return FAILED;
          }
        }

        dou /= 10;

        si[q].type = STACK_ITEM_TYPE_VALUE;
        si[q].value = dou;
        si[q].sign = SI_SIGN_POSITIVE;
        q++;
      }
    }
    else if (*in == '"') {
      /* definitely a string! */

      /* we'll break if the previous item in the stack was a value or a string / label */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;

      /* skip '"' */
      in++;
      
      si[q].sign = SI_SIGN_POSITIVE;
      for (k = 0; k < MAX_NAME_LENGTH; k++) {
        e = *in++;

        if (e == 0xA)
          break;
        if (e == '"')
          break;
        if (e == '\\' && *in == '"')
          e = *in++;

        si[q].string[k] = e;        
      }

      if (e != '"') {
        print_error("Malformed string.\n", ERROR_NUM);
        return FAILED;
      }
      
      si[q].string[k] = 0;
      si[q].type = STACK_ITEM_TYPE_STRING;
      q++;

      if (process_string_for_special_characters(si[q].string, NULL) == FAILED)
        return FAILED;
    }
    else {
      /* it must be a string! */
      int is_string = YES, is_already_processed_function = NO;

      /* we'll break if the previous item in the stack was a value or a string / label */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;

      si[q].sign = SI_SIGN_POSITIVE;
      for (k = 0; k < MAX_NAME_LENGTH; k++) {
        e = *in;

        if (e == '{')
          curly_braces++;
        else if (e == '}')
          curly_braces--;

        if (curly_braces <= 0) {
          if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' || e == '*' ||
              e == '/' || e == ',' || e == '^' || e == '<' || e == '>' || e == '#' || e == ']' ||
              e == '~' || e == '=' || e == '!' || e == 0xA)
            break;
          if (e == '.' && (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L' || *(in+1) == 'd' || *(in+1) == 'D') &&
              (*(in+2) == ' ' || *(in+2) == ')' || *(in+2) == '|' || *(in+2) == '&' || *(in+2) == '+' || *(in+2) == '-' || *(in+2) == '*' ||
               *(in+2) == '/' || *(in+2) == ',' || *(in+2) == '^' || *(in+2) == '<' || *(in+2) == '>' || *(in+2) == '#' || *(in+2) == ']' ||
               *(in+2) == '~' || *(in+2) == 0xA))
            break;
        }
        else if (e == 0xA)
          break;
        
        si[q].string[k] = e;
        in++;

        if (k == 3 && strcaselesscmpn(si[q].string, "asc(", 4) == 0) {
          int parsed_chars = 0;
          
          if (parse_function_asc(in, &d, &parsed_chars) == FAILED)
            return FAILED;
          in += parsed_chars;
          is_string = NO;
          break;
        }
        if (k == 7 && strcaselesscmpn(si[q].string, "defined(", 8) == 0) {
          int parsed_chars = 0;
          
          if (parse_function_defined(in, &d, &parsed_chars) == FAILED)
            return FAILED;
          in += parsed_chars;
          is_string = NO;
          break;
        }
        if (k == 6 && strcaselesscmpn(si[q].string, "exists(", 7) == 0) {
          int parsed_chars = 0;

          if (parse_function_exists(in, &d, &parsed_chars) == FAILED)
            return FAILED;
          in += parsed_chars;
          is_string = NO;          
          break;
        }
        if (k == 6 && strcaselesscmpn(si[q].string, "lobyte(", 7) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_LOW_BYTE;
          in--;
          is_already_processed_function = YES;
          break;
        }
        if (k == 6 && strcaselesscmpn(si[q].string, "hibyte(", 7) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_HIGH_BYTE;
          in--;
          is_already_processed_function = YES;
          break;
        }
        if (k == 6 && strcaselesscmpn(si[q].string, "loword(", 7) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_LOW_WORD;
          in--;
          is_already_processed_function = YES;
          break;
        }
        if (k == 6 && strcaselesscmpn(si[q].string, "hiword(", 7) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_HIGH_WORD;
          in--;
          is_already_processed_function = YES;
          break;
        }
        if (k == 8 && strcaselesscmpn(si[q].string, "bankbyte(", 9) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_BANK_BYTE;
          in--;
          is_already_processed_function = YES;
          break;
        }
        if (k == 4 && strcaselesscmpn(si[q].string, "bank(", 5) == 0) {
          si[q].type = STACK_ITEM_TYPE_OPERATOR;
          si[q].value = SI_OP_BANK;
          in--;
          is_already_processed_function = YES;
          break;
        }
      }

      if (is_already_processed_function == YES) {
      }
      else if (is_string == YES) {
        process_special_labels(si[q].string);
        si[q].string[k] = 0;
        si[q].type = STACK_ITEM_TYPE_STRING;
        got_label = YES;
      }
      else {
        si[q].type = STACK_ITEM_TYPE_VALUE;
        si[q].value = d;
        si[q].sign = SI_SIGN_POSITIVE;
      }
      
      q++;
    }
  }

  if (b != 0) {
    print_error("Unbalanced parentheses.\n", ERROR_STC);
    return FAILED;
  }

  /* are labels 16-bit by default? */
  if (got_label == YES && g_operand_hint_type != HINT_TYPE_GIVEN && g_global_label_hint == HINT_16BIT) {
    g_operand_hint = HINT_16BIT;
    g_operand_hint_type = HINT_TYPE_GIVEN;
  }
  
  /* only one item found -> let the item parser handle it */
  if (q == 1)
    return STACK_CALCULATE_DELAY;

  /* check if there was data before the computation */
  if (q > 1 && (si[0].type == STACK_ITEM_TYPE_STRING || si[0].type == STACK_ITEM_TYPE_VALUE)) {
    if (si[1].type == STACK_ITEM_TYPE_STRING || si[1].type == STACK_ITEM_TYPE_VALUE)
      return STACK_CALCULATE_DELAY;
    if (si[1].type == STACK_ITEM_TYPE_OPERATOR) {
      if (si[1].value == SI_OP_LEFT)
        return STACK_CALCULATE_DELAY;
    }
  }

#ifdef SPC700
  /* check if the computation is of the form "y+X" or "y+Y" and remove that "+X" or "+Y" */
  if (q > 2 && si[q - 2].type == STACK_ITEM_TYPE_OPERATOR && si[q - 2].value == SI_OP_ADD) {
    if (si[q - 1].type == STACK_ITEM_TYPE_STRING && si[q - 1].string[1] == 0) {
      char w = si[q - 1].string[0];

      if (w == 'x' || w == 'X' || w == 'y' || w == 'Y') {
        q -= 2;
        while (*in != '+')
          in--;
      }
    }
  }
#endif

  /* check if the computation is of the form "+-..." and remove that leading "+" */
  if (q > 2 && si[0].type == STACK_ITEM_TYPE_OPERATOR && si[0].value == SI_OP_ADD &&
      si[1].type == STACK_ITEM_TYPE_OPERATOR && si[1].value == SI_OP_SUB) {
    si[0].type = STACK_ITEM_TYPE_DELETED;
  }

  /* update the source pointer */
  g_source_pointer = (int)(in - g_buffer);

  /* fix the sign in every operand */
  for (b = 1, k = 0; k < q; k++) {
    if (g_input_parse_if == NO) {
      if ((q - k) != 1 && si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].value != SI_OP_BANK &&
          si[k + 1].value != SI_OP_BANK_BYTE && si[k + 1].value != SI_OP_HIGH_BYTE && si[k + 1].value != SI_OP_LOW_BYTE &&
          si[k + 1].value != SI_OP_HIGH_WORD && si[k + 1].value != SI_OP_LOW_WORD) {
        if (si[k].value != SI_OP_LEFT && si[k].value != SI_OP_RIGHT && si[k + 1].value != SI_OP_LEFT && si[k + 1].value != SI_OP_RIGHT) {
          print_error("Error in computation syntax.\n", ERROR_STC);
          return FAILED;
        }
      }
    }
    if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_SUB && b == 1) {
      if (si[k + 1].type == STACK_ITEM_TYPE_VALUE || si[k + 1].type == STACK_ITEM_TYPE_STRING) {
        if (si[k + 1].sign == SI_SIGN_POSITIVE)
          si[k + 1].sign = SI_SIGN_NEGATIVE;
        else
          si[k + 1].sign = SI_SIGN_POSITIVE;
        /* it wasn't a minus operator, it was a sign */
        si[k].type = STACK_ITEM_TYPE_DELETED;
      }
      else if (si[k + 1].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].value == SI_OP_LEFT) {
        o = 1;
        l = k + 2;
        while (o > 0 && l < q) {
          if (si[l].type == STACK_ITEM_TYPE_VALUE || si[l].type == STACK_ITEM_TYPE_STRING) {
            if (si[l].sign == SI_SIGN_POSITIVE)
              si[l].sign = SI_SIGN_NEGATIVE;
            else
              si[l].sign = SI_SIGN_POSITIVE;
          }
          else if (si[l].type == STACK_ITEM_TYPE_OPERATOR) {
            if (si[l].value == SI_OP_LEFT)
              o++;
            else if (si[l].value == SI_OP_RIGHT)
              o--;
          }
          l++;
        }

        if (o != 0) {
          print_error("Unbalanced parentheses.\n", ERROR_STC);
          return FAILED;
        }

        si[k].type = STACK_ITEM_TYPE_DELETED;
      }
    }
    /* remove unnecessary + */
    if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_ADD && b == 1) {
      if (si[k + 1].type == STACK_ITEM_TYPE_VALUE || si[k + 1].type == STACK_ITEM_TYPE_STRING)
        si[k].type = STACK_ITEM_TYPE_DELETED;
      else if (si[k + 1].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].value == SI_OP_LEFT)
        si[k].type = STACK_ITEM_TYPE_DELETED;
    }
    else if (si[k].type == STACK_ITEM_TYPE_VALUE || si[k].type == STACK_ITEM_TYPE_STRING)
      b = 0;
    else if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_LEFT)
      b = 1;
  }

  /* turn unary XORs into NOTs */
  for (b = 1, k = 0; k < q; k++) {
    if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_XOR && b == 1)
      si[k].value = SI_OP_NOT;
    else if (si[k].type == STACK_ITEM_TYPE_VALUE || si[k].type == STACK_ITEM_TYPE_STRING)
      b = 0;
    else if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_LEFT)
      b = 1;
  }

  /* are we calculating deltas between two labels? */
  g_is_calculating_deltas = NO;
  g_delta_counter = 0;
  
  for (k = 0; k < q; k++) {
    if (si[k].type == STACK_ITEM_TYPE_STRING) {
      if (k+2 < q && si[k+1].type == STACK_ITEM_TYPE_OPERATOR && si[k+1].value == SI_OP_SUB && si[k+2].type == STACK_ITEM_TYPE_STRING) {
        k += 2;
        g_is_calculating_deltas = YES;
      }
      else {
        g_is_calculating_deltas = NO;
        break;
      }
    }
  }

  data_stream_parser_free();

  /* convert infix stack into postfix stack */
  for (b = 0, k = 0, d = 0; k < q; k++) {
    /* operands pass through */
    if (si[k].type == STACK_ITEM_TYPE_VALUE) {
      ta[d].type = si[k].type;
      ta[d].value = si[k].value;
      ta[d].sign = si[k].sign;
      d++;
    }
    else if (si[k].type == STACK_ITEM_TYPE_STRING) {
      ta[d].type = si[k].type;
      strcpy(ta[d].string, si[k].string);
      ta[d].sign = si[k].sign;
      d++;
    }
    /* operators get inspected */
    else if (si[k].type == STACK_ITEM_TYPE_OPERATOR) {
      if (b == 0) {
        op[0] = (int)si[k].value;
        b++;
      }
      else {
        if (si[k].value == SI_OP_LEFT) {
          op[b] = SI_OP_LEFT;
          b++;
        }
        else if (si[k].value == SI_OP_RIGHT) {
          b--;
          while (op[b] != SI_OP_LEFT) {
            ta[d].type = STACK_ITEM_TYPE_OPERATOR;
            ta[d].value = op[b];
            b--;
            d++;
          }
        }
        else {
          int priority = _get_op_priority((int)(si[k].value));

          b--;
          while (b != -1 && op[b] != SI_OP_LEFT && _get_op_priority(op[b]) >= priority) {
            ta[d].type = STACK_ITEM_TYPE_OPERATOR;
            ta[d].value = op[b];
            b--;
            d++;
          }
          b++;
          op[b] = (int)si[k].value;
          b++;
        }
      }
    }
  }

  /* empty the operator stack */
  while (b > 0) {
    b--;
    ta[d].type = STACK_ITEM_TYPE_OPERATOR;
    ta[d].value = op[b];
    d++;
  }

  /* are all the symbols known? */
  if (resolve_stack(ta, d) == SUCCEEDED) {
    s.stack = ta;
    s.linenumber = g_active_file_info_last->line_current;
    s.filename_id = g_active_file_info_last->filename_id;

    if (compute_stack(&s, d, &dou) == FAILED)
      return FAILED;
    
    g_parsed_double = dou;

    if (g_input_float_mode == ON)
      return INPUT_NUMBER_FLOAT;

    *value = (int)dou;

    return SUCCEEDED;
  }

  /* only one string? */
  if (d == 1 && ta[0].type == STACK_ITEM_TYPE_STRING) {
    strcpy(g_label, ta[0].string);
    process_special_labels(g_label);
    return STACK_RETURN_LABEL;
  }

  /*
    printf("%d %d %s\n", d, ta[0].type, ta[0].string);
  */

  /* we have a stack full of computation and we save it for wlalink */
  g_stacks_tmp = calloc(sizeof(struct stack), 1);
  if (g_stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  g_stacks_tmp->next = NULL;
  g_stacks_tmp->type = STACK_TYPE_UNKNOWN;
  g_stacks_tmp->bank = -123456;
  g_stacks_tmp->stacksize = d;
  g_stacks_tmp->relative_references = 0;
  g_stacks_tmp->stack = calloc(sizeof(struct stack_item) * d, 1);
  if (g_stacks_tmp->stack == NULL) {
    free(g_stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  g_stacks_tmp->linenumber = g_active_file_info_last->line_current;
  g_stacks_tmp->filename_id = g_active_file_info_last->filename_id;
  g_stacks_tmp->special_id = 0;
  g_stacks_tmp->bits_position = 0;
  g_stacks_tmp->bits_to_define = 0;

  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  g_stacks_tmp->position = STACK_POSITION_DEFINITION;

  for (q = 0; q < d; q++) {
    if (ta[q].type == STACK_ITEM_TYPE_OPERATOR) {
      g_stacks_tmp->stack[q].type = STACK_ITEM_TYPE_OPERATOR;
      g_stacks_tmp->stack[q].value = ta[q].value;
    }
    else if (ta[q].type == STACK_ITEM_TYPE_VALUE) {
      g_stacks_tmp->stack[q].type = STACK_ITEM_TYPE_VALUE;
      g_stacks_tmp->stack[q].value = ta[q].value;
      g_stacks_tmp->stack[q].sign = ta[q].sign;
    }
    else if (ta[q].type == STACK_ITEM_TYPE_STACK) {
      g_stacks_tmp->stack[q].type = STACK_ITEM_TYPE_STACK;
      g_stacks_tmp->stack[q].value = ta[q].value;
      g_stacks_tmp->stack[q].sign = ta[q].sign;
    }
    else {
      g_stacks_tmp->stack[q].type = STACK_ITEM_TYPE_STRING;
      g_stacks_tmp->stack[q].sign = ta[q].sign;
      strcpy(g_stacks_tmp->stack[q].string, ta[q].string);
    }
  }

#if WLA_DEBUG
  _debug_print_stack(g_stacks_tmp->linenumber, g_stack_id, g_stacks_tmp->stack, d, 0);
#endif

  _stack_insert();

  return INPUT_NUMBER_STACK;
}


static int _resolve_string(struct stack_item *s, int *cannot_resolve) {

  if (g_macro_active != 0) {
    /* expand e.g., \1 and \@ */
    if (expand_macro_arguments(s->string) == FAILED)
      return FAILED;
  }

  hashmap_get(g_defines_map, s->string, (void*)&g_tmp_def);
  if (g_tmp_def != NULL) {
    if (g_tmp_def->type == DEFINITION_TYPE_STRING) {
      if (g_input_parse_if == NO) {
        snprintf(g_xyz, sizeof(g_xyz), "Definition \"%s\" is a string definition.\n", g_tmp_def->alias);
        print_error(g_xyz, ERROR_STC);
        return FAILED;
      }
      else {
        *cannot_resolve = 1;
        strcpy(s->string, g_tmp_def->string);
      }
    }
    else if (g_tmp_def->type == DEFINITION_TYPE_STACK) {
      /* turn this reference to a stack calculation define into a direct reference to the stack calculation as */
      /* this way we don't have to care if the define is exported or not as stack calculations are always exported */
      s->type = STACK_ITEM_TYPE_STACK;
      s->sign = SI_SIGN_POSITIVE;
      s->value = g_tmp_def->value;
    }
    else if (g_tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      /* wla cannot resolve address labels (unless outside a section) -> only wlalink can do that */
      *cannot_resolve = 1;
      strcpy(s->string, g_tmp_def->string);
    }
    else {
      s->type = STACK_ITEM_TYPE_VALUE;
      s->value = g_tmp_def->value;
    }
  }
  else {
    if (g_is_calculating_deltas == YES) {
      /* the current calculation we are trying to solve contains at least one label pair A-B, and no other
         uses of labels. if we come here then a label wasn't a definition, but we can try to find the label's
         (possibly) non-final address, and use that as that should work when calculating deltas... */
      struct data_stream_item *dSI;
      
      /* read the labels and their addresses from the internal data stream */
      data_stream_parser_parse();

      dSI = data_stream_parser_find_label(s->string);
      if (dSI != NULL) {
        if (g_delta_counter == 0) {
          g_delta_section = dSI->section_id;
          g_delta_address = dSI->address;

          /* store the pointer and type if we need to reverse this change */
          g_delta_old_pointer = s;
          g_delta_old_type = s->type;

          s->type = STACK_ITEM_TYPE_VALUE;
          s->value = dSI->address;

          g_delta_counter = 1;
        }
        else if (g_delta_counter == 1) {
          if (g_delta_section != dSI->section_id) {
            /* ABORT! labels A-B are from different sections, we cannot calculate the delta here... */

            /* reverse the previous change */
            g_delta_old_pointer->type = g_delta_old_type;

            *cannot_resolve = 1;
          }
          else {
            /* success! A-B makes sense! */
            s->type = STACK_ITEM_TYPE_VALUE;
            s->value = dSI->address;
          }

          g_delta_counter = 0;
        }
      }
    }
  }

  process_special_labels(s->string);
  
  /* is this form "string".length? */
  if (is_string_ending_with(s->string, ".length") > 0 ||
      is_string_ending_with(s->string, ".LENGTH") > 0) {
    /* we have a X.length -> parse */
    s->string[strlen(s->string) - 7] = 0;
    s->value = get_label_length(s->string);
    s->type = STACK_ITEM_TYPE_VALUE;
  }
  
  return SUCCEEDED;
}


static int _process_string(struct stack_item *s, int *cannot_resolve) {

  int try_resolve_string = NO;
  struct macro_argument *ma;
  int a, b, k;
  char c;

  if (g_macro_active != 0 && strlen(s->string) > 1 && s->string[0] == '?' && s->string[1] >= '1' && s->string[1] <= '9') {
    for (a = 0, b = 0; s->string[a + 1] != 0 && a < 10; a++) {
      c = s->string[a + 1];
      if (c < '0' || c > '9')
        return FAILED;
      b = (b * 10) + (c - '0');
    }

    if (b > g_macro_runtime_current->supplied_arguments) {
      snprintf(g_xyz, sizeof(g_xyz), "Referencing argument number %d inside .MACRO \"%s\". The .MACRO has only %d arguments.\n", b, g_macro_runtime_current->macro->name, g_macro_runtime_current->supplied_arguments);
      print_error(g_xyz, ERROR_NUM);
      return FAILED;
    }
    if (b == 0) {
      print_error(".MACRO arguments are counted from 1.\n", ERROR_NUM);
      return FAILED;
    }

    /* use the macro argument to find its definition */
    ma = g_macro_runtime_current->argument_data[b - 1];
    k = ma->type;

    if (k == INPUT_NUMBER_ADDRESS_LABEL) {
      strcpy(s->string, ma->string);

      if (_resolve_string(s, cannot_resolve) == FAILED)
        return FAILED;
    }
    else {
      print_error("? can be only used to evaluate definitions.\n", ERROR_ERR);
      return FAILED;
    }
  }
  else if (g_macro_active != 0 && s->string[0] == '\\') {
    if (s->string[1] == '@' && s->string[2] == 0) {
      s->type = STACK_ITEM_TYPE_VALUE;
      s->value = g_macro_runtime_current->macro->calls - 1;
    }
    else {
      try_resolve_string = NO;
      for (a = 0, b = 0; s->string[a + 1] != 0 && a < 10; a++) {
        c = s->string[a + 1];
        if (c < '0' || c > '9') {
          try_resolve_string = YES;
          break;
        }
        b = (b * 10) + (c - '0');
      }

      if (try_resolve_string == YES) {
        if (_resolve_string(s, cannot_resolve) == FAILED)
          return FAILED;
      }
      else {
        if (b > g_macro_runtime_current->supplied_arguments) {
          snprintf(g_xyz, sizeof(g_xyz), "Reference to .MACRO argument number %d (\"%s\") is out of range. The .MACRO has %d arguments.\n", b, s->string, g_macro_runtime_current->supplied_arguments);
          print_error(g_xyz, ERROR_STC);
          return FAILED;
        }
        if (b == 0) {
          print_error(".MACRO arguments are counted from 1.\n", ERROR_STC);
          return FAILED;
        }
          
        /* return the macro argument */
        ma = g_macro_runtime_current->argument_data[b - 1];
        k = ma->type;
          
        if (k == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(g_label, ma->string);
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
          
        if (!(k == SUCCEEDED || k == INPUT_NUMBER_ADDRESS_LABEL || k == INPUT_NUMBER_STACK))
          return FAILED;
          
        if (k == SUCCEEDED) {
          s->type = STACK_ITEM_TYPE_VALUE;
          s->value = g_parsed_double;
        }
        else if (k == INPUT_NUMBER_STACK) {
          s->type = STACK_ITEM_TYPE_STACK;
          s->value = g_latest_stack;
        }
        else
          strcpy(s->string, g_label);
      }
    }
  }
  else {
    if (_resolve_string(s, cannot_resolve) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int resolve_stack(struct stack_item s[], int x) {

  struct stack_item *st;
  int q = x, cannot_resolve = 0;

  st = s;
  while (x > 0) {
    int process_single = YES;
    
    if (x >= 3 && g_input_parse_if == YES) {
      /* [string] [string] ==/!= ? */
      s += 2;
      x -= 2;
      
      if (s->type == STACK_ITEM_TYPE_OPERATOR && (s->value == SI_OP_COMPARE_EQ || s->value == SI_OP_COMPARE_NEQ || s->value == SI_OP_COMPARE_LT ||
                                                  s->value == SI_OP_COMPARE_GT || s->value == SI_OP_COMPARE_LTE || s->value == SI_OP_COMPARE_GTE)) {
        s -= 2;
        x += 2;

        if (s->type == STACK_ITEM_TYPE_STRING) {
          int cannot;

          if (_process_string(s, &cannot) == FAILED)
            return FAILED;
        }
        
        s++;
        x--;

        if (s->type == STACK_ITEM_TYPE_STRING) {
          int cannot;

          if (_process_string(s, &cannot) == FAILED)
            return FAILED;
        }

        s += 2;
        x -= 2;

        process_single = NO;
      }
      else {
        s -= 2;
        x += 2;
      }
    }

    if (process_single == YES) {
      if (s->type == STACK_ITEM_TYPE_STRING) {
        if (_process_string(s, &cannot_resolve) == FAILED)
          return FAILED;
      }
      s++;
      x--;
    }
  }

  if (cannot_resolve != 0)
    return FAILED;

  /* find a string, a stack, bank, or a NOT and fail */
  while (q > 0) {
    int process_single = YES;
    
    if (q >= 3 && g_input_parse_if == YES) {
      /* [string] [string] ==/!= ? */
      st += 2;
      q -= 2;

      if (st->type == STACK_ITEM_TYPE_OPERATOR && (st->value == SI_OP_COMPARE_EQ || st->value == SI_OP_COMPARE_NEQ || st->value == SI_OP_COMPARE_LT ||
                                                   st->value == SI_OP_COMPARE_GT || st->value == SI_OP_COMPARE_LTE || st->value == SI_OP_COMPARE_GTE)) {
        st -= 2;
        q += 2;

        if (st->type == STACK_ITEM_TYPE_STACK || (st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_BANK))
          return FAILED;

        st++;
        q--;

        if (st->type == STACK_ITEM_TYPE_STACK || (st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_BANK))
          return FAILED;
        
        st += 2;
        q -= 2;

        process_single = NO;
      }
      else {
        st -= 2;
        q += 2;
      }
    }

    if (process_single == YES) {
      if (st->type == STACK_ITEM_TYPE_STRING || st->type == STACK_ITEM_TYPE_STACK || (st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_BANK))
        return FAILED;
      if (g_input_parse_if == NO && st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_NOT)
        return FAILED;
      q--;
      st++;
    }
  }

  return SUCCEEDED;
}


static int _comparing_a_string_with_a_number(char *sp1, char *sp2, struct stack *sta) {

  if ((sp1 != NULL && sp2 == NULL) || (sp1 == NULL && sp2 != NULL)) {  
    fprintf(stderr, "%s:%d: COMPUTE_STACK: Comparison between a string and a number doesn't work.\n", get_file_name(sta->filename_id), sta->linenumber);
    return YES;
  }

  return NO;
}


int compute_stack(struct stack *sta, int x, double *result) {

  struct stack_item *s;
  double v[256];
  char *sp[256];
  int r, t, z;

  v[0] = 0.0;

  s = sta->stack;
  for (r = 0, t = 0; r < x; r++, s++) {
    if (s->type == STACK_ITEM_TYPE_VALUE) {
      if (s->sign == SI_SIGN_NEGATIVE)
        v[t] = -s->value;
      else
        v[t] = s->value;
      sp[t] = NULL;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STRING) {
      sp[t] = s->string;
      v[t] = 0;
      t++;
    }
    else {
      switch ((int)s->value) {
      case SI_OP_ADD:
        v[t - 2] += v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_SUB:
        v[t - 2] -= v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_MULTIPLY:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Multiply is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] *= v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_LOW_BYTE:
        z = (int)v[t - 1];
#ifdef AMIGA
        /* on Amiga this needs to be done twice - a bug in SAS/C? */
        z = z & 0xFF;
#endif
        v[t - 1] = z & 0xFF;
        sp[t - 1] = NULL;
        break;
      case SI_OP_HIGH_BYTE:
        z = ((int)v[t - 1]) >> 8;
#ifdef AMIGA
        /* on Amiga this needs to be done twice - a bug in SAS/C? */
        z = z & 0xFF;
#endif
        v[t - 1] = z & 0xFF;
        sp[t - 1] = NULL;
        break;
      case SI_OP_BANK_BYTE:
        z = ((int)v[t - 1]) >> 16;
#ifdef AMIGA
        /* on Amiga this needs to be done twice - a bug in SAS/C? */
        z = z & 0xFF;
#endif
        v[t - 1] = z & 0xFF;
        sp[t - 1] = NULL;
        break;
     case SI_OP_LOW_WORD:
        z = (int)v[t - 1];
#ifdef AMIGA
        /* on Amiga this needs to be done twice - a bug in SAS/C? */
        z = z & 0xFFFF;
#endif
        v[t - 1] = z & 0xFFFF;
        sp[t - 1] = NULL;
        break;
      case SI_OP_HIGH_WORD:
        z = ((int)v[t - 1]) >> 16;
#ifdef AMIGA
        /* on Amiga this needs to be done twice - a bug in SAS/C? */
        z = z & 0xFFFF;
#endif
        v[t - 1] = z & 0xFFFF;
        sp[t - 1] = NULL;
        break;
      case SI_OP_LOGICAL_OR:
        if (v[t-1] != 0 || v[t-2] != 0)
          v[t-2] = 1;
        else
          v[t-2] = 0;
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_LOGICAL_AND:
        if (v[t-1] != 0 && v[t-2] != 0)
          v[t-2] = 1;
        else
          v[t-2] = 0;
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_LT:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) < 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] < v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_GT:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) > 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] > v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_EQ:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) == 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] == v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_NEQ:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) != 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] != v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_LTE:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) <= 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] <= v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_COMPARE_GTE:
        if (_comparing_a_string_with_a_number(sp[t-2], sp[t-1], sta) == YES)
          return FAILED;
        if (sp[t-2] != NULL && sp[t-1] != NULL) {
          if (strcmp(sp[t-2], sp[t-1]) >= 0)
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        else {
          if (v[t-2] >= v[t-1])
            v[t-2] = 1;
          else
            v[t-2] = 0;
        }
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_NOT:
        if (g_input_parse_if == YES) {
          if ((int)v[t - 1] == 0)
            v[t - 1] = 1;
          else
            v[t - 1] = 0;
          sp[t - 1] = NULL;
        }
        else {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: NOT cannot determine the output size.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        break;
      case SI_OP_XOR:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: XOR is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] ^ (int)v[t - 2];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_OR:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: OR is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] | (int)v[t - 2];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_AND:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: AND is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] & (int)v[t - 2];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_MODULO:
        if (((int)v[t - 1]) == 0) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Modulo by zero.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Modulo is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 2] % (int)v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_DIVIDE:
        if (v[t - 1] == 0.0) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Division by zero.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Division is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] /= v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_POWER:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Power is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = pow(v[t - 2], v[t - 1]);
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_SHIFT_LEFT:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Shift left is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 2] << (int)v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      case SI_OP_SHIFT_RIGHT:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Shift right is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 2] >> (int)v[t - 1];
        sp[t - 2] = NULL;
        t--;
        break;
      }
    }
  }

  /*
    #ifdef W65816
    if (v[0] < -8388608 || v[0] > 16777215) {
    print_error("Out of 24-bit range.\n", ERROR_STC);
    return FAILED;
    }
    #else
    if (v[0] < -32768 || v[0] > 65536) {
    print_error("Out of 16-bit range.\n", ERROR_STC);
    return FAILED;
    }
    #endif
  */

  *result = v[0];

  return SUCCEEDED;
}


int stack_create_label_stack(char *label) {

  if (label == NULL)
    return FAILED;

  /* we need to create a stack that holds just one label */
  g_stacks_tmp = calloc(sizeof(struct stack), 1);
  if (g_stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  g_stacks_tmp->next = NULL;
  g_stacks_tmp->type = STACK_TYPE_UNKNOWN;
  g_stacks_tmp->bank = -123456;
  g_stacks_tmp->stacksize = 1;
  g_stacks_tmp->relative_references = 0;
  g_stacks_tmp->stack = calloc(sizeof(struct stack_item), 1);
  if (g_stacks_tmp->stack == NULL) {
    free(g_stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  g_stacks_tmp->linenumber = g_active_file_info_last->line_current;
  g_stacks_tmp->filename_id = g_active_file_info_last->filename_id;
  g_stacks_tmp->special_id = 0;
  g_stacks_tmp->bits_position = 0;
  g_stacks_tmp->bits_to_define = 0;
  
  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  g_stacks_tmp->position = STACK_POSITION_DEFINITION;

  g_stacks_tmp->stack[0].type = STACK_ITEM_TYPE_STRING;
  g_stacks_tmp->stack[0].sign = SI_SIGN_POSITIVE;
  strcpy(g_stacks_tmp->stack[0].string, label);

#if WLA_DEBUG
  _debug_print_stack(g_stacks_tmp->linenumber, g_stack_id, g_stacks_tmp->stack, 1, 1);
#endif
  
  _stack_insert();

  return SUCCEEDED;
}


int stack_create_stack_stack(int stack_id) {

  /* we need to create a stack that holds just one computation stack */
  g_stacks_tmp = calloc(sizeof(struct stack), 1);
  if (g_stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  g_stacks_tmp->next = NULL;
  g_stacks_tmp->type = STACK_TYPE_UNKNOWN;
  g_stacks_tmp->bank = -123456;
  g_stacks_tmp->stacksize = 1;
  g_stacks_tmp->relative_references = 0;
  g_stacks_tmp->stack = calloc(sizeof(struct stack_item), 1);
  if (g_stacks_tmp->stack == NULL) {
    free(g_stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  g_stacks_tmp->linenumber = g_active_file_info_last->line_current;
  g_stacks_tmp->filename_id = g_active_file_info_last->filename_id;
  g_stacks_tmp->special_id = 0;
  g_stacks_tmp->bits_position = 0;
  g_stacks_tmp->bits_to_define = 0;
  
  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  g_stacks_tmp->position = STACK_POSITION_DEFINITION;

  g_stacks_tmp->stack[0].type = STACK_ITEM_TYPE_STACK;
  g_stacks_tmp->stack[0].value = stack_id;
  g_stacks_tmp->stack[0].sign = SI_SIGN_POSITIVE;

#if WLA_DEBUG
  _debug_print_stack(g_stacks_tmp->linenumber, stack_id, g_stacks_tmp->stack, 1, 2);
#endif

  _stack_insert();
    
  return SUCCEEDED;
}


/* TODO: move the following code to its own file... */

#include "pass_3.h"

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")

struct data_stream_item *g_data_stream_items_first = NULL, *g_data_stream_items_last = NULL;
int g_is_data_stream_processed = NO;

extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;
extern char g_namespace[MAX_NAME_LENGTH + 1];
extern FILE *g_file_out_ptr;


int data_stream_parser_free(void) {

  while (g_data_stream_items_first != NULL) {
    struct data_stream_item *next = g_data_stream_items_first->next;
    free(g_data_stream_items_first);
    g_data_stream_items_first = next;
  }

  g_data_stream_items_first = NULL;
  g_data_stream_items_last = NULL;

  g_is_data_stream_processed = NO;

  return SUCCEEDED;
}


int data_stream_parser_parse(void) {

  int add = 0, add_old = 0, section_id = -1, bits_current = 0, inz, line_number = 0;
  struct data_stream_item *parent_labels[10];
  struct section_def *s;
  FILE *f_in;
  char c;
  
  memset(parent_labels, 0, sizeof(parent_labels));
  s = NULL;

  g_namespace[0] = 0;
  
  if (g_is_data_stream_processed == YES)
    return SUCCEEDED;

  if (g_file_out_ptr == NULL) {
    print_error("The internal data stream is closed! It should be open. Please submit a bug report!\n", ERROR_STC);
    return FAILED;
  }

  /* seek to the beginning of the file for parsing */
  fseek(g_file_out_ptr, 0, SEEK_SET);

  f_in = g_file_out_ptr;

  /* parse the internal data stream to find labels and their possibly non-final addresses */
  
  while (fread(&c, 1, 1, f_in) != 0) {
    switch (c) {

    case ' ':
    case 'E':
      continue;

    case 'j':
      continue;
    case 'J':
      continue;

    case 'i':
      fscanf(f_in, "%*s ");
      continue;
    case 'I':
      fscanf(f_in, "%*s ");
      continue;

    case 'P':
      add_old = add;
      continue;
    case 'p':
      add = add_old;
      continue;

    case 'A':
    case 'S':
      if (c == 'A')
        fscanf(f_in, "%d %*d", &section_id);
      else
        fscanf(f_in, "%d ", &section_id);

      add_old = add;

      s = g_sections_first;
      while (s != NULL && s->id != inz)
        s = s->next;
      
      continue;

    case 's':
      section_id = -1;
      s = NULL;
      continue;

    case 'x':
    case 'o':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz;
      continue;

    case 'X':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 2;
      continue;

    case 'h':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 3;
      continue;

    case 'w':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz * 4;
      continue;

    case 'z':
    case 'q':
      fscanf(f_in, "%*s ");
      add += 3;
      continue;

    case 'T':
      fscanf(f_in, "%*d ");
      add += 3;
      continue;

    case 'u':
    case 'V':
      fscanf(f_in, "%*s ");
      add += 4;
      continue;

    case 'U':
      fscanf(f_in, "%*d ");
      add += 4;
      continue;

    case 'v':
      fscanf(f_in, "%*d ");
      continue;
        
    case 'b':
      fscanf(f_in, "%*d ");
      continue;

    case 'R':
    case 'Q':
    case 'd':
    case 'c':
      fscanf(f_in, "%*s ");
      add++;
      continue;

    case 'M':
    case 'r':
      fscanf(f_in, "%*s ");
      add += 2;
      continue;

    case 'y':
    case 'C':
      fscanf(f_in, "%*d ");
      add += 2;
      continue;

#ifdef SUPERFX

    case '*':
      fscanf(f_in, "%*s ");
      add++;
      continue;
      
    case '-':
      fscanf(f_in, "%*d ");
      add++;
      continue;

#endif

    case '+':
      {
        int bits_to_add;
        char type;
          
        fscanf(f_in, "%d ", &bits_to_add);

        if (bits_to_add == 999) {
          bits_current = 0;

          continue;
        }
        else {
          if (bits_current == 0)
            add++;
          bits_current += bits_to_add;
          while (bits_current > 8) {
            bits_current -= 8;
            add++;
          }
          if (bits_to_add == 8)
            bits_current = 0;
        }

        fscanf(f_in, "%c", &type);

        if (type == 'a')
          fscanf(f_in, "%*d");
        else if (type == 'b')
          fscanf(f_in, "%*s");
        else if (type == 'c')
          fscanf(f_in, "%*d");

        continue;
      }

#ifdef SPC700
    case 'n':
      fscanf(f_in, "%*d %*s ");
      add += 2;
      continue;

    case 'N':
      fscanf(f_in, "%*d %*d ");
      add += 2;
      continue;
#endif

    case 'D':
      fscanf(f_in, "%*d %*d %*d %d ", &inz);
      add += inz;
      continue;

    case 'O':
      fscanf(f_in, "%d ", &add);
      continue;

    case 'B':
      fscanf(f_in, "%*d %*d ");
      continue;

    case 'g':
      fscanf(f_in, "%*d ");
      continue;

    case 'G':
      continue;

    case 't':
      fscanf(f_in, "%d ", &inz);
      if (inz == 0)
        g_namespace[0] = 0;
      else
        fscanf(f_in, STRING_READ_FORMAT, g_namespace);
      continue;

    case 'Z': /* breakpoint */
    case 'Y': /* symbol */
    case 'L': /* label */
      if (c == 'Z') {
      }
      else if (c == 'Y') {
        fscanf(f_in, "%*s ");
      }
      else {
        struct data_stream_item *dSI;
        int mangled_label = NO;

        dSI = calloc(sizeof(struct data_stream_item), 1);
        if (dSI == NULL) {
          print_error("Out of memory error while allocating a data_stream_item.\n", ERROR_STC);
          return FAILED;
        }

        fscanf(f_in, "%s ", dSI->label);

        if (is_label_anonymous(dSI->label) == NO) {
          /* if the label has '@' at the start, mangle the label name to make it unique */
          int n = 0, m;

          while (n < 10 && dSI->label[n] == '@')
            n++;
          m = n;
          while (m < 10)
            parent_labels[m++] = NULL;

          if (n < 10)
            parent_labels[n] = dSI;
          n--;
          while (n >= 0 && parent_labels[n] == 0)
            n--;

          if (n >= 0) {
            if (mangle_label(dSI->label, parent_labels[n]->label, n, MAX_NAME_LENGTH) == FAILED)
              return FAILED;
            mangled_label = YES;
          }
        }

        if (is_label_anonymous(dSI->label) == NO && g_namespace[0] != 0 && mangled_label == NO) {
          if (section_id < 0 || s->nspace == NULL) {
            if (add_namespace(dSI->label, g_namespace, sizeof(dSI->label)) == FAILED)
              return FAILED;
          }
        }
        
        dSI->next = NULL;
        dSI->address = add;
        dSI->section_id = section_id;

        if (g_data_stream_items_first == NULL) {
          g_data_stream_items_first = dSI;
          g_data_stream_items_last = dSI;
        }
        else {
          g_data_stream_items_last->next = dSI;
          g_data_stream_items_last = dSI;
        }
      }

      continue;

    case 'f':
      fscanf(f_in, "%*d ");
      continue;

    case 'k':
      fscanf(f_in, "%d ", &line_number);
      continue;

    case 'e':
      fscanf(f_in, "%*d %*d ");
      continue;

    default:
      fprintf(stderr, "data_stream_parser_parse(): Unknown internal symbol \"%c\". Please submit a bug report!\n", c);
      return FAILED;
    }
  }

  /* seek to the very end of the file so we can continue writing to it */
  fseek(g_file_out_ptr, 0, SEEK_END);
  
  g_is_data_stream_processed = YES;

  return SUCCEEDED;
}


struct data_stream_item *data_stream_parser_find_label(char *label) {

  struct data_stream_item *dSI;

  dSI = g_data_stream_items_first;
  while (dSI != NULL) {
    if (strcmp(label, dSI->label) == 0)
      return dSI;
    dSI = dSI->next;
  }

  return NULL;
}


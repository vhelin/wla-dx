
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


extern int input_number_error_msg, bankheader_status, input_float_mode;
extern int i, size, d, macro_active, string_size, section_status, parse_floats;
extern char xyz[512], *buffer, tmp[4096], expanded_macro_string[256], label[MAX_NAME_LENGTH + 1];
extern struct definition *tmp_def;
extern struct map_t *defines_map;
extern struct active_file_info *active_file_info_first, *active_file_info_last, *active_file_info_tmp;
extern struct macro_runtime *macro_runtime_current;
extern struct section_def *sec_tmp;
extern double parsed_double;

int latest_stack = 0, stacks_inside = 0, stacks_outside = 0, stack_id = 0;
struct stack *stacks_first = NULL, *stacks_tmp = NULL, *stacks_last = NULL;
struct stack *stacks_header_first = NULL, *stacks_header_last = NULL;

#ifndef GB
extern int stack_inserted;
#endif

extern int operand_hint, operand_hint_type;


static int _stack_insert(void) {

  /* outside bankheader sections */
  if (bankheader_status == OFF) {
    if (stacks_first == NULL) {
      stacks_first = stacks_tmp;
      stacks_last = stacks_tmp;
    }
    else {
      stacks_last->next = stacks_tmp;
      stacks_last = stacks_tmp;
    }
    stacks_outside++;

#ifndef GB
    stack_inserted = STACK_OUTSIDE;
#endif
  }
  /* inside a bankheader section */
  else {
    if (stacks_header_first == NULL) {
      stacks_header_first = stacks_tmp;
      stacks_header_last = stacks_tmp;
    }
    else {
      stacks_header_last->next = stacks_tmp;
      stacks_header_last = stacks_tmp;
    }
    stacks_inside++;

#ifndef GB
    stack_inserted = STACK_INSIDE;
#endif
  }

  stacks_tmp->id = stack_id;
  stacks_tmp->section_status = section_status;
  if (section_status == ON)
    stacks_tmp->section_id = sec_tmp->id;
  else
    stacks_tmp->section_id = 0;

  latest_stack = stack_id;
  stack_id++;

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
    char ar[] = "+-*()|&/^01%~<>";

    if (ta[k].type == STACK_ITEM_TYPE_OPERATOR) {
      int value = (int)ta[k].value;
      char arr = ar[value];

      /* 0 - shift left, 1 - shift right, otherwise it's the operator itself */
      if (arr == '0')
        printf("<<");
      else if (arr == '1')
        printf(">>");
      else
        printf("%c", arr);
    }
    else if (ta[k].type == STACK_ITEM_TYPE_VALUE)
      printf("V(%f)", ta[k].value);
    else if (ta[k].type == STACK_ITEM_TYPE_STACK)
      printf("C(%d)", (int)ta[k].value);
    else
      printf("S(%s)", ta[k].string);

    if (k < d-1)
      printf(", ");
  }
  printf("\n");
}
#endif


int get_label_length(char *l) {

  int length;
  
  hashmap_get(defines_map, l, (void*)&tmp_def);

  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_STRING)
      return (int)strlen(tmp_def->string);
    else {
      snprintf(xyz, sizeof(xyz), "Definition \"%s\" is not a string definition. .length returns 0 for that...\n", l);
      print_error(xyz, ERROR_NUM);
      return 0;
    }
  }

  length = (int)strlen(l);

  if (l[0] == '"' && l[length-1] == '"')
    length -= 2;

  return length;
}


int stack_calculate(char *in, int *value) {

  int q = 0, b = 0, d, k, op[256], n, o, l;
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
        si[q].value = SI_OP_MINUS;
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
        si[q].value = SI_OP_PLUS;
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
    else if (*in == '|') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_OR;
      q++;
      in++;
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
        if (input_number_error_msg == YES)
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
    else if (*in == '<') {
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
    else if (*in == '>' && *(in + 1) == '>') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_SHIFT_RIGHT;
      q++;
      in += 2;
    }
    else if (*in == '>') {
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
    else if (*in == '=' && *(in + 1) == '=')
      break;
    else if (*in == '!' && *(in + 1) == '=')
      break;
    else if (*in == '(') {
      si[q].type = STACK_ITEM_TYPE_OPERATOR;
      si[q].value = SI_OP_LEFT;
      q++;
      b++;
      in++;
    }
    else if (*in == '.' && (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L')) {
      in++;
      d = operand_hint;
      if (*in == 'b' || *in == 'B') {
        operand_hint = HINT_8BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else if (*in == 'w' || *in == 'W') {
        operand_hint = HINT_16BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else if (*in == 'l' || *in == 'L') {
        operand_hint = HINT_24BIT;
        operand_hint_type = HINT_TYPE_GIVEN;
        in++;
      }
      else
        break;

      if (d != HINT_NONE && d != operand_hint) {
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
    else if (*in == '%') {
      d = 0;
      for (k = 0; k < 31; k++, d = d<<1) {
        in++;
        e = *in;
        if (e == '0' || e == '1')
          d += e - '0';
        else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' || e == '*' ||
                 e == '/' || e == ',' || e == '^' || e == '<' || e == '>' || e == '#' || e == '~' ||
                 e == ']' || e == '.' || e == 0xA)
          break;
        else {
          if (input_number_error_msg == YES) {
            snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected a 0 or 1.\n", e, e);
            print_error(xyz, ERROR_NUM);
          }
          return FAILED;
        }
      }

      d = d>>1;

      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in == '\'') {
      in++;
      d = *in;
      in++;
      if (*in != '\'') {
        snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected \"'\".\n", *in, *in);
        print_error(xyz, ERROR_NUM);
        return FAILED;
      }
      in++;

      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in == '$') {
      /* we'll break if the previous item in the stack was a value or a string */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;
      
      d = 0;
      for (k = 0; k < 8; k++, d = d << 4) {
        in++;
        e = *in;
        if (e >= '0' && e <= '9')
          d += e - '0';
        else if (e >= 'A' && e <= 'F')
          d += e - 'A' + 10;
        else if (e >= 'a' && e <= 'f')
          d += e - 'a' + 10;
        else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' ||
                 e == '*' || e == '/' || e == ',' || e == '^' || e == '<' || e == '>' ||
                 e == '#' || e == '~' || e == ']' || e == '.' || e == 0xA)
          break;
        else {
          if (input_number_error_msg == YES) {
            snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected [0-F].\n", e, e);
            print_error(xyz, ERROR_NUM);
          }
          return FAILED;
        }
      }

      d = d >> 4;

      si[q].type = STACK_ITEM_TYPE_VALUE;
      si[q].value = d;
      si[q].sign = SI_SIGN_POSITIVE;
      q++;
    }
    else if (*in >= '0' && *in <= '9') {
      /* we'll break if the previous item in the stack was a value or a string */
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
        d = 0;
        for (k = 0; k < 8; k++, d = d << 4) {
          e = *(in++);
          if (e >= '0' && e <= '9')
            d += e - '0';
          else if (e >= 'A' && e <= 'F')
            d += e - 'A' + 10;
          else if (e >= 'a' && e <= 'f')
            d += e - 'a' + 10;
          else if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' ||
                   e == '*' || e == '/' || e == ',' || e == '^' || e == '<' || e == '>' ||
                   e == '#' || e == '~' || e == ']' || e == '.' || e == 'h' || e == 'H' || e == 0xA)
            break;
          else {
            if (input_number_error_msg == YES) {
              snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected [0-F].\n", e, e);
              print_error(xyz, ERROR_NUM);
            }
            return FAILED;
          }
        }

        d = d >> 4;

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
                snprintf(xyz, sizeof(xyz), "Too many digits in the floating point value. Max %d is supported.\n", MAX_FLOAT_DIGITS);
                print_error(xyz, ERROR_NUM);
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
                   e == ']' || e == 0xA)
            break;
          else if (e == '.') {
            if (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L')
              break;
            if (parse_floats == NO)
              break;
            if (n == 1) {
              if (input_number_error_msg == YES)
                print_error("Syntax error.\n", ERROR_NUM);
              return FAILED;
            }
            n = 1;
            max_digits = MAX_FLOAT_DIGITS+1;
          }
          else {
            if (input_number_error_msg == YES) {
              snprintf(xyz, sizeof(xyz), "Got '%c' (%d) when expected [0-9].\n", e, e);
              print_error(xyz, ERROR_NUM);
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
    else {
      /* it must be a string! */

      /* we'll break if the previous item in the stack was a value or a string */
      if (_break_before_value_or_string(q, &si[0]) == SUCCEEDED)
        break;

      si[q].sign = SI_SIGN_POSITIVE;
      for (k = 0; k < MAX_NAME_LENGTH; k++) {
        e = *in;
        if (e == ' ' || e == ')' || e == '|' || e == '&' || e == '+' || e == '-' || e == '*' ||
            e == '/' || e == ',' || e == '^' || e == '<' || e == '>' || e == '#' || e == ']' ||
            e == '~' || e == 0xA)
          break;
        if (e == '.' && (*(in+1) == 'b' || *(in+1) == 'B' || *(in+1) == 'w' || *(in+1) == 'W' || *(in+1) == 'l' || *(in+1) == 'L') &&
            (*(in+2) == ' ' || *(in+2) == ')' || *(in+2) == '|' || *(in+2) == '&' || *(in+2) == '+' || *(in+2) == '-' || *(in+2) == '*' ||
             *(in+2) == '/' || *(in+2) == ',' || *(in+2) == '^' || *(in+2) == '<' || *(in+2) == '>' || *(in+2) == '#' || *(in+2) == ']' ||
             *(in+2) == '~' || *(in+2) == 0xA))
          break;
        si[q].string[k] = e;
        in++;
      }
      si[q].string[k] = 0;

      si[q].type = STACK_ITEM_TYPE_STRING;
      q++;
    }

    if (q == 255) {
      print_error("Out of stack space.\n", ERROR_STC);
      return FAILED;
    }
  }

  if (b != 0) {
    print_error("Unbalanced parentheses.\n", ERROR_STC);
    return FAILED;
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
  if (q > 2 && si[q - 2].type == STACK_ITEM_TYPE_OPERATOR && si[q - 2].value == SI_OP_PLUS) {
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
  if (q > 2 && si[0].type == STACK_ITEM_TYPE_OPERATOR && si[0].value == SI_OP_PLUS &&
      si[1].type == STACK_ITEM_TYPE_OPERATOR && si[1].value == SI_OP_MINUS) {
    si[0].type = STACK_ITEM_TYPE_DELETED;
  }

  /* update the source pointer */
  i = (int)(in - buffer);

  /* fix the sign in every operand */
  for (b = 1, k = 0; k < q; k++) {
    if ((q - k) != 1 && si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].type == STACK_ITEM_TYPE_OPERATOR && si[k + 1].value != SI_OP_BANK
        && si[k + 1].value != SI_OP_HIGH_BYTE && si[k + 1].value != SI_OP_LOW_BYTE) {
      if (si[k].value != SI_OP_LEFT && si[k].value != SI_OP_RIGHT && si[k + 1].value != SI_OP_LEFT && si[k + 1].value != SI_OP_RIGHT) {
        print_error("Error in computation syntax.\n", ERROR_STC);
        return FAILED;
      }
    }
    if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_MINUS && b == 1) {
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
    if (si[k].type == STACK_ITEM_TYPE_OPERATOR && si[k].value == SI_OP_PLUS && b == 1) {
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
        if (si[k].value == SI_OP_PLUS ||
            si[k].value == SI_OP_MINUS) {
          b--;
          while (b != -1 && op[b] != SI_OP_LEFT) {
            ta[d].type = STACK_ITEM_TYPE_OPERATOR;
            ta[d].value = op[b];
            b--;
            d++;
          }
          b++;
          op[b] = (int)si[k].value;
          b++;
        }
        else if (si[k].value == SI_OP_LOW_BYTE ||
                 si[k].value == SI_OP_HIGH_BYTE ||
                 si[k].value == SI_OP_BANK) {
          /* unary operator, priority over everything else */
          op[b] = (int)si[k].value;
          b++;
        }
        else if (si[k].value == SI_OP_XOR ||
                 si[k].value == SI_OP_AND ||
                 si[k].value == SI_OP_OR ||
                 si[k].value == SI_OP_MULTIPLY ||
                 si[k].value == SI_OP_DIVIDE ||
                 si[k].value == SI_OP_MODULO ||
                 si[k].value == SI_OP_POWER ||
                 si[k].value == SI_OP_SHIFT_LEFT ||
                 si[k].value == SI_OP_SHIFT_RIGHT) {
          /* these operators have priority over + and - */
          b--;
          while (b != -1 && op[b] != SI_OP_LEFT && op[b] != SI_OP_PLUS && op[b] != SI_OP_MINUS) {
            ta[d].type = STACK_ITEM_TYPE_OPERATOR;
            ta[d].value = op[b];
            b--;
            d++;
          }
          b++;
          op[b] = (int)si[k].value;
          b++;
        }
        else if (si[k].value == SI_OP_NOT) {
          b--;
          while (b != -1 && op[b] != SI_OP_LEFT) {
            ta[d].type = STACK_ITEM_TYPE_OPERATOR;
            ta[d].value = op[b];
            b--;
            d++;
          }
          b++;
          op[b] = SI_OP_NOT;
          b++;
        }
        else if (si[k].value == SI_OP_LEFT) {
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
    s.linenumber = active_file_info_last->line_current;
    s.filename_id = active_file_info_last->filename_id;

    if (compute_stack(&s, d, &dou) == FAILED)
      return FAILED;
    
    parsed_double = dou;

    if (input_float_mode == ON)
      return INPUT_NUMBER_FLOAT;

    *value = (int)dou;

    return SUCCEEDED;
  }

  /* only one string? */
  if (d == 1 && ta[0].type == STACK_ITEM_TYPE_STRING) {
    strcpy(label, ta[0].string);
    return STACK_RETURN_LABEL;
  }

  /*
    printf("%d %d %s\n", d, ta[0].type, ta[0].string);
  */

  /* we have a stack full of computation and we save it for wlalink */
  stacks_tmp = calloc(sizeof(struct stack), 1);
  if (stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  stacks_tmp->next = NULL;
  stacks_tmp->type = STACK_TYPE_UNKNOWN;
  stacks_tmp->bank = -123456;
  stacks_tmp->stacksize = d;
  stacks_tmp->relative_references = 0;
  stacks_tmp->stack = calloc(sizeof(struct stack_item) * d, 1);
  if (stacks_tmp->stack == NULL) {
    free(stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  stacks_tmp->linenumber = active_file_info_last->line_current;
  stacks_tmp->filename_id = active_file_info_last->filename_id;
  stacks_tmp->special_id = 0;

  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  stacks_tmp->position = STACK_POSITION_DEFINITION;

  for (q = 0; q < d; q++) {
    if (ta[q].type == STACK_ITEM_TYPE_OPERATOR) {
      stacks_tmp->stack[q].type = STACK_ITEM_TYPE_OPERATOR;
      stacks_tmp->stack[q].value = ta[q].value;
    }
    else if (ta[q].type == STACK_ITEM_TYPE_VALUE) {
      stacks_tmp->stack[q].type = STACK_ITEM_TYPE_VALUE;
      stacks_tmp->stack[q].value = ta[q].value;
      stacks_tmp->stack[q].sign = ta[q].sign;
    }
    else if (ta[q].type == STACK_ITEM_TYPE_STACK) {
      stacks_tmp->stack[q].type = STACK_ITEM_TYPE_STACK;
      stacks_tmp->stack[q].value = ta[q].value;
      stacks_tmp->stack[q].sign = ta[q].sign;
    }
    else {
      stacks_tmp->stack[q].type = STACK_ITEM_TYPE_STRING;
      stacks_tmp->stack[q].sign = ta[q].sign;
      strcpy(stacks_tmp->stack[q].string, ta[q].string);
    }
  }

#if WLA_DEBUG
  _debug_print_stack(stacks_tmp->linenumber, stack_id, stacks_tmp->stack, d, 0);
#endif

  _stack_insert();

  return INPUT_NUMBER_STACK;
}


static int _resolve_string(struct stack_item *s, int *cannot_resolve) {

  if (macro_active != 0) {
    /* expand e.g., \1 and \@ */
    if (expand_macro_arguments(s->string) == FAILED)
      return FAILED;
  }

  hashmap_get(defines_map, s->string, (void*)&tmp_def);
  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_STRING) {
      snprintf(xyz, sizeof(xyz), "Definition \"%s\" is a string definition.\n", tmp_def->alias);
      print_error(xyz, ERROR_STC);
      return FAILED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_STACK) {
      /* turn this reference to a stack calculation define into a direct reference to the stack calculation as */
      /* this way we don't have to care if the define is exported or not as stack calculations are always exported */
      s->type = STACK_ITEM_TYPE_STACK;
      s->sign = SI_SIGN_POSITIVE;
      s->value = tmp_def->value;
    }
    else if (tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      /* wla cannot resolve address labels (unless outside a section) -> only wlalink can do that */
      *cannot_resolve = 1;
      strcpy(s->string, tmp_def->string);
    }
    else {
      s->type = STACK_ITEM_TYPE_VALUE;
      s->value = tmp_def->value;
    }
  }

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


int resolve_stack(struct stack_item s[], int x) {

  struct macro_argument *ma;
  struct stack_item *st;
  int a, b, k, q = x, cannot_resolve = 0, try_resolve_string = NO;
  char c;


  st = s;
  while (x > 0) {
    if (s->type == STACK_ITEM_TYPE_STRING) {
      if (macro_active != 0 && s->string[0] == '\\') {
        if (s->string[1] == '@' && s->string[2] == 0) {
          s->type = STACK_ITEM_TYPE_VALUE;
          s->value = macro_runtime_current->macro->calls - 1;
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
            if (_resolve_string(s, &cannot_resolve) == FAILED)
              return FAILED;
          }
          else {
            if (b > macro_runtime_current->supplied_arguments) {
              snprintf(xyz, sizeof(xyz), "Reference to MACRO argument number %d (\"%s\") is out of range.\n", b, s->string);
              print_error(xyz, ERROR_STC);
              return FAILED;
            }
          
            /* return the macro argument */
            ma = macro_runtime_current->argument_data[b - 1];
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
          
            if (!(k == SUCCEEDED || k == INPUT_NUMBER_ADDRESS_LABEL || k == INPUT_NUMBER_STACK))
              return FAILED;
          
            if (k == SUCCEEDED) {
              s->type = STACK_ITEM_TYPE_VALUE;
              s->value = parsed_double;
            }
            else if (k == INPUT_NUMBER_STACK) {
              s->type = STACK_ITEM_TYPE_STACK;
              s->value = latest_stack;
            }
            else
              strcpy(s->string, label);
          }
        }
      }
      else {
        if (_resolve_string(s, &cannot_resolve) == FAILED)
          return FAILED;
      }
    }
    s++;
    x--;
  }

  if (cannot_resolve != 0)
    return FAILED;

  /* find a string, a stack, bank, or a NOT and fail */
  while (q > 0) {
    if (st->type == STACK_ITEM_TYPE_STRING || st->type == STACK_ITEM_TYPE_STACK || (st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_NOT) || (st->type == STACK_ITEM_TYPE_OPERATOR && st->value == SI_OP_BANK))
      return FAILED;
    q--;
    st++;
  }

  return SUCCEEDED;
}


int compute_stack(struct stack *sta, int x, double *result) {

  struct stack_item *s;
  double v[256];
  int r, t, z;


  v[0] = 0.0;

  s = sta->stack;
  for (r = 0, t = 0; r < x; r++, s++) {
    if (s->type == STACK_ITEM_TYPE_VALUE) {
      if (s->sign == SI_SIGN_NEGATIVE)
        v[t] = -s->value;
      else
        v[t] = s->value;
      t++;
    }
    else {
      switch ((int)s->value) {
      case SI_OP_PLUS:
        v[t - 2] += v[t - 1];
        t--;
        break;
      case SI_OP_MINUS:
        v[t - 2] -= v[t - 1];
        t--;
        break;
      case SI_OP_MULTIPLY:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Multiply is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] *= v[t - 1];
        t--;
        break;
      case SI_OP_LOW_BYTE:
        z = (int)v[t - 1];
        v[t - 1] = z & 0xFF;
        break;
      case SI_OP_HIGH_BYTE:
        z = (int)v[t - 1];
        v[t - 1] = (z>>8) & 0xFF;
        break;
      case SI_OP_NOT:
        fprintf(stderr, "%s:%d: COMPUTE_STACK: NOT cannot determine the output size.\n", get_file_name(sta->filename_id), sta->linenumber);
        return FAILED;
        break;
      case SI_OP_XOR:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: XOR is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] ^ (int)v[t - 2];
        t--;
        break;
      case SI_OP_OR:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: OR is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] | (int)v[t - 2];
        t--;
        break;
      case SI_OP_AND:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: AND is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 1] & (int)v[t - 2];
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
        t--;
        break;
      case SI_OP_POWER:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Power is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = pow(v[t - 2], v[t - 1]);
        t--;
        break;
      case SI_OP_SHIFT_LEFT:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Shift left is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 2] << (int)v[t - 1];
        t--;
        break;
      case SI_OP_SHIFT_RIGHT:
        if (t <= 1) {
          fprintf(stderr, "%s:%d: COMPUTE_STACK: Shift right is missing an operand.\n", get_file_name(sta->filename_id), sta->linenumber);
          return FAILED;
        }
        v[t - 2] = (int)v[t - 2] >> (int)v[t - 1];
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
  stacks_tmp = calloc(sizeof(struct stack), 1);
  if (stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  stacks_tmp->next = NULL;
  stacks_tmp->type = STACK_TYPE_UNKNOWN;
  stacks_tmp->bank = -123456;
  stacks_tmp->stacksize = 1;
  stacks_tmp->relative_references = 0;
  stacks_tmp->stack = calloc(sizeof(struct stack_item), 1);
  if (stacks_tmp->stack == NULL) {
    free(stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  stacks_tmp->linenumber = active_file_info_last->line_current;
  stacks_tmp->filename_id = active_file_info_last->filename_id;
  stacks_tmp->special_id = 0;
  
  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  stacks_tmp->position = STACK_POSITION_DEFINITION;

  stacks_tmp->stack[0].type = STACK_ITEM_TYPE_STRING;
  stacks_tmp->stack[0].sign = SI_SIGN_POSITIVE;
  strcpy(stacks_tmp->stack[0].string, label);

#if WLA_DEBUG
  _debug_print_stack(stacks_tmp->linenumber, stack_id, stacks_tmp->stack, 1, 1);
#endif
  
  _stack_insert();

  return SUCCEEDED;
}


int stack_create_stack_stack(int stack_id) {

  /* we need to create a stack that holds just one computation stack */
  stacks_tmp = calloc(sizeof(struct stack), 1);
  if (stacks_tmp == NULL) {
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }
  stacks_tmp->next = NULL;
  stacks_tmp->type = STACK_TYPE_UNKNOWN;
  stacks_tmp->bank = -123456;
  stacks_tmp->stacksize = 1;
  stacks_tmp->relative_references = 0;
  stacks_tmp->stack = calloc(sizeof(struct stack_item), 1);
  if (stacks_tmp->stack == NULL) {
    free(stacks_tmp);
    print_error("Out of memory error while allocating room for a new stack.\n", ERROR_STC);
    return FAILED;
  }

  stacks_tmp->linenumber = active_file_info_last->line_current;
  stacks_tmp->filename_id = active_file_info_last->filename_id;
  stacks_tmp->special_id = 0;
  
  /* all stacks will be definition stacks by default. pass_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  stacks_tmp->position = STACK_POSITION_DEFINITION;

  stacks_tmp->stack[0].type = STACK_ITEM_TYPE_STACK;
  stacks_tmp->stack[0].value = stack_id;
  stacks_tmp->stack[0].sign = SI_SIGN_POSITIVE;

#if WLA_DEBUG
  _debug_print_stack(stacks_tmp->linenumber, stack_id, stacks_tmp->stack, 1, 2);
#endif

  _stack_insert();
    
  return SUCCEEDED;
}

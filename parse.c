
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "defines.h"

#include "parse.h"
#include "phase_1.h"
#include "stack.h"
#include "include.h"
#include "printf.h"
#include "mersenne.h"


int g_input_number_error_msg = YES, g_ss, g_string_size, g_input_float_mode = OFF, g_parse_floats = YES;
int g_expect_calculations = YES, g_input_allow_leading_hashtag = NO, g_input_has_leading_hashtag = NO, g_input_parse_special_chars = YES;
int g_input_allow_leading_ampersand = NO, g_plus_and_minus_ends_label = NO, g_get_next_token_use_substitution = YES;
int g_newline_beginning = ON, g_parsed_double_decimal_numbers = 0, g_operand_hint, g_operand_hint_type;
int g_input_number_turn_values_into_strings = NO, g_force_add_namespace = NO, g_force_ignore_namespace = NO;
char g_label[MAX_NAME_LENGTH + 1];
char g_expanded_macro_string[MAX_NAME_LENGTH + 1];
double g_parsed_double;

#if defined(SPC700)
int g_input_number_expects_dot = NO;
#endif

extern int g_source_index, g_source_file_size, g_parsed_int, g_macro_active, g_sizeof_g_tmp;
extern char *g_buffer, *g_tmp, g_current_directive[MAX_NAME_LENGTH + 1], *g_label_stack[256];
extern unsigned char g_asciitable[256];
extern struct active_file_info *g_active_file_info_first, *g_active_file_info_last, *g_active_file_info_tmp;
extern struct map_t *g_defines_map;
extern struct macro_runtime *g_macro_stack, *g_macro_runtime_current;
extern struct function *g_functions_first;
extern struct map_t *g_namespace_map;
extern struct namespace *g_namespaces_first;
extern int g_latest_stack, g_asciitable_defined, g_global_label_hint, g_parsing_function_body, g_resolve_stack_calculations;
extern int g_is_file_isolated_counter, g_can_remember_converted_stack_items;

int parse_string_length(char *end);

PROFILE_GLOBALS_EXTERN();


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
  ii = g_source_index;
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
          print_error(ERROR_NONE, "COMPARE_NEXT_SYMBOL: Macro \"%s\" wasn't called with enough arguments.\n", g_macro_runtime_current->macro->name);
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


int should_we_add_namespace(void) {

  if (g_force_ignore_namespace == YES)
    return NO;
  
  if (g_is_file_isolated_counter > 0 || g_force_add_namespace == YES)
    return YES;

  if (g_macro_active != 0) {
    struct macro_runtime *mrt = &g_macro_stack[g_macro_active - 1];

    if (mrt->macro->namespace[0] != 0)
      return YES;
  }

  return NO;
}


static char *s_arg_type_labels[] = {
  "ARG_IMMEDIATE",
  "ARG_NUMBER",
  "ARG_LABEL",
  "ARG_STRING",
  "ARG_PENDING_CALCULATION",
  NULL
};


int is_macro_arg_type_label(char *label) {

  int i = 0;

  /* quick exit */
  if (label[0] != 'A')
    return NO;
  
  while (s_arg_type_labels[i] != NULL) {
    if (strcaselesscmp(s_arg_type_labels[i], label) == 0)
      return YES;
    i++;
  }

  return NO;
}


static int _add_namespace_to_a_label(char *output, int sizeof_output, char *input, int is_sizeof) {

  int length = (int)strlen(input);

  if (is_sizeof == YES)
    length += 8;
  
  if (length >= sizeof_output - 1) {
    print_error(ERROR_NUM, "The label with the namespace is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
    return FAILED;
  }

  if (is_sizeof == YES)
    snprintf(output, sizeof_output, "_sizeof_%s", input);
  else
    strcpy(output, input);

  return SUCCEEDED;
}


int add_namespace_to_a_label(char *label, int sizeof_label, int add_outside_macros) {

  char namespace_tmp[MAX_NAME_LENGTH + 1], label_tmp[MAX_NAME_LENGTH + 1];
  struct definition *tmp_def;
  int i, is_sizeof = NO;

  if (g_force_ignore_namespace == YES)
    return SUCCEEDED;

  strcpy(label_tmp, label);
  
  /* don't add namespace to some specific labels */
  if (strcaselesscmp(label, "_out") == 0)
    return SUCCEEDED;
  if (label[0] == '\\' || label[0] == '@' || label[0] == '-' || label[0] == '+')
    return SUCCEEDED;
  if (label[0] == '_') {
    if (strncmp(label, "_sizeof_", 8) == 0) {
      is_sizeof = YES;
      strcpy(label_tmp, &label[8]);
    }
    else
      return SUCCEEDED;
  }

  if (g_macro_active != 0) {
    if (is_macro_arg_type_label(label) == YES)
      return SUCCEEDED;
  }
  
  /* does the label already contain a namespace? */
  i = 0;
  while (1) {
    namespace_tmp[i] = label_tmp[i];
    if (label_tmp[i] == '.') {
      if (strcaselesscmp(&label_tmp[i], ".length") != 0) {
        /* we have a dot in the name -> check if we know the namespace */
        struct namespace *namespace = g_namespaces_first;
        
        namespace_tmp[i] = 0;

        while (namespace != NULL) {
          if (strcmp(namespace_tmp, namespace->name) == 0) {
            /* we know the namespace -> don't add it! */
            return SUCCEEDED;
          }
          namespace = namespace->next;
        }

        /* it is not probably a namespace so proceed adding the namespace to the label */
        break;
      }
    }
    if (label_tmp[i] == 0)
      break;
    i++;
  }

  namespace_tmp[0] = 0;
  
  /* label reference inside a namespaced .MACRO? */
  if (g_macro_active != 0) {
    struct macro_runtime *mrt = &g_macro_stack[g_macro_active - 1];

    if (mrt->macro->namespace[0] != 0) {
      /* yes! add the namespace! */
      if (strlen(mrt->macro->namespace) + strlen(label_tmp) >= MAX_NAME_LENGTH) {
        print_error(ERROR_NUM, "The label with the namespace is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
        return FAILED;
      }

      snprintf(namespace_tmp, sizeof(namespace_tmp), "%s.%s", mrt->macro->namespace, label_tmp);
    }
  }
  else if (add_outside_macros == YES) {
    strcpy(namespace_tmp, label_tmp);
    add_namespace_to_string(namespace_tmp, sizeof(namespace_tmp), "add_namespace_to_a_label()");
  }

  if (namespace_tmp[0] == 0)
    return SUCCEEDED;

  if (g_force_add_namespace == YES)
    return _add_namespace_to_a_label(label, sizeof_label, namespace_tmp, is_sizeof);

  /* do we find the label with the namespace in defines? */
  if (is_sizeof == YES) {
    snprintf(label_tmp, sizeof(label_tmp), "_sizeof_%s", namespace_tmp);
    hashmap_get(g_defines_map, label_tmp, (void*)&tmp_def);
  }
  else
    hashmap_get(g_defines_map, namespace_tmp, (void*)&tmp_def);
  if (tmp_def != NULL) {
    /* yes! */
    return _add_namespace_to_a_label(label, sizeof_label, namespace_tmp, is_sizeof);
  }

  /* do we find the label without the namespace in defines? */
  hashmap_get(g_defines_map, label, (void*)&tmp_def);
  if (tmp_def != NULL) {
    /* yes! */
    return SUCCEEDED;
  }
  
  return _add_namespace_to_a_label(label, sizeof_label, namespace_tmp, is_sizeof);
}
 

int input_next_string(void) {

  int k, curly_braces = 0;
  char e;
  
  /* skip white space */
  for (e = g_buffer[g_source_index++]; e == ' ' || e == ','; e = g_buffer[g_source_index++])
    ;

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  /* we assume it is a label */
  g_label[0] = e;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = g_buffer[g_source_index++];
    if (e == '{')
      curly_braces++;
    else if (e == '}')
      curly_braces++;
    else if (e == 0x0A || e == ',') {
      g_source_index--;
      break;
    }
    else if (e == '(' || e == ')') {
      g_source_index--;
      break;
    }
    else if (curly_braces <= 0 && e == ' ')
      break;
    g_label[k] = e;
  }

  if (k == MAX_NAME_LENGTH) {
    if (g_input_number_error_msg == YES) {
      print_error(ERROR_NUM, "The string is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
    }
    return FAILED;
  }

  g_label[k] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_label, NULL, NULL) == FAILED)
      return FAILED;
  }

  if (expand_variables_inside_string(g_label, sizeof(g_label), &k) == FAILED)
    return FAILED;

  if (should_we_add_namespace() == YES) {
    if (add_namespace_to_a_label(g_label, sizeof(g_label), YES) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


static int _input_number_return_definition(struct definition *def) {

  if (def->type == DEFINITION_TYPE_VALUE) {
    g_parsed_int = (int)def->value;
    g_parsed_double = def->value;

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

    if (g_input_float_mode == ON && g_parsed_double - (int)g_parsed_double != 0.0)
      return INPUT_NUMBER_FLOAT;

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
          print_error(ERROR_NUM, "The label is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
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


int expand_variables_inside_string(char *label, int max_size, int *length) {

  char tmp[MAX_NAME_LENGTH + 1], formatting[8], substitution[MAX_NAME_LENGTH + 1], local[MAX_NAME_LENGTH + 1];
  int size, i, k, substitutions = 0, max_size_tmp, p, parsed_int, input_size = 0;
  int q, size_substitution;

  /* quick exit if no curly braces are found */
  i = 0;
  k = NO;
  while (label[i] != 0) {
    if (label[i] == '{')
      k = YES;
    i++;
    input_size++;
  }

  if (k == NO)
    return SUCCEEDED;

  input_size++;
  
  /* copy label -> local as label might be g_label that stack calculator uses */
  if (input_size > (int)sizeof(local)) {
    print_error(ERROR_NUM, "Buffer for substitution is too small! Please submit a bug report!\n");
    return FAILED;
  }

  strcpy(local, label);
  
  max_size_tmp = sizeof(tmp);
  
  if (length == NULL)
    size = input_size;
  else
    size = *length;

  for (i = 0, k = 0; i < size && k < max_size_tmp; i++) {
    if (local[i] == '{') {
      int use_formatting = NO;
      
      i++;
      
      /* skip whitespace */
      while (i < size && local[i] == ' ')
        i++;

      /* do we have formatting? */
      if (i+5 < size && local[i] == '%' && local[i+1] == '.') {
        /* yes! */
        int f = 0, n;
        char c;

        use_formatting = YES;
        formatting[f++] = '%';
        i++;

        if (local[i] != '.') {
          print_error(ERROR_NUM, "The formatting string must begin with \"%%.\".\n");
          return FAILED;
        }

        formatting[f++] = '.';
        i++;

        for (n = 0; n < 3; n++, i++) {
          c = local[i];
          if (c == '{')
            break;
          if (!((c >= '0' && c <= '9') || c == 'x' || c == 'X' || c == 'd' || c == 'i')) {
            print_error(ERROR_NUM, "Unsupported formatting symbol '%c'.\n", c);
            return FAILED;
          }

          formatting[f++] = c;
        }

        if (local[i] != '{') {
          print_error(ERROR_NUM, "Error in formatting. Formatting string is too long?\n");
          return FAILED;
        }

        formatting[f] = 0;
        i++;
      }

      /* launch stack calculator */
      p = stack_calculate(&local[i], &parsed_int, &i, YES);
      i++;

      if (p == STACK_CALCULATE_DELAY || p == INPUT_NUMBER_STACK) {
        print_error(ERROR_NUM, "Postponed calculation is not suitable for substitution as we need immediate results.\n");
        return FAILED;
      }

      if (p == SUCCEEDED || p == INPUT_NUMBER_FLOAT) {
        if (p == INPUT_NUMBER_FLOAT)
          parsed_int = (int)g_parsed_double;
        
        if (use_formatting == YES)
          snprintf(substitution, sizeof(substitution), formatting, parsed_int);
        else
          snprintf(substitution, sizeof(substitution), "%d", parsed_int);
      }
      else if (p == STACK_RETURN_STRING) {
        if (use_formatting == YES) {
          print_error(ERROR_NUM, "Cannot use formatting with strings.\n");
          return FAILED;
        }
        strcpy(substitution, g_label);
      }
      else if (p == STACK_RETURN_LABEL) {
        print_error(ERROR_NUM, "Label \"%s\" cannot be used in a substitution here as its value is unknown.\n", g_label);
        return FAILED;
      }
      else if (p == FAILED)
        return FAILED;
      else {
        print_error(ERROR_NUM, "Unhandled return type %d from stack_calculate()! Please submit a bug report!\n", p);
        return FAILED;
      }

      /* perform substitution */
      size_substitution = (int)strlen(substitution);
      for (q = 0; k < max_size_tmp && q < size_substitution; q++)
        tmp[k++] = substitution[q];
          
      substitutions++;

      if (use_formatting == YES) {
        /* skip whitespace */
        while (i+1 < size && local[i+1] == ' ')
          i++;
        if (i+1 < size && local[i+1] == '}')
          i++;
        else {
          print_error(ERROR_NUM, "The end of the substitution is missing a '}'. Got '%c' instead.\n", local[i+1]);
          return FAILED;
        }
      }

      continue;
    }

    tmp[k++] = local[i];
  }

  if (substitutions > 0) {
    /* terminate the string with substitutions */
    if (k < max_size_tmp)
      tmp[k] = 0;
    else {
      print_error(ERROR_NUM, "Cannot perform substitutions for string \"%s\", buffer is too small.\n", local);
      return FAILED;
    }

    /* copy back the string with substitutions */
    if (k < max_size) {
      strcpy(label, tmp);
      if (length != NULL)
        *length = k;
    }
    else {
      print_error(ERROR_NUM, "Cannot perform substitutions for string \"%s\", buffer is too small.\n", local);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


static int _parse_value_into_string(char e) {

  int k;
        
  /* override! */
  g_label[0] = e;

  for (k = 1; k < MAX_NAME_LENGTH; k++, g_source_index++) {
    e = g_buffer[g_source_index];
    g_label[k] = e;
    if (e >= '0' && e <= '9')
      continue;
    else if (e >= 'a' && e <= 'z')
      continue;
    else if (e >= 'A' && e <= 'Z')
      continue;
    break;
  }

  g_label[k] = 0;
  g_string_size = k;
        
  return INPUT_NUMBER_STRING;
}


#if defined(PROFILE_FUNCTIONS)
int _input_number(void) {
#else
int input_number(void) {
#endif

  char label_tmp[MAX_NAME_LENGTH + 1];
  unsigned char e, ee, check_if_a_definition = YES, can_have_calculations = YES, use_substitution = NO;
  int k, p, q, spaces = 0, curly_braces = 0;
  double decimal_mul;
#if defined(SPC700)
  int dot = 0;
#endif


  g_operand_hint = HINT_NONE;
  g_operand_hint_type = HINT_TYPE_NONE;
  g_input_has_leading_hashtag = NO;
      
  /* skip white space */
  for (e = g_buffer[g_source_index++]; e == ' ' || e == ',' || e == '\\'; e = g_buffer[g_source_index++]) {
    if (e == '\\' && g_buffer[g_source_index] == 0xA) {
      g_source_index++;
      next_line();
    }
    else if (e == '\\' && g_buffer[g_source_index] != 0xA)
      break;
  }

  if (e == 0x0A)
    return INPUT_NUMBER_EOL;

  /* using substitution with quoted strings? */
  if (e == '{') {
    int old_position = g_source_index;
    unsigned char old_e = e;
    
    /* skip white space */
    while (g_buffer[g_source_index] == ' ')
      g_source_index++;

    e = g_buffer[g_source_index++];

    if (e == '"') {
      /* later when we parse a quoted string we'll check that it's followed by a '}', and
         we'll use substitution in that quoted string... */
      use_substitution = YES;
    }
    else {
      /* roll back */
      e = old_e;
      g_source_index = old_position;
    }
  }
  
  /* are we parsing a macro argument, and could it begin with a '#' (ARG_IMMEDIATE)? */
  if (g_input_allow_leading_hashtag == YES) {
    if (e == '#') {
      g_input_has_leading_hashtag = YES;
      e = g_buffer[g_source_index++];
    }
  }

  /* are we parsing a macro argument, and could it begin with a '&' (return
     label as it is, don't check if it's a definition) */
  if (g_input_allow_leading_ampersand == YES) {
    if (e == '&') {
      check_if_a_definition = NO;
      can_have_calculations = NO;
      e = g_buffer[g_source_index++];
    }
  }

  if (g_expect_calculations == YES && can_have_calculations == YES) {
    /* check the type of the expression */
    p = g_source_index;
    ee = e;
    while (ee != 0x0A) {
      if (ee == '{')
        curly_braces++;
      else if (ee == '}')
        curly_braces--;
      /* string / symbol -> no calculating */
      else if (ee == ',')
        break;
      else if (ee == ' ')
        spaces++;
      else if (curly_braces <= 0 && (ee == '-' || ee == '+' || ee == '*' || ee == '/' || ee == '&' || ee == '|' || ee == '^' || ee == '(' ||
                                     ee == '<' || ee == '>' || ee == '#' || ee == '~' || ee == ':' || ee == '!' || (ee == '=' && g_buffer[p] == '='))) {
        if (ee == ':' && spaces > 0)
          break;
        
        /* launch stack calculator */
        p = stack_calculate(&g_buffer[g_source_index - 1], &g_parsed_int, &g_source_index, NO);
        
        if (p == STACK_CALCULATE_DELAY)
          break;
        else if (p == STACK_RETURN_LABEL)
          return INPUT_NUMBER_ADDRESS_LABEL;
        else if (p == STACK_RETURN_STRING)
          return INPUT_NUMBER_STRING;
        else
          return p;
      }
      ee = g_buffer[p];
      p++;
    }
  }

  /* MACRO */
  if (g_macro_active != 0 && e == '?' && g_buffer[g_source_index] >= '1' && g_buffer[g_source_index] <= '9') {
    struct macro_argument *ma;
    
    for (g_parsed_int = 0, k = 0; k < 4; k++) {
      e = g_buffer[g_source_index++];
      if (e >= '0' && e <= '9')
        g_parsed_int = (g_parsed_int * 10) + (e - '0');
      else {
        g_source_index--;
        break;
      }
    }

    if (g_parsed_int > g_macro_runtime_current->supplied_arguments) {
      print_error(ERROR_NUM, "Referencing argument number %d inside macro \"%s\". The macro has only %d arguments.\n", g_parsed_int, g_macro_runtime_current->macro->name, g_macro_runtime_current->supplied_arguments);
      return FAILED;
    }
    if (g_parsed_int == 0) {
      print_error(ERROR_NUM, "Referencing argument number %d inside macro \"%s\". Macro arguments are counted from 1.\n", g_parsed_int, g_macro_runtime_current->macro->name);
      return FAILED;
    }

    /* use the macro argument to find its definition */
    ma = g_macro_runtime_current->argument_data[g_parsed_int - 1];
    k = ma->type;

    if (k == INPUT_NUMBER_ADDRESS_LABEL) {
      struct definition *tmp_def;

      strcpy(g_label, ma->string);

      hashmap_get(g_defines_map, g_label, (void*)&tmp_def);
      if (tmp_def != NULL)
        return _input_number_return_definition(tmp_def);
      else {
        print_error(ERROR_NUM, "Cannot find definition for \"%s\".\n", g_label);
        return FAILED;
      }
    }
    else {
      print_error(ERROR_ERR, "? can be only used to evaluate definitions.\n");
      return FAILED;
    }
  }
  if (g_macro_active != 0 && e == '\\') {
    struct macro_argument *ma;
    int exit_here = YES;
    int start_i = g_source_index;
    unsigned char start_e = e;

    if (g_buffer[g_source_index] == '@') {
      g_source_index++;
      g_parsed_int = g_macro_runtime_current->macro->calls - 1;

      if (g_buffer[g_source_index] != ' ' && g_buffer[g_source_index] != 0xA && g_buffer[g_source_index] != ',')
        exit_here = NO;
      else
        return SUCCEEDED;
    }
    else if (g_buffer[g_source_index] >= '0' && g_buffer[g_source_index] <= '9') {
      for (g_parsed_int = 0, k = 0; k < 4; k++) {
        e = g_buffer[g_source_index++];
        if (e >= '0' && e <= '9')
          g_parsed_int = (g_parsed_int * 10) + (e - '0');
        else {
          g_source_index--;
          break;
        }
      }

      if (g_buffer[g_source_index] != ' ' && g_buffer[g_source_index] != 0xA && g_buffer[g_source_index] != ',' && g_buffer[g_source_index] != '.')
        exit_here = NO;
    }
    else
      exit_here = NO;

    if (exit_here == YES) {
      if (g_parsed_int > g_macro_runtime_current->supplied_arguments) {
        print_error(ERROR_NUM, "Referencing argument number %d inside macro \"%s\". The macro has only %d arguments.\n", g_parsed_int, g_macro_runtime_current->macro->name, g_macro_runtime_current->supplied_arguments);
        return FAILED;
      }
      if (g_parsed_int == 0) {
        print_error(ERROR_NUM, "Referencing argument number %d inside macro \"%s\". Macro arguments are counted from 1.\n", g_parsed_int, g_macro_runtime_current->macro->name);
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
      else if (k == INPUT_NUMBER_FLOAT) {
        g_parsed_int = (int)ma->value;
        g_parsed_double = ma->value;
      }
      else {
        print_error(ERROR_ERR, "Macro argument list has been corrupted! Please send a bug report!\n");
        return FAILED;
      }

      /* does the MACRO argument number end with a .b/.w/.l/.d? */
      if (e == '.') {
        e = g_buffer[g_source_index+1];
        if (e == 'b' || e == 'B') {
          g_operand_hint = HINT_8BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
        }
        else if (e == 'w' || e == 'W') {
          g_operand_hint = HINT_16BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
        }
        else if (e == 'l' || e == 'L') {
          g_operand_hint = HINT_24BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
        }
        else if (e == 'd' || e == 'D') {
          g_operand_hint = HINT_32BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
        }
      }

      if (k == INPUT_NUMBER_FLOAT) {
        if (g_input_float_mode == ON)
          return INPUT_NUMBER_FLOAT;
        else
          return SUCCEEDED;
      }
      else
        return k;
    }
    else {
      g_source_index = start_i;
      e = start_e;
    }
  }

  /* is it a hexadecimal value? */
  g_parsed_int = 0;
  if (e >= '0' && e <= '9') {
    for (k = 0; 1; k++) {
      if (g_buffer[g_source_index+k] >= '0' && g_buffer[g_source_index+k] <= '9')
        continue;
      if (g_buffer[g_source_index+k] >= 'a' && g_buffer[g_source_index+k] <= 'f')
        continue;
      if (g_buffer[g_source_index+k] >= 'A' && g_buffer[g_source_index+k] <= 'F')
        continue;
      if (g_buffer[g_source_index+k] == 'h' || g_buffer[g_source_index+k] == 'H') {
        g_parsed_int = 1;
        break;
      }
      break;
    }
  }

  if (e == '$' || g_parsed_int == 1 || (e == '0' && (g_buffer[g_source_index] == 'x' || g_buffer[g_source_index] == 'X'))) {
    if (g_parsed_int == 1)
      g_source_index--;
    else if (e == '0')
      g_source_index++;
    for (g_parsed_int = 0, k = 0; k < 8; k++, g_source_index++) {
      e = g_buffer[g_source_index];
      if (e >= '0' && e <= '9')
        g_parsed_int = (g_parsed_int << 4) + e - '0';
      else if (e >= 'A' && e <= 'F')
        g_parsed_int = (g_parsed_int << 4) + e - 'A' + 10;
      else if (e >= 'a' && e <= 'f')
        g_parsed_int = (g_parsed_int << 4) + e - 'a' + 10;
      else if (e == 'h' || e == 'H') {
        g_source_index++;
        break;
      }
      else
        break;
    }

    e = g_buffer[g_source_index];

    if (e == '.') {
      e = g_buffer[g_source_index+1];
      if (e == 'b' || e == 'B') {
        g_operand_hint = HINT_8BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'w' || e == 'W') {
        g_operand_hint = HINT_16BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'l' || e == 'L') {
        g_operand_hint = HINT_24BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'd' || e == 'D') {
        g_operand_hint = HINT_32BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
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

  if (e == '%' || (e == '0' && (g_buffer[g_source_index] == 'b' || g_buffer[g_source_index] == 'B'))) {
    if (e == '0' && g_input_number_turn_values_into_strings == YES)
      return _parse_value_into_string(e);
        
    if (e == '0')
      g_source_index++;
    for (g_parsed_int = 0, k = 0; k < 32; k++, g_source_index++) {
      e = g_buffer[g_source_index];
      if (e == '0' || e == '1')
        g_parsed_int = (g_parsed_int << 1) + e - '0';
      else
        break;
    }

    if (k == 32) {
      if (g_buffer[g_source_index] == '0' || g_buffer[g_source_index] == '1') {
        print_error(ERROR_NUM, "Too many bits in a binary value, max is 32.\n");
        return FAILED;
      }
    }

    e = g_buffer[g_source_index];
    
    if (e == '.') {
      e = g_buffer[g_source_index+1];
      if (e == 'b' || e == 'B') {
        g_operand_hint = HINT_8BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'w' || e == 'W') {
        g_operand_hint = HINT_16BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'l' || e == 'L') {
        g_operand_hint = HINT_24BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
      else if (e == 'd' || e == 'D') {
        g_operand_hint = HINT_32BIT;
        g_operand_hint_type = HINT_TYPE_GIVEN;
        g_source_index += 2;
      }
    }

    g_parsed_double = (double)g_parsed_int;

    return SUCCEEDED;
  }
  
  if (e >= '0' && e <= '9') {
    int max_digits = 10;

    if (g_input_number_turn_values_into_strings == YES)
      return _parse_value_into_string(e);
    
    /* we are parsing decimals when q == 1 */
    q = 0;
    g_parsed_double = e-'0';
    g_parsed_double_decimal_numbers = 0;
    decimal_mul = 0.1;
    for (k = 0; k < max_digits; k++, g_source_index++) {
      e = g_buffer[g_source_index];
      if (e >= '0' && e <= '9') {
        if (k == max_digits - 1) {
          if (q == 0)
            print_error(ERROR_NUM, "Too many digits in the integer value. Max 10 is supported.\n");
          else {
            print_error(ERROR_NUM, "Too many digits in the floating point value. Max %d is supported.\n", MAX_FLOAT_DIGITS);
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
          print_error(ERROR_NUM, "Syntax error.\n");
          return FAILED;
        }
        e = g_buffer[g_source_index+1];
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
          g_source_index += 2;
          break;
        }
        else if (e == 'w' || e == 'W') {
          g_operand_hint = HINT_16BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
          break;
        }
        else if (e == 'l' || e == 'L') {
          g_operand_hint = HINT_24BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
          break;
        }
        else if (e == 'd' || e == 'D') {
          g_operand_hint = HINT_32BIT;
          g_operand_hint_type = HINT_TYPE_GIVEN;
          g_source_index += 2;
          break;
        }
      }
      else if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z')) {
        /* a number directly followed by a letter when parsing a integer/float -> syntax error */
        print_error(ERROR_NUM, "Syntax error.\n");
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
    if (g_buffer[g_source_index + 1] == '\'') {
      /* '?' */
      g_parsed_int = g_buffer[g_source_index];
      g_source_index += 2;
    }
    else if (g_buffer[g_source_index] == '\\' && (g_buffer[g_source_index+1] == 't' ||
                                                    g_buffer[g_source_index+1] == 'r' ||
                                                    g_buffer[g_source_index+1] == 'n' ||
                                                    g_buffer[g_source_index+1] == '0') && g_buffer[g_source_index+2] == '\'') {
      /* '\?' */
      g_source_index++;
      if (g_buffer[g_source_index] == 't')
        g_parsed_int = '\t';
      else if (g_buffer[g_source_index] == 'r')
        g_parsed_int = '\r';
      else if (g_buffer[g_source_index] == 'n')
        g_parsed_int = '\n';
      else if (g_buffer[g_source_index] == '0')
        g_parsed_int = '\0';
      g_source_index += 2;
    }
    else {
      if (g_input_number_error_msg == YES)
        print_error(ERROR_NUM, "Malformed '?' detected!\n");
      return FAILED;
    }

    g_parsed_double = (double)g_parsed_int;
    
    return SUCCEEDED;
  }

  if (e == '"') {
    for (k = 0; k < MAX_NAME_LENGTH; ) {
      e = g_buffer[g_source_index++];

      if (e == '\\' && g_buffer[g_source_index] == '\\') {
        /* let process_string_for_special_characters() handle '\\' */
        g_label[k++] = '\\';
        g_label[k++] = '\\';
        g_source_index++;
        continue;
      }
      else if (e == '\\' && g_buffer[g_source_index] == '"') {
        g_label[k++] = '"';
        g_source_index++;
        continue;
      }

      if (e == '"') {
        int is_string_split = -1;
        
        /* check for "string".length */
        if (g_buffer[g_source_index+0] == '.' &&
            (g_buffer[g_source_index+1] == 'l' || g_buffer[g_source_index+1] == 'L') &&
            (g_buffer[g_source_index+2] == 'e' || g_buffer[g_source_index+2] == 'E') &&
            (g_buffer[g_source_index+3] == 'n' || g_buffer[g_source_index+3] == 'N') &&
            (g_buffer[g_source_index+4] == 'g' || g_buffer[g_source_index+4] == 'G') &&
            (g_buffer[g_source_index+5] == 't' || g_buffer[g_source_index+5] == 'T') &&
            (g_buffer[g_source_index+6] == 'h' || g_buffer[g_source_index+6] == 'H')) {
          /* yes, we've got it! calculate the length and return the integer */
          g_source_index += 7;
          g_label[k] = 0;
          g_parsed_int = (int)get_label_length(g_label);
          g_parsed_double = (double)g_parsed_int;

          return SUCCEEDED;
        }

        /* does the string continue on the next line? */
        if (g_buffer[g_source_index] == ' ' && g_buffer[g_source_index+1] == '\\' && g_buffer[g_source_index+2] == 0x0A)
          is_string_split = 3;
        if (g_buffer[g_source_index] == '\\' && g_buffer[g_source_index+1] == 0x0A)
          is_string_split = 2;

        if (is_string_split > 0) {
          int skip = is_string_split;

          while (g_buffer[g_source_index+skip] == ' ')
            skip++;

          if (g_buffer[g_source_index+skip] == '"') {
            g_source_index += skip + 1;
            e = g_buffer[g_source_index++];

            /* as we skipped a 0x0A before we need to advance the line counter as well */
            next_line();
          }
        }

        if (e == '"')
          break;
      }
      
      if (e == 0 || e == 0x0A) {
        print_error(ERROR_NUM, "String wasn't terminated properly.\n");
        return FAILED;
      }

      g_label[k++] = e;
    }

    if (k >= MAX_NAME_LENGTH)
      print_error(ERROR_WRN, "String parsing was interrupted due to buffer getting full.\n");
    
    g_label[k] = 0;

    /* expand e.g., \1 and \@ */
    if (g_macro_active != 0) {
      if (expand_macro_arguments(g_label, NULL, NULL) == FAILED)
        return FAILED;
      k = (int)strlen(g_label);
    }

    if (g_input_parse_special_chars == YES && process_string_for_special_characters(g_label, &k) == FAILED)
      return FAILED;

    if (use_substitution) {
      if (expand_variables_inside_string(g_label, sizeof(g_label), &k) == FAILED)
        return FAILED;

      /* make sure we are followed by a '}' */

      /* skip white space */
      while (g_buffer[g_source_index] == ' ')
        g_source_index++;

      if (g_buffer[g_source_index] != '}') {
        print_error(ERROR_NUM, "The string used in substitution isn't followed by a '}'.\n");
        return FAILED;
      }

      g_source_index++;
    }
    
    if (k >= MAX_NAME_LENGTH) {
      if (g_input_number_error_msg == YES) {
        print_error(ERROR_NUM, "The string is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
      }
      return FAILED;
    }

    g_label[k] = 0;
    g_string_size = k;

    return INPUT_NUMBER_STRING;
  }

  /* the last choice is a label */

  /* sanity checks for the label */
  if (e == '(' || e == ')' || e == ',' || e == ']') {
    if (g_input_number_error_msg == YES)
      print_error(ERROR_NUM, "Unexpected '%c'!\n", e);
    return FAILED;
  }

  g_label[0] = e;
  curly_braces = 0;
  for (k = 1; k < MAX_NAME_LENGTH; k++) {
    e = g_buffer[g_source_index++];
    if (e == '{')
      curly_braces++;
    else if (e == '}')
      curly_braces--;
    else if (e == 0x0A || e == '(' || e == ')' || e == ',' || e == ']') {
      g_source_index--;
      break;
    }
    else if (g_plus_and_minus_ends_label == YES && (e == '-' || e == '+')) {
      g_source_index--;
      break;
    }
#if defined(SPC700)
    else if (e == '.' && g_input_number_expects_dot == YES)
      dot = k;
#endif
    else if (e == ' ' && curly_braces <= 0)
      break;
    g_label[k] = e;
  }

  if (k == MAX_NAME_LENGTH) {
    if (g_input_number_error_msg == YES) {
      print_error(ERROR_NUM, "The label is too long (max %d characters allowed). Please adjust MAX_NAME_LENGTH in shared.h and recompile WLA.\n", MAX_NAME_LENGTH);
    }
    return FAILED;
  }

  /* size hint? */
  if (k > 2 && g_label[k-2] == '.') {
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
#if defined(SPC700)
    else if (g_label[k-1] >= '0' && g_label[k-1] <= '7') {
      k -= 2;
      g_source_index -= 2;
    }
#endif
  }
#if defined(SPC700)
  else if (dot > 0) {
    g_source_index -= k - dot;
    k -= k - dot;
  }
#endif
  
  g_label[k] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    double ret_value;
    int ret_type;
    
    if (expand_macro_arguments(g_label, &ret_type, &ret_value) == FAILED)
      return FAILED;

    /* NOTE! the string could have turned into e.g., a stack calculation! */
    if (ret_type == SUCCEEDED) {
      g_parsed_int = (int)ret_value;
      g_parsed_double = ret_value;

      return SUCCEEDED;
    }
    else if (ret_type == INPUT_NUMBER_FLOAT) {
      g_parsed_int = (int)ret_value;
      g_parsed_double = ret_value;

      if (g_input_float_mode == ON)
        return INPUT_NUMBER_FLOAT;
      else
        return SUCCEEDED;
    }
    else if (ret_type == INPUT_NUMBER_STACK) {
      g_latest_stack = (int)ret_value;

      return INPUT_NUMBER_STACK;
    }
    
    k = (int)strlen(g_label);
  }

  if (expand_variables_inside_string(g_label, sizeof(g_label), &k) == FAILED)
    return FAILED;

  if (g_macro_active > 0 && g_label[0] == '?') {
    /* CHILDLABELS .MACRO and local reference! */
    if (process_label_inside_macro(NO, g_label, sizeof(g_label)) == FAILED)
        return FAILED;
  }
  
  if (should_we_add_namespace() == YES) {
    if (add_namespace_to_a_label(g_label, sizeof(g_label), YES) == FAILED)
      return FAILED;
  }
  else {
    if (add_namespace_to_a_label(g_label, sizeof(g_label), NO) == FAILED)
      return FAILED;
  }

  /* label_tmp contains the label without possible prefix ':' */
  if (strlen(g_label) > 1 && g_label[0] == ':')
    strcpy(label_tmp, &g_label[1]);
  else
    strcpy(label_tmp, g_label);

  /* check for "string".length */
  if (strstr(g_label, ".length") != NULL) {
    if (parse_string_length(strstr(g_label, ".length")) == FAILED)
      return FAILED;
    return SUCCEEDED;
  }
  else if (strstr(g_label, ".LENGTH") != NULL) {
    if (parse_string_length(strstr(g_label, ".LENGTH")) == FAILED)
      return FAILED;
    return SUCCEEDED;
  }

  if (check_if_a_definition == YES && g_parsing_function_body == NO) {
    /* check if the label is actually a definition */
    struct definition *tmp_def;
    
    if (hashmap_get(g_defines_map, g_label, (void*)&tmp_def) != MAP_OK)
      hashmap_get(g_defines_map, label_tmp, (void*)&tmp_def);
    if (tmp_def != NULL)
      return _input_number_return_definition(tmp_def);
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

  struct definition *tmp_def;

  /* remove ".length" from the end of label (end points to inside of label) */
  end[0] = 0;

  /* check if the label is actually a definition - it should be or else we'll give an error */
  hashmap_get(g_defines_map, g_label, (void*)&tmp_def);
  
  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_VALUE) {
      if (g_input_number_error_msg == YES)
        print_error(ERROR_NUM, ".length of a value does not make any sense.\n");
      return FAILED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_STACK) {
      if (g_input_number_error_msg == YES)
        print_error(ERROR_NUM, ".length of a pending computation does not make any sense.\n");
      return FAILED;
    }
    else if (tmp_def->type == DEFINITION_TYPE_ADDRESS_LABEL) {
      if (g_input_number_error_msg == YES)
        print_error(ERROR_NUM, ".length of an address label does not make any sense.\n");
      return FAILED;
    }
    else {
      g_string_size = tmp_def->size;
      memcpy(g_label, tmp_def->string, g_string_size);
      g_label[g_string_size] = 0;

      g_parsed_int = (int)strlen(g_label);
      g_parsed_double = (double)g_parsed_int;

      return SUCCEEDED;
    }
  }

  return FAILED;
}


static void _skip_whitespace(void) {

  while (g_source_index < g_source_file_size) {
    if (g_buffer[g_source_index] == ' ' || (g_buffer[g_source_index] == '\\' && g_buffer[g_source_index+1] == 0xA)) {
      g_source_index++;
      g_newline_beginning = OFF;
      continue;
    }
    if (g_buffer[g_source_index] == 0xA) {
      g_source_index++;
      next_line();
      continue;
    }
    break;
  }
}

 
int get_next_plain_string(void) {

  int curly_braces = 0;
  char c;
  
  _skip_whitespace();

  g_ss = 0;
  while (1) {
    if (g_ss >= MAX_NAME_LENGTH) {
      print_error(ERROR_NONE, "GET_NEXT_PLAIN_STRING: Too long for a string.\n");
      return FAILED;
    }

    c = g_buffer[g_source_index];

    if (c == '{')
      curly_braces++;
    else if (c == '}')
      curly_braces--;
    else if (c == 0xA)
      break;

    if (curly_braces > 0) {
      g_label[g_ss] = c;
      g_ss++;
      g_source_index++;
    }
    /* NOTE: casting to (unsigned char) is needed to get UTF-8 to work */
    else if ((unsigned char)c > 127 || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '{' || c == '}' || c == '\\' || c == '@' || c == ':') {
      g_label[g_ss] = c;
      g_ss++;
      g_source_index++;
    }
    else if (c == '"')
      g_source_index++;
    else
      break;
  }

  g_label[g_ss] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_label, NULL, NULL) == FAILED)
      return FAILED;
  }

  if (expand_variables_inside_string(g_label, sizeof(g_label), &g_ss) == FAILED)
    return FAILED;

  if (should_we_add_namespace() == YES) {
    if (add_namespace_to_a_label(g_label, sizeof(g_label), YES) == FAILED)
      return FAILED;
  }

  g_ss = (int)strlen(g_label);
  
  return SUCCEEDED;
}


int get_next_token(void) {

  struct definition *tmp_def = NULL;
  
  _skip_whitespace();

  /* skip leading commas */
  while (g_buffer[g_source_index] == ',')
    g_source_index++;
  
  /* "string"? */
  if (g_buffer[g_source_index] == '"') {
    for (g_ss = 0, g_source_index++; g_buffer[g_source_index] != 0xA && g_buffer[g_source_index] != '"'; ) {
      if (g_buffer[g_source_index] == '\\' && g_buffer[g_source_index + 1] == '"') {
        g_tmp[g_ss++] = '"';
        g_source_index += 2;
      }
      else
        g_tmp[g_ss++] = g_buffer[g_source_index++];
    }

    if (g_buffer[g_source_index] == 0xA) {
      print_error(ERROR_NONE, "GET_NEXT_TOKEN: String wasn't terminated properly.\n");
      return FAILED;
    }
    g_tmp[g_ss] = 0;
    g_source_index++;

    /* expand e.g., \1 and \@ */
    if (g_macro_active != 0) {
      if (expand_macro_arguments(g_tmp, NULL, NULL) == FAILED)
        return FAILED;
      g_ss = (int)strlen(g_tmp);
    }

    if (process_string_for_special_characters(g_tmp, &g_ss) == FAILED)
      return FAILED;

    if (g_get_next_token_use_substitution == YES) {
      if (expand_variables_inside_string(g_tmp, g_sizeof_g_tmp, &g_ss) == FAILED)
        return FAILED;
    }
    
    return GET_NEXT_TOKEN_STRING;
  }

  if (g_buffer[g_source_index] == '.') {
    g_tmp[0] = '.';
    g_source_index++;
    for (g_ss = 1; g_buffer[g_source_index] != 0x0A && g_buffer[g_source_index] != ' ' && g_buffer[g_source_index] != '-' && g_ss < MAX_NAME_LENGTH; ) {
      g_tmp[g_ss] = g_buffer[g_source_index];
      g_current_directive[g_ss - 1] = toupper((int)g_buffer[g_source_index]);
      g_source_index++;
      g_ss++;
    }
    g_current_directive[g_ss - 1] = 0;
  }
  else if (g_buffer[g_source_index] == '=' || g_buffer[g_source_index] == '>' || g_buffer[g_source_index] == '<' || g_buffer[g_source_index] == '!') {
    for (g_ss = 0; g_buffer[g_source_index] != 0xA && (g_buffer[g_source_index] == '=' || g_buffer[g_source_index] == '!' || g_buffer[g_source_index] == '<' || g_buffer[g_source_index] == '>')
           && g_ss < MAX_NAME_LENGTH; g_tmp[g_ss++] = g_buffer[g_source_index++])
      ;
  }
  else {
    int curly_brackets = 0;
    
    for (g_ss = 0; g_ss < MAX_NAME_LENGTH; ) {
      unsigned char e = g_buffer[g_source_index];

      if (e == 0xA)
        break;
      else if (e == '{')
        curly_brackets++;
      else if (e == '}') {
        curly_brackets--;
        if (curly_brackets < 0)
          break;
      }
      if (curly_brackets == 0) {
        if (e == ',')
          break;
        if (e == ' ')
          break;
        if (g_ss > 0 && e == '(')
          break;
      }
      g_tmp[g_ss] = e;
      g_ss++;
      g_source_index++;
    }
  }

  if (g_ss >= MAX_NAME_LENGTH) {
    print_error(ERROR_NONE, "GET_NEXT_TOKEN: Too long for a token.\n");
    return FAILED;
  }

  g_tmp[g_ss] = 0;

  /* expand e.g., \1 and \@ */
  if (g_macro_active != 0) {
    if (expand_macro_arguments(g_tmp, NULL, NULL) == FAILED)
      return FAILED;
    g_ss = (int)strlen(g_tmp);
  }

  if (g_get_next_token_use_substitution == YES) {
    if (expand_variables_inside_string(g_tmp, g_sizeof_g_tmp, &g_ss) == FAILED)
      return FAILED;
  }

  /* is it actually a string definition? */
  hashmap_get(g_defines_map, g_tmp, (void*)&tmp_def);
  if (tmp_def != NULL) {
    if (tmp_def->type == DEFINITION_TYPE_STRING) {
      strcpy(g_tmp, tmp_def->string);
      g_ss = (int)strlen(g_tmp);
    }
  }
  
  return SUCCEEDED;
}


int compare_and_skip_next_symbol(char symbol) {

  int pos = g_source_index;

  while (g_buffer[pos] == ' ')
    pos++;

  if (g_buffer[pos] == symbol) {
    pos++;
    g_source_index = pos;
    return SUCCEEDED;
  }

  return FAILED;
}


int skip_next_token(void) {

  int c;
  
  for (c = g_buffer[g_source_index]; c == ' ' || c == ',' || (c == '\\' && g_buffer[g_source_index+1] == 0xA) || c == 0xA; c = g_buffer[++g_source_index]) {
    if (c == 0xA)
      next_line();
  }

  if (g_buffer[g_source_index] == '"') {
    for (g_source_index++; g_buffer[g_source_index] != 0x0A && g_buffer[g_source_index] != '"'; g_source_index++)
      ;
    if (g_buffer[g_source_index] == 0x0A) {
      print_error(ERROR_NONE, "SKIP_NEXT_TOKEN: String wasn't terminated properly.\n");
      return FAILED;
    }
    g_source_index++;

    return SUCCEEDED;
  }
  else if (g_buffer[g_source_index] == '=' || g_buffer[g_source_index] == '>' || g_buffer[g_source_index] == '<' || g_buffer[g_source_index] == '!') {
    for (; g_buffer[g_source_index] != 0xA && (g_buffer[g_source_index] == '=' || g_buffer[g_source_index] == '!' || g_buffer[g_source_index] == '<' || g_buffer[g_source_index] == '>'); g_source_index++)
      ;
  }
  else {
    for (; g_buffer[g_source_index] != 0x0A && g_buffer[g_source_index] != ' ' && g_buffer[g_source_index] != ','; g_source_index++)
      ;
  }
  
  return SUCCEEDED;
}


int _expand_macro_arguments_one_pass(char *in, int *expands, int *move_up, int *res_type, double *res_value) {

  char t[MAX_NAME_LENGTH + 1];
  int i, j, k, argument_start;

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
          print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, ?%d is out of range.\n", g_macro_runtime_current->macro->name, d);
        }
    
        return FAILED;
      }

      /* use the macro argument to find its definition */
      ma = g_macro_runtime_current->argument_data[d - 1];

      if (ma->type == INPUT_NUMBER_ADDRESS_LABEL) {
        struct definition *tmp_def;

        strcpy(g_label, ma->string);

        hashmap_get(g_defines_map, g_label, (void*)&tmp_def);
        if (tmp_def != NULL) {
          int type = _input_number_return_definition(tmp_def);

          if (type == SUCCEEDED)
            snprintf(t, sizeof(t), "%d", g_parsed_int);
          else if (type == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(t, g_label);
          else if (type == INPUT_NUMBER_STRING)
            strcpy(t, g_label);
          else {
            print_error(ERROR_ERR, "The definition cannot be converted to a string.\n");
            return FAILED;
          }
        
          for (j = 0; j < MAX_NAME_LENGTH && k < MAX_NAME_LENGTH; j++, k++) {
            g_expanded_macro_string[k] = t[j];
            if (t[j] == 0)
              break;
          }
        }
        else {
          print_error(ERROR_NUM, "Cannot find definition for \"%s\".\n", g_label);
          return FAILED;
        }
      }
      else {
        print_error(ERROR_ERR, "? can be only used to evaluate definitions.\n");
        return FAILED;
      }
    }
    else if (in[i] == '\\') {
      argument_start = i;
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

        snprintf(t, sizeof(t), "%d", g_macro_runtime_current->macro->calls - 1);
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
            print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, \\?%d is out of range.\n", g_macro_runtime_current->macro->name, d);
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
            print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Macro \"%s\" wasn't called with enough arguments, \\%d is out of range.\n", g_macro_runtime_current->macro->name, d);
          }
    
          return FAILED;
        }

        type = g_macro_runtime_current->argument_data[d - 1]->type;
        g_input_has_leading_hashtag = g_macro_runtime_current->argument_data[d - 1]->has_leading_hashtag;

        /* replace e.g., \1 in the string with processed macro argument */
        if (type == SUCCEEDED || type == INPUT_NUMBER_FLOAT) {
          char tmp_string[32];
          int i;

          if (res_type != NULL && argument_start == 0) {
            /* we need to return this value! */
            if (type == INPUT_NUMBER_FLOAT && g_input_float_mode == ON)
              *res_type = INPUT_NUMBER_FLOAT;
            else
              *res_type = SUCCEEDED;
            *move_up = 0;
            *res_value = g_macro_runtime_current->argument_data[d - 1]->value;
            in[0] = 0;

            return SUCCEEDED;            
          }
          else {
            snprintf(tmp_string, sizeof(tmp_string), "%d", (int)g_macro_runtime_current->argument_data[d - 1]->value);
            for (i = 0; k < MAX_NAME_LENGTH && i < (int)sizeof(tmp_string); i++, k++) {
              if (tmp_string[i] == 0)
                break;
              g_expanded_macro_string[k] = tmp_string[i];
            }
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
          /* it's actually a stack calculation! */
          if (res_type != NULL && argument_start == 0) {
            *res_type = INPUT_NUMBER_STACK;
            *move_up = 0;
            *res_value = g_macro_runtime_current->argument_data[d - 1]->value;
            in[0] = 0;

            return SUCCEEDED;
          }
          else {
            if (g_input_number_error_msg == YES)
              print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Macro argument \\%d is a pending stack calculation and cannot be expanded into a string.\n", d);
    
            return FAILED;
          }
        }
        else {
          if (g_input_number_error_msg == YES)
            print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Macro argument \\%d is of unknown type, please submit a bug report.\n", d);
    
          return FAILED;
        }
      }
      else {
        if (g_input_number_error_msg == YES)
          print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: Unsupported special character '%c'.\n", in[i + 1]);
    
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
      print_error(ERROR_NUM, "EXPAND_MACRO_ARGUMENTS: The result string is too large, increase MAX_NAME_LENGTH and compile WLA DX again.\n");
    }
    
    return FAILED;
  }

  memcpy(in, g_expanded_macro_string, MAX_NAME_LENGTH);
  in[MAX_NAME_LENGTH] = '\0';

  return SUCCEEDED;
}


int _expand_macro_arguments(char *in, int *expands, int *type, double *value) {

  int move_up = 0;

  if (_expand_macro_arguments_one_pass(in, expands, &move_up, type, value) == FAILED)
    return FAILED;

  /* macro argument numbers? if we find and expand some, we'll need to recursively call this function */
  if (move_up > 0) {
    /* move up one macro call in the hierarchy */
    g_macro_active--;
    if (g_macro_active > 0) {
      g_macro_runtime_current = &g_macro_stack[g_macro_active - 1];
      /* recursive call to self */
      return _expand_macro_arguments(in, expands, type, value);
    }
  }

  return SUCCEEDED;
}
  

int expand_macro_arguments(char *in, int *type, double *value) {

  /* save the current macro_runtime pointers */
  struct macro_runtime* mr = g_macro_runtime_current;
  int ma = g_macro_active, expands = 0, ret;

  /* by default we expect strings to stay strings, but _expand_macro_arguments() could return e.g.,
     stack calculations as well */
  if (type != NULL)
    *type = INPUT_NUMBER_ADDRESS_LABEL;
  
  ret = _expand_macro_arguments(in, &expands, type, value);

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
    /* handle '\\' */
    if (label[read] == '\\' && label[read + 1] == '\\') {
      label[write++] = '\\';
      read += 2;
    }    
    /* handle '\0' */
    else if (label[read] == '\\' && label[read + 1] == '0') {
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
        print_error(ERROR_INP, "'\\x' needs hexadecimal byte (00-FF) data.\n");
        return FAILED;
      }

      label[write++] = tmp_c;
      read += 2;
    }
    /* handle '\<' */
    else if (label[read] == '\\' && label[read + 1] == '<') {
      read += 2;
      if (read >= size) {
        print_error(ERROR_INP, "'\\<' needs character data.\n");
        return FAILED;
      }
      label[write++] = label[read] | 0x80;
      read++;
    }
    /* handle '\>' */
    else if (read == 0 && label[read] == '\\' && label[read + 1] == '>') {
      print_error(ERROR_INP, "'\\>' needs character data (previous byte).\n");
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


static int _save_stack_calculation(struct stack_item *stack_items, int stacksize, int linenumber, int filename_id) {

  struct stack *s;
  
  s = calloc(sizeof(struct stack), 1);
  if (s == NULL) {
    free(stack_items);
    print_error(ERROR_STC, "Out of memory error while allocating room for a new calculation stack.\n");
    return FAILED;
  }

  init_stack_struct(s);

  s->stacksize = stacksize;
  s->stack_items = stack_items;

  if (linenumber < 0)
    s->linenumber = g_active_file_info_last->line_current;
  else
    s->linenumber = linenumber;

  if (filename_id < 0)
    s->filename_id = g_active_file_info_last->filename_id;
  else
    s->filename_id = filename_id;

  s->is_function_body = g_parsing_function_body;

  /* all stacks will be definition stacks by default. phase_4 will mark
     those that are referenced to be STACK_POSITION_CODE stacks */
  s->position = STACK_POSITION_DEFINITION;

  calculation_stack_insert(s);

  s->is_single_instance = YES;
  
  return SUCCEEDED;
}


static int _clone_stack_calculation(struct stack_item **stack_items_out, struct stack_item *stack_items, int stacksize, char *function_name) {

  struct stack_item *si;
  int i;
  
  si = calloc(sizeof(struct stack_item) * stacksize, 1);
  if (si == NULL) {
    print_error(ERROR_NUM, "Out of memory error while parsing function \"%s\".\n", function_name);
    return FAILED;
  }

  for (i = 0; i < stacksize; i++) {
    struct stack_item *out = &si[i], *in = &stack_items[i];
    
    out->value = in->value;
    out->type = in->type;
    out->sign = in->sign;
    out->can_calculate_deltas = in->can_calculate_deltas;
    out->has_been_replaced = in->has_been_replaced;
    out->is_in_postfix = in->is_in_postfix;
    if (out->type == STACK_ITEM_TYPE_LABEL)
      strcpy(out->string, in->string);
    else
      out->string[0] = 0;
  }

  *stack_items_out = si;

  return SUCCEEDED;
}


static int _reset_has_been_replaced_status(struct stack_item *stack_items, int stacksize) {

  struct stack *s;
  int i;

  for (i = 0; i < stacksize; i++) {
    stack_items[i].has_been_replaced = NO;

    /* dig deeper */
    if (stack_items[i].type == STACK_ITEM_TYPE_STACK) {
      s = find_stack_calculation((int)stack_items[i].value, YES);
      if (s == NULL)
        return FAILED;
      
      _reset_has_been_replaced_status(s->stack_items, s->stacksize);
    }
  }

  return SUCCEEDED;
}


static int _clone_contained_stack_calculations(struct stack_item *stack_items, int stacksize, char *function_name) {

  struct stack_item *si;
  struct stack *s;
  int i;

  for (i = 0; i < stacksize; i++) {
    if (stack_items[i].type == STACK_ITEM_TYPE_STACK) {
      s = find_stack_calculation((int)stack_items[i].value, YES);
      if (s == NULL)
        return FAILED;

      if (_clone_stack_calculation(&si, s->stack_items, s->stacksize, function_name) == FAILED)
        return FAILED;

      if (_save_stack_calculation(si, s->stacksize, s->linenumber, s->filename_id) == FAILED)
        return FAILED;

      stack_items[i].value = g_latest_stack;

      if (_clone_contained_stack_calculations(si, s->stacksize, function_name) == FAILED)
        return FAILED;
    }
  }

  return SUCCEEDED;
}


static int _replace_labels_inside_stack_calculation(struct stack_item *stack_items, int stacksize, char *label, int result, int parsed_int, double parsed_double, char *parsed_label, int parsed_stack) {

  struct stack *s;
  int j;

  for (j = 0; j < stacksize; j++) {
    struct stack_item *si = &stack_items[j];

    if (si->type == STACK_ITEM_TYPE_STACK) {
      /* dig deeper! */
      s = find_stack_calculation((int)si->value, YES);
      if (s == NULL)
        return FAILED;
      
      if (_replace_labels_inside_stack_calculation(s->stack_items, s->stacksize, label, result, parsed_int, parsed_double, parsed_label, parsed_stack) == FAILED)
        return FAILED;
    }
    else if (si->type == STACK_ITEM_TYPE_LABEL) {
      if (si->has_been_replaced == NO && label[0] == si->string[0] && strcmp(label, si->string) == 0) {
        if (result == SUCCEEDED) {
          si->type = STACK_ITEM_TYPE_VALUE;
          si->value = parsed_int;
        }
        else if (result == INPUT_NUMBER_FLOAT) {
          si->type = STACK_ITEM_TYPE_VALUE;
          si->value = parsed_double;
        }
        else if (result == INPUT_NUMBER_ADDRESS_LABEL)
          strcpy(si->string, parsed_label);
        else {
          si->type = STACK_ITEM_TYPE_STACK;
          si->value = parsed_stack;
        }
      }
    }    
  }

  return SUCCEEDED;
}


int parse_function(char *in, char *name, int *found_function, int *parsed_chars) {

  int res, source_index_original = g_source_index, source_index_backup, i, j, input_float_mode, can_remember_converted_stack_items;
  struct function *fun = g_functions_first;
  struct stack_item *si;
  char c1 = name[0];

  /* NOTE! we assume that 'in' is actually '&g_buffer[xyz]', so
     let's update g_source_index for input_number() */

  /* find the function */
  while (fun != NULL) {
    if (c1 == fun->name[0]) {
      if (strcmp(name, fun->name) == 0)
        break;
    }
    fun = fun->next;
  }

  if (fun == NULL) {
    *found_function = NO;
    return SUCCEEDED;
  }

  g_source_index = (int)(in - g_buffer);
  source_index_backup = g_source_index;
  
  /* we found the function! let's parse the arguments */
  *found_function = YES;

  /* is the function just a constant? */
  if (fun->type == SUCCEEDED) {
    g_parsed_int = (int)fun->value;
    g_parsed_double = fun->value;

    return SUCCEEDED;
  }

  /* clone the stack calculation */
  if (_clone_stack_calculation(&si, fun->stack->stack_items, fun->stack->stacksize, name) == FAILED)
    return FAILED;

  /* clone all contained stack calculations */
  if (_clone_contained_stack_calculations(si, fun->stack->stacksize, name) == FAILED) {
    free(si);
    return FAILED;
  }

  if (g_parsing_function_body == NO) {
    if (_reset_has_been_replaced_status(si, fun->stack->stacksize) == FAILED) {
      free(si);
      return FAILED;
    }
  }

  for (i = 0; i < fun->nargument_names; i++) {
    input_float_mode = g_input_float_mode;
    g_input_float_mode = ON;
    res = input_number();
    while (res == INPUT_NUMBER_EOL) {
      next_line();
      res = input_number();
    }
    g_input_float_mode = input_float_mode;

    if (res == FAILED) {
      free(si);
      return FAILED;
    }
    else if (res != SUCCEEDED && res != INPUT_NUMBER_ADDRESS_LABEL && res != INPUT_NUMBER_STACK && res != INPUT_NUMBER_FLOAT) {
      free(si);
      print_error(ERROR_NUM, "Argument %d is not a value, label or a pending calculation.\n", i+1);
      return FAILED;
    }

    if (res == INPUT_NUMBER_STACK) {
      /* mark all instances of argument names in the stack as has-been-replaced */
      struct stack *stack;

      stack = find_stack_calculation(g_latest_stack, YES);
      if (stack == NULL) {
        free(si);
        return FAILED;
      }
      
      for (j = 0; j < stack->stacksize; j++) {
        if (stack->stack_items[j].type == STACK_ITEM_TYPE_LABEL) {
          int k;

          for (k = 0; k < fun->nargument_names; k++) {
            if (strcmp(fun->argument_names[k], stack->stack_items[j].string) == 0) {
              stack->stack_items[j].has_been_replaced = YES;
              break;
            }
          }
        }
      }
    }
    
    /* substitute the result into the stack calculation */
    for (j = 0; j < fun->stack->stacksize; j++) {
      struct stack_item *sij = &si[j];
      
      if (sij->type == STACK_ITEM_TYPE_STACK) {
        struct stack *s;

        s = find_stack_calculation((int)sij->value, YES);
        if (s == NULL) {
          free(si);
          return FAILED;
        }

        if (_replace_labels_inside_stack_calculation(s->stack_items, s->stacksize, fun->argument_names[i], res, g_parsed_int, g_parsed_double, g_label, g_latest_stack) == FAILED) {
          free(si);
          return FAILED;
        }
      }
      else if (sij->type == STACK_ITEM_TYPE_LABEL) {
        if (strcmp(fun->argument_names[i], sij->string) == 0) {
          if (res == SUCCEEDED) {
            sij->type = STACK_ITEM_TYPE_VALUE;
            sij->value = g_parsed_int;
          }
          else if (res == INPUT_NUMBER_FLOAT) {
            sij->type = STACK_ITEM_TYPE_VALUE;
            sij->value = g_parsed_double;
          }
          else if (res == INPUT_NUMBER_ADDRESS_LABEL)
            strcpy(sij->string, g_label);
          else {
            sij->type = STACK_ITEM_TYPE_STACK;
            sij->value = g_latest_stack;
          }
        }
      }
    }
  }

  if (g_buffer[g_source_index] != ')') {
    free(si);
    print_error(ERROR_NUM, "Malformed \"%s()\" detected!\n", name);
    return FAILED;
  }

  /* skip ')' */
  g_source_index++;

  /* count the parsed chars */
  *parsed_chars = (int)(g_source_index - source_index_backup);

  /* return g_source_index */
  g_source_index = source_index_original;

  /* try to parse the stack calculation */
  can_remember_converted_stack_items = g_can_remember_converted_stack_items;
  g_can_remember_converted_stack_items = NO;
  if (g_parsing_function_body == NO && resolve_stack(si, fun->stack->stacksize) == SUCCEEDED) {
    struct stack s;
    double dou;

    init_stack_struct(&s);
    s.stack_items = si;
    s.linenumber = fun->line_number;
    s.filename_id = fun->filename_id;

    g_can_remember_converted_stack_items = can_remember_converted_stack_items;
    
    if (compute_stack(&s, fun->stack->stacksize, &dou) == FAILED)
      return FAILED;

    free(si);

    g_parsed_double = dou;
    g_parsed_int = (int)dou;

    return SUCCEEDED;
  }

  g_can_remember_converted_stack_items = can_remember_converted_stack_items;
  
  /* save the stack calculation */
  _save_stack_calculation(si, fun->stack->stacksize, -1, -1);

  return INPUT_NUMBER_STACK;
}


#if defined(PROFILE_FUNCTIONS)
int input_number(void) {

  int ret;
  PROFILE_VARIABLES();

  PROFILE_START();
  ret = _input_number();
  PROFILE_END("input_number");

  return ret;
}
#endif
 

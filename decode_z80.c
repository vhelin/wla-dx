
case 0:
for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
  if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
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
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
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
          output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

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
    output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
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
          output_assembled_instruction(g_instruction_tmp, "y%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "y%d c%d ", g_instruction_tmp->hex, g_latest_stack);

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
          output_assembled_instruction(g_instruction_tmp, "y%d d%d d%d ", g_instruction_tmp->hex, g_parsed_int, g_instruction_tmp->hex_x);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label, g_instruction_tmp->hex_x);
        else
          output_assembled_instruction(g_instruction_tmp, "y%d c%d d%d ", g_instruction_tmp->hex, g_latest_stack, g_instruction_tmp->hex_x);

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
          output_assembled_instruction(g_instruction_tmp, "y%d y%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d y%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "y%d C%d ", g_instruction_tmp->hex, g_latest_stack);

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
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "y%d d%d ", g_instruction_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "y%d R%s ", g_instruction_tmp->hex, labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "y%d c%d ", g_instruction_tmp->hex, h);

            if (z == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
            else
              output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
            
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
        output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
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
        output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
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
              output_assembled_instruction(g_instruction_tmp, "y%d d%d d%d ", g_instruction_tmp->hex, g_parsed_int, g_instruction_tmp->hex_x);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label, g_instruction_tmp->hex_x);
            else
              output_assembled_instruction(g_instruction_tmp, "y%d c%d d%d ", g_instruction_tmp->hex, g_latest_stack, g_instruction_tmp->hex_x);
            
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
        output_assembled_instruction(g_instruction_tmp, "k%d v1 ", g_active_file_info_last->line_current);
        if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);       

        /* reset to "no special case" */
        output_assembled_instruction(g_instruction_tmp, "v0 ");
        
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

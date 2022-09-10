
case 0:
for ( ; x < INSTRUCTION_STRING_LENGTH_MAX; g_inz++, x++) {
  if (g_instruction_tmp->string[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
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
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
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
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
        
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

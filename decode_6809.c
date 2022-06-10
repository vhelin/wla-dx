
case 0:
/* plain text 8-bit */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    if (g_instruction_tmp->hex > 0xFF)
      output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
    else
      output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 5:
case 1:
/* 8-bit signed operand, relative address */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
          if (g_instruction_tmp->type == 5) {
            /* 5 -> let's configure the stack so that all label references inside are relative */
            g_stacks_tmp->relative_references = 1;
          }
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 2:
/* 16-bit operand */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == '?') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "r%s ", g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);

        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 3:
/* plain text 16-bit */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "y%d ", g_instruction_tmp->hex);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 4:
/* 8-bit unsigned operand, absolute address */
if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "Q%s ", g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 6:
/* 5-bit signed operand, absolute address + post op byte code */
if (g_xbit_size >= 8 && g_instruction_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 's') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

        output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->addressing_mode_bits | (g_parsed_int & 0x1F));
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 7:
/* 8-bit signed operand, relative address + post op byte code */
if (g_xbit_size > 8 && g_instruction_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
                
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->addressing_mode_bits, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->addressing_mode_bits, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->addressing_mode_bits, g_latest_stack);

          /* let's configure the stack so that all label references inside are relative */
          g_stacks_tmp->relative_references = 1;
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 8:
/* 16-bit operand + post op byte code */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == '?') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->addressing_mode_bits, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->addressing_mode_bits, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->addressing_mode_bits, g_latest_stack);

        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 9:
/* plain text 8-bit + post op byte code */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

    if (g_instruction_tmp->hex > 0xFF)
      output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
    else
      output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->addressing_mode_bits);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 10:
/* EXG / TFR */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'r') {
    g_parsed_int = parse_exg_tfr_registers();
    if (g_parsed_int < 0)
      return FAILED;

    output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 11:
/* PSHS / PSHU / PULS / PULU */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'r') {
    g_parsed_int = parse_push_pull_registers(g_instruction_tmp->addressing_mode_bits);
    if (g_parsed_int < 0)
      return FAILED;

    output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
        
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 12:
/* 16-bit signed operand, relative address */
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == '?') {
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

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_instruction_tmp->hex > 0xFF)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", (g_instruction_tmp->hex >> 8) & 0xFF, g_instruction_tmp->hex & 0xFF);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "y%d ", g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "M%s ", g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "C%d ", g_latest_stack);
          /* let's configure the stack so that all label references inside are relative */
          g_stacks_tmp->relative_references = 1;
        }

        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

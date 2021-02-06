
case 0:
/* plain text 8-bit */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    if (opt_tmp->hex > 0xFF)
      output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
    else
      output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 5:
case 1:
/* 8-bit signed operand, relative address */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 127 || d < -128)) {
      print_error("Out of signed 8-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d ", d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "R%s ", g_label);
        else {
          output_assembled_opcode(opt_tmp, "c%d ", latest_stack);
          if (opt_tmp->type == 5) {
            /* 5 -> let's configure the stack so that all label references inside are relative */
            stacks_tmp->relative_references = 1;
          }
        }
        
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 2:
/* 16-bit operand */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "y%d ", d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "r%s ", g_label);
        else
          output_assembled_opcode(opt_tmp, "C%d ", latest_stack);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 3:
/* plain text 16-bit */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    output_assembled_opcode(opt_tmp, "y%d ", opt_tmp->hex);
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 4:
/* 8-bit unsigned operand, absolute address */
if (xbit_size > 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (g_operand_hint == HINT_16BIT)
      break;
    if (z == SUCCEEDED && (d > 255 || d < 0)) {
      if (opt_tmp->skip_8bit == 1)
        break;
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d ", d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "Q%s ", g_label);
        else
          output_assembled_opcode(opt_tmp, "c%d ", latest_stack);
        
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 6:
/* 5-bit signed operand, absolute address + post op byte code */
if (xbit_size >= 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 's') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;

    /* TODO: add a mechanism so that we could use 5-bit addreslabel references and calculations */
    /* currently we just fall back to 8-bit/16-bit ones */
    if (z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)
      break;
    if (z != SUCCEEDED)
      return FAILED;
    if (g_operand_hint == HINT_8BIT || g_operand_hint == HINT_16BIT)
      break;
    if (d > 15 || d < -16)
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);

        output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->addressing_mode_bits | (d & 0x1F));
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 7:
/* 8-bit signed operand, relative address + post op byte code */
if (xbit_size > 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (g_operand_hint == HINT_16BIT)
      break;
    if (z == SUCCEEDED && (d > 127 || d < -128)) {
      if (opt_tmp->skip_8bit == 1)
        break;
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
                
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->addressing_mode_bits, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "d%d R%s ", opt_tmp->addressing_mode_bits, g_label);
        else {
          output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->addressing_mode_bits, latest_stack);

          /* let's configure the stack so that all label references inside are relative */
          stacks_tmp->relative_references = 1;
        }
        
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 8:
/* 16-bit operand + post op byte code */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);

        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->addressing_mode_bits, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "d%d r%s ", opt_tmp->addressing_mode_bits, g_label);
        else
          output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->addressing_mode_bits, latest_stack);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 9:
/* plain text 8-bit + post op byte code */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

    if (opt_tmp->hex > 0xFF)
      output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
    else
      output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);

    output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->addressing_mode_bits);
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 10:
/* EXG / TFR */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'r') {
    d = parse_exg_tfr_registers();
    if (d < 0)
      return FAILED;

    output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);
    output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
    output_assembled_opcode(opt_tmp, "d%d ", d);
        
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 11:
/* PSHS / PSHU / PULS / PULU */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'r') {
    d = parse_push_pull_registers(opt_tmp->addressing_mode_bits);
    if (d < 0)
      return FAILED;

    output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);
    output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
    output_assembled_opcode(opt_tmp, "d%d ", d);
        
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 12:
/* 16-bit signed operand, relative address */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (opt_tmp->hex > 0xFF)
          output_assembled_opcode(opt_tmp, "d%d d%d ", (opt_tmp->hex >> 8) & 0xFF, opt_tmp->hex & 0xFF);
        else
          output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
        
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "y%d ", d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "M%s ", g_label);
        else {
          output_assembled_opcode(opt_tmp, "C%d ", latest_stack);
          /* let's configure the stack so that all label references inside are relative */
          stacks_tmp->relative_references = 1;
        }

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;


case 0:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 11:
case 1:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (g_parsed_int > 127 || g_parsed_int < -128)) {
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else {
          output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);
          if (g_opt_tmp->type == 11) {
            /* 11 -> let's configure the stack so that all label references inside are relative */
            g_stacks_tmp->relative_references = 1;
          }
        }

        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 2:
if (g_xbit_size > 16 && g_opt_tmp->skip_xbit == 1)
  break;
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (g_operand_hint == HINT_24BIT && g_opt_tmp->skip_xbit == 1)
      break;
    if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 65535)) {
      if (g_opt_tmp->skip_xbit == 1)
        break;
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex, g_latest_stack);
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 3:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '&') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_STACK || z == INPUT_NUMBER_ADDRESS_LABEL))
      return FAILED;
    
    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d z%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d q%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "d%d T%d ", g_opt_tmp->hex, g_latest_stack);
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

/* IMMEDIATE VALUES: ACCUMULATOR */

case 4:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    /* select the mode */
    if (g_operand_hint_type == HINT_TYPE_GIVEN) {
      if (g_operand_hint == HINT_8BIT)
        y = 0;
      else
        y = 1;
    }
    else {
      if (g_accu_size == 8)
        y = 0;
      else
        y = 1;
    }
    
    if (y == 0) {
      if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }
    else {
      if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
        print_error("Out of 16-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }
    
    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        /* 8BIT */
        if (y == 0) {
          if (z == SUCCEEDED)
            output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(g_opt_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
          else
            output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);
        }
        /* 16BIT */
        else {
          if (z == SUCCEEDED)
            output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex, g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(g_opt_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
          else
            output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex, g_latest_stack);
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

/* MVN & MVP */

case 5:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
      break;

    e = g_parsed_int;
    v = z;
    h = g_latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, g_label);

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 'x') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
          break;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            /* NOTE: in the source code it's "MVP/MVN x1, x2", but we output "MVP/MVN x2, x1" */
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);
            
            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d Q%s ", g_opt_tmp->hex, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);

            if (v == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d ", e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "Q%s ", labelx);
            else
              output_assembled_opcode(g_opt_tmp, "c%d ", h);

            g_source_pointer = g_inz;
            return SUCCEEDED;
          }
          if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
            break;
        }
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

/* SEP & REP */

case 6:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED) {
      if (g_opt_tmp->skip_xbit == 0)
        print_error("REP needs immediate data.\n", ERROR_LOG);
      else
        print_error("SEP needs immediate data.\n", ERROR_LOG);
      return FAILED;
    }
    if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
      break;
    
    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        
        output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
        
        /* REP */
        if (g_opt_tmp->skip_xbit == 0) {
          if (g_parsed_int & 16)
            g_index_size = 16;
          if (g_parsed_int & 32)
            g_accu_size = 16;
        }
        /* SEP */
        else {
          if (g_parsed_int & 16)
            g_index_size = 8;
          if (g_parsed_int & 32)
            g_accu_size = 8;
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

/* IMMEDIATE VALUES: INDEX */

case 7:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    /* select the mode */
    if (g_operand_hint_type == HINT_TYPE_GIVEN) {
      if (g_operand_hint == HINT_8BIT)
        y = 0;
      else
        y = 1;
    }
    else {
      if (g_index_size == 8)
        y = 0;
      else
        y = 1;
    }
    
    if (y == 0) {
      if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }
    else {
      if (z == SUCCEEDED && (g_parsed_int > 65535 || g_parsed_int < -32768)) {
        print_error("Out of 16-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }
    
    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        /* 8BIT */
        if (y == 0) {
          if (z == SUCCEEDED)
            output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(g_opt_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
          else
            output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);
        }
        /* 16BIT */
        else {
          if (z == SUCCEEDED)
            output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex, g_parsed_int);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(g_opt_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
          else
            output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex, g_latest_stack);
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 8:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_opcode(g_opt_tmp, "y%d ", g_opt_tmp->hex);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 9:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (g_parsed_int < -32768 || g_parsed_int > 32767))
      break;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d M%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else {
          output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex, g_latest_stack);

          /* let's configure the stack so that all label references inside are relative */
          g_stacks_tmp->relative_references = 1;
        }
        
        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 0xA:
if (g_xbit_size > 8 && g_opt_tmp->skip_xbit == 2)
  break;
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if ((g_operand_hint == HINT_16BIT || g_operand_hint == HINT_24BIT) && g_opt_tmp->skip_xbit == 2)
      break;
    if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128)) {
      if (g_opt_tmp->skip_xbit == 2)
        break;
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      return FAILED;
    }
    
    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);

        g_source_pointer = g_inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

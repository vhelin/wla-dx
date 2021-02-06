
case 0:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
    g_source_pointer = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 1:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x' || opt_tmp->op[x] == 's') {
    y = g_source_pointer;
    g_source_pointer = inz;
    if (x > 0 && (opt_tmp->op[x-1] == '+' || opt_tmp->op[x-1] == '-'))
      g_source_pointer--;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED) {
      if ((opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, opt_tmp->hex, g_label);
        else {
          output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, latest_stack);
          
          if (opt_tmp->type == 4) {
            /* 4 -> let's configure the stack so that all label references inside are relative */
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
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, opt_tmp->hex, g_label);
        else
          output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->hex, latest_stack);
        
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
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED || d != opt_tmp->value)
      break;
    
    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
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

case 100:
/* "RST *" that gets delayed to WLALINK */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(opt_tmp, "k%d v2 ", g_active_file_info_last->line_current);
        if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "Q%s ", g_label);
        else
          output_assembled_opcode(opt_tmp, "c%d ", latest_stack);       

        /* reset to "no special case" */
        output_assembled_opcode(opt_tmp, "v0 ");
        
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

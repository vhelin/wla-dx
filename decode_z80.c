
case 0:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex);
    g_source_pointer = inz;

    /* give a warning when assembling "JP (HL)"! */
    /*
      if (g_opt_tmp->hex == 0xE9 && strcmp(g_opt_tmp->op, "JP (HL)") == 0)
      print_error("\"JP (HL)\" is semantically incorrect. Please use \"JP HL\" instead.\n", ERROR_WRN);
    */
    /* give a warning when assembling "JP (HL')"! */
    /*
      if (g_opt_tmp->hex == 0xE9 && strcmp(g_opt_tmp->op, "JP (HL')") == 0)
      print_error("\"JP (HL')\" is semantically incorrect. Please use \"JP HL'\" instead.\n", ERROR_WRN);
    */

    return SUCCEEDED;
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 11:
case 1:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else {
          output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);
          if (g_opt_tmp->type == 11) {
            /* 11 -> let's configure the stack so that all label references inside are relative */
            g_stacks_tmp->relative_references = 1;
          }
        }

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 2:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '?') {
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
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex, g_latest_stack);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 3:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
    output_assembled_opcode(g_opt_tmp, "y%d ", g_opt_tmp->hex);
    g_source_pointer = inz;

    /* give a warning when assembling "JP (IX)"! */
    /*
      if (g_opt_tmp->hex == 0xE9DD && strcmp(g_opt_tmp->op, "JP (IX)") == 0)
      print_error("\"JP (IX)\" is semantically incorrect. Please use \"JP IX\" instead.\n", ERROR_WRN);
    */
    /* give a warning when assembling "JP (IY)"! */
    /*
      if (g_opt_tmp->hex == 0xE9FD && strcmp(g_opt_tmp->op, "JP (IY)") == 0)
      print_error("\"JP (IY)\" is semantically incorrect. Please use \"JP IY\" instead.\n", ERROR_WRN);
    */
    
    return SUCCEEDED;
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 4:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 'x' || g_opt_tmp->op[x] == 's') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    if (z == SUCCEEDED) {
      if ((g_opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (g_opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }

    for (x++; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "y%d d%d ", g_opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d y%d R%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "y%d c%d ", g_opt_tmp->hex, g_latest_stack);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 5:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 'x' || g_opt_tmp->op[x] == 's') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    
    if (z == SUCCEEDED) {
      if ((g_opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (g_opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "y%d d%d d%d ", g_opt_tmp->hex, d, g_opt_tmp->hex_x);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label, g_opt_tmp->hex_x);
        else
          output_assembled_opcode(g_opt_tmp, "y%d c%d d%d ", g_opt_tmp->hex, g_latest_stack, g_opt_tmp->hex_x);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 6:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "y%d y%d ", g_opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "k%d y%d r%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label);
        else
          output_assembled_opcode(g_opt_tmp, "y%d C%d ", g_opt_tmp->hex, g_latest_stack);

        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 7:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == 'x' || g_opt_tmp->op[x] == 's') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;

    if (z == SUCCEEDED) {
      if ((g_opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (g_opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        return FAILED;
      }
    }
    
    e = d;
    v = z;
    h = g_latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, g_label);

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 'x') {
        y = g_source_pointer;
        g_source_pointer = inz;
        z = input_number();
        inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED && (d > 255 || d < -128))
          break;

        for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
            if (v == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "y%d d%d ", g_opt_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "k%d y%d R%s ", g_active_file_info_last->line_current, g_opt_tmp->hex, labelx);
            else
              output_assembled_opcode(g_opt_tmp, "y%d c%d ", g_opt_tmp->hex, h);

            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d ", d);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "k%d R%s ", g_active_file_info_last->line_current, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "c%d ", g_latest_stack);
            
            g_source_pointer = inz;
            return SUCCEEDED;
          }
          if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
            break;
        }
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 8:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED || d != g_opt_tmp->value)
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex);
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 9:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED || d != g_opt_tmp->value)
      break;
    
    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(g_opt_tmp, "y%d ", g_opt_tmp->hex);
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 10:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED || d != g_opt_tmp->value)
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 'x' || g_opt_tmp->op[x] == 's') {
        y = g_source_pointer;
        g_source_pointer = inz;
        z = input_number();
        inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        
        if (z == SUCCEEDED) {
          if ((g_opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
              (g_opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
            print_error("Out of 8-bit range.\n", ERROR_NUM);
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "y%d d%d d%d ", g_opt_tmp->hex, d, g_opt_tmp->hex_x);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "k%d y%d R%s d%d ", g_active_file_info_last->line_current, g_opt_tmp->hex, g_label, g_opt_tmp->hex_x);
            else
              output_assembled_opcode(g_opt_tmp, "y%d c%d d%d ", g_opt_tmp->hex, g_latest_stack, g_opt_tmp->hex_x);
            
            g_source_pointer = inz;
            return SUCCEEDED;
          }
          if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
            break;
        }
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

case 100:
/* "RST *" that gets delayed to WLALINK */
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    y = g_source_pointer;
    g_source_pointer = inz;
    z = input_number();
    inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[inz] == 0x0A) {
        output_assembled_opcode(g_opt_tmp, "k%d v1 ", g_active_file_info_last->line_current);
        if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "Q%s ", g_label);
        else
          output_assembled_opcode(g_opt_tmp, "c%d ", g_latest_stack);       

        /* reset to "no special case" */
        output_assembled_opcode(g_opt_tmp, "v0 ");
        
        g_source_pointer = inz;
        return SUCCEEDED;
      }
      if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
        break;
    }
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[inz]))
    break;
 }
break;

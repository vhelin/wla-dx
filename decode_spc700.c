
case 0:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);
    g_source_pointer = g_inz;
    return SUCCEEDED;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 1:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
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

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);

        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "d%d r%s ", g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d C%d ", g_instruction_tmp->hex, g_latest_stack);

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
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
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
      if (g_instruction_tmp->op[x] == '~') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (z != SUCCEEDED)
          break;
        if (g_parsed_int > 7 || g_parsed_int < 0)
          break;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
            output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g_parsed_int << 5));

            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d ", e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "c%d ", h);

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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 4:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
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
      if (g_instruction_tmp->op[x] == '~') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (z != SUCCEEDED)
          break;
        if (g_parsed_int > 7 || g_parsed_int < 0)
          break;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
            output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (((g_parsed_int << 1) + 1) << 4));

            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d ", e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "c%d ", h);

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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 5:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == '~') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (z != SUCCEEDED)
      break;
    if (g_parsed_int > 0xF || g_parsed_int < 0)
      break;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g_parsed_int << 4));
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

case 0xA:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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
      if (g_instruction_tmp->op[x] == 'x') {
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
          if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);

            if (z == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
            else {
              output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 0xB:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
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
      if (g_instruction_tmp->op[x] == 'x') {
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
          if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
            if (z == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, g_label);
            else
              output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);

            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d ", e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "c%d ", h);

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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 0xC:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
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
      if (g_instruction_tmp->op[x] == '~') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (z != SUCCEEDED)
          break;
        if (g_parsed_int > 7 || g_parsed_int < 0)
          break;
        g = g_parsed_int;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_instruction_tmp->op[x] == 'x') {
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
              if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
                output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (g << 5));

                if (v == SUCCEEDED)
                  output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                  output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                else
                  output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                if (z == SUCCEEDED)
                  output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                else {
                  output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
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
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 0xD:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
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
      if (g_instruction_tmp->op[x] == '~') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (z != SUCCEEDED)
          break;
        if (g_parsed_int > 7 || g_parsed_int < 0)
          break;
        g = g_parsed_int;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_instruction_tmp->op[x] == 'x') {
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
              if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
                output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
                output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex | (((g << 1) + 1) << 4));

                if (v == SUCCEEDED)
                  output_assembled_instruction(g_instruction_tmp, "d%d ", e);
                else if (v == INPUT_NUMBER_ADDRESS_LABEL)
                  output_assembled_instruction(g_instruction_tmp, "Q%s ", labelx);
                else
                  output_assembled_instruction(g_instruction_tmp, "c%d ", h);

                if (z == SUCCEEDED)
                  output_assembled_instruction(g_instruction_tmp, "d%d ", g_parsed_int);
                else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                  output_assembled_instruction(g_instruction_tmp, "R%s ", g_label);
                else {
                  output_assembled_instruction(g_instruction_tmp, "c%d ", g_latest_stack);
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
      }
      if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
        break;
    }
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 0xE:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 'x') {
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
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
          
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "d%d R%s ", g_instruction_tmp->hex, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
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

case 0xF:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == '?') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    g_parse_floats = NO;
    z = input_number();
    g_parse_floats = YES;
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (g_parsed_int > 8191 || g_parsed_int < 0))
      break;

    e = g_parsed_int;
    v = z;
    h = g_latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, g_label);

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == '~') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (z != SUCCEEDED)
          break;
        if (g_parsed_int > 7 || g_parsed_int < 0)
          break;

        for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
              
            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, e | g_parsed_int << 13);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "d%d n%d %s ", g_instruction_tmp->hex, g_parsed_int, labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "d%d N%d %d ", g_instruction_tmp->hex, g_parsed_int, h);

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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;


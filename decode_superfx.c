
case 0:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);
        
    if (g_opt_tmp->prefix != 0)
      output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);

    output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex);
    g_source_pointer = g_inz;

    return SUCCEEDED;
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 1:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (g_opt_tmp->prefix != 0)
          output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);
        
        output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex | tiny);

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
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == 'e') {
    y = g_source_pointer;
    g_source_pointer = g_inz;
    z = input_number();
    g_inz = g_source_pointer;
    g_source_pointer = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED) {
      if (g_parsed_int > 127 || g_parsed_int < -128) {
        print_error(ERROR_NUM, "Out of 8-bit range.\n");
        return FAILED;
      }
    }

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

        if (z == SUCCEEDED)
          output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(g_opt_tmp, "d%d R%s ", g_opt_tmp->hex, g_label);
        else {
          output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex, g_latest_stack);
          
          /* 4 -> let's configure the stack so that all label references inside are relative */
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

case 3:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 'x') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED) {
          if (g_parsed_int > 255 || g_parsed_int < -128) {
            print_error(ERROR_NUM, "Out of 8-bit range.\n");
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (g_opt_tmp->prefix != 0)
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);
            
            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex | tiny, g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d R%s ", g_opt_tmp->hex | tiny, g_label);
            else {
              output_assembled_opcode(g_opt_tmp, "d%d c%d ", g_opt_tmp->hex | tiny, g_latest_stack);

              /* 4 -> let's configure the stack so that all label references inside are relative */
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
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 4:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '?') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED) {
          if (g_parsed_int > 65535 || g_parsed_int < -32768) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (g_opt_tmp->prefix != 0)
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);

            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex | tiny, g_parsed_int);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d r%s ", g_opt_tmp->hex | tiny, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex | tiny, g_latest_stack);

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

case 5:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == 'y') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED) {
          if (g_parsed_int > 510 || g_parsed_int < 0) {
            print_error(ERROR_NUM, "Out of 9-bit range.\n");
            return FAILED;
          }
          if ((g_parsed_int & 1) == 1) {
            print_error(ERROR_NUM, "The RAM address must be even.\n");
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (g_opt_tmp->prefix != 0)
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);

            if (z == SUCCEEDED)
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex | tiny, g_parsed_int >> 1);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d *%s ", g_opt_tmp->hex | tiny, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d -%d ", g_opt_tmp->hex | tiny, g_latest_stack);

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

case 6:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        z = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
        if (z < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);
            if (g_opt_tmp->hex == 0xB0) {
              /* MOVES */
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix | tiny);
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex | z);
            }
            else {
              /* OTHERWISE */
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix | z);
              output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->hex | tiny);
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
  }
  if (g_opt_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 7:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '?') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED) {
          if (g_parsed_int > 65535 || g_parsed_int < -32768) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (z == SUCCEEDED) {
              if (g_parsed_int >= -128 && g_parsed_int <= 127) {
                /* use IBT */
                output_assembled_opcode(g_opt_tmp, "d%d d%d ", 0xA0 | tiny, g_parsed_int);
              }
              else {
                /* use IWT */
                output_assembled_opcode(g_opt_tmp, "d%d y%d ", 0xF0 | tiny, g_parsed_int);
              }
            }
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d r%s ", 0xF0 | tiny, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d C%d ", 0xF0 | tiny, g_latest_stack);

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

case 8:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '?') {
        y = g_source_pointer;
        g_source_pointer = g_inz;
        z = input_number();
        g_inz = g_source_pointer;
        g_source_pointer = y;
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
          return FAILED;
        if (z == SUCCEEDED) {
          if (g_parsed_int > 65535 || g_parsed_int < -32768) {
            print_error(ERROR_NUM, "Out of 16-bit range.\n");
            return FAILED;
          }
        }

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (z == SUCCEEDED) {
              if ((g_parsed_int >= 0 && g_parsed_int <= 510) && ((g_parsed_int & 1) == 0)) {
                /* use LMS */
                output_assembled_opcode(g_opt_tmp, "d%d d%d d%d ", 0x3D, 0xA0 | tiny, g_parsed_int >> 1);
              }
              else {
                /* use LM */
                output_assembled_opcode(g_opt_tmp, "d%d d%d y%d ", 0x3D, 0xF0 | tiny, g_parsed_int);
              }
            }
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d d%d r%s ", 0x3D, 0xF0 | tiny, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d d%d C%d ", 0x3D, 0xF0 | tiny, g_latest_stack);

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
    if (z == SUCCEEDED) {
      if (g_parsed_int > 65535 || g_parsed_int < -32768) {
        print_error(ERROR_NUM, "Out of 16-bit range.\n");
        return FAILED;
      }
      y = g_parsed_int;
    }

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
        if (tiny < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (z == SUCCEEDED) {
              if ((y >= 0 && y <= 510) && ((y & 1) == 0)) {
                /* use SMS */
                output_assembled_opcode(g_opt_tmp, "d%d d%d d%d ", 0x3E, 0xA0 | tiny, y >> 1);
              }
              else {
                /* use SM */
                output_assembled_opcode(g_opt_tmp, "d%d d%d y%d ", 0x3E, 0xF0 | tiny, y);
              }
            }
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(g_opt_tmp, "d%d d%d r%s ", 0x3E, 0xF0 | tiny, g_label);
            else
              output_assembled_opcode(g_opt_tmp, "d%d d%d C%d ", 0x3E, 0xF0 | tiny, g_latest_stack);

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

case 10:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        z = parse_tiny_int(0, 11);
        if (z < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (tiny == 0) /* LDB (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", 0x3D, 0x40 | z);
            else /* TO R* + LDB (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d d%d ", 0x10 | tiny, 0x3D, 0x40 | z);

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

case 11:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(0, 11);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        z = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
        if (z < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (z == 0) /* STB (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", 0x3D, 0x30 | tiny);
            else /* FROM R* + STB (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d d%d ", 0xB0 | z, 0x3D, 0x30 | tiny);

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

case 12:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        z = parse_tiny_int(0, 11);
        if (z < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (tiny == 0) /* LDW (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d ", 0x40 | z);
            else /* TO R* + LDW (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", 0x10 | tiny, 0x40 | z);

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

case 13:
for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_opt_tmp->op[x] == '*') {
    tiny = parse_tiny_int(0, 11);
    if (tiny < 0)
      return FAILED;

    for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_opt_tmp->op[x] == '*') {
        z = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
        if (z < 0)
          return FAILED;

        for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
          if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
            output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

            if (z == 0) /* STW (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d ", 0x30 | tiny);
            else /* FROM R* + STW (R*) */
              output_assembled_opcode(g_opt_tmp, "d%d d%d ", 0xB0 | z, 0x30 | tiny);

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

case 14:
{
  char labelx[MAX_NAME_LENGTH + 1];
  int v, s;
  
  for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
    if (g_opt_tmp->op[x] == '?') {
      y = g_source_pointer;
      g_source_pointer = g_inz;
      z = input_number();
      g_inz = g_source_pointer;
      g_source_pointer = y;
      if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
        return FAILED;
      if (z == SUCCEEDED) {
        if (g_parsed_int > 65535 || g_parsed_int < -32768) {
          print_error(ERROR_NUM, "Out of 16-bit range.\n");
          return FAILED;
        }
      }

      /* backup as parse_tiny_int() will overwrite these */
      v = g_parsed_int;
      strcpy(labelx, g_label);
      s = g_latest_stack;

      for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
        if (g_opt_tmp->op[x] == '*') {
          tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
          if (tiny < 0)
            return FAILED;
          
          for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
            if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
              output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);

              if (g_opt_tmp->prefix != 0)
                output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);

              if (z == SUCCEEDED)
                output_assembled_opcode(g_opt_tmp, "d%d y%d ", g_opt_tmp->hex | tiny, v);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                output_assembled_opcode(g_opt_tmp, "d%d r%s ", g_opt_tmp->hex | tiny, labelx);
              else
                output_assembled_opcode(g_opt_tmp, "d%d C%d ", g_opt_tmp->hex | tiny, s);

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
}
break;

case 15:
{
  char labelx[MAX_NAME_LENGTH + 1];
  int v, s;
  
  for ( ; x < OP_SIZE_MAX; g_inz++, x++) {
    if (g_opt_tmp->op[x] == 'y') {
      y = g_source_pointer;
      g_source_pointer = g_inz;
      z = input_number();
      g_inz = g_source_pointer;
      g_source_pointer = y;
      if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
        return FAILED;
      if (z == SUCCEEDED) {
        if (g_parsed_int > 510 || g_parsed_int < 0) {
          print_error(ERROR_NUM, "Out of 9-bit range.\n");
          return FAILED;
        }
        if ((g_parsed_int & 1) == 1) {
          print_error(ERROR_NUM, "The RAM address must be even.\n");
          return FAILED;
        }
      }

      /* backup as parse_tiny_int() will overwrite these */
      v = g_parsed_int;
      strcpy(labelx, g_label);
      s = g_latest_stack;
      
      for (x++; x < OP_SIZE_MAX; g_inz++, x++) {
        if (g_opt_tmp->op[x] == '*') {
          tiny = parse_tiny_int(g_opt_tmp->min, g_opt_tmp->max);
          if (tiny < 0)
            return FAILED;

          for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
            if (g_opt_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
              output_assembled_opcode(g_opt_tmp, "k%d ", g_active_file_info_last->line_current);
              
              if (g_opt_tmp->prefix != 0)
                output_assembled_opcode(g_opt_tmp, "d%d ", g_opt_tmp->prefix);

              if (z == SUCCEEDED)
                output_assembled_opcode(g_opt_tmp, "d%d d%d ", g_opt_tmp->hex | tiny, v >> 1);
              else if (z == INPUT_NUMBER_ADDRESS_LABEL)
                output_assembled_opcode(g_opt_tmp, "d%d *%s ", g_opt_tmp->hex | tiny, labelx);
              else
                output_assembled_opcode(g_opt_tmp, "d%d -%d ", g_opt_tmp->hex | tiny, s);

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
}
break;

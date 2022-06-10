
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

case 9:
case 1:
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
    if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
      break;
    if (g_operand_hint == HINT_16BIT)
      break;

    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d R%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else {
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
          if (g_instruction_tmp->type == 9) {
            /* 9 -> let's configure the stack so that all label references inside are relative */
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
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d y%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d r%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
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
s = 0;
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
    e = g_parsed_int;
    v = z;
    h = g_latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, g_label);
    s = 1;
    break;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
if (s == 0)
  break;

for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
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
    r = g_parsed_int;
    t = z;
    u = g_latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labely, g_label);
    s = 2;
    break;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
if (s == 1)
  break;

for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
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
    s = 3;
    break;
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
if (s == 2)
  break;

for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
  if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
    output_assembled_instruction(g_instruction_tmp, "k%d ", g_active_file_info_last->line_current);
    output_assembled_instruction(g_instruction_tmp, "d%d ", g_instruction_tmp->hex);

    /* first operand */
    if (v == SUCCEEDED)
      output_assembled_instruction(g_instruction_tmp, "y%d ", e);
    else if (v == INPUT_NUMBER_ADDRESS_LABEL)
      output_assembled_instruction(g_instruction_tmp, "r%s ", labelx);
    else
      output_assembled_instruction(g_instruction_tmp, "C%d ", h);

    /* second operand */
    if (t == SUCCEEDED)
      output_assembled_instruction(g_instruction_tmp, "y%d ", r);
    else if (t == INPUT_NUMBER_ADDRESS_LABEL)
      output_assembled_instruction(g_instruction_tmp, "r%s ", labely);
    else
      output_assembled_instruction(g_instruction_tmp, "C%d ", u);

    /* third operand */
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
break;

case 5:
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
        if (g_operand_hint == HINT_16BIT)
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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 6:
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
              
            if (v == SUCCEEDED)
              output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_instruction(g_instruction_tmp, "d%d Q%s ", g_instruction_tmp->hex, labelx);
            else
              output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, h);
            
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
  }
  if (g_instruction_tmp->op[x] != toupper((int)g_buffer[g_inz]))
    break;
 }
break;

case 7:
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
    if (z == SUCCEEDED && (g_parsed_int > 255 || g_parsed_int < -128))
      break;
    if (g_operand_hint == HINT_16BIT)
      break;
    
    for (x++ ; x < OP_SIZE_MAX; g_inz++, x++) {
      if (g_instruction_tmp->op[x] == 0 && g_buffer[g_inz] == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_instruction(g_instruction_tmp, "d%d d%d ", g_instruction_tmp->hex, g_parsed_int);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_instruction(g_instruction_tmp, "k%d d%d Q%s ", g_active_file_info_last->line_current, g_instruction_tmp->hex, g_label);
        else
          output_assembled_instruction(g_instruction_tmp, "d%d c%d ", g_instruction_tmp->hex, g_latest_stack);
        
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

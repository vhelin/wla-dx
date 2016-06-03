
case 0:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
    output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
    token_stack_pop_state(buffer_stack, 1);
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

case 11:
case 1:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (z == SUCCEEDED && (d > 127 || d < -128)) {
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d R%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else {
          output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, latest_stack);
          if (opt_tmp->type == 11) {
            /* 11 -> let's configure the stack so that all label references inside are relative */
            stacks_tmp->relative_references = 1;
          }
        }

        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

case 2:
if (xbit_size > 16 && opt_tmp->skip_xbit == 1)
  break;
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '?') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (operand_hint == HINT_24BIT && opt_tmp->skip_xbit == 1)
      break;
    if (z == SUCCEEDED && (d < -32768 || d > 65535)) {
      if (opt_tmp->skip_xbit == 1)
        break;
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d r%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else
          output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->hex, latest_stack);
        
        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

case 3:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '&') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_STACK || z == INPUT_NUMBER_ADDRESS_LABEL)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    
    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d z%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d q%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else
          output_assembled_opcode(opt_tmp, "d%d T%d ", opt_tmp->hex, latest_stack);

        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

/* IMMEDIATE VALUES: ACCUMULATOR */

case 4:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    /* select the mode */
    if (operand_hint == HINT_8BIT)
      y = 0;
    else if (operand_hint == HINT_16BIT)
      y = 1;
    else if (accu_size == 8)
      y = 0;
    else
      y = 1;
    
    if (y == 0) {
      if (z == SUCCEEDED && (d > 255 || d < -128)) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }
    else {
      if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
        print_error("Out of 16-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }
    
    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        /* 8BIT */
        if (y == 0) {
          if (z == SUCCEEDED)
            output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(opt_tmp, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, label);
          else
            output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, latest_stack);
        }
        /* 16BIT */
        else {
          if (z == SUCCEEDED)
            output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->hex, d);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(opt_tmp, "k%d d%d r%s ", active_file_info_last->line_current, opt_tmp->hex, label);
          else
            output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->hex, latest_stack);
        }
        
        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

/* MVN & MVP */

case 5:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;

    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);

    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 'x') {
        z = input_number();
        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
          token_stack_pop_state(buffer_stack, 0);
          token_stack_pop_state(buffer_stack, 0);
          return FAILED;
        }
        if (z == SUCCEEDED && (d > 255 || d < -128))
          break;
        
        token_stack_push_state(buffer_stack);
        for (x++; x < OP_SIZE_MAX; x++) {
          current_token = token_stack_get_current_token(buffer_stack);
          if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
            if (v == SUCCEEDED)
              output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(opt_tmp, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, labelx);
            else
              output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, h);
            
            if (z == SUCCEEDED)
              output_assembled_opcode(opt_tmp, "d%d ", d);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(opt_tmp, "k%d Q%s ", active_file_info_last->line_current, label);
            else
              output_assembled_opcode(opt_tmp, "c%d ", latest_stack);
            
            token_stack_pop_state(buffer_stack, 1);
            token_stack_pop_state(buffer_stack, 1);
            token_stack_pop_state(buffer_stack, 1);
            return SUCCEEDED;
          }
          if (opt_tmp->op[x] != toupper((int)current_token))
            break;

          token_stack_move(buffer_stack, 1);
        }
        token_stack_pop_state(buffer_stack, 0);
      }
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

/* SEP & REP */

case 6:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (z != SUCCEEDED) {
      if (opt_tmp->skip_xbit == 0)
        print_error("REP needs immediate data.\n", ERROR_LOG);
      else
        print_error("SEP needs immediate data.\n", ERROR_LOG);
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;
    
    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        
        output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
        
        /* REP */
        if (opt_tmp->skip_xbit == 0) {
          if (d & 16)
            index_size = 16;
          if (d & 32)
            accu_size = 16;
        }
        /* SEP */
        else {
          if (d & 16)
            index_size = 8;
          if (d & 32)
            accu_size = 8;
        }
        
        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

/* IMMEDIATE VALUES: INDEX */

case 7:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    /* select the mode */
    if (operand_hint == HINT_8BIT)
      y = 0;
    else if (operand_hint == HINT_16BIT)
      y = 1;
    else if (index_size == 8)
      y = 0;
    else
      y = 1;
    
    if (y == 0) {
      if (z == SUCCEEDED && (d > 255 || d < -128)) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }
    else {
      if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
        print_error("Out of 16-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }
    
    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        /* 8BIT */
        if (y == 0) {
          if (z == SUCCEEDED)
            output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(opt_tmp, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, label);
          else
            output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, latest_stack);
        }
        /* 16BIT */
        else {
          if (z == SUCCEEDED)
            output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->hex, d);
          else if (z == INPUT_NUMBER_ADDRESS_LABEL)
            output_assembled_opcode(opt_tmp, "k%d d%d r%s ", active_file_info_last->line_current, opt_tmp->hex, label);
          else
            output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->hex, latest_stack);
        }
        
        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

case 8:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
    output_assembled_opcode(opt_tmp, "y%d ", opt_tmp->hex);
    token_stack_pop_state(buffer_stack, 1);
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
 }
break;

case 9:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '?') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (z == SUCCEEDED && (d < -32768 || d > 65535))
      break;

    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d y%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d M%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else {
          output_assembled_opcode(opt_tmp, "d%d C%d ", opt_tmp->hex, latest_stack);
          /* let's configure the stack so that all label references inside are relative */
          stacks_tmp->relative_references = 1;
        }
        
        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

case 0xA:
if (xbit_size > 8 && opt_tmp->skip_xbit == 2)
  break;
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x') {
    z = input_number();
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if ((operand_hint == HINT_16BIT || operand_hint == HINT_24BIT) && opt_tmp->skip_xbit == 2)
      break;
    if (z == SUCCEEDED && (d > 255 || d < -128)) {
      if (opt_tmp->skip_xbit == 2)
        break;
      print_error("Out of 8-bit range.\n", ERROR_NUM);
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    
    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "d%d d%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else
          output_assembled_opcode(opt_tmp, "d%d c%d ", opt_tmp->hex, latest_stack);

        token_stack_pop_state(buffer_stack, 1);
        token_stack_pop_state(buffer_stack, 1);
        return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)current_token))
        break;

      token_stack_move(buffer_stack, 1);
    }
    token_stack_pop_state(buffer_stack, 0);
  }
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] != toupper((int)current_token))
    break;

  token_stack_move(buffer_stack, 1);
}
break;

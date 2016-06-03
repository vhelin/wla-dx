
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
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
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
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '?') {
    z = input_number();

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16-bit range.\n", ERROR_NUM);
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
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

case 4:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x' || opt_tmp->op[x] == 's') {
    z = input_number();

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    if (z == SUCCEEDED) {
      if ((opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }

    token_stack_push_state(buffer_stack);
    for (x++; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "y%d d%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d y%d R%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else
          output_assembled_opcode(opt_tmp, "y%d c%d ", opt_tmp->hex, latest_stack);

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

case 5:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x' || opt_tmp->op[x] == 's') {
    z = input_number();

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }
    
    if (z == SUCCEEDED) {
      if ((opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "y%d d%d d%d ", opt_tmp->hex, d, opt_tmp->hex_x);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d y%d R%s d%d ", active_file_info_last->line_current, opt_tmp->hex, label, opt_tmp->hex_x);
        else
          output_assembled_opcode(opt_tmp, "y%d c%d d%d ", opt_tmp->hex, latest_stack, opt_tmp->hex_x);
        
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

case 6:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '?') {
    z = input_number();

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        if (z == SUCCEEDED)
          output_assembled_opcode(opt_tmp, "y%d y%d ", opt_tmp->hex, d);
        else if (z == INPUT_NUMBER_ADDRESS_LABEL)
          output_assembled_opcode(opt_tmp, "k%d y%d r%s ", active_file_info_last->line_current, opt_tmp->hex, label);
        else
          output_assembled_opcode(opt_tmp, "y%d C%d ", opt_tmp->hex, latest_stack);
        
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

case 7:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == 'x' || opt_tmp->op[x] == 's') {
    z = input_number();

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
      token_stack_pop_state(buffer_stack, 0);
      return FAILED;
    }

    if (z == SUCCEEDED) {
      if ((opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
          (opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
        print_error("Out of 8-bit range.\n", ERROR_NUM);
        token_stack_pop_state(buffer_stack, 0);
        return FAILED;
      }
    }
    
    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
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
        for (x++ ; x < OP_SIZE_MAX; x++) {
          current_token = token_stack_get_current_token(buffer_stack);
          if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
            if (v == SUCCEEDED)
              output_assembled_opcode(opt_tmp, "y%d d%d ", opt_tmp->hex, e);
            else if (v == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(opt_tmp, "k%d y%d R%s ", active_file_info_last->line_current, opt_tmp->hex, labelx);
            else
              output_assembled_opcode(opt_tmp, "y%d c%d ", opt_tmp->hex, h);
        
            if (z == SUCCEEDED)
              output_assembled_opcode(opt_tmp, "d%d ", d);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(opt_tmp, "k%d R%s ", active_file_info_last->line_current, label);
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

case 8:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '*') {
    z = input_number();

    if (z != SUCCEEDED || d != opt_tmp->value)
      break;

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        output_assembled_opcode(opt_tmp, "d%d ", opt_tmp->hex);
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

case 9:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '*') {
    z = input_number();

    if (z != SUCCEEDED || d != opt_tmp->value)
      break;
    
    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
        output_assembled_opcode(opt_tmp, "y%d ", opt_tmp->hex);
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

case 10:
for ( ; x < OP_SIZE_MAX; x++) {
  current_token = token_stack_get_current_token(buffer_stack);
  if (opt_tmp->op[x] == '*') {
    z = input_number();

    if (z != SUCCEEDED || d != opt_tmp->value)
      break;

    token_stack_push_state(buffer_stack);
    for (x++ ; x < OP_SIZE_MAX; x++) {
      current_token = token_stack_get_current_token(buffer_stack);
      if (opt_tmp->op[x] == 'x' || opt_tmp->op[x] == 's') {
        z = input_number();

        if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK)) {
          token_stack_pop_state(buffer_stack, 0);
          token_stack_pop_state(buffer_stack, 0);
          return FAILED;
        }
        
        if (z == SUCCEEDED) {
          if ((opt_tmp->op[x] == 'x' && (d > 255 || d < -128)) ||
              (opt_tmp->op[x] == 's' && (d > 127 || d < -128))) {
            print_error("Out of 8-bit range.\n", ERROR_NUM);
            token_stack_pop_state(buffer_stack, 0);
            token_stack_pop_state(buffer_stack, 0);
            return FAILED;
          }
        }
        
        token_stack_push_state(buffer_stack);
        for (x++ ; x < OP_SIZE_MAX; x++) {
          current_token = token_stack_get_current_token(buffer_stack);
          if (opt_tmp->op[x] == 0 && current_token == 0x0A) {
            if (z == SUCCEEDED)
              output_assembled_opcode(opt_tmp, "y%d d%d d%d ", opt_tmp->hex, d, opt_tmp->hex_x);
            else if (z == INPUT_NUMBER_ADDRESS_LABEL)
              output_assembled_opcode(opt_tmp, "k%d y%d R%s d%d ", active_file_info_last->line_current, opt_tmp->hex, label, opt_tmp->hex_x);
            else
              output_assembled_opcode(opt_tmp, "y%d c%d d%d ", opt_tmp->hex, latest_stack, opt_tmp->hex_x);
            
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

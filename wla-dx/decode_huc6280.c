
case 0:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
    fprintf(file_out_ptr, "d%d ", opt_tmp->hex);
    i = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 9:
case 1:
if (xbit_size > 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;
    if (operand_hint == HINT_16BIT)
      break;

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	if (z == SUCCEEDED)
	  fprintf(file_out_ptr, "d%d d%d ", opt_tmp->hex, d);
	else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	  fprintf(file_out_ptr, "k%d d%d R%s ", active_file_info_last->line_current, opt_tmp->hex, label);
	else {
	  fprintf(file_out_ptr, "d%d c%d ", opt_tmp->hex, latest_stack);
	  if (opt_tmp->type == 9) {
	    /* 9 -> let's configure the stack so that all label references inside are relative */
	    stacks_tmp->relative_references = 1;
	  }
	}

	i = inz;
	return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 2:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16bit range.\n", ERROR_NUM);
      return FAILED;
    }

    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	if (z == SUCCEEDED)
	  fprintf(file_out_ptr, "d%d y%d ", opt_tmp->hex, d);
	else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	  fprintf(file_out_ptr, "k%d d%d r%s ", active_file_info_last->line_current, opt_tmp->hex, label);
	else
	  fprintf(file_out_ptr, "d%d C%d ", opt_tmp->hex, latest_stack);

	i = inz;
	return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 3:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
    fprintf(file_out_ptr, "y%d ", opt_tmp->hex);
    i = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 4:
s = 0;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16bit range.\n", ERROR_NUM);
      return FAILED;
    }
    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);
    s = 1;
    break;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
if (s == 0)
  break;

for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16bit range.\n", ERROR_NUM);
      return FAILED;
    }
    r = d;
    t = z;
    u = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labely, label);
    s = 2;
    break;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
if (s == 1)
  break;

for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == '?') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
      print_error("Out of 16bit range.\n", ERROR_NUM);
      return FAILED;
    }
    s = 3;
    break;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
if (s == 2)
  break;

for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {

    fprintf(file_out_ptr, "d%d ", opt_tmp->hex);

    /* first operand */
    if (v == SUCCEEDED)
      fprintf(file_out_ptr, "y%d ", e);
    else if (v == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, labelx);
    else
      fprintf(file_out_ptr, "C%d ", h);

    /* second operand */
    if (t == SUCCEEDED)
      fprintf(file_out_ptr, "y%d ", r);
    else if (t == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, labely);
    else
      fprintf(file_out_ptr, "C%d ", u);

    /* third operand */
    if (z == SUCCEEDED)
      fprintf(file_out_ptr, "y%d ", d);
    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
      fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, label);
    else
      fprintf(file_out_ptr, "C%d ", latest_stack);
    
    i = inz;
    return SUCCEEDED;
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 5:
if (xbit_size > 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;

    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);
    
    for (x++; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 'x') {
	y = i;
	i = inz;
	z = input_number();
	inz = i;
	i = y;
	if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
	  return FAILED;
	if (z == SUCCEEDED && (d > 255 || d < -128))
	  break;
	if (operand_hint == HINT_16BIT)
	  break;
	
	for (x++; x < OP_SIZE_MAX; inz++, x++) {
	  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	    if (v == SUCCEEDED)
	      fprintf(file_out_ptr, "d%d d%d ", opt_tmp->hex, e);
	    else if (v == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, labelx);
	    else
	      fprintf(file_out_ptr, "d%d c%d ", opt_tmp->hex, h);
	    
	    if (z == SUCCEEDED)
	      fprintf(file_out_ptr, "d%d ", d);
	    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d Q%s ", active_file_info_last->line_current, label);
	    else
	      fprintf(file_out_ptr, "c%d ", latest_stack);
	    
	    i = inz;
	    return SUCCEEDED;
	  }
	  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	    break;
	}
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 6:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;
    
    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);

    for (x++; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == '?') {
	y = i;
	i = inz;
	z = input_number();
	inz = i;
	i = y;
	if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
	  return FAILED;
	if (z == SUCCEEDED && (d > 65535 || d < -32768)) {
	  print_error("Out of 16bit range.\n", ERROR_NUM);
	  return FAILED;
	}

	for (x++; x < OP_SIZE_MAX; inz++, x++) {
	  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	    
	    if (v == SUCCEEDED)
	      fprintf(file_out_ptr, "d%d d%d ", opt_tmp->hex, e);
	    else if (v == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, labelx);
	    else
	      fprintf(file_out_ptr, "d%d c%d ", opt_tmp->hex, h);
	    
	    if (z == SUCCEEDED)
	      fprintf(file_out_ptr, "y%d ", d);
	    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d r%s ", active_file_info_last->line_current, label);
	    else
	      fprintf(file_out_ptr, "C%d ", latest_stack);
	    
	    i = inz;
	    return SUCCEEDED;
	  }
	  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	    break;
	}
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 7:
if (xbit_size > 8 && opt_tmp->skip_8bit == 1)
  break;
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;

    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;
    if (operand_hint == HINT_16BIT)
      break;
    
    for (x++ ; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	if (z == SUCCEEDED)
	  fprintf(file_out_ptr, "d%d d%d ", opt_tmp->hex, d);
	else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	  fprintf(file_out_ptr, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, label);
	else
	  fprintf(file_out_ptr, "d%d c%d ", opt_tmp->hex, latest_stack);
	
	i = inz;
	return SUCCEEDED;
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

case 8:
for ( ; x < OP_SIZE_MAX; inz++, x++) {
  if (opt_tmp->op[x] == 'x') {
    y = i;
    i = inz;
    z = input_number();
    inz = i;
    i = y;
    if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
      return FAILED;
    if (z == SUCCEEDED && (d > 255 || d < -128))
      break;

    e = d;
    v = z;
    h = latest_stack;
    if (z == INPUT_NUMBER_ADDRESS_LABEL)
      strcpy(labelx, label);

    for (x++; x < OP_SIZE_MAX; inz++, x++) {
      if (opt_tmp->op[x] == 'x') {
	y = i;
	i = inz;
	z = input_number();
	inz = i;
	i = y;
	if (!(z == SUCCEEDED || z == INPUT_NUMBER_ADDRESS_LABEL || z == INPUT_NUMBER_STACK))
	  return FAILED;
	if (z == SUCCEEDED && (d > 255 || d < -128))
	  break;
	
	for (x++; x < OP_SIZE_MAX; inz++, x++) {
	  if (opt_tmp->op[x] == 0 && buffer[inz] == 0x0A) {
	    if (v == SUCCEEDED)
	      fprintf(file_out_ptr, "d%d d%d ", opt_tmp->hex, e);
	    else if (v == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d d%d Q%s ", active_file_info_last->line_current, opt_tmp->hex, labelx);
	    else
	      fprintf(file_out_ptr, "d%d c%d ", opt_tmp->hex, h);
	    
	    if (z == SUCCEEDED)
	      fprintf(file_out_ptr, "d%d ", d);
	    else if (z == INPUT_NUMBER_ADDRESS_LABEL)
	      fprintf(file_out_ptr, "k%d R%s ", active_file_info_last->line_current, label);
	    else {
	      fprintf(file_out_ptr, "c%d ", latest_stack);
	      /* let's configure the stack so that all label references inside are relative */
	      stacks_tmp->relative_references = 1;
	    }
	    
	    i = inz;
	    return SUCCEEDED;
	  }
	  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	    break;
	}
      }
      if (opt_tmp->op[x] != toupper((int)buffer[inz]))
	break;
    }
  }
  if (opt_tmp->op[x] != toupper((int)buffer[inz]))
    break;
 }
break;

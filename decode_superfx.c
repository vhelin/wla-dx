
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





#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "parse.h"
#include "files.h"
#include "analyze.h"



extern struct object_file *obj_first, *obj_last, *obj_tmp;
extern struct label *labels_first, *labels_last;
extern unsigned char *file_header, *file_footer;
extern char ext_libdir[MAX_NAME_LENGTH + 1];
extern int file_header_size, file_footer_size, use_libdir;

struct section_fix *sec_fix_first = NULL, *sec_fix_tmp;
char file_name_error[] = "???";



int load_files(char *argv[], int argc) {

  int state = STATE_NONE, i, x, line, bank, slot, base, bank_defined, slot_defined, base_defined, n;
  char tmp[1024], token[1024], tmp_token[1024 + MAX_NAME_LENGTH + 2];
  struct label *l;
  FILE *fop, *f;

  
  fop = fopen(argv[argc - 2], "rb");
  if (fop == NULL) {
    fprintf(stderr, "LOAD_FILES: Could not open file \"%s\".\n", argv[argc - 2]);
    return FAILED;
  }

  line = 0;
  while (fgets(tmp, 255, fop) != NULL) {
    line++;
    x = 0;

    if (tmp[0] == ';' || tmp[0] == '*' || tmp[0] == '#' || tmp[0] == 0x0D || tmp[0] == 0x0A)
      continue;

    /* remove garbage from the end */
    for (i = 0; !(tmp[i] == 0x0D || tmp[i] == 0x0A || tmp[i] == 0x00); i++)
      ;
    tmp[i] = 0;

    /* empty line check */
    if (get_next_token(tmp, token, &x) == FAILED)
      continue;

    /* first checks */
    if (token[0] == '[') {
      if (strcmp("[objects]", token) == 0) {
	state = STATE_OBJECT;
	continue;
      }
      else if (strcmp("[libraries]", token) == 0) {
	state = STATE_LIBRARY;
	continue;
      }
      else if (strcmp("[header]", token) == 0) {
	state = STATE_HEADER;
	continue;
      }
      else if (strcmp("[footer]", token) == 0) {
	state = STATE_FOOTER;
	continue;
      }
      else if (strcmp("[definitions]", token) == 0) {
	state = STATE_DEFINITION;
	continue;
      }
      else if (strcmp("[ramsections]", token) == 0) {
	state = STATE_RAMSECTIONS;
	continue;
      }
      else {
	fprintf(stderr, "%s:%d LOAD_FILES: Unknown group \"%s\".\n", argv[argc - 2], line, token);
	fclose(fop);
	return FAILED;
      }
    }

    if (state == STATE_NONE) {
      fprintf(stderr, "%s:%d: LOAD_FILES: Before file \"%s\" can be loaded you must define a group for it.\n", argv[argc - 2], line, token);
      fclose(fop);
      return FAILED;
    }

    bank_defined = NO;
    slot_defined = NO;
    base_defined = NO;
    bank = 0;
    slot = 0;
    base = 0;

    /* definitions? */
    if (state == STATE_DEFINITION) {
      l = calloc(1, sizeof(struct label));
      if (l == NULL) {
	fprintf(stderr, "LOAD_FILES: Out of memory.\n");
	return FAILED;
      }
      strcpy(l->name, token);
      l->status = LABEL_STATUS_DEFINE;
      l->bank = 0;
      l->slot = 0;
      l->base = 0;
      l->alive = YES;

      if (get_next_number(&tmp[x], &n, &x) == FAILED) {
        fprintf(stderr, "%s:%d: LOAD_FILES: Error in DEFINITION value.\n", argv[argc - 2], line);
        fclose(fop);
        free(l);
        return FAILED;
      }

      l->address = n;
      add_label(l);
      continue;
    }
    /* header loading? */
    else if (state == STATE_HEADER) {
      if (file_header != NULL) {
	fprintf(stderr, "%s:%d: LOAD_FILES: There can be only one header file.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }

      if (load_file_data(token, &file_header, &file_header_size) == FAILED) {
	fclose(fop);
	return FAILED;
      }
      if (get_next_token(&tmp[x], token, &x) == FAILED)
	continue;

      fprintf(stderr, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
      fclose(fop);
      return FAILED;
    }
    /* footer loading? */
    else if (state == STATE_FOOTER) {
      if (file_footer != NULL) {
	fprintf(stderr, "%s:%d: LOAD_FILES: There can be only one footer file.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }

      if (load_file_data(token, &file_footer, &file_footer_size) == FAILED) {
	fclose(fop);
	return FAILED;
      }
      if (get_next_token(&tmp[x], token, &x) == FAILED)
	continue;

      fprintf(stderr, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
      fclose(fop);
      return FAILED;
    }
    /* ramsection settings? */
    else if (state == STATE_RAMSECTIONS) {
      i = SUCCEEDED;
      while (i == SUCCEEDED) {
	if (strcmp(token, "bank") == 0 || strcmp(token, "BANK") == 0) {
	  if (bank_defined == YES) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: BANK defined for the second time for a RAM section.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	  bank_defined = YES;
	  
	  if (get_next_number(&tmp[x], &bank, &x) == FAILED) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: Error in BANK number.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	}
	else if (strcmp(token, "slot") == 0 || strcmp(token, "SLOT") == 0) {
	  if (slot_defined == YES) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: SLOT defined for the second time for a RAM section.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	  slot_defined = YES;
	  
	  if (get_next_number(&tmp[x], &slot, &x) == FAILED) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: Error in SLOT number.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	}
	else
	  break;
	
	i = get_next_token(&tmp[x], token, &x);
      }
      
      if (i == FAILED) {
	fprintf(stderr, "%s:%d: LOAD_FILES: There is something wrong in a RAM section's settings.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      if (slot_defined == NO) {
	fprintf(stderr, "%s:%d: LOAD_FILES: RAM section requires a SLOT.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      if (bank_defined == NO) {
	fprintf(stderr, "%s:%d: LOAD_FILES: RAM sections requires a BANK.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }

      /* add a new entry */
      sec_fix_tmp = calloc(sizeof(struct section_fix), 1);
      if (sec_fix_tmp == NULL) {
	fprintf(stderr, "%s:%d: LOAD_FILES: Out of memory error.\n", argv[argc - 2], line);
	return FAILED;	
      }

      strcpy(sec_fix_tmp->name, token);
      strcpy(sec_fix_tmp->file_name, argv[argc - 2]);
      sec_fix_tmp->line_number = line;
      sec_fix_tmp->bank = bank;
      sec_fix_tmp->slot = slot;

      sec_fix_tmp->next = sec_fix_first;
      sec_fix_first = sec_fix_tmp;

      continue;
    }
    /* library loading? */
    else if (state == STATE_LIBRARY) {
      i = SUCCEEDED;
      while (i == SUCCEEDED) {
	if (strcmp(token, "bank") == 0 || strcmp(token, "BANK") == 0) {
	  if (bank_defined == YES) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: BANK defined for the second time for a library file.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	  bank_defined = YES;
	  
	  if (get_next_number(&tmp[x], &bank, &x) == FAILED) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: Error in BANK number.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	}
	else if (strcmp(token, "slot") == 0 || strcmp(token, "SLOT") == 0) {
	  if (slot_defined == YES) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: SLOT defined for the second time for a library file.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	  slot_defined = YES;
	  
	  if (get_next_number(&tmp[x], &slot, &x) == FAILED) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: Error in SLOT number.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	}
	else if (strcmp(token, "base") == 0 || strcmp(token, "BASE") == 0) {
	  if (base_defined == YES) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: BASE defined for the second time for a library file.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	  base_defined = YES;
	  
	  if (get_next_number(&tmp[x], &base, &x) == FAILED) {
	    fprintf(stderr, "%s:%d: LOAD_FILES: Error in BASE number.\n", argv[argc - 2], line);
	    fclose(fop);
	    return FAILED;
	  }
	}
	else
	  break;
	
	i = get_next_token(&tmp[x], token, &x);
      }
      
      if (i == FAILED) {
	fprintf(stderr, "%s:%d: LOAD_FILES: No library to load.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      if (slot_defined == NO) {
	fprintf(stderr, "%s:%d: LOAD_FILES: Library file requires a SLOT.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      if (bank_defined == NO) {
	fprintf(stderr, "%s:%d: LOAD_FILES: Library file requires a BANK.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      
      if (use_libdir == YES) {
        f = fopen(token, "rb");
      
        /* use the current working directory if the library isn't found in the ext_libdir directory */
        if (f == NULL)
          sprintf(tmp_token, "%s%s", ext_libdir, token);
        else {
          sprintf(tmp_token, "%s", token);
	  fclose(f);
	}
      }
      else
        sprintf(tmp_token, "%s", token);
      
      if (load_file(tmp_token, bank, slot, base, base_defined) == FAILED) {
	fclose(fop);
	return FAILED;
      }
      
      if (get_next_token(&tmp[x], token, &x) == SUCCEEDED) {
	fprintf(stderr, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
	fclose(fop);
	return FAILED;
      }
      
      continue;
    }
    /* object file loading */
    else if (load_file(token, 0, 0, 0, OFF) == FAILED) {
      fclose(fop);
      return FAILED;
    }
    if (get_next_token(&tmp[x], token, &x) == FAILED)
      continue;

    fprintf(stderr, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
    fclose(fop);
    return FAILED;
  }

  fclose(fop);

  return SUCCEEDED;
}


int load_file(char *file_name, int bank, int slot, int base, int base_defined) {

  struct object_file *o;
  unsigned char *data;
  static int id = 0;
  char *name;
  int size;

  
  o = calloc(sizeof(struct object_file), 1);
  name = calloc(strlen(file_name)+1, 1);
  if (o == NULL || name == NULL) {
    if (o != NULL)
      free(o);
    if (name != NULL)
      free(name);
    fprintf(stderr, "LOAD_FILE: Out of memory.\n");
    return FAILED;
  }

  if (load_file_data(file_name, &data, &size) == FAILED) {
    free(name);
    free(o);
    return FAILED;
  }

  /* only valid for library files */
  o->bank = bank;
  o->slot = slot;
  o->base = base;
  o->base_defined = base_defined;

  /* init the rest of the variables */
  o->source_file_names = NULL;
  o->memorymap = NULL;
  o->exported_defines = NULL;
  o->data_blocks = NULL;
  o->source_file_names_list = NULL;

  if (obj_first == NULL) {
    obj_first = o;
    obj_last = o;
  }
  else {
    obj_last->next = o;
    obj_last = o;
  }

  o->next = NULL;
  o->size = size;
  o->data = data;

  strcpy(name, file_name);
  o->name = name;
  o->id = id++;

  return SUCCEEDED;
}


int load_file_data(char *file_name, unsigned char **data, int *size) {

  FILE *fop;


  fop = fopen(file_name, "rb");
  if (fop == NULL) {
    fprintf(stderr, "LOAD_FILE_DATA: Could not open file \"%s\".\n", file_name);
    return FAILED;
  }

  fseek(fop, 0, SEEK_END);
  *size = (int)ftell(fop);
  fseek(fop, 0, SEEK_SET);

  *data = calloc(*size, 1);
  if (*data == NULL) {
    fclose(fop);
    return FAILED;
  }

  fread(*data, 1, *size, fop);
  fclose(fop);

  return SUCCEEDED;
}


char *get_file_name(int id) {

  static char error[] = "GET_FILE_NAME: Internal data corruption.";
  struct object_file *o;

  
  o = obj_first;
  while (o != NULL) {
    if (o->id == id)
      return o->name;
    o = o->next;
  }

  return error;
}


char *get_source_file_name(int file_id, int source_id) {

  struct source_file_name *s;
  struct object_file *o;

  
  o = obj_first;
  while (o != NULL) {
    if (o->id == file_id)
      break;
    o = o->next;
  }

  if (o == NULL)
    return file_name_error;

  s = o->source_file_names_list;
  while (s != NULL) {
    if (s->id == source_id)
      break;
    s = s->next;
  }

  if (s == NULL)
    return file_name_error;

  return s->name;
}


struct object_file *get_file(int file_id) {

  struct object_file *o;

  
  o = obj_first;
  while (o != NULL) {
    if (o->id == file_id)
      return o;
    o = o->next;
  }

  fprintf(stderr, "GET_FILE: Internal error, file %d is missing!\n", file_id);

  return NULL;
}

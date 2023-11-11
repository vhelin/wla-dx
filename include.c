
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crc32.h"
#include "defines.h"

#include "include.h"
#include "phase_1.h"
#include "parse.h"
#include "printf.h"
#include "main.h"


extern int g_source_index, g_extra_definitions, g_parsed_int, g_use_incdir, g_makefile_rules, g_makefile_add_phony_targets;
extern FILE* g_makefile_rule_file;
extern char *g_tmp, g_label[MAX_NAME_LENGTH + 1];
extern struct ext_include_collection g_ext_incdirs;
extern FILE *g_file_out_ptr;
extern struct stringmaptable *g_stringmaptables;
extern struct string *g_fopen_filenames_first;
extern int g_is_file_isolated_counter;

extern int create_tmp_file(FILE **);
        
struct incbin_file_data *g_incbin_file_data_first = NULL, *g_ifd_tmp;
struct active_file_info *g_active_file_info_first = NULL, *g_active_file_info_last = NULL, *g_active_file_info_tmp = NULL;
struct file_name_info *g_file_name_info_first = NULL, *g_file_name_info_last = NULL, *g_file_name_info_tmp;
char *g_include_in_tmp = NULL, *g_tmp_a = NULL, *g_full_name = NULL, *g_include_dir = NULL, *g_buffer = NULL;
int g_include_in_tmp_size = 0, g_tmp_a_size = 0, g_open_files = 0;
int g_include_dir_size = 0, g_source_file_size = 0;

static int s_file_name_id = 1, s_full_name_size = 0;


int create_full_name(char *dir, char *name) {

  char *tmp;
  int i;
  
  if (dir == NULL && name == NULL)
    return SUCCEEDED;

  /* compute the length of the new string */
  i = 0;
  if (dir != NULL)
    i += (int)strlen(dir);
  if (name != NULL)
    i += (int)strlen(name);
  i++;

  if (i > s_full_name_size) {
    tmp = realloc(g_full_name, i);
    if (tmp == NULL) {
      print_text(NO, "CREATE_FULL_NAME: Out of memory error.\n");
      return FAILED;
    }
    g_full_name = tmp;
    s_full_name_size = i;
  }

  if (dir != NULL) {
    strcpy(g_full_name, dir);
    if (name != NULL)
      strcat(g_full_name, name);
  }
  else
    strcpy(g_full_name, name);

  return SUCCEEDED;
}


static int _try_open_file(char* directory, char* partial_name, FILE** out_result) {

  int error_code = create_full_name(directory, partial_name);
  if (error_code != SUCCEEDED)
    return error_code;
  
  *out_result = fopen(g_full_name, "rb");
  if (*out_result == NULL)
    return FAILED;

  return SUCCEEDED;
}


static void _print_find_error(char* name) {

  int index;

  if (g_active_file_info_last)
    print_text(NO, "%s:%d: ", get_file_name(g_active_file_info_last->filename_id), g_active_file_info_last->line_current);

  print_text(NO, "FIND_FILE: Could not open \"%s\", searched in the following directories:\n", name);

  if (g_use_incdir == YES) {
    for (index = 0; index < g_ext_incdirs.count; index++) {
      print_text(NO, "%s\n", g_ext_incdirs.names[index]);
    }
  }

  if (g_include_dir != NULL) {
    print_text(NO, "%s\n", g_include_dir);
  }

  print_text(NO, "./ (current directory)\n");
}


static int _find_file(char *name, FILE **f) {

  int index;

  if (g_use_incdir == YES) {
    /* check all external include directories first */
    for (index = 0; index < g_ext_incdirs.count; index++) {
      _try_open_file(g_ext_incdirs.names[index], name, f);

      /* we succeeded and found a valid file, so escape */
      if (*f != NULL)
        return SUCCEEDED;
    }
  }

  /* fall through to g_include_dir if we either didn't check, or failed to find the file in g_ext_incdirs */
  _try_open_file(g_include_dir, name, f);
  if (*f != NULL)
    return SUCCEEDED;

  /* if failed try to find the file in the current directory */
  (*f) = fopen(name, "rb");
  if (*f != NULL)
    return SUCCEEDED;

  /* if we can't find the file, but are only printing makefile rules, silently use an empty file */
  /* a warning might be nice */
  if (g_makefile_rules == YES) {
    create_tmp_file(f);
    if (*f == NULL) {
      print_error(ERROR_INC, "Error creating a tmp file for \"%s\"!\n", name);
      return FAILED;
    }

    return SUCCEEDED;
  }
  
  _print_find_error(name);

  return FAILED;
}


int include_file(char *name, int *include_size, char *namespace) {

  int file_size, id, change_file_buffer_size, size, isolation_counter;
  char *tmp_b, *n, change_file_buffer[MAX_NAME_LENGTH * 2];
  FILE *f = NULL;

  int error_code = _find_file(name, &f);
  if (error_code != SUCCEEDED)
    return error_code;

  fseek(f, 0, SEEK_END);
  file_size = (int)ftell(f);
  fseek(f, 0, SEEK_SET);

  /* name */
  g_file_name_info_tmp = g_file_name_info_first;
  id = 0;

  /* NOTE: every filename, even included multiple times, is unique
     while (g_file_name_info_tmp != NULL) {
     if (strcmp(g_file_name_info_tmp->name, g_full_name) == 0) {
     id = g_file_name_info_tmp->id;
     break;
     }
     g_file_name_info_tmp = g_file_name_info_tmp->next;
     }
  */

  if (id == 0) {
    g_file_name_info_tmp = calloc(sizeof(struct file_name_info), 1);
    n = calloc(strlen(g_full_name)+1, 1);
    if (g_file_name_info_tmp == NULL || n == NULL) {
      if (g_file_name_info_tmp != NULL)
        free(g_file_name_info_tmp);
      if (n != NULL)
        free(n);
      print_error(ERROR_INC, "Out of memory while trying allocate info structure for file \"%s\".\n", g_full_name);
      return FAILED;
    }
    g_file_name_info_tmp->next = NULL;

    if (g_file_name_info_first == NULL) {
      g_file_name_info_first = g_file_name_info_tmp;
      g_file_name_info_last = g_file_name_info_tmp;
    }
    else {
      g_file_name_info_last->next = g_file_name_info_tmp;
      g_file_name_info_last = g_file_name_info_tmp;
    }

    strcpy(n, g_full_name);
    g_file_name_info_tmp->name = n;
    g_file_name_info_tmp->id = s_file_name_id;
    id = s_file_name_id;
    s_file_name_id++;
  }

  isolation_counter = g_is_file_isolated_counter;
  if (isolation_counter > 0)
    isolation_counter++;

  if (namespace == NULL || namespace[0] == 0)
    snprintf(change_file_buffer, sizeof(change_file_buffer), "%c.CHANGEFILE %d NONAMESPACE %d%c", 0xA, id, isolation_counter, 0xA);
  else
    snprintf(change_file_buffer, sizeof(change_file_buffer), "%c.CHANGEFILE %d NAMESPACE %s %d%c", 0xA, id, namespace, isolation_counter, 0xA);
  change_file_buffer_size = (int)strlen(change_file_buffer);

  /* reallocate buffer */
  if (g_include_in_tmp_size < file_size) {
    if (g_include_in_tmp != NULL)
      free(g_include_in_tmp);

    g_include_in_tmp = calloc(sizeof(char) * file_size, 1);
    if (g_include_in_tmp == NULL) {
      print_error(ERROR_INC, "Out of memory while trying to allocate room for \"%s\".\n", g_full_name);
      return FAILED;
    }

    g_include_in_tmp_size = file_size;
  }

  /* read the whole file into a buffer */
  if (fread(g_include_in_tmp, 1, file_size, f) != (size_t) file_size) {
    print_error(ERROR_INC, "Could not read all %d bytes of \"%s\"!", file_size, g_full_name);
    return FAILED;
  }

  fclose(f);

  /* calculate checksum for post-compilation file verification */
  g_file_name_info_tmp->checksum = crc32((unsigned char*)g_include_in_tmp, file_size);

  if (g_source_file_size == 0) {
    g_buffer = calloc(sizeof(char) * (change_file_buffer_size + (file_size + 4)), 1);
    if (g_buffer == NULL) {
      print_error(ERROR_INC, "Out of memory while trying to allocate room for \"%s\".\n", g_full_name);
      return FAILED;
    }

    memcpy(g_buffer, change_file_buffer, change_file_buffer_size);
    
    /* preprocess */
    preprocess_file(g_include_in_tmp, g_include_in_tmp + file_size, &g_buffer[change_file_buffer_size], &g_source_file_size, g_full_name);
    g_source_file_size += change_file_buffer_size;

    g_buffer[g_source_file_size++] = 0xA;
    g_buffer[g_source_file_size++] = '.';
    g_buffer[g_source_file_size++] = 'E';
    g_buffer[g_source_file_size++] = ' ';

    *include_size = g_source_file_size;

    return SUCCEEDED;
  }

  tmp_b = calloc(sizeof(char) * (g_source_file_size + change_file_buffer_size + file_size + 4), 1);
  if (tmp_b == NULL) {
    print_error(ERROR_INC, "Out of memory while trying to expand the project to incorporate file \"%s\".\n", g_full_name);
    return FAILED;
  }

  /* reallocate g_tmp_a */
  if (g_tmp_a_size < change_file_buffer_size + file_size + 4) {
    if (g_tmp_a != NULL)
      free(g_tmp_a);

    g_tmp_a = calloc(sizeof(char) * (change_file_buffer_size + file_size + 4), 1);
    if (g_tmp_a == NULL) {
      print_error(ERROR_INC, "Out of memory while allocating new room for \"%s\".\n", g_full_name);
      free(tmp_b);
      return FAILED;
    }

    g_tmp_a_size = change_file_buffer_size + file_size + 4;
  }

  memcpy(g_tmp_a, change_file_buffer, change_file_buffer_size);
      
  /* preprocess */
  size = 0;
  preprocess_file(g_include_in_tmp, g_include_in_tmp + file_size, &g_tmp_a[change_file_buffer_size], &size, g_full_name);
  size += change_file_buffer_size;

  g_tmp_a[size++] = 0xA;
  g_tmp_a[size++] = '.';
  g_tmp_a[size++] = 'E';
  g_tmp_a[size++] = ' ';

  memcpy(tmp_b, g_buffer, g_source_index);
  memcpy(tmp_b + g_source_index, g_tmp_a, size);
  memcpy(tmp_b + g_source_index + size, g_buffer + g_source_index, g_source_file_size - g_source_index);

  free(g_buffer);

  g_source_file_size += size;
  g_buffer = tmp_b;

  *include_size = size;
  
  return SUCCEEDED;
}


int incbin_file(char *name, int *id, int *swap, int *skip, int *read, struct macro_static **macro, int *filter_size) {

  struct incbin_file_data *ifd;
  char *in_tmp, *n, freadsize_label[MAX_NAME_LENGTH + 1];
  int file_size = 0, q, error_code;
  FILE *f = NULL;

  error_code = _find_file(name, &f);
  if (error_code != SUCCEEDED)
    return error_code;

  /* try to find the file in our cache */
  ifd = g_incbin_file_data_first;
  q = 0;
  while (ifd != NULL) {
    if (strcmp(g_full_name, ifd->name) == 0) {
      /* found it! */
      file_size = ifd->size;
      fclose(f);
      break;
    }
    else {
      q++;
      ifd = ifd->next;
    }
  }

  if (ifd == NULL) {
    struct incbin_file_data *ifd2;
    
    /* add the file to our cache */
    fseek(f, 0, SEEK_END);
    file_size = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    ifd = (struct incbin_file_data *)calloc(sizeof(struct incbin_file_data), 1);
    n = calloc(sizeof(char) * (strlen(g_full_name)+1), 1);
    in_tmp = (char *)calloc(sizeof(char) * file_size, 1);
    if (ifd == NULL || n == NULL || in_tmp == NULL) {
      free(ifd);
      free(n);
      free(in_tmp);
      print_error(ERROR_INB, "Out of memory while allocating data structure for \"%s\".\n", g_full_name);
      fclose(f);
      return FAILED;
    }

    /* read the whole file into a buffer */
    if (fread(in_tmp, 1, file_size, f) != (size_t) file_size) {
      free(ifd);
      free(n);
      free(in_tmp);
      print_error(ERROR_INC, "Could not read all %d bytes of \"%s\"!", file_size, g_full_name);
      return FAILED;
    }

    fclose(f);

    /* complete the structure */
    ifd->next = NULL;
    ifd->size = file_size;
    strcpy(n, g_full_name);
    ifd->name = n;
    ifd->data = in_tmp;

    /* calculate the index, remember the entry */
    q = 0;
    ifd2 = g_incbin_file_data_first;
    while (ifd2 != NULL) {
      q++;
      if (ifd2->next == NULL) {
        ifd2->next = ifd;
        break;
      }
      else
        ifd2 = ifd2->next;
    }
    
    if (g_incbin_file_data_first == NULL)
      g_incbin_file_data_first = ifd;
  }

  *id = q;
  *skip = 0;
  *read = 0;
  *swap = 0;
  *macro = NULL;
  *filter_size = 1;
  freadsize_label[0] = 0;
  
  while (1) {
    /* SKIP bytes? */
    if (compare_next_token("SKIP") == SUCCEEDED) {
      skip_next_token();
      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, ".INCBIN needs the amount of skipped bytes.\n");
        return FAILED;
      }

      *skip = g_parsed_int;

      if (g_parsed_int >= file_size && g_makefile_rules == NO) {
        print_error(ERROR_INB, "SKIP value (%d) is more than the size (%d) of file \"%s\".\n", g_parsed_int, file_size, g_full_name);
        return FAILED;
      }
    }
    /* READ bytes? */
    else if (compare_next_token("READ") == SUCCEEDED) {
      skip_next_token();
      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, ".INCBIN needs the amount of bytes for reading.\n");
        return FAILED;
      }

      if (g_parsed_int == 0) {
        print_error(ERROR_DIR, "READ must be positive or negative, 0 doesn't make sense.\n");
        return FAILED;
      }

      *read = g_parsed_int;
    }
    /* FILTERSIZE bytes? */
    else if (compare_next_token("FILTERSIZE") == SUCCEEDED) {
      skip_next_token();
      if (input_number() != SUCCEEDED) {
        print_error(ERROR_DIR, ".INCBIN needs the amount of bytes for FILTERSIZE.\n");
        return FAILED;
      }

      if (g_parsed_int < 1 || g_parsed_int > 4) {
        print_error(ERROR_DIR, "FILTERSIZE must be 1, 2, 3 or 4.\n");
        return FAILED;
      }

      *filter_size = g_parsed_int;
    }
    /* SWAP bytes? */
    else if (compare_next_token("SWAP") == SUCCEEDED) {
      skip_next_token();
      *swap = 1;
    }
    /* FREADSIZE? */
    else if (compare_next_token("FREADSIZE") == SUCCEEDED) {
      skip_next_token();

      /* get the definition label */
      if (get_next_plain_string() == FAILED)
        return FAILED;

      strcpy(freadsize_label, g_label);
    }
    /* FSIZE? */
    else if (compare_next_token("FSIZE") == SUCCEEDED) {
      skip_next_token();

      /* get the definition label */
      if (get_next_plain_string() == FAILED)
        return FAILED;

      add_a_new_definition(g_label, (double)file_size, NULL, DEFINITION_TYPE_VALUE, 0);
    }
    /* FILTER? */
    else if (compare_next_token("FILTER") == SUCCEEDED) {
      skip_next_token();

      /* get the filter macro name */
      if (get_next_plain_string() == FAILED)
        return FAILED;

      if (g_is_file_isolated_counter <= 0) {
        if (macro_get(g_label, YES, macro) == FAILED)
          return FAILED;
      }
      else {
        if (macro_get(g_label, NO, macro) == FAILED)
          return FAILED;
      }
      
      if (*macro == NULL) {
        print_error(ERROR_INB, "No MACRO \"%s\" defined.\n", g_label);
        return FAILED;
      }
    }
    else
      break;
  }

  if (g_makefile_rules == YES) {
    /* if in test mode, fake the data to be enough to read */
    if (*read <= 0)
      file_size = *skip;
    else
      file_size = *read + *skip;
    free(ifd->data);
    ifd->data = calloc(sizeof(char) * file_size, 1);
    ifd->size = file_size;
  }

  /* negative READ? */
  if (*read < 0)
    *read = file_size + *read - *skip;
  else if (*read == 0)
    *read = file_size - *skip;

  if (*skip + *read > file_size) {
    print_error(ERROR_INB, "Overreading file \"%s\" by %d bytes.\n", g_full_name, *skip + *read - file_size);
    return FAILED;
  }

  if (*swap == 1) {
    if ((*filter_size == 1 && (*read & 1) != 0) ||
        (*filter_size == 2 && (*read % 2) != 0) ||
        (*filter_size == 3 && (*read % 3) != 0) ||
        (*filter_size == 4 && (*read % 4) != 0)) {
      if (*filter_size == 1 && (*read & 1) != 0)
        print_error(ERROR_INB, "The read size of file \"%s\" is odd (%d). Cannot perform SWAP.\n", g_full_name, *read);
      else
        print_error(ERROR_INB, "Processing %d bytes at a time from file \"%s\" but it's not divisible by the FILTERSIZE %d. Cannot perform SWAP.\n", *read, g_full_name, *filter_size);
      return FAILED;
    }
  }

  if (*filter_size > 1 && (*read % *filter_size) != 0) {
    print_error(ERROR_INB, "The FILTERSIZE is %d, but the number of bytes we are going to read is not divisible by it.\n", *read);
    return FAILED;
  }

  if (freadsize_label[0] != 0)
    add_a_new_definition(freadsize_label, (double)*read, NULL, DEFINITION_TYPE_VALUE, 0);
  
  return SUCCEEDED;
}


static char s_get_file_name_error[] = "GET_FILE_NAME: Internal error.";


char *get_file_name(int id) {

  struct file_name_info *fni;

  fni = g_file_name_info_first;
  while (fni != NULL) {
    if (id == fni->id)
      return fni->name;
    fni = fni->next;
  }

  return s_get_file_name_error;
}


/* converts filename to forward slashes for make compatibility */
static void _print_file_name(FILE *f, char *file_name) {

  char c;
  
  for (c = *file_name++; c != 0; c = *file_name++) {
    if (c == '\\')
      fputc('/', f);
    else
      fputc(c, f);
  }
}


static void _print_makefile_rule(FILE *f, char *target_file_name, char *prerequisite_file_name, int add_phony_target) {

  _print_file_name(f, target_file_name);
  fputc(':', f);
  fputc(' ', f);
  _print_file_name(f, prerequisite_file_name);
  fputc('\n', f);

  if (add_phony_target == YES) {
    _print_file_name(f, prerequisite_file_name);
    fputc(':', f);
    fputc('\n', f);
  }
}


int print_file_names(char *target_file_name) {

  struct incbin_file_data *ifd;
  struct file_name_info *fni;
  struct stringmaptable *smt;
  struct string *fopens;
  int is_first_line = YES;
  
  fni = g_file_name_info_first;
  ifd = g_incbin_file_data_first;
  smt = g_stringmaptables;
  fopens = g_fopen_filenames_first;

  /* included files */
  /* handle the main file name differently */
  while (fni != NULL) {
    if (is_first_line == YES) {
      _print_makefile_rule(g_makefile_rule_file, target_file_name, fni->name, NO);
      is_first_line = NO;
    }
    else
      _print_makefile_rule(g_makefile_rule_file, target_file_name, fni->name, g_makefile_add_phony_targets);
    fni = fni->next;
  }

  /* incbin files */
  while (ifd != NULL) {
    _print_makefile_rule(g_makefile_rule_file, target_file_name, ifd->name, g_makefile_add_phony_targets);
    ifd = ifd->next;
  }

  /* stringmaptable files */
  while (smt != NULL) {
    _print_makefile_rule(g_makefile_rule_file, target_file_name, smt->filename, g_makefile_add_phony_targets);
    smt = smt->next;
  }

  /* filenames used in .fopens */
  while (fopens != NULL) {
    _print_makefile_rule(g_makefile_rule_file, target_file_name, fopens->string, g_makefile_add_phony_targets);
    fopens = fopens->next;
  }

  return SUCCEEDED;
}


/* the mystery preprocessor - touch it and prepare for trouble ;) the preprocessor
   removes as much white space as possible from the source file. this is to make
   the parsing of the file, that follows, simpler. */
int preprocess_file(char *input, char *input_end, char *out_buffer, int *out_size, char *file_name) {

  /* this is set to 1 when the parser finds a non white space symbol on the line it's parsing */
  register int got_chars_on_line = 0;

  /* values for z - z tells us the state of the preprocessor on the line it is processing
     the value of z is 0 at the beginning of a new line, and can only grow: 0 -> 1 -> 2 -> 3
     0 - new line
     1 - 1+ characters on the line
     2 - extra white space removed
     3 - again 1+ characters follow */
  register int z = 0;

#if defined(W65816) || defined(SPC700)
  register int square_bracket_open = 0;
#endif

  char *output = out_buffer;

  while (input < input_end) {
    switch (*input) {
    case ';':
      /* clear a commented line */
      input++;
      for ( ; input < input_end && *input != 0x0A && *input != 0x0D; input++)
        ;
      output--;
      for ( ; output > out_buffer && *output == ' '; output--)
        ;
      if (output < out_buffer)
        output = out_buffer;
      else if (*output != ' ')
        output++;
      break;
    case '*':
      if (got_chars_on_line == 0) {
        /* clear a commented line */
        input++;
        for ( ; input < input_end && *input != 0x0A && *input != 0x0D; input++)
          ;
      }
      else {
        /* multiplication! */
        input++;
        *output = '*';
        output++;
      }
      break;
    case '/':
      if (*(input + 1) == '*') {
        /* remove an ANSI C -style block comment */
        got_chars_on_line = 0;
        input += 2;
        while (got_chars_on_line == 0) {
          for ( ; input < input_end && *input != '/' && *input != 0x0A; input++)
            ;
          if (input >= input_end) {
            print_error(ERROR_INC, "Comment wasn't terminated properly in file \"%s\".\n", file_name);
            return FAILED;
          }
          if (*input == 0x0A) {
            *output = 0x0A;
            output++;
          }
          if (*input == '/' && *(input - 1) == '*') {
            got_chars_on_line = 1;
          }
          input++;
        }
        output--;
        for ( ; output > out_buffer && *output == ' '; output--)
          ;
        if (output < out_buffer)
          output = out_buffer;
        else if (*output != ' ')
          output++;
      }
      else if (*(input + 1) == '/') {
        /* C++ style comment -> clear a commented line */
        input += 2;
        for ( ; input < input_end && *input != 0x0A && *input != 0x0D; input++)
          ;
        output--;
        for ( ; output > out_buffer && *output == ' '; output--)
          ;
        if (output < out_buffer)
          output = out_buffer;
        else if (*output != ' ')
          output++;
      }
      else {
        input++;
        *output = '/';
        output++;
        got_chars_on_line = 1;
      }
      break;
    case ':':
      /* finding a label resets the counters */
      input++;
      *output = ':';
      output++;
      got_chars_on_line = 0;
      break;
    case 0x09:
    case ' ':
      /* remove extra white space */
      input++;
      *output = ' ';
      output++;
      for ( ; input < input_end && (*input == ' ' || *input == 0x09); input++)
        ;
      if (got_chars_on_line == 0 && (*input == '-' || *input == '+')) {
        /* special case - -/--/--- or +/++/+++, but not at the beginning of the line, but after some white space */
        output--;
        for ( ; input < input_end && (*input == '+' || *input == '-'); input++, output++)
          *output = *input;     
      }
      else if (got_chars_on_line == 0 && *input == '_') {
        /* special case - _/__, but not at the beginning of the line, but after some white space */
        output--;
        for ( ; input < input_end && *input == '_'; input++, output++)
          *output = *input;     
      }
      got_chars_on_line = 1;
      if (z == 1)
        z = 2;
      break;
    case 0x0A:
      /* take away white space from the end of the line */
      input++;
      output--;
      for ( ; output > out_buffer && *output == ' '; output--)
        ;
      output++;
      *output = 0x0A;
      output++;
      /* moving on to a new line */
      got_chars_on_line = 0;
      z = 0;
#if defined(W65816) || defined(SPC700)
      square_bracket_open = 0;
#endif
      break;
    case 0x0D:
      input++;
      break;
    case '\'':
      if (*(input + 2) == '\'') {
        *output = '\'';
        input++;
        output++;
        *output = *input;
        input++;
        output++;
        *output = '\'';
        input++;
        output++;
      }
      else {
        *output = '\'';
        input++;
        output++;
      }
      got_chars_on_line = 1;
      break;
    case '"':
      /* don't touch strings */
      *output = '"';
      input++;
      output++;
      got_chars_on_line = 1;
      while (1) {
        for ( ; input < input_end && *input != '"' && *input != 0x0A && *input != 0x0D; ) {
          if (*input == '\\' && input+1 < input_end && *(input+1) == '\\') {
            *output = *input;
            output++;
            input++;
            *output = *input;
            output++;
            input++;
          }
          else if (*input == '\\' && input+1 < input_end && *(input+1) == '"') {
            *output = *input;
            output++;
            input++;
            *output = *input;
            output++;
            input++;
          }
          else {
            *output = *input;
            input++;
            output++;
          }
        }

        if (input >= input_end)
          break;
        else if (*input == 0x0A || *input == 0x0D) {
          /* process 0x0A/0x0D as usual, and later when we try to input a string, the parser will fail as 0x0A comes before a " */
          break;
        }
        else {
          *output = '"';
          input++;
          output++;
          break;
        }
      }
      break;
#if !defined(W65816) && !defined(SPC700)
    case '[':
      /* change '[' to '(' */
#endif
    case '(':
      *output = '(';
      input++;
      output++;
      for ( ; input < input_end && (*input == ' ' || *input == 0x09); input++)
        ;
      got_chars_on_line = 1;
      break;
#if !defined(W65816) && !defined(SPC700)
    case ']':
      /* change ']' to ')' */
#endif
    case ')':
      /* go back? */
      if (got_chars_on_line == 1 && *(output - 1) == ' ')
        output--;
      *output = ')';
      input++;
      output++;
      got_chars_on_line = 1;
      break;
#if defined(W65816) || defined(SPC700)
    case '[':
      *output = *input;
      input++;
      output++;
      got_chars_on_line = 1;
      square_bracket_open = 1;
      break;
#endif
    case ',':
    case '+':
    case '-':
      if (got_chars_on_line == 0) {
        for ( ; input < input_end && (*input == '+' || *input == '-'); input++, output++)
          *output = *input;
        got_chars_on_line = 1;
      }
      else {
#if defined(W65816) || defined(SPC700)
        /* go back? */
        if (*(output - 1) == ' ' && square_bracket_open == 1)
          output--;
#else
        /* go back? */
        if ((z == 3 || *input == ',') && *(output - 1) == ' ' && *(output - 2) == ' ')
          output--;
#endif
        *output = *input;
        input++;
        output++;
        for ( ; input < input_end && (*input == ' ' || *input == 0x09); input++)
          ;
        got_chars_on_line = 1;
      }
      break;
    default:
      *output = *input;
      input++;
      output++;
      got_chars_on_line = 1;

      /* mode changes... */
      if (z == 0)
        z = 1;
      else if (z == 2)
        z = 3;
      break;
    }
  }

  *out_size = (int)(output - out_buffer);

  return SUCCEEDED;
}

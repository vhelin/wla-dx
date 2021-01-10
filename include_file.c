
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crc32.h"
#include "defines.h"

#include "include_file.h"
#include "pass_1.h"
#include "parse.h"
#include "printf.h"


extern int ind, inz, i, unfolded_size, extra_definitions, d, use_incdir;
extern char *unfolded_buffer, tmp[4096], emsg[sizeof(tmp) + MAX_NAME_LENGTH + 1 + 1024];
extern struct ext_include_collection ext_incdirs;
extern FILE *file_out_ptr;

struct incbin_file_data *incbin_file_data_first = NULL, *ifd_tmp;
struct active_file_info *active_file_info_first = NULL, *active_file_info_last = NULL, *active_file_info_tmp = NULL;
struct file_name_info *file_name_info_first = NULL, *file_name_info_last = NULL, *file_name_info_tmp;
char *include_in_tmp = NULL, *tmp_a = NULL;
int include_in_tmp_size = 0, tmp_a_size = 0, file_name_id = 1, open_files = 0;

char *full_name = NULL;
int full_name_size = 0;

char *include_dir = NULL;
int include_dir_size = 0;

char *buffer = NULL;
int size = 0;


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

  if (i > full_name_size) {
    tmp = realloc(full_name, i);
    if (tmp == NULL) {
      fprintf(stderr, "CREATE_FULL_NAME: Out of memory error.\n");
      return FAILED;
    }
    full_name = tmp;
    full_name_size = i;
  }

  if (dir != NULL) {
    strcpy(full_name, dir);
    if (name != NULL)
      strcat(full_name, name);
  }
  else
    strcpy(full_name, name);

  return SUCCEEDED;
}


int try_open_file(char* directory, char* partial_name, int print_errors, FILE** out_result) {

  int error_code = create_full_name(directory, partial_name);
  if (error_code != FAILED)
    *out_result = fopen(full_name, "rb");

  if (*out_result == NULL && (directory == NULL || directory[0] == 0)) {
    snprintf(emsg, sizeof(emsg), "Error opening file \"%s\".\n", partial_name);
    if (print_errors == 0)
      fprintf(stderr, "INCLUDE_FILE: %s", emsg);
    else
      print_error(emsg, ERROR_INC);
    error_code = FAILED;
  }

  return error_code;
}


int include_file(char *name, int *include_size, char *namespace) {

  static int first_load = 0;
  int file_size, id, change_file_buffer_size, index;
  char *tmp_b, *n, change_file_buffer[MAX_NAME_LENGTH * 2];
  FILE *f = NULL;

  if (use_incdir == YES) {
    /* check all external include directories first */
    for (index = 0; index < ext_incdirs.count; index++) {
      int error_code = try_open_file(ext_incdirs.names[index], name, first_load, &f);
      if (error_code != SUCCEEDED)
        return error_code;

      /* we succeeded and found a valid file, so escape */
      if (f != NULL)
        break;
    }
  }

  /* fall through to include_dir if we either didn't check, or failed to find the file in ext_incdirs */
  if (f == NULL) {
    int error_code = try_open_file(include_dir, name, first_load, &f);
    if (error_code != SUCCEEDED)
      return error_code;
  }

  if (f != NULL)
    id = 0;

  /* if failed try to find the file in the current directory */
  if (f == NULL) {
    fprintf(stderr, "%s:%d: ", get_file_name(active_file_info_last->filename_id), active_file_info_last->line_current);
    fprintf(stderr, "INCLUDE_FILE: Could not open \"%s\", trying \"%s\"... ", full_name, name);
    f = fopen(name, "rb");
    id = 1;
  }

  if (f == NULL) {
    fprintf(stderr, "not found.\n");
    snprintf(emsg, sizeof(emsg), "Error opening file \"%s\".\n", full_name);
    if (first_load == 0)
      fprintf(stderr, "INCLUDE_FILE: %s", emsg);
    else
      print_error(emsg, ERROR_INC);
    return FAILED;
  }

  if (id == 1) {
    fprintf(stderr, "found.\n");
    strcpy(full_name, name);
  }

  first_load = 1;

  fseek(f, 0, SEEK_END);
  file_size = (int)ftell(f);
  fseek(f, 0, SEEK_SET);

  /* name */
  file_name_info_tmp = file_name_info_first;
  id = 0;

  /* NOTE: every filename, even included multiple times, is unique
     while (file_name_info_tmp != NULL) {
     if (strcmp(file_name_info_tmp->name, full_name) == 0) {
     id = file_name_info_tmp->id;
     break;
     }
     file_name_info_tmp = file_name_info_tmp->next;
     }
  */

  if (id == 0) {
    file_name_info_tmp = calloc(sizeof(struct file_name_info), 1);
    n = calloc(strlen(full_name)+1, 1);
    if (file_name_info_tmp == NULL || n == NULL) {
      if (file_name_info_tmp != NULL)
        free(file_name_info_tmp);
      if (n != NULL)
        free(n);
      snprintf(emsg, sizeof(emsg), "Out of memory while trying allocate info structure for file \"%s\".\n", full_name);
      print_error(emsg, ERROR_INC);
      return FAILED;
    }
    file_name_info_tmp->next = NULL;

    if (file_name_info_first == NULL) {
      file_name_info_first = file_name_info_tmp;
      file_name_info_last = file_name_info_tmp;
    }
    else {
      file_name_info_last->next = file_name_info_tmp;
      file_name_info_last = file_name_info_tmp;
    }

    strcpy(n, full_name);
    file_name_info_tmp->name = n;
    file_name_info_tmp->id = file_name_id;
    id = file_name_id;
    file_name_id++;
  }

  if (namespace == NULL || namespace[0] == 0)
    snprintf(change_file_buffer, sizeof(change_file_buffer), "%c.CHANGEFILE %d NONAMESPACE%c", 0xA, id, 0xA);
  else
    snprintf(change_file_buffer, sizeof(change_file_buffer), "%c.CHANGEFILE %d NAMESPACE %s%c", 0xA, id, namespace, 0xA);
  change_file_buffer_size = (int)strlen(change_file_buffer);

  /* reallocate buffer */
  if (include_in_tmp_size < file_size) {
    if (include_in_tmp != NULL)
      free(include_in_tmp);

    include_in_tmp = calloc(sizeof(char) * file_size, 1);
    if (include_in_tmp == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while trying to allocate room for \"%s\".\n", full_name);
      print_error(emsg, ERROR_INC);
      return FAILED;
    }

    include_in_tmp_size = file_size;
  }

  /* read the whole file into a buffer */
  fread(include_in_tmp, 1, file_size, f);
  fclose(f);

  /* calculate checksum for post-compilation file verification */
  file_name_info_tmp->checksum = crc32((unsigned char*)include_in_tmp, file_size);

  if (size == 0) {
    buffer = calloc(sizeof(char) * (change_file_buffer_size + (file_size + 4)), 1);
    if (buffer == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while trying to allocate room for \"%s\".\n", full_name);
      print_error(emsg, ERROR_INC);
      return FAILED;
    }

    memcpy(buffer, change_file_buffer, change_file_buffer_size);
    
    /* preprocess */
    preprocess_file(include_in_tmp, include_in_tmp + file_size, &buffer[change_file_buffer_size], &size, full_name);
    size += change_file_buffer_size;

    buffer[size++] = 0xA;
    buffer[size++] = '.';
    buffer[size++] = 'E';
    buffer[size++] = ' ';

    *include_size = size;

    return SUCCEEDED;
  }

  tmp_b = calloc(sizeof(char) * (size + change_file_buffer_size + file_size + 4), 1);
  if (tmp_b == NULL) {
    snprintf(emsg, sizeof(emsg), "Out of memory while trying to expand the project to incorporate file \"%s\".\n", full_name);
    print_error(emsg, ERROR_INC);
    return FAILED;
  }

  /* reallocate tmp_a */
  if (tmp_a_size < change_file_buffer_size + file_size + 4) {
    if (tmp_a != NULL)
      free(tmp_a);

    tmp_a = calloc(sizeof(char) * (change_file_buffer_size + file_size + 4), 1);
    if (tmp_a == NULL) {
      snprintf(emsg, sizeof(emsg), "Out of memory while allocating new room for \"%s\".\n", full_name);
      print_error(emsg, ERROR_INC);
      free(tmp_b);
      return FAILED;
    }

    tmp_a_size = change_file_buffer_size + file_size + 4;
  }

  memcpy(tmp_a, change_file_buffer, change_file_buffer_size);
      
  /* preprocess */
  inz = 0;
  preprocess_file(include_in_tmp, include_in_tmp + file_size, &tmp_a[change_file_buffer_size], &inz, full_name);
  inz += change_file_buffer_size;

  tmp_a[inz++] = 0xA;
  tmp_a[inz++] = '.';
  tmp_a[inz++] = 'E';
  tmp_a[inz++] = ' ';

  memcpy(tmp_b, buffer, i);
  memcpy(tmp_b + i, tmp_a, inz);
  memcpy(tmp_b + i + inz, buffer + i, size - i);

  free(buffer);

  size += inz;
  buffer = tmp_b;

  *include_size = inz;
  
  return SUCCEEDED;
}


int incbin_file(char *name, int *id, int *swap, int *skip, int *read, struct macro_static **macro) {

  struct incbin_file_data *ifd;
  char *in_tmp, *n;
  int file_size, q, index;
  FILE *f = NULL;

  if (use_incdir == YES) {
    int error_code;
    
    /* check all external include directories first */
    for (index = 0; index < ext_incdirs.count; ++index) {
      error_code = try_open_file(ext_incdirs.names[index], name, 1, &f);
      if (error_code != SUCCEEDED)
        return error_code;

      /* we succeeded and found a valid file, so escape */
      if (f != NULL)
        break;
    }
  }

  /* fall through to include_dir if we either didn't check, or failed to find the file in ext_incdirs */
  if (f == NULL) {
    int error_code = try_open_file(include_dir, name, 1, &f);
    if (error_code != SUCCEEDED)
      return error_code;
  }

  if (f != NULL)
    q = 0;

  /* if failed try to find the file in the current directory */
  if (f == NULL) {
    fprintf(stderr, "%s:%d: ", get_file_name(active_file_info_last->filename_id), active_file_info_last->line_current);
    fprintf(stderr, "INCBIN_FILE: Could not open \"%s\", trying \"%s\"... ", full_name, name);
    f = fopen(name, "rb");
    q = 1;
  }

  if (f == NULL) {
    fprintf(stderr, "not found.\n");
    snprintf(emsg, sizeof(emsg), "Error opening file \"%s\".\n", full_name);
    print_error(emsg, ERROR_INB);
    return FAILED;
  }

  if (q == 1) {
    fprintf(stderr, "found.\n");
    strcpy(full_name, name);
  }

  fseek(f, 0, SEEK_END);
  file_size = (int)ftell(f);
  fseek(f, 0, SEEK_SET);

  ifd = (struct incbin_file_data *)calloc(sizeof(struct incbin_file_data), 1);
  n = calloc(sizeof(char) * (strlen(full_name)+1), 1);
  in_tmp = (char *)calloc(sizeof(char) * file_size, 1);
  if (ifd == NULL || n == NULL || in_tmp == NULL) {
    if (ifd != NULL)
      free(ifd);
    if (n != NULL)
      free(n);
    if (in_tmp != NULL)
      free(in_tmp);
    snprintf(emsg, sizeof(emsg), "Out of memory while allocating data structure for \"%s\".\n", full_name);
    print_error(emsg, ERROR_INB);
    fclose(f);
    return FAILED;
  }

  /* read the whole file into a buffer */
  fread(in_tmp, 1, file_size, f);
  fclose(f);

  /* complete the structure */
  ifd->next = NULL;
  ifd->size = file_size;
  strcpy(n, full_name);
  ifd->name = n;
  ifd->data = in_tmp;

  /* find the index */
  q = 0;
  if (incbin_file_data_first != NULL) {
    ifd_tmp = incbin_file_data_first;
    while (ifd_tmp->next != NULL && strcmp(ifd_tmp->name, full_name) != 0) {
      ifd_tmp = ifd_tmp->next;
      q++;
    }
    if (ifd_tmp->next == NULL && strcmp(ifd_tmp->name, full_name) != 0) {
      ifd_tmp->next = ifd;
      q++;
    }
  }
  else
    incbin_file_data_first = ifd;

  *id = q;

  /* SKIP bytes? */
  if (compare_next_token("SKIP") == FAILED)
    *skip = 0;
  else {
    skip_next_token();
    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error(".INCBIN needs the amount of skipped bytes.\n", ERROR_DIR);
      return FAILED;
    }

    *skip = d;

    if (d >= file_size) {
      snprintf(emsg, sizeof(emsg), "SKIP value (%d) is more than the size (%d) of file \"%s\".\n", d, file_size, full_name);
      print_error(emsg, ERROR_INB);
      return FAILED;
    }
  }

  /* READ bytes? */
  if (compare_next_token("READ") == FAILED)
    *read = file_size - *skip;
  else {
    skip_next_token();
    inz = input_number();
    if (inz != SUCCEEDED) {
      print_error(".INCBIN needs the amount of bytes for reading.\n", ERROR_DIR);
      return FAILED;
    }

    *read = d;

    if (*skip + *read > file_size) {
      snprintf(emsg, sizeof(emsg), "Overreading file \"%s\" by %d bytes.\n", full_name, *skip + *read - file_size);
      print_error(emsg, ERROR_INB);
      return FAILED;
    }
  }

  /* SWAP bytes? */
  if (compare_next_token("SWAP") == FAILED)
    *swap = 0;
  else {
    if ((*read & 1) == 1) {
      snprintf(emsg, sizeof(emsg), "The read size of file \"%s\" is odd (%d)! Cannot perform SWAP.\n", full_name, *read);
      print_error(emsg, ERROR_INB);
      return FAILED;
    }
    *swap = 1;
    skip_next_token();
  }

  /* FSIZE? */
  if (compare_next_token("FSIZE") == SUCCEEDED) {
    skip_next_token();

    /* get the definition label */
    if (get_next_token() == FAILED)
      return FAILED;

    add_a_new_definition(tmp, (double)file_size, NULL, DEFINITION_TYPE_VALUE, 0);
  }

  /* FILTER? */
  if (compare_next_token("FILTER") == SUCCEEDED) {
    skip_next_token();

    /* get the filter macro name */
    if (get_next_token() == FAILED)
      return FAILED;

    if (macro_get(tmp, YES, macro) == FAILED)
      return FAILED;

    if (*macro == NULL) {
      snprintf(emsg, sizeof(emsg), "No MACRO \"%s\" defined.\n", tmp);
      print_error(emsg, ERROR_INB);
      return FAILED;
    }
  }
  else
    *macro = NULL;

  return SUCCEEDED;
}


char get_file_name_error[] = "GET_FILE_NAME: Internal error.";


char *get_file_name(int id) {

  struct file_name_info *fni;

  
  fni = file_name_info_first;
  while (fni != NULL) {
    if (id == fni->id)
      return fni->name;
    fni = fni->next;
  }

  return get_file_name_error;
}


int print_file_names(void) {

  struct incbin_file_data *ifd;
  struct file_name_info *fni;

  
  fni = file_name_info_first;
  ifd = incbin_file_data_first;

  /* handle the main file name differently */
  if (fni != NULL) {
    if (fni->next != NULL || ifd != NULL)
      fprintf(stdout, "%s \\\n", fni->name);
    else
      fprintf(stdout, "%s\n", fni->name);
    fni = fni->next;
  }

  /* included files */
  while (fni != NULL) {
    if (fni->next != NULL || ifd != NULL)
      fprintf(stdout, "\t%s \\\n", fni->name);
    else
      fprintf(stdout, "\t%s\n", fni->name);
    fni = fni->next;
  }

  /* incbin files */
  while (ifd != NULL) {
    if (ifd->next != NULL)
      fprintf(stdout, "\t%s \\\n", ifd->name);
    else
      fprintf(stdout, "\t%s\n", ifd->name);
    ifd = ifd->next;
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
            snprintf(emsg, sizeof(emsg), "Comment wasn't terminated properly in file \"%s\".\n", file_name);
            print_error(emsg, ERROR_INC);
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
          *output = *input;
          input++;
          output++;
        }

        if (input >= input_end)
          break;
        else if (*input == 0x0A || *input == 0x0D) {
          /* process 0x0A/0x0D as usual, and later when we try to input a string, the parser will fail as 0x0A comes before a " */
          break;
        }
        else if (*input == '"' && *(input - 1) != '\\') {
          *output = '"';
          input++;
          output++;
          break;
        }
        else {
          *output = '"';
          input++;
          output++;
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
        if ((z == 3 || *input == ',') && *(output - 1) == ' ')
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

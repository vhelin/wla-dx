
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define YES 1
#define NO  0

#define FAILED    0
#define SUCCEEDED 1

#define TRUE  1
#define FALSE 0

#define MAX_BYTES_PER_TEST 2048


#if defined(AMIGACPU)
__near long __stack = 200000;
#endif


static int _append_digit(int *value, int base, int digit) {

  if (*value > (INT_MAX - digit) / base)
    return FAILED;

  *value = (*value * base) + digit;

  return SUCCEEDED;
}


static int _parse_byte_token(const char *token, unsigned char *out) {

  char *endptr;
  unsigned long value;
  size_t len = strlen(token);

  if (len == 1) {
    *out = (unsigned char)token[0];
    return SUCCEEDED;
  }

  if (token[0] == '$') {
    if (len < 2)
      return FAILED;
    value = strtoul(token + 1, &endptr, 16);
    if (*endptr != 0 || value > 0xFF)
      return FAILED;
    *out = (unsigned char)value;
    return SUCCEEDED;
  }

  if (token[0] == '0' && token[1] == 'b') {
    if (token[2] == 0)
      return FAILED;
    value = strtoul(token + 2, &endptr, 2);
    if (*endptr != 0 || value > 0xFF)
      return FAILED;
    *out = (unsigned char)value;
    return SUCCEEDED;
  }

  if (len == 2) {
    if (!isxdigit((unsigned char)token[0]) || !isxdigit((unsigned char)token[1]))
      return FAILED;
    value = strtoul(token, &endptr, 16);
    if (*endptr != 0 || value > 0xFF)
      return FAILED;
    *out = (unsigned char)value;
    return SUCCEEDED;
  }

  if (token[0] == '0' && token[1] == 'd') {
    if (token[2] == 0)
      return FAILED;
    value = strtoul(token + 2, &endptr, 10);
    if (*endptr != 0 || value > 0xFF)
      return FAILED;
    *out = (unsigned char)value;
    return SUCCEEDED;
  }

  if (token[0] == '0' && token[1] == 'x') {
    if (token[2] == 0)
      return FAILED;
    value = strtoul(token + 2, &endptr, 16);
    if (*endptr != 0 || value > 0xFF)
      return FAILED;
    *out = (unsigned char)value;
    return SUCCEEDED;
  }

  return FAILED;
}


int _get_next_number(char *in, int *out) {

  int i = 0, o, n;

  if (in[i] == '$' || (in[i] == '0' && in[i+1] == 'x')) {
    if (in[i] == '$')
      i++;
    else
      i += 2;
    o = 0;
    {
      int digits = 0;
      while (TRUE) {
        if (in[i] >= '0' && in[i] <= '9') {
          if (_append_digit(&o, 16, in[i] - '0') == FAILED)
            return FAILED;
        }
        else if (in[i] >= 'a' && in[i] <= 'f') {
          if (_append_digit(&o, 16, in[i] - 'a' + 0xA) == FAILED)
            return FAILED;
        }
        else if (in[i] >= 'A' && in[i] <= 'F') {
          if (_append_digit(&o, 16, in[i] - 'A' + 0xA) == FAILED)
            return FAILED;
        }
        else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
          break;
        else
          return FAILED;
        digits++;
        i++;
      }

    if (digits == 0)
      return FAILED;
    }
  }
  else if (in[i] == '0' && in[i+1] == 'b') {
    i += 2;
    o = 0;
    {
      int digits = 0;
      while (TRUE) {
        if (in[i] >= '0' && in[i] <= '1') {
          if (_append_digit(&o, 2, in[i] - '0') == FAILED)
            return FAILED;
        }
        else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
          break;
        else
          return FAILED;
        digits++;
        i++;
      }

      if (digits == 0)
        return FAILED;
    }
  }
  else if (in[i] == '0' && in[i+1] == 'd') {
    /* decimal */
    i += 2;
    {
      int digits = 0;
    for (o = 0; in[i] >= '0' && in[i] <= '9'; i++) {
      if (_append_digit(&o, 10, in[i] - '0') == FAILED)
        return FAILED;
      digits++;
    }
    if (digits == 0)
      return FAILED;
    }
    if (!(in[i] == ' ' || in[i] == 0x09 || in[i] == 0))
      return FAILED;
  }
  else if (in[i] >= '0' && in[i] <= '9') {
    /* is it hex after all? */
    n = 0;
    for (o = 0; 1; o++) {
      if (in[i+o] >= '0' && in[i+o] <= '9')
        continue;
      if (in[i+o] >= 'a' && in[i+o] <= 'f') {
        n = 1;
        break;
      }
      if (in[i+o] >= 'A' && in[i+o] <= 'F') {
        n = 1;
        break;
      }
      if (in[i+o] == 'h' || in[i+o] == 'H') {
        n = 1;
        break;
      }
      break;
    }

    if (n == 1) {
      /* hex */
      o = 0;
      {
        int digits = 0;
        while (TRUE) {
          if (in[i] >= '0' && in[i] <= '9') {
            if (_append_digit(&o, 16, in[i] - '0') == FAILED)
              return FAILED;
          }
          else if (in[i] >= 'a' && in[i] <= 'f') {
            if (_append_digit(&o, 16, in[i] - 'a' + 0xA) == FAILED)
              return FAILED;
          }
          else if (in[i] >= 'A' && in[i] <= 'F') {
            if (_append_digit(&o, 16, in[i] - 'A' + 0xA) == FAILED)
              return FAILED;
          }
          else if (in[i] == 'h' || in[i] == 'H') {
            i++;
            if (!(in[i] == ' ' || in[i] == 0x09 || in[i] == 0))
              return FAILED;
            break;
          }
          else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
            break;
          else
            return FAILED;
          digits++;
          i++;
        }

      if (digits == 0)
        return FAILED;
      }
    }
    else {
      /* decimal */
      {
        int digits = 0;
      for (o = 0; in[i] >= '0' && in[i] <= '9'; i++) {
        if (_append_digit(&o, 10, in[i] - '0') == FAILED)
          return FAILED;
        digits++;
      }
      if (digits == 0)
        return FAILED;
      }
      if (!(in[i] == ' ' || in[i] == 0x09 || in[i] == 0))
        return FAILED;
    }
  }
  else
    return FAILED;

  *out = o;

  return SUCCEEDED;
}


unsigned char *g_binary_file = NULL;
int g_get_token_overflow = NO;


static int _looks_like_filename_token(const char *token) {

  return (strchr(token, '.') != NULL || strchr(token, '/') != NULL || strchr(token, '\\') != NULL) ? YES : NO;
}


int _read_binary_file(char *filename, int *did_we_read_data, int *file_size) {

  FILE *fb;
  unsigned char *new_data;
  long file_size_long;
  
  fb = fopen(filename, "rb");

  if (fb == NULL) {
    if (g_binary_file != NULL) {
      if (_looks_like_filename_token(filename) == YES) {
        fprintf(stderr, "Error opening file \"%s\".\n", filename);
        return FAILED;
      }
      *did_we_read_data = NO;
      return SUCCEEDED;
    }
    fprintf(stderr, "Error opening file \"%s\".\n", filename);
    return FAILED;
  }

  if (fseek(fb, 0, SEEK_END) != 0) {
    fprintf(stderr, "Could not seek to end of \"%s\".\n", filename);
    fclose(fb);
    return FAILED;
  }

  file_size_long = ftell(fb);
  if (file_size_long < 0) {
    fprintf(stderr, "Could not determine size of \"%s\".\n", filename);
    fclose(fb);
    return FAILED;
  }

  if (file_size_long > INT_MAX) {
    fprintf(stderr, "File \"%s\" is too large.\n", filename);
    fclose(fb);
    return FAILED;
  }

  *file_size = (int)file_size_long;

  if (fseek(fb, 0, SEEK_SET) != 0) {
    fprintf(stderr, "Could not seek to start of \"%s\".\n", filename);
    fclose(fb);
    return FAILED;
  }

  new_data = calloc((*file_size > 0) ? *file_size : 1, 1);
  if (new_data == NULL) {
    fprintf(stderr, "Error allocating memory for file \"%s\".\n", filename);
    fclose(fb);
    *did_we_read_data = NO;
    return FAILED;
  }

  if (fread(new_data, 1, *file_size, fb) != (size_t) *file_size) {
    fprintf(stderr, "Could not read all %d bytes of \"%s\"!", *file_size, filename);
    free(new_data);
    fclose(fb);
    return FAILED;
  };

  if (g_binary_file != NULL)
    free(g_binary_file);

  g_binary_file = new_data;

  fclose(fb);

  *did_we_read_data = YES;
        
  return SUCCEEDED;
}


/* get the next token from the input string and write the results on the result argument */
int get_token(char **input, char *result) {

  int length = 0;
  char *src = *input;

  g_get_token_overflow = NO;

  while (*src != 0 && isspace((unsigned char)*src))
    src++;

  if (*src == 0)
    return FAILED;

  while (*src != 0 && !isspace((unsigned char)*src)) {
    if (length >= MAX_BYTES_PER_TEST - 1) {
      while (*src != 0 && !isspace((unsigned char)*src))
        src++;
      result[0] = 0;
      *input = src;
      g_get_token_overflow = YES;
      fprintf(stderr, "Token too long (max %d chars).\n", MAX_BYTES_PER_TEST - 1);
      return FAILED;
    }
    result[length++] = *src;
    src++;
  }

  result[length] = 0;

  *input = src;

  return SUCCEEDED;
}


/* extracts contents of byte tester comments in-place. the result is
   placed on the input string itself, overwriting its contents.  */
int extract_comments(char *input, size_t size) {

  char *current = input, *output = input, *end = NULL;

  while (*current) {
    char *comment = NULL, *result;
    int comment_length;

    /* search for the next comment of each type, picking the nearest */

    /* semicolon comment */
    result = strstr(current, "; @BT");
    if (result) {
      comment = result + 5;
      end = strchr(comment, '\n');
    }

    /* double slash comment */
    result = strstr(current, "// @BT");
    if (result && (!comment || result < comment)) {
      comment = result + 6;
      end = strchr(comment, '\n');
    }

    /* block comment */
    result = strstr(current, "/* @BT");
    if (result && (!comment || result < comment)) {
      comment = result + 6;
      end = strstr(comment, "*/");
    }

    /* stop parsing if there is no more comments */
    if (!comment)
      break;

    /* extend unclosed comments to the end of the input string.
       this handles the cases where a semicolon or double slash
       comment is on the last line, or a unclosed block comment. */
    if (!end)
      end = input + size;

    /* overwrite input string with comment contents */
    current = comment;
    comment_length = (int)(end - current);

    if (output != input && output < current)
      *output++ = '\n';

    memmove(output, current, comment_length);
    output += comment_length;
    current += comment_length;
  }

  *output = '\0';

  return SUCCEEDED;
}


static int main_run(int argc, char *argv[], char **input_names, int input_name_count, char *tmp, char *test_id, char *tag_id) {

  int input_size, file_size, end, byte_count, i, j, length, tag_start, tag_end, wrong_bytes, failures, should_extract_comments = NO, use_address = NO, address = 0, did_we_read_data = NO, bytes_capacity = MAX_BYTES_PER_TEST;
  char *input = NULL, *current = NULL;
  unsigned char *bytes = NULL;
  FILE *f = NULL;

  /* parse flags and collect input filenames */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0)
      should_extract_comments = YES;
    else {
      input_names[input_name_count] = calloc(strlen(argv[i]) + 1, 1);
      if (input_names[input_name_count] == NULL) {
        fprintf(stderr, "Error allocating memory for input filename.\n");
        return 1;
      }
      strcpy(input_names[input_name_count], argv[i]);
      input_name_count++;
    }
  }

  if (input_name_count == 0) {
    fprintf(stderr, "No input files given.\n");
    return 1;
  }

  if (should_extract_comments == YES && input_name_count > 1) {
    /* merge all source files into one buffer, then extract comments */
    input_size = 0;

    for (i = 0; i < input_name_count; i++) {
      FILE *fs = fopen(input_names[i], "rb");
      int fs_size;
      long fs_size_long;

      if (fs == NULL) {
        fprintf(stderr, "Error opening file \"%s\".\n", input_names[i]);
        free(input);
        return 1;
      }

      if (fseek(fs, 0, SEEK_END) != 0) {
        fprintf(stderr, "Could not seek to end of \"%s\".\n", input_names[i]);
        fclose(fs);
        free(input);
        return 1;
      }

      fs_size_long = ftell(fs);
      if (fs_size_long < 0) {
        fprintf(stderr, "Could not determine size of \"%s\".\n", input_names[i]);
        fclose(fs);
        free(input);
        return 1;
      }

      if (fs_size_long > INT_MAX) {
        fprintf(stderr, "File \"%s\" is too large.\n", input_names[i]);
        fclose(fs);
        free(input);
        return 1;
      }

      fs_size = (int)fs_size_long;

      if (fseek(fs, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Could not seek to start of \"%s\".\n", input_names[i]);
        fclose(fs);
        free(input);
        return 1;
      }

      /* grow the buffer: existing data + newline separator + new file + terminator */
      {
        char *new_input;

        if (fs_size > INT_MAX - input_size - 2) {
          fprintf(stderr, "Merged input would be too large.\n");
          fclose(fs);
          free(input);
          return 1;
        }

        new_input = realloc(input, input_size + fs_size + 2);

        if (new_input == NULL) {
          fprintf(stderr, "Error allocating memory for merged input.\n");
          fclose(fs);
          free(input);
          return 1;
        }

        input = new_input;
      }

      if (input_size > 0)
        input[input_size++] = '\n';

      if (fread(input + input_size, 1, fs_size, fs) != (size_t)fs_size) {
        fprintf(stderr, "Could not read all %d bytes of \"%s\"!", fs_size, input_names[i]);
        fclose(fs);
        free(input);
        return 1;
      }

      input_size += fs_size;
      fclose(fs);
    }

    input[input_size] = 0;
    f = fopen(input_names[0], "rb");
    if (f == NULL) {
      fprintf(stderr, "Error opening file \"%s\".\n", input_names[0]);
      free(input);
      return 1;
    }
  }
  else {
    /* single file mode */
    f = fopen(input_names[0], "rb");

    if (f == NULL) {
      fprintf(stderr, "Error opening file \"%s\".\n", input_names[0]);
      return 1;
    }

    {
      long input_size_long;

      if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "Could not seek to end of \"%s\".\n", input_names[0]);
        fclose(f);
        return 1;
      }

      input_size_long = ftell(f);
      if (input_size_long < 0) {
        fprintf(stderr, "Could not determine size of \"%s\".\n", input_names[0]);
        fclose(f);
        return 1;
      }

      if (input_size_long > INT_MAX) {
        fprintf(stderr, "File \"%s\" is too large.\n", input_names[0]);
        fclose(f);
        return 1;
      }

      input_size = (int)input_size_long;

      if (fseek(f, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Could not seek to start of \"%s\".\n", input_names[0]);
        fclose(f);
        return 1;
      }
    }
    /* make sure we allocate one extra byte for terminator 0 */
    input = calloc(input_size + 1, 1);
    if (input == NULL) {
      fprintf(stderr, "Error allocating memory for input.\n");
      fclose(f);
      return 1;
    }

    if (fread(input, 1, input_size, f) != (size_t)input_size) {
      fprintf(stderr, "Could not read all %d bytes of \"%s\"!", input_size, input_names[0]);
      fclose(f);
      free(input);
      return 1;
    }
  }

  current = input;

  bytes = calloc(bytes_capacity, 1);
  if (bytes == NULL) {
    fprintf(stderr, "Error allocating memory for test bytes.\n");
    if (f != NULL)
      fclose(f);
    free(input);
    return 1;
  }

  /* extract input from comments in file */
  if (should_extract_comments)
    extract_comments(input, input_size);

  if (!get_token(&current, tmp)) {
    fprintf(stderr, "No binary file name found in input.\n");
    if (f != NULL)
      fclose(f);
    free(input);
    free(bytes);
    return 1;
  }

  /* read the binary file */
  if (_read_binary_file(tmp, &did_we_read_data, &file_size) == FAILED) {
    if (f != NULL)
      fclose(f);
    free(input);
    free(bytes);
    return 1;
  }

  /* execute the tests */
  failures = 0;
  end = 0;
  while (end == 0) {
    use_address = NO;
    address = 0;

    if (!get_token(&current, test_id)) {
      if (g_get_token_overflow == YES)
        failures = 1;
      break;
    }
    /* test_id could be filename */
    if (_read_binary_file(test_id, &did_we_read_data, &file_size) == FAILED) {
      if (f != NULL)
        fclose(f);
      free(input);
      free(bytes);
      return 1;
    }
    if (did_we_read_data == YES)
      continue;
    if (!get_token(&current, tag_id)) {
      if (g_get_token_overflow == YES)
        failures = 1;
      break;
    }

    if (strlen(tag_id) != 2) {
      fprintf(stderr, "Test \"%s\" FAILED - TAG ID must be exactly two characters long. Error in \"%s\".\n", test_id, tag_id);
      failures = 1;
      break;
    }

    if (strcmp(tag_id, "-a") == 0) {
      use_address = YES;

      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        break;
      }

      if (_get_next_number(tmp, &address) == FAILED) {
        fprintf(stderr, "\"%s\" is not a number.\n", tmp);
        failures = 1;
        break;
      }

      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        break;
      }

      if (strcmp(tmp, "START") != 0) {
        fprintf(stderr, "Test \"%s\" FAILED - START is missing in \"%s\".\n", test_id, argv[1]);
        failures = 1;
        break;
      }
    }
    else if (strcmp(tag_id, "-y") == 0) {
      int got_it = NO;

      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        break;
      }

      length = (int)strlen(tmp);

      if (length <= file_size) {
        for (i = 0; i <= file_size - length; i++) {
          for (j = 0; j < length; j++) {
            if (g_binary_file[i+j] != tmp[j])
              break;
          }
          if (j == length) {
            /* we found the string -> ok! */
            got_it = YES;
            break;
          }
        }
      }

      if (got_it == YES) {
        fprintf(stderr, "Test \"%s\" SUCCEEDED!\n", test_id);
        continue;
      }

      fprintf(stderr, "Test \"%s\" FAILED - Could not find string \"%s\".\n", test_id, tmp);
      failures = 1;
      continue;
    }
    else if (strcmp(tag_id, "-n") == 0) {
      int got_it = NO;

      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        break;
      }

      length = (int)strlen(tmp);

      if (length <= file_size) {
        for (i = 0; i <= file_size - length; i++) {
          for (j = 0; j < length; j++) {
            if (g_binary_file[i+j] != tmp[j])
              break;
          }
          if (j == length) {
            /* we found the string -> not ok! */
            got_it = YES;
            break;
          }
        }
      }

      if (got_it == YES) {
        fprintf(stderr, "Test \"%s\" FAILED - Found forbidden string \"%s\".\n", test_id, tmp);
        failures = 1;
        break;
      }

      fprintf(stderr, "Test \"%s\" SUCCEEDED!\n", test_id);

      continue;
    }
    else {
      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        break;
      }

      if (strcmp(tmp, "START") != 0) {
        fprintf(stderr, "Test \"%s\" FAILED - START/-a/-y/-n is missing in \"%s\".\n", test_id, argv[1]);
        failures = 1;
        break;
      }
    }

    byte_count = 0;

    while (1) {
      if (byte_count == bytes_capacity) {
        int new_capacity;
        unsigned char *new_bytes;

        if (bytes_capacity > INT_MAX / 2) {
          fprintf(stderr, "Test \"%s\" FAILED - Test byte buffer would overflow.\n", test_id);
          failures = 1;
          end = 1;
          break;
        }

        new_capacity = bytes_capacity * 2;
        new_bytes = realloc(bytes, new_capacity);

        if (new_bytes == NULL) {
          fprintf(stderr, "Test \"%s\" FAILED - Could not allocate memory for test bytes.\n", test_id);
          failures = 1;
          end = 1;
          break;
        }

        bytes = new_bytes;
        bytes_capacity = new_capacity;
      }

      if (!get_token(&current, tmp)) {
        if (g_get_token_overflow == YES)
          failures = 1;
        end = 1;
        break;
      }

      if (strcmp(tmp, "END") == 0)
        break;

      if (_parse_byte_token(tmp, &bytes[byte_count]) == FAILED) {
        fprintf(stderr, "Test \"%s\" FAILED - Unknown data \"%s\" in test \"%s\"! Must either be a character, two character hexadecimal value, 0d prefixed 8bit decimal value, 0b prefixed 8bit binary value or END.\n", test_id, tmp, test_id);
        failures = 1;
        end = 1;
        break;
      }

      byte_count++;
    }

    /* execute the test */
    if (use_address == NO) {
      if (file_size < 3) {
        fprintf(stderr, "Test \"%s\" FAILED - Could not find tag \"%s>\".\n", test_id, tag_id);
        failures = 1;
        continue;
      }

      for (i = 0; i <= file_size - 3; i++) {
        if (g_binary_file[i] == tag_id[0] && g_binary_file[i+1] == tag_id[1] && g_binary_file[i+2] == '>')
          break;
      }

      if (i > file_size - 3) {
        fprintf(stderr, "Test \"%s\" FAILED - Could not find tag \"%s>\".\n", test_id, tag_id);
        failures = 1;
        continue;
      }

      tag_start = i+3;

      for ( ; i <= file_size - 3; i++) {
        if (g_binary_file[i] == '<' && g_binary_file[i+1] == tag_id[0] && g_binary_file[i+2] == tag_id[1])
          break;
      }

      if (i > file_size - 3) {
        fprintf(stderr, "Test \"%s\" FAILED - Could not find tag \"<%s\".\n", test_id, tag_id);
        failures = 1;
        continue;
      }

      tag_end = i;

      if (tag_end - tag_start != byte_count) {
        fprintf(stderr, "Test \"%s\" FAILED - There is %d bytes between the tags \"%s\", but the test \"%s\" defines %d bytes.\n", test_id, tag_end - tag_start, tag_id, test_id, byte_count);
        failures = 1;
        continue;
      }
    }
    else
      tag_start = address;

    if (tag_start < 0 || tag_start > file_size || byte_count > file_size - tag_start) {
      fprintf(stderr, "Test \"%s\" FAILED - Address range is outside the binary file.\n", test_id);
      failures = 1;
      continue;
    }

    /* compare bytes */
    wrong_bytes = 0;
    for (i = 0; i < byte_count; i++) {
      if (bytes[i] != g_binary_file[tag_start + i]) {
        if (wrong_bytes == 0)
          fprintf(stderr, "Test \"%s\" FAILED - Bytes that don't match: %d", test_id, (i+1));
        else
          fprintf(stderr, ", %d", (i+1));
        fprintf(stderr, " (GOT: $%.2x EXPECTED: $%.2x)", g_binary_file[tag_start + i], bytes[i]);
        wrong_bytes++;
      }
    }

    if (wrong_bytes > 0) {
      failures = 1;
      fprintf(stderr, "\n");
    }
    else
      fprintf(stderr, "Test \"%s\" SUCCEEDED!\n", test_id);
  }

  if (f != NULL)
    fclose(f);

  free(input);
  free(bytes);

  return failures;
}


int main(int argc, char *argv[]) {

  char *tmp = NULL, *test_id = NULL, *tag_id = NULL;
  char **input_names = NULL;
  int input_name_count = 0;
  int i, return_code;

  if (argc < 2 || argv == NULL) {
    fprintf(stderr, "\n");
#if defined(AMIGACPU)
    fprintf(stderr, "Byte tester 2.4 (" AMIGACPU ")\n");
#else
    fprintf(stderr, "Byte tester 2.4\n");
#endif
    fprintf(stderr, "\n");
    fprintf(stderr, "USAGE: %s [-s] <TESTS FILE / SOURCE FILES...>\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "If -s is given then we read tests file data from the comments of source code files.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "TESTS FILE FORMAT:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "...\n");
    fprintf(stderr, "<BINARY FILE NAME>\n");
    fprintf(stderr, "<TEST ID> <TAG ID> START <BYTE 1> <BYTE 2> ... <BYTE N> END /* Require the bytes to be between the tags */\n");
    fprintf(stderr, "<TEST ID> -a <INTEGER ADDRESS> START <BYTE 1> <BYTE 2> ... <BYTE N> END /* Require the bytes to be at the address */\n");
    fprintf(stderr, "<TEST ID> -y <STRING> /* Require that the string is found */\n");
    fprintf(stderr, "<TEST ID> -n <STRING> /* Require that the string is not found */\n");
    fprintf(stderr, "...\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "If <TAG ID> is \"01\", then we test if the bytes between \"01>\" and \"<01\" match with bytes 1...n\n");
    fprintf(stderr, "Byte values can be characters, hexadecimal ($12, 12, 0x12), decimal (0d18), or binary (0b10010).\n");
    fprintf(stderr, "\n");
    return 1;
  }

  input_names = calloc(argc, sizeof(char *));
  if (input_names == NULL) {
    fprintf(stderr, "Error allocating memory for input filenames.\n");
    return 1;
  }

  tmp = calloc(MAX_BYTES_PER_TEST, 1);
  test_id = calloc(MAX_BYTES_PER_TEST, 1);
  tag_id = calloc(MAX_BYTES_PER_TEST, 1);

  if (tmp == NULL || test_id == NULL || tag_id == NULL) {
    fprintf(stderr, "Error allocating memory for test buffers.\n");
    return_code = 1;
  }
  else
    return_code = main_run(argc, argv, input_names, input_name_count, tmp, test_id, tag_id);

  for (i = 0; i < argc; i++) {
    if (input_names[i] != NULL)
      free(input_names[i]);
  }
  free(input_names);
  free(tmp);
  free(test_id);
  free(tag_id);
  free(g_binary_file);

  return return_code;
}

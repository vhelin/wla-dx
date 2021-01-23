
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define YES 1
#define NO  0

#define FAILED    0
#define SUCCEEDED 1

#define TRUE  1
#define FALSE 0


int _get_next_number(char *in, int *out) {

  int i = 0, o, n;

  if (in[i] >= '0' && in[i] <= '9') {
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
      while (TRUE) {
        if (in[i] >= '0' && in[i] <= '9')
          o = (o * 16) + in[i] - '0';
        else if (in[i] >= 'a' && in[i] <= 'f')
          o = (o * 16) + in[i] - 'a' + 0xA;
        else if (in[i] >= 'A' && in[i] <= 'F')
          o = (o * 16) + in[i] - 'A' + 0xA;
        else if (in[i] == 'h' || in[i] == 'H')
          break;
        else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
          break;
        else
          return FAILED;
        i++;
      }
    }
    else {
      /* decimal */
      for (o = 0; in[i] >= '0' && in[i] <= '9'; i++)
        o = (o * 10) + in[i] - '0';
      if (!(in[i] == ' ' || in[i] == 0x09 || in[i] == 0))
        return FAILED;
    }
  }
  else if (in[i] == '$') {
    i++;
    o = 0;
    while (TRUE) {
      if (in[i] >= '0' && in[i] <= '9')
        o = (o * 16) + in[i] - '0';
      else if (in[i] >= 'a' && in[i] <= 'f')
        o = (o * 16) + in[i] - 'a' + 0xA;
      else if (in[i] >= 'A' && in[i] <= 'F')
        o = (o * 16) + in[i] - 'A' + 0xA;
      else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
        break;
      else
        return FAILED;
      i++;
    }
  }
  else
    return FAILED;

  *out = o;

  return SUCCEEDED;
}


unsigned char *binary_file = NULL;


int _read_binary_file(char *filename, int *did_we_read_data, FILE *f, int *file_size) {

  FILE *fb;
  
  fb = fopen(filename, "rb");

  if (fb == NULL) {
    if (binary_file != NULL) {
      *did_we_read_data = NO;
      return SUCCEEDED;
    }
    fprintf(stderr, "Error opening file \"%s\".\n", filename);
    fclose(f);
    return FAILED;
  }

  fseek(fb, 0, SEEK_END);
  *file_size = (int)ftell(fb);
  fseek(fb, 0, SEEK_SET);

  if (binary_file != NULL)
    free(binary_file);
  
  binary_file = calloc(*file_size, 1);
  if (binary_file == NULL) {
    fprintf(stderr, "Error allocating memory for file \"%s\".\n", filename);
    fclose(f);
    fclose(fb);
    *did_we_read_data = NO;
    return FAILED;
  }

  fread(binary_file, 1, *file_size, fb);
  fclose(fb);

  *did_we_read_data = YES;
        
  return SUCCEEDED;
}


int main(int argc, char *argv[]) {

  char tmp[256], test_id[256], tag_id[256];
  unsigned char bytes[256];
  int file_size, end, byte_count, i, j, length, tag_start, tag_end, wrong_bytes, failures, use_address = NO, address = 0, did_we_read_data = NO, got_it = NO;
  FILE *f;
  
  if (argc != 2 || argv == NULL) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Byte tester 1.0\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "USAGE: %s <TESTS FILE>\n", argv[0]);
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
    fprintf(stderr, "\n");
    return 1;
  }

  f = fopen(argv[1], "rb");

  if (f == NULL) {
    fprintf(stderr, "Error opening file \"%s\".\n", argv[1]);
    return 1;
  }

  fscanf(f, "%255s", tmp);

  /* read the binary file */
  if (_read_binary_file(tmp, &did_we_read_data, f, &file_size) == FAILED)
    return 1;
  
  /* execute the tests */
  failures = 0;
  end = 0;
  while (end == 0) {
    if (fscanf(f, "%255s", test_id) == EOF)
      break;
    /* test_id could be filename */
    if (_read_binary_file(test_id, &did_we_read_data, f, &file_size) == FAILED)
      return 1;
    if (did_we_read_data == YES)
      continue;
    if (fscanf(f, "%255s", tag_id) == EOF)
      break;

    if (strlen(tag_id) != 2) {
      fprintf(stderr, "Test \"%s\" FAILED - TAG ID must be exactly two characters long. Error in \"%s\".\n", test_id, tag_id);
      failures = 1;
      break;
    }

    if (strcmp(tag_id, "-a") == 0) {
      use_address = YES;

      if (fscanf(f, "%255s", tmp) == EOF)
        break;

      if (_get_next_number(tmp, &address) == FAILED) {
        fprintf(stderr, "\"%s\" is not a number.\n", tmp);
        failures = 1;
        break;
      }

      if (fscanf(f, "%255s", tmp) == EOF)
        break;

      if (strcmp(tmp, "START") != 0) {
        fprintf(stderr, "Test \"%s\" FAILED - START is missing in \"%s\".\n", test_id, argv[1]);
        failures = 1;
        break;
      }
    }
    else if (strcmp(tag_id, "-y") == 0) {
      if (fscanf(f, "%255s", tmp) == EOF)
        break;

      length = (int)strlen(tmp);
      got_it = NO;

      for (i = 0; i < file_size; i++) {
        for (j = 0; j < length; j++) {
          if (binary_file[i+j] != tmp[j])
            break;
        }
        if (j == length) {
          /* we found the string -> ok! */
          got_it = YES;
          break;
        }
      }

      if (got_it == YES) {
        fprintf(stderr, "Test \"%s\" SUCCEEDED!\n", test_id);
        continue;
      }
    }
    else if (strcmp(tag_id, "-n") == 0) {
      if (fscanf(f, "%255s", tmp) == EOF)
        break;

      length = (int)strlen(tmp);
      got_it = NO;
      
      for (i = 0; i < file_size; i++) {
        for (j = 0; j < length; j++) {
          if (binary_file[i+j] != tmp[j])
            break;
        }
        if (j == length) {
          /* we found the string -> not ok! */
          got_it = YES;
          break;
        }
      }

      if (got_it == YES)
        return 1;

      fprintf(stderr, "Test \"%s\" SUCCEEDED!\n", test_id);

      continue;
    }
    else {
      if (fscanf(f, "%255s", tmp) == EOF)
        break;

      if (strcmp(tmp, "START") != 0) {
        fprintf(stderr, "Test \"%s\" FAILED - START/-a/-y/-n is missing in \"%s\".\n", test_id, argv[1]);
        failures = 1;
        break;
      }
    }
    
    byte_count = 0;

    while (1) {
      if (byte_count == 256) {
        fprintf(stderr, "Test \"%s\" FAILED - Each test can contain max 256 bytes.\n", test_id);
        failures = 1;
        end = 1;
        break;
      }
      
      if (fscanf(f, "%255s", tmp) == EOF) {
        end = 1;
        break;
      }
      
      if (strcmp(tmp, "END") == 0)
        break;

      if (strlen(tmp) == 1)
        bytes[byte_count] = tmp[0];
      else if (strlen(tmp) == 2)
        bytes[byte_count] = (unsigned char)strtol(tmp, NULL, 16);
      else {
        fprintf(stderr, "Test \"%s\" FAILED - Unknown data \"%s\" in test \"%s\"! Must either be a character, two character hexadecimal value or END.\n", test_id, tmp, test_id);
        failures = 1;
        end = 1;
        break;
      }

      byte_count++;
    }

    /* execute the test */
    if (use_address == NO) {
      for (i = 0; i < file_size - 2; i++) {
        if (binary_file[i] == tag_id[0] && binary_file[i+1] == tag_id[1] && binary_file[i+2] == '>')
          break;
      }

      if (i == file_size - 2) {
        fprintf(stderr, "Test \"%s\" FAILED - Could not find tag \"%s>\".\n", test_id, tag_id);
        failures = 1;
        continue;
      }

      tag_start = i+3;

      for ( ; i < file_size - 2; i++) {
        if (binary_file[i] == '<' && binary_file[i+1] == tag_id[0] && binary_file[i+2] == tag_id[1])
          break;
      }

      if (i == file_size - 2) {
        fprintf(stderr, "Test \"%s\" FAILED - Could not find tag \"<%s\".\n", test_id, tag_id);
        failures = 1;
        continue;
      }

      tag_end = i;

      if (tag_end - tag_start != byte_count) {
        fprintf(stderr, "Test \"%s\" FAILED - There is %d bytes between the tags \"%s\", but the test \"%s\" defines only %d bytes.\n", test_id, tag_end - tag_start, tag_id, test_id, byte_count);
        failures = 1;
        continue;
      }
    }
    else
      tag_start = address;
    
    /* compare bytes */
    wrong_bytes = 0;
    for (i = 0; i < byte_count; i++) {
      if (bytes[i] != binary_file[tag_start + i]) {
        if (wrong_bytes == 0)
          fprintf(stderr, "Test \"%s\" FAILED - Bytes that don't match: %d", test_id, (i+1));
        else
          fprintf(stderr, ", %d", (i+1));
        fprintf(stderr, " (GOT: $%.2x EXPECTED: $%.2x)", binary_file[tag_start + i], bytes[i]);
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
  
  fclose(f);
  
  free(binary_file);
  
  return failures;
}

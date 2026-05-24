
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "defines.h"
#include "listfile.h"
#include "files.h"
#include "main.h"


extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct section *g_sec_first, *g_sec_last, *g_sec_bankhd_first, *g_sec_bankhd_last;
extern struct slot g_slots[256];
extern unsigned char *g_rom;
extern int *g_bankaddress;
extern int g_listfile_next_to_object;
extern char g_version_string[];


/* read an integer from t */
#define READ_T t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24); t += 4;


static int _strings_equal(const char *a, const char *b) {

  if (a == NULL || b == NULL)
    return a == b;

  return strcmp(a, b) == 0;
}


static char *_string_duplicate(const char *source) {

  char *copy;
  int length;

  if (source == NULL)
    source = "";

  length = (int)strlen(source) + 1;
  copy = calloc(length, 1);
  if (copy == NULL)
    return NULL;

  memcpy(copy, source, length);

  return copy;
}


static const char *_get_file_name_part(const char *path) {

  const char *last_separator = NULL, *current;

  if (path == NULL)
    return "";

  for (current = path; *current != 0; current++) {
    if (*current == '/' || *current == '\\')
      last_separator = current;
  }

  if (last_separator == NULL)
    return path;

  return last_separator + 1;
}


static int _source_basename_is_unique(struct listfileitem **listfileitems_ptr, int count, int start, char *output_file_name, char *source_file_name) {

  const char *source_base;
  int index;

  source_base = _get_file_name_part(source_file_name);
  if (source_base[0] == 0)
    return NO;

  for (index = start; index < count && _strings_equal(listfileitems_ptr[index]->outputfilename, output_file_name) == YES; index++) {
    if (_strings_equal(listfileitems_ptr[index]->sourcefilename, source_file_name) == NO && strcmp(_get_file_name_part(listfileitems_ptr[index]->sourcefilename), source_base) == 0)
      return NO;
  }

  return YES;
}


static int _replace_extension_with_lst(const char *path, char *output, int output_size) {

  int index, extension_index = -1, length;

  if (path == NULL)
    path = "";

  if ((int)strlen(path) >= output_size)
    return FAILED;

  strcpy(output, path);
  for (index = (int)strlen(output) - 1; index >= 0; index--) {
    if (output[index] == '/' || output[index] == '\\')
      break;
    if (output[index] == '.') {
      extension_index = index;
      break;
    }
  }

  if (extension_index >= 0)
    output[extension_index] = 0;

  length = (int)strlen(output);
  if (length + 4 >= output_size)
    return FAILED;

  strcpy(&output[length], ".lst");

  return SUCCEEDED;
}


static void _free_output_names(struct listfile_output_name *output_names, int output_name_count) {

  int index;

  if (output_names == NULL)
    return;

  for (index = 0; index < output_name_count; index++) {
    if (output_names[index].outputfilename != NULL)
      free(output_names[index].outputfilename);
  }

  free(output_names);
}


static int _assign_output_names(struct listfileitem **listfileitems_ptr, int count, struct listfile_output_name **output_names_out, int *output_name_count_out) {

  struct listfile_output_name *output_names;
  int item_index, output_name_count = 0;

  output_names = calloc((size_t)count, sizeof(struct listfile_output_name));
  if (output_names == NULL) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  for (item_index = 0; item_index < count; item_index++) {
    struct listfileitem *item = listfileitems_ptr[item_index];
    char output_file_name[1024];
    int mode, name_index, matching_index = -1;

    output_file_name[0] = 0;

    if (g_listfile_next_to_object == YES && item->owner != NULL && item->owner->name != NULL) {
      mode = LISTFILE_OUTPUT_OBJECT;
      for (name_index = 0; name_index < output_name_count; name_index++) {
        if (output_names[name_index].mode == mode && output_names[name_index].owner == item->owner) {
          matching_index = name_index;
          break;
        }
      }

      if (matching_index < 0) {
        if (_replace_extension_with_lst(item->owner->name, output_file_name, sizeof(output_file_name)) == FAILED) {
          print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not build listfile path for object file \"%s\".\n", item->owner->name);
          _free_output_names(output_names, output_name_count);
          return FAILED;
        }

        output_names[output_name_count].mode = mode;
        output_names[output_name_count].owner = item->owner;
        output_names[output_name_count].sourcefilename = item->sourcefilename;
        output_names[output_name_count].outputfilename = _string_duplicate(output_file_name);
        if (output_names[output_name_count].outputfilename == NULL) {
          print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
          _free_output_names(output_names, output_name_count);
          return FAILED;
        }
        matching_index = output_name_count++;
      }
    }
    else {
      mode = LISTFILE_OUTPUT_SOURCE;
      for (name_index = 0; name_index < output_name_count; name_index++) {
        if (output_names[name_index].mode == mode && _strings_equal(output_names[name_index].sourcefilename, item->sourcefilename) == YES) {
          matching_index = name_index;
          break;
        }
      }

      if (matching_index < 0) {
        if (_replace_extension_with_lst(item->sourcefilename, output_file_name, sizeof(output_file_name)) == FAILED) {
          print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not build listfile path for source file \"%s\".\n", item->sourcefilename);
          _free_output_names(output_names, output_name_count);
          return FAILED;
        }

        output_names[output_name_count].mode = mode;
        output_names[output_name_count].sourcefilename = item->sourcefilename;
        output_names[output_name_count].outputfilename = _string_duplicate(output_file_name);
        if (output_names[output_name_count].outputfilename == NULL) {
          print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
          _free_output_names(output_names, output_name_count);
          return FAILED;
        }
        matching_index = output_name_count++;
      }
    }

    item->outputfilename = output_names[matching_index].outputfilename;
  }

  *output_names_out = output_names;
  *output_name_count_out = output_name_count;

  return SUCCEEDED;
}


static int _read_source_file(char *source_file_name, char **source_file_out, int *file_size_out, int tolerate_missing) {

  FILE *file;
  char *source_file;
  long file_size_long;
  int file_size;
  size_t bytes_read;

  *source_file_out = NULL;
  *file_size_out = 0;

  file = fopen(source_file_name, "rb");
  if (file == NULL) {
    if (tolerate_missing == YES) {
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not open source file \"%s\" for reading; omitting source text.\n", source_file_name);
      return SUCCEEDED;
    }

    print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not open source file \"%s\" for reading.\n", source_file_name);
    return FAILED;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not seek to the end of source file \"%s\": %s.\n", source_file_name, strerror(errno));
    fclose(file);
    return FAILED;
  }

  file_size_long = ftell(file);
  if (file_size_long < 0) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not determine size of source file \"%s\": %s.\n", source_file_name, strerror(errno));
    fclose(file);
    return FAILED;
  }
  if (file_size_long > INT_MAX) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Source file \"%s\" is too large for listfile generation.\n", source_file_name);
    fclose(file);
    return FAILED;
  }
  file_size = (int)file_size_long;

  if (fseek(file, 0, SEEK_SET) != 0) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not seek to the start of source file \"%s\": %s.\n", source_file_name, strerror(errno));
    fclose(file);
    return FAILED;
  }

  source_file = calloc((size_t)file_size + 1, 1);
  if (source_file == NULL) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    fclose(file);
    return FAILED;
  }

  errno = 0;
  bytes_read = fread(source_file, 1, (size_t)file_size, file);
  if (bytes_read != (size_t)file_size) {
    if (ferror(file) != 0)
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not read all %d bytes of \"%s\": %s.\n", file_size, source_file_name, strerror(errno));
    else
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not read all %d bytes of \"%s\": unexpected end of file.\n", file_size, source_file_name);
    fclose(file);
    free(source_file);
    return FAILED;
  }

  fclose(file);

  *source_file_out = source_file;
  *file_size_out = file_size;

  return SUCCEEDED;
}


static int _source_context_open(struct listfile_source_context *context, char *source_file_name, int tolerate_missing) {

  memset(context, 0, sizeof(struct listfile_source_context));
  context->sourcefilename = source_file_name;

  return _read_source_file(source_file_name, &context->source_file, &context->file_size, tolerate_missing);
}


static void _source_context_free(struct listfile_source_context *context) {

  if (context->source_file != NULL)
    free(context->source_file);
}


static int _source_is_line_break(char c) {

  return c == '\n' || c == '\r';
}


static void _source_buffer_skip_line_break(char *source_file, int file_size, int *position, int *current_linenumber) {

  char line_break;

  line_break = source_file[(*position)++];
  if (*position < file_size) {
    char next = source_file[*position];
    if ((line_break == '\r' && next == '\n') || (line_break == '\n' && next == '\r'))
      (*position)++;
  }

  (*current_linenumber)++;
}


static void _source_context_skip_line_break(struct listfile_source_context *context) {

  _source_buffer_skip_line_break(context->source_file, context->file_size, &context->m, &context->current_linenumber);
}


static int _source_line_length_at(struct listfile_source_context *context, int position) {

  int length = 0;

  while (position + length < context->file_size && _source_is_line_break(context->source_file[position + length]) == NO)
    length++;

  return length;
}


static int _source_next_line_position(struct listfile_source_context *context, int position) {

  while (position < context->file_size && _source_is_line_break(context->source_file[position]) == NO)
    position++;

  if (position < context->file_size) {
    char line_break = context->source_file[position++];
    if (position < context->file_size) {
      char next = context->source_file[position];
      if ((line_break == '\r' && next == '\n') || (line_break == '\n' && next == '\r'))
        position++;
    }
  }

  return position;
}


static void _write_source_line_from_context(FILE *f, struct listfile_source_context *context, int chars) {

  int o;

  if (context == NULL || context->source_file == NULL)
    return;

  for (o = 0; o < chars; o++)
    fprintf(f, " ");

  while (context->m < context->file_size) {
    if (_source_is_line_break(context->source_file[context->m]) == YES) {
      _source_context_skip_line_break(context);
      fprintf(f, "\n");
      return;
    }
    else
      fprintf(f, "%c", context->source_file[context->m++]);
  }

  context->current_linenumber++;
  fprintf(f, "\n");
}


static void _write_source_tail_from_context(FILE *f, struct listfile_source_context *context) {

  if (context == NULL || context->source_file == NULL)
    return;

  while (context->m < context->file_size) {
    if (_source_is_line_break(context->source_file[context->m]) == YES) {
      _source_context_skip_line_break(context);
      fprintf(f, "\n");
      return;
    }
    else
      fprintf(f, "%c", context->source_file[context->m++]);
  }

  context->current_linenumber++;
  fprintf(f, "\n");
}


static void _write_source_until_line(FILE *f, struct listfile_source_context *context, int line_number, int chars) {

  while (context != NULL && context->source_file != NULL && context->current_linenumber < line_number - 1 && context->m < context->file_size)
    _write_source_line_from_context(f, context, chars);
}


static void _write_source_rest(FILE *f, struct listfile_source_context *context, int chars) {

  while (context != NULL && context->source_file != NULL && context->m < context->file_size)
    _write_source_line_from_context(f, context, chars);
}


static int _line_contains_text(const char *line, int line_length, const char *text) {

  int text_length, i;

  if (line == NULL || text == NULL)
    return NO;

  text_length = (int)strlen(text);
  if (text_length <= 0 || line_length < text_length)
    return NO;

  for (i = 0; i <= line_length - text_length; i++) {
    if (strncmp(&line[i], text, text_length) == 0)
      return YES;
  }

  return NO;
}


static int _line_starts_with_directive(const char *line, int line_length, const char *directive, int allow_without_dot) {

  int directive_length, i = 0, j;

  while (i < line_length && isspace((unsigned char)line[i]))
    i++;
  if (i < line_length && line[i] == '.') {
    i++;
  }
  else if (allow_without_dot == NO)
    return NO;

  directive_length = (int)strlen(directive);
  if (line_length - i < directive_length)
    return NO;

  for (j = 0; j < directive_length; j++) {
    if (tolower((unsigned char)line[i + j]) != tolower((unsigned char)directive[j]))
      return NO;
  }

  if (i + directive_length < line_length) {
    unsigned char next = (unsigned char)line[i + directive_length];
    if (isalnum(next) || next == '_')
      return NO;
  }

  return YES;
}


static int _source_line_is_include_directive(const char *line, int line_length) {

  if (_line_starts_with_directive(line, line_length, "include", YES) == YES)
    return YES;
  if (_line_starts_with_directive(line, line_length, "inc", NO) == YES)
    return YES;

  return NO;
}


static int _source_line_references_file(const char *line, int line_length, char *target_source_file_name, int target_base_is_unique) {

  const char *target_base;

  if (target_source_file_name == NULL)
    return NO;

  if (_line_contains_text(line, line_length, target_source_file_name) == YES)
    return YES;

  if (target_base_is_unique == NO)
    return NO;

  target_base = _get_file_name_part(target_source_file_name);
  if (_line_contains_text(line, line_length, target_base) == YES)
    return YES;

  return NO;
}


static int _find_next_include_position(struct listfile_source_context *context, char *target_source_file_name, int target_base_is_unique, int *include_position) {

  int fallback_position = -1, position;

  *include_position = -1;
  if (context == NULL || context->source_file == NULL || target_source_file_name == NULL)
    return NO;

  position = context->m;
  while (position < context->file_size) {
    const char *line_start = &context->source_file[position];
    int line_length = _source_line_length_at(context, position);

    if (_source_line_is_include_directive(line_start, line_length) == YES) {
      if (_source_line_references_file(line_start, line_length, target_source_file_name, target_base_is_unique) == YES) {
        *include_position = position;
        return YES;
      }
      if (fallback_position < 0)
        fallback_position = position;
    }

    position = _source_next_line_position(context, position);
  }

  if (fallback_position >= 0) {
    *include_position = fallback_position;
    return YES;
  }

  return NO;
}


static int _write_source_until_include(FILE *f, struct listfile_source_context *context, char *target_source_file_name, int target_base_is_unique, int chars) {

  int include_position, line_position;

  if (_find_next_include_position(context, target_source_file_name, target_base_is_unique, &include_position) == NO) {
    while (context != NULL && context->source_file != NULL && context->m < context->file_size)
      _write_source_line_from_context(f, context, chars);
    return NO;
  }

  while (context != NULL && context->source_file != NULL && context->m < context->file_size) {
    line_position = context->m;
    _write_source_line_from_context(f, context, chars);
    if (line_position == include_position)
      return YES;
  }

  return NO;
}


static void _write_source_marker(FILE *f, char *source_file_name) {

  fprintf(f, "\n; --- file: %s ---\n", source_file_name);
}


static int _create_interleaved_context(struct listfile_source_context *contexts, int max_contexts, int *context_count, char *source_file_name, struct listfile_source_context **context) {

  if (*context_count >= max_contexts) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Internal source context stack overflow. Please submit a bug report!\n");
    return FAILED;
  }

  *context = &contexts[*context_count];
  (*context_count)++;

  if (_source_context_open(*context, source_file_name, YES) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _switch_interleaved_source(FILE *f, char *target_source_file_name, int target_base_is_unique, int chars, struct listfile_source_context *contexts, int max_contexts, int *context_count, struct listfile_source_context **source_stack, int *source_stack_count, struct listfile_source_context **current_context) {

  while (*current_context != NULL && _strings_equal((*current_context)->sourcefilename, target_source_file_name) == NO) {
    if (_write_source_until_include(f, *current_context, target_source_file_name, target_base_is_unique, chars) == YES) {
      if (*source_stack_count >= max_contexts) {
        print_text(NO, "LISTFILE_WRITE_LISTFILES: Internal source context stack overflow. Please submit a bug report!\n");
        return FAILED;
      }
      source_stack[*source_stack_count] = *current_context;
      (*source_stack_count)++;

      if (_create_interleaved_context(contexts, max_contexts, context_count, target_source_file_name, current_context) == FAILED)
        return FAILED;
      _write_source_marker(f, target_source_file_name);
      return SUCCEEDED;
    }

    if (*source_stack_count > 0) {
      (*source_stack_count)--;
      *current_context = source_stack[*source_stack_count];
      _write_source_marker(f, (*current_context)->sourcefilename);
    }
    else
      *current_context = NULL;
  }

  if (*current_context == NULL) {
    if (_create_interleaved_context(contexts, max_contexts, context_count, target_source_file_name, current_context) == FAILED)
      return FAILED;
    _write_source_marker(f, target_source_file_name);
  }

  return SUCCEEDED;
}


static int _listfile_write_hex(FILE *f, int data) {

  if (data >= 10)
    fprintf(f, "%c", data + 'A' - 10);
  else
    fprintf(f, "%c", data + '0');

  return SUCCEEDED;
}


static int _listfileitem_sort(const void *a, const void *b) {

  struct listfileitem *sa, *sb;
  int comparison;

  sa = *((struct listfileitem **)a);
  sb = *((struct listfileitem **)b);

  comparison = strcmp(sa->outputfilename, sb->outputfilename);
  if (comparison != 0)
    return comparison;

  if (g_listfile_next_to_object == YES) {
    if (sa->running_id > sb->running_id)
      return 1;
    else if (sa->running_id < sb->running_id)
      return -1;

    comparison = strcmp(sa->sourcefilename, sb->sourcefilename);
    if (comparison != 0)
      return comparison;

    if (sa->linenumber > sb->linenumber)
      return 1;
    else if (sa->linenumber < sb->linenumber)
      return -1;

    return 0;
  }

  comparison = strcmp(sa->sourcefilename, sb->sourcefilename);
  if (comparison != 0)
    return comparison;

  if (sa->linenumber > sb->linenumber)
    return 1;
  else if (sa->linenumber < sb->linenumber)
    return -1;

  if (sa->running_id > sb->running_id)
    return 1;
  else if (sa->running_id < sb->running_id)
    return -1;
    
  return 0;
}


static int _get_pc(int slot, int offset) {

  return g_slots[slot].address + offset;
}


static void _write_listfile_item(FILE *f, struct listfileitem *item, struct listfile_source_context *source_context, int chars, char cpu_65816) {

  int is_behind = NO, o, p, wrote_line;

  _write_source_until_line(f, source_context, item->linenumber, chars);

  if (source_context != NULL && source_context->source_file != NULL && source_context->current_linenumber > item->linenumber - 1)
    is_behind = YES;
  if (source_context != NULL && source_context->source_file != NULL && source_context->m >= source_context->file_size && source_context->current_linenumber < item->linenumber - 1)
    is_behind = YES;

  p = 0;

  fprintf(f, "%*d ", 5, item->real_linenumber);
  p += 6;
  if (cpu_65816 == YES) {
    fprintf(f, "%.4X ", item->base);
    p += 5;
  }
  fprintf(f, "%.4X ", item->bank);
  p += 5;
  fprintf(f, "%.4X ", item->slot);
  p += 5;
  fprintf(f, "%.4X ", _get_pc(item->slot, item->offset));
  p += 5;
  fprintf(f, "%.4X   ", item->offset);
  p += 7;

  wrote_line = NO;
  for (o = 0; o < item->length; o++) {
    struct section *s2 = item->section;

    if (s2 != NULL && s2->is_bankheader_section == YES) {
      _listfile_write_hex(f, item->section->data[item->address + o] >> 4);
      _listfile_write_hex(f, item->section->data[item->address + o] & 15);
    }
    else {
      if (s2 != NULL && s2->appended_to == YES) {
        struct section *s3 = s2->appended_to_section;
        int address_new = item->address - s2->output_address + s3->output_address + s2->appended_to_offset;
        while (s3->appended_to == YES) {
          struct section *s4 = s3->appended_to_section;
          address_new = address_new - s3->output_address + s4->output_address + s3->appended_to_offset;
          s3 = s4;
        }

        _listfile_write_hex(f, g_rom[address_new + o] >> 4);
        _listfile_write_hex(f, g_rom[address_new + o] & 15);
      }
      else {
        _listfile_write_hex(f, g_rom[item->address + o] >> 4);
        _listfile_write_hex(f, g_rom[item->address + o] & 15);
      }
    }
    fprintf(f, " ");
    p += 3;
    if ((o % 10) == 9 && o != 0 && o < item->length - 1) {
      if (wrote_line == NO) {
        wrote_line = YES;
        while (p < chars) {
          fprintf(f, " ");
          p++;
        }

        if (is_behind == YES || source_context == NULL || source_context->source_file == NULL)
          fprintf(f, "\n");
        else
          _write_source_tail_from_context(f, source_context);
      }
      else
        fprintf(f, "\n");

      p = 0;
    }
  }

  if (is_behind == YES || source_context == NULL || source_context->source_file == NULL)
    fprintf(f, "\n");
  else {
    if (wrote_line == NO) {
      while (p < chars) {
        fprintf(f, " ");
        p++;
      }

      _write_source_tail_from_context(f, source_context);
    }
    else
      fprintf(f, "\n");
  }
}


static int _write_interleaved_listfile(FILE *f, struct listfileitem **listfileitems_ptr, int count, int *j, char *output_file_name, int chars, char cpu_65816) {

  struct listfile_source_context *source_contexts, *current_context = NULL;
  struct listfile_source_context **source_stack;
  int context_count = 0, group_end, group_start, source_stack_count = 0, max_contexts, start;

  group_start = *j;
  group_end = group_start;
  while (group_end < count && _strings_equal(listfileitems_ptr[group_end]->outputfilename, output_file_name) == YES)
    group_end++;
  max_contexts = group_end - group_start + 1;
  source_contexts = calloc((size_t)max_contexts, sizeof(struct listfile_source_context));
  source_stack = calloc((size_t)max_contexts, sizeof(struct listfile_source_context *));
  if (source_contexts == NULL || source_stack == NULL) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    if (source_contexts != NULL)
      free(source_contexts);
    if (source_stack != NULL)
      free(source_stack);
    return FAILED;
  }

  while (*j < count && _strings_equal(listfileitems_ptr[*j]->outputfilename, output_file_name) == YES) {
    struct listfileitem *item = listfileitems_ptr[*j];
    int target_base_is_unique = _source_basename_is_unique(listfileitems_ptr, count, group_start, output_file_name, item->sourcefilename);

    if (_switch_interleaved_source(f, item->sourcefilename, target_base_is_unique, chars, source_contexts, max_contexts, &context_count, source_stack, &source_stack_count, &current_context) == FAILED) {
      int i;
      for (i = 0; i < context_count; i++)
        _source_context_free(&source_contexts[i]);
      free(source_contexts);
      free(source_stack);
      return FAILED;
    }

    _write_listfile_item(f, item, current_context, chars, cpu_65816);
    (*j)++;
  }

  while (current_context != NULL) {
    _write_source_rest(f, current_context, chars);
    if (source_stack_count > 0) {
      source_stack_count--;
      current_context = source_stack[source_stack_count];
      _write_source_marker(f, current_context->sourcefilename);
    }
    else
      current_context = NULL;
  }

  for (start = 0; start < context_count; start++)
    _source_context_free(&source_contexts[start]);
  free(source_contexts);
  free(source_stack);

  return SUCCEEDED;
}


int listfile_write_listfiles(void) {

  struct listfileitem *listfileitems, **listfileitems_ptr;
  struct listfile_output_name *output_names = NULL;
  struct section *s, **selected_sections;
  struct object_file *obj;
  int count, i, j, current_linenumber, m, o, p, source_file_id = -1, wrote_line, listfile_item_count = 0, section_count = 0, chars = 60;
  int output_name_count = 0;
  char command, *source_file, *source_file_name, *output_file_name, cpu_65816 = NO;

  /* count the listfile items */
  s = g_sec_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else {
      listfile_item_count += s->listfile_items;
      section_count++;
    }

    s = s->next;
  }
  s = g_sec_bankhd_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else {
      listfile_item_count += s->listfile_items;
      section_count++;
    }

    s = s->next;
  }
  obj = g_obj_first;
  while (obj != NULL) {
    listfile_item_count += obj->listfile_items;
    obj = obj->next;
  }
  
  if (listfile_item_count <= 0) {
    /* no listfile data, so we cannot write listfiles... */
    return SUCCEEDED;
  }

  listfileitems = calloc((size_t)listfile_item_count, sizeof(struct listfileitem));
  if (listfileitems == NULL) {
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  listfileitems_ptr = calloc((size_t)listfile_item_count, sizeof(struct listfileitem *));
  if (listfileitems_ptr == NULL) {
    free(listfileitems);
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  selected_sections = calloc((size_t)section_count, sizeof(struct section *));
  if (selected_sections == NULL && section_count > 0) {
    free(listfileitems);
    free(listfileitems_ptr);
    print_text(NO, "LISTFILE_WRITE_LISTFILES: Out of memory error.\n");
    return FAILED;
  }

  for (i = 0; i < listfile_item_count; i++)
    listfileitems_ptr[i] = &listfileitems[i];
  
  /* collect the valid sections */
  j = 0;
  s = g_sec_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else
      selected_sections[j++] = s;

    s = s->next;
  }
  s = g_sec_bankhd_first;
  while (s != NULL) {
    if (s->listfile_items <= 0 || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
        s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
    }
    else
      selected_sections[j++] = s;

    s = s->next;
  }  

  /* collect the list file items from the data */
  count = 0;
  for (i = 0; i < section_count; i++) {
    int add = 0;
    
    s = selected_sections[i];
    
    /* parse the list file information */
    for (j = 0; j < s->listfile_items; j++) {
      command = s->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (s->listfile_ints[j*5 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(s->file_id, source_file_id);
          listfileitems[count].owner = get_file(s->file_id);
          listfileitems[count].linenumber = s->listfile_ints[j*5 + 0];
          listfileitems[count].length = s->listfile_ints[j*5 + 1];
          listfileitems[count].section = s;
          listfileitems[count].base = s->base;
          listfileitems[count].bank = s->bank;
          listfileitems[count].slot = s->slot;
          add += s->listfile_ints[j*5 + 2];
          listfileitems[count].offset = add + s->address;
          listfileitems[count].address = s->output_address + add;
          add += s->listfile_ints[j*5 + 1];
          listfileitems[count].running_id = s->listfile_ints[j*5 + 3];
          listfileitems[count].real_linenumber = s->listfile_ints[j*5 + 4];

          /* fix base, bank, slot, and offset for sections that were appended */
          if (s->appended_to == YES) {
            /* this loop finds the target of possibly chained appendto secions */
            struct section *s3 = s->appended_to_section;
            int offset_new = listfileitems[count].offset - s->address + s3->address + s->appended_to_offset;
            while (s3->appended_to == YES) {
              struct section *s4 = s3->appended_to_section;
              offset_new = offset_new - s3->address + s4->address + s3->appended_to_offset;
              s3 = s4;
            }

            listfileitems[count].offset = offset_new;
            listfileitems[count].base = s3->base;
            listfileitems[count].bank = s3->bank;
            listfileitems[count].slot = s3->slot;
          }

          count++;
          /*
          print_text(NO, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          print_text(NO, "LFI: k SKIPPED\n");
          */

          if (add == -0x12345678) {
            print_text(NO, "LISTFILE_WRITE_LISTFILES: add == -0x12345678! Internal error! Please submit a bug report!\n");
            free(listfileitems);
            free(selected_sections);
            return FAILED;
          }

          add += s->listfile_ints[j*5 + 2];
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = s->listfile_ints[j*5 + 0];
        /*
        print_text(NO, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        print_text(NO, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
        free(listfileitems);
        free(selected_sections);
        return FAILED;
      }
    }
  }
  obj = g_obj_first;
  while (obj != NULL) {
    if (obj->cpu_65816 == YES)
      cpu_65816 = YES;
    for (j = 0; j < obj->listfile_items; j++) {
      command = obj->listfile_cmds[j];
      if (command == 'k') {
        /* new line */
        if (obj->listfile_ints[j*8 + 1] > 0) {
          listfileitems[count].sourcefilename = get_source_file_name(obj->id, source_file_id);
          listfileitems[count].owner = obj;
          listfileitems[count].linenumber = obj->listfile_ints[j*8 + 0];
          listfileitems[count].length = obj->listfile_ints[j*8 + 1];
          listfileitems[count].section = NULL;
          listfileitems[count].base = obj->listfile_ints[j*8 + 3];
          listfileitems[count].bank = obj->listfile_ints[j*8 + 4];
          listfileitems[count].slot = obj->listfile_ints[j*8 + 5];
          listfileitems[count].offset = obj->listfile_ints[j*8 + 2];
          listfileitems[count].address = g_bankaddress[listfileitems[count].bank] + listfileitems[count].offset;
          listfileitems[count].running_id = obj->listfile_ints[j*8 + 6];
          listfileitems[count].real_linenumber = obj->listfile_ints[j*8 + 7];
          count++;
          /*
          print_text(NO, "LFI: k %s %d %d %x\n", listfileitems[count-1].sourcefilename, listfileitems[count-1].linenumber, listfileitems[count-1].length, listfileitems[count-1].address);
          */
        }
        else {
          /* skip */
          /*
          print_text(NO, "LFI: k SKIPPED\n");
          */
        }
      }
      else if (command == 'f') {
        /* another file */
        source_file_id = obj->listfile_ints[j*8 + 0];
        /*
        print_text(NO, "LFI: f FILE_ID %d\n", source_file_id);
        */
      }
      else {
        print_text(NO, "LISTFILE_WRITE_LISTFILES: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", command);
        free(listfileitems);
        free(selected_sections);
        return FAILED;
      }
    }
    obj = obj->next;
  }
  
  free(selected_sections);

  if (count <= 0) {
    free(listfileitems);
    free(listfileitems_ptr);
    return SUCCEEDED;
  }

  if (_assign_output_names(listfileitems_ptr, count, &output_names, &output_name_count) == FAILED) {
    free(listfileitems);
    free(listfileitems_ptr);
    return FAILED;
  }

  /* sort the listfileitems (lexical order) */
  qsort(listfileitems_ptr, count, sizeof(struct listfileitem *), _listfileitem_sort);

  /* 65816 contains "base" information as well */
  if (cpu_65816 == YES)
    chars += 5;
  
  /* write the listfiles */
  j = 0;
  while (j < count) {
    char time_buffer[32];
    struct tm* tm_info;
    time_t timer;
    FILE *f;

    output_file_name = listfileitems_ptr[j]->outputfilename;
    f = fopen(output_file_name, "wb");
    if (f == NULL) {
      print_text(NO, "LISTFILE_WRITE_LISTFILES: Could not open file \"%s\" for writing.\n", output_file_name);
      free(listfileitems);
      free(listfileitems_ptr);
      _free_output_names(output_names, output_name_count);
      return FAILED;
    }

    fprintf(f, "; WLALINK (%s) listing file\n", g_version_string);

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(time_buffer, 32, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(f, "; %s\n", time_buffer);

    fprintf(f, "\n");
    
    if (cpu_65816 == YES)
      fprintf(f, ";Line Base Bank Slot PC   Offset Hex\n");
    else
      fprintf(f, ";Line Bank Slot PC   Offset Hex                             Source\n");

    if (g_listfile_next_to_object == YES) {
      if (_write_interleaved_listfile(f, listfileitems_ptr, count, &j, output_file_name, chars, cpu_65816) == FAILED) {
        fclose(f);
        free(listfileitems);
        free(listfileitems_ptr);
        _free_output_names(output_names, output_name_count);
        return FAILED;
      }
    }
    else {
      while (j < count && _strings_equal(listfileitems_ptr[j]->outputfilename, output_file_name) == YES) {
        int file_size;

        source_file_name = listfileitems_ptr[j]->sourcefilename;
        source_file = NULL;

        if (_read_source_file(source_file_name, &source_file, &file_size, NO) == FAILED) {
          fclose(f);
          free(listfileitems);
          free(listfileitems_ptr);
          _free_output_names(output_names, output_name_count);
          return FAILED;
        }

        /* write the lines */
        current_linenumber = 0;
        m = 0;

        while (j < count && _strings_equal(listfileitems_ptr[j]->outputfilename, output_file_name) == YES &&
               _strings_equal(listfileitems_ptr[j]->sourcefilename, source_file_name) == YES) {
          int is_behind = NO;

          /* goto line x */
          while (source_file != NULL && current_linenumber < listfileitems_ptr[j]->linenumber-1 && m < file_size) {
            for (o = 0; o < chars; o++)
              fprintf(f, " ");
            while (m < file_size) {
              if (_source_is_line_break(source_file[m]) == YES) {
                _source_buffer_skip_line_break(source_file, file_size, &m, &current_linenumber);
                fprintf(f, "\n");
                break;
              }
              else
                fprintf(f, "%c", source_file[m++]);
            }
          }

          if (source_file != NULL && current_linenumber > listfileitems_ptr[j]->linenumber-1)
            is_behind = YES;
          if (source_file != NULL && m >= file_size && current_linenumber < listfileitems_ptr[j]->linenumber-1)
            is_behind = YES;

          p = 0;

          fprintf(f, "%*d ", 5, listfileitems_ptr[j]->real_linenumber);
          p += 6;
          if (cpu_65816 == YES) {
            fprintf(f, "%.4X ", listfileitems_ptr[j]->base);
            p += 5;
          }
          fprintf(f, "%.4X ", listfileitems_ptr[j]->bank);
          p += 5;
          fprintf(f, "%.4X ", listfileitems_ptr[j]->slot);
          p += 5;
          fprintf(f, "%.4X ", _get_pc(listfileitems_ptr[j]->slot, listfileitems_ptr[j]->offset));
          p += 5;
          fprintf(f, "%.4X   ", listfileitems_ptr[j]->offset);
          p += 7;

          /* write the bytes */
          wrote_line = NO;
          for (o = 0; o < listfileitems_ptr[j]->length; o++) {
            struct section *s2 = listfileitems_ptr[j]->section;
            
            if (s2 != NULL && s2->is_bankheader_section == YES) {
              _listfile_write_hex(f, listfileitems_ptr[j]->section->data[listfileitems_ptr[j]->address + o] >> 4);
              _listfile_write_hex(f, listfileitems_ptr[j]->section->data[listfileitems_ptr[j]->address + o] & 15);
            }
            else {
              if (s2 != NULL && s2->appended_to == YES) {
                /* this loop finds the target of possibly chained appendto secions */
                struct section *s3 = s2->appended_to_section;
                int address_new = listfileitems_ptr[j]->address - s2->output_address + s3->output_address + s2->appended_to_offset;
                while (s3->appended_to == YES) {
                  struct section *s4 = s3->appended_to_section;
                  address_new = address_new - s3->output_address + s4->output_address + s3->appended_to_offset;
                  s3 = s4;
                }
                
                _listfile_write_hex(f, g_rom[address_new + o] >> 4);
                _listfile_write_hex(f, g_rom[address_new + o] & 15);
              }
              else {
                _listfile_write_hex(f, g_rom[listfileitems_ptr[j]->address + o] >> 4);
                _listfile_write_hex(f, g_rom[listfileitems_ptr[j]->address + o] & 15);
              }
            }
            fprintf(f, " ");
            p += 3;
            if ((o % 10) == 9 && o != 0 && o < listfileitems_ptr[j]->length-1) {
              if (wrote_line == NO) {
                /* write padding */
                wrote_line = YES;
                while (p < chars) {
                  fprintf(f, " ");
                  p++;
                }

                if (is_behind == YES || source_file == NULL)
                  fprintf(f, "\n");
                else {
                  /* write the rest of the line */
                  while (m < file_size) {
                    if (_source_is_line_break(source_file[m]) == YES) {
                      _source_buffer_skip_line_break(source_file, file_size, &m, &current_linenumber);
                      fprintf(f, "\n");
                      break;
                    }
                    else
                      fprintf(f, "%c", source_file[m++]);
                  }
                }
              }
              else
                fprintf(f, "\n");
              
              p = 0;
            }
          }
          
          if (is_behind == YES || source_file == NULL)
            fprintf(f, "\n");
          else {
            /* has the line been written already? */
            if (wrote_line == NO) {
              /* write padding */
              while (p < chars) {
                fprintf(f, " ");
                p++;
              }

              /* write the rest of the line */
              while (m < file_size) {
                if (_source_is_line_break(source_file[m]) == YES) {
                  _source_buffer_skip_line_break(source_file, file_size, &m, &current_linenumber);
                  fprintf(f, "\n");
                  break;
                }
                else
                  fprintf(f, "%c", source_file[m++]);
              }
            }
            else
              fprintf(f, "\n");
          }

          j++;
        }

        /* write the rest of the file */
        while (source_file != NULL && m < file_size) {
          for (o = 0; o < chars; o++)
            fprintf(f, " ");
          while (m < file_size) {
            if (_source_is_line_break(source_file[m]) == YES) {
              _source_buffer_skip_line_break(source_file, file_size, &m, &current_linenumber);
              fprintf(f, "\n");
              break;
            }
            else
              fprintf(f, "%c", source_file[m++]);
          }
        }

        if (source_file != NULL)
          free(source_file);
      }
    }

    fclose(f);
  }

  _free_output_names(output_names, output_name_count);
  if (listfileitems != NULL)
    free(listfileitems);
  if (listfileitems_ptr != NULL)
    free(listfileitems_ptr);

  return SUCCEEDED;
}


int listfile_block_read(unsigned char **d, struct section *s) {

  unsigned char *t;
  int i;

  if (d == NULL || s == NULL)
    return FAILED;

  if (**d == 0) {
    /* no listfile information! */
    s->listfile_items = 0;
    s->listfile_cmds = NULL;
    s->listfile_ints = NULL;
    (*d)++;
    return SUCCEEDED;
  }

  /* we have listfile information */
  t = *d;
  t++;
  s->listfile_items = READ_T;
  if (s->listfile_items <= 0 || s->listfile_items > INT_MAX / 5) {
    s->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ: Invalid listfile item count.\n");
    return FAILED;
  }

  s->listfile_cmds = calloc((size_t)s->listfile_items, 1);
  s->listfile_ints = calloc((size_t)s->listfile_items, sizeof(int) * 5);

  if (s->listfile_cmds == NULL || s->listfile_ints == NULL) {
    if (s->listfile_cmds != NULL)
      free(s->listfile_cmds);
    if (s->listfile_ints != NULL)
      free(s->listfile_ints);
    s->listfile_cmds = NULL;
    s->listfile_ints = NULL;
    s->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < s->listfile_items; i++) {
    s->listfile_cmds[i] = *(t++);
    if (s->listfile_cmds[i] == 'k') {
      /* new line */
      s->listfile_ints[i*5 + 0] = READ_T;
      s->listfile_ints[i*5 + 1] = READ_T;
      s->listfile_ints[i*5 + 2] = READ_T;
      s->listfile_ints[i*5 + 3] = READ_T;
      s->listfile_ints[i*5 + 4] = READ_T;
    }
    else if (s->listfile_cmds[i] == 'f') {
      /* file name */
      s->listfile_ints[i*5 + 0] = READ_T;
    }
    else {
      s->listfile_items = 0;
      print_text(NO, "LISTFILE_BLOCK_READ: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", s->listfile_cmds[i]);
      return FAILED;
    }
  }

  *d = t;

  return SUCCEEDED;
}


int listfile_block_read_global(unsigned char **d, struct object_file *obj) {

  unsigned char *t;
  int i;

  if (d == NULL || obj == NULL)
    return FAILED;

  t = *d;
  obj->listfile_items = READ_T;
  if (obj->listfile_items == 0) {
    /* no listfile information! */
    obj->listfile_cmds = NULL;
    obj->listfile_ints = NULL;
    *d = t;
    return SUCCEEDED;
  }

  /* we have listfile information */
  if (obj->listfile_items < 0 || obj->listfile_items > INT_MAX / 8) {
    obj->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ_GLOBAL: Invalid listfile item count.\n");
    return FAILED;
  }

  obj->listfile_cmds = calloc((size_t)obj->listfile_items, 1);
  obj->listfile_ints = calloc((size_t)obj->listfile_items, sizeof(int) * 8);

  if (obj->listfile_cmds == NULL || obj->listfile_ints == NULL) {
    if (obj->listfile_cmds != NULL)
      free(obj->listfile_cmds);
    if (obj->listfile_ints != NULL)
      free(obj->listfile_ints);
    obj->listfile_cmds = NULL;
    obj->listfile_ints = NULL;
    obj->listfile_items = 0;
    print_text(NO, "LISTFILE_BLOCK_READ_GLOBAL: Out of memory error.\n");
    return FAILED;
  }

  /* read the items */
  for (i = 0; i < obj->listfile_items; i++) {
    obj->listfile_cmds[i] = *(t++);
    if (obj->listfile_cmds[i] == 'k') {
      /* new line */
      obj->listfile_ints[i*8 + 0] = READ_T;
      obj->listfile_ints[i*8 + 1] = READ_T;
      obj->listfile_ints[i*8 + 2] = READ_T;
      obj->listfile_ints[i*8 + 3] = READ_T;
      obj->listfile_ints[i*8 + 4] = READ_T;
      obj->listfile_ints[i*8 + 5] = READ_T;
      obj->listfile_ints[i*8 + 6] = READ_T;
      obj->listfile_ints[i*8 + 7] = READ_T;
    }
    else if (obj->listfile_cmds[i] == 'f') {
      /* file name */
      obj->listfile_ints[i*8 + 0] = READ_T;
    }
    else {
      obj->listfile_items = 0;
      print_text(NO, "LISTFILE_BLOCK_READ_GLOBAL: Unknown command '%c'. Internal error. Only known commands are 'k' and 'f'.\n", obj->listfile_cmds[i]);
      return FAILED;
    }
  }

  *d = t;

  return SUCCEEDED;
}

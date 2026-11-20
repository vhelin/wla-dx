#ifndef WLALINK_FILE_READER_H
#define WLALINK_FILE_READER_H

#include <stddef.h>

static int _file_has_bytes(const unsigned char *t, const unsigned char *end, size_t count) {

  return t <= end && count <= (size_t)(end - t);
}


static unsigned long _read_be32(const unsigned char *t) {

  return ((unsigned long)t[0] << 24) | ((unsigned long)t[1] << 16) |
         ((unsigned long)t[2] << 8) | (unsigned long)t[3];
}


/* The caller must have checked that the four bytes are available. */
static int _read_file_int(unsigned char **cursor) {

  unsigned long value = _read_be32(*cursor);

  *cursor += 4;
  if (value > 0x7fffffffUL)
    return (int)(-1L - (long)(0xffffffffUL - value));
  return (int)value;
}

#endif


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"



int get_next_string(char *in, char *out, int *pos, int *type) {

  int i, t;

  *type = LABEL_STATUS_LABEL;
  
  /* skip white space */
  for (i = 0; in[i] == ' ' || in[i] == 0x09; i++)
    ;

  if (in[i] == 0)
    return FAILED;

  /* something between ""? */
  if (in[i] == '"') {
    *type = LABEL_STATUS_STRING;
    i++;
    for (t = 0; in[i] != '"' && in[i] != 0x09 && in[i] != 0; i++, t++)
      out[t] = in[i];
    if (in[i] == '"')
      i++;
  }
  else {
    for (t = 0; in[i] != ' ' && in[i] != 0x09 && in[i] != 0; i++, t++)
      out[t] = in[i];
  }
  
  out[t] = 0;

  if (pos != NULL)
    *pos += i;

  return SUCCEEDED;
}


int get_next_token(char *in, char *out, int *pos) {

  int type;

  return get_next_string(in, out, pos, &type);
}


int get_next_number(char *in, int *out, int *pos) {

  int i, o, n;

  /* skip white space */
  for (i = 0; in[i] == ' ' || in[i] == 0x09; i++);
  if (in[i] == 0)
    return FAILED;

  if (in[i] == '$' || (in[i] == '0' && (in[i+1] == 'x' || in[i+1] == 'X'))) {
    /* hex */
    if (in[i] == '$')
      i++;
    else
      i += 2;
    o = 0;
    while (TRUE) {
      if (in[i] >= '0' && in[i] <= '9')
        o = (o << 4) + in[i] - '0';
      else if (in[i] >= 'a' && in[i] <= 'f')
        o = (o << 4) + in[i] - 'a' + 0xA;
      else if (in[i] >= 'A' && in[i] <= 'F')
        o = (o << 4) + in[i] - 'A' + 0xA;
      else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
        break;
      else
        return FAILED;
      i++;
    }
  }
  else if (in[i] == '%' || (in[i] == '0' && (in[i+1] == 'b' || in[i+1] == 'B'))) {
    /* binary */
    if (in[i] == '%')
      i++;
    else
      i += 2;
    for (o = 0; in[i] != 0; i++) {
      if (in[i] == '0' || in[i] == '1')
        o = (o << 1) + (in[i] - '0');
      else if (in[i] == ' ' || in[i] == 0x09 || in[i] == 0)
        break;
      else
        return FAILED;
    }
  }
  else if ((in[i] >= '0' && in[i] <= '9') || (in[i] >= 'a' && in[i] <= 'f') || (in[i] >= 'A' && in[i] <= 'F')) {
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
          o = (o << 4) + in[i] - '0';
        else if (in[i] >= 'a' && in[i] <= 'f')
          o = (o << 4) + in[i] - 'a' + 0xA;
        else if (in[i] >= 'A' && in[i] <= 'F')
          o = (o << 4) + in[i] - 'A' + 0xA;
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
  else
    return FAILED;

  *out = o;

  if (pos != NULL)
    *pos += i;

  return SUCCEEDED;
}

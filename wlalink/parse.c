
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"



int get_next_token(char *in, char *out, int *pos) {

  int local_i, t;


  /* skip white space */
  for (local_i = 0; in[local_i] == ' ' || in[local_i] == 0x09; local_i++);
  if (in[local_i] == 0)
    return FAILED;

  /* something between ""? */
  if (in[local_i] == '"') {
    local_i++;
    for (t = 0; in[local_i] != '"' && in[local_i] != 0x09 && in[local_i] != 0; local_i++, t++)
      out[t] = in[local_i];
    if (in[local_i] == '"')
      local_i++;
  }
  else {
    for (t = 0; in[local_i] != ' ' && in[local_i] != 0x09 && in[local_i] != 0; local_i++, t++)
      out[t] = in[local_i];
  }
  
  out[t] = 0;

  if (pos != NULL)
    *pos += local_i;

  return SUCCEEDED;
}


int get_next_number(char *in, int *out, int *pos) {

  int local_i, o, n;


  /* skip white space */
  for (local_i = 0; in[local_i] == ' ' || in[local_i] == 0x09; local_i++);
  if (in[local_i] == 0)
    return FAILED;

  if (in[local_i] >= '0' && in[local_i] <= '9') {
    /* is it hex after all? */
    n = 0;
    for (o = 0; 1; o++) {
      if (in[local_i+o] >= '0' && in[local_i+o] <= '9')
        continue;
      if (in[local_i+o] >= 'a' && in[local_i+o] <= 'f') {
        n = 1;
        break;
      }
      if (in[local_i+o] >= 'A' && in[local_i+o] <= 'F') {
        n = 1;
        break;
      }
      if (in[local_i+o] == 'h' || in[local_i+o] == 'H') {
        n = 1;
        break;
      }
      break;
    }

    if (n == 1) {
      /* hex */
      o = 0;
      while (TRUE) {
        if (in[local_i] >= '0' && in[local_i] <= '9')
          o = (o * 16) + in[local_i] - '0';
        else if (in[local_i] >= 'a' && in[local_i] <= 'f')
          o = (o * 16) + in[local_i] - 'a' + 0xA;
        else if (in[local_i] >= 'A' && in[local_i] <= 'F')
          o = (o * 16) + in[local_i] - 'A' + 0xA;
        else if (in[local_i] == 'h' || in[local_i] == 'H')
          break;
        else if (in[local_i] == ' ' || in[local_i] == 0x09 || in[local_i] == 0)
          break;
        else
          return FAILED;
        local_i++;
      }
    }
    else {
      /* decimal */
      for (o = 0; in[local_i] >= '0' && in[local_i] <= '9'; local_i++)
        o = (o * 10) + in[local_i] - '0';
      if (!(in[local_i] == ' ' || in[local_i] == 0x09 || in[local_i] == 0))
        return FAILED;
    }
  }
  else if (in[local_i] == '$') {
    local_i++;
    o = 0;
    while (TRUE) {
      if (in[local_i] >= '0' && in[local_i] <= '9')
        o = (o * 16) + in[local_i] - '0';
      else if (in[local_i] >= 'a' && in[local_i] <= 'f')
        o = (o * 16) + in[local_i] - 'a' + 0xA;
      else if (in[local_i] >= 'A' && in[local_i] <= 'F')
        o = (o * 16) + in[local_i] - 'A' + 0xA;
      else if (in[local_i] == ' ' || in[local_i] == 0x09 || in[local_i] == 0)
        break;
      else
        return FAILED;
      local_i++;
    }
  }
  else
    return FAILED;

  *out = o;

  if (pos != NULL)
    *pos += local_i;

  return SUCCEEDED;
}

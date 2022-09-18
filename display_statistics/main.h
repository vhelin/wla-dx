
#ifndef _MAIN_H
#define _MAIN_H

#define YES 1
#define NO  0

#define SUCCEEDED 1
#define FAILED    0

#define MAX_FUNCTION_NAME_LENGTH 63

struct entry {
  char function_name[MAX_FUNCTION_NAME_LENGTH+1];
  double sum_ms;
  int count;
  struct entry *next;
};

#endif

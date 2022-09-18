
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"


struct entry *g_entries = NULL;


static int _entry_add(char *name, double time_ms) {

  struct entry *e = g_entries;

  while (e != NULL) {
    if (strcmp(e->function_name, name) == 0) {
      e->sum_ms += time_ms;
      e->count++;
      return SUCCEEDED;
    }

    e = e->next;
  }

  /* add a new entry */
  e = calloc(sizeof(struct entry), 1);
  if (e == NULL) {
    fprintf(stderr, "_entry_add(): Out of memory error!\n");
    return FAILED;
  }

  strcpy(e->function_name, name);
  e->sum_ms = time_ms;
  e->count = 1;
  e->next = g_entries;
  g_entries = e;
  
  return SUCCEEDED;
}


static void _entries_free(void) {

  struct entry *e1 = g_entries;

  while (e1 != NULL) {
    struct entry *e2 = e1->next;
    free(e1);
    e1 = e2;
  }
}


int main(int argc, char *argv[]) {

  FILE *file_names, *file_times;
  int times_size, i, j, success, got_header, rounds = 1, entries = 0;
  double *times_ms;
  char **names;
  
  if (argc != 1 || argv == NULL) {
    fprintf(stderr, "\n");
#ifdef AMIGACPU
    fprintf(stderr, "Display statistics 1.0 (" AMIGACPU ")\n");
#else
    fprintf(stderr, "Display statistics 1.0\n");
#endif
    fprintf(stderr, "\n");
    fprintf(stderr, "USAGE: %s\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "Define PROFILE_FUNCTIONS in WLA, compile WLA, run WLA and you'll get files called\n");
    fprintf(stderr, "\"_profile_function_names.wla\" and \"_profile_function_times.wla\", repeat the run\n");
    fprintf(stderr, "to recollect the data for more accurate results, and when you are done run this tool.\n");
    fprintf(stderr, "Delete the \"_profile_*\" files when you are done.\n");
    fprintf(stderr, "\n");
    return 1;
  }

  /* open files */

  file_times = fopen("_profile_function_times.wla", "rb");
  if (file_times == NULL) {
    fprintf(stderr, "Error opening file \"_profile_function_times.wla\".\n");
    return 1;
  }

  fseek(file_times, 0, SEEK_END);
  times_size = (int)ftell(file_times);
  fseek(file_times, 0, SEEK_SET);

  if ((times_size & 7) != 0) {
    fclose(file_times);
    fprintf(stderr, "File size of \"_profile_function_times.wla\" is not divisible by 8 -> As it should contain plain doubles we assume the data is corrupt.\n");
    return 1;
  }
  
  file_names = fopen("_profile_function_names.wla", "rb");
  if (file_names == NULL) {
    fclose(file_times);
    fprintf(stderr, "Error opening file \"_profile_function_names.wla\".\n");
    return 1;
  }

  /* allocate buffers */
  times_ms = calloc(sizeof(double) * (times_size / 8), 1);
  if (times_ms == NULL) {
    fprintf(stderr, "Out of memory error (1)!\n");
    fclose(file_times);
    fclose(file_names);
    return 1;
  }

  names = calloc(sizeof(char *) * (times_size / 8), 1);
  if (names == NULL) {
    fprintf(stderr, "Out of memory error (2)!\n");
    free(times_ms);
    fclose(file_times);
    fclose(file_names);
    return 1;
  }
  for (i = 0; i < times_size / 8; i++)
    names[i] = NULL;
  
  /* load data (only the 1st round) */

  success = YES;
  got_header = NO;

  for (i = 0; i < times_size / 8; i++) {
    double *d = &times_ms[i];
    unsigned char *c = (unsigned char *)d;
    char name[1024];
    int items;

    if (i >= (times_size / 8) - 1)
      break;
    
    fscanf(file_times, "%c%c%c%c%c%c%c%c", c, c+1, c+2, c+3, c+4, c+5, c+6, c+7);

    /* sanity check */
    if (times_ms[i] < 0.0)
      times_ms[i] = 0.0;
    
    items = (int)fscanf(file_names, "%1024s", name);
    if (items <= 0) {
      fprintf(stderr, "Out of function names! Please delete old \"_profile_function_*\" files and rerun your measurements!\n");
      success = NO;
      break;
    }

    /* the first "0"? */
    if (i == 0 && got_header == NO) {
      if (strcmp(name, "0") == 0) {
        i--;
        got_header = YES;
        continue;
      }
      else {
        fprintf(stderr, "The first entry must be a function called \"0\"! Got \"%s\" instead! The data is corrupt.\n", name);
        success = NO;
        break;
      }
    }

    /* the second "0"? */
    if (strcmp(name, "0") == 0)
      break;
    
    names[i] = calloc(strlen(name) + 1, 1);
    if (names[i] == NULL) {
      fprintf(stderr, "Out of memory error (3)!\n");
      success = NO;
      break;
    }

    strcpy(names[i], name);
  }

  entries = i;

  if (success == YES && i < (times_size / 8) - 1) {
    /* load data (rounds 2+) */
    j = i + 1;
    rounds++;
    for (i = 0; j < (times_size / 8) - 1; j++, i++) {
      double d;
      unsigned char *c = (unsigned char *)&d;
      char name[1024];
      int items;

      fscanf(file_times, "%c%c%c%c%c%c%c%c", c, c+1, c+2, c+3, c+4, c+5, c+6, c+7);

      /* sanity check */
      if (d < 0.0)
        d = 0.0;

      /* sum */
      times_ms[i] += d;
      
      items = (int)fscanf(file_names, "%1024s", name);
      if (items <= 0) {
        fprintf(stderr, "Out of function names! Please delete old \"_profile_function_*\" files and rerun your measurements!\n");
        success = NO;
        break;
      }

      /* got "0" (start of another round)? */
      if (strcmp(name, "0") == 0) {
        i = -1;
        rounds++;
        continue;
      }
    
      if (strcmp(names[i], name) != 0) {
        fprintf(stderr, "Was expecting \"%s\", got \"%s\" instead! Are you sure the \"_profile_function_*\" files are generated by repeating excatly the same run? Anyway, delete those files and rerun your measurements!\n", names[i], name);
        success = NO;
        break;
      }
    }
  }

  if (success == YES) {
    /* average */
    double multiplier = 1.0 / rounds;
    struct entry *e;
      
    fprintf(stderr, "The data files contain %d rounds of data.\n", rounds);
    
    for (i = 0; i < entries; i++)
      times_ms[i] *= multiplier;

    /* collect and print statistics! */
    for (i = 0; i < entries; i++) {
      if (_entry_add(names[i], times_ms[i]) == FAILED) {
        success = NO;
        break;
      }
    }

    printf("-----------------------------------------------------------------------------------\n");
    printf("---                                STATISTICS                                   ---\n");
    printf("-----------------------------------------------------------------------------------\n");

    e = g_entries;
    while (e != NULL) {
      printf("%32s: %5d CALLS: %08.3fms TOTAL: %08.3fms AVERAGE\n", e->function_name, e->count, e->sum_ms, e->sum_ms / e->count);
      e = e->next;
    }
  }

  /* exit */

  _entries_free();
  
  free(times_ms);

  for (i = 0; i < times_size / 8; i++) {
    if (names[i] != NULL)
      free(names[i]);
  }
  free(names);
  
  fclose(file_times);
  fclose(file_names);

  if (success == NO)
    return 1;
  else
    return 0;
}

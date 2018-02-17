
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

#include "include_file.h"
#include "pass_3.h"


extern struct incbin_file_data *incbin_file_data_first, *ifd_tmp;
extern struct section_def *sections_first, *sections_last, *sec_tmp, *sec_next;
extern struct file_name_info *file_name_info_first, *file_name_info_last, *file_name_info_tmp;
extern unsigned char *rom_banks, *rom_banks_usage_table;
extern FILE *file_out_ptr;
extern char gba_tmp_name[32], *gba_tmp_last_name, tmp[4096];
extern int verbose_mode, section_status, cartridgetype, output_format;


struct label_def *label_next, *label_last, *label_tmp, *labels = NULL;
struct map_t *global_unique_label_map = NULL;
struct block *blocks = NULL;

#define STRING_READ_FORMAT ("%" STRINGIFY(MAX_NAME_LENGTH) "s ")


int pass_3(void) {

  char emsg[256];
  struct section_def *s;
  struct label_def *l;
  struct label_def *parent_labels[10];
  struct block *b;
  FILE *f_in;
  int bank = 0, slot = 0, add = 0, file_name_id = 0, inz, line_number = 0, o, add_old = 0;
  int base = 0x00;
  char c;
  int err;

  memset(parent_labels, 0, sizeof(parent_labels));
  s = NULL;

  if (verbose_mode == ON)
    printf("Internal pass 1...\n");

  if ((f_in = fopen(gba_tmp_name, "rb")) == NULL) {
    fprintf(stderr, "INTERNAL_PASS_1: Error opening file \"%s\".\n", gba_tmp_name);
    return FAILED;
  }

  /* first loop */
  o = 0;
  if (output_format == OUTPUT_OBJECT) {
    while (o == 0 && fread(&c, 1, 1, f_in) != 0) {
      switch (c) {

      case ' ':
      case 'E':
	continue;

      case 'P':
	add_old = add;
	continue;
      case 'p':
	add = add_old;
	continue;

      case 'x':
	fscanf(f_in, "%d %*d ", &inz);
	if (section_status == ON) {
	  add += inz;
	  continue;
	}

	fprintf(stderr, "INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n");
	return FAILED;

      case 'd':
	if (section_status == ON) {
	  fscanf(f_in, "%*s ");
	  add++;
	  continue;
	}

	fprintf(stderr, "INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n");
	return FAILED;

      case 'y':
	if (section_status == ON) {
	  fscanf(f_in, "%*d ");
	  add += 2;
	  continue;
	}

	fprintf(stderr, "INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n");
	return FAILED;

      case 'b':
	fscanf(f_in, "%d ", &base);
	continue;

      case 'f':
	fscanf(f_in, "%d ", &file_name_id);
	continue;

      case 'B':
	fscanf(f_in, "%d %d ", &bank, &slot);
	continue;

      case 'k':
	fscanf(f_in, "%d ", &line_number);
	continue;

      case 'g':
	b = malloc(sizeof(struct block));
	if (b == NULL) {
	  fscanf(f_in, STRING_READ_FORMAT, tmp);
	  fprintf(stderr, "%s:%d INTERNAL_PASS_1: Out of memory while trying to allocate room for block \"%s\".\n",
		  get_file_name(file_name_id), line_number, tmp);
	  return FAILED;
	}
	b->filename_id = file_name_id;
	b->line_number = line_number;
	b->next = blocks;
	blocks = b;
	fscanf(f_in, STRING_READ_FORMAT, b->name);
	b->address = add;
	continue;

      case 'G':
	b = blocks;
	blocks = blocks->next;
	fprintf(stderr, "INTERNAL_PASS_1: Block \"%s\" is %d bytes.\n", b->name, add - b->address);
	free(b);
	continue;

      case 'Z': /* Breakpoint */
      case 'Y': /* Symbol */
      case 'L': /* Label */
	l = malloc(sizeof(struct label_def));

	if (l == NULL) {
		fscanf(f_in, STRING_READ_FORMAT, tmp);
	  fprintf(stderr, "%s:%d INTERNAL_PASS_1: Out of memory while trying to allocate room for label \"%s\".\n",
		  get_file_name(file_name_id), line_number, tmp);
	  return FAILED;
	}

	if (c == 'Y')
	  l->symbol = 1;
	else if (c == 'L')
	  l->symbol = 0;
	else
	  l->symbol = 2;
	
	if (c == 'Z')
	  l->label[0] = 0;
	else
	  fscanf(f_in, STRING_READ_FORMAT, l->label);

        if (c == 'L' && is_label_anonymous(l->label) == FAILED) {
          /* If the label has '@' at the start, mangle the label name to make it
           * unique */
          int n = 0, m;

          while (n < 10 && l->label[n] == '@') {
            n++;
          }
          m = n;
          while (m < 10)
            parent_labels[m++] = NULL;

          if (n < 10)
            parent_labels[n] = l;
          n--;
          while (n >= 0 && parent_labels[n] == 0)
            n--;

          if (n >= 0) {
            if (mangle_label(l->label, parent_labels[n]->label, n, MAX_NAME_LENGTH) == FAILED)
              return FAILED;
          }
        }

	l->next = NULL;
	l->section_status = ON;
	l->filename_id = file_name_id;
	l->linenumber = line_number;
	l->alive = ON;
	l->section_id = s->id;
        l->section_struct = s;
	/* section labels get a relative address */
	l->address = add;
	l->bank = s->bank;
	l->slot = s->slot;
	l->base = base;

	if (c == 'Z' || is_label_anonymous(l->label) == SUCCEEDED) {
	  if (labels != NULL) {
	    label_last->next = l;
	    label_last = l;
	  }
	  else {
	    labels = l;
	    label_last = l;
	  }
	  continue;
	}

        /* Check the label is not already defined */

        sprintf(emsg, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.",
            get_file_name(file_name_id),
            line_number,
            l->label);

        if (s != NULL) {
          /* Always put the label into the section's label_map */
          if (hashmap_get(s->label_map, l->label, NULL) == MAP_OK) {
            fputs(emsg, stderr);
            return FAILED;
          }
          if ((err = hashmap_put(s->label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
            return FAILED;
          }
        }

        /* Don't put local labels into namespaces or the global namespace */
        if (s == NULL || l->label[0] != '_') {
          if (s != NULL && s->nspace != NULL) {
            /* Label in a namespace */
            if (hashmap_get(s->nspace->label_map, l->label, NULL) == MAP_OK) {
              fputs(emsg, stderr);
              return FAILED;
            }
            if ((err = hashmap_put(s->nspace->label_map, l->label, l)) != MAP_OK) {
              fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
              return FAILED;
            }
          }
          else {
            /* Global label */
            if (hashmap_get(global_unique_label_map, l->label, NULL) == MAP_OK) {
              fputs(emsg, stderr);
              return FAILED;
            }
            if ((err = hashmap_put(global_unique_label_map, l->label, l)) != MAP_OK) {
              fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
              return FAILED;
            }
          }
        }


	if (labels != NULL) {
	  label_last->next = l;
	  label_last = l;
	}
	else {
	  labels = l;
	  label_last = l;
	}
	
	continue;

      case 'S':
	fscanf(f_in, "%d ", &inz);

	add_old = add;

	s = sections_first;
	while (s->id != inz)
	  s = s->next;

	/* a RAMSECTION? */
	if (s->status == SECTION_STATUS_RAM) {
	  s->address = 0;
	  s->listfile_items = 1;
	  s->listfile_ints = NULL;
	  s->listfile_cmds = NULL;
	  add = 0;
	  section_status = ON;
	  continue;
	}

	fprintf(stderr, "INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n");
	return FAILED;

      case 's':
	s->size = add - s->address;

	/* discard an empty section? */
	if (s->size == 0) {
	  fprintf(stderr, "INTERNAL_PASS_1: %s: Discarding an empty section \"%s\".\n", get_file_name(file_name_id), s->name);
	  s->alive = OFF;

	  /* discard all labels which belong to this section */
	  l = labels;
	  while (l != NULL) {
	    if (l->section_status == ON && l->section_id == s->id) {
	      l->alive = OFF;
            }
	    l = l->next;
          }
	}

	if (s->advance_org == NO)
	  add = add_old;

	section_status = OFF;
	s = NULL;
	continue;

      case 'O':
	fscanf(f_in, "%d ", &add);
	o++;
	continue;

      default:
	fprintf(stderr, "INTERNAL_PASS_1: .ORG needs to be set before any code/data can be accepted.\n");
	return FAILED;
      }
    }
  }
  else {
    while (o == 0 && fread(&c, 1, 1, f_in) != 0) {
      switch (c) {

      case ' ':
      case 'E':
	continue;

      case 'f':
	fscanf(f_in, "%d ", &file_name_id);
	continue;

      case 'S':
	fscanf(f_in, "%d ", &inz);

	add_old = add;

	s = sections_first;
	while (s->id != inz)
	  s = s->next;

	if (s->status == SECTION_STATUS_FREE)
	  add = 0;

	s->address = add;
	s->bank = bank;
	s->slot = slot;
	s->listfile_items = 1;
	s->listfile_ints = NULL;
	s->listfile_cmds = NULL;
	section_status = ON;
	o++;
	continue;

      default:
	fprintf(stderr, "INTERNAL_PASS_1: A section must be open before any code/data can be accepted.\n");
	return FAILED;
      }
    }
  }

  /* second (major) loop */
  while (fread(&c, 1, 1, f_in) != 0) {
    switch (c) {

    case ' ':
    case 'E':
      continue;

    case 'P':
      add_old = add;
      continue;
    case 'p':
      add = add_old;
      continue;

    case 'A':
    case 'S':
      if (c == 'A')
	fscanf(f_in, "%d %*d", &inz);
      else
	fscanf(f_in, "%d ", &inz);

      add_old = add;

      s = sections_first;
      while (s->id != inz)
	s = s->next;

      if (s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_RAM)
	add = 0;

      if (s->status != SECTION_STATUS_RAM) {
	s->bank = bank;
	s->slot = slot;
      }
      s->address = add;
      s->listfile_items = 1;
      s->listfile_ints = NULL;
      s->listfile_cmds = NULL;
      section_status = ON;
      continue;

    case 's':
      s->size = add - s->address;

      /* discard an empty section? */
      if (s->size == 0) {
        fprintf(stderr, "DISCARD: %s: Discarding an empty section \"%s\".\n", get_file_name(file_name_id), s->name);
        s->alive = OFF;

        /* discard all labels which belong to this section */
        l = labels;
        while (l != NULL) {
          if (l->section_status == ON && l->section_id == s->id) {
            l->alive = OFF;
          }
          l = l->next;
        }
      }

      /* some sections don't affect the ORG outside of them */
      if (s->advance_org == NO)
	add = add_old;

      section_status = OFF;
      s = NULL;
      continue;

    case 'x':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz;
      continue;

    case 'X':
      fscanf(f_in, "%d %*d ", &inz);
      add += inz << 1;
      continue;

#ifdef W65816
    case 'z':
#endif
    case 'q':
      fscanf(f_in, "%*s ");
      add += 3;
      continue;

    case 'T':
      fscanf(f_in, "%*d ");
      add += 3;
      continue;

    case 'b':
      fscanf(f_in, "%d ", &base);
      continue;

    case 'R':
    case 'Q':
    case 'd':
    case 'c':
      fscanf(f_in, "%*s ");
      add++;
      continue;

#ifdef W65816
    case 'M':
#endif
    case 'r':
      fscanf(f_in, "%*s ");
      add += 2;
      continue;

    case 'y':
    case 'C':
      fscanf(f_in, "%*d ");
      add += 2;
      continue;

    case 'D':
      fscanf(f_in, "%*d %*d %*d %d ", &inz);
      add += inz;
      continue;

    case 'O':
      fscanf(f_in, "%d ", &add);
      continue;

    case 'B':
      fscanf(f_in, "%d %d ", &bank, &slot);
      continue;

    case 'g':
      b = malloc(sizeof(struct block));
      if (b == NULL) {
	fscanf(f_in, STRING_READ_FORMAT, tmp);
	fprintf(stderr, "%s:%d INTERNAL_PASS_1: Out of memory while trying to allocate room for block \"%s\".\n",
		get_file_name(file_name_id), line_number, tmp);
	return FAILED;
      }
      b->filename_id = file_name_id;
      b->line_number = line_number;
      b->next = blocks;
      blocks = b;
      fscanf(f_in, STRING_READ_FORMAT, b->name);
      b->address = add;
      continue;

    case 'G':
      b = blocks;
      blocks = blocks->next;
      fprintf(stderr, "INTERNAL_PASS_1: Block \"%s\" is %d bytes.\n", b->name, add - b->address);
      free(b);
      continue;

    case 'Z': /* Breakpoint */
    case 'Y': /* Symbol */
    case 'L': /* Label */
      l = malloc(sizeof(struct label_def));
      if (l == NULL) {
	fscanf(f_in, STRING_READ_FORMAT, tmp);
	fprintf(stderr, "%s:%d INTERNAL_PASS_1: Out of memory while trying to allocate room for label \"%s\".\n",
		get_file_name(file_name_id), line_number, tmp);
	return FAILED;
      }

      if (c == 'Y')
	l->symbol = 1;
      else if (c == 'L')
	l->symbol = 0;
      else
	l->symbol = 2;

      if (c == 'Z')
	l->label[0] = 0;
      else
	fscanf(f_in, STRING_READ_FORMAT, l->label);

      if (c == 'L' && is_label_anonymous(l->label) == FAILED) {
        /* If the label has '@' at the start, mangle the label name to make it
         * unique */
        int n = 0, m;

        while (n < 10 && l->label[n] == '@') {
          n++;
        }
        m = n;
        while (m < 10)
          parent_labels[m++] = NULL;

        if (n < 10)
          parent_labels[n] = l;
        n--;
        while (n >= 0 && parent_labels[n] == 0)
          n--;

        if (n >= 0) {
          if (mangle_label(l->label, parent_labels[n]->label, n, MAX_NAME_LENGTH) == FAILED)
            return FAILED;
        }
      }

      l->next = NULL;
      l->section_status = section_status;
      l->filename_id = file_name_id;
      l->linenumber = line_number;
      l->alive = ON;
      if (section_status == ON) {
        l->section_id = s->id;
        l->section_struct = s;
        /* section labels get a relative address */
        l->address = add - s->address;
        l->bank = s->bank;
        l->slot = s->slot;
      }
      else {
	l->section_id = 0;
        l->section_struct = NULL;
	l->address = add;
	l->bank = bank;
	l->slot = slot;
      }

      l->base = base;

      if (c == 'Z' || is_label_anonymous(l->label) == SUCCEEDED) {
	if (labels != NULL) {
	  label_last->next = l;
	  label_last = l;
	}
	else {
	  labels = l;
	  label_last = l;
	}
	continue;
      }

      sprintf(emsg, "%s:%d: INTERNAL_PASS_1: Label \"%s\" was defined for the second time.",
          get_file_name(file_name_id),
          line_number,
          l->label);

      if (s != NULL) {
        /* Always put the label into the section's label_map */
        if (hashmap_get(s->label_map, l->label, NULL) == MAP_OK) {
          fputs(emsg, stderr);
          return FAILED;
        }
        if ((err = hashmap_put(s->label_map, l->label, l)) != MAP_OK) {
          fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
          return FAILED;
        }
      }

      /* Don't put local labels into namespaces or the global namespace */
      if (s == NULL || l->label[0] != '_') {
        if (s != NULL && s->nspace != NULL) {
          /* Label in a namespace */
          if (hashmap_get(s->nspace->label_map, l->label, NULL) == MAP_OK) {
            fputs(emsg, stderr);
            return FAILED;
          }
          if ((err = hashmap_put(s->nspace->label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
            return FAILED;
          }
        }
        else {
          /* Global label */
          if (hashmap_get(global_unique_label_map, l->label, NULL) == MAP_OK) {
            fputs(emsg, stderr);
            return FAILED;
          }
          if ((err = hashmap_put(global_unique_label_map, l->label, l)) != MAP_OK) {
            fprintf(stderr, "Hashmap error %d. Please send a bug report!", err);
            return FAILED;
          }
        }
      }

      if (labels != NULL) {
        label_last->next = l;
        label_last = l;
      }
      else {
        labels = l;
        label_last = l;
      }

      continue;

    case 'f':
      fscanf(f_in, "%d ", &file_name_id);
      if (s != NULL)
	s->listfile_items++;
      continue;

    case 'k':
      fscanf(f_in, "%d ", &line_number);
      if (s != NULL)
	s->listfile_items++;
      continue;

    default:
      fprintf(stderr, "%s: INTERNAL_PASS_1: Unknown internal symbol \"%c\" in \"%s\" at offset %ld.\n", get_file_name(file_name_id), c, gba_tmp_name, ftell(f_in)-1);
      return FAILED;
    }
  }

  fclose(f_in);
  
  if (blocks != NULL) {
    fprintf(stderr, "%s:%d INTERNAL_PASS_1: .BLOCK \"%s\" was left open.\n", get_file_name(blocks->filename_id), blocks->line_number, blocks->name);
    return FAILED;
  }

  return SUCCEEDED;
}


/* is the label of form __, -, --, ---, +, ++, +++, ... ? */
int is_label_anonymous(char *label) {

  int length, i;
  char c;

  if (strcmp(label, "__") == 0)
    return SUCCEEDED;

  c = *label;
  if (!(c == '-' || c == '+'))
    return FAILED;
  length = strlen(label);
  for (i = 0; i < length; i++) {
    if (*(label + i) != c)
      return FAILED;
  }

  return SUCCEEDED;
}

int mangle_label(char *label, char *parent, int n, unsigned int label_size) {
  char buf[MAX_NAME_LENGTH*2+2];
  int len = strlen(parent);

  strcpy(buf, parent);
  strcpy(&buf[len], label+n);

  if (len+strlen(label+n)+1 > label_size) {
    fprintf(stderr, "MANGLE_LABEL: Child label expands to \"%s\" which is %d bytes too large.\n", buf, (int)(strlen(buf)-label_size+1));
    return FAILED;
  }

  buf[label_size-1] = 0;
  strcpy(label, buf);

  return SUCCEEDED;
}


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "analyze.h"
#include "memory.h"
#include "listfile.h"
#include "files.h"

#if defined(AMIGA)
#include "/printf.h"
#else
#include "../printf.h"
#endif


/* read an integer from t */
#define READ_T (t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24)); t += 4;

/* read a double from t */
#define READ_DOU {                              \
    dtmp = (unsigned char *)&dou;               \
    dtmp[0] = *(t++);                           \
    dtmp[1] = *(t++);                           \
    dtmp[2] = *(t++);                           \
    dtmp[3] = *(t++);                           \
    dtmp[4] = *(t++);                           \
    dtmp[5] = *(t++);                           \
    dtmp[6] = *(t++);                           \
    dtmp[7] = *(t++);                           \
  }


extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct reference *g_reference_first, *g_reference_last;
extern struct section *g_sec_first, *g_sec_last, *g_sec_bankhd_first, *g_sec_bankhd_last;
extern struct stack *g_stacks_first, *g_stacks_last;
extern struct label *g_labels_first, *g_labels_last;
extern struct map_t *g_global_unique_label_map;
extern struct map_t *g_namespace_map;
extern struct slot g_slots[256];
extern struct after_section *g_after_sections, *g_after_tmp;
extern struct label_sizeof *g_label_sizeofs;
extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern int g_rombanks, g_section_overwrite, g_discard_unreferenced_sections;
extern int g_emptyfill;
extern int *g_banksizes, *g_bankaddress, g_banksize;

struct pointer_array **g_section_table_table = NULL;
int g_section_table_table_max = -1;

static int s_section_table_table_array_max = 0;


int is_label_anonymous(char *label);


int add_pointer_to_a_pointer_array(void *ptr, int id, void ***array, int *max, int *array_max, int size_increase) {

  if (id > *max)
    *max = id;

  while (id >= *array_max) {
    int i, j;

    i = *array_max;
    j = i + size_increase;

    /* increase the pointer array size */
    *array_max = j;

    *array = realloc(*array, sizeof(void **) * j);
    if (*array == NULL) {
      fprintf(stderr, "add_pointer_to_a_pointer_array(): Out of memory error!\n");
      *array_max = 0;
      
      return FAILED;
    }

    while (i < j) {
      (*array)[i] = NULL;
      i++;
    }
  }

  (*array)[id] = ptr;
  
  return SUCCEEDED;
}


int free_stack(struct stack *s) {

  struct object_file *file;
  
  if (s == NULL)
    return SUCCEEDED;

  file = get_file(s->file_id);
  file->stacks[s->id] = NULL;
  
  if (s->stack_items != NULL) {
    free(s->stack_items);
    s->stack_items = NULL;
  }
  if (s->prev == NULL)
    g_stacks_first = s->next;
  else
    s->prev->next = s->next;
  if (s->next == NULL)
    g_stacks_last = s->prev;
  else
    s->next->prev = s->prev;

  free(s);

  return SUCCEEDED;
}


int parse_context_from_name(char *name, char *context) {

  int i, j, length;

  length = (int)strlen(name);
  for (i = 0; i < length; i++) {
    if (name[i] == ':') {
      /* there is a context attached to the label! */

      /* remove it from the name... */
      name[i] = 0;
      i++;
      /* ... and copy it to context variable */
      for (j = 0; i < length; i++, j++)
        context[j] = name[i];
      context[j] = 0;
      break;
    }
  }

  return SUCCEEDED;
}


int add_reference(struct reference *r) {

  r->context[0] = 0;
  
  if (is_label_anonymous(r->name) == YES) {
    /* anonymous labels might have a context -> reformat such labels */
    parse_context_from_name(r->name, r->context);
  }

  r->file_id = g_obj_tmp->id;
  r->next = NULL;

  if (g_reference_first == NULL) {
    g_reference_first = r;
    g_reference_last = r;
    r->prev = NULL;
  }
  else {
    r->prev = g_reference_last;
    g_reference_last->next = r;
    g_reference_last = r;
  }

  return SUCCEEDED;
}


int add_stack(struct stack *sta) {

  /* parse the type */
  if ((sta->type & (1 << 7)) != 0)
    sta->relative_references = YES;
  else
    sta->relative_references = NO;
  sta->type &= ~(1 << 7);

  sta->file_id = g_obj_tmp->id;
  sta->next = NULL;
  sta->computed = NO;
  sta->under_work = NO;

  if (g_stacks_first == NULL) {
    g_stacks_first = sta;
    g_stacks_last = sta;
    sta->prev = NULL;
  }
  else {
    sta->prev = g_stacks_last;
    g_stacks_last->next = sta;
    g_stacks_last = sta;
  }

  /* add the pointer also to a pointer array for quick discovery with the ID */
  if (add_pointer_to_a_pointer_array((void *)sta, sta->id, (void ***)&g_obj_tmp->stacks, &g_obj_tmp->stacks_max, &g_obj_tmp->stacks_array_max, 1024) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


/*
static void _print_section_id_tables(void) {

  int i, j;

  fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  
  for (i = 0; i <= g_section_table_table_max; i++) {
    struct pointer_array *pointer_array = (struct pointer_array *)g_section_table_table[i];

    if (pointer_array == NULL)
      continue;
    
    fprintf(stderr, "#####################################################\n");
    fprintf(stderr, "TABLE %d (0 -> %d)\n", i, pointer_array->max);
    fprintf(stderr, "#####################################################\n");

    for (j = 0; j <= pointer_array->max; j++) {
      struct section *s = (struct section *)pointer_array->ptr[j];

      if (s == NULL)
        continue;

      fprintf(stderr, "SECTION %.6d (%.6d): \"%s\"\n", s->id, j, s->name);
    }
  }

  fprintf(stderr, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}
*/


int add_section(struct section *s) {

  struct pointer_array *pointer_array;
  unsigned char *data;
  struct section *ss;
  int table = s->id >> 16, id = s->id, add_pointer_array = NO;
  
  /* create a local copy of the data */
  if (s->size > 0) {
    data = calloc(s->size, 1);
    if (data == NULL) {
      fprintf(stderr, "%s: ADD_SECTION: Out of memory with section \"%s\".\n", g_obj_tmp->name, s->name);
      return FAILED;
    }

    memcpy(data, s->data, s->size);
    s->data = data;
  }
  else
    s->data = NULL;

  s->file_id = g_obj_tmp->id;
  s->next = NULL;
  s->prev = NULL;
  s->after = NULL;
  s->alive = YES;
  s->placed = NO;
  s->appended_to = NO;
  s->appended_to_section = NULL;
  s->appended_to_offset = 0;

  if (s->name[0] == 'B' && strcmp(s->name, "BANKHEADER") == 0) {
    ss = g_sec_bankhd_first;
    while (ss != NULL) {
      if (ss->bank == s->bank) {
        fprintf(stderr, "%s: ADD_SECTION: BANKHEADER section for bank %d was defined for the second time.\n", g_obj_tmp->name, s->bank);
        return FAILED;
      }
      ss = ss->next;
    }

    s->is_bankheader_section = YES;

    if (g_sec_bankhd_first == NULL) {
      g_sec_bankhd_first = s;
      g_sec_bankhd_last = s;
    }
    else {
      s->prev = g_sec_bankhd_last;
      g_sec_bankhd_last->next = s;
      g_sec_bankhd_last = s;
    }
  }
  else {
    s->is_bankheader_section = NO;

    if (g_sec_first == NULL) {
      g_sec_first = s;
      g_sec_last = s;
    }
    else {
      s->prev = g_sec_last;
      g_sec_last->next = s;
      g_sec_last = s;
    }
  }

  /* add the pointer also to a pointer array for quick discovery with the ID */

  if (table > g_section_table_table_max)
    pointer_array = NULL;
  else
    pointer_array = g_section_table_table[table];

  if (pointer_array == NULL) {
    /* allocate container for this table */
    pointer_array = calloc(sizeof(struct pointer_array), 1);
    if (pointer_array == NULL) {
      fprintf(stderr, "%s: ADD_SECTION: Out of memory with section \"%s\".\n", g_obj_tmp->name, s->name);
      return FAILED;
    }

    pointer_array->ptr = NULL;
    pointer_array->max = -1;
    pointer_array->array_max = 0;

    add_pointer_array = YES;
  }
  
  if (table > g_section_table_table_max || add_pointer_array == YES) {
    /* expand pointer array (table) */
    if (add_pointer_to_a_pointer_array((void *)pointer_array, table, (void ***)&g_section_table_table, &g_section_table_table_max, &s_section_table_table_array_max, 256) == FAILED)
      return FAILED;
  }

  /* WARNING: bankheader sections don't go into this speedup array, but there must be a NULL there */
  if (s->is_bankheader_section == YES)
    s = NULL;
  
  if (add_pointer_to_a_pointer_array((void *)s, id & 0xffff, (void ***)&pointer_array->ptr, &pointer_array->max, &pointer_array->array_max, 256) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int free_section(struct section *s) {

  if (s == NULL)
    return FAILED;

  if (s->prev != NULL)
    s->prev->next = s->next;
  if (s->next != NULL)
    s->next->prev = s->prev;

  if (s->name[0] == 'B' && strcmp(s->name, "BANKHEADER") == 0) {
    if (g_sec_bankhd_first == s)
      g_sec_bankhd_first = s->next;
    if (g_sec_bankhd_last == s)
      g_sec_bankhd_last = s->prev;
  }
  else {
    if (g_sec_first == s)
      g_sec_first = s->next;
    if (g_sec_last == s)
      g_sec_last = s->prev;
  }

  /* free label map */
  hashmap_free(s->label_map);

  if (s->listfile_cmds != NULL)
    free(s->listfile_cmds);
  if (s->listfile_ints != NULL)
    free(s->listfile_ints);
  
  if (s->data != NULL)
    free(s->data);

  /* remove from the quick ID LUT */
  g_section_table_table[s->id >> 16]->ptr[s->id & 0xffff] = NULL;

  free(s);

  return SUCCEEDED;
}


int find_label(char *str, struct section *s, struct label **out) {

  char *str2, *stripped;
  char prefix[MAX_NAME_LENGTH*2+2];
  struct label *l = NULL;
  int i;

  str2 = strchr(str, '.');
  i = (int)(str2-str);
  if (str2 == NULL) {
    stripped = str;
    prefix[0] = '\0';
  }
  else {
    stripped = str2+1;
    strncpy(prefix, str, i);
    prefix[i] = '\0';
  }

  *out = NULL;

  if (prefix[0] != '\0') {
    /* a namespace is specified (or at least there's a dot in the label) */
    struct namespace_def *nspace;

    if (hashmap_get(g_namespace_map, prefix, (void*)&nspace) == MAP_OK) {
      if (hashmap_get(nspace->label_map, stripped, (void*)&l) == MAP_OK) {
        *out = l;
        return SUCCEEDED;
      }
    }
  }
  if (s != NULL && s->nspace != NULL) {
    /* check the section's namespace */
    if (hashmap_get(s->nspace->label_map, str, (void*)&l) == MAP_OK) {
      *out = l;
      return SUCCEEDED;
    }
  }
  if (s != NULL) {
    /* check the section's labels. this is a bit redundant but it might have
       local labels (labels starting with an underscore) */
    if (hashmap_get(s->label_map, str, (void*)&l) == MAP_OK) {
      *out = l;
      return SUCCEEDED;
    }
  }
  /* check the global namespace */
  if (hashmap_get(g_global_unique_label_map, str, (void*)&l) == MAP_OK) {
    *out = l;
    return SUCCEEDED;
  }

  return FAILED;
}


int add_label(struct label *l) {

  l->context[0] = 0;

  if (is_label_anonymous(l->name) == YES) {
    /* anonymous labels might have a context -> reformat such labels */
    parse_context_from_name(l->name, l->context);
  }
  
  l->next = NULL;
  l->alive = YES;

  if (g_labels_first == NULL) {
    g_labels_first = l;
    g_labels_last = l;
    l->prev = NULL;
  }
  else {
    l->prev = g_labels_last;
    g_labels_last->next = l;
    g_labels_last = l;
  }

  return SUCCEEDED;
}


int obtain_rombankmap(void) {

  int map_found = OFF, i, x, a;
  struct object_file *o;
  unsigned char *t;

  
  /* initialize values */
  for (i = 0; i < g_rombanks; i++)
    g_banksizes[i] = 0;

  o = g_obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      t = o->data + OBJ_ROMBANKMAP;

      /* obtain status */
      i = *t;
      t++;

      /* general rombanksize? */
      if (i == 0) {
        /* obtain banksize */
        g_banksize = READ_T;

        o->memorymap = t;
        map_found = ON;
        for (i = 0; i < o->rom_banks; i++) {
          if (g_banksizes[i] == 0) {
            g_banksizes[i] = g_banksize;
            g_bankaddress[i] = i * g_banksize;
          }
          else if (g_banksizes[i] != g_banksize) {
            fprintf(stderr, "OBTAIN_ROMBANKMAP: ROMBANKMAPs don't match.\n");
            return FAILED;
          }
        }
      }
      else {
        for (a = 0, x = 0; x < o->rom_banks; x++) {
          g_banksize = READ_T;
          if (g_banksizes[x] == 0) {
            g_banksizes[x] = g_banksize;
            g_bankaddress[x] = a;
          }
          else if (g_banksizes[x] != g_banksize) {
            fprintf(stderr, "OBTAIN_ROMBANKMAP: ROMBANKMAPs don't match.\n");
            return FAILED;
          }
          a += g_banksize;
        }
        
        o->memorymap = t;
        map_found = ON;
      }
    }

    o = o->next;
  }

  if (map_found == OFF) {
    fprintf(stderr, "OBTAIN_ROMBANKMAP: No object files.\n");
    return FAILED;
  }

  return SUCCEEDED;
}


int obtain_source_file_names(void) {

  struct source_file_name *s, **p;
  struct object_file *o;
  unsigned char *t, *m;
  int x, z;

  
  o = g_obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ)
      t = o->source_file_names;
    else
      t = o->data + LIB_SOURCE_FILE_NAMES;

    x = READ_T;

    p = &(o->source_file_names_list);
    for (; x > 0; x--) {
      s = calloc(sizeof(struct source_file_name), 1);
      if (s == NULL) {
        fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
        return FAILED;
      }

      /* compute the length of the name */
      for (m = t, z = 0; *m != 0; m++, z++)
        ;

      s->name = calloc(z+1, 1);
      if (s->name == NULL) {
        free(s);
        fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
        return FAILED;
      }

      /* get the name */
      for (z = 0; *t != 0; t++, z++)
        s->name[z] = *t;
      s->name[z] = 0;

      t++;
      s->id = READ_T;
      s->checksum = (unsigned long)READ_T;
      s->next = NULL;
      *p = s;
      p = &(s->next);
    }

    o->exported_defines = t;
    o = o->next;
  }

  return SUCCEEDED;
}


int obtain_memorymap(void) {

  struct object_file *o;
  int map_found = OFF, i, x, y;
  unsigned char *t;
  struct slot s[256];

  
  for (i = 0; i < 256; i++) {
    g_slots[i].usage = OFF;
    g_slots[i].name[0] = 0;
    g_slots[i].address = 0;
    g_slots[i].size = 0;
  }

  o = g_obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      t = o->memorymap;

      /* obtain slots */
      i = *t;
      t++;

      for (x = 0; i > 0; i--, x++) {
        g_slots[x].usage = ON;
        g_slots[x].address =  READ_T;
        g_slots[x].size =  READ_T;
        for (y = 0; *t != 0; t++, y++)
          g_slots[x].name[y] = *t;
        g_slots[x].name[y] = 0;
        t++;
      }

      o->source_file_names = t;
      map_found = ON;
      break;
    }

    o = o->next;
  }

  if (map_found == OFF) {
    fprintf(stderr, "OBTAIN_MEMORYMAP: No object files.\n");
    return FAILED;
  }

  /* check if the following memorymaps equal to the previous one */
  o = o->next;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      for (i = 0; i < 256; i++)
        s[i].usage = OFF;
      t = o->memorymap;

      /* obtain slots */
      i = *t;
      t++;

      for (x = 0; i > 0; i--, x++) {
        s[x].usage = ON;
        s[x].address =  READ_T;
        s[x].size =  READ_T;
        for (y = 0; *t != 0; t++, y++)
          s[x].name[y] = *t;
        s[x].name[y] = 0;
        t++;
      }

      o->source_file_names = t;

      for (x = 0, i = 0; i < 256; i++) {
        if (s[i].usage == ON) {
          if (g_slots[i].usage == OFF) {
            x = 1;
            break;
          }
          if (g_slots[i].address == s[i].address && g_slots[i].size == s[i].size) {
            if (g_slots[i].name[0] == 0 && s[i].name[0] != 0) {
              /* use the name given to the other slot */
              strcpy(g_slots[i].name, s[i].name);
            }
            else if (g_slots[i].name[0] != 0 && s[i].name[0] != 0) {
              /* check that the names match */
              if (strcmp(g_slots[i].name, s[i].name) != 0)
                fprintf(stderr, "OBTAIN_MEMORYMAP: SLOT %d has two different names (\"%s\" and \"%s\"). Using \"%s\"...\n",
                        i, g_slots[i].name, s[i].name, g_slots[i].name);
            }
            continue;
          }
          x = 1;
          break;
        }
        else {
          if (g_slots[i].usage == ON) {
            x = 1;
            break;
          }
        }
      }

      if (x == 1) {
        fprintf(stderr, "OBTAIN_MEMORYMAP: The object files are compiled for different memory architectures.\n");
        return FAILED;
      }
    }

    o = o->next;
  }

  return SUCCEEDED;
}


int collect_dlr(void) {

  struct reference *r;
  struct stack *s;
  struct label *l;
  struct label_sizeof *ls;
  int section_id_base, x, i, n, q;
  unsigned char *t, *dtmp;
  double dou;
  
  section_id_base = 0;
  g_obj_tmp = g_obj_first;

  while (g_obj_tmp != NULL) {
    /* OBJECT FILE */
    if (g_obj_tmp->format == WLA_VERSION_OBJ) {
      t = g_obj_tmp->exported_defines;
      i = READ_T;

      /* load defines */
      for (; i > 0; i--) {
        l = calloc(1, sizeof(struct label));
        if (l == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the name */
        for (x = 0; !(*t == 0 || *t == 1); t++, x++)
          l->name[x] = *t;
        l->name[x] = 0;
        if (*t == 0)
          l->status = LABEL_STATUS_DEFINE;
        else if (*t == 1)
          l->status = LABEL_STATUS_STACK;
        else {
          fprintf(stderr, "COLLECT_DLR: Unknown definition type \"%d\".\n", *t);
          free(l);
          return FAILED;
        }
        t++;

        READ_DOU;
        l->address = dou;
        l->base = 0;
        l->file_id = g_obj_tmp->id;
        l->section_status = OFF;
        l->section_struct = NULL;

        add_label(l);
      }

      /* load labels */
      i = READ_T;

      for (; i > 0; i--) {
        l = calloc(1, sizeof(struct label));
        if (l == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        
        for (x = 0; !(*t == 0 || *t == 1 || *t == 2); t++, x++)
          l->name[x] = *t;
        l->name[x] = 0;
        
        if (*t == 0)
          l->status = LABEL_STATUS_LABEL;
        else if (*t == 1)
          l->status = LABEL_STATUS_SYMBOL;
        else if (*t == 2)
          l->status = LABEL_STATUS_BREAKPOINT;
        else {
          fprintf(stderr, "COLLECT_DLR: Unknown label type \"%d\".\n", *t);
          free(l);
          return FAILED;
        }

        t++;
        l->slot = *(t++);
        l->file_id_source = READ_T;

        l->section = READ_T;
        if (l->section == 0)
          l->section_status = OFF;
        else {
          l->section_status = ON;
          l->section += section_id_base;
        }
        l->address = READ_T;
        l->linenumber = READ_T;
        l->bank = READ_T;
        l->base = READ_T;
        l->file_id = g_obj_tmp->id;
        l->section_struct = NULL;

        add_label(l);
      }

      i = READ_T;

      /* load references */
      for (; i > 0; i--) {
        r = calloc(sizeof(struct reference), 1);
        if (r == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        for (x = 0; *t != 0; t++, x++)
          r->name[x] = *t;
        r->name[x] = 0;
        t++;
        r->type = *(t++);
        r->special_id = *(t++);
        r->file_id_source = READ_T;

        if (r->type == REFERENCE_TYPE_BITS) {
          r->bits_position = *(t++);
          r->bits_to_define = *(t++);
        }
        else {
          r->bits_position = 0;
          r->bits_to_define = 0;
        }

        r->slot = *(t++);
        r->section = READ_T;
        if (r->section == 0)
          r->section_status = OFF;
        else {
          r->section_status = ON;
          r->section += section_id_base;
        }
        r->linenumber = READ_T;
        r->address = READ_T;
        r->bank = READ_T;
        r->base = READ_T;

        add_reference(r);
      }

      i = READ_T;

      /* load pending calculations */
      for (; i > 0; i--) {
        s = calloc(sizeof(struct stack), 1);
        if (s == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        s->id = READ_T;
        s->type = *(t++);
        s->special_id = *(t++);
        s->section = READ_T;
        if (s->section == 0)
          s->section_status = OFF;
        else {
          s->section_status = ON;
          s->section += section_id_base;
        }
        s->file_id_source = READ_T;
        x = *(t++);
        s->position = *(t++);

        if (s->type == STACK_TYPE_BITS) {
          s->bits_position = *(t++);
          s->bits_to_define = *(t++);
        }
        else {
          s->bits_position = 0;
          s->bits_to_define = 0;
        }
        
        s->slot = *(t++);
        s->address = READ_T;
        s->linenumber = READ_T;
        s->bank = READ_T;
        s->base = READ_T;
        s->stacksize = x;
        
        s->stack_items = calloc(sizeof(struct stack_item) * x, 1);
        if (s->stack_items == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          free(s);
          return FAILED;
        }
        
        add_stack(s);

        for (n = 0; n != x; n++) {
          s->stack_items[n].slot = -1;
          s->stack_items[n].base = -1;
          s->stack_items[n].bank = -1;
          s->stack_items[n].stack_file_id = -1;
          s->stack_items[n].type = *(t++);
          s->stack_items[n].sign = *(t++);
          if (s->stack_items[n].type == STACK_ITEM_TYPE_LABEL || s->stack_items[n].type == STACK_ITEM_TYPE_STRING) {
            for (q = 0; *t != 0; t++, q++)
              s->stack_items[n].string[q] = *t;
            s->stack_items[n].string[q] = 0;
            t++;
          }
          else {
            READ_DOU;
            s->stack_items[n].value_ram = dou;
            s->stack_items[n].value_rom = dou;
          }
        }
      }

      /* label sizeofs */
      i = READ_T;

      while (i > 0) {
        i--;

        ls = calloc(sizeof(struct label_sizeof), 1);
        if (ls == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the name */
        for (x = 0; *t != 0; t++, x++)
          ls->name[x] = *t;
        ls->name[x] = 0;
        t++;

        ls->size = READ_T;
        ls->file_id = g_obj_tmp->id;
        
        ls->next = g_label_sizeofs;
        g_label_sizeofs = ls;
      }

      /* global listfile block */
      if (listfile_block_read_global(&t, g_obj_tmp) == FAILED)
        return FAILED;
      
      /* appendto/after sections */
      i = READ_T;

      while (i > 0) {
        i--;

        g_after_tmp = calloc(1, sizeof(struct after_section));
        if (g_after_tmp == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        g_after_tmp->alive = YES;
        g_after_tmp->inserted = NO;

        g_after_tmp->section_id = READ_T;
        g_after_tmp->file_id = g_obj_tmp->id;
        g_after_tmp->is_appendto = *(t++);
        
        /* copy the names */
        for (x = 0; *t != 0; t++, x++)
          g_after_tmp->section[x] = *t;
        g_after_tmp->section[x] = 0;
        t++;
        for (x = 0; *t != 0; t++, x++)
          g_after_tmp->after[x] = *t;
        g_after_tmp->after[x] = 0;
        t++;

        g_after_tmp->section_s = NULL;
        g_after_tmp->after_s = NULL;
        
        g_after_tmp->next = g_after_sections;
        g_after_sections = g_after_tmp;
      }

      /* save pointer to data block area */
      g_obj_tmp->data_blocks = t;
    }
    /* LIBRARY FILE */
    else if (g_obj_tmp->format == WLA_VERSION_LIB) {
      t = g_obj_tmp->exported_defines;
      i = READ_T;

      /* load definitions */
      for (; i > 0; i--) {
        l = calloc(1, sizeof(struct label));
        if (l == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the name */
        for (x = 0; !(*t == 0 || *t == 1); t++, x++)
          l->name[x] = *t;
        l->name[x] = 0;
        if (*t == 0)
          l->status = LABEL_STATUS_DEFINE;
        else if (*t == 1)
          l->status = LABEL_STATUS_STACK;
        else {
          fprintf(stderr, "COLLECT_DLR: Unknown definition type \"%d\".\n", *t);
          free(l);
          return FAILED;
        }
        t++;

        READ_DOU;
        l->address = dou;
        l->bank = g_obj_tmp->bank;
        l->slot = g_obj_tmp->slot;
        l->base = g_obj_tmp->base;
        l->file_id = g_obj_tmp->id;
        l->section_status = OFF;

        add_label(l);
      }

      i = READ_T;

      /* load labels and symbols */
      for (; i > 0; i--) {
        l = calloc(1, sizeof(struct label));
        if (l == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        
        for (x = 0; !(*t == 0 || *t == 1 || *t == 2); t++, x++)
          l->name[x] = *t;
        l->name[x] = 0;
        
        if (*t == 0)
          l->status = LABEL_STATUS_LABEL;
        else if (*t == 1)
          l->status = LABEL_STATUS_SYMBOL;
        else if (*t == 2)
          l->status = LABEL_STATUS_BREAKPOINT;
        else {
          fprintf(stderr, "COLLECT_DLR: Unknown label type \"%d\".\n", *t);
          free(l);
          return FAILED;
        }

        t++;
        l->section = READ_T;
        l->section += section_id_base;
        l->file_id_source = READ_T;
        l->linenumber = READ_T;
        l->section_status = ON;
        l->address = READ_T;
        l->base = g_obj_tmp->base; /* (((int)l->address) >> 16) & 0xFF; */
        l->bank = g_obj_tmp->bank;
        l->slot = g_obj_tmp->slot;
        l->file_id = g_obj_tmp->id;

        add_label(l);
      }

      i = READ_T;

      /* load references */
      for (; i > 0; i--) {
        r = calloc(sizeof(struct reference), 1);
        if (r == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        for (x = 0; *t != 0; t++, x++)
          r->name[x] = *t;
        r->name[x] = 0;
        t++;
        r->type = *(t++);
        r->special_id = *(t++);
        r->section = READ_T;
        r->section += section_id_base;
        r->file_id_source = READ_T;

        if (r->type == REFERENCE_TYPE_BITS) {
          r->bits_position = *(t++);
          r->bits_to_define = *(t++);
        }
        else {
          r->bits_position = 0;
          r->bits_to_define = 0;
        }

        r->linenumber = READ_T;
        r->section_status = ON;
        r->address = READ_T;

        r->bank = g_obj_tmp->bank;
        r->slot = g_obj_tmp->slot;
        r->base = g_obj_tmp->base;

        add_reference(r);
      }

      i = READ_T;

      /* load pending calculations */
      for (; i > 0; i--) {
        s = calloc(sizeof(struct stack), 1);
        if (s == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        s->id = READ_T;
        s->type = *(t++);
        s->special_id = *(t++);
        s->section = READ_T;
        if (s->section == 0)
          s->section_status = OFF;
        else {
          s->section_status = ON;
          s->section += section_id_base;
        }
        s->file_id_source = READ_T;
        x = *(t++);
        s->position = *(t++);

        if (s->type == STACK_TYPE_BITS) {
          s->bits_position = *(t++);
          s->bits_to_define = *(t++);
        }
        else {
          s->bits_position = 0;
          s->bits_to_define = 0;
        }

        s->address = READ_T;
        s->linenumber = READ_T;
        s->stacksize = x;
        s->bank = g_obj_tmp->bank;
        s->slot = g_obj_tmp->slot;
        s->base = g_obj_tmp->base;
        
        s->stack_items = calloc(sizeof(struct stack_item) * x, 1);
        if (s->stack_items == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          free(s);
          return FAILED;
        }

        add_stack(s);

        for (n = 0; n != x; n++) {
          s->stack_items[n].slot = -1;
          s->stack_items[n].base = -1;
          s->stack_items[n].bank = -1;
          s->stack_items[n].stack_file_id = -1;
          s->stack_items[n].type = *(t++);
          s->stack_items[n].sign = *(t++);
          if (s->stack_items[n].type == STACK_ITEM_TYPE_LABEL || s->stack_items[n].type == STACK_ITEM_TYPE_STRING) {
            for (q = 0; *t != 0; t++, q++)
              s->stack_items[n].string[q] = *t;
            s->stack_items[n].string[q] = 0;
            t++;
          }
          else {
            READ_DOU;
            s->stack_items[n].value_ram = dou;
            s->stack_items[n].value_rom = dou;
          }
        }
      }

      /* label sizeofs */
      i = READ_T;

      while (i > 0) {
        i--;

        ls = calloc(sizeof(struct label_sizeof), 1);
        if (ls == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the name */
        for (x = 0; *t != 0; t++, x++)
          ls->name[x] = *t;
        ls->name[x] = 0;
        t++;

        ls->size = READ_T;
        ls->file_id = g_obj_tmp->id;
        
        ls->next = g_label_sizeofs;
        g_label_sizeofs = ls;
      }
      
      /* global listfile block */
      if (listfile_block_read_global(&t, g_obj_tmp) == FAILED)
        return FAILED;

      /* appendto/after sections */
      i = READ_T;

      while (i > 0) {
        i--;

        g_after_tmp = calloc(1, sizeof(struct after_section));
        if (g_after_tmp == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }
        g_after_tmp->alive = YES;
        g_after_tmp->inserted = NO;

        g_after_tmp->section_id = READ_T;
        g_after_tmp->file_id = g_obj_tmp->id;
        g_after_tmp->is_appendto = *(t++);
        
        /* copy the names */
        for (x = 0; *t != 0; t++, x++)
          g_after_tmp->section[x] = *t;
        g_after_tmp->section[x] = 0;
        t++;
        for (x = 0; *t != 0; t++, x++)
          g_after_tmp->after[x] = *t;
        g_after_tmp->after[x] = 0;
        t++;

        g_after_tmp->section_s = NULL;
        g_after_tmp->after_s = NULL;
        
        g_after_tmp->next = g_after_sections;
        g_after_sections = g_after_tmp;
      }

      /* save pointer to data block area */
      g_obj_tmp->data_blocks = t;
    }

    g_obj_tmp = g_obj_tmp->next;
    section_id_base += 0x10000;
  }

  return SUCCEEDED;
}


static int _compare_sections(struct section *a, struct section *b) {
  
  if (a->priority < b->priority)
    return 1;
  else if (a->priority > b->priority)
    return -1;

  if (a->size < b->size)
    return 1;
  else if (a->size > b->size)
    return -1;

  return 0;
}


static int _find_section(char *section_name, struct section **section) {

  struct section *s = g_sec_first, *ss = NULL;
  char s1 = section_name[0];

  while (s != NULL) {
    if (s1 == s->name[0]) {
      if (strcmp(s->name, section_name) == 0) {
        if (ss != NULL) {
          fprintf(stderr, "_find_section(): Multiple sections called \"%s\" found for APPENDTO/AFTER operation. Please have only one section called \"%s\" in the source files.\n", section_name, section_name);
          return FAILED;
        }
        ss = s;
      }
    }
    s = s->next;
  }

  *section = ss;
  
  return SUCCEEDED;
}


static struct section *_find_after_source_section(struct after_section *as) {

  struct section *s = g_sec_first;
  
  /* use the name? */
  if (as->section_id < 0 && as->file_id < 0) {
    if (_find_section(as->section, &s) == FAILED)
      return FAILED;
    if (s == NULL)
      fprintf(stderr, "_find_after_source_section(): Cannot find section \"%s\" for appending! Is it defined in the source code? If it is, please submit a bug report!\n", as->section);
    return s;
  }

  /* use the IDs */
  while (s != NULL) {
    if (as->file_id == s->file_id && as->section_id == (s->id & 0xffff))
      return s;
    s = s->next;
  }
  
  fprintf(stderr, "_find_after_source_section(): Section \"%s\" from file \"%s\" has gone missing. Please submit a bug report! Retrying using its name...\n", as->section, get_file_name(as->file_id));

  s = g_sec_first;

  /* use the name... */
  while (s != NULL) {
    if (as->file_id == s->file_id && s->name[0] == as->section[0] && strcmp(s->name, as->section) == 0) {
      fprintf(stderr, "... Found it!\n");
      return s;
    }
    s = s->next;
  }

  fprintf(stderr, "_find_after_source_section(): Section \"%s\" from file \"%s\" cannot be found no matter what!\n", as->section, get_file_name(as->file_id));
  
  return NULL;
}


static void _kill_label(char *name, struct section *s) {

  struct label *l = g_labels_first;
  char c1 = name[0];

  while (l != NULL) {
    if (s->id == l->section && c1 == l->name[0]) {
      if (strcmp(name, l->name) == 0)
        l->alive = NO;
    }
    l = l->next;
  }
}


static struct sort_capsule *_create_sort_capsule(void) {

  struct sort_capsule *sc = calloc(sizeof(struct sort_capsule), 1);

  if (sc == NULL) {
    fprintf(stderr, "_create_sort_capsule(): Out of memory error.\n");
    return NULL;
  }

  sc->alive = YES;
  sc->section = NULL;
  sc->after_section = NULL;
  sc->next = NULL;
  sc->prev = NULL;
  sc->children = NULL;
  
  return sc;
}


static int _try_to_insert_after_section(struct sort_capsule *sort_capsules, struct after_section *as) {

  struct sort_capsule *sc = sort_capsules;
  
  while (sc != NULL) {
    if (sc->section == as->after_s) {
      /* we found the parent! now insert! */
      struct sort_capsule *scn = _create_sort_capsule();

      if (scn == NULL)
        return FAILED;

      scn->after_section = as;
      scn->section = as->section_s;

      if (sc->children == NULL)
        sc->children = scn;
      else if (_compare_sections(scn->section, sc->children->section) <= 0) {
        scn->next = sc->children;
        sc->children->prev = scn;
        sc->children = scn;
      }
      else {
        /* find a suitable spot among the children */
        struct sort_capsule *child = sc->children;

        while (child != NULL) {
          if (child->next == NULL) {
            child->next = scn;
            scn->prev = child;
            break;
          }
          else if (_compare_sections(scn->section, child->next->section) <= 0) {
            scn->next = child->next;
            child->next->prev = scn;
            child->next = scn;
            scn->prev = child;
            break;
          }

          child = child->next;
        }
      }

      return SUCCEEDED;
    }
    else if (sc->children != NULL) {
      if (_try_to_insert_after_section(sc->children, as) == SUCCEEDED)
        return SUCCEEDED;
    }
    
    sc = sc->next;
  }
  
  return FAILED;
}


static int _append_sections(struct section *s_source, struct section *s_target) {

  char label_tmp[MAX_NAME_LENGTH + 1];
  struct reference *r;
  struct stack *st;
  struct label *l;
  unsigned char *data;
  int size, offset;
  char c1;

  offset = s_target->size;
  size = s_source->size + s_target->size;
  data = calloc(size, 1);
  if (data == NULL) {
    fprintf(stderr, "_APPEND_SECTIONS: Out of memory while merging \"%s\" -> \"%s\" append.\n", s_source->name, s_target->name);
    return FAILED;
  }
  memcpy(data, s_target->data, s_target->size);
  memcpy(data + s_target->size, s_source->data, s_source->size);
    
  free(s_target->data);
  s_target->data = data;

  /* move labels */
  l = g_labels_first;
  while (l != NULL) {
    if (l->section == s_source->id) {
      l->address += s_target->size;
      l->section = s_target->id;
      l->bank = s_target->bank;
      l->slot = s_target->slot;
    }
    l = l->next;
  }

  /* try to adjust SECTIONEND_%s of the target as the target section has grown */
  snprintf(label_tmp, sizeof(label_tmp), "SECTIONEND_%s", s_target->name);

  c1 = label_tmp[0];
  l = g_labels_first;
  while (l != NULL) {
    if (c1 == l->name[0]) {
      if (strcmp(label_tmp, l->name) == 0) {
        l->address += s_source->size;
        break;
      }
    }
    l = l->next;
  }

  /* remove both SECTIONEND_%s and SECTIONSTART_%s of source */
  snprintf(label_tmp, sizeof(label_tmp), "SECTIONEND_%s", s_source->name);
  _kill_label(label_tmp, s_target);
  snprintf(label_tmp, sizeof(label_tmp), "SECTIONSTART_%s", s_source->name);
  _kill_label(label_tmp, s_target);
      
  /* move references */
  r = g_reference_first;
  while (r != NULL) {
    if (r->section == s_source->id) {
      r->address += s_target->size;
      r->section = s_target->id;
      r->bank = s_target->bank;
      r->slot = s_target->slot;
    }
    r = r->next;
  }
    
  /* move pending calculations */
  st = g_stacks_first;
  while (st != NULL) {
    if (st->section == s_source->id) {
      st->address += s_target->size;
      st->section = s_target->id;
      st->bank = s_target->bank;
      st->slot = s_target->slot;
    }
    st = st->next;
  }
      
  /* finalize */
  s_target->size = size;
  s_source->alive = NO;

  /* listfile writer needs the following data */
  s_source->appended_to = YES;
  s_source->appended_to_section = s_target;
  s_source->appended_to_offset = offset;

  return SUCCEEDED;
}  


static int _find_alive_appends(struct sort_capsule *sc) {

  struct sort_capsule *child;
  
  if (sc->children == NULL)
    return FAILED;

  child = sc->children;

  while (child != NULL) {
    if (child->alive == YES && child->after_section != NULL && child->after_section->is_appendto == YES)
      return SUCCEEDED;
    else if (child->children != NULL) {
      if (_find_alive_appends(child) == SUCCEEDED)
        return SUCCEEDED;
    }

    child = child->next;
  }
  
  return FAILED;
}


static int _has_appendto_children(struct sort_capsule *sc) {

  struct sort_capsule *child;
  
  if (sc == NULL || sc->children == NULL)
    return NO;

  child = sc->children;

  while (child != NULL) {
    if (child->alive == YES && child->after_section != NULL && child->after_section->is_appendto == YES)
      return YES;

    child = child->next;
  }

  return NO;
}


static int _find_and_execute_append(struct sort_capsule *sc) {

  struct sort_capsule *child;
  
  if (sc->children == NULL)
    return FAILED;

  child = sc->children;

  while (child != NULL) {
    if (child->alive == YES && child->after_section != NULL && child->after_section->is_appendto == YES &&
        _has_appendto_children(child) == NO) {
      /* sanity check */
      if (sc->section != child->after_section->after_s) {
        fprintf(stderr, "_find_and_execute_append(): The section dependency tree is corrupted! Please submit a bug report!\n");
        return FAILED;
      }
      
      /* we can do this APPENDTO! */
      if (_append_sections(child->after_section->section_s, child->after_section->after_s) == FAILED)
        return FAILED;
      else {
        /* this node has been taken care of */
        child->after_section->alive = NO;
        child->alive = NO;
        
        return SUCCEEDED;
      }
    }
    else if (child->children != NULL) {
      if (_find_and_execute_append(child) == SUCCEEDED)
        return SUCCEEDED;
    }

    child = child->next;
  }
  
  return FAILED;
}


static void _free_sort_capsules(struct sort_capsule *sc) {

  struct sort_capsule *next;

  while (sc != NULL) {
    next = sc->next;
    if (sc->children != NULL)
      _free_sort_capsules(sc->children);
    free(sc);
    sc = next;
  }
}


#if defined(WLALINK_DEBUG)

static void _print_sort_capsules(struct sort_capsule *sc, int indentation) {

  int i;
  
  while (sc != NULL) {
    for (i = 0; i < indentation; i++)
      printf(" ");
    printf("%s (%d", sc->section->name, sc->section->status);
    if (sc->after_section != NULL && sc->after_section->is_appendto == YES)
      printf(", AP");
    else if (sc->after_section != NULL && sc->after_section->is_appendto == NO)
      printf(", AF");
    printf(")\n");
    if (sc->children != NULL)
      _print_sort_capsules(sc->children, indentation + 2);
    sc = sc->next;
  }
}

#endif


static void _collect_back_sections(struct sort_capsule *sc) {

  while (sc != NULL) {
    if (g_sec_first == NULL) {
      g_sec_first = sc->section;
      g_sec_last = sc->section;
      g_sec_last->next = NULL;
      g_sec_last->prev = NULL;
    }
    else {
      g_sec_last->next = sc->section;
      sc->section->prev = g_sec_last;
      g_sec_last = sc->section;
      g_sec_last->next = NULL;
    }

    if (sc->children != NULL)
      _collect_back_sections(sc->children);
    
    sc = sc->next;
  }
}


static void _propagate_after_target_sections(struct sort_capsule *sc, struct sort_capsule *parent) {

  while (sc != NULL) {
    /* a living AFTER section? */
    if (sc->alive == YES && sc->after_section != NULL && sc->after_section->is_appendto == NO) {
      sc->section->after = parent->section;
      /* make the AFTER section to have the same type as the source section so that we can process
         both one after another in insert_sections() */
      sc->section->status = parent->section->status;
      /* also make sure the AFTER section inherits its source section's base, bank and slot... */
      sc->section->bank = parent->section->bank;
      sc->section->base = parent->section->base;
      sc->section->slot = parent->section->slot;
    }
    if (sc->children != NULL) {
      /* here if the node is a dead (processed) APPENDTO section then we'll pass its parent to its children */
      if (sc->alive == NO && sc->after_section != NULL && sc->after_section->is_appendto == YES)
        _propagate_after_target_sections(sc->children, parent);
      else
        _propagate_after_target_sections(sc->children, sc);
    }

    sc = sc->next;
  }
}


int append_sections(void) {

  struct after_section *as;
  struct section *s;
  struct sort_capsule *sort_capsules_first = NULL, *sort_capsules_last = NULL, *sc;
  int iterations, failures = 0;

  /* when we come here all the sections have been sorted using sort_sections() */
  
  /* clear all section markings */
  s = g_sec_first;
  while (s != NULL) {
    s->marked = NO;
    s = s->next;
  }

  if (g_after_sections == NULL)
    return SUCCEEDED;

  /* mark sections that are in appendto/after operations */
  as = g_after_sections;
  while (as != NULL) {
    as->section_s = _find_after_source_section(as);
    if (_find_section(as->after, &as->after_s) == FAILED)
      return FAILED;

    if (as->section_s == NULL) {
      fprintf(stderr, "APPEND_SECTIONS: Source section \"%s\" was not found, ignoring the -> \"%s\" APPENDTO/AFTER. This shouldn't actually happen so please submit a bug report!\n", as->section, as->after);
      as->alive = NO;
    }
    else if (as->after_s == NULL) {
      fprintf(stderr, "APPEND_SECTIONS: Target section \"%s\" was not found, ignoring the \"%s\" -> \"%s\" APPENDTO/AFTER.\n", as->after, as->section, as->after);
      as->alive = NO;
    }
    else {
      if (as->section_s->marked == YES) {
        /* another operation for this section! only one is allowed! */
        as->alive = NO;
        fprintf(stderr, "APPEND_SECTIONS: More than one APPENDTO/AFTER operation marked for section \"%s\" (in file \"%s\")! Only one is allowed thus disabling the following operations. Please make sure there is only one such operation.\n", as->section, get_file_name(as->file_id));
      }
      else
        as->section_s->marked = YES;
    }

    as = as->next;
  }

  /* put unmarked sections into sort capsules */
  s = g_sec_first;
  while (s != NULL) {
    if (s->marked == NO) {
      struct sort_capsule *sc = _create_sort_capsule();

      if (sc == NULL) {
        _free_sort_capsules(sort_capsules_first);
        return FAILED;
      }

      sc->section = s;
      sc->prev = sort_capsules_last;

      if (sort_capsules_first == NULL)
        sort_capsules_first = sc;
      else
        sort_capsules_last->next = sc;

      sort_capsules_last = sc;
    }
    
    s = s->next;
  }

  /* next make the appendto/after sections children of other sections */
  iterations = 1;
  while (iterations > 0) {
    /* this iteration will be the last, unless we succeeded in inserting a sections */
    iterations--;

    as = g_after_sections;
    while (as != NULL) {
      if (as->alive == YES && as->inserted == NO) {
        if (_try_to_insert_after_section(sort_capsules_first, as) == SUCCEEDED) {
          as->inserted = YES;
          /* we still need to try at least one another round */
          iterations = 1;
        }
      }

      as = as->next;
    }
  }

  /* check if we have appendto/after sections that we were unable to insert */
  as = g_after_sections;
  while (as != NULL) {
    if (as->alive == YES && as->inserted == NO) {
      if (as->is_appendto)
        fprintf(stderr, "APPEND_SECTIONS: Unable to inset operation \"%s\" APPENDTO \"%s\" into our sorting tree. Internal error. Please submit a bug resport!\n", as->section, as->after);
      else
        fprintf(stderr, "APPEND_SECTIONS: Unable to inset operation \"%s\" AFTER \"%s\" into our sorting tree. Internal error. Please submit a bug resport!\n", as->section, as->after);
      failures++;
    }

    as = as->next;
  }

  if (failures > 0) {
    _free_sort_capsules(sort_capsules_first);
    return FAILED;
  }

#if defined(WLALINK_DEBUG)
  
  fflush(stderr);
  fflush(stdout);

  printf("\n");
  printf("----------------------------------------------------------------------\n");
  printf("---                 SECTIONS (BEFORE MERGES)                       ---\n");
  printf("----------------------------------------------------------------------\n");
  printf("\n");

  _print_sort_capsules(sort_capsules_first, 0);

  fflush(stderr);
  fflush(stdout);

#endif
  
  /* execute APPENDTO operations */
  iterations = 1;
  while (iterations > 0) {
    /* this iteration will be the last, unless we succeeded in executing APPENDTO */
    iterations--;

    sc = sort_capsules_first;
    while (sc != NULL) {
      if (sc->children != NULL) {
        if (_find_and_execute_append(sc) == SUCCEEDED)
          iterations++;
      }
      
      sc = sc->next;
    }
  }

  /* make sure we don't have any living APPENDTO operations as all should have been executed in the previous step */
  sc = sort_capsules_first;
  while (sc != NULL) {
    if (sc->children != NULL) {
      if (_find_alive_appends(sc) == SUCCEEDED)
        failures++;
    }
      
    sc = sc->next;
  }

  if (failures > 0) {
    _free_sort_capsules(sort_capsules_first);
    return FAILED;
  }

  /* propagate AFTER target sections (an AFTER section might depend on an APPENDTO section
     that's already merged with its target section, we'll need to update such AFTER sections.
     also copy target section types to source sections as they need to be handled right away
     the source sections in insert_sections() */
  _propagate_after_target_sections(sort_capsules_first, NULL);
  
  /* collect back sections, and make sections to be able to do AFTER operation themselves in insert_sections() */
  g_sec_first = NULL;
  g_sec_last = NULL;
  _collect_back_sections(sort_capsules_first);

#if defined(WLALINK_DEBUG)
  
  /* print all sections */
  fflush(stderr);
  fflush(stdout);
  
  printf("\n");
  printf("----------------------------------------------------------------------\n");
  printf("---              SECTIONS (ORDERED, AFTER MERGES)                  ---\n");
  printf("----------------------------------------------------------------------\n");
  printf("\n");
  
  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES) {
      printf("\"%s\" (%d", s->name, s->status);
      if (s->after != NULL)
        printf(", AF");
      printf(")\n");
    }
    else {
      printf("\"%s\" (%d", s->name, s->status);
      if (s->after != NULL)
        printf(", AF");
      printf(") DEAD & MERGED\n");
    }
    s = s->next;
  }

  printf("\n");

  fflush(stderr);
  fflush(stdout);

#endif
  
  _free_sort_capsules(sort_capsules_first);
  
  return SUCCEEDED;
}


int parse_data_blocks(void) {

  struct section *s;
  int section_id_base, i, x;
  unsigned char *t, *p;
  char buf[256];

  g_obj_tmp = g_obj_first;
  section_id_base = 0;

  while (g_obj_tmp != NULL) {
    /* OBJECT FILE */
    if (g_obj_tmp->format == WLA_VERSION_OBJ) {
      t = g_obj_tmp->data_blocks;
      p = g_obj_tmp->data + g_obj_tmp->size;
      for ( ; t < p; ) {
        x = *(t++);

        if (x == DATA_TYPE_BLOCK) {
          /* address */
          i = READ_T;
          /* amount of bytes */
          x = READ_T;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing fixed data block from \"%s\".", g_obj_tmp->name);

          for (; x > 0; x--, i++)
            if (mem_insert(i, *(t++)) == FAILED)
              return FAILED;
        }
        else if (x == DATA_TYPE_SECTION) {
          s = calloc(sizeof(struct section), 1);
          if (s == NULL) {
            fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
            return FAILED;
          }

          /* name */
          i = 0;
          while (*t != SECTION_STATUS_FREE && *t != SECTION_STATUS_FORCE && *t != SECTION_STATUS_OVERWRITE &&
                 *t != SECTION_STATUS_HEADER && *t != SECTION_STATUS_SEMIFREE && *t != SECTION_STATUS_ABSOLUTE &&
                 *t != SECTION_STATUS_RAM_FREE && *t != SECTION_STATUS_SUPERFREE && *t != SECTION_STATUS_SEMISUBFREE &&
                 *t != SECTION_STATUS_RAM_FORCE && *t != SECTION_STATUS_RAM_SEMIFREE && *t != SECTION_STATUS_RAM_SEMISUBFREE &&
                 *t != SECTION_STATUS_SEMISUPERFREE)
            s->name[i++] = *(t++);
          s->name[i] = 0;
          s->status = *(t++);
          s->keep = *(t++);

          /* namespace */
          i = 0;
          while (*t != 0)
            buf[i++] = *(t++);
          buf[i] = 0;
          t++;
          if (buf[0] == 0)
            s->nspace = NULL;
          else {
            struct namespace_def *nspace;

            hashmap_get(g_namespace_map, buf, (void*)&nspace);
            if (nspace == NULL) {
              nspace = calloc(sizeof(struct namespace_def), 1);
              if (nspace == NULL) {
                fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
                return FAILED;
              }
              nspace->label_map = hashmap_new();
              strcpy(nspace->name, buf);
              hashmap_put(g_namespace_map, nspace->name, nspace);
            }

            s->nspace = nspace;
          }

          /* banks */
          if (s->status == SECTION_STATUS_SEMISUPERFREE) {
            i = 0;
            while (*t != 0)
              s->banks[i++] = *(t++);
            s->banks[i] = 0;
            t++;
          }
          
          s->id = READ_T;
          if (s->id >= 0x10000) {
            fprintf(stderr, "PARSE_DATA_BLOCKS: Section \"%s\"'s ID %d is too much! Currently we allow only 65535 sections inside an object file. Please open an issue about this in GitHub!\n", s->name, s->id);
          }
          s->id += section_id_base;
          s->slot = *(t++);
          s->file_id_source = READ_T;
          s->address = READ_T;
          s->bank = READ_T;
          s->base = READ_T;
          s->size = READ_T;
          s->alignment = READ_T;
          s->offset = READ_T;
          s->priority = READ_T;
          s->bitwindow = READ_T;
          s->window_start = READ_T;
          s->window_end = READ_T;
          s->data = t;

          s->library_status = OFF;
          s->label_map = hashmap_new();
          t += s->size;

          /* listfile block */
          if (listfile_block_read(&t, s) == FAILED)
            return FAILED;

          if (add_section(s) == FAILED)
            return FAILED;
        }
      }
      g_obj_tmp = g_obj_tmp->next;
      section_id_base += 0x10000;
      continue;
    }
    /* LIBRARY FILE */
    else if (g_obj_tmp->format == WLA_VERSION_LIB) {
      t = g_obj_tmp->data_blocks;
      p = g_obj_tmp->data + g_obj_tmp->size;
      for ( ; t < p; ) {
        s = calloc(sizeof(struct section), 1);
        if (s == NULL) {
          fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
          return FAILED;
        }

        /* name */
        i = 0;
        while (*t != SECTION_STATUS_FREE && *t != SECTION_STATUS_FORCE && *t != SECTION_STATUS_OVERWRITE &&
               *t != SECTION_STATUS_HEADER && *t != SECTION_STATUS_SEMIFREE && *t != SECTION_STATUS_ABSOLUTE &&
               *t != SECTION_STATUS_RAM_FREE && *t != SECTION_STATUS_SUPERFREE && *t != SECTION_STATUS_SEMISUBFREE &&
               *t != SECTION_STATUS_RAM_FORCE && *t != SECTION_STATUS_RAM_SEMIFREE && *t != SECTION_STATUS_RAM_SEMISUBFREE)
          s->name[i++] = *(t++);
        s->name[i] = 0;
        s->status = *(t++);
        s->keep = *(t++);
          
        /* namespace */
        i = 0;
        while (*t != 0)
          buf[i++] = *(t++);
        buf[i] = 0;
        t++;
        if (buf[0] == 0)
          s->nspace = NULL;
        else {
          struct namespace_def *nspace;

          hashmap_get(g_namespace_map, buf, (void*)&nspace);
          if (nspace == NULL) {
            nspace = calloc(sizeof(struct namespace_def), 1);
            if (nspace == NULL) {
              fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
              return FAILED;
            }
            nspace->label_map = hashmap_new();
            strcpy(nspace->name, buf);
            hashmap_put(g_namespace_map, nspace->name, nspace);
          }

          s->nspace = nspace;
        }

        s->id = READ_T;
        if (s->id >= 0x10000) {
          fprintf(stderr, "PARSE_DATA_BLOCKS: Section \"%s\"'s ID %d is too much! Currently we allow only 65535 sections inside a library file. Please open an issue about this in GitHub!\n", s->name, s->id);
        }
        s->id += section_id_base;
        s->file_id_source = READ_T;
        s->size = READ_T;
        s->alignment = READ_T;
        s->offset = READ_T;
        s->priority = READ_T;
        s->bitwindow = READ_T;
        s->window_start = READ_T;
        s->window_end = READ_T;
        s->data = t;
          
        s->address = 0;
        s->bank = g_obj_tmp->bank;
        s->slot = g_obj_tmp->slot;
        s->base = g_obj_tmp->base;
        s->library_status = ON;
        s->base_defined = g_obj_tmp->base_defined;
        s->label_map = hashmap_new();
        t += s->size;

        /* library RAM sections have no slots nor banks unless given in [rambanks] in linkfile */
        if (s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE ||
            s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
          s->bank = -1;
          s->slot = -1;
        }

        /* listfile block */
        if (listfile_block_read(&t, s) == FAILED)
          return FAILED;

        add_section(s);
      }
      g_obj_tmp = g_obj_tmp->next;
      section_id_base += 0x10000;
      continue;
    }
  }

  return SUCCEEDED;
}


int obtain_rombanks(void) {

  unsigned char *t;
  int rb = 0, k, s;


  /* obtain the biggest rom size */
  s = 0;
  g_obj_tmp = g_obj_first;

  while (g_obj_tmp != NULL) {
    if (g_obj_tmp->format == WLA_VERSION_OBJ) {

      t = g_obj_tmp->data + OBJ_ROMBANKS;
      k = t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24);

      g_obj_tmp->rom_banks = k;

      if (k != rb)
        s++;
      if (k > rb)
        rb = k;
    }
    g_obj_tmp = g_obj_tmp->next;
  }

  /* emptyfill has been obtained in the header checks */
  g_rombanks = rb;

  if (s > 1)
    fprintf(stderr, "OBTAIN_ROMBANKS: Using the biggest selected amount of ROM banks (%d).\n", g_rombanks);

  return SUCCEEDED;
}


int clean_up_dlr(void) {

  struct reference *r, *re;
  struct stack *s, *st;
  struct label *l, *la;
  struct section *se, *sec, *sect;
  char c1;

  se = g_sec_first;

  while (se != NULL) {
    /* remove duplicates of unique sections and all the related data */
    if (strlen(se->name) >= 3 && se->name[0] == '!' && se->name[1] == '_' && se->name[2] == '_') {
      c1 = se->name[0];
      sec = se->next;
      while (sec != NULL) {
        if (c1 == sec->name[0] && strcmp(se->name, sec->name) == 0) {
          /* free references */
          r = g_reference_first;
          while (r != NULL) {
            if (r->section_status == ON && r->section == sec->id) {
              re = r;
              if (re->prev == NULL)
                g_reference_first = re->next;
              else
                re->prev->next = re->next;
              if (re->next == NULL)
                g_reference_last = re->prev;
              else
                re->next->prev = re->prev;
              
              r = r->next;
              free(re);
            }
            else
              r = r->next;
          }
          
          /* free pending calculations */
          s = g_stacks_first;
          while (s != NULL) {
            if (s->section_status == ON && s->section == sec->id) {
              st = s->next;
              free_stack(s);
              s = st;
            }
            else
              s = s->next;
          }
          
          /* free labels */
          l = g_labels_first;
          while (l != NULL) {
            if (l->section_status == ON && l->section == sec->id) {
              la = l;
              if (la->prev == NULL)
                g_labels_first = la->next;
              else
                la->prev->next = la->next;
              if (la->next == NULL)
                g_labels_last = la->prev;
              else
                la->next->prev = la->prev;
              
              l = l->next;
              free(la);
            }
            else
              l = l->next;
          }

          sect = sec;
          sec = sec->next;

          free_section(sect);
        }
        else
          sec = sec->next;
      }
    }
    se = se->next;
  }

  return SUCCEEDED;
}

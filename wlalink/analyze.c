
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "analyze.h"
#include "memory.h"
#include "listfile.h"

#ifdef AMIGA
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


extern struct object_file *obj_first, *obj_last, *obj_tmp;
extern struct reference *reference_first, *reference_last;
extern struct section *sec_first, *sec_last, *sec_bankhd_first, *sec_bankhd_last;
extern struct stack *stacks_first, *stacks_last;
extern struct label *labels_first, *labels_last;
extern struct map_t *global_unique_label_map;
extern struct map_t *namespace_map;
extern struct slot slots[256];
extern struct append_section *append_sections, *append_tmp;
extern struct label_sizeof *label_sizeofs;
extern char mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern int rombanks, verbose_mode, section_overwrite, symbol_mode, discard_unreferenced_sections;
extern int emptyfill;
extern int *banksizes, *bankaddress, banksize;



int add_reference(struct reference *r) {

  r->file_id = obj_tmp->id;
  r->next = NULL;

  if (reference_first == NULL) {
    reference_first = r;
    reference_last = r;
    r->prev = NULL;
  }
  else {
    r->prev = reference_last;
    reference_last->next = r;
    reference_last = r;
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

  sta->file_id = obj_tmp->id;
  sta->next = NULL;
  sta->computed = NO;
  sta->under_work = NO;

  if (stacks_first == NULL) {
    stacks_first = sta;
    stacks_last = sta;
    sta->prev = NULL;
  }
  else {
    sta->prev = stacks_last;
    stacks_last->next = sta;
    stacks_last = sta;
  }

  return SUCCEEDED;
}


int add_section(struct section *s) {

  unsigned char *data;
  struct section *ss;

  
  /* create a local copy of the data */
  if (s->size > 0) {
    data = calloc(s->size, 1);
    if (data == NULL) {
      fprintf(stderr, "%s: ADD_SECTION: Out of memory.\n", obj_tmp->name);
      return FAILED;
    }

    memcpy(data, s->data, s->size);
    s->data = data;
  }
  else {
    s->data = NULL;
  }

  s->file_id = obj_tmp->id;
  s->next = NULL;
  s->alive = YES;

  if (strcmp(s->name, "BANKHEADER") == 0) {
    ss = sec_bankhd_first;
    while (ss != NULL) {
      if (ss->bank == s->bank) {
        fprintf(stderr, "%s: ADD_SECTION: BANKHEADER section for bank %d was defined for the second time.\n", obj_tmp->name, s->bank);
        return FAILED;
      }
      ss = ss->next;
    }

    if (sec_bankhd_first == NULL) {
      sec_bankhd_first = s;
      sec_bankhd_last = s;
    }
    else {
      sec_bankhd_last->next = s;
      sec_bankhd_last = s;
    }
  }
  else {
    if (sec_first == NULL) {
      sec_first = s;
      sec_last = s;
      s->prev = NULL;
    }
    else {
      s->prev = sec_last;
      sec_last->next = s;
      sec_last = s;
    }
  }

  return SUCCEEDED;
}


int free_section(struct section *s) {

  if (s == NULL)
    return FAILED;

  if (s->prev != NULL)
    s->prev->next = s->next;
  if (s->next != NULL)
    s->next->prev = s->prev;
  else
    sec_last = s->prev;

  /* free label map */
  hashmap_free(s->label_map);
          
  if (s->data != NULL)
    free(s->data);
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

    if (hashmap_get(namespace_map, prefix, (void*)&nspace) == MAP_OK) {
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
    /* check the section's labels. This is a bit redundant but it might have
     * local labels (labels starting with an underscore)
     */
    if (hashmap_get(s->label_map, str, (void*)&l) == MAP_OK) {
      *out = l;
      return SUCCEEDED;
    }
  }
  /* check the global namespace */
  if (hashmap_get(global_unique_label_map, str, (void*)&l) == MAP_OK) {
    *out = l;
    return SUCCEEDED;
  }

  return FAILED;
}


int add_label(struct label *l) {

  l->next = NULL;
  l->alive = YES;

  if (labels_first == NULL) {
    labels_first = l;
    labels_last = l;
    l->prev = NULL;
  }
  else {
    l->prev = labels_last;
    labels_last->next = l;
    labels_last = l;
  }

  return SUCCEEDED;
}


int obtain_rombankmap(void) {

  int map_found = OFF, i, x, a;
  struct object_file *o;
  unsigned char *t;

  
  /* initialize values */
  for (i = 0; i < rombanks; i++)
    banksizes[i] = 0;

  o = obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      t = o->data + OBJ_ROMBANKMAP;

      /* obtain status */
      i = *t;
      t++;

      /* general rombanksize? */
      if (i == 0) {
        /* obtain banksize */
        banksize = READ_T;

        o->memorymap = t;
        map_found = ON;
        for (i = 0; i < o->rom_banks; i++) {
          if (banksizes[i] == 0) {
            banksizes[i] = banksize;
            bankaddress[i] = i * banksize;
          }
          else if (banksizes[i] != banksize) {
            fprintf(stderr, "OBTAIN_ROMBANKMAP: ROMBANKMAPs don't match.\n");
            return FAILED;
          }
        }
      }
      else {
        for (a = 0, x = 0; x < o->rom_banks; x++) {
          banksize = READ_T;
          if (banksizes[x] == 0) {
            banksizes[x] = banksize;
            bankaddress[x] = a;
          }
          else if (banksizes[x] != banksize) {
            fprintf(stderr, "OBTAIN_ROMBANKMAP: ROMBANKMAPs don't match.\n");
            return FAILED;
          }
          a += banksize;
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

  
  o = obj_first;
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
      s->id = *(t++);
      s->checksum = (unsigned) READ_T;
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

  
  for (i = 0; i < 256; i++)
    slots[i].usage = OFF;

  o = obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      t = o->memorymap;

      /* obtain slots */
      i = *t;
      t++;

      for (x = 0; i > 0; i--, x++) {
        slots[x].usage = ON;
        slots[x].address =  READ_T;
        slots[x].size =  READ_T;
        for (y = 0; *t != 0; t++, y++)
          slots[x].name[y] = *t;
        slots[x].name[y] = 0;
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
          if (slots[i].usage == OFF) {
            x = 1;
            break;
          }
          if (slots[i].address == s[i].address && slots[i].size == s[i].size) {
            if (slots[i].name[0] == 0 && s[i].name[0] != 0) {
              /* use the name given to the other slot */
              strcpy(slots[i].name, s[i].name);
            }
            else if (slots[i].name[0] != 0 && s[i].name[0] != 0) {
              /* check that the names match */
              if (strcmp(slots[i].name, s[i].name) != 0)
                fprintf(stderr, "OBTAIN_MEMORYMAP: SLOT %d has two different names (\"%s\" and \"%s\"). Using \"%s\"...\n",
                        i, slots[i].name, s[i].name, slots[i].name);
            }
            continue;
          }
          x = 1;
          break;
        }
        else {
          if (slots[i].usage == ON) {
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
  int section, x, i, n, q;
  unsigned char *t, *dtmp;
  double dou;

  
  section = 0;
  obj_tmp = obj_first;
  while (obj_tmp != NULL) {
    /* OBJECT FILE */
    if (obj_tmp->format == WLA_VERSION_OBJ) {
      t = obj_tmp->exported_defines;
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
        l->file_id = obj_tmp->id;
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
        l->file_id_source = *(t++);

        l->section = READ_T;
        if (l->section == 0)
          l->section_status = OFF;
        else {
          l->section_status = ON;
          l->section += section;
        }
        l->address = READ_T;
        l->linenumber = READ_T;
        l->bank = READ_T;
        l->base = READ_T;
        l->file_id = obj_tmp->id;
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
        r->file_id_source = *(t++);
        r->slot = *(t++);
        r->section = READ_T;
        if (r->section == 0)
          r->section_status = OFF;
        else {
          r->section_status = ON;
          r->section += section;
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
          s->section += section;
        }
        s->file_id_source = *(t++);
        x = *(t++);
        s->position = *(t++);
        s->slot = *(t++);
        s->address = READ_T;
        s->linenumber = READ_T;
        s->bank = READ_T;
        s->base = READ_T;
        s->stacksize = x;
        
        s->stack = calloc(sizeof(struct stack_item) * x, 1);
        if (s->stack == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          free(s);
          return FAILED;
        }
        
        add_stack(s);

        for (n = 0; n != x; n++) {
          s->stack[n].slot = -1;
          s->stack[n].base = -1;
          s->stack[n].type = *(t++);
          s->stack[n].sign = *(t++);
          if (s->stack[n].type == STACK_ITEM_TYPE_STRING) {
            for (q = 0; *t != 0; t++, q++)
              s->stack[n].string[q] = *t;
            s->stack[n].string[q] = 0;
            t++;
          }
          else {
            READ_DOU;
            s->stack[n].value_ram = dou;
            s->stack[n].value_rom = dou;
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
        ls->file_id = obj_tmp->id;
        
        ls->next = label_sizeofs;
        label_sizeofs = ls;
      }

      /* append sections */
      i = READ_T;

      while (i > 0) {
        i--;

        append_tmp = calloc(1, sizeof(struct append_section));
        if (append_tmp == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the names */
        for (x = 0; *t != 0; t++, x++)
          append_tmp->section[x] = *t;
        append_tmp->section[x] = 0;
        t++;
        for (x = 0; *t != 0; t++, x++)
          append_tmp->append_to[x] = *t;
        append_tmp->append_to[x] = 0;
        t++;
        
        append_tmp->next = append_sections;
        append_sections = append_tmp;
      }

      /* save pointer to data block area */
      obj_tmp->data_blocks = t;
    }
    /* LIBRARY FILE */
    else if (obj_tmp->format == WLA_VERSION_LIB) {
      t = obj_tmp->exported_defines;
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
        l->bank = obj_tmp->bank;
        l->slot = obj_tmp->slot;
        l->base = obj_tmp->base;
        l->file_id = obj_tmp->id;
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
        l->section += section;
        l->file_id_source = *(t++);
        l->linenumber = READ_T;
        l->section_status = ON;
        l->address = READ_T;
        l->base = obj_tmp->base; /* (((int)l->address) >> 16) & 0xFF; */
        l->bank = obj_tmp->bank;
        l->slot = obj_tmp->slot;
        l->file_id = obj_tmp->id;

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
        r->section += section;
        r->file_id_source = *(t++);
        r->linenumber = READ_T;
        r->section_status = ON;
        r->address = READ_T;

        r->bank = obj_tmp->bank;
        r->slot = obj_tmp->slot;
        r->base = obj_tmp->base;

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
          s->section += section;
        }
        s->file_id_source = *(t++);
        x = *(t++);
        s->position = *(t++);
        s->address = READ_T;
        s->linenumber = READ_T;
        s->stacksize = x;
        s->bank = obj_tmp->bank;
        s->slot = obj_tmp->slot;
        s->base = obj_tmp->base;
        
        s->stack = calloc(sizeof(struct stack_item) * x, 1);
        if (s->stack == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          free(s);
          return FAILED;
        }

        add_stack(s);

        for (n = 0; n != x; n++) {
          s->stack[n].slot = -1;
          s->stack[n].base = -1;
          s->stack[n].type = *(t++);
          s->stack[n].sign = *(t++);
          if (s->stack[n].type == STACK_ITEM_TYPE_STRING) {
            for (q = 0; *t != 0; t++, q++)
              s->stack[n].string[q] = *t;
            s->stack[n].string[q] = 0;
            t++;
          }
          else {
            READ_DOU;
            s->stack[n].value_ram = dou;
            s->stack[n].value_rom = dou;
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
        ls->file_id = obj_tmp->id;
        
        ls->next = label_sizeofs;
        label_sizeofs = ls;
      }
      
      /* append sections */
      i = READ_T;

      while (i > 0) {
        i--;

        append_tmp = calloc(1, sizeof(struct append_section));
        if (append_tmp == NULL) {
          fprintf(stderr, "COLLECT_DLR: Out of memory.\n");
          return FAILED;
        }

        /* copy the names */
        for (x = 0; *t != 0; t++, x++)
          append_tmp->section[x] = *t;
        append_tmp->section[x] = 0;
        t++;
        for (x = 0; *t != 0; t++, x++)
          append_tmp->append_to[x] = *t;
        append_tmp->append_to[x] = 0;
        t++;
        
        append_tmp->next = append_sections;
        append_sections = append_tmp;
      }

      /* save pointer to data block area */
      obj_tmp->data_blocks = t;
    }

    obj_tmp = obj_tmp->next;
    section += 1000000;
  }

  return SUCCEEDED;
}


int merge_sections(void) {

  int warning_given_s = NO, warning_given_t = NO, size;
  struct section *s, *s_source, *s_target;
  struct append_section *as;
  unsigned char *data;
  struct reference *r;
  struct stack *st;
  struct label *l;


  as = append_sections;
  while (as != NULL) {
    s_source = NULL;
    s_target = NULL;
    
    s = sec_first;
    while (s != NULL) {
      if (strcmp(as->section, s->name) == 0) {
        if (s_source != NULL && warning_given_s == NO) {
          fprintf(stderr, "MERGE_SECTIONS: Multiple source sections called \"%s\" found, using the last one for append.\n", s->name);
          warning_given_s = YES;
        }
        s_source = s;
      }
      else if (strcmp(as->append_to, s->name) == 0) {
        if (s_target != NULL && warning_given_t == NO) {
          fprintf(stderr, "MERGE_SECTIONS: Multiple target sections called \"%s\" found, using the last one for append.\n", s->name);
          warning_given_t = YES;
        }
        s_target = s;
      }
      s = s->next;
    }

    if (s_source == NULL)
      fprintf(stderr, "MERGE_SECTIONS: Source section \"%s\" was not found, ignoring the \"%s\" -> \"%s\" append.\n", as->section, as->section, as->append_to);
    else if (s_target == NULL)
      fprintf(stderr, "MERGE_SECTIONS: Target section \"%s\" was not found, ignoring the \"%s\" -> \"%s\" append.\n", as->append_to, as->section, as->append_to);
    else {
      /* merge data */
      size = s_source->size + s_target->size;
      data = calloc(size, 1);
      if (data == NULL) {
        fprintf(stderr, "MERGE_SECTIONS: Out of memory while merging \"%s\" -> \"%s\" append.\n", as->section, as->append_to);
        return FAILED;
      }
      memcpy(data, s_target->data, s_target->size);
      memcpy(data + s_target->size, s_source->data, s_source->size);

      free(s_target->data);
      s_target->data = data;
      
      /* move labels */
      l = labels_first;
      while (l != NULL) {
        if (l->section == s_source->id) {
          l->address += s_target->size;
          l->section = s_target->id;
          l->bank = s_target->bank;
          l->slot = s_target->slot;
        }
        l = l->next;
      }

      /* move references */
      r = reference_first;
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
      st = stacks_first;
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

      /* kill the appended section */
      free_section(s_source);
    }
    
    as = as->next;
  }
  
  return SUCCEEDED;
}


int parse_data_blocks(void) {

  struct section *s;
  int section, i, x;
  unsigned char *t, *p;
  char buf[256];


  obj_tmp = obj_first;
  section = 0;

  while (obj_tmp != NULL) {
    /* OBJECT FILE */
    if (obj_tmp->format == WLA_VERSION_OBJ) {
      t = obj_tmp->data_blocks;
      p = obj_tmp->data + obj_tmp->size;
      for ( ; t < p; ) {
        x = *(t++);

        if (x == DATA_TYPE_BLOCK) {
          /* address */
          i = READ_T;
          /* amount of bytes */
          x = READ_T;

          /* create a what-we-are-doing message for mem_insert*() warnings/errors */
          snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing fixed data block from \"%s\".", obj_tmp->name);

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

            hashmap_get(namespace_map, buf, (void*)&nspace);
            if (nspace == NULL) {
              nspace = calloc(sizeof(struct namespace_def), 1);
              if (nspace == NULL) {
                fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
                return FAILED;
              }
              nspace->label_map = hashmap_new();
              strcpy(nspace->name, buf);
              hashmap_put(namespace_map, nspace->name, nspace);
            }

            s->nspace = nspace;
          }

          s->id = READ_T;
          s->id += section;
          s->slot = *(t++);
          s->file_id_source = *(t++);
          s->address = READ_T;
          s->bank = READ_T;
          s->base = READ_T;
          s->size = READ_T;
          s->alignment = READ_T;
          s->offset = READ_T;
          s->priority = READ_T;
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
      obj_tmp = obj_tmp->next;
      section += 1000000;
      continue;
    }
    /* LIBRARY FILE */
    else if (obj_tmp->format == WLA_VERSION_LIB) {
      t = obj_tmp->data_blocks;
      p = obj_tmp->data + obj_tmp->size;
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

          hashmap_get(namespace_map, buf, (void*)&nspace);
          if (nspace == NULL) {
            nspace = calloc(sizeof(struct namespace_def), 1);
            if (nspace == NULL) {
              fprintf(stderr, "PARSE_DATA_BLOCKS: Out of memory.\n");
              return FAILED;
            }
            nspace->label_map = hashmap_new();
            strcpy(nspace->name, buf);
            hashmap_put(namespace_map, nspace->name, nspace);
          }

          s->nspace = nspace;
        }

        s->id = READ_T;
        s->id += section;
        s->file_id_source = *(t++);
        s->size = READ_T;
        s->alignment = READ_T;
        s->offset = READ_T;
        s->priority = READ_T;
        s->data = t;
        s->address = 0;
        s->bank = obj_tmp->bank;
        s->slot = obj_tmp->slot;
        s->base = obj_tmp->base;
        s->library_status = ON;
        s->base_defined = obj_tmp->base_defined;
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
      obj_tmp = obj_tmp->next;
      section += 1000000;
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
  obj_tmp = obj_first;

  while (obj_tmp != NULL) {
    if (obj_tmp->format == WLA_VERSION_OBJ) {

      t = obj_tmp->data + OBJ_ROMBANKS;
      k = t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24);

      obj_tmp->rom_banks = k;

      if (k != rb)
        s++;
      if (k > rb)
        rb = k;
    }
    obj_tmp = obj_tmp->next;
  }

  /* emptyfill has been obtained in the header checks */
  rombanks = rb;

  if (s > 1)
    fprintf(stderr, "OBTAIN_ROMBANKS: Using the biggest selected amount of ROM banks (%d).\n", rombanks);

  return SUCCEEDED;
}


int clean_up_dlr(void) {

  struct reference *r, *re;
  struct stack *s, *st;
  struct label *l, *la;
  struct section *se, *sec, *sect;


  se = sec_first;

  while (se != NULL) {
    /* remove duplicates of unique sections and all the related data */
    if (strlen(se->name) >= 3 && se->name[0] == '!' && se->name[1] == '_' && se->name[2] == '_') {
      sec = se->next;
      while (sec != NULL) {
        if (strcmp(se->name, sec->name) == 0) {
          /* free references */
          r = reference_first;
          while (r != NULL) {
            if (r->section_status == ON && r->section == sec->id) {
              re = r;
              if (re->prev == NULL)
                reference_first = re->next;
              else
                re->prev->next = re->next;
              if (re->next == NULL)
                reference_last = re->prev;
              else
                re->next->prev = re->prev;
              
              r = r->next;
              free(re);
            }
            else
              r = r->next;
          }
          
          /* free pending calculations */
          s = stacks_first;
          while (s != NULL) {
            if (s->section_status == ON && s->section == sec->id) {
              st = s;
              if (st->stack != NULL) {
                free(st->stack);
                st->stack = NULL;
              }
              if (st->prev == NULL)
                stacks_first = st->next;
              else
                st->prev->next = st->next;
              if (st->next == NULL)
                stacks_last = st->prev;
              else
                st->next->prev = st->prev;
              
              s = s->next;
              free(st);
            }
            else
              s = s->next;
          }
          
          /* free labels */
          l = labels_first;
          while (l != NULL) {
            if (l->section_status == ON && l->section == sec->id) {
              la = l;
              if (la->prev == NULL)
                labels_first = la->next;
              else
                la->prev->next = la->next;
              if (la->next == NULL)
                labels_last = la->prev;
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

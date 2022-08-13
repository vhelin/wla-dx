
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "discard.h"
#include "files.h"
#include "write.h"
#include "analyze.h"


extern struct reference *g_reference_first, *g_reference_last;
extern struct section *g_sec_first, *g_sec_last;
extern struct section *g_sec_bankhd_first;
extern struct label *g_labels_first, *g_labels_last;
extern struct stack *g_stacks_first, *g_stacks_last;

extern int g_verbose_level;


int discard_unused_sections(void) {

  struct section *s;
  int a = 0, b = -1;

  
  /* iterate section discarding until there's no change in the amount of dropped sections */
  while (a != b) {
    s = g_sec_first;
    while (s != NULL) {
      s->referenced = 0;
      s = s->next;
    }

    a = b;
    discard_iteration();

    b = 0;
    s = g_sec_first;
    while (s != NULL) {
      if (s->referenced == 0)
        s->alive = NO;
      else
        s->alive = YES;
      if (s->alive == NO)
        b++;
      s = s->next;
    }
  }

  if (g_verbose_level >= 1) {
    /* announce all the unreferenced sections that will get dropped */
    s = g_sec_first;
    while (s != NULL) {
      if (s->alive == NO)
        fprintf(stderr, "DISCARD: %s: %s: Section \"%s\" was discarded.\n",
                get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);
      s = s->next;
    }
  }
  
  return SUCCEEDED;
}


int discard_iteration(void) {

  struct reference *r;
  struct stack_item *si;
  struct section *s, *ss;
  struct label *l;
  struct stack *st;
  int i;

  
  /* check section names for special characters '!', and check if the section is of proper type */
  s = g_sec_first;
  while (s != NULL) {
    if (s->keep == YES || s->name[0] == '!' || !(s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_SEMIFREE || s->status == SECTION_STATUS_SEMISUBFREE || s->status == SECTION_STATUS_SUPERFREE || s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE)) {
      s->referenced++;
      s->alive = YES;
    }
    s = s->next;
  }

  /* loop through references */
  r = g_reference_first;
  while (r != NULL) {
    /* references to local labels don't count */
    if (r->name[0] == '_' || is_label_anonymous(r->name) == YES) {
      r = r->next;
      continue;
    }

    s = NULL;
    if (r->section_status != 0) {
      s = g_sec_first;
      while (s != NULL) {
        if (s->id == r->section)
          break;
        s = s->next;
      }
    }
    find_label(r->name, s, &l);

    if (l != NULL && l->section_status == ON) {
      s = l->section_struct;
      if (s == NULL)
        fprintf(stderr, "DISCARD_ITERATION: Internal error! Please send a bug report.\n");
      if (r->section_status == OFF)
        s->referenced++;
      else if (r->section != s->id) {
        ss = g_sec_bankhd_first;
        /* find it in special sections first */
        while (ss != NULL && ss->id != r->section)
          ss = ss->next;
        if (ss == NULL) {
          ss = g_sec_first;
          while (ss->id != r->section)
            ss = ss->next;
        }
        if (ss->alive == YES)
          s->referenced++;
      }
    }
    r = r->next;
  }

  /* loop through computations */
  st = g_stacks_first;
  while (st != NULL) {
    ss = NULL;
    if (st->section_status != 0) {
      ss = g_sec_first;
      while (ss != NULL) {
        if (ss->id == st->section)
          break;
        ss = ss->next;
      }
    }

    si = st->stack;
    i = 0;
    while (i != st->stacksize) {
      if (si->type == STACK_ITEM_TYPE_LABEL && is_label_anonymous(si->string) == NO) {
        find_label(si->string, ss, &l);

        if (l != NULL && l->section_struct != NULL) {
          s = l->section_struct;
          if (st->section_status == OFF)
            s->referenced++;
          else if (st->section != s->id) {
            if (ss->alive == YES)
              s->referenced++;
          }
        }
      }
      si++;
      i++;
    }
    st = st->next;
  }


  return SUCCEEDED;
}


/* drop labels that are inside discarded sections */
int discard_dropped_labels(void) {

  struct section *s;
  struct label *l;

  
  l = g_labels_first;
  while (l != NULL) {
    if (l->section_status == ON) {
      /* find the section */
      s = g_sec_first;
      while (s->id != l->section)
        s = s->next;
      /* is it dead? */
      if (s->alive == NO) {
        /* nope! remove the label! */

        /* don't actually free the label from memory, but mark it dead
           struct label *ll;

           if (l->prev == NULL)
           labels_first = l->next;
           else
           l->prev->next = l->next;
           if (l->next != NULL)
           l->next->prev = l->prev;
           ll = l->next;

           free(l);

           l = ll;
        */
        l->alive = NO;

        l = l->next;
      }
      else
        l = l->next;
    }
    else
      l = l->next;
  }

  return SUCCEEDED;
}

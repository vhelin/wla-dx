
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "discard.h"
#include "files.h"
#include "write.h"
#include "analyze.h"
#include "main.h"


extern struct reference *g_reference_first, *g_reference_last;
extern struct section *g_sec_first, *g_sec_last;
extern struct section *g_sec_bankhd_first;
extern struct label *g_labels_first, *g_labels_last;
extern struct stack *g_stacks_first, *g_stacks_last;

extern int g_verbose_level;


int discard_unused_sections(void) {

  struct section *s;
  int num_dropped_before = 0, num_dropped_after = -1;
  
  /* iterate section discarding until there's no change in the amount of dropped sections */
  while (num_dropped_before != num_dropped_after) {
    s = g_sec_first;
    while (s != NULL) {
      s->referenced = 0;
      s = s->next;
    }

    num_dropped_before = num_dropped_after;
    discard_iteration();

    num_dropped_after = 0;
    s = g_sec_first;
    while (s != NULL) {
      if (s->referenced == 0)
        s->alive = NO;
      else
        s->alive = YES;
      if (s->alive == NO)
        num_dropped_after++;
      s = s->next;
    }
  }

  if (g_verbose_level >= 1) {
    /* announce all the unreferenced sections that will get dropped */
    s = g_sec_first;
    while (s != NULL) {
      /* skip .SECTIONs that have been appended to other .SECTIONs */
      if (s->alive == NO && s->appended_to == NO)
        print_text(NO, "DISCARD: %s: %s: Section \"%s\" was discarded.\n",
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

    if (r->section_status != 0)
      s = find_section(r->section);
    else
      s = NULL;

    find_label(r->name, s, &l);

    if (l != NULL && l->section_status == ON) {
      s = l->section_struct;
      if (s == NULL)
        print_text(NO, "DISCARD_ITERATION: Internal error! Please send a bug report.\n");
      if (r->section_status == OFF)
        s->referenced++;
      else if (r->section != s->id) {
        ss = g_sec_bankhd_first;
        /* find it in special sections first */
        while (ss != NULL && ss->id != r->section)
          ss = ss->next;
        if (ss == NULL)
          ss = find_section(r->section);
        if (ss->alive == YES)
          s->referenced++;
      }
    }
    r = r->next;
  }

  /* loop through computations */
  st = g_stacks_first;
  while (st != NULL) {
    if (st->section_status != 0)
      ss = find_section(st->section);
    else
      ss = NULL;

    si = st->stack_items;
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
      s = find_section(l->section);

      /* is it dead? */
      if (s->alive == NO) {
        /* don't actually free the label from memory, but mark it dead */
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

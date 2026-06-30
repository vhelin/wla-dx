#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "files.h"
#include "write.h"
#include "dbgmodel.h"

#if defined(AMIGA)
#include "/printf.h"
#else
#include "../printf.h"
#endif

extern struct object_file *g_obj_first;
extern struct section *g_sec_first, *g_sec_bankhd_first;
extern struct label *g_labels_first;
extern struct slot g_slots[256];
extern int *g_bankaddress;
extern int g_romsize, g_rombanks;

static char *_debug_strdup(const char *s) {

  char *d;
  int len;

  if (s == NULL)
    s = "";

  len = (int)strlen(s);
  d = calloc(len + 1, 1);
  if (d == NULL)
    return NULL;

  strcpy(d, s);

  return d;
}


int debug_model_is_ram_section_status(int status) {

  if (status == SECTION_STATUS_RAM_FREE || status == SECTION_STATUS_RAM_FORCE ||
      status == SECTION_STATUS_RAM_SEMIFREE || status == SECTION_STATUS_RAM_SEMISUBFREE)
    return YES;

  return NO;
}


const char *debug_model_label_status_name(int status) {

  if (status == LABEL_STATUS_LABEL)
    return "label";
  if (status == LABEL_STATUS_DEFINE)
    return "definition";
  if (status == LABEL_STATUS_STACK)
    return "stack";
  if (status == LABEL_STATUS_SYMBOL)
    return "symbol";
  if (status == LABEL_STATUS_BREAKPOINT)
    return "breakpoint";
  if (status == LABEL_STATUS_STRING)
    return "string";

  return "unknown";
}


const char *debug_model_section_status_name(int status) {

  if (status == SECTION_STATUS_FREE)
    return "free";
  if (status == SECTION_STATUS_FORCE)
    return "force";
  if (status == SECTION_STATUS_OVERWRITE)
    return "overwrite";
  if (status == SECTION_STATUS_HEADER)
    return "header";
  if (status == SECTION_STATUS_SEMIFREE)
    return "semifree";
  if (status == SECTION_STATUS_ABSOLUTE)
    return "absolute";
  if (status == SECTION_STATUS_RAM_FREE)
    return "ramfree";
  if (status == SECTION_STATUS_SUPERFREE)
    return "superfree";
  if (status == SECTION_STATUS_SEMISUBFREE)
    return "semisubfree";
  if (status == SECTION_STATUS_RAM_FORCE)
    return "ramforce";
  if (status == SECTION_STATUS_RAM_SEMIFREE)
    return "ramsemifree";
  if (status == SECTION_STATUS_RAM_SEMISUBFREE)
    return "ramsemisubfree";
  if (status == SECTION_STATUS_SEMISUPERFREE)
    return "semisuperfree";

  return "unknown";
}


struct debug_section *debug_model_find_section(struct debug_model *model, int id) {

  int i;

  if (model == NULL)
    return NULL;

  for (i = 0; i < model->section_count; i++) {
    if (model->sections[i].id == id)
      return &model->sections[i];
  }

  return NULL;
}


struct debug_source *debug_model_find_source(struct debug_model *model, int file_id, int source_id) {

  int i;

  if (model == NULL)
    return NULL;

  for (i = 0; i < model->source_count; i++) {
    if (model->sources[i].file_id == file_id && model->sources[i].source_id == source_id)
      return &model->sources[i];
  }

  return NULL;
}


static int _count_sections(struct section *s) {

  int count;

  count = 0;
  while (s != NULL) {
    if (s->alive == YES)
      count++;
    s = s->next;
  }

  return count;
}


static void _copy_section(struct debug_section *dst, struct section *src) {

  strcpy(dst->name, src->name);
  dst->id = src->id;
  dst->status = src->status;
  dst->file_id = src->file_id;
  dst->file_id_source = src->file_id_source;
  dst->address = src->address;
  dst->output_address = src->output_address;
  dst->pc_address = g_slots[src->slot].address + src->address;
  dst->size = src->size;
  dst->bank = src->bank;
  dst->base = src->base;
  dst->slot = src->slot;
  dst->slot_address = g_slots[src->slot].address;
  dst->is_ram = debug_model_is_ram_section_status(src->status);
  dst->is_bankheader = src->is_bankheader_section;
}


static int _build_sources(struct debug_model *model) {

  struct object_file *obj;
  struct source_file_name *src;
  int count, i;

  count = 0;
  obj = g_obj_first;
  while (obj != NULL) {
    src = obj->source_file_names_list;
    while (src != NULL) {
      count++;
      src = src->next;
    }
    obj = obj->next;
  }

  if (count == 0)
    return SUCCEEDED;

  model->sources = calloc(sizeof(struct debug_source) * count, 1);
  if (model->sources == NULL)
    return FAILED;

  model->source_count = count;
  i = 0;
  obj = g_obj_first;
  while (obj != NULL) {
    src = obj->source_file_names_list;
    while (src != NULL) {
      model->sources[i].file_id = obj->id;
      model->sources[i].source_id = src->id;
      model->sources[i].checksum = src->checksum;
      model->sources[i].name = _debug_strdup(src->name);
      if (model->sources[i].name == NULL)
        return FAILED;
      i++;
      src = src->next;
    }
    obj = obj->next;
  }

  return SUCCEEDED;
}


static int _build_sections(struct debug_model *model) {

  struct section *s;
  int i, count;

  count = _count_sections(g_sec_first) + _count_sections(g_sec_bankhd_first);
  if (count == 0)
    return SUCCEEDED;

  model->sections = calloc(sizeof(struct debug_section) * count, 1);
  if (model->sections == NULL)
    return FAILED;

  model->section_count = count;
  i = 0;
  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES)
      _copy_section(&model->sections[i++], s);
    s = s->next;
  }
  s = g_sec_bankhd_first;
  while (s != NULL) {
    if (s->alive == YES)
      _copy_section(&model->sections[i++], s);
    s = s->next;
  }

  return SUCCEEDED;
}


static int _build_labels(struct debug_model *model) {

  struct label *l;
  int count, i;

  count = 0;
  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES)
      count++;
    l = l->next;
  }

  if (count == 0)
    return SUCCEEDED;

  model->labels = calloc(sizeof(struct debug_label) * count, 1);
  if (model->labels == NULL)
    return FAILED;

  model->label_count = count;
  i = 0;
  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES) {
      strcpy(model->labels[i].name, l->name);
      model->labels[i].status = l->status;
      model->labels[i].file_id = l->file_id;
      model->labels[i].file_id_source = l->file_id_source;
      model->labels[i].linenumber = l->linenumber;
      model->labels[i].section = l->section;
      model->labels[i].section_status = l->section_status;
      model->labels[i].address = (int)l->address;
      model->labels[i].rom_address = (int)l->rom_address;
      model->labels[i].address_in_section = l->address_in_section;
      model->labels[i].bank = l->bank;
      model->labels[i].base = l->base;
      model->labels[i].slot = l->slot;
      model->labels[i].is_anonymous = is_label_anonymous(l->name);
      i++;
    }
    l = l->next;
  }

  return SUCCEEDED;
}


static int _append_mapping(struct debug_model *model, int *count, int *capacity, struct debug_mapping *mapping) {

  struct debug_mapping *new_mappings;

  if (*count >= *capacity) {
    int new_capacity;

    new_capacity = (*capacity == 0) ? 64 : (*capacity * 2);
    new_mappings = realloc(model->mappings, sizeof(struct debug_mapping) * new_capacity);
    if (new_mappings == NULL)
      return FAILED;
    model->mappings = new_mappings;
    memset(&model->mappings[*capacity], 0, sizeof(struct debug_mapping) * (new_capacity - *capacity));
    *capacity = new_capacity;
  }

  model->mappings[*count] = *mapping;
  (*count)++;

  return SUCCEEDED;
}


static void _fix_appended_section_addresses(struct section *s, int *offset, int *output_address, int *base, int *bank, int *slot) {

  struct section *s3;
  int offset_new, address_new;

  if (s->appended_to != YES)
    return;

  s3 = s->appended_to_section;
  offset_new = *offset - s->address + s3->address + s->appended_to_offset;
  address_new = *output_address - s->output_address + s3->output_address + s->appended_to_offset;
  while (s3->appended_to == YES) {
    struct section *s4;

    s4 = s3->appended_to_section;
    offset_new = offset_new - s3->address + s4->address + s3->appended_to_offset;
    address_new = address_new - s3->output_address + s4->output_address + s3->appended_to_offset;
    s3 = s4;
  }

  *offset = offset_new;
  *output_address = address_new;
  *base = s3->base;
  *bank = s3->bank;
  *slot = s3->slot;
}


static int _collect_section_mappings(struct debug_model *model, struct section *s, int *count, int *capacity) {

  struct debug_mapping mapping;
  int j, add, source_id, length, skip, offset, output_address, base, bank, slot;
  char command;

  while (s != NULL) {
    if (s->alive == YES && s->listfile_items > 0 && debug_model_is_ram_section_status(s->status) == NO) {
      add = 0;
      source_id = -1;
      for (j = 0; j < s->listfile_items; j++) {
        command = s->listfile_cmds[j];
        if (command == 'k') {
          length = s->listfile_ints[j*7 + 1];
          skip = s->listfile_ints[j*7 + 2];
          if (length > 0) {
            add += skip;
            offset = s->address + add;
            output_address = s->output_address + add;
            base = s->base;
            bank = s->bank;
            slot = s->slot;
            _fix_appended_section_addresses(s, &offset, &output_address, &base, &bank, &slot);

            memset(&mapping, 0, sizeof(mapping));
            mapping.file_id = s->file_id;
            mapping.source_id = source_id;
            mapping.section = s->id;
            mapping.output_address = output_address;
            mapping.pc_address = g_slots[slot].address + offset;
            mapping.offset = offset;
            mapping.length = length;
            mapping.bank = bank;
            mapping.base = base;
            mapping.slot = slot;
            mapping.linenumber = s->listfile_ints[j*7 + 0];
            mapping.real_linenumber = s->listfile_ints[j*7 + 4];
            mapping.source_name = get_source_file_name(s->file_id, source_id);
            if (_append_mapping(model, count, capacity, &mapping) == FAILED)
              return FAILED;
            add += length;
          }
          else
            add += skip;
        }
        else if (command == 'f')
          source_id = s->listfile_ints[j*7 + 0];
      }
    }
    s = s->next;
  }

  return SUCCEEDED;
}


static int _collect_global_mappings(struct debug_model *model, int *count, int *capacity) {

  struct debug_mapping mapping;
  struct object_file *obj;
  int j, source_id;
  char command;

  obj = g_obj_first;
  while (obj != NULL) {
    source_id = -1;
    for (j = 0; j < obj->listfile_items; j++) {
      command = obj->listfile_cmds[j];
      if (command == 'k') {
        if (obj->listfile_ints[j*10 + 1] > 0) {
          memset(&mapping, 0, sizeof(mapping));
          mapping.file_id = obj->id;
          mapping.source_id = source_id;
          mapping.section = -1;
          mapping.offset = obj->listfile_ints[j*10 + 2];
          mapping.length = obj->listfile_ints[j*10 + 1];
          mapping.base = obj->listfile_ints[j*10 + 3];
          mapping.bank = obj->listfile_ints[j*10 + 4];
          mapping.slot = obj->listfile_ints[j*10 + 5];
          mapping.output_address = g_bankaddress[mapping.bank] + mapping.offset;
          mapping.pc_address = g_slots[mapping.slot].address + mapping.offset;
          mapping.linenumber = obj->listfile_ints[j*10 + 0];
          mapping.real_linenumber = obj->listfile_ints[j*10 + 7];
          mapping.source_name = get_source_file_name(obj->id, source_id);
          if (_append_mapping(model, count, capacity, &mapping) == FAILED)
            return FAILED;
        }
      }
      else if (command == 'f')
        source_id = obj->listfile_ints[j*10 + 0];
    }
    obj = obj->next;
  }

  return SUCCEEDED;
}


static int _build_mappings(struct debug_model *model) {

  int count, capacity;

  count = 0;
  capacity = 0;
  if (_collect_section_mappings(model, g_sec_first, &count, &capacity) == FAILED)
    return FAILED;
  if (_collect_section_mappings(model, g_sec_bankhd_first, &count, &capacity) == FAILED)
    return FAILED;
  if (_collect_global_mappings(model, &count, &capacity) == FAILED)
    return FAILED;

  model->mapping_count = count;

  return SUCCEEDED;
}


struct debug_model *debug_model_build(char *outname) {

  struct debug_model *model;

  model = calloc(1, sizeof(struct debug_model));
  if (model == NULL)
    return NULL;

  model->output_name = _debug_strdup(outname);
  if (model->output_name == NULL) {
    debug_model_free(model);
    return NULL;
  }
  model->rom_size = g_romsize;
  model->rom_banks = g_rombanks;

  if (_build_sources(model) == FAILED || _build_sections(model) == FAILED ||
      _build_labels(model) == FAILED || _build_mappings(model) == FAILED) {
    debug_model_free(model);
    return NULL;
  }

  return model;
}


void debug_model_free(struct debug_model *model) {

  int i;

  if (model == NULL)
    return;

  if (model->output_name != NULL)
    free(model->output_name);
  if (model->sources != NULL) {
    for (i = 0; i < model->source_count; i++) {
      if (model->sources[i].name != NULL)
        free(model->sources[i].name);
    }
    free(model->sources);
  }
  if (model->sections != NULL)
    free(model->sections);
  if (model->labels != NULL)
    free(model->labels);
  if (model->mappings != NULL)
    free(model->mappings);

  free(model);
}

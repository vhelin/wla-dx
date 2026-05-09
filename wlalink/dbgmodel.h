#ifndef _DBGMODEL_H
#define _DBGMODEL_H

struct debug_source {
  char *name;
  int file_id;
  int source_id;
  unsigned long checksum;
};

struct debug_section {
  char name[MAX_NAME_LENGTH + 1];
  int id;
  int status;
  int file_id;
  int file_id_source;
  int address;
  int output_address;
  int pc_address;
  int size;
  int bank;
  int base;
  int slot;
  int slot_address;
  int is_ram;
  int is_bankheader;
};

struct debug_label {
  char name[MAX_NAME_LENGTH + 1];
  int status;
  int file_id;
  int file_id_source;
  int linenumber;
  int section;
  int section_status;
  int address;
  int rom_address;
  int address_in_section;
  int bank;
  int base;
  int slot;
  int is_anonymous;
};

struct debug_mapping {
  char *source_name;
  int file_id;
  int source_id;
  int section;
  int output_address;
  int pc_address;
  int offset;
  int length;
  int bank;
  int base;
  int slot;
  int linenumber;
  int real_linenumber;
};

struct debug_model {
  char *output_name;
  struct debug_source *sources;
  int source_count;
  struct debug_section *sections;
  int section_count;
  struct debug_label *labels;
  int label_count;
  struct debug_mapping *mappings;
  int mapping_count;
  int rom_size;
  int rom_banks;
};

struct debug_model *debug_model_build(char *outname);
void debug_model_free(struct debug_model *model);

int debug_model_is_ram_section_status(int status);
const char *debug_model_label_status_name(int status);
const char *debug_model_section_status_name(int status);
struct debug_section *debug_model_find_section(struct debug_model *model, int id);
struct debug_source *debug_model_find_source(struct debug_model *model, int file_id, int source_id);

#endif

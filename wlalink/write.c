
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "defines.h"
#include "memory.h"
#include "write.h"
#include "files.h"
#include "analyze.h"

#if defined(AMIGA)
#include "/crc32.h"
#include "/printf.h"
#else
#include "../crc32.h"
#include "../printf.h"
#endif


extern struct section_fix *g_sec_fix_first, *g_sec_fix_tmp;
extern struct reference *g_reference_first, *g_reference_last;
extern struct label *g_labels_first, *g_labels_last;
extern struct label **g_sorted_anonymous_labels;
extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct section *g_sec_first, *g_sec_last, *g_sec_bankhd_first, g_sec_bankhd_last;
extern struct stack *g_stacks_first, *g_stacks_last;
extern struct map_t *g_global_unique_label_map;
extern struct map_t *g_namespace_map;
extern struct slot g_slots[256];
extern struct label_sizeof *g_label_sizeofs;
extern struct pointer_array **g_section_table_table;
extern unsigned char *g_rom, *g_rom_usage;
extern unsigned char *g_file_header, *g_file_footer;
extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024], **g_ram_slots[256];
extern char g_load_address_label[MAX_NAME_LENGTH + 1];
extern char g_program_address_start_label[MAX_NAME_LENGTH + 1], g_program_address_end_label[MAX_NAME_LENGTH + 1];
extern int g_load_address, g_load_address_type, g_emptyfill;
extern int g_romsize, g_rombanks, g_banksize, g_section_overwrite;
extern int g_pc_bank, g_pc_full, g_pc_slot, g_pc_slot_max, g_snes_rom_mode;
extern int g_file_header_size, g_file_footer_size, *g_bankaddress, *g_banksizes;
extern int g_memory_file_id, g_memory_file_id_source, g_memory_line_number, g_output_mode;
extern int g_program_start, g_program_end, g_snes_mode, g_smc_status;
extern int g_snes_sramsize, g_num_sorted_anonymous_labels, g_sort_sections;
extern int g_output_type, g_program_address_start, g_program_address_end, g_program_address_start_type, g_program_address_end_type;
extern int g_section_table_table_max, g_section_write_order[SECTION_TYPES_COUNT-2], g_ramsection_write_order[RAMSECTION_TYPES_COUNT];
extern int g_use_priority_only_writing_sections, g_use_priority_only_writing_ramsections;
extern int g_allow_duplicate_labels_and_definitions;

static int s_current_stack_calculation_addr = 0;


static int _sections_sort(const void *a, const void *b) {

  if ((*((struct section **)a))->priority < (*((struct section **)b))->priority)
    return 1;
  else if ((*((struct section **)a))->priority > (*((struct section **)b))->priority)
    return -1;

  if ((*((struct section **)a))->size < (*((struct section **)b))->size)
    return 1;

  return -1;
}


int strcaselesscmp(char *s1, const char *s2) {

  if (s1 == NULL || s2 == NULL)
    return 0;

  while (*s1 != 0) {
    if (toupper((int)*s1) != toupper((int)*s2))
      return 1;
    s1++;
    s2++;
  }

  if (*s2 != 0)
    return 1;

  return 0;
}


int _cbm_write_prg_header(FILE *f) {

  int address = 0;
  
  if (f == NULL)
    return FAILED;

  if (g_load_address_type == LOAD_ADDRESS_TYPE_VALUE) {
    address = g_load_address;
    fprintf(stderr, "Using the address $%x as the load address for the PRG.\n", address & 0xFFFF);
  }
  else if (g_load_address_type == LOAD_ADDRESS_TYPE_LABEL) {
    /* find the address of the label */
    struct label *l;
    
    find_label(g_load_address_label, NULL, &l);

    if (l == NULL) {
      fprintf(stderr, "_CBM_WRITE_PRG_HEADER: Cannot find label \"%s\".\n", g_load_address_label);
      return FAILED;
    }

    if (l->status != LABEL_STATUS_LABEL || (l->section_struct != NULL && (l->section_struct->status == SECTION_STATUS_RAM_FREE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_FORCE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_SEMIFREE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_SEMISUBFREE ||
                                                                          l->section_struct->alive == NO))) {
      fprintf(stderr, "_CBM_WRITE_PRG_HEADER: \"%s\" cannot be used as the load address.\n", g_load_address_label);
      return FAILED;
    }

    address = (int)l->address;

    fprintf(stderr, "Using the address $%x (of label \"%s\") as the load address for the PRG.\n", address & 0xFFFF, l->name);
  }
  else if (g_load_address_type == LOAD_ADDRESS_TYPE_UNDEFINED) {
    /* find a suitable load address, i.e., the label with the smallest address value */
    struct label *l = g_labels_first, *label = NULL;
    int address2 = 0xFFFFFF;
    
    while (l != NULL) {
      if (l->status != LABEL_STATUS_LABEL || (l->section_struct != NULL && (l->section_struct->status == SECTION_STATUS_RAM_FREE ||
                                                                            l->section_struct->status == SECTION_STATUS_RAM_FORCE ||
                                                                            l->section_struct->status == SECTION_STATUS_RAM_SEMIFREE ||
                                                                            l->section_struct->status == SECTION_STATUS_RAM_SEMISUBFREE ||
                                                                            l->section_struct->alive == NO))) {
        l = l->next;
        continue;
      }
      /* skip RAM_USAGE_SLOT_* labels that we've generated ourselves */
      if (l->file_id < 0) {
        l = l->next;
        continue;
      }
      
      if ((int)l->address < address2 || label == NULL) {
        address2 = (int)l->address;
        label = l;
      }

      l = l->next;
    }

    if (label == NULL) {
      fprintf(stderr, "_CBM_WRITE_PRG_HEADER: Cannot find a suitable label to be used as the load address for the PRG.\n");
      return FAILED;
    }

    address = address2;

    fprintf(stderr, "Using the address $%x (of label \"%s\") as the load address for the PRG.\n", address & 0xFFFF, label->name);
  }

  fprintf(f, "%c", address & 0xFF);
  fprintf(f, "%c", (address >> 8) & 0xFF);

  return SUCCEEDED;
}


int _smc_create_and_write(FILE *f) {

  int i;

  if (f == NULL)
    return FAILED;

  if (g_output_mode != OUTPUT_ROM)
    return FAILED;

  i = g_romsize/(8*1024);

  /* low byte of 8KB page count */
  fprintf(f, "%c", i & 0xFF);
  /* high byte of 8KB page count */
  fprintf(f, "%c", (i>>8) & 0xFF);

  /* emulation mode select (?) */
  i = 0;
  if (g_snes_rom_mode == SNES_ROM_MODE_HIROM || g_snes_rom_mode == SNES_ROM_MODE_EXHIROM)
    i |= (1<<5) | (1<<4);
  i |= (g_snes_sramsize ^ 3) << 2;

  fprintf(f, "%c", i);

  /* bytes 3 to 7 are reserved, should be zero */
  fprintf(f, "%c" ,0);
  fprintf(f, "%c" ,0);
  fprintf(f, "%c" ,0);
  fprintf(f, "%c" ,0);
  fprintf(f, "%c" ,0);

  /* bytes 8, 9 = ID Code = 0xAA, 0xBB */
  fprintf(f, "%c", 0xAA);
  fprintf(f, "%c", 0xBB);

  /* 0x04 = SWC & SMC Game */
  fprintf(f, "%c", 0x04);

  /* the rest of the header is zeroes */
  for (i = 0; i < 512-11; i++)
    fprintf(f, "%c", 0);

  return SUCCEEDED;
}


struct section *find_section(int id) {

  /* NOTE! this function doesn't return bankheader sections */
  return (struct section *)g_section_table_table[id >> 16]->ptr[id & 0xffff];
}


int sort_sections(void) {

  struct section *s, **sa;
  int i, sn;
  
  /* sort the sections by priority first and then by size, biggest first */
  if (g_sort_sections == NO)
    return SUCCEEDED;

  /* count the sections */
  sn = 0;
  s = g_sec_first;
  while (s != NULL) {
    sn++;
    s = s->next;
  }

  if (sn == 0)
    return SUCCEEDED;

  sa = calloc(sizeof(struct section *) * sn, 1);
  if (sa == NULL) {
    fprintf(stderr, "SORT_SECTIONS: Out of memory error.\n");
    return FAILED;
  }

  /* insert the sections into an array for sorting */
  i = 0;
  s = g_sec_first;
  while (s != NULL) {
    sa[i++] = s;
    s = s->next;
  }

  /* sort the sections by priority first and then by size, biggest first */
  qsort(sa, sn, sizeof(struct section *), _sections_sort);

  /* rebuild the section linked list */
  g_sec_first = sa[0];
  for (i = 0; i < sn; i++) {
    if (i == sn-1)
      sa[i]->next = NULL;
    else
      sa[i]->next = sa[i+1];

    if (i-1 >= 0)
      sa[i]->prev = sa[i-1];
    else
      sa[i]->prev = NULL;
  }
  g_sec_last = sa[sn-1];

  free(sa);

  return SUCCEEDED;
}


static int _after_section_override_ram(struct section *s, char **ram_slots[256], int *address) {

  int i, q, a;
  char *c;
  
  if (s->after->placed == NO) {
    fprintf(stderr, "INSERT_SECTIONS: Trying to insert RAMSECTION \"%s\" after \"%s\", but its address is unknown. Internal error. Please submit a bug report!\n", s->name, s->after->name);
    return FAILED;
  }

  a = s->after->address + s->after->size + s->offset;
  
  s->address = a;
  s->output_address = a;

  *address = a;

  /* do we have room for this section? */
  c = ram_slots[s->bank][s->slot];
  i = g_slots[s->slot].size;
  for (q = 0; a + q < i && q < s->size; q++) {
    if (c[a + q] != 0) {
      fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in RAM bank %d.\n", s->name, s->size, s->bank);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


static int _mark_ram_section_area(struct section *s, int address, int max_address, char *c) {

  int i;
        
  /* mark as used */
  for (i = 0; i < s->size && address < max_address; i++, address++)
    c[address] = 1;

  if (i < s->size) {
    /* address overflow! */
    fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in RAM bank %d.\n", s->name, s->size, s->bank);
    return FAILED;        
  }

  s->placed = YES;

  return SUCCEEDED;
}


static int _insert_rom_after_section(struct section *s) {

  int address, end_address;
  
  /* NOTE! SUPERFREE sections can have a new bank at this point */
  s->bank = s->after->bank;

  s->address = s->after->address + s->after->size + s->offset;

  g_memory_file_id = s->file_id;
  g_banksize = g_banksizes[s->bank];
  g_pc_bank = s->address;
  g_pc_slot = g_slots[s->slot].address + g_pc_bank;
  g_pc_full = g_pc_bank + g_bankaddress[s->bank];
  g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
  address = g_pc_full;
  end_address = address + s->size;
  s->output_address = address;
  g_section_overwrite = OFF;

  if (end_address > g_romsize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
    return FAILED;
  }
  if (s->address + s->size > g_banksize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
    return FAILED;
  }

  for (; address < end_address; address++) {
    if (g_rom_usage[address] != 0 && g_rom[address] != s->data[address - g_pc_full])
      break;
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

  if (address == end_address) {
    int i;
    
    for (i = 0; i < s->size; i++) {
      if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
        return FAILED;
    }
  }
  else {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
    return FAILED;
  }

  s->placed = YES;

  return SUCCEEDED;
}


static int _does_window_allow_section_placement(struct section *s, int address) {
  
  if (s->window_start >= 0 && s->window_end >= 0) {
    int end = address + s->size;

    if (address < s->window_start || address > s->window_end)
      return NO;
    if (end < s->window_start || end > s->window_end)
      return NO;
  }

  if (s->bitwindow != 0) {
    int i, mask_bottom, mask_top, upper_bits;

    mask_bottom = (1 << s->bitwindow) - 1;
    mask_top = ~mask_bottom;

    upper_bits = address & mask_top;
    
    for (i = 0; i < s->size; i++) {
      if (((address + i) & mask_top) != upper_bits)
        return NO;
    }
  }

  return YES;
}


static int _unroll_banks(struct section *s, int banks[1024], int *banks_max) {

  int i = 0, max = 0, bank, bank2, digits;
  char c;
  
  while (s->banks[i] != 0) {
    bank = 0;
    digits = 0;
    
    /* parse the 1st number */
    while (1) {
      c = s->banks[i];
      if (c >= '0' && c <= '9') {
        bank = bank * 10 + c - '0';
        digits++;
        i++;
      }
      else
        break;
    }

    if (digits == 0) {
      fprintf(stderr, "%s: %s: _UNROLL_BANKS: Section \"%s\" has malformed BANKS list (%s).\n", get_file_name(s->file_id),
              get_source_file_name(s->file_id, s->file_id_source), s->name, s->banks);
      return FAILED;
    }

    /* single bank number? */
    c = s->banks[i];
    if (c == 0 || c == '/') {
      if (bank >= 1024) {
        fprintf(stderr, "%s: %s: _UNROLL_BANKS: Bank %d in Section \"%s\"'s BANKS list is out of range [0, 1023]. Please submit a bug report if this should work!\n", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), bank, s->name);
        return FAILED;
      }

      banks[max++] = bank;

      if (c == '/')
        i++;
      
      continue;
    }

    /* range? */
    if (c != '-') {
      fprintf(stderr, "%s: %s: _UNROLL_BANKS: Section \"%s\" has malformed BANKS list (%s).\n", get_file_name(s->file_id),
              get_source_file_name(s->file_id, s->file_id_source), s->name, s->banks);
      return FAILED;
    }

    i++;

    bank2 = 0;
    digits = 0;
    
    /* parse the 1st number */
    while (1) {
      c = s->banks[i];
      if (c >= '0' && c <= '9') {
        bank2 = bank2 * 10 + c - '0';
        digits++;
        i++;
      }
      else
        break;
    }

    if (digits == 0 || (c != 0 && c != '/')) {
      fprintf(stderr, "%s: %s: _UNROLL_BANKS: Section \"%s\" has malformed BANKS list (%s).\n", get_file_name(s->file_id),
              get_source_file_name(s->file_id, s->file_id_source), s->name, s->banks);
      return FAILED;
    }

    while (1) {
      if (bank >= 1024) {
        fprintf(stderr, "%s: %s: _UNROLL_BANKS: Bank %d in Section \"%s\"'s BANKS list is out of range [0, 1023]. Please submit a bug report if this should work!\n", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), bank, s->name);
        return FAILED;
      }

      banks[max++] = bank;

      if (bank < bank2)
        bank++;
      else if (bank > bank2)
        bank--;
      else
        break;
    }

    if (c == '/')
      i++;
  }

  if (max <= 0) {
    fprintf(stderr, "%s: %s: _UNROLL_BANKS: No banks in section \"%s\"'s BANKS list (%s).\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->banks);
    return FAILED;
  }
  
  *banks_max = max;

  return SUCCEEDED;
}


static int _try_to_insert_semisuperfree_section(struct section *s, int bank) {

  int i, d, f, x;
  
  s->bank = bank;
  
  g_pc_bank = s->address;
  d = g_bankaddress[s->bank];

  /* align the starting address */
  f = (g_pc_bank + d) % s->alignment;
  if (f > 0)
    g_pc_bank += s->alignment - f;

  i = FAILED;
  while (i == FAILED) {
    f = g_pc_bank;
    for (x = 0; g_pc_bank + s->offset < g_banksizes[s->bank] && g_rom_usage[g_pc_bank + s->offset + d] == 0 && x < s->size; g_pc_bank++, x++)
      ;
    if (x == s->size) {
      if (_does_window_allow_section_placement(s, g_slots[s->slot].address + f + s->offset) == YES)
        break;
      else
        g_pc_bank = f + 1;
    }
    if (g_pc_bank + s->offset >= g_banksizes[s->bank])
      return FAILED;

    /* find the next starting address */
    f = (g_pc_bank + d) % s->alignment;
    if (f > 0)
      g_pc_bank += s->alignment - f;
    for (; g_pc_bank + s->offset < g_banksizes[s->bank] && g_rom_usage[g_pc_bank + s->offset + d] != 0; g_pc_bank += s->alignment)
      ;
  }

  g_memory_file_id = s->file_id;
  g_banksize = g_banksizes[s->bank];
  g_pc_bank = f + s->offset;
  g_pc_slot = g_slots[s->slot].address + g_pc_bank;
  g_pc_full = g_pc_bank + g_bankaddress[s->bank];
  g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
  g_section_overwrite = OFF;

  s->address = g_pc_bank;
  s->output_address = g_pc_full;
  s->placed = YES;
      
  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

  for (i = 0; i < s->size; i++) {
    if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


static int _write_section_absolute(struct section *s) {

  int d, i;

  if (s->after != NULL) {
    /* AFTER section override! */
    if (s->after->placed == NO) {
      fprintf(stderr, "INSERT_SECTIONS: Trying to insert SECTION \"%s\" after \"%s\", but its address is unknown. Internal error. Please submit a bug report!\n", s->name, s->after->name);
      return FAILED;
    }

    s->address = s->after->address + s->after->size + s->offset;
  }

  d = s->address;
  s->output_address = d;
  g_section_overwrite = ON;
  s->placed = YES;

  if (d + s->size > g_romsize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
    return FAILED;
  }
      
  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

  for (i = 0; i < s->size; i++) {
    if (mem_insert(d + i, s->data[i]) == FAILED)
      return FAILED;
  }
      
  return SUCCEEDED;
}


static int _write_sections_absolute(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_ABSOLUTE) {
      if (_write_section_absolute(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_section_force(struct section *s) {

  int d, i;

  if (s->after != NULL) {
    /* AFTER section override! */
    if (s->after->placed == NO) {
      fprintf(stderr, "INSERT_SECTIONS: Trying to insert SECTION \"%s\" after \"%s\", but its address is unknown. Internal error. Please submit a bug report!\n", s->name, s->after->name);
      return FAILED;
    }

    s->address = s->after->address + s->after->size + s->offset;
  }
      
  g_memory_file_id = s->file_id;
  g_banksize = g_banksizes[s->bank];
  g_pc_bank = s->address;
  g_pc_slot = g_slots[s->slot].address + g_pc_bank;
  g_pc_full = g_pc_bank + g_bankaddress[s->bank];
  g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
  d = g_pc_full;
  i = d + s->size;
  s->output_address = d;
  g_section_overwrite = OFF;
  s->placed = YES;
  if (i > g_romsize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
    return FAILED;
  }
  if (s->address + s->size > g_banksize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
    return FAILED;
  }
  for (; d < i; d++) {
    if (g_rom_usage[d] != 0 && g_rom[d] != s->data[d - g_pc_full])
      break;
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

  if (d == i) {
    for (i = 0; i < s->size; i++) {
      if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
        return FAILED;
    }
  }
  else {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
    return FAILED;
  }

  return SUCCEEDED;
}


static int _write_sections_force(void) {

  struct section *s;
  
  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_FORCE) {
      if (_write_section_force(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_section_semisuperfree(struct section *s) {

  if (s->after != NULL) {
    if (_insert_rom_after_section(s) == FAILED)
      return FAILED;
  }
  else {
    int banks[1024], bank = 0, banks_max = 0;

    if (_unroll_banks(s, banks, &banks_max) == FAILED)
      return FAILED;

    while (1) {
      if (banks[bank] >= g_rombanks) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: Bank %d for section \"%s\" is out of range [0, %d].\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), banks[bank], s->name, g_rombanks);
        return FAILED;
      }
          
      if (_try_to_insert_semisuperfree_section(s, banks[bank]) == SUCCEEDED)
        break;

      bank++;
      if (bank == banks_max) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
        return FAILED;
      }
    }
  }
      
  return SUCCEEDED;
}


static int _write_sections_semisuperfree(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_SEMISUPERFREE) {
      if (_write_section_semisuperfree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }
  
  return SUCCEEDED;
}


static int _write_section_semisubfree(struct section *s) {

  int d, i, f, x;

  if (s->after != NULL) {
    if (_insert_rom_after_section(s) == FAILED)
      return FAILED;
  }
  else {
    g_pc_bank = 0;
    d = g_bankaddress[s->bank];
        
    /* align the starting address */
    f = (g_pc_bank + d) % s->alignment;
    if (f > 0)
      g_pc_bank += s->alignment - f;

    i = FAILED;
    while (i == FAILED) {
      f = g_pc_bank;
      for (x = 0; g_pc_bank + s->offset < s->address && g_rom_usage[g_pc_bank + s->offset + d] == 0 && x < s->size; g_pc_bank++, x++)
        ;
      if (x == s->size) {
        if (_does_window_allow_section_placement(s, g_slots[s->slot].address + f + s->offset) == YES)
          break;
        else
          g_pc_bank = f + 1;
      }
      if (g_pc_bank + s->offset >= s->address) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
        return FAILED;
      }

      /* find the next starting address */
      f = (g_pc_bank + d) % s->alignment;
      if (f > 0)
        g_pc_bank += s->alignment - f;
      for (; g_pc_bank + s->offset < s->address && g_rom_usage[g_pc_bank + s->offset + d] != 0; g_pc_bank += s->alignment)
        ;
    }

    g_memory_file_id = s->file_id;
    g_banksize = g_banksizes[s->bank];
    g_pc_bank = f + s->offset;
    g_pc_slot = g_slots[s->slot].address + g_pc_bank;
    g_pc_full = g_pc_bank + g_bankaddress[s->bank];
    g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
    g_section_overwrite = OFF;
          
    s->address = g_pc_bank;
    s->output_address = g_pc_full;
    s->placed = YES;
      
    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

    for (i = 0; i < s->size; i++) {
      if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
        return FAILED;
    }
  }
      
  return SUCCEEDED;
}


static int _write_sections_semisubfree(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_SEMISUBFREE) {
      if (_write_section_semisubfree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_section_free_and_semifree(struct section *s) {

  int d, i, f, x;

  if (s->after != NULL) {
    if (_insert_rom_after_section(s) == FAILED)
      return FAILED;
  }
  else {
    g_pc_bank = s->address;
    d = g_bankaddress[s->bank];

    /* align the starting address */
    f = (g_pc_bank + d) % s->alignment;
    if (f > 0)
      g_pc_bank += s->alignment - f;

    i = FAILED;
    while (i == FAILED) {
      f = g_pc_bank;
      for (x = 0; g_pc_bank + s->offset < g_banksizes[s->bank] && g_rom_usage[g_pc_bank + s->offset + d] == 0 && x < s->size; g_pc_bank++, x++)
        ;
      if (x == s->size) {
        if (_does_window_allow_section_placement(s, g_slots[s->slot].address + f + s->offset) == YES)
          break;
        else
          g_pc_bank = f + 1;
      }
      if (g_pc_bank + s->offset >= g_banksizes[s->bank]) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
        return FAILED;
      }

      /* find the next starting address */
      f = (g_pc_bank + d) % s->alignment;
      if (f > 0)
        g_pc_bank += s->alignment - f;
      for (; g_pc_bank + s->offset < g_banksizes[s->bank] && g_rom_usage[g_pc_bank + s->offset + d] != 0; g_pc_bank += s->alignment)
        ;
    }

    g_memory_file_id = s->file_id;
    g_banksize = g_banksizes[s->bank];
    g_pc_bank = f + s->offset;
    g_pc_slot = g_slots[s->slot].address + g_pc_bank;
    g_pc_full = g_pc_bank + g_bankaddress[s->bank];
    g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
    g_section_overwrite = OFF;

    s->address = g_pc_bank;
    s->output_address = g_pc_full;
    s->placed = YES;
      
    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

    for (i = 0; i < s->size; i++) {
      if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
        return FAILED;
    }
  }
  
  return SUCCEEDED;
}


static int _write_sections_free_and_semifree(int status) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == status) {
      if (_write_section_free_and_semifree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_section_superfree(struct section *s) {

  int d, i, f, q, x;

  if (s->after != NULL) {
    if (_insert_rom_after_section(s) == FAILED)
      return FAILED;
  }
  else {
    /* go through all the banks */
    i = FAILED;
    f = 0;

    for (q = 0; i == FAILED && q < g_rombanks; q++) {
      g_pc_bank = 0;
      d = g_bankaddress[q];

      /* align the starting address */
      f = (g_pc_bank + d) % s->alignment;
      if (f > 0)
        g_pc_bank += s->alignment - f;

      /* if the slotsize and banksize differ -> try the next bank */
      if (g_banksizes[q] != g_slots[s->slot].size)
        continue;

      while (i == FAILED) {
        f = g_pc_bank;
        for (x = 0; g_pc_bank + s->offset < g_banksizes[q] && g_rom_usage[g_pc_bank + s->offset + d] == 0 && x < s->size; g_pc_bank++, x++)
          ;
        if (x == s->size) {
          if (_does_window_allow_section_placement(s, g_slots[s->slot].address + f + s->offset) == YES) {
            i = SUCCEEDED;
            break;
          }
          else
            g_pc_bank = f + 1;
        }
        if (g_pc_bank + s->offset >= g_banksizes[q])
          break;

        /* find the next starting address */
        f = (g_pc_bank + d) % s->alignment;
        if (f > 0)
          g_pc_bank += s->alignment - f;
        for (; g_pc_bank + s->offset < g_banksizes[s->bank] && g_rom_usage[g_pc_bank + s->offset + d] != 0; g_pc_bank += s->alignment)
          ;
      }
    }

    if (i == SUCCEEDED) {
      s->bank = q-1;
      g_memory_file_id = s->file_id;
      g_banksize = g_banksizes[s->bank];
      g_pc_bank = f + s->offset;
      g_pc_slot = g_pc_bank;
      g_pc_full = g_pc_bank + g_bankaddress[s->bank];
      g_pc_slot_max = g_slots[s->slot].size;
      g_section_overwrite = OFF;

      s->address = g_pc_bank;
      s->output_address = g_pc_full;
      s->placed = YES;
      
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++)
        if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
          return FAILED;
    }
    else {
      fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
              get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
      return FAILED;
    }
  }

  return SUCCEEDED;
}


static int _write_sections_superfree(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_SUPERFREE) {
      if (_write_section_superfree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_section_overwrite(struct section *s) {

  int i;

  if (s->after != NULL) {
    /* AFTER section override! */
    if (s->after->placed == NO) {
      fprintf(stderr, "INSERT_SECTIONS: Trying to insert SECTION \"%s\" after \"%s\", but its address is unknown. Internal error. Please submit a bug report!\n", s->name, s->after->name);
      return FAILED;
    }
    
    s->address = s->after->address + s->after->size + s->offset;
  }

  g_memory_file_id = s->file_id;
  g_banksize = g_banksizes[s->bank];
  g_pc_bank = s->address;
  g_pc_slot = g_slots[s->slot].address + g_pc_bank;
  g_pc_full = g_pc_bank + g_bankaddress[s->bank];
  g_pc_slot_max = g_slots[s->slot].address + g_slots[s->slot].size;
  g_section_overwrite = ON;

  s->output_address = g_pc_full;
  s->placed = YES;
      
  if (g_pc_full + s->size > g_romsize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
    return FAILED;
  }
  if (s->address + s->size > g_banksize) {
    fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
            get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
    return FAILED;
  }
  
  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

  for (i = 0; i < s->size; i++) {
    if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


static int _write_sections_overwrite(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_OVERWRITE) {
      if (_write_section_overwrite(s) == FAILED)
        return FAILED;
    }
    
    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_ramsection_force(struct section *s) {

  int slot_address, slot_size, overflow, address, q;
  char *c;

  slot_address = g_slots[s->slot].address;

  /* align the starting address */
  overflow = (slot_address + s->address) % s->alignment;
  address = s->address;
  address += overflow;
  address += s->offset;

  c = g_ram_slots[s->bank][s->slot];
  slot_size = g_slots[s->slot].size;
  if (s->after != NULL) {
    /* AFTER section override! */
    if (_after_section_override_ram(s, g_ram_slots, &address) == FAILED)
      return FAILED;
  }
  else {
    for (q = 0; address + q < slot_size && q < s->size; q++) {
      if (c[address + q] != 0) {
        fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in RAM bank %d.\n", s->name, s->size, s->bank);
        return FAILED;
      }
    }
      
    s->address = address;
    s->output_address = address;
  }

  if (_mark_ram_section_area(s, address, slot_size, c) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _write_ramsections_force(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_RAM_FORCE) {
      if (_write_ramsection_force(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


static int _write_ramsection_semisubfree(struct section *s) {

  int slot_address, max_address, overflow, address, offset, t, q;
  char *c;

  slot_address = g_slots[s->slot].address;

  /* align the starting address */
  overflow = slot_address % s->alignment;
  address = 0;
  offset = s->offset;
  address += overflow;

  c = g_ram_slots[s->bank][s->slot];
  max_address = s->address;
  if (s->after != NULL) {
    /* AFTER section override! */
    if (_after_section_override_ram(s, g_ram_slots, &address) == FAILED)
      return FAILED;
  }
  else {
    t = 0;
    for (; address < max_address; address += s->alignment) {
      for (q = 0; address + offset + q < max_address && q < s->size; q++) {
        if (c[address + offset + q] != 0)
          break;
      }
      if (q == s->size) {
        if (_does_window_allow_section_placement(s, slot_address + address + offset) == YES) {
          t = 1;
          break;
        }
      }
    }

    if (t == 0) {
      fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in RAM bank %d.\n", s->name, s->size, s->bank);
      return FAILED;
    }

    address += offset;
    s->address = address;
    s->output_address = address;
  }

  if (_mark_ram_section_area(s, address, max_address, c) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _write_ramsections_semisubfree(void) {

  struct section *s;
  
  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
      if (_write_ramsection_semisubfree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }
  
  return SUCCEEDED;
}


static int _write_ramsection_free_and_semifree(struct section *s) {

  int slot_address, slot_size, overflow, address, offset, t, q;
  char *c;

  slot_address = g_slots[s->slot].address;

  /* align the starting address */
  overflow = (slot_address + s->address) % s->alignment;
  address = s->address;
  offset = s->offset;
  address += overflow;

  c = g_ram_slots[s->bank][s->slot];
  slot_size = g_slots[s->slot].size;
  if (s->after != NULL) {
    /* AFTER section override! */
    if (_after_section_override_ram(s, g_ram_slots, &address) == FAILED)
      return FAILED;
  }
  else {
    t = 0;
    for (; address < slot_size; address += s->alignment) {
      for (q = 0; address + offset + q < slot_size && q < s->size; q++) {
        if (c[address + offset + q] != 0)
          break;
      }
      if (q == s->size) {
        if (_does_window_allow_section_placement(s, slot_address + address + offset) == YES) {
          t = 1;
          break;
        }
      }
    }

    if (t == 0) {
      fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in RAM bank %d.\n", s->name, s->size, s->bank);
      return FAILED;
    }

    address += offset;
    s->address = address;
    s->output_address = address;
  }

  if (_mark_ram_section_area(s, address, slot_size, c) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


static int _write_ramsections_free_and_semifree(int status) {

  struct section *s;
  
  s = g_sec_first;
  while (s != NULL) {
    if (s->alive == YES && s->status == status) {
      if (_write_ramsection_free_and_semifree(s) == FAILED)
        return FAILED;
    }

    s = s->next;
  }

  return SUCCEEDED;
}


int insert_sections(void) {

  struct section *s;
  int i;

  /* find all touched slots */
  s = g_sec_first;
  while (s != NULL) {
    if (s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE || s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
      if (g_ram_slots[s->bank] == NULL) {
        g_ram_slots[s->bank] = calloc(sizeof(char *) * 256, 1);
        if (g_ram_slots[s->bank] == NULL) {
          fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
          return FAILED;
        }
        for (i = 0; i < 256; i++)
          g_ram_slots[s->bank][i] = NULL;
      }
      if (g_ram_slots[s->bank][s->slot] == NULL) {
        g_ram_slots[s->bank][s->slot] = calloc(g_slots[s->slot].size, 1);
        if (g_ram_slots[s->bank][s->slot] == NULL) {
          fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
          return FAILED;
        }
        memset(g_ram_slots[s->bank][s->slot], 0, g_slots[s->slot].size);
      }
    }

    s = s->next;
  }

  /*******************************************************************************************/
  /* RAM sections */
  /*******************************************************************************************/

  if (g_use_priority_only_writing_ramsections == YES) {
    s = g_sec_first;
    while (s != NULL) {
      if (s->alive == YES) {
        if (s->status == SECTION_STATUS_RAM_FREE) {
          if (_write_ramsection_free_and_semifree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_RAM_SEMIFREE) {
          if (_write_ramsection_free_and_semifree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_RAM_FORCE) {
          if (_write_ramsection_force(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
          if (_write_ramsection_semisubfree(s) == FAILED)
            return FAILED;
        }
      }
      
      s = s->next;
    }
  }
  else {
    for (i = 0; i < RAMSECTION_TYPES_COUNT; i++) {
      int status = g_ramsection_write_order[i];

      if (status == SECTION_STATUS_RAM_FREE) {
        if (_write_ramsections_free_and_semifree(SECTION_STATUS_RAM_FREE) == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_RAM_FORCE) {
        if (_write_ramsections_force() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_RAM_SEMISUBFREE) {
        if (_write_ramsections_semisubfree() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_RAM_SEMIFREE) {
        if (_write_ramsections_free_and_semifree(SECTION_STATUS_RAM_SEMIFREE) == FAILED)
          return FAILED;
      }
      else {
        fprintf(stderr, "INSERT_SECTIONS: Unhandled .RAMSECTION type %d. Please submit a bug report!\n", status);
        return FAILED;
      }
    }
  }

  /*******************************************************************************************/
  /* ROM sections */
  /*******************************************************************************************/

  if (_write_sections_absolute() == FAILED)
    return FAILED;

  /* write user definable sections */

  if (g_use_priority_only_writing_sections == YES) {
    s = g_sec_first;
    while (s != NULL) {
      if (s->alive == YES) {
        if (s->status == SECTION_STATUS_FORCE) {
          if (_write_section_force(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_SEMISUPERFREE) {
          if (_write_section_semisuperfree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_SEMISUBFREE) {
          if (_write_section_semisubfree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_SEMIFREE) {
          if (_write_section_free_and_semifree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_FREE) {
          if (_write_section_free_and_semifree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_SUPERFREE) {
          if (_write_section_superfree(s) == FAILED)
            return FAILED;
        }
        else if (s->status == SECTION_STATUS_OVERWRITE) {
          if (_write_section_overwrite(s) == FAILED)
            return FAILED;
        }
      }
      
      s = s->next;
    }
  }
  else {
    for (i = 0; i < SECTION_TYPES_COUNT-2; i++) {
      int status = g_section_write_order[i];

      if (status == SECTION_STATUS_FORCE) {
        if (_write_sections_force() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_SEMISUPERFREE) {
        if (_write_sections_semisuperfree() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_SEMISUBFREE) {
        if (_write_sections_semisubfree() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_SEMIFREE) {
        if (_write_sections_free_and_semifree(SECTION_STATUS_SEMIFREE) == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_FREE) {
        if (_write_sections_free_and_semifree(SECTION_STATUS_FREE) == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_SUPERFREE) {
        if (_write_sections_superfree() == FAILED)
          return FAILED;
      }
      else if (status == SECTION_STATUS_OVERWRITE) {
        if (_write_sections_overwrite() == FAILED)
          return FAILED;
      }
      else {
        fprintf(stderr, "INSERT_SECTIONS: Unhandled .SECTION type %d. Please submit a bug report!\n", status);
        return FAILED;
      }
    }
  }
  
  return SUCCEEDED;
}


struct stack *find_stack(int id, int file_id) {

  static struct object_file *file = NULL;

  if (file == NULL || file->id != file_id)
    file = get_file(file_id);
  
  return file->stacks[id];
}


/* transform computation stack definitions to ordinary definitions */
int transform_stack_definitions(void) {

  struct label *l;
  struct stack *s;

  l = g_labels_first;
  while (l != NULL) {
    if (l->status == LABEL_STATUS_STACK) {
      /* DEBUG
         printf("--------------------------------------\n");
         printf("name: \"%s\"\n", l->name);
         printf("sect: \"%d\"\n", l->section);
         printf("slot: \"%d\"\n", l->slot);
         printf("status: \"%d\"\n", l->status);
         printf("file_id: \"%d\"\n", l->file_id);
         printf("value: \"%d\"\n", l->address);
      */

      /* find the stack associated with the definition */
      s = find_stack((int)l->address, l->file_id);

      /* did we find it? */
      if (s == NULL) {
        fprintf(stderr, "TRANSFORM_STACK_DEFINITIONS: No computation stack associated with computation definition label \"%s\". This is a fatal internal error. Please send the WLA DX author a bug report.\n", l->name);
        return FAILED;
      }
      /* is it ok? */
      if (s->computed == NO) {
        fprintf(stderr, "TRANSFORM_STACK_DEFINITIONS: The computation of definition \"%s\" hasn't been solved. This is a fatal internal error. Please send the WLA DX author a bug report.\n", l->name);
        return FAILED;
      }

      /* do the transformation */
      l->status = LABEL_STATUS_DEFINE;
      l->address = s->result_ram;
      l->rom_address = s->result_rom;
    }

    l = l->next;
  }

  return SUCCEEDED;
}


static int _try_put_label(map_t map, struct label *l) {

  int err;

  if (hashmap_get(map, l->name, NULL) == MAP_OK) {
    if (l->status == LABEL_STATUS_DEFINE)
      fprintf(stderr, "%s: _TRY_PUT_LABEL: Definition \"%s\" was defined more than once.\n", get_file_name(l->file_id), l->name);
    else
      fprintf(stderr, "%s: %s:%d: _TRY_PUT_LABEL: Label \"%s\" was defined more than once.\n", get_file_name(l->file_id),
              get_source_file_name(l->file_id, l->file_id_source), l->linenumber, l->name);

    if (g_allow_duplicate_labels_and_definitions == NO)
      return FAILED;
  }
  
  if ((err = hashmap_put(map, l->name, l)) != MAP_OK) {
    fprintf(stderr, "_TRY_PUT_LABEL: Hashmap error %d. Please send a bug report!\n", err);
    return FAILED;
  }

  return SUCCEEDED;
}


/* check that all RAM sections inside libraries are given a bank and a slot */
int check_ramsections(void) {

  struct section *s;

  s = g_sec_first;
  while (s != NULL) {
    if (s->bank < 0 && s->slot < 0) {
      fprintf(stderr, "%s: %s: CHECK_RAMSECTIONS: RAM section \"%s\" has no BANK/SLOT. Give them in the linkfile under [ramsections].\n", get_file_name(s->file_id),
              get_source_file_name(s->file_id, s->file_id_source), s->name);
      return FAILED;
    }
    s = s->next;
  }

  return SUCCEEDED;
}


/* fix the slot, bank, org/orga, etc. of sections inside libraries, as given in the linkfile */
int fix_all_sections(void) {

  struct section *s;
  
  g_sec_fix_tmp = g_sec_fix_first;
  while (g_sec_fix_tmp != NULL) {
    /* find the section, and fix bank, slot, org/orga, etc... */
    char c1 = g_sec_fix_tmp->name[0];
    
    s = g_sec_first;
    while (s != NULL) {
      if (c1 == s->name[0] && strcmp(s->name, g_sec_fix_tmp->name) == 0) {
        s->bank = g_sec_fix_tmp->bank;

        if (g_sec_fix_tmp->slot < 0) {
          if (get_slot_by_its_name(g_sec_fix_tmp->slot_name, &(s->slot)) == FAILED)
            return FAILED;
        }
        else {
          if (get_slot_by_a_value(g_sec_fix_tmp->slot, &(s->slot)) == FAILED)
            return FAILED;
        }

        if (g_sec_fix_tmp->banks[0] != 0)
          strcpy(s->banks, g_sec_fix_tmp->banks);
        
        if (g_sec_fix_tmp->status >= 0)
          s->status = g_sec_fix_tmp->status;

        if (g_sec_fix_tmp->priority_defined == YES)
          s->priority = g_sec_fix_tmp->priority;

        if (g_sec_fix_tmp->keep == YES)
          s->keep = YES;

        if (g_sec_fix_tmp->alignment >= 0)
          s->alignment = g_sec_fix_tmp->alignment;

        if (g_sec_fix_tmp->offset >= 0)
          s->offset = g_sec_fix_tmp->offset;

        if (g_sec_fix_tmp->bitwindow != 0)
          s->bitwindow = g_sec_fix_tmp->bitwindow;

        if (g_sec_fix_tmp->window_start != -1)
          s->window_start = g_sec_fix_tmp->window_start;

        if (g_sec_fix_tmp->window_end != -1)
          s->window_end = g_sec_fix_tmp->window_end;
        
        if (g_sec_fix_tmp->orga >= 0) {
          if (g_sec_fix_tmp->orga < g_slots[s->slot].address || g_sec_fix_tmp->orga >= g_slots[s->slot].address + g_slots[s->slot].size) {
            fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: ORGA $%.4x is outside of the SLOT %d.\n", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number, g_sec_fix_tmp->orga, s->slot);
            return FAILED;
          }
          s->address = g_sec_fix_tmp->orga - g_slots[s->slot].address;
        }
        if (g_sec_fix_tmp->org >= 0)
          s->address = g_sec_fix_tmp->org;

        if (g_sec_fix_tmp->size >= 0) {
          if (g_sec_fix_tmp->size == s->size) {
            /* do nothing */
          }
          else if (g_sec_fix_tmp->size < s->size) {
            /* we cannot shrink the section */
            fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Shrinking a section (\"%s\" in this case) is not allowed.\n", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number, s->name);
            return FAILED;
          }
          else {
            /* the new size is larger than the old one -> enlarge the buffer */
            int i;

            s->data = realloc(s->data, g_sec_fix_tmp->size);
            if (s->data == NULL) {
              fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Out of memory error while enlarging section \"%s\".\n", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number, s->name);
              return FAILED;
            }

            /* emptyfill the new data */
            for (i = s->size; i < g_sec_fix_tmp->size; i++)
              s->data[i] = g_emptyfill;

            s->size = g_sec_fix_tmp->size;
          }
        }

        if (s->status == SECTION_STATUS_SEMISUPERFREE && s->banks[0] == 0) {
          fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Section \"%s\" is marked as SEMISUPERFREE, but no BANKS are defined.\n", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number, s->name);
          return FAILED;
        }
        if (s->status != SECTION_STATUS_SEMISUPERFREE && s->banks[0] != 0) {
          fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Section \"%s\" is not SEMISUPERFREE, but BANKS are defined.\n", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number, s->name);
          return FAILED;
        }
        
        break;
      }
      s = s->next;
    }

    if (s == NULL) {
      fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Could not find ", g_sec_fix_tmp->file_name, g_sec_fix_tmp->line_number);
      if (g_sec_fix_tmp->is_ramsection == YES)
        fprintf(stderr, "RAM section");
      else
        fprintf(stderr, "section");
      fprintf(stderr, " \"%s\".\n", g_sec_fix_tmp->name);
      return FAILED;
    }

    g_sec_fix_tmp = g_sec_fix_tmp->next;
  }

  return SUCCEEDED;
}


/* determines the section for each label, and calls "insert_label_into_maps" for each. */
int fix_label_sections(void) {

  struct section *s;
  struct label *l;

  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES) {
      if (l->section_status == ON) {
        /* search for the label's section */
        s = find_section(l->section);
        if (s == NULL) {
          fprintf(stderr, "FIX_LABEL_SECTIONS: Internal error: couldn't find section %d for label \"%s\".\n",
                  l->section, l->name);
          return FAILED;
        }

        l->section_struct = s;
      }

      if (insert_label_into_maps(l, 0) == FAILED)
        return FAILED;
    }
    
    l = l->next;
  }

  return SUCCEEDED;
}


/* determines which hashmaps are relevant for the label, and adds it to them. */
int insert_label_into_maps(struct label* l, int is_sizeof) {

  int put_in_global = 1;
  int put_in_anything = 1;
  char* base_name;

  /* for "sizeof" labels, "base_name" refers to the label name without the "_sizeof_"
   * prefix. */
  base_name = l->name;
  if (is_sizeof)
    base_name += 8;

  if (l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT ||
      is_label_anonymous(base_name) == YES) {
    /* don't put anonymous labels, breakpoints, or symbols into any maps */
    put_in_anything = 0;
  }

  if (l->section_status == ON) {
    struct section *s;

    s = l->section_struct;

    if (put_in_anything) {
      /* put label into section's label map */
      if (_try_put_label(s->label_map, l) == FAILED)
        return FAILED;

      if (base_name[0] == '_')
        put_in_global = 0;

      /* put label into section's namespace's label map, if it's not a local label */
      if (s->nspace != NULL && base_name[0] != '_') {
        if (_try_put_label(s->nspace->label_map, l) == FAILED)
          return FAILED;
        put_in_global = 0;
      }
    }
  }

  /* put the label into the global namespace */
  if (put_in_anything && put_in_global) {
    if (_try_put_label(g_global_unique_label_map, l) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int fix_label_addresses(void) {

  struct section *s;
  struct label *l;

  /* fix labels' addresses */
  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES) {
      if (l->status == LABEL_STATUS_LABEL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
        if (l->section_status == ON) {
          if (l->section_struct == NULL) {
            fprintf(stderr, "FIX_LABEL_ADDRESSES: Internal error: section_struct is NULL.\n");
            return FAILED;
          }
          s = l->section_struct;
          if (s->id == l->section) {
            l->bank = s->bank;
            l->slot = s->slot;
            l->address_in_section = (int)l->address;
            l->address += s->address;

            if (s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE || s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE)
              l->rom_address = l->address + g_slots[l->slot].size * l->bank;
            else
              l->rom_address = l->address + g_bankaddress[l->bank];

            if (s->status != SECTION_STATUS_ABSOLUTE)
              l->address += g_slots[l->slot].address;
          }
          else {
            fprintf(stderr, "FIX_LABELS: Internal error: label's section ID and its sections section ID don't match!\n");
          }
        }
        else {
          l->address_in_section = (int)l->address;
          l->rom_address = l->address + g_bankaddress[l->bank];
          l->address += g_slots[l->slot].address;
        }
      }
    }
    l = l->next;
  }

  return SUCCEEDED;
}


static int _handle_special_case(int special_id, int file_id, int file_id_source, int linenumber, int value, int *out) {

  if (special_id == 1) {
    /* Z80/GB-Z80 RST */
    if (value == 0x00)
      *out = 0xC7;
    else if (value == 0x08)
      *out = 0xCF;
    else if (value == 0x10)
      *out = 0xD7;
    else if (value == 0x18)
      *out = 0xDF;
    else if (value == 0x20)
      *out = 0xE7;
    else if (value == 0x28)
      *out = 0xEF;
    else if (value == 0x30)
      *out = 0xF7;
    else if (value == 0x38)
      *out = 0xFF;
    else {
      fprintf(stderr, "%s: %s:%d: _HANDLE_SPECIAL_CASE: RST is expecting $00/$08/$10/$18/$20/$28/$30/$38, got $%.2x instead.\n",
              get_file_name(file_id), get_source_file_name(file_id, file_id_source), linenumber, value);
      return FAILED;
    }
  }
  else if (special_id == 2) {
    /* 8008 RST */
    if (value == 0)
      *out = 0x05;
    else if (value == 1)
      *out = 0x0D;
    else if (value == 2)
      *out = 0x15;
    else if (value == 3)
      *out = 0x1D;
    else if (value == 4)
      *out = 0x25;
    else if (value == 5)
      *out = 0x2D;
    else if (value == 6)
      *out = 0x35;
    else if (value == 7)
      *out = 0x3D;
    else {
      fprintf(stderr, "%s: %s:%d: _HANDLE_SPECIAL_CASE: RST is expecting 0/1/2/3/4/5/6/7, got $%.2x instead.\n",
              get_file_name(file_id), get_source_file_name(file_id, file_id_source), linenumber, value);
      return FAILED;
    }
  }
  else if (special_id == 3) {
    /* 8080 RST */
    if (value == 0)
      *out = 0xC7;
    else if (value == 1)
      *out = 0xCF;
    else if (value == 2)
      *out = 0xD7;
    else if (value == 3)
      *out = 0xDF;
    else if (value == 4)
      *out = 0xE7;
    else if (value == 5)
      *out = 0xEF;
    else if (value == 6)
      *out = 0xF7;
    else if (value == 7)
      *out = 0xFF;
    else {
      fprintf(stderr, "%s: %s:%d: _HANDLE_SPECIAL_CASE: RST is expecting 0/1/2/3/4/5/6/7, got $%.2x instead.\n",
              get_file_name(file_id), get_source_file_name(file_id, file_id_source), linenumber, value);
      return FAILED;
    }
  }
  else if (special_id == 4) {
    /* endianess flipping is handled elsewhere */
  }
  else {
    fprintf(stderr, "_HANDLE_SPECIAL_CASE: Unknown special case ID %d! This in an internal WLA error. Please submit a bug report!\n", special_id);
    return FAILED;
  }

  return SUCCEEDED;
}


static int _flip_endianess(int value, int bits) {

  if (bits == 16) {
    int top, bottom;

    top = (value >> 8) & 0xFF;
    bottom = value & 0xFF;

    return (bottom << 8) | top;
  }
  else {
    fprintf(stderr, "_FLIP_ENDIANESS: Only 16-bit values can be flipped at the moment!\n");
    return value;
  }
}


int fix_references(void) {

  struct reference *r;
  struct section *s;
  struct label *l, lt;
  int i, x;

  g_section_overwrite = OFF;

  /* insert references */
  r = g_reference_first;
  while (r != NULL) {
    s = NULL;

    x = r->address;
    /* search for the section of the reference and fix the address */
    if (r->section_status == ON) {
      s = find_section(r->section);
      if (s != NULL) {
        r->bank = s->bank;
        x += s->address;
        r->address += s->address;
      }
      /* reference is inside a discarded section? */
      if (s != NULL && s->alive == NO) {
        r = r->next;
        continue;
      }
      if (s == NULL) {
        if (write_bank_header_references(r) == FAILED)
          return FAILED;
        r = r->next;
        continue;
      }
    }

    if (!(r->section_status == ON && s->status == SECTION_STATUS_ABSOLUTE)) {
      x += g_bankaddress[r->bank];
      r->address += g_slots[r->slot].address;
    }

    /* find the destination */
    l = NULL;

    /* request for bank number? */
    if (r->name[0] == ':') {
      if (is_label_anonymous(&r->name[1]) == YES)
        l = get_closest_anonymous_label(&r->name[1], r->context, x, r->file_id, r->section_status, r->section);
      else if (strcaselesscmp(&r->name[1], "CADDR") == 0) {
        lt.status = LABEL_STATUS_LABEL;
        strcpy(lt.name, &r->name[1]);
        lt.address = r->address;
        lt.bank = r->bank;
        lt.base = r->base;
        lt.slot = r->slot;
        lt.section_status = OFF;
        l = &lt;
      }
      else
        find_label(&r->name[1], s, &l);

      if (l == NULL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
        fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Bank number request for an unknown label \"%s\".\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, &r->name[1]);
        return FAILED;
      }

      if (get_file(r->file_id)->cpu_65816 == YES)
        i = get_snes_pc_bank(l) >> 16;
      else
        i = l->base + l->bank;

      g_memory_file_id = r->file_id;
      g_memory_file_id_source = r->file_id_source;
      g_memory_line_number = r->linenumber;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT || r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
        /* special case ID handling! */
        if (r->special_id == 4) {
          /* flip endianess */
          i = _flip_endianess(i, 16);
        }

        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 8) & 0xFF);
          mem_insert_ref(x + 1, i & 0xFF);
        }
      }
      /* direct 13-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_13BIT) {
        /* this is always little endian */
        mem_insert(x, i & 0xFF);
        mem_insert_ref_13bit_high(x + 1, (i >> 8) & 0xFF);
      }
      /* direct / relative 8-bit with a definition */
      else if (l->status == LABEL_STATUS_DEFINE) {
        fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Bank number request for a definition \"%s\"?\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, l->name);
        return FAILED;
      }
      /* direct 24-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, (i >> 16) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 16) & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, i & 0xFF);
        }
      }
      /* direct 32-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_32BIT) {
        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, (i >> 16) & 0xFF);
          mem_insert_ref(x + 3, (i >> 24) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 24) & 0xFF);
          mem_insert_ref(x + 1, (i >> 16) & 0xFF);
          mem_insert_ref(x + 2, (i >> 8) & 0xFF);
          mem_insert_ref(x + 3, i & 0xFF);
        }
      }
      /* bits */
      else if (r->type == REFERENCE_TYPE_BITS) {
        int mask = 0;

        if (r->bits_to_define < 32)
          mask = ~((1 << r->bits_to_define) - 1);

        if ((i & mask) != 0) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: We are defining %d bits, but the given value $%x (%d) uses more bits!\n", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->bits_to_define, i, i);
          return FAILED;
        }

        if (mem_insert_bits(x, i, r->bits_position, r->bits_to_define) == FAILED)
          return FAILED;
      }
      /* relative/direct 8-bit with a label */
      else {
        mem_insert_ref(x, i & 0xFF);
      }
    }
    /* normal reference */
    else {
      if (is_label_anonymous(r->name) == YES)
        l = get_closest_anonymous_label(r->name, r->context, x, r->file_id, r->section_status, r->section);
      else if (strcaselesscmp(r->name, "CADDR") == 0) {
        lt.status = LABEL_STATUS_DEFINE;
        strcpy(lt.name, r->name);
        lt.address = r->address;
        lt.bank = r->bank;
        lt.slot = r->slot;
        lt.base = r->base;
        lt.section_status = OFF;
        l = &lt;
      }
      else
        find_label(r->name, s, &l);

      if (l == NULL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
        fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Reference to an unknown label \"%s\".\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);
        return FAILED;
      }

      g_memory_file_id = r->file_id;
      g_memory_file_id_source = r->file_id_source;
      g_memory_line_number = r->linenumber;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
        i = (int)l->address;

        /* special case ID handling! */
        if (r->special_id == 4) {
          /* flip endianess */
          i = _flip_endianess(i, 16);
        }

        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 8) & 0xFF);
          mem_insert_ref(x + 1, i & 0xFF);
        }
      }
      /* direct 13-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_13BIT) {
        i = (int)l->address;
        if (i > 8191 || i < 0) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 13-bit value.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }
        /* this is always little endian */
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref_13bit_high(x + 1, (i >> 8) & 0xFF);
      }
      /* direct 9-bit short */
      else if (r->type == REFERENCE_TYPE_DIRECT_9BIT_SHORT) {
        i = (int)l->address;
        if (i > 510 || i < 0) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 9-bit value.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }
        if ((i & 1) == 1) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: The RAM address needs to be even.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber);
          return FAILED;
        }
        mem_insert_ref(x, (i >> 1) & 0xFF);
      }
      /* direct / relative 8-bit with a value definition */
      else if (l->status == LABEL_STATUS_DEFINE && (r->type == REFERENCE_TYPE_DIRECT_8BIT || r->type == REFERENCE_TYPE_RELATIVE_8BIT)) {
        i = ((int)l->address) & 0xFFFF;
        if (i > 255 || i < -128) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 8-bit value.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
      }
      /* direct 24-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
        i = (int)l->address;
        if (l->status == LABEL_STATUS_LABEL)
          i += get_snes_pc_bank(l);
        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, (i >> 16) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 16) & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, i & 0xFF);
        }
      }
      /* direct 32-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_32BIT) {
        i = (int)l->address;
        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
          mem_insert_ref(x + 2, (i >> 16) & 0xFF);
          mem_insert_ref(x + 3, (i >> 24) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 24) & 0xFF);
          mem_insert_ref(x + 1, (i >> 16) & 0xFF);
          mem_insert_ref(x + 2, (i >> 8) & 0xFF);
          mem_insert_ref(x + 3, i & 0xFF);
        }
      }
      /* relative 8-bit with a label */
      else if (r->type == REFERENCE_TYPE_RELATIVE_8BIT) {
        i = (((int)l->address) & 0xFFFF) - r->address - 1;
        if (i < -128 || i > 127) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a relative 8-bit reference.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
      }
      /* relative 16-bit with a label */
      else if (r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
        i = (((int)l->address) & 0xFFFF) - r->address - 2;
        /* NOTE: on 65ce02 the 16-bit relative references don't use the next
           instruction as the starting point, but one byte before it */
        if (get_file(r->file_id)->cpu_65ce02 == YES)
          i += 1;
        
        if (i < -32768 || i > 32767) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a relative 16-bit reference.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }

        if (get_file(r->file_id)->little_endian == YES) {
          mem_insert_ref(x, i & 0xFF);
          mem_insert_ref(x + 1, (i >> 8) & 0xFF);
        }
        else {
          mem_insert_ref(x, (i >> 8) & 0xFF);
          mem_insert_ref(x + 1, i & 0xFF);
        }
      }
      /* bits */
      else if (r->type == REFERENCE_TYPE_BITS) {
        int mask = 0;

        i = (int)l->address;

        if (r->bits_to_define < 32)
          mask = ~((1 << r->bits_to_define) - 1);

        if ((i & mask) != 0) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: We are defining %d bits, but the given value $%x (%d) uses more bits!\n", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->bits_to_define, i, i);
          return FAILED;
        }

        if (mem_insert_bits(x, i, r->bits_position, r->bits_to_define) == FAILED)
          return FAILED;
      }
      else {
        i = ((int)l->address) & 0xFFFF;
        if (i > 255) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 8-bit value.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }

        /* special case ID handling! */
        if (r->special_id > 0 && r->special_id != 4) {
          if (_handle_special_case(r->special_id, r->file_id, r->file_id_source, r->linenumber, i, &i) == FAILED)
            return FAILED;
        }

        mem_insert_ref(x, i & 0xFF);
      }
    }

    r = r->next;
  }

  return SUCCEEDED;
}


static void _fprintf_snes_label(FILE *f, struct label *l, int noca5h) {

  int bank = get_snes_pc_bank(l) >> 16;
  int address = (int)l->address;

  /* SECTION_END fix */
  if (address == 0x10000)
    address = 0;
  else if (address > 0xffff)
    fprintf(stderr, "%s: %s:%d: _fprintf_snes_label(): The address of label \"%s\" inside a bank is $%.4x > $FFFF! Please submit a bug report!\n", get_file_name(l->file_id), get_source_file_name(l->file_id, l->file_id_source), l->linenumber, l->name, address);

  if (noca5h == YES)
    fprintf(f, "%.4x%.4x %s\n", bank, address, l->name);
  else
    fprintf(f, "%.2x:%.4x %s\n", bank, address, l->name);
}


int write_symbol_file(char *outname, int mode, int output_addr_to_line) {

  struct source_file_name *src_file;
  struct object_file *obj_file;
  struct section *s;
  struct label *l;
  char name[256], list_cmd, *outfile_tmp;
  FILE *f, *outfile;
  int list_cmd_idx, list_source_file, list_address_offset, y, outfile_size, got_sections = NO, got_ramsections = NO;
  unsigned long outfile_crc;
  unsigned int name_len;

  if (outname == NULL)
    return FAILED;

  name_len = (unsigned int)strlen(outname);
  if (name_len > sizeof(name)-5) {
    fprintf(stderr, "WRITE_SYMBOL_FILE: File name too long.\n");
    return FAILED;
  }

  strcpy(name, outname);
  y = name_len-1;
  while (y >= 0 && name[y] != '.' && name[y] != 0)
    y--;
  if (y < 0)
    y = name_len;

  name[y++] = '.';
  name[y++] = 's';
  name[y++] = 'y';
  name[y++] = 'm';
  name[y] = 0;

  f = fopen(name, "wb");
  if (f == NULL) {
    fprintf(stderr, "WRITE_SYMBOL_FILE: Error opening file \"%s\" for writing.\n", name);
    return FAILED;
  }

  fprintf(f, "; this file was created with wlalink by ville helin <ville.helin@iki.fi>.\n");

  if (mode == SYMBOL_MODE_NOCA5H) {
    /* NOCA$H SYMBOL FILE */
    if (g_snes_mode == 0)
      fprintf(f, "; no$gmb symbolic information for \"%s\".\n", outname);
    else
      fprintf(f, "; no$snes symbolic information for \"%s\".\n", outname);

    l = g_labels_first;
    while (l != NULL) {
      if (l->alive == NO || is_label_anonymous(l->name) == YES || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT || l->status == LABEL_STATUS_DEFINE || l->status == LABEL_STATUS_STACK) {
        l = l->next;
        continue;
      }
      /* skip all dropped section labels */
      if (l->section_status == ON) {
        s = find_section(l->section);
        if (s->alive == NO) {
          l = l->next;
          continue;
        }
      }
      if (g_snes_mode == 0) {
        if (l->status == LABEL_STATUS_LABEL)
          fprintf(f, "%.2x:%.4x %s\n", l->base + l->bank, (int)l->address, l->name);
        else
          fprintf(f, "00:%.4x %s\n", (int)l->address, l->name);
      }
      else {
        if (l->status == LABEL_STATUS_LABEL)
          _fprintf_snes_label(f, l, YES);
        else
          fprintf(f, "%.8x %s\n", (int)l->address, l->name);
      }
      l = l->next;
    }
  }
  else {
    /* WLA SYMBOL FILE */
    fprintf(f, "; wla symbolic information for \"%s\".\n", outname);

    /* info section */
    fprintf(f, "\n[information]\n");
    fprintf(f, "version 3\n");
    
    /* labels */
    l = g_labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_LABEL) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[labels]\n");

      l = g_labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_LABEL) {
          l = l->next;
          continue;
        }
        if (is_label_anonymous(l->name) == YES) {
          l = l->next;
          continue;
        }

        /* skip all dropped section labels */
        if (l->section_status == ON) {
          s = find_section(l->section);
          if (s->alive == NO) {
            l = l->next;
            continue;
          }
        }

        if (g_snes_mode == 0)
          fprintf(f, "%.2x:%.4x %s\n", l->base + l->bank, (int)l->address, l->name);
        else
          _fprintf_snes_label(f, l, NO);

        l = l->next;
      }
    }

    /* symbols */
    l = g_labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_SYMBOL) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[symbols]\n");

      l = g_labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_SYMBOL) {
          l = l->next;
          continue;
        }

        if (g_snes_mode == 0)
          fprintf(f, "%.2x:%.4x %s\n", l->bank, (int)l->address, l->name);
        else
          fprintf(f, "%.2x:%.4x %s\n", get_snes_pc_bank(l)>>16, (int)l->address, l->name);

        l = l->next;
      }
    }

    /* breakpoints */
    l = g_labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_BREAKPOINT) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[breakpoints]\n");

      l = g_labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_BREAKPOINT) {
          l = l->next;
          continue;
        }

        if (g_snes_mode == 0)
          fprintf(f, "%.2x:%.4x\n", l->bank, (int)l->address);
        else
          fprintf(f, "%.2x:%.4x\n", get_snes_pc_bank(l)>>16, (int)l->address);

        l = l->next;
      }
    }

    /* definitions */
    l = g_labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_DEFINE) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[definitions]\n");

      l = g_labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_DEFINE) {
          l = l->next;
          continue;
        }
        if (is_label_anonymous(l->name) == YES) {
          l = l->next;
          continue;
        }

        fprintf(f, "%.8x %s\n", (unsigned int)l->address, l->name);

        l = l->next;
      }
    }

    /* sections */
    s = g_sec_first;
    while (s != NULL) {
      if (s->is_bankheader_section == NO && s->alive == YES) {
        if (s->status == SECTION_STATUS_FORCE || s->status == SECTION_STATUS_SEMISUPERFREE ||
            s->status == SECTION_STATUS_SEMISUBFREE || s->status == SECTION_STATUS_SEMIFREE ||
            s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_SUPERFREE ||
            s->status == SECTION_STATUS_OVERWRITE) {
          if (got_sections == NO) {
            fprintf(f, "\n[sections]\n");
            got_sections = YES;
          }
          
          fprintf(f, "%.8x %.2x:%.4x %.4x %.8x %s\n", s->output_address, s->bank + s->base, s->address, g_slots[s->slot].address + s->address, s->size, s->name);
        }
      }
      s = s->next;
    }

    /* ramsections */
    s = g_sec_first;
    while (s != NULL) {
      if (s->is_bankheader_section == NO && s->alive == YES) {
        if (s->status == SECTION_STATUS_RAM_FORCE || s->status == SECTION_STATUS_RAM_SEMIFREE ||
            s->status == SECTION_STATUS_RAM_SEMISUBFREE || s->status == SECTION_STATUS_RAM_FREE) {
          if (got_ramsections == NO) {
            fprintf(f, "\n[ramsections]\n");
            got_ramsections = YES;
          }
          
          fprintf(f, "%.2x:%.4x %.4x %.8x %s\n", s->bank + s->base, s->address, g_slots[s->slot].address + s->address, s->size, s->name);
        }
      }
      s = s->next;
    }

    if (output_addr_to_line == ON) {
      /* file_id_source to source files */
      fprintf(f, "\n[source files v2]\n");
      obj_file = g_obj_first;
      while (obj_file != NULL) {
        src_file = obj_file->source_file_names_list;
        while (src_file != NULL) {
          fprintf(f, "%.4x:%.4x %.8lx %s \n", obj_file->id + 1, src_file->id, src_file->checksum & 0xffffffffUL, src_file->name);
          src_file = src_file->next;
        }
        obj_file = obj_file->next;
      }

      /* full rom-output checksum */
      outfile = fopen(outname, "rb");
      fseek(outfile, 0, SEEK_END);
      outfile_size = (int)ftell(outfile);
      fseek(outfile, 0, SEEK_SET);
      outfile_tmp = calloc(sizeof(char) * outfile_size, 1);

      if (fread(outfile_tmp, 1, outfile_size, outfile) != (size_t) outfile_size) {
        fprintf(stderr, "WRITE_SYMBOL_FILE: Could not read all %d bytes of \"%s\"!", outfile_size, outname);
        return FAILED;
      }

      fclose(outfile);
      outfile_crc = crc32((unsigned char*)outfile_tmp, outfile_size);
      free(outfile_tmp);
      fprintf(f, "\n[rom checksum]\n%.8lx\n", outfile_crc);

      /* addr -> file/line mappings */
      s = g_sec_first;
      while (s != NULL) {
        if (s->listfile_items > 0) {
          fprintf(f, "\n[addr-to-line mapping v2]\n");
          break;
        }
        s = s->next;
      }

      s = g_sec_first;
      while (s != NULL) {
        /* parse the list file information */
        list_address_offset = 0;
        list_source_file = -1;
        for (list_cmd_idx = 0; list_cmd_idx < s->listfile_items; list_cmd_idx++) {
          list_cmd = s->listfile_cmds[list_cmd_idx];
          if (list_cmd == 'k') {
            /* new line */
            if (s->listfile_ints[list_cmd_idx * 3 + 1] > 0) {
              fprintf(f, "%.8x %.2x:%.4x %.4x %.4x:%.4x:%.8lx\n", s->output_address + list_address_offset, s->bank + s->base, s->address + list_address_offset, g_slots[s->slot].address + s->address + list_address_offset, s->file_id + 1, list_source_file, (long unsigned int)s->listfile_ints[list_cmd_idx * 3 + 0]);
              list_address_offset += s->listfile_ints[list_cmd_idx * 3 + 1];
            }
          }
          else if (list_cmd == 'f') {
            /* another file */
            list_source_file = s->listfile_ints[list_cmd_idx * 3 + 0];
          }
        }
        s = s->next;
      }
    }
  }

  fclose(f);

  return SUCCEEDED;
}


static int _get_rom_address_of_label(char *label, int *address) {

  struct label *l;

  find_label(label, NULL, &l);

  if (l == NULL) {
    fprintf(stderr, "_GET_ROM_ADDRESS_OF_LABEL: Cannot find label \"%s\".\n", label);
    return FAILED;
  }

  if (l->status != LABEL_STATUS_LABEL || (l->section_struct != NULL && (l->section_struct->status == SECTION_STATUS_RAM_FREE ||
                                                                        l->section_struct->status == SECTION_STATUS_RAM_FORCE ||
                                                                        l->section_struct->status == SECTION_STATUS_RAM_SEMIFREE ||
                                                                        l->section_struct->status == SECTION_STATUS_RAM_SEMISUBFREE ||
                                                                        l->section_struct->alive == NO))) {
    fprintf(stderr, "_GET_ROM_ADDRESS_OF_LABEL: \"%s\" cannot be used.\n", label);
    return FAILED;
  }

  *address = (int)l->rom_address;

  return SUCCEEDED;
}


int write_rom_file(char *outname) {

  struct section *s;
  FILE *f;
  int i, b, e;
  
  /* get the addresses of the program start and end */
  if (g_program_address_start_type == LOAD_ADDRESS_TYPE_LABEL) {
    if (_get_rom_address_of_label(g_program_address_start_label, &g_program_address_start) == FAILED)
      return FAILED;
  }
  if (g_program_address_end_type == LOAD_ADDRESS_TYPE_LABEL) {
    if (_get_rom_address_of_label(g_program_address_end_label, &g_program_address_end) == FAILED)
      return FAILED;
  }

  if (g_program_address_start > g_romsize) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) overflows from the ROM!\n", g_program_address_start);
    return FAILED;
  }
  if (g_program_address_end > g_romsize) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bE ($%x) overflows from the ROM!\n", g_program_address_end);
    return FAILED;
  }
  if (g_program_address_start >= 0 && g_program_address_end >= 0 && g_program_address_start > g_program_address_end) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) is larger than -bE ($%x).\n", g_program_address_start, g_program_address_end);
    return FAILED;
  }

  f = fopen(outname, "wb");
  if (f == NULL) {
    fprintf(stderr, "WRITE_ROM_FILE: Error opening file \"%s\" for writing.\n", outname);
    return FAILED;
  }

  if (g_file_header != NULL)
    fwrite(g_file_header, 1, g_file_header_size, f);

  /* SMC header */
  if (g_smc_status != 0)
    _smc_create_and_write(f);

  /* CBM PRG */
  if (g_output_type == OUTPUT_TYPE_CBM_PRG) {
    if (_cbm_write_prg_header(f) == FAILED) {
      fclose(f);
      return FAILED;
    }
  }
  
  /* ROM output mode */
  if (g_output_mode == OUTPUT_ROM) {
    /* write bank by bank and bank header sections */
    for (i = 0; i < g_rombanks; i++) {
      s = g_sec_bankhd_first;
      while (s != NULL) {
        if (s->bank == i) {
          fwrite(s->data, 1, s->size, f);
          break;
        }
        s = s->next;
      }

      fwrite(g_rom + g_bankaddress[i], 1, g_banksizes[i], f);
    }
  }
  /* program file output mode */
  else {
    for (i = 0; i < g_romsize; i++) {
      if (g_rom_usage[i] != 0)
        break;
    }
    b = i;
    for (e = b; i < g_romsize; i++) {
      if (g_rom_usage[i] != 0)
        e = i;
    }

    /* overrides from the options to WLALINK */
    if (g_program_address_start >= 0)
      b = g_program_address_start;
    if (g_program_address_end >= 0)
      e = g_program_address_end;

    s = g_sec_bankhd_first;
    while (s != NULL) {
      if (s->bank == 0) {
        fwrite(s->data, 1, s->size, f);
        break;
      }
      s = s->next;
    }

    fwrite(g_rom + b, 1, e - b + 1, f);
    g_program_start = b;
    g_program_end = e;

    if (g_program_address_start >= 0 && g_program_address_end < 0 && b > e) {
      fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) is larger than calculated end ($%x).\n", b, e);
      return FAILED;
    }
  }

  if (g_file_footer != NULL)
    fwrite(g_file_footer, 1, g_file_footer_size, f);

  fclose(f);

  return SUCCEEDED;
}


int compute_pending_calculations(void) {

  struct section *s;
  struct stack *sta;
  double result;
  int k, a;

  g_section_overwrite = ON;

  /* first place the calculation stacks into the output */
  sta = g_stacks_first;
  while (sta != NULL) {
    if (sta->position == STACK_POSITION_DEFINITION) {
      /* skip definition stacks */
      sta = sta->next;
      continue;
    }

    if (sta->section_status == ON) {
      /* get section address */
      s = find_section(sta->section);
      if (s != NULL)
        sta->bank = s->bank;
      /* the computation is inside a discarded section? */
      if (s != NULL && s->alive == NO) {
        sta = sta->next;
        continue;
      }
      /* it must be a bank header section! */
      if (s == NULL) {
        sta = sta->next;
        continue;
      }

      /* remember the memory address (for CADDR) */
      sta->memory_address = s->address + sta->address + g_slots[sta->slot].address;

      if (s->status != SECTION_STATUS_ABSOLUTE)
        sta->address += s->address + g_bankaddress[s->bank];
      else
        sta->address += s->address;
    }
    else {
      /* remember the memory address (for CADDR) */
      sta->memory_address = sta->address + g_slots[sta->slot].address;

      sta->address += g_bankaddress[sta->bank];
    }

    sta = sta->next;
  }

  /* next parse the stack items */
  sta = g_stacks_first;
  while (sta != NULL) {
    if (sta->position == STACK_POSITION_DEFINITION)
      k = 1;
    else {
      /* skip the calculations inside discarded sections */
      if (sta->section_status == ON) {
        /* get the section */
        s = find_section(sta->section);
        if (s != NULL && s->alive == YES)
          k = 1;
        else
          k = 0;
      }
      else
        k = 1;
    }
    if (k == 1) {
      if (parse_stack(sta) == FAILED)
        return FAILED;
    }
    sta = sta->next;
  }

  /* then compute and place the results */
  sta = g_stacks_first;
  while (sta != NULL) {
    /* is the stack inside a definition? */
    if (sta->position == STACK_POSITION_DEFINITION) {
      /* all the references have been decoded, now compute */
      if (compute_stack(sta, NULL, NULL, NULL, NULL, NULL) == FAILED)
        return FAILED;
      /* next stack computation */
      sta = sta->next;
      continue;
    }

    /* find source address */
    if (sta->section_status == ON) {
      /* get section address */
      s = find_section(sta->section);
      if (s != NULL)
        sta->bank = s->bank;
      /* the computation is inside a discarded section? */
      if (s != NULL && s->alive == NO) {
        sta = sta->next;
        continue;
      }
      /* it must be a bank header section! */
      if (s == NULL) {
        if (write_bank_header_calculations(sta) == FAILED)
          return FAILED;
        sta = sta->next;
        continue;
      }
    }

    a = sta->address;

    /* we save the address for all those CADDRs inside definition stacks that are
       encountered during the next compute_stack() */
    s_current_stack_calculation_addr = sta->memory_address;

    /* all the references have been decoded, now compute */
    if (compute_stack(sta, &result, NULL, NULL, NULL, NULL) == FAILED)
      return FAILED;

    k = (int)result;

    g_memory_file_id = sta->file_id;
    g_memory_file_id_source = sta->file_id_source;
    g_memory_line_number = sta->linenumber;

    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing pending calculation %s: %s:%d.", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);

    if (sta->type == STACK_TYPE_8BIT) {
      if (k < -128 || k > 255) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
      }

      /* special case ID handling! */
      if (sta->special_id > 0) {
        if (_handle_special_case(sta->special_id, sta->file_id, sta->file_id_source, sta->linenumber, k, &k) == FAILED)
          return FAILED;
      }

      if (mem_insert_ref(a, k) == FAILED)
        return FAILED;
    }
    else if (sta->type == STACK_TYPE_9BIT_SHORT) {
      if (k < 0 || k > 510) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 9-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
      }
      if ((k & 1) == 1) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: The RAM address needs to be even.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
        return FAILED;
      }

      if (mem_insert_ref(a, k >> 1) == FAILED)
        return FAILED;
    }
    else if (sta->type == STACK_TYPE_16BIT) {
      if (k < -32768 || k > 65535) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
      }

      /* special case ID handling! */
      if (sta->special_id == 4) {
        /* flip endianess */
        k = _flip_endianess(k, 16);
      }
      
      if (get_file(sta->file_id)->little_endian == YES) {
        if (mem_insert_ref(a, k & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert_ref(a, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, k & 0xFF) == FAILED)
          return FAILED;
      }
    }
    else if (sta->type == STACK_TYPE_13BIT) {
      if (k < 0 || k > 8191) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 13-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
      }
      /* this is always little endian */
      if (mem_insert_ref(a, k & 0xFF) == FAILED)
        return FAILED;
      if (mem_insert_ref_13bit_high(a + 1, (k >> 8) & 0xFF) == FAILED)
        return FAILED;
    }
    else if (sta->type == STACK_TYPE_24BIT) {
      if (k < -8388608 || k > 16777215) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 24-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
      }
      if (get_file(sta->file_id)->little_endian == YES) {
        if (mem_insert_ref(a, k & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 2, (k >> 16) & 0xFF) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert_ref(a, (k >> 16) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 2, k & 0xFF) == FAILED)
          return FAILED;
      }
    }
    else if (sta->type == STACK_TYPE_32BIT) {
      if (get_file(sta->file_id)->little_endian == YES) {
        if (mem_insert_ref(a, k & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 2, (k >> 16) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 3, (k >> 24) & 0xFF) == FAILED)
          return FAILED;
      }
      else {
        if (mem_insert_ref(a, (k >> 24) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 1, (k >> 16) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 2, (k >> 8) & 0xFF) == FAILED)
          return FAILED;
        if (mem_insert_ref(a + 3, k & 0xFF) == FAILED)
          return FAILED;
      }
    }
    else if (sta->type == STACK_TYPE_BITS) {
      int mask = 0;

      if (sta->bits_to_define < 32)
        mask = ~((1 << sta->bits_to_define) - 1);

      if ((k & mask) != 0) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: We are defining %d bits, but the given value $%x (%d) uses more bits!\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, sta->bits_to_define, k, k);
        return FAILED;
      }

      if (mem_insert_bits(a, k, sta->bits_position, sta->bits_to_define) == FAILED)
        return FAILED;
    }
    else {
      fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Unsupported pending calculation type. Please send an error report!\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
    }

    /* next stack computation */
    sta = sta->next;
  }

  return SUCCEEDED;
}


static int _get_bank_of_address(int address, int slot) {

  int start_address, slot_size, j;

  if (address < 0)
    return -1;

  if (slot < 0) {
    /* we don't know the SLOT so we'll use the .ROMBANKSIZE for the calculation */
    start_address = 0;
    j = 0;

    while (1) {
      if (address >= start_address && address < start_address + g_banksize)
        return j;
      start_address += g_banksize;
      j++;
      if (j > 1000000000) {
        fprintf(stderr, "_GET_BANK_OF_ADDRESS: j > 1000000000! Internal error. Cannot find the BANK. Please submit a bug report.\n");
        return -1;
      }
    }

    return -1;
  }

  /* TODO: check if we can just use the banksize in every case and ignore slots[slot].size completely */
  
  slot_size = g_slots[slot].size;;
  start_address = 0;
  j = 0;
  
  while (1) {
    if (address >= start_address && address < start_address + slot_size)
      return j;
    start_address += slot_size;
    j++;
    if (j > 1000000000) {
      fprintf(stderr, "_GET_BANK_OF_ADDRESS: j > 1000000000! Internal error. Cannot find the BANK. Please submit a bug report.\n");
      return -1;
    }
  }
}


static void _pass_on_slot(int *slot, int t) {

  if (slot[t - 2] < 0 && slot[t - 1] >= 0)
    slot[t - 2] = slot[t - 1];
  else if (slot[t - 2] >= 0 && slot[t - 1] >= 0) {
    /* sanity check */
    /*
    if (slot[t - 2] != slot[t - 1]) {
      fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: The passed on SLOT changed from $%x to $%x. This might have no effect, but just to let you know. Please check that the result of this calculation is correct.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, slot[t - 2], slot[t - 1]);
    }
    */
    slot[t - 2] = slot[t - 1];
  }
}


static void _pass_on_base(int *base, int t) {

  if (base[t - 2] < 0 && base[t - 1] >= 0)
    base[t - 2] = base[t - 1];
  else if (base[t - 2] >= 0 && base[t - 1] >= 0) {
    /* sanity check */
    /*
    if (base[t - 2] != base[t - 1]) {
      fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: The passed on BASE changed from $%x to $%x. This might have no effect, but just to let you know. Please check that the result of this calculation is correct.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, base[t - 2], base[t - 1]);
    }
    */
    base[t - 2] = base[t - 1];
  }
}


static void _pass_on_bank(int *bank, int t) {

  if (bank[t - 2] < 0 && bank[t - 1] >= 0)
    bank[t - 2] = bank[t - 1];
  else if (bank[t - 2] >= 0 && bank[t - 1] >= 0) {
    /* sanity check */
    /*
    if (bank[t - 2] != bank[t - 1]) {
      fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: The passed on BANK changed from $%x to $%x. This might have no effect, but just to let you know. Please check that the result of this calculation is correct.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, bank[t - 2], bank[t - 1]);
    }
    */
    bank[t - 2] = bank[t - 1];
  }
}


static double _round(double d) {

  int i = (int)d;

  double delta = d - (double)i;
  if (delta < 0.0) {
    if (delta <= -0.5)
      return (double)(i - 1);
    else
      return (double)i;
  }
  else {
    if (delta < 0.5)
      return (double)i;
    else
      return (double)(i + 1);
  }
}

static int _comparing_a_string_with_a_number(struct stack_item *sp1, struct stack_item *sp2, struct stack *sta) {
  
  if (sp1->type == STACK_ITEM_TYPE_STRING && sp2->type != STACK_ITEM_TYPE_STRING) {  
    fprintf(stderr, "%s:%d: COMPUTE_STACK: Comparison between a string \"%s\" and a number doesn't work.\n", get_file_name(sta->file_id), sta->linenumber, sp1->string);
    return YES;
  }
  else if (sp1->type != STACK_ITEM_TYPE_STRING && sp2->type == STACK_ITEM_TYPE_STRING) {  
    fprintf(stderr, "%s:%d: COMPUTE_STACK: Comparison between a string \"%s\" and a number doesn't work.\n", get_file_name(sta->file_id), sta->linenumber, sp2->string);
    return YES;
  }
  
  return NO;
}

const char *get_stack_item_operator_name(int operator);
char *get_stack_item_description(struct stack_item *si, int file_id);


/* we have trouble with "and" on Amiga thus this function - a bug in SAS/C? perhaps issues on other platforms as well? */
static int _perform_and(int a, int b) {

  return a & b;
}


int compute_stack(struct stack *sta, double *result_ram, double *result_rom, int *result_slot, int *result_base, int *result_bank) {

  int r, t, z, y, x, res_base, res_bank, res_slot, slot[256], base[256], bank[256];
  double v_ram[256], v_rom[256], q, res_ram, res_rom;
  struct stack_item *s;
  struct stack *st;

  if (sta->under_work == YES) {
    fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: A loop found in computation.\n", get_file_name(sta->file_id),
            get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
    return FAILED;
  }

  /* because there might be CADDR in a definition stack calculation, we'll need to recalculate 
     the stack calculation every time again... */
  /*
    if (sta->computed == YES && sta->position != STACK_POSITION_DEFINITION) {
    if (result_ram != NULL)
    *result_ram = sta->result_ram;
    if (result_rom != NULL)
    *result_rom = sta->result_rom;
    if (result_slot != NULL)
    *result_slot = sta->result_slot;
    if (result_base != NULL)
    *result_base = sta->result_base;
    return SUCCEEDED;
    }
  */

  for (x = 0; x < 256; x++) {
    slot[x] = -1;
    base[x] = -1;
    bank[x] = -1;
  }

  sta->under_work = YES;
  v_ram[0] = 0.0;
  v_rom[0] = 0.0;

  /*
    {
    char *get_stack_item_description(struct stack_item *si, int file_id);
    int z;

    printf("----------------------------------------------------------------------\n");

    for (z = 0; z < sta->stacksize; z++) {
    struct stack_item *si = &sta->stack[z];
    printf(get_stack_item_description(si, sta->file_id));
    }

    printf("id: %d file: %s line: %d type %d bank: %d position %d\n", sta->id, get_file_name(sta->file_id), sta->linenumber, sta->type, sta->bank, sta->position);
    }
  */

  x = sta->stacksize;
  s = sta->stack_items;
  for (r = 0, t = 0; r < x; r++, s++) {
    if (s->type == STACK_ITEM_TYPE_VALUE) {
      if (s->sign == SI_SIGN_NEGATIVE) {
        v_ram[t] = -s->value_ram;
        v_rom[t] = -s->value_rom;
      }
      else {
        v_ram[t] = s->value_ram;
        v_rom[t] = s->value_rom;
      }

      slot[t] = s->slot;
      base[t] = s->base;
      bank[t] = s->bank;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STRING) {
      v_ram[t] = -1;
      v_rom[t] = -1;
      slot[t] = -1;
      base[t] = -1;
      bank[t] = -1;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_LABEL) {
      /* parse_stack() turned this string into a value, and embedded the sign into the value */
      if (s->sign == SI_SIGN_NEGATIVE) {
        v_ram[t] = -s->value_ram;
        v_rom[t] = -s->value_rom;
      }
      else {
        v_ram[t] = s->value_ram;
        v_rom[t] = s->value_rom;
      }

      slot[t] = s->slot;
      base[t] = s->base;
      bank[t] = s->bank;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STACK) {
      int file_id;
      
      /* we have a stack (A) inside a stack (B)! find the stack (A)! */
      if (s->stack_file_id >= 0)
        file_id = s->stack_file_id;
      else
        file_id = sta->file_id;
      
      st = find_stack((int)s->value_ram, file_id);

      if (st == NULL) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: A computation stack (id = %d, %s) has gone missing. This is a fatal internal error. Please send the WLA DX author a bug report.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, (int)s->value_ram, get_file_name(file_id));
        return FAILED;
      }

      if (st->position == STACK_POSITION_DEFINITION) {
        /* we'll need to do this as "st" might contain CADDR */
        if (parse_stack(st) == FAILED)
          return FAILED;
      }
      if (compute_stack(st, &res_ram, &res_rom, &res_slot, &res_base, &res_bank) == FAILED)
        return FAILED;

      if (s->sign == SI_SIGN_NEGATIVE) {
        v_ram[t] = -res_ram;
        v_rom[t] = -res_rom;
      }
      else {
        v_ram[t] = res_ram;
        v_rom[t] = res_rom;
      }
      
      slot[t] = res_slot;
      base[t] = res_base;
      bank[t] = res_bank;
      t++;
    }
    else {
      switch ((int)s->value_ram) {
      case SI_OP_ADD:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Addition is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] += v_ram[t - 1];
        v_rom[t - 2] += v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_SUB:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Subtraction is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] -= v_ram[t - 1];
        v_rom[t - 2] -= v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_NEGATE:
        v_ram[t - 1] = -v_ram[t - 1];
        v_rom[t - 1] = -v_rom[t - 1];
        break;
      case SI_OP_NOT:
        if (sta->type == STACK_TYPE_8BIT)
          y = 0xFF;
        else if (sta->type == STACK_TYPE_9BIT_SHORT)
          y = 0x1FF;
        else if (sta->type == STACK_TYPE_13BIT)
          y = 8191;
        else if (sta->type == STACK_TYPE_16BIT)
          y = 0xFFFF;
        else if (sta->type == STACK_TYPE_24BIT)
          y = 0xFFFFFF;
        else if (sta->type == STACK_TYPE_32BIT)
          y = 0xFFFFFFFF;
        else {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: NOT cannot determine the output size.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 1] = (int)v_ram[t - 1] ^ y;
        v_rom[t - 1] = (int)v_rom[t - 1] ^ y;
        break;
      case SI_OP_XOR:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: XOR is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 1] ^ (int)v_ram[t - 2];
        v_rom[t - 2] = (int)v_rom[t - 1] ^ (int)v_rom[t - 2];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_MULTIPLY:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Multiply is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] *= v_ram[t - 1];
        v_rom[t - 2] *= v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_OR:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: OR is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 1] | (int)v_ram[t - 2];
        v_rom[t - 2] = (int)v_rom[t - 1] | (int)v_rom[t - 2];
        if (slot[t - 2] < 0 && slot[t - 1] >= 0)
          slot[t - 2] = slot[t - 1];
        t--;
        break;
      case SI_OP_AND:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: AND is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 1] & (int)v_ram[t - 2];
        v_rom[t - 2] = (int)v_rom[t - 1] & (int)v_rom[t - 2];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_ROUND:
        v_ram[t - 1] = _round(v_ram[t - 1]);
        v_rom[t - 1] = _round(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_FLOOR:
        v_ram[t - 1] = floor(v_ram[t - 1]);
        v_rom[t - 1] = floor(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_CEIL:
        v_ram[t - 1] = ceil(v_ram[t - 1]);
        v_rom[t - 1] = ceil(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_MIN:
        if (v_ram[t - 1] < v_ram[t - 2])
          v_ram[t - 2] = v_ram[t - 1];
        if (v_rom[t - 1] < v_rom[t - 2])
          v_rom[t - 2] = v_rom[t - 2];
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 2] = -v_ram[t - 2];
          v_rom[t - 2] = -v_rom[t - 2];
        }
        t--;
        break;
      case SI_OP_MAX:
        if (v_ram[t - 1] > v_ram[t - 2])
          v_ram[t - 2] = v_ram[t - 1];
        if (v_rom[t - 1] > v_rom[t - 2])
          v_rom[t - 2] = v_rom[t - 1];
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 2] = -v_ram[t - 2];
          v_rom[t - 2] = -v_rom[t - 2];
        }
        t--;
        break;        
      case SI_OP_SQRT:
        if (v_ram[t - 1] < 0.0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: sqrt() needs a value that is >= 0.0, %f doesn't work!\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, v_ram[t - 1]);
          return FAILED;
        }
        if (v_rom[t - 1] < 0.0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: sqrt() needs a value that is >= 0.0, %f doesn't work!\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, v_rom[t - 1]);
          return FAILED;
        }
        v_ram[t - 1] = sqrt(v_ram[t - 1]);
        v_rom[t - 1] = sqrt(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_ABS:
        v_ram[t - 1] = fabs(v_ram[t - 1]);
        v_rom[t - 1] = fabs(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_SIGN:
        if (v_ram[t - 1] == 0.0)
          v_ram[t - 1] = 0.0;
        else if (v_ram[t - 1] < 0.0)
          v_ram[t - 1] = -1.0;
        else
          v_ram[t - 1] = 1.0;

        if (v_rom[t - 1] == 0.0)
          v_rom[t - 1] = 0.0;
        else if (v_rom[t - 1] < 0.0)
          v_rom[t - 1] = -1.0;
        else
          v_rom[t - 1] = 1.0;
        
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_COS:
        v_ram[t - 1] = cos(v_ram[t - 1]);
        v_rom[t - 1] = cos(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_SIN:
        v_ram[t - 1] = sin(v_ram[t - 1]);
        v_rom[t - 1] = sin(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_TAN:
        v_ram[t - 1] = tan(v_ram[t - 1]);
        v_rom[t - 1] = tan(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_LOG:
        v_ram[t - 1] = log(v_ram[t - 1]);
        v_rom[t - 1] = log(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_LOG10:
        v_ram[t - 1] = log10(v_ram[t - 1]);
        v_rom[t - 1] = log10(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_COSH:
        v_ram[t - 1] = cosh(v_ram[t - 1]);
        v_rom[t - 1] = cosh(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_SINH:
        v_ram[t - 1] = sinh(v_ram[t - 1]);
        v_rom[t - 1] = sinh(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_TANH:
        v_ram[t - 1] = tanh(v_ram[t - 1]);
        v_rom[t - 1] = tanh(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_ATAN:
        v_ram[t - 1] = atan(v_ram[t - 1]);
        v_rom[t - 1] = atan(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_ATAN2:
        v_ram[t - 2] = atan2(v_ram[t - 2], v_ram[t - 1]);
        v_rom[t - 2] = atan2(v_rom[t - 2], v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 2] = -v_ram[t - 2];
          v_rom[t - 2] = -v_rom[t - 2];
        }
        t--;
        break;
      case SI_OP_CLAMP:
        if (v_ram[t - 3] < v_ram[t - 2])
          v_ram[t - 3] = v_ram[t - 2];
        else if (v_ram[t - 3] > v_ram[t - 1])
          v_ram[t - 3] = v_ram[t - 1];

        if (v_rom[t - 3] < v_rom[t - 2])
          v_rom[t - 3] = v_rom[t - 2];
        else if (v_rom[t - 3] > v_rom[t - 1])
          v_rom[t - 3] = v_rom[t - 1];
        
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 3] = -v_ram[t - 3];
          v_rom[t - 3] = -v_rom[t - 3];
        }
        
        t -= 2;
        break;
      case SI_OP_POW:
        v_ram[t - 2] = pow(v_ram[t - 2], v_ram[t - 1]);
        v_rom[t - 2] = pow(v_rom[t - 2], v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 2] = -v_ram[t - 2];
          v_rom[t - 2] = -v_rom[t - 2];
        }
        t--;
        break;
      case SI_OP_ASIN:
        v_ram[t - 1] = asin(v_ram[t - 1]);
        v_rom[t - 1] = asin(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_ACOS:
        v_ram[t - 1] = acos(v_ram[t - 1]);
        v_rom[t - 1] = acos(v_rom[t - 1]);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_BANK:
        z = (int)v_rom[t - 1];
        y = _get_bank_of_address(z, slot[t - 1]);
        if (y < 0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Could not get the bank number for ROM address %d/$%x (out of bounds).\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, z, z);
          return FAILED;
        }
        if (base[t - 1] > 0)
          y += base[t - 1];
        v_ram[t - 1] = y & 0xFF;
        v_rom[t - 1] = y & 0xFF;
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_BANK_BYTE:
        z = ((int)v_ram[t - 1]) >> 16;
        y = ((int)v_rom[t - 1]) >> 16;

        if (z == 0) {
          if (bank[t - 1] > 0)
            z += bank[t - 1];
          if (base[t - 1] > 0)
            z += base[t - 1];
        }
        if (y == 0) {
          if (bank[t - 1] > 0)
            y += bank[t - 1];
          if (base[t - 1] > 0)
            y += base[t - 1];
        }
                
        v_ram[t - 1] = _perform_and(z, 0xFF);
        v_rom[t - 1] = _perform_and(y, 0xFF);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_LOW_BYTE:
        z = (int)v_ram[t - 1];
        y = (int)v_rom[t - 1];
        v_ram[t - 1] = _perform_and(z, 0xFF);
        v_rom[t - 1] = _perform_and(y, 0xFF);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_HIGH_BYTE:
        z = ((int)v_ram[t - 1]) >> 8;
        y = ((int)v_rom[t - 1]) >> 8;
        v_ram[t - 1] = _perform_and(z, 0xFF);
        v_rom[t - 1] = _perform_and(y, 0xFF);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_LOW_WORD:
        z = (int)v_ram[t - 1];
        y = (int)v_rom[t - 1];
        v_ram[t - 1] = _perform_and(z, 0xFFFF);
        v_rom[t - 1] = _perform_and(y, 0xFFFF);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_HIGH_WORD:
        z = ((int)v_ram[t - 1]) >> 16;
        y = ((int)v_rom[t - 1]) >> 16;
        v_ram[t - 1] = _perform_and(z, 0xFFFF);
        v_rom[t - 1] = _perform_and(y, 0xFFFF);
        if (s->sign == SI_SIGN_NEGATIVE) {
          v_ram[t - 1] = -v_ram[t - 1];
          v_rom[t - 1] = -v_rom[t - 1];
        }
        break;
      case SI_OP_MODULO:
        if (((int)v_ram[t - 1]) == 0 || ((int)v_rom[t - 1]) == 0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Modulo by zero.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Modulo is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 2] % (int)v_ram[t - 1];
        v_rom[t - 2] = (int)v_rom[t - 2] % (int)v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_DIVIDE:
        if (v_ram[t - 1] == 0.0 || v_rom[t - 1] == 0.0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Division by zero.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Division is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] /= v_ram[t - 1];
        v_rom[t - 2] /= v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_POWER:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Power is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        /* RAM */
        q = 1;
        for (z = 0; z < v_ram[t - 1]; z++)
          q *= v_ram[t - 2];
        v_ram[t - 2] = q;
        /* ROM */
        q = 1;
        for (z = 0; z < v_rom[t - 1]; z++)
          q *= v_rom[t - 2];
        v_rom[t - 2] = q;
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_SHIFT_LEFT:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Shift left is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 2] << (int)v_ram[t - 1];
        v_rom[t - 2] = (int)v_rom[t - 2] << (int)v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_SHIFT_RIGHT:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Shift right is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 2] = (int)v_ram[t - 2] >> (int)v_ram[t - 1];
        v_rom[t - 2] = (int)v_rom[t - 2] >> (int)v_rom[t - 1];
        _pass_on_slot(slot, t);
        _pass_on_base(base, t);
        _pass_on_bank(bank, t);
        t--;
        break;
      case SI_OP_LOGICAL_OR:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Logical or is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        if (v_ram[t-1] != 0 || v_ram[t-2] != 0)
          v_ram[t-2] = 1;
        else
          v_ram[t-2] = 0;
        if (v_rom[t-1] != 0 || v_rom[t-2] != 0)
          v_rom[t-2] = 1;
        else
          v_rom[t-2] = 0;
        t--;
        break;
      case SI_OP_LOGICAL_AND:
        if (t <= 1) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Logical and is missing an operand.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        if (v_ram[t-1] != 0 && v_ram[t-2] != 0)
          v_ram[t-2] = 1;
        else
          v_ram[t-2] = 0;
        if (v_rom[t-1] != 0 && v_rom[t-2] != 0)
          v_rom[t-2] = 1;
        else
          v_rom[t-2] = 0;
        t--;
        break;
      case SI_OP_COMPARE_LT:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) < 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] < v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] < v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      case SI_OP_COMPARE_GT:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) > 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] > v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] > v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      case SI_OP_COMPARE_EQ:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) == 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] == v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] == v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      case SI_OP_COMPARE_NEQ:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) != 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] != v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] != v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      case SI_OP_COMPARE_LTE:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) <= 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] <= v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] <= v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      case SI_OP_COMPARE_GTE:
        if (_comparing_a_string_with_a_number(&sta->stack_items[r-2], &sta->stack_items[r-1], sta) == YES)
          return FAILED;
        if (sta->stack_items[r-2].type == STACK_ITEM_TYPE_STRING && sta->stack_items[r-1].type == STACK_ITEM_TYPE_STRING) {
          if (strcmp(sta->stack_items[r-2].string, sta->stack_items[r-1].string) >= 0) {
            v_ram[t-2] = 1;
            v_rom[t-2] = 1;
          }
          else {
            v_ram[t-2] = 0;
            v_rom[t-2] = 0;
          }
        }
        else {
          if (v_ram[t-2] >= v_ram[t-1])
            v_ram[t-2] = 1;
          else
            v_ram[t-2] = 0;
          if (v_rom[t-2] >= v_rom[t-1])
            v_rom[t-2] = 1;
          else
            v_rom[t-2] = 0;
        }
        t--;
        break;
      }
    }
  }

  if (result_ram != NULL)
    *result_ram = v_ram[0];
  if (result_rom != NULL)
    *result_rom = v_rom[0];
  if (result_slot != NULL)
    *result_slot = sta->result_slot;
  if (result_base != NULL)
    *result_base = sta->result_base;
  if (result_bank != NULL)
    *result_bank = sta->result_bank;

  sta->result_ram = v_ram[0];
  sta->result_rom = v_rom[0];
  sta->result_slot = (int)slot[0];
  sta->result_base = (int)base[0];
  sta->result_bank = (int)bank[0];

  sta->computed = YES;
  sta->under_work = NO;

  /*
    printf("RESULT: %f (RAM) %f (ROM)\n", sta->result_ram, sta->result_rom);
  */

  return SUCCEEDED;
}


int write_bank_header_calculations(struct stack *sta) {

  struct section *s;
  unsigned char *t;
  double result;
  int k;

  /* parse stack items */
  if (parse_stack(sta) == FAILED)
    return FAILED;

  /* all the references have been decoded, now compute */
  if (compute_stack(sta, &result, NULL, NULL, NULL, NULL) == FAILED)
    return FAILED;

  k = (int)result;
  
  s = g_sec_bankhd_first;
  while (s != NULL && sta->section != s->id)
    s = s->next;

  /* the calculation was not in any bank header? */
  if (s == NULL) {
    fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: This calculation is marked to be in a section, but we cannot find the section. Skipping... Please send us a bug report about this!\n",
            get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
    return SUCCEEDED;
  }
  
  t = s->data + sta->address;

  if (sta->type == STACK_TYPE_8BIT) {
    if (k < -128 || k > 255) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;
  }
  else if (sta->type == STACK_TYPE_9BIT_SHORT) {
    if (k < 0 || k > 510) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 9-bit range.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    if ((k & 1) == 1) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: The RAM address needs to be even.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
      return FAILED;
    }
    *t = (k >> 1) & 0xFF;
  }
  else if (sta->type == STACK_TYPE_16BIT) {
    if (k < -32768 || k > 65535) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    if (get_file(sta->file_id)->little_endian == YES) {
      *t = k & 0xFF;
      t++;
      *t = (k >> 8) & 0xFF;
    }
    else {
      *t = (k >> 8) & 0xFF;
      t++;
      *t = k & 0xFF;
    }
  }
  else if (sta->type == STACK_TYPE_13BIT) {
    if (k < 0 || k > 8191) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 13-bit range.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;
    t++;
    *t = (*t & 0xE0) | ((k >> 8) & 0x1F);
  }
  else if (sta->type == STACK_TYPE_BITS) {
    int mask = 0, bits_position, bits_to_define, bits_byte;

    if (sta->bits_to_define < 32)
      mask = ~((1 << sta->bits_to_define) - 1);

    if ((k & mask) != 0) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: We are defining %d bits, but the given value $%x (%d) uses more bits!\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, sta->bits_to_define, k, k);
      return FAILED;
    }

    bits_position = sta->bits_position;
    bits_to_define = sta->bits_to_define;
    
    bits_byte = *t;
  
    while (bits_to_define > 0) {
      int bits_to_define_this_byte = 8 - bits_position;
      int bits;

      if (bits_to_define_this_byte > bits_to_define)
        bits_to_define_this_byte = bits_to_define;

      for (bits = 0; bits < bits_to_define_this_byte; bits++)
        bits_add_bit(&bits_byte, &bits_position, &bits_to_define, k);

      if (bits_position == 8) {
        bits_position = 0;
        *t = (unsigned char)bits_byte;
        t++;
        bits_byte = *t;
      }
    }

    if (bits_position > 0)
      *t = (unsigned char)bits_byte;
  }
  else if (sta->type == STACK_TYPE_24BIT) {
    if (k < -8388608 || k > 16777215) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 24-bit range.\n",
              get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    if (get_file(sta->file_id)->little_endian == YES) {
      *t = k & 0xFF;
      t++;
      *t = (k >> 8) & 0xFF;
      t++;
      *t = (k >> 16) & 0xFF;
    }
    else {
      *t = (k >> 16) & 0xFF;
      t++;
      *t = (k >> 8) & 0xFF;
      t++;
      *t = k & 0xFF;
    }
  }
  else if (sta->type == STACK_TYPE_32BIT) {
    if (get_file(sta->file_id)->little_endian == YES) {
      *t = k & 0xFF;
      t++;
      *t = (k >> 8) & 0xFF;
      t++;
      *t = (k >> 16) & 0xFF;
      t++;
      *t = (k >> 24) & 0xFF;
    }
    else {
      *t = (k >> 24) & 0xFF;
      t++;
      *t = (k >> 16) & 0xFF;
      t++;
      *t = (k >> 8) & 0xFF;
      t++;
      *t = k & 0xFF;
    }
  }
  else {
    fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Unsupported pending calculation type. Please send an error report!\n",
            get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
  }
  
  return SUCCEEDED;
}


int write_bank_header_references(struct reference *r) {

  struct section *s;
  struct label *l;
  unsigned char *t;
  int a;

  s = g_sec_bankhd_first;
  while (r->section != s->id)
    s = s->next;

  t = s->data + r->address;

  /* find the destination */
  find_label(r->name, s, &l);
  if (l != NULL) {
    a = (int)l->address;
    /* direct 16-bit */
    if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
      if (get_file(r->file_id)->little_endian == YES) {
        *t = a & 0xFF;
        t++;
        *t = (a >> 8) & 0xFF;
      }
      else {
        *t = (a >> 8) & 0xFF;
        t++;
        *t = a & 0xFF;
      }
    }
    /* direct 13-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_13BIT) {
      if (a > 8191 || a < 0) {
        fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: Value (%d/$%x) of \"%s\" is too much to be a 13-bit value.\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, a, a, l->name);
        return FAILED;
      }
      *t = a & 0xFF;
      t++;
      *t = (*t & 0xE0) | ((a >> 8) & 0x1F);
    }
    /* direct 8-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_8BIT) {
      if (a > 255 || a < -128) {
        fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: Value (%d/$%x) of \"%s\" is too much to be a 8-bit value.\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, a, a, l->name);
        return FAILED;
      }
      *t = a & 0xFF;
    }
    /* direct 9-bit short */
    else if (r->type == REFERENCE_TYPE_DIRECT_9BIT_SHORT) {
      if (a > 510 || a < 0) {
        fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: Value (%d/$%x) of \"%s\" is too much to be a 9-bit value.\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, a, a, l->name);
        return FAILED;
      }
      if ((a & 1) == 1) {
        fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: The RAM address needs to be even.\n",
                get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber);
        return FAILED;
      }
      *t = (a >> 1) & 0xFF;
    }
    /* direct 24-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
      if (l->status == LABEL_STATUS_LABEL)
        a += get_snes_pc_bank(l);
      if (get_file(r->file_id)->little_endian == YES) {
        *t = a & 0xFF;
        t++;
        *t = (a >> 8) & 0xFF;
        t++;
        *t = (a >> 16) & 0xFF;
      }
      else {
        *t = (a >> 16) & 0xFF;
        t++;
        *t = (a >> 8) & 0xFF;
        t++;
        *t = a & 0xFF;
      }
    }
    /* direct 32-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_32BIT) {
      if (get_file(r->file_id)->little_endian == YES) {
        *t = a & 0xFF;
        t++;
        *t = (a >> 8) & 0xFF;
        t++;
        *t = (a >> 16) & 0xFF;
        t++;
        *t = (a >> 24) & 0xFF;
      }
      else {
        *t = (a >> 24) & 0xFF;
        t++;
        *t = (a >> 16) & 0xFF;
        t++;
        *t = (a >> 8) & 0xFF;
        t++;
        *t = a & 0xFF;
      }
    }
    /* bits */
    else if (r->type == REFERENCE_TYPE_BITS) {
      int mask = 0, bits_position, bits_to_define, bits_byte;

      if (r->bits_to_define < 32)
        mask = ~((1 << r->bits_to_define) - 1);

      if ((a & mask) != 0) {
        fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: We are defining %d bits, but the given value $%x (%d) uses more bits!\n", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->bits_to_define, a, a);
        return FAILED;
      }

      bits_position = r->bits_position;
      bits_to_define = r->bits_to_define;
    
      bits_byte = *t;
  
      while (bits_to_define > 0) {
        int bits_to_define_this_byte = 8 - bits_position;
        int bits;

        if (bits_to_define_this_byte > bits_to_define)
          bits_to_define_this_byte = bits_to_define;

        for (bits = 0; bits < bits_to_define_this_byte; bits++)
          bits_add_bit(&bits_byte, &bits_position, &bits_to_define, a);

        if (bits_position == 8) {
          bits_position = 0;
          *t = (unsigned char)bits_byte;
          t++;
          bits_byte = *t;
        }
      }

      if (bits_position > 0)
        *t = (unsigned char)bits_byte;
    }
    else {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: A relative reference (type %d) to label \"%s\".\n",
              get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->type, l->name);
      return FAILED;
    }
  }
  else {
    fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_REFERENCES: Reference to an unknown label \"%s\".\n",
            get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);
    return FAILED;
  }

  return SUCCEEDED;
}


/* transform all string items inside a computation stack into corresponding numbers */
int parse_stack(struct stack *sta) {

  struct stack_item *si;
  struct section *s;
  struct label *l, lt;
  double k_ram, k_rom;
  int g, ed;

  if (sta->section_status != 0)
    s = find_section(sta->section);
  else
    s = NULL;

  /* calculate extra displacement (ed) depending on relative operand size:
     6809, 65816 and 65ce02 can have 16-bit relative operands so the start of
     next instruction is one byte farther away than "usual" */
  switch (sta->type) {
  case STACK_TYPE_8BIT:
  case STACK_TYPE_9BIT_SHORT:
    ed = 1;
    break;
  case STACK_TYPE_16BIT:
    /* NOTE: on 65ce02 the 16-bit relative references don't use the next
       instruction as the starting point, but one byte before it */
    if (get_file(sta->file_id)->cpu_65ce02 == YES)
      ed = 1;
    else
      ed = 2;
    break;
  case STACK_TYPE_24BIT: /* not presently used by any CPU arch supported */
    ed = 3;
    break;
  case STACK_TYPE_32BIT: /* not presently used by any CPU arch supported */
    ed = 4;
    break;
  default:
    ed = 1;
    break;
  }

  si = sta->stack_items;
  g = 0;
  k_ram = 0.0;
  k_rom = 0.0;
  while (g != sta->stacksize) {
    if (si->type == STACK_ITEM_TYPE_LABEL) {
      l = NULL;

      /* bank number search */
      if (si->string[0] == ':') {
        fprintf(stderr, "%s: %s:%d: PARSE_STACK: It should be impossible for a string in the stack calculator to begin with ':', but here we are with the label \"%s\"... Internal error!\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, si->string);
        return FAILED;
        /*
          if (is_label_anonymous(&si->string[1]) == YES) {
          l = get_closest_anonymous_label(&si->string[1], sta->address, sta->file_id, sta->section_status, sta->section);
          if (l != NULL)
          k = l->bank;
          }
          else if (strcaselesscmp(&si->string[1], "CADDR") == 0) {
          k = sta->bank + sta->base;
          lt.status = LABEL_STATUS_DEFINE;
          strcpy(lt.name, &si->string[1]);
          lt.address = sta->address;
          lt.bank = sta->bank;
          lt.base = sta->base;
          lt.section_status = OFF;
          l = &lt;
          }
          else {
          find_label(&si->string[1], s, &l);

          if (l != NULL) {
          if (get_file(sta->file_id)->cpu_65816 == YES)
          k = get_snes_pc_bank(l) >> 16;
          else
          k = l->base + l->bank;
          }
          }
        */
      }
      /* normal label address search */
      else {
        if (is_label_anonymous(si->string) == YES) {
          l = get_closest_anonymous_label(si->string, NULL, sta->address, sta->file_id, sta->section_status, sta->section);
          if (l != NULL) {
            k_rom = l->rom_address;
            k_ram = l->address;
          }

          /* is the reference relative? */
          if (sta->relative_references == YES) {
            k_rom = k_rom - sta->address - ed;
            k_ram = k_ram - sta->memory_address - ed;
          }
        }
        else if (strcaselesscmp(si->string, "CADDR") == 0) {
          if (sta->position == STACK_POSITION_DEFINITION) {
            k_rom = s_current_stack_calculation_addr;
            k_ram = s_current_stack_calculation_addr;
            fprintf(stderr, "%s: %s:%d: PARSE_STACK: We have a CADDR inside a calculation inside a definition. Please check that the result is correct...\n", get_file_name(sta->file_id),
                    get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          }
          else {
            k_rom = sta->address;
            k_ram = sta->memory_address;
          }
          lt.status = LABEL_STATUS_DEFINE;
          strcpy(lt.name, si->string);
          lt.address = sta->address;
          lt.bank = sta->bank;
          lt.slot = sta->slot;
          lt.base = sta->base;
          lt.section_status = OFF;
          l = &lt;
        }
        else {
          find_label(si->string, s, &l);

          /* find matches until something else than a label is found */
          while (l != NULL && l->status == LABEL_STATUS_LABEL) {
            struct label *label_old = l;
            
            find_label(l->string, s, &l);

            if (l == NULL) {
              l = label_old;
              break;
            }
            if (l->status != LABEL_STATUS_LABEL)
              break;
          }
          
          if (l != NULL) {
            if (l->status == LABEL_STATUS_STRING) {
              strcpy(si->string, l->string);
              si->type = STACK_ITEM_TYPE_STRING;
            }
            else {
              k_rom = l->rom_address;
              k_ram = l->address;

              /* is the reference relative? */
              if (sta->relative_references == YES) {
                k_rom = k_rom - sta->address - ed;
                k_ram = k_ram - sta->memory_address - ed;
              }
            }
          }
        }
      }

      if (l == NULL) {
        fprintf(stderr, "%s: %s:%d: PARSE_STACK: Unresolved reference to \"%s\".\n", get_file_name(sta->file_id),
                get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, si->string);
        return FAILED;
      }

      /* 65816 cpu bank fix */
      if (sta->type == STACK_TYPE_24BIT && l->status == LABEL_STATUS_LABEL) {
        k_rom += get_snes_pc_bank(l);
        k_ram += get_snes_pc_bank(l);
      }

      si->slot = l->slot;
      si->base = l->base;
      si->bank = l->bank;

      /*
        fprintf(stdout, "%s: %s:%d: %s %x %d\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber,
        si->string, (int)k, sta->relative_references);
      */

      if (l->status == LABEL_STATUS_STACK) {
        /* HACK: here we abuse the stack item structure's members */
        si->value_ram = l->address;
        si->value_rom = l->address;
        si->stack_file_id = l->file_id;
        si->type = STACK_ITEM_TYPE_STACK;
      }
      else {
        si->value_rom = k_rom;
        si->value_ram = k_ram;
      }
    }

    si++;
    g++;
  }

  return SUCCEEDED;
}


int get_snes_pc_bank(struct label *l) {

  int x, k;

  /* TODO: clean up this mess */

  /* do we override the user's banking scheme (.HIROM/.LOROM/.EXHIROM/.EXLOROM)? */
  if (g_snes_mode != 0) {
    if (l->section_status == ON && l->section_struct != NULL && (l->section_struct->status == SECTION_STATUS_RAM_FREE ||
                                                                 l->section_struct->status == SECTION_STATUS_RAM_SEMIFREE ||
                                                                 l->section_struct->status == SECTION_STATUS_RAM_SEMISUBFREE ||
                                                                 l->section_struct->status == SECTION_STATUS_RAM_FORCE)) {
      /* on SNES RAMSECTION labels are handled differently */
      x = l->bank;
    }
    else {
      /* use rom_address instead of address, as address points to
         the position in destination machine's memory, not in rom */
      k = (int)l->rom_address;

      if (g_snes_rom_mode == SNES_ROM_MODE_HIROM || g_snes_rom_mode == SNES_ROM_MODE_EXHIROM)
        x = k / 0x10000;
      else
        x = k / 0x8000;
    }
  }
  /* or just use the user's banking chart */
  else {
    x = l->bank;
  }

  x = (x + l->base) << 16;

  return x;
}


int correct_65816_library_sections(void) {

  struct section *s;
  struct label *l;

  s = g_sec_first;
  while (s != NULL) {
    if (s->library_status == ON && s->base_defined == YES) {
      l = g_labels_first;
      while (l != NULL) {
        if (l->section_status == ON && l->section == s->id)
          l->base = s->base;
        l = l->next;
      }
    }
    s = s->next;
  }

  return SUCCEEDED;
}


/* is the label of form -, --, ---, +, ++, +++, ... ? */
int is_label_anonymous(char *label) {

  int length, i;
  char c;

  length = (int)strlen(label);
  if (length >= 2 && label[0] == '_') {
    c = label[1];
    
    if (c == 'f' || c == 'F' || c == 'b' || c == 'B' || c == '_') {
      if (length == 2)
        return YES;
      if (length > 2 && label[2] == ':')
        return YES;
    }
  }
  
  c = *label;
  if (!(c == '-' || c == '+'))
    return NO;
  
  for (i = 1; i < length; i++) {
    if (*(label + i) == c)
      continue;
    if (*(label + i) == ':')
      return YES;
    
    return NO;
  }

  return YES;
}


static int _labels_compare(const void *a, const void *b) {

  const struct label *l1 = a;
  const struct label *l2 = b;

  if (l1->section_status == OFF && l2->section_status == ON)
    return 1;
  if (l1->section_status == ON && l2->section_status == OFF)
    return -1;

  if (l1->section > l2->section)
    return 1;
  else if (l1->section < l2->section)
    return -1;

  if (l1->rom_address > l2->rom_address)
    return 1;
  else if (l1->rom_address < l2->rom_address)
    return -1;

  if (l1->linenumber > l2->linenumber)
    return 1;
  else if (l1->linenumber < l2->linenumber)
    return -1;

  if (strlen(l1->name) < strlen(l2->name))
    return -1;
  else if (strlen(l1->name) > strlen(l2->name))
    return 1;

  return -1;
}


static int _labels_sort(const void *a, const void *b) {

  return _labels_compare(*(void**)a, *(void**)b);
}


int sort_anonymous_labels(void) {

  int j = 0;
  struct label *l;

  g_num_sorted_anonymous_labels = 0;

  /* count # of anonymous labels */
  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES && is_label_anonymous(l->name) == YES)
      g_num_sorted_anonymous_labels++;
    l = l->next;
  }

  if (g_num_sorted_anonymous_labels == 0)
    return SUCCEEDED;

  g_sorted_anonymous_labels = calloc(sizeof(struct label *) * g_num_sorted_anonymous_labels, 1);
  if (g_sorted_anonymous_labels == NULL) {
    fprintf(stderr, "SORT_ANONYMOUS_LABELS: Out of memory error.\n");
    return FAILED;
  }

  /* load anonymous labels */
  l = g_labels_first;
  while (l != NULL) {
    if (l->alive == YES && is_label_anonymous(l->name) == YES)
      g_sorted_anonymous_labels[j++] = l;
    l = l->next;
  }

  qsort(g_sorted_anonymous_labels, g_num_sorted_anonymous_labels, sizeof(struct label *), _labels_sort);

  return SUCCEEDED;
}


/* sort_anonymous_labels must be called before this. though currently, this doesn't take advantage
   of the fact that they're sorted. */
struct label *get_closest_anonymous_label(char *name, char *context, int rom_address, int file_id, int section_status, int section) {

  char context_name[MAX_NAME_LENGTH + 1];
  struct label *l;
  struct label *closest = NULL;
  int d = 999999999, e;
  int j;

  if (context == NULL) {
    /* get the context from the label */
    context_name[0] = 0;
    parse_context_from_name(name, context_name);
    context = context_name;
  }
  
  j = 0;

  if (strcmp(name, "_b") == 0 || strcmp(name, "_B") == 0) {
    while (j < g_num_sorted_anonymous_labels) {
      l = g_sorted_anonymous_labels[j];
      if (strcmp("__", l->name) == 0 && strcmp(context, l->context) == 0 && file_id == l->file_id && section_status == l->section_status) {
        if (section_status == OFF || (section_status == ON && section == l->section)) {
          e = rom_address - (int)l->rom_address;
          if (e >= 0 && e < d) {
            closest = l;
            d = e;
          }
        }
      }
      j++;
    }
    return closest;
  }

  if (strcmp(name, "_f") == 0 || strcmp(name, "_F") == 0) {
    while (j < g_num_sorted_anonymous_labels) {
      l = g_sorted_anonymous_labels[j];
      if (strcmp("__", l->name) == 0 && strcmp(context, l->context) == 0 && file_id == l->file_id && section_status == l->section_status) {
        if (section_status == OFF || (section_status == ON && section == l->section)) {
          e = (int)l->rom_address - rom_address;
          if (e > 0 && e < d) {
            closest = l;
            d = e;
          }
        }
      }
      j++;
    }
    return closest;
  }

  j = 0;
  /* -, --, +, ++, ... */
  while (j < g_num_sorted_anonymous_labels) {
    l = g_sorted_anonymous_labels[j];
    if (strcmp(name, l->name) == 0 && strcmp(context, l->context) == 0 && file_id == l->file_id && section_status == l->section_status) {
      if (section_status == OFF || (section_status == ON && section == l->section)) {
        if (name[0] == '-') {
          e = rom_address - (int)l->rom_address;
          if (e >= 0 && e < d) {
            closest = l;
            d = e;
          }
        }
        else {
          e = (int)l->rom_address - rom_address;
          if (e > 0 && e < d) {
            closest = l;
            d = e;
          }
        }
      }
    }
    j++;
  }

  return closest;
}


int is_label_ok_for_sizeof(char *label) {

  if (is_label_anonymous(label) == YES)
    return NO;

  if (strncmp("SECTIONSTART", label, 12) == 0)
    return NO;
  if (strncmp("SECTIONEND", label, 10) == 0)
    return NO;
  
  return YES;
}


static int _create_ram_bank_usage_label(int bank, int slot, char *slot_name, const char *start_or_end, int address) {

  struct label *l;

  /* create symbols */
  l = calloc(1, sizeof(struct label));
  if (l == NULL) {
    fprintf(stderr, "CREATE_RAM_BANK_USAGE_SYMBOL: Out of memory error.\n");
    return FAILED;
  }
  
  snprintf(l->name, sizeof(l->name), "RAM_USAGE_SLOT_%s_BANK_%d_%s", slot_name, bank, start_or_end);

  l->status = LABEL_STATUS_LABEL;
  l->alive = YES;
  l->address = address;
  l->rom_address = address;
  l->base = 0;
  l->bank = bank;
  l->slot = slot;
  l->file_id = -1;

  l->section_status = OFF;
  l->section_struct = NULL;
  l->section = -1;

  if (insert_label_into_maps(l, 1) == FAILED)
    return FAILED;

  add_label(l);

  return SUCCEEDED;
}


/* generate RAM bank usage labels (RAM_USAGE_SLOT_x_BANK_y_START + RAM_USAGE_SLOT_x_BANK_y_END) */
int generate_ram_bank_usage_labels(void) {

  int bank, slot, min, max, i;
  char *slot_usage_data, slot_name[MAX_NAME_LENGTH+1];

  for (bank = 0; bank < 256; bank++) {
    if (g_ram_slots[bank] == NULL)
      continue;

    for (slot = 0; slot < 256; slot++) {
      slot_usage_data = g_ram_slots[bank][slot];
      if (slot_usage_data == NULL)
        continue;

      min = -1;
      max = -1;

      /* find min and max used addresses of the bank/slot */
      for (i = 0; i < g_slots[slot].size; i++) {
        if (slot_usage_data[i] > 0) {
          if (min < 0)
            min = i;
          max = i;
        }
      }

      /* not used? */
      if (min < 0)
        continue;

      /* get slot name */
      if (g_slots[slot].name[0] != 0)
        snprintf(slot_name, sizeof(slot_name), "%s", g_slots[slot].name);
      else
        snprintf(slot_name, sizeof(slot_name), "%d", slot);

      if (_create_ram_bank_usage_label(bank, slot, slot_name, "START", min + g_slots[slot].address) == FAILED)
        return FAILED;
      if (_create_ram_bank_usage_label(bank, slot, slot_name, "END", max + g_slots[slot].address) == FAILED)
        return FAILED;
    }
  }
  
  return SUCCEEDED;
}


int generate_sizeof_label_definitions(void) {

  struct label *l, *lastL, **labels;
  int labelsN = 0, j;
  double size = 0.0;

  if (g_labels_first == NULL)
    return SUCCEEDED;

  /* generate _sizeof_[label] definitions */
  l = g_labels_first;
  lastL = NULL;
  while (l != NULL) {
    /* skip anonymous labels & child labels */
    if (l->status == LABEL_STATUS_LABEL && is_label_ok_for_sizeof(l->name) == YES && l->alive == YES &&
        (lastL == NULL || !(strncmp(lastL->name, l->name, strlen(lastL->name)) == 0 && l->name[strlen(lastL->name)] == '@'))) {
      labelsN++;
      lastL = l;
    }

    l = l->next;
  }

  if (labelsN <= 1)
    return SUCCEEDED;

  labels = calloc(sizeof(struct label *) * labelsN, 1);
  if (labels == NULL) {
    fprintf(stderr, "GENERATE_SIZEOF_LABEL_DEFINITIONS: Out of memory error.\n");
    return FAILED;
  }

  j = 0;
  l = g_labels_first;
  lastL = NULL;
  while (l != NULL) {
    /* skip anonymous labels & child labels */
    if (l->status == LABEL_STATUS_LABEL && is_label_ok_for_sizeof(l->name) == YES && l->alive == YES &&
        (lastL == NULL || !(strncmp(lastL->name, l->name, strlen(lastL->name)) == 0 && l->name[strlen(lastL->name)] == '@'))) {
      labels[j++] = l;
      lastL = l;
    }

    l = l->next;
  }

  /* sort the labels by address, smallest first */
  qsort(labels, labelsN, sizeof(struct label *), _labels_sort);

  /*
    for (j = 0; j < labelsN; j++) {
    fprintf(stderr, "LABEL: %s:%d section=%d\n", labels[j]->name, labels[j]->rom_address, labels[j]->section);
    }
  */

  /* process the labels */
  for (j = 0; j < labelsN; j++) {
    struct label_sizeof *ls = g_label_sizeofs;
    char c1 = labels[j]->name[0];

    /* try to find the size in "label sizeofs"... */
    while (ls != NULL) {
      if (labels[j]->file_id == ls->file_id && c1 == ls->name[0] && strcmp(labels[j]->name, ls->name) == 0) {
        size = ls->size;
        break;
      }
      ls = ls->next;
    }

    if (ls == NULL) {
      if (j == labelsN - 1 || labels[j]->section != labels[j+1]->section) {
        /* last label in this section */
        if (labels[j]->section_struct != NULL)
          size = labels[j]->section_struct->size - labels[j]->address_in_section;
        else
          continue;
      }
      else {
        size = (int)labels[j+1]->rom_address - (int)labels[j]->rom_address;
      }
    }

    l = calloc(1, sizeof(struct label));
    if (l == NULL) {
      fprintf(stderr, "GENERATE_SIZEOF_LABEL_DEFINITIONS: Out of memory error.\n");
      free(labels);
      return FAILED;
    }

    if (strlen(labels[j]->name)+8+1 >= sizeof(l->name)) {
      fprintf(stderr, "GENERATE_SIZEOF_LABEL_DEFINITIONS: Expanded label name \"_sizeof_%s\" is %d characters too large.\n",
              labels[j]->name, (int)(strlen(labels[j]->name)+8+1-sizeof(l->name)));
      free(labels);
      return FAILED;
    }
    else
      snprintf(l->name, sizeof(l->name), "_sizeof_%.*s", (int)(sizeof(l->name)-8-1), labels[j]->name);

    l->status = LABEL_STATUS_DEFINE;
    l->alive = YES;
    l->address = size;
    l->rom_address = size;
    l->base = 0;
    l->file_id = labels[j]->file_id;

    l->section_status = labels[j]->section_status;
    l->section_struct = labels[j]->section_struct;
    l->section        = labels[j]->section;

    if (insert_label_into_maps(l, 1) == FAILED) {
      free(labels);
      return FAILED;
    }

    add_label(l);
  }

  free(labels);

  return SUCCEEDED;
}


int fix_sectionstartend_labels(void) {

  struct label *l1, *l2;
  int count;
  char tmp[MAX_NAME_LENGTH * 2];

  if (g_labels_first == NULL)
    return SUCCEEDED;

  /* generate _sizeof_[label] definitions */
  l1 = g_labels_first;
  while (l1 != NULL) {
    if (strncmp("SECTIONSTART", l1->name, 12) == 0 || strncmp("SECTIONEND", l1->name, 10) == 0) {
      l2 = l1->next;
      count = 1;
      while (l2 != NULL) {
        if (strcmp(l1->name, l2->name) == 0) {
          /* l2 has the same name as l1! change l2's name... */
          snprintf(tmp, sizeof(tmp), "%s_%d", l2->name, count);
          count++;
          if (strlen(tmp) < MAX_NAME_LENGTH)
            strcpy(l2->name, tmp);
          else
            fprintf(stderr, "FIX_SECTIONSTARTEND_LABELS: Duplicate label \"%s\" is too long to be renamed.\n", l2->name);           
        }
        l2 = l2->next;
      }
    }

    l1 = l1->next;
  }  
  
  return SUCCEEDED;
}


int get_slot_by_its_name(char *name, int *slot) {

  char c1;
  int i;
  
  if (name == NULL || slot == NULL)
    return FAILED;

  c1 = name[0];
  
  for (i = 0; i < 256; i++) {
    if (g_slots[i].usage == ON) {
      if (c1 == g_slots[i].name[0] && strcmp(g_slots[i].name, name) == 0) {
        *slot = i;
        return SUCCEEDED;
      }
    }
  }

  fprintf(stderr, "GET_SLOT_BY_ITS_NAME: Could not find SLOT \"%s\".\n", name);

  return FAILED;
}


int get_slot_by_a_value(int value, int *slot) {

  int i;

  if (slot == NULL)
    return FAILED;

  if (value < 0) {
    *slot = value;
    fprintf(stderr, "GET_SLOT_BY_A_VALUE: Internal error.\n");
    return FAILED;
  }
  
  /* value can be the direct SLOT ID, but can it be a SLOT's address as well? */
  if (value < 256) {
    for (i = 0; i < 256; i++) {
      if (g_slots[i].usage == ON && g_slots[i].address == value && value != i && g_slots[value].usage == ON) {
        fprintf(stderr, "GET_SLOT_BY_A_VALUE: There is a SLOT number %d, but there also is a SLOT (with ID %d) with starting address %d ($%x)... Using SLOT %d.\n", value, i, value, value, value);
        *slot = value;
        return SUCCEEDED;
      }
    }

    *slot = value;
    return SUCCEEDED;
  }

  for (i = 0; i < 256; i++) {
    if (g_slots[i].usage == ON && g_slots[i].address == value) {
      *slot = i;
      return SUCCEEDED;
    }
  }

  *slot = -1;

  fprintf(stderr, "GET_SLOT_BY_A_VALUE: Cannot find SLOT %d.\n", value);

  return FAILED;
}

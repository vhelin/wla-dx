
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "write.h"
#include "files.h"
#include "analyze.h"

#ifdef AMIGA
#include "/crc32.h"
#include "/printf.h"
#else
#include "../crc32.h"
#include "../printf.h"
#endif


extern struct section_fix *sec_fix_first, *sec_fix_tmp;
extern struct reference *reference_first, *reference_last;
extern struct label *labels_first, *labels_last;
extern struct label **sorted_anonymous_labels;
extern struct object_file *obj_first, *obj_last, *obj_tmp;
extern struct section *sec_first, *sec_last, *sec_bankhd_first, sec_bankhd_last;
extern struct stack *stacks_first, *stacks_last;
extern struct map_t *global_unique_label_map;
extern struct map_t *namespace_map;
extern struct slot slots[256];
extern struct label_sizeof *label_sizeofs;
extern unsigned char *rom, *rom_usage;
extern unsigned char *file_header, *file_footer;
extern char mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern char load_address_label[MAX_NAME_LENGTH + 1];
extern char program_address_start_label[MAX_NAME_LENGTH + 1], program_address_end_label[MAX_NAME_LENGTH + 1];
extern int load_address, load_address_type;
extern int romsize, rombanks, banksize, verbose_mode, section_overwrite, symbol_mode;
extern int pc_bank, pc_full, pc_slot, pc_slot_max, snes_rom_mode;
extern int file_header_size, file_footer_size, *bankaddress, *banksizes;
extern int memory_file_id, memory_file_id_source, memory_line_number, output_mode;
extern int program_start, program_end, snes_mode, smc_status;
extern int snes_sramsize, num_sorted_anonymous_labels, sort_sections;
extern int output_type, program_address_start, program_address_end, program_address_start_type, program_address_end_type;

int current_stack_calculation_addr = 0;



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

  if (load_address_type == LOAD_ADDRESS_TYPE_VALUE) {
    address = load_address;
    fprintf(stderr, "Using the address $%x as the load address for the PRG.\n", address & 0xFFFF);
  }
  else if (load_address_type == LOAD_ADDRESS_TYPE_LABEL) {
    /* find the address of the label */
    struct label *l;
    
    find_label(load_address_label, NULL, &l);

    if (l == NULL) {
      fprintf(stderr, "_CBM_WRITE_PRG_HEADER: Cannot find label \"%s\".\n", load_address_label);
      return FAILED;
    }

    if (l->status != LABEL_STATUS_LABEL || (l->section_struct != NULL && (l->section_struct->status == SECTION_STATUS_RAM_FREE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_FORCE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_SEMIFREE ||
                                                                          l->section_struct->status == SECTION_STATUS_RAM_SEMISUBFREE ||
                                                                          l->section_struct->alive == NO))) {
      fprintf(stderr, "_CBM_WRITE_PRG_HEADER: \"%s\" cannot be used as the load address.\n", load_address_label);
      return FAILED;
    }

    address = (int)l->address;

    fprintf(stderr, "Using the address $%x (of label \"%s\") as the load address for the PRG.\n", address & 0xFFFF, l->name);
  }
  else if (load_address_type == LOAD_ADDRESS_TYPE_UNDEFINED) {
    /* find a suitable load address, i.e., the label with the smallest address value */
    struct label *l = labels_first, *label = NULL;
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

  if (output_mode != OUTPUT_ROM)
    return FAILED;

  i = romsize/(8*1024);

  /* low byte of 8KB page count */
  fprintf(f, "%c", i & 0xFF);
  /* high byte of 8KB page count */
  fprintf(f, "%c", (i>>8) & 0xFF);

  /* emulation mode select (?) */
  i = 0;
  if (snes_rom_mode == SNES_ROM_MODE_HIROM || snes_rom_mode == SNES_ROM_MODE_EXHIROM)
    i |= (1<<5) | (1<<4);
  i |= (snes_sramsize ^ 3) << 2;

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


int insert_sections(void) {

  struct section *s, **sa;
  int d, f, i, x, t, q, sn, p;
  char **ram_slots[256], *c;


  /* initialize ram slots */
  for (i = 0; i < 256; i++)
    ram_slots[i] = NULL;

  /* find all touched slots */
  s = sec_first;
  while (s != NULL) {
    if (s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_FORCE || s->status == SECTION_STATUS_RAM_SEMIFREE || s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
      if (ram_slots[s->bank] == NULL) {
        ram_slots[s->bank] = calloc(sizeof(char *) * 256, 1);
        if (ram_slots[s->bank] == NULL) {
          fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
          return FAILED;
        }
        for (i = 0; i < 256; i++)
          ram_slots[s->bank][i] = NULL;
      }
      if (ram_slots[s->bank][s->slot] == NULL) {
        ram_slots[s->bank][s->slot] = calloc(slots[s->slot].size, 1);
        if (ram_slots[s->bank][s->slot] == NULL) {
          fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
          return FAILED;
        }
        memset(ram_slots[s->bank][s->slot], 0, slots[s->slot].size);
      }
    }
    s = s->next;
  }

  /* count the sections */
  i = 0;
  s = sec_first;
  while (s != NULL) {
    if (s->alive == YES)
      i++;
    s = s->next;
  }
  sn = i;

  if (sn == 0)
    return SUCCEEDED;

  sa = calloc(sizeof(struct section *) * sn, 1);
  if (sa == NULL) {
    fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
    return FAILED;
  }

  /* insert the sections into an array for sorting */
  i = 0;
  s = sec_first;
  while (s != NULL) {
    /* no references - skip it */
    if (s->alive == YES)
      sa[i++] = s;
    s = s->next;
  }

  /* sort the sections by priority first and then by size, biggest first */
  if (sort_sections == YES)
    qsort(sa, sn, sizeof(struct section *), _sections_sort);

  /* print the sizes (DEBUG) */
  /*
    for (d = 0; d < i; d++)
    fprintf(stderr, "SIZE: %d\n", sa[d]->size);
  */

  /* ram sections */

  /* RAM FORCE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];

    if (s->status == SECTION_STATUS_RAM_FORCE) {
      int slotAddress = slots[s->slot].address;

      /* align the starting address */
      int overflow = (slotAddress + s->address) % s->alignment;
      int address = s->address;

      address += overflow;
      address += s->offset;

      c = ram_slots[s->bank][s->slot];
      i = slots[s->slot].size;
      for (q = 0; address + q < i && q < s->size; q++) {
        if (c[address + q] != 0) {
          fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in slot %d.\n", s->name, s->size, s->slot);
          free(sa);
          return FAILED;
        }
      }

      s->address = address;
      s->output_address = address;

      /* mark as used */
      for (i = 0; i < s->size; i++, address++)
        c[address] = 1;
    }
  }

  /* RAM SEMISUBFREE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    
    if (s->status == SECTION_STATUS_RAM_SEMISUBFREE) {
      int slotAddress = slots[s->slot].address;

      /* align the starting address */
      int overflow = slotAddress % s->alignment;
      int address = 0;
      int offset = s->offset;

      address += overflow;

      c = ram_slots[s->bank][s->slot];
      i = s->address;
      t = 0;
      for (; address < i; address += s->alignment) {
        if (c[address] == 0) {
          for (q = 0; address + offset + q < i && q < s->size; q++) {
            if (c[address + offset + q] != 0)
              break;
          }
          if (q == s->size) {
            t = 1;
            break;
          }
        }
      }

      if (t == 0) {
        fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in slot %d.\n", s->name, s->size, s->slot);
        free(sa);
        return FAILED;
      }

      address += offset;
      s->address = address;
      s->output_address = address;

      /* mark as used */
      for (i = 0; i < s->size; i++, address++)
        c[address] = 1;
    }
  }
  
  /* RAM FREE & RAM SEMIFREE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    
    if (s->status == SECTION_STATUS_RAM_FREE || s->status == SECTION_STATUS_RAM_SEMIFREE) {
      int slotAddress = slots[s->slot].address;

      /* align the starting address */
      int overflow = (slotAddress + s->address) % s->alignment;
      int address = s->address;
      int offset = s->offset;

      address += overflow;

      c = ram_slots[s->bank][s->slot];
      i = slots[s->slot].size;
      t = 0;
      for (; address < i; address += s->alignment) {
        if (c[address] == 0) {
          for (q = 0; address + offset + q < i && q < s->size; q++) {
            if (c[address + offset + q] != 0)
              break;
          }
          if (q == s->size) {
            t = 1;
            break;
          }
        }
      }

      if (t == 0) {
        fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in slot %d.\n", s->name, s->size, s->slot);
        free(sa);
        return FAILED;
      }

      address += offset;
      s->address = address;
      s->output_address = address;

      /* mark as used */
      for (i = 0; i < s->size; i++, address++)
        c[address] = 1;
    }
  }

  /* free tmp memory */
  for (i = 0; i < 256; i++) {
    if (ram_slots[i] != NULL) {
      for (p = 0; p < 256; p++) {
        if (ram_slots[i][p] != NULL) {
          free(ram_slots[i][p]);
          ram_slots[i][p] = NULL;
        }
      }
      free(ram_slots[i]);
      ram_slots[i] = NULL;
    }
  }

  /* FORCE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_FORCE) {
      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = s->address;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      d = pc_full;
      i = d + s->size;
      s->output_address = d;
      section_overwrite = OFF;
      if (i > romsize) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
        return FAILED;
      }
      if (s->address + s->size > banksize) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
        return FAILED;
      }
      for (; d < i; d++) {
        if (rom_usage[d] != 0 && rom[d] != s->data[d - pc_full])
          break;
      }

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      if (d == i) {
        for (i = 0; i < s->size; i++) {
          if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
            return FAILED;
        }
      }
      else {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
        free(sa);
        return FAILED;
      }
    }
  }

  /* ABSOLUTE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_ABSOLUTE) {
      d = s->address;
      s->output_address = d;
      section_overwrite = ON;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
        if (mem_insert(d + i, s->data[i]) == FAILED)
          return FAILED;
      }
    }
  }

  /* SEMISUBFREE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_SEMISUBFREE) {
      pc_bank = 0;
      d = bankaddress[s->bank];

      /* align the starting address */
      f = (pc_bank + d) % s->alignment;
      if (f > 0)
        pc_bank += s->alignment - f;

      i = FAILED;
      while (i == FAILED) {
        f = pc_bank;
        for (x = 0; pc_bank + s->offset < s->address && rom_usage[pc_bank + s->offset + d] == 0 && x < s->size; pc_bank++, x++)
          ;
        if (x == s->size)
          break;
        if (pc_bank == s->address) {
          fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
                  get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
          return FAILED;
        }

        /* find the next starting address */
        f = (pc_bank + d) % s->alignment;
        if (f > 0)
          pc_bank += s->alignment - f;
        for (; pc_bank + s->offset < s->address && rom_usage[pc_bank + s->offset + d] != 0; pc_bank += s->alignment)
          ;
      }

      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = f + s->offset;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      s->address = pc_bank;
      s->output_address = pc_full;
      section_overwrite = OFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
        if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
          return FAILED;
      }
    }
  }

  /* FREE & SEMIFREE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_FREE || s->status == SECTION_STATUS_SEMIFREE) {
      pc_bank = s->address;
      d = bankaddress[s->bank];

      /* align the starting address */
      f = (pc_bank + d) % s->alignment;
      if (f > 0)
        pc_bank += s->alignment - f;

      i = FAILED;
      while (i == FAILED) {
        f = pc_bank;
        for (x = 0; pc_bank + s->offset < banksizes[s->bank] && rom_usage[pc_bank + s->offset + d] == 0 && x < s->size; pc_bank++, x++)
          ;
        if (x == s->size)
          break;
        if (pc_bank == banksizes[s->bank]) {
          fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
                  get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
          return FAILED;
        }

        /* find the next starting address */
        f = (pc_bank + d) % s->alignment;
        if (f > 0)
          pc_bank += s->alignment - f;
        for (; pc_bank + s->offset < banksizes[s->bank] && rom_usage[pc_bank + s->offset + d] != 0; pc_bank += s->alignment)
          ;
      }

      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = f + s->offset;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      s->address = pc_bank;
      s->output_address = pc_full;
      section_overwrite = OFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
        if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
          return FAILED;
      }
    }
  }

  /* SUPERFREE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_SUPERFREE) {
      /* go through all the banks */
      i = FAILED;
      f = 0;

      for (q = 0; i == FAILED && q < rombanks; q++) {
        pc_bank = 0;
        d = bankaddress[q];

        /* align the starting address */
        f = (pc_bank + d) % s->alignment;
        if (f > 0)
          pc_bank += s->alignment - f;

        /* if the slotsize and banksize differ -> try the next bank */
        if (banksizes[q] != slots[s->slot].size)
          continue;

        while (i == FAILED) {
          f = pc_bank;
          for (x = 0; pc_bank + s->offset < banksizes[q] && rom_usage[pc_bank + s->offset + d] == 0 && x < s->size; pc_bank++, x++)
            ;
          if (x == s->size) {
            i = SUCCEEDED;
            break;
          }
          if (pc_bank == banksizes[q])
            break;

          /* find the next starting address */
          f = (pc_bank + d) % s->alignment;
          if (f > 0)
            pc_bank += s->alignment - f;
          for (; pc_bank + s->offset < banksizes[s->bank] && rom_usage[pc_bank + s->offset + d] != 0; pc_bank += s->alignment)
            ;
        }
      }

      if (i == SUCCEEDED) {
        s->bank = q-1;
        memory_file_id = s->file_id;
        banksize = banksizes[s->bank];
        pc_bank = f + s->offset;
        pc_slot = pc_bank;
        pc_full = pc_bank + bankaddress[s->bank];
        pc_slot_max = slots[s->slot].size;
        s->address = pc_bank;
        s->output_address = pc_full;
        section_overwrite = OFF;

        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

        for (i = 0; i < s->size; i++)
          if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
            return FAILED;
      }
      else {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
        return FAILED;
      }
    }
  }

  /* OVERWRITE sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_OVERWRITE) {
      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = s->address;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      s->output_address = pc_full;
      section_overwrite = ON;
      if (pc_full + s->size > romsize) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
        return FAILED;
      }
      if (s->address + s->size > banksize) {
        fprintf(stderr, "%s: %s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
                get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
        return FAILED;
      }

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
        if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
          return FAILED;
      }
    }
  }

  free(sa);

  return SUCCEEDED;
}


/* transform computation stack definitions to ordinary definitions */
int transform_stack_definitions(void) {

  struct label *l;
  struct stack *s;


  l = labels_first;
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

      s = stacks_first;
      /* find the stack associated with the definition */
      while (s != NULL) {
        if (s->file_id == l->file_id && s->id == l->address)
          break;
        s = s->next;
      }
      /* did we find it? */
      if (s == NULL) {
        fprintf(stderr, "TRANSFORM_STACK_DEFINITIONS: No computation stack associated with computation definition label \"%s\". This is a fatal internal error. Please send the WLA DX author a bug report.\n", l->name);
        return FAILED;
      }
      /* is it ok? */
      if (s->computed == 0) {
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


int try_put_label(map_t map, struct label *l) {

  int err;


  if (hashmap_get(map, l->name, NULL) == MAP_OK) {
    if (l->status == LABEL_STATUS_DEFINE)
      fprintf(stderr, "%s: TRY_PUT_LABEL: Definition \"%s\" was defined more than once.\n", get_file_name(l->file_id), l->name);
    else
      fprintf(stderr, "%s: %s:%d: TRY_PUT_LABEL: Label \"%s\" was defined more than once.\n", get_file_name(l->file_id),
              get_source_file_name(l->file_id, l->file_id_source), l->linenumber, l->name);
    return FAILED;
  }
  if ((err = hashmap_put(map, l->name, l)) != MAP_OK) {
    fprintf(stderr, "TRY_PUT_LABEL: Hashmap error %d. Please send a bug report!\n", err);
    return FAILED;
  }

  return SUCCEEDED;
}


/* check that all RAM sections inside libraries are given a bank and a slot */
int check_ramsections(void) {

  struct section *s;


  s = sec_first;
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

  
  sec_fix_tmp = sec_fix_first;
  while (sec_fix_tmp != NULL) {
    /* find the section, and fix bank, slot and org/orga */
    s = sec_first;
    while (s != NULL) {
      if (strcmp(s->name, sec_fix_tmp->name) == 0) {
        s->bank = sec_fix_tmp->bank;

        if (sec_fix_tmp->slot < 0) {
          if (get_slot_by_its_name(sec_fix_tmp->slot_name, &(s->slot)) == FAILED)
            return FAILED;
        }
        else {
          if (get_slot_by_a_value(sec_fix_tmp->slot, &(s->slot)) == FAILED)
            return FAILED;
        }

        if (sec_fix_tmp->status >= 0)
          s->status = sec_fix_tmp->status;

        if (sec_fix_tmp->priority_defined == YES)
          s->priority = sec_fix_tmp->priority;

        if (sec_fix_tmp->keep == YES)
          s->keep = YES;

        if (sec_fix_tmp->alignment >= 0)
          s->alignment = sec_fix_tmp->alignment;

        if (sec_fix_tmp->offset >= 0)
          s->offset = sec_fix_tmp->offset;
        
        if (sec_fix_tmp->orga >= 0) {
          if (sec_fix_tmp->orga < slots[s->slot].address || sec_fix_tmp->orga >= slots[s->slot].address + slots[s->slot].size) {
            fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: ORGA $%.4x is outside of the SLOT %d.\n", sec_fix_tmp->file_name, sec_fix_tmp->line_number, sec_fix_tmp->orga, s->slot);
            return FAILED;
          }
          s->address = sec_fix_tmp->orga - slots[s->slot].address;
        }
        if (sec_fix_tmp->org >= 0)
          s->address = sec_fix_tmp->org;
        
        break;
      }
      s = s->next;
    }

    if (s == NULL) {
      fprintf(stderr, "%s:%d: FIX_ALL_SECTIONS: Could not find ", sec_fix_tmp->file_name, sec_fix_tmp->line_number);
      if (sec_fix_tmp->is_ramsection == YES)
        fprintf(stderr, "RAM section");
      else
        fprintf(stderr, "section");
      fprintf(stderr, " \"%s\".\n", sec_fix_tmp->name);
      return FAILED;
    }

    sec_fix_tmp = sec_fix_tmp->next;
  }

  return SUCCEEDED;
}


/* determines the section for each label, and calls "insert_label_into_maps" for each. */
int fix_label_sections(void) {

  struct section *s;
  struct label *l;

  l = labels_first;
  while (l != NULL) {
    if (l->section_status == ON) {
      /* search for the label's section */
      s = sec_first;
      while (s != NULL) {
        if (s->id == l->section) {
          l->section_struct = s;
          break;
        }
        s = s->next;
      }

      if (s == NULL) {
        fprintf(stderr, "FIX_LABEL_SECTIONS: Internal error: couldn't find section %d for label \"%s\".\n",
                l->section, l->name);
        return FAILED;
      }
    }

    insert_label_into_maps(l, 0);

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
      if (try_put_label(s->label_map, l) == FAILED)
        return FAILED;

      if (base_name[0] == '_')
        put_in_global = 0;

      /* put label into section's namespace's label map, if it's not a local label */
      if (s->nspace != NULL && base_name[0] != '_') {
        if (try_put_label(s->nspace->label_map, l) == FAILED)
          return FAILED;
        put_in_global = 0;
      }
    }
  }

  /* put the label into the global namespace */
  if (put_in_anything && put_in_global) {
    if (try_put_label(global_unique_label_map, l) == FAILED)
      return FAILED;
  }

  return SUCCEEDED;
}


int fix_label_addresses(void) {

  struct section *s;
  struct label *l;


  /* fix labels' addresses */
  l = labels_first;
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
              l->rom_address = (int)l->address + slots[l->slot].size * l->bank;
            else
              l->rom_address = (int)l->address + bankaddress[l->bank];

            if (s->status != SECTION_STATUS_ABSOLUTE)
              l->address += slots[l->slot].address;
          }
          else {
            fprintf(stderr, "FIX_LABELS: Internal error: label's section ID and its sections section ID don't match!\n");
          }
        }
        else {
          l->address_in_section = (int)l->address;
          l->rom_address = (int)l->address + bankaddress[l->bank];
          l->address += slots[l->slot].address;
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
  else {
    fprintf(stderr, "_HANDLE_SPECIAL_CASE: Unknown special case ID %d! This in an internal WLA error. Please submit a bug report!\n", special_id);
    return FAILED;
  }

  return SUCCEEDED;
}


int fix_references(void) {

  struct reference *r;
  struct section *s;
  struct label *l, lt;
  int i, x;


  section_overwrite = OFF;

  /* insert references */
  r = reference_first;
  while (r != NULL) {
    s = NULL;

    x = r->address;
    /* search for the section of the reference and fix the address */
    if (r->section_status == ON) {
      s = sec_first;
      while (s != NULL) {
        if (s->id == r->section) {
          r->bank = s->bank;
          x += s->address;
          r->address += s->address;
          break;
        }
        s = s->next;
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
      x += bankaddress[r->bank];
      r->address += slots[r->slot].address;
    }

    /* find the destination */
    l = NULL;

    /* request for bank number? */
    if (r->name[0] == ':') {
      if (is_label_anonymous(&r->name[1]) == YES)
        l = get_closest_anonymous_label(&r->name[1], x, r->file_id, r->section_status, r->section);
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

      memory_file_id = r->file_id;
      memory_file_id_source = r->file_id_source;
      memory_line_number = r->linenumber;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT || r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
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
      /* relative/direct 8-bit with a label */
      else {
        mem_insert_ref(x, i & 0xFF);
      }
    }
    /* normal reference */
    else {
      if (is_label_anonymous(r->name) == YES)
        l = get_closest_anonymous_label(r->name, x, r->file_id, r->section_status, r->section);
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

      memory_file_id = r->file_id;
      memory_file_id_source = r->file_id_source;
      memory_line_number = r->linenumber;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
        i = (int)l->address;
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
      else {
        i = ((int)l->address) & 0xFFFF;
        if (i > 255) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 8-bit value.\n",
                  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }

        /* special case ID handling! */
        if (r->special_id > 0) {
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


int write_symbol_file(char *outname, unsigned char mode, unsigned char outputAddrToLine) {

  struct source_file_name *src_file;
  struct object_file *obj_file;
  struct section *s;
  struct label *l;
  char name[256], list_cmd, *outfile_tmp;
  FILE *f, *outfile;
  int list_cmd_idx, list_source_file, list_address_offset, y, outfile_size;
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
  name[y++] = 0;

  f = fopen(name, "wb");
  if (f == NULL) {
    fprintf(stderr, "WRITE_SYMBOL_FILE: Error opening file \"%s\" for writing.\n", name);
    return FAILED;
  }

  fprintf(f, "; this file was created with wlalink by ville helin <vhelin@iki.fi>.\n");

  if (mode == SYMBOL_MODE_NOCA5H) {
    /* NO$GMB SYMBOL FILE */
    if (snes_mode == 0)
      fprintf(f, "; no$gmb symbolic information for \"%s\".\n", outname);
    else
      fprintf(f, "; no$snes symbolic information for \"%s\".\n", outname);

    l = labels_first;
    while (l != NULL) {
      if (l->alive == NO || is_label_anonymous(l->name) == YES || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT || l->status == LABEL_STATUS_DEFINE || l->status == LABEL_STATUS_STACK) {
        l = l->next;
        continue;
      }
      /* skip all dropped section labels */
      if (l->section_status == ON) {
        s = sec_first;
        while (l->section != s->id)
          s = s->next;
        if (s->alive == NO) {
          l = l->next;
          continue;
        }
      }
      if (snes_mode == 0) {
        if (l->status == LABEL_STATUS_LABEL)
          fprintf(f, "%.2x:%.4x %s\n", l->base + l->bank, (int)l->address, l->name);
        else
          fprintf(f, "00:%.4x %s\n", (int)l->address, l->name);
      }
      else {
        if (l->status == LABEL_STATUS_LABEL)
          fprintf(f, "%.4x%.4x %s\n", get_snes_pc_bank(l) >> 16, (int)l->address, l->name);
        else
          fprintf(f, "%.8x %s\n", (int)l->address, l->name);
      }
      l = l->next;
    }
  }
  else {
    /* WLA SYMBOL FILE */
    fprintf(f, "; wla symbolic information for \"%s\".\n", outname);

    /* labels */
    l = labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_LABEL) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[labels]\n");

      l = labels_first;
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
          s = sec_first;
          while (l->section != s->id)
            s = s->next;
          if (s->alive == NO) {
            l = l->next;
            continue;
          }
        }

        if (snes_mode == 0)
          fprintf(f, "%.2x:%.4x %s\n", l->base + l->bank, (int)l->address, l->name);
        else
          fprintf(f, "%.2x:%.4x %s\n", get_snes_pc_bank(l)>>16, (int)l->address, l->name);

        l = l->next;
      }
    }

    /* symbols */
    l = labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_SYMBOL) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[symbols]\n");

      l = labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_SYMBOL) {
          l = l->next;
          continue;
        }

        if (snes_mode == 0)
          fprintf(f, "%.2x:%.4x %s\n", l->bank, (int)l->address, l->name);
        else
          fprintf(f, "%.2x:%.4x %s\n", get_snes_pc_bank(l)>>16, (int)l->address, l->name);

        l = l->next;
      }
    }

    /* breakpoints */
    l = labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_BREAKPOINT) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[breakpoints]\n");

      l = labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_BREAKPOINT) {
          l = l->next;
          continue;
        }

        if (snes_mode == 0)
          fprintf(f, "%.2x:%.4x\n", l->bank, (int)l->address);
        else
          fprintf(f, "%.2x:%.4x\n", get_snes_pc_bank(l)>>16, (int)l->address);

        l = l->next;
      }
    }

    /* definitions */
    l = labels_first;
    while (l != NULL) {
      if (l->status != LABEL_STATUS_DEFINE) {
        l = l->next;
        continue;
      }
      break;
    }

    if (l != NULL) {
      fprintf(f, "\n[definitions]\n");

      l = labels_first;
      while (l != NULL) {
        if (l->alive == NO || l->status != LABEL_STATUS_DEFINE) {
          l = l->next;
          continue;
        }
        if (is_label_anonymous(l->name) == YES) {
          l = l->next;
          continue;
        }

        fprintf(f, "%.8lx %s\n", (long unsigned int)l->address, l->name);

        l = l->next;
      }
    }

    if (outputAddrToLine == ON) {
      /* file_id_source to source files */
      fprintf(f, "\n[source files]\n");
      obj_file = obj_first;
      while (obj_file != NULL) {
        src_file = obj_file->source_file_names_list;
        while (src_file != NULL) {
          fprintf(f, "%.4x %.8lx %s \n", src_file->id, src_file->checksum, src_file->name);
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
      fread(outfile_tmp, 1, outfile_size, outfile);
      fclose(outfile);
      outfile_crc = crc32((unsigned char*)outfile_tmp, outfile_size);
      free(outfile_tmp);
      fprintf(f, "\n[rom checksum]\n%.8lx\n", outfile_crc);

      /* addr -> file/line mappings */
      s = sec_first;
      while (s != NULL) {
        if (s->listfile_items > 0) {
          fprintf(f, "\n[addr-to-line mapping]\n");
          break;
        }
        s = s->next;
      }

      s = sec_first;
      while (s != NULL) {
        /* parse the list file information */
        list_address_offset = 0;
        list_source_file = -1;
        for (list_cmd_idx = 0; list_cmd_idx < s->listfile_items; list_cmd_idx++) {
          list_cmd = s->listfile_cmds[list_cmd_idx];
          if (list_cmd == 'k') {
            /* new line */
            if (s->listfile_ints[list_cmd_idx * 3 + 1] > 0) {
              fprintf(f, "%.2x:%.4x %.4x:%.8lx\n", s->bank + s->base, (s->output_address + list_address_offset) & 0xFFFF, list_source_file, (long unsigned int)s->listfile_ints[list_cmd_idx * 3 + 0]);
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
  if (program_address_start_type == LOAD_ADDRESS_TYPE_LABEL) {
    if (_get_rom_address_of_label(program_address_start_label, &program_address_start) == FAILED)
      return FAILED;
  }
  if (program_address_end_type == LOAD_ADDRESS_TYPE_LABEL) {
    if (_get_rom_address_of_label(program_address_end_label, &program_address_end) == FAILED)
      return FAILED;
  }

  if (program_address_start > romsize) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) overflows from the ROM!\n", program_address_start);
    return FAILED;
  }
  if (program_address_end > romsize) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bE ($%x) overflows from the ROM!\n", program_address_end);
    return FAILED;
  }
  if (program_address_start >= 0 && program_address_end >= 0 && program_address_start > program_address_end) {
    fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) is larger than -bE ($%x).\n", program_address_start, program_address_end);
    return FAILED;
  }

  f = fopen(outname, "wb");
  if (f == NULL) {
    fprintf(stderr, "WRITE_ROM_FILE: Error opening file \"%s\" for writing.\n", outname);
    return FAILED;
  }

  if (file_header != NULL)
    fwrite(file_header, 1, file_header_size, f);

  /* SMC header */
  if (smc_status != 0)
    _smc_create_and_write(f);

  /* CBM PRG */
  if (output_type == OUTPUT_TYPE_CBM_PRG) {
    if (_cbm_write_prg_header(f) == FAILED) {
      fclose(f);
      return FAILED;
    }
  }
  
  /* ROM output mode */
  if (output_mode == OUTPUT_ROM) {
    /* write bank by bank and bank header sections */
    for (i = 0; i < rombanks; i++) {
      s = sec_bankhd_first;
      while (s != NULL) {
        if (s->bank == i) {
          fwrite(s->data, 1, s->size, f);
          break;
        }
        s = s->next;
      }

      fwrite(rom + bankaddress[i], 1, banksizes[i], f);
    }
  }
  /* program file output mode */
  else {
    for (i = 0; i < romsize; i++) {
      if (rom_usage[i] != 0)
        break;
    }
    b = i;
    for (e = b; i < romsize; i++) {
      if (rom_usage[i] != 0)
        e = i;
    }

    /* overrides from the options to WLALINK */
    if (program_address_start >= 0)
      b = program_address_start;
    if (program_address_end >= 0)
      e = program_address_end;

    s = sec_bankhd_first;
    while (s != NULL) {
      if (s->bank == 0) {
        fwrite(s->data, 1, s->size, f);
        break;
      }
      s = s->next;
    }

    fwrite(rom + b, 1, e - b + 1, f);
    program_start = b;
    program_end = e;

    if (program_address_start >= 0 && program_address_end < 0 && b > e) {
      fprintf(stderr, "WRITE_ROM_FILE: The supplied -bS ($%x) is larger than calculated end ($%x).\n", b, e);
      return FAILED;
    }

    fprintf(stderr, "Program start $%x, end $%x.\n", b, e);
  }

  if (file_footer != NULL)
    fwrite(file_footer, 1, file_footer_size, f);

  fclose(f);

  return SUCCEEDED;
}


int compute_pending_calculations(void) {

  struct section *s;
  struct stack *sta;
  int k, a;


  section_overwrite = ON;

  /* first place the calculation stacks into the output */
  sta = stacks_first;
  while (sta != NULL) {
    if (sta->position == STACK_POSITION_DEFINITION) {
      /* skip definition stacks */
      sta = sta->next;
      continue;
    }

    if (sta->section_status == ON) {
      /* get section address */
      s = sec_first;
      while (s != NULL) {
        if (sta->section == s->id) {
          sta->bank = s->bank;
          break;
        }
        s = s->next;
      }
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
      sta->memory_address = s->address + sta->address + slots[sta->slot].address;

      if (s->status != SECTION_STATUS_ABSOLUTE)
        sta->address += s->address + bankaddress[s->bank];
      else
        sta->address += s->address;
    }
    else {
      /* remember the memory address (for CADDR) */
      sta->memory_address = sta->address + slots[sta->slot].address;

      sta->address += bankaddress[sta->bank];
    }

    sta = sta->next;
  }

  /* next parse the stack items */
  sta = stacks_first;
  while (sta != NULL) {
    if (sta->position == STACK_POSITION_DEFINITION)
      k = 1;
    else {
      /* skip the calculations inside discarded sections */
      if (sta->section_status == ON) {
        /* get the section */
        s = sec_first;
        while (s != NULL) {
          if (sta->section == s->id) {
            break;
          }
          s = s->next;
        }
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
  sta = stacks_first;
  while (sta != NULL) {
    /* is the stack inside a definition? */
    if (sta->position == STACK_POSITION_DEFINITION) {
      /* all the references have been decoded, now compute */
      if (compute_stack(sta, NULL, NULL, NULL, NULL) == FAILED)
        return FAILED;
      /* next stack computation */
      sta = sta->next;
      continue;
    }

    /* find source address */
    if (sta->section_status == ON) {
      /* get section address */
      s = sec_first;
      while (s != NULL) {
        if (sta->section == s->id) {
          sta->bank = s->bank;
          break;
        }
        s = s->next;
      }
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
    current_stack_calculation_addr = sta->memory_address;

    /* all the references have been decoded, now compute */
    if (compute_stack(sta, &k, NULL, NULL, NULL) == FAILED)
      return FAILED;

    memory_file_id = sta->file_id;
    memory_file_id_source = sta->file_id_source;
    memory_line_number = sta->linenumber;

    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(mem_insert_action, sizeof(mem_insert_action), "Writing pending calculation %s: %s:%d.", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);

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
    else if (sta->type == STACK_TYPE_16BIT) {
      if (k < -32768 || k > 65535) {
        fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
                get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
        return FAILED;
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
      if (address >= start_address && address < start_address + banksize)
        return j;
      start_address += banksize;
      j++;
      if (j > 1000000000) {
        fprintf(stderr, "_GET_BANK_OF_ADDRESS: j > 1000000000! Internal error. Cannot find the BANK. Please submit a bug report.\n");
        return -1;
      }
    }

    return -1;
  }

  /* TODO: check if we can just use the banksize in every case and ignore slots[slot].size completely */
  
  slot_size = slots[slot].size;;
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


struct stack *find_stack(int id, int file_id) {

  struct stack *st = stacks_first;

  while (st != NULL) {
    if (st->id == id && st->file_id == file_id)
      return st;
    st = st->next;
  }

  return NULL;
}


static void _pass_on_slot(int *slot, int t, struct stack *sta) {

  if (slot[t - 2] < 0 && slot[t - 1] >= 0)
    slot[t - 2] = slot[t - 1];
  else if (slot[t - 2] >= 0 && slot[t - 1] >= 0) {
    /* sanity check */
    if (slot[t - 2] != slot[t - 1]) {
      fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: The passed on SLOT changed from $%x to $%x. This might have no effect, but just to let you know. Please check that the result of this calculation is correct.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, slot[t - 2], slot[t - 1]);
    }
    slot[t - 2] = slot[t - 1];
  }
}


static void _pass_on_base(int *base, int t, struct stack *sta) {

  if (base[t - 2] < 0 && base[t - 1] >= 0)
    base[t - 2] = base[t - 1];
  else if (base[t - 2] >= 0 && base[t - 1] >= 0) {
    /* sanity check */
    if (base[t - 2] != base[t - 1]) {
      fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: The passed on BASE changed from $%x to $%x. This might have no effect, but just to let you know. Please check that the result of this calculation is correct.\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, base[t - 2], base[t - 1]);
    }
    base[t - 2] = base[t - 1];
  }
}


int compute_stack(struct stack *sta, int *result_ram, int *result_rom, int *result_slot, int *result_base) {

  struct stack_item *s;
  struct stack *st;
  int r, t, z, y, x, res_ram, res_rom, res_base, res_slot, slot[256], base[256];
  double v_ram[256], v_rom[256], q;


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
  s = sta->stack;
  for (r = 0, t = 0; r < x; r++, s++) {
    if (s->type == STACK_ITEM_TYPE_VALUE) {
      /* RAM */
      if (s->sign == SI_SIGN_NEGATIVE)
        v_ram[t] = -s->value_ram;
      else
        v_ram[t] = s->value_ram;
      /* ROM */
      if (s->sign == SI_SIGN_NEGATIVE)
        v_rom[t] = -s->value_rom;
      else
        v_rom[t] = s->value_rom;
      slot[t] = s->slot;
      base[t] = s->base;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STRING) {
      /* parse_stack() turned this string into a value */
      v_ram[t] = s->value_ram;
      v_rom[t] = s->value_rom;
      slot[t] = s->slot;
      base[t] = s->base;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STACK) {
      /* we have a stack (A) inside a stack (B)! find the stack (A)! */
      st = find_stack((int)s->value_ram, sta->file_id);

      if (st == NULL) {
        fprintf(stderr, "COMPUTE_STACK: A computation stack has gone missing. This is a fatal internal error. Please send the WLA DX author a bug report.\n");
        return FAILED;
      }

      if (st->position == STACK_POSITION_DEFINITION) {
        /* we'll need to do this as "st" might contain CADDR */
        if (parse_stack(st) == FAILED)
          return FAILED;
      }
      if (compute_stack(st, &res_ram, &res_rom, &res_slot, &res_base) == FAILED)
        return FAILED;

      v_ram[t] = res_ram;
      v_rom[t] = res_rom;
      slot[t] = res_slot;
      base[t] = res_base;
      t++;
    }
    else {
      switch ((int)s->value_ram) {
      case SI_OP_PLUS:
        v_ram[t - 2] += v_ram[t - 1];
        v_rom[t - 2] += v_rom[t - 1];
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
        t--;
        break;
      case SI_OP_MINUS:
        v_ram[t - 2] -= v_ram[t - 1];
        v_rom[t - 2] -= v_rom[t - 1];
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
        t--;
        break;
      case SI_OP_NOT:
        if (sta->type == STACK_TYPE_8BIT)
          y = 0xFF;
        else if (sta->type == STACK_TYPE_13BIT)
          y = 8191;
        else if (sta->type == STACK_TYPE_16BIT)
          y = 0xFFFF;
        else if (sta->type == STACK_TYPE_24BIT)
          y = 0xFFFFFF;
        else {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: NOT cannot determine the output size.\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
          return FAILED;
        }
        v_ram[t - 1] = (int)v_ram[t - 1] ^ y;
        v_rom[t - 1] = (int)v_rom[t - 1] ^ y;
        break;
      case SI_OP_XOR:
        v_ram[t - 2] = (int)v_ram[t - 1] ^ (int)v_ram[t - 2];
        v_rom[t - 2] = (int)v_rom[t - 1] ^ (int)v_rom[t - 2];
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
        t--;
        break;
      case SI_OP_BANK:
        z = (int)v_rom[t - 1];
        y = _get_bank_of_address(z, slot[t - 1]);
        if (y < 0) {
          fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Could not get the bank number for ROM address %d/$%x (out of bounds).\n", get_file_name(sta->file_id),
                  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, z, z);
          return FAILED;
        }
        if (base[t - 1] >= 0)
          y += base[t - 1];
        v_ram[t - 1] = y & 0xFF;
        v_rom[t - 1] = y & 0xFF;
        break;
      case SI_OP_LOW_BYTE:
        v_ram[t - 1] = ((int)v_ram[t - 1]) & 0xFF;
        v_rom[t - 1] = ((int)v_rom[t - 1]) & 0xFF;
        break;
      case SI_OP_HIGH_BYTE:
        v_ram[t - 1] = (((int)v_ram[t - 1]) >> 8) & 0xFF;
        v_rom[t - 1] = (((int)v_rom[t - 1]) >> 8) & 0xFF;
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
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
        _pass_on_slot(slot, t, sta);
        _pass_on_base(base, t, sta);
        t--;
        break;
      }
    }
  }

  if (result_ram != NULL)
    *result_ram = (int)v_ram[0];
  if (result_rom != NULL)
    *result_rom = (int)v_rom[0];
  if (result_slot != NULL)
    *result_slot = sta->result_slot;
  if (result_base != NULL)
    *result_base = sta->result_base;

  sta->result_ram = (int)v_ram[0];
  sta->result_rom = (int)v_rom[0];
  sta->result_slot = (int)slot[0];
  sta->result_base = (int)base[0];
  
  sta->computed = YES;
  sta->under_work = NO;

  /*
    printf("RESULT: %d (RAM) %d (ROM)\n", sta->result_ram, sta->result_rom);
  */

  return SUCCEEDED;
}


int write_bank_header_calculations(struct stack *sta) {

  struct section *s;
  unsigned char *t;
  int k;


  /* parse stack items */
  if (parse_stack(sta) == FAILED)
    return FAILED;

  /* all the references have been decoded, now compute */
  if (compute_stack(sta, &k, NULL, NULL, NULL) == FAILED)
    return FAILED;

  s = sec_bankhd_first;
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
  else {
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

  return SUCCEEDED;
}


int write_bank_header_references(struct reference *r) {

  struct section *s;
  struct label *l;
  unsigned char *t;
  int a;


  s = sec_bankhd_first;
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


  s = NULL;
  if (sta->section_status != 0) {
    s = sec_first;
    while (s != NULL) {
      if (s->id == sta->section)
        break;
      s = s->next;
    }
  }

  /* calculate extra displacement (ed) depending on relative operand size:
     6809, 65816 and 65ce02 can have 16-bit relative operands so the start of
     next instruction is one byte farther away than "usual" */
  switch (sta->type) {
  case STACK_TYPE_8BIT:
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
  default:
    ed = 1;
    break;
  }

  si = sta->stack;
  g = 0;
  k_ram = 0.0;
  k_rom = 0.0;
  while (g != sta->stacksize) {
    if (si->type == STACK_ITEM_TYPE_STRING) {
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
          l = get_closest_anonymous_label(si->string, sta->address, sta->file_id, sta->section_status, sta->section);
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
            k_rom = current_stack_calculation_addr;
            k_ram = current_stack_calculation_addr;
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

          if (l != NULL) {
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

      /*
        fprintf(stdout, "%s: %s:%d: %s %x %d\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber,
        si->string, (int)k, sta->relative_references);
      */

      if (l->status == LABEL_STATUS_STACK) {
        /* HACK: here we abuse the stack item structure's members */
        si->value_ram = l->address;
        si->value_rom = l->address;
        si->sign = l->file_id;
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
  if (snes_mode != 0) {
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
      k = l->rom_address;

      if (snes_rom_mode == SNES_ROM_MODE_HIROM || snes_rom_mode == SNES_ROM_MODE_EXHIROM)
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


  s = sec_first;
  while (s != NULL) {
    if (s->library_status == ON && s->base_defined == YES) {
      l = labels_first;
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


  if (strcmp(label, "_f") == 0 || strcmp(label, "_F") == 0 || strcmp(label, "_b") == 0 || strcmp(label, "_B") == 0 || strcmp(label, "__") == 0)
    return YES;

  c = *label;
  if (!(c == '-' || c == '+'))
    return NO;
  length = (int)strlen(label);
  for (i = 0; i < length; i++) {
    if (*(label + i) != c)
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


int sort_anonymous_labels() {

  int j = 0;
  struct label *l;


  num_sorted_anonymous_labels = 0;

  /* count # of anonymous labels */
  l = labels_first;
  while (l != NULL) {
    if (l->alive == YES && is_label_anonymous(l->name) == YES)
      num_sorted_anonymous_labels++;
    l = l->next;
  }

  if (num_sorted_anonymous_labels == 0)
    return SUCCEEDED;

  sorted_anonymous_labels = calloc(sizeof(struct label *) * num_sorted_anonymous_labels, 1);
  if (sorted_anonymous_labels == NULL) {
    fprintf(stderr, "SORT_ANONYMOUS_LABELS: Out of memory error.\n");
    return FAILED;
  }

  /* load anonymous labels */
  l = labels_first;
  while (l != NULL) {
    if (l->alive == YES && is_label_anonymous(l->name) == YES)
      sorted_anonymous_labels[j++] = l;
    l = l->next;
  }

  qsort(sorted_anonymous_labels, num_sorted_anonymous_labels, sizeof(struct label *), _labels_sort);

  return SUCCEEDED;
}


/* sort_anonymous_labels must be called before this. */
/* Though currently, this doesn't take advantage of the fact that they're sorted. */
struct label *get_closest_anonymous_label(char *name, int rom_address, int file_id, int section_status, int section) {

  struct label *l;
  struct label *closest = NULL;
  int d = 999999999, e;
  int j;


  j = 0;

  if (strcmp(name, "_b") == 0 || strcmp(name, "_B") == 0) {
    while (j < num_sorted_anonymous_labels) {
      l = sorted_anonymous_labels[j];
      if (strcmp("__", l->name) == 0 && file_id == l->file_id && section_status == l->section_status) {
        if (section_status == OFF || (section_status == ON && section == l->section)) {
          e = rom_address - l->rom_address;
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
    while (j < num_sorted_anonymous_labels) {
      l = sorted_anonymous_labels[j];
      if (strcmp("__", l->name) == 0 && file_id == l->file_id && section_status == l->section_status) {
        if (section_status == OFF || (section_status == ON && section == l->section)) {
          e = l->rom_address - rom_address;
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
  while (j < num_sorted_anonymous_labels) {
    l = sorted_anonymous_labels[j];
    if (strcmp(name, l->name) == 0 && file_id == l->file_id && section_status == l->section_status) {
      if (section_status == OFF || (section_status == ON && section == l->section)) {
        if (name[0] == '-') {
          e = rom_address - l->rom_address;
          if (e >= 0 && e < d) {
            closest = l;
            d = e;
          }
        }
        else {
          e = l->rom_address - rom_address;
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


int generate_sizeof_label_definitions(void) {

  struct label *l, *lastL, **labels;
  int labelsN = 0, j;
  double size = 0.0;


  if (labels_first == NULL)
    return SUCCEEDED;

  /* generate _sizeof_[label] definitions */
  l = labels_first;
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
  l = labels_first;
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
    struct label_sizeof *ls = label_sizeofs;

    /* try to find the size in "label sizeofs"... */
    while (ls != NULL) {
      if (labels[j]->file_id == ls->file_id && strcmp(labels[j]->name, ls->name) == 0) {
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
        size = labels[j+1]->rom_address - labels[j]->rom_address;
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
    l->rom_address = (int)size;
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


  if (labels_first == NULL)
    return SUCCEEDED;

  /* generate _sizeof_[label] definitions */
  l1 = labels_first;
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

  int i;
  
  if (name == NULL || slot == NULL)
    return FAILED;

  for (i = 0; i < 256; i++) {
    if (slots[i].usage == ON) {
      if (strcmp(slots[i].name, name) == 0) {
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
      if (slots[i].usage == ON && slots[i].address == value && value != i && slots[value].usage == ON) {
        fprintf(stderr, "GET_SLOT_BY_A_VALUE: There is a SLOT number %d, but there also is a SLOT (with ID %d) with starting address %d ($%x)... Using SLOT %d.\n", value, i, value, value, value);
        *slot = value;
        return SUCCEEDED;
      }
    }

    *slot = value;
    return SUCCEEDED;
  }

  for (i = 0; i < 256; i++) {
    if (slots[i].usage == ON && slots[i].address == value) {
      *slot = i;
      return SUCCEEDED;
    }
  }

  *slot = -1;

  fprintf(stderr, "GET_SLOT_BY_A_VALUE: Cannot find SLOT %d.\n", value);

  return FAILED;
}

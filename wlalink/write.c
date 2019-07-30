
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
#else
#include "../crc32.h"
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
extern int romsize, rombanks, banksize, verbose_mode, section_overwrite, symbol_mode;
extern int pc_bank, pc_full, pc_slot, pc_slot_max, snes_rom_mode;
extern int file_header_size, file_footer_size, *bankaddress, *banksizes;
extern int memory_file_id, memory_file_id_source, memory_line_number, output_mode;
extern int program_start, program_end, cpu_65816, snes_mode, smc_status;
extern int snes_sramsize, num_sorted_anonymous_labels, little_endian;


static int _sections_sort(const void *a, const void *b) {

  if ((*((struct section **)a))->priority < (*((struct section **)b))->priority)
    return 1;
  else if ((*((struct section **)a))->priority > (*((struct section **)b))->priority)
    return -1;
  
  if ((*((struct section **)a))->size < (*((struct section **)b))->size)
    return 1;

  return -1;
}


int smc_create_and_write(FILE *f) {

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
    if (s->status == SECTION_STATUS_RAM) {
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

  /* sort the sections by size, biggest first */
  qsort(sa, sn, sizeof(struct section *), _sections_sort);

  /* print the sizes (DEBUG) */
  /*
    for (d = 0; d < i; d++)
    fprintf(stderr, "SIZE: %d\n", sa[d]->size);
  */

  /* ram sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];

    /* search for free space */
    if (s->status == SECTION_STATUS_RAM) {
      int slotAddress = slots[s->slot].address;

      /* align the starting address */
      int address = slotAddress % s->alignment;
      if (address != 0)
	address = s->alignment - address;

      c = ram_slots[s->bank][s->slot];
      i = slots[s->slot].size;
      t = 0;
      for (; address < i; address += s->alignment) {
	if (c[address] == 0) {
	  for (q = 0; address + q < i && q < s->size; q++) {
	    if (c[address + q] != 0) {
	      address += q;
	      break;
	    }
	  }
	  if (q == s->size) {
	    t = 1;
	    break;
	  }
	}
      }

      if (t == 0) {
	fprintf(stderr, "INSERT_SECTIONS: No room for RAMSECTION \"%s\" (%d bytes) in slot %d.\n", s->name, s->size, s->slot);
	return FAILED;
      }

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

  /* force sections */
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
      sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      if (d == i) {
	for (i = 0; i < s->size; i++) {
	  if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
	    return FAILED;
	}
      }
      else {
	fprintf(stderr, "%s: %s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
		get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
	return FAILED;
      }
    }
  }

  /* absolute sections */
  p = 0;
  while (p < sn) {
    s = sa[p++];
    if (s->status == SECTION_STATUS_ABSOLUTE) {
      d = s->address;
      s->output_address = d;
      section_overwrite = ON;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
	if (mem_insert(d + i, s->data[i]) == FAILED)
	  return FAILED;
      }
    }
  }

  /* semisubfree sections */
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
	for (x = 0; pc_bank < s->address && rom_usage[pc_bank + d] == 0 && x < s->size; pc_bank++, x++)
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
        for (; pc_bank < s->address && rom_usage[pc_bank + d] != 0; pc_bank += s->alignment)
	  ;
      }

      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = f;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      s->address = pc_bank;
      s->output_address = pc_full;
      section_overwrite = OFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
	if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
	  return FAILED;
      }
    }
  }

  /* free & semifree sections */
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
	for (x = 0; pc_bank < banksizes[s->bank] && rom_usage[pc_bank + d] == 0 && x < s->size; pc_bank++, x++)
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
        for (; pc_bank < banksizes[s->bank] && rom_usage[pc_bank + d] != 0; pc_bank += s->alignment)
	  ;
      }

      memory_file_id = s->file_id;
      banksize = banksizes[s->bank];
      pc_bank = f;
      pc_slot = slots[s->slot].address + pc_bank;
      pc_full = pc_bank + bankaddress[s->bank];
      pc_slot_max = slots[s->slot].address + slots[s->slot].size;
      s->address = pc_bank;
      s->output_address = pc_full;
      section_overwrite = OFF;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

      for (i = 0; i < s->size; i++) {
	if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
	  return FAILED;
      }
    }
  }

  /* superfree sections */
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
	  for (x = 0; pc_bank < banksizes[q] && rom_usage[pc_bank + d] == 0 && x < s->size; pc_bank++, x++)
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
	  for (; pc_bank < banksizes[s->bank] && rom_usage[pc_bank + d] != 0; pc_bank += s->alignment)
	    ;
	}
      }

      if (i == SUCCEEDED) {
	s->bank = q-1;
	memory_file_id = s->file_id;
	banksize = banksizes[s->bank];
	pc_bank = f;
	pc_slot = pc_bank;
	pc_full = pc_bank + bankaddress[s->bank];
	pc_slot_max = slots[s->slot].size;
	s->address = pc_bank;
	s->output_address = pc_full;
	section_overwrite = OFF;

	/* create a what-we-are-doing message for mem_insert*() warnings/errors */
	sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

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

  /* overwrite sections */
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
      sprintf(mem_insert_action, "Writing section %s: %s: %s.", get_file_name(s->file_id), get_source_file_name(s->file_id, s->file_id_source), s->name);

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
      l->address = s->result;
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


/* fix the slot and bank of RAM sections inside libraries, as given in the linkfile */
int fix_ramsections(void) {

  struct section *s;

  
  sec_fix_tmp = sec_fix_first;
  while (sec_fix_tmp != NULL) {
    /* find the section, and fix bank and slot */
    s = sec_first;
    while (s != NULL) {
      if (strcmp(s->name, sec_fix_tmp->name) == 0) {
	s->bank = sec_fix_tmp->bank;
	s->slot = sec_fix_tmp->slot;
	break;
      }
      s = s->next;
    }

    if (s == NULL) {
      fprintf(stderr, "%s:%d: LOAD_FILES: Could not find RAM section \"%s\".\n", sec_fix_tmp->file_name, sec_fix_tmp->line_number, sec_fix_tmp->name);
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

  if (l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT
      || is_label_anonymous(base_name) == YES) {
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
	    fprintf(stderr, "FIX_LABELS: Internal error: section_struct is NULL.\n");
	    return FAILED;
	  }
	  s = l->section_struct;
	  if (s->id == l->section) {
	    l->bank = s->bank;
	    l->slot = s->slot;
	    l->address_in_section = (int)l->address;
	    l->address += s->address;

	    if (s->status == SECTION_STATUS_RAM)
	      l->rom_address = (int)l->address;
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
      else if (strcmp(&r->name[1], "CADDR") == 0 || strcmp(&r->name[1], "caddr") == 0) {
        lt.status = LABEL_STATUS_LABEL;
        strcpy(lt.name, &r->name[1]);
        lt.address = r->address;
        lt.bank = r->bank;
	lt.base = r->base;
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

      if (cpu_65816 != 0)
        i = get_snes_pc_bank(l) >> 16;
      else
        i = l->base + l->bank;

      memory_file_id = r->file_id;
      memory_file_id_source = r->file_id_source;
      memory_line_number = r->linenumber;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      sprintf(mem_insert_action, "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT || r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
	if (little_endian == YES) {
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
	if (little_endian == YES) {
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
      else if (strcmp(r->name, "CADDR") == 0 || strcmp(r->name, "caddr") == 0) {
        lt.status = LABEL_STATUS_DEFINE;
        strcpy(lt.name, r->name);
        lt.address = r->address;
        lt.bank = r->bank;
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
      sprintf(mem_insert_action, "Writing reference %s: %s:%d: %s.", get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
        i = (int)l->address;
	if (little_endian == YES) {
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
	if (little_endian == YES) {
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
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a 8-bit reference.\n",
		  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
      }
      /* relative 16-bit with a label */
      else if (r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
        i = (((int)l->address) & 0xFFFF) - r->address - 2;
        if (i < -32768 || i > 65535) {
          fprintf(stderr, "%s: %s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a 16-bit reference.\n",
		  get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }
	if (little_endian == YES) {
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
  char name[256], *p, list_cmd, *outfile_tmp;
  FILE *f, *outfile;
  int list_cmd_idx, list_source_file, list_address_offset, y, outfile_size;
  unsigned long outfile_crc;

  if (outname == NULL)
    return FAILED;

  strcpy(name, outname);
  p = name;
  for (y = 0; y < 255 && *p != '.' && *p != 0; y++, p++);
  *(p++) = '.';
  *(p++) = 's';
  *(p++) = 'y';
  *(p++) = 'm';
  *p = 0;

  f = fopen(name, "wb");
  if (f == NULL) {
    fprintf(stderr, "MAIN: Error opening file \"%s\".\n", name);
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
      if (l->alive == NO || is_label_anonymous(l->name) == YES || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
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
	  fprintf(f, "0000%.4x %s\n", (int)l->address, l->name);
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
              fprintf(f, "%.2x:%.4x %.4x:%.8lx\n", s->bank + s->base, (s->output_address + list_address_offset) & 0xFFFF, list_source_file, (long unsigned int)s->listfile_ints[list_cmd_idx * 2 + 0]);
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


int write_rom_file(char *outname) {

  struct section *s;
  FILE *f;
  int i, b, e;


  f = fopen(outname, "wb");
  if (f == NULL) {
    fprintf(stderr, "WRITE_ROM_FILE: Error opening file \"%s\".\n", outname);
    return FAILED;
  }

  if (file_header != NULL)
    fwrite(file_header, 1, file_header_size, f);

  /* SMC header */
  if (smc_status != 0)
    smc_create_and_write(f);

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
    for (i = 0; i < romsize; i++)
      if (rom_usage[i] != 0)
	break;
    b = i;
    for (e = b; i < romsize; i++)
      if (rom_usage[i] != 0)
	e = i;

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
      if (compute_stack(sta, &k) == FAILED)
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

    /* all the references have been decoded, now compute */
    if (compute_stack(sta, &k) == FAILED)
      return FAILED;

    memory_file_id = sta->file_id;
    memory_file_id_source = sta->file_id_source;
    memory_line_number = sta->linenumber;

    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    sprintf(mem_insert_action, "Writing pending calculation %s: %s:%d.", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);

    if (sta->type == STACKS_TYPE_8BIT) {
      if (k < -128 || k > 255) {
	fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (mem_insert_ref(a, k) == FAILED)
	return FAILED;
    }
    else if (sta->type == STACKS_TYPE_16BIT) {
      if (k < -32768 || k > 65535) {
	fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (little_endian == YES) {
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
    else if (sta->type == STACKS_TYPE_13BIT) {
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
    else if (sta->type == STACKS_TYPE_24BIT) {
      if (k < -8388608 || k > 16777215) {
	fprintf(stderr, "%s: %s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 24-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (little_endian == YES) {
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


struct stack *find_stack(int id, int file_id) {

  struct stack *st = stacks_first;


  while (st != NULL) {
    if (st->id == id && st->file_id == file_id)
      return st;
    st = st->next;
  }

  return NULL;
}


int compute_stack(struct stack *sta, int *result) {

  struct stack_item *s;
  struct stack *st;
  int r, t, z, x, res;
  double v[256], q;


  if (sta->under_work == YES) {
    fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: A loop found in computation.\n", get_file_name(sta->file_id),
	    get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
    return FAILED;
  }

  if (sta->computed == YES) {
    *result = sta->result;
    return SUCCEEDED;
  }

  sta->under_work = YES;
  v[0] = 0.0;

  /*
  {
    char *get_stack_item_description(struct stack_item *si);
    int z;

    printf("----------------------------------------------------------------------\n");

    for (z = 0; z < sta->stacksize; z++) {
      struct stack_item *si = &sta->stack[z];
      printf(get_stack_item_description(si));
    }

    printf("id: %d file: %s line: %d type %d bank: %d position %d\n", sta->id, get_file_name(sta->file_id), sta->linenumber, sta->type, sta->bank, sta->position);
  }
  */

  x = sta->stacksize;
  s = sta->stack;
  for (r = 0, t = 0; r < x; r++, s++) {
    if (s->type == STACK_ITEM_TYPE_VALUE) {
      if (s->sign == SI_SIGN_NEGATIVE)
	v[t] = -s->value;
      else
	v[t] = s->value;
      t++;
    }
    else if (s->type == STACK_ITEM_TYPE_STACK) {
      /* we have a stack (A) inside a stack (B)! find the stack (A)! */
      st = find_stack((int)s->value, sta->file_id);

      if (st == NULL) {
	fprintf(stderr, "COMPUTE_STACK: A computation stack has gone missing. This is a fatal internal error. Please send the WLA DX author a bug report.\n");
	return FAILED;
      }

      if (compute_stack(st, &res) == FAILED)
	return FAILED;

      v[t] = res;
      t++;
    }
    else {
      switch ((int)s->value) {
      case SI_OP_PLUS:
	v[t - 2] += v[t - 1];
	t--;
	break;
      case SI_OP_MINUS:
	v[t - 2] -= v[t - 1];
	t--;
	break;
      case SI_OP_XOR:
	/* 16-bit XOR? */
	if (v[t - 2] > 0xFF || v[t - 2] < -128 || v[t - 1] > 0xFF || v[t - 1] < -128)
	  v[t - 2] = ((int)v[t - 1] ^ (int)v[t - 2]) & 0xFFFF;
	/* 8-bit XOR */
	else
	  v[t - 2] = ((int)v[t - 1] ^ (int)v[t - 2]) & 0xFF;
	t--;
	break;
      case SI_OP_MULTIPLY:
	v[t - 2] *= v[t - 1];
	t--;
	break;
      case SI_OP_OR:
	v[t - 2] = (int)v[t - 1] | (int)v[t - 2];
	t--;
	break;
      case SI_OP_AND:
	v[t - 2] = (int)v[t - 1] & (int)v[t - 2];
	t--;
	break;
      case SI_OP_LOW_BYTE:
	z = (int)v[t - 1];
	v[t - 1] = z & 0xFF;
	break;
      case SI_OP_HIGH_BYTE:
	z = (int)v[t - 1];
	v[t - 1] = (z>>8) & 0xFF;
	break;
      case SI_OP_MODULO:
	if (((int)v[t - 1]) == 0) {
	  fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Modulo by zero.\n", get_file_name(sta->file_id),
		  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
	  return FAILED;
	}
	v[t - 2] = (int)v[t - 2] % (int)v[t - 1];
	t--;
	break;
      case SI_OP_DIVIDE:
	if (v[t - 1] == 0.0) {
	  fprintf(stderr, "%s: %s:%d: COMPUTE_STACK: Division by zero.\n", get_file_name(sta->file_id),
		  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
	  return FAILED;
	}
	v[t - 2] /= v[t - 1];
	t--;
	break;
      case SI_OP_POWER:
	q = 1;
	for (z = 0; z < v[t - 1]; z++)
	  q *= v[t - 2];
	v[t - 2] = q;
	t--;
	break;
      case SI_OP_SHIFT_LEFT:
	v[t - 2] = (int)v[t - 2] << (int)v[t - 1];
	t--;
	break;
      case SI_OP_SHIFT_RIGHT:
	v[t - 2] = (int)v[t - 2] >> (int)v[t - 1];
	t--;
	break;
      }
    }
  }

  *result = (int)v[0];
  sta->result = (int)v[0];
  sta->computed = YES;
  sta->under_work = NO;

  /*
  printf("RESULT: %d\n", sta->result);
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
  if (compute_stack(sta, &k) == FAILED)
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

  if (sta->type == STACKS_TYPE_8BIT) {
    if (k < -128 || k > 255) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
	      get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;
  }
  else if (sta->type == STACKS_TYPE_16BIT) {
    if (k < -32768 || k > 65535) {
      fprintf(stderr, "%s: %s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
	      get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    if (little_endian == YES) {
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
  else if (sta->type == STACKS_TYPE_13BIT) {
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
    if (little_endian == YES) {
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
      if (little_endian == YES) {
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
      if (little_endian == YES) {
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
  double k;
  int g;


  s = NULL;
  if (sta->section_status != 0) {
    s = sec_first;
    while (s != NULL) {
      if (s->id == sta->section)
        break;
      s = s->next;
    }
  }

  si = sta->stack;
  g = 0;
  k = 0;
  while (g != sta->stacksize) {
    if (si->type == STACK_ITEM_TYPE_STRING) {
      l = NULL;

      /* bank number search */
      if (si->string[0] == ':') {
	if (is_label_anonymous(&si->string[1]) == YES) {
	  l = get_closest_anonymous_label(&si->string[1], sta->address, sta->file_id, sta->section_status, sta->section);
	  if (l != NULL)
	    k = l->bank;
	}
	else if (strcmp(&si->string[1], "CADDR") == 0 || strcmp(&si->string[1], "caddr") == 0) {
	  k = sta->bank + sta->base;
	  lt.status = LABEL_STATUS_DEFINE;
	  l = &lt;
	}
	else {
          find_label(&si->string[1], s, &l);

          if (l != NULL) {
            if (cpu_65816 != 0)
              k = get_snes_pc_bank(l) >> 16;
            else
              k = l->base + l->bank;
          }
	}
      }
      /* normal label address search */
      else {
	if (is_label_anonymous(si->string) == YES) {
	  l = get_closest_anonymous_label(si->string, sta->address, sta->file_id, sta->section_status, sta->section);
	  if (l != NULL)
	    k = l->address;

	  /* is the reference relative? */
	  if (sta->relative_references == YES)
	    k = k - sta->memory_address - 1;
	}
	else if (strcmp(si->string, "CADDR") == 0 || strcmp(si->string, "caddr") == 0) {
	  k = sta->memory_address;
	  lt.status = LABEL_STATUS_DEFINE;
	  l = &lt;
	}
	else {
          find_label(si->string, s, &l);

          if (l != NULL) {
            k = l->address;

            /* is the reference relative? */
            if (sta->relative_references == YES)
              k = k - sta->memory_address - 1;
          }
	}
      }

      if (l == NULL) {
	fprintf(stderr, "%s: %s:%d: PARSE_STACK: Unresolved reference to \"%s\".\n", get_file_name(sta->file_id),
		get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, si->string);
	return FAILED;
      }

      /* 65816 cpu bank fix */
      if (sta->type == STACKS_TYPE_24BIT && l->status == LABEL_STATUS_LABEL)
	k += get_snes_pc_bank(l);

      /*
      fprintf(stdout, "%s: %s:%d: %s %x %d\n", get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber,
	      si->string, (int)k, sta->relative_references);
      */

      if (l->status == LABEL_STATUS_STACK) {
	/* HACK: here we abuse the stack item structure's members */
	si->value = l->address;
	si->sign = l->file_id;
	si->type = STACK_ITEM_TYPE_STACK;
      }
      else {
	/* transform the label string into a value */
	si->value = k;
	si->type = STACK_ITEM_TYPE_VALUE;
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
    if (l->section_status == ON && l->section_struct != NULL && l->section_struct->status == SECTION_STATUS_RAM) {
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
  double size;


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

    if (strlen(labels[j]->name)+8 >= sizeof(l->name)) {
      fprintf(stderr, "GENERATE_SIZEOF_LABEL_DEFINITIONS: Expanded label name \"_sizeof_%s\" is %d characters too large.\n",
              labels[j]->name, (int)(strlen(labels[j]->name)+8+1-sizeof(l->name)));
      free(labels);
      return FAILED;
    }
    else
      sprintf(l->name, "_sizeof_%s", labels[j]->name);

    l->status = LABEL_STATUS_DEFINE;
    l->alive = YES;
    l->address = size;
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
	  sprintf(tmp, "%s_%d", l2->name, count);
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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "write.h"
#include "files.h"
#include "analyze.h"



extern struct reference *reference_first, *reference_last;
extern struct label *labels_first, *labels_last;
extern struct object_file *obj_first, *obj_last, *obj_tmp;
extern struct section *sec_first, *sec_last, *sec_hd_first, sec_hd_last;
extern struct stack *stacks_first, *stacks_last;
extern struct map_t *global_unique_label_map;
extern struct map_t *namespace_map;
extern struct slot slots[256];
extern unsigned char *rom, *rom_usage;
extern unsigned char *file_header, *file_footer;
extern int romsize, rombanks, banksize, verbose_mode, section_overwrite, symbol_mode;
extern int pc_bank, pc_full, pc_slot, pc_slot_max, snes_rom_mode;
extern int file_header_size, file_footer_size, *bankaddress, *banksizes;
extern int memory_file_id, memory_file_id_source, memory_line_number, output_mode;
extern int program_start, program_end, cpu_65816, snes_mode, smc_status;
extern int snes_sramsize;


static int _sections_sort(const void *a, const void *b) {

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
  if (snes_rom_mode == SNES_ROM_MODE_HIROM)
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
	ram_slots[s->bank] = malloc(sizeof(char *) * 256);
	if (ram_slots[s->bank] == NULL) {
	  fprintf(stderr, "INSERT_SECTIONS: Out of memory error.\n");
	  return FAILED;
	}
	for (i = 0; i < 256; i++)
	  ram_slots[s->bank][i] = NULL;
      }
      if (ram_slots[s->bank][s->slot] == NULL) {
	ram_slots[s->bank][s->slot] = malloc(slots[s->slot].size);
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

  sa = malloc(sizeof(struct section *) * sn);
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
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
		get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
	return FAILED;
      }
      if (s->address + s->size > banksize) {
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
		get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
	return FAILED;
      }
      for (; d < i; d++) {
	if (rom_usage[d] != 0 && rom[d] != s->data[d - pc_full])
	  break;
      }
      if (d == i) {
	for (i = 0; i < s->size; i++) {
	  if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
	    return FAILED;
	}
      }
      else {
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
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
	if (x == s->size) {
	  i = SUCCEEDED;
	  break;
	}
	if (pc_bank == s->address) {
	  fprintf(stderr, "%s:%s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
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
	if (x == s->size) {
	  i = SUCCEEDED;
	  break;
	}
	if (pc_bank == banksizes[s->bank]) {
	  fprintf(stderr, "%s:%s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes) in ROM bank %d.\n", get_file_name(s->file_id),
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

	for (i = 0; i < s->size; i++)
	  if (mem_insert_pc(s->data[i], s->slot, s->bank) == FAILED)
	    return FAILED;
      }
      else {
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: No room for section \"%s\" (%d bytes).\n", get_file_name(s->file_id),
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
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: Section \"%s\" (%d bytes) goes beyond the ROM size.\n", get_file_name(s->file_id),
		get_source_file_name(s->file_id, s->file_id_source), s->name, s->size);
	return FAILED;
      }
      if (s->address + s->size > banksize) {
	fprintf(stderr, "%s:%s: INSERT_SECTIONS: Section \"%s\" (%d bytes) overflows from ROM bank %d.\n", get_file_name(s->file_id),
		get_source_file_name(s->file_id, s->file_id_source), s->name, s->size, s->bank);
	return FAILED;
      }
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
      fprintf(stderr, "%s:%s:%d: TRY_PUT_LABEL: Label \"%s\" was defined more than once.\n", get_file_name(l->file_id),
          get_source_file_name(l->file_id, l->file_id_source), l->linenumber, l->name);
    return FAILED;
  }
  if ((err = hashmap_put(map, l->name, l)) != MAP_OK) {
    fprintf(stderr, "TRY_PUT_LABEL: Hashmap error %d. Please send a bug report!\n", err);
    return FAILED;
  }
  return SUCCEEDED;
}

int fix_label_sections(void) {

  struct section *s;
  struct label *l;

  l = labels_first;
  while (l != NULL) {
    int put_in_global = 1;
    int put_in_anything = 1;

    if (l->status == LABEL_STATUS_SYMBOL
        || l->status == LABEL_STATUS_BREAKPOINT
        || is_label_anonymous(l->name) == SUCCEEDED) {
      /* Don't put anonymous labels, breakpoints, or symbols into any maps */
      put_in_anything = 0;
    }

    if (l->section_status == ON) {
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
            l->section,
            l->name);
        return FAILED;
      }

      if (put_in_anything) {
        /* Put label into section's label map */
        if (try_put_label(s->label_map, l) == FAILED)
          return FAILED;

        if (l->name[0] == '_')
          put_in_global = 0;

        /* Put label into section's namespace's label map, if it's not
         * a local label */
        if (s->nspace != NULL && l->name[0] != '_') {
          if (try_put_label(s->nspace->label_map, l) == FAILED)
            return FAILED;
          put_in_global = 0;
        }
      }
    }

    /* Put the label into the global namespace */
    if (put_in_anything && put_in_global) {
      if (try_put_label(global_unique_label_map, l) == FAILED)
        return FAILED;
    }

    l = l->next;
  }

  return SUCCEEDED;
}

int fix_label_addresses(void) {

  struct section *s = NULL;
  struct label *l;

  /* fix labels' addresses */
  l = labels_first;
  while (l != NULL) {
    if (l->status == LABEL_STATUS_LABEL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
      if (l->section_status == ON) {
        if (l->section_struct == NULL) {
          fprintf(stderr, "FIX_LABELS: Internal error: section_struct is null.\n");
          return FAILED;
        }
        s = l->section_struct;
        if (s->id == l->section) {
          l->bank = s->bank;
          l->address += s->address;
          l->rom_address = l->address + bankaddress[l->bank];
          if (s->status != SECTION_STATUS_ABSOLUTE)
            l->address += slots[l->slot].address;
        }
      }
      else {
        l->rom_address = l->address + bankaddress[l->bank];
        l->address += slots[l->slot].address;
      }
    }
    l = l->next;
  }

  return SUCCEEDED;
}


int fix_references(void) {

  struct reference *r;
  struct section *s = NULL;
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
    l = labels_first;

    /* request for bank number? */
    if (r->name[0] == ':') {
      if (is_label_anonymous(&r->name[1]) == SUCCEEDED) {
        l = get_closest_anonymous_label(&r->name[1], x, r->file_id, l, r->section_status, r->section);
      }
      else if (strcmp(&r->name[1], "CADDR") == 0 || strcmp(&r->name[1], "caddr") == 0) {
        lt.status = LABEL_STATUS_LABEL;
        strcpy(lt.name, &r->name[1]);
        lt.address = r->address;
        lt.bank = r->bank;
        lt.section_status = OFF;
        l = &lt;
      }
      else {
        find_label(&r->name[1], s, &l);
      }

      if (l == NULL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
        fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Bank number request for an unknown label \"%s\".\n",
            get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, &r->name[1]);
        return FAILED;
      }

      if (cpu_65816 != 0)
        i = get_snes_pc_bank(l) >> 16;
      else
        i = l->bank;

      memory_file_id = r->file_id;
      memory_file_id_source = r->file_id_source;
      memory_line_number = r->linenumber;

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT || r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref(x + 1, (i >> 8) & 0xFF);
      }
      /* direct / relative 8-bit with a definition */
      else if (l->status == LABEL_STATUS_DEFINE) {
        fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Bank number request for a definition \"%s\"?\n",
            get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, l->name);
        return FAILED;
      }
      /* direct 24-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref(x + 1, (i >> 8) & 0xFF);
        mem_insert_ref(x + 2, (i >> 16) & 0xFF);
      }
      /* relative/direct 8-bit with a label */
      else {
        mem_insert_ref(x, i & 0xFF);
      }
    }
    /* normal reference */
    else {
      if (is_label_anonymous(r->name) == SUCCEEDED) {
        l = get_closest_anonymous_label(r->name, x, r->file_id, l, r->section_status, r->section);
      }
      else if (strcmp(r->name, "CADDR") == 0 || strcmp(r->name, "caddr") == 0) {
        lt.status = LABEL_STATUS_DEFINE;
        strcpy(lt.name, r->name);
        lt.address = r->address;
        lt.bank = r->bank;
        lt.section_status = OFF;
        l = &lt;
      }
      else {
        find_label(r->name, s, &l);
      }

      if (l == NULL || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
        fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Reference to an unknown label \"%s\".\n",
            get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);
        return FAILED;
      }

      memory_file_id = r->file_id;
      memory_file_id_source = r->file_id_source;
      memory_line_number = r->linenumber;

      /* direct 16-bit */
      if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
        i = l->address;
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref(x + 1, (i >> 8) & 0xFF);
      }
      /* direct / relative 8-bit with a value definition */
      else if (l->status == LABEL_STATUS_DEFINE && (r->type == REFERENCE_TYPE_DIRECT_8BIT || r->type == REFERENCE_TYPE_RELATIVE_8BIT)) {
        i = ((int)l->address) & 0xFFFF;
        if (i > 255 || i < -128) {
          fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 8-bit value.\n",
              get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
      }
      /* direct 24-bit */
      else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
        i = l->address;
        if (l->status == LABEL_STATUS_LABEL)
          i += get_snes_pc_bank(l);
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref(x + 1, (i >> 8) & 0xFF);
        mem_insert_ref(x + 2, (i >> 16) & 0xFF);
      }
      /* relative 8-bit with a label */
      else if (r->type == REFERENCE_TYPE_RELATIVE_8BIT) {
        i = (((int)l->address) & 0xFFFF) - r->address - 1;
        if (i < -128 || i > 127) {
          fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a 8-bit reference.\n",
              get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
      }
      /* relative 16-bit with a label */
      else if (r->type == REFERENCE_TYPE_RELATIVE_16BIT) {
        i = (((int)l->address) & 0xFFFF) - r->address - 2;
        if (i < -32768 || i > 65535) {
          fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Too large distance (%d bytes from $%x to $%x \"%s\") for a 16-bit reference.\n",
              get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, i, r->address, (int)l->address, l->name);
          return FAILED;
        }
        mem_insert_ref(x, i & 0xFF);
        mem_insert_ref(x + 1, (i >> 8) & 0xFF);
      }
      else {
        i = ((int)l->address) & 0xFFFF;
        if (i > 255) {
          fprintf(stderr, "%s:%s:%d: FIX_REFERENCES: Value ($%x) of \"%s\" is too much to be a 8-bit value.\n",
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


int write_symbol_file(char *outname, unsigned char mode) {

  struct section *s;
  struct label *l;
  char name[256], *p;
  FILE *f;
  int y;

  
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
    fprintf(f, "; no$gmb symbolic information for \"%s\".\n", outname);

    l = labels_first;
    while (l != NULL) {
      if (is_label_anonymous(l->name) == SUCCEEDED || l->status == LABEL_STATUS_SYMBOL || l->status == LABEL_STATUS_BREAKPOINT) {
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
      if (l->status == LABEL_STATUS_LABEL) {
	if (snes_mode == 0)
	  fprintf(f, "%.2x:%.4x %s\n", l->bank, (int)l->address, l->name);
	else
	  fprintf(f, "%.2x:%.4x %s\n", get_snes_pc_bank(l)>>16, (int)l->address, l->name);
      }
      else
	fprintf(f, "00:%.4x %s\n", (int)l->address, l->name);
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
	if (l->status != LABEL_STATUS_LABEL) {
	  l = l->next;
	  continue;
	}
	if (is_label_anonymous(l->name) == SUCCEEDED) {
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
	  fprintf(f, "%.2x:%.4x %s\n", l->bank, (int)l->address, l->name);
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
	if (l->status != LABEL_STATUS_SYMBOL) {
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
	if (l->status != LABEL_STATUS_BREAKPOINT) {
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
	if (l->status != LABEL_STATUS_DEFINE) {
	  l = l->next;
	  continue;
	}
	if (is_label_anonymous(l->name) == SUCCEEDED) {
	  l = l->next;
	  continue;
	}
	
	fprintf(f, "%.8x %s\n", (int)l->address, l->name);
	
	l = l->next;
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
      s = sec_hd_first;
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

    s = sec_hd_first;
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

    if (sta->type == STACKS_TYPE_8BIT) {
      if (k < -128 || k > 255) {
	fprintf(stderr, "%s:%s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (mem_insert_ref(a, k) == FAILED)
	return FAILED;
    }
    else if (sta->type == STACKS_TYPE_16BIT) {
      if (k < -32768 || k > 65535) {
	fprintf(stderr, "%s:%s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (mem_insert_ref(a, k & 0xFF) == FAILED)
	return FAILED;
      if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
	return FAILED;
    }
    else {
      if (k < -8388608 || k > 16777215) {
	fprintf(stderr, "%s:%s:%d: COMPUTE_PENDING_CALCULATIONS: Result (%d/$%x) of a computation is out of 24-bit range.\n",
		get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
	return FAILED;
      }
      if (mem_insert_ref(a, k & 0xFF) == FAILED)
	return FAILED;
      if (mem_insert_ref(a + 1, (k >> 8) & 0xFF) == FAILED)
	return FAILED;
      if (mem_insert_ref(a + 2, (k >> 16) & 0xFF) == FAILED)
	return FAILED;
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

  struct stackitem *s;
  struct stack *st;
  int r, t, z, x, res;
  double v[256], q;

  
  if (sta->under_work == YES) {
    fprintf(stderr, "%s:%s:%d: COMPUTE_STACK: A loop found in computation.\n", get_file_name(sta->file_id),
	    get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
    return FAILED;
  }

  if (sta->computed == YES) {
    *result = sta->result;
    return SUCCEEDED;
  }

  sta->under_work = YES;

  /*
  {
    char *get_stack_item_description(struct stackitem *si);
    int z;
    
    printf("----------------------------------------------------------------------\n");
	
    for (z = 0; z < sta->stacksize; z++) {
      struct stackitem *si = &sta->stack[z];
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
      /* we have a stack inside a stack! find the stack */
      /* HACK! we abuse sign here... */
      st = find_stack(s->value, s->sign);

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
	  fprintf(stderr, "%s:%s:%d: COMPUTE_STACK: Modulo by zero.\n", get_file_name(sta->file_id),
		  get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber);
	  return FAILED;
	}
	v[t - 2] = (int)v[t - 2] % (int)v[t - 1];
	t--;
	break;
      case SI_OP_DIVIDE:
	if (((int)v[t - 1]) == 0) {
	  fprintf(stderr, "%s:%s:%d: COMPUTE_STACK: Division by zero.\n", get_file_name(sta->file_id),
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

  *result = v[0];
  sta->result = v[0];
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

  s = sec_hd_first;
  while (sta->section != s->id)
    s = s->next;

  t = s->data + sta->address;

  if (sta->type == STACKS_TYPE_8BIT) {
    if (k < -128 || k > 255) {
      fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 8-bit range.\n",
	      get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;    
  }
  else if (sta->type == STACKS_TYPE_16BIT) {
    if (k < -32768 || k > 65535) {
      fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 16-bit range.\n",
	      get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;
    t++;
    *t = (k >> 8) & 0xFF;
  }
  else {
    if (k < -8388608 || k > 16777215) {
      fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_CALCULATIONS: Result (%d/$%x) of a computation is out of 24-bit range.\n",
	      get_file_name(sta->file_id), get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, k, k);
      return FAILED;
    }
    *t = k & 0xFF;
    t++;
    *t = (k >> 8) & 0xFF;
    t++;
    *t = (k >> 16) & 0xFF;
  }

  return SUCCEEDED;
}


int write_bank_header_references(struct reference *r) {

  struct section *s;
  struct label *l;
  unsigned char *t;
  int a;

  
  s = sec_hd_first;
  while (r->section != s->id)
    s = s->next;

  t = s->data + r->address;

  /* find the destination */
  find_label(r->name, s, &l);
  if (l != NULL) {
    a = l->address;
    /* direct 16-bit */
    if (r->type == REFERENCE_TYPE_DIRECT_16BIT) {
      *t = a & 0xFF;
      t++;
      *t = (a >> 8) & 0xFF;
    }
    /* direct 8-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_8BIT) {
      if (a > 255 || a < -128) {
        fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_REFERENCES: Value (%d/$%x) of \"%s\" is too much to be a 8-bit value.\n",
            get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, a, a, l->name);
        return FAILED;
      }
      *t = a & 0xFF;
    }
    /* direct 24-bit */
    else if (r->type == REFERENCE_TYPE_DIRECT_24BIT) {
      if (l->status == LABEL_STATUS_LABEL)
        a += get_snes_pc_bank(l);
      *t = a & 0xFF;
      t++;
      *t = (a >> 8) & 0xFF;
      t++;
      *t = (a >> 16) & 0xFF;
    }
    else {
      fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_REFERENCES: A relative reference (type %d) to label \"%s\".\n",
          get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->type, l->name);
      return FAILED;
    }
  }
  else {
    fprintf(stderr, "%s:%s:%d: WRITE_BANK_HEADER_REFERENCES: Reference to an unknown label \"%s\".\n",
	    get_file_name(r->file_id), get_source_file_name(r->file_id, r->file_id_source), r->linenumber, r->name);
    return FAILED;
  }

  return SUCCEEDED;
}


/* transform all string items inside a computation stack into corresponding numbers */
int parse_stack(struct stack *sta) {

  struct stackitem *si;
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
      /* bank number search */
      if (si->string[0] == ':') {
	if (is_label_anonymous(&si->string[1]) == SUCCEEDED) {
	  l = get_closest_anonymous_label(&si->string[1], sta->address, sta->file_id, l, sta->section_status, sta->section);
	  if (l != NULL)
	    k = l->bank;
	}
	else if (strcmp(&si->string[1], "CADDR") == 0 || strcmp(&si->string[1], "caddr") == 0) {
	  k = sta->bank;
	  lt.status = LABEL_STATUS_DEFINE;
	  l = &lt;
	}
	else {
          find_label(&si->string[1], s, &l);

          if (l != NULL) {
            if (cpu_65816 != 0)
              k = get_snes_pc_bank(l) >> 16;
            else
              k = l->bank;
          }
	}
      }
      /* normal label address search */
      else {
	if (is_label_anonymous(si->string) == SUCCEEDED) {
	  l = get_closest_anonymous_label(si->string, sta->address, sta->file_id, l, sta->section_status, sta->section);
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
	fprintf(stderr, "%s:%s:%d: PARSE_STACK: Unresolved reference to \"%s\".\n", get_file_name(sta->file_id),
		get_source_file_name(sta->file_id, sta->file_id_source), sta->linenumber, si->string);
	return FAILED;
      }

      /* 65816 cpu bank fix */
      if (sta->type == STACKS_TYPE_24BIT && l->status == LABEL_STATUS_LABEL)
	k += get_snes_pc_bank(l);

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


  /* do we override the user's banking scheme (.HIROM/.LOROM)? */
  if (snes_mode != 0) {
    /* use rom_address instead of address, as address points to
       the position in destination machine's memory, not in rom */
    k = l->rom_address;

    if (snes_rom_mode == SNES_ROM_MODE_HIROM)
      x = k / 0x10000;
    else
      x = k / 0x8000;
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
    if (s->library_status == ON && s->base_defined == ON) {
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


struct label *get_closest_anonymous_label(char *name, int rom_address, int file_id, struct label *l, int section_status, int section) {

  struct label *closest = NULL;
  int d = 999999999, e;

  
  if (strcmp(name, "_b") == 0 || strcmp(name, "_B") == 0) {
    while (l != NULL) {
      if (strcmp("__", l->name) == 0 && file_id == l->file_id && section_status == l->section_status) {
	if (section_status == OFF || (section_status == ON && section == l->section)) {
	  e = rom_address - l->rom_address;
	  if (e >= 0 && e < d) {
	    closest = l;
	    d = e;
	  }
	}
      }
      l = l->next;
    }
    return closest;
  }

  if (strcmp(name, "_f") == 0 || strcmp(name, "_F") == 0) {
    while (l != NULL) {
      if (strcmp("__", l->name) == 0 && file_id == l->file_id && section_status == l->section_status) {
	if (section_status == OFF || (section_status == ON && section == l->section)) {
	  e = l->rom_address - rom_address;
	  if (e > 0 && e < d) {
	    closest = l;
	    d = e;
	  }
	}
      }
      l = l->next;
    }
    return closest;
  }

  /* -, --, +, ++, ... */
  while (l != NULL) {
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
    l = l->next;
  }

  return closest;
}


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "files.h"



extern unsigned char *g_rom, *g_rom_usage;
extern int g_romsize, g_banksize, g_section_overwrite;
extern int g_pc_bank, g_pc_full, g_pc_slot, g_pc_slot_max;
extern struct object_file *g_obj_tmp;

char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
int g_memory_file_id, g_memory_file_id_source, g_memory_line_number;



int mem_insert(int address, unsigned char data) {

  if (g_rom[address] == data) {
    g_rom_usage[address]++;
    return SUCCEEDED;
  }
  if (g_rom_usage[address] != 0) {
    fprintf(stderr, "%s: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n", get_file_name(g_memory_file_id), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_usage[address]++;
  g_rom[address] = data;

  return SUCCEEDED;
}


int mem_insert_allow_overwrite(int address, unsigned char data, unsigned int allowed_overwrites) {

  if (g_rom_usage[address] > allowed_overwrites)
    return mem_insert(address, data);

  g_rom_usage[address]++;
  g_rom[address] = data;

  return SUCCEEDED;
}


int mem_insert_ref(int address, unsigned char data) {

  if (address > g_romsize || address < 0) {
    fprintf(stderr, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
            get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[address] > 1 && g_rom[address] != data) {
    if (g_memory_line_number != 0)
      fprintf(stderr, "%s: %s:%d: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_rom[address], data);
    else
      fprintf(stderr, "%s: %s:[WLA]: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_usage[address]++;
  g_rom[address] = data;

  return SUCCEEDED;
}


int mem_insert_ref_13bit_high(int address, unsigned char data) {

  if (address > g_romsize || address < 0) {
    fprintf(stderr, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
            get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[address] > 1 && (g_rom[address] & 0x1F) != data) {
    if (g_memory_line_number != 0)
      fprintf(stderr, "%s: %s:%d: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_rom[address], data);
    else
      fprintf(stderr, "%s: %s:[WLA]: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_usage[address]++;
  g_rom[address] = (g_rom[address] & 0xE0) | data;

  return SUCCEEDED;
}


int mem_insert_pc(unsigned char d, int slot_current, int bank_current) {

  if (g_pc_slot >= g_pc_slot_max) {
    fprintf(stderr, "%s: MEM_INSERT: Overflow from slot %d.\n", get_file_name(g_memory_file_id), slot_current);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_pc_bank >= g_banksize) {
    fprintf(stderr, "%s: MEM_INSERT: Overflow from bank %d.\n", get_file_name(g_memory_file_id), bank_current);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[g_pc_full] != 0 && g_rom[g_pc_full] != d && g_section_overwrite == OFF) {
    fprintf(stderr, "%s: MEM_INSERT: Overwrite at $%x, old $%x new $%x\n", get_file_name(g_memory_file_id), g_pc_full, g_rom[g_pc_full], d);
    if (g_mem_insert_action[0] != 0)
      fprintf(stderr, "   ^ %s\n", g_mem_insert_action);
  }
  
  if (g_section_overwrite == OFF)
    g_rom_usage[g_pc_full]++;
  g_rom[g_pc_full] = d;
  g_pc_slot++;
  g_pc_bank++;
  g_pc_full++;

  return SUCCEEDED;
}

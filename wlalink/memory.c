
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "files.h"
#include "main.h"



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
    print_text(NO, "%s: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n", get_file_name(g_memory_file_id), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
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
    print_text(NO, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
            get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[address] > 1 && g_rom[address] != data) {
    if (g_memory_line_number != 0)
      print_text(NO, "%s: %s:%d: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_rom[address], data);
    else
      print_text(NO, "%s: %s:[WLA]: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
              get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_usage[address]++;
  g_rom[address] = data;

  return SUCCEEDED;
}


int mem_insert_ref_13bit_high(int address, unsigned char data) {

  if (address > g_romsize || address < 0) {
    print_text(NO, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
               get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[address] > 1 && (g_rom[address] & 0x1F) != data) {
    if (g_memory_line_number != 0)
      print_text(NO, "%s: %s:%d: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
                 get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_rom[address], data);
    else
      print_text(NO, "%s: %s:[WLA]: MEM_INSERT: Overwrite at $%x (old $%.2x new $%.2x).\n",
                 get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), address, g_rom[address], data);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
  }

  g_rom_usage[address]++;
  g_rom[address] = (g_rom[address] & 0xE0) | data;

  return SUCCEEDED;
}


int mem_insert_pc(unsigned char d, int slot_current, int bank_current) {

  if (g_pc_slot >= g_pc_slot_max) {
    print_text(NO, "%s: MEM_INSERT: Overflow from slot %d.\n", get_file_name(g_memory_file_id), slot_current);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_pc_bank >= g_banksize) {
    print_text(NO, "%s: MEM_INSERT: Overflow from bank %d.\n", get_file_name(g_memory_file_id), bank_current);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }
  if (g_rom_usage[g_pc_full] != 0 && g_rom[g_pc_full] != d && g_section_overwrite == OFF) {
    print_text(NO, "%s: MEM_INSERT: Overwrite at $%x, old $%x new $%x\n", get_file_name(g_memory_file_id), g_pc_full, g_rom[g_pc_full], d);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
  }
  
  if (g_section_overwrite == OFF)
    g_rom_usage[g_pc_full]++;
  else
    g_rom_usage[g_pc_full] = 1;

  g_rom[g_pc_full] = d;
  g_pc_slot++;
  g_pc_bank++;
  g_pc_full++;

  return SUCCEEDED;
}


void bits_add_bit(int *bits_byte, int *bits_position, int *bits_to_define, int data) {

  int bit = (data >> (*bits_to_define - 1)) & 1;

  *bits_to_define = *bits_to_define - 1;

  *bits_byte = *bits_byte | (bit << (7 - *bits_position));

  *bits_position = *bits_position + 1;
}


int mem_insert_bits(int address, int data, int bits_position, int bits_to_define) {

  int bits_byte;
  
  if (address > g_romsize || address < 0) {
    print_text(NO, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
               get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
    if (g_mem_insert_action[0] != 0)
      print_text(NO, "   ^ %s\n", g_mem_insert_action);
    return FAILED;
  }

  bits_byte = g_rom[address];
  
  while (bits_to_define > 0) {
    int bits_to_define_this_byte = 8 - bits_position;
    int bits;

    if (bits_to_define_this_byte > bits_to_define)
      bits_to_define_this_byte = bits_to_define;

    for (bits = 0; bits < bits_to_define_this_byte; bits++)
      bits_add_bit(&bits_byte, &bits_position, &bits_to_define, data);

    if (bits_position == 8) {
      bits_position = 0;
      g_rom[address++] = (unsigned char)bits_byte;
      bits_byte = g_rom[address];
    }
  }

  if (bits_position > 0)
    g_rom[address] = (unsigned char)bits_byte;
  
  return SUCCEEDED;
}

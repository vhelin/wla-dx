
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "files.h"
#include "main.h"
#include "memory.h"



extern unsigned char *g_rom, *g_rom_usage;
extern int g_romsize, g_banksize, g_section_overwrite;
extern int g_pc_bank, g_pc_full, g_pc_slot, g_pc_slot_max;
extern struct object_file *g_obj_tmp;

char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
int g_memory_file_id, g_memory_file_id_source, g_memory_line_number;



/* NOTE: g_rom[] and g_rom_usage[] both hold exactly g_romsize bytes
   (_allocate_rom() in main.c calloc()s them that way), so the only valid
   addresses are 0 .. g_romsize - 1. mem_insert() and
   mem_insert_allow_overwrite() below used to touch both arrays with whatever
   address the caller supplied: parse_data_blocks() feeds them the address and
   the byte count of a DATA_TYPE_BLOCK straight out of the object file
   ("i = READ_T" then "for (; x > 0; x--, i++) mem_insert(i, *(t++))"), so a
   crafted .o file wrote past the end of the two heap buffers. Check the address
   in one place and reuse it. */
static int _mem_address_is_valid(int address) {

  if (address >= 0 && address < g_romsize)
    return YES;

  print_text(NO, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
          get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
  if (g_mem_insert_action[0] != 0)
    print_text(NO, "   ^ %s\n", g_mem_insert_action);

  return NO;
}


int mem_insert(int address, unsigned char data) {

  if (_mem_address_is_valid(address) == NO)
    return FAILED;

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

  /* NOTE: same missing bound as mem_insert() above - every other caller in
     compute.c passes fixed header offsets that fit, but keeping the check here
     makes the two entry points behave identically for a bad address. */
  if (_mem_address_is_valid(address) == NO)
    return FAILED;

  if (g_rom_usage[address] > allowed_overwrites)
    return mem_insert(address, data);

  g_rom_usage[address]++;
  g_rom[address] = data;

  return SUCCEEDED;
}


/* NOTE: the old test here was "address > g_romsize", but g_rom[] has only
   g_romsize bytes, so the last usable address is g_romsize - 1. An address of
   exactly g_romsize slipped through and the "g_rom_usage[address]++" /
   "g_rom[address] = data" below then wrote one byte past both heap buffers
   (SIGSEGV with a guard page right after the ROM). Same off-by-one was in
   mem_insert_ref_13bit_high() and in mem_insert_bits(). */
static int _mem_ref_address_is_valid(int address) {

  if (address >= 0 && address < g_romsize)
    return YES;

  print_text(NO, "%s: %s:%d: MEM_INSERT: Address $%x is out of the output range $0-$%x.\n",
          get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, address, g_romsize);
  if (g_mem_insert_action[0] != 0)
    print_text(NO, "   ^ %s\n", g_mem_insert_action);

  return NO;
}


int mem_insert_ref(int address, unsigned char data) {

  if (_mem_ref_address_is_valid(address) == NO)
    return FAILED;
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

  /* NOTE: see _mem_ref_address_is_valid() - "> g_romsize" let address ==
     g_romsize through and wrote one byte past g_rom[]. */
  if (_mem_ref_address_is_valid(address) == NO)
    return FAILED;
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
  
  /* NOTE: see _mem_ref_address_is_valid() - "> g_romsize" let address ==
     g_romsize through, and the loop below then ran off the end of g_rom[] as
     well: it writes the byte and reads the next one with "address++", never
     checking g_romsize. A ".BITS 16 label" placed at the last ROM byte (or a
     reference in a section at the end of the ROM) walked past the buffer
     (SIGSEGV with a guard page right after the ROM). */
  if (_mem_ref_address_is_valid(address) == NO)
    return FAILED;

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

      if (address >= g_romsize) {
        print_text(NO, "%s: %s:%d: MEM_INSERT: BITS value runs past the end of the output range $0-$%x.\n",
                get_file_name(g_memory_file_id), get_source_file_name(g_memory_file_id, g_memory_file_id_source), g_memory_line_number, g_romsize);
        if (g_mem_insert_action[0] != 0)
          print_text(NO, "   ^ %s\n", g_mem_insert_action);
        return FAILED;
      }

      bits_byte = g_rom[address];
    }
  }

  if (bits_position > 0)
    g_rom[address] = (unsigned char)bits_byte;
  
  return SUCCEEDED;
}

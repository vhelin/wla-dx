
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "compute.h"


extern unsigned char *rom;
extern int romsize, sms_checksum, smstag_defined, gb_checksum, gb_complement_check, snes_checksum;
extern int snes_rom_mode;


int compute_checksums(void) {

  if (sms_checksum != 0)
    compute_sms_checksum();
  if (smstag_defined != 0)
    add_sms_tag();
  if (gb_complement_check != 0)
    compute_gb_complement_check();
  if (gb_checksum != 0)
    compute_gb_checksum();
  if (snes_checksum != 0)
    compute_snes_checksum();

  return SUCCEEDED;
}


int compute_gb_complement_check(void) {

  int i, x;

  if (romsize < 0x8000) {
    fprintf(stderr, "COMPUTE_GB_COMPLEMENT_CHECK: GB complement check computing requires a ROM of at least 32KB.\n");
    return FAILED;
  }

  i = 0;
  for (x = 0x134; x <= 0x14C; x++)
    i += rom[x];
  i += 25;
  mem_insert(0x14D, 0 - (i & 0xFF));

  return SUCCEEDED;
}


int compute_gb_checksum(void) {

  int i, x;

  if (romsize < 0x8000) {
    fprintf(stderr, "COMPUTE_GB_CHECKSUM: GB checksum computing requires a ROM of at least 32KB.\n");
    return FAILED;
  }

  i = 0;
  for (x = 0; x < 0x14E; x++)
    i += rom[x];
  for (x = 0x150; x < romsize; x++)
    i += rom[x];

  mem_insert(0x14E, (i >> 8) & 0xFF);
  mem_insert(0x14F, i & 0xFF);

  return SUCCEEDED;
}


int compute_snes_checksum(void) {

  int i, x, n, m, l, o;

  if (snes_rom_mode == SNES_ROM_MODE_LOROM) {
    if (romsize < 0x8000) {
      fprintf(stderr, "COMPUTE_SNES_CHECKSUM: SNES checksum computing for a LoROM image requires a ROM of at least 32KB.\n");
      return FAILED;
    }
  }
  else {
    if (romsize < 0x10000) {
      fprintf(stderr, "COMPUTE_SNES_CHECKSUM: SNES checksum computing for a HiROM image requires a ROM of at least 64KB.\n");
      return FAILED;
    }
  }

  /* n = data inside 4mbit blocks, m = data outside that */
  if (romsize < 512*1024) {
    n = romsize;
    m = 0;
  }
  else {
    n = (romsize/(512*1024))*512*1024;
    m = romsize - n;
  }

  /* sum all the bytes inside the 4mbit blocks */
  x = 0;
  for (i = 0; i < n; i++) {
    if (snes_rom_mode == SNES_ROM_MODE_LOROM) {
      /* skip the checksum bytes */
      if (!(i == 0x7FDC || i == 0x7FDD || i == 0x7FDE || i == 0x7FDF))
	x += rom[i];
    }
    else {
      /* skip the checksum bytes */
      if (!(i == 0xFFDC || i == 0xFFDD || i == 0xFFDE || i == 0xFFDF))
	x += rom[i];
    }
  }

  /* add to that the data outside the 4mbit blocks, ringbuffer style repeating 
     the remaining block until the the final part reaches 4mbits */
  for (o = 0, l = i; i < romsize; i++, o++)
    x += rom[(o % m) + l];

  /* 2*255 is for the checksum and its complement bytes that we skipped earlier */
  x += 2*255;

  /* compute the inverse checksum */
  l = (x & 0xFFFF) ^ 0xFFFF;

  /* insert the checksum bytes */
  if (snes_rom_mode == SNES_ROM_MODE_LOROM) {
    mem_insert(0x7FDC, l & 0xFF);
    mem_insert(0x7FDD, (l >> 8) & 0xFF);
    mem_insert(0x7FDE, x & 0xFF);
    mem_insert(0x7FDF, (x >> 8) & 0xFF);
  }
  else {
    mem_insert(0xFFDC, l & 0xFF);
    mem_insert(0xFFDD, (l >> 8) & 0xFF);
    mem_insert(0xFFDE, x & 0xFF);
    mem_insert(0xFFDF, (x >> 8) & 0xFF);
  }

  return SUCCEEDED;
}


int add_sms_tag(void) {

  int tag_address = 0x7FF0;

  if (romsize < 0x8000)
    tag_address = 0x3FF0;

  if (romsize < 0x4000) {
    fprintf(stderr, "ADD_SMS_TAG: SMS/GG tag requires a ROM of at least 16KBs.\n");
    return SUCCEEDED;
  }

  /* TMR SEGA */
  mem_insert(tag_address + 0x0, 0x54);
  mem_insert(tag_address + 0x1, 0x4D);
  mem_insert(tag_address + 0x2, 0x52);
  mem_insert(tag_address + 0x3, 0x20);
  mem_insert(tag_address + 0x4, 0x53);
  mem_insert(tag_address + 0x5, 0x45);
  mem_insert(tag_address + 0x6, 0x47);
  mem_insert(tag_address + 0x7, 0x41);

  return SUCCEEDED;
}


int compute_sms_checksum(void) {

  int tag_address = 0x7FF0, x, i;
  /* SMS Export + 32KB ROM */
  int final_byte = 0x4C;

  if (romsize < 0x8000) {
    /* let's assume it's a 16KB ROM */
    tag_address = 0x3FF0;
    /* SMS Export + 16KB ROM */
    final_byte = 0x4B;
  }

  if (romsize < 0x4000) {
    fprintf(stderr, "COMPUTE_SMS_CHECKSUM: SMS/GG checksum computing requires a ROM of at least 16KBs.\n");
    return SUCCEEDED;
  }

  /* add together 16/32KB minus SMS/GG header */
  i = 0;
  for (x = 0; x < tag_address; x++)
    i += rom[x];

  mem_insert(tag_address + 0xA, i & 0xFF);
  mem_insert(tag_address + 0xB, (i >> 8) & 0xFF);

  /* region code + ROM size */
  mem_insert(tag_address + 0xF, final_byte);

  return SUCCEEDED;
}

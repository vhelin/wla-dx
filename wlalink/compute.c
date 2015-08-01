
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


int reserve_checksum_bytes(void) {

  /* reserve checksum bytes so that no free type sections will be placed over them */
  
  if (sms_checksum != 0) {
    int tag_address = 0x7FF0;
    if (romsize < 0x4000)
      tag_address = 0x1FF0;
    else if (romsize < 0x8000)
      tag_address = 0x3FF0;

    if (romsize >= 0x2000) {
      mem_insert(tag_address + 0xA, 0x0);
      mem_insert(tag_address + 0xB, 0x0);
      mem_insert(tag_address + 0xF, 0x0);
    }
  }
	       
  if (smstag_defined != 0) {
    int tag_address = 0x7FF0;
    if (romsize < 0x4000)
      tag_address = 0x1FF0;
    else if (romsize < 0x8000)
      tag_address = 0x3FF0;
    
    if (romsize >= 0x2000) {
      mem_insert(tag_address + 0x0, 0);
      mem_insert(tag_address + 0x1, 0);
      mem_insert(tag_address + 0x2, 0);
      mem_insert(tag_address + 0x3, 0);
      mem_insert(tag_address + 0x4, 0);
      mem_insert(tag_address + 0x5, 0);
      mem_insert(tag_address + 0x6, 0);
      mem_insert(tag_address + 0x7, 0);
    }
  }

  if (gb_complement_check != 0) {
    if (romsize >= 0x8000)
      mem_insert(0x14D, 0);
  }

  if (gb_checksum != 0) {
    if (romsize >= 0x8000) {
      mem_insert(0x14E, 0);
      mem_insert(0x14F, 0);
    }
  }

  if (snes_checksum != 0) {
    if (snes_rom_mode == SNES_ROM_MODE_LOROM && romsize >= 0x8000) {
      mem_insert(0x7FDC, 0);
      mem_insert(0x7FDD, 0);
      mem_insert(0x7FDE, 0);
      mem_insert(0x7FDF, 0);
    }
    else if (snes_rom_mode == SNES_ROM_MODE_HIROM && romsize >= 0x10000) {
      mem_insert(0xFFDC, 0);
      mem_insert(0xFFDD, 0);
      mem_insert(0xFFDE, 0);
      mem_insert(0xFFDF, 0);
    }
  }
  
  return SUCCEEDED;
}


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

  int res, j;


  if (romsize < 0x8000) {
    fprintf(stderr, "COMPUTE_GB_COMPLEMENT_CHECK: GB complement check computing requires a ROM of at least 32KB.\n");
    return FAILED;
  }

  res = 0;
  for (j = 0x134; j <= 0x14C; j++)
    res += rom[j];
  res += 25;
  mem_insert_allow_overwrite(0x14D, 0 - (res & 0xFF), 1);

  return SUCCEEDED;
}


int compute_gb_checksum(void) {

  int checksum, j;

  
  if (romsize < 0x8000) {
    fprintf(stderr, "COMPUTE_GB_CHECKSUM: GB checksum computing requires a ROM of at least 32KB.\n");
    return FAILED;
  }

  checksum = 0;
  for (j = 0; j < 0x14E; j++)
    checksum += rom[j];
  for (j = 0x150; j < romsize; j++)
    checksum += rom[j];

  mem_insert_allow_overwrite(0x14E, (checksum >> 8) & 0xFF, 1);
  mem_insert_allow_overwrite(0x14F, checksum & 0xFF, 1);

  return SUCCEEDED;
}


int compute_snes_checksum(void) {

  int i, j, k, res, n, m, inv;

  
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
  res = 0;
  for (i = 0; i < n; i++) {
    if (snes_rom_mode == SNES_ROM_MODE_LOROM) {
      /* skip the checksum bytes */
      if (!(i == 0x7FDC || i == 0x7FDD || i == 0x7FDE || i == 0x7FDF))
	res += rom[i];
    }
    else {
      /* skip the checksum bytes */
      if (!(i == 0xFFDC || i == 0xFFDD || i == 0xFFDE || i == 0xFFDF))
	res += rom[i];
    }
  }

  /* add to that the data outside the 4mbit blocks, ringbuffer style repeating 
     the remaining block until the the final part reaches 4mbits */
  for (j = 0, k = i; i < romsize; i++, j++)
    res += rom[(j % m) + k];

  /* 2*255 is for the checksum and its complement bytes that we skipped earlier */
  res += 2*255;

  /* compute the inverse checksum */
  inv = (res & 0xFFFF) ^ 0xFFFF;

  /* insert the checksum bytes */
  if (snes_rom_mode == SNES_ROM_MODE_LOROM) {
    mem_insert_allow_overwrite(0x7FDC, inv & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDD, (inv >> 8) & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDE, res & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDF, (res >> 8) & 0xFF, 1);
  }
  else {
    mem_insert_allow_overwrite(0xFFDC, inv & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDD, (inv >> 8) & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDE, res & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDF, (res >> 8) & 0xFF, 1);
  }

  return SUCCEEDED;
}


int add_sms_tag(void) {

  int tag_address = 0x7FF0;

  
  if (romsize < 0x4000)
    tag_address = 0x1FF0;
  else if (romsize < 0x8000)
    tag_address = 0x3FF0;

  if (romsize < 0x2000) {
    fprintf(stderr, "ADD_SMS_TAG: SMS/GG tag requires a ROM of at least 8KBs.\n");
    return SUCCEEDED;
  }

  /* TMR SEGA */
  mem_insert_allow_overwrite(tag_address + 0x0, 0x54, 1);
  mem_insert_allow_overwrite(tag_address + 0x1, 0x4D, 1);
  mem_insert_allow_overwrite(tag_address + 0x2, 0x52, 1);
  mem_insert_allow_overwrite(tag_address + 0x3, 0x20, 1);
  mem_insert_allow_overwrite(tag_address + 0x4, 0x53, 1);
  mem_insert_allow_overwrite(tag_address + 0x5, 0x45, 1);
  mem_insert_allow_overwrite(tag_address + 0x6, 0x47, 1);
  mem_insert_allow_overwrite(tag_address + 0x7, 0x41, 1);

  return SUCCEEDED;
}


int compute_sms_checksum(void) {

  int tag_address = 0x7FF0, j, checksum;
  /* SMS Export + 32KB ROM */
  int final_byte = 0x4C;

  
  if (romsize < 0x4000) {
    /* let's assume it's a 8KB ROM */
    tag_address = 0x1FF0;
    /* SMS Export + 8KB ROM */
    final_byte = 0x4A;
  }
  else if (romsize < 0x8000) {
    /* let's assume it's a 16KB ROM */
    tag_address = 0x3FF0;
    /* SMS Export + 16KB ROM */
    final_byte = 0x4B;
  }

  if (romsize < 0x2000) {
    fprintf(stderr, "COMPUTE_SMS_CHECKSUM: SMS/GG checksum computing requires a ROM of at least 8KBs.\n");
    return SUCCEEDED;
  }

  /* add together 8-32KB minus SMS/GG header */
  checksum = 0;
  for (j = 0; j < tag_address; j++)
    checksum += rom[j];

  mem_insert_allow_overwrite(tag_address + 0xA, checksum & 0xFF, 1);
  mem_insert_allow_overwrite(tag_address + 0xB, (checksum >> 8) & 0xFF, 1);

  /* region code + ROM size */
  mem_insert_allow_overwrite(tag_address + 0xF, final_byte, 1);

  return SUCCEEDED;
}

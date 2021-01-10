
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "compute.h"

#ifdef AMIGA
#include "/printf.h"
#else
#include "../printf.h"
#endif


extern unsigned char *rom, *rom_usage;
extern char mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern int romsize, sms_checksum, smstag_defined, gb_checksum, gb_complement_check, snes_checksum, sms_header;
extern int snes_rom_mode;


int reserve_checksum_bytes(void) {

  /* reserve checksum bytes so that no free type sections will be placed over them */
  
  if (sms_checksum != 0 || sms_header != 0) {
    int tag_address = 0x7FF0;
    
    if (romsize < 0x4000)
      tag_address = 0x1FF0;
    else if (romsize < 0x8000)
      tag_address = 0x3FF0;

    if (romsize >= 0x2000) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving SMS ROM checksum bytes");

      /* checksum */
      mem_insert(tag_address + 0xA, 0x0);
      mem_insert(tag_address + 0xB, 0x0);

      if (sms_checksum != 0) {
        /* create a what-we-are-doing message for mem_insert*() warnings/errors */
        snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving SMS ROM region code byte");

        /* region code */
        if (rom_usage[tag_address + 0xF] == 0) {
          int rs = 0;

          /* try to calculate the correct romsize value */
          if (romsize < 16*1024)
            rs = 0xA; /* 8KB */
          else if (romsize < 32*1024)
            rs = 0xB; /* 16KB */
          else
            rs = 0xC; /* 32KB+ */
          
          mem_insert(tag_address + 0xF, 4 << 4 | rs);
        }
        else
          mem_insert(tag_address + 0xF, rom[tag_address + 0xF]);
      }
    }
  }

  if (smstag_defined != 0 || sms_header != 0) {
    int tag_address = 0x7FF0;
    
    if (romsize < 0x4000)
      tag_address = 0x1FF0;
    else if (romsize < 0x8000)
      tag_address = 0x3FF0;
    
    if (romsize >= 0x2000) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving SMS ROM TMR SEGA bytes");

      /* tmr sega */
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
    if (romsize >= 0x8000) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving GB ROM complement check byte");

      mem_insert(0x14D, 0);
    }
  }

  if (gb_checksum != 0) {
    if (romsize >= 0x8000) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving GB ROM checksum bytes");

      mem_insert(0x14E, 0);
      mem_insert(0x14F, 0);
    }
  }

  if (snes_checksum != 0) {
    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Reserving SNES ROM checksum bytes");

    if ((snes_rom_mode == SNES_ROM_MODE_LOROM || snes_rom_mode == SNES_ROM_MODE_EXLOROM) && romsize >= 0x8000) {
      mem_insert(0x7FDC, 0);
      mem_insert(0x7FDD, 0);
      mem_insert(0x7FDE, 0);
      mem_insert(0x7FDF, 0);
    }
    else if ((snes_rom_mode == SNES_ROM_MODE_HIROM || snes_rom_mode == SNES_ROM_MODE_EXHIROM) && romsize >= 0x10000) {
      mem_insert(0xFFDC, 0);
      mem_insert(0xFFDD, 0);
      mem_insert(0xFFDE, 0);
      mem_insert(0xFFDF, 0);
    }
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
    
  return SUCCEEDED;
}


int compute_checksums(void) {

  if (sms_checksum != 0)
    compute_sms_checksum();
  if (sms_header != 0)
    compute_sms_checksum();
  if (smstag_defined != 0 || sms_header != 0)
    add_tmr_sega();
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

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing GB ROM checksum complement");

  mem_insert_allow_overwrite(0x14D, 0 - (res & 0xFF), 1);

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
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

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing GB ROM checksum bytes");
    
  mem_insert_allow_overwrite(0x14E, (checksum >> 8) & 0xFF, 1);
  mem_insert_allow_overwrite(0x14F, checksum & 0xFF, 1);

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
  return SUCCEEDED;
}


int finalize_snes_rom(void) {

  int i;
  
  
  if (snes_rom_mode == SNES_ROM_MODE_EXHIROM && romsize >= 0x410000) {
    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Mirroring SNES ROM header from $40ffb0-$40ffff -> $ffb0-$ffff");

    /* mirror the cartridge rom header from $40ffb0-$40ffff -> $ffb0-$ffff */
    for (i = 0; i < 5*16; i++)
      mem_insert(0xffb0 + i, rom[0x40ffb0 + i]);
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
  return SUCCEEDED;
}


int compute_snes_exhirom_checksum(void) {

  int i, j, checksum = 0, inv;


  /* do first the low 40-8Mbits (32Mbits) */
  for (i = 0; i < 32/8*1024*1024; i++) {
    /* skip the (mirrored) checksum bytes */
    if (!(i == 0xFFDC || i == 0xFFDD || i == 0xFFDE || i == 0xFFDF))
      checksum += rom[i];
  }

  /* 2*255 is for the checksum and its complement bytes that we skipped earlier */
  checksum += 2*255;

  /* next loop the remaining data until 64MBits are summed */
  j = 32/8*1024*1024;
  for (i = 0; i < 32/8*1024*1024; i++) {
    /* loop around? */
    if (j >= romsize)
      j = 32/8*1024*1024;
    if (!(j == 0x40FFDC || j == 0x40FFDD || j == 0x40FFDE || j == 0x40FFDF))
      checksum += rom[j];
    if (j == 0x40FFDC) {
      /* 2*255 is for the checksum and its complement bytes that we are skipping */
      checksum += 2*255;
    }
    j++;
  }

  /* compute the inverse checksum */
  inv = (checksum & 0xFFFF) ^ 0xFFFF;

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing SNES ROM checksum bytes");
  
  /* insert the checksum bytes */
  mem_insert_allow_overwrite(0x40FFDC, inv & 0xFF, 1);
  mem_insert_allow_overwrite(0x40FFDD, (inv >> 8) & 0xFF, 1);
  mem_insert_allow_overwrite(0x40FFDE, checksum & 0xFF, 1);
  mem_insert_allow_overwrite(0x40FFDF, (checksum >> 8) & 0xFF, 1);

  /* ... and mirror them */
  mem_insert_allow_overwrite(0xFFDC, inv & 0xFF, 1);
  mem_insert_allow_overwrite(0xFFDD, (inv >> 8) & 0xFF, 1);
  mem_insert_allow_overwrite(0xFFDE, checksum & 0xFF, 1);
  mem_insert_allow_overwrite(0xFFDF, (checksum >> 8) & 0xFF, 1);

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
  return SUCCEEDED;  
}


static int round_up_to_next_power_of_2(int x) {

  int exponent;

  if (x < 0)
    return -1;
  for (exponent = 0; exponent < 31; exponent++) {
    int power_of_two = 1 << exponent;

    if (x <= power_of_two)
      return power_of_two;
  }
  
  return -1;
}


int compute_snes_checksum(void) {

  int i, checksum, inv;
  int mirror_begin, mirror_end;

  /* ExHiROM jump */
  if (snes_rom_mode == SNES_ROM_MODE_EXHIROM && romsize >= 0x410000)
    return compute_snes_exhirom_checksum();
  
  if (snes_rom_mode == SNES_ROM_MODE_LOROM || snes_rom_mode == SNES_ROM_MODE_EXLOROM) {
    if (romsize < 0x8000) {
      fprintf(stderr, "COMPUTE_SNES_CHECKSUM: SNES checksum computing for a LoROM/ExLoROM image requires a ROM of at least 32KB.\n");
      return FAILED;
    }
  }
  else {
    if (romsize < 0x10000) {
      fprintf(stderr, "COMPUTE_SNES_CHECKSUM: SNES checksum computing for a HiROM/ExHiROM image requires a ROM of at least 64KB.\n");
      return FAILED;
    }
  }

  mirror_end = round_up_to_next_power_of_2(romsize);
  if (mirror_end == -1) {
    fprintf(stderr, "COMPUTE_SNES_CHECKSUM: Internal error: failed to round ROM size (%#x).\n", romsize);
    return FAILED;
  }
  mirror_begin = (mirror_end == romsize) ? romsize : mirror_end / 2;

  checksum = 0;
  for (i = 0; i < mirror_begin; i++) {
    if (snes_rom_mode == SNES_ROM_MODE_LOROM || snes_rom_mode == SNES_ROM_MODE_EXLOROM) {
      /* skip the checksum bytes */
      if (!(i == 0x7FDC || i == 0x7FDD || i == 0x7FDE || i == 0x7FDF))
        checksum += rom[i];
    }
    else {
      /* skip the checksum bytes */
      if (!(i == 0xFFDC || i == 0xFFDD || i == 0xFFDE || i == 0xFFDF))
        checksum += rom[i];
    }
  }
  for (i = mirror_begin; i < mirror_end; i++) {
    int index = (i - mirror_begin) % (romsize - mirror_begin) + mirror_begin;
    if (index >= romsize) {
      fprintf(stderr, "COMPUTE_SNES_CHECKSUM: Internal error: attempted to access byte %#x of ROM with size %#x.\n", index, romsize);
      return FAILED;
    }
    checksum += rom[index];
  }

  /* 2*255 (0x1FE) is for the checksum and its complement bytes that we skipped earlier */
  checksum += 2*255;

  /* compute the inverse checksum */
  inv = (checksum & 0xFFFF) ^ 0xFFFF;

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing SNES ROM checksum bytes");

  /* insert the checksum bytes */
  if (snes_rom_mode == SNES_ROM_MODE_LOROM || snes_rom_mode == SNES_ROM_MODE_EXLOROM) {
    mem_insert_allow_overwrite(0x7FDC, inv & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDD, (inv >> 8) & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDE, checksum & 0xFF, 1);
    mem_insert_allow_overwrite(0x7FDF, (checksum >> 8) & 0xFF, 1);
  }
  else {
    mem_insert_allow_overwrite(0xFFDC, inv & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDD, (inv >> 8) & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDE, checksum & 0xFF, 1);
    mem_insert_allow_overwrite(0xFFDF, (checksum >> 8) & 0xFF, 1);
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
    
  return SUCCEEDED;
}


int add_tmr_sega(void) {

  int tag_address = 0x7FF0;

  
  if (romsize < 0x4000)
    tag_address = 0x1FF0;
  else if (romsize < 0x8000)
    tag_address = 0x3FF0;

  if (romsize < 0x2000) {
    fprintf(stderr, "ADD_TMR_SEGA: A ROM of at least 8KBs is required.\n");
    return SUCCEEDED;
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing TMR SEGA");

  /* TMR SEGA */
  mem_insert_allow_overwrite(tag_address + 0x0, 0x54, 1);
  mem_insert_allow_overwrite(tag_address + 0x1, 0x4D, 1);
  mem_insert_allow_overwrite(tag_address + 0x2, 0x52, 1);
  mem_insert_allow_overwrite(tag_address + 0x3, 0x20, 1);
  mem_insert_allow_overwrite(tag_address + 0x4, 0x53, 1);
  mem_insert_allow_overwrite(tag_address + 0x5, 0x45, 1);
  mem_insert_allow_overwrite(tag_address + 0x6, 0x47, 1);
  mem_insert_allow_overwrite(tag_address + 0x7, 0x41, 1);

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
  return SUCCEEDED;
}


int compute_sms_checksum(void) {

  int tag_address = 0x7FF0, j, checksum, checksum_max = 32*1024, rom_size = 0;


  if (romsize < 0x2000) {
    fprintf(stderr, "COMPUTE_SMS_CHECKSUM: SMS/GG checksum computing requires a ROM of at least 8KBs.\n");
    return FAILED;
  }

  if (romsize < 16*1024)
    tag_address = 0x1FF0; /* 8KB */
  else if (romsize < 32*1024)
    tag_address = 0x3FF0; /* 16KB */
  else
    tag_address = 0x7FF0; /* 32KB+ */

  /* NOTE: this is one only if we have nothing written there as we've then reserved the byte (one write). if we had written to that
     previously then it would be > 1 as reserving it would increase the counter by one... */
  if (rom_usage[tag_address + 0xF] > 0) {
    /* we have the ROM size written in the header so let's use it */
    rom_size = rom[tag_address + 0xF] & 0xF;
    if (rom_size == 0x0A) {
      tag_address = 0x1FF0;
      checksum_max = 8*1024;
    }
    else if (rom_size == 0x0B) {
      tag_address = 0x3FF0;
      checksum_max = 16*1024;
    }
    else if (rom_size == 0x0C) {
      tag_address = 0x7FF0;
      checksum_max = 32*1024;
    }
    else if (rom_size == 0x0D) {
      tag_address = 0x7FF0;
      checksum_max = 48*1024;
    }
    else if (rom_size == 0x0E) {
      tag_address = 0x7FF0;
      checksum_max = 64*1024;
    }
    else if (rom_size == 0x0F) {
      tag_address = 0x7FF0;
      checksum_max = 128*1024;
    }
    else if (rom_size == 0x00) {
      tag_address = 0x7FF0;
      checksum_max = 256*1024;
    }
    else if (rom_size == 0x01) {
      tag_address = 0x7FF0;
      checksum_max = 512*1024;
    }
    else if (rom_size == 0x02) {
      tag_address = 0x7FF0;
      checksum_max = 1024*1024;
    }
    else {
      fprintf(stderr, "COMPUTE_SMS_CHECKSUM: Unsupported ROMSIZE $%x - not calculating the checksum.\n", rom_size);
      return SUCCEEDED;
    }

    if (checksum_max > romsize) {
      fprintf(stderr, "COMPUTE_SMS_CHECKSUM: Defined ROMSIZE $%x (%dKBs) is bigger than the ROM image (%dKBs) itself - not calculating the checksum.\n", rom_size, checksum_max / 1024, romsize / 1024);
      return FAILED;
    }
  }
  else {
    /* no ROM size defined in the header, calculate until the header */
    checksum_max = tag_address;
  }

  /* add together ROM SIZE minus SMS/GG header */
  checksum = 0;
  for (j = 0; j < checksum_max; j++) {
    if (j < tag_address || j >= tag_address + 0x10)
      checksum += rom[j];
  }

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "%s", "Writing SMS/GG ROM checksum bytes");
  
  mem_insert_allow_overwrite(tag_address + 0xA, checksum & 0xFF, 1);
  mem_insert_allow_overwrite(tag_address + 0xB, (checksum >> 8) & 0xFF, 1);

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(mem_insert_action, sizeof(mem_insert_action), "???");
  
  return SUCCEEDED;
}

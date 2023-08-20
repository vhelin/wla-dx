
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "memory.h"
#include "files.h"


/* read an integer from t */
#define READ_T (t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24)); t += 4;
#define READ_T_FINAL (t[3] + (t[2] << 8) + (t[1] << 16) + (t[0] << 24));

extern struct object_file *g_obj_first;
extern int g_emptyfill, g_sms_checksum, g_smstag_defined, g_snes_rom_mode, g_snes_rom_speed, g_smc_status, g_sms_header;
extern int g_gb_checksum, g_gb_complement_check, g_snes_checksum, g_snes_mode, g_snes_sramsize, g_sms_checksum_already_written;
extern int g_sms_checksum_size_defined, g_sms_checksum_size, g_smd_checksum, g_romheader_baseaddress;


int check_file_types(void) {

  struct object_file *o;
  
  o = g_obj_first;
  while (o != NULL) {
    if (strncmp((char *)o->data, "WLAl", 4) == 0)
      o->format = WLA_VERSION_OBJ;
    else if (strncmp((char *)o->data, "WLAI", 4) == 0)
      o->format = WLA_VERSION_LIB;
    else {
      fprintf(stderr, "CHECK_FILE_TYPES: File \"%s\" is of unknown format (\"%c%c%c%c\").\n", o->name, o->data[0], o->data[1], o->data[2], o->data[3]);
      return FAILED;
    }
    o = o->next;
  }

  return SUCCEEDED;
}


static char *_get_snes_rom_speed(int speed) {

  if (speed == 0)
    return "\"200ns\"";
  else
    return "\"120ns\"";
}


static char *_get_snes_rom_mode(int mode) {

  if (mode == SNES_ROM_MODE_LOROM)
    return "\"LoROM\"";
  else if (mode == SNES_ROM_MODE_HIROM)
    return "\"HiROM\"";
  else if (mode == SNES_ROM_MODE_EXHIROM)
    return "\"ExHiROM\"";
  else if (mode == SNES_ROM_MODE_EXLOROM)
    return "\"ExLoROM\"";
  else
    return "\"???\"";
}


int check_headers(void) {

  int count = 0, misc_bits, e;
  struct object_file *o;
  unsigned char *t;
  
  o = g_obj_first;
  while (o != NULL) {
    if (o->format == WLA_VERSION_OBJ) {
      int extr_bits, more_bits;

      /* object file */
      misc_bits = *(o->data + OBJ_MISC_BITS);
      more_bits = *(o->data + OBJ_MORE_BITS);
      extr_bits = *(o->data + OBJ_EXTR_BITS);
      t = o->data + OBJ_SMS_CHECKSUM_SIZE;

      if ((misc_bits & 128) != 0)
        o->cpu_65816 = YES;
      else
        o->cpu_65816 = NO;

      if ((extr_bits & 1) != 0)
        o->cpu_65ce02 = YES;
      else
        o->cpu_65ce02 = NO;
      
      if (((more_bits >> 7) & 1) != 0)
        o->little_endian = NO;
      else
        o->little_endian = YES;
      
      if (count == 0) {
        /* empty fill */
        g_emptyfill = *(o->data + OBJ_EMPTY_FILL);
        /* misc bits */
        g_sms_checksum = misc_bits & 1;
        g_snes_rom_mode = misc_bits & 2;
        g_snes_mode = misc_bits & 4;
        g_snes_rom_speed = misc_bits & 8;
        g_gb_checksum = misc_bits & 16;
        g_gb_complement_check = misc_bits & 32;
        g_snes_checksum = misc_bits & 64;
        /* more bits */
        g_smc_status = more_bits & 1;
        g_snes_sramsize = (more_bits >> 1) & 3;
        g_smstag_defined = (more_bits >> 3) & 1;
        g_sms_header = (more_bits >> 4) & 1;
        if (((more_bits >> 5) & 1) == 1)
          g_snes_rom_mode = SNES_ROM_MODE_EXHIROM;
        if (((more_bits >> 6) & 1) == 1)
          g_snes_rom_mode = SNES_ROM_MODE_EXLOROM;
        /* extr bits */
        g_sms_checksum_already_written = (extr_bits >> 1) & 1;
        g_sms_checksum_size_defined = (extr_bits >> 2) & 1;
        g_smd_checksum = (extr_bits >> 3) & 1;
        /* sms checksum size */
        g_sms_checksum_size = READ_T;
        /* ROM header base address */
        g_romheader_baseaddress = READ_T_FINAL;
      }
      else {
        e = *(o->data + OBJ_EMPTY_FILL);
        if (e != g_emptyfill) {
          printf("CHECK_HEADERS: Conflicting empty fill in file \"%s\". Old $%.2x, new $%.2x. Using $%.2x.\n", o->name, g_emptyfill, e, e);
          g_emptyfill = e;
        }

        if (g_sms_checksum == 0 && (misc_bits & 1) != 0)
          g_sms_checksum = misc_bits & 1;
        if (g_gb_checksum == 0 && (misc_bits & 16) != 0)
          g_gb_checksum = misc_bits & 16;
        if (g_gb_complement_check == 0 && (misc_bits & 32) != 0)
          g_gb_complement_check = misc_bits & 32;
        if (g_snes_checksum == 0 && (misc_bits & 64) != 0)
          g_snes_checksum = misc_bits & 64;

        if (g_smc_status == 0 && (more_bits & 1) != 0)
          g_smc_status = more_bits & 1;
        if (g_smstag_defined == 0 && (more_bits & 8) != 0)
          g_smstag_defined = more_bits & 8;
        if (g_sms_header == 0 && (more_bits & 16) != 0)
          g_sms_header = more_bits & 16;

        if (g_smd_checksum == 0 && ((extr_bits >> 3) & 1) != 0)
          g_smd_checksum = 1;
        
        e = (more_bits >> 1) & 3;
        if (g_snes_sramsize < e) {
          printf("CHECK_HEADERS: Conflicting SNES SRAM size in file \"%s\". Old $%.2x, new $%.2x. Using $%.2x as it's bigger.\n", o->name, g_snes_sramsize, e, e);
          g_snes_sramsize = e;
        }

        if (o->cpu_65816) {
          e = misc_bits & 8;
          if (g_snes_rom_speed != e) {
            printf("CHECK_HEADERS: Conflicting SNES ROM speed in file \"%s\". Old %s, new %s. Using %s.\n", o->name, _get_snes_rom_speed(g_snes_rom_speed), _get_snes_rom_speed(e), _get_snes_rom_speed(e));
            g_snes_rom_speed = e;
          }

          e = misc_bits & 4;
          if (g_snes_mode == 0 && e != 0) {
            /* previously snes_mode was 0, so here we get the real bits first time */
            g_snes_mode = e;
            g_snes_rom_mode = misc_bits & 2;
            if (((more_bits >> 5) & 1) == 1)
              g_snes_rom_mode = SNES_ROM_MODE_EXHIROM;
            if (((more_bits >> 6) & 1) == 1)
              g_snes_rom_mode = SNES_ROM_MODE_EXLOROM;
          }
          else if (g_snes_mode != 0 && e == 0) {
            /* don't do anything */
          }
          else {
            /* check that the SNES ROM mode is the same */
            e = misc_bits & 2;
            if (((more_bits >> 5) & 1) == 1)
              e = SNES_ROM_MODE_EXHIROM;
            if (((more_bits >> 6) & 1) == 1)
              e = SNES_ROM_MODE_EXLOROM;

            if (g_snes_rom_mode != e) {
              printf("CHECK_HEADERS: Conflicting SNES ROM mode in file \"%s\". Old %s, new %s. Using %s.\n", o->name, _get_snes_rom_mode(g_snes_rom_mode), _get_snes_rom_mode(e), _get_snes_rom_mode(e));
              g_snes_rom_mode = e;
            }
          }
        }

        if (g_sms_checksum_already_written == 0 && ((extr_bits >> 1) & 1) != 0)
          g_sms_checksum_already_written = (extr_bits >> 1) & 1;
        if (g_sms_checksum_size_defined == 0 && ((extr_bits >> 2) & 1) != 0) {
          g_sms_checksum_size_defined = (extr_bits >> 2) & 1;
          g_sms_checksum_size = READ_T_FINAL;
        }
        
        t += 4;
        e = READ_T_FINAL;
        if (e != -1)
          g_romheader_baseaddress = e;
      }

      count++;
    }
    else {
      /* library file */
      misc_bits = *(o->data + LIB_MISC_BITS);

      if ((misc_bits & 4) != 0)
        o->cpu_65ce02 = YES;
      else
        o->cpu_65ce02 = NO;

      if ((misc_bits & 2) != 0)
        o->cpu_65816 = YES;
      else
        o->cpu_65816 = NO;

      if ((misc_bits & 1) != 0)
        o->little_endian = NO;
      else
        o->little_endian = YES;
    }
    
    o = o->next;
  }

  if (count == 0) {
    fprintf(stderr, "CHECK_HEADERS: There's nothing to link.\n");
    return FAILED;
  }

  return SUCCEEDED;
}

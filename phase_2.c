
#include "flags.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "defines.h"
#include "phase_1.h"
#include "phase_2.h"
#include "phase_4.h"
#include "hashmap.h"
#include "printf.h"


#if defined(GB)
extern int g_licenseecodeold, g_romsize, g_romsize_defined;
extern int g_nintendologo_defined;
extern int g_name_defined, g_licenseecodeold_defined, g_licenseecodenew_defined;
extern int g_countrycode, g_countrycode_defined;
extern int g_version, g_version_defined, g_cartridgetype, g_cartridgetype_defined;
extern char g_name[32], g_licenseecodenew_c1, g_licenseecodenew_c2;

static unsigned char s_nintendo_logo_dat[] = {
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};
#endif

#if defined(Z80)
extern char *g_sdsctag_name_str, *g_sdsctag_notes_str, *g_sdsctag_author_str;
extern int g_sdsctag_name_type, g_sdsctag_notes_type, g_sdsctag_author_type, g_sdsc_ma, g_sdsc_mi;
extern int g_sdsctag_name_value, g_sdsctag_notes_value, g_sdsctag_author_value;
extern int g_computesmschecksum_defined, g_sdsctag_defined, g_smstag_defined;
extern int g_smsheader_defined, g_smsversion, g_smsversion_defined, g_smsregioncode, g_smsregioncode_defined;
extern int g_smsproductcode_defined, g_smsproductcode1, g_smsproductcode2, g_smsproductcode3, g_smsreservedspace1, g_smsreservedspace2;
extern int g_smsromsize, g_smsromsize_defined;
extern int g_smsforcechecksum, g_smsforcechecksum_defined, g_smschecksumsize, g_smschecksumsize_defined;
#endif

#if defined(W65816)
extern char g_name[32];
extern char g_snesid[4];
extern int g_snesid_defined, g_snesromsize;
extern int g_sramsize_defined, g_sramsize, g_country_defined, g_country;
extern int g_licenseecode_defined, g_licenseecode;
extern int g_version_defined, g_version, g_cartridgetype, g_cartridgetype_defined;
extern int g_hirom_defined, g_lorom_defined, g_slowrom_defined, g_fastrom_defined, g_snes_mode, g_name_defined;
extern int g_computesneschecksum_defined, g_exhirom_defined, g_exlorom_defined;
#endif

#if defined(MC68000)
extern char g_smdheader_systemtype[17];
extern char g_smdheader_copyright[17];
extern char g_smdheader_titledomestic[49];
extern char g_smdheader_titleoverseas[49];
extern char g_smdheader_serialnumber[15];
extern char g_smdheader_devicesupport[17];
extern int g_smdheader_romaddressrange_start, g_smdheader_romaddressrange_end;
extern int g_smdheader_ramaddressrange_start, g_smdheader_ramaddressrange_end;
extern char g_smdheader_extramemory_type_1[3];
extern int g_smdheader_extramemory_type_2, g_smdheader_extramemory_type_3, g_smdheader_extramemory_start, g_smdheader_extramemory_end;
extern char g_smdheader_modemsupport[13];
extern char g_smdheader_regionsupport[4];
extern int g_computesmdchecksum_defined, g_smdheader_defined;
#endif

extern FILE *g_file_out_ptr;
extern int g_rambanks, g_rambanks_defined, g_ifdef, g_romheader_baseaddress, g_romheader_baseaddress_defined;
extern int g_rombanks_defined, g_rombanks;
extern int g_output_format, g_romgbc, g_romsgb, g_romdmg, g_max_address;
extern int g_romtype, g_verbose_level, g_section_status, g_background_defined, g_memorymap_defined;
extern int g_emptyfill_defined, g_emptyfill, g_rombankmap_defined, g_section_id;
extern unsigned char *g_rom_banks, *g_rom_banks_usage_table;
extern char g_mem_insert_action[MAX_NAME_LENGTH*3 + 1024];
extern struct section_def *g_sections_first, *g_sections_last, *g_sec_tmp, *g_sec_next;

static char s_include_directives_name[] = "INCLUDE_DIRECTIVES:";


#if defined(W65816)

void _write_snes_cartridge_information(int start) {

  int info_bits;

  info_bits = 32;
  if (g_hirom_defined != 0)
    info_bits |= 1;
  else if (g_exhirom_defined != 0)
    info_bits |= (1 << 2) | 1;
  else if (g_exlorom_defined != 0)
    info_bits |= (1 << 1);

  if (g_fastrom_defined != 0)
    info_bits += 16;

  /* create a what-we-are-doing message for mem_insert*() warnings/errors */
  snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing SNES ROM information");

  mem_insert_absolute(start, info_bits);
  
  if (g_cartridgetype_defined != 0) 
    mem_insert_absolute(start + 1, g_cartridgetype);

  if (g_snesromsize != 0) 
    mem_insert_absolute(start + 2, g_snesromsize);

  if (g_sramsize_defined != 0) 
    mem_insert_absolute(start + 3, g_sramsize);

  if (g_country_defined != 0)
    mem_insert_absolute(start + 4, g_country);

  if (g_licenseecode_defined != 0)
    mem_insert_absolute(start + 5, g_licenseecode);

  if (g_version_defined != 0)
    mem_insert_absolute(start + 6, g_version);
  
  /* snes cartridge ID */
  if (g_snesid_defined != 0) {
    int i;
    
    for (i = 0; i < 4; i++) 
      mem_insert_absolute(start - (0xD5 - 0xB2) + i, g_snesid[i]);
  }
      
  /* snes cartridge name */
  if (g_name_defined != 0) {
    int i;
    
    for (i = 0; i < 21; i++)
      mem_insert_absolute(start - (0xD5 - 0xC0) + i, g_name[i]);
  }
}

#endif


int phase_2(void) {

  if (g_ifdef > 0) {
    fprintf(stderr, "PHASE_2: %x x .ENDIFs are missing.\n", g_ifdef);
    return FAILED;
  }

  if (g_verbose_level >= 100)
    printf("Directive checks...\n");

  if (g_section_status == ON) {
    fprintf(stderr, "PHASE_2: %s The section \"%s\" was not closed.\n", s_include_directives_name, g_sections_last->name);
    return FAILED;
  }

  if (g_output_format != OUTPUT_LIBRARY && g_rombanks_defined == 0 && g_rombankmap_defined == 0) {
    fprintf(stderr, "PHASE_2: %s ROMBANKS/ROMBANKMAP wasn't defined.\n", s_include_directives_name);
    return FAILED;
  }

#if defined(Z80)
  /* SMSTAG */
  if (g_smstag_defined != 0) {
    /* OBSOLETE: MOVED TO wlalink/compute.c/compute_sms_checksum()
       mem_insert_absolute(0x7FF0, 0x54);
       mem_insert_absolute(0x7FF1, 0x4D);
       mem_insert_absolute(0x7FF2, 0x52);
       mem_insert_absolute(0x7FF3, 0x20);
       mem_insert_absolute(0x7FF4, 0x53);
       mem_insert_absolute(0x7FF5, 0x45);
       mem_insert_absolute(0x7FF6, 0x47);
       mem_insert_absolute(0x7FF7, 0x41);
    */
  }

  /* SMSHEADER */
  if (g_smsheader_defined != 0) {
    int tag_address = 0x7FF0;
    int rs, rc = 4;

    if (g_smsromsize_defined != 0)
      rs = g_smsromsize;
    else {
      /* try to calculate the correct romsize value */
      if (g_max_address < 16*1024)
        rs = 0xA; /* 8KB */
      else if (g_max_address < 32*1024)
        rs = 0xB; /* 16KB */
      else
        rs = 0xC; /* 32KB+ */
    }

    if (g_max_address < 0x4000) {
      /* let's assume it's a 8KB ROM */
      tag_address = 0x1FF0;
    }
    else if (g_max_address < 0x8000) {
      /* let's assume it's a 16KB ROM */
      tag_address = 0x3FF0;
    }

    if (g_smsregioncode_defined != 0)
      rc = g_smsregioncode;

    /* BASEADDRESS override */
    if (g_romheader_baseaddress_defined != 0) {
      if (g_romheader_baseaddress + 16 > g_max_address) {
        fprintf(stderr, "PHASE_2: Baseaddress ($%x) for SMS ROM header is too large, it overflows from the ROM ($%x bytes)!\n", g_romheader_baseaddress, g_max_address);
        return FAILED;
      }
      tag_address = g_romheader_baseaddress;
    }
    
    /* create a what-we-are-doing message for mem_insert*() warnings/errors */
    snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing SMS ROM header bytes");

    mem_insert_absolute(tag_address + 0x8, g_smsreservedspace1);
    mem_insert_absolute(tag_address + 0x9, g_smsreservedspace2);
    mem_insert_absolute(tag_address + 0xC, g_smsproductcode1);
    mem_insert_absolute(tag_address + 0xD, g_smsproductcode2);
    mem_insert_absolute(tag_address + 0xE, (g_smsproductcode3 << 4) | g_smsversion);
    mem_insert_absolute(tag_address + 0xF, (rc << 4) | rs);

    /* force checksum? */
    if (g_smsforcechecksum_defined != 0) {
      mem_insert_absolute(tag_address + 0xA, g_smsforcechecksum & 0xff);
      mem_insert_absolute(tag_address + 0xB, (g_smsforcechecksum >> 8) & 0xff);
    }
  }

  /* SDSCTAG */
  if (g_sdsctag_defined != 0) {
    struct tm *ti;
    time_t t;
    int da, mo, ye_l, ye_h, ye, x, q;
    
    /* insert string pointers */
    if (create_a_new_section_structure() == FAILED)
      return FAILED;
    strcpy(g_sec_tmp->name, "!__WLA_SDSCTAG_STRINGS");
    g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;
    fprintf(g_file_out_ptr, "A%d %d ", g_sec_tmp->id, 0x7FEA);

    /* insert the system line (0) */
    fprintf(g_file_out_ptr, "k0 ");

    /* the data */
    if (g_sdsctag_author_type == TYPE_VALUE)
      fprintf(g_file_out_ptr, "y%d ", g_sdsctag_author_value);
    else if (g_sdsctag_author_type == TYPE_LABEL)
      fprintf(g_file_out_ptr, "r%s ", g_sdsctag_author_str);
    else if (g_sdsctag_author_type == TYPE_STACK_CALCULATION)
      fprintf(g_file_out_ptr, "C%d ", g_sdsctag_author_value);
    else
      fprintf(g_file_out_ptr, "r *WLA_SDSC_PRG_AUTHOR_PTR ");

    if (g_sdsctag_name_type == TYPE_VALUE)
      fprintf(g_file_out_ptr, "y%d ", g_sdsctag_name_value);
    else if (g_sdsctag_name_type == TYPE_LABEL)
      fprintf(g_file_out_ptr, "r%s ", g_sdsctag_name_str);
    else if (g_sdsctag_name_type == TYPE_STACK_CALCULATION)
      fprintf(g_file_out_ptr, "C%d ", g_sdsctag_name_value);
    else
      fprintf(g_file_out_ptr, "r *WLA_SDSC_PRG_NAME_PTR ");

    if (g_sdsctag_notes_type == TYPE_VALUE)
      fprintf(g_file_out_ptr, "y%d ", g_sdsctag_notes_value);
    else if (g_sdsctag_notes_type == TYPE_LABEL)
      fprintf(g_file_out_ptr, "r%s ", g_sdsctag_notes_str);
    else if (g_sdsctag_notes_type == TYPE_STACK_CALCULATION)
      fprintf(g_file_out_ptr, "C%d ", g_sdsctag_notes_value);
    else
      fprintf(g_file_out_ptr, "r *WLA_SDSC_PRG_NOTES_PTR ");

    fprintf(g_file_out_ptr, "s ");

    /* create string sections */
    if (g_sdsctag_author_type == TYPE_STRING) {
      if (create_a_new_section_structure() == FAILED)
        return FAILED;
      strcpy(g_sec_tmp->name, "!__WLA_SDSCTAG_STRING_AUTHOR");
      g_sec_tmp->status = SECTION_STATUS_SEMIFREE;

      fprintf(g_file_out_ptr, "B0 0 O1 S%d L *WLA_SDSC_PRG_AUTHOR_PTR ", g_sec_tmp->id);
      for (q = 0; q < (int)strlen(g_sdsctag_author_str); q++)
        fprintf(g_file_out_ptr, "d%d ", g_sdsctag_author_str[q]);
      fprintf(g_file_out_ptr, "d0 s ");
    }
    if (g_sdsctag_name_type == TYPE_STRING) {
      if (create_a_new_section_structure() == FAILED)
        return FAILED;
      strcpy(g_sec_tmp->name, "!__WLA_SDSCTAG_STRING_NAME");
      g_sec_tmp->status = SECTION_STATUS_SEMIFREE;
      
      fprintf(g_file_out_ptr, "B0 0 O1 S%d L *WLA_SDSC_PRG_NAME_PTR ", g_sec_tmp->id);
      for (q = 0; q < (int)strlen(g_sdsctag_name_str); q++)
        fprintf(g_file_out_ptr, "d%d ", g_sdsctag_name_str[q]);
      fprintf(g_file_out_ptr, "d0 s ");
    }
    if (g_sdsctag_notes_type == TYPE_STRING) {
      if (create_a_new_section_structure() == FAILED)
        return FAILED;
      strcpy(g_sec_tmp->name, "!__WLA_SDSCTAG_STRING_NOTES");
      g_sec_tmp->status = SECTION_STATUS_SEMIFREE;

      fprintf(g_file_out_ptr, "B0 0 O1 S%d L *WLA_SDSC_PRG_NOTES_PTR ", g_sec_tmp->id);
      for (q = 0; q < (int)strlen(g_sdsctag_notes_str); q++)
        fprintf(g_file_out_ptr, "d%d ", g_sdsctag_notes_str[q]);
      fprintf(g_file_out_ptr, "d0 s ");
    }

    /* create the time and date data */
    time(&t);
    ti = localtime(&t);
    da = ti->tm_mday;
    mo = ti->tm_mon + 1;
    ye = ti->tm_year + 1900;

    /* BCD the values */
    q = (g_sdsc_ma / 10);
    g_sdsc_ma = (q << 4) | (g_sdsc_ma - (q * 10));
    q = (g_sdsc_mi / 10);
    g_sdsc_mi = (q << 4) | (g_sdsc_mi - (q * 10));
    q = (da / 10);
    da = (q << 4) | (da - (q * 10));
    q = (mo / 10);
    mo = (q << 4) | (mo - (q * 10));

    q = (ye / 1000);
    ye -= (q * 1000);
    x = (ye / 100);
    ye -= (x * 100);
    ye_h = (q << 4) | x;

    q = (ye / 10);
    ye -= (q * 10);
    ye_l = (q << 4) | ye;

    if (create_a_new_section_structure() == FAILED)
      return FAILED;
    strcpy(g_sec_tmp->name, "!__WLA_SDSCTAG_TIMEDATE");
    g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;
    fprintf(g_file_out_ptr, "A%d %d ", g_sec_tmp->id, 0x7FE0);

    /* insert the system line (0) */
    fprintf(g_file_out_ptr, "k0 ");

    /* SDSC header data */
    /* SDSC */
    fprintf(g_file_out_ptr, "d%d ", 0x53);
    fprintf(g_file_out_ptr, "d%d ", 0x44);
    fprintf(g_file_out_ptr, "d%d ", 0x53);
    fprintf(g_file_out_ptr, "d%d ", 0x43);
    /* version */
    fprintf(g_file_out_ptr, "d%d ", g_sdsc_ma);
    fprintf(g_file_out_ptr, "d%d ", g_sdsc_mi);
    /* date */
    fprintf(g_file_out_ptr, "d%d ", da);
    fprintf(g_file_out_ptr, "d%d ", mo);
    fprintf(g_file_out_ptr, "d%d ", ye_l);
    fprintf(g_file_out_ptr, "d%d ", ye_h);
    
    fprintf(g_file_out_ptr, "s ");

    /*
      mem_insert_absolute(0x7FE0, 0x53);
      mem_insert_absolute(0x7FE1, 0x44);
      mem_insert_absolute(0x7FE2, 0x53);
      mem_insert_absolute(0x7FE3, 0x43);
      mem_insert_absolute(0x7FE4, g_sdsc_ma);
      mem_insert_absolute(0x7FE5, g_sdsc_mi);
      mem_insert_absolute(0x7FE6, da);
      mem_insert_absolute(0x7FE7, mo);
      mem_insert_absolute(0x7FE8, ye_l);
      mem_insert_absolute(0x7FE9, ye_h);
    */
  }
#endif

#if defined(MC68000)
  /* SMDHEADER */
  if (g_smdheader_defined == YES) {
    int i;
    
    /* create a section that contains half of the header data */
    if (create_a_new_section_structure() == FAILED)
      return FAILED;
    strcpy(g_sec_tmp->name, "!__WLA_SMDHEADER_PART_1");
    g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;
    fprintf(g_file_out_ptr, "A%d %d ", g_sec_tmp->id, 0x100);

    /* insert the system line (0) */
    fprintf(g_file_out_ptr, "k0 ");

    for (i = 0; i < 16; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_systemtype[i]);
    for (i = 0; i < 16; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_copyright[i]);
    for (i = 0; i < 48; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_titledomestic[i]);
    for (i = 0; i < 48; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_titleoverseas[i]);
    for (i = 0; i < 14; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_serialnumber[i]);
    
    fprintf(g_file_out_ptr, "s ");

    /* create a section that contains half of the header data */
    if (create_a_new_section_structure() == FAILED)
      return FAILED;
    strcpy(g_sec_tmp->name, "!__WLA_SMDHEADER_PART_2");
    g_sec_tmp->status = SECTION_STATUS_ABSOLUTE;
    fprintf(g_file_out_ptr, "A%d %d ", g_sec_tmp->id, 0x190);

    /* insert the system line (0) */
    fprintf(g_file_out_ptr, "k0 ");

    for (i = 0; i < 16; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_devicesupport[i]);
    fprintf(g_file_out_ptr, "u%d ", g_smdheader_romaddressrange_start);
    if (g_smdheader_romaddressrange_end < 0)
      fprintf(g_file_out_ptr, "u%d ", g_max_address - 1);
    else
      fprintf(g_file_out_ptr, "u%d ", g_smdheader_romaddressrange_end);
    fprintf(g_file_out_ptr, "u%d ", g_smdheader_ramaddressrange_start);
    fprintf(g_file_out_ptr, "u%d ", g_smdheader_ramaddressrange_end);
    for (i = 0; i < 2; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_extramemory_type_1[i]);
    fprintf(g_file_out_ptr, "d%d ", g_smdheader_extramemory_type_2);
    fprintf(g_file_out_ptr, "d%d ", g_smdheader_extramemory_type_3);
    fprintf(g_file_out_ptr, "u%d ", g_smdheader_extramemory_start);
    fprintf(g_file_out_ptr, "u%d ", g_smdheader_extramemory_end);
    for (i = 0; i < 12; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_modemsupport[i]);
    for (i = 0; i < 40; i++)
      fprintf(g_file_out_ptr, "d%d ", ' ');
    for (i = 0; i < 3; i++)
      fprintf(g_file_out_ptr, "d%d ", g_smdheader_regionsupport[i]);
    for (i = 0; i < 13; i++)
      fprintf(g_file_out_ptr, "d%d ", ' ');
    
    fprintf(g_file_out_ptr, "s ");

  }
#endif

#if defined(W65816)
  if (g_output_format != OUTPUT_LIBRARY) {
    /* snes cartridge information */
    if (g_snes_mode != 0) {
      if (g_hirom_defined != 0)
        _write_snes_cartridge_information(0xFFD5);
      else if (g_exhirom_defined != 0) {
        _write_snes_cartridge_information(0x40FFD5);
        /* mirror the info */
        _write_snes_cartridge_information(0xFFD5);
      }
      else
        _write_snes_cartridge_information(0x7FD5);
    }
  }
#endif

#if defined(GB)
  /* insert the descriptive data (not in library files) */
  if (g_output_format == OUTPUT_OBJECT) {
    if (g_nintendologo_defined != 0) {
      int nl1 = 0x104;
      unsigned int nl2 = 0;
      
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM Nintendo logo bytes");

      while (nl2 < sizeof(s_nintendo_logo_dat)) {
        mem_insert_absolute(nl1, s_nintendo_logo_dat[nl2]);
        nl1++;
        nl2++;
      }
    }
    
    if (g_romgbc == 1) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM type: GB Color compatible");

      mem_insert_absolute(323, 0x80);
    }
    else if (g_romgbc == 2) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM type: GB Color only");

      mem_insert_absolute(323, 0xc0);
    }
    if (g_romdmg != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM type: GB original");

      mem_insert_absolute(326, 0);
    }
    if (g_romsgb != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM type: Super GB");

      mem_insert_absolute(326, 3);
    }
    
    if (g_rambanks_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB RAM size");

      mem_insert_absolute(329, g_rambanks);
    }
    if (g_rombanks_defined != 0 && g_romsize_defined != 0) {
      int romsize = g_romsize;

      if (romsize < 0) {
        int b = g_rombanks;

        if (b == 2)
          romsize = 0;
        else if (b == 4)
          romsize = 1;
        else if (b == 8)
          romsize = 2;
        else if (b == 16)
          romsize = 3;
        else if (b == 32)
          romsize = 4;
        else if (b == 64)
          romsize = 5;
        else if (b == 72)
          romsize = 0x52;
        else if (b == 80)
          romsize = 0x53;
        else if (b == 96)
          romsize = 0x54;
        else if (b == 128)
          romsize = 6;
        else if (b == 256)
          romsize = 7;
        else if (b == 512)
          romsize = 8;
        else {
          if (b <= 2)
            romsize = 0;
          else if (b <= 4)
            romsize = 1;
          else if (b <= 8)
            romsize = 2;
          else if (b <= 16)
            romsize = 3;
          else if (b <= 32)
            romsize = 4;
          else if (b <= 64)
            romsize = 5;
          else if (b <= 72)
            romsize = 0x52;
          else if (b <= 80)
            romsize = 0x53;
          else if (b <= 96)
            romsize = 0x54;
          else if (b <= 128)
            romsize = 6;
          else if (b <= 256)
            romsize = 7;
          else /* if (b <= 512) */
            romsize = 8;
          fprintf(stderr, "PHASE_2: WARNING: The number of ROM banks (%d) is not officially supported. Setting ROM size indicator byte at $0148 to %X...\n", b, romsize);
        }
      }
      
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM size");

      mem_insert_absolute(328, romsize);
    }
    if (g_cartridgetype_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB cartridge type");

      mem_insert_absolute(327, g_cartridgetype);
    }
    if (g_licenseecodeold_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM licensee code (old)");

      mem_insert_absolute(331, g_licenseecodeold);
      mem_insert_absolute(324, 0);
      mem_insert_absolute(325, 0);
    }
    if (g_licenseecodenew_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM licensee code (new)");

      mem_insert_absolute(331, 51);
      mem_insert_absolute(324, g_licenseecodenew_c1);
      mem_insert_absolute(325, g_licenseecodenew_c2);
    }
    if (g_countrycode_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM country code");

      mem_insert_absolute(330, g_countrycode);
    }
    if (g_name_defined != 0) {
      int i, length;
      
      if (g_romgbc != 0)
        length = 15;
      else
        length = 16;

      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM name");

      for (i = 0; i < length; i++)
        mem_insert_absolute(308 + i, g_name[i]);
    }
    if (g_version_defined != 0) {
      /* create a what-we-are-doing message for mem_insert*() warnings/errors */
      snprintf(g_mem_insert_action, sizeof(g_mem_insert_action), "Writing GB ROM version");

      mem_insert_absolute(332, g_version);
    }
  }
#endif

  /* clear the mem_insert*() warnings/errors buffer */
  g_mem_insert_action[0] = 0;

  return SUCCEEDED;
}


int create_a_new_section_structure(void) {

  g_sec_tmp = calloc(sizeof(struct section_def), 1);
  if (g_sec_tmp == NULL) {
    print_error(ERROR_DIR, "Out of memory while allocating room for a new SECTION.\n");
    return FAILED;
  }

  g_sec_tmp->name[0] = 0;
  g_sec_tmp->alignment = 1;
  g_sec_tmp->offset = 0;
  g_sec_tmp->priority = 0;
  g_sec_tmp->address = -1;
  g_sec_tmp->address_from_dsp = -1;
  g_sec_tmp->keep = NO;
  g_sec_tmp->bank = -1;
  g_sec_tmp->slot = -1;
  g_sec_tmp->base = 0;
  g_sec_tmp->size = 0;
  g_sec_tmp->status = 0;
  g_sec_tmp->bitwindow = 0;
  g_sec_tmp->window_start = -1;
  g_sec_tmp->window_end = -1;
  g_sec_tmp->alive = YES;
  g_sec_tmp->id = g_section_id++;
  g_sec_tmp->i = 0;
  g_sec_tmp->filename_id = 0;
  g_sec_tmp->advance_org = NO;
  g_sec_tmp->maxsize_status = OFF;
  g_sec_tmp->maxsize = 0;
  g_sec_tmp->listfile_items = 0;
  g_sec_tmp->listfile_ints = NULL;
  g_sec_tmp->listfile_cmds = NULL;
  g_sec_tmp->data = NULL;
  g_sec_tmp->nspace = NULL;
  g_sec_tmp->label_map = hashmap_new();
  g_sec_tmp->next = NULL;

  if (g_sections_first == NULL) {
    g_sections_first = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }
  else {
    g_sections_last->next = g_sec_tmp;
    g_sections_last = g_sec_tmp;
  }

  return SUCCEEDED;
}

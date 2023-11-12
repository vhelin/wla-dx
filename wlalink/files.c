
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "parse.h"
#include "files.h"
#include "analyze.h"
#include "write.h"
#include "main.h"

#if defined(AMIGA)
#include "/printf.h"
#else
#include "../printf.h"
#endif


extern struct object_file *g_obj_first, *g_obj_last, *g_obj_tmp;
extern struct label *g_labels_first, *g_labels_last;
extern unsigned char *g_file_header, *g_file_footer;
extern char g_ext_libdir[MAX_NAME_LENGTH + 1];
extern int g_file_header_size, g_file_footer_size, g_use_libdir, g_paths_in_linkfile_are_relative_to_linkfile;
extern struct after_section *g_after_sections, *g_after_tmp;
extern struct section_fix *g_sec_fix_first, *g_sec_fix_tmp;

struct object_file **g_object_files = NULL;

static int s_object_files_max = -1, s_object_files_array_max = 0;

int g_section_write_order[SECTION_TYPES_COUNT-2] = {
  SECTION_STATUS_FORCE,
  SECTION_STATUS_SEMISUPERFREE,
  SECTION_STATUS_SEMISUBFREE,
  SECTION_STATUS_SEMIFREE,
  SECTION_STATUS_FREE,
  SECTION_STATUS_SUPERFREE,
  SECTION_STATUS_OVERWRITE
};

static int s_section_write_order_index = 0;

int g_ramsection_write_order[RAMSECTION_TYPES_COUNT] = {
  SECTION_STATUS_RAM_FORCE,
  SECTION_STATUS_RAM_SEMISUBFREE,
  SECTION_STATUS_RAM_SEMIFREE,
  SECTION_STATUS_RAM_FREE
};

static int s_ramsection_write_order_index = 0;


static void _process_tmp(char *tmp) {

  int i;
  
  for (i = 0; !(tmp[i] == 0x0D || tmp[i] == 0x0A || tmp[i] == 0x00); i++)
    ;
  tmp[i] = 0;
}


int load_files(char *argv[], int argc) {

  int state = STATE_NONE, i, x, line, bank, slot, base, bank_defined, slot_defined, base_defined, n, alignment, offset;
  int org_defined, org, orga_defined, orga, status_defined, status, priority_defined, priority, appendto_defined, keep_defined;
  int alignment_defined, offset_defined, after_defined, bitwindow_defined, window_defined, size, size_defined, banks_defined;
  int bitwindow, window_start, window_end, sectionwriteorder_defined = NO, ramsectionwriteorder_defined = NO;
  char tmp[1024], token[1024], tmp_token[1024 + MAX_NAME_LENGTH + 2], slot_name[MAX_NAME_LENGTH + 1], state_name[32], appendto_name[MAX_NAME_LENGTH + 1], after_name[MAX_NAME_LENGTH + 1], linkfile_path[MAX_NAME_LENGTH + 1], banks[MAX_NAME_LENGTH + 1];
  struct label *l;
  FILE *fop, *f;

  if (g_paths_in_linkfile_are_relative_to_linkfile == YES) {
    /* extract the path from linkfile */
    snprintf(linkfile_path, sizeof(linkfile_path), "%s", argv[argc - 2]);
    for (i = (int)strlen(linkfile_path) - 1; i >= 0; i--) {
      if (linkfile_path[i] == '\\' || linkfile_path[i] == '/')
        break;
    }
    linkfile_path[i + 1] = 0;
  }
  
  fop = fopen(argv[argc - 2], "rb");
  if (fop == NULL) {
    print_text(NO, "LOAD_FILES: Could not open file \"%s\".\n", argv[argc - 2]);
    return FAILED;
  }

  line = 0;
  while (fgets(tmp, 255, fop) != NULL) {
    line++;
    x = 0;

    if (tmp[0] == ';' || tmp[0] == '*' || tmp[0] == '#' || tmp[0] == 0x0D || tmp[0] == 0x0A)
      continue;

    /* remove garbage from the end */
    _process_tmp(tmp);

    /* empty line check */
    if (get_next_token(tmp, token, &x) == FAILED)
      continue;

    /* first checks */
    if (token[0] == '[') {
      if (strcaselesscmp(token, "[objects]") == 0) {
        state = STATE_OBJECT;
        continue;
      }
      else if (strcaselesscmp(token, "[libraries]") == 0) {
        state = STATE_LIBRARY;
        continue;
      }
      else if (strcaselesscmp(token, "[header]") == 0) {
        state = STATE_HEADER;
        continue;
      }
      else if (strcaselesscmp(token, "[footer]") == 0) {
        state = STATE_FOOTER;
        continue;
      }
      else if (strcaselesscmp(token, "[definitions]") == 0) {
        state = STATE_DEFINITION;
        continue;
      }
      else if (strcaselesscmp(token, "[ramsections]") == 0) {
        state = STATE_RAMSECTIONS;
        strcpy(state_name, "RAM section");
        continue;
      }
      else if (strcaselesscmp(token, "[sections]") == 0) {
        state = STATE_SECTIONS;
        strcpy(state_name, "section");
        continue;
      }
      else if (strcaselesscmp(token, "[sectionwriteorder]") == 0) {
        state = STATE_SECTION_WRITE_ORDER;
        continue;
      }
      else if (strcaselesscmp(token, "[ramsectionwriteorder]") == 0) {
        state = STATE_RAMSECTION_WRITE_ORDER;
        continue;
      }
      else {
        print_text(NO, "%s:%d LOAD_FILES: Unknown group \"%s\".\n", argv[argc - 2], line, token);
        fclose(fop);
        return FAILED;
      }
    }

    if (state == STATE_NONE) {
      print_text(NO, "%s:%d: LOAD_FILES: Before file \"%s\" can be loaded you must define a group for it.\n", argv[argc - 2], line, token);
      fclose(fop);
      return FAILED;
    }

    bank_defined = NO;
    banks_defined = NO;
    slot_defined = NO;
    base_defined = NO;
    orga_defined = NO;
    org_defined = NO;
    status_defined = NO;
    priority_defined = NO;
    appendto_defined = NO;
    after_defined = NO;
    keep_defined = NO;
    alignment_defined = NO;
    offset_defined = NO;
    bitwindow_defined = NO;
    window_defined = NO;
    size_defined = NO;
    size = 0;
    bank = 0;
    slot = 0;
    base = 0;
    orga = 0;
    org = 0;
    status = -1;
    priority = 0;
    alignment = 0;
    offset = 0;
    bitwindow = 0;
    window_start = -1;
    window_end = -1;

    /* definitions? */
    if (state == STATE_DEFINITION) {
      l = calloc(1, sizeof(struct label));
      if (l == NULL) {
        print_text(NO, "LOAD_FILES: Out of memory.\n");
        return FAILED;
      }
      strcpy(l->name, token);
      l->status = LABEL_STATUS_DEFINE;
      l->bank = 0;
      l->slot = 0;
      l->base = 0;
      l->alive = YES;

      if (get_next_number(&tmp[x], &n, &x) == FAILED) {
        int type;
        
        if (get_next_string(&tmp[x], l->string, &x, &type) == FAILED) {
          print_text(NO, "%s:%d: LOAD_FILES: Error in DEFINITION value.\n", argv[argc - 2], line);
          fclose(fop);
          free(l);
          return FAILED;
        }

        l->status = type;
        l->address = 0;
        l->rom_address = 0;
        add_label(l);
        continue;
      }

      l->address = n;
      l->rom_address = n;
      add_label(l);
      continue;
    }
    /* header loading? */
    else if (state == STATE_HEADER) {
      if (g_file_header != NULL) {
        print_text(NO, "%s:%d: LOAD_FILES: There can be only one header file.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }

      if (load_file_data(token, &g_file_header, &g_file_header_size) == FAILED) {
        fclose(fop);
        return FAILED;
      }
      if (get_next_token(&tmp[x], token, &x) == FAILED)
        continue;

      print_text(NO, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
      fclose(fop);
      return FAILED;
    }
    /* footer loading? */
    else if (state == STATE_FOOTER) {
      if (g_file_footer != NULL) {
        print_text(NO, "%s:%d: LOAD_FILES: There can be only one footer file.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }

      if (load_file_data(token, &g_file_footer, &g_file_footer_size) == FAILED) {
        fclose(fop);
        return FAILED;
      }
      if (get_next_token(&tmp[x], token, &x) == FAILED)
        continue;

      print_text(NO, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
      fclose(fop);
      return FAILED;
    }
    /* section write order? */
    else if (state == STATE_SECTION_WRITE_ORDER) {
      int statuses[SECTION_TYPES_COUNT_ALL], sum, j;

      if (s_section_write_order_index > 0) {
        print_text(NO, "%s:%d: LOAD_FILES: Excess data in [sectionwriteorder].\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      if (sectionwriteorder_defined == YES) {
        print_text(NO, "%s:%d: LOAD_FILES: [sectionwriteorder] was defined twice.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      for (i = 0; i < SECTION_TYPES_COUNT_ALL; i++)
        statuses[i] = 0;
      
      j = SUCCEEDED;

      while (j == SUCCEEDED) {
        int status;

        if (strcaselesscmp(token, "force") == 0)
          status = SECTION_STATUS_FORCE;
        else if (strcaselesscmp(token, "semisuperfree") == 0)
          status = SECTION_STATUS_SEMISUPERFREE;
        else if (strcaselesscmp(token, "semisubfree") == 0)
          status = SECTION_STATUS_SEMISUBFREE;
        else if (strcaselesscmp(token, "semifree") == 0)
          status = SECTION_STATUS_SEMIFREE;
        else if (strcaselesscmp(token, "free") == 0)
          status = SECTION_STATUS_FREE;
        else if (strcaselesscmp(token, "superfree") == 0)
          status = SECTION_STATUS_SUPERFREE;
        else if (strcaselesscmp(token, "overwrite") == 0)
          status = SECTION_STATUS_OVERWRITE;
        else {
          if (strlen(token) > 0 && token[0] != '[') {
            print_text(NO, "%s:%d: LOAD_FILES: Unknown token \"%s\" in [sectionwriteorder].\n", argv[argc - 2], line, token);
            fclose(fop);
            return FAILED;
          }
          break;
        }

        if (statuses[status] != 0) {
          print_text(NO, "%s:%d: LOAD_FILES: \"%s\" was already defined.\n", argv[argc - 2], line, token);
          fclose(fop);
          return FAILED;
        }

        statuses[status] = 1;

        if (s_section_write_order_index >= SECTION_TYPES_COUNT-2) {
          print_text(NO, "%s:%d: LOAD_FILES: Too many section types in [sectionwriteorder].\n", argv[argc - 2], line);
          fclose(fop);
          return FAILED;
        }

        g_section_write_order[s_section_write_order_index++] = status;

        if (s_section_write_order_index >= SECTION_TYPES_COUNT-2)
          break;

        if (fgets(tmp, 255, fop) == NULL) {
          print_text(NO, "%s:%d: LOAD_FILES: Out of lines in [sectionwriteorder].\n", argv[argc - 2], line);
          fclose(fop);
          return FAILED;          
        }
        
        line++;

        /* remove garbage from the end */
        _process_tmp(tmp);
        
        x = 0;
        j = get_next_token(tmp, token, &x);
      }
      
      /* count the types */
      sum = 0;
      for (i = 0; i < SECTION_TYPES_COUNT_ALL; i++) {
        if (statuses[i] != 0)
          sum++;
      }

      if (sum != SECTION_TYPES_COUNT-2) {
        print_text(NO, "%s:%d: LOAD_FILES: Not enough section types (all must be given) in [sectionwriteorder].\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }

      sectionwriteorder_defined = YES;
    }
    /* ramsection write order? */
    else if (state == STATE_RAMSECTION_WRITE_ORDER) {
      int statuses[SECTION_TYPES_COUNT_ALL], sum, j;

      if (s_ramsection_write_order_index > 0) {
        print_text(NO, "%s:%d: LOAD_FILES: Excess data in [ramsectionwriteorder].\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      if (ramsectionwriteorder_defined == YES) {
        print_text(NO, "%s:%d: LOAD_FILES: [ramsectionwriteorder] was defined twice.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      for (i = 0; i < SECTION_TYPES_COUNT_ALL; i++)
        statuses[i] = 0;
      
      j = SUCCEEDED;

      while (j == SUCCEEDED) {
        int status;

        if (strcaselesscmp(token, "force") == 0)
          status = SECTION_STATUS_RAM_FORCE;
        else if (strcaselesscmp(token, "semisubfree") == 0)
          status = SECTION_STATUS_RAM_SEMISUBFREE;
        else if (strcaselesscmp(token, "semifree") == 0)
          status = SECTION_STATUS_RAM_SEMIFREE;
        else if (strcaselesscmp(token, "free") == 0)
          status = SECTION_STATUS_RAM_FREE;
        else {
          if (strlen(token) > 0 && token[0] != '[') {
            print_text(NO, "%s:%d: LOAD_FILES: Unknown token \"%s\" in [ramsectionwriteorder].\n", argv[argc - 2], line, token);
            fclose(fop);
            return FAILED;
          }
          break;
        }

        if (statuses[status] != 0) {
          print_text(NO, "%s:%d: LOAD_FILES: \"%s\" was already defined.\n", argv[argc - 2], line, token);
          fclose(fop);
          return FAILED;
        }

        statuses[status] = 1;

        if (s_ramsection_write_order_index >= RAMSECTION_TYPES_COUNT) {
          print_text(NO, "%s:%d: LOAD_FILES: Too many section types in [ramsectionwriteorder].\n", argv[argc - 2], line);
          fclose(fop);
          return FAILED;
        }

        g_ramsection_write_order[s_ramsection_write_order_index++] = status;

        if (s_ramsection_write_order_index >= RAMSECTION_TYPES_COUNT)
          break;

        if (fgets(tmp, 255, fop) == NULL) {
          print_text(NO, "%s:%d: LOAD_FILES: Out of lines in [ramsectionwriteorder].\n", argv[argc - 2], line);
          fclose(fop);
          return FAILED;          
        }
        
        line++;

        /* remove garbage from the end */
        _process_tmp(tmp);
        
        x = 0;
        j = get_next_token(tmp, token, &x);
      }
      
      /* count the types */
      sum = 0;
      for (i = 0; i < SECTION_TYPES_COUNT_ALL; i++) {
        if (statuses[i] != 0)
          sum++;
      }

      if (sum != RAMSECTION_TYPES_COUNT) {
        print_text(NO, "%s:%d: LOAD_FILES: Not enough section types (all must be given) in [ramsectionwriteorder].\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }

      ramsectionwriteorder_defined = YES;
    }
    /* section / ramsection settings? */
    else if (state == STATE_RAMSECTIONS || state == STATE_SECTIONS) {
      i = SUCCEEDED;
      while (i == SUCCEEDED) {
        if (strcaselesscmp(token, "bank") == 0) {
          if (bank_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: BANK defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          bank_defined = YES;
          
          if (get_next_number(&tmp[x], &bank, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in BANK number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "banks") == 0) {
          if (banks_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: BANKS defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          banks_defined = YES;
          
          if (get_next_token(&tmp[x], token, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in BANKS list.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          strcpy(banks, token);
        }
        else if (strcaselesscmp(token, "slot") == 0) {
          if (slot_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: SLOT defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          slot_defined = YES;
          slot_name[0] = 0;
          
          if (get_next_number(&tmp[x], &slot, &x) == FAILED) {
            if (get_next_token(&tmp[x], token, &x) == FAILED) {
              print_text(NO, "%s:%d: LOAD_FILES: Error in SLOT number.\n", argv[argc - 2], line);
              fclose(fop);
              return FAILED;
            }

            strcpy(slot_name, token);
            slot = -1;
          }
        }
        else if (strcaselesscmp(token, "orga") == 0) {
          if (orga_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: ORGA defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          if (org_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: ORG was already defined before, cannot define ORGA.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
          
          orga_defined = YES;

          if (get_next_number(&tmp[x], &orga, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in ORGA number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "org") == 0) {
          if (org_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: ORG defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          if (orga_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: ORGA was already defined before, cannot define ORG.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          org_defined = YES;

          if (get_next_number(&tmp[x], &org, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in ORG number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "size") == 0) {
          if (size_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: SIZE defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          size_defined = YES;

          if (get_next_number(&tmp[x], &size, &x) == FAILED || size < 0) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in SIZE number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "bitwindow") == 0) {
          if (bitwindow_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: BITWINDOW defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          
          bitwindow_defined = YES;

          if (get_next_number(&tmp[x], &bitwindow, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in BITWINDOW number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          if (bitwindow == 0) {
            print_text(NO, "%s:%d: LOAD_FILES: BITWINDOW 0 means that the section cannot be placed anywhere.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "window") == 0) {
          if (window_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: WINDOW defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }
          
          window_defined = YES;

          if (get_next_number(&tmp[x], &window_start, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in WINDOW (start) number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
          
          if (get_next_number(&tmp[x], &window_end, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in WINDOW (end) number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          if (window_start > window_end) {
            print_text(NO, "%s:%d: LOAD_FILES: WINDOW start ($%.4x) is bigger than WINDOW end ($%.4x) number.\n", argv[argc - 2], line, window_start, window_end);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "priority") == 0) {
          if (priority_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: PRIORITY defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          priority_defined = YES;

          if (get_next_number(&tmp[x], &priority, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in PRIORITY number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "alignment") == 0) {
          if (alignment_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: ALIGNMENT defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          alignment_defined = YES;

          if (get_next_number(&tmp[x], &alignment, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in ALIGNMENT number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "offset") == 0) {
          if (offset_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: OFFSET defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          offset_defined = YES;

          if (get_next_number(&tmp[x], &offset, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in OFFSET number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "appendto") == 0) {
          if (appendto_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: APPENDTO defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          appendto_defined = YES;

          if (get_next_token(&tmp[x], appendto_name, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in APPENDTO section name.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "after") == 0) {
          if (after_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: AFTER defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          after_defined = YES;

          if (get_next_token(&tmp[x], after_name, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in AFTER section name.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "keep") == 0) {
          if (keep_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: KEEP defined for the second time for a %s.\n", argv[argc - 2], line, state_name);
            fclose(fop);
            return FAILED;
          }

          keep_defined = YES;
        }
        else if (state == STATE_SECTIONS && (strcaselesscmp(token, "free") == 0 ||
                                             strcaselesscmp(token, "force") == 0 ||
                                             strcaselesscmp(token, "semisubfree") == 0 ||
                                             strcaselesscmp(token, "semifree") == 0 ||
                                             strcaselesscmp(token, "superfree") == 0 ||
                                             strcaselesscmp(token, "overwrite") == 0 ||
                                             strcaselesscmp(token, "semisuperfree") == 0)) {
          if (status_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: Section type was defined for the second time.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          status_defined = YES;
          
          if (strcaselesscmp(token, "free") == 0)
            status = SECTION_STATUS_FREE;
          else if (strcaselesscmp(token, "force") == 0)
            status = SECTION_STATUS_FORCE;
          else if (strcaselesscmp(token, "overwrite") == 0)
            status = SECTION_STATUS_OVERWRITE;
          else if (strcaselesscmp(token, "semifree") == 0)
            status = SECTION_STATUS_SEMIFREE;
          else if (strcaselesscmp(token, "superfree") == 0)
            status = SECTION_STATUS_SUPERFREE;
          else if (strcaselesscmp(token, "semisubfree") == 0)
            status = SECTION_STATUS_SEMISUBFREE;
          else if (strcaselesscmp(token, "semisuperfree") == 0)
            status = SECTION_STATUS_SEMISUPERFREE;
        }
        else if (state == STATE_RAMSECTIONS && (strcaselesscmp(token, "free") == 0 ||
                                                strcaselesscmp(token, "semifree") == 0 ||
                                                strcaselesscmp(token, "semisubfree") == 0 ||
                                                strcaselesscmp(token, "force") == 0)) {
          if (status_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: RAM section type was defined for the second time.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }

          status_defined = YES;
          
          if (strcaselesscmp(token, "free") == 0)
            status = SECTION_STATUS_RAM_FREE;
          else if (strcaselesscmp(token, "force") == 0)
            status = SECTION_STATUS_RAM_FORCE;
          else if (strcaselesscmp(token, "semifree") == 0)
            status = SECTION_STATUS_RAM_SEMIFREE;
          else if (strcaselesscmp(token, "semisubfree") == 0)
            status = SECTION_STATUS_RAM_SEMISUBFREE;
        }
        else
          break;
        
        i = get_next_token(&tmp[x], token, &x);
      }
      
      if (i == FAILED) {
        print_text(NO, "%s:%d: LOAD_FILES: There is something wrong in a %s's settings.\n", argv[argc - 2], line, state_name);
        fclose(fop);
        return FAILED;
      }
      if (slot_defined == NO) {
        print_text(NO, "%s:%d: LOAD_FILES: \"%s\" requires a SLOT.\n", argv[argc - 2], line, state_name);
        fclose(fop);
        return FAILED;
      }
      if (status_defined == YES && status == SECTION_STATUS_SEMISUPERFREE) {
        if (banks_defined == NO) {
          print_text(NO, "%s:%d: LOAD_FILES: SEMISUPERFREE .SECTION \"%s\" requires BANKS to be defined.\n", argv[argc - 2], line, state_name);
          fclose(fop);
          return FAILED;
        }
      }
      else {
        if (bank_defined == NO) {
          print_text(NO, "%s:%d: LOAD_FILES: \"%s\" requires a BANK to be defined.\n", argv[argc - 2], line, state_name);
          fclose(fop);
          return FAILED;
        }
      }
      if (appendto_defined == YES && (org_defined == YES || orga_defined == YES || after_defined == YES)) {
        print_text(NO, "%s:%d: LOAD_FILES: \"%s\" can't use APPENDTO with ORG, ORGA or AFTER.\n", argv[argc - 2], line, state_name);
        fclose(fop);
        return FAILED;
      }

      /* add a new entry */
      g_sec_fix_tmp = calloc(sizeof(struct section_fix), 1);
      if (g_sec_fix_tmp == NULL) {
        print_text(NO, "%s:%d: LOAD_FILES: Out of memory error.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }

      strcpy(g_sec_fix_tmp->name, token);
      strcpy(g_sec_fix_tmp->file_name, argv[argc - 2]);
      strcpy(g_sec_fix_tmp->slot_name, slot_name);
      g_sec_fix_tmp->line_number = line;
      g_sec_fix_tmp->bank = bank;
      g_sec_fix_tmp->slot = slot;
      g_sec_fix_tmp->keep = keep_defined;
      g_sec_fix_tmp->bitwindow = bitwindow;
      g_sec_fix_tmp->window_start = window_start;
      g_sec_fix_tmp->window_end = window_end;

      if (banks_defined == YES)
        strcpy(g_sec_fix_tmp->banks, banks);
      else
        g_sec_fix_tmp->banks[0] = 0;
      if (orga_defined == YES)
        g_sec_fix_tmp->orga = orga;
      else
        g_sec_fix_tmp->orga = -1;

      if (org_defined == YES)
        g_sec_fix_tmp->org = org;
      else
        g_sec_fix_tmp->org = -1;

      if (status_defined == YES)
        g_sec_fix_tmp->status = status;
      else
        g_sec_fix_tmp->status = -1;

      if (alignment_defined == YES)
        g_sec_fix_tmp->alignment = alignment;
      else
        g_sec_fix_tmp->alignment = -1;

      if (offset_defined == YES)
        g_sec_fix_tmp->offset = offset;
      else
        g_sec_fix_tmp->offset = -1;

      if (size_defined == YES)
        g_sec_fix_tmp->size = size;
      else
        g_sec_fix_tmp->size = -1;

      if (appendto_defined == YES || after_defined == YES) {
        g_after_tmp = calloc(1, sizeof(struct after_section));
        if (g_after_tmp == NULL) {
          print_text(NO, "%s:%d: LOAD_FILES: Out of memory error.\n", argv[argc - 2], line);
          fclose(fop);
          return FAILED;
        }
        g_after_tmp->alive = YES;
        g_after_tmp->inserted = NO;
        g_after_tmp->section_s = NULL;
        g_after_tmp->after_s = NULL;
        g_after_tmp->section_id = -1;
        g_after_tmp->file_id = -1;
        if (appendto_defined == YES) {
          g_after_tmp->is_appendto = YES;
          strcpy(g_after_tmp->after, appendto_name);
        }
        else {
          g_after_tmp->is_appendto = NO;
          strcpy(g_after_tmp->after, after_name);
        }
        strcpy(g_after_tmp->section, g_sec_fix_tmp->name);
        g_after_tmp->next = g_after_sections;
        g_after_sections = g_after_tmp;
      }

      g_sec_fix_tmp->priority_defined = priority_defined;
      g_sec_fix_tmp->priority = priority;

      if (state == STATE_RAMSECTIONS)
        g_sec_fix_tmp->is_ramsection = YES;
      else
        g_sec_fix_tmp->is_ramsection = NO;
      
      g_sec_fix_tmp->next = g_sec_fix_first;
      g_sec_fix_first = g_sec_fix_tmp;

      continue;
    }
    /* library loading? */
    else if (state == STATE_LIBRARY) {
      i = SUCCEEDED;
      while (i == SUCCEEDED) {
        if (strcaselesscmp(token, "bank") == 0) {
          if (bank_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: BANK defined for the second time for a library file.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
          bank_defined = YES;
          
          if (get_next_number(&tmp[x], &bank, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in BANK number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else if (strcaselesscmp(token, "slot") == 0) {
          if (slot_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: SLOT defined for the second time for a library file.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
          slot_defined = YES;
          slot_name[0] = 0;
          
          if (get_next_number(&tmp[x], &slot, &x) == FAILED) {
            if (get_next_token(&tmp[x], token, &x) == FAILED) {
              print_text(NO, "%s:%d: LOAD_FILES: Error in SLOT number.\n", argv[argc - 2], line);
              fclose(fop);
              return FAILED;
            }

            strcpy(slot_name, token);
            slot = -1;
          }
        }
        else if (strcaselesscmp(token, "base") == 0) {
          if (base_defined == YES) {
            print_text(NO, "%s:%d: LOAD_FILES: BASE defined for the second time for a library file.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
          base_defined = YES;
          
          if (get_next_number(&tmp[x], &base, &x) == FAILED) {
            print_text(NO, "%s:%d: LOAD_FILES: Error in BASE number.\n", argv[argc - 2], line);
            fclose(fop);
            return FAILED;
          }
        }
        else
          break;
        
        i = get_next_token(&tmp[x], token, &x);
      }
      
      if (i == FAILED) {
        print_text(NO, "%s:%d: LOAD_FILES: No library to load.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      if (slot_defined == NO) {
        print_text(NO, "%s:%d: LOAD_FILES: Library file requires a SLOT.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      if (bank_defined == NO) {
        print_text(NO, "%s:%d: LOAD_FILES: Library file requires a BANK.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      if (g_use_libdir == YES) {
        if (g_paths_in_linkfile_are_relative_to_linkfile == YES) {
          snprintf(tmp_token, sizeof(tmp_token), "%s%s", linkfile_path, token);
          f = fopen(tmp_token, "rb");
        }
        else
          f = fopen(token, "rb");
      
        /* use the current working directory if the library isn't found in the ext_libdir directory */
        if (f == NULL)
          snprintf(tmp_token, sizeof(tmp_token), "%s%s", g_ext_libdir, token);
        else {
          if (g_paths_in_linkfile_are_relative_to_linkfile == YES)
            snprintf(tmp_token, sizeof(tmp_token), "%s%s", linkfile_path, token);
          else
            snprintf(tmp_token, sizeof(tmp_token), "%s", token);
          fclose(f);
        }
      }
      else {
        if (g_paths_in_linkfile_are_relative_to_linkfile == YES)
          snprintf(tmp_token, sizeof(tmp_token), "%s%s", linkfile_path, token);
        else
          snprintf(tmp_token, sizeof(tmp_token), "%s", token);
      }

      if (load_file(tmp_token, bank, slot, slot_name, YES, base, base_defined) == FAILED) {
        fclose(fop);
        return FAILED;
      }
      
      if (get_next_token(&tmp[x], token, &x) == SUCCEEDED) {
        print_text(NO, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
        fclose(fop);
        return FAILED;
      }
      
      continue;
    }
    /* object file loading */
    else {
      if (g_paths_in_linkfile_are_relative_to_linkfile == YES)
        snprintf(tmp_token, sizeof(tmp_token), "%s%s", linkfile_path, token);
      else
        snprintf(tmp_token, sizeof(tmp_token), "%s", token);
      
      if (load_file(tmp_token, 0, 0, NULL, NO, 0, OFF) == FAILED) {
        fclose(fop);
        return FAILED;
      }
    }
    
    if (get_next_token(&tmp[x], token, &x) == FAILED)
      continue;

    print_text(NO, "%s:%d: LOAD_FILES: Syntax error.\n", argv[argc - 2], line);
    fclose(fop);
    return FAILED;
  }

  fclose(fop);

  return SUCCEEDED;
}


int load_file(char *file_name, int bank, int slot, char *slot_name, int fix_slot, int base, int base_defined) {

  struct object_file *o;
  unsigned char *data;
  static int id = 0;
  char *name;
  int size;

  o = calloc(sizeof(struct object_file), 1);
  name = calloc(strlen(file_name)+1, 1);
  if (o == NULL || name == NULL) {
    free(o);
    free(name);
    print_text(NO, "LOAD_FILE: Out of memory.\n");
    return FAILED;
  }

  if (load_file_data(file_name, &data, &size) == FAILED) {
    free(name);
    free(o);
    return FAILED;
  }

  /* only valid for library files */
  o->bank = bank;
  o->slot = slot;
  o->base = base;
  o->base_defined = base_defined;

  /* init the rest of the variables */
  o->source_file_names = NULL;
  o->memorymap = NULL;
  o->exported_defines = NULL;
  o->data_blocks = NULL;
  o->source_file_names_list = NULL;
  o->listfile_items = 0;
  o->listfile_ints = NULL;
  o->listfile_cmds = NULL;
  o->stacks = NULL;
  o->stacks_max = -1;
  o->stacks_array_max = 0;

  o->slot_name[0] = 0;
  if (slot_name != NULL)
    strcpy(o->slot_name, slot_name);

  if (g_obj_first == NULL) {
    g_obj_first = o;
    g_obj_last = o;
  }
  else {
    g_obj_last->next = o;
    g_obj_last = o;
  }

  o->next = NULL;
  o->size = size;
  o->data = data;
  o->fix_slot = fix_slot;

  strcpy(name, file_name);
  o->name = name;
  o->id = id++;

  /* add the pointer also to a pointer array for quick discovery with the ID */
  if (add_pointer_to_a_pointer_array((void *)o, o->id, (void ***)&g_object_files, &s_object_files_max, &s_object_files_array_max, 64) == FAILED)
    return FAILED;

  return SUCCEEDED;
}


int load_file_data(char *file_name, unsigned char **data, int *size) {

  FILE *fop;

  fop = fopen(file_name, "rb");
  if (fop == NULL) {
    print_text(NO, "LOAD_FILE_DATA: Could not open file \"%s\".\n", file_name);
    return FAILED;
  }

  fseek(fop, 0, SEEK_END);
  *size = (int)ftell(fop);
  fseek(fop, 0, SEEK_SET);

  if (*size == 0) {
    print_text(NO, "LOAD_FILE_DATA: File \"%s\" is empty!\n", file_name);
    return FAILED;
  }
  
  *data = calloc(*size, 1);
  if (*data == NULL) {
    fclose(fop);
    return FAILED;
  }

  if (fread(*data, 1, *size, fop) != (size_t) *size) {
    print_text(NO, "LOAD_FILE_DATA: Could not read all %d bytes of \"%s\"!", *size, file_name);
    return FAILED;
  }

  fclose(fop);

  return SUCCEEDED;
}


static char s_unknown_file_name[] = "?";


/* NOTE: get_file_name(-1) must return something, like "?", as it's a special case */
char *get_file_name(int file_id) {

  if (file_id < 0 || file_id > s_object_files_max || g_object_files[file_id] == NULL)
    return s_unknown_file_name;
  else
    return g_object_files[file_id]->name;
}


struct object_file *get_file(int file_id) {

  if (file_id < 0 || file_id > s_object_files_max || g_object_files[file_id] == NULL) {
    print_text(NO, "GET_FILE: Trying to find file %d which has gone missing! Please submit a bug report!\n", file_id);
    return NULL;
  }
  
  return g_object_files[file_id];
}


char *get_source_file_name(int file_id, int source_id) {

  struct source_file_name *s;
  struct object_file *o;
  
  o = g_obj_first;
  while (o != NULL) {
    if (o->id == file_id)
      break;
    o = o->next;
  }

  if (o == NULL)
    return s_unknown_file_name;

  s = o->source_file_names_list;
  while (s != NULL) {
    if (s->id == source_id)
      break;
    s = s->next;
  }

  if (s == NULL)
    return s_unknown_file_name;

  return s->name;
}


int convert_slot_names_and_addresses(void) {

  struct object_file *o;
  
  o = g_obj_first;
  while (o != NULL) {
    if (o->fix_slot) {
      if (o->slot < 0) {
        if (get_slot_by_its_name(o->slot_name, &(o->slot)) == FAILED)
          return FAILED;
      }
      else {
        /* here o->slot, given in the linkfile, might be $2000, so we'll need to find its SLOT number */
        if (get_slot_by_a_value(o->slot, &(o->slot)) == FAILED)
          return FAILED;
      }
    }
    
    o = o->next;
  }
  
  return SUCCEEDED;
}

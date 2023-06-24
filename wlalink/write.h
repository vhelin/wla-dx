
#ifndef _WRITE_H
#define _WRITE_H

int strcaselesscmp(char *s1, const char *s2);
int check_ramsections(void);
int fix_all_sections(void);
int fix_references(void);
int fix_label_sections(void);
int insert_label_into_maps(struct label *l, int is_sizeof);
int fix_label_addresses(void);
int transform_stack_definitions(void);
int sort_sections(void);
int insert_sections(void);
int write_symbol_file(char *outname, int mode, int output_addr_to_line);
int write_gb_header(int name_status);
int write_rom_file(char *outname);
int compute_pending_calculations(void);
int compute_stack(struct stack *sta, double *result_ram, double *result_rom, int *result_slot, int *result_base, int *result_bank);
int write_bank_header_calculations(struct stack *sta);
int write_bank_header_references(struct reference *r);
int parse_stack(struct stack *sta);
int get_snes_pc_bank(struct label *l);
int correct_65816_library_sections(void);
int is_label_anonymous(char *label);
int sort_anonymous_labels(void);
struct label *get_closest_anonymous_label(char *name, char *context, int rom_address, int file_id, int section_status, int section);
struct stack *find_stack(int id, int file_id);
struct section *find_section(int id);
int generate_ram_bank_usage_labels(void);
int generate_sizeof_label_definitions(void);
int fix_sectionstartend_labels(void);
int get_slot_by_its_name(char *name, int *slot);
int get_slot_by_a_value(int value, int *slot);

#endif

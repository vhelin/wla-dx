
#ifndef _PHASE_3_H
#define _PHASE_3_H

int phase_3(void);
int is_label_anonymous(char *label);
int mangle_label(char *label, char *parent, int n, unsigned int label_size, int file_name_id, int line_number);
int add_namespace(char *label, char *name_space, unsigned int label_size, int file_name_id, int line_number);
int macro_get_by_id(int id, struct macro_static **macro_out);
int process_macro_in(int id, char *name, int file_name_id, int line_number);
int process_macro_out(int id, char *name, int file_name_id, int line_number);
int free_label_context_allocations(void);
int add_context_to_anonymous_label(char *label, int label_size, struct label_context *label_context, int file_name_id, int line_number);
int print_fscanf_error_accessing_internal_data_stream(void);

#endif

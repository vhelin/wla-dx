
#ifndef _PHASE_1_H
#define _PHASE_1_H

int phase_1(void);
int add_a_new_definition(char *name, double value, char *string, int type, int size);
int export_a_definition(char *name);
int redefine(char *name, double value, char *string, int type, int size);
int undefine(char *name);
int parse_directive(void);
int parse_if_directive(void);
int enum_add_label(char *name, int type, int size, struct structure *instance);
int add_label_to_enum_or_ramsection(char *name, int size);
int parse_enum_token(void);
int find_next_point(char *name);
int get_new_definition_data(int *b, char *c, int *size, double *data, int *export);
int localize_path(char *path);
int verify_name_length(char *name);
int is_reserved_definition(char *t);
int strcaselesscmp(char *s1, char *s2);
int process_label_inside_macro(int add_namespace, char *buffer, int sizeof_buffer);

void generate_label(char *header, char *footer);
void reset_label_stack(void);
int add_label_to_label_stack(char *l);
int get_full_label(char *l, char *out);

#if defined(__GNUC__)
__attribute__((format(printf, 2, 3)))
#elif defined(__RESHARPER__)
[[rscpp::format(printf, 2, 3)]]
#endif
void print_error(int type, char *error, ...);

void next_line(void);
void use_dir(char *directory, char *path);
void get_dir(char *full_path, char *tmp);

int add_namespace_to_string(char *s, int sizeof_s, char *type);
int macro_get(char *name, int add_namespace, struct macro_static **macro_out);

#endif

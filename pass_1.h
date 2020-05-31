
#ifndef _PASS_1_H
#define _PASS_1_H

int pass_1(void);
int evaluate_token(void);
int add_a_new_definition(char *name, double value, char *string, int type, int size);
int export_a_definition(char *name);
int redefine(char *name, double value, char *string, int type, int size);
int undefine(char *name);
int parse_directive(void);
int parse_if_directive(void);
void free_struct(struct structure *st);
int enum_add_label(char *name, int type, int size, struct structure *instance);
int add_label_to_enum_or_ramsection(char *name, int size);
int parse_enum_token(void);
int find_next_point(char *name);
int get_new_definition_data(int *b, char *c, int *size, double *data, int *export);
int localize_path(char *path);
int verify_name_length(char *name);
int is_reserved_definition(char *t);

void generate_label(char *header, char *footer);
int add_label_to_label_stack(char *l);
int get_full_label(char *l, char *out);
void print_error(char *error, int type);
void next_line(void);
void delete_stack(struct stack *s);
void parse_print_string(char *input, char *output, int output_size);

int add_namespace_to_string(char *s, int sizeof_s, char *type);
int macro_get(char *name, int add_namespace, struct macro_static **macro_out);

#endif

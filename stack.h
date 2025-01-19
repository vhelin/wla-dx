
#ifndef _STACK_H
#define _STACK_H

int get_label_length(char *l);
int stack_calculate_free_allocations(void);
int stack_calculate(char *in, int *value, int *bytes_parsed, unsigned char from_substitutor);
int stack_create_label_stack(char *label);
int stack_create_stack_stack(int stack_id);
int resolve_stack(struct stack_item s[], int stack_item_count);
int compute_stack(struct stack *sta, int stack_item_count, double *result);
int compress_stack_calculation_ids(void);
int calculation_stack_insert(struct stack *s);
void init_stack_struct(struct stack *s);
void free_stack_calculations(void);
void delete_stack_calculation_struct(struct stack *s);
struct stack *find_stack_calculation(int id, int print_error_message);

#if defined(MC68000)
int stack_create_stack_caddr_offset(int type, int data, char *label);
int stack_create_stack_caddr_offset_plus_n(int type, int data, char *label, int n);
int stack_add_offset_plus_n_to_stack(int id, int n);
int does_stack_contain_one_label(int id);
#endif

/* TODO: move these to their own file */
int data_stream_parser_free(void);
int data_stream_parser_parse(void);
int data_stream_parser_get_current_bank(void);
int data_stream_parser_get_current_base(void);
struct section_def *data_stream_parser_get_current_section(void);
int data_stream_parser_get_current_address(void);

#endif


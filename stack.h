
#ifndef _STACK_H
#define _STACK_H

int get_label_length(char *l);
int stack_calculate_free_allocations(void);
int stack_calculate(char *in, int *value, int *bytes_parsed, unsigned char from_substitutor);
int stack_create_label_stack(char *label);
int stack_create_stack_stack(int stack_id);
int resolve_stack(struct stack_item s[], int stack_item_count);
int compute_stack(struct stack *sta, int stack_item_count, double *result);
int calculation_stack_insert(struct stack *s);
int compress_stack_calculation_ids(void);
void debug_print_stack(int line_number, int stack_id, struct stack_item *ta, int count, int id, struct stack *stack);
void init_stack_struct(struct stack *s);
void free_stack_calculations(void);
void delete_stack_calculation_struct(struct stack *s);
struct stack *find_stack_calculation(int id, int print_error_message);

/* TODO: move these to their own file */
int data_stream_parser_free(void);
int data_stream_parser_parse(void);
struct data_stream_item *data_stream_parser_find_label(char *label, int file_name_id, int line_number);

#endif


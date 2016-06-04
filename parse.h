
#ifndef _PARSE_H
#define _PARSE_H

int input_number(void);
int input_number_token(struct token_stack_root *token_stack);
int input_next_string(void);
int get_next_token(void);
int skip_next_token(void);
int compare_next_token(char *token, int length);
int expand_macro_arguments(char *in);

/* parse helper methods */
void parse_while_any_token(struct token_stack_root *token_stack, char *tokens);
int parse_until_any_token(struct token_stack_root *token_stack, char *tokens, char *store, int size);

/* parse type methods */
int parse_type_expression(struct token_stack_root *token_stack, int *value, char *string, int max_buffer);
int parse_type_argument_to_string(struct token_stack_root *token_stack, char *string, int max_buffer);
int parse_type_hex_value(struct token_stack_root *token_stack, int *value, int *hint);
int parse_type_double_value(struct token_stack_root *token_stack, double *value, int *dec_value, int *int_value, int *hint);
int parse_type_binary_value(struct token_stack_root *token_stack, int *value, int *hint);
int parse_type_char(struct token_stack_root *token_stack, int *result);
int parse_type_string(struct token_stack_root *token_stack, char *string, int buffer_max, int *length);
int parse_type_label(struct token_stack_root *token_stack, char *string, int buffer_max);
int parse_hint_attribute(struct token_stack_root *token_stack, int hint);
int parse_hint_by_value(int value, int hint);
int parse_resolve_label(char *string, int *string_len, int *value, int *hint, struct definition **label_def);
int parse_validate_macro_bounds(struct token_stack_root *token_stack);

#endif

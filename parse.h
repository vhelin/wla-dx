
#ifndef _PARSE_H
#define _PARSE_H

int input_number(void);
int input_next_string(void);
int get_next_plain_string(void);
int get_next_token(void);
int skip_next_token(void);
int compare_next_token(char *token);
int expand_macro_arguments(char *in);
int is_string_ending_with(char *s, char *e);

#endif

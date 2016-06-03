#ifndef _TOKEN_STACK_H
#define _TOKEN_STACK_H

#define MACRO_ARG_NONE        0
#define MACRO_ARG_INSTANCE    1
#define MACRO_ARG_FILENAME    2
#define MACRO_ARG_SLASH       3
#define MACRO_ARG_NUMBER      4

#define TS_STATE_FLAG_LOCAL   0x01
#define TS_STATE_FLAG_GLOBAL  0x02
#define TS_STATE_FLAG_ANY     TS_STATE_FLAG_LOCAL | TS_STATE_FLAG_GLOBAL

#define TS_STATE_NONE           0x00
#define TS_STATE_IN_STRING      0x01
#define TS_STATE_IGNORE_MACROS  0x02

struct token_stack_entry {
  /* Parent token stack pointer, return to this entry when the current token iteration is complete. */
  struct token_stack_entry *parent_entry;
  /* The source token buffer. */
  char *buffer_root;
  /* The current offset in the token buffer. */
  int buffer_offset;
  /* Flag used to indicate if the buffer_root was allocated. */
  int buffer_allocated;
  /* The adjustment offset when returning to this stack entry from a pop. */
  int restore_offset;
  /* The local token stack state. */
  int local_state;
};

struct token_stack_root {
  /* Pointer to the current root token stack entry. */
  struct token_stack_entry *active_entry;
  /* Pointer to the token buffer state history. */
  struct token_stack_root *state_history;
  /* Pointer to the token stack process argument function. */
  void (*process_argument) (struct token_stack_root *token_stack, int incrementing);
  /* The global token stack state. */
  int global_state;
};

typedef void (*token_stack_argument_ptr)(struct token_stack_root *token_stack, int incrementing);

struct token_stack_root *token_stack_new_empty(void);
struct token_stack_root *token_stack_copy(struct token_stack_root *source, int copy_history);
struct token_stack_root *token_stack_new(char *buffer_ptr, int buffer_offset);
struct token_stack_root *token_stack_new_number(int number, int start);
void token_stack_free(struct token_stack_root *token_stack);
void token_stack_push_state(struct token_stack_root *token_stack);
void token_stack_pop_state(struct token_stack_root *token_stack, int discard_state);
char token_stack_get_current_token(struct token_stack_root *token_stack);
char token_stack_peek_token(struct token_stack_root *token_stack, int offset);
int token_stack_read(struct token_stack_root *token_stack, char *read_buffer, int size);
int token_stack_move(struct token_stack_root *token_stack, int move);
void token_stack_reset(struct token_stack_root *token_stack, int start);
void token_stack_update_root_offset(struct token_stack_root *token_stack, int buffer_offset);
void token_stack_set_state_flag(struct token_stack_root *token_stack, int state, int local);
void token_stack_clear_state_flag(struct token_stack_root *token_stack, int state, int local);
int token_stack_get_flag_state(struct token_stack_root *token_stack, int state, int local);

struct token_stack_entry *token_stack_push(struct token_stack_root *token_stack, char *buffer_ptr, int buffer_offset);
struct token_stack_entry *token_stack_push_number(struct token_stack_root *token_stack, int number, int start);
struct token_stack_entry *token_stack_pop(struct token_stack_root *token_stack);
void token_stack_process_argument(struct token_stack_root *active_stack, int incrementing);

int token_stack_cmp(struct token_stack_root *token_stack, char compare, int ignore_case);
int token_stack_strncmp(struct token_stack_root *token_stack, char *compare, int length, int ignore_case);
int token_stack_read_to_token(struct token_stack_root *token_stack, char *result, int max_length, char end_token);

#endif /* _TOKEN_STACK_H */

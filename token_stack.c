#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "token_stack.h"
#include "include_file.h"
#include "pass_1.h"
#include <string.h>
#include <ctype.h>

extern int macro_active;
extern struct macro_runtime *macro_runtime_current;
extern struct active_file_info *active_file_info_last;

/*****************************
* Internal function definitions. 
* Functions that should not be exposed to other objects.
*****************************/
int token_stack_increment(struct token_stack_root *token_stack, int increment);
int token_stack_decrement(struct token_stack_root *token_stack, int decrement);

struct token_stack_entry *token_stack_entry_new(char *buffer_ptr, int buffer_offset);
struct token_stack_entry *token_stack_entry_new_number(int number, int start);
void token_stack_entry_free(struct token_stack_entry *stack_entry);
char token_stack_entry_get_current_token(struct token_stack_entry *stack_entry);

/*****************************
* token_stack_new_empty
* Purpose: Create a new token empty token_stack_root.
* Returns: A new empty token_stack_root.
*****************************/
struct token_stack_root *token_stack_new_empty(void) {
  
  struct token_stack_root *new_stack = (struct token_stack_root*) malloc(sizeof(struct token_stack_root));

  if (new_stack != NULL) {
    /* Set the token entry data. */
    new_stack->active_entry = NULL;
    new_stack->state_history = NULL;
    new_stack->process_argument = token_stack_process_argument;
    new_stack->global_state = TS_STATE_NONE;
  }
  else
    print_error("Out of memory while allocating a new token_stack_root.\n", ERROR_ERR);

  return new_stack;
}

/*****************************
* token_stack_copy
* Purpose: Makes a copy of the token_stack_root.
* Param source: The source token stack to copy.
* Param copy_history: flag used to copy the history state.
* Returns: A copy of the source token stack.
*****************************/
struct token_stack_root *token_stack_copy(struct token_stack_root *source, int copy_history) {

  /* Copy the token stack root. */
  struct token_stack_root *new_root = token_stack_new_empty();

  if (new_root != NULL) {
    struct token_stack_entry *cur_entry = source->active_entry;
    struct token_stack_entry *last_entry = NULL;

    /* Copy the argument function. */
    new_root->process_argument = source->process_argument;
    new_root->global_state = source->global_state;

    /* Copy the active entry data. */
    while (cur_entry) {
      struct token_stack_entry *new_entry = NULL;

      /* Create a copy of current entry data. */
      char *buffer_data = cur_entry->buffer_root;
      if (cur_entry->buffer_allocated == 1) {
        unsigned int string_length = strlen(buffer_data) + 1;

	/* TODO: fix this, did this to get rid of sprintf_s() */
	if (string_length < strlen(cur_entry->buffer_root) + 1)
	  string_length = strlen(cur_entry->buffer_root) + 1;
	
        buffer_data = (char *)malloc(sizeof(char)* string_length);
        if (buffer_data != NULL)
          sprintf(buffer_data, "%s", cur_entry->buffer_root);
        else
          print_error("Out of memory while copying token_stack buffer data.\n", ERROR_ERR);
      }
      new_entry = token_stack_entry_new(buffer_data, cur_entry->buffer_offset);

      if (new_entry != NULL) {
        new_entry->buffer_allocated = cur_entry->buffer_allocated;
        new_entry->restore_offset = cur_entry->restore_offset;
        new_entry->local_state = cur_entry->local_state;
   
        /* Update the list. */
        if (last_entry != NULL)
          last_entry->parent_entry = new_entry;
        last_entry = new_entry;

        if (new_root->active_entry == NULL)
          new_root->active_entry = new_entry;
      }
      else
        print_error("Out of memory while copying token_stack entry.\n", ERROR_ERR);

      cur_entry = cur_entry->parent_entry;
    }

    if (copy_history == 1) {
      /* Copy the history state. */
      struct token_stack_root *history_state = source->state_history;
      struct token_stack_root *last_history_state = new_root;

      while (history_state != NULL && last_history_state != NULL) {
        /* Copy the active_entry data from the history state, do not copy the child history. */
        last_history_state->state_history = token_stack_copy(history_state, 0);
        last_history_state = last_history_state->state_history;

        history_state = history_state->state_history;
      }
    }
  }
  else
    print_error("Out of memory while copying token_stack_root.\n", ERROR_ERR);

  return new_root;
}

/*****************************
* token_stack_new
* Purpose: Creates a new token stack from a buffer and an offset in the buffer.
* Param buffer_ptr: The source buffer data.
* Param buffer_offset: The character offset within the buffer.
* Returns: A new token stack with an entry for the buffer and position.
*****************************/
struct token_stack_root *token_stack_new(char *buffer_ptr, int buffer_offset) {

  struct token_stack_root *new_stack = token_stack_new_empty();

  if (new_stack != NULL) {
    /* Add the token stack entry. */
    token_stack_push(new_stack, buffer_ptr, buffer_offset);
  }

  return new_stack;
}

/*****************************
* token_stack_new_number
* Purpose: Creates a new token stack from a number and a start flag.
* Param number: The value to place in the token stack.
* Param start: A flag to set the buffer offset to the start (1) or end (non-1).
* Returns: A new token stack with an entry for the number and start flag.
*****************************/
struct token_stack_root *token_stack_new_number(int number, int start) {
  
  struct token_stack_root *new_stack = token_stack_new_empty();

  if (new_stack != NULL) {
    /* Add the token stack entry. */
    token_stack_push_number(new_stack, number, start);
  }

  return new_stack;
}

/*****************************
* token_stack_free
* Purpose: Frees the memory allocated by a token_stack.
* Param token_stack: The token stack to free.
*****************************/
void token_stack_free(struct token_stack_root *token_stack) {

  if (token_stack != NULL) {
    /* Remove all the history entries. */
    while (token_stack->state_history != NULL)
      token_stack_pop_state(token_stack, 1);

    /* Remove all the stack entries. */
    while (token_stack_pop(token_stack) != NULL)
      ;

    free(token_stack);
    token_stack = NULL;
  }
}

/*****************************
* token_stack_push_state
* Purpose: Push the current token iteration state.
* Param token_stack: The token stack to save the state for.
*****************************/
void token_stack_push_state(struct token_stack_root *token_stack) {

  if (token_stack != NULL) {
    /* Push the token state. */
    struct token_stack_root *stack_copy = token_stack_copy(token_stack, 0);

    /* Store the copy in the token stack. */
    stack_copy->state_history = token_stack->state_history;
    token_stack->state_history = stack_copy;
  }
}

/*****************************
* token_stack_pop_state
* Purpose: Pop the token stack state to restore to an earlier state, or discard that state.
* Param token_stack: The token stack to restore the state for.
* Param discard_state: 0 to use the current state as the active state, 1 to discard the stored state.
*****************************/
void token_stack_pop_state(struct token_stack_root *token_stack, int discard_state) {

  if (token_stack != NULL && token_stack->state_history != NULL) {
    /* Pop the token stack entry. */
    struct token_stack_root *history = token_stack->state_history;

    /* Update the history pointers. */
    token_stack->state_history = history->state_history;
    history->state_history = NULL;

    /* The contents in the history state will be discarded, determine if
       we should swap the active state with the history state. */
    if (discard_state == 0) {
      /* Swap the contents of the active entry and the history entry. */
      struct token_stack_entry *discard_entry = token_stack->active_entry;
      token_stack->active_entry = history->active_entry;
      history->active_entry = discard_entry;
    }

    /* Delete the history state. */
    token_stack_free(history);
  }
}

/*****************************
* token_stack_push
* Purpose: Push a new entry on the token stack.
* Param token_stack: The token stack to push the entry on.
* Param buffer_ptr: The source buffer data.
* Param buffer_offset: The character offset within the buffer.
* Returns: The new entry created from the push process.
*****************************/
struct token_stack_entry *token_stack_push(struct token_stack_root *token_stack, char *buffer_ptr, int buffer_offset) {

  struct token_stack_entry *push_entry = token_stack_entry_new(buffer_ptr, buffer_offset);

  if (push_entry != NULL) {
    /* Set the parent pointer. */
    push_entry->parent_entry = token_stack->active_entry;
    token_stack->active_entry = push_entry;
  }
  else
    print_error("Out of memory while pushing a token_stack_entry buffer on the token_stack_root.\n", ERROR_ERR);

  return push_entry;
}

/*****************************
* token_stack_push_number
* Purpose: Push a new number entry on the token stack.
* Param token_stack: The token stack to push the entry on.
* Param number: The value to place in the token stack.
* Param start: A flag to set the buffer offset to the start (1) or end (non-1).
* Returns: The new entry created from the push process.
*****************************/
struct token_stack_entry *token_stack_push_number(struct token_stack_root *token_stack, int number, int start) {

  struct token_stack_entry *push_entry = token_stack_entry_new_number(number, start);

  if (push_entry != NULL) {
    /* Set the parent pointer. */
    push_entry->parent_entry = token_stack->active_entry;
    token_stack->active_entry = push_entry;
  }
  else
    print_error("Out of memory while pushing a token_stack_entry number on the token_stack_root.\n", ERROR_ERR);

  return push_entry;
}

/*****************************
* token_stack_pop
* Purpose: Pop the active entry from the token stack.
* Param token_stack: The token stack to pop the entry from.
* Returns: The parent entry of the entry that was popped.
*****************************/
struct token_stack_entry *token_stack_pop(struct token_stack_root *token_stack) {

  struct token_stack_entry *root_entry = NULL;

  if (token_stack != NULL) {
    struct token_stack_entry *active_entry = token_stack->active_entry;
    if (active_entry != NULL)
      token_stack->active_entry = active_entry->parent_entry;

    token_stack_entry_free(active_entry);
    root_entry = token_stack->active_entry;
  }

  return root_entry;
}

/*****************************
* token_stack_get_current_token
* Purpose: Get the current token the buffer is pointing at.
* Param token_stack: The token stack to get the current character from.
* Returns: The current token the buffer is pointing at, or a null character if the position is invalid.
*****************************/
char token_stack_get_current_token(struct token_stack_root *token_stack) {

  char token = 0;

  if (token_stack != NULL && token_stack->active_entry != NULL) {
    if (token_stack->active_entry->parent_entry == NULL && token_stack->active_entry->buffer_offset == 0) {
      /* Always perform macro analysis on the first character. */
      token_stack_move (token_stack, 0);
    }

    token = token_stack_entry_get_current_token(token_stack->active_entry);
  }

  return token;
}

/*****************************
* token_stack_peek_token
* Purpose: Look at the token at the given offset without adjusting the lookup pointers
* Param token_stack: The token stack to get the peek character from.
* Param offset: The offset of the token to look at.
* Returns: The token at the peek offset, or a null character if the position is invalid.
*****************************/
char token_stack_peek_token(struct token_stack_root *token_stack, int offset) {
  
    /* NOTE: this should probably use a push/pop state. I think moving back and forward is going to be
       more expensive. */
    int move_amount = token_stack_move(token_stack, offset);
    char token = token_stack_get_current_token(token_stack);
    token_stack_move(token_stack, -move_amount);

    return token;
}

/*****************************
* token_stack_read
* Purpose: Read a number of tokens from the token stack.
* Param token_stack: The token stack to read from.
* Param read_buffer: The buffer to store the read tokens.
* Param size: The number of tokens to read.
* Returns: The number of tokens read.
*****************************/
int token_stack_read(struct token_stack_root *token_stack, char *read_buffer, int size) {

  /* Number of tokens read. */
  int read = 0;
  
  if (read_buffer != NULL) {
    /* Read size tokens from the stack. */
    while (read < size) {
      int move_amount = 0;
      char current_token = token_stack_get_current_token(token_stack);
      move_amount = token_stack_move(token_stack, 1);

      if (move_amount == 0)
        break;
      else {
        /* Update the buffer with the current token if its valid. */
        read_buffer[read] = current_token;
        ++read;
      }
    }
  }

  return read;
}

/*****************************
* token_stack_process_argument
* Purpose: Process macro argument tokens. This only handles arguments when the token is at the start (incrementing)
*          or at the end (decrementing).
* Param active_stack: The token stack to process arguments on.
* Param incrementing: Flag to determine if the buffer is incrementing or decrementing (1=incremented, 0=decrementing).
*****************************/
void token_stack_process_argument(struct token_stack_root *active_stack, int incrementing) {

  /* Determine if the current buffer position is within an argument. */
  char current_token = token_stack_entry_get_current_token(active_stack->active_entry);
  int token_count = 0;

  /* Process macro arguments if a macro is active and the state allows it. */
  if (macro_active != 0 && token_stack_get_flag_state(active_stack, TS_STATE_IGNORE_MACROS, TS_STATE_FLAG_ANY) == 0) {
    if (incrementing == 1) {
      /* Handle argument on increment. */

      if (current_token == '\\') {
        /* Scan forward to find the end of the argument. */
        struct token_stack_entry *active_entry = active_stack->active_entry;

        /* Determine if this is an argument substitution. */
        int macro_number = 0;
        int macro_argument = MACRO_ARG_NONE;
        token_count = 1;

        /* Extract the macro number. */
        for (; token_count < MAX_NAME_LENGTH; ++token_count) {
          char macro_char = active_entry->buffer_root[active_entry->buffer_offset + token_count];
          if (macro_char >= '0' && macro_char <= '9')
            macro_number = (macro_number * 10) + macro_char - '0';
          else if (token_count == 1 && macro_char == '@') {
            macro_argument = MACRO_ARG_INSTANCE;
            break;
          }
          else if (token_count == 1 && macro_char == '!') {
            macro_argument = MACRO_ARG_FILENAME;
            break;
          }
          else if (token_count == 1 && macro_char == '\\') {
            macro_argument = MACRO_ARG_SLASH;
            break;
          }
          else
            break;
        }

        if (macro_argument == MACRO_ARG_INSTANCE) {
          /* Translate the macro instance to a string. */
          active_entry->restore_offset = token_count + 1;
          active_entry = token_stack_push_number(active_stack, macro_runtime_current->macro->calls - 1, 1);
        }
        else if (macro_argument == MACRO_ARG_FILENAME) {
          /* Substitute the active macro file. */
          char *macro_file_name = get_file_name(active_file_info_last->filename_id);
          active_entry->restore_offset = token_count + 1;
          active_entry = token_stack_push(active_stack, macro_file_name, 0);
          /* Flag this string so it ignores macro replacement, filenames will have slashes that could result in 
             unexpected replacement. */
          active_entry->local_state = TS_STATE_IGNORE_MACROS;
        }
        else if (macro_argument == MACRO_ARG_SLASH) {
          /* Substitute the active macro slash. */
          active_entry->restore_offset = token_count + 1;
          active_entry = token_stack_push(active_stack, "\\", 0);
        }
        else if (macro_number > 0 && macro_number <= macro_runtime_current->supplied_arguments) {
          /* Return the macro replacement. */
          struct macro_argument *argument = macro_runtime_current->argument_data[macro_number - 1];

          /* Set the return offset. */
          active_entry->restore_offset = token_count;

          if (argument->type == INPUT_NUMBER_ADDRESS_LABEL || argument->type == INPUT_NUMBER_STRING) {
            /* Valid macro, update the index and push a new token. */
            active_entry = token_stack_push(active_stack, argument->string, 0);
          }
          else if (argument->type == SUCCEEDED || argument->type == INPUT_NUMBER_STACK)
            active_entry = token_stack_push_number (active_stack, argument->value, 1);
          else if (argument->type == INPUT_MACRO_ARGUMENT) {
            int string_length = strlen(argument->string);
            char *copy_string = NULL;
            int strip_quotes = 0;

            if (token_stack_get_flag_state(active_stack, TS_STATE_IN_STRING, TS_STATE_FLAG_ANY) != 0) {
              if (string_length >= 2 && argument->string[0] == '"' && argument->string[string_length - 1] == '"')
                strip_quotes = 1;
            }
            
            if (strip_quotes == 0) {
              /* Add 1 for the null terminator. */
              string_length++;
              copy_string = (char *)malloc(string_length);
              strcpy(copy_string, argument->string);
            }
            else {
              /* Minus 2 for the quotes +1 for the null terminator. */
              string_length--;
              copy_string = (char *)malloc(string_length);
              memcpy(copy_string, &argument->string[1], string_length - 1);
              copy_string[string_length - 1] = '\0';
            }

            active_entry = token_stack_push(active_stack, copy_string, 0);
            active_entry->buffer_allocated = 1;
          }
        }
      }
    }
    else {
      int macro_type = MACRO_ARG_NONE;
      int valid_macro = 0;
      int argument_index = 0;
      struct token_stack_entry *active_entry = active_stack->active_entry;

      token_count = 1;

      /* Read and replace known macro data. */
      if (current_token == '!')
        macro_type = MACRO_ARG_FILENAME;
      else if (current_token == '@')
        macro_type = MACRO_ARG_INSTANCE;
      else if (current_token == '\\')
        macro_type = MACRO_ARG_SLASH;
      else {
        int arg_scale = 1;
        token_count = 0;
        
        /* Determine the argument to replace. */
        while (current_token >= '0' && current_token <= '9') {
          argument_index += arg_scale * (current_token - '0');
          arg_scale *= 10;
          token_count++;
          current_token = active_entry->buffer_root[active_entry->buffer_offset - token_count];
        }

        if (current_token == '\\') {
          /* Only flag a potential number argument if it has a preceeding slash. */
          macro_type = MACRO_ARG_NUMBER;
        }
      }

      if (macro_type != MACRO_ARG_NONE) {
        int slash_end = 0;
        current_token = active_entry->buffer_root[active_entry->buffer_offset - token_count];

        while (current_token == '\\') {
          /* Find the leading slash, if the slash count is odd this is a valid argument.
             An even number of slashes will subsitute as a slash. */
          valid_macro = valid_macro == 0 ? 1 : 0;
          slash_end++;
          current_token = active_entry->buffer_root[active_entry->buffer_offset - token_count - slash_end];
        }

        if (valid_macro != 0) {
          switch(macro_type) {
          case MACRO_ARG_NUMBER:
            if (argument_index > 0 && argument_index <= macro_runtime_current->supplied_arguments) {
              /* Return the macro replacement. */
              struct macro_argument *argument = macro_runtime_current->argument_data[argument_index - 1];
              
              /* Set the return offset. */
              active_entry->buffer_offset -= token_count;
              active_entry->restore_offset = token_count + 1;

              if (argument->type == INPUT_NUMBER_ADDRESS_LABEL || argument->type == INPUT_NUMBER_STRING) {
                /* Valid macro, update the index and push a new token. */
                active_entry = token_stack_push(active_stack, argument->string, strlen(argument->string) - 1);
              }
              else if (argument->type == SUCCEEDED || argument->type == INPUT_NUMBER_STACK)
                active_entry = token_stack_push_number(active_stack, argument->value, 0);
              else if (argument->type == INPUT_MACRO_ARGUMENT) {
                int string_length = strlen(argument->string);
                char *copy_string = NULL;
                int strip_quotes = 0;

                if (token_stack_get_flag_state(active_stack, TS_STATE_IN_STRING, TS_STATE_FLAG_ANY) != 0) {
                  if (string_length >= 2 && argument->string[0] == '"' && argument->string[string_length - 1] == '"')
                    strip_quotes = 1;
                }

                if (strip_quotes == 0) {
                  /* Add 1 for the null terminator. */
                  string_length += 1;
                  copy_string = (char *)malloc(string_length);
                  strcpy(copy_string, argument->string);
                }
                else {
                  /* Minus 2 for the quotes +1 for the null terminator. */
                  string_length -= 1;
                  copy_string = (char *)malloc(string_length);
                  memcpy(copy_string, &argument->string[1], string_length - 1);
                  copy_string[string_length - 1] = '\0';
                }

                active_entry = token_stack_push(active_stack, copy_string, strlen(copy_string) - 1);
                active_entry->buffer_allocated = 1;
              }
            }
            break;
          case MACRO_ARG_INSTANCE: {
              /* Return the macro instance. */
              active_entry->restore_offset = token_count + 1;
              active_entry->buffer_offset -= token_count;

              active_entry = token_stack_push_number(active_stack, macro_runtime_current->macro->calls - 1, 0);
            }
            break;
          case MACRO_ARG_SLASH: {
              char *slash = "\\";
	      
              /* Return the macro instance. */
              active_entry->restore_offset = token_count + 1;
              active_entry->buffer_offset -= token_count;

              active_entry = token_stack_push(active_stack, slash, strlen(slash) - 1);
            }
            break;
          case MACRO_ARG_FILENAME: {
              char *macro_file_name = get_file_name(active_file_info_last->filename_id);

              /* Return the macro instance. */
              active_entry->restore_offset = token_count + 1;
              active_entry->buffer_offset -= token_count;

              active_entry = token_stack_push(active_stack, macro_file_name, strlen(macro_file_name) - 1);
              /* Flag this string so it ignores macro replacement, filenames will have slashes that could result in 
                 unexpected replacement. */
              active_entry->local_state = TS_STATE_IGNORE_MACROS;
            }
            break;
          }
        }
      }
    }
  }
}

/*****************************
* token_stack_move
* Purpose: Move the current token position in the buffer.
* Param token_stack: The token stack to move the current position on.
* Param move: The amount of buffer to move.
* Returns: The number of token positions moved.
*****************************/
int token_stack_move(struct token_stack_root *token_stack, int move) {

  int incremented = 0;

  if (move > 0)
    incremented = token_stack_increment(token_stack, move);
  else if (move < 0)
    incremented = token_stack_decrement(token_stack, move);
  else {
    token_stack_argument_ptr process_arg_function = token_stack->process_argument;
    struct token_stack_entry *active_entry = NULL;

    if (process_arg_function == NULL) {
      /* Assign the default process argument function. */
      process_arg_function = token_stack_process_argument;
    }

    while (token_stack->active_entry != active_entry) {
      process_arg_function(token_stack, 1);
      active_entry = token_stack->active_entry;
    }
  }

  return incremented;
}

/*****************************
* token_stack_increment
* Purpose: Increment the token position
* Param token_stack: The token stack to move the current position on.
* Param move: The amount of buffer to move.
* Returns: The number of token positions moved.
*****************************/
int token_stack_increment(struct token_stack_root *token_stack, int increment) {

  int incremented = 0;

  if (token_stack != NULL && token_stack->active_entry != NULL) {
    if (increment > 0) {
      struct token_stack_entry *active_entry = token_stack->active_entry;
      token_stack_argument_ptr process_arg_function = token_stack->process_argument;
      int macro_replacement = 0;

      if (process_arg_function == NULL) {
        /* Assign the default process argument function. */
        process_arg_function = token_stack_process_argument;
      }

      while (incremented < increment) {
        char current_token = token_stack_entry_get_current_token(active_entry);

        if (current_token == 0) {
          /* Handle the null terminator. */
          if (active_entry->parent_entry == NULL) {
            /* Exit we are at the final token of the buffer. */
            break;
          }
          else {
            /* Pop the token stack. */
            active_entry = token_stack_pop(token_stack);
            active_entry->buffer_offset += active_entry->restore_offset;
            active_entry->restore_offset = 0;
            macro_replacement = 1;
          }
        }

        /* Increment the buffer. */
        if (macro_replacement == 0) {
          /* Don't increment the buffer if a macro was just replaced. */
          active_entry->buffer_offset++;
        }
        else {
          /* Reset the macro replacement flag. */
          macro_replacement = 0;
        }

        /* Process the token stack. */
        process_arg_function(token_stack, 1);

        if (token_stack->active_entry != active_entry) {
          macro_replacement = 1;
          active_entry = token_stack->active_entry;
        }
        else if (token_stack_entry_get_current_token(active_entry) != 0 ||
		 (token_stack_entry_get_current_token(active_entry) == 0 && active_entry->parent_entry == NULL)) {
          incremented++;
        }
      }
    }
  }

  return incremented;
}

/*****************************
* token_stack_decrement
* Purpose: Decrement the token position
* Param token_stack: The token stack to move the current position on.
* Param move: The amount of buffer to move.
* Returns: The number of token positions moved.
*****************************/
int token_stack_decrement(struct token_stack_root *token_stack, int decrement) {

  int decremented = 0;
  
  if (token_stack != NULL && token_stack->active_entry != NULL) {
    if (decrement < 0) {
      struct token_stack_entry *active_entry = token_stack->active_entry;
      token_stack_argument_ptr process_arg_function = token_stack->process_argument;
      int macro_replacement = 0;

      if (process_arg_function == NULL) {
        /* Assign the default process argument function. */
        process_arg_function = token_stack_process_argument;
      }

      while (decremented > decrement) {
        /* Decrement the token position. Find any macros and replace them. */

        /* Decrement the buffer. */
        if (macro_replacement == 0) {
          /* Dont decrement the buffer if a macro was just replaced. */
          active_entry->buffer_offset--;
        }
        else {
          /* Reset the macro replacement flag. */
          macro_replacement = 0;
        }

        if (active_entry->buffer_offset < 0) {
          active_entry->buffer_offset = 0;
          /* Handle the null terminator. */
          if (active_entry->parent_entry == NULL) {
            /* Exit we are at the final token of the buffer. */
            break;
          }
          else {
            /* Pop the token stack. */
            active_entry = token_stack_pop(token_stack);
            active_entry->restore_offset = 0;
            continue;
          }
        }

        /* Process the token stack. */
        process_arg_function(token_stack, 0);

        if (token_stack->active_entry != active_entry) {
          /* Flag a macro replacement. */
          macro_replacement = 1;
          active_entry = token_stack->active_entry;
        }
        else {
          /* Handle flagging a decrement on the last token. */
          decremented--;
        }
      }
    }
  }

  return decremented;
}

/*****************************
* token_stack_cmp
* Purpose: Compares the current token against compare, if the values match the buffer is incremented.
* Param token_stack: The token_stack_entry to compare the token against.
* Param compare: The value to compare against.
* Param ignore_case: Flag used to ignore the case during compare (1=ignore case, anything else compares the case).
* Returns: 0 if the tokens dont match, 1 if they do.
*****************************/
int token_stack_cmp(struct token_stack_root *token_stack, char compare, int ignore_case) {

  /* Compare the current token. */
  char current_token = token_stack_get_current_token(token_stack);
  int buffer_move = 0;

  /* If the values are equal move the token stack. */
  if (ignore_case == 1) {
    /* Adjust all values to upper case if ignoring case. */
    compare = toupper(compare);
    current_token = toupper(current_token);
  }

  if (compare == current_token) {
    /* Found a match increment the buffer. */
    buffer_move = token_stack_move(token_stack, 1);
  }

  return (buffer_move > 0);
}

/*****************************
* token_stack_strncmp
* Purpose: Compares a token string of length against the token_stacks current buffer string.
*          The buffer position is incremnted on a successful compare.
* Param token_stack: The token_stack_entry to compare the token against.
* Param compare: The string to compare against.
* Param length: The length of the string to compare.
* Param ignore_case: Flag used to ignore the case during compare (1=ignore case, anything else compares the case).
* Returns: 0 if there is a match, a value otherwise.
*****************************/
int token_stack_strncmp(struct token_stack_root *token_stack, char *compare, int length, int ignore_case) {

  int total_move = 0;
  int move = 0;

  while (total_move < length) {
    /* Compare the current token. */
    move = token_stack_cmp(token_stack, compare[total_move], ignore_case);
    total_move += move;

    if (move == 0) {
      /* Exit early if we failed to move the token buffer. */
      break;
    }
  }

  if (total_move != length) {
    /* Reset the buffer offset. */
    token_stack_move(token_stack, -total_move);
    total_move = 0;
  }

  return !(total_move == length);
}

/*****************************
* token_stack_read_to_token
* Purpose: Read from the token_stack until the end_token is found, max_length characters are read or the 
*          end of the token_stack is reached.
* Param token_stack: The token_stack_entry to read from.
* Param result: Buffer to store the results of the read in.
* Param max_length: The max storage size of the buffer.
* Param end_token: The character to stop reading after.
* Returns: The number of tokens read.
*****************************/
int token_stack_read_to_token(struct token_stack_root *token_stack, char *result, int max_length, char end_token) {

  /* Read the buffer until the requested token is found, max_length is reached or the buffer ends. */
  int total_move = 0;
  int move = 0;
  char current_token;

  do {
    /* Get the current token and update the buffer. */
    current_token = token_stack_get_current_token(token_stack);
    result[total_move] = current_token;

    /* Move the current token. */
    move = token_stack_move(token_stack, 1);
    total_move += move;
  }
  while (current_token != end_token && total_move < max_length && move != 0);

  /* Add a null terminator. */
  if (move == 0)
    total_move++;
  result[total_move] = '\0';

  return total_move;
}

/*****************************
* token_stack_reset
* Purpose: Reset the token stack pointer to the start or end of the buffer.
* Param token_stack: The token stack to reset.
* Param start: Flag to reset the buffer to the start/end (1=start, anything else resets to the end).
*****************************/
void token_stack_reset(struct token_stack_root *token_stack, int start) {

  /* Reset the token stack to the start or end. */
  struct token_stack_entry *active_entry = token_stack->active_entry;

  while (active_entry != NULL && active_entry->parent_entry != NULL) {
    /* Pop the active entry. */
    active_entry = token_stack_pop(token_stack);
  }

  if (active_entry != NULL) {
    /* Clean up the active entry state. */
    active_entry->restore_offset = 0;

    /* Set the buffer start. */
    if (start == 1)
      active_entry->buffer_offset = 0;
    else
      active_entry->buffer_offset = strlen(active_entry->buffer_root);
  }
}

/*****************************
* token_stack_set_state_flag
* Purpose: Set the state flag.
* Param token_stack: The token stack to set the flag on.
* Param state: The state flag to set.
* Param local: Flag used to determine if the local/global or both states are set.
*****************************/
void token_stack_set_state_flag(struct token_stack_root *token_stack, int state, int local) {

  if (token_stack != NULL) {
    /* Set the global state. */
    if ((local & TS_STATE_FLAG_GLOBAL) != 0)
      token_stack->global_state |= state;

    if (token_stack->active_entry != NULL) {
      /* Set the local state. */
      if ((local & TS_STATE_FLAG_LOCAL) != 0)
        token_stack->active_entry->local_state |= state;
    }
  }
}

/*****************************
* token_stack_clear_state_flag
* Purpose: Clear the state flag.
* Param token_stack: The token stack to clear the flag on.
* Param state: The state flag to clear.
* Param local: Flag used to determine if the local/global or both states are cleared.
*****************************/
void token_stack_clear_state_flag(struct token_stack_root *token_stack, int state, int local) {

  if (token_stack != NULL) {
    /* Clear the global state. */
    if ((local & TS_STATE_FLAG_GLOBAL) != 0)
      token_stack->global_state &= ~state;

    if (token_stack->active_entry != NULL) {
      /* Clear the local state. */
      if ((local & TS_STATE_FLAG_LOCAL) != 0)
        token_stack->active_entry->local_state &= ~state;
    }
  }
}

/*****************************
* token_stack_get_flag_state
* Purpose: Test if the token stack state is set.
* Param token_stack: The token stack to test the flag on.
* Param state: The state flag to test.
* Param local: Flag used to determine if the local/global or both states are tested.
* Returns: 0 if no flag is set, non-zero if a flag is set.
*****************************/
int token_stack_get_flag_state(struct token_stack_root *token_stack, int state, int local) {

  int return_state = 0;

  if (token_stack != NULL) {
    /* Test the global state. */
    if ((local & TS_STATE_FLAG_GLOBAL) != 0)
      return_state |= (token_stack->global_state & state);

    if (token_stack->active_entry != NULL) {
      /* Test the local state. */
      if ((local & TS_STATE_FLAG_LOCAL) != 0)
        return_state |= (token_stack->active_entry->local_state & state);
    }
  }

  return return_state;
}


/*****************************
* token_stack_entry_new
* Purpose: Create a new token stack entry.
* Param buffer_ptr: The source buffer data.
* Param buffer_offset: The character offset within the buffer.
* Returns: The new token stack entry.
*****************************/
struct token_stack_entry *token_stack_entry_new(char *buffer_ptr, int buffer_offset) {

  struct token_stack_entry *new_entry = (struct token_stack_entry*) malloc(sizeof(struct token_stack_entry));

  if (new_entry != NULL) {
    /* Set the token entry data. */
    new_entry->parent_entry = NULL;
    new_entry->buffer_root = buffer_ptr;
    new_entry->buffer_offset = buffer_offset;
    new_entry->buffer_allocated = 0;
    new_entry->restore_offset = 0;
    new_entry->local_state = TS_STATE_NONE;
  }
  else
    print_error("Out of memory while allocating a new token_stack_entry.\n", ERROR_ERR);

  return new_entry;
}

/*****************************
* token_stack_entry_new_number
* Purpose: Create a new token stack entry from a number.
* Param number: The value to place in the token stack.
* Param start: A flag to set the buffer offset to the start (1) or end (non-1).
* Returns: The new token stack entry.
*****************************/
struct token_stack_entry *token_stack_entry_new_number(int number, int start) {

  struct token_stack_entry *new_entry = NULL;
  char *new_token_string = (char *)malloc(sizeof(char) * MAX_NAME_LENGTH);

  if (new_token_string != NULL) {
    int buffer_offset = 0;
    
    sprintf(new_token_string, "%i", number);

    if (start != 1) {
      /* Set the buffer offset to the end of the string. */
      buffer_offset = strlen(new_token_string) - 1;
    }

    new_entry = token_stack_entry_new(new_token_string, buffer_offset);
    if (new_entry != NULL)
      new_entry->buffer_allocated = 1;
    else {
      print_error("Out of memory while allocating a buffer for a new number token_stack_entry.\n", ERROR_ERR);
      free(new_token_string);
    }
  }

  return new_entry;
}

/*****************************
* token_stack_entry_free
* Purpose: Free the memory allocated by the token stack entry.
* Param stack_entry: The token_stack_entry to free.
*****************************/
void token_stack_entry_free(struct token_stack_entry *stack_entry) {

  if (stack_entry != NULL) {
    if (stack_entry->buffer_allocated != 0)
      free(stack_entry->buffer_root);
    free(stack_entry);
  }
}

/*****************************
* token_stack_entry_get_current_token
* Purpose: Get the current token at the entry.
* Param stack_entry: The token_stack_entry to get the current token for.
* Returns: The current token in the buffer.
*****************************/
char token_stack_entry_get_current_token(struct token_stack_entry *stack_entry) {

  char token = 0;
  
  if (stack_entry->buffer_root != NULL)
    token = stack_entry->buffer_root[stack_entry->buffer_offset];

  return token;
}

/*****************************
* token_stack_update_root_offset
* Purpose: Updates the position in the root buffer.
* Param token_stack: The token stack to update the root buffer on.
* Param buffer_offset: The buffer offset to adjust to.
*****************************/
void token_stack_update_root_offset(struct token_stack_root *token_stack, int buffer_offset) {

  /* Pop buffers until we are at the root. */
  struct token_stack_entry *root_entry = token_stack->active_entry;

  while (root_entry->parent_entry != NULL)
    root_entry = token_stack_pop(token_stack);

  root_entry->buffer_offset = buffer_offset;
}

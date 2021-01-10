
1. If you want to submit a pull request, please note that we're using ANSI C89/C90 here so that WLA DX could be compiled on old systems as well.
2. Please try to write your code using the same style as the code is written in general inside WLA DX. Examples:
   - Two space indentation
   - Function names and variables are: running_index, not runningIndex or RunningIndex
   - No opening curly brackets on their own line, but on the same line with code
   - Every calloc() (don't use malloc()) must be matched with a free() so that all the memory that is allocated is freed at the end of the
     program. See main.c/procedures_at_exit() for a place to free everything. Targets like AmigaOS need this.
   - The following global variable names are used by the parser (an old and a bad decision, there might be more):
     - i (the current character index to source file being parsed)
     - d (input_number() returns the parsed integer in this)
   - BONUS: Keep comments in lower case, though some keywords etc. can be in upper case

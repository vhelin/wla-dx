# WLA DX Contributing Guide

First off, thanks for taking the time to contribute! 

Contributions are welcome, and are accepted via pull requests. Please review
these guidelines before submitting any pull requests.

## ANSI C Standard
We're using ANSI C89/C90 here so that WLA DX could be compiled on old systems as
well.

## Code Style
Please write your code using the conventions below. When in doubt, use the same
style as the code is written in general inside WLA DX.

### Indent using spaces, not tabs
Do not use tabs in your code. You should set your editor to emit spaces when you
hit the tab key.

### Two space indentation
Indent 2 spaces at a time.

Bad:
```c
if (status == OFF) {
    if (last == NULL) {
        labels = label;
        last = label;
    }
    else {
        last->next = label;
        last = label;
    }
}
```

Good:
```c
if (status == OFF) {
  if (last == NULL) {
    labels = label;
    last = label;
  }
  else {
    last->next = label;
    last = label;
  }
}
```

### Use snake_case function and variable names
Bad:
```c
int runningIndex() {
  // ...
}

int IncludeSize = 0;
```

Good:
```c
int running_index() {
  // ...
}

int include_size = 0;
```

### Put curly braces on the same line
Keep curly brackets on the same line with code, not on their own line.

Bad:
```c
int main()
{
  // ...
}

if (condition)
{
  do_one_thing();
  do_another_thing();
}
```

Good:
```c
int main() {
  // ...
}

if (condition) {
  do_one_thing();
  do_another_thing();
}
```

### Free all memory allocations
Every `calloc()` (don't use `malloc()`) must be matched with a `free()` so that
all the memory that is allocated is freed at the end of the program. See
`procedures_at_exit()` at `main.c` for a place to free everything. Targets like
AmigaOS need this.

### Be aware of global variables
The following global variable names are used by the parser (an old and a bad
decision, there might be more):
  - `ind` and `inz` (could be replaced with local vars)
  - `d` (input_number() returns the parsed integer in this)

### Keep comments in lower case
Keep comments in lower case, though some keywords etc. can be in upper
case

### How to add tests

a) Clone an existing test directory and modify it
or
b) Insert tests into existing test source files / makefiles

No need to list the new tests anywhere; a directory inside tests is considered one test directory
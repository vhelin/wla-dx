
#ifndef _MAIN_H
#define _MAIN_H

#include <stdarg.h>
#include <stdio.h>

void print_text(int is_stdout, const char *format, ...);
void print_text_using_args(int is_stdout, const char *format, va_list args);
int create_tmp_file(FILE **file_out_ptr);

#endif

// Formatted print implementation

#ifndef SPRINT_H
#define SPRINT_H

#include<mint/types.h>
#include <stdarg.h>

u32 print_format_to_buffer_arg(const char* data, char* buf, u32 len, va_list arg);

#endif

// Formatted print implementation

#ifndef SPRINT_H
#define SPRINT_H

#include<mint/types.h>
#include <stdarg.h>

u32 sprint(const char* data, char* buf, u32 size, va_list arg);

#endif

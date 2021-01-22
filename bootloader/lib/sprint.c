// Formatted print implementation

#include <lib/sprint.h>


u32 sprint(const char* data, char* buf, u32 size, va_list arg) {
    u32 count;
    for(count = 0; *data; count++) {
        *buf++ = *data++;
    }
    return count;
}

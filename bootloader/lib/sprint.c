// Formatted print implementation

#include <lib/sprint.h>

#define FLAG_MINUS  0x01
#define FLAG_PLUS   0x02
#define FLAG_SPACE  0x04
#define FLAG_HASH   0x08
#define FLAG_NULL   0x10
#define FLAG_LITTLE 0x20
#define FLAG_SIGN   0x40

// This formats a given string as places the given result in a buffer
u32 sprint(const char* data, char* buf, u32 max_size, va_list arg) {
    u32 buf_size = 0;
    for (; *data; data++) {
        if (*data != '%') {
            if (buf_size >= max_size) {
                // Buffer overload
                return buf_size;
            }
            *buf++ = *data;
            buf_size++;
            continue;
        }

        // If the character starts a format exspression
        u8 flags = 0;
        flag_loop:
        data++;

        switch (*data) {
            case '-': flags |= FLAG_MINUS; goto flag_loop;
            case '+': flags |= FLAG_PLUS;  goto flag_loop;
            case ' ': flags |= FLAG_SPACE; goto flag_loop;
            case '#': flags |= FLAG_HASH;  goto flag_loop;
            case '0': flags |= FLAG_NULL;  goto flag_loop;
        }

        // Parse the formal number
        u32 size_valid = 0;
        u32 size = 0;
        if (*data == '*') {
            size = (u32)va_arg(arg, unsigned int);
            size_valid = 1;
            data++;
        } else {
            while ((*data >= '0') && (*data <= '9')) {
                size = size * 10 + (*data++ - '0');
                size_valid = 1;
            }
        }

        // Parse the type field
        u32 base = 0;
        const char* ptr;
        switch (*data) {
            case 's':
                ptr = va_arg(arg, char*);
                if (size_valid == 0) {
                    while (*ptr) {
                        if (buf_size >= max_size) {
                            // Overflow
                            return buf_size;
                        }
                        *buf++ = *ptr++;
                        buf_size++;
                    }
                } else {
                    u32 pad = size;
                    for (u32 i = 0; i < size; i++) {
                        if (ptr[i] == 0) {
                            break;
                        }
                        pad--;
                    }
                    size -= pad;

                    // Pad will hold the padding or zero
                    // Size will hold the total number of bytes to be written

                    if ((flags & FLAG_MINUS) == 0) {
                        while (pad--) {
                            if (buf_size >= max_size) {
                                // Overflow
                                return buf_size;
                            }
                            *buf++ = ' ';
                            buf_size++;
                        }
                    }

                    for (u32 i = 0; i < size; i++) {
                        if (buf_size >= max_size) {
                            // Overflow
                            return buf_size;
                        }
                        *buf++ = *ptr++;
                        buf_size++;
                    }

                    if (flags & FLAG_MINUS) {
                        while (1);
                        while (pad--) {
                            if (buf_size >= max_size) {
                                // Overflow
                                return buf_size;
                            }
                            *buf++ = ' ';
                            buf_size++;
                        }
                    }
                }
                continue;
            
            case 'c':
                char c = (char)va_arg(arg, char);
                if (buf_size >= max_size) {
                    // Overflow
                    return buf_size;
                }
                *buf++ = c;
                buf_size++;
                continue;

            case '%':
                if (buf_size >= max_size) {
                    // Overflow
                    return buf_size;
                }
                *buf++ = '%';
                buf_size++;
                continue;

            case 'p':
                flags |= FLAG_LITTLE;
            
            case 'P':
                flags |= FLAG_HASH;
                size = 8;
                base = 16;
                break;
            
            case 'i':
                flags |= FLAG_SIGN;

            case 'd':
                base = 10;
                break;

            case 'x':
                flags |= FLAG_LITTLE;

            case 'X':
                base = 16;
                break;
            
            case 'b':

            case 'B':
                base = 2;
                break;
            
            default:
                data--;
                continue;
        }

    }
    return buf_size;
}
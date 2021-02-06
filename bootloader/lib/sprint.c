// Formatted print implementation

#include <lib/sprint.h>

#define FLAG_LEFT        0x0001
#define FLAG_FORCE_SIGN  0x0002
#define FLAG_IGNORE_SIGN 0x0004
#define FLAG_PREFIX      0x0008
#define FLAG_PAD_ZERO    0x0010
#define FLAG_LOWERCASE   0x0020
#define FLAG_SIGNED      0x0040
#define FLAG_STRING      0x0080
#define FLAG_CHAR        0x0100
#define FLAG_BRACKET     0x0200

// Writes one character to the buf double pointer and increments the buf pointer
// accordingly. This will ignore the write if the buffer is full
static inline void write_char(char c, char** buf, char* end) {
    if (*buf < end) {
        **buf = c;
        (*buf)++;
    }
}

// This formats a given string as places the given result in a buffer
u32 sprint(const char* data, char* buf, u32 len, va_list arg) {
    // Get a pointer to the char after the buffer
    char* end = buf + len;
    for (; *data; data++) {
        if (*data != '{') {
            write_char(*data, &buf, end);
            continue;
        }

        // Parse the flags
        u16 flags = 0;

        while (*++data) {
            if      (*data == '<') flags |= FLAG_LEFT;
            else if (*data == '+') flags |= FLAG_FORCE_SIGN;
            else if (*data == ' ') flags |= FLAG_SIGNED;
            else if (*data == '!') flags |= FLAG_PREFIX;
            else if (*data == '0') flags |= FLAG_PAD_ZERO;
            else break;
        }

        // Skip any delimiter
        if (*data == ':') {
            data++;
        }

        // Parse the format number
        i32 width = -1;
        if (*data == '_') {
            width = (i32)va_arg(arg, int);
            data++;
        } else if ((*data >= '0') && (*data <= '9')) {
            width = 0;
            while ((*data >= '0') && (*data <= '9')) {
                width = width * 10 + (*data++ - '0');
            }
        }

        

    }
    return buf - (end - len);
}

// Formatted print implementation

#include <mint/print_format.h>

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

static const char* lookup = "0123456789ABCDEF";

// Writes one character to the buf double pointer and increments the buf pointer
// accordingly. This will ignore the write if the buffer is full
static inline void write_char(char c, char** buf, char* end) {
    if (*buf < end) {
        **buf = c;
        (*buf)++;
    }
}

// This formats a given string as places the given result in a buffer
u32 print_format_to_buffer_arg(const char* data, char* buf, u32 len, va_list arg) {
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
            else if (*data == ' ') flags |= FLAG_IGNORE_SIGN;
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

        // Skip any delimiter
        if (*data == ':') {
            data++;
        }

        // Parse the option
        u32 base = 0;
        switch (*data++) {
            case 'h':
                flags |= FLAG_LOWERCASE;
            case 'H':
                base = 16;
                flags |= FLAG_PREFIX;
                break;
            case 'b':
                base = 2;
                flags |= FLAG_PREFIX;
                break;
            case 'p':
                flags |= FLAG_LOWERCASE;
            case 'P':
                base = 16;
                width = 10;
                flags |= FLAG_PREFIX | FLAG_PAD_ZERO;
                break;
            case 'r':
                base = 2;
                width = 34;
                flags |= FLAG_PREFIX | FLAG_PAD_ZERO;
                break;
            case 'i':
                flags |= FLAG_SIGNED;
            case 'u':
                base = 10;
                break;
            case 's':
                flags |= FLAG_STRING;
                break;
            case 'c':
                flags |= FLAG_CHAR;
                break;
            case '{':
                flags |= FLAG_BRACKET;
                break;
            default:
                data--;
                continue;
        }

        // Print a bracket
        if (flags & FLAG_BRACKET) {
            write_char('{', &buf, end);
        
        // Print a char
        } else if (flags & FLAG_CHAR) {
            write_char((char)va_arg(arg, int), &buf, end);
        
        // Print a string
        } else if (flags & FLAG_STRING) {
            // Get the string pointer
            char* input = (char*)va_arg(arg, char *);

            if (width < 0) {
                while (*input) {
                    write_char(*input++, &buf, end);
                }
            } else {
                // Pad holds the number of padding
                // Num holds the nomber of characters to print from the input buffer 
                u32 num;
                for (num = 0; num < width && input[num]; num++);
                u32 pad = width - num;

                // Pad in front of the string
                if ((flags & FLAG_LEFT) == 0) {
                    for (u32 i = 0; i < pad; i++) {
                        write_char(' ', &buf, end);
                    }
                }

                // write the string
                for (u32 i = 0; i < num; i++) {
                    write_char(*input++, &buf, end);
                }
                
                // Padd after the string
                if (flags & FLAG_LEFT) {
                    for (u32 i = 0; i < pad; i++) {
                        write_char(' ', &buf, end);
                    }
                }

            }
        } else {
            u32 input = va_arg(arg, int);
            char sign = 0;  
            // Chack for a negative input
            if (flags & FLAG_SIGNED && (i32)input < 0) {
                sign = '-';
                input = -(i32)input;
            }

            // Check if we have to write a pluss sign
            if (sign == 0 && (flags & FLAG_FORCE_SIGN)) {
                sign = '+';
            }
            // Ignore the sign
            if (flags & FLAG_IGNORE_SIGN) {
                sign = ' ';
            }

            // Convert to a char buffer
            char buffer[32];
            u32 index = 0;
            do {
                buffer[index++] = lookup[input % base] | (flags & FLAG_LOWERCASE);
                input = input / base;
            } while (input);
            
            // Find the width of the prefix and sign
            u32 prefix_width = 0;

            if (flags & FLAG_PREFIX && (base == 2 || base == 16)) {
                prefix_width += 2;
            }

            if (sign) {
                prefix_width += 1;
            }

            // Find the padding
            u32 pad = 0;
            if (width != -1) {
                pad = ((index + prefix_width) > width) ? 0 : width - prefix_width - index;
            }

            if (sign) {
                write_char(sign, &buf, end);
            }

            if (flags & FLAG_PREFIX) {
                if (base == 2) {
                    write_char('0', &buf, end);
                    write_char('b', &buf, end);
                } else if (base == 16) {
                    write_char('0', &buf, end);
                    write_char('x', &buf, end);

                }
            }

            // Find the padding character
            char pad_char = (flags & FLAG_PAD_ZERO) ? '0' : ' ';
            
            // Left pad
            if ((flags & FLAG_LEFT) == 0) {
                while (pad--) {
                    write_char(pad_char, &buf, end);
                }
            }

            // Write the number
            while (index--) {
                write_char(buffer[index], &buf, end);
            }

            // Right pad
            if (flags & FLAG_LEFT) {
                while (pad--) {
                    write_char(pad_char, &buf, end);
                }
            }
        }

        // Skip any end bracket
        if (*data != '}') {
            data--;
        }
    }
    return buf - (end - len);
}

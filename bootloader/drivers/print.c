// Driver for print over UART1

#include <drivers/print.h>
#include <drivers/uart.h>
#include <mint/print_format.h>

#define PRINT_BUF_SIZE 512
static char print_buf[PRINT_BUF_SIZE];

void print_init(void) {
    uart_init(UART1_REG, 115200);
}

void print(const char* data, ...) {
    va_list arg;
    va_start(arg, data);
    u32 count = print_format_to_buffer_arg(data, print_buf, PRINT_BUF_SIZE, arg);
    va_end(arg);

    const char* ptr = print_buf;
    while (count--) {
        if (*ptr == '\n') {
            uart_transmit_char(UART1_REG, '\r');
        }
        uart_transmit_char(UART1_REG, *ptr++);
    }
}


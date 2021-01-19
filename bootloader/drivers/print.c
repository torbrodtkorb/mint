#include <drivers/print.h>
#include <drivers/uart.h>

void print_init(void) {
    uart_init(UART1, 115200);
}

void print(const char* data) {
    uart_print(UART1, data);
}


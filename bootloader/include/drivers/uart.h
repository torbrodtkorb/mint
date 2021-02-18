
#ifndef UART_H
#define UART_H

#define UART_CNT 4

#include <mint/types.h>
#include <mint/regmap.h>

void uart_init(struct uart_reg* reg, u32 baud);

void uart_transmit_char(struct uart_reg* reg, char c);

void uart_print(struct uart_reg* reg, const char* data);

#endif

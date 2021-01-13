
#ifndef UART_H
#define UART_H

#include <mint/types.h>
#include <mint/regmap.h>

void uart_init(struct uart_reg* reg, u32 baud);

#endif

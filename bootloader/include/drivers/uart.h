
#ifndef UART_H
#define UART_H

#define UART_CNT 4

#include <mint/types.h>
#include <mint/regmap.h>


static inline u32 uart_to_pid(struct uart_reg* reg);

void uart_init(struct uart_reg* reg, u32 baud);

void uart_print(struct uart_reg* reg, const char* data);

#endif

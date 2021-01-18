// Driver for UART

#include <drivers/uart.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>


// Uart pin rx on PD2 and TX on PD3, both on peripheral A
void uart_init(struct uart_reg* reg, u32 baud) {
    clk_peripheral_enable(25);
    u32 per_freq = clk_get_peripheral_freq(25);

    gpio_set_function(GPIOD, 2, GPIO_FUNC_A);
    gpio_set_function(GPIOD, 3, GPIO_FUNC_A);

}

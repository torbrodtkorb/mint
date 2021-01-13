#include <mint/types.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>

// PA10, PB1, PA31
void main(void)
{
    // Enable the clock of the PIO module
    clk_peripheral_enable(18);
    
    // LED configuration
    struct gpio_conf conf = {
        .dir = GPIO_OUTPUT,
        .drive = GPIO_HIGH,
        .pull = GPIO_PULL_NONE
    };


    // Turn on LED
    gpio_set_conf(GPIOA, 31, &conf);

    while (1) {
        gpio_clear(GPIOA, 10);
        gpio_set(GPIOA, 31);
        for (u32 i = 0; i < 5000000; i++) {
            asm("nop");
        }
        gpio_clear(GPIOA, 31);
        gpio_set(GPIOA, 10);
        for (u32 i = 0; i < 5000000; i++) {
            asm("nop");
        }
    }
    
}

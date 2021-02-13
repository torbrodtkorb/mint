#include <mint/types.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>
#include <drivers/print.h>

// PA10, PB1, PA31
void main(void)
{
    clk_init();
    print_init();

    // Enable the clock of the PIO module
    clk_peripheral_enable(18);
    
    // LED configuration
    struct gpio_conf conf = {
        .dir = GPIO_OUTPUT,
        .drive = GPIO_HIGH,
        .pull = GPIO_PULL_NONE
    };


    // Turn on LED
    gpio_set_conf(GPIOA_REG, 31, &conf);

    while (1) {
        gpio_clear(GPIOA_REG, 10);
        gpio_set(GPIOA_REG, 31);
        for (u32 i = 0; i < 500000; i++) {
            asm("nop");
        }
        gpio_clear(GPIOA_REG, 31);
        gpio_set(GPIOA_REG, 10);
        for (u32 i = 0; i < 500000; i++) {
            asm("nop");
        }
        char len = 10
        u32 = "hei"
        print("Dette er en{_:s}\n", len, "hei");
    }
}

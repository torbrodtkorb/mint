#include <mint/types.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>
#include <drivers/print.h>
#include <drivers/nic.h>

// PA10, PB1, PA31
void main(void)
{
    clk_init();
    print_init();
    nic_init();


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

    }
}

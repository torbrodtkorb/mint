// Driver for the GPIO module

#include <drivers/gpio.h>

// This funktion will mask a pin. This must be done before applying a 
// configuration on the pin 
static inline void mask_pin(struct gpio_reg* port, u32 pin) { 
    port->MSKR = (1 << pin);
}

void gpio_set_conf(struct gpio_reg* port, u32 pin, struct gpio_conf* conf) {
    // Mask the pin
    mask_pin(port, pin);
    u32 reg = (conf->dir << 8) |
              (conf->drive << 16);

    // Set pull mode
    if (conf->pull == GPIO_PULL_UP) {
        reg |= (1 << 9);
    }
    else if (conf->pull == GPIO_PULL_DOWN) {
        reg |= (1 << 10);
    }
    port->CFGR = reg;
}

void gpio_set(struct gpio_reg* port, u32 pin) {

}
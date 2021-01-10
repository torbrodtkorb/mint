// Driver for the GPIO module

#include <drivers/gpio.h>

// This function will mask a pin. This must be done before applying a 
// configuration on the pin 
static inline void mask_pin(struct gpio_reg* port, u32 pin) { 
    port->MSKR = (1 << pin);
}

// Applies a configuration for the given pin. This includes direction, drive and
// pull mode
void gpio_set_conf(struct gpio_reg* port, u32 pin, struct gpio_conf* conf) {
    // Mask the pin to configure 
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

// Drives the given pin to VCC
void gpio_set(struct gpio_reg* port, u32 pin) {
    port->SODR = (1 << pin);
}

// Drives the given pin to GND
void gpio_clear(struct gpio_reg* port, u32 pin) {
    port->CODR = (1 << pin);
}
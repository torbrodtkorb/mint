
#ifndef GPIO_H
#define GPIO_H

// Start of PIOA 0xFC038000

#include <mint/types.h>

struct gpio_reg {
    _rw u32 MSKR;
    _rw u32 CFGR;
    __r u32 PDSR;
    __r u32 LOCKSRS;
    __w u32 SODR;
    __w u32 CODR;
    _rw u32 ODSR;
    __r u32 RESERVED1;
    __w u32 IER;
    __w u32 IDR;
    __r u32 IMR;
    __r u32 ISR;
};

#define GPIOA ((struct gpio_reg *)0xFC038000)
#define GPIOB ((struct gpio_reg *)0xFC038040)
#define GPIOC ((struct gpio_reg *)0xFC038080)
#define GPIOD ((struct gpio_reg *)0xFC0380C0)

enum gpio_dir {
    GPIO_INPUT,
    GPIO_OUTPUT
};

enum gpio_pull {
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_PULL_NONE
};

enum gpio_drive {
    GPIO_LOW = 1,
    GPIO_MEDIUM,
    GPIO_HIGH
};

struct gpio_conf {
    enum gpio_dir dir;
    enum gpio_pull pull;
    enum gpio_drive drive;
};

void gpio_set_conf(struct gpio_reg* port, u32 pin, struct gpio_conf* conf);

void gpio_set(struct gpio_reg* port, u32 pin);

#endif

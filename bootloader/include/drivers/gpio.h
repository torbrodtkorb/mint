
#ifndef GPIO_H
#define GPIO_H

#include <mint/types.h>
#include <mint/regmap.h>

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

void gpio_clear(struct gpio_reg* port, u32 pin);

#endif

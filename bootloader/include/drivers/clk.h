// Driver for the PMC clock module

#ifndef CLK_H
#define CLK_H

#include <mint/types.h>
#include <drivers/matrix.h>

void clk_init(void);

void clk_peripheral_enable(u32 pid);

void clk_peripheral_disable(u32 pid);

u32 clk_get_peripheral_freq(u32 pid);

#endif
 
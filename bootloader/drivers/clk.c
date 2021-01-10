#include <drivers/clk.h>
#include <mint/reg_map.h>


// Enables the peripheral clock that corresponds with the given PID
void clk_peripheral_enable(u32 pid) {
    if (pid < 2) { return; }

    // Two registers are used to enables the peripheral clock
    if (pid < 32) {
        PMC->PCER0 = (1 << pid);
    } else if (pid < 64) {
        PMC->PCER1 = (1 << (pid - 32));
    }
}
// Disables the peripheral clock that corresponds with the given PID
void clk_peripheral_disable(u32 pid) {
    if (pid < 2) { return; }

    // Two registers are used to disables the peripheral clock
    if (pid < 32) {
        PMC->PCDR0 = (1 << pid);
    } else if (pid < 64) {
        PMC->PCDR1 = (1 << (pid - 32));
    }
}

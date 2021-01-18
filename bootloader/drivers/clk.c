#include <drivers/clk.h>
#include <mint/regmap.h>

static u32 matrix_64_freq;
static u32 matrix_32_freq;

// Initializes clk frequency
void clk_init(void) {
    matrix_32_freq = 12000000;
    matrix_64_freq = 12000000;
}

// Enables the peripheral clock that corresponds with the given PID
void clk_peripheral_enable(u32 pid) {
    // Get the hardware
    struct pmc_reg* const hw = PMC;

    if (pid < 2) { return; }

    // Two registers are used to enables the peripheral clock
    if (pid < 32) {
        hw->pcer0 = (1 << pid);
    } else if (pid < 64) {
        hw->pcer1 = (1 << (pid - 32));
    }
}
// Disables the peripheral clock that corresponds with the given PID
void clk_peripheral_disable(u32 pid) {
    // Get the hardware
    struct pmc_reg* const hw = PMC;
    
    if (pid < 2) { return; }

    // Two registers are used to disables the peripheral clock
    if (pid < 32) {
        hw->pcdr0 = (1 << pid);
    } else if (pid < 64) {
        hw->pcdr1 = (1 << (pid - 32));
    }
}

// This returns the peripheral frequenzy givenen thie PID number. 
u32 clk_get_peripheral_freq(u32 pid) {
    if (get_matrix(pid) == MATRIX32) {
        return matrix_32_freq;
    } else {
        return matrix_64_freq;
    }
}

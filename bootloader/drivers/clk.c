#include <drivers/clk.h>
#include <mint/regmap.h>

static u32 matrix_64_freq;
static u32 matrix_32_freq;

// Initializes clk frequency
void clk_init(void) {
    struct pmc_reg* const hw = PMC_REG_REG;
    u32 reg;
    
    // Enable clock source 12 Mhz
    reg = hw->mor & ~((0xFF << 16) | (1 << 4) | (1 << 6));
    hw->mor = reg | (1 << 3) | (1 << 5) | 0x370000;
    
    // Wait for the PC to start
    while ((hw->sr & (1 << 17)) == 0);
    
    // Set master clock to MAIN CLOCK
    reg = hw->mckr & ~0b11;
    hw->mckr = reg | 0b1;

    // wait for the swith to complete
    while ((hw->sr & (1 << 3)) == 0);
    
    // Disable the PLL to avoid overclocking
    hw->pllar &= ~(0b1 | (0x3F << 18));

    // Select clock source (MAIN CLOCK)
    reg = hw->mor & ~((0xFF << 16) | (1 << 4) | (1 << 6) | (1 << 24));
    hw->mor = reg | 0x370000;

    while ((hw->sr & (1 << 16)) == 0);

    // Configurate PLLA to 12 Mhz x 83 = 996 Mhz
    hw->pllar = (1 << 29) | (82 << 18) | (0x3F << 8) | 0x1;

    // wait for PLLA configuration to complete
    while ((hw->sr & (1 << 1)) == 0);

    // Set the prescaler, PLL div and H32MX div
    reg = hw->mckr & ~(0b111 << 4);
    hw->mckr = reg | (1 << 12) | (1 << 24);
    while ((hw->sr & (1 << 3)) == 0);

    reg = hw->mckr & ~(0b11 << 8);
    hw->mckr = reg | (3 << 8);
    while ((hw->sr & (1 << 3)) == 0);

    reg = hw->mckr & ~0b11;
    hw->mckr = reg | 2;
    while ((hw->sr & (1 << 3)) == 0);

    // Update the frequency variables
    matrix_32_freq = 83000000;
    matrix_64_freq = 166000000;
}

// Enables the peripheral clock that corresponds with the given PID
void clk_peripheral_enable(u32 pid) {
    // Get the hardware
    struct pmc_reg* const hw = PMC_REG_REG;

    if (pid < 2) { return; }

    // Two registers are used to enables the peripheral clock
    if (pid < 32) {
        hw->pcer0 = (1 << pid);
        while ((hw->pcsr0 & (1 << pid)) == 0);
    } else if (pid < 64) {
        hw->pcer1 = (1 << (pid - 32));
        while ((hw->pcsr1 & (1 << (pid - 32))) == 0);
    }
}

// Disables the peripheral clock that corresponds with the given PID
void clk_peripheral_disable(u32 pid) {
    // Get the hardware
    struct pmc_reg* const hw = PMC_REG_REG;
    
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

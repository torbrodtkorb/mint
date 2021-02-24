// NIC driver

#include <drivers/nic.h>
#include <mint/regmap.h>
#include <drivers/gpio.h>
#include <drivers/clk.h>
#include <drivers/print.h>

#define PHY_ADDR 0

// Both the register index and the phy address is between 0 and 31
static u16 phy_read(u8 reg, u8 phy) {
    NicReg* regs = NIC_REG;

    regs->man = (1 << 17) | (reg << 18) | (phy << 23) | (1 << 29) | (1 << 30);
    
    while ((regs->nsr & (1 << 2)) == 0);

    return (u16)regs->man;
}

static void phy_test() {
    for (u32 i = 0; i < 32; i++) {
        u16 reg = phy_read(2, i);
        print("Phy reg {!0:6:h}\n", reg);
    }
}

void nic_init() {
    // Configurate the nic pins
    gpio_set_function(GPIOD_REG, 9 , GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 10, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 11, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 12, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 13, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 14, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 15, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 16, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 17, GPIO_FUNC_D);
    gpio_set_function(GPIOD_REG, 18, GPIO_FUNC_D);

    // Enable the clock
    clk_peripheral_enable(5);

    // Get the hardware base address
    NicReg* regs = NIC_REG;

    // Enable the PHY interface
    regs->ncfgr = (3 << 18);
    regs->ncr = (1 << 4);

}

Netbuf* nic_recive() {
return 0;
}

void nic_send(Netbuf* buf) {

}

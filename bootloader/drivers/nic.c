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

// The grgister has to be in range 0 to 31
static void phy_write(u8 reg, u8 phy, u16 data) {
    NicReg* regs = NIC_REG;

    regs->man = (1 << 30) | (1 << 28) | (phy << 23) | (reg << 18) | (1 << 17) | data;

    while ((regs->nsr & (1 << 2)) == 0);
}

typedef enum {
    SPEED_10,
    SPEED_100
} SpeedType;

typedef enum {
    HALF_DUPLEX,
    FULL_DUPLEX
} DuplexType;

typedef struct {
    SpeedType speed;
    DuplexType duplex;
} LinkSetting;

// Configures the PHY
static void phy_init(LinkSetting* setting) {

    // Set our capability to half and full duplex, 10 and 100 Mbit/sek
    u16 reg = phy_read(4, PHY_ADDR);
    reg |= (0b1111 << 5);
    phy_write(4, PHY_ADDR, reg);

    // Reset autoneg
    reg = phy_read(0, PHY_ADDR);
    reg |= (1 << 9);
    phy_write(0, PHY_ADDR, reg);

    // Wait for autoneg to complete
    while ((phy_read(1, PHY_ADDR) & (1 << 5)) == 0);

    print("Autoneg cloplete\n");

    // Read LINK PARTNER capability
    reg = phy_read(5, PHY_ADDR);
    
    if (reg & (0b11 << 7)) {
        setting->speed = SPEED_100;

        if (reg & (1 << 8)) {
            setting->duplex = FULL_DUPLEX;
        } else {
            setting->duplex = HALF_DUPLEX;
        }
    } else {
        setting->speed = SPEED_10;

        if (reg & (1 << 6)) {
            setting->duplex = FULL_DUPLEX;
        } else {
            setting->duplex = HALF_DUPLEX;
        }
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

    LinkSetting setting;
    phy_init(&setting);
    print("Link status D{u} S{u}\n", setting.duplex, setting.speed);

}

Netbuf* nic_recive() {
return 0;
}

void nic_send(Netbuf* buf) {

}

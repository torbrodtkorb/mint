// NIC driver

#include <drivers/nic.h>
#include <mint/regmap.h>
#include <drivers/gpio.h>
#include <drivers/clk.h>
#include <drivers/print.h>
#include <stdalign.h>

#define PHY_ADDR 0
#define OWNER_DMA 0
#define OWNER_CPU 1

typedef struct {
    union {
        u32 addr_word;
        struct {
            u32 owner : 1;
            u32 wrap  : 1;
            u32 addr  : 30;
        };
    };

    union {
        u32 status_word;
        struct {
            u32 len                  : 13;
            u32 fcs_status           : 1;
            u32 sof                  : 1;
            u32 eof                  : 1;
            u32 cfi                  : 1;
            u32 vlan_pri             : 3;
            u32 pri_tag_detected     : 1;
            u32 vlan_tag_detected    : 1;
            u32 type_id              : 2;
            u32 type_id_match        : 1;
            u32 addr_match_reg       : 2;
            u32 addr_match           : 1;
            u32 reserved             : 1;
            u32 unicast_hash_match   : 1;
            u32 multicast_hash_match : 1;
            u32 broadcast_detected   : 1;
        };
    };
} ReceiveDesc;

typedef struct {
    u32 addr;
    union {
        u32 status_word;
        struct {
            u32 len            : 14;
            u32 reserved0      : 1;
            u32 last           : 1;
            u32 ignore_crc     : 1;
            u32 reserved1      : 3;
            u32 crc_errors     : 3;
            u32 reserved2      : 3;
            u32 late_collision : 1;
            u32 ahb_corrupted  : 1;
            u32 reserved3      : 1;
            u32 retry_error    : 1;
            u32 wrap           : 1;
            u32 owner          : 1;
        };
    };
} TransmitDesc;

u32 receive_index;
u32 transmit_index;

// This is the main queue
#define QUEUE0_TX_RING_SIZE 4
#define QUEUE0_RX_RING_SIZE 8

static alignas(8) TransmitDesc queue0_tx_ring[QUEUE0_TX_RING_SIZE];
static alignas(8) ReceiveDesc queue0_rx_ring[QUEUE0_RX_RING_SIZE];

// Three unused queues
#define UNUSED_TX_RING_SIZE 2
#define UNUSED_RX_RING_SIZE 2

static alignas(8) TransmitDesc queue1_tx_ring[UNUSED_TX_RING_SIZE];
static alignas(8) ReceiveDesc queue1_rx_ring[UNUSED_RX_RING_SIZE];

static alignas(8) TransmitDesc queue2_tx_ring[UNUSED_TX_RING_SIZE];
static alignas(8) ReceiveDesc queue2_rx_ring[UNUSED_RX_RING_SIZE];

static alignas(8) TransmitDesc queue3_tx_ring[UNUSED_TX_RING_SIZE];
static alignas(8) ReceiveDesc queue3_rx_ring[UNUSED_RX_RING_SIZE];

#define NUM_QUEUES 4

typedef struct {
    ReceiveDesc* rx;
    u32 rx_size;

    TransmitDesc* tx;
    u32 tx_size;
} Queue;

Queue queues[NUM_QUEUES] = {
    {
        .rx = queue0_rx_ring,
        .tx = queue0_tx_ring,

        .rx_size = QUEUE0_RX_RING_SIZE,
        .tx_size = QUEUE0_TX_RING_SIZE
    },
    {
        .rx = queue1_rx_ring,
        .tx = queue1_tx_ring,

        .rx_size = UNUSED_RX_RING_SIZE,
        .tx_size = UNUSED_TX_RING_SIZE
    },
    {
        .rx = queue2_rx_ring,
        .tx = queue2_tx_ring,

        .rx_size = UNUSED_RX_RING_SIZE,
        .tx_size = UNUSED_TX_RING_SIZE
    },
    {
        .rx = queue3_rx_ring,
        .tx = queue3_tx_ring,

        .rx_size = UNUSED_RX_RING_SIZE,
        .tx_size = UNUSED_TX_RING_SIZE
    },
};

// Init transmit and recive buffers
void init_queues() {

    receive_index = 0;
    transmit_index = 0;
    
    for (u32 i = 0; i < NUM_QUEUES; i++) {
        Queue* curr = &queues[i];

        // Settup the receive descriptores
        for (u32 ii = 0; ii < curr->rx_size; ii++) {
            ReceiveDesc* desc = &curr->rx[ii];
            
            desc->status_word = 0;
            desc->addr_word   = 0;
            desc->owner       = OWNER_DMA;
            desc->wrap        = 0;
            
            Netbuf* buf = alloc_netbuf();

            if (buf == NULL) {
                print("Cannot alloocate enough netbuffers for the init ring\n");
            }
            desc->addr = (u32)buf->buf >> 2;

        }

        curr->rx[curr->rx_size - 1].wrap = 1;

        // Settup the transmit descriptores
        for (u32 ii = 0; ii < curr->tx_size; ii++) {
            TransmitDesc* desc = &curr->tx[ii];
            
            desc->status_word = 0;
            desc->addr        = 0;
            desc->owner       = OWNER_CPU;
            desc->wrap        = 0;

            Netbuf* buf = alloc_netbuf();

            if (buf == NULL) {
                print("Cannot alloocate enough netbuffers for the init ring\n");
            }

            desc->addr = (u32)buf->buf;
        }
        curr->tx[curr->tx_size - 1].wrap = 1;
    }

    // Sett the base address of the descreptor
    NicReg* regs = NIC_REG;

    regs->rbqb = (u32)queue0_rx_ring;
    regs->tbqb = (u32)queue0_tx_ring;

    regs->rbqbapq[0] = (u32)queue1_rx_ring;
    regs->tbqbapq[0] = (u32)queue1_tx_ring;

    regs->rbqbapq[1] = (u32)queue2_rx_ring;
    regs->tbqbapq[1] = (u32)queue2_tx_ring;

    regs->rbqbapq[2] = (u32)queue3_rx_ring;
    regs->tbqbapq[2] = (u32)queue3_tx_ring;
}





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

    // Reset the registers
    regs->ncr   = 0;
    regs->ncfgr = 0;

    regs->idr = 0xFFFFFFFF;

    (void)regs->isr;

    // Enable the PHY interface
    regs->ncfgr = (3 << 18);
    regs->ncr = (1 << 4);

    LinkSetting setting;
    phy_init(&setting);

    // Init the netbuffer interface
    netbuf_init();
    init_queues();


    regs->ncfgr |= (1 << 0) | (1 << 1) | (1 << 4);
    
    // Choose RMII interface
    regs->ur = 1;

    // Configure the DMA controller
    regs->dcfgr = (4 << 0) | (3 << 8) | (1 << 10) | (0x18 << 16);

    // Enable the reciver and transmitter
    regs->ncr |= (1 << 2) | (0 << 3);

}

Netbuf* nic_recive() {
    ReceiveDesc* desc = &queue0_rx_ring[receive_index];

    if (desc->owner == OWNER_CPU) {
        receive_index++;
        if (receive_index & QUEUE0_RX_RING_SIZE) {
            receive_index = 0;
        }

        Netbuf*  result = (Netbuf *)((desc->addr << 2) - offsetof(Netbuf, buf));
        result->len = desc->len;

        Netbuf* new = alloc_netbuf();
        
        desc->addr = (u32)new->buf >> 2;
        desc->owner = OWNER_DMA;

        return result;
    }

    return 0;
}

void nic_send(Netbuf* buf) {

}

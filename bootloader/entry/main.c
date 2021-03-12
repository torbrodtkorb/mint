#include <mint/types.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>
#include <drivers/print.h>
#include <drivers/nic.h>

// PA10, PB1, PA31
void main(void)
{
    clk_init();
    print_init();

    print("This is a little test\n");
    
    nic_init();

    while (1) {
        Netbuf* send = alloc_netbuf();
        
        char* data = "Betegnelsen «truse» kommer trolig fra fransk trousses «pasjebukser», knelange beinklær båret av mannlige pasjer, flertall av trousse «pakke, bylt, noe som kan bindes sammen», et ordlån fra latin.";

        u32 i = 0;
        while (*data != 0) {
            send->buf[i++] = *data++;
        }
        send->len = i;
        nic_send(send);
    }

    while (1) {
        Netbuf* netbuf = nic_recive();
        if (netbuf) {
            print("Got packet => ");
            for (u32 i = 0; i < netbuf->len; i++) {
                print("{c} ", netbuf->buf[i]);
            }
            print("\n\n");

            free_netbuf(netbuf);
        }
    }

    // Enable the clock of the PIO module
    clk_peripheral_enable(18);
    
    // LED configuration
    struct gpio_conf conf = {
        .dir = GPIO_OUTPUT,
        .drive = GPIO_HIGH,
        .pull = GPIO_PULL_NONE
    };

    // Turn on LED
    gpio_set_conf(GPIOA_REG, 31, &conf);

    while (1) {

    }
}

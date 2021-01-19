// Driver for UART

#include <drivers/uart.h>
#include <drivers/clk.h>
#include <drivers/gpio.h>


struct uart_pid_map {
    void* uart;
    u32 pid;
};

const struct uart_pid_map uart_map[UART_CNT] = {
    { .uart = UART0, .pid = 45 },
    { .uart = UART1, .pid = 56 },
    { .uart = UART3, .pid = 56 },
    { .uart = UART4, .pid = 56 },
};


static inline u32 uart_to_pid(struct uart_reg* reg) {
    for (u32 i = 0; i < UART_CNT; i++) {
        if (uart_map[i].uart == reg) {
            return uart_map[i].pid;
        }
    }
    return 0;
}

// Uart pin rx on PD2 and TX on PD3, both on peripheral A
void uart_init(struct uart_reg* reg, u32 baud) {
    u32 pid = uart_to_pid(UART1);
    
    clk_peripheral_enable(pid);
    u32 per_freq = clk_get_peripheral_freq(pid);

    gpio_set_function(GPIOD, 2, GPIO_FUNC_A);
    gpio_set_function(GPIOD, 3, GPIO_FUNC_A);

    // Set mode register
    reg->mr = 0;

    // Set Baud rate
    reg->brgr = 64000000 / 16 / baud;

    // Enable transmitter
    reg->cr = (1 << 6);
}

void uart_print(struct uart_reg* reg, const char* data) { 
    while (*data) {
        while ((reg->sr & (1 << 1)) == 0);
        reg->thr = *data++;
    }
}

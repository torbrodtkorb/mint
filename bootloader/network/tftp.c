// tftp

#include <mint/tftp.h>
#include <drivers/print.h>
#include <mint/netbuf.h>
#include <drivers/nic.h>

#define PACKED __attribute__((packed))

#define ETHER_TYPE_IPv4 0x0800
#define ETHER_TYPE_ARP  0x0806


static const u8* our_mac_address = { 0xC0, 0xDE, 0xBA, 0xBE, 0xCA, 0xFE };

struct PACKED MacHeadder {
    u8 dest_mac_address[6];
    u8 source_mac_address[6];
    u16 ether_type;
};

void mac_send(Netbuf* netbuf, u32 destination_ip, u16 ether_type) {
    const u8* dest_mac_address = { 0x00, 0xe0, 0x4c, 0x3d, 0x10, 0x4c };

    netbuf->lenght  += 14;
    netbuf->pointer -= 14;
    
    MacHeadder* header = (MacHeadder *)netbuf->pointer;

    &headder->eter_type;

    nic_send(netbuf);
}

void test() {
    print("hei");
}

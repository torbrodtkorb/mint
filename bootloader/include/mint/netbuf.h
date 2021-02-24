// Network buffer implementation

#ifndef NETBUF_H
#define NETBUF_H

#include <mint/types.h>
#include <mint/list.h>

typedef struct {
    // This will hold the entire network packet data
    u8 buf[1500];
    
    u8* ptr;
    u32 len;

    List node;
} Netbuf;

Netbuf* alloc_netbuf();

void free_netbuf(Netbuf* buf);

void netbuf_init();

#endif

// Network buffer implementation

#ifndef NETBUF_H
#define NETBUF_H

#include <mint/types.h>
#include <mint/list.h>

typedef struct {
    u32 lenght;

    // This will hold the entire network packet data - must be aligned with 4 bytes
    u8 buf[1536];
    
    u8* pointer;

    List node;
} Netbuf;

Netbuf* alloc_netbuf();

void free_netbuf(Netbuf* buf);

void netbuf_init();

#endif

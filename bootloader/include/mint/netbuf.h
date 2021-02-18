// Network buffer implementation

#ifndef NETBUF_H
#define NETBUF_H

#include<mint/types.h>
#include<mint/list.h>

struct netbuf {
    u8 buf[1500];
    
    u8* ptr;

    struct list node;    
};

#endif

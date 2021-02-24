// Network buffer implementation

#include <mint/netbuf.h>

#define NETBUF_COUNT 32

// Allocate the netbuffers
static Netbuf network_buffers[NETBUF_COUNT];

// List for grouping all the netbuffers together
static List netbuf_list;

void netbuf_init() {
    list_init(&netbuf_list);
    for (u32 i = 0; i < NETBUF_COUNT; i++) {
        list_push_front(&network_buffers[i].node, &netbuf_list);
    }
}

Netbuf* alloc_netbuf() {
    List* element = list_pop_back(&netbuf_list);
    Netbuf* buf = list_get_struct(element, Netbuf, node);

    buf->ptr = buf->buf + 134;

    return buf;
}

void free_netbuf(Netbuf* buf) {
    list_push_front(&buf->node, &netbuf_list);
}

// Network buffer implementation

#include <mint/netbuf.h>

#define NETBUF_COUNT 32

// Allocate the netbuffers
static struct netbuf network_buffers[NETBUF_COUNT];

// List for grouping all the netbuffers together
static struct list netbuf_list;

void netbuf_init() {
    list_init(&netbuf_list);
    for (u32 i = 0; i < NETBUF_COUNT; i++) {
        list_push_front(&network_buffers[i].node, &netbuf_list);
    }
}

struct netbuf* alloc_netbuf() {
    struct list* element = list_pop_back(&netbuf_list);
    struct netbuf* buf = list_get_struct(element, struct netbuf, node);

    buf->ptr = buf->buf + 134;

    return buf;
}

void free_netbuf(struct netbuf* buf) {
    list_push_front(&buf->node, &netbuf_list);
}

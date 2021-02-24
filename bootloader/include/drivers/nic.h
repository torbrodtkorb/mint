// NIC driver

#ifndef NIC_H
#define NIC_H

#include <mint/types.h>
#include <mint/netbuf.h>

void nic_init();

Netbuf* nic_recive();

void nic_send(Netbuf* buf);

#endif

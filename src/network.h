/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef NETWORK_H
#define NETWORK_H

typedef struct{
    char* interface;
    unsigned long rx_kbytes;
    unsigned long rx_packets;
    unsigned long rx_errs;
    unsigned long rx_drop;
    unsigned long tx_kbytes;
    unsigned long tx_packets;
    unsigned long tx_errs;
    unsigned long tx_drop;
    unsigned long errs;
    unsigned long drop;
} nd_struct;

typedef struct{
    size_t size;
    nd_struct** devices;
} net_struct;

net_struct* scan_network(void);
void free_network(net_struct* net);

#endif /* NETWORK_H */

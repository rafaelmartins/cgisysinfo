/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "network.h"

void scan_network(net_struct* net){
    char buffer[256], aux[100], *key;
    int line = 0;
    FILE* fp = fopen("/proc/net/dev", "r");
    if(fp == NULL){
        net = NULL;
        return;
    }
    net->size = 0;
    net->devices = NULL;
    while(fgets(buffer, 256, fp) != NULL){
        if(++line < 3) continue;
        key = strtok(buffer, ":");
        net->devices = (nd_struct**) realloc(net->devices, (net->size+1) * sizeof(nd_struct*));
        if(net->devices == NULL){
            net = NULL;
            return;
        }
        net->devices[net->size] = (nd_struct*) malloc(sizeof(nd_struct));
        if(net->devices[net->size] == NULL){
            net = NULL;
            return;
        }
        for(int i = 0; (i < 13) && (key != NULL); i++){
            switch(i){
                case 0: // interface
                    sscanf(key, "%s", aux); // removing spaces
                    net->devices[net->size]->interface = my_strdup(aux);
                    break;
                case 1:
                    net->devices[net->size]->rx_kbytes = strtoul(key, NULL, 10)/1024;
                    break;
                case 2:
                    net->devices[net->size]->rx_packets = strtoul(key, NULL, 10);
                    break;
                case 3:
                    net->devices[net->size]->rx_errs = strtoul(key, NULL, 10);
                    break;
                case 4:
                    net->devices[net->size]->rx_drop = strtoul(key, NULL, 10);
                    break;
                case 9:
                    net->devices[net->size]->tx_kbytes = strtoul(key, NULL, 10)/1024;
                    break;
                case 10:
                    net->devices[net->size]->tx_packets = strtoul(key, NULL, 10);
                    break;
                case 11:
                    net->devices[net->size]->tx_errs = strtoul(key, NULL, 10);
                    break;
                case 12:
                    net->devices[net->size]->tx_drop = strtoul(key, NULL, 10);
                    break;
            }
            key = strtok(NULL, " ");
        }
        net->devices[net->size]->errs = net->devices[net->size]->rx_errs +
            net->devices[net->size]->tx_errs;
        net->devices[net->size]->drop = net->devices[net->size]->rx_drop +
            net->devices[net->size]->tx_drop;
        net->size++;
    }
    fclose(fp);
}

void print_network(net_struct net){
    char buffer[256];
    printf(
        "<table border=\"1\">\n"
        "  <tr><th colspan=\"4\">Network Usage</th></tr>\n"
        "  <tr><th>Device</th><th>Received</th><th>Sent</th><th>Err/Drop</th></tr>\n");
    for(int i = 0; i < net.size; i++){
        printf("  <tr><td>%s</td>", net.devices[i]->interface);
        format_memory(net.devices[i]->rx_kbytes, buffer);
        printf("<td>%s</td>", buffer);
        format_memory(net.devices[i]->tx_kbytes, buffer);
        printf("<td>%s</td>", buffer);
        printf("<td>%lu/%lu</td></tr>\n", net.devices[i]->errs, net.devices[i]->drop);
    }
    printf("</table>\n");
}

void free_network(net_struct* net){
    if(net == NULL)
        return;
    for(int i = 0; i < net->size; i++){
        free(net->devices[i]->interface);
        free(net->devices[i]);
    }
}

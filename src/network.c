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
                    net->devices[net->size]->rx_bytes = strtoul(key, NULL, 10);
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
                    net->devices[net->size]->tx_bytes = strtoul(key, NULL, 10);
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

void free_network(net_struct* net){
    if(net == NULL)
        return;
    for(int i = 0; i < net->size; i++){
        free(net->devices[i]->interface);
        free(net->devices[i]);
    }
}

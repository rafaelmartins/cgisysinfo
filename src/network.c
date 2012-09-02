/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2012 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "utils.h"
#include "network.h"

int active_interface(char* name){
    struct ifreq ifr;
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s < 0){
        return 0;
    }
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    if(ioctl(s, SIOCGIFFLAGS, (caddr_t)&ifr) < 0){
        return 0;
    }
    return (int) ifr.ifr_flags & (IFF_UP | IFF_RUNNING);
}

int compare_nd_struct(const void* nd1, const void* nd2){
    return strcmp((*(nd_struct**)nd1)->interface, (*(nd_struct**)nd2)->interface);
}

net_struct* scan_network(void){
    char buffer[BUFFER_SIZE], aux[BUFFER_SIZE], *key;
    int line = 0;
    FILE* fp = fopen("/proc/net/dev", "r");
    if(fp == NULL){
        return NULL;
    }
    net_struct* net = (net_struct*) malloc(sizeof(net_struct));
    if(net == NULL){
        return NULL;
    }
    net->size = 0;
    net->devices = NULL;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL){
        if(++line < 3) continue;
        key = strtok(buffer, ":");
        sscanf(key, "%s", aux); // removing spaces
        if(!active_interface(aux)){
            continue;
        }
        net->devices = (nd_struct**) realloc(net->devices, (net->size+1) * sizeof(nd_struct*));
        if(net->devices == NULL){
            return NULL;
        }
        net->devices[net->size] = (nd_struct*) malloc(sizeof(nd_struct));
        if(net->devices[net->size] == NULL){
            return NULL;
        }
        net->devices[net->size]->interface = my_strdup(aux);
        for(int i = 0; (i < 13) && (key != NULL); i++){
            switch(i){
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
    qsort(net->devices, net->size, sizeof(nd_struct*), compare_nd_struct);
    return net;
}

void free_network(net_struct* net){
    if(net == NULL){
        return;
    }
    for(int i = 0; i < net->size; i++){
        free(net->devices[i]->interface);
        free(net->devices[i]);
    }
    free(net->devices);
    free(net);
}

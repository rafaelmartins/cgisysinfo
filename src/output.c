/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <fcgi_stdio.h>
#include "filesystem.h"
#include "memory.h"
#include "network.h"
#include "system.h"
#include "utils.h"
#include "output.h"

void format_memory(unsigned long mem, char* buf){
    double aux;
    char format[3];
    if(mem > GB){
        aux = (double) mem / GB;
        strcpy(format, "GB");
    }
    else if(mem > MB){
        aux = (double) mem / MB;
        strcpy(format, "MB");
    }
    else{
        aux = (double) mem;
        strcpy(format, "KB");
    }
    snprintf(buf, BUFFER_SIZE, "%.2f %s", aux, format);
}

void print_filesystem(fs_struct* fs){
    char buffer[BUFFER_SIZE];
    if(fs == NULL){
        return;
    }
    printf(
        "<table>\r\n"
        "  <tr><th colspan=\"6\">Mounted Filesystems</th></tr>\r\n"
        "  <tr><th>Partition</th><th>Mount</th><th>Type</th><th>Free</th><th>Used</th><th>Total</th></tr>\r\n");
    for(int i = 0; i < fs->size; i++){
        printf(
            "  <tr><td>%s</td><td>%s</td><td>%s</td>",
            fs->mounts[i]->partition,
            fs->mounts[i]->mount,
            fs->mounts[i]->type);
        format_memory(fs->mounts[i]->free, buffer);
        printf("<td>%s</td>", buffer);
        format_memory(fs->mounts[i]->used, buffer);
        printf("<td>%s (%.1f%%)</td>", buffer, fs->mounts[i]->percent);
        format_memory(fs->mounts[i]->total, buffer);
        printf("<td>%s</td></tr>\r\n", buffer);
    }
    printf("</table>\r\n");
}

void print_memory(mem_struct* mem){
    char buffer[BUFFER_SIZE];
    if(mem == NULL){
        return;
    }
    printf(
        "<table>\r\n"
        "  <tr><th colspan=\"6\">Memory Usage</th></tr>\r\n"
        "  <tr><th>&nbsp;</th><th>Free</th><th>Used</th><th>Total</th><th>Buffers</th><th>Cached</th></tr>\r\n");
    format_memory(mem->physical_free, buffer);
    printf("  <tr><td>Physical:</td><td>%s</td>", buffer);
    format_memory(mem->physical_used, buffer);
    printf("<td>%s (%.1f%%)</td>", buffer, mem->physical_percent);
    format_memory(mem->physical_total, buffer);
    printf("<td>%s</td>", buffer);
    if(mem->have_buffers){
        format_memory(mem->buffers, buffer);
        printf("<td>%s</td>", buffer);
    }
    else{
        printf("<td>&nbsp;</td>", buffer);
    }
    if(mem->have_cached){
        format_memory(mem->cached, buffer);
        printf("<td>%s</td></tr>\r\n", buffer);
    }
    else{
        printf("<td>&nbsp;</td></tr>\r\n", buffer);
    }
    if(mem->have_applications){
        format_memory(mem->applications_free, buffer);
        printf("  <tr><td>Applications + Kernel:</td><td>%s</td>", buffer);
        format_memory(mem->applications_used, buffer);
        printf("<td>%s (%.1f%%)</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\r\n",
            buffer, mem->applications_percent);
    }
    format_memory(mem->swap_free, buffer);
    printf("  <tr><td>Swap:</td><td>%s</td>", buffer);
    format_memory(mem->swap_used, buffer);
    printf("<td>%s (%.1f%%)</td>", buffer, mem->swap_percent);
    format_memory(mem->swap_total, buffer);
    printf("<td>%s</td><td>&nbsp;</td><td>&nbsp;</td></tr>\r\n</table>\r\n", buffer);
}

void print_network(net_struct *net){
    char buffer[BUFFER_SIZE];
    if(net == NULL){
        return;
    }
    printf(
        "<table>\r\n"
        "  <tr><th colspan=\"4\">Network Usage</th></tr>\r\n"
        "  <tr><th>Device</th><th>Received</th><th>Sent</th><th>Err/Drop</th></tr>\r\n");
    for(int i = 0; i < net->size; i++){
        printf("  <tr><td>%s</td>", net->devices[i]->interface);
        format_memory(net->devices[i]->rx_kbytes, buffer);
        printf("<td>%s</td>", buffer);
        format_memory(net->devices[i]->tx_kbytes, buffer);
        printf("<td>%s</td>", buffer);
        printf("<td>%lu/%lu</td></tr>\r\n", net->devices[i]->errs, net->devices[i]->drop);
    }
    printf("</table>\r\n");
}

void print_system(sys_struct* sys){
    if(sys == NULL){
        return;
    }
    printf(
        "<table>\r\n"
        "  <tr><th colspan=\"2\">System Vital</th></tr>\r\n");
    if(sys->ip_address != NULL){
        printf(
            "  <tr><td>Listening IP</td><td>%s</td></tr>\r\n",
            sys->ip_address);
    }
    printf(
        "  <tr><td>Canonical Hostname</td><td>%s</td></tr>\r\n"
        "  <tr><td>Kernel Version</td><td>%s</td></tr>\r\n",
        sys->canonical_hostname, sys->kernel_version);
    if(sys->have_uptime){
        printf(
            "  <tr><td>Uptime</td><td>%d days %d hours %d minutes %d seconds</td></tr>\r\n",
            sys->uptime->days, sys->uptime->hours, sys->uptime->minutes, sys->uptime->seconds);
    }
    if(sys->have_load_avg){
        printf(
            "  <tr><td>Load Average</td><td>%.2f %.2f %.2f</td></tr>\r\n",
            sys->load_avg[0], sys->load_avg[1], sys->load_avg[2]);
    }
    printf("</table>\r\n");
}

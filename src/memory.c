/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
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
#include "utils.h"
#include "memory.h"

void memory_info(mem_struct* mem){
    char buffer[100], *key;
    FILE* fp = fopen("/proc/meminfo", "r");
    if(fp == NULL){
        return;
    }
    mem->have_cached = 0;
    mem->have_buffers = 0;
    mem->have_applications = 0;
    while(fgets(buffer, 100, fp) != NULL){
        key = strtok(buffer, ":");
        if(strcmp(key, "MemTotal") == 0){
            mem->physical_total = strtoul(strtok(NULL, "\r\n"), NULL, 10);
        }
        else if(strcmp(key, "MemFree") == 0){
            mem->physical_free = strtoul(strtok(NULL, "\r\n"), NULL, 10);
        }
        else if(strcmp(key, "Cached") == 0){
            mem->cached = strtoul(strtok(NULL, "\r\n"), NULL, 10);
            mem->have_cached = 1;
        }
        else if(strcmp(key, "Buffers") == 0){
            mem->buffers = strtoul(strtok(NULL, "\r\n"), NULL, 10);
            mem->have_buffers = 1;
        }
        else if(strcmp(key, "SwapTotal") == 0){
            mem->swap_total = strtoul(strtok(NULL, "\r\n"), NULL, 10);
        }
        else if(strcmp(key, "SwapFree") == 0){
            mem->swap_free = strtoul(strtok(NULL, "\r\n"), NULL, 10);
        }
    }
    fclose(fp);
    mem->physical_used = mem->physical_total - mem->physical_free;
    mem->physical_percent = ((double)mem->physical_used / (double)mem->physical_total) * 100;
    mem->swap_used = mem->swap_total - mem->swap_free;
    mem->swap_percent = ((double)mem->swap_used / (double)mem->swap_total) * 100;
    if(mem->have_cached && mem->have_buffers){
        mem->applications_used = mem->physical_used - mem->cached - mem->buffers;
        mem->applications_free = mem->physical_total - mem->applications_used;
        mem->applications_percent = ((double)mem->applications_used / (double)mem->physical_total) * 100;
        mem->have_applications = 1;
    }
}

void print_memory(mem_struct mem){
    char buffer[256];
    printf(
        "<table>\r\n"
        "  <tr><th colspan=\"6\">Memory Usage</th></tr>\r\n"
        "  <tr><th>&nbsp;</th><th>Free</th><th>Used</th><th>Total</th><th>Buffers</th><th>Cached</th></tr>\r\n");
    format_memory(mem.physical_free, buffer);
    printf("  <tr><td>Physical:</td><td>%s</td>", buffer);
    format_memory(mem.physical_used, buffer);
    printf("<td>%s (%.1f%%)</td>", buffer, mem.physical_percent);
    format_memory(mem.physical_total, buffer);
    printf("<td>%s</td>", buffer);
    if(mem.have_buffers){
        format_memory(mem.buffers, buffer);
        printf("<td>%s</td>", buffer);
    }
    else{
        printf("<td>&nbsp;</td>", buffer);
    }
    if(mem.have_cached){
        format_memory(mem.cached, buffer);
        printf("<td>%s</td></tr>\r\n", buffer);
    }
    else{
        printf("<td>&nbsp;</td></tr>\r\n", buffer);
    }
    if(mem.have_applications){
        format_memory(mem.applications_free, buffer);
        printf("  <tr><td>Applications + Kernel:</td><td>%s</td>", buffer);
        format_memory(mem.applications_used, buffer);
        printf("<td>%s (%.1f%%)</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\r\n",
            buffer, mem.applications_percent);
    }
    format_memory(mem.swap_free, buffer);
    printf("  <tr><td>Swap:</td><td>%s</td>", buffer);
    format_memory(mem.swap_used, buffer);
    printf("<td>%s (%.1f%%)</td>", buffer, mem.swap_percent);
    format_memory(mem.swap_total, buffer);
    printf("<td>%s</td><td>&nbsp;</td><td>&nbsp;</td></tr>\r\n</table>\r\n", buffer);
}

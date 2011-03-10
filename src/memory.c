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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "memory.h"

mem_struct* memory_info(void){
    char buffer[BUFFER_SIZE], *key;
    FILE* fp = fopen("/proc/meminfo", "r");
    if(fp == NULL){
        return NULL;
    }
    mem_struct *mem = (mem_struct*) malloc(sizeof(mem_struct));
    if(mem == NULL){
        return NULL;
    }
    mem->have_cached = 0;
    mem->have_buffers = 0;
    mem->have_applications = 0;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL){
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
    return mem;
}

void free_memory(mem_struct* mem){
    if(mem != NULL){
        free(mem);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            mem->physical_total = strtoul(strtok(NULL, "\n"), NULL, 10);
        }
        else if(strcmp(key, "MemFree") == 0){
            mem->physical_free = strtoul(strtok(NULL, "\n"), NULL, 10);
        }
        else if(strcmp(key, "Cached") == 0){
            mem->cached = strtoul(strtok(NULL, "\n"), NULL, 10);
            mem->have_cached = 1;
        }
        else if(strcmp(key, "Buffers") == 0){
            mem->buffers = strtoul(strtok(NULL, "\n"), NULL, 10);
            mem->have_buffers = 1;
        }
        else if(strcmp(key, "SwapTotal") == 0){
            mem->swap_total = strtoul(strtok(NULL, "\n"), NULL, 10);
        }
        else if(strcmp(key, "SwapFree") == 0){
            mem->swap_free = strtoul(strtok(NULL, "\n"), NULL, 10);
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

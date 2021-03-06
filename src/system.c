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
#include <math.h>
#include "utils.h"
#include "system.h"

sys_struct* system_info(void){
    char buffer[BUFFER_SIZE], *aux;
    FILE* fp;
    
    sys_struct *sys = (sys_struct*) malloc(sizeof(sys_struct));
    if(sys == NULL){
        return NULL;
    }
    sys->have_uptime = 0;
    sys->have_load_avg = 0;
    
    // ip address
    sys->ip_address = getenv("SERVER_ADDR");
    
    // virtual hostname
    sys->virtual_hostname = getenv("SERVER_NAME");
    
    // canonical hostname
    fp = fopen("/proc/sys/kernel/hostname", "r");
    if(fp == NULL){
        sys->canonical_hostname = NULL;
    }
    else{
        if(fscanf(fp, "%[^\r\n]", buffer) != 1){
            sys->canonical_hostname = NULL;
        }
        else{
            sys->canonical_hostname = my_strdup(buffer);
        }
        fclose(fp);
    }
    
    // kernel version
    fp = fopen("/proc/version", "r");
    if(fp == NULL){
        sys->kernel_version = NULL;
    }
    else{
        if(fscanf(fp, "%*s %*s %s", buffer) != 1){
            sys->kernel_version = NULL;
        }
        else{
            sys->kernel_version = my_strdup(buffer);
        }
        fclose(fp);
    }
    
    // uptime
    fp = fopen("/proc/uptime", "r");
    if(fp == NULL){
        sys->uptime = NULL;
    }
    else{
        sys->uptime = (uptime_struct*) malloc(sizeof(uptime_struct));
        if((sys->uptime != NULL) && (fscanf(fp, "%lu", &sys->uptime->timestamp) == 1)){
            sys->uptime->days = (unsigned int) floor(sys->uptime->timestamp / DAY);
            sys->uptime->hours = (unsigned int) floor((sys->uptime->timestamp % DAY) / HOUR);
            sys->uptime->minutes = (unsigned int) floor(((sys->uptime->timestamp % DAY) % HOUR) / MINUTE);
            sys->uptime->seconds = (unsigned int) floor(((sys->uptime->timestamp % DAY) % HOUR) % MINUTE);
            sys->have_uptime = 1;
        }
        fclose(fp);
    }
    
    // load average
    fp = fopen("/proc/loadavg", "r");
    if(fp != NULL){
        if(fscanf(fp, "%lf %lf %lf", &sys->load_avg[0], &sys->load_avg[1], &sys->load_avg[2]) == 3){
            sys->have_load_avg = 1;
        }
        fclose(fp);
    }
    
    return sys;
}

char* system_header(sys_struct* sys){
    size_t size_aux = 0;
    if(sys == NULL){
        return "";
    }
    char* aux = (char*) malloc(sizeof(char));
    if(aux == NULL){
        return "";
    }
    strcpy(aux, "");    
    if(sys->virtual_hostname != NULL || sys->ip_address != NULL){
        size_aux += 3;
        aux = (char*) realloc(aux, size_aux + sizeof(char));
        strcat(aux, " - ");
    }
    if(sys->virtual_hostname != NULL){
        size_aux += strlen(sys->virtual_hostname) * sizeof(char);
        aux = (char*) realloc(aux, size_aux + sizeof(char));
        strcat(aux, sys->virtual_hostname);
    }
    if(sys->ip_address != NULL){
        if(sys->virtual_hostname != NULL){
            size_aux += (strlen(sys->ip_address) + 3) * sizeof(char);
            aux = (char*) realloc(aux, size_aux + sizeof(char));
            strcat(aux, " (");
            strcat(aux, sys->ip_address);
            strcat(aux, ")");
        }
        else{
            size_aux += strlen(sys->ip_address) * sizeof(char);
            aux = (char*) realloc(aux, size_aux + sizeof(char));
            strcat(aux, sys->ip_address);
        }
    }
    return aux;
}

void free_system(sys_struct* sys){
    if(sys == NULL)
        return;
    free(sys->canonical_hostname);
    free(sys->kernel_version);
    free(sys->uptime);
    free(sys);
}

void free_system_header(char* header){
    if(header != NULL){
        free(header);
    }
}

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
#include <math.h>
#include <string.h>
#include "utils.h"

char* my_strdup(char* src){
    char* dest = (char*) malloc((strlen(src) + 1) * sizeof(char));
    if(dest != NULL){
        strcpy(dest, src);
    }
    return dest;
}

void format_memory(unsigned long mem, char* buf){
    double aux;
    char format[3];
    if(mem > TB){
        aux = (double) mem / TB;
        strcpy(format, "TB");
    }
    else if(mem > GB){
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

void format_memory_with_percent(unsigned long mem, double percent, char* buf){
    format_memory(mem, buf);
    if(isnormal(percent)){
        char buf2[BUFFER_SIZE];
        snprintf(buf2, BUFFER_SIZE, "%s (%.1f%%)", buf, percent);
        strcpy(buf, buf2);
    }
}

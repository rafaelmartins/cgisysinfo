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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void format_memory(unsigned long mem, char* buf){
    double aux;
    char format[3];
    if(mem > (2 * GB)){
        aux = (double) mem / GB;
        strcpy(format, "GB");
    }
    else if(mem > (2 * MB)){
        aux = (double) mem / MB;
        strcpy(format, "MB");
    }
    else{
        aux = (double) mem;
        strcpy(format, "KB");
    }
    sprintf(buf, "%.2f %s", aux, format);
}

char* my_strdup(char* src){
    char* dest = (char*) malloc((strlen(src) + 1) * sizeof(char));
    if(dest != NULL){
        strcpy(dest, src);
    }
    return dest;
}

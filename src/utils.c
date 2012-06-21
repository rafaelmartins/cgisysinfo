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

#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* my_strdup(char* src){
    char* dest = (char*) malloc((strlen(src) + 1) * sizeof(char));
    if(dest != NULL){
        strcpy(dest, src);
    }
    return dest;
}

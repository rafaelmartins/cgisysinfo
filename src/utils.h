/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2012 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef UTILS_H
#define UTILS_H

#define MB (double) 1024
#define GB (double) (1024 * MB)
#define TB (double) (1024 * GB)

#define BUFFER_SIZE (size_t) 1024

char* my_strdup(char* src);

#endif /* UTILS_H */

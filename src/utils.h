/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef UTILS_H
#define UTILS_H

#define MB (double) 1024
#define GB (double) (1024 * MB)

void format_memory(unsigned long mem, char* buf);
char* my_strdup(char* src);

#endif /* UTILS_H */

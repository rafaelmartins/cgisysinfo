/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2012 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include "filesystem.h"
#include "memory.h"
#include "network.h"
#include "system.h"

void format_memory_with_percent(unsigned long mem, double percent, char* buf);
void format_memory(unsigned long mem, char* buf);
void print_filesystem(fs_struct* fs);
void print_memory(mem_struct* mem);
void print_network(net_struct *net);
void print_system(sys_struct* sys);

#endif /* OUTPUT_H */

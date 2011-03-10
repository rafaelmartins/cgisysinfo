/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010-2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef MEMORY_H
#define MEMORY_H

// Memory usage
typedef struct{
    unsigned long physical_total;
    unsigned long physical_free;
    unsigned long physical_used;
    double physical_percent;
    unsigned long cached;
    unsigned long buffers;
    unsigned long applications_free;
    unsigned long applications_used;
    double applications_percent;
    unsigned long swap_total;
    unsigned long swap_free;
    unsigned long swap_used;
    double swap_percent;
    unsigned int have_cached:1;
    unsigned int have_buffers:1;
    unsigned int have_applications:1;
} mem_struct;

mem_struct* memory_info(void);
void free_memory(mem_struct* mem);

#endif /* MEMORY_H */

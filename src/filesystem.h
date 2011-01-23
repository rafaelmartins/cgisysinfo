/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

// Mount point
typedef struct{
    char* partition;
    char* mount;
    char* type;
    unsigned long total;
    unsigned long free;
    unsigned long available;
    unsigned long used;
    double percent;
} mp_struct;

// File system
typedef struct{
    size_t size;
    mp_struct** mounts;
} fs_struct;

fs_struct* scan_filesystem(void);
void print_filesystem(fs_struct* fs);
void free_filesystem(fs_struct* fs);

#endif /* FILESYSTEM_H */

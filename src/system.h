/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#define MINUTE 60
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

typedef struct{
    unsigned long timestamp;
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int days;
} uptime_struct;

typedef struct{
    char* ip_address;
    char* virtual_hostname;
    char* canonical_hostname;
    char* kernel_version;
    uptime_struct uptime;
    double load_avg[3];
    unsigned int have_uptime:1;
    unsigned int have_load_avg:1;
} sys_struct;

void system_info(sys_struct* sys);
void free_system(sys_struct* sys);
char* system_header(sys_struct sys);
void print_system(sys_struct sys);

#endif /* SYSTEM_H */

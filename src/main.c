/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include "system.h"
#include "memory.h"
#include "filesystem.h"
#include "network.h"

int main(int argc, char* argv[]){
    sys_struct sys;
    mem_struct mem;
    fs_struct fs;
    net_struct net;
    
    printf(
        "Content-Type: text/html; charset=utf-8\n"
        "X-Powered-By: " PACKAGE_STRING "\n"
        "\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "  <title>System Information - " PACKAGE "</title>\n"
        "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
        "  <meta name=\"generator\" content=\"" PACKAGE_STRING "\" />\n"
        "</head>\n"
        "<body>\n"
        "<h1>System Information - " PACKAGE "</h1>\n");
    
    system_info(&sys);
    print_system(sys);
    free_system(&sys);
    
    memory_info(&mem);
    print_memory(mem);
    
    scan_filesystem(&fs);
    print_filesystem(fs);
    free_filesystem(&fs);
    
    scan_network(&net);
    print_network(net);
    free_network(&net);
    
    printf("</body>\n</html>\n");
    
    return EXIT_SUCCESS;
}

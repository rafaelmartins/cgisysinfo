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
#include "system.h"
#include "memory.h"
#include "filesystem.h"
#include "network.h"

int main(int argc, char* argv[]){
    sys_struct sys;
    mem_struct mem;
    fs_struct fs;
    net_struct net;
    
    system_info(&sys);
    char* header = system_header(sys);
    
    printf(
        //"Content-Type: text/html; charset=utf-8\n"
        //"X-Powered-By: " PACKAGE_STRING "\n"
        //"\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "  <title>System Information%s</title>\n"
        "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
        "  <meta name=\"generator\" content=\"" PACKAGE_STRING "\" />\n"
        "    <style type=\"text/css\">\n"
        "      * {margin:0; padding: 0}\n"
        "      body {text-align: center; font-family: Verdana, Arial}\n"
        "      h1 {font-size: 26px; margin-top: 10px; margin-bottom: 30px}\n"
        "      table {margin: 25px auto; margin-top: 0; border: 1px solid #000; border-spacing: 0}\n"
        "      td, th {padding: 3px 15px}\n"
        "      th {border-bottom: 1px solid #000}\n"
        "      p {margin: 5px auto}\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "<h1>System Information%s</h1>\n", header, header);
    
    free(header);
    
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
    
    printf(
        "<hr />\n"
        "<p><em>Powered by: <strong>" PACKAGE_STRING "</strong></em></p>\n"
        "</body>\n"
        "</html>\n");
    
    return EXIT_SUCCESS;
}

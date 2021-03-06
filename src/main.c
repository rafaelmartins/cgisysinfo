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

#ifdef ENABLE_FASTCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif /* ENABLE_FASTCGI */

#include <stdlib.h>
#include "system.h"
#include "memory.h"
#include "filesystem.h"
#include "network.h"
#include "output.h"

#ifndef PACKAGE_STRING
#define PACKAGE_STRING "cgisysinfo"
#endif /* PACKAGE_STRING */

#ifdef HGTIP
#define MY_PACKAGE_STRING PACKAGE_STRING "/" HGTIP
#else
#define MY_PACKAGE_STRING PACKAGE_STRING
#endif /* HGTIP */

int main(int argc, char** argv){
    
    char* header;
    
    sys_struct *sys;
    mem_struct *mem;
    fs_struct *fs;
    net_struct *net;

#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0){
#endif
        
        sys = system_info();
        header = system_header(sys);
        
        if(sys->ip_address != NULL && sys->virtual_hostname != NULL){
            printf(
                "Content-Type: text/html; charset=utf-8\r\n"
                "X-Powered-By: " MY_PACKAGE_STRING "\r\n"
                "\r\n");
        }
        
        printf(
            "<!DOCTYPE html>\r\n"
            "<html>\r\n"
            "<head>\r\n"
            "  <title>System Information%s</title>\r\n"
            "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n"
            "  <meta name=\"generator\" content=\"" MY_PACKAGE_STRING "\" />\r\n"
            "  <style type=\"text/css\">\r\n"
            "    * {margin:0; padding: 0}\r\n"
            "    body {text-align: center; font-family: Verdana, Arial}\r\n"
            "    h1 {font-size: 26px; margin-top: 10px; margin-bottom: 30px}\r\n"
            "    table {margin: 25px auto; margin-top: 0; border: 1px solid #000; border-spacing: 0}\r\n"
            "    td, th {padding: 3px 15px}\r\n"
            "    th {border-bottom: 1px solid #000}\r\n"
            "    p {margin: 5px auto}\r\n"
            "    a {color: #009}\r\n"
            "  </style>\r\n"
            "</head>\r\n"
            "<body>\r\n"
            "<h1>System Information%s</h1>\r\n", header, header);
        
        free_system_header(header);
        
        print_system(sys);
        free_system(sys);
        
        mem = memory_info();
        print_memory(mem);
        free_memory(mem);
        
        fs = scan_filesystem();
        print_filesystem(fs);
        free_filesystem(fs);
        
        net = scan_network();
        print_network(net);
        free_network(net);
        
        printf(
            "<hr />\r\n"
            "<p><em>Powered by: "
#ifdef PACKAGE_URL
            "<a href=\"" PACKAGE_URL "\">"
#endif
            MY_PACKAGE_STRING
#ifdef PACKAGE_URL
            "</a>"
#endif
            "</em></p>\r\n"
            "</body>\r\n"
            "</html>\r\n");

#ifdef ENABLE_FASTCGI
    }
#endif /* ENABLE_FASTCGI */
    
    return EXIT_SUCCESS;
}

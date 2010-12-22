/**
 * cgisysinfo: A small cgi utility to show basic system information on
 *             linux machines.
 * Copyright (C) 2010 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include "utils.h"
#include "filesystem.h"

void strcpy_malloc(char** dest, char* src){
    *dest = (char*) malloc((strlen(src) + 1) * sizeof(char));
    *dest = strcpy(*dest, src);
}

void scan_filesystem(fs_struct* fs){
    char buffer[256], *key;
    struct statvfs fs_buffer;
    FILE* fp = fopen("/proc/mounts", "r");
    if(fp == NULL){
        fs = NULL;
        return;
    }
    fs->size = 0;
    fs->mounts = NULL;
    while(fgets(buffer, 256, fp) != NULL){
        key = strtok(buffer, " ");
        if(key[0] == '/'){ // just the "real" filesystems
            fs->mounts = (mp_struct**) realloc(fs->mounts, (fs->size+1) * sizeof(mp_struct*));
            if(fs->mounts == NULL){
                fs = NULL;
                return;
            }
            fs->mounts[fs->size] = (mp_struct*) malloc(sizeof(mp_struct));
            if(fs->mounts[fs->size] == NULL){
                fs = NULL;
                return;
            }
            for(int i = 0; (i < 3) && (key != NULL); i++){
                switch(i){
                    case 0: // partition
                        fs->mounts[fs->size]->partition = my_strdup(key);
                        break;
                    case 1: // mount
                        fs->mounts[fs->size]->mount = my_strdup(key);
                        break;
                    case 2: // type
                        fs->mounts[fs->size]->type = my_strdup(key);
                        break;
                }
                key = strtok(NULL, " ");
            }
            if(statvfs(fs->mounts[fs->size]->mount, &fs_buffer) != 0){
                fs = NULL;
                return;
            }
            fs->mounts[fs->size]->total = (fs_buffer.f_frsize * fs_buffer.f_blocks) / 1024;
            fs->mounts[fs->size]->free = (fs_buffer.f_frsize * fs_buffer.f_bfree) / 1024;
            fs->mounts[fs->size]->available = (fs_buffer.f_frsize * fs_buffer.f_bavail) / 1024;
            fs->mounts[fs->size]->used = fs->mounts[fs->size]->total - fs->mounts[fs->size]->free;
            fs->mounts[fs->size]->percent = 
                (double) (fs->mounts[fs->size]->used * 100) /
                (double) (fs->mounts[fs->size]->used + fs->mounts[fs->size]->available);
            fs->size++;
        }
    }
    fclose(fp);
}

void free_filesystem(fs_struct* fs){
    if(fs == NULL)
        return;
    for(int i = 0; i < fs->size; i++){
        free(fs->mounts[i]->partition);
        free(fs->mounts[i]->mount);
        free(fs->mounts[i]->type);
        free(fs->mounts[i]);
    }
}

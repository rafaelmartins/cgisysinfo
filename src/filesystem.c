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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include "utils.h"
#include "filesystem.h"

fs_struct* scan_filesystem(void){
    char buffer[BUFFER_SIZE], *key;
    struct statvfs fs_buffer;
    FILE* fp = fopen("/proc/mounts", "r");
    if(fp == NULL){
        return NULL;
    }
    fs_struct *fs = (fs_struct*) malloc(sizeof(fs_struct));
    fs->size = 0;
    fs->mounts = NULL;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL){
        key = strtok(buffer, " ");
        if(key[0] == '/'){ // just the "real" filesystems
            fs->mounts = (mp_struct**) realloc(fs->mounts, (fs->size+1) * sizeof(mp_struct*));
            if(fs->mounts == NULL){
                return NULL;
            }
            fs->mounts[fs->size] = (mp_struct*) malloc(sizeof(mp_struct));
            if(fs->mounts[fs->size] == NULL){
                return NULL;
            }
            for(int i = 0; (i < 3) && (key != NULL); i++){
                switch(i){
                    case 0: // partition
                        fs->mounts[fs->size]->partition = realpath(key, NULL);
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
                return NULL;
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
    return fs;
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
    free(fs->mounts);
    free(fs);
}

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

// Name: Alan Luo
// netID: afl59
// RUID: 176008323
// your code for readdir goes here

struct dirent *readdir(DIR *dirp) {
    void *handle;
    struct dirent* (*readdir_func)(DIR *);
    char* error;

    handle = dlopen("libc.so.6", RTLD_LAZY); // Real readdir's library

    // Debugging stuff
    if(!handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }

    readdir_func = dlsym(handle, "readdir");
    if((error = dlerror()) != NULL) {
        fputs(error, stderr);
        exit(1);
    }

    char* pattern = getenv("HIDDEN"); // Pattern we want to hide is set as env variable
    struct dirent *dire = NULL;
    do {
        dire = readdir_func(dirp);
        if(pattern == NULL) break; // No hidden set
        if(dire != NULL) {
            if(strcmp(dire->d_name, pattern) != 0) { // Get first pattern that is not what we're hiding
                break;
            }
        }
    } while (dire != NULL);

    dlclose(handle);
    return dire;
}

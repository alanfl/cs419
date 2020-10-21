#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

// Name: Alan Luo
// netID: afl59
// RUID: 176008323
// your code for time() goes here

int first = 1; // Track first-time execution
time_t time(time_t *tloc) {
    // Fake time ONLY ON FIRST CALL
    if(first) {
        first = 0;
        struct tm t;
        time_t t_of_day;

        t.tm_year = 2020-1900;
        t.tm_mon = 8;
        t.tm_mday = 11;
        t.tm_hour = 14;
        t.tm_min = 45;
        t.tm_sec = 7;
        t.tm_isdst = -1;

        *tloc = mktime(&t);
        return *tloc;
    } else {
        // Second call, pass to normal time function
        void *handle;
        time_t(*time_func)(time_t *);
        char *error;

        handle = dlopen("librt.so.1", RTLD_LAZY); //time()'s original source
        if(!handle) {
            fputs(dlerror(), stderr);
            exit(1);
        }

        time_func = dlsym(handle, "time");
        if((error = dlerror()) != NULL) {
            fputs(error, stderr);
            exit(1);
        }

        dlclose(handle);
        return(*time_func)(tloc);
    }
}
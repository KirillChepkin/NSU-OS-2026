#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ulimit.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH 8192


char* optstring = "ispuU:cC:dvV:";


int main(int argc, char* argv[], char* envp[]) {
    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        /* printf("%c\n", opt); */
        switch (opt) {
            case 'i': {
                pid_t ruid, euid, rgid, egid;
                ruid = getuid();
                euid = geteuid();
                rgid = getgid();
                egid = getegid();
                if (ruid == -1 || euid == -1) {
                    fprintf(stderr, "Could not retrieve user id.\n");
                    break;
                }
                if (rgid == -1 || egid == -1) {
                    fprintf(stderr, "Could not retrieve group id.\n");
                    break;
                }
                printf("RUID: %10d, EUID: %10d\n", ruid, euid);
                printf("RGID: %10d, EGID: %10d\n", rgid, rgid);
                break;
            }
            case 's': {
                int ret;
                /* pid_t pid = getpid();
                if (pid == -1) {
                    fprintf(stderr, "Could not retrieve process id.\n");
                    break;
                }
                pid_t gid = getpgrp();
                if (gid == -1) {
                    fprintf(stderr, "Could not retrieve group id.\n");
                    break;
                }
                printf("%d %d\n", getpid(), getpgrp()); */
                ret = setpgid(0, 0);
                /* printf("%d %d\n", getpid(), getpgrp()); */
                if (ret == -1) {
                    fprintf(stderr, "Could not make a process its group leader.\n");
                    /* printf("%s\n", strerror(errno)); */
                    break;
                }
                break;
            }
            case 'p': {
                pid_t pid = getpid();
                pid_t ppid = getppid();
                pid_t pgid = getpgrp();
                if (pid == -1) {
                    fprintf(stderr, "Could not retrieve process id.\n");
                    break;
                }
                if (ppid == -1) {
                    fprintf(stderr, "Could not retrieve parent id.\n");
                    break;
                }
                if (pgid == -1) {
                    fprintf(stderr, "Could not retrieve group id.\n");
                    break;
                }
                printf("PID: %10d, PPID: %10d, PGID: %10d\n", pid, ppid, pgid);
                break;
            }
            case 'u': {
                int ret;
                struct rlimit limits = {};
                ret = getrlimit(UL_GETFSIZE, &limits);
                if (ret != 0 ) {
                    fprintf(stderr, "failed to get UL_GETFSIZE\n");
                }
                else {
                    printf("UL_GETFSIZE Soft: %30llu Hard: %30llu\n", limits.rlim_cur, limits.rlim_max);
                }
                break;
            }
            case 'U': {
                long long limit;
                int ret;
                struct rlimit limits = {};
                char* endptr = NULL;
                if (optarg == NULL) {
                    fprintf(stderr, "Incorrect option. Expected: [-Uulimit]\n");
                    break;
                }

                limit = strtoll(optarg, &endptr, 10);
                /* printf("**endptr value: %d\n", *endptr); */
                if (*endptr != '\0') {
                    fprintf(stderr, "Incorrect parameter. Expected decimal number\n");
                    break;
                }

                ret = getrlimit(UL_GETFSIZE, &limits);
                if (ret == -1) {
                    fprintf(stderr, "Failed to retrieve current ulimit.");
                    break;
                }
                /* printf("%lld\n", limit); */
                limits.rlim_cur = limit;
                ret = setrlimit(RLIMIT_FSIZE, &limits);
                /* printf("%s %llu %llu\n", strerror(errno), limits.rlim_cur, limits.rlim_max); */
                if (ret == -1) {
                    fprintf(stderr, "Failed to set ulimit.\n");
                    break;
                }
                break;
            }
            case 'c': {
                int ret;
                struct rlimit limits = {};
                ret = getrlimit(RLIMIT_CORE, &limits);
                if (ret != 0) {
                    fprintf(stderr, "Failed to retrieve core file size limits\n");
                    break;
                }
                else {
                    printf("RLIMIT_CORE Soft: %30llu Hard: %30llu\n", limits.rlim_cur, limits.rlim_max);
                }
                break;
            }
            case 'C': {
                struct rlimit limits = {};
                long long size;
                int ret;
                char* endptr;
                if (optarg == NULL) {
                    fprintf(stderr, "Incorrect option. Expected: [-Csize]\n");
                    break;
                }
                size = strtoll(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "Incorrect parameter. Expected decimal number\n");
                    break;
                }
                ret = getrlimit(RLIMIT_CORE, &limits);
                if (ret != 0) {
                    fprintf(stderr, "Failed to retrieve core file size limits\n");
                }
                /* printf("Current RLIMIT_CORE: Soft %30llu Hard: %30llu\n", limits.rlim_cur, limits.rlim_max); */
                limits.rlim_cur = size;
                ret = setrlimit(RLIMIT_CORE, &limits);
                if (ret == -1) {
                    fprintf(stderr, "Failed to set new core file size limit.\n");
                    break;
                }
                /* ret = getrlimit(RLIMIT_CORE, &limits);
                if (ret == -1) {
                    fprintf(stderr, "Failed to retrieve new core file size limits\n");
                }
                printf("New RLIMIT_CORE: Soft %30llu Hard: %30llu\n", limits.rlim_cur, limits.rlim_max); */
                break;
            }
            case 'd': {
                char* ret_getcwd;
                char path[MAX_PATH];
                ret_getcwd = getcwd(path, MAX_PATH);
                if (ret_getcwd == NULL) {
                    fprintf(stderr, "The directory path turned out to be longer than allowed.");
                    break;
                }
                printf("DIRECTORY: %s\n", path);
                break;
            }
            case 'v': {
                int i;
                i = 0;
                while (envp[i] != NULL) {
                    printf("%s\n", envp[i]);
                    i++;
                }
                break;
            }
            case 'V': {
                char* copy;
                char* value;
                int len;
                int ret;
                if (optarg == NULL) {
                    fprintf(stderr, "Incorrect option. Expected: [-Vname=value]\n");
                    break;
                }
                len = strlen(optarg) + 1;
                copy = malloc(len);
                if (copy == NULL) {
                    fprintf(stderr, "Failed to allocate memory.\n");
                    break;
                }
                strcpy(copy, optarg);
                value = strchr(copy, '=');
                if (value == NULL) {
                    fprintf(stderr, "Incorrect option. Expected: [-Vname=value]\n");
                    free(copy);
                    break;
                }
                *value = 0;
                value++;
                ret = setenv(copy, value, 1);
                if (ret == -1) {
                    fprintf(stderr, "Could not set given environment variable.\n");
                    free(copy);
                    break;
                }
                /* printf("%s\n", getenv(copy));
                printf("VARIABLE: %s VALUE: %s\n", copy, value); */
                free(copy);
                break;
            }
        }
    }
}
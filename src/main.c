#include "csapp.h"
#include "runner.h"

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>

void sigpipe_handler(int sig) {}

int main(int argc, char **argv) {
    char *f = NULL;

    //Get file to interpret
    opterr = 0;
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            f = optarg;
            break;
        case '?': // Unrecognized/Unvalued argument
            printf("Argument '%c' is an unrecognized argument.\n", optopt);
            return 1;
        default:
            printf("Unreachable case statement.\n");
            return 1;
        }
    }

    //Handle SIGPIPE signals
    Signal(SIGPIPE, sigpipe_handler);

    if (f == NULL) {
        printf("Error: Please use ./httpness -f <filename> to run your program\n");
        return 1;
    }

    FILE *fd = fopen(f, "r");
    char url[4096]; //Buffer
    if (fd == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    runtime_env *R = init_run_env();

    while (fgets(url, sizeof(url), fd) != NULL) {
        append_url(R, url);
    }

    //Run Program :)
    if (run_env(R) == 1) {
        return 1;
    }

    return 0;
}

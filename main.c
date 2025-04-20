#include "csapp.h"

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
    char *f;

    //Get file to interpret
    opterr = 0;
    int opt;
    while ((opt = getopt(argc, argv, "f")) != -1) {
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

    FILE *fd = fopen(f, "r");
    char url[4096]; //Buffer
    if (fd == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(url, sizeof(url), fd) != NULL) {
        handleHTTP(url);
    }

    return 0;
}

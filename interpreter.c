#include "csapp.h"
#include "url-parser.h"

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

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>


static const char *header_user_agent = "Mozilla/5.0"
                                       " (X11; Linux x86_64; rv:3.10.0)"
                                       " Gecko/20250408 Firefox/63.0.1";

void sigpipe_handler(int sig) {}

#include <stdio.h>

void *handleHTTP(char *url) {
    URL_PARTS *parsedURL = calloc(1, sizeof(parsedURL));
    if (parsedURL == NULL){
        printf("Malloc error: retry again!!");
        return (void*)1;
    }
    if (!(ParseURL(url, parsedURL))) {
        printf("Error parsing URL!!!!");
        return (void*)1;
    }

    char serverRequest[MAXLINE];
    int buflen = snprintf(serverRequest, MAXLINE,
                    "GET %s HTTP/1.1\r\n" \
                    "Host: %s\r\n" \
                    "User-Agent: %s\r\n" \
                    "Connection: close\r\n",
                    parsedURL->path, parsedURL->authority, header_user_agent);
    if (buflen >= MAXLINE) {
        printf("OVERFLOW ERROR!!!!");
    }
    // printf("%d\n", buflen);

    printf("Sending the following: \n%s", serverRequest);
    printf("HOST: %s, PORT: %s\n", parsedURL->authority, parsedURL->port);

    int client_fd = open_clientfd(parsedURL->authority, parsedURL->port);
    if (client_fd < 0) {
        clienterror(conctfd, "500", "Host Error",
                    "Unable to connect to host server");
        close(conctfd);
        return (void*)(1);
    }
    printf("Connected to Host\n");
    printf("Forwarding request...\n");

    if (rio_writen(client_fd, buf, strlen(buf)) < 0) {
        printf("Error writing to host\n");
        clienterror(conctfd, "500", "Host Error", "Unable to write to host...");
        close(conctfd);
        return (void*)(1);
    }

    printf("Reading from server...\n");

    rio_readinitb(&rio, client_fd);
    ssize_t n;
    printf("Starting...\n\n");
    while ((n = rio_readnb(&rio, buf, MAXLINE)) != 0) {
        // printf("%ld\n", n);
        rio_writen(conctfd, buf, n);
    }
    close(client_fd);

    printf("Closed!\n");

    parser_free(par);
    close(conctfd);
    return (void*)(0);
}

int main(int argc, char **argv) {
    struct sockaddr proxy_sock;
    socklen_t sock_len;
    char *port;
    char client_hostname[MAXLINE], client_port[MAXLINE];
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
    char url[512]; //Buffer
    if (fd == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(url, sizeof(url), fd) != NULL) {
        handleHTTP(url);
    }

    return 0;
}

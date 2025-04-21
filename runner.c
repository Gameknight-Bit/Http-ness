/**
 * @brief Code that interprets the actual http response from the server
 * and turns it into an actual programming language!!!
 * 
 * @author: Jace Parks <andrewID: jacep>
 * @date: 4/20/2025
 */

#include "Stack.h"
#include "url-parser.h"
#include "csapp.h"
#include "functionalities.h"

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

#define MAXLINE 8192 /* Max text line length */

struct runtime {
    stack_o playground; //actual env
    int current_url; //instruction pointer
    char **url_strings; //All url strings to run
    size_t size_of_prog; //Length of url_strings array
};
typedef struct runtime runtime_env;

struct responseParsed {
    int responseCode; //Response Code
    char **headers; //Headers
    char **headers_info; //Header-content corresponding to header in index
};
typedef struct responseParsed ServerResponse;

/**
 * @brief Intiltialize Program Env
 */
runtime_env* init_run_env() {
    runtime_env *ret = calloc(1, sizeof(runtime_env)); 
    ret->playground = init_stack();
    ret->current_url = 0;
    ret->url_strings = calloc(4096, sizeof(char*));
    ret->size_of_prog = 0;
}

char** strSplit(char* a_str, const char a_delim)
{
    //https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    /* Count how many elements will be extracted. */
    while (*tmp){
        if (a_delim == *tmp){
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


static const char *header_user_agent = "Mozilla/5.0"
                                       " (X11; Linux x86_64; rv:3.10.0)"
                                       " Gecko/20250408 Firefox/63.0.1";
/**
 * @brief Handle's url string and returns a server response (or NULL if it failed...)
 */
char *handleHTTP(char *url) {
    URL_PARTS *parsedURL = calloc(1, sizeof(parsedURL));
    if (parsedURL == NULL){
        printf("Malloc error: retry again!!");
        return NULL;
    }
    if (!(ParseURL(url, parsedURL))) {
        printf("Error parsing URL!!!!");
        return NULL;
    }

    char serverRequest[MAXLINE];
    char buf[MAXLINE];
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
        printf("Unable to connect to server!!!");
        return NULL;
    }
    printf("Connected to Host\n");
    printf("Forwarding request...\n");
    free(parsedURL);

    if (rio_writen(client_fd, serverRequest, strlen(serverRequest)) < 0) {
        printf("Error writing to host\n");
        return NULL;
    }

    printf("Reading from server...\n");

    rio_t rio;
    rio_readinitb(&rio, client_fd);
    ssize_t n;
    printf("Starting...\n\n");
    char *ret[MAXLINE*20];
    while ((n = rio_readnb(&rio, buf, MAXLINE)) != 0) {
        // printf("%ld\n", n);
        strcat(ret, buf);
    }
    close(client_fd);

    printf("Closed!\n");

    return ret;
}

/**
 * @brief Parses a string (server response) into a struct to
 * be read by the runtime thingy!
 */
ServerResponse* parseResponse(char *response) {

    ServerResponse* ret = calloc(1, sizeof(ServerResponse));
    ret->headers = calloc(256, sizeof(char *));
    ret->headers_info = calloc(256, sizeof(char *));

    char **tokens = strSplit(response, "\n");
    char *cur = tokens[0];
    int cur_iter = 0;
    while (cur != NULL) {
        printf("Current token: %s\n", cur);
        if (strncmp(cur, "HTTP", 5)) {
            char **subTok = strSplit(cur, " ");
            ret->responseCode = atoi(subTok[1]); 
            free(subTok);
        } else {
            char **subTok = strSplit(cur, ": ");
            ret->headers[cur_iter] = subTok[0];
            ret->headers_info[cur_iter] = subTok[1];
            free(subTok);
        }
    }

    free(tokens);
    return ret;
}

/**
 * @brief add all url strings to this before running the env;
 */
void append_url(runtime_env* R, char *url) {
    R->url_strings[R->size_of_prog] = url;
    R->size_of_prog += 1;
}

/**
 * @brief run environment!!!!
 * @return 1 if error 0 if successful run!
 */
int run_env(runtime_env *R) {
    while (R->current_url < R->size_of_prog) {
        char *cur_URL = R->url_strings[R->current_url];
        char *ServResp = handleHTTP(cur_URL);
        if (ServResp == NULL) {
            R->current_url++;
            printf("Error!");
            continue;
        }
        ServerResponse *resp = parseResponse(ServResp);
        route(resp, R); //Does functionality of Response Code :)
        free(resp);
        R->current_url++;
    }
}
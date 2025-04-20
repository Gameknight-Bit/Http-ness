/**
 * @brief Code that interprets the actual http response from the server
 * and turns it into an actual programming language!!!
 * 
 * @author: Jace Parks <andrewID: jacep>
 * @date: 4/20/2025
 */

#include "dict.h"

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

struct runtime {
    struct nlist; //Dict of variables
    
};
typedef struct runtime runtime_env;

struct responseParsed {
    int responseCode; //Response Code
    char **headers; //Headers
    char **headers_info; //Header-content corresponding to header in index
};
typedef struct responseParsed ServerResponse;

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
    return 0;
}

/**
 * @brief Intiltialize Program Env
 */
runtime_env* init_env() {
    runtime_env *ret = calloc(1, sizeof(runtime_env)); 
}

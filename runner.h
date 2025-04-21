/**
 * @brief Code that interprets the actual http response from the server
 * and turns it into an actual programming language!!!
 * 
 * @author: Jace Parks <andrewID: jacep>
 * @date: 4/20/2025
 */

#include "Stack.h"
#include <stdlib.h>

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
 * @brief Parses a string (server response) into a struct to
 * be read by the runtime thingy!
 */
ServerResponse* parseResponse(char *response);

void append_url(runtime_env* R, char *url);

runtime_env* init_run_env();


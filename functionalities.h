/*
Response Code | Action (equiv code expression)
--------------------------------------------
(Builtin Functions)
100           | print Top of stack (print)

(Simple Operations)
200           | Push 'VAL' in stack as a char* (push)
201           | Add two top values on stack (add Top+Nxt)
202           | Subtract two top values on stack (sub Top-Nxt)
203           | Div two top values on stack (div Top/Nxt)
204           | Mult two top values on stack (mul Top*Nxt)
205           | Push 'VAL' in stack as a long (push) [uses aoti]
206           | Duplicate top value (dup Top -> Top, Top)
207           | Inc top value on stack (inc ++Top)
208           | Dec top value on stack (dec --Top)

(Looping/If Statements)
300           | If top != 0 jump back 'VAL' urls (jmp)
301           | Not of top (if 0 => 1 | _ => 0) (! op)
302           | Skip 'VAL' urls (goto)

(Misc)
400           | Pop top of stack off (pop)
402           | Payment required to execute this instruction
404           | Reset stack
418           | Do something random! (* abstract *)
425           | Push date string onto top of stack (push)
500           | Terminate Program (exit)
501           | Not implemented.
508           | fork() (fork()) ((fork())) (((fork()))) ... ((((((((...))))))))

Other Codes   | NOP
*/

#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

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
 * @brief searches for header (0 if its not found)
 */
long searchForHeader(ServerResponse *S, char *header) {
    char *cur = S->headers[0];
    size_t cur_ind = 0;
    while (cur != NULL) {
        if (strcmp(cur, header) == 0) {
            return (long)(S->headers_info[cur_ind]);
        }
        cur_ind++;
        cur = S->headers[cur_ind];
    }
    return 0;      
}

void code100(ServerResponse *S, runtime_env *R) {
    printf("%s\n", (char *)peek(R->playground));
}
void code200(ServerResponse *S, runtime_env *R) {
    push(R->playground, searchForHeader(S, "VAL"));
}
void code201(ServerResponse *S, runtime_env *R) {
    if (length(S) < 2) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground);
    long top1 = pop(R->playground);
    push(R->playground, top+top1);
}
void code202(ServerResponse *S, runtime_env *R) {
    if (length(S) < 2) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground);
    long top1 = pop(R->playground);
    push(R->playground, top-top1);
}
void code203(ServerResponse *S, runtime_env *R) {
    if (length(S) < 2) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground);
    long top1 = pop(R->playground);
    push(R->playground, top/top1);
}
void code204(ServerResponse *S, runtime_env *R) {
    if (length(S) < 2) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground);
    long top1 = pop(R->playground);
    push(R->playground, top*top1);
}
void code205(ServerResponse *S, runtime_env *R) {
    char *s = (char *)(searchForHeader(S, "VAL"));
    long d = strtoul(s, NULL, 10); //Convert to base 10 digits
    push(R->playground, d);
}
void code206(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long d = peek(R->playground);
    push(R->playground, d);
}
void code207(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground) + 1;
    push(R->playground, top);
}
void code208(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = pop(R->playground) - 1;
    push(R->playground, top);
}

void code300(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = peek(R->playground);
    if (top != 0) {
        char *s = (char *)(searchForHeader(S, "VAL"));
        long d = strtoul(s, NULL, 10); //Convert to base 10 digits
        R->current_url -= (d + 1);
        //Skip current url executing this cmd as well :)
    }
}
void code301(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    long top = !(pop(R->playground));
    push(R->playground, top);
}
void code302(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    char *s = (char *)(searchForHeader(S, "VAL"));
    long d = strtoul(s, NULL, 10); //Convert to base 10 digits
    R->current_url += d; 
}

void code400(ServerResponse *S, runtime_env *R) {
    if (length(S) < 1) {
        printf("Error: stack not large enough...\n");
        return;
    }
    pop(S);
}


/**
 * @brief Handles http response and routes it to the correct function
 */
void route(ServerResponse *S, runtime_env *R) {
    switch (S->responseCode) {
        case (100):
            code100(S, R);
            break;
        case (200):
            code200(S, R);
            break;
        case (201):
            code201(S,R);
            break;
        case (202):
            code202(S,R);
            break;
        case (203):
            code203(S,R);
            break;
        case (204):
            code204(S,R);
            break;
        case (205):
            code205(S,R);
            break;
        case (206):
            code206(S,R);
            break;
        case (207):
            code207(S,R);
            break;
        case (208):
            code208(S,R);
            break;

        case (300):
            code300(S,R);
            break;
        case (301):
            code301(S,R);
            break;
        case (302):
            code302(S,R);
            break;
        default:
            //NOP
            break;
    }
}
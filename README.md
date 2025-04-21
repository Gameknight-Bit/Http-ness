Wow, this is an awesome programming language by `Jace Parks`
<AndrewId: jacep>

/////////////// The HTTP-NESS Specification ////////////////

This was created for my Esoteric Programming languages final project

Essentially, the language is just a series of http GET requests which depends
on the server response code and response headers. (the response body is ignored
[however, content-length and other headers that depend on the body can still be 
used])


A good portion of this code is copied from stackoverflow, github, ect, so please
check each file to see c:

So here we go with the specs:

Remember, all of these codes and headers depend on the server response so find
reliable links/servers ;)

#Interpreter:
The language should be just a stack of 64-bit values. Using arithmetic operations with
REVERSE-POLISH-NOTATION is pretty much how the language works (with a few extra stuff).

#Response Codes:
Response codes are an important part of the language and act as very basic
operators to change data, edit variables, print, etc. They also can be paired
with Response Headers for various functionality

Header names are notated with '' around them... (if not specified in response,
the response value will be 0)

###Stack Notation:
1. Top
2. Nxt
...
n. Bot

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
302           | Skip 'Val' urls (goto)

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

#CODING TIPS:
Use httpstat.us!!!!
(although this gets rid of the fun...)

To compile the executable just run the command
`make`
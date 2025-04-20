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
*/
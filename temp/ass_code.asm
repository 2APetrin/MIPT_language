# Programm is written in mipt languege
# Which is created by Petrin Anton MIPT 1 course

# VAR_TABLE:
# num - [0]
# num_temp - [1]
# fact_temp - [2]
# x - [3]


jmp :main

:fact
   pop ax
   pop [0]
   push ax

# IF
   push [0]
   push 0
   ja :asm_if_lbl0
   push 1
   pop ax
   ret
   :asm_if_lbl0
# END IF

   push 0
   pop [1]
   push [0]
   push 1
   sub
   pop [1]
   push 0
   pop [2]
   push [1]
   push [1]
   call :fact

   pop [1]
   push ax
   pop [2]
   push [2]
   push [1]
   push 1
   add
   mul
   pop [2]
   push [2]
   pop ax
   ret

:main
   push 0
   pop [3]
   in
   pop [3]
   push [3]
   push [3]
   call :fact

   pop [3]
   push ax
   pop [3]
   push [3]
   out
hlt

# Programm is written in mipt languege
# Which is created by Petrin Anton MIPT 1 course

# VAR_TABLE:
# num - [0]
# fact_temp - [1]
# x - [2]


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
   push [0]
   push 1
   sub
   call :fact

   pop [0]
   push ax
   pop [1]
   push [1]
   push [0]
   mul
   pop [1]
   push [1]
   pop ax
   ret

:main
   push 0
   pop [2]
   in
   pop [2]
   push [2]
   push [2]
   call :fact

   pop [2]
   push ax
   pop [2]
   push [2]
   out
hlt

#pragma once

#include "../asm/asm.h"
#include "stack.h"

typedef struct
{
    my_stack_t stack;

    elem_t* exe_code_arr;
    size_t  exe_code_len;

    elem_t* ram;
    elem_t  reg_ax;
    elem_t  reg_bx;
    elem_t  reg_cx;
    elem_t  reg_dx;
} cpu_t;


const unsigned RAM_CAPACITY = (1 << 6);


int run_cpu(FILE* in_stream);

int cpu_ctor(cpu_t* cpu, FILE* in_stream);

int execute_code(cpu_t* cpu);

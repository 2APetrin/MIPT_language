#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../asm/asm.h"
#include "stack.h"

#define ACCURACY 100

typedef struct
{
    my_stack_t stack;
    size_t exe_code_len;
    int *  exe_code_arr;
    //int *  ram;
    int    reg_ax;
    int    reg_bx;
    int    reg_cx;
    int    reg_dx;
} cpu_t;


FILE * open_inputfile(const char * filename);

FILE * open_outputfile(const char * filename);

int    run_cpu(FILE* in_stream);

int    cpu_ctor(cpu_t* cpu_t);

int    get_num_of_symbols(cpu_t* cpu, FILE* in_stream);

int    get_exe_arr(cpu_t* cpu_t, FILE * in_stream);

int    execute_code(cpu_t cpu);

int    get_register_val(int * ret_val, int reg_num, cpu_t* cpu);

int    register_fill(int reg_num, int value, cpu_t* cpu);
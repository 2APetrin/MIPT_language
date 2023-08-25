#pragma once

#include "../asm/asm.h"
#include "stack.h"

// ================ CPU DSL ================
#define CPU_CODE_ARR cpu->exe_code_arr // buffer with cpu commands
#define CPU_CODE_LEN cpu->exe_code_len // length of buffer with cpu commands

#define CPU_RAM   cpu->ram   // buffer of elem_t elements. Is equal to RAM in real cpu
#define CPU_STACK cpu->stack // CPU stack
// =========================================


//! @brief main structure of cpu porocess
typedef struct
{
    my_stack_t stack;     // CPU stack

    elem_t* exe_code_arr; // buffer with cpu commands
    size_t  exe_code_len; // length of buffer with cpu commands

    elem_t* ram;          // buffer of elem_t elements. Is equal to RAM in real cpu
    elem_t  reg_ax;
    elem_t  reg_bx;
    elem_t  reg_cx;
    elem_t  reg_dx;
} cpu_t;


//! @brief length of ram buffer
const unsigned RAM_CAPACITY = (1 << 12);

//! @brief runs the main process of CPU
//!
//! @param [in] in_stream input stream of cpu commands
//!
//! @return 0 if worked correctly and 1 if it didn't
int run_cpu(FILE* in_stream);

//! @brief constructor of cpu
//!
//! @param [out] cpu pointer to main structure of cpu porocess
//! @param [in] in_stream input stream of cpu commands
//!
//! @return 0 if worked correctly and 1 if it didn't
int cpu_ctor(cpu_t* cpu, FILE* in_stream);

//! @brief main process of CPU that executes commands
//!
//! @param [out] cpu pointer to main structure of cpu porocess
//!
//! @return 0 if worked correctly and 1 if it didn't
int execute_code(cpu_t* cpu);

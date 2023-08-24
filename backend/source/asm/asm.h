#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ASM_DSL.h"


#define ASSERT(cond)                                                 \
if (!(cond))                                                          \
{                                                                      \
    printf("\nError in %s in line %d in function %s in file %s\n\n",    \
            #cond, __LINE__, __PRETTY_FUNCTION__, __FILE__),             \
    abort();                                                              \
}


#define ASM_SYNTAX_ERROR(str, err)                                              \
{                                                                                \
    printf(str);                                                                  \
    printf("\nline:%u\npos:%u\n\n", TOKEN_BUFF[i].line, TOKEN_BUFF[i].inline_pos); \
    assembly->status |= err;                                                        \
}


extern FILE* logfile;

typedef double elem_t;

//! @brief maximum variables count
const unsigned MAX_VAR_COUNT = 128;

const unsigned MAX_WORD_LEN   = 128;
const unsigned MAX_LABELS_CNT = 128;
const unsigned MAX_LINES_CNT  = (1 << 12);

enum cmd_codes
{
    CMD_ERROR = -1,

    CMD_HLT   = 0,

    CMD_PUSH     = 1,
    CMD_PUSH_REG = 2,
    CMD_PUSH_RAM = 3,

    CMD_POP     = 4,
    CMD_POP_REG = 5,
    CMD_POP_RAM = 6,

    CMD_OUT    = 7,
    CMD_IN     = 8,

    CMD_ADD    = 9,
    CMD_SUB    = 10,
    CMD_MUL    = 11,
    CMD_DIV    = 12,

    CMD_JMP    = 13,
    CMD_JMP_B  = 14,
    CMD_JMP_BE = 15,
    CMD_JMP_A  = 16,
    CMD_JMP_AE = 17,
    CMD_JMP_E  = 18,
    CMD_JMP_NE = 19,

    CMD_CALL   = 20,
    CMD_RET    = 21,

    CMD_SQRT   = 22,
    CMD_NROOTS = 23,
    CMD_ALLNUM = 24,

    REG_AX = 25,
    REG_BX = 26,
    REG_CX = 27,
    REG_DX = 28
};


enum token_type
{
    TYPE_CMD       = 1,

    TYPE_NUM       = 2,
    TYPE_REG       = 3,
    RAM_PTR        = 4,

    TYPE_LABEL     = 5,
    TYPE_JMP_LABEL = 6,

    TYPE_COMMENT   = 7
};


enum tokens_errors
{
    ERR_NO_HLT     = (1 << 0),
    BAD_NEXT_NODE  = (1 << 1),
    JMP_WTHT_LABEL = (1 << 2),
    BAD_PUSH_ARG   = (1 << 3),
};


typedef struct
{
    char*  word;
    elem_t val;

    cmd_codes  cmd;
    token_type type;

    unsigned line;
    unsigned inline_pos;
} token_t;


typedef struct
{
    char*    name;
    unsigned pos;
    unsigned init_cnt;
} label_t;


typedef struct
{
    char*    prog_buff;
    unsigned text_len;

    char**   lines_buff;
    unsigned lines_cnt;
} asm_text_t;


typedef struct
{
    asm_text_t prog_text;

    label_t* label_buff;
    unsigned label_cnt;

    token_t* token_buff;
    unsigned token_cnt;

    double*  out_buff;
    unsigned out_len;

    unsigned* commented_lines;
    unsigned  commented_lines_cnt;

    unsigned minus_cnt;
    unsigned status;
    unsigned pos;
} asm_t;


//! @brief Runs the main process of assembly
//! 
//! @param [in] in_stream input stream of asm code
//! @param [in] out_stream output stream into file with binary code
//!
int run_asm(FILE* in_stream, FILE* out_stream);


//! @brief Function that fill in the asm_data struct with needed data. Makes tokens from text
//!
//! @param [in] in_stream input stream of asm code
//! @param [in] asm_data_t pointer to struct which contains data of asm code
//!
int asm_ctor(asm_t* assembly, FILE* stream);

int asm_dtor(asm_t* assembly);

int get_lines(asm_t* assembly);

int get_num_of_tokens(asm_t* assembly);

int get_tokens(asm_t* assembly);

int tokenize_line(asm_t* assembly, unsigned line_num);

int skip_blanks(char* base, unsigned* inline_pos);

int get_word(asm_t* assembly, unsigned line_num, unsigned* inline_pos);

int init_tokens(asm_t* assembly);

int single_token_init(asm_t* assembly);

int is_label(char* word);

int is_num(char* word);

int is_ram_ptr(char* word);

cmd_codes get_cmd(char * word);

int init_labels(asm_t* assembly);

int second_init_labels(asm_t* assembly);

int is_init_label(asm_t* assembly);

int is_jmp_label(asm_t* assembly);

int tokens_check(asm_t* assembly);

int init_push_pop(asm_t* assembly);

int write_exe_array(asm_t* assembly, FILE* out_stream);
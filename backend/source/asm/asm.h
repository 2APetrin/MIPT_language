#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define ASSERT(cond)                                                 \
if (!(cond))                                                          \
{                                                                      \
    printf("\nError in %s in line %d in function %s in file %s\n\n",    \
            #cond, __LINE__, __PRETTY_FUNCTION__, __FILE__),             \
    abort();                                                              \
}


extern FILE* logfile;

typedef double elem_t;

const unsigned MAX_WORD_LEN   = 128;
const unsigned MAX_LABELS_CNT = 128;

enum cmd_codes
{
    ERROR = -1,

    HLT   = 0,

    PUSH     = 1,
    PUSH_REG = 2,
    PUSH_RAM = 3,

    POP     = 4,
    POP_REG = 5,
    POP_RAM = 6,

    OUT    = 7,
    IN     = 8,

    ADD    = 9,
    SUB    = 10,
    MUL    = 11,
    DIV    = 12,

    JMP    = 13,
    JMP_B  = 14,
    JMP_BE = 15,
    JMP_A  = 16,
    JMP_AE = 17,
    JMP_E  = 18,
    JMP_NE = 19,

    CALL   = 20,
    RET    = 21,

    SQRT   = 22,
    NROOTS = 23,
    ALLNUM = 24,

    AX = 25,
    BX = 26,
    CX = 27,
    DX = 28
};


enum token_type
{
    TYPE_CMD       = 1,

    TYPE_NUM       = 2,
    REGISTER       = 3,
    RAM_PTR        = 4,

    TYPE_LABEL     = 5,
    TYPE_JMP_LABEL = 6,
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
    unsigned cnt;
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
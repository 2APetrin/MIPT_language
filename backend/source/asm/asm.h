#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ASM_DSL.h"

//! @brief asm.h custom assert
#define ASSERT(cond)                                                 \
if (!(cond))                                                          \
{                                                                      \
    printf("\nError in %s in line %d in function %s in file %s\n\n",    \
            #cond, __LINE__, __PRETTY_FUNCTION__, __FILE__),             \
    abort();                                                              \
}


//! @brief asm.h macros to report asm errors
#define ASM_SYNTAX_ERROR(str, err)                                              \
{                                                                                \
    printf(str);                                                                  \
    printf("\nline:%u\npos:%u\n\n", TOKEN_BUFF[i].line, TOKEN_BUFF[i].inline_pos); \
    assembly->status |= err;                                                        \
}


//! @brief global log_file
extern FILE* logfile;

//! @brief type of number elements in asm
typedef double elem_t;

//! @brief maximum variables count
const unsigned MAX_VAR_COUNT = 128;

//! @brief maximum word length
const unsigned MAX_WORD_LEN   = 128;

//! @brief maximum labels count
const unsigned MAX_LABELS_CNT = 128;

//! @brief maximum lines of code count
const unsigned MAX_LINES_CNT  = (1 << 12);


//! @brief codes of cpu comands
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


//! @brief type of token
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


//! @brief errors in tokens to translate into message
enum tokens_errors
{
    ERR_NO_HLT     = (1 << 0),
    BAD_NEXT_NODE  = (1 << 1),
    JMP_WTHT_LABEL = (1 << 2),
    BAD_PUSH_ARG   = (1 << 3),
};


//! @brief struct of token of asm code
typedef struct
{
    char*  word;
    elem_t val;

    cmd_codes  cmd;
    token_type type;

    unsigned line;
    unsigned inline_pos;
} token_t;


//! @brief struct of label in code
typedef struct
{
    char*    name;
    unsigned pos;
    unsigned init_cnt;
} label_t;


//! @brief struct of assembly text
typedef struct
{
    char*    prog_buff;
    unsigned text_len;

    char**   lines_buff;
    unsigned lines_cnt;
} asm_text_t;


//! @brief main structure of assembly
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
//! @return 0 if worked correctly and 1 if it didn't
int run_asm(FILE* in_stream, FILE* out_stream);


//! @brief Function that fill in the asm_data struct with needed data. Makes tokens from text
//!
//! @param [in] in_stream input stream of asm code
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int asm_ctor(asm_t* assembly, FILE* stream);

//! @brief destructor of asm struct
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int asm_dtor(asm_t* assembly);

//! @brief function gets lines in LINE_BUFF
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int get_lines(asm_t* assembly);

//! @brief gets number of separated tokens
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int get_num_of_tokens(asm_t* assembly);

//! @brief destructor of asm struct
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int get_tokens(asm_t* assembly);

//! @brief gets tokens from line (with it's word)
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int tokenize_line(asm_t* assembly, unsigned line_num);

//! @brief skips blanks in line
//!
//! @param [in] base base pointer of line
//! @param [in] inline_pos position in line
//!
//! @return 0 if worked correctly and 1 if it didn't
int skip_blanks(char* base, unsigned* inline_pos);

//! @brief gets tokens from line (with it's word)
//!
//! @param [out] assembly pointer to struct which contains data of asm code
//! @param [in] line_num number of tokenizing line
//! @param [in] inline_pos position in line
//!
//! @return 0 if worked correctly and 1 if it didn't
int get_word(asm_t* assembly, unsigned line_num, unsigned* inline_pos);

//! @brief gets tokens from line (with it's word)
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int init_tokens(asm_t* assembly);

//! @brief gets most info about token
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int single_token_init(asm_t* assembly);

//! @brief checks word for label
//!
//! @param [in] word pointer to word of checkes token
//!
//! @return true if is label and false if not
int is_label(char* word);

//! @brief checks word for number
//!
//! @param [in] word pointer to word of checkes token
//!
//! @return true if is number and false if not
int is_num(char* word);

//! @brief checks word for ram pointer ( [num] )
//!
//! @param [in] word pointer to word of checkes token
//!
//! @return true if is ram ptr and false if not
int is_ram_ptr(char* word);

//! @brief gets cmd type from word
//!
//! @param [in] word pointer to word of checkes token
//!
//! @return cmd_code of that word
cmd_codes get_cmd(char* word);

//! @brief first lap of lables initialization
//!
//! @param [out] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int init_labels(asm_t* assembly);

//! @brief second lap of lables initialization
//!
//! @param [out] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int second_init_labels(asm_t* assembly);

//! @brief checks word for initialization of label
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return true if is initialization of label and false if not
int is_init_label(asm_t* assembly);

//! @brief checks word for label after jump
//!
//! @param [in] assembly pointer to struct which contains data of asm code
//!
//! @return true if is label after jump and false if not
int is_jmp_label(asm_t* assembly);

//! @brief checks tokens for some errors
//!
//! @param [out] assembly pointer to struct which contains data of asm code
void tokens_check(asm_t* assembly);

//! @brief initialization of push and pops with differet tokens after (number/register/ram_ptr)
//!
//! @param [out] assembly pointer to struct which contains data of asm code
//!
//! @return 0 if worked correctly and 1 if it didn't
int init_push_pop(asm_t* assembly);

//! @brief writes array of cmd codes into stream
//!
//! @param [out] assembly pointer to struct which contains data of asm code
//! @param [out] out_stream output stream into file with binary code
//!
//! @return 0 if worked correctly and 1 if it didn't
int write_exe_array(asm_t* assembly, FILE* out_stream);
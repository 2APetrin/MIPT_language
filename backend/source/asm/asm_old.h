#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern FILE* logfile;

#define ACCURACY 100

typedef struct
{
    char * text;
    int    val;
    int    tok_type;
    int    cmd_type;
    size_t line;
} asm_tok_t;

typedef struct
{
    char* prog_text;
    int*  labels_array;
    int   labels_cnt;
    int*  labels_count_array;
    int*  out_array;

    int    out_arr_len;
    size_t num_of_symbols;
    size_t num_of_toks;

    asm_tok_t* toks_array;
} asm_data_t;

enum labels
{
    NUM_OF_LABELS = 20,
    FREE          = -1
};

enum token_types
{
    CMD       = 1,
    NUM       = 2,
    LABEL     = 3,
    LABEL_JMP = 4,
    REGISTER  = 5
};

enum cmd_codes
{
    ERROR  = -1,
    HLT    = 0,
    PUSH   = 1,
    ADD    = 2,
    SUB    = 3,
    MUL    = 4,
    DIV    = 5,
    OUT    = 6,
    POP    = 7,
    PUSHR  = 8,
    JMP    = 9,
    IN     = 10,
    CALL   = 11,
    RET    = 12,
    JMP_B  = 13,
    JMP_BE = 14,
    JMP_A  = 15,
    JMP_AE = 16,
    JMP_E  = 17,
    JMP_NE = 18,
    SQRT   = 19,
    NROOTS = 20,
    ALLNUM = 21,
    CAST   = 22
};

enum register_number
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};


//! @brief Runs the main process of assembly
//! 
//! @param [in] in_stream input stream of asm code
//! @param [in] out_stream output stream into file with binary code
void   run_asm(FILE* in_stream, FILE* out_stream);


//! @brief Function that fill in the asm_data struct with needed data. Makes tokens from text
//!
//! @param [in] in_stream input stream of asm code
//! @param [in] asm_data_t pointer to struct which contains data of asm code
int    asm_ctor(FILE* in_stream, asm_data_t* asm_data);


//! @brief Gets number of any type tokens
//!
size_t get_num_of_tokens(char* prog_text, size_t num_of_toks);

int    get_tokens(asm_tok_t* toks, size_t num_of_toks, char* prog_text);

char*  skip_spaces(char* start_ptr, size_t* line_ptr);

char*  get_end_of_word(char* word_ptr, size_t* p_line);

int    put_null_to_end_of_last_token(char* str_ptr);

int    asm_toks_init(asm_data_t* asm_data);

int    tok_info_init(asm_tok_t* tok, asm_data_t* asm_data, size_t itteration, int* minus_count);

int    is_jump(int cmd_type);

int    is_register(char* str);

int    check_tkn_for_string(char* str);

int    check_tkn_for_num(char* str);

int    check_tkn_for_label(char* str);

int    get_type(char* str);

int    labels_init(asm_data_t* asm_data);

//int    check_toks_arr(asm_tok_t* tok_arr, size_t num_of_toks, asm_data_t asm_data);

int    get_out_arr_len(asm_tok_t * tok_arr, size_t arr_len);
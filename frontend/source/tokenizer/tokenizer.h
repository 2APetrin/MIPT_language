#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "DSL.h"

#define ASSERT(cond)                                                 \
if (!(cond))                                                          \
{                                                                      \
    printf("\nError in %s in line %d in function %s in file %s\n\n",    \
            #cond, __LINE__, __PRETTY_FUNCTION__, __FILE__),             \
    abort();                                                              \
}

#define FREE()                     \
{                                   \
    POS++;                           \
    free(TOKEN_BUFF[POS-1]->word);    \
    TOKEN_BUFF[POS-1]->word = nullptr; \
    free(TOKEN_BUFF[POS-1]);            \
    TOKEN_BUFF[POS-1] = nullptr;         \
}


//! @brief extern code file
extern FILE* code_file;


//! @brief type of our elements values
typedef double elem_t;


//! @brief max nuber of funcs
const unsigned MAX_FUNC_CNT = 1024;

//! @brief maximum length of word
const unsigned MAX_WORD_LEN  = 128;


//! @brief maximum variables count
const unsigned MAX_VAR_COUNT = 128;


//! @brief maximum number of connecting elements
const unsigned MAX_DOT_COUNT = (1 << 13);


//! @brief const for error while compiling
const int COMPILE_ERROR = 1;


//! @brief universal const for poison
const unsigned POISON = 0xDEADBEEF;


//! @brief structure of variable
typedef struct
{
    char*  name;
    elem_t value;
} var_t;


//! @brief struct of function
typedef struct 
{
    char*    name;
    unsigned var_cnt;
} func_t;



//! @brief types of lexical tokens
enum token_type
{
    TYPE_NUM = 1,

    OP_ADD   = 2,
    OP_SUB   = 3,
    OP_MUL   = 4,
    OP_DIV   = 5,

    TYPE_VAR        = 6,
    TYPE_FUNC_INIT  = 7,

    TYPE_PRINT      = 8,

    TYPE_START        = 9,
    TYPE_FINISH       = 10,
    TYPE_O_BRCKT      = 11,
    TYPE_C_BRCKT      = 12,
    TYPE_DOT          = 13,
    TYPE_PRINT_BRCKET = 14,
    TYPE_STRING       = 15,

    TYPE_VAR_INIT     = 16,
    TYPE_ASSIGNMENT   = 17,
    TYPE_IF           = 18,
    TYPE_ELSE         = 19,
    TYPE_IF_BRCKET    = 20,

    TYPE_EQ           = 21,
    TYPE_NEQ          = 22,
    TYPE_GREATER      = 23,
    TYPE_GREATER_EQ   = 24,
    TYPE_LESS         = 25,
    TYPE_LESS_EQ      = 26,

    TYPE_EXPR_O_BR    = 27,
    TYPE_EXPR_C_BR    = 28,

    TYPE_LOOP_FIRST   = 29,
    TYPE_LOOP_CLOSE   = 30,

    TYPE_DECREASE     = 31,
    TYPE_FUNC_ID      = 32,

    TYPE_RETURN       = 33,
    TYPE_RETURN_BRCKT = 34,
    TYPE_FUNC_CALL    = 35,
    TYPE_VAR_SEPARATE = 36,

    TYPE_O_F_BR       = 37,
    TYPE_C_F_BR       = 38,

    TYPE_RETURN_VOID  = 39,

    TYPE_SCANF        = 40,
    TYPE_SQRT         = 41,
    TYPE_SQRT_BRCKT   = 42,
    TYPE_NROOTS       = 43,
    TYPE_ALLNUM       = 44,
};


//! @brief struct of lexical token
typedef struct token
{
    char*      word;
    token_type type;
    elem_t     value;

    token*     parent;
    token*     left_child;
    token*     right_child;

    unsigned   line;
    unsigned   pos;
} token_t;


//! @brief struct of string separation
typedef struct
{
    char**   text_lines;
    char*    text_buff;

    unsigned lines_cnt;
    unsigned words_cnt;
    unsigned text_len;
} code_buff_t;


//! @brief struct of our code
typedef struct
{
    unsigned  position;   // universal position counter

    token_t** token_buff;
    token_t*  tree_root;
    token_t*  end;

    var_t**   var_buff;
    unsigned  var_cnt;

    func_t**  func_buff;
    unsigned  func_cnt;

    token_t** dot_tokens;
    unsigned  dot_count;
    int       status;

    code_buff_t code_buff;

} text_t;


//! @brief constructor of text struct
//!
//! @param [in] codefile_name pointer to name of file with code
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int text_ctor(const char* codefile_name, text_t* text);


//! @brief gets pointers to strings in TEXT_LINES
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int get_strings(text_t* text);


//! @brief gets numer of strings in text buffer
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int get_num_of_words(text_t* text);


//! @brief gets number of lines in text buffer
//! @brief puts 0 in the end of line
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int get_num_of_lines(text_t* text);


//! @brief makes tokens out of a line
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int get_tokens(text_t* text);


//! @brief skips all blank symbols like spaces tabs and
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//! @param [in] i number of line in TEXT_LINES
//!
//! @return 0 if all is ok, 1 if error
int skip_blanks(text_t* text, unsigned int i);


//! @brief destructor of text struct
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
void text_dtor(text_t* text);


//! @brief gets tokens from line
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int tokenize_line(text_t* text, unsigned int i);


//! @brief gets word from line
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//! @param [in] i number of line in TEXT_LINES
//!
//! @return 0 if all is ok, 1 if error
int get_word(text_t* text, unsigned int i);


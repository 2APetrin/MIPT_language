#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "../tree/tree.h"

#define ASSERT(cond)                                                     \
    if (!(cond))                                                          \
    {                                                                      \
        printf("\nError in %s in line %d in function %s in file %s\n\n",    \
                #cond, __LINE__, __PRETTY_FUNCTION__, __FILE__),             \
        abort();                                                              \
    }

#define FREE()                    \
    {                              \
    POS++;                          \
    free(TOKEN_BUFF[POS-1]->word);   \
    free(TOKEN_BUFF[POS-1]);          \
    }


// ========================== DSL ==========================

#define TEXT_LINES text->code_buff.text_lines // pointers to first elements of lines
#define TEXT_BUFF  text->code_buff.text_buff
#define TEXT_LEN   text->code_buff.text_len   // number of symbols in text without 0 in the end
#define WORDS_CNT  text->code_buff.words_cnt
#define LINES_CNT  text->code_buff.lines_cnt
#define VAR_CNT    text->var_cnt
#define POS        text->position
#define TOKEN_BUFF text->token_buff
#define VAR_BUFF   text->var_buff

// =========================================================


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


//! @brief universal const for poison
const unsigned POISON = 0xDEADBEEF;


//! @brief structure of variable
typedef struct
{
    char*  name;
    elem_t value;
} var_t;


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
    TYPE_GREATER      = 22,
    TYPE_GREATER_EQ   = 23,
    TYPE_LESS         = 24,
    TYPE_LESS_EQ      = 25,

    TYPE_EXPR_O_BR    = 26,
    TYPE_EXPR_C_BR    = 27,

    TYPE_LOOP_FIRST   = 28,
    TYPE_LOOP_CLOSE   = 29,

    TYPE_DECREASE     = 30,
    TYPE_FUNC_ID      = 31,

    TYPE_RETURN       = 32,
    TYPE_RETURN_BRCKT = 33,
    TYPE_FUNC_CALL    = 34,
    TYPE_VAR_SEPARATE = 35,

    TYPE_O_F_BR       = 36,
    TYPE_C_F_BR       = 37
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
    unsigned  position;   // universal var

    token_t** token_buff;
    token_t*  tree_root;
    token_t*  end;

    var_t**   var_buff;
    unsigned  var_cnt;

    char**    func_names;
    unsigned  func_cnt;

    code_buff_t code_buff;
} text_t;


//! @brief constructor of text struct
int text_ctor(const char* codefile_name, text_t* text);


//! @brief gets pointers to strings
int get_strings(text_t* text);


//! @brief gets numer of strings in text buffer
int get_num_of_words(text_t* text);


//! @brief gets number of lines in text buffer
int get_num_of_lines(text_t* text);


//! @brief makes tokens out of a line
int get_tokens(text_t* text);


//! @brief skips all blank symbols like spaces tabs and \n
int skip_blanks(text_t* text, unsigned int i);


//! @brief destructor of text struct
int text_dtor(text_t* text);


//! @brief gets tokens from line
int tokenize_line(text_t* text, unsigned int i);


//! @brief gets word from line
int get_word(text_t* text, unsigned int i);


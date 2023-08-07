#pragma once

#include "../tree/tree.h"
#include "../tokenizer/tokenizer.h"

//! @brief writes error in log file and returns nullptr
#define GET_SYNTAX_ERROR(str)                                                                                                             \
{                                                                                                                                          \
    fprintf(log_file, "<pre>\n");                                                                                                           \
    fprintf(log_file, str);                                                                                                                  \
    fprintf(log_file, "\nFound - %s\nline - %u\npos - %u\n</pre>\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);     \
    text->status = COMPILE_ERROR;                                                                                                              \
    return nullptr;                                                                                                                             \
}


//! @brief initializes node that is going to be created while copile executing
#define CREATE_RET_NODE();                     \
{                                               \
    ret_node = create_node(TYPE_DOT, POISON);    \
    DOTS_BUFF[DOTS_CNT] = ret_node;               \
    DOTS_CNT++;                                    \
}


token_t* get_general(text_t* text);

token_t* get_def_function(text_t* text);

token_t* get_start(text_t* text);

token_t* get_comp(text_t* text);

token_t* get_operator(text_t* text);

token_t* get_print(text_t* text);

token_t* get_If(text_t* text);

token_t* get_expr(text_t* text);

token_t* get_T(text_t* text);

token_t* get_P(text_t* text);

token_t* get_elem(text_t* text);

token_t* get_logical_expr(text_t* text);

token_t* get_variable_op(text_t* text);

token_t* get_var_initialization(text_t* text);

token_t* get_assignment(text_t* text);

token_t* get_loop(text_t* text);

token_t* get_decrease(text_t* text);

token_t* get_return(text_t* text);

token_t* get_func_call(text_t* text);
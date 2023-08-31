#pragma once

#include "../tree/tree.h"
#include "../tokenizer/tokenizer.h"

//! @brief get.cpp macros that writes error in log_file and returns nullptr
#define GET_SYNTAX_ERROR(str)                                                                                                             \
{                                                                                                                                          \
    text->status++;                                                                                                                            \
    fprintf(log_file, "<pre>\n");                                                                                                           \
    fprintf(log_file, str);                                                                                                                  \
    fprintf(log_file, "\nFound - %s\nline - %u\npos - %u\n</pre>\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);     \
    return nullptr;                                                                                                                             \
}


//! @brief get.cpp macros initializes node that is going to be created while compile executing
#define CREATE_RET_NODE();                     \
{                                               \
    ret_node = create_node(TYPE_DOT, POISON);    \
    DOTS_BUFF[DOTS_CNT] = ret_node;               \
    DOTS_CNT++;                                    \
}

//! @brief gets main rule of EBNF grammar of our programming language
//!
//! @param [out] text text_t pointer to text struct - main structure of frontend
//!
//! @see get_start() and get_def_function()
//!
//! @return root of a tree that is created while recursive descent parsing or a nullptr if error happened
token_t* get_general(text_t* text);

//! @brief gets definition of function based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a tree that is tree of function definition or a nullptr if error happened
token_t* get_def_function(text_t* text);

//! @brief gets main block of our programm (main function) based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a main codetree of user's programm that is created while recursive descent parsing or a nullptr if error happened
token_t* get_start(text_t* text);

//! @brief gets operators while it is able to do it (based on our EBNF grammar)
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a tree that is tree of code block in brackets ({ *** }) or a nullptr if error happened
token_t* get_comp(text_t* text);

//! @brief gets one operator of our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a operator's tree or a nullptr if error happened
token_t* get_operator(text_t* text);

//! @brief gets definition of print function of our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a print tree or a nullptr if error happened
token_t* get_print(text_t* text);

//! @brief gets IF operator of our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a IF tree with or without else or a nullptr if error happened
token_t* get_If(text_t* text);

//! @brief gets math expression with nubers, math operators and variables
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a expression tree or a nullptr if error happened
token_t* get_expr(text_t* text);

//! @brief part of get expression, gets multiplication and division operators
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a multiplication or division tree or a nullptr if error happened
token_t* get_T(text_t* text);

//! @brief part of get expression, gets brackets and what is in there
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a expression tree of bracets content or a nullptr if error happened
token_t* get_P(text_t* text);

//! @brief part of get expression, gets number or variable
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a number or variable token/tree_node or a nullptr if error happened
token_t* get_elem(text_t* text);

//! @brief gets logical expression with its sign based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of a condition tree or a nullptr if error happened
token_t* get_logical_expr(text_t* text);

//! @brief gets variable operator (initialization/assignment/function_call)
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of fitting for command tree or a nullptr if error happened
token_t* get_variable_op(text_t* text);

//! @brief gets variable initialization based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of var initialization tree or a nullptr if error happened
token_t* get_var_initialization(text_t* text);

//! @brief gets variable assignment based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of var assignment tree or a nullptr if error happened
token_t* get_assignment(text_t* text);

//! @brief gets while loop tree based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of while loop tree or a nullptr if error happened
token_t* get_loop(text_t* text);

//! @brief gets needed tree based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of needed function tree or a nullptr if error happened
token_t* get_func(text_t* text);

//! @brief gets decrease for 1 operator tree based on our EBNF grammar
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of decrease operator tree or a nullptr if error happened
token_t* get_decrease(text_t* text);

//! @brief gets return operator tree based on our EBNF grammar.
//! @brief It can be void return (withot arguments) or a normal return with expression in it
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of decrease operator tree or a nullptr if error happened
token_t* get_return(text_t* text);

//! @brief gets function call tree based on our EBNF grammar.
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of function call tree or a nullptr if error happened
token_t* get_func_call(text_t* text);

//! @brief gets scanf call tree based on our EBNF grammar.
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of scanf call tree or a nullptr if error happened
token_t* get_scanf(text_t* text);

//! @brief gets sqrt call tree based on our EBNF grammar.
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of sqrt call tree or a nullptr if error happened
token_t* get_sqrt(text_t* text);

//! @brief gets allnum call tree based on our EBNF grammar.
//!
//! @note that is a command that writes in terminal that every number fits (is used in square equasion solver)
//! @see examples/quadratic.mipt
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of allnum call tree or a nullptr if error happened
token_t* get_allnum(text_t* text);

//! @brief gets noroots call tree based on our EBNF grammar.
//!
//! @note that is a command that writes in terminal that no numbers fit (is used in square equasion solver)
//! @see examples/quadratic.mipt
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return root of noroots call tree or a nullptr if error happened
token_t* get_nroots(text_t* text);

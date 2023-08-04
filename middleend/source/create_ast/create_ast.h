#pragma once

#include <stdio.h>
#include "../../../frontend/source/tokenizer/tokenizer.h"


typedef struct
{
    char*    ast_buff;
    unsigned len;

    char**   vars;
    unsigned var_count;

    token_t* ast_root;
    unsigned pos;
} ast_tree_t;


extern FILE* log_file;


//! @brief reads ast tree from temp file
int read_ast_tree(void);


//! @brief initializes ast tree struct
int ast_ctor(ast_tree_t* tree, const char* filename);


//! @brief makes tree from file
int create_ast_tree(ast_tree_t* tree);


//! @brief gets one pair of brackets and their insides
token_t* get_module(ast_tree_t* tree);


int schet(int n);

#pragma once

#include "../../../frontend/source/tokenizer/tokenizer.h"


//! error processing for ast_tree
#define AST_SYNTAX_ERROR(str)                                    \
{                                                                 \
    fprintf(ast_log_file, "<pre>ERROR in pos %u\n", TEXT_POS + 1); \
    fprintf(ast_log_file, str);                                     \
    fprintf(ast_log_file, "\nfound %c</pre>\n", BUFF[TEXT_POS]);     \
    tree->status++;                                                   \
    return nullptr;                                                    \
}

#define BUFF  tree->ast_buff // tree text buffer
#define LEN   tree->len      // length of tree text buffer
#define TEXT_POS tree->pos


typedef struct
{
    char*    ast_buff;
    unsigned len;

    char**   vars;
    unsigned var_count;

    token_t* ast_root;
    unsigned pos;

    int simplify_status;
    int status;
} ast_tree_t;


extern FILE* log_file;


//! @brief reads ast tree from temp file
int read_ast_tree(ast_tree_t* tree);


//! @brief initializes ast tree struct
int ast_ctor(ast_tree_t* tree, const char* filename);

int ast_dtor(ast_tree_t* tree);


//! @brief makes tree from file
int create_ast_tree(ast_tree_t* tree);


//! @brief gets one pair of brackets and their insides
token_t* get_module(ast_tree_t* tree);

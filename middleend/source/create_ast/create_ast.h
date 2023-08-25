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
#define TEXT_POS tree->pos   // universal position marker


//! @brief main ast structure
typedef struct
{
    char*    ast_buff; // tree text buffer
    unsigned len;      // length of tree text buffer

    char**   vars;      // buffer with variables of ast tree
    unsigned var_count; // number of variables in ast tree

    token_t* ast_root; // root of ast tree
    unsigned pos;      // universal position marker

    int simplify_status; // status for simplification
    int status;          // error status
} ast_tree_t;


//! @brief global log_file variable
extern FILE* log_file;


//! @brief reads ast tree from temp file
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int read_ast_tree(ast_tree_t* tree);


//! @brief initializes ast tree struct
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int ast_ctor(ast_tree_t* tree, const char* filename);

int ast_dtor(ast_tree_t* tree);


//! @brief makes tree from file
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int create_ast_tree(ast_tree_t* tree);


//! @brief gets one pair of brackets and their insides
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
token_t* get_module(ast_tree_t* tree);

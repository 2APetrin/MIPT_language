#pragma once

#include "../create_ast/create_ast.h"

//! @brief const word for void argument
#define VOID_WORD_MIPT "neud"

//! @brief main function of converting ast to our asm code
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int ast_to_assembly_converter(ast_tree_t* tree);

//! @brief writes to stream root and what's below in my asm language
//!
//! @param [in] node pointer to current node to write down
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//! @param [in] stream stream pointer to file to write in
//!
//! @return 0 if worked correctly and 1 if it didn't
int subtree_to_assembly(token_t* node, ast_tree_t* tree, FILE* stream);

//! @brief pops function parameters when the function is called
//!
//! @param [in] node pointer to current node to write down
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//! @param [in] stream stream pointer to file to write in
//!
//! @return 0 if worked correctly and 1 if it didn't
int pop_func_params(token_t* node, ast_tree_t* tree, FILE* stream);


//! @brief writes push for local variables to save them in stack
//!
//! @param [in] node pointer to current node to write down
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//! @param [in] stream stream pointer to file to write in
//!
//! @return 0 if worked correctly and 1 if it didn't
int push_local_vars(token_t* node, ast_tree_t* tree, FILE* stream);

//! @brief writes pop for local variables to take them back from stack
//!
//! @param [in] node pointer to current node to write down
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//! @param [in] stream stream pointer to file to write in
//!
//! @return 0 if worked correctly and 1 if it didn't
int get_local_vars(token_t* node, ast_tree_t* tree, FILE* stream);

//! @brief finds word for needed math operator
//!
//! @param [in] type type of operator
//!
//! @return pointer to needed word or nullptr
const char* get_op_word_from_type(token_type type);
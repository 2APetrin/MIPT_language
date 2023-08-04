#pragma once

#include "../tokenizer/tokenizer.h"
#include "../tree/tree.h"
#include "../get/get.h"

//! @brief creates a tree of our token-nodes
token_t* create_tree_of_tokens(text_t* text);


//! @brief extern log file
extern FILE* log_file;


//! @brief initializes tokens
int init_tokens(token_t** tok_buff, unsigned int tok_count);


//! @brief gets token type out of it's word
token_type get_tok_type(const char* word);


//! @brief checks if word is a number
int number_check(const char* word);


//! @brief gets type name from type number
const char* get_typename_from_toktype(token_type type_num);


//! @brief writes tree in preorder to transfer it to the middle end part
int write_tree_preorder(text_t* text);


//! @brief writes node and what is below
int write_subtree_preorder(token_t* node, FILE* stream);

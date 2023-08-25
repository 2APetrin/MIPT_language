#pragma once

#include "../tokenizer/tokenizer.h"
#include "../tree/tree.h"
#include "../get/get.h"

//! @brief extern log file
extern FILE* log_file;


//! @brief creates a tree of our token-nodes
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return first tree root
int create_tree_of_tokens(text_t* text);


//! @brief initializes tokens
//!
//! @param [out] tok_buff buffer of tokens
//! @param [out] tok_count number of tokens in tok_buff
//!
//! @return 0 if all is ok, 1 if error
int init_tokens(token_t** tok_buff, unsigned int tok_count);


//! @brief gets token type out of it's word
//!
//! @param [in] word pointer to token word
//!
//! @return token type or TYPE_VAR if unknown name
token_type get_tok_type(const char* word);


//! @brief checks if word is a number
//!
//! @param [in] word pointer to token word
//!
//! @return 1 if word is a number and 0 if it isn't
int number_check(const char* word);


//! @brief gets type name from type number
//!
//! @param [in] type_num type of token
//!
//! @return pointer to word that is connected to type_num
const char* get_typename_from_toktype(token_type type_num);


//! @brief writes tree in preorder to transfer it to the middle end part
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//!
//! @return 0 if all is ok, 1 if error
int write_tree_preorder(text_t* text);


//! @brief writes node and what is below
//!
//! @param [out] text pointer to text_t struct - main structure of frontend
//! @param [in] stream pointer to OUT stream to file with ast tree
//!
//! @return 0 if all is ok, 1 if error
int write_subtree_preorder(token_t* node, FILE* stream);

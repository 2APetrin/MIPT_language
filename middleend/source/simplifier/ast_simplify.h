#pragma once

#include "../create_ast/create_ast.h"

//! @brief global ast log file variable
extern FILE* ast_log_file;

//! @brief constant for comparing double numbers. If they are close enough (difference is less that EPSYLON) they are equal
const double EPSYLON = 0.00001;


//! @brief simplyfies ast subtree
//!
//! @param [out] node pointer to pointer to node which subtree is needed to simplify
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int simplify_ast_subtree(token_t** node, ast_tree_t* tree);

//! @brief simplyfies ast tree
//!
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return 0 if worked correctly and 1 if it didn't
int simplify_ast_tree(ast_tree_t* tree);

//! @brief evaluates expression subtree value
//!
//! @param [out] node pointer to pointer to node which subtree is needed to simplify
//! @param [out] tree pointer to ast_tree_t struct - main ast block structure
//!
//! @return result of expression tree
elem_t eval(token_t* node, ast_tree_t* tree);


//! @brief checks if doubles are equal
//!
//! @param [out] val1 first double number
//! @param [out] val2 second double number
//!
//! @return true if doubles are equal and false if not
int equald(double val1, double val2);

//! @brief checs for vars in subtree of that node
//!
//! @param [in] node pointer to node that is checked
//!
//! @return true if there are vars in subtree and false if not
int subtree_var_check(token_t* node);

//! @brief checks if that node is dot that can be removed
//!
//! @param [in] node pointer to node that is checked
//!
//! @return true if dot is not nessesary and false if not
int is_useless_dot(token_t* node);

//! @brief makes copy of that subtree
//!
//! @param [in] node pointer to node that is root of subtree
//!
//! @return pointer to root of copied tree
token_t* copy_subtree(token_t* node);
#pragma once

#include "../create_ast/create_ast.h"

#define VOID_WORD_MIPT "neud"

int ast_to_assembly_converter(ast_tree_t* tree);

int subtree_to_assembly(token_t* node, ast_tree_t* tree, FILE* stream);

int print_asm_node(token_t* node, FILE* stream);

int pop_func_params(token_t* node, ast_tree_t* tree, FILE* stream);

int push_call_params(token_t* node, ast_tree_t* tree, FILE* stream);

int push_local_vars(token_t* node, ast_tree_t* tree, FILE* stream);

int get_local_vars(token_t* node, ast_tree_t* tree, FILE* stream);

const char* get_op_word_from_type(token_type type);
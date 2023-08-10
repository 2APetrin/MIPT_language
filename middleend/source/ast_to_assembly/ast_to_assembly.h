#pragma once

#include "../create_ast/create_ast.h"

int ast_to_assembly_converter(ast_tree_t* tree);

int subtree_to_assembly(token_t* node, FILE* stream);

int print_asm_node(token_t* node, FILE* stream);
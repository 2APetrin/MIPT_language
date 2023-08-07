#pragma once

#include "../tokenizer/tokenizer.h"

token_t* create_node(token_type type, elem_t value, token_t* l_node = nullptr, token_t* r_node = nullptr);
token_t* new_num(elem_t value);
int tree_free(token_t * node);

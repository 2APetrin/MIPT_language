#pragma once

#include "../tokenizer/tokenizer.h"

token_t* create_node(token_type type, elem_t value, token_t* l_node = nullptr, token_t* r_node = nullptr);
int tree_free(token_t * node);

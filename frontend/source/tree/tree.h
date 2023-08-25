#pragma once

#include "../tokenizer/tokenizer.h"


/**
 * \brief creates node. allocates memory for it etc
 *
 * \param type token type
 * \param value value of created node
 * \param l_node left node to linr
 * \param r_node right node to link
 *
 * \return pointer to created node
*/
token_t* create_node(token_type type, elem_t value, token_t* l_node = nullptr, token_t* r_node = nullptr);

/**
 * \brief creates number node
 *
 * \param value value of created node
 *
 * \return pointer to created node
*/
token_t* new_num(elem_t value);

/**
 * \brief frees subtree of that node
 *
 * \return 1 if node is nullptr and 0 if all's ok
*/
int tree_free(token_t * node);

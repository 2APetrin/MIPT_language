#include "tree.h"
#include "../tokenizer/tokenizer.h"

token_t* create_node(token_type type, elem_t value, token_t* l_node, token_t* r_node)
{
    token_t* new_node = (token_t*) calloc (1, sizeof(token_t));

    if (type == TYPE_DOT)
    {
        new_node->type        = TYPE_DOT;

        new_node->left_child  = l_node;
        if (l_node) l_node->parent = new_node;
        new_node->right_child = r_node;
        if (r_node) r_node->parent = new_node;

        new_node->parent      = nullptr;
        new_node->word        = nullptr;

        new_node->pos         = POISON;
        new_node->line        = POISON;
        new_node->value       = value;

        return new_node;
    }

    if ((type >= OP_ADD) && (type <= OP_DIV))
    {
        new_node->type        = type;

        new_node->left_child  = l_node;
        if (l_node) l_node->parent = new_node;
        new_node->right_child = r_node;
        if (r_node) r_node->parent = new_node;

        new_node->parent      = nullptr;
        new_node->word        = nullptr;

        new_node->pos         = POISON;
        new_node->line        = POISON;
        new_node->value       = value;

        return new_node;
    }

    new_node->type        = type;
    new_node->left_child  = l_node;
    if (l_node) l_node->parent = new_node;
    new_node->right_child = r_node;
    if (r_node) r_node->parent = new_node;

    new_node->parent      = nullptr;
    new_node->word        = nullptr;

    new_node->pos         = POISON;
    new_node->line        = POISON;
    new_node->value       = value;

    return new_node;
}


int tree_free(token_t* node)
{
    if (node == nullptr)
        return 1;

    tree_free(node->left_child);
    tree_free(node->right_child);

    free(node->word);
    node->word = nullptr;

    free(node);
    node = nullptr;

    return 0;
}


token_t* new_num(elem_t value)
{
    return create_node(TYPE_NUM, value);
}
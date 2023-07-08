#include "get.h"
#include "../main_frontend/frontend.h"
#include "../tree/log_tree.h"

token_t* get_general(text_t* text)
{
    ASSERT(text);
    POS = 0;

    token_t* temp = get_def_function(text);
    token_t* curr = temp;

    token_t* def_tree_root = temp;

    while (temp)
    {
        temp = get_def_function(text);
        curr->right_child = temp;
        curr = temp;
    }

    token_t* main_block = get_start(text);

    token_t* ret = create_node(TYPE_DOT, POISON);

    ret->left_child    = def_tree_root;
    ret->right_child   = main_block;
    if (main_block) main_block->parent = ret;

    tree_print_dump(ret);

    return ret;
}


token_t* get_def_function(text_t* text) 
{
    ASSERT(text);

    return nullptr;
} 


token_t* get_start(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_START) FREE()
    else
    {
        printf("error in get start. Start (born) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
    {
        printf("error in get start. Open bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* ret = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
    {
        printf("error in get start. Close bracket (get_sent_down) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    if (TOKEN_BUFF[POS]->type == TYPE_FINISH)
    {
        free(TOKEN_BUFF[POS]->word);
        free(TOKEN_BUFF[POS]);
    }
    else
    {
        printf("error in get start. Finish (die) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    return ret;
}


token_t* get_comp(text_t* text)
{
    ASSERT(text);

    token_t* temp = get_operator(text);
    token_t* curr = temp;

    token_t* comp_tree_root = temp;

    while (temp)
    {
        temp = get_operator(text);
        curr->right_child = temp;
        if (temp) temp->parent = curr;

        curr = temp;
    }

    return comp_tree_root;
}


token_t* get_operator(text_t* text)
{
    ASSERT(text);

    switch (TOKEN_BUFF[POS]->type)
    {
        case TYPE_PRINT: return get_print(text);
        case TYPE_IF:    return get_If(text);

        default:         return nullptr;
    }
}


token_t* get_If(text_t* text)
{
    ASSERT(text);

    return nullptr;
}


token_t* get_print(text_t* text)
{
    ASSERT(text);

    token_t* ret_root = create_node(TYPE_DOT, POISON);
    ret_root->left_child  = TOKEN_BUFF[POS];

    TOKEN_BUFF[POS]->parent = ret_root;
    POS++;

    token_t* node = get_expr(text);
    ret_root->left_child->left_child = node;

    if (node) node->parent = ret_root->left_child;

    if (TOKEN_BUFF[POS]->type != TYPE_PRINT_BRCKET)
    {
        printf("error in print. Print bracket is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
    {
        printf("error in print. Dot in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    return ret_root;
}


token_t* get_expr(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_T(text);
    if (!node1) return nullptr;

    while (TOKEN_BUFF[POS]->type == OP_ADD || TOKEN_BUFF[POS]->type == OP_SUB)
    {
        int type = TOKEN_BUFF[POS]->type;
        POS++;

        token_t* node2 = get_T(text);
        if (!node2) return nullptr;

        token_t* node3 = nullptr;

        if (type == OP_ADD) node3 = create_node(OP_ADD, POISON, node1, node2);
        if (type == OP_SUB) node3 = create_node(OP_SUB, POISON, node1, node2);

        node3->left_child->parent  = node3;
        node3->right_child->parent = node3;

        node1 = node3;
    }

    return node1;
}


token_t* get_T(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_P(text);
    if (!node1) return nullptr;

    while (TOKEN_BUFF[POS]->type == OP_MUL || TOKEN_BUFF[POS]->type == OP_DIV)
    {
        int type = TOKEN_BUFF[POS]->type;
        POS++;

        token_t* node2 = get_P(text);
        if (!node2) return nullptr;

        token_t* node3 = nullptr;

        if (type == OP_MUL) node3 = create_node(OP_MUL, POISON, node1, node2);
        if (type == OP_DIV) node3 = create_node(OP_DIV, POISON, node1, node2);

        node3->left_child->parent  = node3;
        node3->right_child->parent = node3;

        node1 = node3;
    }

    return node1;
}


token_t* get_P(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_EXPR_O_BR)
    {
        FREE();
        token_t* expr_root = get_expr(text);

        if (TOKEN_BUFF[POS]->type != TYPE_EXPR_C_BR)
        {
            printf("Error in get expression: expected )\nFound - %s\nline - %u\npos - %u\n.", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
            return nullptr;
        }
        FREE();

        return expr_root;
    }

    token_t* node = get_elem(text);
    return node;
}


token_t* get_elem(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_NUM || TOKEN_BUFF[POS]->type == TYPE_VAR)
    {
        POS++;
        return TOKEN_BUFF[POS-1];
    }

    printf("Error in get expression: expected num or var\nFound - %s\nline - %u\npos - %u\n.", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
    return nullptr;
}

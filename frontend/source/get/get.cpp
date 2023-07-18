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

    //printf("entered if\n");

    token_t* ret_root    = create_node(TYPE_DOT, POISON);
    ret_root->left_child = TOKEN_BUFF[POS];

    TOKEN_BUFF[POS]->parent = ret_root;
    token_t* curr = TOKEN_BUFF[POS];
    POS++;

    curr->left_child = get_logical_expr(text);
    if (curr->left_child) curr->left_child->parent = curr;

    if (TOKEN_BUFF[POS]->type == TYPE_IF_BRCKET) FREE()
    else
    {
        printf("error in get if. Close bracket (togda) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
    {
        printf("error in get if. Open bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* if_code_tree   = get_comp(text);
    token_t* else_code_tree = nullptr;

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
    {
        printf("error in get if. Close bracket (get_sent_down) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    if (TOKEN_BUFF[POS]->type == TYPE_ELSE)
    {
        token_t* else_token = TOKEN_BUFF[POS];
        POS++;

        if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
        else
        {
            printf("error in get if. Open bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
            return nullptr;
        }

        else_code_tree = get_comp(text);

        if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
        else
        {
            printf("error in get if. Close bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
            return nullptr;
        }

        curr->right_child  = else_token;
        else_token->parent = curr;

        curr->right_child->left_child  = if_code_tree;
        if (if_code_tree) if_code_tree->parent = curr->right_child;

        curr->right_child->right_child = else_code_tree;
        if (else_code_tree) else_code_tree->parent = curr->right_child;

        return ret_root;
    }

    curr->right_child = if_code_tree;
    if (curr->right_child) curr->right_child->parent = curr;

    return ret_root;
}


token_t* get_logical_expr(text_t* text)
{
    ASSERT(text);

    token_t* left_part = get_expr(text);

    if ((TOKEN_BUFF[POS]->type < TYPE_EQ) || (TOKEN_BUFF[POS]->type > TYPE_LESS_EQ))
    {
        printf("error in get logical expression. Sign is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* sign = TOKEN_BUFF[POS];
    POS++;

    token_t* right_part = get_expr(text);

    sign->left_child  = left_part;
    if (left_part) left_part->parent = sign;

    sign->right_child = right_part;
    if (right_part) right_part->parent = sign;

    return sign;
}


token_t* get_print(text_t* text)
{
    ASSERT(text);

    token_t* ret_root    = create_node(TYPE_DOT, POISON);
    ret_root->left_child = TOKEN_BUFF[POS];

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


// =============================== expr zone ===============================


token_t* get_expr(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_T(text);
    if (!node1) return nullptr;

    token_t* node = nullptr;

    while (TOKEN_BUFF[POS]->type == OP_ADD || TOKEN_BUFF[POS]->type == OP_SUB)
    {
        node = TOKEN_BUFF[POS];
        POS++;

        token_t* node2 = get_T(text);
        if (!node2) return nullptr;

        node->left_child  = node1;
        node->right_child = node2;

        node1->parent = node;
        node2->parent = node;

        node1 = node;
    }

    return node1;
}


token_t* get_T(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_P(text);
    if (!node1) return nullptr;

    token_t* node = nullptr;

    while (TOKEN_BUFF[POS]->type == OP_MUL || TOKEN_BUFF[POS]->type == OP_DIV)
    {
        node = TOKEN_BUFF[POS];
        POS++;

        token_t* node2 = get_T(text);
        if (!node2) return nullptr;

        node->left_child  = node1;
        node->right_child = node2;

        node1->parent = node;
        node2->parent = node;

        node1 = node;
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

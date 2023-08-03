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
    if (def_tree_root) def_tree_root->parent = ret;

    return ret;
}


token_t* get_def_function(text_t* text) 
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_FUNC_INIT) return nullptr;
    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
    {
        printf("error in get function def. Error in name of function\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* ret_node       = create_node(TYPE_DOT, POISON);

    TOKEN_BUFF[POS]->type   = TYPE_FUNC_ID;

    token_t* middle_node    = create_node(TYPE_DOT, POISON);
    middle_node->left_child = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = middle_node;
    middle_node->parent     = ret_node;
    POS++;

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
    {
        printf("error in get definition of func. Open bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* func_code = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
    {
        printf("error in get definition of func. Close bracket (get_sent_down) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    middle_node->right_child = func_code;
    if (func_code) func_code->parent = middle_node;
    ret_node->left_child = middle_node;

    return ret_node;
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
        temp                   = get_operator(text);
        curr->right_child      = temp;
        if (temp) temp->parent = curr;

        curr = temp;
    }

    return comp_tree_root;
}


token_t* get_operator(text_t* text)
{
    ASSERT(text);

    token_type type = TOKEN_BUFF[POS]->type;

    if (type == TYPE_IF)                           return get_If(text);

    if (type == TYPE_VAR_INIT || type == TYPE_VAR) return get_variable_op(text);

    if (type == TYPE_PRINT)                        return get_print(text);

    if (type == TYPE_LOOP_FIRST)                   return get_loop(text);

    if (type == TYPE_DECREASE)                     return get_decrease(text);

    if (type == TYPE_RETURN)                       return get_return(text);

    return nullptr;
}


token_t* get_loop(text_t* text)
{
    ASSERT(text);

    token_t* ret_node    = create_node(TYPE_DOT, POISON);
    token_t* curr_node   = TOKEN_BUFF[POS];

    ret_node->left_child = curr_node;
    curr_node->parent    = ret_node;
    POS++;

    //printf("1 -- Found - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);

    token_t* logical_expr = get_expr(text);

    //printf("2 -- Found - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);

    if (TOKEN_BUFF[POS]->type != TYPE_LOOP_CLOSE)
    {
        printf("error in loop initialization. Next node after expression isn't close loop bracket (nulevok_untill_they_run_out)\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    curr_node->left_child                  = logical_expr;
    if (logical_expr) logical_expr->parent = curr_node;

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
    {
        printf("error in get loop. Open bracket (enter_mipt) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* right_subtree = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
    {
        printf("error in get loop. Close bracket (get_sent_down) is missing\nFound - %s\nline - %u\npos - %u\n", get_typename_from_toktype(TOKEN_BUFF[POS]->type), TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    curr_node->right_child                   = right_subtree;
    if (right_subtree) right_subtree->parent = curr_node;

    return ret_node;
}


token_t* get_decrease(text_t* text)
{
    ASSERT(text);

    token_t* ret_node    = create_node(TYPE_DOT, POISON);
    token_t* curr_node   = TOKEN_BUFF[POS];

    ret_node->left_child = curr_node;
    curr_node->parent    = ret_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
    {
        printf("error in decrease operator. Next node after solve_nulevka init isn't variable. Found - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    curr_node->left_child   = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
    {
        printf("error in decrease operator. Dot in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    return ret_node;
}


token_t* get_return(text_t* text)
{
    ASSERT(text);

    token_t* ret_node    = create_node(TYPE_DOT, POISON);
    token_t* curr_node   = TOKEN_BUFF[POS];

    curr_node->parent    = ret_node;
    ret_node->left_child = curr_node;

    POS++;

    token_t* expr = get_expr(text);

    curr_node->left_child = expr;
    if (expr) expr->parent = curr_node;

    if (TOKEN_BUFF[POS]->type != TYPE_RETURN_BRCKT)
    {
        printf("error in return. Zadach in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
    {
        printf("error in return. Dot in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    return ret_node;
}


// ============================================================== variables zone ==============================================================


token_t* get_variable_op(text_t* text)
{
    ASSERT(text);

    token_type type = TOKEN_BUFF[POS]->type;

    if (type == TYPE_VAR_INIT) return get_var_initialization(text);
    if (type == TYPE_VAR)      return get_assignment(text);

    return nullptr;
}


token_t* get_var_initialization(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_VAR_INIT) return nullptr;

    token_t* ret_node    = create_node(TYPE_DOT, POISON);
    token_t* curr_node   = TOKEN_BUFF[POS];

    ret_node->left_child = TOKEN_BUFF[POS];
    curr_node->parent    = ret_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
    {
        printf("error in get var initialization. Next node after var init isn't variable\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    curr_node->left_child   = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_NUM)
    {
        printf("error in get var initialization. Next node after variable isn't number\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    curr_node->right_child = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
    {
        printf("error in var init. Dot in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    return ret_node;
}


token_t* get_assignment(text_t* text)
{
    token_t* ret_root   = create_node(TYPE_DOT, POISON);
    token_t* first_node = TOKEN_BUFF[POS]; 
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_ASSIGNMENT)
    {
        printf("error in assignment. podgon_pod is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }

    token_t* middle_part    = TOKEN_BUFF[POS];
    middle_part->left_child = first_node;
    first_node->parent      = middle_part;
    ret_root->left_child    = middle_part;
    middle_part->parent     = ret_root;
    POS++;

    token_t* right_part = get_expr(text);

    middle_part->right_child           = right_part;
    if (right_part) right_part->parent = middle_part;

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
    {
        printf("error in var assignment. Dot in the end is missing\nFound - %s\nline - %u\npos - %u\n", TOKEN_BUFF[POS]->word, TOKEN_BUFF[POS]->line, TOKEN_BUFF[POS]->pos);
        return nullptr;
    }
    FREE();

    return ret_root;
}



// ============================================================== if zone ==============================================================


token_t* get_If(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_IF) return nullptr;

    token_t* ret_root    = create_node(TYPE_DOT, POISON);
    token_t* curr        = TOKEN_BUFF[POS];
    ret_root->left_child = TOKEN_BUFF[POS];

    TOKEN_BUFF[POS]->parent = ret_root;
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

        curr->right_child->left_child          = if_code_tree;
        if (if_code_tree) if_code_tree->parent = curr->right_child;

        curr->right_child->right_child             = else_code_tree;
        if (else_code_tree) else_code_tree->parent = curr->right_child;

        return ret_root;
    }

    curr->right_child                                = if_code_tree;
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

    sign->left_child                 = left_part;
    if (left_part) left_part->parent = sign;

    sign->right_child                  = right_part;
    if (right_part) right_part->parent = sign;

    return sign;
}


// ============================================================== expr zone ==============================================================


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


// ============================================================== func zone ==============================================================


token_t* get_print(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_PRINT) return nullptr;

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
#include "ast_simplify.h"
#include "../../../frontend/source/tree/tree.h"
#include <math.h>


#define AST_SIMPLIFY_ERROR() \
{                             \
    return NAN;                \
}


int simplify_ast_subtree(token_t** node)
{
    ASSERT(node);

    if (!(*node) || (*node)->type == TYPE_NUM || (*node)->type == TYPE_VAR) return 0;

    simplify_ast_subtree(&((*node)->left_child));
    simplify_ast_subtree(&((*node)->right_child));

    token_t* old_node = *node;

    if (!subtree_var_check(old_node))
    {
        *node = new_num(eval(*node));
        tree_free(old_node);

        return 0;
    }

    if (is_useless_dot(*node))
    {
        if ((*node)->parent->right_child == *node)
            *(&((*node)->parent->right_child)) = copy_subtree((*node)->left_child);
        else
            *(&((*node)->parent->left_child)) = copy_subtree((*node)->left_child);

        tree_free(old_node);
        return 0;
    }

    return 0;
}


int is_useless_dot(token_t* node)
{
    ASSERT(node);

    if (node->type != TYPE_DOT)
        return 0;

    if (node->parent)
    {
        if (!node->right_child)
            return 1;
    }

    return 0;
}


token_t* copy_subtree(token_t* node)
{
    if (!node)
        return nullptr;

    token_t * ret_node = (token_t *) calloc(1, sizeof(token_t));

    ret_node->left_child  = copy_subtree(node->left_child);
    ret_node->right_child = copy_subtree(node->right_child);

    if (ret_node->left_child)  ret_node->left_child->parent  = ret_node;
    if (ret_node->right_child) ret_node->right_child->parent = ret_node;

    ret_node->type  = node->type;
    ret_node->value = node->value;

    if (node->type == TYPE_VAR || node->type == TYPE_FUNC_ID || node->type == TYPE_FUNC_CALL)
    {
        ret_node->word = (char*) calloc (MAX_WORD_LEN, sizeof(char));
        strncpy(ret_node->word, node->word, MAX_WORD_LEN-1);
    }

    return ret_node;
}


elem_t eval(token_t* node)
{
    if (!node) return NAN;

    token_t* Lc = node->left_child;
    token_t* Rc = node->right_child;

    token_type type = node->type;

    if (type == TYPE_NUM)
        return node->value;

    switch (type)
    {
        case OP_ADD:
        {
            if (Lc && Rc) return eval(Lc) + eval(Rc);
            AST_SIMPLIFY_ERROR();
        }

        case OP_SUB:
        {
            if (Lc && Rc) return eval(Lc) - eval(Rc);
            AST_SIMPLIFY_ERROR();
        }

        case OP_MUL:
        {
            if (Lc && Rc) return eval(Lc) * eval(Rc);
            AST_SIMPLIFY_ERROR();
        }

        case OP_DIV:
        {
            if (Lc && Rc)
            {
                elem_t val_r = eval(Rc);
                elem_t val_l = eval(Lc);

                if (!equald(val_r, 0))
                    return (val_l / val_r);

                AST_SIMPLIFY_ERROR();
            }
            AST_SIMPLIFY_ERROR();
        }

        default:
            AST_SIMPLIFY_ERROR();
    }
    AST_SIMPLIFY_ERROR();
}


int subtree_var_check(token_t * node)
{
    if (!node)
        return 0;

    token_type type = node->type;

    if (type > OP_DIV)
        return 1;

    if (node->type == TYPE_NUM)
        return 0;

    return subtree_var_check(node->left_child) || subtree_var_check(node->right_child);
}


int equald(double val1, double val2)
{
    if (!isfinite(val1)) return 0;
    if (!isfinite(val2)) return 0;

    if (val2 - val1 < EPSYLON && val2 - val1 > -1 * EPSYLON)
        return 1;
    return 0;
}
#include "ast_simplify.h"
#include "../../../frontend/source/tree/tree.h"
#include <math.h>


#define AST_SIMPLIFY_ERROR(str)     \
{                                    \
    fprintf(ast_log_file, "<pre>\n"); \
    fprintf(ast_log_file, str);        \
    fprintf(ast_log_file, "\n</pre>\n");\
    tree->status++;                      \
    return 1;                             \
}


int simplify_ast_tree(ast_tree_t* tree)
{
    ASSERT(tree);

    while (tree->simplify_status)
    {
        printf("%d\n", tree->simplify_status);
        tree->simplify_status = 0;
        simplify_ast_subtree(&(tree->ast_root), tree);
    }

    return 0;
}


#define CURR_NODE (*node)
#define CURR_LC CURR_NODE->left_child
#define CURR_RC CURR_NODE->right_child
#define CURR_LC_VAL_IS(num) ((CURR_LC->type == TYPE_NUM) && (equald(CURR_LC->value, (num))))
#define CURR_RC_VAL_IS(num) ((CURR_RC->type == TYPE_NUM) && (equald(CURR_RC->value, (num))))


int simplify_ast_subtree(token_t** node, ast_tree_t* tree)
{
    ASSERT(node);

    if (!CURR_NODE || CURR_NODE->type == TYPE_NUM || CURR_NODE->type == TYPE_VAR) return 0;

    simplify_ast_subtree(&(CURR_NODE->left_child),  tree);
    simplify_ast_subtree(&(CURR_NODE->right_child), tree);

    token_t* old_node = CURR_NODE;

    if (is_useless_dot(CURR_NODE))
    {
        CURR_NODE = copy_subtree(CURR_NODE->left_child);
        tree->simplify_status++;

        tree_free(old_node);
        return 0;
    }

    if (!subtree_var_check(old_node))
    {
        CURR_NODE = new_num(eval(CURR_NODE, tree));
        tree_free(old_node);
        tree->simplify_status++;

        return 0;
    }

    if ((CURR_NODE->type == OP_ADD || CURR_NODE->type == OP_SUB) && (CURR_LC_VAL_IS(0) || CURR_RC_VAL_IS(0)))
    {
        if (CURR_LC_VAL_IS(0))
        {
            CURR_NODE = copy_subtree(CURR_NODE->right_child);
            tree->simplify_status++;
            tree_free(old_node);

            return 0;
        }

        CURR_NODE = copy_subtree(CURR_NODE->left_child);
        tree->simplify_status++;
        tree_free(old_node);

        return 0;
    }

    if (CURR_NODE->type == OP_MUL && (CURR_LC_VAL_IS(0) || CURR_RC_VAL_IS(0)))
    {
        CURR_NODE = new_num(0);
        tree->simplify_status++;
        tree_free(old_node);

        return 0;
    }

    if (CURR_NODE->type == OP_MUL && (CURR_LC_VAL_IS(1) || CURR_RC_VAL_IS(1)))
    {
        if (CURR_LC_VAL_IS(1))
        {
            CURR_NODE = copy_subtree(CURR_NODE->right_child);
            tree->simplify_status++;

            tree_free(old_node);
            return 0;
        }
        CURR_NODE = copy_subtree(CURR_NODE->left_child);
        tree->simplify_status++;

        tree_free(old_node);
        return 0;
    }

    if (CURR_NODE->type == OP_DIV)
    {
        if (CURR_RC_VAL_IS(0))
        {
            AST_SIMPLIFY_ERROR("ERROR in simplify. Division by zero");
        }

        if (CURR_LC_VAL_IS(0))
        {
            CURR_NODE = new_num(0);
            tree->simplify_status++;
            tree_free(old_node);

            return 0;
        }

        if (CURR_RC_VAL_IS(1))
        {
            CURR_NODE = copy_subtree(CURR_NODE->left_child);
            tree->simplify_status++;

            tree_free(old_node);
            return 0;
        }
    }

    return 0;
}


int is_useless_dot(token_t* node)
{
    ASSERT(node);

    if (node->type != TYPE_DOT)
        return 0;

    if (!node->right_child)
        return 1;

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


elem_t eval(token_t* node, ast_tree_t* tree)
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
            if (Lc && Rc) return eval(Lc, tree) + eval(Rc, tree);
            AST_SIMPLIFY_ERROR("ERROR in evaluation. Left or right child is nullptr");
        }

        case OP_SUB:
        {
            if (Lc && Rc) return eval(Lc, tree) - eval(Rc, tree);
            AST_SIMPLIFY_ERROR("ERROR in evaluation. Left or right child is nullptr");
        }

        case OP_MUL:
        {
            if (Lc && Rc) return eval(Lc, tree) * eval(Rc, tree);
            AST_SIMPLIFY_ERROR("ERROR in evaluation. Left or right child is nullptr");
        }

        case OP_DIV:
        {
            if (Lc && Rc)
            {
                elem_t val_r = eval(Rc, tree);
                elem_t val_l = eval(Lc, tree);

                if (!equald(val_r, 0))
                    return (val_l / val_r);

                AST_SIMPLIFY_ERROR("ERROR in evaluation. Division by zero");
            }
            AST_SIMPLIFY_ERROR("ERROR in evaluation. Left or right child is nullptr");
        }

        default:
            AST_SIMPLIFY_ERROR("ERROR in evaluation. Unknown error");
    }
    AST_SIMPLIFY_ERROR("ERROR in evaluation. Unknown error");
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
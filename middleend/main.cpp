#include "source/simplifier/ast_simplify.h"
#include "../frontend/source/tree/tree.h"
#include "../frontend/source/tree/log_tree.h"

int main(void)
{
    ast_tree_t ast_tree = {0};

    if (read_ast_tree(&ast_tree))
    {
        printf("Error.Cannot read ast tree\nProgramm shutdown\n");
        return 1;
    }

    simplify_ast_subtree(&(ast_tree.ast_root));
    simplify_ast_subtree(&(ast_tree.ast_root));
    simplify_ast_subtree(&(ast_tree.ast_root));

    ast_tree_print_dump(ast_tree.ast_root);

    tree_free(ast_tree.ast_root);

    return 0;
}
#include "ast_to_assembly.h"
#include "../../../file_work/file_work.h"

int ast_to_assembly_converter(ast_tree_t* tree)
{
    ASSERT(tree);
    if (tree->status) return 1;

    FILE* stream = nullptr;
    open_write_file("temp/ass_code.asm", &stream);
    fprintf(stream, "# Programm is written in mipt languege\n# Which is created by Petrin Anton MIPT 1 course\n\n");

    subtree_to_assembly(tree->ast_root, stream);

    fprintf(stream, "hlt\n");

    return 0;
}

int subtree_to_assembly(token_t* node, FILE* stream)
{
    ASSERT(node);
    if (!node) return 0;

    switch (node->type)
    {
        case TYPE_DOT:
            subtree_to_assembly(node->left_child, stream);
            subtree_to_assembly(node->right_child, stream);
            return 0;

        case TYPE_PRINT:
            subtree_to_assembly(node->left_child, stream);
            fprintf(stream, "out\n");
            return 0;

        case TYPE_NUM:
            fprintf(stream, "push %lg\n", node->value);
            return 0;

        default:
            return 0;
    }
}

#include "ast_to_assembly.h"
#include "../../../file_work/file_work.h"

int ast_to_assembly_converter(ast_tree_t* tree)
{
    ASSERT(tree);
    if (tree->status) return 1;

    FILE* stream = nullptr;
    open_write_file("temp/ass_code.asm", &stream);
    fprintf(stream, "# Programm is written in mipt languege\n# Which is created by Petrin Anton MIPT 1 course\n\n");

    fprintf(stream, "# VAR_TABLE:\n");
    for (unsigned i = 0; i < tree->var_count; i++)
        fprintf(stream, "# %s - [%u]\n", tree->vars[i], i);

    fprintf(stream, "\n\njmp :main\n\n");

    if (tree->ast_root->left_child->type != TYPE_DOT && tree->ast_root->left_child->type != TYPE_FUNC_ID)
    {
        fprintf(stream, ":main\n");
        subtree_to_assembly(tree->ast_root, tree, stream);
        fprintf(stream, "hlt\n");
        fclose(stream);

        printf("AST tree is converted into asm code\n");

        return 0;
    }

    subtree_to_assembly(tree->ast_root->left_child, tree, stream);
    fprintf(stream, ":main\n");
    subtree_to_assembly(tree->ast_root->right_child, tree, stream);

    fprintf(stream, "hlt\n");
    fclose(stream);

    printf("AST tree is converted into asm code\n");

    return 0;
}

int subtree_to_assembly(token_t* node, ast_tree_t* tree, FILE* stream)
{
    if (!node) return 0;

    static int if_lbl_cnt    = -2;
    static int while_lbl_cnt = -2;
    int local_while_cnt = 0;
    int local_if_cnt = 0;

    switch (node->type)
    {
        case TYPE_DOT:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            return 0;

        case TYPE_SCANF:
            fprintf(stream, "   in\n");
            for (unsigned i = 0; i < tree->var_count; i++)
                if (!strcmp(node->left_child->word, tree->vars[i]))
                {
                    fprintf(stream, "   pop [%u]\n", i);
                    break;
                }
            return 0;

        case TYPE_PRINT:
            subtree_to_assembly(node->left_child, tree, stream);
            fprintf(stream, "   out\n");
            return 0;

        case TYPE_NUM:
            fprintf(stream, "   push %lg\n", node->value);
            return 0;

        case TYPE_VAR:
            for (unsigned i = 0; i < tree->var_count; i++)
                if (!strcmp(node->word, tree->vars[i]))
                {
                    fprintf(stream, "   push [%u]\n", i);
                    break;
                }
            return 0;

        case TYPE_ASSIGNMENT:
        case TYPE_VAR_INIT:
            subtree_to_assembly(node->right_child, tree, stream);
            if (node->right_child->type == TYPE_FUNC_CALL)
                fprintf(stream, "   push ax\n");
            for (unsigned i = 0; i < tree->var_count; i++)
                if (!strcmp(node->left_child->word, tree->vars[i]))
                {
                    fprintf(stream, "   pop [%u]\n", i);
                    break;
                }
            return 0;

        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "%s\n", get_op_word_from_type(node->type));
            return 0;

        case TYPE_FUNC_ID:
            if (!node->right_child)
            {
                fprintf(stream, ":%s\n", node->word);
                subtree_to_assembly(node->left_child, tree, stream);
                fprintf(stream, "\n");
                return 0;
            }

            fprintf(stream, ":%s\n", node->word);
            fprintf(stream, "   pop ax\n");
            pop_func_params(node->left_child, tree, stream);
            fprintf(stream, "   push ax\n");

            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "\n");
            return 0;

        case TYPE_RETURN:
            subtree_to_assembly(node->left_child, tree, stream);
            fprintf(stream, "   pop ax\n   ret\n");
            return 0;

        case TYPE_FUNC_CALL:
            if (!node->right_child && !node->left_child)
            {
                fprintf(stream, "   call :%s\n\n", node->word);
                return 0;
            }
            push_local_vars(node->left_child, tree, stream);

            subtree_to_assembly(node->left_child, tree, stream);
            fprintf(stream, "   call :%s\n\n", node->word);
            get_local_vars(node->left_child, tree, stream);

            return 0;

        case TYPE_RETURN_VOID:
            fprintf(stream, "   ret\n");
            return 0;

        case TYPE_DECREASE:
            for (unsigned i = 0; i < tree->var_count; i++)
                if (!strcmp(node->left_child->word, tree->vars[i]))
                {
                    fprintf(stream, "   push [%u]\n", i);
                    break;
                }

            fprintf(stream, "   push 1\n");
            fprintf(stream, "   sub\n");
            for (unsigned i = 0; i < tree->var_count; i++)
                if (!strcmp(node->left_child->word, tree->vars[i]))
                {
                    fprintf(stream, "   pop [%u]\n", i);
                    break;
                }
            return 0;

        case TYPE_IF:
            if_lbl_cnt += 2;
            local_if_cnt = if_lbl_cnt;

            fprintf(stream, "\n# IF\n");
            subtree_to_assembly(node->left_child, tree, stream);
            if (node->right_child->type == TYPE_ELSE)
            {
                subtree_to_assembly(node->right_child->left_child, tree, stream);
                fprintf(stream, "   jmp :asm_if_lbl%d\n", local_if_cnt + 1);
                fprintf(stream, "   :asm_if_lbl%d\n", local_if_cnt);

                subtree_to_assembly(node->right_child->right_child, tree, stream);
                fprintf(stream, "   :asm_if_lbl%d\n", local_if_cnt + 1);
                fprintf(stream, "# END IF\n\n");
                return 0;
            }
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   :asm_if_lbl%d\n", local_if_cnt);
            fprintf(stream, "# END IF\n\n");
            return 0;

        case TYPE_EQ:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   jne :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_NEQ:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   je :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_GREATER:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   jbe :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_GREATER_EQ:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   jb :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_LESS:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   jae :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_LESS_EQ:
            subtree_to_assembly(node->left_child, tree, stream);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   ja :asm_if_lbl%d\n", if_lbl_cnt);
            return 0;

        case TYPE_LOOP_FIRST:
            while_lbl_cnt += 2;
            local_while_cnt = while_lbl_cnt;
            fprintf(stream, "\n# LOOP\n");
            fprintf(stream, "   :asm_while_lbl%d\n", local_while_cnt);
            fprintf(stream, "   push 0\n");
            subtree_to_assembly(node->left_child, tree, stream);
            fprintf(stream, "   je :asm_while_lbl%d\n", local_while_cnt + 1);
            subtree_to_assembly(node->right_child, tree, stream);
            fprintf(stream, "   jmp :asm_while_lbl%d\n", local_while_cnt);
            fprintf(stream, "   :asm_while_lbl%d\n", local_while_cnt+1);
            fprintf(stream, "# END LOOP\n\n");
            return 0;

        case TYPE_SQRT:
            subtree_to_assembly(node->left_child, tree, stream);
            fprintf(stream, "   sqrt\n");
            return 0;

        case TYPE_ALLNUM:
            fprintf(stream, "   allnum\n");
            return 0;

        case TYPE_NROOTS:
            fprintf(stream, "   nroots\n");
            return 0;

        default:
            return 0;
    }

    return 0;
}


int get_local_vars(token_t* node, ast_tree_t* tree, FILE* stream)
{
    if (!node) return 0;

    get_local_vars(node->right_child, tree, stream);
    get_local_vars(node->left_child, tree, stream);

    if (node->type != TYPE_VAR) return 0;

    for (unsigned i = 0; i < tree->var_count; i++)
        if (!strcmp(node->word, tree->vars[i]))
        {
            fprintf(stream, "   pop [%u]\n", i);
            break;
        }

    return 0;
}


int push_local_vars(token_t* node, ast_tree_t* tree, FILE* stream)
{
    if (!node) return 0;

    push_local_vars(node->left_child, tree, stream);
    push_local_vars(node->right_child, tree, stream);

    if (node->type != TYPE_VAR) return 0;

    for (unsigned i = 0; i < tree->var_count; i++)
        if (!strcmp(node->word, tree->vars[i]))
        {
            fprintf(stream, "   push [%u]\n", i);
            break;
        }

    return 0;
}


int pop_func_params(token_t* node, ast_tree_t* tree, FILE* stream)
{
    if (!node) return 0;

    pop_func_params(node->left_child, tree, stream);
    pop_func_params(node->right_child, tree, stream);

    if (node->type != TYPE_VAR)              return 0;
    if (!strcmp(node->word, VOID_WORD_MIPT)) return 0;

    for (unsigned i = 0; i < tree->var_count; i++)
        if (!strcmp(node->word, tree->vars[i]))
        {
            fprintf(stream, "   pop [%u]\n", i);
            break;
        }

    return 0;
}


const char* get_op_word_from_type(token_type type)
{
    switch (type)
    {
        case OP_ADD: return "   add";
        case OP_SUB: return "   sub";
        case OP_MUL: return "   mul";
        case OP_DIV: return "   div";

        default:
            return nullptr;
    }

    return nullptr;
}

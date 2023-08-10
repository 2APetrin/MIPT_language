#include "source/simplifier/ast_simplify.h"
#include "source/ast_to_assembly/ast_to_assembly.h"
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

    simplify_ast_tree(&ast_tree);

    if (ast_tree.status)
    {
        printf("ERROR in ast tree. Go and see log!\n");
        ast_dtor(&ast_tree);
        fprintf(ast_log_file, "\n</html>\n");
        fclose (ast_log_file);
        system("firefox middleend/logs/log.html");
        return 1;
    }

    ast_tree_print_dump(ast_tree.ast_root);

    ast_to_assembly_converter(&ast_tree);

    ast_dtor(&ast_tree);
    fprintf(ast_log_file, "\n</html>\n");
    fclose (ast_log_file);
    return 0;
}
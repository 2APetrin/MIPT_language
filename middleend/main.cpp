#include "source/create_ast/create_ast.h"

int main(void)
{
    if (read_ast_tree())
    {
        printf("Error.Cannot read ast tree\nProgramm shutdown\n");
        return 1;
    }

    return 0;
}
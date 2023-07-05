#include "../tokenizer/tokenizer.h"
#include "../main_frontend/frontend.h"
#include "log_tree.h"

FILE * graphviz_file;
int graphviz_png_count = 0;

int open_graphviz_file(void)
{
    if ((graphviz_file = fopen("frontend/logs/log_graphviz.dot", "w")) == nullptr)
    {
        printf("Cannot open graphviz file. Programm shutdown\n");
        return 1;
    }

    return 0;
}


int close_graphviz_file(void)
{
    ASSERT(graphviz_file);

    fprintf(graphviz_file, "}");

    fclose(graphviz_file);
    return 0;
}


int init_graphviz_file(void)
{
    ASSERT(graphviz_file);

    fprintf(graphviz_file, "digraph\n{\n");

    return 0;
}


int graphviz_add_node(token_t * node)
{
    fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%s} | {%s} | {line %u} | {pos %u}}\", style = \"filled\", fillcolor = \"#F0FFFF    \"];\n", node, node, node->word, get_typename_from_toktype(node->type), node->line, node->pos);
    return 0;
}


int print_dump(void)
{
    system("dot frontend/logs/log_graphviz.dot -Tpng -o frontend/logs/images/simple_log.png");
    return 0;
}

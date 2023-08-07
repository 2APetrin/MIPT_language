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
    if ((node->type >= TYPE_EQ) && (node->type <= TYPE_LESS_EQ))
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%s} | {line %u} | {pos %u}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, node, get_typename_from_toktype(node->type), node->line, node->pos, get_node_color_from_type(node->type));
        return 0;
    }
    if (node->type == TYPE_DOT)
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%s}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, node, get_typename_from_toktype(node->type), get_node_color_from_type(node->type));
        return 0;
    }
    fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%s} | {%s} | {line %u} | {pos %u}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, node, node->word, get_typename_from_toktype(node->type), node->line, node->pos, get_node_color_from_type(node->type));
    return 0;
}


int print_dump(void)
{
    system("dot frontend/logs/log_graphviz.dot -Tpng -o frontend/logs/images/simple_log.png");
    return 0;
}


int node_print(token_t * node)
{
    if (node == nullptr)
        return 1;

    graphviz_add_node(node);

    if (node->left_child != nullptr)
        node_print(node->left_child);

    if (node->right_child != nullptr)
        node_print(node->right_child);

    return 0;
}


int link_nodes(token_t * node1, token_t * node2)
{
    ASSERT(node1);
    ASSERT(node2);
    ASSERT(graphviz_file);

    if (node1 == node2->parent)      fprintf(graphviz_file, "    node_%p->node_%p [color = \"#000000\"];\n", node2, node1);
    if (node2 == node1->parent)      fprintf(graphviz_file, "    node_%p->node_%p [color = \"#000000\"];\n", node1, node2);

    if (node2 == node1->right_child) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#1164B4\"];\n", node1, node2);
    if (node2 == node1->left_child)  fprintf(graphviz_file, "    node_%p->node_%p [color = \"#E32636\"];\n", node1, node2);

    return 0;
}


int ast_link_nodes(token_t * node1, token_t * node2)
{
    ASSERT(node1);
    ASSERT(node2);
    ASSERT(graphviz_file);

    if (node2 == node1->right_child) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#1164B4\"];\n", node1, node2);
    if (node2 == node1->left_child)  fprintf(graphviz_file, "    node_%p->node_%p [color = \"#E32636\"];\n", node1, node2);

    return 0;
}


int node_link(token_t* node)
{
    if (node == nullptr)
        return 1;

    if (node->left_child != nullptr)
    {
        link_nodes(node, node->left_child);
        node_link(node->left_child);
    }

    if (node->right_child != nullptr)
    {
        link_nodes(node, node->right_child);
        node_link(node->right_child);
    }

    return 0;
}


int ast_node_link(token_t* node)
{
    if (node == nullptr)
        return 1;

    if (node->left_child != nullptr)
    {
        ast_link_nodes(node, node->left_child);
        ast_node_link(node->left_child);
    }

    if (node->right_child != nullptr)
    {
        ast_link_nodes(node, node->right_child);
        ast_node_link(node->right_child);
    }

    return 0;
}


int tree_print_dump(token_t* root)
{
    //printf("-- %d\n", graphviz_png_count);
    if (root == nullptr)
    {
        printf("error in dump tree. root is null\n");
        return 1;
    }

    open_graphviz_file();
    init_graphviz_file();

    node_print(root);
    node_link(root);

    close_graphviz_file();

    char sys_cmd[200] = "dot frontend/logs/log_graphviz.dot -Tpng -o frontend/logs/images/tree_dump";
    snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count);
    system(sys_cmd);

    //fprintf(log_file, "\n<img src=\"images/list_dump%d.png\" width=\"60%%\">\n", graphviz_png_count);

    graphviz_png_count++;

    return 0;
}


unsigned get_node_color_from_type(token_type type)
{
    if ((type >= TYPE_EQ) && (type <= TYPE_LESS_EQ)) return 0xBDECB6;
    if ((type >= OP_ADD) && (type <= OP_DIV))        return 0xE6A8D7;
    if (type == TYPE_DOT) return 0xC6DF90;
    if (type == TYPE_VAR) return 0x9ACEEB;
    if (type == TYPE_NUM) return 0xDDB9E9;

    return 0xF0FFFF;
}


int ast_tree_print_dump(token_t* root)
{
    //printf("-- %d\n", graphviz_png_count);
    if (root == nullptr)
    {
        printf("error in dump tree. root is null\n");
        return 1;
    }

    open_graphviz_file();
    init_graphviz_file();

    ast_node_print(root);
    ast_node_link(root);

    close_graphviz_file();

    char sys_cmd[200] = "dot frontend/logs/log_graphviz.dot -Tpng -o middleend/logs/images/tree_dump";
    snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count + 100);
    system(sys_cmd);

    //fprintf(log_file, "\n<img src=\"images/list_dump%d.png\" width=\"60%%\">\n", graphviz_png_count);

    graphviz_png_count++;

    return 0;
}


int ast_node_print(token_t* node)
{
    if (node == nullptr)
        return 1;

    graphviz_add_ast_node(node);

    if (node->left_child != nullptr)
        ast_node_print(node->left_child);

    if (node->right_child != nullptr)
        ast_node_print(node->right_child);

    return 0;
}


int graphviz_add_ast_node(token_t* node)
{
    token_type type = node->type;
    switch (type)
    {
        case TYPE_NUM:
        {
            fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%lg}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, node->value, get_node_color_from_type(type));
            return 0;
        }

        case TYPE_FUNC_CALL:
        case TYPE_FUNC_ID:
        case TYPE_VAR:
        {
            fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%s}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, node->word, get_node_color_from_type(type));
            return 0;
        }

        default: 
        {
            fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%s}}\", style = \"filled\", fillcolor = \"#%X\"];\n", node, get_typename_from_toktype(type), get_node_color_from_type(type));
            return 0;
        }
    }
    return 0;
}
#include "create_ast.h"
#include "../../../file_work/file_work.h"
#include "../../../frontend/source/tree/tree.h"
#include "../../../frontend/source/tree/log_tree.h"

FILE* ast_log_file;


int read_ast_tree(ast_tree_t* tree)
{
    ast_log_file = fopen("middleend/logs/log.html", "w");
    if (!ast_log_file)
    {
        printf("cannot open log file, programm shutdown\n");
        return 1;
    }
    fprintf(ast_log_file, "<html>\n");

    if (ast_ctor(tree, "temp/ast_tree.ast")) return 1;

    create_ast_tree(tree);
    printf("Tree is ready\n");

    ast_tree_print_dump(tree->ast_root);

    fprintf(ast_log_file, "\n</html>\n");
    fclose (ast_log_file);

    ast_dtor(tree);
    return 0;
}


#define BUFF  tree->ast_buff // tree text buffer
#define LEN   tree->len      // length of tree text buffer
#define TEXT_POS tree->pos


int ast_ctor(ast_tree_t* tree, const char* filename)
{
    ASSERT(tree);
    ASSERT(filename);

    FILE* in_stream = nullptr;
    if (open_read_file(filename, &in_stream)) return 1;

    fseek(in_stream, 0L, SEEK_END);
    LEN = (unsigned) ftell(in_stream);
    rewind(in_stream);

    BUFF = (char*) calloc (LEN + 1, sizeof(char));
    if (!BUFF) return 1;

    fread(BUFF, sizeof(char), LEN, in_stream);
    BUFF[LEN] = '\0';

    fclose(in_stream);

    tree->var_count = 0;
    TEXT_POS        = 0;

    tree->ast_root  = nullptr;
    tree->vars      = nullptr;

    return 0;
}


int ast_dtor(ast_tree_t* tree)
{
    ASSERT(tree);

    free(BUFF);

    return 0;
}


int create_ast_tree(ast_tree_t* tree)
{
    ASSERT(tree);

    tree->ast_root = get_module(tree);

    return 0;
}


token_t* get_module(ast_tree_t* tree)
{
    if (BUFF[TEXT_POS] != '(')
    {
        //fprintf(ast_log_file, "<pre>error in pos %u. module. Expected (, found %c</pre>\n", TEXT_POS + 1, BUFF[TEXT_POS]);
        return nullptr;
    }
    TEXT_POS++;

    int cmd     = 0;
    int cmd_len = 0;

    if (!sscanf(BUFF + TEXT_POS, "%d%n", &cmd, &cmd_len))
        AST_SYNTAX_ERROR("Expected command");

    TEXT_POS += (unsigned) cmd_len;

    switch (cmd)
    {
        case TYPE_DOT:
        {
            token_t* left_node  = get_module(tree);
            if (!left_node) 
                AST_SYNTAX_ERROR("(type dot) Expected (");

            token_t* right_node = get_module(tree);

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type dot) Expected )");

            TEXT_POS++;

            return create_node(TYPE_DOT, POISON, left_node, right_node);
        }

        case TYPE_DECREASE:
        case TYPE_RETURN:
        case TYPE_PRINT:
        {
            //printf("entered type print\n"); 
            token_t* left_node = get_module(tree);
            if (!left_node)
                AST_SYNTAX_ERROR("(type decrease/return/print) Expected (");

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type decrease/return/print) Expected )");

            TEXT_POS++;

            token_t* ret = create_node((token_type) cmd, POISON, left_node, nullptr);

            return ret;
        }

        case TYPE_NUM:
        {
            if (BUFF[TEXT_POS] != ':')
                AST_SYNTAX_ERROR("(type num) Expected :");

            TEXT_POS++;

            elem_t num  = 0;
            int num_len = 0;

            sscanf(BUFF + TEXT_POS, "%lg%n", &num, &num_len);

            if (!num_len)
                AST_SYNTAX_ERROR("(type num) Expected number");

            TEXT_POS += (unsigned) num_len;

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type num) Expected )");

            TEXT_POS++;

            token_t* ret = create_node(TYPE_NUM, num, nullptr, nullptr);

            return ret;
        }

        case TYPE_FUNC_ID: // остановился тут на дописывании случаев для объявления функции и вызова функции. надо сделать чтобы после имени функции возможно шли скобки открывающиеся
        {
            if (BUFF[TEXT_POS] != ':')
                AST_SYNTAX_ERROR("(type def function) Expected :");

            TEXT_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + TEXT_POS, "%127[_a-z]%n", word, &len);
            if (!len)
                AST_SYNTAX_ERROR("(type def function) Expected variable");

            TEXT_POS += (unsigned) len;

            token_t* var_root  = get_module(tree);
            token_t* code_node = get_module(tree);

            token_t* ret    = create_node(TYPE_FUNC_ID, POISON, nullptr, nullptr);
            ret->word       = (char*) calloc (MAX_WORD_LEN, sizeof(char));

            ret->left_child  = var_root;
            ret->right_child = code_node;

            strncpy(ret->word, word, MAX_WORD_LEN-1);

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type def function) Expected )");

            TEXT_POS++;

            return ret;
        }

        case TYPE_FUNC_CALL:
        {
            if (BUFF[TEXT_POS] != ':')
                AST_SYNTAX_ERROR("(type func call) Expected :");

            TEXT_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + TEXT_POS, "%127[_a-z]%n", word, &len);
            if (!len)
                AST_SYNTAX_ERROR("(type func call) Expected variable");

            TEXT_POS += (unsigned) len;

            token_t* var_root  = get_module(tree);

            token_t* ret    = create_node(TYPE_FUNC_CALL, POISON, var_root, nullptr);
            ret->word       = (char*) calloc (MAX_WORD_LEN, sizeof(char));
            strncpy(ret->word, word, MAX_WORD_LEN-1);

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type func call) Expected )");

            TEXT_POS++;

            return ret;
        }

        case TYPE_VAR:
        {
            if (BUFF[TEXT_POS] != ':')
                AST_SYNTAX_ERROR("(type variable) Expected :");

            TEXT_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + TEXT_POS, "%127[_a-z]%n", word, &len);
            if (!len)
                AST_SYNTAX_ERROR("(type variable) Expected variable");

            TEXT_POS += (unsigned) len;

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type variable) Expected )");

            TEXT_POS++;

            token_t* ret = create_node(TYPE_VAR, POISON, nullptr, nullptr);
            ret->word = (char*) calloc (MAX_WORD_LEN, sizeof(char));
            strncpy(ret->word, word, MAX_WORD_LEN-1);

            return ret;
        }

        default:
        {
            token_t* left_node  = get_module(tree);
            if (!left_node)  return nullptr;

            token_t* right_node = get_module(tree);
            if (!right_node) return nullptr;

            if (BUFF[TEXT_POS] != ')')
                AST_SYNTAX_ERROR("(type default) Expected )");

            TEXT_POS++;

            token_t* ret = create_node((token_type) cmd, POISON, left_node, right_node);
            return ret;
        }
    }
    return nullptr;
}
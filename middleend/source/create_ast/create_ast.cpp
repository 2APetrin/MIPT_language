#include "create_ast.h"
#include "../../../file_work/file_work.h"
#include "../../../frontend/source/tree/tree.h"
#include "../../../frontend/source/tree/log_tree.h"

FILE* ast_log_file;

int read_ast_tree(void)
{
    ast_tree_t ast_tree = {0};

    ast_log_file = fopen("middleend/logs/log.html", "w");
    if (!ast_log_file)
    {
        printf("cannot open log file, programm shutdown\n");
        return 1;
    }
    fprintf(ast_log_file, "<html>\n");

    if (ast_ctor(&ast_tree, "temp/ast_tree.ast")) return 1;

    create_ast_tree(&ast_tree);
    printf("Tree is ready\n");

    ast_tree_print_dump(ast_tree.ast_root);

    fprintf(ast_log_file, "\n</html>\n");
    fclose (ast_log_file);

    tree_free(ast_tree.ast_root);

    return 0;
}


#define BUFF  tree->ast_buff // tree text buffer
#define LEN   tree->len      // length of tree text buffer
#define T_POS tree->pos

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
    T_POS       = 0;

    tree->ast_root  = nullptr;
    tree->vars      = nullptr;

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
    if (BUFF[T_POS] != '(')
    {
        //fprintf(ast_log_file, "<pre>error in pos %u. module. Expected (, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
        return nullptr;
    }
    T_POS++;

    int cmd     = 0;
    int cmd_len = 0;

    if (!sscanf(BUFF + T_POS, "%d%n", &cmd, &cmd_len))
    {
        fprintf(ast_log_file, "<pre>error in pos %u. Expected command, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
        return nullptr;
    }

    T_POS += (unsigned) cmd_len;

    switch (cmd)
    {
        case TYPE_DOT:
        {
            token_t* left_node  = get_module(tree);
            if (!left_node)
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ( (left module), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }

            token_t* right_node = get_module(tree);

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ) (right module), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            return create_node(TYPE_DOT, POISON, left_node, right_node);
        }

        case TYPE_DECREASE:
        case TYPE_RETURN:
        case TYPE_PRINT:
        {
            //printf("entered type print\n"); 
            token_t* left_node = get_module(tree);
            if (!left_node)
            {
                fprintf(ast_log_file, "<pre>error in pos %u (print). Expected (, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u (print). Expected ), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            token_t* ret = create_node((token_type) cmd, POISON, left_node, nullptr);

            //printf("print ptr - %p\n", ret);

            return ret;
        }

        case TYPE_NUM:
        {
            if (BUFF[T_POS] != ':')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected :, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            elem_t num  = 0;
            int num_len = 0;

            sscanf(BUFF + T_POS, "%lg%n", &num, &num_len);

            if (!num_len)
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected number, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS += (unsigned) num_len;

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ) (num), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            token_t* ret = create_node(TYPE_NUM, num, nullptr, nullptr);

            return ret;
        }

        case TYPE_FUNC_ID: // остановился тут на дописывании случаев для объявления функции и вызова функции. надо сделать чтобы после имени функции возможно шли скобки открывающиеся
        {
            if (BUFF[T_POS] != ':')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected :, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + T_POS, "%127[_a-z]%n", word, &len);
            if (!len)
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected var, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS += (unsigned) len;

            token_t* var_root  = get_module(tree);
            token_t* code_node = get_module(tree);

            token_t* ret    = create_node(TYPE_FUNC_ID, POISON, nullptr, nullptr);
            ret->word       = (char*) calloc (MAX_WORD_LEN, sizeof(char));

            ret->left_child  = var_root;
            ret->right_child = code_node;

            strncpy(ret->word, word, MAX_WORD_LEN-1);

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ) (var), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            return ret;
        }

        case TYPE_FUNC_CALL:
        {
            if (BUFF[T_POS] != ':')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected :, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + T_POS, "%127[_a-z]%n", word, &len);
            if (!len)
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected var, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS += (unsigned) len;

            token_t* var_root  = get_module(tree);

            token_t* ret    = create_node(TYPE_FUNC_CALL, POISON, var_root, nullptr);
            ret->word       = (char*) calloc (MAX_WORD_LEN, sizeof(char));
            strncpy(ret->word, word, MAX_WORD_LEN-1);

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ) (var), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            return ret;
        }

        case TYPE_VAR:
        {
            if (BUFF[T_POS] != ':')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected :, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            char word[MAX_WORD_LEN] = {0};
            int  len = 0;

            sscanf(BUFF + T_POS, "%127[_a-z]%n", word, &len);
            if (!len)
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected var, found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS += (unsigned) len;

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ) (var), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

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

            if (BUFF[T_POS] != ')')
            {
                fprintf(ast_log_file, "<pre>error in pos %u. Expected ), found %c</pre>\n", T_POS + 1, BUFF[T_POS]);
                return nullptr;
            }
            T_POS++;

            token_t* ret = create_node((token_type) cmd, POISON, left_node, right_node);
            return ret;
        }
    }
    return nullptr;
}



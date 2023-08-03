#include "frontend.h"
#include "../tree/tree.h"
#include "../tree/log_tree.h"


token_t* create_tree_of_tokens(text_t* text)
{
    ASSERT(text);

    init_tokens(TOKEN_BUFF, WORDS_CNT);

    text->tree_root = get_general(text);

    tree_print_dump(text->tree_root);

    printf("Tree is constructed\n");

    return 0;
}


int init_tokens(token_t** tok_buff, unsigned int tok_count)
{
    ASSERT(tok_buff);

    open_graphviz_file();
    init_graphviz_file();

    for (unsigned i = 0; i < tok_count; i++)
    {
        tok_buff[i]->type = get_tok_type(tok_buff[i]->word);
        graphviz_add_node(tok_buff[i]);
    }

    close_graphviz_file();

    print_dump();

    return 0;
}


token_type get_tok_type(const char* word)
{
    ASSERT(word);

    if (number_check(word))               return TYPE_NUM;
    if (!strcmp(word, "born"))            return TYPE_START;
    if (!strcmp(word, "die"))             return TYPE_FINISH;

    if (!strcmp(word, "+"))               return OP_ADD;
    if (!strcmp(word, "-"))               return OP_SUB;
    if (!strcmp(word, "*"))               return OP_MUL;
    if (!strcmp(word, "/"))               return OP_DIV;

    if (!strcmp(word, "."))               return TYPE_DOT;

    if (!strcmp(word, "enter_mipt"))      return TYPE_O_BRCKT;
    if (!strcmp(word, "get_sent_down"))   return TYPE_C_BRCKT;
    if (!strcmp(word, "in_the_ear"))      return TYPE_PRINT_BRCKET;

    if (!strcmp(word, "invent"))          return TYPE_VAR_INIT;
    if (!strcmp(word, "podgon_pod"))      return TYPE_ASSIGNMENT;
    if (!strcmp(word, "when"))            return TYPE_IF;
    if (!strcmp(word, "togda"))           return TYPE_IF_BRCKET;
    if (!strcmp(word, "when_nepravilno")) return TYPE_ELSE;
    if (!strcmp(word, "podgon_pod"))      return TYPE_ASSIGNMENT;

    if (!strcmp(word, "<->"))             return TYPE_EQ;
    if (!strcmp(word, ">"))               return TYPE_GREATER;
    if (!strcmp(word, "->"))              return TYPE_GREATER_EQ;
    if (!strcmp(word, "<"))               return TYPE_LESS;
    if (!strcmp(word, "<-"))              return TYPE_LESS_EQ;

    if (!strcmp(word, "("))               return TYPE_EXPR_O_BR;
    if (!strcmp(word, ")"))               return TYPE_EXPR_C_BR;

    if (!strcmp(word, "pereeb"))          return TYPE_LOOP_FIRST;
    if (!strcmp(word, "solve_nulevka"))   return TYPE_DECREASE;
    if (!strcmp(word, "skill_to"))        return TYPE_FUNC_INIT;

    if (!strcmp(word, "hand_in"))         return TYPE_RETURN;
    if (!strcmp(word, "zadach"))          return TYPE_RETURN_BRCKT;

    if (!strcmp(word, "nulevok_untill_they_run_out")) return TYPE_LOOP_CLOSE;

    if (!strcmp(word, "whisper_to_the_professor"))    return TYPE_PRINT;

    return TYPE_VAR;
}


const char* get_typename_from_toktype(token_type type_num)
{
    switch (type_num)
    {
        case TYPE_NUM:     return "num";
        case OP_ADD:       return "+";
        case OP_SUB:       return "-";
        case OP_MUL:       return "*";
        case OP_DIV:       return "/";

        case TYPE_VAR:     return "var";

        case TYPE_PRINT:   return "print";
        case TYPE_START:   return "start";
        case TYPE_FINISH:  return "finish";
        case TYPE_O_BRCKT: return "begin";
        case TYPE_C_BRCKT: return "close bracket";
        case TYPE_STRING:  return "str";

        case TYPE_DOT:     return "and";

        case TYPE_FUNC_INIT:    return "func_init";
        case TYPE_VAR_INIT:     return "var init";
        case TYPE_PRINT_BRCKET: return "print brcket";
        case TYPE_ASSIGNMENT:   return "=";
        case TYPE_IF:           return "if";
        case TYPE_IF_BRCKET:    return "togda";
        case TYPE_ELSE:         return "else";

        case TYPE_EQ:           return "==";
        case TYPE_GREATER:      return "greater";
        case TYPE_GREATER_EQ:   return "gr or eq";
        case TYPE_LESS:         return "less";
        case TYPE_LESS_EQ:      return "less or eq";

        case TYPE_EXPR_O_BR:    return "(";
        case TYPE_EXPR_C_BR:    return ")";

        case TYPE_LOOP_FIRST:   return "while";
        case TYPE_LOOP_CLOSE:   return "while_close_br";

        case TYPE_DECREASE:     return "--";
        case TYPE_FUNC_ID:      return "func id";

        case TYPE_RETURN:       return "return";
        case TYPE_RETURN_BRCKT: return "ret bracket";

        default: return nullptr;
    }
    return nullptr;
}

int number_check(const char* word)
{
    ASSERT(word);

    unsigned i = 0;
    while (word[i])
    {
        if (!isdigit(word[i])) return 0;
        i++;
    }

    return 1;
}


int write_tree_preorder(text_t* text)
{
    ASSERT(text);

    // закончил тут. Написать открытие файла и запись в него дерева

    write_subtree_preorder(text->tree_root, stream);
}

int print_pre_order(node_t * node, FILE * out_stream)
{
    fprintf(out_stream, "(");

    if (!node->type)
        fprintf(out_stream, "%lg", node->value);
    
    else if (node->type != TYPE_VAR)
    {
        fprintf(out_stream, "%s", get_type(node->type));
    }

    else
    {
        fprintf(out_stream, "%c", (int)node->value);
    }

    if (node->left_child != nullptr)
    {
        print_pre_order(node->left_child, out_stream);
    }

    if (node->right_child != nullptr)
    {
        print_pre_order(node->right_child, out_stream);
    }

    fprintf(out_stream, ")");

    return 0;
}
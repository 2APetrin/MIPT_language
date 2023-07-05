#include "frontend.h"
#include "../tree/tree.h"
#include "../tree/log_tree.h"

int create_tree_of_tokens(text_t* text)
{
    ASSERT(text);


    init_tokens(TOKEN_BUFF, WORDS_CNT);

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

    if (number_check(word))             return TYPE_NUM;
    if (!strcmp(word, "born"))          return TYPE_START;
    if (!strcmp(word, "die"))           return TYPE_FINISH;

    if (!strcmp(word, "."))             return TYPE_DOT;

    if (!strcmp(word, "enter_mipt"))    return TYPE_O_BRCKT;
    if (!strcmp(word, "get_sent_down")) return TYPE_C_BRCKT;
    if (!strcmp(word, "in_the_ear"))    return TYPE_PRINT_BRCKET;

    if (!strcmp(word, "whisper_to_the_professor")) return TYPE_PRINT;

    return TYPE_VAR;
}


const char* get_typename_from_toktype(token_type type_num)
{
    switch (type_num)
    {
        case TYPE_NUM:
            return "num";
        case OP_ADD:
            return "+";
        case OP_SUB:
            return "-";
        case OP_MUL:
            return "*";
        case OP_DIV:
            return "/";
        case TYPE_VAR:
            return "var";
        case TYPE_FUNC:
            return "func";
        case TYPE_PRINT:
            return "print";
        case TYPE_START:
            return "start";
        case TYPE_FINISH:
            return "finish";
        case TYPE_O_BRCKT:
            return "begin";
        case TYPE_C_BRCKT:
            return "end";
        case TYPE_DOT:
            return "dot";
        case TYPE_PRINT_BRCKET:
            return "print brcket";
        default:
            return nullptr;
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

#include "source/tokenizer/tokenizer.h"
#include "source/main_frontend/frontend.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("codefile name is not given\n");
        return 1;
    }

    text_t text = {0};
    if (text_ctor(argv[1], &text)) return 1;

    create_tree_of_tokens(&text);

    write_tree_preorder(&text);

    text_dtor(&text);
    return 0;
}
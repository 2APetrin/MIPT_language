#include "tokenizer.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("codefile name is not given\n");
        return 1;
    }

    text_t text = {0};
    text_ctor(argv[1], &text);

    text_dtor(&text);
    return 0;
}
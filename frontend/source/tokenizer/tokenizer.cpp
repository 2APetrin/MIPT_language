#include "tokenizer.h"
#include "../../../file_work/file_work.h"
#include "../tree/tree.h"

FILE* code_file = nullptr;

int text_ctor(const char* codefile_name, text_t* text)
{
    ASSERT(codefile_name);
    ASSERT(text);

    TEXT_BUFF  = nullptr;
    TEXT_LINES = nullptr;
    TOKEN_BUFF = nullptr;
    VAR_BUFF   = nullptr;
    text->end  = nullptr;
    VAR_CNT    = 0;
    POS        = 0;
    text->tree_root = nullptr;

    if (open_read_file(codefile_name, &code_file)) return 1;

    fseek(code_file, 0L, SEEK_END);
    TEXT_LEN = (unsigned) ftell(code_file);
    rewind(code_file);

    TEXT_BUFF = (char *) calloc(TEXT_LEN + 1, sizeof(char));
    if (!TEXT_BUFF) return 1;

    fread(TEXT_BUFF, sizeof(char), TEXT_LEN, code_file);
    TEXT_BUFF[TEXT_LEN] = '\0';

    get_num_of_lines(text);

    TEXT_LINES = (char**) calloc(LINES_CNT, sizeof(char*));
    if (!TEXT_LINES) return 1;

    get_strings(text);
    get_num_of_words(text);

    TOKEN_BUFF = (token_t**) calloc(WORDS_CNT, sizeof(token_t*));
    if (!TOKEN_BUFF) return 1;

    if (get_tokens(text))
    {
        printf("Error in tokenization\nnothing happened\n");
        return 1;
    }

    VAR_BUFF     = (var_t**)   calloc(MAX_VAR_COUNT, sizeof(var_t*));
    FUNC_BUFF    = (func_t**)  calloc(MAX_FUNC_CNT,  sizeof(func_t*));
    DOTS_BUFF    = (token_t**) calloc(MAX_DOT_COUNT, sizeof(token_t*));
    DOTS_CNT     = 0;
    text->status = 0;

    return 0;
}


void text_dtor(text_t* text)
{
    ASSERT(text);

    free(TEXT_BUFF);
    TEXT_BUFF  = nullptr;
    free(TEXT_LINES);
    TEXT_LINES = nullptr;

    if (text->status)
    {
        printf ("ERROR in code. Nothing happened. Go see frontend_log!\n");

        system("firefox frontend/logs/frontend_log.html");

        for (unsigned i = 0; i < DOTS_CNT; i++)
        {
            free(DOTS_BUFF[i]);
            DOTS_BUFF[i] = nullptr;
        }

        for (unsigned i = 0; i < WORDS_CNT; i++)
        {
            if (TOKEN_BUFF[i])
            {
                free (TOKEN_BUFF[i]->word);
                TOKEN_BUFF[i]->word = nullptr;
            }
            free(TOKEN_BUFF[i]);
            TOKEN_BUFF[i] = nullptr;
        }
    }
    else
        tree_free(text->tree_root);

    text->tree_root = nullptr;

    free(TOKEN_BUFF);
    TOKEN_BUFF = nullptr;

    for (unsigned i = 0; i < FUNC_CNT; i++)
    {
        free(FUNC_BUFF[i]->name);
        FUNC_BUFF[i]->name = nullptr;

        free(FUNC_BUFF[i]);
        FUNC_BUFF[i] = nullptr;
    }

    free(FUNC_BUFF);
    FUNC_BUFF = nullptr;

    for (unsigned i = 0; i < VAR_CNT; i++)
    {
        free(VAR_BUFF[i]->name);
        VAR_BUFF[i]->name = nullptr;

        free(VAR_BUFF[i]);
        VAR_BUFF[i] = nullptr;
    }
    free(VAR_BUFF);
    VAR_BUFF = nullptr;

    free(DOTS_BUFF);
    DOTS_BUFF = nullptr;

    printf("Local codetree is destructed\n\n");
}


int get_tokens(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);

    POS = 0;

    for (unsigned int i = 0; i < LINES_CNT; i++)
        if (tokenize_line(text, i)) return 1;

    return 0;
}


int tokenize_line(text_t* text, unsigned int i)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);
    ASSERT(TEXT_LINES);

    POS = 0;

    while (*(TEXT_LINES[i] + POS) != 0)
    {
        skip_blanks(text, i);
        if (get_word(text, i)) return 1;
    }
    return 0;
}


int get_word(text_t* text, unsigned int i)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);
    ASSERT(TEXT_LINES);

    static unsigned int word_number = 0;

    if (*(TEXT_LINES[i] + POS) == 0) return 0;

    unsigned initial_pos = POS;

    while (isgraph(*(TEXT_LINES[i] + POS)) && *(TEXT_LINES[i] + POS) && *(TEXT_LINES[i] + POS) != '$')
        POS++;

    unsigned len = POS - initial_pos;
    if (len > MAX_WORD_LEN)
    {
        printf("word is too long in line %u, pos %u\n", i, initial_pos);
        return 1;
    }

    if (len)
    {
        TOKEN_BUFF[word_number]       = (token_t*) calloc(1, sizeof(token_t));
        TOKEN_BUFF[word_number]->word = (char*) calloc(MAX_WORD_LEN+1, sizeof(char));
        TOKEN_BUFF[word_number]->line = i+1;
        TOKEN_BUFF[word_number]->pos  = initial_pos+1;
        TOKEN_BUFF[word_number]->word = strncpy(TOKEN_BUFF[word_number]->word, (TEXT_LINES[i] + initial_pos), len);

        TOKEN_BUFF[word_number]->left_child = nullptr;
        TOKEN_BUFF[word_number]->right_child = nullptr;

        word_number++;
    }

    if (*(TEXT_LINES[i] + POS) == '$')
    {
        len = 1;
        initial_pos = POS;

        POS++;

        TOKEN_BUFF[word_number]       = (token_t*) calloc(1, sizeof(token_t));
        TOKEN_BUFF[word_number]->word = (char*) calloc(MAX_WORD_LEN+1, sizeof(char));
        TOKEN_BUFF[word_number]->line = i+1;
        TOKEN_BUFF[word_number]->pos  = initial_pos+1;
        TOKEN_BUFF[word_number]->word = strncpy(TOKEN_BUFF[word_number]->word, (TEXT_LINES[i] + initial_pos), len);

        TOKEN_BUFF[word_number]->left_child = nullptr;
        TOKEN_BUFF[word_number]->right_child = nullptr;

        word_number++;
    }
    return 0;
}


int get_num_of_words(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);

    for (unsigned i = 0; i < TEXT_LEN; i++)
    {
        if (isgraph(TEXT_BUFF[i]) && (!isgraph(TEXT_BUFF[i+1]) || TEXT_BUFF[i+1] == '$')) WORDS_CNT++;
        if (TEXT_BUFF[i] == '$' && isgraph(TEXT_BUFF[i+1])) WORDS_CNT++;
    }

    return 0;
}


int get_strings(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);
    POS = 0;

    for (unsigned i = 0; i < LINES_CNT; i++)
    {
        TEXT_LINES[i] = TEXT_BUFF + POS;
        while (TEXT_BUFF[POS] != 0 && POS < TEXT_LEN) POS++;
        POS++;
    }
    return 0;
}


int skip_blanks(text_t* text, unsigned int i)
{
    ASSERT(text);

    while ((!isgraph(*(TEXT_LINES[i] + POS)) || *(TEXT_LINES[i] + POS) == '\n') && *(TEXT_LINES[i] + POS))
    {
        POS++;
    }
    return 0;
}


int get_num_of_lines(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);

    for (unsigned i = 0; i < TEXT_LEN; i++) 
    {
        if (TEXT_BUFF[i] == '\n')
        {
            LINES_CNT++;
            TEXT_BUFF[i] = 0;
        }
    }
    LINES_CNT++;
    return 0;
}

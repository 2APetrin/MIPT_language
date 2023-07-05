#include "tokenizer.h"
#include "../../../file_work/file_work.h"

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
    text->tree_root = nullptr;
    text->var_cnt   = 0;
    POS             = 0;

    if (open_code_file(codefile_name, &code_file)) return 1;

    fseek(code_file, 0L, SEEK_END);
    TEXT_LEN = (unsigned) ftell(code_file);
    rewind(code_file);

    TEXT_BUFF = (char *) calloc(TEXT_LEN + 1, sizeof(char));
    if (!TEXT_BUFF) return 1;

    fread(TEXT_BUFF, sizeof(char), TEXT_LEN, code_file);
    TEXT_BUFF[TEXT_LEN] = '\0';

    get_num_of_lines(text);   // puts 0 instead of \n
    printf("num of lines %u\n", LINES_CNT);

    TEXT_LINES = (char**) calloc(LINES_CNT, sizeof(char*));
    if (!TEXT_LINES) return 1;

    get_strings(text);
    //for (unsigned i = 0; i < LINES_CNT; i++) printf("%s\n", TEXT_LINES[i]);   // debug print

    get_num_of_words(text);
    printf("num of words %u\n\n", WORDS_CNT);
    TOKEN_BUFF = (token_t**) calloc(WORDS_CNT, sizeof(token_t*));
    if (!TOKEN_BUFF) return 1;

    if (get_tokens(text))
    {
        // dtor
        printf("Error in assembly\nnothing happened\n");
        return 1;
    }

    //for (unsigned i = 0; i < WORDS_CNT; i++) printf("%s\nline - %u\npos - %u\n\n", TOKEN_BUFF[i]->word, TOKEN_BUFF[i]->line, TOKEN_BUFF[i]->pos);

    return 0;
}


int get_tokens(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);

    POS = 0;

    for (unsigned int i = 0; i < LINES_CNT; i++)
    {
        if (tokenize_line(text, i)) return 1;
    }
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

    //printf("cock%u\n", i);
    //printf("\n%s\n", TEXT_LINES[i] + POS);

    if (*(TEXT_LINES[i] + POS) == 0) return 0;

    unsigned initial_pos = POS;

    while (isgraph(*(TEXT_LINES[i] + POS)) && *(TEXT_LINES[i] + POS) && *(TEXT_LINES[i] + POS) != '.')
    {
        //printf("%c", *(TEXT_LINES[i] + POS));
        POS++;
    }

    unsigned len = POS - initial_pos;
    if (len > MAX_WORD_LEN)
    {
        printf("word is too long in line %u, pos %u\n", i, initial_pos);
        return 1;
    }


    if (len)
    {
        //printf("word number - %u\n", word_number);
        TOKEN_BUFF[word_number]       = (token_t*) calloc(1, sizeof(token_t));
        TOKEN_BUFF[word_number]->word = (char*) calloc(MAX_WORD_LEN+1, sizeof(char));
        TOKEN_BUFF[word_number]->line = i+1;
        TOKEN_BUFF[word_number]->pos  = initial_pos+1;
        TOKEN_BUFF[word_number]->word = strncpy(TOKEN_BUFF[word_number]->word, (TEXT_LINES[i] + initial_pos), len);
        word_number++;
    }

    if (*(TEXT_LINES[i] + POS) == '.')
    {
        len = 1;
        initial_pos = POS;

        //printf("\n%s\n", TEXT_LINES[i] + POS);
        //printf("word number - %u\n", word_number);
        POS++;

        TOKEN_BUFF[word_number]       = (token_t*) calloc(1, sizeof(token_t));
        TOKEN_BUFF[word_number]->word = (char*) calloc(MAX_WORD_LEN+1, sizeof(char));
        TOKEN_BUFF[word_number]->line = i+1;
        TOKEN_BUFF[word_number]->pos  = initial_pos+1;
        TOKEN_BUFF[word_number]->word = strncpy(TOKEN_BUFF[word_number]->word, (TEXT_LINES[i] + initial_pos), len);
        word_number++;
    }
    return 0;
}


int get_num_of_words(text_t* text)
{
    ASSERT(text);
    ASSERT(TEXT_BUFF);

    //printf("%u\n", TEXT_LEN);
    for (unsigned i = 0; i < TEXT_LEN; i++)
    {
        //printf("%u\n", i);
        if (isgraph(TEXT_BUFF[i]) && (!isgraph(TEXT_BUFF[i+1]) || TEXT_BUFF[i+1] == '.')) WORDS_CNT++;
        if (TEXT_BUFF[i] == '.' && isgraph(TEXT_BUFF[i+1])) WORDS_CNT++;
    }
    //printf("wc - %u\n", WORDS_CNT);

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


int text_dtor(text_t* text)
{
    ASSERT(text);

    free(TEXT_BUFF);
    free(TEXT_LINES);
    free(TOKEN_BUFF);
    return 0;
}



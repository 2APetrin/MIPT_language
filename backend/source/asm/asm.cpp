#include "asm.h"

#define POS        assembly->pos

#define TEXT       assembly->prog_text
#define TEXT_BUFF  assembly->prog_text.prog_buff
#define TEXT_LEN assembly->prog_text.text_len

#define LINES_BUFF assembly->prog_text.lines_buff
#define LINES_CNT  assembly->prog_text.lines_cnt

#define TOKEN_BUFF assembly->token_buff
#define TOKEN_CNT  assembly->token_cnt

#define LABELS_BUFF assembly->label_buff
#define LABELS_CNT  assembly->label_cnt


int run_asm(FILE* in_stream, FILE* out_stream)
{
    asm_t* assembly = (asm_t*) calloc(1, sizeof(asm_t));
    asm_ctor(assembly, in_stream);

    return 0;
}


int asm_ctor(asm_t* assembly, FILE* stream)
{
    ASSERT(assembly);
    ASSERT(stream);

    fseek(stream, 0L, SEEK_END);
    TEXT.text_len = (unsigned) ftell(stream);
    rewind(stream);

    TEXT_BUFF = (char *) calloc(TEXT.text_len + 1, sizeof(char));
    fread(TEXT_BUFF, sizeof(char), TEXT.text_len, stream);
    fclose(stream);

    POS = 0;
    get_lines(assembly);

    /* for (unsigned i = 0; i < LINES_CNT; i++)
        printf("%u - %s\n", i, LINES_BUFF[i]); */

    get_num_of_tokens(assembly);

    TOKEN_BUFF = (token_t*) calloc(TOKEN_CNT, sizeof(token_t));

    get_tokens(assembly);

    for (unsigned i = 0; i < TOKEN_CNT; i++)
        printf("tok_word: %s\nline: %u\npos: %u\n\n", TOKEN_BUFF[i].word, TOKEN_BUFF[i].line, TOKEN_BUFF[i].inline_pos);

    init_tokens(assembly);

    assembly->status    = 0;
    assembly->minus_cnt = 0;
    return 0;
}


int get_lines(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(TEXT_BUFF);

    for (unsigned i = 0; i < TEXT_LEN; i++)
        if (TEXT_BUFF[i] == '\n') LINES_CNT++;

    LINES_CNT++;

    LINES_BUFF = (char**) calloc(LINES_CNT, sizeof(char*));

    LINES_BUFF[0] = TEXT_BUFF;
    POS = 1;
    for (unsigned i = 0; i < TEXT_LEN; i++)
    {
        if (TEXT_BUFF[i] == '\n')
        {
            TEXT_BUFF[i] = 0;

            LINES_BUFF[POS] = TEXT_BUFF + i + 1;
            POS++;
        }
    }
    POS = 0;

    return 0;
}


int get_num_of_tokens(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(TEXT_BUFF);

    unsigned i = 0;
    if (isgraph(TEXT_BUFF[i]))
    {
        TOKEN_CNT++;
        i++;
    }

    for (; i < TEXT_LEN; i++)
        if (!isgraph(TEXT_BUFF[i]) && isgraph(TEXT_BUFF[i+1])) TOKEN_CNT++;

    return 0;
}


int get_tokens(asm_t* assembly)
{
    ASSERT(assembly);

    POS = 0;
    for (unsigned line_num = 0; line_num < LINES_CNT; line_num++)
        tokenize_line(assembly, line_num);

    return 0;
}


int tokenize_line(asm_t* assembly, unsigned line_num)
{
    ASSERT(TOKEN_BUFF);
    ASSERT(LINES_BUFF);
    ASSERT(TEXT_BUFF);

    //printf("%u\n", line_num);

    char*    base = LINES_BUFF[line_num];
    unsigned inline_pos = 0;

    while (*(base + inline_pos))
    {
        //printf("%s", (base + inline_pos));
        skip_blanks(base, &inline_pos);
        if (get_word(assembly, line_num, &inline_pos)) return 1; // недописал получение слов в токены.
    }
    return 0;
}


int get_word(asm_t* assembly, unsigned line_num, unsigned* inline_pos)
{
    ASSERT(inline_pos);

    char* base = LINES_BUFF[line_num];

    unsigned beg_pos = *inline_pos;
    while (isgraph(*(base + *inline_pos))) (*inline_pos)++;

    if ((*inline_pos - beg_pos) >= MAX_WORD_LEN)
        return 1;

    if (*inline_pos == beg_pos)
        return 0;

    TOKEN_BUFF[POS].word = (char*) calloc((*inline_pos - beg_pos) + 1, sizeof(char));
    strncpy(TOKEN_BUFF[POS].word, base + beg_pos, (*inline_pos - beg_pos));

    TOKEN_BUFF[POS].line       = line_num + 1;
    TOKEN_BUFF[POS].inline_pos = beg_pos + 1;
    POS++;

    return 0;
}


int skip_blanks(char* base, unsigned* inline_pos)
{
    ASSERT(base);
    ASSERT(inline_pos);

    while (*(base + *inline_pos) && !isgraph(*(base + *inline_pos))) (*inline_pos)++;
    return 0;
}


int init_tokens(asm_t* assembly)
{
    ASSERT(assembly);

    POS = 0;
    for (; POS < TOKEN_CNT; POS++) single_token_init(assembly);
}


int single_token_init(asm_t* assembly)
{
    ASSERT(assembly);

    char* word = TOKEN_BUFF[POS].word;

    if (is_label(word))
    {
        TOKEN_BUFF[POS].type = TYPE_LABEL;

        int flag = 1;
        for (unsigned i = 0; i < LABELS_CNT; i++)
            if (!strcmp(word, LABELS_BUFF[i].name)) flag = 0;

        if (flag)
        {
            LABELS_BUFF[LABELS_CNT].name = (char*) calloc(MAX_WORD_LEN, sizeof(char));
            strncpy(LABELS_BUFF[LABELS_CNT].name, word + 1, MAX_WORD_LEN-1);
            LABELS_CNT++;

            return 0;
        }
    }

    if (is_num(word))
    {
        TOKEN_BUFF[POS].type = TYPE_NUM;

        sscanf(word, "%lg", &TOKEN_BUFF[POS].val);

        return 0;
    }

    if (is_ram_ptr(word))
    {
        TOKEN_BUFF[POS].type = RAM_PTR;

        sscanf(word + 1, "%lg", &TOKEN_BUFF[POS].val);

        return 0;
    }

    TOKEN_BUFF[POS].type = get_type(word);
    TOKEN_BUFF[POS].cmd  = get_cmd(word);

    return 0; // дописать тут функции и обработку ошибок
}


int is_num(char* word)
{
    ASSERT(word);
    int i = 0;

    if (word[i] == '-') i++;

    for (; word[i]; i++)
        if (!isdigit(word[i])) return 0;

    return 1;
}


int is_label(char* word)
{
    ASSERT(word);

    int i = 0;
    if (word[i++] != ':') return 0;

    for (; word[i]; i++)
        if (!isalpha(word[i]) && word[i] != '_') return 0;

    return 1;
}
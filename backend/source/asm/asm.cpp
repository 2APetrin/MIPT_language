#include "asm.h"

#define POS       assembly->pos
#define MINUS_CNT assembly->minus_cnt

#define TEXT      assembly->prog_text
#define TEXT_BUFF assembly->prog_text.prog_buff
#define TEXT_LEN  assembly->prog_text.text_len

#define LINES_BUFF assembly->prog_text.lines_buff
#define LINES_CNT  assembly->prog_text.lines_cnt

#define TOKEN_BUFF assembly->token_buff
#define TOKEN_CNT  assembly->token_cnt

#define LABELS_BUFF assembly->label_buff
#define LABELS_CNT  assembly->label_cnt

#define OUT_BUFF     assembly->out_buff
#define OUT_BUFF_LEN assembly->out_len

#define COMM_LINES assembly->commented_lines


#define ASM_SYNTAX_ERROR(str, err)                                              \
{                                                                                \
    printf(str);                                                                  \
    printf("\nline:%u\npos:%u\n\n", TOKEN_BUFF[i].line, TOKEN_BUFF[i].inline_pos); \
    assembly->status |= err;                                                        \
}


int run_asm(FILE* in_stream, FILE* out_stream)
{
    ASSERT(in_stream);
    ASSERT(out_stream);

    asm_t* assembly = (asm_t*) calloc(1, sizeof(asm_t));
    asm_ctor(assembly, in_stream);

    tokens_check(assembly);

    write_exe_array(assembly, out_stream);

    asm_dtor(assembly);

    return 0;
}


int asm_dtor(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(!assembly->status);

    for (unsigned i = 0; i < LABELS_CNT; i++)
    {
        free(LABELS_BUFF[i].name);
        LABELS_BUFF[i].name = nullptr;
    }

    free(LABELS_BUFF);
    LABELS_BUFF = nullptr;

    free(OUT_BUFF);
    OUT_BUFF = nullptr;

    free(TEXT_BUFF);
    TEXT_BUFF = nullptr;

    for (unsigned i = 0; i < TOKEN_CNT; i++)
    {
        free(TOKEN_BUFF[i].word);
        TOKEN_BUFF[i].word = nullptr;
    }
    free(TOKEN_BUFF);
    TOKEN_BUFF = nullptr;

    free(LINES_BUFF);
    LINES_BUFF = nullptr;

    free(COMM_LINES);
    COMM_LINES = nullptr;

    free(assembly);

    return 0;
}


int write_exe_array(asm_t* assembly, FILE* out_stream)
{
    ASSERT(assembly);
    ASSERT(out_stream);
    if (assembly->status)
    {
        fclose(out_stream);
        ASSERT(!assembly->status);
    }

    OUT_BUFF = (elem_t*) calloc (OUT_BUFF_LEN, sizeof(elem_t));

    unsigned i = 0;
    token_type type = TYPE_CMD;
    for (POS = 0; POS < TOKEN_CNT; POS++)
    {
        type = TOKEN_BUFF[POS].type;

        if (COMM_LINES[TOKEN_BUFF[POS].line]) continue;
        if (type == TYPE_LABEL) continue;

        if (type == TYPE_NUM || type == TYPE_JMP_LABEL || type == RAM_PTR)
        {
            OUT_BUFF[i++] = TOKEN_BUFF[POS].val;
            continue;
        }

        OUT_BUFF[i++] = (elem_t) TOKEN_BUFF[POS].cmd;
    }

    /* printf("Debug:\n");
    for (unsigned m = 0; m < OUT_BUFF_LEN; m++) printf("%3u ", m);
    printf("\n");
    for (unsigned m = 0; m < OUT_BUFF_LEN; m++)
        printf("%3lg ", OUT_BUFF[m]);
    printf("\n"); */

    fwrite(OUT_BUFF, sizeof(elem_t), OUT_BUFF_LEN, out_stream);
    fclose(out_stream);
    printf("ASM code is written\n");
    printf("Compilation OK\n");


    return 0;
}


int tokens_check(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(!assembly->status);

    unsigned i = 0;
    cmd_codes  curr_cmd  = CMD_ERROR;
    token_type next_type = TYPE_CMD;
    token_type curr_type = TYPE_CMD;

    for (; i < TOKEN_CNT-1; i++)
    {
        curr_cmd  = TOKEN_BUFF[i].cmd;
        curr_type = TOKEN_BUFF[i].type;
        next_type = TOKEN_BUFF[i+1].type;

        if (COMM_LINES[TOKEN_BUFF[i].line]) continue;

        if (((curr_type >= TYPE_NUM && curr_type <= RAM_PTR) || curr_type == TYPE_JMP_LABEL) && next_type != TYPE_CMD && next_type != TYPE_LABEL && next_type != TYPE_COMMENT)
            ASM_SYNTAX_ERROR("ERROR. next node doesn't fit", BAD_NEXT_NODE);

        if (curr_cmd >= CMD_JMP && curr_cmd <= CMD_JMP_NE && next_type != TYPE_JMP_LABEL)
            ASM_SYNTAX_ERROR("ERROR in jump. no label after jump", JMP_WTHT_LABEL);
    }

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

    COMM_LINES = (unsigned*) calloc(MAX_LINES_CNT, sizeof(unsigned));

    POS = 0;
    get_lines(assembly);

    get_num_of_tokens(assembly);

    TOKEN_BUFF  = (token_t*) calloc(TOKEN_CNT, sizeof(token_t));
    LABELS_BUFF = (label_t*) calloc(MAX_LABELS_CNT, sizeof(label_t));

    get_tokens(assembly);

    init_tokens(assembly);

    if (assembly->status) return 1;
    init_labels(assembly);
    second_init_labels(assembly);
    if (assembly->status) return 1;

    init_push_pop(assembly);
    if (assembly->status) return 1;

    /* for (unsigned i = 0; i < TOKEN_CNT; i++)
       printf("tok_word: %s\nline: %u\npos: %u\ntype: %d\nval: %lg\ncmd: %d\n\n", TOKEN_BUFF[i].word, TOKEN_BUFF[i].line, TOKEN_BUFF[i].inline_pos, TOKEN_BUFF[i].type, TOKEN_BUFF[i].val, TOKEN_BUFF[i].cmd);

    for (unsigned i = 0; i < LABELS_CNT; i++)
        printf("%u) %s\npos: %u\n", i, LABELS_BUFF[i].name, LABELS_BUFF[i].pos);

    for (unsigned i = 0; i < MAX_LINES_CNT; i++)
        if (COMM_LINES[i])
            printf("commented - %u\n", i); */

    OUT_BUFF_LEN = TOKEN_CNT - MINUS_CNT;

    assembly->status    = 0;
    assembly->minus_cnt = 0;

    printf("ASM code is constructed\n");

    return 0;
}


int init_push_pop(asm_t* assembly)
{
    ASSERT(assembly);

    for (unsigned i = 0; i < TOKEN_CNT; i++)
    {
        if (TOKEN_BUFF[i].cmd == CMD_PUSH)
        {
            if (i+1 >= TOKEN_CNT)
            ASM_SYNTAX_ERROR("ERROR in last token. push without argument", BAD_PUSH_ARG);

            token_type next = TOKEN_BUFF[i+1].type;

            if (next == TYPE_NUM) continue;

            if (next == TYPE_REG)
            {
                TOKEN_BUFF[i].cmd = CMD_PUSH_REG;
                continue;
            }

            if (next == RAM_PTR)
            {
                TOKEN_BUFF[i].cmd = CMD_PUSH_RAM;
                continue;
            }

            ASM_SYNTAX_ERROR("ERROR in push. Bad token type after push", BAD_PUSH_ARG);
        }

        if (TOKEN_BUFF[i].cmd == CMD_POP)
        {
            token_type next = TYPE_CMD;
            if (i+1 < TOKEN_CNT) next = TOKEN_BUFF[i+1].type;

            if (next == TYPE_REG)
            {
                TOKEN_BUFF[i].cmd = CMD_POP_REG;
                continue;
            }

            if (next == RAM_PTR)
            {
                TOKEN_BUFF[i].cmd = CMD_POP_RAM;
                continue;
            }
        }
    }

    return 0;
}


int init_labels(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(LABELS_BUFF);
    ASSERT(TOKEN_BUFF);

    POS = 0;
    MINUS_CNT = 0;
    if (TOKEN_BUFF[POS].type == TYPE_LABEL)
    {
        for (int i = 0; i < (int) LABELS_CNT; i++)
            if (!strcmp(TOKEN_BUFF[POS].word + 1, LABELS_BUFF[i].name))
            {
                LABELS_BUFF[i].pos = POS;
                LABELS_BUFF[i].init_cnt++;
                MINUS_CNT++;
                break;
            }
    }

    if (TOKEN_BUFF[POS].type == TYPE_COMMENT)
        MINUS_CNT++;
    POS++;

    for (; POS < TOKEN_CNT; POS++)
    {
        if (COMM_LINES[TOKEN_BUFF[POS].line])
        {
            MINUS_CNT++;
            continue;
        }

        if (is_init_label(assembly))
        {
            for (unsigned i = 0; i < LABELS_CNT; i++)
                if (!strcmp(TOKEN_BUFF[POS].word + 1, LABELS_BUFF[i].name))
                {
                    LABELS_BUFF[i].pos = POS - MINUS_CNT;
                    LABELS_BUFF[i].init_cnt++;
                    MINUS_CNT++;
                    break;
                }
            continue;
        }

        if (is_jmp_label(assembly))
        {
            TOKEN_BUFF[POS].type = TYPE_JMP_LABEL;
            for (unsigned i = 0; i < LABELS_CNT; i++)
                if (!strcmp(TOKEN_BUFF[POS].word + 1, LABELS_BUFF[i].name))
                {
                    TOKEN_BUFF[POS].val = (elem_t) LABELS_BUFF[i].pos;
                    break;
                }
            continue;
        }
    }

    return 0;
}


int second_init_labels(asm_t* assembly)
{
    ASSERT(assembly);
    ASSERT(LABELS_BUFF);
    ASSERT(TOKEN_BUFF);

    for (POS = 0; POS < TOKEN_CNT; POS++)
    {
        if (TOKEN_BUFF[POS].type == TYPE_JMP_LABEL)
        {
            for (unsigned i = 0; i < LABELS_CNT; i++)
                if (!strcmp(TOKEN_BUFF[POS].word + 1, LABELS_BUFF[i].name))
                {
                    TOKEN_BUFF[POS].val = (elem_t) LABELS_BUFF[i].pos;
                    break;
                }
            continue;
        }
    }

    return 0;
}


int is_jmp_label(asm_t* assembly)
{
    ASSERT(assembly);

    if (TOKEN_BUFF[POS].type != TYPE_LABEL) return 0;

    if (TOKEN_BUFF[POS-1].type == TYPE_CMD && TOKEN_BUFF[POS-1].cmd >= CMD_JMP && TOKEN_BUFF[POS-1].cmd <= CMD_CALL)
        return 1;
    return 0;
}


int is_init_label(asm_t* assembly)
{
    ASSERT(assembly);

    if (TOKEN_BUFF[POS].type != TYPE_LABEL) return 0;

    if (TOKEN_BUFF[POS-1].type == TYPE_CMD && TOKEN_BUFF[POS-1].cmd >= CMD_JMP && TOKEN_BUFF[POS-1].cmd <= CMD_CALL)
        return 0;
    return 1;
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

    char*    base = LINES_BUFF[line_num];
    unsigned inline_pos = 0;

    while (*(base + inline_pos))
    {
        skip_blanks(base, &inline_pos);
        if (get_word(assembly, line_num, &inline_pos)) return 1;
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

    return 0;
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
            if (!strcmp(word + 1, LABELS_BUFF[i].name)) flag = 0;

        if (flag)
        {
            LABELS_BUFF[LABELS_CNT].name = (char*) calloc(MAX_WORD_LEN, sizeof(char));
            strncpy(LABELS_BUFF[LABELS_CNT].name, word + 1, MAX_WORD_LEN-1);
            LABELS_CNT++;

            return 0;
        }
        return 0;
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

    if (strlen(word) == 1 && word[0] == '#')
    {
        TOKEN_BUFF[POS].type = TYPE_COMMENT;
        COMM_LINES[TOKEN_BUFF[POS].line]++;

        return 0;
    }

    TOKEN_BUFF[POS].cmd  = get_cmd(word);
    TOKEN_BUFF[POS].type = TYPE_CMD;
    if (TOKEN_BUFF[POS].cmd >= REG_AX)
        TOKEN_BUFF[POS].type = TYPE_REG;

    return 0;
}


cmd_codes get_cmd(char* word)
{
    assert(word);

    if (!strcmp(word, "hlt"))  return CMD_HLT;

    if (!strcmp(word, "push")) return CMD_PUSH;
    if (!strcmp(word, "pop"))  return CMD_POP;
    if (!strcmp(word, "out"))  return CMD_OUT;
    if (!strcmp(word, "in"))   return CMD_IN;

    if (!strcmp(word, "add"))  return CMD_ADD;
    if (!strcmp(word, "sub"))  return CMD_SUB;
    if (!strcmp(word, "mul"))  return CMD_MUL;
    if (!strcmp(word, "div"))  return CMD_DIV;

    if (!strcmp(word, "jmp")) return CMD_JMP;
    if (!strcmp(word, "jb"))  return CMD_JMP_B;
    if (!strcmp(word, "jbe")) return CMD_JMP_BE;
    if (!strcmp(word, "ja"))  return CMD_JMP_A;
    if (!strcmp(word, "jae")) return CMD_JMP_AE;
    if (!strcmp(word, "je"))  return CMD_JMP_E;
    if (!strcmp(word, "jne")) return CMD_JMP_NE;

    if (!strcmp(word, "call")) return CMD_CALL;
    if (!strcmp(word, "ret"))  return CMD_RET;

    if (!strcmp(word, "sqrt"))    return CMD_SQRT;
    if (!strcmp(word, "nroots")) return CMD_NROOTS;
    if (!strcmp(word, "allnum"))  return CMD_ALLNUM;

    if (!strcmp(word, "ax")) return REG_AX;
    if (!strcmp(word, "bx")) return REG_BX;
    if (!strcmp(word, "cx")) return REG_CX;
    if (!strcmp(word, "dx")) return REG_DX;

    return CMD_ERROR;
}


int is_num(char* word)
{
    ASSERT(word);
    int i = 0;

    if (word[i] == '-') i++;

    for (; word[i]; i++)
        if (!isdigit(word[i]) && word[i] != '.') return 0;

    return 1;
}


int is_ram_ptr(char* word)
{
    ASSERT(word);
    int i = 0;

    if (word[i++] != '[')  return 0;
    for (; isdigit(word[i]); i++);
    if (word[i++] != ']')  return 0;
    if (strlen(word) != (size_t) i) return 0; 

    return 1;
}


int is_label(char* word)
{
    ASSERT(word);

    int i = 0;
    if (word[i++] != ':') return 0;

    for (; word[i]; i++)
        if (!isalnum(word[i]) && word[i] != '_') return 0;

    return 1;
}
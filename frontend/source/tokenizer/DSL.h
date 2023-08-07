#pragma once

#define TEXT_LINES text->code_buff.text_lines // pointers to first elements of lines
#define LINES_CNT  text->code_buff.lines_cnt

#define TEXT_BUFF  text->code_buff.text_buff  // buffer with code text
#define TEXT_LEN   text->code_buff.text_len   // number of symbols in text without 0 in the end

#define TOKEN_BUFF text->token_buff           // buffer with tokens for tree (exept created in executing process)
#define WORDS_CNT  text->code_buff.words_cnt

#define VAR_BUFF   text->var_buff             // buffer with variables
#define VAR_CNT    text->var_cnt

#define DOTS_BUFF  text->dot_tokens           // buffer with created nodes while executing like create_node() for freing in the end
#define DOTS_CNT   text->dot_count

#define FUNC_BUFF  text->func_buff            // buffer with functions and info about them
#define FUNC_CNT   text->func_cnt

#define POS        text->position
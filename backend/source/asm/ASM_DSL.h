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
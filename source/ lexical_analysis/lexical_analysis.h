#pragma once

//! @brief type of our elements values
typedef double elem_t;


//! @brief struct of our code
typedef struct
{
    char*     text_buff;
    unsigned  text_len;
    
    unsigned  token_cnt;
    token_t** token_buff;
    
    unsigned  var_cnt;
    var_t**   var_buff;
} text_t;


//! @brief types of lexical tokens
enum token_type
{
    NUM     = 1,
    VAR     = 2,
    OP      = 3,
    FUNC    = 4,
    STRING  = 5,
    O_BRCKT = 6,
    C_BRCKT = 7,
    START   = 8,
    FINISH  = 9,
    OTHER   = 10
};


//! @brief struct of lexical token
typedef struct
{
    char*      token_str;
    token_type type;
    elem_t     value;
    
    token_t*   parent;
    token_t*   prev;
    token_t*   next;
    
    unsigned   line;
    unsigned   pos;
} token_t;


typedef struct
{
    char*  name;
    elem_t value;
} var_t;

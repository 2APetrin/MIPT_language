#pragma once

//! @brief types of lexical tokens
enum token_type
{
    TYPE_NUM = 1,

    OP_ADD   = 2,
    OP_SUB   = 3,
    OP_MUL   = 4,
    OP_DIV   = 5,

    TYPE_VAR   = 6,
    TYPE_FUNC  = 7,

    TYPE_PRINT = 8,

    TYPE_START        = 9,
    TYPE_FINISH       = 10,
    TYPE_O_BRCKT      = 11,
    TYPE_C_BRCKT      = 12,
    TYPE_DOT          = 13,
    TYPE_PRINT_BRCKET = 14,
};



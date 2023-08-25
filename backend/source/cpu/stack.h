#pragma once

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define stack_ctor(stk, size) _stack_ctor((stk), (size), var_info_t {#stk, LOCATION})
#define stack_check(stk) _stack_check((stk), LOCATION)
#define MIN_CAPACITY 8

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//! @brief type of elements in stack
typedef double stk_elem_t;


//! @brief global variable for logs
extern FILE* logfile;


//! \brief struct of info about stack variable
typedef struct
{
    const char * name;
    const char * func;
    const char * file;
    int          line;
} var_info_t;


//! \brief main stack struct
typedef struct
{
    stk_elem_t* data;     // buffer of stack elements
    size_t      elemAmt;  // amount of elements
    size_t      capacity; // current capacity of stack

    var_info_t stack_info; // struct with information about stack variable
} my_stack_t;


//! \brief numbers for reasons entering in dump
enum DumpEnterReasons
{
    DUMP_ENTRY       = 0,
    DUMP_EXITING     = 1,
    DUMP_FOR_ERROR   = 2
};


//! \brief error codes for stack
enum ErrorCodes
{
    BAD_STK_POINTER  = (1 << 0),
    BAD_DATA_POINTER = (1 << 1),
    BIG_ELEM_AMT     = (1 << 2),
    BAD_STK_NAME     = (1 << 3),
    BAD_CAPACITY     = (1 << 4),
    BAD_STK_FILENAME = (1 << 5),
    BAD_STK_FUNCNAME = (1 << 6)
};


//! \brief codes to resize stack
enum ResizeCodes
{
    SIZEUP   = 1,
    SIZEDOWN = 0
};


//! @brief constant for comparing double numbers. If they are close enough (difference is less that EPSYLON) they are equal
const double EPSYLON = 0.00001;

/**
 * \brief constructor of stack structure
 *
 * \param stk pointer to stack variable
 * \param cap capacity of creating stack
 * \param info structure of info about stack variable
*/
void  _stack_ctor(my_stack_t* stk, size_t cap, var_info_t info);

/**
 * \brief function for stack check macros
 *
 * \param stk pointer to stack variable
 * \param func_name __PRETTY_FUNCTION__
 * \param file_name __FILE__
 * \param lineofcall __LINE__
*/
void  _stack_check(my_stack_t* stk, const char* func_name, const char* file_name, int lineofcall);

/**
 * \brief constructor of stack structure
 *
 * \param stk pointer to stack variable
 *
 * \return error number
*/
size_t err_check(my_stack_t* stk);

/**
 * \brief dumps stack into log_file
 *
 * \param stk pointer to stack variable
 * \param func_name __PRETTY_FUNCTION__
 * \param file_name __FILE__
 * \param lineofcall __LINE__
 * \param entry_reason reason of calling dump to print it in log
*/
void   stack_dump(my_stack_t* stk, const char* func_name, const char* file_name, int lineofcall, int entry_reason);

/**
 * \brief destructor of stack
 *
 * \param stk pointer to stack variable
*/
void   stack_dtor(my_stack_t* stk);

/**
 * \brief pushes number on the top of stack
 *
 * \param stk pointer to stack variable
 * \param val value to push in stack
*/
void   stack_push(my_stack_t* stk, stk_elem_t val);

/**
 * \brief get number from the top of stack
 *
 * \param stk pointer to stack variable
 * \param var pointer to variable to put number in
*/
void   stack_pop(my_stack_t* stk, stk_elem_t* var);

/**
 * \brief resizes stack
 *
 * \see ResizeCodes
 *
 * \param size_cmd type of resize
 * \param stk pointer to stack variable
 *
 * \return 0 if all is ok and 1 if error
*/
int    stack_resize(int size_cmd, my_stack_t* stk);

/**
 * \brief gets words of dump call reason
 *
 * \see ResizeCodes
 *
 * \param entry_reason code of calling reason
 *
 * \return pointer to line with reason of dump
*/
const char * get_dump_reason(int entry_reason);

//! @brief checks if doubles are equal
//!
//! @param [out] val1 first double number
//! @param [out] val2 second double number
//!
//! @return true if doubles are equal and false if not
int equald(double val1, double val2);
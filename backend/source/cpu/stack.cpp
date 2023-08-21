#include "cpu.h"

int _stack_ctor(my_stack_t * stk, size_t cap, var_info_t info)
{
    assert(stk       != NULL);
    assert(info.name != NULL);
    assert(info.func != NULL);
    assert(info.file != NULL);
    assert(info.line  > 0);

    stk->capacity  = cap;
    stk->elemAmt   = 0;
    stk->data      = (stk_elem_t *) calloc(stk->capacity, sizeof(stk_elem_t));

    stk->stack_info = info;

    stack_check(stk);
    return 0;
}

void _stack_check(my_stack_t * stk, const char * func_name, const char * file_name, int lineofcall)
{
    size_t num_of_err = err_check(stk);
    if (num_of_err)
    {
        fprintf(logfile, "\n   ERROR!!!\nerror catched in:\nfile - %s,\nfunc - %s,\nline - %d\n", file_name, func_name, lineofcall);
        fprintf(logfile, "error code - %lu\n\n", num_of_err);
        fclose(logfile);

        abort();
    }
}

size_t err_check(my_stack_t * stk)
{
    size_t sum = 0;

    if (stk == NULL)
        sum += BAD_STK_POINTER;
    if (stk->data == NULL)
        sum += BAD_DATA_POINTER; 
    if (stk->elemAmt > stk->capacity)
        sum += BIG_ELEM_AMT;
    if (stk->stack_info.name == NULL)
        sum += BAD_STK_NAME;
    if (stk->capacity < MIN_CAPACITY)
        sum += BAD_CAPACITY;
    if (stk->stack_info.file == NULL)
        sum += BAD_STK_FILENAME;
    if (stk->stack_info.func == NULL)
        sum += BAD_STK_FUNCNAME;

    return sum;
}

void stack_dump(my_stack_t * stk, const char * func_name, const char * file_name, int lineofcall, int entry_reason)
{
    stack_check(stk);

    fprintf(logfile, "\n-----------------------------------------------------------------------\n");

    fprintf(logfile, "   DUMP\n was called:\n\n%s,\n", func_name);
    fprintf(logfile, "%s\n", get_dump_reason(entry_reason));
    fprintf(logfile, "\nfile - %s,\nline - %d\n", file_name, lineofcall);

    fprintf(logfile, "\n   Stack info:\n");
    fprintf(logfile, "element amount = %lu\n", stk->elemAmt);
    fprintf(logfile, "stack capacity = %lu\n", stk->capacity);
    fprintf(logfile, "pointer to data - %p\n", stk->data);

    char ch = ' ';

    for (size_t i = 0; i < stk->capacity; i++)
    {
        ch = ' ';
        if (i < stk->elemAmt)
            ch = '*';

        fprintf(logfile, "%c[%2lu] - %lg\n", ch, (i + 1), stk->data[i]);
    }

    fprintf(logfile, "\n   Variable info:\n");
    fprintf(logfile, "variable name - %s\n", stk->stack_info.name);
    fprintf(logfile, "creation file - %s\n", stk->stack_info.file);
    fprintf(logfile, "creation func - %s\n", stk->stack_info.func);
    fprintf(logfile, "creation line - %d\n", stk->stack_info.line);

    fprintf(logfile, "-----------------------------------------------------------------------\n");

    stack_check(stk);
}

const char * get_dump_reason(int entry_reason)
{
    if (entry_reason == DUMP_ENTRY)
        return "Reason - Entry into function\n";
    if (entry_reason == DUMP_EXITING)
        return "Reason - Exiting from function\n";
    if (entry_reason == DUMP_FOR_ERROR)
        return "Reason - Dumping from middle of function, because of error\n";

    return "Unknown reason\n";
}

int stack_dtor(my_stack_t * stk)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = 0;
    }

    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->elemAmt  = 0;
    stk = NULL;

    return 0;
}

void stack_push(my_stack_t * stk, stk_elem_t val)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    if (stk->elemAmt == stk->capacity)
    {
        stack_resize(SIZEUP, stk);
    }

    stk->data[stk->elemAmt++] = val;

    if ((stk->capacity >= (MIN_CAPACITY * 2)) && (stk->elemAmt <= (stk->capacity / 4)))
    {
        stack_resize(SIZEDOWN, stk);
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);
}

void stack_pop(my_stack_t* stk, stk_elem_t* var)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    if (stk->elemAmt > 0)
    {
        *var = stk->data[stk->elemAmt - 1];
        stk->data[stk->elemAmt - 1] = 0;
        stk->elemAmt--; 
    }
    else
    {
        fprintf(logfile, "\n   WARNING!!!\nThere are no elements in stack data\nAny value was poped\nThere is dump under\n");
        stack_dump(stk, LOCATION, DUMP_FOR_ERROR);
    }

    if ((stk->capacity >= (MIN_CAPACITY * 2)) && (stk->elemAmt <= (stk->capacity / 4)))
    {
        stack_resize(SIZEDOWN, stk);
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);
}

int stack_resize(int size_cmd, my_stack_t * stk)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    fprintf(logfile, "\n-----------------------------------------------------------------------\n");
    fprintf(logfile, "   REALLOCATION!!!\ntype of reallocation - %d\n", size_cmd);
    fprintf(logfile, "-----------------------------------------------------------------------\n");

    if (size_cmd)
    {
        stk->data = (stk_elem_t *) realloc(stk->data, stk->capacity * 2 * sizeof(stk_elem_t));
        stk->capacity *=2;
        for (size_t i = stk->elemAmt; i < stk->capacity; i++)
        {
            stk->data[i] = 0;
        }
    }
    else
    {
        stk->data = (stk_elem_t *) realloc(stk->data, stk->capacity / 2 * sizeof(stk_elem_t));
        stk->capacity /= 2;
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);

    return 0;
}


int equald(double val1, double val2)
{
    if (!isfinite(val1)) return 0;
    if (!isfinite(val2)) return 0;

    if (val2 - val1 < EPSYLON && val2 - val1 > -1 * EPSYLON)
        return 1;
    return 0;
}
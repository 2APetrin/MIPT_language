#include "cpu.h"

int run_cpu(FILE* in_stream)
{
    assert(in_stream);

    cpu_t cpu = {};
    cpu_ctor(&cpu);

    if (get_num_of_symbols(&cpu, in_stream)) return 1;

    if (get_exe_arr(&cpu, in_stream)) return 1;

    if (execute_code(cpu))
    {
        free(cpu.exe_code_arr);
        stack_dtor(&cpu.stack);
        return 1; //в оригиналном коде тут было еще printf("Error: unknown command\n");
    }

    free(cpu.exe_code_arr);
    stack_dtor(&cpu.stack);

    return 0;
}

int cpu_ctor(cpu_t * cpu)
{
    assert(cpu);

    stack_ctor(&(cpu->stack), MIN_CAPACITY);

    return 0;
}

int get_num_of_symbols(cpu_t * cpu, FILE * in_stream)
{
    assert(in_stream);
    assert(cpu);

    fseek(in_stream, 0L, SEEK_END);
    cpu->exe_code_len = (size_t) ftell(in_stream);
    rewind(in_stream);

    return 0;
}

int get_exe_arr(cpu_t * cpu, FILE * in_stream)
{
    cpu->exe_code_arr = (int *) calloc (cpu->exe_code_len, sizeof(int));

    fread(cpu->exe_code_arr, sizeof(int), cpu->exe_code_len, in_stream);
    fclose(in_stream);

    if (cpu->exe_code_arr == NULL) return 1;

    return 0;
}

int execute_code(cpu_t cpu)
{
    printf("\n\nCode execution:\n");
    for (int i = 0; i < (int) cpu.exe_code_len; i++)
    {
        switch(cpu.exe_code_arr[i])
        {
            case HLT:
            {
                if (cpu.stack.elemAmt > 0)
                {
                    int k = 0;
                    size_t reps = cpu.stack.elemAmt; 
                    printf("Stack isn't empty. You've forgotten about %lu elements:\n", reps);

                    for (int t = 0; t < (int) reps; t++)
                    {
                        stack_pop(&cpu.stack, &k);
                        printf("%d\n", k / ACCURACY);
                    }
                }

                return 0;
            }
                break;

            case PUSH:
            {
                stack_push(&cpu.stack, cpu.exe_code_arr[++i]);
            }
                break;

            case ADD:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot ADD elements, there are less than two elements in stack %d\n", i);
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 
                stack_push(&cpu.stack, (val2 + val1));
            }
                break;

            case SUB:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot SUB elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 
                stack_push(&cpu.stack, (val2 - val1));
            }
                break;

            case MUL:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot MUL elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 
                stack_push(&cpu.stack, (val2 * val1) / ACCURACY);
            }
                break;

            case DIV:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot DIV elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 
                stack_push(&cpu.stack, (int) (((double) val2 / (double) val1) * ACCURACY));
            }
                break;
            
            case OUT:
            {
                if (cpu.stack.elemAmt < 1)
                {
                    printf("Error: cannot OUT elements, there is less than one element in stack\n");
                    return 1; // заменить на ретёрн / заменить на свой аборт
                }

                int val1 = 0;
                stack_pop (&cpu.stack, &val1);
                printf("OUT: %.3lg\n", ((double) val1) / ACCURACY);
            }
                break;
            
            case POP:
            {
                int val = 0;
                stack_pop (&cpu.stack, &val);

                //printf("pop val = %d\n", val);

                if (register_fill(cpu.exe_code_arr[++i], val, &cpu)) // мне не нравится этот движ с инкрементами, если что, надо за ними следить
                    return 1;
            }
                break;
            
            case PUSHR:
            {
                int val = 0;

                if (get_register_val(&val, cpu.exe_code_arr[++i], &cpu))
                    return 1;

                //printf("pushr val = %d\n", val);

                stack_push(&cpu.stack, val);
            }
                break;
            
            case JMP:
            {
                //printf("%d jmp\n", i);
                i = (cpu.exe_code_arr[i + 1] - 1);
                //printf("%d jmp\n", i);
            }
                break;
            
            case IN:
            {
                double val = 0;
                scanf("%lf", &val);

                //printf("printf in in - %d\n", (int) (val * ACCURACY));

                stack_push(&cpu.stack, (int) (val * ACCURACY));
            }
                break;
            
            case CALL:
            {
                stack_push(&cpu.stack, i + 1);

                //printf("%lu pushed\n", i + 1);

                i = (cpu.exe_code_arr[i + 1] - 1);
            }
                break;
            
            case RET:
            {
                int val = 0;

                stack_pop(&cpu.stack, &val);
                //printf("%lu poped\n", val);

                i = val;
            }
                break;
            
            case JMP_B:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 < val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case JMP_BE:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_be elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 <= val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case JMP_A:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_a elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 > val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case JMP_AE:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_ae elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 >= val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case JMP_E:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_e elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 == val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case JMP_NE:
            {
                if (cpu.stack.elemAmt < 2)
                {
                    printf("Error: cannot compare_ne elements, there are less than two elements in stack\n");
                    return 1;
                }

                int val1 = 0, val2 = 0;
                stack_pop (&cpu.stack, &val1);
                stack_pop (&cpu.stack, &val2); 

                if (val2 != val1)
                {
                    i = (cpu.exe_code_arr[i + 1] - 1);
                }
                else
                {
                    i += 1;
                }
            }
                break;
            
            case SQRT:
            {
                if (cpu.stack.elemAmt < 1)
                {
                    printf("Error: cannot sqrt elements, there are less than one element in stack\n");
                    return 1;
                }

                int val = 0;
                stack_pop (&cpu.stack, &val);

                if (val < 0)
                {
                    printf("Error: sqrt val is below zero\n");
                    return 1;
                }
                //val = (int)sqrt((double) val);
                //printf("%lg\n", sqrt(val));
                //printf("sqrt print - %d\n", (int) (sqrt(val) * sqrt(ACCURACY)));
                stack_push(&cpu.stack, (int) (sqrt(val) * sqrt(ACCURACY)));
            }
                break;
            
            case NROOTS:
            {
                printf("There are no roots in ur equation\n");
            }
                break;
            
            case ALLNUM:
            {
                printf("Every number fits\n");
            }
                break;
            
            case CAST:
            {
                //printf("cock\n");
                if (cpu.stack.elemAmt < 1)
                {
                    printf("Error: cannot cast element, there is less than one element in stack\n");
                    return 1;
                }

                int val = 0;
                stack_pop(&cpu.stack, &val);
                val++;
            }
                break;
            
            default:
            {
                printf("ERROR: Unknown code of command\n");
                return 1;
            }
                break;
        }
    }

    return 0;
}

int get_register_val(int * ret_val, int reg_num, cpu_t * cpu)
{
    //printf("reg num in get_register_val - %d\n", reg_num);
    switch (reg_num)
    {
        case AX:
        {
            *ret_val = cpu->reg_ax;
        }
            break;
        
        case BX:
        {
            *ret_val = cpu->reg_bx;
        }
            break;
        
        case CX:
        {
            *ret_val = cpu->reg_cx;
        }
            break;
        
        case DX:
        {
            *ret_val = cpu->reg_dx;
        }
            break;
        
        default:
        {
            printf("ERROR: bad register num in pushr\n");
            return 1;
        }
            break;
    }

    return 0;
}

int register_fill(int reg_num, int value, cpu_t * cpu)
{
    //printf("reg fill value = %d\n", value);
    switch (reg_num)
    {
        case AX:
        {
            cpu->reg_ax = value;
        }
            break;

        case BX:
        {
            cpu->reg_bx = value;
        }
            break;

        case CX:
        {
            cpu->reg_cx = value;
        }
            break;

        case DX:
        {
            cpu->reg_dx = value;
        }
            break;

        default:
        {
            printf("ERROR: bad register num in pop\n");
            return 1;
        }
            break;
    }

    return 0;
}
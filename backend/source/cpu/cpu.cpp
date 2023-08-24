#include "cpu.h"
#include "../../../file_work/file_work.h"

unsigned global_recursion_cntr = 0;

int run_cpu(FILE* in_stream)
{
    ASSERT(in_stream);

    global_recursion_cntr = 0;
    cpu_t* cpu = (cpu_t*) calloc(1, sizeof(cpu_t));
    cpu_ctor(cpu, in_stream);

    if (execute_code(cpu))
    {
        free(CPU_CODE_ARR);
        free(CPU_RAM);
        stack_dtor(&CPU_STACK);
        free(cpu);
        return 1;
    }

    free(CPU_CODE_ARR);
    free(CPU_RAM);
    stack_dtor(&CPU_STACK);
    free(cpu);

    return 0;
}


int execute_code(cpu_t* cpu)
{
    ASSERT(cpu);
    ASSERT(CPU_RAM);
    ASSERT(CPU_CODE_ARR);

    printf("\nCode execution:\n");
    int cmd = 0;
    stk_elem_t val = 0;
    stk_elem_t val1 = 0, val2 = 0;
    for (size_t i = 0; i < CPU_CODE_LEN; i++)
    {
        cmd  = (int) CPU_CODE_ARR[i];
        val  = 0;
        val1 = 0;
        val2 = 0;
        switch(cmd)
        {
            case CMD_HLT:
                if (CPU_STACK.elemAmt > 0)
                {
                    stk_elem_t stack_elem = 0;
                    size_t reps = CPU_STACK.elemAmt; 
                    printf("Stack isn't empty. You've forgotten about %lu elements:\n", reps);

                    for (size_t t = 0; t < reps; t++)
                    {
                        stack_pop(&CPU_STACK, &stack_elem);
                        printf("%lg\n", stack_elem);
                    }
                }
                return 0;

            case CMD_PUSH:
                stack_push(&CPU_STACK, CPU_CODE_ARR[++i]);
                break;

            case CMD_PUSH_RAM:
                if ((unsigned) CPU_CODE_ARR[i+1] + MAX_VAR_COUNT * global_recursion_cntr >= RAM_CAPACITY)
                    {
                        printf("Error: cannot push RAM\n");
                        return 1;
                    }
                //printf("push - [%u]\n", (unsigned) CPU_CODE_ARR[i + 1] + MAX_VAR_COUNT * global_recursion_cntr);
                stack_push(&CPU_STACK, CPU_RAM[(unsigned) CPU_CODE_ARR[++i] + MAX_VAR_COUNT * global_recursion_cntr]);
                break;

            case CMD_PUSH_REG:
                switch((int) CPU_CODE_ARR[++i])
                {
                    case REG_AX:
                        stack_push(&CPU_STACK, cpu->reg_ax);
                        break;

                    case REG_BX:
                        stack_push(&CPU_STACK, cpu->reg_bx);
                        break;

                    case REG_CX:
                        stack_push(&CPU_STACK, cpu->reg_cx);
                        break;

                    case REG_DX:
                        stack_push(&CPU_STACK, cpu->reg_dx);
                        break;

                    default:
                        printf("ERROR: bad register num in push reg\n\n");
                        return 1;
                }
                break;

            case CMD_POP:
                stack_pop (&CPU_STACK, &val);
                break;

            case CMD_POP_RAM:
                if ((unsigned) CPU_CODE_ARR[i+1] + MAX_VAR_COUNT * global_recursion_cntr >= RAM_CAPACITY)
                    {
                        printf("Error: cannot pop RAM\nAddress is too big = %u\n", (unsigned) CPU_CODE_ARR[i+1] + MAX_VAR_COUNT * global_recursion_cntr);
                        return 1;
                    }
                //printf("pop - [%u]\n", (unsigned) CPU_CODE_ARR[i + 1] + MAX_VAR_COUNT * global_recursion_cntr);
                stack_pop (&CPU_STACK, &val);
                CPU_RAM[(unsigned) CPU_CODE_ARR[++i] + MAX_VAR_COUNT * global_recursion_cntr] = val;
                break;

            case CMD_POP_REG:
                stack_pop (&CPU_STACK, &val);
                switch((int) CPU_CODE_ARR[++i])
                {
                    case REG_AX:
                        cpu->reg_ax = val;
                        break;

                    case REG_BX:
                        cpu->reg_bx = val;
                        break;

                    case REG_CX:
                        cpu->reg_cx = val;
                        break;

                    case REG_DX:
                        cpu->reg_dx = val;
                        break;

                    default:
                        printf("ERROR: bad register num in pop reg\n\n");
                        return 1;
                }
                break;

            case CMD_OUT:
                if (CPU_STACK.elemAmt < 1)
                {
                    printf("Error: cannot OUT elements, there is less than one element in stack\n");
                    return 1;
                }

                stack_pop (&CPU_STACK, &val);
                printf("out: %lg\n", val);
                break;

            case CMD_IN:
                printf("ins: ");
                scanf("%lf", &val);

                stack_push(&CPU_STACK, val);
                break;

            case CMD_ADD:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot ADD elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);
                stack_push(&CPU_STACK, (val2 + val1));
                break;

            case CMD_SUB:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot SUB elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);
                stack_push(&CPU_STACK, (val2 - val1));
                break;

            case CMD_MUL:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot MUL elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);
                stack_push(&CPU_STACK, (val2 * val1));
                break;

            case CMD_DIV:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot DIV elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);
                stack_push(&CPU_STACK, (val2 / val1));
                break;

            case CMD_JMP:
                i = (size_t) CPU_CODE_ARR[i+1] - 1;
                break;

            case CMD_JMP_B:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (val2 < val1)
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_JMP_BE:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (val2 < val1 || equald(val1, val2))
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_JMP_A:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (val2 > val1)
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_JMP_AE:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (val2 > val1 || equald(val1, val2))
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_JMP_E:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (equald(val1, val2))
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_JMP_NE:
                if (CPU_STACK.elemAmt < 2)
                {
                    printf("Error: cannot compare_b elements, there are less than two elements in stack %lu\n", i);
                    return 1;
                }

                stack_pop (&CPU_STACK, &val1);
                stack_pop (&CPU_STACK, &val2);

                if (!equald(val1, val2))
                    i = (size_t) CPU_CODE_ARR[i+1] - 1;
                else
                    i++;
                break;

            case CMD_CALL:
                if (RAM_CAPACITY / MAX_VAR_COUNT - 1 <= global_recursion_cntr)
                {
                    printf("Error: too many recursions\n");
                    return 1;
                }
                stack_push(&CPU_STACK, (stk_elem_t) i + 1);
                i = (size_t) CPU_CODE_ARR[i+1] - 1;
                global_recursion_cntr++;
                //printf("recursion = %u\n", global_recursion_cntr);
                break;

            case CMD_RET:
                stack_pop(&CPU_STACK, &val);
                i = (size_t) val;
                global_recursion_cntr--;
                break;

            case CMD_SQRT:
                stack_pop(&CPU_STACK, &val);
                stack_push(&CPU_STACK, sqrt(val));
                break;

            case CMD_NROOTS:
                printf("There are no roots in ur equation\n");
                break;

            case CMD_ALLNUM:
                printf("Every number fits\n");
                break;

            default:
                printf("ERROR: Unknown code of command - %d\nposition - %lu\n", cmd, i);
                return 1;
        }
    }

    return 0;
}


int cpu_ctor(cpu_t * cpu, FILE* in_stream)
{
    assert(cpu);

    stack_ctor(&(CPU_STACK), MIN_CAPACITY);
    CPU_RAM = (elem_t*) calloc(RAM_CAPACITY, sizeof(elem_t));

    fseek(in_stream, 0L, SEEK_END);
    long int len = ftell(in_stream);
    CPU_CODE_LEN = ((size_t) len) / sizeof(elem_t) + 1;
    rewind(in_stream);

    CPU_CODE_ARR = (elem_t*) calloc(CPU_CODE_LEN, sizeof(elem_t));

    fread(CPU_CODE_ARR, sizeof(elem_t), CPU_CODE_LEN, in_stream);
    fclose(in_stream);
    if (!CPU_CODE_ARR) return 1;

    /* for (unsigned i = 0; i < CPU_CODE_LEN; i++)
        printf("%3u", i);

    printf("\n");

    for (unsigned i = 0; i < CPU_CODE_LEN; i++)
        printf("%3lg", CPU_CODE_ARR[i]);

    printf("\n"); */

    return 0;
}
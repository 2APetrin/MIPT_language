#include "../file_work/file_work.h"
#include "source/asm/asm.h"
#include "source/cpu/cpu.h"
#include "source/cpu/stack.h"

FILE* logfile;

int main(void)
{
    FILE * input_file  = nullptr;
    FILE * output_file = nullptr;
    open_read_file ("temp/ass_code.asm", &input_file);
    open_write_file("temp/file.exe", &output_file);

    open_write_file("backend/logs/backend_log.txt", &logfile);

    run_asm(input_file, output_file);

    FILE* codefile = nullptr;
    open_read_file("temp/file.exe", &codefile);
    if (run_cpu(codefile))
    {
        printf("Execution failed\n");
    }

    fclose(logfile);
    return 0;
}
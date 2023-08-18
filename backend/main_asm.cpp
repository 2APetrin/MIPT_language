#include "source/asm/asm.h"
#include "../file_work/file_work.h"

int main(void)
{
    FILE* codefile = nullptr;
    FILE* outfile  = nullptr;

    open_read_file ("temp/ass_code.asm", &codefile);
    open_write_file("temp/prog.exe", &outfile);

    run_asm(codefile, outfile);

    return 0;
}
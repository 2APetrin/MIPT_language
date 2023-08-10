#include "source/asm/asm.h"
#include "../file_work/file_work.h"

int main(void)
{
    FILE* codefile = nullptr;
    open_read_file("temp/ass_code.asm", &codefile);

    asm_t* assembly = (asm_t*) calloc(1, sizeof(asm_t));
    asm_ctor(assembly, codefile);

    return 0;
}
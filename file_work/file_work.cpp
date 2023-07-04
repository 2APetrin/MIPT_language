#include "file_work.h"

int open_code_file(const char* name, FILE** code_file)
{
    if ((*code_file = fopen(name, "r")) == nullptr)
    {
        printf("Cannot open code file. Programm shutdown\n");
        return 1;
    }
    return 0;
}

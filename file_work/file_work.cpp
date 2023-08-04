#include "file_work.h"


int open_read_file(const char* name, FILE** code_file)
{
    if ((*code_file = fopen(name, "r")) == nullptr)
    {
        printf("Cannot open %s file. Programm shutdown\n", name);
        return 1;
    }
    return 0;
}


int open_write_file(const char* name, FILE** code_file)
{
    if ((*code_file = fopen(name, "w")) == nullptr)
    {
        printf("Cannot open %s file. Programm shutdown\n", name);
        return 1;
    }
    return 0;
}
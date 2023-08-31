#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("codefile name is not given\n");
        return 1;
    }

    char cmd[200] = "./obj/front ";
    snprintf(cmd + strlen(cmd), 100, "%s\n", argv[1]);

    system(cmd);
    system("./obj/middle\n");
    system("./obj/back\n");

    //system("rm -rf temp/*");

    return 0;
}
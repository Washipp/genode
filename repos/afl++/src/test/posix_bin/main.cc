/* Genode includes */
#include <base/log.h>
#include <util/string.h>
/* libc includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include <stdio.h>
#include <string.h>
#pragma GCC diagnostic pop  /* restore -Wconversion warnings */


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    printf("Hello from posix bin..\n");
    char alloc[5];
    char input[] = { 'b', 'u', 'g', '!','!'};

    if (input[0] == 'b') {
        printf("B");
        if (input[1] == 'u') {
            printf("U");
            if (input[2] == 'g') {
                printf("G");
                Genode::log("Bug");
                strcpy(alloc, input);
                alloc[500] = 'b';
                return -1;
            }
        }
    }
    return 0;
}

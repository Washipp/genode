/* Genode includes*/
#include <base/log.h>
#include <base/component.h>


int function_to_fuzz(void * input) {

    char * inp = (char *) input;
    char alloc[5];

    if (inp[0] == 'b') {
        alloc[0] = inp[0];
        if (inp[1] == 'u') {
            alloc[1] = inp[1];
            if (inp[2] == 'g') {
                Genode::memcpy(alloc, input, 5);
                alloc[-50] = 'b';
                return 1;
            }
        }
    }
    return 0;
}
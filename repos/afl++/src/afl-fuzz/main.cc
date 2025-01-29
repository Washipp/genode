#include <libc/component.h>
#include <base/log.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma GCC diagnostic pop  /* restore -Wconversion warnings */

extern "C" int main(int argc, char **argv_orig, char **envp);

/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

void Libc::Component::construct(Libc::Env &env)
{
    Libc::with_libc([&] () {
        Genode::log("LIBC construct called"); (void)env;
        int argc = 7;
        char **argv_orig = (char **) malloc(argc * sizeof(char *));
        char **envp = (char **) malloc(1 * sizeof(char *));

        argv_orig[0] = strdup("./program");
        argv_orig[1] = strdup("-i");
        argv_orig[2] = strdup("./input");
        argv_orig[3] = strdup("-o");
        argv_orig[4] = strdup("./output");
        argv_orig[5] = strdup("--");
        argv_orig[6] = strdup("/home/g-user/code/sandbox/target");

//        wait_for_continue();
        main(argc, argv_orig, envp);
        Genode::log("afl-fuzz test completed.");
    });
}
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
        int argc = 1;
        char **argv_orig = (char **) malloc(argc * sizeof(char *));
        char **envp = (char **) malloc(argc * sizeof(char *));

        argv_orig[0] = strdup("program");

        printf("Wait\n");
        wait_for_continue();
        main(argc, argv_orig, envp);
        Genode::log("afl-fuzz test completed.");
    });
}
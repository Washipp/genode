#include "init.h"
#include "sys/shm.h"

/* Genode includes*/
#include <libc/component.h>
#include <base/log.h>

/* libc includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include <stdlib.h>
#include <string.h>
#pragma GCC diagnostic pop  /* restore -Wconversion warnings */


/* Used for debugging. Upon call, the execution waits for an ENTER input. */
extern "C" void wait_for_continue(void);

const char *int_to_str(char *buffer, int num) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = static_cast<char>(num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    return buffer;
}

void Libc::Component::construct(Libc::Env &env)
{

    Libc::with_libc([&] () {
        Genode::log("Starting shm-server...");
        shm_init(env);

        int shmid = shmget(0, 4096, 0);

        Genode::log("Got new shmid: ", shmid);

        auto addr = shmat(shmid, NULL, 0);

        Genode::log("Attached segment to: ", addr);

        Genode::memcpy(addr, "Test string from shm_server.", 29);
        Genode::log("Successfully written to ", addr);
    });
}
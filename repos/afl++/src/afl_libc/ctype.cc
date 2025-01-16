#include "ctype.h"
#include <util/string.h>

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A'); // Convert uppercase to lowercase
    }
    return c;
}

int isspace(int c) {
    return Genode::is_whitespace((char) c);
}
int isdigit(int c) {
    return Genode::is_digit((char) c);
}
int isxdigit(int c) {
    return Genode::is_digit((char) c, true);
}

/* Printable characters in ASCII range from 32 (' ') to 126 ('~') according to
 * https://www.ascii-code.com/characters/printable-characters */
int isprint(int c) {
    // TODO: check if this function works the same as the ctype.h isprint function.
    return (c >= 32 && c <= 126);
}

/* Implementation from ctype.h */
int isascii(int c) {
    return (((c) & ~0x7f) == 0);
}

/* Note, this function does not comply with the libc standard.
 * In the context of AFL it is only used to check if the provided CLI arguments can be used to create a directory.
 * (See `afl-fuzz-init.c`) */
int isalnum(int c) {
    return (isascii(c) || isdigit(c));
}
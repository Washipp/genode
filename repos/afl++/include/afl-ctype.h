#ifndef GENODE_AFL_CTYPE_H
#define GENODE_AFL_CTYPE_H

/* Return lowercase of C if it exists. */
extern "C" int tolower(int c);
/* Checks  for  white-space characters.  In the "C" and "POSIX" locales,
 * these are: space, form-feed ('\f'), newline ('\n'),  carriage  return  ('\r'),
 * horizontal  tab ('\t'), and vertical tab('\v').*/
extern "C" int isspace(int c);
/* Checks for a digit (0 through 9). */
extern "C" int isdigit(int c);
/* Checks for hexadecimal digits, that is, one of
0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.*/
extern "C" int isxdigit(int c);
/* Checks for any printable character including space. */
extern "C" int isprint(int c);
/* Checks  whether  c is a 7-bit unsigned char value that fits into the ASCII character set. */
extern "C" int isascii(int c);
/* Checks for an alphanumeric character; it is equivalent to (isalpha(c) || isdigit(c)). */
extern "C" int isalnum(int c);

#endif //GENODE_AFL_CTYPE_H

#ifndef GENODE_AFL_CTYPE_H
#define GENODE_AFL_CTYPE_H

#define _ISbit(bit)	((bit) < 8 ? ((1 << (bit)) << 8) : ((1 << (bit)) >> 8))
enum
{
    _ISupper = _ISbit (0),	/* UPPERCASE.  */
    _ISlower = _ISbit (1),	/* lowercase.  */
    _ISalpha = _ISbit (2),	/* Alphabetic.  */
    _ISdigit = _ISbit (3),	/* Numeric.  */
    _ISxdigit = _ISbit (4),	/* Hexadecimal numeric.  */
    _ISspace = _ISbit (5),	/* Whitespace.  */
    _ISprint = _ISbit (6),	/* Printing.  */
    _ISgraph = _ISbit (7),	/* Graphical.  */
    _ISblank = _ISbit (8),	/* Blank (usually SPC and TAB).  */
    _IScntrl = _ISbit (9),	/* Control character.  */
    _ISpunct = _ISbit (10),	/* Punctuation.  */
    _ISalnum = _ISbit (11)	/* Alphanumeric.  */
};

extern const unsigned short int **__ctype_b_loc (void);

extern int tolower (int __c);

# define __isctype(c, type)  ((*__ctype_b_loc ())[(int) (c)] & (unsigned short int) type)
# define isspace(c)	__isctype((c), _ISspace)
# define isalnum(c)	__isctype((c), _ISalnum)

# define isdigit(c)	__isctype((c), _ISdigit)

# define isxdigit(c)	__isctype((c), _ISxdigit)
# define isprint(c)	__isctype((c), _ISprint)
#define	__isascii(c)	(((c) & ~0x7f) == 0)	/* If C is a 7 bit value.  */
#  define isascii(c)	__isascii (c)
#endif //GENODE_AFL_CTYPE_H

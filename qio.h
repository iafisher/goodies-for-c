#ifndef QIO_H
#define QIO_H

#include <stdio.h>
#include "qstring.h"

/**
 * Read the entire file located at `pathname`. A heap-allocated character buffer
 * containing the contents of the file is returned, and the number of characters
 * read is placed in `nptr`.
 *
 * This function uses the stat syscall and so will only work with Linux.
 */
char* qio_readpath(const char* pathname, size_t* nptr);

/**
 * Read a line from the file. A heap-allocated character buffer containing the
 * contents of the line is returned, and the number of characters read is
 * placed in `nptr`.
 */
char* qio_readline(FILE*, size_t* nptr);

/**
 * Variants of the regular qio functions that use qstrings.
 */
qstring qio_readpath_qs(const char* pathname);
qstring qio_readline_qs(FILE*);

#endif

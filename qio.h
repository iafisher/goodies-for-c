#ifndef QIO_H
#define QIO_H

#include <stdio.h>
#include "qstring.h"

qstring qio_read(FILE*);
qstring qio_readn(FILE*, size_t);
qstring qio_readline(FILE*);

size_t qio_write(FILE*, qstring);

#endif

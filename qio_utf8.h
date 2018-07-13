#ifndef QIO_UTF8_H
#define QIO_UTF8_H

#include "qstring_utf8.h"

qstring qio_utf8_read(FILE*);
qstring qio_utf8_readline(FILE*);

size_t qio_utf8_write(FILE*, qstring_utf8);

#endif

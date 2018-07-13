#ifndef QSTRING_UTF8_H
#define QSTRING_UTF8_H

/* Define the utf8_char type as a 32-bit integer. */
#if __STDC_VERSION__ >= 199901L
#include <stdint.h>
typedef uint_least32_t utf8_char;
#else
typedef unsigned long int utf8_char;
#endif

typedef struct {
    size_t len;
    utf8_char* data;
} qstring_utf8;

#endif

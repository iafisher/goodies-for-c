/* Implementation of the qstring library. See qstring.h for API documentation.
 *
 * Author:  Ian Fisher (iafisher@protonmail.com)
 * Version: July 2018
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qstring.h"

qstring qstring_new(const char* cs) {
    return qstring_new_buffer(cs, strlen(cs));
}

qstring qstring_new_buffer(const char* buffer, size_t n) {
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(n + 1);
    if (ret.data == NULL) {
        return ret;
    }
    memcpy(ret.data, buffer, n);
    ret.len = n;
    ret.data[ret.len] = '\0';
    return ret;
}

qstring qstring_repeat(char c, size_t n) {
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(n + 1);
    if (ret.data == NULL) {
        return ret;
    }
    for (size_t i = 0; i < n; i++) {
        ret.data[i] = c;
    }
    ret.len = n;
    ret.data[ret.len] = '\0';
    return ret;
}

qstring qliteral(const char* cs) {
    /* Casting const char* to char* is obviously dangerous, which is why the
     * API documentation for this function comes with a warning about misusing
     * the return value.
     */
    qstring ret = {.len = strlen(cs), .data = (char*)cs};
    return ret;
}

void qstring_cleanup(qstring qs) {
    free(qs.data);
}

qstring qstring_copy(qstring qs) {
    return qstring_new_buffer(qs.data, qs.len);
}

qstring qstring_substr(qstring qs, size_t start, size_t n) {
    if (start >= qs.len) {
        return qstring_new("");
    }
    if (start + n >= qs.len) {
        n = qs.len - start;
    }
    return qstring_new_buffer(qs.data + start, n);
}

qstring qstring_remove(qstring qs, size_t start, size_t n) {
    if (start >= qs.len) {
        return qstring_copy(qs);
    }
    if (start + n >= qs.len) {
        n = qs.len - start;
    }
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(qs.len - n + 1);
    if (ret.data == NULL) {
        return ret;
    }
    /* Copy the data before the removed substring. */
    memcpy(ret.data, qs.data, start);
    /* Copy the data after the removed substring, including the null terminator.
     */
    memcpy(ret.data + start, qs.data + start + n, qs.len - n - start + 1);
    ret.len = qs.len - n;
    return ret;
}

qstring qstring_concat(qstring qs1, qstring qs2) {
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(qs1.len + qs2.len + 1);
    if (ret.data == NULL) {
        return ret;
    }
    memcpy(ret.data, qs1.data, qs1.len);
    memcpy(ret.data + qs1.len, qs2.data, qs2.len + 1);
    ret.len = qs1.len + qs2.len;
    return ret;
}

qstring qstring_format(qstring fmtstr, ...) {
    va_list args;
    va_start(args, fmtstr);
    va_list args2;
    va_copy(args2, args);

    qstring ret = {.len = 0, .data = NULL};
    int sz = vsnprintf(NULL, 0, fmtstr.data, args);
    va_end(args);
    if (sz == -1) {
        return ret;
    }
    ret.data = malloc(sz + 1);
    if (ret.data == NULL) {
        return ret;
    }

    sz = vsnprintf(ret.data, sz+1, fmtstr.data, args2);
    va_end(args2);
    if (sz == -1) {
        free(ret.data);
        return ret;
    }
    ret.len = sz;
    return ret;
}

qstring qstring_replace_all(qstring qs, qstring before, qstring after) {
    size_t count = qstring_count(qs, before);
    if (count == 0) {
        return qstring_copy(qs);
    }
    size_t newlen = qs.len + count * (after.len - before.len);
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(newlen + 1);
    if (ret.data == NULL) {
        return ret;
    }
    //size_t index = 0;
    // TODO: Finish this with qstring_find_in
    return ret;
}

static qstring replace_substr(qstring qs, size_t start, size_t n,
    qstring replacing) {
    size_t newlen = qs.len - n + replacing.len;
    qstring ret = {.len = 0, .data = NULL};
    ret.data = malloc(newlen + 1);
    if (ret.data == NULL) {
        return ret;
    }
    /* Copy the original string before the replaced substring. */
    memcpy(ret.data, qs.data, start);
    /* Copy the replacement string. */
    memcpy(ret.data + start, replacing.data, replacing.len);
    /* Copy the original string after the replaced substring. */
    memcpy(ret.data + start + replacing.len, qs.data + start + n,
        qs.len - (start - n + 1));
    ret.len = newlen;
    return ret;
}

qstring qstring_replace_first(qstring qs, qstring before, qstring after) {
    size_t index = qstring_find(qs, before);
    if (index == qs.len) {
        return qstring_copy(qs);
    }
    return replace_substr(qs, index, before.len, after);
}

qstring qstring_replace_last(qstring qs, qstring before, qstring after) {
    size_t index = qstring_rfind(qs, before);
    if (index == qs.len) {
        return qstring_copy(qs);
    }
    return replace_substr(qs, index, before.len, after);
}

size_t qstring_find(qstring qs, qstring datum) {
    return qstring_find_in(qs, datum, 0, qs.len);
}

size_t qstring_find_in(qstring qs, qstring datum, size_t start, size_t n) {
    if (start >= qs.len) {
        return qs.len;
    }
    if (start + n >= qs.len) {
        n = qs.len - start;
    }
    /* This check is not just an optimization: it's necessary to prevent memcmp
     * from examining invalid memory.
     */
    if (datum.len > n) {
        return qs.len;
    }

    for (size_t i = start; i < start + n; i++) {
        if (memcmp(qs.data + i, datum.data, datum.len) == 0) {
            return i;
        }
    }
    return qs.len;
}

size_t qstring_rfind(qstring qs, qstring datum) {
    return qstring_rfind_in(qs, datum, 0, qs.len);
}

size_t qstring_rfind_in(qstring qs, qstring datum, size_t start, size_t n) {
    if (start >= qs.len) {
        return qs.len;
    }
    if (start + n >= qs.len) {
        n = qs.len - start;
    }
    /* This check is not just an optimization: it's necessary to prevent memcmp
     * from examining invalid memory.
     */
    if (datum.len > n) {
        return qs.len;
    }

    for (size_t i = start; i < start + n; i++) {
        size_t ri = (start + n) - (i + 1);
        if (memcmp(qs.data + ri, datum.data, datum.len) == 0) {
            return ri;
        }
    }
    return qs.len;
}

size_t qstring_count(qstring qs, qstring datum) {
    /* Make sure to test weird cases like count("aaa", "aa") == 1 */
    if (datum.len > qs.len) {
        return 0;
    }

    size_t count = 0;
    for (size_t i = 0; i < qs.len; i++) {
        if (memcmp(qs.data + i, datum.data, datum.len) == 0) {
            count++;
            i += datum.len;
        }
    }
    return count;
}

bool qstring_startswith(qstring qs, qstring prefix) {
    return qs.len >= prefix.len &&
        memcmp(qs.data, prefix.data, prefix.len) == 0;
}

bool qstring_endswith(qstring qs, qstring suffix) {
    return qs.len >= suffix.len &&
        memcmp(qs.data + (qs.len - suffix.len), suffix.data, suffix.len) == 0;
}

qstring qstring_lstrip(qstring qs, qstring to_strip) {
    size_t nprefix = 0;
    while (memchr(to_strip.data, qs.data[nprefix], to_strip.len) != NULL) {
        nprefix++;
    }
    return qstring_substr(qs, nprefix, qs.len - nprefix);
}

qstring qstring_rstrip(qstring qs, qstring to_strip) {
    size_t nsuffix = 0;
    while (memchr(to_strip.data, qs.data[qs.len - (nsuffix + 1)], to_strip.len)
            != NULL) {
        nsuffix++;
    }
    return qstring_substr(qs, 0, qs.len - nsuffix);
}

qstring qstring_strip(qstring qs, qstring to_strip) {
    size_t nsuffix = 0;
    while (memchr(to_strip.data, qs.data[qs.len - (nsuffix + 1)], to_strip.len)
            != NULL) {
        nsuffix++;
    }
    size_t nprefix = 0;
    while (memchr(to_strip.data, qs.data[nprefix], to_strip.len) != NULL) {
        nprefix++;
    }
    return qstring_substr(qs, nprefix, qs.len - nprefix - nsuffix);
}

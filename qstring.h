/* THIS MODULE IS NOT YET FULLY IMPLEMENTED.
 *
 * Easy strings. Length-prefixed so calculating length is O(1) instead of O(n).
 * Handles null bytes correctly. Buffer overflows are impossible. Compatible
 * with the C standard library string functions.
 *
 * A qstring is a length paired with a data field. Qstrings are immutable. The
 * qstring structs are stack-allocated and the data field is heap-allocated,
 * except for qstrings returned by qliteral.
 *
 * If a qstring method returns a qstring, then that qstring is newly-allocated
 * from the heap, does not share data with any of the parameters, and must be
 * passed to qstring_cleanup to free memory. If allocation fails, then a
 * qstring with a length of 0 and a NULL data field is returned.
 *
 *
 * Author:  Ian Fisher (iafisher@protonmail.com)
 * Version: July 2018
 */

#ifndef QSTRING_H
#define QSTRING_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    /* Both fields are considered public and read-only. */

    /* The number of bytes in the data field, excluding the null terminator. */
    size_t len;
    /* This character array is null-terminated, so it can be passed to any
       standard C function. */
    char* data;
    /* TODO: unsigned char*? */
} qstring;

/* TODO: Add a qrange type? */

/**
 * Return a qstring containing a heap-allocated copy of the string parameter,
 * which must be null-terminated.
 *
 * The returned qstring must eventually be passed to qstring_cleanup to avoid a
 * memory leak.
 */
qstring qstring_new(const char*);

/**
 * Return a qstring containing a heap-allocated copy of the first `n` bytes of
 * the string parameter. The parameter may contain null bytes and does not need
 * to be null-terminated, although the data field in the returned qstring will
 * be. The null terminator will be inserted after the characters from the
 * buffer, so
 *
 *   qstring_new_buffer("hello\0", 6)
 *
 * will result in a qstring whose data field is "hello\0\0".
 */
qstring qstring_new_buffer(const char*, size_t n);

/**
 * Return a stack-allocated qstring struct whose data field is a heap-allocated
 * buffer with `n` copies of the character `c`.
 */
qstring qstring_repeat(char c, size_t n);

/**
 * Return a stack-allocated qstring struct whose data field is set to the
 * string parameter, which must be null-terminated. Use qliteral where you would
 * otherwise use a string literal. For example,
 *
 *   qstring qs = qstring_concat(qs_old, "!");
 *
 * instead of
 *
 *   qstring bang = qstring_new("!");
 *   qstring qs = qstring_concat(qs_old, bang);
 *   qstring_cleanup(bang);
 *
 * which is longer and makes an unnecessary heap allocation.
 *
 * The returned qstring should NOT be passed to qstring_cleanup.
 */
qstring qliteral(const char*);

/**
 * Free the heap-allocated data in the qstring. Do not pass objects returned by
 * qliteral to this method.
 */
void qstring_cleanup(qstring);

/**
 * Copy the data inside the qstring parameter into a new qstring object.
 */
qstring qstring_copy(qstring);

/**
 * Copy a substring of `n` characters of `qs`, starting at index `start`. If
 * `start` is out of bounds, the empty string is returned. If `start` is within
 * bounds but `start+n` is not, then a copy of the substring from `start` to
 * the end of string is returned.
 */
qstring qstring_substr(qstring qs, size_t start, size_t n);

/**
 * Return `qs` with the substring removed. If `start` is out of bounds then a
 * copy of the original qstring is returned. If `start + n` is out of bounds
 * (but `start` isn't) then all bytes from `start` to the end of `qs` are
 * removed.
 */
qstring qstring_remove(qstring qs, size_t start, size_t n);

/**
 * Concatenate the two qstrings.
 */
qstring qstring_concat(qstring, qstring);

/**
 * Insert the arguments into `fmtstr`, a format string as in printf. Note that
 * qstrings cannot be formatted directly into the string, so
 *
 *   qstring_format("%s%s", qs1, qs2); // WRONG!
 *
 * has to be written as
 *
 *   qstring_format("%s%s", qs1.data, qs2.data);
 *
 * instead. As this implies, qstring parameters that contain null bytes will
 * be truncated.
 *
 * A future implementation may support a "%qs" format code for qstrings that
 * works will qstrings containing null bytes.
 */
qstring qstring_format(qstring fmtstr, ...);

/**
 * Replace all non-overlapping instances of `before` with `after` in `qs`.
 */
qstring qstring_replace_all(qstring qs, qstring before, qstring after);

/**
 * Replace the first instance of `before` with `after` in `qs`.
 */
qstring qstring_replace_first(qstring qs, qstring before, qstring after);

/**
 * Replace the last instance of `before` with `after` in `qs`.
 */
qstring qstring_replace_last(qstring qs, qstring before, qstring after);

/**
 * Return the index of the first instance of `datum` in `qs`. If `datum` is not
 * found, then `qs.len` is returned.
 */
size_t qstring_find(qstring qs, qstring datum);

/**
 * Return the first instance of `datum` in the specified substring of `qs`.
 */
size_t qstring_find_in(qstring qs, qstring datum, size_t start, size_t n);

/**
 * Return the index of the last instance of `datum` in `qs`. If `datum` is not
 * found, then `qs.len` is returned.
 */
size_t qstring_rfind(qstring qs, qstring datum);

/**
 * Return the last instance of `datum` in the specified substring of `qs`.
 */
size_t qstring_rfind_in(qstring datum, qstring qs, size_t start, size_t n);

/**
 * Return the number of non-overlapping occurrences of `datum` in `qs`.
 */
size_t qstring_count(qstring qs, qstring datum);

/**
 * Return true if `qs` starts with `prefix`.
 */
bool qstring_startswith(qstring qs, qstring prefix);

/**
 * Return true if `qs` ends with `suffix`.
 */
bool qstring_endswith(qstring qs, qstring suffix);

/**
 * Remove characters from the beginning of `qs` until the first character not
 * in `to_strip` is encountered. The order of characters in `to_strip` does
 * not matter.
 *
 * qstring_lstrip("abbac", "ab") == "c"
 */
qstring qstring_lstrip(qstring qs, qstring to_strip);

/**
 * Same as qstring_lstrip, except from the end instead of the beginning.
 */
qstring qstring_rstrip(qstring, qstring to_strip);

/**
 * Equivalent to qstring_lstrip(qstring_rstrip(qs, to_strip), to_strip), but
 * more efficient.
 */
qstring qstring_strip(qstring, qstring to_strip);

/**
 * Convenience macros for stripping whitespace.
 */
#define qstring_rstrip_ws(qs) qstring_rstrip(qs, " \t\n\r\v\f")
#define qstring_lstrip_ws(qs) qstring_lstrip(qs, " \t\n\r\v\f")
#define qstring_strip_ws(qs)  qstring_strip(qs, " \t\n\r\v\f")

#endif

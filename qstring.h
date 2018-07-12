/* THIS MODULE IS NOT YET IMPLEMENTED.
 *
 * Easy strings. Length-prefixed so calculating length is O(1) instead of O(n).
 * Handles null bytes correctly. Buffer overflows are impossible. Compatible
 * with the C standard library string functions.
 *
 * Qstrings are immutable. The qstring structs are stack-allocated and the data
 * field is heap-allocated, except for qstrings returned by qliteral_new.
 *
 * qliteral_new is a convenience method to use when you would otherwise use a
 * bare string literals. It lets you write
 *
 *   qstring concatted = qstring_concat(qs, qliteral("..."));
 *
 * instead of
 *
 *   qstring literal = qstring_new("...");
 *   qstring concatted = qstring_concat(qs, literal);
 *   qstring_cleanup(literal);
 *
 * Qstrings returned by qliteral_new (call them "qliterals") can be passed to
 * any qstring library function, except for qstring_cleanup. qliteral_new
 * should only be used to wrap string literals in the source code. You must be
 * careful to avoid writing code where a regular qstring and a qliteral could
 * be confused, for instance
 *
 * qstring bad(bool cond) {
 *   if (cond) {
 *     return qstring_new("uh oh");
 *   } else {
 *     return qliteral_new("uh oh");
 *   }
 * }
 *
 * Callers of this function cannot distinguish between qstrings and qliterals,
 * so either they don't free the result and leak memory, or they do free it and
 * cause a segfault when a qliteral is returned.
 *
 * The moral: Use qliteral_new when you want to pass a string literal to a
 * function. Use qstring_new everywhere else.
 *
 * If a qstring method returns a qstring, then that qstring is newly-allocated
 * from the heap, does not share data with any of the parameters, and must be
 * passed to qstring_cleanup to free memory.
 *
 *
 * Author:  Ian Fisher (iafisher@protonmail.com)
 * Version: July 2018
 */

#ifndef QSTRING_H
#define QSTRING_H

typedef struct {
    /* Both fields are considered public and read-only. */
    size_t len;
    /* This character array is null-terminated, so it can be used with any
       standard C function. */
    const char* data;
} qstring;


/**
 * Return a stack-allocated qstring struct whose data field is a heap-allocated
 * copy of the string parameter, which must be null-terminated.
 *
 * The returned qstring must eventually be passed to qstring_cleanup to avoid a
 * memory leak.
 */
qstring qstring_new(const char*);

/**
 * Return a stack-allocated qstrng struct whose data field is a heap-allocated
 * copy of the first `n` bytes of the string parameter. The parameter may
 * contain null bytes and does not need to be null-terminated, although the
 * data field in the returned qstring will be. The null terminator will be
 * inserted after the `n` characters of the buffer, so
 *
 *   qstring_new_buffer("hello\0", 6)
 *
 * will result in a qstring whose data field is "hello\0\0".
 */
qstring qstring_new_buffer(const char*, size_t n);

/**
 * Return a stack-allocated qstring struct whose data field is set to the string
 * parameter, which must be null-terminated.
 *
 * The returned qstring should NOT be passed to qstring_cleanup.
 *
 * If you are unsure whether to use qstring_new or qliteral_new, please read the
 * comment at the top of this module. If in doubt, choose qstring_new.
 */
qstring qliteral_new(const char*);

/**
 * Free the heap-allocated data in the qstring. Do not pass objects returned by
 * qliteral_new to this method.
 */
void qstring_cleanup(qstring);

/**
 * Copy the data inside the qstring parameter into a new qstring object.
 */
qstring qstring_copy(qstring);

/**
 * Copy a substring of `n` characters of the qstring parameter, starting at
 * index `start`. If `start` is past the end of the string, the empty string
 * is returned. If `start` is within bounds but `start+n` is not, then a copy
 * of the substring from `start` to the end of string is returned.
 */
qstring qstring_substr(qstring, size_t start, size_t n);

/**
 * Return the first parameter with the substring removed. The `start` and `n`
 * parameters have the same meaning as with qstring_substr.
 */
qstring qstring_remove(qstring, size_t start, size_t n);

/**
 * Concatenate the two qstrings together.
 */
qstring qstring_concat(qstring, qstring);

/**
 * Format the format string parameter with the arguments, as printf does.
 */
qstring qstring_format(qstring fmtstr, ...);

/**
 * Replace all instances of the second qstring in the first.
 */
qstring qstring_replace_all(qstring, qstring);

/**
 * Replace the first instance of the second qstring in the first.
 */
qstring qstring_replace_first(qstring, qstring);

/**
 * Replace the last instance of the second qstring in the first.
 */
qstring qstring_replace_last(qstring, qstring);

/**
 * Return the first instance of the second qstring in the first. If the second
 * is not found, then the length of the first qstring is returned.
 */
size_t qstring_find(qstring, qstring);

/**
 * Return the last instance of the second qstring in the first. If the second
 * is not found, then the length of the first is returned.
 */
size_t qstring_rfind(qstring, qstring);

/**
 * Return the number of occurrences of the second qstring in the first.
 */
size_t qstring_count(qstring, qstring);

/**
 * Return true if the first qstring starts with the second.
 *
 * qstring_startswith(qliteral_new("abcdef"), qliteral_new("abc")) == TRUE
 */
bool qstring_startswith(qstring, qstring);

/**
 * Return true if the first qstring ends with the second.
 *
 * qstring_endswith("abcdef", "def") == TRUE
 */
bool qstring_endswith(qstring, qstring);

/**
 * Remove characters from the beginning of the first qstring until the first
 * character not in `to_strip` is encountered. The order of characters in the
 * second parameter does not matter.
 *
 * qstring_lstrip("abbac", "ab") == "c"
 */
qstring qstring_lstrip(qstring, qstring to_strip);

/**
 * Same as qstring_lstrip, except from the end instead of the beginning.
 */
qstring qstring_rstrip(qstring, qstring to_strip);

/**
 * Equivalent to qstring_lstrip(qstring_rstrip(qs, to_strip), to_strip)
 */
qstring qstring_strip(qstring, qstring to_strip);

/**
 * These methods are the same at their equivalent qstring_strip methods, with
 * the constant string " \t\n\v\f\r" supplied as the second parameter.
 */
qstring qstring_rstrip_ws(qstring);
qstring qstring_lstrip_ws(qstring);
qstring qstring_strip_ws(qstring);

#endif

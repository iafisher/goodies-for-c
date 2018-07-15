/* Lightweight unit testing.
 *
 * The minimal set-up is something like this:
 *
 *     #include <stdio.h>
 *
 *     TEST_MODULE;
 *
 *     int main() {
 *         ASSERT(1 + 1 == 2);
 *     }
 *
 *
 * Author:  Ian Fisher (iafisher@protonmail.com)
 * Version: July 2018
 */

#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h>

/**
 * A macro that declares global variables that the ASSERT macro assumes will
 * exist. Place this macro in the global scope of all of your test files.
 */
#define TEST_MODULE \
    unsigned int tests_passed = 0; \
    unsigned int tests_failed = 0

/**
 * Assert that the condition is true. If true, tests_passed is incremented. If
 * false, tests_failed is incremented and an error message that includes the
 * file name, line number, and text of the condition is printed to stderr.
 *
 * Requires stdio.h to be included for fprintf.
 *
 * ASSERT has to be a macro for two reasons:
 *  - It uses undeclared globals, tests_failed and tests_passed.
 *  - It uses the __FILE__ and __LINE__ macro constants, which we want to
 *    expand to the file and line of the test suite, not the file and line of
 *    the unittest implementation.
 *
 * Wrapping the body of a macro in a do-while loop without a terminating
 * semicolon is a common idiom. It ensures that the macro is parsed as a single
 * statement, and that forgetting the terminating semicolon is a syntax error.
 */
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            tests_failed++; \
            fprintf(stderr, "ASSERTION FAILED, %s, line %d: `%s` is FALSE.\n", \
                __FILE__, __LINE__, #condition); \
        } else { \
            tests_passed++; \
        } \
    } while (0)

/**
 * A shortcut for ASSERT(strcmp(expected, got) == 0), with a more informative
 * error message that prints the contents of both strings.
 *
 * Requires string.h to be included, for strcmp, and stdio.h, for fprintf.
 */
#define ASSERT_STREQ(expected, got) \
    do { \
        const char* expectedv = expected; \
        const char* gotv = got; \
        if (strcmp(expectedv, gotv) != 0) { \
            tests_failed++; \
            fprintf(stderr, "ASSERTION FAILED, %s, line %d:" \
                " expected \"%s\", got \"%s\".\n", __FILE__, __LINE__, \
                expectedv, gotv); \
        } else { \
            tests_passed++; \
        } \
    } while (0)

/**
 * A short cut for ASSERT(expected == got) for any signed integer type, with a
 * more informative error message than ASSERT provides. It is not safe to pass
 * unsigned integers to this function--use ASSERT_UINTEQ instead.
 *
 * Requires stdio.h to be included, for fprintf.
 */
#define ASSERT_INTEQ(expected, got) \
    ASSERT_EQ(long long, "%lld", expected, got)

/**
 * A short cut for ASSERT(expected == got) for any unsigned integer type, with a
 * more informative error message than ASSERT provides. It is not safe to pass
 * signed integers to this function--use ASSERT_INTEQ instead.
 *
 * Requires stdio.h to be included, for fprintf.
 */
#define ASSERT_UINTEQ(expected, got) \
    ASSERT_EQ(unsigned long long, "%llu", expected, got)

/**
 * A short cut for ASSERT(expected == got) for any floating-point type, with a
 * more informative error message than ASSERT provides.
 *
 * Requires stdio.h to be included, for fprintf.
 */
#define ASSERT_FLOATEQ(expected, got) \
    ASSERT_EQ(long double, "%Lf", expected, got)

/**
 * A short cut for ASSERT(expected == got) for any character type, with a more
 * informative error message than ASSERT provides.
 *
 * Requires stdio.h to be included, for fprintf.
 *
 * TODO: Print non-printing characters better.
 */
#define ASSERT_CHAREQ(expected, got) \
    ASSERT_EQ(int, "'%c'", expected, got)

/**
 * Assert that the two values of type `type` with printf format specifier
 * `format` (e.g., "%lld") are equal.
 *
 * This macro is used to implement some of the other macros in this library.
 * End-users will generally use the more convenient ASSERT_STREQ, ASSERT_INTEQ,
 * etc. instead of this macro.
 *
 * Requires stdio.h to be included, for fprintf.
 */
#define ASSERT_EQ(type, format, expected, got) \
    do { \
        type expectedv = expected; \
        type gotv = got; \
        if (expectedv != gotv) { \
            tests_failed++; \
            fprintf(stderr, "ASSERTION FAILED, %s, line %d:" \
                " expected " format ", got " format " for `%s`.\n", __FILE__, \
                __LINE__, expectedv, gotv, #got); \
        } else { \
            tests_passed++; \
        } \
    } while (0)

#endif

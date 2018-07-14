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
 * A shortcut for ASSERT(strcmp(expected, got) == 0), but with a more
 * informative error message that prints the contents of both strings.
 *
 * Requires string.h to be included for strcmp, and stdio.h for fprintf.
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

#endif

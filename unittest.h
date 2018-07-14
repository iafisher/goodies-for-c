/* Lightweight unit testing.
 *
 * All macros in this module assume the existence of two global int variables,
 * tests_passed and tests_failed, which the macros update. Thus the minimal
 * set-up is something like this:
 *
 *     unsigned int tests_passed = 0;
 *     unsigned int tests_failed = 0;
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
 * Assert that the condition is true. If true, tests_passed is incremented. If
 * false, tests_failed is incremented and an error message that includes the
 * file name, line number, and text of the condition is printed to stderr.
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
            fprintf(stderr, "ASSERTION FAILED, %s:%d: %s\n", \
                __FILE__, __LINE__, #condition); \
        } else { \
            tests_passed++; \
        } \
    } while (0)

#endif

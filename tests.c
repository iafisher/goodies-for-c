/**
 * The test suite for all the modules in this repository.
 *
 * Author:  Ian Fisher (iafisher@protonmail.com)
 * Version: July 2018
 */

#include <stdio.h>
#include <string.h>
#include "qstring.h"
#include "unittest.h"


TEST_MODULE;

const char* helloworld = "Hello, world!";

void test_qstring_new() {
    qstring qs = qstring_new(helloworld);

    ASSERT(qs.data != helloworld); /* Make sure the data is newly allocated. */
    ASSERT_STREQ(helloworld, qs.data);
    ASSERT(qs.len == strlen(helloworld));
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_new_buffer() {
    const char* binmsg = "a\0b\0c";
    qstring qs = qstring_new_buffer(binmsg, 5);

    ASSERT(qs.data != binmsg);
    ASSERT(qs.len == 5);
    ASSERT(qs.data[0] == 'a');
    ASSERT(qs.data[1] == '\0');
    ASSERT(qs.data[2] == 'b');
    ASSERT(qs.data[3] == '\0');
    ASSERT(qs.data[4] == 'c');
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_repeat() {
    qstring qs = qstring_repeat('A', 3);

    ASSERT(qs.len == 3);
    ASSERT(qs.data[0] == 'A');
    ASSERT(qs.data[1] == 'A');
    ASSERT(qs.data[2] == 'A');
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    qs = qstring_repeat('A', 0);

    ASSERT(qs.len == 0);
    ASSERT(qs.data != NULL);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qliteral_new() {
    qstring qs = qliteral_new(helloworld);

    ASSERT(qs.data == helloworld);
    ASSERT(qs.len == strlen(helloworld));
    ASSERT(qs.data[qs.len] == '\0');
}

void test_qstring_copy() {
    qstring qs = qstring_copy(qliteral_new(helloworld));

    ASSERT(qs.data != helloworld);
    ASSERT(qs.len == strlen(helloworld));
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_substr() {
    qstring qs = qstring_substr(qliteral_new(helloworld), 0, 5);

    ASSERT_STREQ("Hello", qs.data);
    ASSERT(qs.len == 5);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    qs = qstring_substr(qliteral_new(helloworld), 7, 6);

    ASSERT_STREQ("world!", qs.data);
    ASSERT(qs.len == 6);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* `start + n` out of bounds */
    qs = qstring_substr(qliteral_new(helloworld), 7, 1000);

    ASSERT_STREQ("world!", qs.data);
    ASSERT(qs.len == 6);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* `start` out of bounds */
    qs = qstring_substr(qliteral_new(helloworld), 1000, 5);

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_remove() {
    qstring qs = qstring_remove(qliteral_new(helloworld), 0, 7);

    ASSERT_STREQ("world!", qs.data);
    ASSERT(qs.len == 6);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Remove up to the end of the string. */
    qs = qstring_remove(qliteral_new(helloworld), 5, strlen(helloworld) - 5);

    ASSERT_STREQ("Hello", qs.data);
    ASSERT(qs.len == 5);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Remove the whole string (should return empty string). */
    qs = qstring_remove(qliteral_new(helloworld), 0, strlen(helloworld));

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Remove past the end of the string (should return empty string). */
    qs = qstring_remove(qliteral_new(helloworld), 0, 100);

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_concat() {
    qstring qs = qstring_concat(qliteral_new("Hello, "), qliteral_new("world!"));

    ASSERT_STREQ(helloworld, qs.data);
    ASSERT(qs.len == strlen(helloworld));
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Concatenate with the empty string. */
    qs = qstring_concat(qliteral_new(helloworld), qliteral_new(""));

    ASSERT_STREQ(helloworld, qs.data);
    ASSERT(qs.len == strlen(helloworld));
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Concatenate the empty string twice. */
    qs = qstring_concat(qliteral_new(""), qliteral_new(""));

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

void test_qstring_format() {
    // TODO
}

void test_qstring_replace() {
    // TODO (replace_all, replace_first, replace_last)
}

void test_qstring_find() {
    // TODO (find, find_in, rfind, rfind_in)
}

void test_qstring_count() {
    // TODO
}

void test_qstring_startswith_endswith() {
    qstring qs = qliteral_new(helloworld);

    ASSERT(qstring_startswith(qs, qliteral_new("")));
    ASSERT(qstring_startswith(qs, qliteral_new("Hello")));
    ASSERT(qstring_startswith(qs, qliteral_new("Hello, world!")));
    ASSERT(!qstring_startswith(qs, qliteral_new("hello")));
    ASSERT(!qstring_startswith(qs, qliteral_new("ello")));

    ASSERT(qstring_endswith(qs, qliteral_new("")));
    ASSERT(qstring_endswith(qs, qliteral_new("world!")));
    ASSERT(qstring_endswith(qs, qliteral_new("Hello, world!")));
    ASSERT(!qstring_endswith(qs, qliteral_new("World!")));
    ASSERT(!qstring_endswith(qs, qliteral_new("world")));
}

void test_qstring_strip() {
    /* Test qstring_lstrip. */
    qstring qs = qstring_lstrip(qliteral_new("abababCCCab"),
            qliteral_new("ba"));

    ASSERT_STREQ("CCCab", qs.data);
    ASSERT(qs.len == 5);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Left-strip the entire string. */
    qs = qstring_lstrip(qliteral_new("012210"), qliteral_new("201"));

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Left-strip with duplicates in strip string. */
    qs = qstring_lstrip(qliteral_new("   text"), qliteral_new("    "));

    ASSERT_STREQ("text", qs.data);
    ASSERT(qs.len == 4);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Test qstring_rstrip. */
    qs = qstring_rstrip(qliteral_new("abababCCCab"), qliteral_new("ba"));

    ASSERT_STREQ("abababCCC", qs.data);
    ASSERT(qs.len == 9);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Right-strip the entire string. */
    qs = qstring_rstrip(qliteral_new(".,!,..."), qliteral_new("!,."));

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Right-strip with duplicates in strip string. */
    qs = qstring_rstrip(qliteral_new("((()))"), qliteral_new("))"));

    ASSERT_STREQ("(((", qs.data);
    ASSERT(qs.len == 3);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Test qstring_strip. */
    qs = qstring_strip(qliteral_new("OXOXO__OX__OXOXO"), qliteral_new("OX"));

    ASSERT_STREQ("__OX__", qs.data);
    ASSERT(qs.len == 6);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Strip the entire string. */
    qs = qstring_strip(qliteral_new("     \t \v\r\n\n\t"),
            qliteral_new(" \t\v\r\n"));

    ASSERT(qs.len == 0);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);

    /* Strip with duplicates in strip string. */
    qs = qstring_strip(qliteral_new("Schenectady"), qliteral_new("SdySd"));

    ASSERT_STREQ("chenecta", qs.data);
    ASSERT(qs.len == 8);
    ASSERT(qs.data[qs.len] == '\0');

    qstring_cleanup(qs);
}

int main() {
    /* Test the qstring library. */
    test_qstring_new();
    test_qstring_new_buffer();
    test_qstring_repeat();
    test_qliteral_new();
    test_qstring_copy();
    test_qstring_substr();
    test_qstring_remove();
    test_qstring_concat();
    test_qstring_format();
    test_qstring_replace();
    test_qstring_find();
    test_qstring_count();
    test_qstring_startswith_endswith();
    test_qstring_strip();

    unsigned int tests_run = tests_failed + tests_passed;
    const char* plural = (tests_run == 1) ? "" : "s";
    if (tests_failed == 0) {
        printf("%d test%s passed.\n", tests_run, plural);
        return 0;
    } else {
        printf("\nFAILURE: %d out of %d test%s failed.\n", tests_failed,
            tests_run, plural);
        return 1;
    }
}

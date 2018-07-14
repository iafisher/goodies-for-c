#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "qstring.h"

qstring qio_readn(FILE* fp, size_t n) {
    qstring ret = {.len = 0, .data = NULL};
    char* data = malloc(n + 1);
    if (data == NULL) {
        return ret;
    }
    fread(data, sizeof(char), n, fp);
    data[n] = '\0';
    ret.data = data;
    ret.len = n;
    return ret;
}

qstring qio_readpath(const char* pathname) {
    qstring ret = {.len = 0, .data = NULL};
    struct stat sbuf;
    int errcode = stat(pathname, &sbuf);
    if (errcode != 0) {
        return ret;
    }
    char* data = malloc(sbuf.st_size + 1);
    if (data == NULL) {
        return ret;
    }
    FILE* fp = fopen(pathname, "r");
    size_t nread = fread(data, sizeof(char), sbuf.st_size, fp);
    data[nread] = '\0';
    ret.data = data;
    ret.len = nread;
    fclose(fp);
    return ret;
}

qstring qio_read(FILE* fp) {
    size_t n = 1024;
    qstring ret = {.len = 0, .data = NULL};
    char* data = malloc(n + 1);
    if (data == NULL) {
        return ret;
    }
    size_t nread = 0;
    while ((nread += fread(data, sizeof(char), n, fp)) == n) {
        n *= 2;
        char* new_data = realloc(data, n + 1);
        if (new_data != NULL) {
            data = new_data;
        } else {
            free(data);
            return ret;
        }
    }
    data[nread] = '\0';
    ret.data = data;
    ret.len = nread;
    return ret;
}

qstring qio_readline(FILE* fp) {
    size_t n = 100;
    qstring ret = {.len = 0, .data = NULL};
    char* data = malloc(n + 1);
    if (data == NULL) {
        return ret;
    }
    size_t pos = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            break;
        }

        data[pos++] = ch;
        if (pos == n) {
            n += 100;
            char* new_data = realloc(data, n + 1);
            if (new_data != NULL) {
                data = new_data;
            } else {
                free(data);
                return ret;
            }
        }
    }
    data[pos] = '\0';
    ret.data = data;
    ret.len = pos;
    return ret;
}

size_t qio_write(FILE* fp, qstring qs) {
    return fwrite(qs.data, sizeof(char), qs.len, fp);
}

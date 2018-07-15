#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "qstring.h"

char* qio_readpath(const char* pathname, size_t* nptr) {
    struct stat sbuf;
    int errcode = stat(pathname, &sbuf);
    if (errcode != 0) {
        return NULL;
    }
    char* data = malloc(sbuf.st_size + 1);
    if (data == NULL) {
        return NULL;
    }
    FILE* fp = fopen(pathname, "r");
    *nptr = fread(data, sizeof(char), sbuf.st_size, fp);
    data[*nptr] = '\0';
    fclose(fp);
    return data;
}

char* qio_readline(FILE* fp, size_t* nptr) {
    size_t readsz = 100;
    char* data = malloc(readsz + 1);
    if (data == NULL) {
        return NULL;
    }
    size_t pos = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            break;
        }

        data[pos++] = ch;
        if (pos == readsz) {
            readsz += 100;
            char* new_data = realloc(data, readsz + 1);
            if (new_data != NULL) {
                data = new_data;
            } else {
                free(data);
                return NULL;
            }
        }
    }
    data[pos] = '\0';
    *nptr = pos;
    return data;
}

qstring qio_readpath_qs(const char* pathname) {
    size_t n;
    char* data = qio_readpath(pathname, &n);
    qstring ret = {.len = n, .data = data};
    return ret;
}

qstring qio_readline_qs(FILE* fp) {
    size_t n;
    char* data = qio_readline(fp, &n);
    qstring ret = {.len = n, .data = data};
    return ret;
}

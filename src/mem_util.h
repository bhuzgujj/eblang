#ifndef EBLANG_TOKENIZER_str_h
#define EBLANG_TOKENIZER_str_h

#include <stdlib.h>

struct mem_slice {
    size_t size;
    void* mem;
};

static char* slice(const char* original, size_t start, size_t end) {
    char* slice = malloc(end - start + 1);
    size_t index = 0;
    for (size_t cursor = start; cursor < end; cursor++, index++) {
        slice[index] = original[cursor];
    }
    slice[index] = '\0';
    return slice;
}

#endif

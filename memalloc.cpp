#include "memalloc.h"

char *memory = NULL;

struct memblock {
    ssize_t len;
    memblock *next;
} *head;

void init() {
    if (memory == NULL) {
        memory = (char*) malloc(HEAP_SIZE + sizeof(ssize_t));
        head = (memblock*) memory;
        head->len = HEAP_SIZE;
        head->next = NULL;
    }
}

void release() {
    if (memory != NULL) {
        free(memory);
    }
    memory = NULL;
}

ssize_t ptblcksize(ssize_t l) {
    if (l < BLOCK_SIZE)
        return BLOCK_SIZE;
    return l + (l % BLOCK_SIZE);
}

void *memalloc(ssize_t bytes) {
    init();
    memblock *it = head, *prev = NULL;
    bytes = ptblcksize(bytes);
    while (it) {
        if (it->len >= bytes) {
            if (it->len - bytes > SPLIT_TRESHOLD) {
                memblock *split = (memblock*) (((char*) it) + bytes + LEN_SIZE);
                split->len = it->len - bytes - LEN_SIZE;
                it->len = bytes;
                split->next = it->next;
                it->next = split;
            }
            if (prev == NULL) {
                head = it->next;
            } else {
                prev->next = it->next;
            }
            return ((char*) it) + LEN_SIZE;
        }
        prev = it;
        it = it->next;
    }
    return NULL;
}

void memfree(void *mem) {
    memblock *block = (memblock*) ((char*)mem - sizeof(ssize_t));
    if (!head) {
        head = block;
        return;
    }
    std::less<void*> l;
    if (!l((char*)head, (char*)block)) {
        if ((char*)block + block->len + LEN_SIZE == (char*)head) {
            block->len = head->len + block->len + LEN_SIZE;
            block->next = head->next;
            head = block;
            return;
        }
        block->next = head;
        head = block;
        return;
    }

    memblock *it = head;
    bool merged = false;
    while (it) {
        if (it < block && (it->next == NULL || it->next > block)) {
            if ((char*)it + it->len + LEN_SIZE == (char*)block) {
                // ...[block][current]...
                it->len += block->len + LEN_SIZE;
                block = it;
                merged = true;
            }
            if ((char*)block + block->len + LEN_SIZE == (char*)it->next) {
                // ...[current][block]...
                block->len += it->next->len + LEN_SIZE;
                block->next = it->next->next;
                merged = true;
            }
            if (merged)
                return;

            block->next = it->next;
            it->next = block;
            return;
        }
        it = it->next;
    }

    throw std::exception();
}

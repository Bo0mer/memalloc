#ifndef MEMALLOC_H_INCLUDED
#define MEMALLOC_H_INCLUDED

#include <cstdlib>
#include <sys/types.h>
#include <functional>
#include <exception>

#define HEAP_SIZE 128*1024
#define BLOCK_SIZE 8
#define PTR_SIZE sizeof(void*)
#define LEN_SIZE sizeof(ssize_t)
#define SPLIT_TRESHOLD 16

void *memalloc(ssize_t bytes);
void memfree(void *mem);

#endif // MEMALLOC_H_INCLUDED

# memalloc
Simple memory allocator

## Usage
Add the files to your project and then: 

```C++
#include "memalloc.h"

...

int *intPtr = (int*) memalloc(sizeof(int));
...
memfree(ptr);

```

## Tunning the heap preferences

ATM this is not possible. It has fixed length of 128*1024 bytes and does NOT support resizing.

## Overhead

For every free block of memory, `sizeof(ssize_t) + sizeof(char*)` will be used.

For every allocated block, the overhead is as big as `sizeof(ssize_t)`.

## Performance

`memalloc` uses first fit as allocation technique, so it gives you the first available block which has size bigger or equal to the one you requested. This means it should be pretty performant.

However ATM `memfree` is not so performant, because it tries to keep the list of free blocks sorted. This means every time you free some memory, it takes O(n) time to find the place of the newly freed block, which in some cases is slow.

## Notes

* `ssize_t` is defined in `sys/types.h`

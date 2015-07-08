#include <iostream>

#include "memalloc.h"

using namespace std;

int main()
{
    int *ptr = (int*) memalloc(sizeof(int));
    *ptr = 4;
    cout << *ptr;
    memfree(ptr);
    return 0;
}

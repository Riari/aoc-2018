#include "hash.h"

#include <stdlib.h>
#include <string.h>

unsigned long hash(const char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

unsigned long hash_int(const void* key)
{
    int value = *(const int*)key;
    unsigned long h = (unsigned long)value;
    h = ((h >> 16) ^ h) * 0x45d9f3b;
    h = ((h >> 16) ^ h) * 0x45d9f3b;
    h = (h >> 16) ^ h;
    return h;
}

unsigned long hash_string(const void* key)
{
    return hash((const char*)key);
}

int compare_int(const void* a, const void* b)
{
    int val_a = *(const int*)a;
    int val_b = *(const int*)b;
    return val_a - val_b;
}

int compare_string(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}

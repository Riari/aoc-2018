#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct SetNode
{
    void* key;
    struct SetNode *next;
} SetNode;

typedef struct
{
    SetNode** buckets;
    size_t size;
    size_t capacity;
    unsigned long (*hash_func)(const void*);
    int (*compare_func)(const void*, const void*);
    void (*free_func)(void*);
} Set;

// Create a set with custom hash, compare, and free functions
Set* set_create(unsigned long (*hash_func)(const void*),
                  int (*compare_func)(const void*, const void*),
                  void (*free_func)(void*));

// Helper functions to create common set types
Set* set_create_int(void);
Set* set_create_string(void);

void set_insert(Set* set, const void* key);

bool set_contains(Set* set, const void* key);

bool set_remove(Set* set, const void* key);

void set_resize(Set* set);

void set_destroy(Set* set);

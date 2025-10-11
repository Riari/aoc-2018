#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct set_node_t
{
    void* key;
    struct set_node_t *next;
} set_node_t;

typedef struct
{
    set_node_t** buckets;
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

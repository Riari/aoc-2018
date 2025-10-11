#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct HashSet HashSet;

typedef unsigned long (*HashFunc)(const void* key);
typedef int (*CompareFunc)(const void* a, const void* b);

// Creates a new hash set with initial capacity
HashSet* hash_set_create(size_t capacity, HashFunc hash_func, CompareFunc compare_func);

// Destroys the hash set and frees memory
void hash_set_destroy(HashSet* set);

// Inserts an element into the set (returns true if inserted, false if already exists)
bool hash_set_insert(HashSet* set, void* key);

bool hash_set_contains(HashSet* set, const void* key);

// Removes an element from the set (returns the removed key, or NULL if not found)
// Caller is responsible for freeing the returned key if needed
void* hash_set_remove(HashSet* set, const void* key);

size_t hash_set_size(HashSet* set);

bool hash_set_is_empty(HashSet* set);

void hash_set_clear(HashSet* set);

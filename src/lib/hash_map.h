#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct HashMap HashMap;

typedef unsigned long (*HashFunc)(const void* key);
typedef int (*CompareFunc)(const void* a, const void* b);
typedef void (*HashMapIterator)(void* key, void* value, void* user_data);

// Creates a new hash map with initial capacity
HashMap* hash_map_create(size_t capacity, HashFunc hash_func, CompareFunc compare_func);

// Destroys the hash map and frees memory
void hash_map_destroy(HashMap* map);

// Inserts or updates a key-value pair (returns true if new key inserted, false if updated)
bool hash_map_put(HashMap* map, void* key, void* value);

// Gets the value associated with a key (returns NULL if not found)
void* hash_map_get(HashMap* map, const void* key);

bool hash_map_contains_key(HashMap* map, const void* key);

// Removes a key-value pair (returns the removed key, or NULL if not found)
// Caller is responsible for freeing the returned key if needed
void* hash_map_remove(HashMap* map, const void* key);

size_t hash_map_size(HashMap* map);

bool hash_map_is_empty(HashMap* map);

void hash_map_clear(HashMap* map);

void hash_map_foreach(HashMap* map, HashMapIterator iterator, void* user_data);

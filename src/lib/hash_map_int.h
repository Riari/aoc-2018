#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct HashMapInt HashMapInt;

// Creates a new integer hash map with initial capacity
HashMapInt* hash_map_int_create(size_t capacity);

// Destroys the hash map and frees memory
void hash_map_int_destroy(HashMapInt* map);

// Inserts or updates a key-value pair (returns true if new key inserted, false if updated)
bool hash_map_int_put(HashMapInt* map, int key, int value);

// Gets the value associated with a key (returns true if found, false otherwise)
bool hash_map_int_get(HashMapInt* map, int key, int* out_value);

bool hash_map_int_contains_key(HashMapInt* map, int key);

// Removes a key-value pair (returns true if removed, false if not found)
bool hash_map_int_remove(HashMapInt* map, int key);

size_t hash_map_int_size(HashMapInt* map);

bool hash_map_int_is_empty(HashMapInt* map);

void hash_map_int_clear(HashMapInt* map);

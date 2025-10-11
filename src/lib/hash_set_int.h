#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct HashSetInt HashSetInt;

// Creates a new integer hash set with initial capacity
HashSetInt* hash_set_int_create(size_t capacity);

// Destroys the hash set and frees memory
void hash_set_int_destroy(HashSetInt* set);

// Inserts an integer into the set (returns true if inserted, false if already exists)
bool hash_set_int_insert(HashSetInt* set, int value);

bool hash_set_int_contains(HashSetInt* set, int value);

// Removes an integer from the set (returns true if removed, false if not found)
bool hash_set_int_remove(HashSetInt* set, int value);

size_t hash_set_int_size(HashSetInt* set);

bool hash_set_int_is_empty(HashSetInt* set);

void hash_set_int_clear(HashSetInt* set);

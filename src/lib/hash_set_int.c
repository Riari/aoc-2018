#include "hash_set_int.h"

#include <stdlib.h>

#include "hash_map.h"
#include "hash.h"

struct HashSetInt
{
    HashMap* map;
};

static void free_key_callback(void* key, void* value, void* user_data)
{
    (void)value;
    (void)user_data;
    free(key);
}

HashSetInt* hash_set_int_create(size_t capacity)
{
    HashSetInt* set = malloc(sizeof(HashSetInt));
    if (!set) return NULL;

    set->map = hash_map_create(capacity, hash_int, compare_int);
    if (!set->map)
    {
        free(set);
        return NULL;
    }

    return set;
}

void hash_set_int_destroy(HashSetInt* set)
{
    if (!set) return;

    // Free all allocated integer keys
    hash_map_foreach(set->map, free_key_callback, NULL);

    hash_map_destroy(set->map);
    free(set);
}

bool hash_set_int_insert(HashSetInt* set, int value)
{
    if (!set) return false;

    int* key_ptr = malloc(sizeof(int));
    if (!key_ptr) return false;

    *key_ptr = value;

    // If the key already exists, hash_map_put returns false and we need to free our allocation
    if (!hash_map_put(set->map, key_ptr, NULL))
    {
        free(key_ptr);
        return false;
    }

    return true;
}

bool hash_set_int_contains(HashSetInt* set, int value)
{
    if (!set) return false;
    return hash_map_contains_key(set->map, &value);
}

bool hash_set_int_remove(HashSetInt* set, int value)
{
    if (!set) return false;

    int temp = value;
    void* removed_key = hash_map_remove(set->map, &temp);

    if (removed_key)
    {
        free(removed_key);
        return true;
    }

    return false;
}

size_t hash_set_int_size(HashSetInt* set)
{
    return set ? hash_map_size(set->map) : 0;
}

bool hash_set_int_is_empty(HashSetInt* set)
{
    return set ? hash_map_is_empty(set->map) : true;
}

void hash_set_int_clear(HashSetInt* set)
{
    if (!set) return;

    // Free all allocated integer keys
    hash_map_foreach(set->map, free_key_callback, NULL);

    hash_map_clear(set->map);
}

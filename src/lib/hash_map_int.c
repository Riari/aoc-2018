#include "hash_map_int.h"

#include <stdlib.h>

#include "hash_map.h"
#include "hash.h"

struct HashMapInt
{
    HashMap* map;
};

static void free_key_value_callback(void* key, void* value, void* user_data)
{
    (void)user_data;
    free(key);
    free(value);
}

HashMapInt* hash_map_int_create(size_t capacity)
{
    HashMapInt* map = malloc(sizeof(HashMapInt));
    if (!map) return NULL;

    map->map = hash_map_create(capacity, hash_int, compare_int);
    if (!map->map)
    {
        free(map);
        return NULL;
    }

    return map;
}

void hash_map_int_destroy(HashMapInt* map)
{
    if (!map) return;

    hash_map_foreach(map->map, free_key_value_callback, NULL);

    hash_map_destroy(map->map);
    free(map);
}

bool hash_map_int_put(HashMapInt* map, int key, int value)
{
    if (!map) return false;

    int* existing_value = hash_map_get(map->map, &key);
    if (existing_value)
    {
        // Key exists, just update the value in place
        *existing_value = value;
        return false;
    }

    // Key doesn't exist, allocate both key and value
    int* key_ptr = malloc(sizeof(int));
    int* value_ptr = malloc(sizeof(int));
    if (!key_ptr || !value_ptr)
    {
        free(key_ptr);
        free(value_ptr);
        return false;
    }

    *key_ptr = key;
    *value_ptr = value;

    if (!hash_map_put(map->map, key_ptr, value_ptr))
    {
        // This shouldn't happen since we checked above
        free(key_ptr);
        free(value_ptr);
        return false;
    }

    return true;
}

bool hash_map_int_get(HashMapInt* map, int key, int* out_value)
{
    if (!map || !out_value) return false;

    int* value_ptr = hash_map_get(map->map, &key);
    if (!value_ptr) return false;

    *out_value = *value_ptr;
    return true;
}

bool hash_map_int_contains_key(HashMapInt* map, int key)
{
    if (!map) return false;
    return hash_map_contains_key(map->map, &key);
}

bool hash_map_int_remove(HashMapInt* map, int key)
{
    if (!map) return false;

    int* value_ptr = hash_map_get(map->map, &key);
    if (!value_ptr) return false;

    int temp = key;
    void* removed_key = hash_map_remove(map->map, &temp);

    if (removed_key)
    {
        free(removed_key);
        free(value_ptr);
        return true;
    }

    return false;
}

size_t hash_map_int_size(HashMapInt* map)
{
    return map ? hash_map_size(map->map) : 0;
}

bool hash_map_int_is_empty(HashMapInt* map)
{
    return map ? hash_map_is_empty(map->map) : true;
}

void hash_map_int_clear(HashMapInt* map)
{
    if (!map) return;

    hash_map_foreach(map->map, free_key_value_callback, NULL);

    hash_map_clear(map->map);
}

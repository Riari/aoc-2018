#include "hash_set.h"
#include "hash_map.h"
#include <stdlib.h>

struct HashSet
{
    HashMap* map;
};

HashSet* hash_set_create(size_t capacity, HashFunc hash_func, CompareFunc compare_func)
{
    HashSet* set = malloc(sizeof(HashSet));
    if (!set) return NULL;

    set->map = hash_map_create(capacity, hash_func, compare_func);
    if (!set->map)
    {
        free(set);
        return NULL;
    }

    return set;
}

void hash_set_destroy(HashSet* set)
{
    if (!set) return;

    hash_map_destroy(set->map);
    free(set);
}

bool hash_set_insert(HashSet* set, void* key)
{
    if (!set) return false;
    return hash_map_put(set->map, key, NULL);
}

bool hash_set_contains(HashSet* set, const void* key)
{
    if (!set) return false;
    return hash_map_contains_key(set->map, key);
}

void* hash_set_remove(HashSet* set, const void* key)
{
    if (!set) return NULL;
    return hash_map_remove(set->map, key);
}

size_t hash_set_size(HashSet* set)
{
    return set ? hash_map_size(set->map) : 0;
}

bool hash_set_is_empty(HashSet* set)
{
    return set ? hash_map_is_empty(set->map) : true;
}

void hash_set_clear(HashSet* set)
{
    if (!set) return;
    hash_map_clear(set->map);
}

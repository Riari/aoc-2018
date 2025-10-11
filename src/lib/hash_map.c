#include "hash_map.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

typedef struct HashMapNode
{
    void* key;
    void* value;
    struct HashMapNode* next;
} HashMapNode;

struct HashMap
{
    HashMapNode** buckets;
    size_t capacity;
    size_t size;
    HashFunc hash_func;
    CompareFunc compare_func;
};

HashMap* hash_map_create(size_t capacity, HashFunc hash_func, CompareFunc compare_func)
{
    if (capacity == 0)
    {
        capacity = INITIAL_CAPACITY;
    }

    HashMap* map = malloc(sizeof(HashMap));
    if (!map) return NULL;

    map->buckets = calloc(capacity, sizeof(HashMapNode*));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }

    map->capacity = capacity;
    map->size = 0;
    map->hash_func = hash_func;
    map->compare_func = compare_func;

    return map;
}

void hash_map_destroy(HashMap* map)
{
    if (!map) return;

    hash_map_clear(map);
    free(map->buckets);
    free(map);
}

static void hash_map_resize(HashMap* map)
{
    size_t new_capacity = map->capacity * 2;
    HashMapNode** new_buckets = calloc(new_capacity, sizeof(HashMapNode*));
    if (!new_buckets) return;

    for (size_t i = 0; i < map->capacity; i++)
    {
        HashMapNode* node = map->buckets[i];
        while (node)
        {
            HashMapNode* next = node->next;
            unsigned long hash = map->hash_func(node->key);
            size_t index = hash % new_capacity;
            node->next = new_buckets[index];
            new_buckets[index] = node;
            node = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}

bool hash_map_put(HashMap* map, void* key, void* value)
{
    if (!map || !key) return false;

    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity;

    HashMapNode* current = map->buckets[index];
    while (current)
    {
        if (map->compare_func(current->key, key) == 0)
        {
            current->value = value;
            return false;
        }
        current = current->next;
    }

    HashMapNode* new_node = malloc(sizeof(HashMapNode));
    if (!new_node) return false;

    new_node->key = key;
    new_node->value = value;
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
    map->size++;

    if ((double)map->size / map->capacity > LOAD_FACTOR)
    {
        hash_map_resize(map);
    }

    return true;
}

void* hash_map_get(HashMap* map, const void* key)
{
    if (!map || !key) return NULL;

    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity;

    HashMapNode* current = map->buckets[index];
    while (current)
    {
        if (map->compare_func(current->key, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

bool hash_map_contains_key(HashMap* map, const void* key)
{
    if (!map || !key) return false;

    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity;

    HashMapNode* current = map->buckets[index];
    while (current)
    {
        if (map->compare_func(current->key, key) == 0)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

void* hash_map_remove(HashMap* map, const void* key)
{
    if (!map || !key) return NULL;

    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity;

    HashMapNode* current = map->buckets[index];
    HashMapNode* prev = NULL;

    while (current)
    {
        if (map->compare_func(current->key, key) == 0)
        {
            void* removed_key = current->key;

            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                map->buckets[index] = current->next;
            }

            free(current);
            map->size--;
            return removed_key;
        }
        prev = current;
        current = current->next;
    }

    return NULL;
}

size_t hash_map_size(HashMap* map)
{
    return map ? map->size : 0;
}

bool hash_map_is_empty(HashMap* map)
{
    return map ? map->size == 0 : true;
}

void hash_map_clear(HashMap* map)
{
    if (!map) return;

    for (size_t i = 0; i < map->capacity; i++)
    {
        HashMapNode* current = map->buckets[i];
        while (current)
        {
            HashMapNode* next = current->next;
            free(current);
            current = next;
        }

        map->buckets[i] = NULL;
    }

    map->size = 0;
}

void hash_map_foreach(HashMap* map, HashMapIterator iterator, void* user_data)
{
    if (!map || !iterator) return;

    for (size_t i = 0; i < map->capacity; i++)
    {
        HashMapNode* current = map->buckets[i];
        while (current)
        {
            iterator(current->key, current->value, user_data);
            current = current->next;
        }
    }
}

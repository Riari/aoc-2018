#include "map.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

Map* map_create(unsigned long (*hash_func)(const void*),
                int (*compare_func)(const void*, const void*),
                void (*free_key_func)(void*),
                void (*free_value_func)(void*))
{
    Map* map = malloc(sizeof(Map));
    map->capacity = 16;
    map->size = 0;
    map->hash_func = hash_func;
    map->compare_func = compare_func;
    map->free_key_func = free_key_func;
    map->free_value_func = free_value_func;
    map->buckets = calloc(map->capacity, sizeof(MapNode*));
    return map;
}

Map* map_create_int_key(void (*free_value_func)(void*))
{
    return map_create(hash_int, compare_int, free, free_value_func);
}

Map* map_create_string_key(void (*free_value_func)(void*))
{
    return map_create(hash_string, compare_string, free, free_value_func);
}

static void map_resize(Map* map)
{
    size_t old_capacity = map->capacity;
    MapNode** old_buckets = map->buckets;

    map->capacity *= 2;
    map->buckets = calloc(map->capacity, sizeof(MapNode*));
    map->size = 0;

    // Rehash all elements
    for (size_t i = 0; i < old_capacity; i++)
    {
        MapNode* node = old_buckets[i];
        while (node != NULL)
        {
            MapNode* next = node->next;
            map_put(map, node->key, node->value);
            free(node);
            node = next;
        }
    }

    free(old_buckets);
}

void map_put(Map* map, const void* key, void* value)
{
    if (map->size >= map->capacity * 0.75)
    {
        map_resize(map);
    }

    unsigned long h = map->hash_func(key) % map->capacity;
    MapNode* node = map->buckets[h];

    while (node != NULL)
    {
        if (map->compare_func(node->key, key) == 0)
        {
            if (map->free_value_func && node->value != value)
            {
                map->free_value_func(node->value);
            }
            node->value = value;
            return;
        }
        node = node->next;
    }

    MapNode* new_node = malloc(sizeof(MapNode));

    if (map->hash_func == hash_int)
    {
        new_node->key = malloc(sizeof(int));
        *(int*)new_node->key = *(const int*)key;
    }
    else if (map->hash_func == hash_string)
    {
        new_node->key = strdup((const char*)key);
    }
    else
    {
        // For custom types, assume key is already allocated
        new_node->key = (void*)key;
    }

    new_node->value = value;
    new_node->next = map->buckets[h];
    map->buckets[h] = new_node;
    map->size++;
}

void* map_get(const Map* map, const void* key)
{
    unsigned long h = map->hash_func(key) % map->capacity;
    MapNode* node = map->buckets[h];

    while (node != NULL)
    {
        if (map->compare_func(node->key, key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

bool map_contains(const Map* map, const void* key)
{
    return map_get(map, key) != NULL;
}

bool map_remove(Map* map, const void* key)
{
    unsigned long h = map->hash_func(key) % map->capacity;
    MapNode* node = map->buckets[h];
    MapNode* prev = NULL;

    while (node != NULL)
    {
        if (map->compare_func(node->key, key) == 0)
        {
            if (prev == NULL)
            {
                map->buckets[h] = node->next;
            }
            else
            {
                prev->next = node->next;
            }

            if (map->free_key_func)
            {
                map->free_key_func(node->key);
            }

            if (map->free_value_func)
            {
                map->free_value_func(node->value);
            }

            free(node);
            map->size--;
            return true;
        }

        prev = node;
        node = node->next;
    }

    return false;
}

void map_clear(Map* map)
{
    for (size_t i = 0; i < map->capacity; ++i)
    {
        MapNode* node = map->buckets[i];
        while (node != NULL)
        {
            MapNode* next = node->next;

            if (map->free_key_func)
            {
                map->free_key_func(node->key);
            }

            if (map->free_value_func)
            {
                map->free_value_func(node->value);
            }

            free(node);
            node = next;
        }

        map->buckets[i] = NULL;
    }

    map->size = 0;
}

void map_destroy(Map* map)
{
    if (map == NULL) return;

    map_clear(map);
    free(map->buckets);
    free(map);
}

void map_put_int(Map* map, const void* key, int value)
{
    map_put(map, key, (void*)(intptr_t)value);
}

bool map_get_int(const Map* map, const void* key, int* out_value)
{
    void* value = map_get(map, key);
    if (value == NULL) return false;
    *out_value = (int)(intptr_t)value;
    return true;
}

size_t map_size(const Map* map)
{
    return map->size;
}

bool map_is_empty(const Map* map)
{
    return map->size == 0;
}

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct MapNode
{
    void* key;
    void* value;
    struct MapNode* next;
} MapNode;

typedef struct
{
    MapNode** buckets;
    size_t size;
    size_t capacity;
    unsigned long (*hash_func)(const void*);
    int (*compare_func)(const void*, const void*);
    void (*free_key_func)(void*);
    void (*free_value_func)(void*);
} Map;

Map* map_create(unsigned long (*hash_func)(const void*),
                int (*compare_func)(const void*, const void*),
                void (*free_key_func)(void*),
                void (*free_value_func)(void*));

Map* map_create_int_key(void (*free_value_func)(void*));
Map* map_create_string_key(void (*free_value_func)(void*));

void map_put(Map* map, const void* key, void* value);
void* map_get(const Map* map, const void* key);
bool map_contains(const Map* map, const void* key);
bool map_remove(Map* map, const void* key);
void map_clear(Map* map);
void map_destroy(Map* map);

void map_put_int(Map* map, const void* key, int value);
bool map_get_int(const Map* map, const void* key, int* out_value);

size_t map_size(const Map* map);
bool map_is_empty(const Map* map);

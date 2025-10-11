#include "set.h"

#include <stdlib.h>
#include <string.h>

#include "hash.h"

Set* set_create(unsigned long (*hash_func)(const void*),
                  int (*compare_func)(const void*, const void*),
                  void (*free_func)(void*))
{
    Set* set = malloc(sizeof(Set));
    set->capacity = 16;
    set->size = 0;
    set->hash_func = hash_func;
    set->compare_func = compare_func;
    set->free_func = free_func;
    set->buckets = calloc(set->capacity, sizeof(set_node_t*));
    return set;
}

Set* set_create_int(void)
{
    return set_create(hash_int, compare_int, free_int);
}

Set* set_create_string(void)
{
    return set_create(hash_string, compare_string, free_string);
}

void set_insert(Set* set, const void* key)
{
    if (set->size >= set->capacity * 0.75)
        set_resize(set);

    unsigned long h = set->hash_func(key) % set->capacity;
    set_node_t* node = set->buckets[h];

    while (node != NULL)
    {
        if (set->compare_func(node->key, key) == 0)
            return;
        node = node->next;
    }

    set_node_t* new_node = malloc(sizeof(set_node_t));
    
    // For integers, allocate and copy the value
    // For strings, strdup is called by the user before passing
    if (set->hash_func == hash_int)
    {
        new_node->key = malloc(sizeof(int));
        *(int*)new_node->key = *(const int*)key;
    }
    else if (set->hash_func == hash_string)
    {
        new_node->key = strdup((const char*)key);
    }
    else
    {
        // For custom types, assume key is already allocated
        new_node->key = (void*)key;
    }
    
    new_node->next = set->buckets[h];
    set->buckets[h] = new_node;
    set->size++;
}

bool set_contains(Set *set, const void *key)
{
    unsigned long h = set->hash_func(key) % set->capacity;
    set_node_t* node = set->buckets[h];

    while (node != NULL)
    {
        if (set->compare_func(node->key, key) == 0)
            return true;

        node = node->next;
    }

    return false;
}

bool set_remove(Set *set, const void *key)
{
    unsigned long h = set->hash_func(key) % set->capacity;
    set_node_t* node = set->buckets[h];
    set_node_t* prev = NULL;

    while (node != NULL)
    {
        if (set->compare_func(node->key, key) == 0)
        {
            if (prev == NULL)
                set->buckets[h] = node->next;
            else
                prev->next = node->next;

            if (set->free_func)
                set->free_func(node->key);
            free(node);
            set->size--;

            return true;
        }
        prev = node;
        node = node->next;
    }

    return false;
}

void set_resize(Set *set)
{
    size_t old_capacity = set->capacity;
    set_node_t** old_buckets = set->buckets;

    set->capacity *= 2;
    set->buckets = calloc(set->capacity, sizeof(set_node_t*));
    set->size = 0;

    for (size_t i = 0; i < old_capacity; ++i)
    {
        set_node_t* node = old_buckets[i];
        while (node != NULL)
        {
            set_node_t* next = node->next;

            unsigned long h = set->hash_func(node->key) % set->capacity;
            node->next = set->buckets[h];
            set->buckets[h] = node;
            set->size++;

            node = next;
        }
    }

    free(old_buckets);
}

void set_destroy(Set *set)
{
    for (size_t i = 0; i < set->capacity; ++i)
    {
        set_node_t* node = set->buckets[i];
        while (node != NULL)
        {
            set_node_t* next = node->next;
            if (set->free_func)
                set->free_func(node->key);
            free(node);
            node = next;
        }
    }

    free(set->buckets);
    free(set);
}

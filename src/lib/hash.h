#pragma once

// DJB2 hash function
unsigned long hash(const char* str);

unsigned long hash_int(const void* key);

unsigned long hash_string(const void* key);

int compare_int(const void* a, const void* b);

int compare_string(const void* a, const void* b);

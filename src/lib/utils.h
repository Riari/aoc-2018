#pragma once

#include <time.h>

// File I/O

char* read_file(const char *filename);

typedef struct {
    char **lines;
    size_t count;
} Lines;

Lines read_lines(const char *filename);
void free_lines(Lines lines);

// Timer

typedef struct {
    clock_t start_time;
} Timer;

Timer start_timer(void);
double end_timer(Timer timer);

// Comparison

int compare_int_asc(const void* a, const void* b);
int compare_int_desc(const void* a, const void* b);

#pragma once

#include <time.h>

// File utilities

char* read_file(const char *filename);

typedef struct {
    char **lines;
    size_t count;
} lines_t;

lines_t read_lines(const char *filename);
void free_lines(lines_t lines);

// Timer utilities

typedef struct {
    clock_t start_time;
} timer_t;

timer_t start_timer(void);
double end_timer(timer_t timer);

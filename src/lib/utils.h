#pragma once

#include <time.h>

// File utilities

char* read_file(const char *filename);

typedef struct {
    char **lines;
    size_t count;
} Lines;

Lines read_lines(const char *filename);
void free_lines(Lines lines);

// Timer utilities

typedef struct {
    clock_t start_time;
} Timer;

Timer start_timer(void);
double end_timer(Timer timer);

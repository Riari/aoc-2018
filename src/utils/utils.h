#pragma once

#include <time.h>

// Utility function to read input file
char* read_input_file(const char *filename);

// Timer utilities
typedef struct {
    clock_t start_time;
} timer_t;

timer_t start_timer(void);
double end_timer(timer_t timer);

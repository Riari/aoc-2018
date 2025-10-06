#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* read_input_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return NULL;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate buffer and read file
    char *buffer = malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    return buffer;
}

timer_t start_timer(void) {
    timer_t timer;
    timer.start_time = clock();
    return timer;
}

double end_timer(timer_t timer) {
    clock_t end_time = clock();
    return ((double)(end_time - timer.start_time)) / CLOCKS_PER_SEC;
}

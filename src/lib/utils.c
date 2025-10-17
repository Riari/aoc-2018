#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* read_file(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Failed to open %s\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    return buffer;
}

Lines read_lines(const char *filename) {
    Lines result = {NULL, 0};

    char *content = read_file(filename);
    if (!content)
    {
        return result;
    }

    size_t line_count = 0;
    for (char *p = content; *p; p++)
    {
        if (*p == '\n')
        {
            line_count++;
        }
    }

    if (content[0] != '\0' && content[strlen(content) - 1] != '\n')
    {
        line_count++;
    }

    result.lines = malloc(line_count * sizeof(char*));
    if (!result.lines)
    {
        fprintf(stderr, "Failed to allocate memory for lines\n");
        free(content);
        return result;
    }

    result.count = 0;
    char *line_start = content;
    char *p = content;

    while (*p)
    {
        if (*p == '\n')
        {
            size_t len = p - line_start;
            result.lines[result.count] = malloc(len + 1);
            if (!result.lines[result.count]) {
                fprintf(stderr, "Failed to allocate memory for line\n");
                free_lines(result);
                free(content);
                result.lines = NULL;
                result.count = 0;
                return result;
            }
            memcpy(result.lines[result.count], line_start, len);
            result.lines[result.count][len] = '\0';
            result.count++;
            line_start = p + 1;
        }
        p++;
    }

    if (line_start < p)
    {
        size_t len = p - line_start;
        result.lines[result.count] = malloc(len + 1);
        if (!result.lines[result.count])
        {
            fprintf(stderr, "Failed to allocate memory for line\n");
            free_lines(result);
            free(content);
            result.lines = NULL;
            result.count = 0;
            return result;
        }
        memcpy(result.lines[result.count], line_start, len);
        result.lines[result.count][len] = '\0';
        result.count++;
    }

    free(content);
    return result;
}

void free_lines(Lines lines)
{
    if (lines.lines)
    {
        for (size_t i = 0; i < lines.count; i++)
        {
            free(lines.lines[i]);
        }
        free(lines.lines);
    }
}

Timer start_timer(void)
{
    Timer timer;
    timer.start_time = clock();
    return timer;
}

double end_timer(Timer timer)
{
    clock_t end_time = clock();
    return ((double)(end_time - timer.start_time)) / CLOCKS_PER_SEC;
}

int compare_int_asc(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

int compare_int_desc(const void* a, const void* b)
{
    return *(const int*)b - *(const int*)a;
}

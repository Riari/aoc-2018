#pragma once

#include <stdbool.h>

typedef struct
{
    void (*part1)(void);
    void (*part2)(void);
    bool (*test_part1)(void);
    bool (*test_part2)(void);
} Solution;

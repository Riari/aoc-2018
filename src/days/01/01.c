#include "01.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include "cc.h"
#include "utils.h"

static int solve_p1(Lines lines)
{
    int frequency = 0;
    for (size_t i = 0; i < lines.count; ++i)
    {
        int value = atoi(lines.lines[i]);
        frequency += value;
    }

    return frequency;
}

static int solve_p2(Lines lines)
{
    cc_set(int) set;
    cc_init(&set);

    int frequency = 0;
    bool found_repeat = false;
    while (!found_repeat)
    {
        for (size_t i = 0; i < lines.count; ++i)
        {
            int value = atoi(lines.lines[i]);
            frequency += value;
            int* current = cc_get(&set, frequency);
            if (current)
            {
                found_repeat = true;
                break;
            }

            cc_insert(&set, frequency);
        }
    }

    cc_cleanup(&set);

    return frequency;
}

void day01_part1(void)
{
    Lines lines = read_lines("01_input.txt");
    if (!lines.lines) return;

    int result = solve_p1(lines);
    printf("%d", result);

    free_lines(lines);
}

void day01_part2(void)
{
    Lines lines = read_lines("01_input.txt");
    if (!lines.lines) return;

    int result = solve_p2(lines);
    printf("%d", result);

    free_lines(lines);
}

bool day01_test_part1(void)
{
    Lines lines = read_lines("01_test_input.txt");
    if (!lines.lines) return false;

    int result = solve_p1(lines);

    free_lines(lines);
    return result == 3;
}

bool day01_test_part2(void)
{
    Lines lines = read_lines("01_test_input.txt");
    if (!lines.lines) return false;

    int result = solve_p2(lines);

    free_lines(lines);
    return result == 2;
}

const Solution day01 = {
    day01_part1,
    day01_part2,
    day01_test_part1,
    day01_test_part2
};

#include "02.h"

#include <stdio.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include "cc.h"
#include "utils.h"

int solve_p1(Lines* lines)
{
    int count_two = 0;
    int count_three = 0;
    bool counted_two = false;
    bool counted_three = false;

    cc_map(char, int) map;
    cc_init(&map);
    for (int i = 0; i < lines->count; ++i) 
    {
        char* line = lines->lines[i];
        int len = strlen(line);
        for (int j = 0; j < len; ++j)
        {
            int count = 0;
            int* current = cc_get(&map, line[j]);
            cc_insert(&map, line[j], current ? *current + 1 : 1);
        }

        cc_for_each(&map, val)
        {
            if (*val == 2 && !counted_two)
            {
                count_two += 1;
                counted_two = true;
            }
            else if (*val == 3 && !counted_three)
            {
                count_three += 1;
                counted_three = true;
            }
        }

        counted_two = false;
        counted_three = false;
        cc_clear(&map);
    }

    cc_cleanup(&map);

    return count_two * count_three;
}

void solve_p2(Lines* lines, char out_result[])
{
    // Crude solution - could use the maps produced in p1, but this is easier
    for (int i = 0; i < lines->count; ++i)
    {
        char* line_a = lines->lines[i];
        for (int j = 0; j < lines->count; ++j)
        {
            if (i == j) continue;

            char* line_b = lines->lines[j];

            int diff_count = 0;
            int diff_index = 0;
            int char_index = 0;
            for (int k = 0; k < strlen(line_a); ++k)
            {
                char a = line_a[k];
                char b = line_b[k];

                if (a != b)
                {
                    ++diff_count;
                    diff_index = k;
                }
                else
                {
                    out_result[char_index++] = a;
                }
            }

            if (diff_count == 1)
            {
                out_result[char_index] = 0;
                return;
            }
            else
            {
                out_result[0] = 0;
            }
        }
    }
}

void day02_part1(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    int result = solve_p1(&lines);
    printf("%d", result);

    free_lines(lines);
}

void day02_part2(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    char id[32];
    solve_p2(&lines, id);
    printf("%s", id);

    free_lines(lines);
}

bool day02_test_part1(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    int result = solve_p1(&lines);

    free_lines(lines);
    return result == 12;
}

bool day02_test_part2(void)
{
    Lines lines = read_lines("02_p2_test_input.txt");
    if (!lines.lines) return false;

    char id[32];
    solve_p2(&lines, id);

    bool result = strcmp(id, "fgij") == 0;

    free_lines(lines);
    return result;
}

const Solution day02 = {
    day02_part1,
    day02_part2,
    day02_test_part1,
    day02_test_part2
};

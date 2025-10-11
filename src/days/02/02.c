#include "02.h"

#include <stdio.h>
#include <string.h>

#define NAME str_int_map
#define KEY_TY char
#define VAL_TY int
#include "verstable.h"

#include "utils.h"

int solve(Lines* lines)
{
    int count_two = 0;
    int count_three = 0;
    bool counted_two = false;
    bool counted_three = false;

    str_int_map map;
    vt_init(&map);
    for (int i = 0; i < lines->count; ++i) 
    {
        char* line = lines->lines[i];
        int len = strlen(line);
        for (int j = 0; j < len; ++j)
        {
            int count = 0;
            str_int_map_itr iter = vt_get(&map, line[j]);
            if (vt_is_end(iter))
            {
                vt_insert(&map, line[j], 1);
            }
            else
            {
                vt_insert(&map, line[j], iter.data->val + 1);
            }
        }

        for (
            str_int_map_itr iter = vt_first(&map);
            !vt_is_end(iter);
            iter = vt_next(iter)
        )
        {
            if (iter.data->val == 2 && !counted_two)
            {
                count_two += 1;
                counted_two = true;
            }
            else if (iter.data->val == 3 && !counted_three)
            {
                count_three += 1;
                counted_three = true;
            }
        }

        counted_two = false;
        counted_three = false;
        vt_clear(&map);
    }

    vt_cleanup(&map);

    return count_two * count_three;
}

void day02_part1(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    int result = solve(&lines);
    printf("%d\n", result);

    free_lines(lines);
}

void day02_part2(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    printf("\n");

    free_lines(lines);
}

bool day02_test_part1(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines);

    free_lines(lines);
    return result == 12;
}

bool day02_test_part2(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

const Solution day02 = {
    day02_part1,
    day02_part2,
    day02_test_part1,
    day02_test_part2
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "days/solution.h"

#include "days/01/01.h"

static const solution_t *days[] = {
    &day01,
};

static const int num_days = sizeof(days) / sizeof(days[0]);

void print_usage(const char *program_name)
{
    printf("Usage: %s <day> [part] [test]\n", program_name);
    printf("  day: Day number (1-25)\n");
    printf("  part: 1 or 2 (optional, defaults to both)\n");
    printf("  test: 'test' to run tests instead of main solution\n");
    printf("\nExamples:\n");
    printf("  %s 1          # Run both parts of day 1\n", program_name);
    printf("  %s 1 1        # Run only part 1 of day 1\n", program_name);
    printf("  %s 1 test     # Run tests for day 1\n", program_name);
    printf("  %s 1 1 test   # Run test for part 1 of day 1\n", program_name);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    int day_num = atoi(argv[1]);
    if (day_num < 1 || day_num > 25)
    {
        fprintf(stderr, "Error: Day must be between 1 and 25\n");
        return 1;
    }

    const solution_t *day = NULL;
    for (int i = 0; i < num_days; i++)
    {
        if (days[i]->day == day_num)
        {
            day = days[i];
            break;
        }
    }

    if (!day)
    {
        fprintf(stderr, "Error: Day %d not implemented yet\n", day_num);
        return 1;
    }

    bool run_part1 = true;
    bool run_part2 = true;
    bool run_tests = false;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "test") == 0)
        {
            run_tests = true;
        }
        else if (strcmp(argv[i], "1") == 0)
        {
            run_part1 = true;
            run_part2 = false;
        }
        else if (strcmp(argv[i], "2") == 0)
        {
            run_part1 = false;
            run_part2 = true;
        }
    }

    printf("Advent of Code 2018 - Day %d\n", day_num);
    printf("============================\n\n");

    if (run_tests)
    {
        printf("Running tests...\n");
        bool all_passed = true;

        if (run_part1)
        {
            printf("Part 1 test: ");
            timer_t timer = start_timer();
            bool result = day->test_part1();
            double time_taken = end_timer(timer);
            if (result)
            {
                printf("PASSED (%.6f seconds)\n", time_taken);
            }
            else
            {
                printf("FAILED (%.6f seconds)\n", time_taken);
                all_passed = false;
            }
        }

        if (run_part2)
        {
            printf("Part 2 test: ");
            timer_t timer = start_timer();
            bool result = day->test_part2();
            double time_taken = end_timer(timer);
            if (result)
            {
                printf("PASSED (%.6f seconds)\n", time_taken);
            }
            else
            {
                printf("FAILED (%.6f seconds)\n", time_taken);
                all_passed = false;
            }
        }

        printf("\nOverall: %s\n", all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
        return all_passed ? 0 : 1;
    }
    else
    {
        printf("Running solution...\n");

        if (run_part1)
        {
            printf("\nPart 1:\n");
            timer_t timer = start_timer();
            day->part1();
            double time_taken = end_timer(timer);
            printf("Part 1 completed in %.6f seconds\n", time_taken);
        }

        if (run_part2)
        {
            printf("\nPart 2:\n");
            timer_t timer = start_timer();
            day->part2();
            double time_taken = end_timer(timer);
            printf("Part 2 completed in %.6f seconds\n", time_taken);
        }
    }

    return 0;
}

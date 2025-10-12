#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "days/solution.h"

#include "days/01/01.h"
#include "days/02/02.h"

static const Solution* solutions[] = {
    &day01,
    &day02,
};

static const int num_solutions = sizeof(solutions) / sizeof(solutions[0]);

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void print_usage(const char* program_name)
{
    printf("Usage: %s [--help|-h] [--test|-t] [day]\n", program_name);
    printf("  --help|-h: Print this help message\n");
    printf("  --test|-t: Run tests instead of main solution\n");
    printf("  day: The day to run the solution for (1-25, optional - defaults to all implemented)\n");
    printf("\nExamples:\n");
    printf("  %s        # Run all implemented solutions\n", program_name);
    printf("  %s 1      # Run solution for day 1\n", program_name);
    printf("  %s -t 1   # Run tests for day 1\n", program_name);
}

int parse_day(char* arg)
{
    int day = atoi(arg);
    if (day >= 1 && day <= 25)
    {
        return day;
    }

    fprintf(stderr, "Error: Invalid day number '%s'\n", arg);
    return -1;
}

void run_part(void (*part)(void))
{
    Timer timer = start_timer();
    (*part)();
    double time_taken = end_timer(timer);
    printf("(Completed in %.6fs)\n", time_taken);
}

void run_test(bool (*test)(void))
{
    Timer timer = start_timer();
    bool result = (*test)();
    double time_taken = end_timer(timer);
    if (result)
    {
        printf(GRN "PASS" RESET " (%.6fs)\n", time_taken);
    }
    else
    {
        printf(RED "FAIL" RESET " (%.6fs)\n", time_taken);
    }
}

void run_solution(const int day, const Solution* solution, bool run_tests)
{
    printf("Day %d\n", day);

    if (run_tests)
    {
        printf("Part 1 test: ");
        run_test(solution->test_part1);

        printf("Part 2 test: ");
        run_test(solution->test_part2);
    }
    else
    {
        printf("\nPart 1:\n");
        run_part(solution->part1);

        printf("\nPart 2:\n");
        run_part(solution->part2);
    }
}

int main(int argc, char *argv[])
{
    int day = 0;  // 0 means run all days
    bool run_tests = false;

    if (argc >= 2)
    {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }

        if (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)
        {
            run_tests = true;

            if (argc >= 3)
            {
                day = parse_day(argv[2]);
            }
        }
        else
        {
            day = parse_day(argv[1]);
        }
    }

    printf("Advent of Code 2018\n");
    printf("===================\n\n");

    if (day == 0)
    {
        printf("Running %d solution(s)...\n\n", num_solutions);
        for (int i = 0; i < num_solutions; ++i)
        {
            if (i > 0) printf("\n\n");
            run_solution(i + 1, solutions[i], run_tests);
        }
    }
    else
    {
        if (day > num_solutions)
        {
            fprintf(stderr, "Error: Solution for day %d not created yet\n", day);
            return 1;
        }

        run_solution(day, solutions[day - 1], run_tests);
    }

    return 0;
}

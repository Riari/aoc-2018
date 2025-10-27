#include "04.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include "cc.h"
#include "utils.h"

enum Event
{
    BEGINS_SHIFT,
    FALLS_ASLEEP,
    WAKES_UP,
};

typedef struct
{
    int datetime;
    enum Event event;
    int guard_id;
} Record;

Record parse_record(const char* line)
{
    Record record;
    int stage = 0;
    char datetime[9];
    char guard_id[9];
    int str_pos = 0;
    for (int i = 0; i < strlen(line); ++i)
    {
        char c = line[i];
        switch (stage)
        {
            case 0: // Waiting for month
                if (c == '-') ++stage;
                break;
            case 1: // Reading month
                if (c == '-')
                {
                    ++stage;
                    break;
                }
                datetime[str_pos++] = c;
                break;
            case 2: // Reading day
                if (c == ' ')
                {
                    ++stage;
                    break;
                }
                datetime[str_pos++] = c;
                break;
            case 3: // Reading hour
                if (c == ':')
                {
                    ++stage;
                    break;
                }
                datetime[str_pos++] = c;
                break;
            case 4: // Reading minutes
                if (c == ']')
                {
                    datetime[str_pos] = '\0';
                    record.datetime = atoi(datetime);
                    str_pos = 0;
                    ++stage;
                    break;
                }
                datetime[str_pos++] = c;
                break;
            case 5: // Reading event
                if (c == 'G')
                {
                    ++stage;
                    record.event = BEGINS_SHIFT;
                }
                else if (c == 'w')
                {
                    record.event = WAKES_UP;
                    record.guard_id = -1;
                    return record;
                }
                else if (c == 'f')
                {
                    record.event = FALLS_ASLEEP;
                    record.guard_id = -1;
                    return record;
                }
                break;
            case 6: // Waiting for guard ID
                if (c == '#') ++stage;
                break;
            case 7: // Reading guard ID
                if (c == ' ')
                {
                    guard_id[str_pos] = 0;
                    record.guard_id = atoi(guard_id);
                    return record;
                }
                guard_id[str_pos++] = c;
                break;
        }
    }

    // Unreachable
    return record;
}

int compare_records(const void* a, const void* b)
{
    const Record* record_a = (const Record*)a;
    const Record* record_b = (const Record*)b;
    return compare_int_asc(&record_a->datetime, &record_b->datetime);
}

Record* parse_records(const Lines* lines)
{
    Record* records = malloc(lines->count * sizeof(Record));

    for (int i = 0; i < lines->count; ++i)
    {
        records[i] = parse_record(lines->lines[i]);
    }

    qsort(records, lines->count, sizeof(Record), compare_records);

    return records;
}

typedef struct Range
{
    int from;
    int to;
} Range;

typedef struct Guard
{
    Range ranges[128];
    size_t range_count;
} Guard;

static int solve(const Lines* lines, bool p2)
{
    Record* records = parse_records(lines);

    cc_map(int, Guard*) sleep_map;
    cc_init(&sleep_map);

    Guard* current_guard;
    for (int i = 0; i < lines->count; ++i)
    {
        Record record = records[i];
        switch (record.event)
        {
            case BEGINS_SHIFT:
                Guard** guard = cc_get(&sleep_map, record.guard_id);
                if (guard == NULL)
                {
                    current_guard = (Guard*)calloc(1, sizeof(Guard));
                    cc_insert(&sleep_map, record.guard_id, current_guard);
                }
                else
                {
                    current_guard = *guard;
                }
                break;
            case FALLS_ASLEEP:
                current_guard->ranges[current_guard->range_count].from = record.datetime;
                break;
            case WAKES_UP:
                current_guard->ranges[current_guard->range_count++].to = record.datetime;
                break;
        }
    }

    int result_guard_id = 0;
    int result_minute = 0;

    if (!p2)
    {
        // Strategy 1

        int max_sleep_time = 0;
        cc_for_each(&sleep_map, guard_id, guard)
        {
            int total_sleep = 0;
            for (int i = 0; i < (*guard)->range_count; ++i)
            {
                total_sleep += (*guard)->ranges[i].to - (*guard)->ranges[i].from;
            }

            if (total_sleep > max_sleep_time)
            {
                max_sleep_time = total_sleep;
                result_guard_id = *guard_id;
            }
        }

        Guard** sleepiest_guard = cc_get(&sleep_map, result_guard_id);
        int minute_counts[60] = {0};
        for (int i = 0; i < (*sleepiest_guard)->range_count; ++i)
        {
            Range range = (*sleepiest_guard)->ranges[i];
            int from_minute = range.from % 100;
            int to_minute = range.to % 100;

            for (int minute = from_minute; minute < to_minute; ++minute)
            {
                minute_counts[minute]++;
            }
        }

        int max_frequency = 0;
        for (int minute = 0; minute < 60; ++minute)
        {
            if (minute_counts[minute] > max_frequency)
            {
                max_frequency = minute_counts[minute];
                result_minute = minute;
            }
        }
    }
    else
    {
        // Strategy 2

        int max_frequency = 0;
        cc_for_each(&sleep_map, guard_id, guard)
        {
            int minute_counts[60] = {0};
            for (int i = 0; i < (*guard)->range_count; ++i)
            {
                Range range = (*guard)->ranges[i];
                int from_minute = range.from % 100;
                int to_minute = range.to % 100;

                for (int minute = from_minute; minute < to_minute; ++minute)
                {
                    minute_counts[minute]++;
                }
            }

            for (int minute = 0; minute < 60; ++minute)
            {
                if (minute_counts[minute] > max_frequency)
                {
                    max_frequency = minute_counts[minute];
                    result_guard_id = *guard_id;
                    result_minute = minute;
                }
            }
        }
    }

    int result = result_guard_id * result_minute;

    cc_for_each(&sleep_map, guard_id, guard)
    {
        free(*guard);
    }

    cc_cleanup(&sleep_map);

    free(records);

    return result;
}

void day04_part1(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, false));

    free_lines(lines);
}

void day04_part2(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, true));

    free_lines(lines);
}

bool day04_test_part1(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, false);

    free_lines(lines);
    return result == 240;
}

bool day04_test_part2(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, true);

    free_lines(lines);
    return result == 4455;
}

const Solution day04 = {
    day04_part1,
    day04_part2,
    day04_test_part1,
    day04_test_part2
};

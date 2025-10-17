#include "04.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char datetime[8];
    char guard_id[8];
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

    printf("Sorted records:\n");
    for (int i = 0; i < lines->count; ++i)
    {
        printf("Datetime: %d, event: %d, guard ID: %d\n", records[i].datetime, records[i].event, records[i].guard_id);
    }

    return records;
}

void day04_part1(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    Record* records = parse_records(&lines);

    free_lines(lines);
    free(records);
}

void day04_part2(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

bool day04_test_part1(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    Record* records = parse_records(&lines);

    free_lines(lines);
    free(records);
    return false;
}

bool day04_test_part2(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

const Solution day04 = {
    day04_part1,
    day04_part2,
    day04_test_part1,
    day04_test_part2
};

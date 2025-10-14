#include "03.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include "cc.h"

#include "maths.h"
#include "utils.h"

typedef struct Vec2
{
    int x;
    int y;
} Vec2;

typedef struct Claim
{
    Vec2 start;
    Vec2 end;
    bool has_overlap;
} Claim;

Claim parse_claim(char* definition)
{
    Claim claim;
    claim.has_overlap = false;
    int stage = 0;
    char buf[4];
    int i_buf = 0;
    for (int i = 0; i < strlen(definition); ++i)
    {
        char c = definition[i];
        switch (stage)
        {
            case 0: // Waiting for x coord
                if (c == '@') ++stage;
                break;
            case 1: // Parsing x coord
                if (isdigit(c))
                {
                    buf[i_buf++] = c;
                }
                else if (c == ',')
                {
                    buf[i_buf] = 0;
                    claim.start.x = atoi(buf);
                    buf[0] = 0;
                    i_buf = 0;
                    ++stage;
                }
                break;
            case 2: // Parsing y coord
                if (isdigit(c))
                {
                    buf[i_buf++] = c;
                }
                else if (c == ':')
                {
                    buf[i_buf] = 0;
                    claim.start.y = atoi(buf);
                    buf[0] = 0;
                    i_buf = 0;
                    ++stage;
                }
                break;
            case 3: // Parsing width
                if (isdigit(c))
                {
                    buf[i_buf++] = c;
                }
                else if (c == 'x')
                {
                    buf[i_buf] = 0;
                    claim.end.x = claim.start.x + atoi(buf);
                    buf[0] = 0;
                    i_buf = 0;
                    ++stage;
                }
                break;
            case 4: // Parsing height
                if (isdigit(c))
                {
                    buf[i_buf++] = c;
                }
                break;
        }
    }

    buf[i_buf] = 0;
    claim.end.y = claim.start.y + atoi(buf);

    return claim;
}

static Claim* parse_claims(const Lines* lines)
{
    Claim* claims = malloc(lines->count * sizeof(Claim));
    for (int i = 0; i < lines->count; ++i)
    {
        claims[i] = parse_claim(lines->lines[i]);
    }

    return claims;
}

int solve(Claim* claims, const int count, const bool p2)
{
    cc_set(char*) set;
    cc_init(&set);
    for (int i = 0; i < count; ++i)
    {
        Claim* a = &claims[i];
        for (int j = i + 1; j < count; ++j)
        {
            Claim* b = &claims[j];

            int max_left = MAX(a->start.x, b->start.x);
            int max_top = MAX(a->start.y, b->start.y);
            int min_right = MIN(a->end.x, b->end.x);
            int min_bottom = MIN(a->end.y, b->end.y);

            if (max_left >= min_right || max_top >= min_bottom)
            {
                // No overlap
                continue;
            }

            a->has_overlap = true;
            b->has_overlap = true;

            for (int x = max_left; x < min_right; ++x)
            {
                for (int y = max_top; y < min_bottom; ++y)
                {
                    char* str = (char*)malloc(8 * sizeof(char));
                    sprintf(str, "%d,%d", x, y);
                    if (cc_get(&set, str) == NULL)
                    {
                        cc_insert(&set, str);
                    }
                    else
                    {
                        free(str);
                    }
                }
            }
        }
    }

    int overlaps = cc_size(&set);

    cc_for_each(&set, str)
    {
        free(*str);
    }

    cc_cleanup(&set);

    if (p2)
    {
        for (int i = 0; i < count; ++i)
        {
            if (!claims[i].has_overlap)
            {
                return i + 1;
            }
        }
    }

    return overlaps;
}

void day03_part1(void)
{
    Lines lines = read_lines("03_input.txt");
    if (!lines.lines) return;

    Claim* claims = parse_claims(&lines);
    int result = solve(claims, lines.count, false);
    printf("%d\n", result);

    free_lines(lines);
    free(claims);
}

void day03_part2(void)
{
    Lines lines = read_lines("03_input.txt");
    if (!lines.lines) return;

    Claim* claims = parse_claims(&lines);
    int result = solve(claims, lines.count, true);
    printf("%d\n", result);

    free_lines(lines);
    free(claims);
}

bool day03_test_part1(void)
{
    Lines lines = read_lines("03_test_input.txt");
    if (!lines.lines) return false;

    Claim* claims = parse_claims(&lines);
    int result = solve(claims, lines.count, false);

    free_lines(lines);
    free(claims);
    return result == 4;
}

bool day03_test_part2(void)
{
    Lines lines = read_lines("03_test_input.txt");
    if (!lines.lines) return false;

    Claim* claims = parse_claims(&lines);
    int result = solve(claims, lines.count, true);

    free_lines(lines);
    free(claims);
    return result == 3;
}

const Solution day03 = {
    day03_part1,
    day03_part2,
    day03_test_part1,
    day03_test_part2
};

#!/bin/bash
set -e

if [ $# -ne 1 ]; then
  echo "Usage: $0 <day>"
  exit 1
fi

DAY=$(printf "%02d" $1)
DEST="src/days/$DAY"

if [ -d "$DEST" ]; then
  echo "Day $DAY already exists at $DEST"
  exit 2
fi

mkdir -p "$DEST"

cat > "$DEST/$DAY.h" << EOF
#pragma once

#include <stdbool.h>

#include "../solution.h"

void day${DAY}_part1(void);
void day${DAY}_part2(void);
bool day${DAY}_test_part1(void);
bool day${DAY}_test_part2(void);

extern const Solution day${DAY};
EOF

cat > "$DEST/$DAY.c" << EOF
#include "$DAY.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void day${DAY}_part1(void)
{
    Lines lines = read_lines("${DAY}_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

void day${DAY}_part2(void)
{
    Lines lines = read_lines("${DAY}_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

bool day${DAY}_test_part1(void)
{
    Lines lines = read_lines("${DAY}_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

bool day${DAY}_test_part2(void)
{
    Lines lines = read_lines("${DAY}_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

const Solution day${DAY} = {
    day${DAY}_part1,
    day${DAY}_part2,
    day${DAY}_test_part1,
    day${DAY}_test_part2
};
EOF

touch "$DEST/${DAY}_input.txt"
touch "$DEST/${DAY}_test_input.txt"

echo "Created $DEST. Don't forget to:"
echo "1. Include the header: #include \"days/$DAY/$DAY.h\""
echo "2. Add &day${DAY}, to the days array"

#!/bin/bash
set -e

if [ $# -ne 1 ]; then
  echo "Usage: $0 <day>"
  exit 1
fi

DAY=$1
DEST="src/days/$DAY"

if [ -d "$DEST" ]; then
  echo "Day $DAY already exists at $DEST"
  exit 2
fi

mkdir -p "$DEST"

cat > "$DEST/$DAY.h" << EOF
#pragma once

#include <stdbool.h>

// Day $DAY function declarations
void day${DAY}_part1(void);
void day${DAY}_part2(void);
bool day${DAY}_test_part1(void);
bool day${DAY}_test_part2(void);
EOF

cat > "$DEST/$DAY.c" << EOF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "solution.h"

void day${DAY}_part1(void) {
    char *input = read_input_file("input.txt");
    if (!input) return;

    // TODO: Implement part 1 solution
    printf("Result: TODO\n");

    free(input);
}

void day${DAY}_part2(void) {
    char *input = read_input_file("input.txt");
    if (!input) return;

    // TODO: Implement part 2 solution
    printf("Result: TODO\n");

    free(input);
}

bool day${DAY}_test_part1(void) {
    char *input = read_input_file("test_input.txt");
    if (!input) return false;

    // TODO: Implement part 1 test
    // Expected result: TODO
    bool passed = false;

    free(input);
    return passed;
}

bool day${DAY}_test_part2(void) {
    char *input = read_input_file("test_input.txt");
    if (!input) return false;

    // TODO: Implement part 2 test
    // Expected result: TODO
    bool passed = false;

    free(input);
    return passed;
}
EOF

touch "$DEST/input.txt"
touch "$DEST/test_input.txt"

echo "Created $DEST"
echo "Don't forget to:"
echo "1. Add the day to the registry in src/main.c"
echo "2. Include the header: #include \"days/$DAY/$DAY.h\""
echo "3. Add the day functions to the days array"

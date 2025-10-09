#include "01.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


void day01_part1(void) {
    char *input = read_input_file("input.txt");
    if (!input) return;
    

}

void day01_part2(void) {
    char *input = read_input_file("input.txt");
    if (!input) return;


}

bool day01_test_part1(void) {
    char *input = read_input_file("test_input.txt");
    if (!input) return false;

    return false;
}

bool day01_test_part2(void) {
    char *input = read_input_file("test_input.txt");
    if (!input) return false;

    return false;
}

const solution_t day01 = {
    1,
    day01_part1,
    day01_part2,
    day01_test_part1,
    day01_test_part2
};

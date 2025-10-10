cmake_minimum_required(VERSION 3.16)
project(aoc_2018 C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

file(GLOB LIB_SRC "src/lib/*.c")
file(GLOB DAY_SRC "src/days/*/*.c")

add_executable(aoc_2018 src/main.c ${LIB_SRC} ${DAY_SRC})

target_include_directories(aoc_2018 PRIVATE
    "${CMAKE_SOURCE_DIR}/src/lib"
    "${CMAKE_SOURCE_DIR}/src/days"
)

file(GLOB DAY_DIRS RELATIVE "${CMAKE_SOURCE_DIR}/src/days" "${CMAKE_SOURCE_DIR}/src/days/*")
foreach(DAY ${DAY_DIRS})
    file(GLOB INPUT_FILES "${CMAKE_SOURCE_DIR}/src/days/${DAY}/*_input.txt")
    foreach(INPUT_FILE ${INPUT_FILES})
        get_filename_component(INPUT_FILENAME ${INPUT_FILE} NAME)
        add_custom_command(
            TARGET aoc_2018 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${INPUT_FILE}"
                "${CMAKE_BINARY_DIR}/${INPUT_FILENAME}"
        )
    endforeach()
endforeach()

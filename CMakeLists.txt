cmake_minimum_required(VERSION 3.16)
project(aoc_2018 C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

file(GLOB UTILS_SRC "src/utils/*.c")
file(GLOB DAY_SRC "src/days/*/*.c")

add_executable(aoc_2018 src/main.c ${UTILS_SRC} ${DAY_SRC})

target_include_directories(aoc_2018 PRIVATE 
    "${CMAKE_SOURCE_DIR}/src/utils"
    "${CMAKE_SOURCE_DIR}/src/days"
)

file(GLOB DAY_DIRS RELATIVE "${CMAKE_SOURCE_DIR}/src/days" "${CMAKE_SOURCE_DIR}/src/days/*")
foreach(DAY ${DAY_DIRS})
    foreach(INPUT_FILE input.txt test_input.txt)
        if(EXISTS "${CMAKE_SOURCE_DIR}/src/days/${DAY}/${INPUT_FILE}")
            add_custom_command(
                TARGET aoc_2018 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/src/days/${DAY}/${INPUT_FILE}"
                    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${INPUT_FILE}"
            )
        endif()
    endforeach()
endforeach()

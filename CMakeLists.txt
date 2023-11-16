cmake_minimum_required(VERSION 3.22)
project(crossword_constructor VERSION 1.0)

include_directories(crossword_constructor ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(crossword_constructor
    main.cpp
    src/crossword_utils.h
    src/crossword_utils.cpp
    src/crossword_constructor.h
    src/crossword_entry.h
    src/crossword_entry.cpp
    src/crossword_word_entry.h
    src/mrv_heuristic.h
    src/mrv_heuristic.cpp
    src/lcv_heuristic.h
    src/lcv_heuristic.cpp
    src/constraints.h
    src/forward_checking_data.h
    src/forward_checking_data.cpp
)

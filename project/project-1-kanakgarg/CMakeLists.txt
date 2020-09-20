cmake_minimum_required(VERSION 3.13)
project(project_1_kanakgarg C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(tests/cunit)

add_executable(project_1_kanakgarg
        tests/cunit/test.c
        tests/cunit/test.h
        main.c
        preprocessor.c
        preprocessor.h
        string-helpers.c
        string-helpers.h
        tokenizer-errors.c
        tokenizer-errors.h
        tokenizer.c
        tokenizer.h
        tokens.c
        tokens.h
        utils.c
        utils.h)

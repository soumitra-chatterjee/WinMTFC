cmake_minimum_required(VERSION 4.0)

project(MT_FileCopy C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(MT_FileCopy 
                src/main.c 
                src/handle_error.c
                src/file_copy.c)

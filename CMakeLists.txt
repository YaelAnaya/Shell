cmake_minimum_required(VERSION 3.24)
project(Shell C)

set(CMAKE_C_STANDARD 23)

add_executable(Shell main.c Prompt.c Prompt.h Process.c Process.h Memory.c Memory.h Queue.c Queue.h List.c List.h)

cmake_minimum_required(VERSION 3.10)
project(read_tcb)

include_directories("include")
set(SOURCES "read_tcb.c" "include/read_tcb.h")

add_executable(${PROJECT_NAME} ${SOURCES})

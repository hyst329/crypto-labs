cmake_minimum_required(VERSION 3.0)

project(3des)

set(SOURCES src/main.cpp src/3des.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
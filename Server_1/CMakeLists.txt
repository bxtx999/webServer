CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(tiny LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE(tiny_server tiny.cpp)
TARGET_INCLUDE_DIRECTORIES(
        tiny_server
        PRIVATE
        /usr/include/x86_64-linux-gnu/
        /usr/include/
)
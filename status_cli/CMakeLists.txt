cmake_minimum_required(VERSION 2.8)
project(status_cli)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        network.cpp
)

add_executable(status_cli ${SOURCE_FILES})
target_link_libraries(status_cli general -lncurses -ljson-c)

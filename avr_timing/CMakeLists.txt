cmake_minimum_required(VERSION 2.8.4)

# Use gcc and g++
set(CMAKE_C_COMPILER avr32-gcc)
set(CMAKE_CXX_COMPILER avr32-g++)

project("avr32-timing")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -Wall -mmcu=at32uc3b064 -D__AVR_AT32UC3B064__ -DF_CPU=51200000 -DARCH_UC3")

include_directories("/usr/lib/avr/include")
include_directories("../lufa/LUFA")

add_executable(avr32-timing main.c)

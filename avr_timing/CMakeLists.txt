cmake_minimum_required(VERSION 2.8.4)

project("avr-timing")

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CMCU "-mmcu=atxmega32a4u")
SET(CDEFS "-DF_CPU=25600000")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

include_directories("/usr/lib/avr/include")
include_directories("../lufa/LUFA")

add_executable(avr-timing main.c leds.cpp leds.h)

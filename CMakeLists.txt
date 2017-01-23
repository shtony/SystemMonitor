cmake_minimum_required(VERSION 2.4)

project(SystemMonitor C ASM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

unset(COMMON_FLAGS)
set(COMMON_FLAGS "${COMMON_FLAGS} -pipe")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wall")
set(COMMON_FLAGS "${COMMON_FLAGS} -Os")

set(COMMON_FLAGS "${COMMON_FLAGS} -mcpu=cortex-m0plus")
set(COMMON_FLAGS "${COMMON_FLAGS} -mthumb")

add_definitions(-DSTM32L052xx)

include_directories("${CMAKE_SOURCE_DIR}/inc")
include_directories("${CMAKE_SOURCE_DIR}/device/inc")
include_directories("${CMAKE_SOURCE_DIR}/CMSIS/inc")

aux_source_directory("${CMAKE_SOURCE_DIR}/device/src" DEVICE_SRC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/STM32L052.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=output.map")

set(STARTUP_SRC "${CMAKE_SOURCE_DIR}/device/startup_gcc/startup_stm32l052xx.s")

set(SRCS
    "${CMAKE_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}/src/board.c"
    "${CMAKE_SOURCE_DIR}/src/interrupts.c"
)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_executable(${PROJECT_NAME} ${SRCS} ${STARTUP_SRC} ${DEVICE_SRC})

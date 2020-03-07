cmake_minimum_required(VERSION 2.8)

project (testnormal)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ansi -pedantic -Wall")
endif()

ADD_EXECUTABLE(testnormal test-normal.cpp)

INCLUDE_DIRECTORIES(/path/to/your/gperftools/and/unwind/include) #unwind gperftools-2.0

FIND_LIBRARY(UNWIND_LIB NAMES unwind PATHS "/path/to/your/unwind/lib/")
MESSAGE(STATUS "Library path UNWIND_LIB is " ${UNWIND_LIB})
FIND_LIBRARY(PROFILER_LIB NAMES profiler PATHS "/path/to/your/gperftools/lib/")
MESSAGE(STATUS "Library path PROFILER_LIB is " ${PROFILER_LIB})

TARGET_LINK_LIBRARIES(testnormal ${UNWIND_LIB})
TARGET_LINK_LIBRARIES(testnormal ${PROFILER_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

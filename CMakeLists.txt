cmake_minimum_required(VERSION 3.17)

#Tootchain file
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-gcc.cmake)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#Languages
enable_language(C ASM)

#Project name
project(clock)

#Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Unit tests
include(CTest)
add_subdirectory(test/unit_test/unity unity)
add_subdirectory(test/unit_test/events)
add_subdirectory(test/unit_test/weather_control)
add_subdirectory(test/unit_test/sys_comm)

#Source files
add_subdirectory(src)

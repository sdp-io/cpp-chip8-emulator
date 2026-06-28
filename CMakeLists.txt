cmake_minimum_required(VERSION 3.28)

project(chip8-emu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

add_executable(chip8 src/main.cpp)
target_sources(chip8
    PUBLIC
        FILE_SET chip8_mobo TYPE CXX_MODULES FILES
        src/cpu.cppm
        src/memory.cppm
        src/display.cppm
)

target_compile_features(chip8 PUBLIC cxx_std_20)
target_link_libraries(chip8 PRIVATE SDL3::SDL3)

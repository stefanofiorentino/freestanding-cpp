cmake_minimum_required(VERSION 3.16)

# --- Compiler preference: use clang++. ------------------------------------
# Fall back to the default compiler if clang++ isn't found, so the build still
# works on non-clang machines. C++23 needs a recent compiler (clang 16+).
find_program(CLANG_CXX_EXECUTABLE NAMES clang++-21 clang++-18 clang++-17 clang++)
if(CLANG_CXX_EXECUTABLE)
    set(CMAKE_CXX_COMPILER "${CLANG_CXX_EXECUTABLE}")
endif()

project(freestanding-cpp LANGUAGES CXX)

# C++23 is the project standard. The freestanding feature set (P1642)
# only exists in C++23, so nothing below may drop below C++23.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Freestanding-only core library.
#
# Built with freestanding flags so it may rely only on the C++23
# freestanding subset of the standard library. It must compile with
# -ffreestanding -fno-exceptions -fno-rtti. No -nostdinc/-nostdlib: the
# freestanding subset is provided by the (host) standard library headers.
# ---------------------------------------------------------------------------
add_library(freestanding_core STATIC
    src/freestanding_core.cpp
)
target_include_directories(freestanding_core PUBLIC src)
target_compile_options(freestanding_core PRIVATE
    -ffreestanding
    -fno-exceptions
    -fno-rtti
)

# ---------------------------------------------------------------------------
# Tests (host environment).
#
# Running on the OS, so the test binary is compiled with the normal host
# flags and may use the full standard library. It links the freestanding
# core to verify it compiles, links and behaves correctly.
# ---------------------------------------------------------------------------
enable_testing()

add_executable(freestanding_core_test
    test/freestanding_core_test.cpp
)
target_link_libraries(freestanding_core_test PRIVATE freestanding_core)
target_compile_options(freestanding_core_test PRIVATE -Wall -Wextra)

add_test(NAME freestanding_core_test COMMAND freestanding_core_test)

# ---------------------------------------------------------------------------
# Tooling: clang-format and clang-tidy.
#
# clang-format/clang-tidy ship with LLVM but may be keg-only (not on PATH),
# so locate them via find_program and only define the targets if found.
# ---------------------------------------------------------------------------
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin)
find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin)

set(FREESTANDING_SOURCES
    src/freestanding_core.hpp
    src/freestanding_core.cpp
    test/freestanding_core_test.cpp
)

if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(format
        COMMAND "${CLANG_FORMAT_EXECUTABLE}" -i ${FREESTANDING_SOURCES}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Formatting sources with clang-format"
    )
else()
    message(WARNING "clang-format not found; 'format' target disabled")
endif()

if(CLANG_TIDY_EXECUTABLE)
    add_custom_target(tidy
        COMMAND "${CLANG_TIDY_EXECUTABLE}" ${FREESTANDING_SOURCES}
            --extra-arg=-std=c++23
            -- -Isrc
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Running clang-tidy on sources"
    )
else()
    message(WARNING "clang-tidy not found; 'tidy' target disabled")
endif()

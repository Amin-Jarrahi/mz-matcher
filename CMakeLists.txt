cmake_minimum_required(VERSION 3.20)
project(mz-matcher
    VERSION   2.0.0
    LANGUAGES CXX
    DESCRIPTION "LCMS / MALDI m/z matching and deduplication tool"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Main executable ──────────────────────────────────────────────────────────
add_executable(mz-matcher
    src/main.cpp
    src/LCMSData.cpp
    src/MALDIData.cpp
    src/MZMatcher.cpp
)

target_include_directories(mz-matcher PRIVATE src)

# Compiler warnings
if(MSVC)
    target_compile_options(mz-matcher PRIVATE /W4)
else()
    target_compile_options(mz-matcher PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ── Install ──────────────────────────────────────────────────────────────────
install(TARGETS mz-matcher DESTINATION bin)

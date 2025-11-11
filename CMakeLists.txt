cmake_minimum_required(VERSION 3.28)
project(MZ_Matcher)

set(CMAKE_CXX_STANDARD 20)

add_executable(MZ_Matcher main.cpp
        LCMS_MZ_Manipulator.cpp
        LCMS_MZ_Manipulator.h
        MALDI_MZ_Manipulator.cpp
        MALDI_MZ_Manipulator.h
        MZ_Analyzer.cpp
        MZ_Analyzer.h
)

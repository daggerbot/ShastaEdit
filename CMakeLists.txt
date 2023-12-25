# Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION "3.25")
project("ShastaEdit" CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_AUTOMOC ON)

find_package("Qt6" REQUIRED COMPONENTS "UiTools" "Widgets")

#---------------------------------------------------------------------------------------------------
# Configure compiler options

add_library("shasta_compiler_options" INTERFACE)

target_compile_definitions("shasta_compiler_options" INTERFACE
    $<$<CONFIG:MinSizeRel,Release,RelWithDebInfo>:QT_NO_DEBUG_OUTPUT>
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options("shasta_compiler_options" INTERFACE
        "-std=c++17"
        "-pedantic"
        "-Wall"
        "-Wconversion"
        "-Wextra"
        "-Wsuggest-override"
    )
else()
    set(CMAKE_CXX_STANDARD "17")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions("shasta_compiler_options" INTERFACE
        "_UNICODE"
        "_WIN32_WINNT=0x0A00"
        "NOMINMAX"
        "UNICODE"
        "WINVER=0x0A00"
    )
endif()

#---------------------------------------------------------------------------------------------------
# Build components

add_subdirectory("src")

#---------------------------------------------------------------------------------------------------
# Test run

set(RUN_ENV
    "SHASTAEDIT_DATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/share"
)

if(CMAKE_GENERATOR STREQUAL "Ninja" OR CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    add_custom_target("run"
        COMMAND "${CMAKE_COMMAND}" "-E" "env" ${RUN_ENV} "$<TARGET_FILE:shastaedit>"
        DEPENDS "shastaedit"
        USES_TERMINAL)
endif()

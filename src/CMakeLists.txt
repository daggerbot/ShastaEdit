# Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_executable("shastaedit" WIN32
    "Application.cpp"
    "CommandLine.cpp"
    "Main.cpp"
    "MainWindow.cpp"
)

target_link_libraries("shastaedit"
    PRIVATE
        "shasta_compiler_options"
        "Qt6::Widgets"
)

if(UNIX)
    target_sources("shastaedit" PRIVATE
        "Platform_unix.cpp"
    )
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

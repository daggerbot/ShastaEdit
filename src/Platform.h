// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_PLATFORM_H_INCLUDED
#define SHASTAEDIT_PLATFORM_H_INCLUDED

#include <filesystem>
#include <vector>

#include <QString>

#include "Types.h"

// ANSI escape sequences for decorating log messages. This is supported on Windows and most common
// Unix terminals.
#define ANSI_RESET "\x1B[0m"
#define ANSI_BOLD "\x1B[1m"
#define ANSI_BLACK "\x1B[30m"
#define ANSI_RED "\x1B[31m"
#define ANSI_GREEN "\x1B[32m"
#define ANSI_YELLOW "\x1B[33m"
#define ANSI_BLUE "\x1B[34m"
#define ANSI_MAGENTA "\x1B[35m"
#define ANSI_CYAN "\x1B[36m"
#define ANSI_WHITE "\x1B[37m"

// Log message prefixes
#define LOG_PREFIX_DEBUG ANSI_BOLD ANSI_MAGENTA "Debug: " ANSI_RESET
#define LOG_PREFIX_WARNING ANSI_BOLD ANSI_YELLOW "Warning: " ANSI_RESET
#define LOG_PREFIX_CRITICAL ANSI_BOLD ANSI_RED "Critical: " ANSI_RESET
#define LOG_PREFIX_FATAL ANSI_BOLD ANSI_RED "Fatal: " ANSI_RESET ANSI_BOLD
#define LOG_PREFIX_INFO ANSI_BOLD ANSI_BLUE "Info: " ANSI_RESET

namespace ShastaEdit {

    namespace Platform {

        std::vector<std::filesystem::path> getDataDirs();
        void installMessageHandler();
        QString toQString(OsStringView osString);
        QString toQString(const std::filesystem::path& path);

    } // namespace Platform

} // namespace ShastaEdit

#endif // SHASTAEDIT_PLATFORM_H_INCLUDED

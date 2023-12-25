// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <system_error>

#include <QtGlobal>

#include "AppDefs.h"
#include "Platform.h"

using namespace ShastaEdit;

namespace {

    const char* const fallbackDataDirs[] = {
        "/usr/local/share/" APP_UNIX_NAME,
        "/usr/share/" APP_UNIX_NAME,
    };

    void handleMessage(QtMsgType type, const QMessageLogContext&, const QString& message)
    {
        const char* prefix;

        switch (type) {
        case QtDebugMsg:
            prefix = LOG_PREFIX_DEBUG;
            break;
        case QtWarningMsg:
            prefix = LOG_PREFIX_WARNING;
            break;
        case QtCriticalMsg:
            prefix = LOG_PREFIX_CRITICAL;
            break;
        case QtFatalMsg:
            prefix = LOG_PREFIX_FATAL;
            break;
        case QtInfoMsg:
            prefix = LOG_PREFIX_INFO;
            break;
        default:
            return;
        }

        QByteArray messageBytes = message.toLocal8Bit();
        messageBytes.push_back(char{0});

        // Lock a mutex so we don't try to write to stderr from multiple threads at once.
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock{mutex};

        std::fprintf(stderr, "%s%s\n", prefix, messageBytes.data());

        if (type == QtFatalMsg) {
            std::exit(EXIT_FAILURE);
        }
    }

} // namespace

std::vector<std::filesystem::path> Platform::getDataDirs()
{
    std::vector<std::filesystem::path> dirs;
    std::error_code errorCode;

    // Use the application-specific override variable
    if (auto env = std::getenv(APP_ENV_DATA_DIR)) {
        std::filesystem::path path{env};

        if (std::filesystem::is_directory(path, errorCode)) {
            dirs.push_back(path);
            return dirs;
        }
    }

    // Use XDG_DATA_DIRS
    if (auto env = std::getenv("XDG_DATA_DIRS")) {
        const char* pathStart = env;
        const char* pathEnd = pathStart;

        while (*pathStart) {
            // Move to the end of the current path
            while (*pathEnd && *pathEnd != ':') ++pathEnd;

            if (pathEnd != pathStart) {
                auto path = std::filesystem::path{pathStart, pathEnd} / APP_UNIX_NAME;
                if (std::filesystem::is_directory(path, errorCode)) {
                    dirs.push_back(path);
                }
            }

            if (*pathEnd) {
                pathStart = ++pathEnd;
            } else {
                break;
            }
        }

        if (!dirs.empty()) {
            return dirs;
        }
    }

    // If we found nothing usable thus far, use fallbacks
    for (const char* path : fallbackDataDirs) {
        if (std::filesystem::is_directory(path, errorCode)) {
            dirs.push_back(path);
        }
    }

    return dirs;
}

void Platform::installMessageHandler()
{
    qInstallMessageHandler(&handleMessage);
}

QString Platform::toQString(OsStringView osString)
{
    return QString::fromLocal8Bit(osString);
}

QString Platform::toQString(const std::filesystem::path& path)
{
    return toQString(OsStringView{path.string()});
}

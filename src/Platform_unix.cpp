// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstdio>
#include <cstdlib>
#include <mutex>

#include <QtGlobal>

#include "Platform.h"

using namespace ShastaEdit;

namespace {

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

void Platform::installMessageHandler()
{
    qInstallMessageHandler(&handleMessage);
}

QString Platform::toQString(OsStringView osString)
{
    return QString::fromLocal8Bit(osString);
}

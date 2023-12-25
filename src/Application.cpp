// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstdlib>

#include <QMessageBox>

#include "Application.h"
#include "CommandLine.h"

using namespace ShastaEdit;

Application::Application(int& argc, char** argv)
    : QApplication{argc, argv}
    , m_mainWindow{*this}
{
}

Application::~Application()
{
}

int Application::exec(const ApplicationParams& params)
{
    // Show an error message if there was a problem parsing the command line.
    if (!params.startupErrorMessage.isEmpty()) {
        auto messageBytes = params.startupErrorMessage.toUtf8();
        messageBytes.push_back(char{0});
        qCritical("%s", messageBytes.data());
        QMessageBox::critical(nullptr, tr("Error"), params.startupErrorMessage);
        return EXIT_FAILURE;
    }

    m_mainWindow.show();
    return QApplication::exec();
}

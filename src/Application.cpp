// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstdlib>

#include <QFile>
#include <QMessageBox>
#include <QUiLoader>

#include "AppDefs.h"
#include "Application.h"
#include "CommandLine.h"
#include "MainWindow.h"
#include "Platform.h"
#include "UiLoader.h"

using namespace ShastaEdit;

Application::Application(int& argc, char** argv, const ApplicationParams& params)
    : QApplication{argc, argv}
{
    if (!params.startupErrorMessage.isEmpty()) {
        fatal(params.startupErrorMessage);
    }

    initDataDirs();

    initMainWindow();
}

Application::~Application()
{
}

int Application::exec()
{
    if (!m_mainWindow) {
        return EXIT_FAILURE;
    }

    m_mainWindow->show();
    return QApplication::exec();
}

void Application::fatal(const QString& message)
{
    if (m_mainWindow) {
        m_mainWindow->hide();
    }

    auto messageBytes = message.toUtf8();
    messageBytes.push_back(char{0});
    qCritical("%s", messageBytes.data());
    QMessageBox::critical(nullptr, tr("Error"), message);
    std::exit(EXIT_FAILURE);
}

bool Application::openDataFile(QFile& file, const char* name, QString& outError)
{
    std::error_code errorCode;

    file.close();

    for (auto dir : m_dataDirs) {
        auto path = dir / name;
        if (std::filesystem::exists(path, errorCode)) {
            file.setFileName(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                outError = Platform::toQString(path) + ": " + file.errorString();
                return false;
            }
            return true;
        }
    }

    outError = QString{name} + ": File not found";
    return false;
}

void Application::initDataDirs()
{
    m_dataDirs = Platform::getDataDirs();
    if (m_dataDirs.empty()) {
        fatal("No suitable data directories found");
    }
}

void Application::initMainWindow()
{
    QFile file;
    QString error;
    UiLoader loader;
    std::unique_ptr<QWidget> widget;

    if (!openDataFile(file, APP_FILENAME_MAINWINDOW_UI, error)) {
        fatal(error);
    }

    widget.reset(loader.load(&file));
    if (!widget) {
        fatal(QString{APP_FILENAME_MAINWINDOW_UI} + ": " + loader.errorString());
    }

    m_mainWindow.reset(qobject_cast<MainWindow*>(widget.get()));
    if (!m_mainWindow) {
        fatal(QString{APP_FILENAME_MAINWINDOW_UI} + ": Not a MainWindow");
    }

    widget.release();
}

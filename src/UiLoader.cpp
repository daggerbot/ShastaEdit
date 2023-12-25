// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "MainWindow.h"
#include "UiLoader.h"

using namespace ShastaEdit;

UiLoader::UiLoader()
    : QUiLoader{}
{
}

UiLoader::~UiLoader()
{
}

QWidget* UiLoader::createWidget(const QString& className, QWidget* parent, const QString& name)
{
    if (className == "MainWindow") {
        auto mainWindow = new MainWindow;
        mainWindow->setObjectName(name);
        return mainWindow;
    } else {
        return QUiLoader::createWidget(className, parent, name);
    }
}

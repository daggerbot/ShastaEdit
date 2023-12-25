// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "MainWindow.h"

using namespace ShastaEdit;

MainWindow::MainWindow(Application& app)
    : QMainWindow{}
    , m_app{app}
{
}

MainWindow::~MainWindow()
{
}

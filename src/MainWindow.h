// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_MAINWINDOW_H_INCLUDED
#define SHASTAEDIT_MAINWINDOW_H_INCLUDED

#include <QMainWindow>

namespace ShastaEdit {

    class Application;

    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();
    };

} // namespace ShastaEdit

#endif // SHASTAEDIT_MAINWINDOW_H_INCLUDED

// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_APPLICATION_H_INCLUDED
#define SHASTAEDIT_APPLICATION_H_INCLUDED

#include <QApplication>

#include "MainWindow.h"

namespace ShastaEdit {

    struct ApplicationParams;

    class Application : public QApplication {
    public:
        explicit Application(int& argc, char** argv);
        ~Application();

        MainWindow& mainWindow() { return m_mainWindow; }

        int exec(const ApplicationParams& params);

    private:
        MainWindow m_mainWindow;
    };

} // namespace ShastaEdit

#endif // SHASTAEDIT_APPLICATION_H_INCLUDED

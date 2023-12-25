// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_APPLICATION_H_INCLUDED
#define SHASTAEDIT_APPLICATION_H_INCLUDED

#include <filesystem>
#include <memory>
#include <vector>

#include <QApplication>

class QFile;

namespace ShastaEdit {

    struct ApplicationParams;
    class MainWindow;

    class Application : public QApplication {
    public:
        explicit Application(int& argc, char** argv, const ApplicationParams& params);
        ~Application();

        MainWindow* mainWindow() { return m_mainWindow.get(); }

        int exec();
        [[noreturn]] void fatal(const QString& message);
        bool openDataFile(QFile& file, const char* name, QString& outError);

    private:
        std::vector<std::filesystem::path> m_dataDirs;

        std::unique_ptr<MainWindow> m_mainWindow;

        void initDataDirs();

        void initMainWindow();
    };

} // namespace ShastaEdit

#endif // SHASTAEDIT_APPLICATION_H_INCLUDED

// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifdef _WIN32
# include <windows.h>
#endif

#include <QtGlobal>

#include "Application.h"
#include "CommandLine.h"
#include "Platform.h"

using namespace ShastaEdit;

namespace {

    int appMain(int argc, const oschar_t* const* argv)
    {
        Platform::installMessageHandler();

        // QApplication wants to handle command line arguments in some weird non-standard way, so
        // let's trick it and give it some fake args.
#ifdef _WIN32
        std::string programName = __argv[0];
#else
        std::string programName = argv[0];
#endif
        int fakeArgc = 1;
        char* fakeArgv[] = {programName.data(), nullptr};

        return Application{fakeArgc, fakeArgv, ApplicationParams::parse(argc, argv)}.exec();
    }

} // namespace

#ifdef _WIN32

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    return appMain(__argc, __wargv);
}

#else // !defined(_WIN32)

int main(int argc, char* argv[])
{
    return appMain(argc, argv);
}

#endif // !defined(_WIN32)

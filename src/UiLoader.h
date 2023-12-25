// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_UILOADER_H_INCLUDED
#define SHASTAEDIT_UILOADER_H_INCLUDED

#include <QUiLoader>

#include "Types.h"

namespace ShastaEdit {

    class UiLoader : public QUiLoader {
    public:
        UiLoader();
        ~UiLoader();

        QWidget* createWidget(const QString& className, QWidget* parent = nullptr, const QString& name = QString{}) override;
    };

} // namespace ShastaEdit

#endif // SHASTAEDIT_UILOADER_H_INCLUDED

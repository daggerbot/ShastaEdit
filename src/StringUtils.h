// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_STRINGUTILS_H_INCLUDED
#define SHASTAEDIT_STRINGUTILS_H_INCLUDED

#include "Types.h"

namespace ShastaEdit {

    namespace StringUtils {

        template<typename CharT>
        constexpr const CharT* findInCString(const CharT* str, CharT ch)
        {
            if (!str) {
                return nullptr;
            }

            while (true) {
                if (*str == ch) {
                    return str;
                } else if (!*str) {
                    return nullptr;
                } else {
                    ++str;
                }
            }
        }

        template<typename CharT>
        constexpr size_t getCStringLength(const CharT* str)
        {
            size_t length = 0;

            if (str) {
                while (str[length]) {
                    ++length;
                }
            }

            return length;
        }

    } // namespace StringUtils

} // namespace ShastaEdit

#endif // SHASTAEDIT_STRINGUTILS_H_INCLUDED

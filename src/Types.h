// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_TYPES_H_INCLUDED
#define SHASTAEDIT_TYPES_H_INCLUDED

#include <string>
#include <string_view>

namespace ShastaEdit {

    using nullptr_t = decltype(nullptr);
    using ptrdiff_t = decltype(static_cast<int*>(0) - static_cast<int*>(0));
    using size_t = decltype(sizeof(int));

    // oschar_t: Preferred character type when using the underlying platform APIs.
    // OSSTR: Encodes a string literal as an array of oschar_t.
#ifdef _WIN32
# define OSSTR_(x) L##x
# define OSSTR(x) OSSTR_(x)
    using oschar_t = wchar_t;
#else
# define OSSTR(x) x
    using oschar_t = char;
#endif

    using OsString = std::basic_string<oschar_t>;
    using OsStringView = std::basic_string_view<oschar_t>;

} // namespace ShastaEdit

#endif // SHASTAEDIT_TYPES_H_INCLUDED

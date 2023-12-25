// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SHASTAEDIT_COMMANDLINE_H_INCLUDED
#define SHASTAEDIT_COMMANDLINE_H_INCLUDED

#include <optional>

#include <QString>

#include "Types.h"

namespace ShastaEdit {

    class CommandLineParser;

    // Options used to initialize the application.
    struct ApplicationParams {
        QString startupErrorMessage;

        static ApplicationParams parse(int argc, const oschar_t* const* argv);
    };

    // Interface for handling command line options.
    class CommandLineHandler {
    public:
        virtual ~CommandLineHandler() = 0;

        // These functions can return false to stop parsing.
        virtual bool handleOperand(OsStringView operand) = 0;
        virtual bool handleShortOption(oschar_t option, CommandLineParser& parser) = 0;
        virtual bool handleLongOption(OsStringView operand, CommandLineParser& parser) = 0;

        CommandLineHandler& operator=(const CommandLineHandler&) = delete;
        CommandLineHandler& operator=(CommandLineHandler&&) = delete;
    };

    inline CommandLineHandler::~CommandLineHandler() = default;

    // Parses the command line.
    class CommandLineParser {
        friend class CommandLineHandler;

    public:
        CommandLineParser() = delete;
        CommandLineParser(const CommandLineParser&) = delete;
        CommandLineParser(CommandLineParser&&) = delete;

        // Parses the command line. Returns false if any of the handler functions returned false to
        // stop parsing.
        static bool parse(int argc, const oschar_t* const* argv, CommandLineHandler& handler);

        // Consumes and returns the parameter for the current option. Returns null if no parameter
        // is available. If an option expects a parameter, this must be called even if the parameter
        // will not be used, or the parameter will not be consumed and may be mistaken for something
        // else.
        const oschar_t* getParam();

        // Returns true if a parameter has been consumed for the current option. If the option has
        // the syntax '--option=param', this will always be true, If the option has any other
        // syntax, i.e. '--option param' or '-Oparam', this will return false unless getParam() has
        // been called for this option.
        bool hasParam() const { return m_param.has_value(); }

        CommandLineParser& operator=(const CommandLineParser&) = delete;
        CommandLineParser& operator=(CommandLineParser&&) = delete;

    private:
        enum class State { Initial, ShortOption, LongOption, OperandsOnly };

        int m_argCount;
        const oschar_t* const* m_args;
        int m_argIndex = 1;
        size_t m_charIndex = 0;
        State m_state = State::Initial;
        std::optional<const oschar_t*> m_param;

        explicit CommandLineParser(int argc, const oschar_t* const* argv)
            : m_argCount{argc}, m_args{argv}
        {
        }

        bool isFinished() const { return m_argIndex >= m_argCount; }
        bool parseNext(CommandLineHandler& handler);
    };

} // namespace ShastaEdit

#endif // SHASTAEDIT_COMMANDLINE_H_INCLUDED

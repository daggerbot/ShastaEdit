// Copyright (c) 2023 Martin Mills <daggerbot@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <QtGlobal>

#include "CommandLine.h"
#include "Platform.h"
#include "StringUtils.h"

using namespace ShastaEdit;

namespace {

    class AppCommandLineHandler : public CommandLineHandler {
    public:
        explicit AppCommandLineHandler(ApplicationParams& params)
            : m_params{params}
        {
        }

        bool handleOperand(OsStringView operand) override
        {
            m_params.startupErrorMessage = QString{"Unexpected operand: "} + Platform::toQString(operand);
            return false;
        }

        bool handleShortOption(oschar_t option, CommandLineParser&) override
        {
            OsStringView optionString{&option, 1};
            m_params.startupErrorMessage = QString{"Invalid option: -"} + Platform::toQString(optionString);
            return false;
        }

        bool handleLongOption(OsStringView option, CommandLineParser&) override
        {
            m_params.startupErrorMessage = QString{"Invalid option: --"} + Platform::toQString(option);
            return false;
        }

    private:
        ApplicationParams& m_params;
    };

} // namespace

ApplicationParams ApplicationParams::parse(int argc, const oschar_t* const* argv)
{
    ApplicationParams params = {};
    AppCommandLineHandler handler{params};
    CommandLineParser::parse(argc, argv, handler);
    return params;
}

//--------------------------------------------------------------------------------------------------

bool CommandLineParser::parse(int argc, const oschar_t* const* argv, CommandLineHandler& handler)
{
    CommandLineParser parser{argc, argv};

    while (!parser.isFinished()) {
        if (!parser.parseNext(handler)) {
            return false;
        }
    }

    return true;
}

const oschar_t* CommandLineParser::getParam()
{
    if (isFinished()) {
        return nullptr;
    } else if (m_param) {
        return *m_param;
    }

    switch (m_state) {
    case State::ShortOption:
        if (m_args[m_argIndex][m_charIndex]) {
            // The rest of the short option chain is interpreted as the parameter, i.e. '-Oparam'.
            m_param = &m_args[m_argIndex][m_charIndex];
            m_charIndex = StringUtils::getCStringLength(*m_param);
        } else if (m_argIndex + 1 < m_argCount) {
            // We've already reached the end of the short option chain, so the next arg is
            // interpreted as the parameter, e.g. '-O param'.
            m_param = m_args[++m_argIndex];
            m_charIndex = StringUtils::getCStringLength(*m_param);
        } else {
            // No parameter is available.
            m_param = nullptr;
        }
        return *m_param;

    case State::LongOption:
        if (m_argIndex + 1 < m_argCount) {
            // If the option has the syntax '--option=param', the parameter would already have been
            // extracted by parseNext(). If we made it this far, the next arg is interpreted as the
            // parameter, i.e. '--option param'.
            m_param = m_args[++m_argIndex];
            m_charIndex = StringUtils::getCStringLength(*m_param);
        } else {
            // No parameter is available.
            m_param = nullptr;
        }
        return *m_param;

    default:
        return nullptr;
    }
}

bool CommandLineParser::parseNext(CommandLineHandler& handler)
{
    OsStringView operand;
    oschar_t shortOption;
    OsStringView longOption;
    const oschar_t* longOptionStart;
    const oschar_t* longOptionEnd;
    bool result;

    if (isFinished()) {
        // Should be unreachable.
        return true;
    } else if (m_charIndex) {
        // Get the next option in a short option chain.
        shortOption = m_args[m_argIndex][m_charIndex++];
        m_state = State::ShortOption;
        m_param = {};
        result = handler.handleShortOption(shortOption, *this);
    } else if (m_state == State::OperandsOnly || m_args[m_argIndex][0] != '-') {
        // Arg is an operand.
        operand = m_args[m_argIndex];
        m_charIndex = operand.length();
        result = handler.handleOperand(operand);
    } else if (m_args[m_argIndex][1] == '-') {
        if (m_args[m_argIndex][2]) {
            // Arg is a long option.
            longOptionStart = &m_args[m_argIndex][2];
            longOptionEnd = StringUtils::findInCString(longOptionStart, oschar_t{'='});

            if (longOptionEnd) {
                // Option has the syntax '--option=param'. We can extract the parameter now.
                longOption = {longOptionStart, size_t(longOptionEnd - longOptionStart)};
                m_param = longOptionEnd + 1;
            } else {
                // Option has the syntax '--option'. A parameter may still be extracted later with
                // getParam().
                longOption = longOptionStart;
                m_param = {};
            }

            m_charIndex = 2 + StringUtils::getCStringLength(longOptionStart);
            m_state = State::LongOption;
            result = handler.handleLongOption(longOption, *this);
        } else {
            // Arg is '--'; all args that follow are operands, even if they start with '-'.
            ++m_argIndex;
            m_state = State::OperandsOnly;
            return true;
        }
    } else {
        // Start of a short option chain, i.e. '-a' or '-abc'. If the entire arg is simply '-', it
        // will be interpreted as a short option with option='\0'.
        shortOption = m_args[m_argIndex][++m_charIndex];
        if (shortOption) {
            ++m_charIndex;
        }
        m_state = State::ShortOption;
        m_param = {};
        result = handler.handleShortOption(shortOption, *this);
    }

    // If we've reached the end of an arg, advance to the next one.
    if (!isFinished() && !m_args[m_argIndex][m_charIndex]) {
        ++m_argIndex;
        m_charIndex = 0;
    }

    return result;
}

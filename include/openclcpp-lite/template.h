// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/exception.h"
#include <string>
#include <map>
#include <sstream>

namespace openclcpp_lite {

/// Class for simple templating
///
class Template {
public:
    /// Template parameters
    class Params {
    public:
        Params();

        /// Set template parameter
        ///
        /// @tparam T C++ type
        /// @param name Parameter name
        /// @param value Parameter value
        template <typename T>
        void
        set(const std::string & name, const T & value)
        {
            std::string val = std::to_string(value);
            this->subst.insert(std::make_pair(name, val));
        }

        /// Get parameter value
        ///
        /// @param name Parameter name
        /// @return Parameter value
        std::string
        get(const std::string & name) const
        {
            try {
                return this->subst.at(name);
            }
            catch (std::exception & e) {
                throw Exception("No variable named '{}'", name);
            }
        }

    private:
        std::map<std::string, std::string> subst;
    };

private:
    struct Token {
        enum Type { OpenVar, CloseVar, Character, Variable, End };
        Type type;
        std::string val;
    };

    /// Lexer for reading tokens
    class Lexer {
    public:
        /// Mode
        enum Mode {
            /// Reading normal text
            Text,
            /// Reading variable name
            Var
        };

        /// Construct lexer
        ///
        /// @param source template source
        Lexer(const std::string & source);

        /// Look at the next token
        Token peek();

        /// Read the next token
        Token read();

        /// Set mode
        void set_mode(Mode mode);

    private:
        Token read_token();
        char read_char();
        int peek_char();

        std::istringstream sstr;
        Mode mode;
        /// Flag indicating if token is cached
        bool have_token;
        /// Cached token
        Token curr;
    };

    /// Create a template
    ///
    /// @param templ Template source
    Template(const std::string & templ);

    /// Substitute template variables
    ///
    /// @param params Variable names with their substitutions
    /// @return String with substituted values
    std::string subst(const Params & params);

    /// Lexer for reading tokens
    Lexer lexer;

public:
    /// Use template and substitute variables
    ///
    /// @param tpl Template source
    /// @param params Substitution parameters
    /// @return String with substituted variables
    static std::string build(const std::string & tpl, const Params & params);
};

/// Set string parameter
///
/// @param name Parameter name
/// @param value Parameter value
template <>
void
Template::Params::set(const std::string & name, const std::string & value)
{
    this->subst.insert(std::make_pair(name, value));
}

/// Set string parameter
///
/// @param name Parameter name
/// @param value Parameter value
template <>
void
Template::Params::set(const std::string & name, char * const & value)
{
    this->subst.insert(std::make_pair(name, std::string(value)));
}

} // namespace openclcpp_lite

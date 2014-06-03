/*******************************************************************************

    Copyright (C) 2014 by Cole Bambrick
    cole.bambrick@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#ifndef FILTER_H
#define FILTER_H
#include <QList>
#include <QString>

///
/// \brief The Filter class provides simple pattern matching.
///
/// Patterns are parsed into tokens of the following types:
///
///   * Any block of non-whitespace characters seperated by whitespace.
///   * Any block of non-double-quote characters enclosed within double-quotes.
///   * Any of the above, prepended with a '-' symbol.
///
/// If a token begins with a '-' symbol, then the token, excluding the leading
/// '-', must not exist in given string for that string to match the filter.
/// Otherwise, a given string must contain the token to match the filter. In
/// the case of quoted tokens, the quotes themselves are ignored when matching.
///
/// For example, for a string to match a filter of "red green -blue", the string
/// must contain the substrings "red" and "green" but must not contain the
/// substring "blue".
///
/// Similarly, for a string to match a filter "this "is a" filter", the string
/// must contain the substrings "this", "is a", and "filter".
///
/// \note Escaping double-quotes is not supported at this time.
///
class Filter
{
public:

    ///
    /// \brief Constructs an empty filter. Any text will successfully match
    /// against an empty filter.
    ///
    Filter();

    ///
    /// \brief Constructs a filter from the given pattern.
    /// \param pattern The pattern to generate tokens from.
    ///
    Filter(const QString &pattern);

    ///
    /// \brief Determines whether or not a string passes the filter.
    /// \param text The string to check the filter against.
    /// \return True if the string matches the filter; otherwise, false.
    ///
    bool match(const QString &text) const;

private:

    ///
    /// \brief The Token struct represents a token to match against in a filter.
    ///
    /// A string must satisfy every token in the filter to be considered a
    /// match.
    ///
    struct Token {

        ///
        /// \brief The TokenType enum represents different behaviours for tokens.
        ///
        enum TokenType {
            Negative    ///< The token must not exist in the text to match.
            ,Positive   ///< The token must exist in the text to match.
        };

        /// The type of the token.
        TokenType type;

        /// The textual value of the token.
        QString value;
    };

    /// The list of tokens generated from the pattern.
    QList<Token> _tokens;
};

#endif // FILTER_H

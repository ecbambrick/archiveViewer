/*******************************************************************************

    Copyright (C) 2013 by Cole Bambrick
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
#include <QString>

///
/// \brief The Filter class provides simple pattern matching.
///
/// Patterns are parsed into tokens of the following type:
///
///   * Any block of non-white space characters seperated by whitespace.
///   * Any block of characters enclosed within double-quotes.
///   * Any of the above, prepended with a '-' symbol.
///
/// If a token begins with a '-' symbol, then the token must not exist in a
/// string for that string to match the filter. Otherwise, the string must
/// contain the token to match the filter.
///
/// For example, for a string to match a filter of "red green -blue", the string
/// must contain the substrings "red" and "green" but must not contain the
/// substring "blue".
///
/// \note Escaping double-quotes is not supported at this time.
///
class Filter
{
public:

    ///
    /// \brief Constructor.
    /// \param pattern The query string to generate tokens from.
    ///
    explicit Filter(const QString &pattern);

    ///
    /// \brief Determines whether or not a string passes the filter.
    /// \param text The string to check the filter against.
    /// \return True if the string matches the filter, and false otherwise.
    ///
    bool match(const QString &text) const;

private:

    ///
    /// \brief The TokenType enum represents different behaviours for tokens.
    ///
    enum TokenType {
        Negative       ///< The token must not exist in the text to match.
        ,Positive        ///< The token must exist in the text to match.
    };

    ///
    /// \brief The Token struct represents a token to match against.
    ///
    /// A string must satisfy every token in the filter to be considered a
    /// match.
    ///
    struct Token {
        TokenType type; ///< The type of the token.
        QString value;  ///< The textual value of the token.
    };

    /// The list of tokens generated from the pattern.
    QList<Token> _tokens;
};

#endif // FILTER_H
